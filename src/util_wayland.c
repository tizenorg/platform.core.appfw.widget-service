#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h> /* access */
#include <stdlib.h> /* free */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dlog.h>

#include <sqlite3.h>
#include <unicode/uloc.h>

#include <wayland-client.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include "widget_errno.h"
#include "util.h"
#include "widget_service.h"
#include "widget_service_internal.h"
#include "debug.h"
#include "widget_wayland-drm-client-protocol.h"
#include "widget_wayland-drm-server-protocol.h"

#define CONF_PATH_FORMAT "/usr/share/data-provider-master/%dx%d/resolution.ini"

int errno;

static struct {
	unsigned int w;
	unsigned int h;
	int res_resolved;
} s_info = {
	.w = 0,
	.h = 0,
	.res_resolved = 0,
};

static int update_info(struct supported_size_list *SIZE_LIST, int width_type, int height_type, int width, int height)
{
	int idx;

	if (width_type == 1 && height_type == 1) {
		idx = 0;
	} else if (width_type == 2 && height_type == 1) {
		idx = 1;
	} else if (width_type == 2 && height_type == 2) {
		idx = 2;
	} else if (width_type == 4 && height_type == 1) {
		idx = 3;
	} else if (width_type == 4 && height_type == 2) {
		idx = 4;
	} else if (width_type == 4 && height_type == 3) {
		idx = 5;
	} else if (width_type == 4 && height_type == 4) {
		idx = 6;
	} else if (width_type == 4 && height_type == 5) {
		idx = 7;
	} else if (width_type == 4 && height_type == 6) {
		idx = 8;
	} else if (width_type == 21 && height_type == 21) {
		idx = 9;
	} else if (width_type == 23 && height_type == 21) {
		idx = 10;
	} else if (width_type == 23 && height_type == 23) {
		idx = 11;
	} else if (width_type == 0 && height_type == 0) {
		idx = 12;
	} else {
		ErrPrint("Unknown size type: %dx%d (%dx%d)\n", width_type, height_type, width, height);
		return 0;
	}

	SIZE_LIST[idx].w = width;
	SIZE_LIST[idx].h = height;
	return 1;
}

static inline int update_from_file(struct service_info *info, struct supported_size_list *SIZE_LIST)
{
	FILE *fp;
	int updated;
	int width_type;
	int height_type;
	int width;
	int height;
	char buffer[MAX_COLUMN];
	int ch;
	int idx;
	enum status {
		START = 0x0,
		TYPE = 0x01,
		SIZE = 0x02,
		COMMENT = 0x03,
		ERROR = 0x04,
		EOL = 0x05,
		TYPE_END = 0x06,
		SIZE_START = 0x07
	} status;

	fp = fopen(info->conf_file, "r");
	if (!fp) {
		ErrPrint("Open failed: %d\n", errno);
		return WIDGET_ERROR_IO_ERROR;
	}

	updated = 0;
	status = START;
	idx = 0;
	do {
		ch = fgetc(fp);

		if (idx == MAX_COLUMN) {
			ErrPrint("Buffer overflow. Too long line. LINE MUST BE SHOT THAN %d\n", MAX_COLUMN);
			status = ERROR;
		}

		switch (status) {
		case START:
			if (isspace(ch) || ch == EOF) {
				continue;
			}

			info->base_parse = 0;

			if (ch == '#') {
				status = COMMENT;
			} else {
				status = TYPE;
				idx = 0;
				ungetc(ch, fp);
			}
			break;
		case TYPE:
			if (isblank(ch)) {
				buffer[idx] = '\0';
				status = TYPE_END;
				if (sscanf(buffer, "%dx%d", &width_type, &height_type) != 2) {
					if (!strcasecmp(buffer, "base")) {
						info->base_parse = 1;
					} else {
						ErrPrint("Invalid syntax: [%s]\n", buffer);
						status = ERROR;
					}
				}
				break;
			} else if (ch == '=') {
				buffer[idx] = '\0';
				status = SIZE_START;
				if (sscanf(buffer, "%dx%d", &width_type, &height_type) != 2) {
					if (!strcasecmp(buffer, "base")) {
						info->base_parse = 1;
					} else {
						ErrPrint("Invalid syntax: [%s]\n", buffer);
						status = ERROR;
					}
				}
				break;
			} else if (ch == EOF) {
				ErrPrint("Invalid Syntax\n");
				status = ERROR;
				continue;
			}
			buffer[idx++] = ch;
			break;
		case TYPE_END:
			if (ch == '=') {
				status = SIZE_START;
			}
			break;
		case SIZE_START:
			if (isspace(ch) || ch == EOF) {
				continue;
			}

			status = SIZE;
			idx = 0;
			ungetc(ch, fp);
			break;
		case SIZE:
			if (isspace(ch) || ch == EOF) {
				buffer[idx] = '\0';
				status = EOL;

				if (sscanf(buffer, "%dx%d", &width, &height) != 2) {
					ErrPrint("Invalid syntax: [%s]\n", buffer);
					status = ERROR;
				} else if (ch == EOF) {
					if (info->base_parse) {
						info->base_w = width;
						info->base_h = height;
					} else {
						updated += update_info(SIZE_LIST, width_type, height_type, width, height);
					}
				}
				break;
			}
			buffer[idx++] = ch;
			break;
		case EOL:
			if (!info->base_parse) {
				updated += update_info(SIZE_LIST, width_type, height_type, width, height);
			} else {
				info->base_w = width;
				info->base_h =  height;
			}
			status = START;
			ungetc(ch, fp);
			break;
		case ERROR:
			if (ch == '\n' || ch == '\r' || ch == '\f') {
				status = START;
			}
			break;
		case COMMENT:
			if (ch == '\n' || ch == '\r' || ch == '\f') {
				status = START;
			}
			break;
		default:
			ErrPrint("Unknown status. couldn't be reach to here\n");
			break;
		}
	} while (!feof(fp));

	if (fclose(fp) != 0) {
		ErrPrint("fclose: %d\n", errno);
	}

	return WIDGET_NR_OF_SIZE_LIST - updated;
}

/*
 * Find proper configuration and install(link) it to conf path.
 */
static char *conf_path(void)
{
	char *path;
	int length;

	length = strlen(CONF_PATH_FORMAT) + 12;    // 12 == RESERVED SPACE
	path = calloc(1, length);
	if (!path) {
		ErrPrint("calloc: %d\n", errno);
		return NULL;
	}

	if (s_info.w == 0 || s_info.h == 0) {
		/* Try to update resolution first if it is not initialized */
		util_screen_size_get(NULL, NULL);
	}

	snprintf(path, length, CONF_PATH_FORMAT, s_info.w, s_info.h);
	DbgPrint("Selected conf file: %s\n", path);
	if (access(path, F_OK) != 0) {
		ErrPrint("Fallback to default, access: %d\n", errno);
		strncpy(path, RESOLUTION_FILE, length);
		if (access(path, F_OK) != 0) {
			ErrPrint("Serious error - there is no conf file, use default setting: %d\n", errno);
			free(path);
			path = NULL;
		}
	}

	return path;
}

int util_screen_size_get(unsigned int *width, unsigned int *height)
{
	return WIDGET_ERROR_NONE;
}

int util_update_resolution(struct service_info *info, struct supported_size_list *SIZE_LIST)
{
	if (s_info.res_resolved) {
		return WIDGET_ERROR_NONE;
	}

	if (!info->conf_file) {
		info->conf_file = conf_path();
	}

	if (info->conf_file) {
		register int i;
		unsigned int width = 0;
		unsigned int height = 0;

		i = util_screen_size_get(&width, &height);
		if (i != WIDGET_ERROR_NONE) {
			return i;
		}

		if (update_from_file(info, SIZE_LIST) == 0) {
			DbgPrint("Resolution info is all updated by file\n");
		}

		if (width != info->base_w) {
			for (i = 0; i < WIDGET_NR_OF_SIZE_LIST; i++) {
				SIZE_LIST[i].w = (unsigned int)((double)SIZE_LIST[i].w * (double)width / (double)info->base_w);
				SIZE_LIST[i].h = (unsigned int)((double)SIZE_LIST[i].h * (double)width / (double)info->base_w);
			}
		}
	} else {
		DbgPrint("Conf file is not loaded\n");
	}

	s_info.res_resolved = 1;
	return WIDGET_ERROR_NONE;
}

struct wl_drm_info {
	struct wl_drm *wl_drm;
	int authenticated;
	int fd;
};

static void wl_client_drm_handle_device(void *data, struct wl_drm *drm, const char *device)
{
	struct wl_drm_info *drm_info = (struct wl_drm_info *)data;
	drm_magic_t magic;

	DbgPrint("device[%s]\n", device);
	drm_info->fd = open(device, O_RDWR | O_CLOEXEC);
	if (drm_info->fd < 0) {
		ErrPrint("Failed to open a device: %d (%s)\n", errno, device);
		return;
	}

	drmGetMagic(drm_info->fd, &magic);
	DbgPrint("magic[%x]\n", magic);
	wl_drm_authenticate(drm_info->wl_drm, magic);
}

static void wl_client_drm_handle_format(void *data, struct wl_drm *drm, uint32_t format)
{
	/* Do nothing */
	DbgPrint("");
}

static void wl_client_drm_handle_authenticated(void *data, struct wl_drm *drm)
{
	struct wl_drm_info *drm_info = (struct wl_drm_info *)data;
	drm_info->authenticated = 1;
	DbgPrint("");
}

static void wl_client_drm_handle_capabilities(void *data, struct wl_drm *drm, uint32_t value)
{
	/* Do nothing */
	DbgPrint("");
}

static void wl_client_registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
	struct wl_drm_info *info = (struct wl_drm_info *)data;
	static const struct wl_drm_listener wl_drm_client_listener = {
		wl_client_drm_handle_device,
		wl_client_drm_handle_format,
		wl_client_drm_handle_authenticated,
		wl_client_drm_handle_capabilities
	};

	DbgPrint("interface[%s]\n", interface);
	if (!strcmp(interface, "wl_drm")) {
		info->wl_drm = wl_registry_bind(registry, name, &wl_drm_interface, (version > 2) ? 2 : version);
		wl_drm_add_listener(info->wl_drm, &wl_drm_client_listener, data);
	}
}

EAPI int widget_util_get_drm_fd(void *dpy, int *fd)
{
	struct wl_display *disp = NULL;
	struct wl_event_queue *wl_queue;
	struct wl_registry *wl_registry;
	int ret = 0;
	struct wl_drm_info info = {
		.wl_drm = NULL,
		.authenticated = 0,
		.fd = -1,
	};
	static const struct wl_registry_listener registry_listener = {
		wl_client_registry_handle_global,
		NULL
	};

	if (!fd) {
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	if (!dpy) {
		disp = wl_display_connect(NULL);
		if (!disp) {
			ErrPrint("Failed to create a new display connection\n");
			return WIDGET_ERROR_FAULT;
		}
		dpy = disp;
	}

	wl_queue = wl_display_create_queue(dpy);
	if (!wl_queue) {
		ErrPrint("Failed to create a WL Queue\n");
		if (disp == dpy) {
			wl_display_disconnect(disp);
		}
		return WIDGET_ERROR_FAULT;
	}

	wl_registry = wl_display_get_registry(dpy);
	if (!wl_registry) {
		ErrPrint("Failed to get registry\n");
		wl_event_queue_destroy(wl_queue);
		if (disp == dpy) {
			wl_display_disconnect(disp);
		}
		return WIDGET_ERROR_FAULT;
	}

	wl_proxy_set_queue((struct wl_proxy *)wl_registry, wl_queue);

	wl_registry_add_listener(dpy, &registry_listener, &info);
	DbgPrint("Consuming Dispatch Queue begin\n");
	while (ret != -1 && !info.authenticated) {
		ret = wl_display_dispatch_queue(dpy, wl_queue);
		DbgPrint("Dispatch Queue consumed: %d\n", ret);
	}
	DbgPrint("Consuming Dispatch Queue end\n");

	wl_event_queue_destroy(wl_queue);
	wl_registry_destroy(wl_registry);
	wl_drm_destroy(info.wl_drm);

	*fd = info.fd;
	if (disp == dpy) {
		wl_display_disconnect(disp);
	}
	return *fd >= 0 ? WIDGET_ERROR_NONE : WIDGET_ERROR_FAULT;
}

EAPI int widget_util_release_drm_fd(int fd)
{
	if (fd < 0) {
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	if (close(fd) < 0) {
		ErrPrint("close: %d\n", errno);
		return WIDGET_ERROR_FAULT;
	}

	return WIDGET_ERROR_NONE;
}

/* End of a file */

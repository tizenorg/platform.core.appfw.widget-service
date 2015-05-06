/*
 * Copyright 2013  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.1 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strdup, strerror */

#include <sqlite3.h>
#include <unicode/uloc.h>

#include <com-core_packet.h>
#include <packet.h>
#include <dlog.h>

#include "dlist.h"
#include "widget_errno.h"
#include "debug.h"
#include "widget_service.h"
#include "widget_cmd_list.h"
#include "util.h"

int errno;

struct lifecycle_monitor_item {
	char *widget_id;
	widget_lifecycle_event_cb cb;
	void *data;
};

static struct info {
	struct dlist *lifecycle_monitor_list;
	int lifecycle_monitor_handle;
} s_info = {
	.lifecycle_monitor_list = NULL,
	.lifecycle_monitor_handle = -1,
};

static inline void send_monitor_command(unsigned int cmd, const char *widget_id)
{
	struct packet *packet;

	if (s_info.lifecycle_monitor_handle < 0) {
		ErrPrint("Not yet initialized\n");
		return;
	}

	packet = packet_create_noack((const char *)&cmd, "s", widget_id ? widget_id : "*");
	if (!packet) {
		ErrPrint("Failed to create a packet\n");
	} else {
		if (com_core_packet_send_only(s_info.lifecycle_monitor_handle, packet) < 0) {
			ErrPrint("Failed to send packet\n");
		}
		packet_destroy(packet);
	}
}

static inline void invoke_handler(widget_lifecycle_event_e event, const char *widget_id, const char *instance_id)
{
	struct lifecycle_monitor_item *item;
	struct dlist *l;

	DbgPrint("%s %s\n", widget_id, instance_id);
	dlist_foreach(s_info.lifecycle_monitor_list, l, item) {
		if (item->widget_id == NULL) {
			item->cb(widget_id, event, instance_id, item->data);
		} else if (!strcmp(item->widget_id, widget_id)) {
			item->cb(widget_id, event, instance_id, item->data);
		}
	}
}

static struct packet *monitor_create_handler(pid_t pid, int handle, const struct packet *packet)
{
	const char *widget_id;
	const char *instance_id;
	const char *content_info;
	double timestamp;

	if (packet_get(packet, "dsss", &timestamp, &widget_id, &instance_id, &content_info) != 4) {
		ErrPrint("Invalid packet\n");
	} else {
		invoke_handler(WIDGET_LIFE_CYCLE_EVENT_CREATE, widget_id, instance_id);
	}

	return NULL;
}

static struct packet *monitor_destroy_handler(pid_t pid, int handle, const struct packet *packet)
{
	const char *widget_id;
	const char *instance_id;
	const char *content_info;
	double timestamp;

	if (packet_get(packet, "dsss", &timestamp, &widget_id, &instance_id, &content_info) != 4) {
		ErrPrint("Invalid packet\n");
	} else {
		invoke_handler(WIDGET_LIFE_CYCLE_EVENT_DESTROY, widget_id, instance_id);
	}

	return NULL;
}

static struct packet *monitor_pause_handler(pid_t pid, int handle, const struct packet *packet)
{
	const char *widget_id;
	const char *instance_id;
	const char *content_info;
	double timestamp;

	if (packet_get(packet, "dsss", &timestamp, &widget_id, &instance_id, &content_info) != 4) {
		ErrPrint("Invalid packet\n");
	} else {
		invoke_handler(WIDGET_LIFE_CYCLE_EVENT_PAUSE, widget_id, instance_id);
	}

	return NULL;
}

static struct packet *monitor_resume_handler(pid_t pid, int handle, const struct packet *packet)
{
	const char *widget_id;
	const char *instance_id;
	const char *content_info;
	double timestamp;

	if (packet_get(packet, "dsss", &timestamp, &widget_id, &instance_id, &content_info) != 4) {
		ErrPrint("Invalid packet\n");
	} else {
		invoke_handler(WIDGET_LIFE_CYCLE_EVENT_RESUME, widget_id, instance_id);
	}

	return NULL;
}

EAPI int widget_service_set_lifecycle_event_cb(const char *widget_id, widget_lifecycle_event_cb cb, void *data)
{
	struct lifecycle_monitor_item *item;
	struct dlist *l;

	if (!cb) {
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	if (!widget_id) {
		DbgPrint("Register monitor for all widgets\n");
	}

	dlist_foreach(s_info.lifecycle_monitor_list, l, item) {
		if (item->widget_id == NULL && widget_id == NULL) {
			return WIDGET_ERROR_ALREADY_EXIST;
		} else if (item->widget_id && widget_id) {
			if (!strcmp(widget_id, item->widget_id)) {
				return WIDGET_ERROR_ALREADY_EXIST;
			}
		}
	}

	item = calloc(1, sizeof(*item));
	if (!item) {
		ErrPrint("calloc: %d\n", errno);
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	if (widget_id) {
		item->widget_id = strdup(widget_id);
		if (!item->widget_id) {
			ErrPrint("strdup: %d\n", errno);
			free(item);
			return WIDGET_ERROR_OUT_OF_MEMORY;
		}
	} else {
		item->widget_id = NULL;
	}

	item->cb = cb;
	item->data = data;

	s_info.lifecycle_monitor_list = dlist_append(s_info.lifecycle_monitor_list, item);

	if (s_info.lifecycle_monitor_handle < 0) {
		static struct method table[] = {
			{
				.cmd = CMD_STR_MONITOR_CREATE,
				.handler = monitor_create_handler,
			},
			{
				.cmd = CMD_STR_MONITOR_DESTROY,
				.handler = monitor_destroy_handler,
			},
			{
				.cmd = CMD_STR_MONITOR_PAUSE,
				.handler = monitor_pause_handler,
			},
			{
				.cmd = CMD_STR_MONITOR_RESUME,
				.handler = monitor_resume_handler,
			},
			{
				.cmd = NULL,
				.handler = NULL,
			},
		};

		/**
		 * @todo
		 * Send service_register command to master
		 */
		s_info.lifecycle_monitor_handle = com_core_packet_client_init(SERVICE_SOCKET, 0, table);
		DbgPrint("Monitor handle: %d\n", s_info.lifecycle_monitor_handle);
	}

	send_monitor_command(CMD_MONITOR_REGISTER, widget_id);

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_unset_lifecycle_event_cb(const char *widget_id, void **user_data)
{
	struct dlist *l;
	struct dlist *n;
	struct lifecycle_monitor_item *item;
	int ret = WIDGET_ERROR_NOT_EXIST

	dlist_foreach_safe(s_info.lifecycle_monitor_list, l, n, item) {
		if (widget_id == NULL && item->widget_id == NULL) {
			s_info.lifecycle_monitor_list = dlist_remove(s_info.lifecycle_monitor_list, l);
			if (user_data) {
				*user_data = item->data;
			}
			free(item);

			/**
			 * @todo
			 * Send service_unregister command to master
			 */
			send_monitor_command(CMD_MONITOR_UNREGISTER, widget_id);

			ret = WIDGET_ERROR_NONE;
			break;
		} else if (widget_id && item->widget_id) {
			if (!strcmp(item->widget_id, widget_id)) {
				s_info.lifecycle_monitor_list = dlist_remove(s_info.lifecycle_monitor_list, l);
				if (user_data) {
					*user_data = item->data;
				}
				free(item->widget_id);
				free(item);

				/**
				 * @todo
				 * Send service_unregister command to master
				 */
				send_monitor_command(CMD_MONITOR_UNREGISTER, widget_id);
				ret = WIDGET_ERROR_NONE;
				break;
			}
		}
	}

	if (s_info.lifecycle_monitor_handle >= 0 && !s_info.lifecycle_monitor_list) {
		com_core_packet_client_fini(s_info.lifecycle_monitor_handle);
		s_info.lifecycle_monitor_handle = -1;
	}

	return ret;
}

EAPI int widget_service_get_content_of_widget_instance(const char *widget_id, const char *widget_instance_id, bundle **b)
{
	struct packet *packet;
	struct packet *result;
	unsigned int cmd = CMD_SERVICE_GET_CONTENT;
	char *uri;
	const char *_content;
	int ret;

	if (!widget_instance_id || !b) {
		ErrPrint("Invalid argument\n");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	uri = util_id_to_uri(widget_instance_id);
	if (!uri) {
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	packet = packet_create((const char *)&cmd, "ss", widget_id, uri);
	free(uri);
	if (!packet) {
		ErrPrint("Failed to create a packet for period changing\n");
		return WIDGET_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "is", &ret, &_content) != 2) {
			ErrPrint("Failed to parse a result packet\n");
			ret = WIDGET_ERROR_INVALID_PARAMETER;
		} else if (ret == WIDGET_ERROR_NONE) {
			*b = bundle_decode((bundle_raw *)_content, strlen(_content));
			if (!*b) {
				ErrPrint("bundle_decode: %s\n", _content);
			}
		} else {
			ErrPrint("ret: %d\n", ret);
		}
		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = WIDGET_ERROR_FAULT;
	}

	return ret;
}

EAPI int widget_service_get_widget_instance_list(const char *widget_id, widget_instance_list_cb cb, void *data)
{
	struct packet *packet;
	struct packet *result;
	unsigned int cmd = CMD_SERVICE_GET_INST_LIST;
	const char *list;
	int ret;

	if (!widget_id || !cb) {
		ErrPrint("Invalid argument\n");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	packet = packet_create((const char *)&cmd, "s", widget_id);
	if (!packet) {
		ErrPrint("Failed to create a packet for period changing\n");
		return WIDGET_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		int cnt;
		if (packet_get(result, "iis", &ret, &cnt, &list) != 3) {
			ErrPrint("Failed to parse a result packet\n");
			ret = WIDGET_ERROR_INVALID_PARAMETER;
		} else if (ret == WIDGET_ERROR_NONE) {
			const char *ptr;
			register int i;
			/**
			 * Parse the list
			 */
			ptr = list;
			i = 0;
			do {
				switch (list[i]) {
				case '\0':
					if (ptr != list + i) {
						cb(widget_id, ptr, data);
					}
					break;
				case '\n':
					*((char *)list + i) = '\0';
					cb(widget_id, ptr, data);
					ptr = list + i + 1;
				default:
					i++;
					break;
				}
			} while (list[i]);

			if (i != cnt) {
				DbgPrint("Total count is not valid: %d <> %d\n", i, cnt);
			}
		}
		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = WIDGET_ERROR_FAULT;
	}

	return ret;
}

/* End of a file */

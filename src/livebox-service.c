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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <ctype.h>

#include <com-core_packet.h>
#include <packet.h>
#include <dlog.h>
#include <db-util.h>
#include <package-manager.h>
#include <pkgmgr-info.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <ail.h>

#include "dlist.h"
#include "util.h"
#include "debug.h"
#include "livebox-service.h"
#include "livebox-errno.h"

#define SAMSUNG_PREFIX	"com.samsung."
#define EAPI __attribute__((visibility("default")))
#define DEFAULT_TIMEOUT 2.0
#define MAX_COLUMN 80

static struct supported_size_list {
	int w;
	int h;
} SIZE_LIST[NR_OF_SIZE_LIST] = {
	{ 175, 175 }, /*!< 1x1 */
	{ 354, 175 }, /*!< 2x1 */
	{ 354, 354 }, /*!< 2x2 */
	{ 712, 175 }, /*!< 4x1 */
	{ 712, 354 }, /*!< 4x2 */
	{ 712, 533 }, /*!< 4x3 */
	{ 712, 712 }, /*!< 4x4 */
	{ 712, 891 }, /*!< 4x5 */
	{ 712, 1070 }, /*!< 4x6 */
	{ 207, 207 }, /*!< 21x21 */
	{ 645, 207 }, /*!< 23x21 */
	{ 645, 645 }, /*!< 23x23 */
	{ 720, 1280 }, /*!< 0x0 */
};

static struct info {
	sqlite3 *handle;
	const char *dbfile;
	const char *conf_file;
	int init_count;
	int res_resolved;
} s_info = {
	.handle = NULL,
	.dbfile = "/opt/dbspace/.livebox.db", 
	.conf_file = "/usr/share/data-provider-master/resolution.ini",
	.init_count = 0,
	.res_resolved = 0,
};

static inline int update_info(int width_type, int height_type, int width, int height)
{
	int idx;

	if (width_type == 1 && height_type == 1) {
		DbgPrint("1x1 Updated to %dx%d\n", width, height);
		idx = 0;
	} else if (width_type == 2 && height_type == 1) {
		DbgPrint("2x1 Updated to %dx%d\n", width, height);
		idx = 1;
	} else if (width_type == 2 && height_type == 2) {
		DbgPrint("2x2 Updated to %dx%d\n", width, height);
		idx = 2;
	} else if (width_type == 4 && height_type == 1) {
		DbgPrint("4x1 Updated to %dx%d\n", width, height);
		idx = 3;
	} else if (width_type == 4 && height_type == 2) {
		DbgPrint("4x2 Updated to %dx%d\n", width, height);
		idx = 4;
	} else if (width_type == 4 && height_type == 3) {
		DbgPrint("4x3 Updated to %dx%d\n", width, height);
		idx = 5;
	} else if (width_type == 4 && height_type == 4) {
		DbgPrint("4x4 Updated to %dx%d\n", width, height);
		idx = 6;
	} else if (width_type == 4 && height_type == 5) {
		DbgPrint("4x5 Updated to %dx%d\n", width, height);
		idx = 7;
	} else if (width_type == 4 && height_type == 6) {
		DbgPrint("4x6 Updated to %dx%d\n", width, height);
		idx = 8;
	} else if (width_type == 21 && height_type == 21) {
		DbgPrint("Easy 1x1 Updated to %dx%d\n", width, height);
		idx = 9;
	} else if (width_type == 23 && height_type == 21) {
		DbgPrint("Easy 3x1 Updated to %dx%d\n", width, height);
		idx = 10;
	} else if (width_type == 23 && height_type == 23) {
		DbgPrint("Easy 3x3 Updated to %dx%d\n", width, height);
		idx = 11;
	} else if (width_type == 0 && height_type == 0) {
		DbgPrint("Special 0x0 Updated to %dx%d\n", width, height);
		idx = 12;
	} else {
		ErrPrint("Unknown size type: %dx%d (%dx%d)\n", width_type, height_type, width, height);
		return 0;
	}

	SIZE_LIST[idx].w = width;
	SIZE_LIST[idx].h = height;
	return 1;
}

static inline int update_from_file(void)
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
		SIZE_START = 0x07,
	} status;

	fp = fopen(s_info.conf_file, "r");
	if (!fp) {
		ErrPrint("Open failed: %s\n", strerror(errno));
		return LB_STATUS_ERROR_IO;
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
			if (isspace(ch) || ch == EOF)
				continue;

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
					ErrPrint("Invalid syntax: [%s]\n", buffer);
					status = ERROR;
				}
				break;
			} else if (ch == '=') {
				buffer[idx] = '\0';
				status = SIZE_START;
				if (sscanf(buffer, "%dx%d", &width_type, &height_type) != 2) {
					ErrPrint("Invalid syntax: [%s]\n", buffer);
					status = ERROR;
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
			if (ch == '=')
				status = SIZE_START;
			break;
		case SIZE_START:
			if (isspace(ch) || ch == EOF)
				continue;

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
					updated += update_info(width_type, height_type, width, height);
				}
				break;
			}
			buffer[idx++] = ch;
			break;
		case EOL:
			updated += update_info(width_type, height_type, width, height);
			status = START;
			ungetc(ch, fp);
			break;
		case ERROR:
			if (ch == '\n' || ch == '\r' || ch == '\f')
				status = START;
			break;
		case COMMENT:
			if (ch == '\n' || ch == '\r' || ch == '\f')
				status = START;
			break;
		default:
			ErrPrint("Unknown status. couldn't be reach to here\n");
			break;
		}
	} while (!feof(fp));
	fclose(fp);

	return NR_OF_SIZE_LIST - updated;
}

static int update_resolution(void)
{
	Display *disp;
	Window root;
	Window dummy;
	int x, y;
	unsigned int width;
	unsigned int height;
	unsigned int border;
	unsigned int depth;
	register int i;

	if (s_info.res_resolved)
		return LB_STATUS_SUCCESS;

	disp = XOpenDisplay(NULL);
	if (!disp) {
		ErrPrint("Failed to open a display\n");
		return LB_STATUS_ERROR_FAULT;
	}

	root = XDefaultRootWindow(disp);
	if (!XGetGeometry(disp, root, &dummy, &x, &y, &width, &height, &border, &depth)) {
		XCloseDisplay(disp);
		return LB_STATUS_ERROR_FAULT;
	}

	if (update_from_file() == 0)
		DbgPrint("Resolution info is all updated by file\n");

	DbgPrint("Screen resolution: %dx%d\n", width, height);
	for (i = 0; i < NR_OF_SIZE_LIST; i++) {
		SIZE_LIST[i].w = (unsigned int)((double)SIZE_LIST[i].w * (double)width / 720.0f);
		SIZE_LIST[i].h = (unsigned int)((double)SIZE_LIST[i].h * (double)width / 720.0f);
		DbgPrint("(Ratio)Size is updated [%d] %dx%d\n", i, SIZE_LIST[i].w, SIZE_LIST[i].h);
	}

	XCloseDisplay(disp);
	s_info.res_resolved = 1;
	return LB_STATUS_SUCCESS;
}

static inline sqlite3 *open_db(void)
{
	sqlite3 *handle;

	if (!s_info.handle) {
		int ret;

		ret = db_util_open(s_info.dbfile, &handle, DB_UTIL_REGISTER_HOOK_METHOD);
		if (ret != SQLITE_OK) {
			ErrPrint("Failed to open a DB\n");
			return NULL;
		}
	} else {
		handle = s_info.handle;
	}

	return handle;
}

static inline void close_db(sqlite3 *handle)
{
	if (!s_info.handle)
		db_util_close(handle);
}

static inline int convert_size_from_type(enum livebox_size_type type, int *width, int *height)
{
	int idx;

	switch (type) {
	case LB_SIZE_TYPE_1x1: /*!< 175x175 */
		idx = 0;
		break;
	case LB_SIZE_TYPE_2x1: /*!< 354x175 */
		idx = 1;
		break;
	case LB_SIZE_TYPE_2x2: /*!< 354x354 */
		idx = 2;
		break;
	case LB_SIZE_TYPE_4x1: /*!< 712x175 */
		idx = 3;
		break;
	case LB_SIZE_TYPE_4x2: /*!< 712x354 */
		idx = 4;
		break;
	case LB_SIZE_TYPE_4x3: /*!< 712x533 */
		idx = 5;
		break;
	case LB_SIZE_TYPE_4x4: /*!< 712x712 */
		idx = 6;
		break;
	case LB_SIZE_TYPE_4x5: /*!< 712x891 */
		idx = 7;
		break;
	case LB_SIZE_TYPE_4x6: /*!< 712x1070 */
		idx = 8;
		break;
	case LB_SIZE_TYPE_EASY_1x1: /*< 207x207 */
		idx = 9;
		break;
	case LB_SIZE_TYPE_EASY_3x1: /*!< 645x207 */
		idx = 10;
		break;
	case LB_SIZE_TYPE_EASY_3x3: /*!< 645x645 */
		idx = 11;
		break;
	case LB_SIZE_TYPE_0x0: /*!< 720x1280 */
		idx = 12;
		break;
	default:
		return LB_STATUS_ERROR_INVALID;
	}

	if (update_resolution() < 0)
		ErrPrint("Failed to update resolution\n");

	*width = SIZE_LIST[idx].w;
	*height = SIZE_LIST[idx].h;
	return LB_STATUS_SUCCESS;
}

EAPI int livebox_service_change_period(const char *pkgname, const char *id, double period)
{
	struct packet *packet;
	struct packet *result;
	char *uri;
	int ret;

	if (!pkgname || !id || period < 0.0f) {
		ErrPrint("Invalid argument\n");
		return LB_STATUS_ERROR_INVALID;
	}

	uri = util_id_to_uri(id);
	if (!uri)
		return LB_STATUS_ERROR_MEMORY;

	packet = packet_create("service_change_period", "ssd", pkgname, uri, period);
	free(uri);
	if (!packet) {
		ErrPrint("Failed to create a packet for period changing\n");
		return LB_STATUS_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "i", &ret) != 1) {
			ErrPrint("Failed to parse a result packet\n");
			ret = LB_STATUS_ERROR_INVALID;
		}
		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = LB_STATUS_ERROR_FAULT;
	}

	return ret;
}

EAPI int livebox_service_trigger_update(const char *pkgname, const char *id, const char *cluster, const char *category, int force)
{
	struct packet *packet;
	struct packet *result;
	char *uri;
	int ret;

	if (!pkgname) {
		ErrPrint("Invalid argument\n");
		return LB_STATUS_ERROR_INVALID;
	}

	if (!force && access("/tmp/.live.paused", R_OK) == 0) {
		DbgPrint("Provider is paused\n");
		return LB_STATUS_ERROR_CANCEL;
	}

	uri = util_id_to_uri(id);
	if (!uri)
		return LB_STATUS_ERROR_MEMORY;

	if (!cluster)
		cluster = "user,created";

	if (!category)
		category = "default";

	packet = packet_create("service_update", "ssss", pkgname, uri, cluster, category);
	free(uri);
	if (!packet) {
		ErrPrint("Failed to create a packet for service_update\n");
		return LB_STATUS_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "i", &ret) != 1) {
			ErrPrint("Failed to parse a result packet\n");
			ret = LB_STATUS_ERROR_INVALID;
		}

		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = LB_STATUS_ERROR_FAULT;
	}

	return ret;
}

EAPI int livebox_service_get_pkglist(int (*cb)(const char *appid, const char *pkgname, int is_prime, void *data), void *data)
{
	int ret;
	sqlite3_stmt *stmt;
	char *appid;
	char *pkgid;
	int is_prime;
	sqlite3 *handle;

	if (!cb)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	ret = sqlite3_prepare_v2(handle, "SELECT appid, pkgid, prime FROM pkgmap", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		appid = (char *)sqlite3_column_text(stmt, 0);
		if (!appid || !strlen(appid)) {
			ErrPrint("APPID is not valid\n");
			continue;
		}

		pkgid = (char *)sqlite3_column_text(stmt, 1);
		if (!pkgid || !strlen(pkgid)) {
			ErrPrint("pkgid is not valid\n");
			continue;
		}

		is_prime = sqlite3_column_int(stmt, 2);

		ret++;

		if (cb(appid, pkgid, is_prime, data) < 0) {
			DbgPrint("Callback stopped package crawling\n");
			break;
		}
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

out:
	close_db(handle);
	return ret;
}

EAPI int livebox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *lbid, int is_prime, void *data), void *data)
{
	int ret;
	sqlite3_stmt *stmt;
	const char *lbid;
	int is_prime;
	sqlite3 *handle;

	if (!cb)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid, prime FROM pkgmap WHERE appid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		lbid = (const char *)sqlite3_column_text(stmt, 0);
		if (!lbid || !strlen(lbid)) {
			ErrPrint("LBID is not valid\n");
			continue;
		}

		is_prime = sqlite3_column_int(stmt, 1);

		ret++;

		if (cb(lbid, is_prime, data) < 0) {
			DbgPrint("Callback stopped package crawling\n");
			break;
		}
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

out:
	close_db(handle);
	return ret;
}

struct pkgmgr_cbdata {
	const char *lbid;
	void (*cb)(const char *lbid, const char *appid, void *data);
	void *cbdata;
};

static int pkgmgr_cb(const pkgmgrinfo_appinfo_h handle, void *user_data)
{
	struct pkgmgr_cbdata *cbdata = (struct pkgmgr_cbdata *)user_data;
	char *appid;
	int ret;

	ret = pkgmgrinfo_appinfo_get_appid(handle, &appid);
	if (ret < 0)
		ErrPrint("Unable to get appid\n");
	else
		cbdata->cb(cbdata->lbid, appid, cbdata->cbdata);

	return 0;
}

static inline int pkgmgr_get_applist(const char *pkgid, const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data)
{
	struct pkgmgr_cbdata cbdata;
	pkgmgrinfo_pkginfo_h handle;
	int ret;

	ret = pkgmgrinfo_pkginfo_get_pkginfo(pkgid, &handle);
	if (ret < 0) {
		ErrPrint("Unable to get pkginfo: %s\n", pkgid);
		return ret;
	}

	cbdata.lbid = lbid;
	cbdata.cb = cb;
	cbdata.cbdata = data;

	ret = pkgmgrinfo_appinfo_get_list(handle, PM_UI_APP, pkgmgr_cb, &cbdata);
	if (ret < 0)
		ErrPrint("Failed to get applist\n");

	pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
	return ret;
}

EAPI int livebox_service_get_applist(const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data)
{
	sqlite3_stmt *stmt;
	const char *tmp;
	char *pkgid;
	sqlite3 *handle;
	int ret;

	if (!lbid || !cb)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	ret = sqlite3_prepare_v2(handle, "SELECT appid FROM pkgmap WHERE (pkgid = ?) or (appid = ?)", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, lbid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, lbid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ret = LB_STATUS_ERROR_INVALID;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (const char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("Invalid package name (%s)\n", lbid);
		ret = LB_STATUS_ERROR_INVALID;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	pkgid = strdup(tmp);
	if (!pkgid) {
		ErrPrint("Error: %s\n", strerror(errno));
		ret = LB_STATUS_ERROR_MEMORY;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	ret = pkgmgr_get_applist(pkgid, lbid, cb, data);
	free(pkgid);

out:
	close_db(handle);
	return ret;
}

EAPI int livebox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int size;
	int ret;

	if (!types || !cnt || !pkgid)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	ret = sqlite3_prepare_v2(handle, "SELECT size_type FROM box_size WHERE pkgid = ? ORDER BY size_type ASC", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	if (*cnt > NR_OF_SIZE_LIST)
		*cnt = NR_OF_SIZE_LIST;

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW && ret < *cnt) {
		size = sqlite3_column_int(stmt, 0);
		types[ret] = size;
		ret++;
	}

	*cnt = ret;
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	ret = 0;
out:
	close_db(handle);
	return ret;
}

static inline char *cur_locale(void)
{
	char *language;
	language = vconf_get_str(VCONFKEY_LANGSET);
	if (language) {
		char *ptr;

		ptr = language;
		while (*ptr) {
			if (*ptr == '.') {
				*ptr = '\0';
				break;
			}

			if (*ptr == '_')
				*ptr = '-';

			ptr++;
		}
	} else {
		language = strdup("en-us");
		if (!language)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

	return language;
}

static inline char *get_default_name(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *name = NULL;
	int ret;

	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT name FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret ==  SQLITE_ROW) {
		const char *tmp;

		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (tmp && strlen(tmp)) {
			name = strdup(tmp);
			if (!name)
				ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return name;
}

static inline char *get_default_icon(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *icon = NULL;
	int ret;

	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT icon FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;

		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (tmp && strlen(tmp)) {
			icon = strdup(tmp);
			if (!icon)
				ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return icon;
}

EAPI char *livebox_service_content(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *content = NULL;
	int ret;

	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT content FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;

		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (tmp && strlen(tmp)) {
			content = strdup(tmp);
			if (!content)
				ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return content;
}

EAPI char *livebox_service_setup_appid(const char *lbid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *appid;

	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT setup FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	appid = NULL;
	ret = sqlite3_bind_text(stmt, 1, lbid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;

		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (!tmp || !strlen(tmp))
			goto out;

		appid = strdup(tmp);
		if (!appid)
			ErrPrint("Error: %s\n", strerror(errno));
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return appid;
}

EAPI int livebox_service_nodisplay(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;

	handle = open_db();
	if (!handle)
		return 0;

	ret = sqlite3_prepare_v2(handle, "SELECT nodisplay FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 0;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = 0;
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW)
		ret = !!sqlite3_column_int(stmt, 0);
	else
		ret = 0;

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

static inline char *get_lb_pkgname_by_appid(const char *appid)
{
	sqlite3_stmt *stmt;
	char *pkgid;
	char *tmp;
	sqlite3 *handle;
	int ret;

	if (!appid)
		return NULL;

	pkgid = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid FROM pkgmap WHERE (appid = ? AND prime = 1) OR pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, appid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, appid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ErrPrint("Error: %s (has no record? - %s)\n", sqlite3_errmsg(handle), appid);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (tmp && strlen(tmp)) {
		pkgid = strdup(tmp);
		if (!pkgid)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return pkgid;
}

EAPI int livebox_service_need_frame(const char *pkgid, int size_type)
{
	char *lbid;
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;

	handle = open_db();
	if (!handle) {
		ErrPrint("Unable to open a DB\n");
		return 0;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT need_frame FROM box_size WHERE pkgid = ? AND size_type = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 0;
	}

	/*!
	 */
	lbid = livebox_service_pkgname(pkgid);
	if (!lbid) {
		ErrPrint("Invalid appid (%s)\n", pkgid);
		ret = 0;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, lbid, -1, SQLITE_TRANSIENT);
	free(lbid);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = 0;
		goto out;
	}

	ret = sqlite3_bind_int(stmt, 2, size_type);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = 0;
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		ret = !!sqlite3_column_int(stmt, 0);
	} else {
		ret = 0;
		ErrPrint("There is no such result\n");
	}
out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

EAPI int livebox_service_touch_effect(const char *pkgid, int size_type)
{
	char *lbid;
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;

	handle = open_db();
	if (!handle) {
		ErrPrint("Unable to open a DB\n");
		return 1;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT touch_effect FROM box_size WHERE pkgid = ? AND size_type = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 1;
	}

	/*!
	 * \note
	 * This function will validate the "pkgid"
	 * call the exported API in the exported API is not recomended
	 * but... I used.
	 */
	lbid = livebox_service_pkgname(pkgid);
	if (!lbid) {
		ErrPrint("Invalid appid (%s)\n", pkgid);
		ret = 1;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, lbid, -1, SQLITE_TRANSIENT);
	free(lbid);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = 1;
		goto out;
	}

	ret = sqlite3_bind_int(stmt, 2, size_type);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = 1;
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		ret = !!sqlite3_column_int(stmt, 0);
	} else {
		ret = 1; /*!< Default true: In this case the DB is corrupted. */
		ErrPrint("There is no result\n");
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

EAPI int livebox_service_mouse_event(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *lbid;
	int ret;

	handle = open_db();
	if (!handle)
		return 0;

	ret = sqlite3_prepare_v2(handle, "SELECT mouse_event FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 0;
	}

	lbid = livebox_service_pkgname(pkgid);
	if (!lbid) {
		ErrPrint("Failed to get lbid: %s\n", pkgid);
		ret = 0;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, lbid, -1, SQLITE_TRANSIENT);
	free(lbid);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = 0;
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		ret = !!sqlite3_column_int(stmt, 0);
	} else {
		ret = 0; /*!< Default is false, In this case the DB is corrupted */
		ErrPrint("There is no result.\n");
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

EAPI char *livebox_service_preview(const char *pkgid, int size_type)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *preview = NULL;

	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT preview FROM box_size WHERE pkgid = ? AND size_type = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_int(stmt, 2, size_type);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;
		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (tmp && strlen(tmp)) {
			preview = strdup(tmp);
			if (!preview)
				ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return preview;
}

EAPI char *livebox_service_i18n_icon(const char *pkgid, const char *lang)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *language;
	char *icon = NULL;
	int ret;

	if (lang) {
		language = strdup(lang);
		if (!language) {
			ErrPrint("Heap: %s\n", strerror(errno));
			return NULL;
		}
	} else {
		language = cur_locale();
		if (!language)
			return NULL;
	}

	handle = open_db();
	if (!handle) {
		free(language);
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT icon FROM i18n WHERE pkgid = ? AND lang = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		free(language);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, language, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;
		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (!tmp || !strlen(tmp)) {
			icon = get_default_icon(pkgid);
		} else {
			icon = strdup(tmp);
			if (!icon)
				ErrPrint("Heap: %s\n", strerror(errno));
		}
	} else {
		icon = get_default_icon(pkgid);
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	free(language);
	return icon;
}

EAPI char *livebox_service_i18n_name(const char *pkgid, const char *lang)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *language;
	char *name = NULL;
	int ret;

	if (lang) {
		language = strdup(lang);
		if (!language) {
			ErrPrint("Error: %s\n", strerror(errno));
			return NULL;
		}
	} else {
		language = cur_locale();
		if (!language)
			return NULL;
	}

	handle = open_db();
	if (!handle) {
		free(language);
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT name FROM i18n WHERE pkgid = ? AND lang = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		free(language);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, language, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;
		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (!tmp || !strlen(tmp)) {
			name = get_default_name(pkgid);
		} else {
			name = strdup(tmp);
			if (!name)
				ErrPrint("Heap: %s\n", strerror(errno));
		}
	} else {
		name = get_default_name(pkgid);
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	free(language);
	return name;
}

EAPI int livebox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int size;
	int ret;

	if (!w || !h || !cnt || !pkgid)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	ret = sqlite3_prepare_v2(handle, "SELECT size_type FROM box_size WHERE pkgid = ? ORDER BY size_type ASC", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = LB_STATUS_ERROR_IO;
		goto out;
	}

	if (*cnt > NR_OF_SIZE_LIST)
		*cnt = NR_OF_SIZE_LIST;

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW && ret < *cnt) {
		size = sqlite3_column_int(stmt, 0);
		ret += (convert_size_from_type(size, w + ret, h + ret) == 0);
	}

	*cnt = ret;
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	ret = 0;
out:
	close_db(handle);
	return ret;
}

EAPI char *livebox_service_libexec(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *libexec;
	char *appid;
	char *path;

	if (!pkgid)
		return NULL;

	libexec = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.libexec FROM pkgmap, provider WHERE pkgmap.pkgid = ? AND provider.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);

		libexec = util_conf_get_libexec(pkgid);
		DbgPrint("Fallback to conf checker: %s\n", libexec);
		goto out;
	}

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Invalid appid: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = (char *)sqlite3_column_text(stmt, 1);
	if (!path || !strlen(path)) {
		ErrPrint("Invalid libexec: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	libexec = strdup(path);
	if (!libexec) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("libexec: %s\n", libexec);

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return libexec;
}

EAPI char *livebox_service_pkgname(const char *appid)
{
	char *lb_pkgname;
	pkgmgr_appinfo_h handle;
	int ret;
	char *new_appid;

	if (!appid)
		return NULL;

	lb_pkgname = get_lb_pkgname_by_appid(appid);
	if (lb_pkgname)
		return lb_pkgname;

	/*!
	 * \note
	 * Try to get the package id using given appid
	 */
	ret = pkgmgr_appinfo_get_appinfo(appid, &handle);
	if (ret != PKGMGR_R_OK) {
		ErrPrint("Failed to get appinfo\n");
		return NULL;
	}

	ret = pkgmgr_appinfo_get_pkgname(handle, &new_appid);
	if (ret != PKGMGR_R_OK) {
		pkgmgr_appinfo_destroy_appinfo(handle);
		ErrPrint("Failed to get pkgname for (%s)\n", appid);
		return NULL;
	}

	lb_pkgname = get_lb_pkgname_by_appid(new_appid);
	pkgmgr_appinfo_destroy_appinfo(handle);

	if (!lb_pkgname && util_validate_livebox_package(appid) == 0)
		return strdup(appid);

	return lb_pkgname;
}

EAPI char *livebox_service_provider_name(const char *lbid)
{
	char *ret;
	int stage = 0;
	int seq = 0;
	int idx = 0;
	char *str = SAMSUNG_PREFIX;

	if (!lbid)
		return NULL;

	while (str[idx] && lbid[idx] && lbid[idx] == str[idx]) {
		idx++;
		if (seq < 2 && lbid[idx] == '.') {
			stage = idx;
			seq++;
		}
	}

	if (!str[idx] && lbid[idx]) {
		/* Inhouse */
		return strdup(lbid);
	} else if (seq < 2) {
		while (seq < 2) {
			if (lbid[idx] == '.') {
				seq++;
			} else if (!lbid[idx]) {
				ErrPrint("Invalid lbid: %s\n", lbid);
				return NULL;
			}

			idx++;
		}

		stage = idx;
	} else {
		stage++;
	}

	ret = strdup(lbid + stage);
	if (!ret) {
		ErrPrint("Error: %s\n", strerror(errno));
		return NULL;
	}

	return ret;
}

EAPI int livebox_service_is_enabled(const char *lbid)
{
	return 1;
	/*
	ail_appinfo_h ai;
	char *pkgname;
	bool enabled;
	int ret;

	pkgname = livebox_service_appid(lbid);
	if (!pkgname)
		return 0;

	ret = ail_get_appinfo(pkgname, &ai);
	if (ret != AIL_ERROR_OK) {
		free(pkgname);
		return 0;
	}

	if (ail_appinfo_get_bool(ai, AIL_PROP_X_SLP_ENABLED_BOOL, &enabled) != AIL_ERROR_OK)
		enabled = false;

	ail_destroy_appinfo(ai);
	free(pkgname);
	return enabled == true;
	*/
}

EAPI int livebox_service_is_primary(const char *lbid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret = 0;

	if (!lbid)
		return 0;

	handle = open_db();
	if (!handle)
		return 0;

	ret = sqlite3_prepare_v2(handle, "SELECT prime FROM pkgmap WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 0;
	}

	ret = sqlite3_bind_text(stmt, 1, lbid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_column_int(stmt, 0);

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

/*!
 * appid == Package ID
 * pkgid == Livebox ID
 */
EAPI char *livebox_service_appid(const char *pkgname)
{
	sqlite3_stmt *stmt;
	char *appid;
	char *tmp;
	sqlite3 *handle;
	int is_prime __attribute__((__unused__));
	int ret;

	if (!pkgname)
		return NULL;

	appid = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT appid, prime FROM pkgmap WHERE pkgid = ? OR appid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgname, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgname, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		pkgmgr_appinfo_h pkg_handle;
		char *new_appid;

		ErrPrint("Has no record?: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);

		ret = pkgmgr_appinfo_get_appinfo(pkgname, &pkg_handle);
		if (ret != PKGMGR_R_OK) {
			ErrPrint("Failed to get appinfo: %s\n", pkgname);
			goto out;
		}

		ret = pkgmgr_appinfo_get_pkgname(pkg_handle, &new_appid);
		if (ret != PKGMGR_R_OK) {
			ErrPrint("Failed to get pkgname for (%s)\n", appid);
			pkgmgr_appinfo_destroy_appinfo(pkg_handle);
			goto out;
		}

		appid = strdup(new_appid);
		if (!appid)
			ErrPrint("Heap: %s\n", strerror(errno));

		pkgmgr_appinfo_destroy_appinfo(pkg_handle);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("APPID is NIL\n");
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	appid = strdup(tmp);
	if (!appid) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	is_prime = sqlite3_column_int(stmt, 1);

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return appid;
}

EAPI char *livebox_service_lb_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *appid;
	char *lb_src;

	if (!pkgid)
		return NULL;

	path = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.box_src FROM provider, pkgmap WHERE pkgmap.pkgid = ? AND provider.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Invalid appid : %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	lb_src = (char *)sqlite3_column_text(stmt, 1);
	if (!lb_src || !strlen(lb_src)) {
		ErrPrint("No records for lb src : %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = strdup(lb_src);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("LB Src: %s\n", path);

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return path;
}

EAPI char *livebox_service_lb_script_group(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *group;
	char *tmp;

	if (!pkgid)
		return NULL;

	group = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT box_group FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (tmp && strlen(tmp)) {
		group = strdup(tmp);
		if (!group)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return group;
}

EAPI char *livebox_service_pd_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *pd_src;
	const char *appid;

	if (!pkgid)
		return NULL;

	path = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.pd_src FROM provider, pkgmap WHERE provider.pkgid = ? AND pkgmap.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	pd_src = (char *)sqlite3_column_text(stmt, 1);
	if (!pd_src || !strlen(pd_src)) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = strdup(pd_src);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("PD Src: %s\n", path);
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return path;
}

EAPI char *livebox_service_pd_script_group(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *group;
	char *tmp;

	if (!pkgid)
		return NULL;

	group = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pd_group FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (tmp && strlen(tmp)) {
		group = strdup(tmp);
		if (!group)
			ErrPrint("Heap: %s\n", strerror(errno));
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return group;
}

EAPI int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	const char *cluster;
	int cnt;
	int ret;

	if (!cb)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	cnt = 0;
	ret = sqlite3_prepare_v2(handle, "SELECT DISTINCT cluster FROM groupinfo", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		cnt = LB_STATUS_ERROR_IO;
		goto out;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		cluster = (const char *)sqlite3_column_text(stmt, 0);
		if (!cluster || !strlen(cluster))
			continue;

		if (cb(cluster, data) < 0)
			break;

		cnt++;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return cnt;
}

EAPI int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	const char *category;
	int cnt;
	int ret;

	if (!cluster || !cb)
		return LB_STATUS_ERROR_INVALID;

	handle = open_db();
	if (!handle)
		return LB_STATUS_ERROR_IO;

	ret = sqlite3_prepare_v2(handle, "SELECT DISTINCT category FROM groupinfo WHERE cluster = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		cnt = LB_STATUS_ERROR_IO;
		goto out;
	}

	cnt = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		category = (const char *)sqlite3_column_text(stmt, 0);
		if (!category || !strlen(category))
			continue;

		if (cb(cluster, category, data) < 0)
			break;

		cnt++;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return cnt;
}

EAPI int livebox_service_init(void)
{
	if (s_info.handle) {
		DbgPrint("Already initialized\n");
		s_info.init_count++;
		return 0;
	}

	s_info.handle = open_db();
	if (s_info.handle) {
		s_info.init_count++;
		return 0;
	}

	return LB_STATUS_ERROR_IO;
}

EAPI int livebox_service_fini(void)
{
	if (!s_info.handle || s_info.init_count <= 0) {
		ErrPrint("Service is not initialized\n");
		return LB_STATUS_ERROR_IO;
	}

	s_info.init_count--;
	if (s_info.init_count > 0) {
		DbgPrint("Init count %d\n", s_info.init_count);
		return 0;
	}

	db_util_close(s_info.handle);
	s_info.handle = NULL;
	return 0;
}

EAPI int livebox_service_get_size(int type, int *width, int *height)
{
	int _width;
	int _height;

	if (!width)
		width = &_width;

	if (!height)
		height = &_height;

	return convert_size_from_type(type, width, height);
}

EAPI int livebox_service_size_type(int width, int height)
{
	int idx;

	if (update_resolution() < 0)
		ErrPrint("Failed to update the size list\n");

	for (idx = 0; idx < NR_OF_SIZE_LIST; idx++) {
		if (SIZE_LIST[idx].w == width && SIZE_LIST[idx].h == height)
			break;
	}

	switch (idx) {
	case 0:
		return LB_SIZE_TYPE_1x1;
	case 1:
		return LB_SIZE_TYPE_2x1;
	case 2:
		return LB_SIZE_TYPE_2x2;
	case 3:
		return LB_SIZE_TYPE_4x1;
	case 4:
		return LB_SIZE_TYPE_4x2;
	case 5:
		return LB_SIZE_TYPE_4x3;
	case 6:
		return LB_SIZE_TYPE_4x4;
	case 7:
		return LB_SIZE_TYPE_4x5;
	case 8:
		return LB_SIZE_TYPE_4x6;
	case 9:
		return LB_SIZE_TYPE_EASY_1x1;
	case 10:
		return LB_SIZE_TYPE_EASY_3x1;
	case 11:
		return LB_SIZE_TYPE_EASY_3x3;
	case 12:
		return LB_SIZE_TYPE_0x0;
	default:
		break;
	}

	return LB_SIZE_TYPE_UNKNOWN;
}

/* End of a file */

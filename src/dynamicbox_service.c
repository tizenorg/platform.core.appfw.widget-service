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
#include <unicode/uloc.h>

#include "dlist.h"
#include "util.h"
#include "debug.h"
#include "dynamicbox_service.h"
#include "dynamicbox_errno.h"
#include "dynamicbox_cmd_list.h"

#define SAMSUNG_PREFIX	"com.samsung."
#define EAPI __attribute__((visibility("default")))
#define DEFAULT_TIMEOUT 2.0

static struct supported_size_list SIZE_LIST[DBOX_NR_OF_SIZE_LIST] = {
	{ 175, 175 }, /*!< 1x1 */
	{ 354, 175 }, /*!< 2x1 */
	{ 354, 354 }, /*!< 2x2 */
	{ 712, 175 }, /*!< 4x1 */
	{ 712, 354 }, /*!< 4x2 */
	{ 712, 533 }, /*!< 4x3 */
	{ 712, 712 }, /*!< 4x4 */
	{ 712, 891 }, /*!< 4x5 */
	{ 712, 1070 }, /*!< 4x6 */
	{ 224, 215 }, /*!< 21x21 */
	{ 680, 215 }, /*!< 23x21 */
	{ 680, 653 }, /*!< 23x23 */
	{ 720, 1280 }, /*!< 0x0 */
};

struct dynamicbox_pkglist_handle {
	enum pkglist_type {
		PKGLIST_TYPE_DBOX_LIST = 0x00beef00,
		PKGLIST_TYPE_UNKNOWN = 0x00dead00
	} type;
	sqlite3 *handle;
	sqlite3_stmt *stmt;
};

static struct service_info s_info = {
	.handle = NULL,
	.dbfile = DB_FILE, 
	.conf_file = NULL,
	.init_count = 0,

	.iso3lang = NULL,
	.country = { 0, },
	.syslang = NULL,
	.country_len = 0,

	.base_w = 720,
	.base_h = 1280,

	.base_parse = 0,
};

struct pkgmgr_cbdata {
	const char *dboxid;
	void (*cb)(const char *dboxid, const char *appid, void *data);
	void *cbdata;
};

static sqlite3 *open_db(void)
{
	sqlite3 *handle;

	if (!s_info.handle) {
		int ret;

		ret = db_util_open_with_options(s_info.dbfile, &handle, SQLITE_OPEN_READONLY, NULL);
		if (ret != SQLITE_OK) {
			ErrPrint("Failed to open a DB\n");
			return NULL;
		}
	} else {
		handle = s_info.handle;
	}

	return handle;
}

static inline __attribute__((always_inline)) void close_db(sqlite3 *handle)
{
	if (!s_info.handle) {
		db_util_close(handle);
	}
}

static int convert_size_from_type(enum dynamicbox_size_type type, int *width, int *height)
{
	int idx;

	switch (type) {
	case DBOX_SIZE_TYPE_1x1: /*!< 175x175 */
		idx = 0;
		break;
	case DBOX_SIZE_TYPE_2x1: /*!< 354x175 */
		idx = 1;
		break;
	case DBOX_SIZE_TYPE_2x2: /*!< 354x354 */
		idx = 2;
		break;
	case DBOX_SIZE_TYPE_4x1: /*!< 712x175 */
		idx = 3;
		break;
	case DBOX_SIZE_TYPE_4x2: /*!< 712x354 */
		idx = 4;
		break;
	case DBOX_SIZE_TYPE_4x3: /*!< 712x533 */
		idx = 5;
		break;
	case DBOX_SIZE_TYPE_4x4: /*!< 712x712 */
		idx = 6;
		break;
	case DBOX_SIZE_TYPE_4x5: /*!< 712x891 */
		idx = 7;
		break;
	case DBOX_SIZE_TYPE_4x6: /*!< 712x1070 */
		idx = 8;
		break;
	case DBOX_SIZE_TYPE_EASY_1x1: /*< 224x215 */
		idx = 9;
		break;
	case DBOX_SIZE_TYPE_EASY_3x1: /*!< 680x215 */
		idx = 10;
		break;
	case DBOX_SIZE_TYPE_EASY_3x3: /*!< 680x653 */
		idx = 11;
		break;
	case DBOX_SIZE_TYPE_0x0: /*!< 720x1280 */
		idx = 12;
		break;
	default:
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	if (util_update_resolution(&s_info, SIZE_LIST) < 0) {
		ErrPrint("Failed to update resolution\n");
	}

	*width = SIZE_LIST[idx].w;
	*height = SIZE_LIST[idx].h;
	return DBOX_STATUS_ERROR_NONE;
}

static int pkgmgr_cb(const pkgmgrinfo_appinfo_h handle, void *user_data)
{
	struct pkgmgr_cbdata *cbdata = (struct pkgmgr_cbdata *)user_data;
	char *appid;
	int ret;

	ret = pkgmgrinfo_appinfo_get_appid(handle, &appid);
	if (ret < 0) {
		ErrPrint("Unable to get appid\n");
	} else {
		cbdata->cb(cbdata->dboxid, appid, cbdata->cbdata);
	}

	return 0;
}

static inline char *pkgmgr_get_mainapp(const char *pkgid)
{
	pkgmgrinfo_pkginfo_h handle;
	char *ret = NULL;

	if (pkgmgrinfo_pkginfo_get_pkginfo(pkgid, &handle) != PMINFO_R_OK) {
		ErrPrint("Unable to get mainapp: %s\n", pkgid);
		return NULL;
	}

	if (pkgmgrinfo_pkginfo_get_mainappid(handle, &ret) == PMINFO_R_OK) {
		ret = strdup(ret);
	} else {
		ErrPrint("Failed to get mainappid\n");
		ret = NULL; /* I cannot believe the pkgmgrinfo_pkginfo_get_mainappid. it maybe able to touch my "ret" even though it fails */
	
	}

	pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
	return ret;
}

static inline int pkgmgr_get_applist(const char *pkgid, const char *dboxid, void (*cb)(const char *dboxid, const char *appid, void *data), void *data)
{
	struct pkgmgr_cbdata cbdata;
	pkgmgrinfo_pkginfo_h handle;
	int ret;

	ret = pkgmgrinfo_pkginfo_get_pkginfo(pkgid, &handle);
	if (ret < 0) {
		ErrPrint("Unable to get pkginfo: %s\n", pkgid);
		return ret;
	}

	cbdata.dboxid = dboxid;
	cbdata.cb = cb;
	cbdata.cbdata = data;

	ret = pkgmgrinfo_appinfo_get_list(handle, PM_UI_APP, pkgmgr_cb, &cbdata);
	if (ret < 0) {
		ErrPrint("Failed to get applist\n");
	}

	pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
	return ret;
}

static char *cur_locale(void)
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

			if (*ptr == '_') {
				*ptr = '-';
			}

			ptr++;
		}
	} else {
		language = strdup("en-us");
		if (!language) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

	return language;
}

static char *get_default_name(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *name = NULL;
	int ret;

	handle = open_db();
	if (!handle) {
		return NULL;
	}

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
			if (!name) {
				ErrPrint("Heap: %s\n", strerror(errno));
			}
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return name;
}

static char *get_default_icon(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *icon = NULL;
	int ret;

	handle = open_db();
	if (!handle) {
		return NULL;
	}

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
			if (!icon) {
				ErrPrint("Heap: %s\n", strerror(errno));
			}
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return icon;
}

static char *get_dbox_pkgname_by_appid(const char *appid)
{
	sqlite3_stmt *stmt;
	char *pkgid;
	char *tmp;
	sqlite3 *handle;
	int ret;

	if (!appid) {
		return NULL;
	}

	pkgid = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid FROM pkgmap WHERE (appid = ? AND prime = 1) OR (uiapp = ? AND prime = 1) OR pkgid = ?", -1, &stmt, NULL);
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

	ret = sqlite3_bind_text(stmt, 3, appid, -1, SQLITE_TRANSIENT);
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
		if (!pkgid) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return pkgid;
}

static inline int update_lang_info(void)
{
	char *syslang;
	UErrorCode err;

	syslang = vconf_get_str(VCONFKEY_LANGSET);
	if (!syslang) {
		ErrPrint("Failed to get vconf-lang\n");
		return -EFAULT;
	}

	if (s_info.syslang && !strcmp(s_info.syslang, syslang)) {
		DbgPrint("Syslang is not changed: %s\n", syslang);
		free(syslang);
		return 0;
	}

	free(s_info.syslang);
	s_info.syslang = syslang;

	err = U_ZERO_ERROR;
	uloc_setDefault((const char *)s_info.syslang, &err);
	if (!U_SUCCESS(err)) {
		ErrPrint("Failed to set default lang: %s\n", u_errorName(err));
		free(s_info.syslang);
		s_info.syslang = NULL;
		return -EFAULT;
	}

	s_info.iso3lang = uloc_getISO3Language(uloc_getDefault());
	if (!s_info.iso3lang || !strlen(s_info.iso3lang)) {
		ErrPrint("Failed to get iso3lang\n");
		free(s_info.syslang);
		s_info.syslang = NULL;
		return -EFAULT;
	}

	err = U_ZERO_ERROR;
	s_info.country_len = uloc_getCountry(uloc_getDefault(), s_info.country, ULOC_COUNTRY_CAPACITY, &err);
	if (!U_SUCCESS(err) || s_info.country_len <= 0) {
		ErrPrint("Failed to get locale: %s, %s, %d (%s)\n", u_errorName(err), s_info.iso3lang, s_info.country_len, s_info.country);
		free(s_info.syslang);
		s_info.syslang = NULL;
		return -EFAULT;
	}

	return 0;
}

EAPI int dynamicbox_service_change_period(const char *pkgname, const char *id, double period)
{
	struct packet *packet;
	struct packet *result;
	unsigned int cmd = CMD_SERVICE_CHANGE_PERIOD;
	char *uri;
	int ret;

	if (!pkgname || !id || period < 0.0f) {
		ErrPrint("Invalid argument\n");
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	uri = util_id_to_uri(id);
	if (!uri) {
		return DBOX_STATUS_ERROR_OUT_OF_MEMORY;
	}

	packet = packet_create((const char *)&cmd, "ssd", pkgname, uri, period);
	free(uri);
	if (!packet) {
		ErrPrint("Failed to create a packet for period changing\n");
		return DBOX_STATUS_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "i", &ret) != 1) {
			ErrPrint("Failed to parse a result packet\n");
			ret = DBOX_STATUS_ERROR_INVALID_PARAMETER;
		}
		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = DBOX_STATUS_ERROR_FAULT;
	}

	return ret;
}

EAPI int dynamicbox_service_get_instance_count(const char *pkgname, const char *cluster, const char *category)
{
	struct packet *packet;
	struct packet *result;
	unsigned int cmd = CMD_SERVICE_INST_CNT;
	int ret;

	packet = packet_create((const char *)&cmd, "sssd", pkgname, cluster, category, util_timestamp());
	if (!packet) {
		ErrPrint("Failed to create a packet for period changing\n");
		return DBOX_STATUS_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "i", &ret) != 1) {
			ErrPrint("Failed to parse a result packet\n");
			ret = DBOX_STATUS_ERROR_INVALID_PARAMETER;
		}
		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = DBOX_STATUS_ERROR_FAULT;
	}

	return ret;
}

EAPI int dynamicbox_service_trigger_update(const char *pkgname, const char *id, const char *cluster, const char *category, const char *content, int force)
{
	struct packet *packet;
	struct packet *result;
	unsigned int cmd = CMD_SERVICE_UPDATE;
	char *uri;
	int ret;

	if (!pkgname) {
		ErrPrint("Invalid argument\n");
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	if (!force && access("/tmp/.live.paused", R_OK) == 0) {
		DbgPrint("Provider is paused\n");
		return DBOX_STATUS_ERROR_CANCEL;
	}

	if (id) {
		uri = util_id_to_uri(id);
		if (!uri) {
			return DBOX_STATUS_ERROR_OUT_OF_MEMORY;
		}
	} else {
		uri = NULL;
	}

	if (!cluster) {
		cluster = "user,created";
	}

	if (!category) {
		category = "default";
	}

	packet = packet_create((const char *)&cmd, "sssssi", pkgname, uri, cluster, category, content, force);
	/*!
	 * \note
	 * "free" function accepts NULL
	 */
	free(uri);
	if (!packet) {
		ErrPrint("Failed to create a packet for service_update\n");
		return DBOX_STATUS_ERROR_FAULT;
	}

	result = com_core_packet_oneshot_send(SERVICE_SOCKET, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "i", &ret) != 1) {
			ErrPrint("Failed to parse a result packet\n");
			ret = DBOX_STATUS_ERROR_INVALID_PARAMETER;
		}

		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = DBOX_STATUS_ERROR_FAULT;
	}

	return ret;
}

EAPI dynamicbox_pkglist_h dynamicbox_service_pkglist_create(const char *pkgid, dynamicbox_pkglist_h handle)
{
	int ret;

	if (handle) {
		if (handle->type != PKGLIST_TYPE_DBOX_LIST) {
			ErrPrint("Invalid handle\n");
			return NULL;
		}

		if (pkgid) {
			ErrPrint("pkgid should be NULL\n");
			return NULL;
		}

		sqlite3_reset(handle->stmt);
		return handle;
	}

	handle = calloc(1, sizeof(*handle));
	if (!handle) {
		ErrPrint("Heap: %s\n", strerror(errno));
		return NULL;
	}

	handle->type = PKGLIST_TYPE_DBOX_LIST;

	handle->handle = open_db();
	if (!handle->handle) {
		free(handle);
		return NULL;
	}

	if (!pkgid) {
		ret = sqlite3_prepare_v2(handle->handle, "SELECT appid, pkgid, prime FROM pkgmap", -1, &handle->stmt, NULL);
		if (ret != SQLITE_OK) {
			ErrPrint("Error: %s\n", sqlite3_errmsg(handle->handle));
			close_db(handle->handle);
			free(handle);
			return NULL;
		}
	} else {
		ret = sqlite3_prepare_v2(handle->handle, "SELECT appid, pkgid, prime FROM pkgmap WHERE appid = ?", -1, &handle->stmt, NULL);
		if (ret != SQLITE_OK) {
			ErrPrint("Error: %s\n", sqlite3_errmsg(handle->handle));
			close_db(handle->handle);
			free(handle);
			return NULL;
		}

		ret = sqlite3_bind_text(handle->stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
		if (ret != SQLITE_OK) {
			ErrPrint("Error: %s\n", sqlite3_errmsg(handle->handle));
			sqlite3_finalize(handle->stmt);
			close_db(handle->handle);
			free(handle);
			return NULL;
		}
	}

	return handle;
}

EAPI int dynamicbox_service_get_pkglist_item(dynamicbox_pkglist_h handle, char **appid, char **pkgname, int *is_prime)
{
	const char *tmp;
	char *_appid = NULL;
	char *_pkgname = NULL;

	if (!handle || handle->type != PKGLIST_TYPE_DBOX_LIST) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	if (sqlite3_step(handle->stmt) != SQLITE_ROW) {
		return DBOX_STATUS_ERROR_NOT_EXIST;
	}

	if (appid) {
		tmp = (const char *)sqlite3_column_text(handle->stmt, 0);
		if (tmp && strlen(tmp)) {
			_appid = strdup(tmp);
			if (!_appid) {
				ErrPrint("Heap: %s\n", strerror(errno));
				return DBOX_STATUS_ERROR_OUT_OF_MEMORY;
			}
		}
	}

	if (pkgname) {
		tmp = (const char *)sqlite3_column_text(handle->stmt, 1);
		if (tmp && strlen(tmp)) {
			_pkgname = strdup(tmp);
			if (!_pkgname) {
				ErrPrint("Heap: %s\n", strerror(errno));
				free(_appid);
				return DBOX_STATUS_ERROR_OUT_OF_MEMORY;
			}
		}
	}

	if (is_prime) {
		*is_prime = sqlite3_column_int(handle->stmt, 2);
	}

	if (appid) {
		*appid = _appid;
	}

	if (pkgname) {
		*pkgname = _pkgname;
	}

	return DBOX_STATUS_ERROR_NONE;
}

EAPI int dynamicbox_service_pkglist_destroy(dynamicbox_pkglist_h handle)
{
	if (!handle || handle->type != PKGLIST_TYPE_DBOX_LIST) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle->type = PKGLIST_TYPE_UNKNOWN;
	sqlite3_reset(handle->stmt);
	sqlite3_finalize(handle->stmt);
	close_db(handle->handle);
	free(handle);
	return DBOX_STATUS_ERROR_NONE;
}

EAPI int dynamicbox_service_get_pkglist(int (*cb)(const char *appid, const char *pkgname, int is_prime, void *data), void *data)
{
	int ret;
	sqlite3_stmt *stmt;
	char *appid;
	char *pkgid;
	int is_prime;
	sqlite3 *handle;

	if (!cb) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT appid, pkgid, prime FROM pkgmap", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
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

EAPI int dynamicbox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *dboxid, int is_prime, void *data), void *data)
{
	int ret;
	sqlite3_stmt *stmt;
	const char *dboxid;
	int is_prime;
	sqlite3 *handle;

	if (!cb || !pkgid) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid, prime FROM pkgmap WHERE appid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		dboxid = (const char *)sqlite3_column_text(stmt, 0);
		if (!dboxid || !strlen(dboxid)) {
			ErrPrint("DBOXID is not valid\n");
			continue;
		}

		is_prime = sqlite3_column_int(stmt, 1);

		ret++;

		if (cb(dboxid, is_prime, data) < 0) {
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

EAPI int dynamicbox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *dboxid, void *data), void *data)
{
	int ret;
	sqlite3_stmt *stmt;
	const char *dboxid;
	sqlite3 *handle;

	if (!cb || !category) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid FROM pkgmap WHERE category = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, category, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		dboxid = (const char *)sqlite3_column_text(stmt, 0);
		if (!dboxid || !strlen(dboxid)) {
			ErrPrint("DBOXID is not valid\n");
			continue;
		}

		ret++;

		if (cb(dboxid, data) < 0) {
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

EAPI int dynamicbox_service_get_applist(const char *dboxid, void (*cb)(const char *dboxid, const char *appid, void *data), void *data)
{
	sqlite3_stmt *stmt;
	const char *tmp;
	char *pkgid;
	sqlite3 *handle;
	int ret;

	if (!dboxid || !cb) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT appid FROM pkgmap WHERE (pkgid = ?) or (appid = ?)", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, dboxid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ret = DBOX_STATUS_ERROR_INVALID_PARAMETER;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (const char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("Invalid package name (%s)\n", dboxid);
		ret = DBOX_STATUS_ERROR_INVALID_PARAMETER;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	pkgid = strdup(tmp);
	if (!pkgid) {
		ErrPrint("Error: %s\n", strerror(errno));
		ret = DBOX_STATUS_ERROR_OUT_OF_MEMORY;
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	ret = pkgmgr_get_applist(pkgid, dboxid, cb, data);
	free(pkgid);

	switch (ret) {
	case PMINFO_R_EINVAL:
		ret = DBOX_STATUS_ERROR_INVALID_PARAMETER;
		break;
	case PMINFO_R_OK:
		ret = DBOX_STATUS_ERROR_NONE;
		break;
	case PMINFO_R_ERROR:
	default:
		ret = DBOX_STATUS_ERROR_FAULT;
		break;
	}

out:
	close_db(handle);
	return ret;
}

EAPI char *dynamicbox_service_mainappid(const char *dboxid)
{
	sqlite3_stmt *stmt;
	const char *tmp;
	const char *pkgid;
	sqlite3 *handle;
	char *ret = NULL;

	if (!dboxid) {
		return NULL;
	}

	handle = open_db();
	if (!handle) {
		return NULL;
	}

	if (sqlite3_prepare_v2(handle, "SELECT appid, uiapp FROM pkgmap WHERE (pkgid = ?) or (appid = ? and prime = 1)", -1, &stmt, NULL) != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	if (sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	if (sqlite3_bind_text(stmt, 2, dboxid, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (const char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("Invalid package name (%s)\n", dboxid);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	pkgid = (const char *)sqlite3_column_text(stmt, 1);
	if (!pkgid || !strlen(pkgid)) {
		/*
		 * This record has no uiapp.
		 * Try to find the main ui-app id.
		 */
		ret = pkgmgr_get_mainapp(tmp);
	} else {
		ret = strdup(pkgid);
		if (!ret) {
			ErrPrint("Error: %s\n", strerror(errno));
		}
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

out:
	close_db(handle);
	return ret;
}

EAPI int dynamicbox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int size;
	int ret;

	if (!types || !cnt || !pkgid) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT size_type FROM box_size WHERE pkgid = ? ORDER BY size_type ASC", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	if (*cnt > DBOX_NR_OF_SIZE_LIST) {
		*cnt = DBOX_NR_OF_SIZE_LIST;
	}

	ret = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW && ret < *cnt) {
		size = sqlite3_column_int(stmt, 0);
		types[ret] = size;
		ret++;
	}

	*cnt = ret;
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	ret = DBOX_STATUS_ERROR_NONE;
out:
	close_db(handle);
	return ret;
}

EAPI char *dynamicbox_service_content(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *content = NULL;
	int ret;

	handle = open_db();
	if (!handle) {
		return NULL;
	}

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
			if (!content) {
				ErrPrint("Heap: %s\n", strerror(errno));
			}
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return content;
}

EAPI char *dynamicbox_service_setup_appid(const char *dboxid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *appid;

	if (!dboxid) {
		return NULL;
	}

	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT setup FROM client WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	appid = NULL;
	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		const char *tmp;

		tmp = (const char *)sqlite3_column_text(stmt, 0);
		if (!tmp || !strlen(tmp)) {
			goto out;
		}

		appid = strdup(tmp);
		if (!appid) {
			ErrPrint("Error: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return appid;
}

EAPI int dynamicbox_service_nodisplay(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;

	if (!pkgid) {
		return 0;
	}

	handle = open_db();
	if (!handle) {
		return 0;
	}

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
	if (ret == SQLITE_ROW) {
		ret = !!sqlite3_column_int(stmt, 0);
	} else {
		ret = 0;
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

EAPI int dynamicbox_service_need_frame(const char *pkgid, int size_type)
{
	char *dboxid;
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
	dboxid = dynamicbox_service_dbox_id(pkgid);
	if (!dboxid) {
		ErrPrint("Invalid appid (%s)\n", pkgid);
		ret = 0;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	free(dboxid);
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

EAPI int dynamicbox_service_touch_effect(const char *pkgid, int size_type)
{
	char *dboxid;
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
	dboxid = dynamicbox_service_dbox_id(pkgid);
	if (!dboxid) {
		ErrPrint("Invalid appid (%s)\n", pkgid);
		ret = 1;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	free(dboxid);
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

EAPI int dynamicbox_service_mouse_event(const char *pkgid, int size_type)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *dboxid;
	int ret;

	handle = open_db();
	if (!handle) {
		return 0;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT mouse_event FROM box_size WHERE pkgid = ? AND size_type = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 0;
	}

	dboxid = dynamicbox_service_dbox_id(pkgid);
	if (!dboxid) {
		ErrPrint("Failed to get dboxid: %s\n", pkgid);
		ret = 0;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	free(dboxid);
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
		ret = 0; /*!< Default is false, In this case the DB is corrupted */
		ErrPrint("There is no result.\n");
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return ret;
}

EAPI char *dynamicbox_service_preview(const char *pkgid, int size_type)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *preview = NULL;
	const char *tmp;
	int tmp_len;
	int buf_len;
	register int i;
	int printed;

	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT preview FROM box_size WHERE pkgid = ? AND size_type = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s, %s\n", sqlite3_errmsg(handle), pkgid);
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s, %s\n", sqlite3_errmsg(handle), pkgid);
		goto out;
	}

	ret = sqlite3_bind_int(stmt, 2, size_type);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s, %s\n", sqlite3_errmsg(handle), pkgid);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s, %s\n", sqlite3_errmsg(handle), pkgid);
		goto out;
	}

	tmp = (const char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !(tmp_len = strlen(tmp))) {
		ErrPrint("Failed to get data (%s)\n", pkgid);
		goto out;
	}

	if (update_lang_info() != 0) {
		preview = strdup(tmp);
		if (!preview) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}
		goto out;
	}

	buf_len = tmp_len + strlen(s_info.iso3lang) + s_info.country_len + 3; /* '/' '-' '/' */
	preview = malloc(buf_len + 1);
	if (!preview) {
		ErrPrint("Heap: %s\n", strerror(errno));
		goto out;
	}

	for (i = tmp_len; i >= 0 && tmp[i] != '/'; i--);
	i++; /* Skip '/' */

	strncpy(preview, tmp, i);
	printed = snprintf(preview + i, buf_len - i, "%s-%s/%s", s_info.iso3lang, s_info.country, tmp + i);
	if (preview[i + printed] != '\0') {
		ErrPrint("Path is truncated\n");
		preview[i + printed] = '\0';
	}

	if (access(preview, R_OK) != 0) {
		DbgPrint("Access failed: %s, %s\n", preview, strerror(errno));
		free(preview);

		preview = strdup(tmp);
		if (!preview) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return preview;
}

EAPI char *dynamicbox_service_i18n_icon(const char *pkgid, const char *lang)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *language;
	char *icon = NULL;
	int ret;

	if (!pkgid) {
		return NULL;
	}

	if (lang) {
		language = strdup(lang);
		if (!language) {
			ErrPrint("Heap: %s\n", strerror(errno));
			return NULL;
		}
	} else {
		language = cur_locale();
		if (!language) {
			return NULL;
		}
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
			if (!icon) {
				ErrPrint("Heap: %s\n", strerror(errno));
			}
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

EAPI char *dynamicbox_service_i18n_name(const char *pkgid, const char *lang)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	char *language;
	char *name = NULL;
	int ret;

	if (!pkgid) {
		return NULL;
	}

	if (lang) {
		language = strdup(lang);
		if (!language) {
			ErrPrint("Error: %s\n", strerror(errno));
			return NULL;
		}
	} else {
		language = cur_locale();
		if (!language) {
			return NULL;
		}
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
			if (!name) {
				ErrPrint("Heap: %s\n", strerror(errno));
			}
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

EAPI int dynamicbox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int size;
	int ret;

	if (!w || !h || !cnt || !pkgid) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT size_type FROM box_size WHERE pkgid = ? ORDER BY size_type ASC", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	if (*cnt > DBOX_NR_OF_SIZE_LIST) {
		*cnt = DBOX_NR_OF_SIZE_LIST;
	}

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

EAPI char *dynamicbox_service_abi(const char *dboxid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *abi;
	char *tmp;

	if (!dboxid) {
		ErrPrint("Invalid argument\n");
		return NULL;
	}

	abi = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT abi FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s (%d)\n", sqlite3_errmsg(handle), ret);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("Invalid abi: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	abi = strdup(tmp);
	if (!abi) {
		ErrPrint("strdup: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("abi: %s\n", abi);

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return abi;
}

EAPI char *dynamicbox_service_dbox_id_by_libexec(const char *libexec)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *pkgid;
	char *tmp;
	char *_libexec;
	int len;

	if (!libexec) {
		ErrPrint("Invalid argument\n");
		return NULL;
	}

	pkgid = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

	len = strlen(libexec) + 3;

	_libexec = malloc(len);
	if (!_libexec) {
		ErrPrint("Heap: %s\n", strerror(errno));
		close_db(handle);
		return NULL;
	}

	snprintf(_libexec, len - 1, "%%%s", libexec);

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid FROM provider WHERE libexec like ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, _libexec, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ErrPrint("No records (%s) for (%s)\n", sqlite3_errmsg(handle), libexec);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("Invalid pkgid: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	pkgid = strdup(tmp);
	if (!pkgid) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}

	DbgPrint("pkgid: %s\n", pkgid);

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	free(_libexec);
	return pkgid;
}

EAPI char *dynamicbox_service_libexec(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *libexec;
	char *appid;
	char *path;

	if (!pkgid) {
		ErrPrint("Invalid argument\n");
		return NULL;
	}

	libexec = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

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

EAPI char *dynamicbox_service_dbox_id(const char *appid)
{
	char *dbox_pkgname;
	pkgmgrinfo_appinfo_h handle;
	int ret;
	char *new_appid;

	if (!appid) {
		return NULL;
	}

	dbox_pkgname = get_dbox_pkgname_by_appid(appid);
	if (dbox_pkgname) {
		return dbox_pkgname;
	}

	/*!
	 * \note
	 * Try to get the package id using given appid
	 */
	ret = pkgmgrinfo_appinfo_get_appinfo(appid, &handle);
	if (ret != PKGMGR_R_OK) {
		ErrPrint("Failed to get appinfo\n");
		return NULL;
	}

	ret = pkgmgrinfo_appinfo_get_pkgid(handle, &new_appid);
	if (ret != PKGMGR_R_OK) {
		pkgmgrinfo_appinfo_destroy_appinfo(handle);
		ErrPrint("Failed to get pkgname for (%s)\n", appid);
		return NULL;
	}

	dbox_pkgname = get_dbox_pkgname_by_appid(new_appid);
	pkgmgrinfo_appinfo_destroy_appinfo(handle);

	if (!dbox_pkgname) {
		dbox_pkgname = strdup(appid);
	}

	return dbox_pkgname;
}

EAPI char *dynamicbox_service_package_id(const char *pkgname)
{
	sqlite3_stmt *stmt;
	char *appid;
	char *tmp;
	sqlite3 *handle;
	int is_prime __attribute__((__unused__));
	int ret;

	if (!pkgname) {
		return NULL;
	}

	appid = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

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
		pkgmgrinfo_appinfo_h pkg_handle;
		char *new_appid;

		ErrPrint("Has no record?: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);

		ret = pkgmgrinfo_appinfo_get_appinfo(pkgname, &pkg_handle);
		if (ret != PKGMGR_R_OK) {
			ErrPrint("Failed to get appinfo: %s\n", pkgname);
			goto out;
		}

		ret = pkgmgrinfo_appinfo_get_pkgid(pkg_handle, &new_appid);
		if (ret != PKGMGR_R_OK) {
			ErrPrint("Failed to get pkgname for (%s)\n", appid);
			pkgmgrinfo_appinfo_destroy_appinfo(pkg_handle);
			goto out;
		}

		appid = strdup(new_appid);
		if (!appid) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}

		pkgmgrinfo_appinfo_destroy_appinfo(pkg_handle);
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

EAPI char *dynamicbox_service_provider_name(const char *dboxid)
{
	char *ret;
	int stage = 0;
	int seq = 0;
	int idx = 0;
	char *str = SAMSUNG_PREFIX;

	if (!dboxid) {
		return NULL;
	}

	while (str[idx] && dboxid[idx] && dboxid[idx] == str[idx]) {
		idx++;
		if (seq < 2 && dboxid[idx] == '.') {
			stage = idx;
			seq++;
		}
	}

	if (!str[idx] && dboxid[idx]) {
		/* Inhouse */
		return strdup(dboxid);
	} else if (seq < 2) {
		while (seq < 2) {
			if (dboxid[idx] == '.') {
				seq++;
			} else if (!dboxid[idx]) {
				ErrPrint("Invalid dboxid: %s\n", dboxid);
				return NULL;
			}

			idx++;
		}

		stage = idx;
	} else {
		stage++;
	}

	ret = strdup(dboxid + stage);
	if (!ret) {
		ErrPrint("Error: %s\n", strerror(errno));
		return NULL;
	}

	return ret;
}

EAPI int dynamicbox_service_is_enabled(const char *dboxid)
{
	return 1;
	/*
	ail_appinfo_h ai;
	char *pkgname;
	bool enabled;
	int ret;

	pkgname = dynamicbox_service_package_id(dboxid);
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

EAPI int dynamicbox_service_is_primary(const char *dboxid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret = 0;

	if (!dboxid) {
		return 0;
	}

	handle = open_db();
	if (!handle) {
		return 0;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT prime FROM pkgmap WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return 0;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
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

EAPI char *dynamicbox_service_category(const char *dboxid)
{
	sqlite3_stmt *stmt;
	char *category = NULL;
	char *tmp;
	sqlite3 *handle;
	int ret;

	if (!dboxid) {
		return NULL;
	}

	category = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT category FROM pkgmap WHERE pkgid = ? OR appid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, dboxid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, dboxid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Has no record?: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (!tmp || !strlen(tmp)) {
		ErrPrint("APPID is NIL\n");
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	category = strdup(tmp);
	if (!category) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return category;
}

EAPI char *dynamicbox_service_dbox_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *appid;
	char *dbox_src;

	if (!pkgid) {
		return NULL;
	}

	path = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.box_src FROM provider, pkgmap WHERE pkgmap.pkgid = ? AND provider.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s, pkgid(%s), ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s, pkgid(%s), ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s, pkgid(%s), ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s, pkgid(%s), ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Invalid appid : %s, pkgid(%s)\n", sqlite3_errmsg(handle), pkgid);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	dbox_src = (char *)sqlite3_column_text(stmt, 1);
	if (!dbox_src || !strlen(dbox_src)) {
		ErrPrint("No records for dbox src : %s, pkgid(%s), appid(%s)\n", sqlite3_errmsg(handle), pkgid, appid);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = strdup(dbox_src);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("DBOX Src: %s\n", path);
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return path;
}

EAPI char *dynamicbox_service_dbox_script_group(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *group;
	char *tmp;

	if (!pkgid) {
		return NULL;
	}

	group = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

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
		if (!group) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return group;
}

EAPI char *dynamicbox_service_gbar_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *gbar_src;
	const char *appid;

	if (!pkgid) {
		return NULL;
	}

	path = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.pd_src FROM provider, pkgmap WHERE provider.pkgid = ? AND pkgmap.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s pkgid(%s) ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s pkgid(%s) ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, SQLITE_TRANSIENT);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s pkgid(%s) ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		ErrPrint("Error: %s pkgid(%s) ret(%d)\n", sqlite3_errmsg(handle), pkgid, ret);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Error: %s pkgid(%s)\n", sqlite3_errmsg(handle), pkgid);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	gbar_src = (char *)sqlite3_column_text(stmt, 1);
	if (!gbar_src || !strlen(gbar_src)) {
		ErrPrint("Error: %s pkgid(%s) appid(%s)\n", sqlite3_errmsg(handle), pkgid, appid);
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = strdup(gbar_src);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("GBAR Src: %s\n", path);
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return path;
}

EAPI char *dynamicbox_service_gbar_script_group(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *group;
	char *tmp;

	if (!pkgid) {
		return NULL;
	}

	group = NULL;
	handle = open_db();
	if (!handle) {
		return NULL;
	}

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
		if (!group) {
			ErrPrint("Heap: %s\n", strerror(errno));
		}
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return group;
}

EAPI int dynamicbox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	const char *cluster;
	int cnt;
	int ret;

	if (!cb) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	cnt = 0;
	ret = sqlite3_prepare_v2(handle, "SELECT DISTINCT cluster FROM groupinfo", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		cnt = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		cluster = (const char *)sqlite3_column_text(stmt, 0);
		if (!cluster || !strlen(cluster)) {
			continue;
		}

		if (cb(cluster, data) < 0) {
			break;
		}

		cnt++;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return cnt;
}

EAPI int dynamicbox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	const char *category;
	int cnt;
	int ret;

	if (!cluster || !cb) {
		return DBOX_STATUS_ERROR_INVALID_PARAMETER;
	}

	handle = open_db();
	if (!handle) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(handle, "SELECT DISTINCT category FROM groupinfo WHERE cluster = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		cnt = DBOX_STATUS_ERROR_IO_ERROR;
		goto out;
	}

	cnt = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		category = (const char *)sqlite3_column_text(stmt, 0);
		if (!category || !strlen(category)) {
			continue;
		}

		if (cb(cluster, category, data) < 0) {
			break;
		}

		cnt++;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return cnt;
}

EAPI int dynamicbox_service_init(void)
{
	if (s_info.handle) {
		DbgPrint("Already initialized\n");
		s_info.init_count++;
		return DBOX_STATUS_ERROR_NONE;
	}

	s_info.handle = open_db();
	if (s_info.handle) {
		s_info.init_count++;
		return DBOX_STATUS_ERROR_NONE;
	}

	return DBOX_STATUS_ERROR_IO_ERROR;
}

EAPI int dynamicbox_service_fini(void)
{
	if (!s_info.handle || s_info.init_count <= 0) {
		ErrPrint("Service is not initialized\n");
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	s_info.init_count--;
	if (s_info.init_count > 0) {
		DbgPrint("Init count %d\n", s_info.init_count);
		return DBOX_STATUS_ERROR_NONE;
	}

	db_util_close(s_info.handle);
	s_info.handle = NULL;
	return DBOX_STATUS_ERROR_NONE;
}

EAPI int dynamicbox_service_get_size(int type, int *width, int *height)
{
	int _width;
	int _height;

	if (!width) {
		width = &_width;
	}

	if (!height) {
		height = &_height;
	}

	return convert_size_from_type(type, width, height);
}

EAPI int dynamicbox_service_size_type(int width, int height)
{
	int idx;

	if (util_update_resolution(&s_info, SIZE_LIST) < 0) {
		ErrPrint("Failed to update the size list\n");
	}

	for (idx = 0; idx < DBOX_NR_OF_SIZE_LIST; idx++) {
		if (SIZE_LIST[idx].w == width && SIZE_LIST[idx].h == height) {
			break;
		}
	}

	switch (idx) {
	case 0:
		return DBOX_SIZE_TYPE_1x1;
	case 1:
		return DBOX_SIZE_TYPE_2x1;
	case 2:
		return DBOX_SIZE_TYPE_2x2;
	case 3:
		return DBOX_SIZE_TYPE_4x1;
	case 4:
		return DBOX_SIZE_TYPE_4x2;
	case 5:
		return DBOX_SIZE_TYPE_4x3;
	case 6:
		return DBOX_SIZE_TYPE_4x4;
	case 7:
		return DBOX_SIZE_TYPE_4x5;
	case 8:
		return DBOX_SIZE_TYPE_4x6;
	case 9:
		return DBOX_SIZE_TYPE_EASY_1x1;
	case 10:
		return DBOX_SIZE_TYPE_EASY_3x1;
	case 11:
		return DBOX_SIZE_TYPE_EASY_3x3;
	case 12:
		return DBOX_SIZE_TYPE_0x0;
	default:
		break;
	}

	return DBOX_SIZE_TYPE_UNKNOWN;
}

/* End of a file */

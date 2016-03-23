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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

#include <glib.h>
#include <sqlite3.h>

#include <tzplatform_config.h>
#include <pkgmgr-info.h>
#include <system_info.h>
#include <dlog.h>

#include "widget_errno.h"
#include "debug.h"
#include "widget_conf.h"
#include "widget_instance.h"
#include "widget_service.h"

#define MAX_BUF_SIZE 4096

static inline bool _is_widget_feature_enabled(void)
{
	static bool feature = false;
	static bool retrieved = false;
	int ret;

	if (retrieved == true)
		return feature;

	ret = system_info_get_platform_bool(
			"http://tizen.org/feature/shell.appwidget", &feature);
	if (ret != SYSTEM_INFO_ERROR_NONE) {
		_E("failed to get system info");
		return false;
	}

	retrieved = true;

	return feature;
}

#define ROOT_USER 0
#define GLOBALAPP_USER tzplatform_getuid(TZ_SYS_GLOBALAPP_USER)
static int _is_global(uid_t uid)
{
	if (uid == ROOT_USER || uid == GLOBALAPP_USER)
		return 1;
	else
		return 0;
}

static const char *_get_db_path(uid_t uid)
{
	const char *path;

	if (!_is_global(uid))
		tzplatform_set_user(uid);

	path = tzplatform_mkpath(_is_global(uid) ?
			TZ_SYS_DB : TZ_USER_DB, ".widget.db");

	tzplatform_reset_user();

	return path;
}

static sqlite3 *_open_db(uid_t uid)
{
	int ret;
	const char *path;
	sqlite3 *db;

	path = _get_db_path(uid);
	if (path == NULL)
		return NULL;

	ret = sqlite3_open_v2(path, &db, SQLITE_OPEN_READONLY, NULL);
	if (ret != SQLITE_OK) {
		_E("failed to open widget db: %d", ret);
		return NULL;
	}

	return db;
}

static void _get_column_str(sqlite3_stmt *stmt, int idx, char **str)
{
	char *val;

	val = (char *)sqlite3_column_text(stmt, idx);
	if (val)
		*str = strdup(val);
	else
		*str = NULL;
}

static void _get_column_int(sqlite3_stmt *stmt, int idx, int *i)
{
	*i = sqlite3_column_int(stmt, idx);
}

/* TODO: get pixel size type from configuration file */
#define WIDGET_SIZE_TYPE_MAX 13
static int size_list[WIDGET_SIZE_TYPE_MAX][5] = {
	{ 1, 1, 175, 175, WIDGET_SIZE_TYPE_1x1 }, /*!< 1x1 */
	{ 2, 1, 354, 175, WIDGET_SIZE_TYPE_2x1 }, /*!< 2x1 */
	{ 2, 2, 354, 354, WIDGET_SIZE_TYPE_2x2 }, /*!< 2x2 */
	{ 4, 1, 712, 175, WIDGET_SIZE_TYPE_4x1 }, /*!< 4x1 */
	{ 4, 2, 712, 354, WIDGET_SIZE_TYPE_4x2 }, /*!< 4x2 */
	{ 4, 3, 712, 533, WIDGET_SIZE_TYPE_4x3 }, /*!< 4x3 */
	{ 4, 4, 712, 712, WIDGET_SIZE_TYPE_4x4 }, /*!< 4x4 */
	{ 4, 5, 712, 891, WIDGET_SIZE_TYPE_4x5 }, /*!< 4x5 */
	{ 4, 6, 712, 1070, WIDGET_SIZE_TYPE_4x6 }, /*!< 4x6 */
	{ 21, 21, 224, 215, WIDGET_SIZE_TYPE_EASY_1x1 }, /*!< 21x21 */
	{ 23, 21, 680, 215, WIDGET_SIZE_TYPE_EASY_3x1 }, /*!< 23x21 */
	{ 23, 23, 680, 653, WIDGET_SIZE_TYPE_EASY_3x3 }, /*!< 23x23 */
	{ 0, 0, 720, 1280, WIDGET_SIZE_TYPE_FULL }, /*!< 0x0 */
};

static int _get_supported_size_type(int width, int height, int *type)
{
	int i;

	for (i = 0; i < WIDGET_SIZE_TYPE_MAX; i++) {
		if (width == size_list[i][2] && height == size_list[i][3])
			break;
	}

	if (i == WIDGET_SIZE_TYPE_MAX) {
		_E("failed to get supported size");
		return -1;
	}

	*type = size_list[i][4];

	return 0;
}

static int _get_supported_size(int type, int *width, int *height)
{
	int i;

	for (i = 0; i < WIDGET_SIZE_TYPE_MAX; i++) {
		if (type == size_list[i][4])
			break;
	}

	if (i == WIDGET_SIZE_TYPE_MAX) {
		_E("failed to get supported size type");
		return -1;
	}

	*width = size_list[i][2];
	*height = size_list[i][3];

	return 0;
}

static int _convert_to_support_size(int *width, int *height, int count)
{
	int i;
	int j;

	for (j = 0; j < count; j++) {
		for (i = 0; i < WIDGET_SIZE_TYPE_MAX; i++) {
			if (size_list[i][0] == width[j] &&
					size_list[i][1] == height[j])
				break;
		}
		if (i == WIDGET_SIZE_TYPE_MAX) {
			_E("failed to get supported size type");
			return -1;
		}
		width[j] = size_list[i][2];
		height[j] = size_list[i][3];
	}

	return 0;
}

static int _convert_to_support_size_ratio(int width, int height, int *w, int *h)
{
	int i;

	for (i = 0; i < WIDGET_SIZE_TYPE_MAX; i++) {
		if (size_list[i][2] == width &&
				size_list[i][3] == height)
			break;
	}

	if (i == WIDGET_SIZE_TYPE_MAX) {
		_E("failed to get support size ratio");
		return -1;
	}

	*w = size_list[i][0];
	*h = size_list[i][1];

	return 0;
}

static int _get_widget_supported_sizes(const char *widget_id, uid_t uid,
		int *cnt, int **w, int **h)
{
	static const char query[] =
		"SELECT width, height FROM support_size WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int count = 0;
	int i;
	int *width;
	int *height;

	db = _open_db(uid);
	if (db == NULL)
		return WIDGET_ERROR_IO_ERROR;

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		return WIDGET_ERROR_FAULT;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	while (sqlite3_step(stmt) == SQLITE_ROW)
		count++;

	if (count == 0) {
		_E("cannot find supported sizes for widget %s", widget_id);
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		*cnt = 0;
		*w = NULL;
		*h = NULL;
		return WIDGET_ERROR_NOT_EXIST;
	}

	sqlite3_reset(stmt);
	width = malloc(sizeof(int) * count);
	height = malloc(sizeof(int) * count);

	for (i = 0; i < count; i++) {
		sqlite3_step(stmt);
		_get_column_int(stmt, 0, &width[i]);
		_get_column_int(stmt, 1, &height[i]);
	}

	if (_convert_to_support_size(width, height, count)) {
		_E("failed to convert size");
		free(width);
		free(height);
		return WIDGET_ERROR_FAULT;
	}
	*w = width;
	*h = height;
	*cnt = count;

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_change_period(const char *pkgname, const char *id, double period)
{
	widget_instance_h instance;
	int ret;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (!pkgname || !id || period < 0.0f) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	instance = widget_instance_get_instance(pkgname, id);

	if (!instance) {
		_E("instance not exists or out of bound(package)");
		return WIDGET_ERROR_PERMISSION_DENIED;
	}

	ret = widget_instance_change_period(instance, period);

	widget_instance_unref(instance);

	return ret;
}

EAPI int widget_service_trigger_update(const char *widget_id, const char *id, bundle *b, int force)
{
	widget_instance_h instance;
	int ret;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (!widget_id) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	instance = widget_instance_get_instance(widget_id, id);
	if (!instance) {
		_E("instance not exists or out of bound(package)");
		return WIDGET_ERROR_PERMISSION_DENIED;
	}

	ret = widget_instance_trigger_update(instance, b, force);

	widget_instance_unref(instance);

	return ret;
}

static int _get_widget_list(const char *pkgid, uid_t uid, GHashTable *list)
{
	static const char query[] =
		"SELECT classid, pkgid FROM widget_class";
	static const char query_where[] =
		" WHERE pkgid = ?";
	char query_buf[MAX_BUF_SIZE];
	int ret;
	int len;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *r_classid;
	char *r_pkgid;

	len = snprintf(query_buf, sizeof(query_buf), "%s", query);
	if (pkgid != NULL)
		strncat(query_buf, query_where, MAX_BUF_SIZE - len - 1);

	db = _open_db(uid);
	if (db == NULL)
		return WIDGET_ERROR_IO_ERROR;

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		return WIDGET_ERROR_FAULT;
	}

	if (pkgid != NULL)
		sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_STATIC);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		_get_column_str(stmt, 0, &r_classid);
		_get_column_str(stmt, 1, &r_pkgid);

		g_hash_table_insert(list, (gpointer)r_classid,
				(gpointer)r_pkgid);
	}

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_get_widget_list(widget_list_cb cb, void *data)
{
	int ret;
	GHashTable *list;
	GHashTableIter iter;
	char *classid;
	char *pkgid;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (cb == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	list = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
	if (list == NULL) {
		_E("out of memory");
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	ret = _get_widget_list(NULL, getuid(), list);
	if (ret == WIDGET_ERROR_NONE)
		ret = _get_widget_list(NULL, GLOBALAPP_USER, list);

	g_hash_table_iter_init(&iter, list);
	while (g_hash_table_iter_next(&iter, (gpointer *)&classid,
				(gpointer *)&pkgid)) {
		/* TODO: get 'is_prime' */
		if (cb(pkgid, classid, 0, data) != WIDGET_ERROR_NONE)
			break;
	}

	ret = g_hash_table_size(list);
	g_hash_table_destroy(list);

	return ret;
}

EAPI int widget_service_get_widget_list_by_pkgid(const char *pkgid,
		widget_list_by_pkgid_cb cb, void *data)
{
	int ret;
	GHashTable *list;
	GHashTableIter iter;
	char *classid;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (pkgid == NULL || cb == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	list = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
	if (list == NULL) {
		_E("out of memory");
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	ret = _get_widget_list(pkgid, getuid(), list);
	if (ret == WIDGET_ERROR_NONE)
		ret = _get_widget_list(pkgid, GLOBALAPP_USER, list);

	g_hash_table_iter_init(&iter, list);
	while (g_hash_table_iter_next(&iter, (gpointer *)&classid, NULL)) {
		/* TODO: get 'is_prime' */
		if (cb(classid, 0, data) != WIDGET_ERROR_NONE)
			break;
	}

	ret = g_hash_table_size(list);
	g_hash_table_destroy(list);

	return ret;
}

static char *_get_main_app_id(const char *widget_id, uid_t uid)
{
	static const char query[] =
		"SELECT pkgid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *pkgid;
	pkgmgrinfo_pkginfo_h pkginfo;
	char *mainappid;
	char *appid;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &pkgid);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	ret = pkgmgrinfo_pkginfo_get_usr_pkginfo(pkgid, getuid(), &pkginfo);
	free(pkgid);
	if (ret != PMINFO_R_OK) {
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	ret = pkgmgrinfo_pkginfo_get_mainappid(pkginfo, &mainappid);
	if (ret != PMINFO_R_OK) {
		_E("failed to get mainappid of widget %s", widget_id);
		set_last_result(WIDGET_ERROR_FAULT);
		pkgmgrinfo_pkginfo_destroy_pkginfo(pkginfo);
		return NULL;
	}

	appid = strdup(mainappid);
	pkgmgrinfo_pkginfo_destroy_pkginfo(pkginfo);

	set_last_result(WIDGET_ERROR_NONE);

	return appid;
}

EAPI char *widget_service_get_main_app_id(const char *widget_id)
{
	char *appid;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	appid = _get_main_app_id(widget_id, getuid());
	if (appid == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		appid = _get_main_app_id(widget_id, GLOBALAPP_USER);

	return appid;
}

EAPI int widget_service_get_supported_size_types(const char *widget_id,
		int *cnt, int **types)
{
	int ret;
	int i;
	int *width = NULL;
	int *height = NULL;
	int type;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (widget_id == NULL || cnt == NULL || types == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	ret = _get_widget_supported_sizes(widget_id, getuid(), cnt,
			&width, &height);
	if (ret == WIDGET_ERROR_NOT_EXIST)
		ret = _get_widget_supported_sizes(widget_id, GLOBALAPP_USER,
				cnt, &width, &height);

	if (ret == WIDGET_ERROR_NOT_EXIST) {
		*types = NULL;
		return WIDGET_ERROR_NONE;
	} else if (ret != WIDGET_ERROR_NONE) {
		return ret;
	}

	*types = malloc(sizeof(int) * (*cnt));
	if (*types == NULL) {
		_E("out of memory");
		free(width);
		free(height);
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	for (i = 0; i < (*cnt); i++) {
		if (_get_supported_size_type(width[i], height[i], &type)) {
			free(types);
			free(width);
			free(height);
			return WIDGET_ERROR_NOT_SUPPORTED;
		}

		*types[i] = type;
	}

	free(width);
	free(height);

	return WIDGET_ERROR_NONE;
}

static char *_get_app_id_of_setup_app(const char *widget_id, uid_t uid)
{
	static const char query[] =
		"SELECT setup_appid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *appid;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &appid);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	set_last_result(WIDGET_ERROR_NONE);

	return appid;

}

EAPI char *widget_service_get_app_id_of_setup_app(const char *widget_id)
{
	char *appid;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	appid = _get_app_id_of_setup_app(widget_id, getuid());
	if (appid == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		appid = _get_app_id_of_setup_app(widget_id, GLOBALAPP_USER);

	return appid;
}

static bool _get_nodisplay(const char *widget_id, uid_t uid)
{
	static const char query[] =
		"SELECT appid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *appid;
	pkgmgrinfo_appinfo_h appinfo;
	bool nodisplay;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return false;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return false;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return false;
	}

	_get_column_str(stmt, 0, &appid);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	ret = pkgmgrinfo_appinfo_get_usr_appinfo(appid, getuid(), &appinfo);
	free(appid);
	if (ret != PMINFO_R_OK) {
		set_last_result(WIDGET_ERROR_FAULT);
		return false;
	}

	ret = pkgmgrinfo_appinfo_is_nodisplay(appinfo, &nodisplay);
	pkgmgrinfo_appinfo_destroy_appinfo(appinfo);
	if (ret != PMINFO_R_OK) {
		_E("failed to get nodisplay of widget %s", widget_id);
		set_last_result(WIDGET_ERROR_FAULT);
		return false;
	}

	set_last_result(WIDGET_ERROR_NONE);

	return nodisplay;
}

EAPI int widget_service_get_nodisplay(const char *widget_id)
{
	bool nodisplay;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return 0;
	}

	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return 0;
	}

	nodisplay = _get_nodisplay(widget_id, getuid());
	if (get_last_result() == WIDGET_ERROR_NOT_EXIST)
		nodisplay = _get_nodisplay(widget_id, GLOBALAPP_USER);

	return (int)nodisplay;
}

/* deprecated, always return need_of_frame as false */
EAPI int widget_service_get_need_of_frame(const char *pkgid, widget_size_type_e size_type, bool *need_of_frame)
{
	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (!pkgid) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	*need_of_frame = false;

	return WIDGET_ERROR_NONE;
}

/* deprecated, always return need_of_touch_event as false */
EAPI int widget_service_get_need_of_touch_effect(const char *pkgid, widget_size_type_e size_type, bool *need_of_touch_event)
{
	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (!pkgid) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	*need_of_touch_event = false;

	return WIDGET_ERROR_NONE;
}

/* deprecated, always return need_of_mouse_event as false */
EAPI int widget_service_get_need_of_mouse_event(const char *pkgid, widget_size_type_e size_type, bool *need_of_mouse_event)
{
	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (!pkgid) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	*need_of_mouse_event = false;
	return WIDGET_ERROR_NONE;
}

static char *_get_preview_image_path(const char *widget_id, int width,
		int height, uid_t uid)
{
	static const char query[] =
		"SELECT preview FROM support_size WHERE "
		"classid=? AND width=? AND height=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *path;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, width);
	sqlite3_bind_int(stmt, 3, height);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &path);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	set_last_result(WIDGET_ERROR_NONE);

	return path;

}

EAPI char *widget_service_get_preview_image_path(const char *widget_id,
		widget_size_type_e size_type)
{
	char *path;
	int width = -1;
	int height = -1;
	int w = -1;
	int h = -1;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	/* TODO: validate size_type */
	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	if (_get_supported_size(size_type, &width, &height)) {
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (_convert_to_support_size_ratio(width, height, &w, &h)) {
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	path = _get_preview_image_path(widget_id, w, h, getuid());
	if (path == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		path = _get_preview_image_path(widget_id, w, h, GLOBALAPP_USER);

	return path;
}

static char *_get_icon(const char *widget_id, const char *lang, uid_t uid)
{
	static const char query[] =
		"SELECT icon FROM icon "
		"WHERE classid=? AND (locale=? OR locale IS NULL) "
		"ORDER BY locale DESC";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *icon;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
	if (lang)
		sqlite3_bind_text(stmt, 2, lang, -1, SQLITE_STATIC);
	else
		sqlite3_bind_null(stmt, 2);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		if (ret == SQLITE_DONE)
			_E("cannot find icon for widget %s", widget_id);
		else
			_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &icon);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	set_last_result(WIDGET_ERROR_NONE);

	return icon;

}

EAPI char *widget_service_get_icon(const char *widget_id, const char *lang)
{
	char *icon;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	icon = _get_icon(widget_id, lang, getuid());
	if (icon == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		icon = _get_icon(widget_id, lang, GLOBALAPP_USER);

	return icon;
}

static char *_get_name(const char *widget_id, const char *lang, uid_t uid)
{
	static const char query[] =
		"SELECT label FROM label "
		"WHERE classid=? AND (locale=? OR locale IS NULL) "
		"ORDER BY locale DESC";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *label;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
	if (lang)
		sqlite3_bind_text(stmt, 2, lang, -1, SQLITE_STATIC);
	else
		sqlite3_bind_null(stmt, 2);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		if (ret == SQLITE_DONE)
			_E("cannot find label for widget %s", widget_id);
		else
			_E("step error: %s", sqlite3_errmsg(db));
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &label);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	set_last_result(WIDGET_ERROR_NONE);

	return label;

}

EAPI char *widget_service_get_name(const char *widget_id, const char *lang)
{
	char *name;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}
	name = _get_name(widget_id, lang, getuid());
	if (name == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		name = _get_name(widget_id, lang, GLOBALAPP_USER);

	return name;
}

EAPI int widget_service_get_supported_sizes(const char *widget_id, int *cnt,
		int **w, int **h)
{
	int ret;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (widget_id == NULL || cnt == NULL || w == NULL || h == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	ret = _get_widget_supported_sizes(widget_id, getuid(), cnt, w, h);
	if (ret == WIDGET_ERROR_NOT_EXIST)
		ret = _get_widget_supported_sizes(widget_id, GLOBALAPP_USER,
				cnt, w, h);

	return ret;
}

static char *_get_widget_id(const char *appid, uid_t uid)
{
	static const char query[] =
		"SELECT classid FROM widget_class WHERE appid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *classid;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, appid, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &classid);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	set_last_result(WIDGET_ERROR_NONE);

	return classid;
}

EAPI char *widget_service_get_widget_id(const char *appid)
{
	char *classid;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (appid == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	classid = _get_widget_id(appid, getuid());
	if (classid == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		classid = _get_widget_id(appid, GLOBALAPP_USER);

	return classid;
}

static char *_get_package_id(const char *widget_id, uid_t uid)
{
	static const char query[] =
		"SELECT pkgid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *pkgid;

	db = _open_db(uid);
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		_E("prepare error: %s", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		_E("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return NULL;
	}

	_get_column_str(stmt, 0, &pkgid);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	set_last_result(WIDGET_ERROR_NONE);

	return pkgid;
}

EAPI char *widget_service_get_package_id(const char *widget_id)
{
	char *pkgid;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	if (widget_id == NULL) {
		_E("invalid parameter");
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	pkgid = _get_package_id(widget_id, getuid());
	if (pkgid == NULL && get_last_result() == WIDGET_ERROR_NOT_EXIST)
		pkgid = _get_package_id(widget_id, GLOBALAPP_USER);

	return pkgid;
}

EAPI int widget_service_get_size(widget_size_type_e type, int *width,
		int *height)
{
	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (width == NULL || height == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	if (_get_supported_size(type, width, height))
		return WIDGET_ERROR_INVALID_PARAMETER;

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_get_size_type(int width, int height,
		widget_size_type_e *size_type)
{
	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (width <= 0 || height <= 0 || size_type == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	if (_get_supported_size_type(width, height, (int *)size_type))
		return WIDGET_ERROR_INVALID_PARAMETER;

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_get_content_of_widget_instance(const char *widget_id, const char *widget_instance_id, bundle **b)
{
	widget_instance_h instance;
	bundle *kb = NULL;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (widget_id == NULL || widget_instance_id == NULL || b == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	instance = widget_instance_get_instance(widget_id, widget_instance_id);

	if (instance) {
		widget_instance_get_content(instance, &kb);
		if (kb) {
			*b = bundle_dup(kb);
			widget_instance_unref(instance);
			return WIDGET_ERROR_NONE;
		}
	}

	return WIDGET_ERROR_INVALID_PARAMETER;
}

struct instance_cb {
	widget_instance_list_cb cb;
	void *data;
};

static int __instance_list_cb(const char *widget_id, const char *instance_id, void *data)
{
	struct instance_cb *cb_data = (struct instance_cb *)data;

	if (cb_data && cb_data->cb)
		return cb_data->cb(widget_id, instance_id, cb_data->data);

	return -1;
}

EAPI int widget_service_get_widget_instance_list(const char *widget_id, widget_instance_list_cb cb, void *data)
{
	struct instance_cb cb_data;
	int ret = WIDGET_ERROR_NONE;

	cb_data.cb = cb;
	cb_data.data = data;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (widget_id == NULL || cb == NULL) {
		_E("inavlid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	ret = widget_instance_get_instance_list(widget_id, __instance_list_cb, &cb_data);

	return ret < 0 ? (ret == -2 ? WIDGET_ERROR_PERMISSION_DENIED : WIDGET_ERROR_NOT_EXIST) : WIDGET_ERROR_NONE;
}

EAPI int widget_service_set_lifecycle_event_cb(const char *widget_id, widget_lifecycle_event_cb cb, void *data)
{
	int ret = WIDGET_ERROR_NONE;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}

	if (cb == NULL) {
		_E("invalid parameter");
		return WIDGET_ERROR_INVALID_PARAMETER;
	}

	/* TODO */

	return ret;
}

EAPI int widget_service_unset_lifecycle_event_cb(const char *widget_id, void **user_data)
{
	int ret = WIDGET_ERROR_NONE;

	if (!_is_widget_feature_enabled()) {
		_E("not supported");
		return WIDGET_ERROR_NOT_SUPPORTED;
	}
	/* TODO */

	return ret;
}

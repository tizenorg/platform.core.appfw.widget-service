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

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <sqlite3.h>

#include <tzplatform_config.h>
#include <pkgmgr-info.h>

#include "widget_errno.h"
#include "debug.h"
#include "widget_conf.h"
#include "widget_service.h"

static const char *_get_db_path(uid_t uid)
{
	/* TODO: tizenglobalapp */
	return tzplatform_mkpath(TZ_USER_DB, ".widget.db");
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
	if (ret != SQLITE_OK)
		return NULL;

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
static int size_list[WIDGET_SIZE_TYPE_MAX][3] = {
	{ 175, 175, WIDGET_SIZE_TYPE_1x1 }, /*!< 1x1 */
	{ 354, 175, WIDGET_SIZE_TYPE_2x1 }, /*!< 2x1 */
	{ 354, 354, WIDGET_SIZE_TYPE_2x2 }, /*!< 2x2 */
	{ 712, 175, WIDGET_SIZE_TYPE_4x1 }, /*!< 4x1 */
	{ 712, 354, WIDGET_SIZE_TYPE_4x2 }, /*!< 4x2 */
	{ 712, 533, WIDGET_SIZE_TYPE_4x3 }, /*!< 4x3 */
	{ 712, 712, WIDGET_SIZE_TYPE_4x4 }, /*!< 4x4 */
	{ 712, 891, WIDGET_SIZE_TYPE_4x5 }, /*!< 4x5 */
	{ 712, 1070, WIDGET_SIZE_TYPE_4x6 }, /*!< 4x6 */
	{ 224, 215, WIDGET_SIZE_TYPE_EASY_1x1 }, /*!< 21x21 */
	{ 680, 215, WIDGET_SIZE_TYPE_EASY_3x1 }, /*!< 23x21 */
	{ 680, 653, WIDGET_SIZE_TYPE_EASY_3x3 }, /*!< 23x23 */
	{ 720, 1280, WIDGET_SIZE_TYPE_FULL }, /*!< 0x0 */
};

static int _get_supported_size_type(int width, int height, int *type)
{
	int i;

	for (i = 0; i < WIDGET_SIZE_TYPE_MAX; i++) {
		if (width == size_list[i][0] && height == size_list[i][1])
			break;
	}

	if (i == WIDGET_SIZE_TYPE_MAX)
		return -1;

	*type = size_list[i][2];

	return 0;
}

static int _get_supported_size(int type, int *width, int *height)
{
	int i;

	for (i = 0; i < WIDGET_SIZE_TYPE_MAX; i++) {
		if (type == size_list[i][2])
			break;
	}

	if (i == WIDGET_SIZE_TYPE_MAX)
		return -1;

	*width = size_list[i][0];
	*height = size_list[i][1];

	return 0;
}

static int _get_widget_supported_sizes(const char *widget_id, int *cnt,
		int **w, int **h)
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

	db = _open_db(getuid());
	if (db == NULL) {
		return WIDGET_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		return WIDGET_ERROR_FAULT;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	while (sqlite3_step(stmt) == SQLITE_ROW)
		count++;

	if (count == 0) {
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
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

	*cnt = count;
	*w = width;
	*h = height;

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_change_period(const char *pkgname, const char *id, double period)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_trigger_update(const char *widget_id, const char *id, bundle *b, int force)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_widget_list(widget_list_cb cb, void *data)
{
	static const char query[] =
		"SELECT classid, pkgid FROM widget_class";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *classid;
	char *pkgid;

	if (cb == NULL)
		return WIDGET_ERROR_INVALID_PARAMETER;

	db = _open_db(getuid());
	if (db == NULL) {
		return WIDGET_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		return WIDGET_ERROR_FAULT;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		_get_column_str(stmt, 0, &classid);
		_get_column_str(stmt, 0, &pkgid);
		/* TODO: get 'is_prime' */
		ret = cb(pkgid, classid, 0, data);
		free(classid);
		free(pkgid);
		if (ret != WIDGET_ERROR_NONE)
			break;
	}

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	return WIDGET_ERROR_NONE;
}

EAPI int widget_service_get_widget_list_by_pkgid(const char *pkgid,
		widget_list_by_pkgid_cb cb, void *data)
{
	static const char query[] =
		"SELECT classid FROM widget_class WHERE pkgid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *classid;

	if (pkgid == NULL || cb == NULL)
		return WIDGET_ERROR_INVALID_PARAMETER;

	db = _open_db(getuid());
	if (db == NULL) {
		return WIDGET_ERROR_IO_ERROR;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		return WIDGET_ERROR_FAULT;
	}

	sqlite3_bind_text(stmt, 1, pkgid, -1, SQLITE_STATIC);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		_get_column_str(stmt, 0, &classid);
		/* TODO: get 'is_prime' */
		ret = cb(classid, 0, data);
		free(classid);
		if (ret != WIDGET_ERROR_NONE)
			break;
	}

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	return WIDGET_ERROR_NONE;
}

EAPI char *widget_service_get_main_app_id(const char *widget_id)
{
	static const char query[] =
		"SELECT pkgid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *pkgid;
	pkgmgrinfo_appinfo_h appinfo;
	char *mainappid;
	char *appid;

	if (widget_id == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
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

	ret = pkgmgrinfo_pkginfo_get_pkginfo(pkgid, &appinfo);
	free(pkgid);
	if (ret != PMINFO_R_OK) {
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	ret = pkgmgrinfo_pkginfo_get_mainappid(appinfo, &mainappid);
	if (ret != PMINFO_R_OK) {
		set_last_result(WIDGET_ERROR_FAULT);
		pkgmgrinfo_appinfo_destroy_appinfo(appinfo);
		return NULL;
	}

	appid = strdup(mainappid);
	pkgmgrinfo_appinfo_destroy_appinfo(appinfo);

	set_last_result(WIDGET_ERROR_NONE);

	return appid;
}

EAPI int widget_service_get_supported_size_types(const char *widget_id,
		int *cnt, int **types)
{
	int ret;
	int i;
	int *width;
	int *height;
	int type;

	if (widget_id == NULL || cnt == NULL || types == NULL)
		return WIDGET_ERROR_INVALID_PARAMETER;

	ret = _get_widget_supported_sizes(widget_id, cnt, &width, &height);
	if (ret != WIDGET_ERROR_NONE)
		return ret;

	*types = malloc(sizeof(int) * (*cnt));
	if (*types == NULL)
		return WIDGET_ERROR_OUT_OF_MEMORY;

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

	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI char *widget_service_get_app_id_of_setup_app(const char *widget_id)
{
	static const char query[] =
		"SELECT setup_appid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *appid;

	if (widget_id == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
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

EAPI int widget_service_get_nodisplay(const char *widget_id)
{
	static const char query[] =
		"SELECT appid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *appid;
	pkgmgrinfo_appinfo_h appinfo;
	bool nodisplay;

	if (widget_id == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return 0;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return 0;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return 0;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
		sqlite3_finalize(stmt);
		sqlite3_close_v2(db);
		/* TODO: which error should be set? */
		set_last_result(ret == SQLITE_DONE ? WIDGET_ERROR_NOT_EXIST :
				WIDGET_ERROR_FAULT);
		return 0;
	}

	_get_column_str(stmt, 0, &appid);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	ret = pkgmgrinfo_appinfo_get_appinfo(appid, &appinfo);
	free(appid);
	if (ret != PMINFO_R_OK) {
		set_last_result(WIDGET_ERROR_FAULT);
		return 0;
	}

	ret = pkgmgrinfo_appinfo_is_nodisplay(appinfo, &nodisplay);
	pkgmgrinfo_appinfo_destroy_appinfo(appinfo);
	if (ret != PMINFO_R_OK) {
		set_last_result(WIDGET_ERROR_FAULT);
		return 0;
	}

	set_last_result(WIDGET_ERROR_NONE);

	return (int)nodisplay;
}

EAPI int widget_service_get_need_of_frame(const char *pkgid, widget_size_type_e size_type, bool *need_of_frame)
{
	/* deprecated */
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_need_of_touch_effect(const char *pkgid, widget_size_type_e size_type, bool *need_of_touch_event)
{
	/* deprecated */
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_need_of_mouse_event(const char *pkgid, widget_size_type_e size_type, bool *need_of_mouse_event)
{
	/* deprecated */
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI char *widget_service_get_preview_image_path(const char *widget_id,
		widget_size_type_e size_type)
{
	static const char query[] =
		"SELECT preview FROM support_size WHERE "
		"classid=? AND width=? AND height=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int width;
	int height;
	char *path;

	/* TODO: validate size_type */
	if (widget_id == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	if (_get_supported_size(size_type, &width, &height)) {
		set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, width);
	sqlite3_bind_int(stmt, 3, height);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
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

EAPI char *widget_service_get_icon(const char *widget_id, const char *lang)
{
	static const char query[] =
		"SELECT icon FROM icon WHERE classid=? AND locale=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *icon;

	if (widget_id == NULL || lang == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, lang, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_DONE) {
		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
		sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
		/* TODO: fix hardcoded locale */
		sqlite3_bind_text(stmt, 2, "No Locale", -1, SQLITE_STATIC);
		ret = sqlite3_step(stmt);
	}

	if (ret != SQLITE_ROW) {
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

EAPI char *widget_service_get_name(const char *widget_id, const char *lang)
{
	static const char query[] =
		"SELECT label FROM label WHERE classid=? AND locale=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *label;

	if (widget_id == NULL || lang == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, lang, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_DONE) {
		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
		sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);
		/* TODO: fix hardcoded locale */
		sqlite3_bind_text(stmt, 2, "No Locale", -1, SQLITE_STATIC);
		ret = sqlite3_step(stmt);
	}

	if (ret != SQLITE_ROW) {
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

EAPI int widget_service_get_supported_sizes(const char *widget_id, int *cnt,
		int **w, int **h)
{
	if (widget_id == NULL || cnt == NULL || w == NULL || h == NULL)
		return WIDGET_ERROR_INVALID_PARAMETER;

	return _get_widget_supported_sizes(widget_id, cnt, w, h);
}

EAPI char *widget_service_get_widget_id(const char *appid)
{
	static const char query[] =
		"SELECT classid FROM widget_class WHERE appid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *classid;

	if (appid == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, appid, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
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

EAPI char *widget_service_get_package_id(const char *widget_id)
{
	static const char query[] =
		"SELECT pkgid FROM widget_class WHERE classid=?";
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *pkgid;

	if (widget_id == NULL) {
		set_last_result(WIDGET_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	db = _open_db(getuid());
	if (db == NULL) {
		set_last_result(WIDGET_ERROR_IO_ERROR);
		return NULL;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		sqlite3_close_v2(db);
		set_last_result(WIDGET_ERROR_FAULT);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, widget_id, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_ROW) {
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

EAPI int widget_service_get_size(widget_size_type_e type, int *width,
		int *height)
{
	if (width == NULL || height == NULL)
		return WIDGET_ERROR_INVALID_PARAMETER;

	if (_get_supported_size(type, width, height))
		return WIDGET_ERROR_INVALID_PARAMETER;

	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_size_type(int width, int height,
		widget_size_type_e *size_type)
{
	if (width <= 0 || height <= 0 || size_type == NULL)
		return WIDGET_ERROR_INVALID_PARAMETER;

	if (_get_supported_size_type(width, height, (int *)size_type))
		return WIDGET_ERROR_INVALID_PARAMETER;

	return WIDGET_ERROR_NOT_SUPPORTED;
}

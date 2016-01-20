/*
 * Copyright 2015  Samsung Electronics Co., Ltd
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
#include <unistd.h>
#include <sys/types.h>

#include <glib.h>
#include <sqlite3.h>

#include <dlog.h>

#include "widget_plugin_parser_internal.h"

static int _bind_text(sqlite3_stmt *stmt, int idx, const char *text)
{
	if (text)
		return sqlite3_bind_text(stmt, idx, text, -1, SQLITE_STATIC);
	else
		return sqlite3_bind_null(stmt, idx);
}

static int _insert_support_size(sqlite3 *db, const char *classid, GList *sizes)
{
	int ret;
	static const char query[] =
		"INSERT INTO support_size "
		"(classid, preview, frame, width, height) "
		"VALUES (?, ?, ?, ?, ?)";
	GList *tmp;
	struct support_size *size;
	sqlite3_stmt *stmt = NULL;
	int idx;

	for (tmp = sizes; tmp; tmp = tmp->next) {
		size = (struct support_size *)tmp->data;
		ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_OK) {
			LOGE("prepare error: %s", sqlite3_errmsg(db));
			return -1;
		}

		idx = 1;
		_bind_text(stmt, idx++, classid);
		_bind_text(stmt, idx++, size->preview);
		sqlite3_bind_int(stmt, idx++, size->frame);
		sqlite3_bind_int(stmt, idx++, size->width);
		sqlite3_bind_int(stmt, idx++, size->height);

		ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE) {
			LOGE("step error: %s", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return -1;
		}

		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
	}

	if (stmt)
		sqlite3_finalize(stmt);

	return 0;
}

static int _insert_label(sqlite3 *db, const char *classid, GList *labels)
{
	int ret;
	static const char query[] =
		"INSERT INTO label (classid, locale, label) "
		"VALUES (?, ?, ?)";
	GList *tmp;
	struct label *label;
	sqlite3_stmt *stmt = NULL;
	int idx;

	for (tmp = labels; tmp; tmp = tmp->next) {
		label = (struct label *)tmp->data;
		ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_OK) {
			LOGE("prepare error: %s", sqlite3_errmsg(db));
			return -1;
		}

		idx = 1;
		_bind_text(stmt, idx++, classid);
		_bind_text(stmt, idx++,
				label->lang ? label->lang : "No Locale");
		_bind_text(stmt, idx++, label->label);

		ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE) {
			LOGE("step error: %s", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return -1;
		}

		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
	}

	if (stmt)
		sqlite3_finalize(stmt);

	return 0;
}

static int _insert_icon(sqlite3 *db, const char *classid, GList *icons)
{
	int ret;
	static const char query[] =
		"INSERT INTO icon (classid, locale, icon) "
		"VALUES (?, ?, ?)";
	GList *tmp;
	struct icon *icon;
	sqlite3_stmt *stmt = NULL;
	int idx;

	for (tmp = icons; tmp; tmp = tmp->next) {
		icon = (struct icon *)tmp->data;
		ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_OK) {
			LOGE("prepare error: %s", sqlite3_errmsg(db));
			return -1;
		}

		idx = 1;
		_bind_text(stmt, idx++, classid);
		_bind_text(stmt, idx++, icon->lang ? icon->lang : "No Locale");
		_bind_text(stmt, idx++, icon->icon);

		ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE) {
			LOGE("step error: %s", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return -1;
		}

		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
	}

	if (stmt)
		sqlite3_finalize(stmt);

	return 0;
}

static int _insert_widget_class(sqlite3 *db, const char *pkgid, GList *wcs)
{
	int ret;
	static const char query[] =
		"INSERT INTO widget_class (classid, update_period, "
		"setup_appid, appid, pkgid) "
		"VALUES (?, ?, ?, ?, ?)";
	GList *tmp;
	struct widget_class *wc;
	sqlite3_stmt *stmt = NULL;
	int idx;

	for (tmp = wcs; tmp; tmp = tmp->next) {
		wc = (struct widget_class *)tmp->data;
		ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_OK) {
			LOGE("prepare error: %s", sqlite3_errmsg(db));
			return -1;
		}

		idx = 1;
		_bind_text(stmt, idx++, wc->classid);
		sqlite3_bind_int(stmt, idx++, wc->update_period);
		_bind_text(stmt, idx++, wc->setup_appid);
		_bind_text(stmt, idx++, wc->appid);
		_bind_text(stmt, idx++, pkgid);

		ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE) {
			LOGE("step error: %s", sqlite3_errmsg(db));
			break;
		}

		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);

		if (_insert_support_size(db, wc->classid, wc->support_size))
			return -1;
		if (_insert_label(db, wc->classid, wc->label))
			return -1;
		if (_insert_icon(db, wc->classid, wc->icon))
			return -1;
	}

	if (stmt)
		sqlite3_finalize(stmt);

	return 0;
}

int widget_parser_db_insert_widget_class(const char *pkgid, GList *widget_list)
{
	int ret;
	sqlite3 *db;

	db = _open_db(getuid(), false);
	if (db == NULL)
		return -1;

	if (sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL)) {
		LOGE("begin transaction error");
		sqlite3_close_v2(db);
		return -1;
	}

	ret = _insert_widget_class(db, pkgid, widget_list);
	if (ret) {
		LOGE("failed to insert widget class data");
		sqlite3_close_v2(db);
		return -1;
	}

	if (sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL)) {
		LOGE("begin transaction error");
		sqlite3_close_v2(db);
		return -1;
	}

	_close_db(db);

	return 0;
}

static int _remove_widget_class(sqlite3 *db, const char *pkgid)
{
	int ret;
	static const char query[] =
		"DELETE FROM widget_class WHERE pkgid=?";
	sqlite3_stmt *stmt = NULL;

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		LOGE("prepare error: %s", sqlite3_errmsg(db));
		return -1;
	}

	_bind_text(stmt, 1, pkgid);

	ret = sqlite3_step(stmt);
	if (ret != SQLITE_DONE) {
		LOGE("step error: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);

	return 0;
}

int widget_parser_db_remove_widget_class(const char *pkgid)
{
	int ret;
	sqlite3 *db;

	db = _open_db(getuid(), false);
	if (db == NULL)
		return -1;

	if (sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL)) {
		LOGE("begin transaction error");
		sqlite3_close_v2(db);
		return -1;
	}

	ret = _remove_widget_class(db, pkgid);
	if (ret) {
		LOGE("failed to remove widget class data");
		sqlite3_close_v2(db);
		return -1;
	}

	if (sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL)) {
		LOGE("begin transaction error");
		sqlite3_close_v2(db);
		return -1;
	}

	_close_db(db);

	return 0;
}

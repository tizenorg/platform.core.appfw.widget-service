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

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <glib.h>
#include <sqlite3.h>

#include <dlog.h>
#include <tzplatform_config.h>

#include "widget_plugin_parser_internal.h"
#include "widget_plugin_parser_db_query.h"

void _free_support_size(gpointer data)
{
	struct support_size *size = (struct support_size *)data;

	if (size == NULL)
		return;

	free(size->preview);
	free(size);
}

void _free_label(gpointer data)
{
	struct label *label = (struct label *)data;

	if (label == NULL)
		return;

	free(label->label);
	free(label->lang);
	free(label);
}

void _free_icon(gpointer data)
{
	struct icon *icon = (struct icon *)data;

	if (icon == NULL)
		return;

	free(icon->icon);
	free(icon->lang);
	free(icon);
}

void _free_widget_class(gpointer data)
{
	struct widget_class *wc = (struct widget_class *)data;

	if (wc == NULL)
		return;

	free(wc->classid);
	free(wc->setup_appid);
	free(wc->appid);

	g_list_free_full(wc->support_size, _free_support_size);
	g_list_free_full(wc->label, _free_label);
	g_list_free_full(wc->icon, _free_icon);

	free(wc);
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

static char *_get_db_path(uid_t uid)
{
	const char *path;

	if (!_is_global(uid))
		tzplatform_set_user(uid);

	path = tzplatform_mkpath(_is_global(uid) ?
			TZ_SYS_DB : TZ_USER_DB, ".widget.db");

	tzplatform_reset_user();

	return strdup(path);
}

static int _initialize_db(const char *path)
{
	int ret;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	const char *start;
	const char *tail;
	size_t len;
	int result = 0;

	ret = sqlite3_open_v2(path, &db,
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (ret != SQLITE_OK) {
		LOGE("open db(%s) error: %d", path, ret);
		return -1;
	}

	len = strlen(query_initialize_db);
	start = query_initialize_db;
	do {
		ret = sqlite3_prepare_v2(db, start, strlen(start), &stmt,
				&tail);
		if (ret != SQLITE_OK) {
			LOGE("prepare failed: %s", sqlite3_errmsg(db));
			result = -1;
			break;
		}

		ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE && ret != SQLITE_ROW) {
			LOGE("step failed: %s", sqlite3_errmsg(db));
			result = -1;
			break;
		}

		sqlite3_reset(stmt);
		start = tail;
	} while (start - query_initialize_db < len);

	sqlite3_finalize(stmt);
	sqlite3_close_v2(db);

	return result;
}

sqlite3 *_open_db(uid_t uid, bool readonly)
{
	int ret;
	sqlite3 *db;
	char *path;

	path = _get_db_path(uid);

	if (access(path, F_OK) == -1) {
		LOGD("db(%s) does not exist, create one", path);
		if (_initialize_db(path)) {
			LOGE("create db failed");
			return NULL;
		}
	}

	ret = sqlite3_open_v2(path, &db,
			readonly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE,
			NULL);
	if (ret != SQLITE_OK) {
		LOGE("open db(%s) error: %d", path, ret);
		free(path);
		return NULL;
	}

	free(path);

	return db;
}

void _close_db(sqlite3 *db)
{
	sqlite3_close_v2(db);
}

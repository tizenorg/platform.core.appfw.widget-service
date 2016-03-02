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

#include "widget_instance.h"
#include <dlog.h>
#include "debug.h"
#include <uuid/uuid.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glib.h>
#include <sqlite3.h>
#include <aul.h>
#include <aul_svc.h>
#include <aul_app_com.h>
#include <widget_service.h>
#include <app.h>

#define USER_UID_MIN 5000
#define MAX_INSTANCE_ID_LEN 256
#define WIDGET_CLASS_DELIMITER "@"
#define QUERY_MAX_LEN 8192

#ifndef TIZEN_PATH_MAX
#define TIZEN_PATH_MAX 1024
#endif

enum {
	WIDGET_INSTANCE_CREATED,
	WIDGET_INSTANCE_RUNNING,
	WIDGET_INSTANCE_TERMINATED,
	WIDGET_INSTANCE_DELETED
};

struct _widget_instance {
	char *id;
	pid_t pid;
	char *widget_id;
	int w;
	int h;
	double period;
	bundle *content_info;
	int status;
	int stored;
	int ref;
};

struct widget_app {
	char *viewer_id;
	char *widget_id;
	GList *instances;
};

struct widget_status {
	char *widget_id;
	char *instance_id;
	int event;
};

static char *wayland_display = NULL;
static char *xdg_runtime_dir = NULL;

static GList *_widget_instances = NULL;
static GList *_widget_apps = NULL;

static sqlite3 *_widget_db = NULL;
static char *viewer_appid = NULL;
static aul_app_com_connection_h conn = NULL;
static aul_app_com_connection_h status_conn = NULL;


#define QUERY_CREATE_TABLE_WIDGET \
	"create table if not exists widget_instance" \
	"(widget_id text, " \
	"viewer_id text, " \
	"content_info text, " \
	"instance_id text, " \
	"PRIMARY KEY(instance_id)) "

static int __init(bool readonly)
{
	int rc;
	char db_path[TIZEN_PATH_MAX];
	char *app_path = NULL;

	if (_widget_db) {
		_D("already initialized");
		return 0;
	}

	app_path = app_get_data_path();
	if (!app_path) {
		_D("failed to get app path");
		return 0;
	}

	snprintf(db_path, sizeof(db_path), "%s/%s", app_path, ".widget_instance.db");

	rc = sqlite3_open_v2(db_path, &_widget_db,
			readonly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (rc != SQLITE_OK) {
		_E("Can't open database: %d, %s, extended: %d", rc, sqlite3_errmsg(_widget_db),
				sqlite3_extended_errcode(_widget_db));
		goto err;
	}

	rc = sqlite3_exec(_widget_db, "PRAGMA journal_mode = PERSIST", NULL, NULL, NULL);
	if (rc != SQLITE_OK) {
		_E("Fail to change journal mode\n");
		goto err;
	}

	rc = sqlite3_exec(_widget_db, QUERY_CREATE_TABLE_WIDGET, NULL, NULL, NULL);
	if (rc != SQLITE_OK) {
		_E("Fail to create tables\n");
		goto err;
	}

	return 0;
err:
	if (_widget_db) {
		sqlite3_close(_widget_db);
		_widget_db = NULL;
	}

	return -1;
}

static int __fini(void)
{
	if (_widget_db) {
		sqlite3_close(_widget_db);
		_widget_db = NULL;
	}

	return 0;
}

static struct _widget_instance *__pick_instance(const char *widget_id, const char *instance_id)
{
	GList *apps = _widget_apps;
	GList *instances = NULL;
	struct widget_app *app = NULL;
	struct _widget_instance *instance = NULL;

	while (apps) {
		app = apps->data;
		if (app && g_strcmp0(widget_id, app->widget_id) == 0) {
			instances = app->instances;
			while (instances) {
				instance = instances->data;
				if (instance && g_strcmp0(instance_id, instance->id) == 0)
					return instance;
				instances = instances->next;
			}
		}
		apps = apps->next;
	}

	return NULL;
}

static struct widget_app *__pick_app(const char *widget_id)
{
	GList *apps = _widget_apps;
	struct widget_app *app = NULL;

	while (apps) {
		app = apps->data;
		if (app && app->widget_id && g_strcmp0(widget_id, app->widget_id) == 0)
			return app;

		apps = apps->next;
	}

	return NULL;
}

static struct widget_app *__add_app(const char *widget_id, const char *viewer_id)
{
	struct widget_app *app = NULL;

	app = (struct widget_app *)malloc(sizeof(struct widget_app));
	if (app == NULL) {
		_E("out of memory");
		return NULL;
	}

	app->viewer_id = g_strdup(viewer_id);
	app->widget_id = g_strdup(widget_id);
	app->instances = NULL;

	_widget_apps = g_list_append(_widget_apps, app);

	return app;
}

static const char *__create_instance_id(const char *widget_id)
{
	char uuid[37];
	char instance_id[MAX_INSTANCE_ID_LEN];
	uuid_t u;
	struct widget_app *app = NULL;

	if (widget_id == NULL)
		return NULL;

	app = __pick_app(widget_id);

	if (app == NULL) {
		app = __add_app(widget_id, viewer_appid);
		if (app == NULL)
			return NULL;
	}

	uuid_generate(u);
	uuid_unparse(u, uuid);

	snprintf(instance_id, MAX_INSTANCE_ID_LEN, "%s:%s", uuid, widget_id);

	_D("new instance: %s", instance_id);

	return g_strdup(instance_id);
}

static struct _widget_instance *__add_instance(const char *id, const char *widget_id)
{
	struct _widget_instance *instance = NULL;
	struct widget_app *app = NULL;

	instance = (struct _widget_instance *)malloc(sizeof(struct _widget_instance));
	if (instance == NULL) {
		_E("out of memory");
		return NULL;
	}

	instance->status = WIDGET_INSTANCE_CREATED;
	instance->id = (char *)id;
	instance->pid = 0;
	instance->stored = 0;
	instance->widget_id = g_strdup(widget_id);
	instance->content_info = NULL;
	instance->ref = 0;

	_widget_instances = g_list_append(_widget_instances, instance);

	app = __pick_app(widget_id);
	if (app)
		app->instances = g_list_append(app->instances, instance);
	else
		_E("failed to find app: %s", widget_id);

	return instance;
}

static void __remove_instance(struct _widget_instance *instance)
{
	struct widget_app *app = NULL;

	if (instance == NULL) {
		_E("invalid argument");
		return;
	}

	if (instance->widget_id)
		app = __pick_app(instance->widget_id);

	if (app) {
		app->instances = g_list_remove(app->instances, instance);
		if (app->instances == NULL) {
			_widget_apps = g_list_remove(_widget_apps, app);
			free(app);
			app = NULL;
		}
	}

	_widget_instances = g_list_remove(_widget_instances, instance);

	if (instance->widget_id) {
		free(instance->widget_id);
		instance->widget_id = NULL;
	}

	if (instance->id) {
		free(instance->id);
		instance->id = NULL;
	}

	if (instance->content_info) {
		bundle_free(instance->content_info);
		instance->content_info = NULL;
	}

	free(instance);
}

static int __load_instance_list()
{
	int rc;
	char select_query[] = "SELECT widget_id, content_info, instance_id FROM widget_instance WHERE viewer_id=?";
	char *widget_id;
	char *content_info;
	char *instance_id;
	struct _widget_instance *instance = NULL;
	struct widget_app *app = NULL;

	sqlite3_stmt* p_statement;

	if (_widget_db == NULL) {
		_E("widget db is not initialized");
		return -1;
	}

	rc = sqlite3_prepare_v2(_widget_db, select_query, strlen(select_query),
			&p_statement, NULL);

	if (rc != SQLITE_OK) {
		_E("Sqlite3 error [%d] : <%s> executing statement\n", rc,
				sqlite3_errmsg(_widget_db));
		return -1;
	}

	sqlite3_bind_text(p_statement, 1, viewer_appid, -1, SQLITE_TRANSIENT);

	while (sqlite3_step(p_statement) == SQLITE_ROW) {
		widget_id = (char *)sqlite3_column_text(p_statement, 0);
		content_info = (char *)sqlite3_column_text(p_statement, 1);
		instance_id = (char *)sqlite3_column_text(p_statement, 2);

		app = __pick_app(widget_id);
		if (app == NULL) {
			app = __add_app(widget_id, viewer_appid);
			if (app == NULL) {
				_E("failed to add app: %s", widget_id);
				continue;
			}
		}

		instance = __add_instance(instance_id, widget_id);
		if (instance == NULL) {
			_E("failed to add instance: %s", instance_id);
			continue;
		}

		instance->content_info = bundle_decode((const bundle_raw *)content_info, strlen(content_info));
		if (instance->content_info == NULL)
			_E("failed to decode bundle: %s", instance_id);
		instance->status = WIDGET_INSTANCE_CREATED;
		instance->stored = 1;
	}

	rc = sqlite3_finalize(p_statement);
	if (rc != SQLITE_OK) {
		_E("Sqlite3 error [%d] : <%s> finalizing statement\n", rc,
				sqlite3_errmsg(_widget_db));
	}

	return 0;
}

static int __update_instance_info(struct _widget_instance *instance)
{
	int rc = 0;
	const char insert_query[] = "INSERT INTO widget_instance(widget_id, viewer_id, content_info, instance_id) VALUES(?,?,?,?)";
	const char update_query[] = "UPDATE widget_instance SET content_info=? WHERE instance_id=?";
	const char delete_query[] = "DELETE FROM widget_instance WHERE instance_id=?";
	sqlite3_stmt* p_statement;
	struct widget_app  *app = NULL;
	char *content = NULL;
	int content_len = 0;

	if (_widget_db == NULL) {
		_E("call widget_instance_init() first");
		return -1;
	}

	if (instance == NULL) {
		_E("wrong argument");
		return -1;
	}

	if (instance->content_info) {
		rc = bundle_encode(instance->content_info, (bundle_raw **)&content, &content_len);
		if (rc != BUNDLE_ERROR_NONE)
			_E("failed to get bundle data: %s", instance->id);
	}

	if (content == NULL) {
		content = g_strdup("NULL");
		content_len = strlen("NULL");
	}

	if (instance->stored) {
		if (instance->status == WIDGET_INSTANCE_DELETED) {
			rc = sqlite3_prepare_v2(_widget_db, delete_query, strlen(delete_query),
					&p_statement, NULL);
			if (rc != SQLITE_OK) {
				_E("Sqlite3 error [%d] : <%s> executing statement\n", rc,
						sqlite3_errmsg(_widget_db));
				goto cleanup;
			}

			sqlite3_bind_text(p_statement, 1, instance->id, -1, SQLITE_TRANSIENT);
		} else {
			rc = sqlite3_prepare_v2(_widget_db, update_query, strlen(update_query),
					&p_statement, NULL);

			if (rc != SQLITE_OK) {
				_E("Sqlite3 error [%d] : <%s> executing statement\n", rc,
						sqlite3_errmsg(_widget_db));
				goto cleanup;
			}

			sqlite3_bind_text(p_statement, 1, content, -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(p_statement, 2, instance->id, -1, SQLITE_TRANSIENT);
		}
	} else {
		app = __pick_app(instance->widget_id);
		if (app == NULL) {
			_E("can not find app: %s", instance->id);
			goto cleanup;
		}

		rc = sqlite3_prepare_v2(_widget_db, insert_query, strlen(insert_query),
				&p_statement, NULL);

		if (rc != SQLITE_OK) {
			_E("Sqlite3 error [%d] : <%s> executing statement\n", rc,
					sqlite3_errmsg(_widget_db));
			goto cleanup;
		}

		sqlite3_bind_text(p_statement, 1, instance->widget_id, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(p_statement, 2, app->viewer_id, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(p_statement, 3, content, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(p_statement, 4, instance->id, -1, SQLITE_TRANSIENT);
	}

	rc = sqlite3_step(p_statement);

	if (rc == SQLITE_DONE) {
		if (instance->status == WIDGET_INSTANCE_DELETED)
			widget_instance_unref(instance);
		else
			instance->stored = 1;
	}

	if (rc != SQLITE_DONE) {
		_E("Sqlite3 error [%d] : <%s> executing statement\n", rc,
				sqlite3_errmsg(_widget_db));
	}

cleanup:

	if (p_statement) {
		rc = sqlite3_finalize(p_statement);
		if (rc != SQLITE_OK) {
			_E("Sqlite3 error [%d] : <%s> finalizing statement\n", rc,
					sqlite3_errmsg(_widget_db));
		}
	}

	if (content) {
		free(content);
		content = NULL;
	}

	return rc;
}

EAPI int widget_instance_create(const char *widget_id, char **instance_id)
{
	const char *id = NULL;
	struct _widget_instance *instance = NULL;

	_D("create: %s", widget_id);

	id = __create_instance_id(widget_id);
	if (!id) {
		_E("failed to get instance id for %s", widget_id);
		return -1;
	}

	instance = __add_instance(id, widget_id);

	if (instance) {
		*instance_id = (char *)id;
		_D("create done");
		return 0;
	}

	*instance_id = NULL;

	return -1;
}

static int __send_aul_cmd(const char *widget_id, const char *instance_id, bundle *extra)
{
	int ret = 0;
	const char *appid;
	const char *classid = widget_id;
	bundle *b = extra;

	appid = g_strstr_len(widget_id, strlen(widget_id), WIDGET_CLASS_DELIMITER) + 1;
	if (appid != (const char *)1) { /* move pointer next to delimiter */
		if (appid > widget_id + (sizeof(char) * strlen(widget_id)))
			return -1; /* bad ptr */
	} else {
		appid = widget_id;
	}

	if (b == NULL)
		b = bundle_create();

	bundle_add_str(b, WIDGET_K_INSTANCE, instance_id);
	bundle_add_str(b, WIDGET_K_CLASS, classid);

	aul_svc_set_loader_id(b, 1);

	ret = aul_launch_app(appid, b);

	if (!extra) {
		bundle_free(b);
		b = NULL;
	}

	return ret;
}

static int __set_width(bundle *content_info, int w)
{
	char wbuf[6];
	if (content_info == NULL || w < 0)
		return -1;

	snprintf(wbuf, 6, "%d", w);
	bundle_add_str(content_info, WIDGET_K_WIDTH, wbuf);

	return 0;
}

static int __set_height(bundle *content_info, int h)
{
	char hbuf[6];
	if (content_info == NULL || h < 0)
		return -1;

	snprintf(hbuf, 6, "%d", h);
	bundle_add_str(content_info, WIDGET_K_HEIGHT, hbuf);

	return 0;
}

EAPI int widget_instance_launch(const char *widget_id, const char *instance_id, bundle *content_info, int w, int h)
{
	int ret = 0;
	char pid_buf[6];
	bundle *b = content_info;
	char *instance = (char *)instance_id;

	_D("launch: %s %s", widget_id, instance_id);

	if (widget_id == NULL) {
		_E("wrong arguments");
		return -1;
	}

	if (xdg_runtime_dir == NULL) {
		xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
		if (xdg_runtime_dir == NULL) {
			_E("failed to get XDG_RUNTIME_DIR");
			return -1;
		}
	}

	if (wayland_display == NULL) {
		wayland_display = getenv("WAYLAND_DISPLAY");
		if (wayland_display == NULL) {
			_E("unable to get wayland display port");
			return -1;
		}
	}

	if (instance == NULL) {
		ret = widget_instance_create(widget_id, &instance);
		if (ret < 0 || instance == NULL) {
			_E("failed to create instance for %s", widget_id);
			return -1;
		}
	}

	if (b == NULL) {
		b = bundle_create();
		if (b == NULL) {
			_E("out of memory");
			return -1;
		}
	}

	snprintf(pid_buf, sizeof(pid_buf), "%d", getpid());
	bundle_add_str(b, WIDGET_K_CALLER, pid_buf);
	bundle_add_str(b, AUL_K_WAYLAND_DISPLAY, wayland_display);
	bundle_add_str(b, AUL_K_WAYLAND_WORKING_DIR, xdg_runtime_dir);
	bundle_add_str(b, WIDGET_K_OPERATION, "create");

	__set_width(b, w);
	__set_height(b, h);

	ret = __send_aul_cmd(widget_id, instance, b);

	if (ret) {
		struct _widget_instance *i = __pick_instance(widget_id, instance);
		if (i)
			i->pid = ret;
	}

	if (content_info == NULL)
		bundle_free(b);

	return ret;
}

EAPI int widget_instance_terminate(const char *widget_id, const char *instance_id)
{
	int ret = 0;
	bundle *b = bundle_create();

	if (widget_id == NULL || instance_id == NULL || b == NULL)
		return -1;

	bundle_add_str(b, WIDGET_K_OPERATION, "terminate");

	ret = __send_aul_cmd(widget_id, instance_id, b);

	bundle_free(b);

	return ret;
}

EAPI int widget_instance_destroy(const char *widget_id, const char *instance_id)
{
	int ret = 0;
	bundle *b = bundle_create();

	if (widget_id == NULL || instance_id == NULL || b == NULL)
		return -1;

	bundle_add_str(b, WIDGET_K_OPERATION, "destroy");

	ret = __send_aul_cmd(widget_id, instance_id, b);

	bundle_free(b);

	return ret;
}

EAPI int widget_instance_foreach(const char *widget_id, widget_instance_foreach_cb cb, void *data)
{
	GList *apps = _widget_apps;
	GList *instances = NULL;
	struct widget_app *app = NULL;
	struct _widget_instance *instance = NULL;

	if (widget_id == NULL || cb == NULL)
		return -1;

	while (apps) {
		app = apps->data;
		if (app && g_strcmp0(app->widget_id, widget_id) == 0) {
			instances = app->instances;
			while (instances) {
				instance = instances->data;
				if (instance && cb(instance, data) < 0)
					break;

				instances = instances->next;
			}
		}
		apps = apps->next;
	}

	return 0;
}

static gboolean __send_lifecycle_event_cb(gpointer user_data)
{
	bundle *b = (bundle *)user_data;

	aul_app_com_send("widget://status", b);

	bundle_free(b);

	return FALSE;
}

static void __send_lifecycle_event(char *widget_id, char *instance_id, int event)
{
	char buf[8];
	bundle *b = bundle_create();

	snprintf(buf, sizeof(buf), "%d", event);
	bundle_add_str(b, WIDGET_K_CLASS, widget_id);
	bundle_add_str(b, WIDGET_K_INSTANCE, instance_id);
	bundle_add_str(b, "LIFECYCLE_EVENT", buf);

	g_idle_add(__send_lifecycle_event_cb, b);
}

static int __widget_handler(const char *viewer_id, aul_app_com_result_e e, bundle *envelope, void *user_data)
{
	char *widget_id = NULL;
	char *instance_id = NULL;
	int *status = NULL;
	size_t status_sz = 0;
	int cmd = 0;
	struct _widget_instance *instance;

	bundle_get_str(envelope, WIDGET_K_CLASS, &widget_id);
	bundle_get_str(envelope, WIDGET_K_INSTANCE, &instance_id);
	bundle_get_byte(envelope, WIDGET_K_STATUS, (void **)&status, &status_sz);

	if (widget_id == NULL || instance_id == NULL || status == NULL) {
		_E("undefined class or instance %s of %s", instance_id, widget_id);
		if (status != NULL)
			_E("cmd: %d", *status);

		return 0;
	}

	instance = __pick_instance(widget_id, instance_id);

	if (instance == NULL) {
		_E("undefined instance id: %s of %s", instance_id, widget_id);
		return 0;
	}

	cmd = *status;

	switch (cmd) {
	case WIDGET_INSTANCE_EVENT_CREATE:
		if (instance->content_info)
			bundle_free(instance->content_info);

		instance->content_info = bundle_dup(envelope);
		__update_instance_info(instance);
		__send_lifecycle_event(widget_id, instance_id, WIDGET_LIFE_CYCLE_EVENT_CREATE);
		break;
	case WIDGET_INSTANCE_EVENT_TERMINATE:
		if (instance->content_info)
			bundle_free(instance->content_info);

		instance->content_info = bundle_dup(envelope);
		instance->status = WIDGET_INSTANCE_TERMINATED;
		__update_instance_info(instance);
		break;
	case WIDGET_INSTANCE_EVENT_DESTROY:
		instance->status = WIDGET_INSTANCE_DELETED;
		__update_instance_info(instance);
		__send_lifecycle_event(widget_id, instance_id, WIDGET_LIFE_CYCLE_EVENT_DESTROY);
		break;
	case WIDGET_INSTANCE_EVENT_PAUSE:
		__send_lifecycle_event(widget_id, instance_id, WIDGET_LIFE_CYCLE_EVENT_PAUSE);
		break;
	case WIDGET_INSTANCE_EVENT_RESUME:
		__send_lifecycle_event(widget_id, instance_id, WIDGET_LIFE_CYCLE_EVENT_RESUME);
		instance->status = WIDGET_INSTANCE_RUNNING;
		break;
	case WIDGET_INSTANCE_EVENT_UPDATE:
		if (instance->content_info)
			bundle_free(instance->content_info);

		instance->content_info = bundle_dup(envelope);
		__update_instance_info(instance);
		break;
	default:
		_E("unknown command: %d", cmd);
		break;
	}

	return 0;
}

static int __status_handler(const char *viewer_id, aul_app_com_result_e e, bundle *envelope, void *user_data)
{
	return 0;
}

EAPI int widget_instance_init(const char *viewer_id)
{
	if (viewer_id == NULL)
		return -1;

	viewer_appid = g_strdup(viewer_id);
	__init(false);

	__load_instance_list();

	if (aul_app_com_create(viewer_id, NULL, __widget_handler, NULL, &conn) < 0) {
		_E("failed to create app com endpoint");
		return -1;
	}

	if (aul_app_com_create("widget://status", NULL, __status_handler, NULL, &status_conn) < 0) {
		_E("failed to create app com endpoint");
		return -1;
	}

	return 0;
}

EAPI int widget_instance_fini()
{

	if (conn) {
		if (aul_app_com_leave(conn) < 0)
			_E("failed to leave app com endpoint");
	}

	if (status_conn) {
		if (aul_app_com_leave(status_conn) < 0)
			_E("failed to leave app com endpoint");
	}

	__fini();

	if (viewer_appid) {
		g_free(viewer_appid);
		viewer_appid = NULL;
	}

	return 0;
}

EAPI int widget_instance_get_id(widget_instance_h instance, char **id)
{
	if (instance == NULL || id == NULL)
		return -1;

	*id = (char *)instance->id;
	return 0;
}

EAPI int widget_instance_get_content(widget_instance_h instance, bundle **content)
{
	if (instance == NULL || content == NULL)
		return -1;

	*content = instance->content_info;
	return 0;
}

EAPI int widget_instance_get_width(widget_instance_h instance, int *w)
{
	if (instance == NULL || w == NULL)
		return -1;

	*w = instance->w;
	return 0;
}

EAPI int widget_instance_get_height(widget_instance_h instance, int *h)
{
	if (instance == NULL || h == NULL)
		return -1;

	*h = instance->h;
	return 0;
}

EAPI int widget_instance_get_period(widget_instance_h instance, double *period)
{
	if (instance == NULL || period == NULL)
		return -1;

	*period = instance->period;
	return 0;
}

EAPI int widget_instance_change_period(widget_instance_h instance, double period)
{
	return 0;
}

EAPI int widget_instance_trigger_update(widget_instance_h instance, bundle *b, int force)
{
	return 0;
}

EAPI widget_instance_h widget_instance_get_instance(const char *widget_id, const char *instance_id)
{
	widget_instance_h instance;

	if (widget_id == NULL || instance_id == NULL)
		return NULL;

	if (_widget_apps && _widget_instances) {
		instance = __pick_instance(widget_id, instance_id);
		return widget_instance_ref(instance);
	}

	return NULL;
}

EAPI int widget_instance_get_instance_list(const char *widget_id, widget_instance_list_cb cb, void *data)
{
	widget_instance_h instance;
	struct widget_app *app;
	GList *head = NULL;

	if (widget_id == NULL)
		return -1;

	if (_widget_apps)
		app = __pick_app(widget_id);
	else
		return -2;

	if (app) {
		head = app->instances;

		while (head) {
			instance = (widget_instance_h)head->data;
			if (cb(instance->widget_id, instance->id, data) != 0)
				break;

			head = head->next;
		}
	}

	return 0;
}

EAPI void widget_instance_unref(widget_instance_h instance)
{
	if (instance == NULL)
		return;

	instance->ref--;

	if (instance->ref > -1)
		return;

	__remove_instance(instance);
}

EAPI widget_instance_h widget_instance_ref(widget_instance_h instance)
{
	if (instance)
		instance->ref++;

	return instance;
}



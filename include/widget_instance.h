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

#ifndef __WIDGET_INSTANCE_H__
#define __WIDGET_INSTANCE_H__

#include <bundle.h>
#include <aul.h>
#include <widget_service.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIDGET_K_ID		AUL_K_WIDGET_ID
#define WIDGET_K_CLASS		AUL_K_WIDGET_ID
#define WIDGET_K_INSTANCE	AUL_K_WIDGET_INSTANCE_ID
#define WIDGET_K_STATUS		"__WIDGET_STATUS__"
#define WIDGET_K_ENDPOINT	"__WIDGET_ENDPOINT__"

#define WIDGET_K_CALLER		"__WIDGET_CALLER_PID__"
#define WIDGET_K_OPERATION	"__WIDGET_OP__"
#define WIDGET_K_WIDTH		"__WIDGET_WIDTH__"
#define WIDGET_K_HEIGHT		"__WIDGET_HEIGHT__"
#define WIDGET_K_REASON		"__WIDGET_REASON__"
#define WIDGET_K_PERIOD		"__WIDGET_PERIOD__"
#define WIDGET_K_FORCE		"__WIDGET_FORCE__"
#define WIDGET_K_CONTENT_INFO	"__WIDGET_CONTENT_INFO__"

typedef enum widget_instance_event {
	WIDGET_INSTANCE_EVENT_CREATE = 0,
	WIDGET_INSTANCE_EVENT_DESTROY = 1,
	WIDGET_INSTANCE_EVENT_TERMINATE = 2,
	WIDGET_INSTANCE_EVENT_PAUSE = 3,
	WIDGET_INSTANCE_EVENT_RESUME = 4,
	WIDGET_INSTANCE_EVENT_UPDATE = 5,
	WIDGET_INSTANCE_EVENT_PERIOD_CHANGED = 6,
	WIDGET_INSTANCE_EVENT_SIZE_CHANGED = 7,
	WIDGET_INSTANCE_EVENT_EXTRA_UPDATED = 8,
	WIDGET_INSTANCE_EVENT_FAULT = 9,
	WIDGET_INSTANCE_EVENT_APP_RESTART_REQUEST = 10,
	WIDGET_INSTANCE_EVENT_MAX,
} widget_instance_event_e;

typedef struct _widget_instance *widget_instance_h;
typedef int (*widget_instance_foreach_cb)(widget_instance_h instance, void *data);
int widget_instance_foreach(const char *widget_id, widget_instance_foreach_cb cb, void *data);

int widget_instance_get_id(widget_instance_h instance, char **id);
int widget_instance_get_content(widget_instance_h instance, char **content);
int widget_instance_get_width(widget_instance_h instance, int *w);
int widget_instance_get_height(widget_instance_h instance, int *h);
int widget_instance_get_period(widget_instance_h instance, double *period);

int widget_instance_create(const char *widget_id, char **instance_id);
int widget_instance_launch(const char *instance_id, char *content_info, int w, int h);
int widget_instance_terminate(const char *instance_id);
int widget_instance_destroy(const char *instance_id);
int widget_instance_resume(const char *instance_id);
int widget_instance_pause(const char *instance_id);
int widget_instance_resize(const char *instance_id, int w, int h);
int widget_instance_init(const char *viewer_id);
int widget_instance_fini();

typedef int (*widget_instance_event_cb)(const char *widget_id, const char *instance_id, int event, void *data);

widget_instance_h widget_instance_get_instance(const char *widget_id, const char *instance_id);
int widget_instance_get_instance_list(const char *widget_id, widget_instance_list_cb cb, void *data);
void widget_instance_unref(widget_instance_h instance);
widget_instance_h widget_instance_ref(widget_instance_h instance);
int widget_instance_change_period(widget_instance_h instance, double period);
int widget_instance_trigger_update(widget_instance_h instance, const char *content_info, int force);
int widget_instance_listen_event(widget_instance_event_cb cb, void *data);
int widget_instance_unlisten_event(widget_instance_event_cb cb);
int widget_instance_listen_status(const char *widget_id, widget_instance_event_cb cb, void *data);
int widget_instance_unlisten_status(const char *widget_id);

int widget_instance_trigger_update_v2(const char *widget_id, const char *instance_id, const char *content_info, int force);

#ifdef __cplusplus
}
#endif

#endif


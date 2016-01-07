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

#include "widget_errno.h"
#include "debug.h"
#include "widget_conf.h"
#include "widget_service.h"

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
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_widget_list_by_pkgid(const char *pkgid, widget_list_by_pkgid_cb cb, void *data)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI char *widget_service_get_main_app_id(const char *widgetid)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI int widget_service_get_supported_size_types(const char *pkgid, int *cnt, int **types)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI char *widget_service_get_app_id_of_setup_app(const char *widgetid)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI int widget_service_get_nodisplay(const char *pkgid)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return 0;
}

EAPI int widget_service_get_need_of_frame(const char *pkgid, widget_size_type_e size_type, bool *need_of_frame)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_need_of_touch_effect(const char *pkgid, widget_size_type_e size_type, bool *need_of_touch_event)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_need_of_mouse_event(const char *pkgid, widget_size_type_e size_type, bool *need_of_mouse_event)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI char *widget_service_get_preview_image_path(const char *pkgid, widget_size_type_e size_type)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI char *widget_service_get_icon(const char *pkgid, const char *lang)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI char *widget_service_get_name(const char *pkgid, const char *lang)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI int widget_service_get_supported_sizes(const char *pkgid, int *cnt, int **w, int **h)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI char *widget_service_get_widget_id(const char *appid)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI char *widget_service_get_package_id(const char *pkgname)
{
	set_last_result(WIDGET_ERROR_NOT_SUPPORTED);
	return NULL;
}

EAPI int widget_service_get_size(widget_size_type_e type, int *width, int *height)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}

EAPI int widget_service_get_size_type(int width, int height, widget_size_type_e *size_type)
{
	return WIDGET_ERROR_NOT_SUPPORTED;
}



/* End of a file */

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
#include "livebox-service.h"
#include "livebox-service_product.h"

#include "dynamicbox-service.h"


#define EAPI __attribute__((visibility("default")))

EAPI int dynamicbox_service_change_period(const char *pkgname, const char *id, double period)
{
	return livebox_service_change_period(pkgname, id, period);
}

EAPI int dynamicbox_service_get_instance_count(const char *pkgname, const char *cluster, const char *category)
{
	return livebox_service_get_instance_count(pkgname, cluster, category);
}

EAPI int dynamicbox_service_trigger_update(const char *pkgname, const char *id, const char *cluster, const char *category, const char *content, int force)
{
	if (!content) {
		return livebox_service_trigger_update(pkgname, id, cluster, category, force);
	} else {
		return livebox_service_trigger_update_with_content(pkgname, id, cluster, category, content, force);
	}
}

EAPI struct pkglist_handle *dynamicbox_service_pkglist_create(const char *pkgid, struct pkglist_handle *handle)
{
	return livebox_service_pkglist_create(pkgid, handle);
}

EAPI int dynamicbox_service_get_pkglist_item(struct pkglist_handle *handle, char **appid, char **pkgname, int *is_prime)
{
	return livebox_service_get_pkglist_item(handle, appid, pkgname, is_prime);
}

EAPI int dynamicbox_service_pkglist_destroy(struct pkglist_handle *handle)
{
	return livebox_service_pkglist_destroy(handle);
}

EAPI int dynamicbox_service_get_pkglist(int (*cb)(const char *appid, const char *pkgname, int is_prime, void *data), void *data)
{
	return livebox_service_get_pkglist(cb, data);
}

EAPI int dynamicbox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *dboxid, int is_prime, void *data), void *data)
{
	return livebox_service_get_pkglist_by_pkgid(pkgid, cb, data);
}

EAPI int dynamicbox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *dboxid, void *data), void *data)
{
	return livebox_service_get_pkglist_by_category(category, cb, data);
}

EAPI int dynamicbox_service_get_applist(const char *dboxid, void (*cb)(const char *dboxid, const char *appid, void *data), void *data)
{
	return livebox_service_get_applist(dboxid, cb, data);
}

EAPI char *dynamicbox_service_mainappid(const char *dboxid)
{
	return livebox_service_mainappid(dboxid);
}

EAPI int dynamicbox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types)
{
	return livebox_service_get_supported_size_types(pkgid, cnt, types);
}

EAPI char *dynamicbox_service_content(const char *pkgid)
{
	return livebox_service_content(pkgid);
}

EAPI char *dynamicbox_service_setup_appid(const char *dboxid)
{
	return livebox_service_setup_appid(dboxid);
}

EAPI int dynamicbox_service_nodisplay(const char *pkgid)
{
	return livebox_service_nodisplay(pkgid);
}

EAPI int dynamicbox_service_need_frame(const char *pkgid, int size_type)
{
	return livebox_service_need_frame(pkgid, size_type);
}

EAPI int dynamicbox_service_touch_effect(const char *pkgid, int size_type)
{
	return livebox_service_touch_effect(pkgid, size_type);
}

EAPI int dynamicbox_service_mouse_event(const char *pkgid, int size_type)
{
	return livebox_service_mouse_event(pkgid, size_type);
}

EAPI char *dynamicbox_service_preview(const char *pkgid, int size_type)
{
	return livebox_service_preview(pkgid, size_type);
}

EAPI char *dynamicbox_service_i18n_icon(const char *pkgid, const char *lang)
{
	return livebox_service_i18n_icon(pkgid, lang);
}

EAPI char *dynamicbox_service_i18n_name(const char *pkgid, const char *lang)
{
	return livebox_service_i18n_name(pkgid, lang);
}

EAPI int dynamicbox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h)
{
	return livebox_service_get_supported_sizes(pkgid, cnt, w, h);
}

EAPI char *dynamicbox_service_abi(const char *dboxid)
{
	return livebox_service_abi(dboxid);
}

EAPI char *dynamicbox_service_pkgname_by_libexec(const char *libexec)
{
	return livebox_service_pkgname_by_libexec(libexec);
}

EAPI char *dynamicbox_service_libexec(const char *pkgid)
{
	return livebox_service_libexec(pkgid);
}

EAPI char *dynamicbox_service_pkgname(const char *appid)
{
	return livebox_service_pkgname(appid);
}

EAPI char *dynamicbox_service_provider_name(const char *dboxid)
{
	return livebox_service_provider_name(dboxid);
}

EAPI int dynamicbox_service_is_enabled(const char *dboxid)
{
	return livebox_service_is_enabled(dboxid);
}

EAPI int dynamicbox_service_is_primary(const char *dboxid)
{
	return livebox_service_is_primary(dboxid);
}

EAPI char *dynamicbox_service_category(const char *dboxid)
{
	return livebox_service_category(dboxid);
}

EAPI char *dynamicbox_service_appid(const char *pkgname)
{
	return livebox_service_appid(pkgname);
}

EAPI char *dynamicbox_service_dbox_script_path(const char *pkgid)
{
	return livebox_service_lb_script_path(pkgid);
}

EAPI char *dynamicbox_service_dbox_script_group(const char *pkgid)
{
	return livebox_service_lb_script_group(pkgid);
}

EAPI char *dynamicbox_service_pd_script_path(const char *pkgid)
{
	return livebox_service_pd_script_path(pkgid);
}

EAPI char *dynamicbox_service_pd_script_group(const char *pkgid)
{
	return livebox_service_pd_script_group(pkgid);
}

EAPI int dynamicbox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data)
{
	return livebox_service_enumerate_cluster_list(cb, data);
}

EAPI int dynamicbox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data)
{
	return livebox_service_enumerate_category_list(cluster, cb, data);
}

EAPI int dynamicbox_service_init(void)
{
	return livebox_service_init();
}

EAPI int dynamicbox_service_fini(void)
{
	return livebox_service_fini();
}

EAPI int dynamicbox_service_get_size(int type, int *width, int *height)
{
	return livebox_service_get_size(type, width, height);
}

EAPI int dynamicbox_service_size_type(int width, int height)
{
	return livebox_service_size_type(width, height);
}

/* End of a file */

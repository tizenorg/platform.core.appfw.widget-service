#include <stdio.h>

#include <sqlite3.h>

#include "livebox-service.h"
#include "livebox-errno.h"

struct pkglist_handle {
	enum pkglist_type {
		PKGLIST_TYPE_LB_LIST = 0x00beef00,
		PKGLIST_TYPE_UNKNOWN = 0x00dead00
	} type;
	sqlite3 *handle;
	sqlite3_stmt *stmt;
};

int livebox_service_get_size(int type, int *width, int *height)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_size_type(int width, int height)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_mouse_event(const char *lbid, int size_type)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_touch_effect(const char *lbid, int size_type)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_need_frame(const char *lbid, int size_type)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_trigger_update(const char *lbid, const char *instance_id, const char *cluster, const char *category, int force)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_trigger_update_with_content(const char *lbid, const char *instance_id, const char *cluster, const char *category, const char *content, int force)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_change_period(const char *lbid, const char *instance_id, double period)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_get_pkglist(int (*cb)(const char *pkgid, const char *lbid, int is_prime, void *data), void *data)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_get_applist(const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

char *livebox_service_mainappid(const char *lbid)
{
    return NULL;
}

int livebox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *lbid, int is_prime, void *data), void *data)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *lbid, void *data), void *data)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

char *livebox_service_pkgname(const char *lbid)
{
    return NULL;
}

int livebox_service_is_primary(const char *lbid)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

char *livebox_service_category(const char *lbid)
{
    return NULL;
}

char *livebox_service_provider_name(const char *lbid)
{
    return NULL;
}

char *livebox_service_setup_appid(const char *lbid)
{
    return NULL;
}

char *livebox_service_appid(const char *lbid)
{
    return NULL;
}

char *livebox_service_i18n_name(const char *lbid, const char *lang)
{
    return NULL;
}

char *livebox_service_preview(const char *lbid, int size_type)
{
    return NULL;
}

char *livebox_service_content(const char *lbid)
{
    return NULL;
}

char *livebox_service_i18n_icon(const char *pkgid, const char *lang)
{
    return NULL;
}

int livebox_service_nodisplay(const char *lbid)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

char *livebox_service_abi(const char *lbid)
{
    return NULL;
}

int livebox_service_is_enabled(const char *lbid)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

char *livebox_service_lb_script_path(const char *lbid)
{
    return NULL;
}

char *livebox_service_lb_script_group(const char *lbid)
{
    return NULL;
}

char *livebox_service_pd_script_path(const char *lbid)
{
    return NULL;
}

char *livebox_service_pd_script_group(const char *lbid)
{
    return NULL;
}

int livebox_service_get_supported_sizes(const char *lbid, int *cnt, int *w, int *h)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_get_supported_size_types(const char *lbid, int *cnt, int *types)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_init(void)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_fini(void)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

struct pkglist_handle *livebox_service_pkglist_create(const char *lbid, struct pkglist_handle *handle)
{
    return NULL;
}

int livebox_service_get_pkglist_item(struct pkglist_handle *handle, char **lbid, char **pkgname, int *is_prime)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_pkglist_destroy(struct pkglist_handle *handle)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_get_instance_count(const char *lbid, const char *cluster, const char *category)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

int livebox_service_max_instance_count(const char *lbid)
{
    return LB_STATUS_ERROR_NOT_IMPLEMENTED;
}

char *livebox_service_libexec(const char *lbid)
{
    return NULL;
}

char *livebox_service_pkgname_by_libexec(const char *libexec)
{
    return NULL;
}

/* End of a file */

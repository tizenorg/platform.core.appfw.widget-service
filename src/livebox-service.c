#include <stdio.h>
#include <errno.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strdup, strerror */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>
#include <X11/X.h>
#include <X11/Xlib.h>

#include <com-core_packet.h>
#include <packet.h>
#include <dlog.h>
#include <db-util.h>
#include <package-manager.h>

#include "dlist.h"
#include "util.h"
#include "debug.h"
#include "livebox-service.h"

#define EAPI __attribute__((visibility("default")))
#define DEFAULT_TIMEOUT 2.0

static struct supported_size_list {
	int w;
	int h;
} SIZE_LIST[NR_OF_SIZE_LIST] = {
	{ 172, 172 }, /*!< 1x1 */
	{ 348, 172 }, /*!< 2x1 */
	{ 348, 348 }, /*!< 2x2 */
	{ 700, 172 }, /*!< 4x1 */
	{ 700, 348 }, /*!< 4x2 */
	{ 700, 700 }, /*!< 4x4 */
};

static struct info {
	sqlite3 *handle;
	const char *dbfile;
	const char *conf_file;
	int init_count;
	int res_updated;
} s_info = {
	.handle = NULL,
	.dbfile = "/opt/dbspace/.livebox.db", 
	.conf_file = "/usr/share/data-provider-master/resolution.ini",
	.init_count = 0,
	.res_updated = 0,
};

static inline int update_resolution(void)
{
	Display *disp;
	Window root;
	Window dummy;
	int x, y;
	unsigned int width;
	unsigned int height;
	unsigned int border;
	unsigned int depth;
	register int i;

	const struct size_list {
		int w;
		int h;
	} list[NR_OF_SIZE_LIST] = {
		{ 172, 172 }, /*!< 1x1 */
		{ 348, 172 }, /*!< 2x1 */
		{ 348, 348 }, /*!< 2x2 */
		{ 700, 172 }, /*!< 4x1 */
		{ 700, 348 }, /*!< 4x2 */
		{ 700, 700 }, /*!< 4x4 */
	};

	if (s_info.res_updated)
		return 0;

	disp = XOpenDisplay(NULL);
	if (!disp) {
		ErrPrint("Failed to open a display\n");
		return -EFAULT;
	}

	root = XDefaultRootWindow(disp);
	if (!XGetGeometry(disp, root, &dummy, &x, &y, &width, &height, &border, &depth)) {
		XCloseDisplay(disp);
		return -EFAULT;
	}

	DbgPrint("Screen resolution: %dx%d\n", width, height);
	for (i = 0; i < NR_OF_SIZE_LIST; i++) {
		SIZE_LIST[i].w = (unsigned int)((double)list[i].w * 720.0f / (double)width);
		SIZE_LIST[i].h = (unsigned int)((double)list[i].h * 1280.0f / (double)height);
		DbgPrint("Size is updated to %dx%d --> %dx%d\n", list[i].w, list[i].h, SIZE_LIST[i].w, SIZE_LIST[i].h);
	}

	XCloseDisplay(disp);
	s_info.res_updated = 1;
	return 0;
}

static inline sqlite3 *open_db(void)
{
	sqlite3 *handle;

	if (!s_info.handle) {
		int ret;

		ret = db_util_open(s_info.dbfile, &handle, DB_UTIL_REGISTER_HOOK_METHOD);
		if (ret != SQLITE_OK) {
			ErrPrint("Failed to open a DB\n");
			return NULL;
		}
	} else {
		handle = s_info.handle;
	}

	return handle;
}

static inline void close_db(sqlite3 *handle)
{
	if (!s_info.handle)
		db_util_close(handle);
}

static inline int convert_size_from_type(enum livebox_size_type type, int *width, int *height)
{
	int idx;

	switch (type) {
	case LB_SIZE_TYPE_1x1: /*!< 172x172 */
		idx = 0;
		break;
	case LB_SIZE_TYPE_2x1: /*!< 348x172 */
		idx = 1;
		break;
	case LB_SIZE_TYPE_2x2: /*!< 348x348 */
		idx = 2;
		break;
	case LB_SIZE_TYPE_4x1: /*!< 700x172 */
		idx = 3;
		break;
	case LB_SIZE_TYPE_4x2: /*!< 700x348 */
		idx = 4;
		break;
	case LB_SIZE_TYPE_4x4: /*!< 700x700 */
		idx = 5;
		break;
	default:
		return -EINVAL;
	}

	update_resolution();

	*width = SIZE_LIST[idx].w;
	*height = SIZE_LIST[idx].h;

	return 0;
}

EAPI int livebox_service_trigger_update(const char *pkgname, const char *cluster, const char *category)
{
	struct packet *packet;
	struct packet *result;
	int ret;

	if (!pkgname) {
		ErrPrint("Invalid argument\n");
		return -EINVAL;
	}

	if (!cluster)
		cluster = "user,created";

	if (!category)
		category = "default";

	packet = packet_create("service_update", "sss", pkgname, cluster, category);
	if (!packet) {
		ErrPrint("Failed to create a packet for service_update\n");
		return -EFAULT;
	}

	result = com_core_packet_oneshot_send(SOCKET_FILE, packet, DEFAULT_TIMEOUT);
	packet_unref(packet);

	if (result) {
		if (packet_get(result, "i", &ret) != 1) {
			ErrPrint("Failed to parse a result packet\n");
			ret = -EINVAL;
		}

		packet_unref(result);
	} else {
		ErrPrint("Failed to get result packet\n");
		ret = -EFAULT;
	}

	return ret;
}

EAPI int livebox_service_get_pkglist(int (*cb)(const char *appid, const char *pkgname, int is_prime, void *data), void *data)
{
	int ret;
	sqlite3_stmt *stmt;
	char *appid;
	char *pkgid;
	int is_prime;
	sqlite3 *handle;

	if (!cb)
		return -EINVAL;

	handle = open_db();
	if (!handle)
		return -EIO;

	ret = sqlite3_prepare_v2(handle, "SELECT appid, pkgid, prime FROM pkgmap", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = -EIO;
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

EAPI int livebox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int size;
	int ret;

	if (!w || !h || !cnt || !pkgid)
		return -EINVAL;

	handle = open_db();
	if (!handle)
		return -EIO;

	ret = sqlite3_prepare_v2(handle, "SELECT size_type FROM box_size WHERE pkgid = ? ORDER BY size_type ASC", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		ret = -EIO;
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		ret = -EIO;
		goto out;
	}

	if (*cnt > NR_OF_SIZE_LIST)
		*cnt = NR_OF_SIZE_LIST;

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

EAPI char *livebox_service_libexec(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *libexec;
	char *appid;
	char *path;

	if (!pkgid)
		return NULL;

	libexec = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.libexec FROM pkgmap, provider WHERE pkgmap.pkgid = ? AND provider.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, NULL);
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

static inline char *get_lb_pkgname_by_appid(const char *appid)
{
	sqlite3_stmt *stmt;
	char *pkgid;
	char *tmp;
	sqlite3 *handle;
	int ret;

	if (!appid)
		return NULL;

	pkgid = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid FROM pkgmap WHERE (appid = ? AND prime = 1) OR pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		close_db(handle);
		return NULL;
	}

	ret = sqlite3_bind_text(stmt, 1, appid, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, appid, -1, NULL);
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
		if (!pkgid)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

out:
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	close_db(handle);
	return pkgid;
}

EAPI char *livebox_service_pkgname(const char *appid)
{
	char *lb_pkgname;
	pkgmgr_appinfo_h handle;
	int ret;
	char *new_appid;

	if (!appid)
		return NULL;

	lb_pkgname = get_lb_pkgname_by_appid(appid);
	if (lb_pkgname)
		return lb_pkgname;

	/*!
	 * \note
	 * Try to get the package id using given appid
	 */
	ret = pkgmgr_appinfo_get_appinfo(appid, &handle);
	if (ret != PKGMGR_R_OK) {
		ErrPrint("Failed to get appinfo\n");
		return NULL;
	}

	ret = pkgmgr_appinfo_get_pkgname(handle, &new_appid);
	if (ret != PKGMGR_R_OK) {
		pkgmgr_appinfo_destroy_appinfo(handle);
		ErrPrint("Failed to get pkgname for (%s)\n", appid);
		return NULL;
	}

	lb_pkgname = get_lb_pkgname_by_appid(new_appid);
	pkgmgr_appinfo_destroy_appinfo(handle);

	return lb_pkgname;
}

EAPI char *livebox_service_appid(const char *pkgname)
{
	sqlite3_stmt *stmt;
	char *appid;
	char *tmp;
	sqlite3 *handle;
	int is_prime;
	int ret;

	if (!pkgname)
		return NULL;

	appid = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT appid, prime FROM pkgmap WHERE pkgid = ? OR appid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgname, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgname, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
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

EAPI char *livebox_service_lb_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *appid;
	char *lb_src;

	if (!pkgid)
		return NULL;

	path = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.box_src FROM provider, pkgmap WHERE pkgmap.pkgid = ? AND provider.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, NULL);
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

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Invalid appid : %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	lb_src = (char *)sqlite3_column_text(stmt, 1);
	if (!lb_src || !strlen(lb_src)) {
		ErrPrint("No records for lb src : %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = strdup(lb_src);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("LB Src: %s\n", path);

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return path;
}

EAPI char *livebox_service_lb_script_group(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *group;
	char *tmp;

	if (!pkgid)
		return NULL;

	group = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT box_group FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, NULL);
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
		if (!group)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return group;
}

EAPI char *livebox_service_pd_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *pd_src;
	const char *appid;

	if (!pkgid)
		return NULL;

	path = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgmap.appid, provider.pd_src FROM provider, pkgmap WHERE provider.pkgid = ? AND pkgmap.pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_finalize(stmt);
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 2, pkgid, -1, NULL);
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

	appid = (char *)sqlite3_column_text(stmt, 0);
	if (!appid || !strlen(appid)) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	pd_src = (char *)sqlite3_column_text(stmt, 1);
	if (!pd_src || !strlen(pd_src)) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	path = strdup(pd_src);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	DbgPrint("PD Src: %s\n", path);
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return path;
}

EAPI char *livebox_service_pd_script_group(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *group;
	char *tmp;

	if (!pkgid)
		return NULL;

	group = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pd_group FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		goto out;
	}

	ret = sqlite3_bind_text(stmt, 1, pkgid, -1, NULL);
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
		if (!group)
			ErrPrint("Heap: %s\n", strerror(errno));
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return group;
}

EAPI int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	const char *cluster;
	int cnt;
	int ret;

	if (!cb)
		return -EINVAL;

	handle = open_db();
	if (!handle)
		return -EIO;

	cnt = 0;
	ret = sqlite3_prepare_v2(handle, "SELECT DISTINCT cluster FROM groupinfo", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		cnt = -EIO;
		goto out;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		cluster = (const char *)sqlite3_column_text(stmt, 0);
		if (!cluster || !strlen(cluster))
			continue;

		if (cb(cluster, data) < 0)
			break;

		cnt++;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return cnt;
}

EAPI int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	const char *category;
	int cnt;
	int ret;

	if (!cluster || !cb)
		return -EINVAL;

	handle = open_db();
	if (!handle)
		return -EIO;

	ret = sqlite3_prepare_v2(handle, "SELECT DISTINCT category FROM groupinfo WHERE cluster = ?", -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		cnt = -EIO;
		goto out;
	}

	cnt = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		category = (const char *)sqlite3_column_text(stmt, 0);
		if (!category || !strlen(category))
			continue;

		if (cb(cluster, category, data) < 0)
			break;

		cnt++;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return cnt;
}

EAPI int livebox_service_init(void)
{
	if (s_info.handle) {
		DbgPrint("Already initialized\n");
		s_info.init_count++;
		return 0;
	}

	s_info.handle = open_db();
	if (s_info.handle) {
		s_info.init_count++;
		return 0;
	}

	return -EIO;
}

EAPI int livebox_service_fini(void)
{
	if (!s_info.handle || s_info.init_count <= 0) {
		ErrPrint("Service is not initialized\n");
		return -EIO;
	}

	s_info.init_count--;
	if (s_info.init_count > 0) {
		DbgPrint("Init count %d\n", s_info.init_count);
		return 0;
	}

	db_util_close(s_info.handle);
	s_info.handle = NULL;
	return 0;
}

EAPI int livebox_service_get_size(int type, int *width, int *height)
{
	int _width;
	int _height;

	if (!width)
		width = &_width;

	if (!height)
		height = &_height;

	return convert_size_from_type(type, width, height);
}

EAPI int livebox_service_size_type(int width, int height)
{
	int idx;

	for (idx = 0; idx < NR_OF_SIZE_LIST; idx++) {
		if (SIZE_LIST[idx].w == width && SIZE_LIST[idx].h == height)
			break;
	}

	switch (idx) {
	case 0:
		return LB_SIZE_TYPE_1x1;
	case 1:
		return LB_SIZE_TYPE_2x1;
	case 2:
		return LB_SIZE_TYPE_2x2;
	case 3:
		return LB_SIZE_TYPE_4x1;
	case 4:
		return LB_SIZE_TYPE_4x2;
	case 5:
		return LB_SIZE_TYPE_4x4;
	default:
		break;
	}

	return LB_SIZE_TYPE_UNKNOWN;
}

/* End of a file */

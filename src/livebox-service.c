#include <stdio.h>
#include <errno.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strdup, strerror */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>

#include <com-core_packet.h>
#include <packet.h>
#include <dlog.h>
#include <db-util.h>

#include "dlist.h"
#include "util.h"
#include "debug.h"
#include "livebox-service.h"

#define EAPI __attribute__((visibility("default")))
#define DEFAULT_TIMEOUT 2.0

static struct info {
	sqlite3 *handle;
	const char *dbfile;
} s_info = {
	.handle = NULL,
	.dbfile = "/opt/dbspace/.livebox.db", 
};

static inline sqlite3 *open_db(void)
{
	sqlite3 *handle;
	struct stat stat;

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

	if (lstat(s_info.dbfile, &stat) < 0) {
		ErrPrint("stat: %s\n", strerror(errno));
		goto out;
	}

	if (!S_ISREG(stat.st_mode)) {
		ErrPrint("DBFile is not valid\n");
		goto out;
	}

	if (!stat.st_size)
		goto out;

	return handle;

out:
	if (!s_info.handle && handle)
		db_util_close(handle);

	return NULL;
}

static inline void close_db(sqlite3 *handle)
{
	if (!s_info.handle)
		db_util_close(handle);
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
		if (!appid) {
			ErrPrint("APPID is not valid\n");
			continue;
		}

		pkgid = (char *)sqlite3_column_text(stmt, 1);
		if (!pkgid) {
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

EAPI char *livebox_service_libexec(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *libexec;

	libexec = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT libexec FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
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

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	libexec = (char *)sqlite3_column_text(stmt, 0);
	if (libexec) {
		char *tmp;
		tmp = strdup(libexec);
		if (!tmp)
			ErrPrint("Heap: %s\n", strerror(errno));

		libexec = tmp;
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return libexec;
}

EAPI char *livebox_service_pkgname(const char *pkgname)
{
	sqlite3_stmt *stmt;
	char *pkgid;
	char *tmp;
	sqlite3 *handle;
	int ret;

	pkgid = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pkgid, libexec FROM pkgmap WHERE (appid = ? AND is_prime = 1) OR pkgid = ?", -1, &stmt, NULL);
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

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		ErrPrint("Error: %s\n", sqlite3_errmsg(handle));
		sqlite3_reset(stmt);
		sqlite3_finalize(stmt);
		goto out;
	}

	tmp = (char *)sqlite3_column_text(stmt, 1);
	if (tmp) {
		struct stat stat;

		DbgPrint("libexec: %s\n", tmp);
		if (lstat(tmp, &stat) < 0) {
			ErrPrint("stat: %s\n", strerror(errno));
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
			goto out;
		}

		if (!S_ISREG(stat.st_mode)) {
			ErrPrint("libexec is not valid\n");
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
			goto out;
		}

		if (!stat.st_size) {
			ErrPrint("libexec is not valid\n");
			sqlite3_reset(stmt);
			sqlite3_finalize(stmt);
			goto out;
		}
	}

	tmp = (char *)sqlite3_column_text(stmt, 0);
	if (tmp) {
		pkgid = strdup(tmp);
		if (!pkgid)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
out:
	close_db(handle);
	return pkgid;
}

EAPI char *livebox_service_lb_script_path(const char *pkgid)
{
	sqlite3_stmt *stmt;
	sqlite3 *handle;
	int ret;
	char *path;
	char *tmp;

	path = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT box_src FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
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
	if (tmp) {
		path = strdup(tmp);
		if (!path)
			ErrPrint("Heap: %s\n", strerror(errno));
	}

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
	if (tmp) {
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
	char *tmp;

	path = NULL;
	handle = open_db();
	if (!handle)
		return NULL;

	ret = sqlite3_prepare_v2(handle, "SELECT pd_src FROM provider WHERE pkgid = ?", -1, &stmt, NULL);
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
	if (tmp) {
		path = strdup(tmp);
		if (!path)
			ErrPrint("Heap: %s\n", strerror(errno));
	}
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
	if (tmp) {
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

EAPI int livebox_service_init(void)
{
	if (s_info.handle) {
		DbgPrint("Already initialized\n");
		return 0;
	}

	s_info.handle = open_db();
	return s_info.handle ? 0 : -EIO;
}

EAPI int livebox_service_fini(void)
{
	if (!s_info.handle) {
		ErrPrint("Service is not initialized\n");
		return -EIO;
	}

	db_util_close(s_info.handle);
	s_info.handle = NULL;
	return 0;
}

/* End of a file */

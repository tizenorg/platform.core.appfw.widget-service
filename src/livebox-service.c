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
#include "livebox-service_internal.h"

#define EAPI __attribute__((visibility("default")))
#define DEFAULT_TIMEOUT 2.0

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
	sqlite3 *handle = NULL;
	struct stat stat;
	const char *dbfile = "/opt/dbspace/.livebox.db";
	sqlite3_stmt *stmt;
	char *appid;
	char *pkgid;
	int is_prime;

	if (!cb)
		return -EINVAL;

	ret = db_util_open(dbfile, &handle, DB_UTIL_REGISTER_HOOK_METHOD);
	if (ret != SQLITE_OK) {
		ErrPrint("Failed to open a DB\n");
		return -EIO;
	}

	if (lstat(dbfile, &stat) < 0) {
		ErrPrint("stat: %s\n", strerror(errno));
		ret = -EIO;
		goto out;
	}

	if (!S_ISREG(stat.st_mode)) {
		ErrPrint("DBFile is not valid\n");
		ret = -EIO;
		goto out;
	}

	if (!stat.st_size) {
		ret = 0;
		goto out;
	}

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

		ret ++;

		if (cb(appid, pkgid, is_prime, data) < 0) {
			DbgPrint("Callback stopped package crawling\n");
			break;
		}
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

out:
	db_util_close(handle);
	return ret;
}

/* End of a file */

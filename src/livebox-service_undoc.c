#include <stdio.h>
#include <errno.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strdup, strerror */

#include <com-core_packet.h>
#include <packet.h>
#include <dlog.h>

#include "dlist.h"
#include "debug.h"
#include "livebox-service.h"
#include "livebox-service_internal.h"

#define EAPI __attribute__((visibility("default")))
#define UNDOC

struct livebox_service_slave_info {
	pid_t pid;
	char *name;
	char *pkgname;
	char *abi;
	int secured;
	int refcnt;
	int fault;
	enum livebox_service_slave_state state;
	int inst_cnt;
	int pkg_cnt;
	double ttl;
};

UNDOC EAPI pid_t const livebox_service_slave_pid(struct livebox_service_slave_info *info)
{
	return info->pid;
}

UNDOC EAPI const char * const livebox_service_slave_name(struct livebox_service_slave_info *info)
{
	return info->name;
}

UNDOC EAPI const char * const livebox_service_slave_pkgname(struct livebox_service_slave_info *info)
{
	return info->pkgname;
}

UNDOC EAPI const char * const livebox_service_slave_abi(struct livebox_service_slave_info *info)
{
	return info->abi;
}

UNDOC EAPI int const livebox_service_slave_is_secured(struct livebox_service_slave_info *info)
{
	return info->secured;
}

UNDOC EAPI int const livebox_service_slave_refcnt(struct livebox_service_slave_info *info)
{
	return info->refcnt;
}

UNDOC EAPI int const livebox_service_slave_fault(struct livebox_service_slave_info *info)
{
	return info->fault;
}

UNDOC EAPI enum livebox_service_slave_state const livebox_service_slave_state(struct livebox_service_slave_info *info)
{
	return info->state;
}

UNDOC EAPI int const livebox_service_count_of_instances(struct livebox_service_slave_info *info)
{
	return info->inst_cnt;
}

UNDOC EAPI int const livebox_service_count_of_packages(struct livebox_service_slave_info *info)
{
	return info->pkg_cnt;
}

UNDOC EAPI double const livebox_service_slave_ttl(struct livebox_service_slave_info *info)
{
	return info->ttl;
}

UNDOC EAPI int livebox_service_release_info(struct livebox_service_slave_info *info)
{
	return 0;
}

UNDOC EAPI int livebox_service_get_slave_list(int (*cb)(struct livebox_service_slave_info *info, void *data), void *data)
{
	return 0;
}


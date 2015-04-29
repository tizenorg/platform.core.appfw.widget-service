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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sqlite3.h>
#include <unicode/uloc.h>
#include <stdlib.h>
#include <ctype.h>

#include <dlog.h>
#include <widget_errno.h>

#include "util.h"
#include "debug.h"
#include "dlist.h"

int errno;

#define MAX_ABI		256
#define MAX_PKGNAME	512

struct item {
	char *abi;
	char *pkgname; /*!< Slave package name */
};

static struct {
	struct dlist *list;
} s_abi = {
	.list = NULL,
};

static int abi_add_entry(const char *abi, const char *pkgname)
{
	struct item *item;

	item = malloc(sizeof(*item));
	if (!item) {
		ErrPrint("Failed to add a new entry for abi[%s - %s]\n", abi, pkgname);
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	item->abi = strdup(abi);
	if (!item->abi) {
		ErrPrint("Heap: %d\n", errno);
		free(item);
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	item->pkgname = strdup(pkgname);
	if (!item->pkgname) {
		ErrPrint("Heap: %d\n", errno);
		free(item->abi);
		free(item);
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	s_abi.list = dlist_append(s_abi.list, item);
	return WIDGET_ERROR_NONE;
}

static int abi_load_table(void)
{
	FILE *fp;
	int ch;
	int idx = 0;
	int tag_id = 0;
	enum {
		INIT = 0x0,
		GROUP = 0x1,
		TAG = 0x02,
		VALUE = 0x03,
		ERROR = 0x05
	} state;
	enum {
		PKGNAME = 0x0,
	};
	static const char *field[] = {
		"package",
		NULL,
	};
	const char *ptr = NULL;

	char group[MAX_ABI + 1];
	char pkgname[MAX_PKGNAME + 1];

	fp = fopen("/usr/share/data-provider-master/abi.ini", "rt");
	if (!fp) {
		return WIDGET_ERROR_IO_ERROR;
	}

	state = INIT;
	while ((ch = getc(fp)) != EOF && state != ERROR) {
		switch (state) {
		case INIT:
			if (isspace(ch)) {
				continue;
			}
			if (ch == '[') {
				state = GROUP;
				idx = 0;
			} else {
				state = ERROR;
			}
			break;
		case GROUP:
			if (ch == ']') {
				if (idx == 0) {
					state = ERROR;
				} else {
					group[idx] = '\0';
					state = TAG;
					idx = 0;
					ptr = NULL;
				}
			} else if (idx < MAX_ABI) {
				group[idx++] = ch;
			} else {
				ErrPrint("Overflow\n");
				state = ERROR;
			}
			break;
		case TAG:
			if (ptr == NULL) {
				if (idx == 0) {
					if (isspace(ch)) {
						continue;
					}

					/* New group started */
					if (ch == '[') {
						ungetc(ch, fp);
						state = INIT;
						continue;
					}
				}

				ptr = field[idx];
			}

			if (ptr == NULL) {
				ErrPrint("unknown tag\n");
				state = ERROR;
				continue;
			}

			if (*ptr == '\0' && ch == '=') {
				/* MATCHED */
				state = VALUE;
				tag_id = idx;
				idx = 0;
				ptr = NULL;
			} else if (*ptr == ch) {
				ptr++;
			} else {
				ungetc(ch, fp);
				ptr--;
				while (ptr >= field[idx]) {
					ungetc(*ptr, fp);
					ptr--;
				}
				ptr = NULL;
				idx++;
			}
			break;
		case VALUE:
			switch (tag_id) {
			case PKGNAME:
				if (idx == 0) { /* LTRIM */
					if (isspace(ch)) {
						continue;
					}

					pkgname[idx] = ch;
					idx++;
				} else if (isspace(ch)) {
					int ret;
					pkgname[idx] = '\0';

					ret = abi_add_entry(group, pkgname);
					if (ret != 0) {
						ErrPrint("Failed to add %s for %s\n", pkgname, group);
					}

					state = TAG;
					idx = 0;
				} else if (idx < MAX_PKGNAME) {
					pkgname[idx] = ch;
					idx++;
				} else {
					ErrPrint("Overflow\n");
					state = ERROR;
				}
				break;
			default:
				break;
			}
			break;
		case ERROR:
		default:
			break;
		}
	}

	if (state == VALUE) {
		switch (tag_id) {
		case PKGNAME:
			if (idx) {
				int ret;
				pkgname[idx] = '\0';
				ret = abi_add_entry(group, pkgname);
				if (ret != 0) {
					ErrPrint("Failed to add %s for %s\n", pkgname, group);
				}
			}
			break;
		default:
			break;
		}
	}

	if (fclose(fp) != 0) {
		ErrPrint("fclose: %d\n", errno);
	}
	return WIDGET_ERROR_NONE;
}

static void abi_del_all(void)
{
	struct item *item;
	struct dlist *l;
	struct dlist *n;

	dlist_foreach_safe(s_abi.list, l, n, item) {
		s_abi.list = dlist_remove(s_abi.list, l);
		free(item->abi);
		free(item->pkgname);
		free(item);
	}
}

EAPI int widget_abi_init(void)
{
	return abi_load_table();
}

EAPI int widget_abi_fini(void)
{
	abi_del_all();
	return WIDGET_ERROR_NONE;
}

EAPI const char *widget_abi_get_pkgname_by_abi(const char *abi)
{
	struct dlist *l;
	struct item *item;

	dlist_foreach(s_abi.list, l, item) {
		if (!strcasecmp(item->abi, abi)) {
			return item->pkgname;
		}
	}

	return NULL;
}

EAPI const char *widget_abi_get_abi_by_pkgname(const char *pkgname)
{
	struct dlist *l;
	struct item *item;

	dlist_foreach(s_abi.list, l, item) {
		if (!strcmp(item->pkgname, pkgname)) {
			return item->abi;
		}
	}

	return NULL;
}

/* End of a file */

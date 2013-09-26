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
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <time.h>

#include <dlog.h>

#include "util.h"
#include "debug.h"
#include "livebox-errno.h"

int errno;
static struct {
	clockid_t type;
} s_info = {
	.type = CLOCK_MONOTONIC,
};

static inline char *check_native_livebox(const char *pkgname)
{
	int len;
	char *path;

	len = strlen(pkgname) * 2;
	len += strlen("/opt/usr/live/%s/libexec/liblive-%s.so");

	path = malloc(len + 1);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		return NULL;
	}

	snprintf(path, len, "/opt/usr/live/%s/libexec/liblive-%s.so", pkgname, pkgname);
	if (access(path, F_OK | R_OK) != 0) {
		ErrPrint("%s is not a valid package (%s)\n", pkgname, strerror(errno));
		free(path);
		return NULL;
	}

	return path;
}

static inline char *check_web_livebox(const char *pkgname)
{
	int len;
	char *path;

	len = strlen(pkgname) * 2;
	len += strlen("/opt/usr/apps/%s/res/wgt/livebox/index.html");

	path = malloc(len + 1);
	if (!path) {
		ErrPrint("Heap: %s\n", strerror(errno));
		return NULL;
	}

	snprintf(path, len, "/opt/usr/apps/%s/res/wgt/livebox/index.html", pkgname);
	if (access(path, F_OK | R_OK) != 0) {
		ErrPrint("%s is not a valid package (%s)\n", pkgname, strerror(errno));
		free(path);
		return NULL;
	}

	return path;
}

int util_validate_livebox_package(const char *pkgname)
{
	char *path;

	if (!pkgname) {
		ErrPrint("Invalid argument\n");
		return LB_STATUS_ERROR_INVALID;
	}

	path = check_native_livebox(pkgname);
	if (path) {
		free(path);
		return 0;
	}

	path = check_web_livebox(pkgname);
	if (path) {
		free(path);
		return 0;
	}

	return LB_STATUS_ERROR_INVALID;
}

double util_timestamp(void)
{
	struct timespec ts;

	do {
		if (clock_gettime(s_info.type, &ts) == 0) {
			return ts.tv_sec + ts.tv_nsec / 1000000000.0f;
		}

		ErrPrint("%d: %s\n", s_info.type, strerror(errno));
		if (s_info.type == CLOCK_MONOTONIC) {
			s_info.type = CLOCK_REALTIME;
		} else if (s_info.type == CLOCK_REALTIME) {
			break;
		}
	} while (1);

	return 0.0f;
}

const char *util_basename(const char *name)
{
	int length;
	length = name ? strlen(name) : 0;
	if (!length) {
		return ".";
	}

	while (--length > 0 && name[length] != '/');

	return length <= 0 ? name : (name + length + (name[length] == '/'));
}

unsigned long util_free_space(const char *path)
{
	struct statvfs st;
	unsigned long space;

	if (statvfs(path, &st) < 0) {
		ErrPrint("statvfs: %s\n", strerror(errno));
		return 0lu;
	}

	space = st.f_bsize * st.f_bfree;
	/*!
	 * \note
	 * Must have to check the overflow
	 */
	return space;
}

char *util_replace_string(const char *src, const char *pattern, const char *replace)
{
	int s_idx;
	int p_idx;
	int n_idx;
	int t_idx;
	int r_idx;
	int idx;
	char *result;
	int len;
	int rlen;
	int matched;

	if (!src || !pattern || !replace || !src[0] || !pattern[0]) {
		ErrPrint("Invalid argument: %s %s %s\n", src, pattern, replace);
		return NULL;
	}

	rlen = strlen(replace);
	len = strlen(src);
	result = malloc(len + 1);
	if (!result) {
		ErrPrint("Heap:%s\n", strerror(errno));
		return NULL;
	}

	r_idx = 0;
	idx = 0;
	matched = 0;
	for (s_idx = 0; src[s_idx]; s_idx++) {
		if (idx == len) {
			char *tmp;

			len += (rlen > len ? rlen : len);
			tmp = realloc(result, len + 1);
			if (!tmp) {
				ErrPrint("Heap: %s\n", strerror(errno));
				free(result);
				return NULL;
			}
			result = tmp;
		}

		if (src[s_idx] == pattern[0]) {
			n_idx = -1;
			t_idx = s_idx;
			r_idx = idx;

			if (r_idx == len) {
				char *tmp;
				len += (rlen > len ? rlen : len);
				tmp = realloc(result, len + 1);
				if (!tmp) {
					ErrPrint("Heap: %s\n", strerror(errno));
					free(result);
					return NULL;
				}
				result = tmp;
			}
			result[r_idx++] = src[t_idx++];
			p_idx = 1;
			while (pattern[p_idx]) {
				if (src[t_idx] == pattern[p_idx]) {
					if (n_idx < 0) {
						if (src[t_idx] == pattern[0]) {
							n_idx = t_idx;
						} else {
							if (r_idx == len) {
								char *tmp;
								len += (rlen > len ? rlen : len);
								tmp = realloc(result, len + 1);
								if (!tmp) {
									ErrPrint("Heap: %s\n", strerror(errno));
									free(result);
									return NULL;
								}
								result = tmp;
							}
							result[r_idx++] = src[t_idx];
						}
					}

					p_idx++;
					t_idx++;
					continue;
				}

				if (n_idx < 0) {
					s_idx = t_idx;
				} else {
					s_idx = n_idx;
				}

				break;
			}

			if (pattern[p_idx] == '\0') {
				if (idx + rlen >= len) {
					char *tmp;
					len += (rlen > len ? rlen : len);
					tmp = realloc(result, len + 1);
					if (!tmp) {
						ErrPrint("Heap: %s\n", strerror(errno));
						free(result);
						return NULL;
					}
					result = tmp;
					matched++;
				}
				strcpy(result + idx, replace);
				idx += strlen(replace);
				s_idx = t_idx - 1;
			} else {
				idx = r_idx;
				s_idx = (n_idx < 0 ? t_idx : n_idx) - 1;
			}
		} else {
			result[idx++] = src[s_idx];
		}
	}

	result[idx] = '\0';

	if (!matched) {
		free(result);
		result = NULL;
	}

	return result;
}

const char *util_uri_to_path(const char *uri)
{
	int len;

	len = strlen(SCHEMA_FILE);
	if (strncasecmp(uri, SCHEMA_FILE, len)) {
		return NULL;
	}

	return uri + len;
}

char *util_conf_get_libexec(const char *pkgname)
{
	char *path;

	if (!pkgname) {
		ErrPrint("Invalid argument\n");
		return NULL;
	}

	path = check_native_livebox(pkgname);
	if (!path) {
		path = check_web_livebox(pkgname);
	}

	return path;
}

char *util_id_to_uri(const char *id)
{
	char *uri;

	if (!id) {
		uri = strdup("");
		if (!uri) {
			ErrPrint("Heap: %s\n", strerror(errno));
			return NULL;
		}
	} else if (strncmp(id, SCHEMA_FILE, strlen(SCHEMA_FILE))) {
		int len;
		len = strlen(SCHEMA_FILE) + strlen(id) + 2;
		uri = malloc(len);
		if (!uri) {
			ErrPrint("Heap: %s\n", strerror(errno));
			return NULL;
		}

		snprintf(uri, len, SCHEMA_FILE"%s", id);
	} else {
		uri = strdup(id);
		if (!uri) {
			ErrPrint("Heap: %s\n", strerror(errno));
			return NULL;
		}
	}

	return uri;
}

/* End of a file */

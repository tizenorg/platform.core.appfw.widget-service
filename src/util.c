#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/statvfs.h>

#include <dlog.h>

#include "util.h"
#include "debug.h"

int errno;

double util_timestamp(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0f;
}

const char *util_basename(const char *name)
{
	int length;
	length = name ? strlen(name) : 0;
	if (!length)
		return ".";

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
	result = malloc(len);
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
			tmp = realloc(result, len);
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
				tmp = realloc(result, len);
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
								tmp = realloc(result, len);
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

				if (n_idx < 0)
					s_idx = t_idx;
				else
					s_idx = n_idx;

				break;
			}

			if (pattern[p_idx] == '\0') {
				if (idx + rlen >= len) {
					char *tmp;
					len += (rlen > len ? rlen : len);
					tmp = realloc(result, len);
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
	if (strncasecmp(uri, SCHEMA_FILE, len))
		return NULL;

	return uri + len;
}

/* End of a file */

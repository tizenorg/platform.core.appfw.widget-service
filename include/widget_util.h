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

#ifndef __WIDGET_SERVICE_UTIL_H
#define __WIDGET_SERVICE_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

extern char *widget_util_replace_string(const char *src, const char *pattern, const char *replace);
extern const char *widget_util_uri_to_path(const char *uri);
extern int widget_util_get_drm_fd(void *dpy, int *fd);
extern int widget_util_release_drm_fd(int fd);
extern const char *widget_util_basename(const char *name);

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

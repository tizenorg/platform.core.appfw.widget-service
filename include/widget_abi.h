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

#ifndef __WIDGET_SERVICE_ABI_H
#define __WIDGET_SERVICE_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 */
extern int widget_abi_init(void);

/**
 */
extern int widget_abi_fini(void);

/**
 */
extern const char *widget_abi_get_pkgname_by_abi(const char *abi);

/**
 */
extern const char *widget_abi_get_abi_by_pkgname(const char *pkgname);

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */

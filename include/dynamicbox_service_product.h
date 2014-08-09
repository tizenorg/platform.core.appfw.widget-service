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

#ifndef __DYNAMICBOX_SERVICE_PRODUCT_H
#define __DYNAMICBOX_SERVICE_PRODUCT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file dynamicbox-service_product.h
 * @brief  This file declares API of libdynamicbox-service product library
 */

/**
 * @addtogroup CAPI_DYNAMICBOX_SERVICE_PRODUCT_MODULE
 * @{
 */

/**
 * @brief Gets the path of the plug-in module.
 * @param[in] lbid Dynamicbox AppId
 * @privlevel platform
 * @privilege %http://developer.samsung.com/privilege/core/dynamicbox.info
 * @return char * type
 * @retval path String which is allocated on the heap
 * @retval @c NULL No libexec attribute
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_libexec(const char *lbid);

/**
 * @brief Find the lbid using its libexec
 * @details only if the dynamicbox should use the "libexec" attribute in its "<dynamicbox>" tag
 * @remars Only usable for inhouse dynamicboxes
 * @param[in] libexec so filename
 * @privlevel platform
 * @privilege %http://developer.samsung.com/privilege/core/dynamicbox.info
 * @return char *
 * @retval @c NULL if it fails to get pkgname
 * @retval address heap address of pkgname
 * @post return'd string should be released by "free()"
 */
extern char *dynamicbox_service_pkgname_by_libexec(const char *libexec);

/*@
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

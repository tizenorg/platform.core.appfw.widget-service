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

#ifndef __LIVEBOX_SERVICE_PRODUCT_H
#define __LIVEBOX_SERVICE_PRODUCT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file livebox-service_product.h
 * @brief  This file declares API of liblivebox-service product library
 */

/**
 * @addtogroup CAPI_LIVEBOX_SERVICE_PRODUCT_MODULE
 * @{
 */

/**
 * @brief Gets the path of the plug-in module.
 * @param[in] lbid Livebox AppId
 * @privlevel platform
 * @privilege %http://developer.samsung.com/privilege/core/dynamicbox.info
 * @return char * type
 * @retval path String which is allocated on the heap
 * @retval NULL No libexec attribute
 * @post Returned string must be free'd manually.
 */
extern char *livebox_service_libexec(const char *lbid);

/**
 * @brief Find the lbid using its libexec
 * @details only if the livebox should use the "libexec" attribute in its "<livebox>" tag
 * @remars Only usable for inhouse liveboxes
 * @param[in] libexec so filename
 * @privlevel platform
 * @privilege %http://developer.samsung.com/privilege/core/dynamicbox.info
 * @return char *
 * @retval NULL if it fails to get pkgname
 * @retval address heap address of pkgname
 * @post return'd string should be released via "free"
 */
extern char *livebox_service_pkgname_by_libexec(const char *libexec);

/*@
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

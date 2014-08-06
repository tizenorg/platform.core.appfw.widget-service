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

#ifndef __LIVEBOX_SERVICE_DOC_H__
#define __LIVEBOX_SERVICE_DOC_H__

/**
 * @defgroup DYNAMICBOX_SERVICE_MODULE Dynamic Box
 * @brief To get the information of installed Dynamic Boxes
 * @ingroup CAPI_DYNAMICBOX_FRAMEWORK 
 * @section DYNAMICBOX_SERVICE_MODULE_HEADER Required Header
 *   \#include <livebox-service.h>
 *   \#include <livebox-errno.h>
 * @section DYNAMICBOX_SERVICE_MODULE_OVERVIEW Overview
<H1>1. Livebox Service API</H1>
Tizen supports various kinds of API set for retrieving information of liveboxes.
Every liveboxes are installed via package manager utilities.
The information of installed liveboxes are stored in the system DB.

In some cases, application developer needs to send update request to its livebox.
To make it possible, this module provider such kinds of API set.

Those APIs are communicate with master service provider called data-provider-master. When it receives request from applications, it will find the proper service provider called data-provider-slave or application livebox provider.
If it finds proper service provider, it will forward the requests to the service provider.
Then each livebox application can gets update request.

<H2>1.1 APIs for retrieving information of liveboxes</H2>

Using following APIs, you can read XML data which are stored in DB.

\code
int livebox_service_get_size(int type, int *width, int *height);
int livebox_service_size_type(int width, int height);
int livebox_service_mouse_event(const char *lbid, int size_type);
int livebox_service_touch_effect(const char *lbid, int size_type);
int livebox_service_need_frame(const char *lbid, int size_type);
int livebox_service_get_pkglist(int (*cb)(const char *pkgid, const char *lbid, int is_prime, void *data), void *data);
int livebox_service_get_applist(const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data);
char *livebox_service_mainappid(const char *lbid);
int livebox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *lbid, int is_prime, void *data), void *data);
int livebox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *lbid, void *data), void *data);
char *livebox_service_pkgname(const char *lbid);
int livebox_service_is_primary(const char *lbid);
char *livebox_service_category(const char *lbid);
char *livebox_service_provider_name(const char *lbid);
char *livebox_service_setup_appid(const char *lbid);
char *livebox_service_appid(const char *lbid);
char *livebox_service_i18n_name(const char *lbid, const char *lang);
char *livebox_service_preview(const char *lbid, int size_type);
char *livebox_service_content(const char *lbid);
char *livebox_service_i18n_icon(const char *pkgid, const char *lang);
int livebox_service_nodisplay(const char *lbid);
char *livebox_service_abi(const char *lbid);
int livebox_service_is_enabled(const char *lbid);
char *livebox_service_lb_script_path(const char *lbid);
char *livebox_service_lb_script_group(const char *lbid);
char *livebox_service_pd_script_path(const char *lbid);
char *livebox_service_pd_script_group(const char *lbid);
int livebox_service_get_supported_sizes(const char *lbid, int *cnt, int *w, int *h);
int livebox_service_get_supported_size_types(const char *lbid, int *cnt, int *types);
int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);
int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);
int livebox_service_init(void);
int livebox_service_fini(void);
struct pkglist_handle *livebox_service_pkglist_create(const char *lbid, struct pkglist_handle *handle);
int livebox_service_get_pkglist_item(struct pkglist_handle *handle, char **lbid, char **pkgname, int *is_prime);
int livebox_service_pkglist_destroy(struct pkglist_handle *handle);
int livebox_service_get_instance_count(const char *lbid, const char *cluster, const char *category);
\endcode

<H2>1.2 APIs for communicating with running livebox instances</H2>
\code
int livebox_service_trigger_update(const char *lbid, const char *instance_id, const char *cluster, const char *category, int force);
int livebox_service_trigger_update_with_content(const char *lbid, const char *instance_id, const char *cluster, const char *category, const char *content, int force);
int livebox_service_change_period(const char *lbid, const char *instance_id, double period);
\endcode

<H1>2. Size of Livebox</H1>
Livebox has predefined size types.


<TABLE>
<TR>
<TD>Size Type</TD><TD>Size in pixels</TD><TD>Supported profile</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_1x1</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_2x1</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_2x2</TD><TD> - </TD><TD>Redwood, Wearable</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_4x1</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_4x2</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_4x3</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_4x4</TD><TD> - </TD><TD>Redwood, Kiran</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_4x5</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_4x6</TD><TD> - </TD><TD>Redwood</TD>
</TR>
</TABLE>

In case of samsung only types,
There are Easy Mode
<TABLE>
<TR>
<TD>Size Type</TD><TD>Size in pixels</TD><TD>Supported profile</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_EASY_1x1</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_EASY_3x1</TD><TD> - </TD><TD>Redwood</TD>
</TR>
<TR>
<TD>LB_SIZE_TYPE_EASY_3x3</TD><TD> - </TD><TD>Redwood</TD>
</TR>
</TABLE>

But in case of easy mode, it is not officially supported.
So you will not be able to create your liveboxes using these size types.
if you are developing the homescreen, you should care these types of liveboxes.

 *
 */

#endif

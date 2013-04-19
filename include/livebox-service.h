/*
 * Copyright 2013  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the "License");
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

#ifndef __LIVEBOX_SERVICE_H
#define __LIVEBOX_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define NR_OF_SIZE_LIST 13

enum livebox_size_type {
	LB_SIZE_TYPE_1x1 = 0x0001,
	LB_SIZE_TYPE_2x1 = 0x0002,
	LB_SIZE_TYPE_2x2 = 0x0004,
	LB_SIZE_TYPE_4x1 = 0x0008,
	LB_SIZE_TYPE_4x2 = 0x0010,
	LB_SIZE_TYPE_4x3 = 0x0020,
	LB_SIZE_TYPE_4x4 = 0x0040,
	LB_SIZE_TYPE_4x5 = 0x0080,
	LB_SIZE_TYPE_4x6 = 0x0100,
	LB_SIZE_TYPE_EASY_1x1 = 0x1000,
	LB_SIZE_TYPE_EASY_3x1 = 0x2000,
	LB_SIZE_TYPE_EASY_3x3 = 0x4000,
	LB_SIZE_TYPE_0x0 = 0x0800,
	LB_SIZE_TYPE_UNKNOWN = 0xFFFF,
};

enum livebox_script_event {
	LB_SCRIPT_ACCESS_EVENT		= 0x01000000,
	LB_SCRIPT_MOUSE_EVENT		= 0x02000000,
	LB_SCRIPT_KEY_EVENT		= 0x04000000,

	LB_SCRIPT_ACCESS_HIGHLIGHT	= LB_SCRIPT_ACCESS_EVENT | 0x00000001,
	LB_SCRIPT_ACCESS_HIGHLIGHT_NEXT	= LB_SCRIPT_ACCESS_EVENT | 0x00000002,
	LB_SCRIPT_ACCESS_HIGHLIGHT_PREV	= LB_SCRIPT_ACCESS_EVENT | 0x00000004,
	LB_SCRIPT_ACCESS_ACTIVATE	= LB_SCRIPT_ACCESS_EVENT | 0x00000008,
	LB_SCRIPT_ACCESS_VALUE_CHANGE	= LB_SCRIPT_ACCESS_EVENT | 0x00000010,
	LB_SCRIPT_ACCESS_SCROLL		= LB_SCRIPT_ACCESS_EVENT | 0x00000020,
	LB_SCRIPT_ACCESS_UNHIGHLIGHT	= LB_SCRIPT_ACCESS_EVENT | 0x00000040,

	LB_SCRIPT_MOUSE_DOWN	= LB_SCRIPT_MOUSE_EVENT | 0x00001000,
	LB_SCRIPT_MOUSE_MOVE	= LB_SCRIPT_MOUSE_EVENT | 0x00002000,
	LB_SCRIPT_MOUSE_UP	= LB_SCRIPT_MOUSE_EVENT | 0x00004000,
	LB_SCRIPT_MOUSE_IN	= LB_SCRIPT_MOUSE_EVENT | 0x00008000,
	LB_SCRIPT_MOUSE_OUT	= LB_SCRIPT_MOUSE_EVENT | 0x00010000,

	LB_SCRIPT_KEY_DOWN	= LB_SCRIPT_KEY_EVENT | 0x00020000,
	LB_SCRIPT_KEY_UP	= LB_SCRIPT_KEY_EVENT | 0x00040000,
};

enum livebox_access_status {
	LB_ACCESS_STATUS_DONE,
	LB_ACCESS_STATUS_FIRST, /*!< Reach to the first item */
	LB_ACCESS_STATUS_LAST, /*!< Reach to the last item */
	LB_ACCESS_STATUS_READ, /* TTS done */
	LB_ACCESS_STATUS_ERROR,
};

/*!
 * \brief
 * \param[in] type
 * \param[out] width
 * \param[out] height
 * \return 0 for success
 */
extern int livebox_service_get_size(int type, int *width, int *height);

/*!
 * \brief
 * \param[in] width
 * \param[in] height
 * \return Type of a livebox
 */
extern int livebox_service_size_type(int width, int height);

/*!
 * \brief
 * \param[in] pkgid Livebox's appid
 * \return true(1) / false(0)
 */
extern int livebox_service_mouse_event(const char *pkgid);

/*!
 * \brief
 * \param[in] pkgid Livebox's appid
 * \param[in] size_type
 * \return true(1) / false(0)
 */
extern int livebox_service_touch_effect(const char *pkgid, int size_type);

/*!
 * \brief
 * \param[in] pkgid Livebox's appid
 * \param[in] size_type
 * \return true(1) / false(0)
 */
extern int livebox_service_need_frame(const char *pkgid, int size_type);

/*!
 * \brief
 * \param[in] pkgname Livebox package name
 * \param[in] id Set NULL if you don't know what the Id is.
 * \param[in] cluster Cluster name. Default NULL
 * \param[in] category Category name, Default NULL
 * \param[in] force 1 if you want to update your livebox even if the provider is paused or 0. 0 is default
 */
extern int livebox_service_trigger_update(const char *pkgname, const char *id, const char *cluster, const char *category, int force);

/*!
 * \brief Change the period of given livebox instance
 * \param[in] pkgname Livebox package name
 * \param[in] id Livebox instance id
 * \param[in] period New update period in sec
 */
extern int livebox_service_change_period(const char *pkgname, const char *id, double period);

/*!
 * \brief Synchronous package list getter
 *        callback (appid, lbid, is_prime)
 *        appid == Application Package ID (pkgname)
 *        lbid = Livebox Package ID
 *        is_prime = 1 if the livebox is default one for associated application package
 * \param[in] cb Callback function
 * \return Count of livebox packages or errno (-EIO, -EINVAL)
 */
extern int livebox_service_get_pkglist(int (*cb)(const char *appid, const char *lbid, int is_prime, void *data), void *data);

/*!
 * \brief Get the pkgname of a primary livebox using given lbid or pkgid.
 * \param[in] id Livebox Id or Package Id
 * \return pkgname String which is allocated on the heap
 */
extern char *livebox_service_pkgname(const char *id);

/*!
 * \brief Check the pirmary flag of given livebox Id.
 * \param[in] lbid Livebox Id
 * \return int 0 if is not a primary or 1
 */
extern int livebox_service_is_primary(const char *lbid);

/*!
 * \brief Get the name of a livebox (provider name == livebox appid), you have to release the return value after use it
 * \param[in] Livebox AppID
 * \return Livebox AppId (Same with input)
 */
extern char *livebox_service_provider_name(const char *lbid);

/*!
 * \brief Get the appId of setup app which is specified by given livebox Id's manifest.
 * \param[in] Livebox AppId
 * \return Setup AppId if exists or NULL
 */
extern char *livebox_service_setup_appid(const char *lbid);

/*!
 *\brief Get the application pkgname(appid) of given livebox package name, <manifest package="AAA">
 * \param[in] pkgname
 * \return appid String which is allocated on the heap
 */
extern char *livebox_service_appid(const char *pkgname);

/*!
 * \brief Internationalized name of given livebox package.
 *        USER must has to do "free" after using the returned string.
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return name if it fails to get name, returns NULL, or allocated heap address.
 */
extern char *livebox_service_i18n_name(const char *pkgid, const char *lang);

/*!
 * \brief
 * \param[in] pkgid livebox ID. NOT the UI-APP ID and PACKAGE ID
 * \param[in] size_type
 * \return path preview image path
 */
extern char *livebox_service_preview(const char *pkgid, int size_type);

/*!
 * \brief Get the default content string of the given livebox
 * \param[in] pkgid Livebox ID. Not the UI-APP ID and PACKAGE ID
 * \return content content string
 */
extern char *livebox_service_content(const char *pkgid);

/*!
 * \brief Internationalized icon path of given livebox package.
 *        USER must has to do "free" after using the returned string.
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return name if it fails to get path of an icon, returns NULL, or allocated heap address.
 */
extern char *livebox_service_i18n_icon(const char *pkgid, const char *lang);

/*!
 * \brief Get the path of the plug-in module
 * \param[in] appid Package name of a livebox
 * \return path String which is allocated on the heap
 */
extern char *livebox_service_libexec(const char *appid);

/*!
 * \brief Get the "nodisplay" value
 * \param[in] pkgid Livebox package id.
 * \return true/false
 */
extern int livebox_service_nodisplay(const char *pkgid);

/*!
 * \brief Check the status of the livebox
 * \param[in] lbid Livebox Id
 * \return int 1 enabled, 0 disabled
 */
extern int livebox_service_is_enabled(const char *lbid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_lb_script_path(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_lb_script_group(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_pd_script_path(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_pd_script_group(const char *pkgid);

/*!
 * \brief
 * \return
 */
extern int livebox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h);

/*!
 * \brief
 * \return
 */
extern int livebox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types);

/*!
 */
extern int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/*!
 */
extern int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);

/*!
 * \brief
 * \return
 */
extern int livebox_service_init(void);

/*!
 * \brief
 * \return
 */
extern int livebox_service_fini(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */

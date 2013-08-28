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

#ifndef __LIVEBOX_SERVICE_H
#define __LIVEBOX_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ingroup CAPI_LIVEBOX_FRAMEWORK Tizen livebox framework 
 * \{
 * \defgroup LIVEBOX_SERVICE Livebox Service API
 * \{
 * \section Intro Introduction
 * Livebox Service API supporting various kinds of DB access operations.
 * \subsection SubSectionA Sub section A
 * \section SectionB Section B
 * \subsection SubSectionB Sub section B
 * \section SectionC Section C
 * \subsection SubSectionC Sub section C
 */

/*!
 * \brief
 * Maximum number of supported livebox size type
 */
#define NR_OF_SIZE_LIST 13

/*!
 * \brief
 * List of supporting livebox size types
 */
enum livebox_size_type {
	LB_SIZE_TYPE_1x1 = 0x0001, /*!< 175x175 */
	LB_SIZE_TYPE_2x1 = 0x0002, /*!< 354x175 */
	LB_SIZE_TYPE_2x2 = 0x0004, /*!< 354x354 */
	LB_SIZE_TYPE_4x1 = 0x0008, /*!< 712x175 */
	LB_SIZE_TYPE_4x2 = 0x0010, /*!< 712x354 */
	LB_SIZE_TYPE_4x3 = 0x0020, /*!< 712x533 */
	LB_SIZE_TYPE_4x4 = 0x0040, /*!< 712x712 */
	LB_SIZE_TYPE_4x5 = 0x0080, /*!< 712x891 */
	LB_SIZE_TYPE_4x6 = 0x0100, /*!< 712x1070 */
	LB_SIZE_TYPE_EASY_1x1 = 0x1000, /*!< 224x215 */
	LB_SIZE_TYPE_EASY_3x1 = 0x2000, /*!< 680x215 */
	LB_SIZE_TYPE_EASY_3x3 = 0x4000, /*!< 680x653 */
	LB_SIZE_TYPE_0x0 = 0x0800, /*!< 720x1280 */
	LB_SIZE_TYPE_UNKNOWN = 0xFFFF /*!< Error */
};

/*!
 * \brief
 * Script event of livebox
 * This enumeration value will be used for script plugin of data-provider-master.
 * master will send down these accessibility events to the script plugin.
 */
enum livebox_script_event {
	LB_SCRIPT_ACCESS_EVENT		= 0x01000000, /*!< Mask value */
	LB_SCRIPT_MOUSE_EVENT		= 0x02000000, /*!< Mask value */
	LB_SCRIPT_KEY_EVENT		= 0x04000000, /*!< Mask value */

	LB_SCRIPT_ACCESS_HIGHLIGHT	= LB_SCRIPT_ACCESS_EVENT | 0x00000001, /*!< Highlight */
	LB_SCRIPT_ACCESS_HIGHLIGHT_NEXT	= LB_SCRIPT_ACCESS_EVENT | 0x00000002, /*!< Move Highlight focus to the next object */
	LB_SCRIPT_ACCESS_HIGHLIGHT_PREV	= LB_SCRIPT_ACCESS_EVENT | 0x00000004, /*!< Move Highlight focus to the prev object */
	LB_SCRIPT_ACCESS_ACTIVATE	= LB_SCRIPT_ACCESS_EVENT | 0x00000008, /*!< Activate the highlighted object */
	LB_SCRIPT_ACCESS_ACTION		= LB_SCRIPT_ACCESS_EVENT | 0x00000010, /*!< Do specified action for the highlighted object */
	LB_SCRIPT_ACCESS_SCROLL		= LB_SCRIPT_ACCESS_EVENT | 0x00000020, /*!< Scroll operation */
	LB_SCRIPT_ACCESS_UNHIGHLIGHT	= LB_SCRIPT_ACCESS_EVENT | 0x00000040, /*!< Remove highlight */

	LB_SCRIPT_MOUSE_DOWN	= LB_SCRIPT_MOUSE_EVENT | 0x00001000, /*!< Mouse down */
	LB_SCRIPT_MOUSE_MOVE	= LB_SCRIPT_MOUSE_EVENT | 0x00002000, /*!< Mouse move */
	LB_SCRIPT_MOUSE_UP	= LB_SCRIPT_MOUSE_EVENT | 0x00004000, /*!< Mouse up */
	LB_SCRIPT_MOUSE_IN	= LB_SCRIPT_MOUSE_EVENT | 0x00008000, /*!< Mouse in */
	LB_SCRIPT_MOUSE_OUT	= LB_SCRIPT_MOUSE_EVENT | 0x00010000, /*!< Mouse out */

	LB_SCRIPT_KEY_DOWN	= LB_SCRIPT_KEY_EVENT | 0x00020000, /*!< Key pressed */
	LB_SCRIPT_KEY_UP	= LB_SCRIPT_KEY_EVENT | 0x00040000 /*!< Key released */
};

/*!
 * \brief Result of Accessibility event processing
 * Reference the libprovider & liblivebox-viewer
 */
enum livebox_access_status {
	LB_ACCESS_STATUS_ERROR = 0x80000000, /*!< Mask value */
	LB_ACCESS_STATUS_DONE = 0x00000000, /*!< Successfully finished */
	LB_ACCESS_STATUS_FIRST, /*!< Reach to the first item */
	LB_ACCESS_STATUS_LAST, /*!< Reach to the last item */
	LB_ACCESS_STATUS_READ /*!< TTS is done */
};

/*!
 * \brief
 * Handle of Package List
 */
struct pkglist_handle;

/*!
 * \brief Get the pixel size of given size type.
 * \param[in] type Size type
 * \param[out] width Pixel size width
 * \param[out] height Pixel size height
 * \return LB_STATUS_SUCCESS for success
 * \sa livebox_size_type
 * \sa livebox_service_size_type
 */
extern int livebox_service_get_size(int type, int *width, int *height);

/*!
 * \brief Get the size type for given pixel size.
 * \param[in] width Pixel size width
 * \param[in] height Pixel size height
 * \return int Size type of given pixel size., LB_SIZE_TYPE_UNKNOWN if the given pixel size is not valid.
 * \sa livebox_size_type
 * \sa livebox_service_get_size
 */
extern int livebox_service_size_type(int width, int height);

/*!
 * \brief Is this livebox support the mouse event of livebox content?
 * \param[in] pkgid Livebox's appid
 * \return true(1) / false(0)
 * \sa livebox_service_touch_effect
 * \sa livebox_service_need_frame
 */
extern int livebox_service_mouse_event(const char *pkgid);

/*!
 * \brief Is this livebox requires touch effect?
 *        If this API returns true(1), the viewer should make touch effect when a user click the livebox.
 * \param[in] pkgid Livebox's appid
 * \param[in] size_type
 * \return true(1) / false(0)
 * \sa livebox_service_mouse_event
 * \sa livebox_service_need_frame
 */
extern int livebox_service_touch_effect(const char *pkgid, int size_type);

/*!
 * \brief Is this livebox requires decoration frame?
 *        If this API returns true(1), the viewer should make decoration border on the livebox content.
 * \param[in] pkgid Livebox's appid
 * \param[in] size_type
 * \return true(1) / false(0)
 * \sa livebox_service_mouse_event
 * \sa livebox_service_touch_effect
 */
extern int livebox_service_need_frame(const char *pkgid, int size_type);

/*!
 * \brief Trigger the update event for given livebox instance
 * \param[in] pkgname Livebox package name
 * \param[in] id Set NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event.
 * \param[in] cluster Cluster name. Default NULL
 * \param[in] category Category name, Default NULL
 * \param[in] force 1 if you want to update your livebox even if the provider is paused or 0. 0 is default
 * \return int
 */
extern int livebox_service_trigger_update(const char *pkgname, const char *id, const char *cluster, const char *category, int force);

/*!
 * \brief Change the update period of given livebox instance
 * \param[in] pkgname Livebox package name
 * \param[in] id Livebox instance id
 * \param[in] period New update period in sec
 * \return int LB_STATUS_SUCCESS or LB_STATUS_ERROR_INVALID, LB_STATUS_ERROR_FAULT, LB_STATUS_ERROR_MEMORY, ...
 */
extern int livebox_service_change_period(const char *pkgname, const char *id, double period);

/*!
 * \brief Synchronous package list getter
 *        callback (appid, lbid, is_prime)
 *        appid == Application Package ID (pkgname)
 *        lbid = Livebox Package ID
 *        is_prime = 1 if the livebox is default one for associated application package
 *        If the callback returns negative value, the list crawling will be stopped
 * \param[in] cb Callback function
 * \param[in] data Callback data
 * \return Count of livebox packages or LB_STATUS_ERROR_IO, LB_STATUS_ERROR_INVALID
 */
extern int livebox_service_get_pkglist(int (*cb)(const char *pkgid, const char *lbid, int is_prime, void *data), void *data);

/*!
 * \brief Synchronous uiapp list getter.
 *        callback (appid, data)
 *        This function will retrieve all UI Apps in a package which has given livebox appid(lbid).
 *        If you need to get all ui-app list, using a livebox appid, this function is able to help you.
 * \param[in] lbid Livebox App Id
 * \param[in] cb Callback function
 * \param[in] data Callback Data
 * \return int LB_STATUS_SUCCESS or LB_STATUS_ERROR_INVALID, LB_STATUS_ERROR_IO, LB_STATUS_ERROR_MEMORY, LB_STATUS_ERROR_FAULT
 */
extern int livebox_service_get_applist(const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data);

/*!
 * \brief Get the MAIN application Id of given livebox package Id.
 * \param[in] lbid Livebox Package Id
 * \return string Main App Id
 */
extern char *livebox_service_mainappid(const char *lbid);

/*!
 * \brief Synchronous package list getter
 *	  callback (lbid, is_prime)
 *	  lbid == Livebox Package Id
 *	  is_prime = 1 if the livebox is default one for selected package
 *        If the callback returns negative value, the list crawling will be stopped
 * \param[in] pkgid Package Id (Not the UI App Id)
 * \param[in] cb Callback function
 * \param[in] data Callback data
 * \return Count of livebox packages or LB_STATUS_ERROR
 * \sa livebox_service_get_pkglist
 */
extern int livebox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *lbid, int is_prime, void *data), void *data);

/*!
 * \brief Get the appid of a primary livebox using given lbid or pkgid or UI appid.
 * \param[in] id Livebox Id or Package Id
 * \return pkgname String which is allocated on the heap
 * \sa livebox_service_appid
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
 * \return Livebox AppId which is allocated on the heap
 */
extern char *livebox_service_provider_name(const char *lbid);

/*!
 * \brief Get the appId of setup app which is specified by given livebox Id's manifest.
 *        This setup app should be launched before add the livebox to get the content_info
 * \param[in] Livebox AppId
 * \return Setup AppId if exists or NULL
 */
extern char *livebox_service_setup_appid(const char *lbid);

/*!
 * \brief Get the Package Id (Not the UI App Id) of given livebox, &lt;manifest package="AAA"&gt;
 * \param[in] lbid Livebox AppId 
 * \return appid String which is allocated on the heap
 * \sa livebox_service_pkgname
 */
extern char *livebox_service_appid(const char *lbid);

/*!
 * \brief Internationalized name of given livebox package.
 *        USER must has to do "free" after using the returned string.
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return name if it fails to get name, returns NULL, or allocated heap address.
 */
extern char *livebox_service_i18n_name(const char *pkgid, const char *lang);

/*!
 * \brief Get the preview image path of given size type.
 *        This function will returns i18nized preview image path.
 * \param[in] pkgid livebox ID. NOT the UI-APP ID and PACKAGE ID
 * \param[in] size_type Livebox size type.
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
extern char *livebox_service_libexec(const char *lbid);

/*!
 * \brief Get the "nodisplay" value
 * \param[in] pkgid Livebox package id.
 * \return true/false
 */
extern int livebox_service_nodisplay(const char *pkgid);

/*!
 * \brief Check the status of the livebox
 *        Currently this API is not implemented. just returns 1 all the time.
 * \param[in] lbid Livebox Id
 * \return int 1 enabled, 0 disabled
 */
extern int livebox_service_is_enabled(const char *lbid);

/*!
 * \brief Get the script file of livebox.
 * \param[in] pkgid Livebox Package Id
 * \return string script file path
 * \sa livebox_service_lb_script_group
 */
extern char *livebox_service_lb_script_path(const char *lbid);

/*!
 * \brief Get the script group of livebox
 * \param[in] pkgid Livebox Package Id
 * \return Group name of livebox
 * \sa livebox_service_lb_script_path
 */
extern char *livebox_service_lb_script_group(const char *lbid);

/*!
 * \brief Get the script file path of given livebox package.
 * \param[in] pkgid Livebox Package Id
 * \return string script file path.
 * \sa livebox_service_pd_script_group
 */
extern char *livebox_service_pd_script_path(const char *lbid);

/*!
 * \brief Get the group name for script file to load it.
 * \param[in] pkgid Livebox package Id
 * \return string script group name
 * \sa livebox_service_pd_script_path
 */
extern char *livebox_service_pd_script_group(const char *lbid);

/*!
 * \brief
 * \param[in] pkgid Livebox Package Id
 * \param[inout] cnt Count of array w and h
 * \param[inout] w Width array
 * \param[inout] h Height array
 * \return LB_STATUS_SUCCESS if succeed to get supported size list or LB_STATUS_ERROR_IO, LB_STATUS_ERROR_INVALID
 * \sa livebox_service_get_supported_size_types
 */
extern int livebox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h);

/*!
 * \brief Get the supported size list of given pkgid
 * \param[in] pkgid Package Name of livebox
 * \param[inout] cnt IN size of types array, OUT result count of types array
 * \param[out] types array of types
 * \return int
 * \sa livebox_service_get_supported_sizes
 */
extern int livebox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types);

/*!
 * \brief Get the category list of given cluster
 * \param[in] cluster Cluster name
 * \param[in] cb Callback function
 * \param[in] data Callback data
 * \return int LB_STATUS_SUCCESS or LB_STATUS_ERROR_IO, LB_STATUS_ERROR_INVALID
 * \sa livebox_service_enumerate_cluster_list
 */
extern int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/*!
 * \brief Get the cluster list
 * \param[in] cb Callback function for retrieving the cluster list
 * \param[in] data Callback data
 * \return int Count of cluster
 * \sa livebox_service_enumerate_category_list
 */
extern int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);

/*!
 * \brief Initialize the Livebox service API
 * \return int LB_STATUS_SUCCESS if succeed to initialize or LB_STATUS_ERROR_IO
 * \sa livebox_service_fini
 */
extern int livebox_service_init(void);

/*!
 * \brief Finalize the livebox service API
 * \return int LB_STATUS_SUCCESS if succeed to finalize or LB_STATUS_ERROR_IO
 * \sa livebox_service_init
 */
extern int livebox_service_fini(void);

/*!
 * \brief Create a handle for getting the package list.
 * \param[in] pkgid Livebox Package Id
 * \param[in] handle NULL if you call this first. or it will be reseted.
 * \return handle
 * \sa livebox_service_pkglist_destroy
 */
extern struct pkglist_handle *livebox_service_pkglist_create(const char *pkgid, struct pkglist_handle *handle);

/*!
 * \brief Get the appid & package name & is_prime flag.
 * \param[in] handle
 * \param[out] appid
 * \param[out] pkgname
 * \param[out] is_prime
 * \return int
 * \sa livebox_service_pkglist_create
 * \sa livebox_service_pkglist_destroy
 */
extern int livebox_service_get_pkglist_item(struct pkglist_handle *handle, char **appid, char **pkgname, int *is_prime);

/*!
 * \brief
 * \param[in] handle
 * \return int
 * \sa livebox_service_pkglist_create
 */
extern int livebox_service_pkglist_destroy(struct pkglist_handle *handle);

/*!
 * \}
 */

/*!
 * \}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

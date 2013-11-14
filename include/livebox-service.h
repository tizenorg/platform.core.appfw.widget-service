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
 * \addtogroup CAPI_LIVEBOX_SERVICE_MODULE
 * \{
 */

/*!
 * \brief
 * Maximum number of supported livebox size type
 */
#define NR_OF_SIZE_LIST 13

#define CH_IDX(a, idx) (((char *)(a))[(idx)])
#define IS_INHOUSE_LIVEBOX(abi)	((CH_IDX(abi, 0) == 'c' || CH_IDX(abi, 0) == 'C') && (CH_IDX(abi, 1) == '\0' || ((CH_IDX(abi, 1) == 'p' || CH_IDX(abi, 1) == 'P') && (CH_IDX(abi, 2) == 'p' || CH_IDX(abi, 2) == 'P') && CH_IDX(abi, 3) == '\0')))

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
 * \details N/A
 * \remarks N/A
 * \param[in] type Size type
 * \param[out] width Pixel size width
 * \param[out] height Pixel size height
 * \return int
 * \retval LB_STATUS_SUCCESS for success
 * \pre N/A
 * \post N/A
 * \see livebox_size_type
 * \see livebox_service_size_type
 */
extern int livebox_service_get_size(int type, int *width, int *height);

/*!
 * \brief Get the size type for given pixel size.
 * \details N/A
 * \remarks N/A
 * \param[in] width Pixel size width
 * \param[in] height Pixel size height
 * \return int
 * \retval LB_SIZE_TYPE_WxH Size type of given pixel size.
 * \retval LB_SIZE_TYPE_UNKNOWN if the given pixel size is not valid.
 * \pre N/A
 * \post N/A
 * \see livebox_size_type
 * \see livebox_service_get_size
 */
extern int livebox_service_size_type(int width, int height);

/*!
 * \brief Is this livebox support the mouse event of livebox content?
 * \details N/A
 * \remarks N/A
 * \param[in] pkgid Livebox's appid
 * \return int
 * \retval 1 if the box requires mouse event
 * \retval 0 if the box doesn't requires mouse event
 * \pre N/A
 * \post N/A
 * \see livebox_service_touch_effect
 * \see livebox_service_need_frame
 */
extern int livebox_service_mouse_event(const char *pkgid);

/*!
 * \brief Is this livebox requires touch effect?
 *        If this API returns true(1), the viewer should make touch effect when a user click the livebox.
 * \param[in] pkgid Livebox's appid
 * \param[in] size_type
 * \return int
 * \retval 1 if the box requires touch effect
 * \retval 0 if the box doesn't requires touch effect, the box will make touch effect itself
 * \pre N/A
 * \post N/A
 * \see livebox_service_mouse_event
 * \see livebox_service_need_frame
 */
extern int livebox_service_touch_effect(const char *pkgid, int size_type);

/*!
 * \brief Is this livebox requires decoration frame?
 *        If this API returns true(1), the viewer should make decoration border on the livebox content.
 * \details N/A
 * \remarks N/A
 * \param[in] pkgid Livebox's appid
 * \param[in] size_type
 * \return int
 * \retval 1 If the box requires frame for decorating its contents
 * \retval 0 If the box doesn't requires frame
 * \see livebox_service_mouse_event
 * \see livebox_service_touch_effect
 */
extern int livebox_service_need_frame(const char *pkgid, int size_type);

/*!
 * \brief Trigger the update event for given livebox instance
 * \details N/A
 * \remarks N/A
 * \param[in] pkgname Livebox package name
 * \param[in] id Set NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event.
 * \param[in] cluster Cluster name. Default NULL
 * \param[in] category Category name, Default NULL
 * \param[in] force 1 if you want to update your livebox even if the provider is paused or 0. 0 is default
 * \return int
 * \retval LB_STATUS_ERROR_INVALID Invalid argument
 * \retval LB_STATUS_ERROR_CANCEL Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * \retval LB_STATUS_ERROR_MEMORY Memory is not enough to make request
 * \retval LB_STATUS_ERROR_FAULT Failed to create a request packet
 * \retval LB_STATUS_SUCCESS Successfully requested
 * \see N/A
 */
extern int livebox_service_trigger_update(const char *pkgname, const char *id, const char *cluster, const char *category, int force);

/*!
 * \brief Change the update period of given livebox instance
 * \details N/A
 * \remarks N/A
 * \param[in] pkgname Livebox package name
 * \param[in] id Livebox instance id
 * \param[in] period New update period in sec
 * \return int
 * \retval LB_STATUS_SUCCESS Successfully changed(requested)
 * \retval LB_STATUS_ERROR_INVALID Invalid argument
 * \retval LB_STATUS_ERROR_FAULT Failed to create a request packet
 * \retval LB_STATUS_ERROR_MEMORY Not enough memory
 * \see N/A
 */
extern int livebox_service_change_period(const char *pkgname, const char *id, double period);

/*!
 * \brief Synchronous package list getter
 * \details
 *        callback (appid, lbid, is_prime)
 *        appid == Application Package ID (pkgname)
 *        lbid = Livebox Package ID
 *        is_prime = 1 if the livebox is default one for associated application package
 *        If the callback returns negative value, the list crawling will be stopped
 * \remarks N/A
 * \param[in] cb Callback function
 * \param[in] data Callback data
 * \return int
 * \retval LB_STATUS_ERROR_IO Failed to access DB
 * \retval LB_STATUS_ERROR_INVALID Invalid argument
 * \retval count count of livebox packages
 * \pre N/A
 * \post N/A
 * \see livebox_service_get_pkglist_by_pkgid
 */
extern int livebox_service_get_pkglist(int (*cb)(const char *pkgid, const char *lbid, int is_prime, void *data), void *data);

/*!
 * \brief Synchronous uiapp list getter.
 * \details
 *        callback (appid, data)
 *        This function will retrieve all UI Apps in a package which has given livebox appid(lbid).
 *        If you need to get all ui-app list, using a livebox appid, this function is able to help you.
 * \remarks N/A
 * \param[in] lbid Livebox App Id
 * \param[in] cb Callback function
 * \param[in] data Callback Data
 * \return int
 * \retval LB_STATUS_SUCCESS
 * \retval LB_STATUS_ERROR_INVALID
 * \retval LB_STATUS_ERROR_IO
 * \retval LB_STATUS_ERROR_MEMORY
 * \retval LB_STATUS_ERROR_FAULT
 * \pre N/A
 * \post N/A
 * \see N/A
 */
extern int livebox_service_get_applist(const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data);

/*!
 * \brief Get the MAIN application Id of given livebox package Id.
 * \details N/A
 * \remarks N/A
 * \param[in] lbid Livebox Package Id
 * \return char *
 * \retval NULL if it fails to get main application Id (UI-APPID)
 * \retval appid Main application Id
 * \pre N/A
 * \post N/A
 */
extern char *livebox_service_mainappid(const char *lbid);

/*!
 * \brief Synchronous package list getter
 * \details
 *	  callback (lbid, is_prime)
 *	  lbid == Livebox Package Id
 *	  is_prime = 1 if the livebox is default one for selected package
 *        If the callback returns negative value, the list crawling will be stopped
 * \remarks N/A
 * \param[in] pkgid Package Id (Not the UI App Id)
 * \param[in] cb Callback function
 * \param[in] data Callback data
 * \return int
 * \retval Count of livebox packages
 * \retval LB_STATUS_ERROR_INVALID Invalid argument
 * \retval LB_STATUS_ERROR_IO Failed to access DB
 * \see livebox_service_get_pkglist
 */
extern int livebox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *lbid, int is_prime, void *data), void *data);

/*!
 * \brief Get the lbid of a primary livebox using given lbid or pkgid or UI appid.
 * \details N/A
 * \remarks N/A
 * \param[in] id Livebox Id or Package Id
 * \return char *
 * \retval NULL Failed to get primary lbid
 * \retval lbid Primary livebox Id. which is allocated in the heap
 * \pre Must be released returned string by manually
 * \post N/A
 * \see livebox_service_appid
 */
extern char *livebox_service_pkgname(const char *id);

/*!
 * \brief Check the pirmary flag of given livebox Id.
 * \details N/A
 * \param[in] lbid Livebox Id
 * \return int
 * \retval 0 if is not a primary
 * \retval 1 if it is a primary livebox
 * \pre N/A
 * \post N/A
 * \see N/A
 */
extern int livebox_service_is_primary(const char *lbid);

/*!
 * \brief Get the name of a livebox (provider name == livebox appid), you have to release the return value after use it
 * \details
 *    OSP livebox has provider process for each livebox instances.
 *    To get the provider's package name, you can use this API.
 *    If the given lbid is inhouse livebox, the return string will be the same with argument but it is allocated in the heap.
 * \param[in] lbid AppID
 * \return char *
 * \retval NULL failed to get provider name
 * \retval lbid Livebox AppId which is allocated on the heap
 * \pre N/A
 * \post Return'd string must be free'd manually
 * \see N/A
 */
extern char *livebox_service_provider_name(const char *lbid);

/*!
 * \brief Get the appId of setup app which is specified by given livebox Id's manifest.
 *        This setup app should be launched before add the livebox to get the content_info
 * \details
 *    This function should be called before add a livebox.
 *    To determine the content information string.
 * \param[in] lbid AppId
 * \return char *
 * \retval NULL there is no setup application
 * \retval appid AppId if exists or NULL
 * \pre N/A
 * \post return'd string must be free'd manually
 * \see N/A
 */
extern char *livebox_service_setup_appid(const char *lbid);

/*!
 * \brief Get the Package Id (Not the UI App Id) of given livebox, &lt;manifest package="AAA"&gt;
 * \details N/A
 * \param[in] lbid Livebox AppId 
 * \return char *
 * \retval appid String which is allocated on the heap
 * \retval NULL invalid appid
 * \pre N/A
 * \post return'd string must be free'd manually
 * \see livebox_service_pkgname
 */
extern char *livebox_service_appid(const char *lbid);

/*!
 * \brief Internationalized name of given livebox package.
 * \details N/A
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return char *
 * \retval name if it fails to get name
 * \retval NULL, or allocated heap address.
 * \pre N/A
 * \post Return'd string must be free'd by manually
 * \see livebox_service_i18n_icon
 * \see livebox_service_preview
 */
extern char *livebox_service_i18n_name(const char *pkgid, const char *lang);

/*!
 * \brief Get the preview image path of given size type.
 *        This function will returns i18nized preview image path.
 * \details N/A
 * \param[in] pkgid livebox ID. NOT the UI-APP ID and PACKAGE ID
 * \param[in] size_type Livebox size type.
 * \return char *
 * \retval path preview image path
 * \retval NULL if there is no preview image file
 * \pre N/A
 * \post Return'd string must be free'd manually
 * \see livebox_service_i18n_icon
 * \see livebox_service_i18n_name
 */
extern char *livebox_service_preview(const char *pkgid, int size_type);

/*!
 * \brief Get the default content string of the given livebox
 * \details
 *    If a user defins default content string in the manifest file (.xml)
 *    This API will returns it.
 * \param[in] pkgid Livebox ID. Not the UI-APP ID and PACKAGE ID
 * \return char *
 * \retval content content string
 * \retval NULL there is no specified content string
 * \pre manifest has the default content string. &lt;content&gt;Default content string&lt;content&gt;
 * \post return'd string must be free'd manually
 * \see N/A
 */
extern char *livebox_service_content(const char *pkgid);

/*!
 * \brief Internationalized icon path of given livebox package.
 *        USER must has to do "free" after using the returned string.
 * \details N/A
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return char *
 * \retval name allocated heap address.
 * \retval NULL if it fails to get path of an icon
 * \pre N/A
 * \post return'd string must be free'd manually
 * \see livebox_service_i18n_name
 * \see livebox_service_preview
 */
extern char *livebox_service_i18n_icon(const char *pkgid, const char *lang);

/*!
 * \brief Get the path of the plug-in module
 * \details N/A
 * \param[in] lbid Package name of a livebox
 * \return char *
 * \retval path String which is allocated on the heap
 * \retval NULL if there is no libexec attribute
 * \pre N/A
 * \post return'd string must be free'd manually
 * \see N/A
 */
extern char *livebox_service_libexec(const char *lbid);

/*!
 * \brief Get the "nodisplay" value
 * \details N/A
 * \remarks N/A
 * \param[in] pkgid Livebox package id.
 * \return int
 * \retval 1 if the box should not be listed by the livebox list app
 * \retval 0 box should be listed
 * \pre livebox tag includes "nodisplay" attribute.
 * \post N/A
 * \see N/A
 */
extern int livebox_service_nodisplay(const char *pkgid);

/*!
 * \brief Get the "ABI" of given package
 * \details N/A
 * \remarks N/A
 * \param[in] lbid Livebox package id.
 * \return char *
 * \retval abi String which is allocated in the heap
 * \retval NULL if failed to get ABI of given livebox.
 * \pre N/A
 * \post return'd string must be free'd manually
 * \see N/A
 */
extern char *livebox_service_abi(const char *lbid);

/*!
 * \brief Check the status of the livebox
 *        Currently this API is not implemented. just returns 1 all the time.
 * \details N/A
 * \remarks This API is not implemented. it will always returns 1
 * \param[in] lbid Livebox Id
 * \return int
 * \retval 1 enabled
 * \retval 0 disabled
 * \pre N/A
 * \post N/A
 * \see N/A
 */
extern int livebox_service_is_enabled(const char *lbid);

/*!
 * \brief Get the script file of livebox.
 * \details N/A
 * \remarks N/A
 * \param[in] lbid Livebox Package Id
 * \return char *
 * \retval string script file path
 * \retval NULL if there is not specified script file
 * \pre livebox should be developed as script type
 * \post Return'd string must be free'd manually
 * \see livebox_service_lb_script_group
 */
extern char *livebox_service_lb_script_path(const char *lbid);

/*!
 * \brief Get the script group of livebox
 * \details N/A
 * \remarks N/A
 * \param[in] lbid Livebox Package Id
 * \return char *
 * \retval Group name of livebox
 * \retval NULL if there is no group defined
 * \pre livebox should be developed as script type
 * \post return'd string must be free'd manually
 * \see livebox_service_lb_script_path
 */
extern char *livebox_service_lb_script_group(const char *lbid);

/*!
 * \brief Get the script file path of given livebox package.
 * \details N/A
 * \remarks N/A
 * \param[in] lbid Livebox Package Id
 * \return char *
 * \retval string script file path.
 * \retval NULL if there is no specified script file for PD layout
 * \pre N/A
 * \post Return'd string must be free'd manually
 * \see livebox_service_pd_script_group
 */
extern char *livebox_service_pd_script_path(const char *lbid);

/*!
 * \brief Get the group name for script file to load it.
 * \details N/A
 * \remarks N/A
 * \param[in] lbid Livebox package Id
 * \return char *
 * \retval string script group name
 * \retval NULL if there is no script path
 * \pre N/A
 * \post return'd string must be free'd manually
 * \see livebox_service_pd_script_path
 */
extern char *livebox_service_pd_script_group(const char *lbid);

/*!
 * \brief Get the supported size list.
 * \details N/A
 * \remarks N/A
 * \param[in] pkgid Livebox Package Id
 * \param[in] cnt Count of array w and h
 * \param[in] w Width array
 * \param[in] h Height array
 * \param[out] cnt Count of array w and h
 * \param[out] w Width array
 * \param[out] h Height array
 * \return int
 * \retval LB_STATUS_SUCCESS if succeed to get supported size list
 * \retval LB_STATUS_ERROR_IO
 * \retval LB_STATUS_ERROR_INVALID
 * \pre N/A
 * \post N/A
 * \see livebox_service_get_supported_size_types
 */
extern int livebox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h);

/*!
 * \brief Get the supported size list of given pkgid
 * \param[in] pkgid Package Name of livebox
 * \param[in] cnt size of types array
 * \param[out] cnt result count of types array
 * \param[out] types array of types
 * \return int
 * \retval LB_STATUS_ERROR_INVALID
 * \retval LB_STATUS_ERROR_IO
 * \retval LB_STATUS_SUCCESS
 * \pre N/A
 * \post N/A
 * \see livebox_service_get_supported_sizes
 */
extern int livebox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types);

/*!
 * \brief Get the category list of given cluster
 * \details N/A
 * \remarks N/A
 * \param[in] cluster Cluster name
 * \param[in] cb Callback function
 * \param[in] data Callback data
 * \return int
 * \retval LB_STATUS_SUCCESS
 * \retval LB_STATUS_ERROR_IO
 * \retval LB_STATUS_ERROR_INVALID
 * \pre N/A
 * \post N/A
 * \see livebox_service_enumerate_cluster_list
 */
extern int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/*!
 * \brief Get the cluster list
 * \details N/A
 * \remarks N/A
 * \param[in] cb Callback function for retrieving the cluster list
 * \param[in] data Callback data
 * \return int
 * \retval LB_STATUS_ERROR_INVALID
 * \retval LB_STATUS_ERROR_IO
 * \retval count of category items
 * \pre N/A
 * \post N/A
 * \see livebox_service_enumerate_category_list
 */
extern int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);

/*!
 * \brief Initialize the Livebox service API
 * \details Open the DB file.
 *    You don't need to call this if you are using livebox service API shortly.
 *    but if you are using livebox service API while whole life of your S/W,
 *    using this, initate the livebox service is more benefical to you.
 *    This API will prepare the DB operation, if you don't initiate the livebox service,
 *    every API which are related with DB operation will open DB and close it before return from it.
 * \remarks N/A
 * \return int
 * \retval LB_STATUS_SUCCESS if succeed to initialize
 * \retval LB_STATUS_ERROR_IO failed to access a DB
 * \pre N/A
 * \post N/A
 * \see livebox_service_fini
 */
extern int livebox_service_init(void);

/*!
 * \brief Finalize the livebox service API
 * \details N/A
 * \return int
 * \retval LB_STATUS_SUCCESS if succeed to finalize
 * \retval LB_STATUS_ERROR_IO Failed to close the DB (access failed to DB)
 * \pre livebox_service_init
 * \post N/A
 * \see livebox_service_init
 */
extern int livebox_service_fini(void);

/*!
 * \brief Create a handle for getting the package list.
 * \details
 *    If you want get the record one by one from DB, use this.
 *    This function will create a iterator.
 *    Then you can get the record one by one, but there is no backward iterator.
 *    Only you can get forward iterator.
 *    After call this function the iterator will be moved to next record automatically.
 * \remarks
 *    If you call this function again using created pkglist handle, it will be reset'd
 *    so you can get record from first one again.
 * \param[in] pkgid Livebox Package Id
 * \param[in] handle NULL if you call this first. or it will be reseted.
 * \return handle
 * \retval NULL if it fails
 * \retval handle if it successfully create the package list iterator
 * \see livebox_service_pkglist_destroy
 */
extern struct pkglist_handle *livebox_service_pkglist_create(const char *pkgid, struct pkglist_handle *handle);

/*!
 * \brief Get the lbid & package name & is_prime flag.
 * \param[in] handle Handle which is created by livebox_service_pkglist_create function
 * \param[out] lbid  Livebox Id
 * \param[out] pkgname Package Id that includes lbid
 * \param[out] is_prime if the returned lbid is primary, this will be 1 or 0
 * \return int
 * \retval LB_STATUS_SUCCESS Successfully get the record
 * \retval LB_STATUS_ERROR_INVALID Invalid argument
 * \retval LB_STATUS_ERROR_NOT_EXIST reach to the end of result set. you can rewind the iterator call livebox_service_pkglist_create again with current handle.
 * \retval LB_STATUS_ERROR_MEMORY Not enough memory
 * \pre N/A
 * \post you must release the lbid, pkgname manually
 * \see livebox_service_pkglist_create
 * \see livebox_service_pkglist_destroy
 */
extern int livebox_service_get_pkglist_item(struct pkglist_handle *handle, char **lbid, char **pkgname, int *is_prime);

/*!
 * \brief Destroy the iterator of pkglist
 * \details N/A
 * \param[in] handle
 * \return int
 * \retval LB_STATUS_ERROR_INVALID Invalid handle
 * \retval LB_STATUS_SUCCESS Successfully destroyed
 * \pre handle must be created by livebox_service_pkglist_create
 * \post you have not to use the handle again after destroy it.
 * \see livebox_service_pkglist_create
 */
extern int livebox_service_pkglist_destroy(struct pkglist_handle *handle);

/*!
 * \}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

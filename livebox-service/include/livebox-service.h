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

/**
 * @file livebox-service.h
 * @brief  This file declares API of liblivebox-service library
 */

/**
 * @addtogroup CAPI_LIVEBOX_SERVICE_MODULE
 * @{
 */

/**
 * @brief Definition for maximum number of supported livebox size type.
 */
#define NR_OF_SIZE_LIST 13

#define CH_IDX(a, idx) (((char *)(a))[(idx)])
#define IS_INHOUSE_LIVEBOX(abi)	((CH_IDX(abi, 0) == 'c' || CH_IDX(abi, 0) == 'C') && (CH_IDX(abi, 1) == '\0' || ((CH_IDX(abi, 1) == 'p' || CH_IDX(abi, 1) == 'P') && (CH_IDX(abi, 2) == 'p' || CH_IDX(abi, 2) == 'P') && CH_IDX(abi, 3) == '\0')))

/**
 * @brief Enumeration for list of supporting livebox size types.
 */
enum livebox_size_type {
	LB_SIZE_TYPE_1x1 = 0x0001, /**< 175x175 based on 720x1280 resolution */
	LB_SIZE_TYPE_2x1 = 0x0002, /**< 354x175 based on 720x1280 resolution */
	LB_SIZE_TYPE_2x2 = 0x0004, /**< 354x354 based on 720x1280 resolution */
	LB_SIZE_TYPE_4x1 = 0x0008, /**< 712x175 based on 720x1280 resolution */
	LB_SIZE_TYPE_4x2 = 0x0010, /**< 712x354 based on 720x1280 resolution */
	LB_SIZE_TYPE_4x3 = 0x0020, /**< 712x533 based on 720x1280 resolution */
	LB_SIZE_TYPE_4x4 = 0x0040, /**< 712x712 based on 720x1280 resolution */
	LB_SIZE_TYPE_4x5 = 0x0080, /**< 712x891 based on 720x1280 resolution */
	LB_SIZE_TYPE_4x6 = 0x0100, /**< 712x1070 based on 720x1280 resolution */
	LB_SIZE_TYPE_EASY_1x1 = 0x1000, /**< 224x215 based on 720x1280 resolution */
	LB_SIZE_TYPE_EASY_3x1 = 0x2000, /**< 680x215 based on 720x1280 resolution */
	LB_SIZE_TYPE_EASY_3x3 = 0x4000, /**< 680x653 based on 720x1280 resolution */
	LB_SIZE_TYPE_0x0 = 0x0800, /**< 720x1280 based on 720x1280 resolution */
	LB_SIZE_TYPE_UNKNOWN = 0xFFFF /**< Error */
};

/**
 * @brief Enumeration for script event of livebox.
 * @details
 * This enumeration values will be used for script plugin of data-provider-master.
 * Master will send down these accessibility events to the script plugin.
 */
enum livebox_script_event {
	LB_SCRIPT_ACCESS_EVENT		= 0x01000000, /**< Mask value */
	LB_SCRIPT_MOUSE_EVENT		= 0x02000000, /**< Mask value */
	LB_SCRIPT_KEY_EVENT		= 0x04000000, /**< Mask value */

	LB_SCRIPT_ACCESS_HIGHLIGHT	= LB_SCRIPT_ACCESS_EVENT | 0x00000001, /**< Highlight */
	LB_SCRIPT_ACCESS_HIGHLIGHT_NEXT	= LB_SCRIPT_ACCESS_EVENT | 0x00000002, /**< Move Highlight focus to the next object */
	LB_SCRIPT_ACCESS_HIGHLIGHT_PREV	= LB_SCRIPT_ACCESS_EVENT | 0x00000004, /**< Move Highlight focus to the prev object */
	LB_SCRIPT_ACCESS_ACTIVATE	= LB_SCRIPT_ACCESS_EVENT | 0x00000008, /**< Activate the highlighted object */
	LB_SCRIPT_ACCESS_ACTION		= LB_SCRIPT_ACCESS_EVENT | 0x00000010, /**< Do specified action for the highlighted object */
	LB_SCRIPT_ACCESS_SCROLL		= LB_SCRIPT_ACCESS_EVENT | 0x00000020, /**< Scroll operation */
	LB_SCRIPT_ACCESS_UNHIGHLIGHT	= LB_SCRIPT_ACCESS_EVENT | 0x00000040, /**< Remove highlight */
	LB_SCRIPT_ACCESS_VALUE_CHANGE	= LB_SCRIPT_ACCESS_EVENT | 0x00000080, /* TODO: deprecate this */
	LB_SCRIPT_ACCESS_MOUSE		= LB_SCRIPT_ACCESS_EVENT | 0x00000100, /* give mouse event to highlight object */
	LB_SCRIPT_ACCESS_BACK		= LB_SCRIPT_ACCESS_EVENT | 0x00000200, /* go back to a previous view ex: pop naviframe item */
	LB_SCRIPT_ACCESS_OVER		= LB_SCRIPT_ACCESS_EVENT | 0x00000400, /* mouse over an object */
	LB_SCRIPT_ACCESS_READ		= LB_SCRIPT_ACCESS_EVENT | 0x00000800, /* highlight an object */
	LB_SCRIPT_ACCESS_ENABLE		= LB_SCRIPT_ACCESS_EVENT | 0x00001000, /* enable highlight and read ability */
	LB_SCRIPT_ACCESS_DISABLE	= LB_SCRIPT_ACCESS_EVENT | 0x00002000, /* disable highlight and read ability */

	LB_SCRIPT_MOUSE_DOWN		= LB_SCRIPT_MOUSE_EVENT | 0x00001000, /**< Mouse down */
	LB_SCRIPT_MOUSE_MOVE		= LB_SCRIPT_MOUSE_EVENT | 0x00002000, /**< Mouse move */
	LB_SCRIPT_MOUSE_UP		= LB_SCRIPT_MOUSE_EVENT | 0x00004000, /**< Mouse up */
	LB_SCRIPT_MOUSE_IN		= LB_SCRIPT_MOUSE_EVENT | 0x00008000, /**< Mouse in */
	LB_SCRIPT_MOUSE_OUT		= LB_SCRIPT_MOUSE_EVENT | 0x00010000, /**< Mouse out */

	LB_SCRIPT_MOUSE_ON_SCROLL	= LB_SCRIPT_MOUSE_EVENT | 0x00020000, /**< If the box in in scroller and it is scrolled */
	LB_SCRIPT_MOUSE_OFF_SCROLL	= LB_SCRIPT_MOUSE_EVENT | 0x00040000, /**< If the box is in scroller but the scroll is stopped */
	LB_SCRIPT_MOUSE_ON_HOLD		= LB_SCRIPT_MOUSE_EVENT | 0x00080000, /**< Even though the box gets mouse_up event, the click event will not be generated */
	LB_SCRIPT_MOUSE_OFF_HOLD	= LB_SCRIPT_MOUSE_EVENT | 0x00100000, /**< Generate the click event if the mouse_up event occurred as normal */

	LB_SCRIPT_KEY_DOWN		= LB_SCRIPT_KEY_EVENT | 0x00020000, /**< Key pressed */
	LB_SCRIPT_KEY_UP		= LB_SCRIPT_KEY_EVENT | 0x00040000, /**< Key released */
	LB_SCRIPT_KEY_FOCUS_IN		= LB_SCRIPT_KEY_EVENT | 0x00080000, /**< Key focus in */
	LB_SCRIPT_KEY_FOCUS_OUT 	= LB_SCRIPT_KEY_EVENT | 0x00100000, /**< Key focus out */
};

/**
 * @brief Enumeration for result of accessibility event processing.
 * @details Reference the libprovider & liblivebox-viewer.
 */
enum livebox_access_status {
	LB_ACCESS_STATUS_ERROR = 0x80000000,	/**< Mask value */
	LB_ACCESS_STATUS_DONE = 0x00000000,	/**< Successfully finished */
	LB_ACCESS_STATUS_FIRST,			/**< Reach to the first item */
	LB_ACCESS_STATUS_LAST,			/**< Reach to the last item */
	LB_ACCESS_STATUS_READ			/**< TTS is done */
};

enum livebox_key_status {
	LB_KEY_STATUS_ERROR = 0x80000000,	/**< Key operation is failed */
	LB_KEY_STATUS_DONE = 0x00000000,	/**< Key operation is successfully done */
	LB_KEY_STATUS_FIRST,			/**< Focusable object item reaches to the first in it */
	LB_KEY_STATUS_LAST,			/**< Focusable object item reaches to the last in it */
};

enum livebox_delete_type {
	LB_DELETE_PERMANENTLY = 0x01,	/**< The livebox is removed from the homescreen temporary */
	LB_DELETE_TEMPORARY = 0x02,	/**< The livebox is removed from the homescreen by user permanently */
	LB_DELETE_INVALID = 0xff,	/**< Unknown event type */
};

enum livebox_pd_close_type {
	LB_CLOSE_PD_NORMAL = 0x00,	/**< PD is closed normally */
	LB_CLOSE_PD_TIMEOUT = 0x01,	/**< PD is closed because of timeout, there is no response */
	LB_CLOSE_PD_FAULT = 0x02,	/**< PD is closed because of unrecoverable error */
	LB_CLOSE_PD_INVALID = 0xff,	/**< Unknown event type */
};

/**
 * @brief Package list handle.
 */
struct pkglist_handle;

/**
 * @brief Gets the pixel size of given size type.
 * @details
 *  Size types would be\n
 *  #LB_SIZE_TYPE_1x1\n
 *  #LB_SIZE_TYPE_2x1\n
 *  #LB_SIZE_TYPE_2x2\n
 *  #LB_SIZE_TYPE_4x1\n
 *  #LB_SIZE_TYPE_4x2\n
 *  #LB_SIZE_TYPE_4x3\n
 *  #LB_SIZE_TYPE_4x4\n
 *  #LB_SIZE_TYPE_4x5\n
 *  #LB_SIZE_TYPE_4x6\n
 *  #LB_SIZE_TYPE_0x0\n
 *  #LB_SIZE_TYPE_EASY_1x1\n
 *  #LB_SIZE_TYPE_EASY_3x1\n
 *  #LB_SIZE_TYPE_EASY_3x3.
 * @param[in] type Size type
 * @param[out] width Pixel size width
 * @param[out] height Pixel size height
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS Successfully done
 * @see livebox_size_type()
 * @see livebox_service_size_type()
 */
extern int livebox_service_get_size(int type, int *width, int *height);

/**
 * @brief Gets the size type for given pixel size.
 * @details
 *  Returnable size types are\n
 *  #LB_SIZE_TYPE_1x1\n
 *  #LB_SIZE_TYPE_2x1\n
 *  #LB_SIZE_TYPE_2x2\n
 *  #LB_SIZE_TYPE_4x1\n
 *  #LB_SIZE_TYPE_4x2\n
 *  #LB_SIZE_TYPE_4x3\n
 *  #LB_SIZE_TYPE_4x4\n
 *  #LB_SIZE_TYPE_4x5\n
 *  #LB_SIZE_TYPE_4x6\n
 *  #LB_SIZE_TYPE_0x0\n
 *  #LB_SIZE_TYPE_EASY_1x1\n
 *  #LB_SIZE_TYPE_EASY_3x1\n
 *  #LB_SIZE_TYPE_EASY_3x3\n
 *  or\n
 *  #LB_SIZE_TYPE_UNKNOWN for error.
 * @param[in] width Pixel size width
 * @param[in] height Pixel size height
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_SIZE_TYPE_[EASY_]WxH Size type of given pixel size
 * @retval #LB_SIZE_TYPE_UNKNOWN If the given pixel size is not valid
 * @see livebox_size_type()
 * @see livebox_service_get_size()
 */
extern int livebox_service_size_type(int width, int height);

/**
 * @brief Supports the mouse event of livebox content.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 If the box requires mouse event, A viewer must has to feed the mouse event to the box instance
 * @retval 0 If the box doesn't requires mouse event
 * @see livebox_service_touch_effect()
 * @see livebox_service_need_frame()
 */
extern int livebox_service_mouse_event(const char *lbid, int size_type);

/**
 * @brief Requires touch effect.
 * @details If this API returns true(1), the viewer should make touch effect when a user click the livebox.
 * @param[in] lbid Livebox AppId
 * @param[in] size_type Size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 If the box requires touch effect, A viewer should make the touch effect, but it is just recomendation.
 * @retval 0 If the box doesn't requires touch effect, the box will make touch effect itself
 * @see livebox_service_mouse_event()
 * @see livebox_service_need_frame()
 */
extern int livebox_service_touch_effect(const char *lbid, int size_type);

/**
 * @brief Requires decoration frame.
 * @details If this API returns true(1), the viewer should make decoration border on the livebox content.
 * @param[in] lbid Livebox AppId
 * @param[in] size_type Size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 If the box requires frame for decorating its contents
 * @retval 0 If the box doesn't requires frame
 * @see livebox_service_mouse_event()
 * @see livebox_service_touch_effect()
 */
extern int livebox_service_need_frame(const char *lbid, int size_type);

/**
 * @brief Triggers the update event for given livebox instance.
 * @param[in] lbid Livebox AppId
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] cluster Cluster name. Default @c NULL
 * @param[in] category Category name, Default @c NULL
 * @param[in] force 1 if you want to update your livebox even if the provider is paused or 0. 0 is default
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return int type
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_CANCEL Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * @retval #LB_STATUS_ERROR_MEMORY Memory is not enough to make request
 * @retval #LB_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #LB_STATUS_SUCCESS Successfully requested
 */
extern int livebox_service_trigger_update(const char *lbid, const char *instance_id, const char *cluster, const char *category, int force);

/**
 * @brief Triggers the update event for given livebox instance.
 * @param[in] lbid Livebox AppId
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] cluster Cluster name. Default @c NULL
 * @param[in] category Category name, Default @c NULL
 * @param[in] content New content information, Default @c NULL
 * @param[in] force 1 if you want to update your livebox even if the provider is paused or 0. 0 is default
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return int type
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_CANCEL Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * @retval #LB_STATUS_ERROR_MEMORY Memory is not enough to make request
 * @retval #LB_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #LB_STATUS_SUCCESS Successfully requested
 * @see livebox_service_trigger_update()
 */
extern int livebox_service_trigger_update_with_content(const char *lbid, const char *instance_id, const char *cluster, const char *category, const char *content, int force);

/**
 * @brief Changes the update period of given livebox instance.
 * @param[in] lbid Livebox AppId
 * @param[in] instance_id Livebox instance id
 * @param[in] period New update period in sec
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return int type
 * @retval #LB_STATUS_SUCCESS Successfully changed(requested)
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #LB_STATUS_ERROR_MEMORY Not enough memory
 */
extern int livebox_service_change_period(const char *lbid, const char *instance_id, double period);

/**
 * @brief Gets synchronous package list.
 * @details
 *        callback (appid, lbid, is_prime)\n
 *        pkgid == Package ID (pkgname)\n
 *        lbid = Livebox AppId\n
 *        is_prime = 1 if the livebox is default one for associated application package\n
 *        If the callback returns negative value, the list crawling will be stopped.
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval count Count of livebox packages
 * @see livebox_service_get_pkglist_by_pkgid()
 */
extern int livebox_service_get_pkglist(int (*cb)(const char *pkgid, const char *lbid, int is_prime, void *data), void *data);

/**
 * @brief Gets synchronous uiapp list.
 * @details
 *        Callback (appid, data)\n
 *        This function will retrieve all UI Apps in a package which has given livebox appid(lbid).\n
 *        If you need to get all ui-app list, using a livebox appid, this function is able to help you.
 * @param[in] lbid Livebox App Id
 * @param[in] cb Callback function
 * @param[in] data Callback Data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS Status success
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @retval #LB_STATUS_ERROR_MEMORY Memory error
 * @retval #LB_STATUS_ERROR_FAULT
 */
extern int livebox_service_get_applist(const char *lbid, void (*cb)(const char *lbid, const char *appid, void *data), void *data);

/**
 * @brief Gets the MAIN application Id of given livebox package Id.
 * @param[in] lbid Livebox Package Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL If it fails to get main application Id (UI-APPID)
 * @retval appid Main application Id
 */
extern char *livebox_service_mainappid(const char *lbid);

/**
 * @brief Gets synchronous package list.
 * @details
 *	  callback (lbid, is_prime)\n
 *	  lbid == Livebox AppId\n
 *	  is_prime = 1 if the livebox is default one for selected package\n
 *      If the callback returns negative value, the list crawling will be stopped.
 * @param[in] pkgid Package Id (Not the UI App Id)
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval int Count of livebox packages
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @see livebox_service_get_pkglist()
 */
extern int livebox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *lbid, int is_prime, void *data), void *data);

/**
 * @brief Gets synchronous package list.
 * @details
 *	  callback (lbid)\n
 *	  lbid == Livebox AppId\n
 *        If the callback returns negative value, the list crawling will be stopped
 * @param[in] category Name of category
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int count
 * @retval Count of livebox packages
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @see livebox_service_get_pkglist_by_pkgid()
 */
extern int livebox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *lbid, void *data), void *data);

/**
 * @brief Gets the lbid of a primary livebox using given lbid or pkgid or UI appid.
 * @param[in] id Livebox Id or Package Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL Failed to get primary lbid
 * @retval lbid Primary livebox Id. which is allocated in the heap
 * @pre Must be released returned string by manually.
 * @see livebox_service_appid()
 */
extern char *livebox_service_pkgname(const char *lbid);

/**
 * @brief Checks the primary flag of given livebox Id.
 * @param[in] lbid Livebox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 0 If is not a primary
 * @retval 1 If it is a primary livebox
 */
extern int livebox_service_is_primary(const char *lbid);

/**
 * @brief Get the category using given lbid.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char *
 * @retval @c NULL Failed to get primary lbid
 * @retval category Category string which is allocated in the heap.
 * @pre Must be released returned string by manually
 * @post N/A
 * @see livebox_service_pkgname()
 */
extern char *livebox_service_category(const char *lbid);

/**
 * @brief Gets the name of a livebox (provider name == livebox appid), you have to release the return value after use it.
 * @details
 *    OSP livebox has provider process for each livebox instances.\n
 *    To get the provider's package name, you can use this API.\n
 *    If the given lbid is inhouse livebox, the return string will be the same with given argument but it is allocated in the heap.\n
 *    So you have to free it if you don't need it anymore.
 * @param[in] lbid Livebox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL Failed to get provider name
 * @retval lbid Livebox AppId which is allocated on the heap
 * @post Returned string must be free'd manually.
 */
extern char *livebox_service_provider_name(const char *lbid);

/**
 * @brief Gets the appId of setup app which is specified by given livebox Id's manifest.
 * @details This setup app should be launched before adding the livebox to get the content_info.
 * @details
 *    This function should be called before add a livebox.
 *    To determine the content information string.
 * @param[in] lbid Livebox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL There is no setup application
 * @retval appid AppId if exists or @c NULL
 * @post Returned string must be free'd manually.
 */
extern char *livebox_service_setup_appid(const char *lbid);

/**
 * @brief Gets the Package Id (Not the UI App Id) of given livebox, &lt;manifest package="AAA"&gt; tag.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval appid String which is allocated in the heap
 * @retval @c NULL Invalid appid
 * @post Returned string must be free'd manually.
 * @see livebox_service_pkgname()
 */
extern char *livebox_service_appid(const char *lbid);

/**
 * @brief Gives Internationalized name of livebox package.
 * @param[in] lbid Livebox AppId
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval name If it fails to get name
 * @retval @c NULL Allocated heap address
 * @post Returned string must be free'd by manually.
 * @see livebox_service_i18n_icon()
 * @see livebox_service_preview()
 */
extern char *livebox_service_i18n_name(const char *lbid, const char *lang);

/**
 * @brief Gets the preview image path of given size type.
 * @details This function will returns i18nized preview image path.
 * @param[in] lbid Livebox AppId
 * @param[in] size_type Livebox size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval path Preview image path
 * @retval @c NULL There is no preview image file
 * @post Returned string must be free'd manually.
 * @see livebox_service_i18n_icon()
 * @see livebox_service_i18n_name()
 */
extern char *livebox_service_preview(const char *lbid, int size_type);

/**
 * @brief Gets the default content string of the given livebox.
 * @details
 *    If the user defines the default content string in the manifest file (.xml),
 *    this API will return it.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval content Content string
 * @retval @c NULL There is no specified content string
 * @pre Manifest has the default content string. &lt;content&gt;Default content string&lt;content&gt; tag.
 * @post Returned string must be free'd manually.
 */
extern char *livebox_service_content(const char *lbid);

/**
 * @brief Gives Internationalized icon path of given livebox package.
 * @details The user should free the returned string with free().
 * @param[in] lbid Livebox AppId (It must has to be a livebox package ID. not the UI-APP and the PACKAGE)
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval name Allocated heap address
 * @retval @c NULL Fails to get path of an icon
 * @post Returned string must be free'd manually.
 * @see livebox_service_i18n_name()
 * @see livebox_service_preview()
 */
extern char *livebox_service_i18n_icon(const char *pkgid, const char *lang);

/**
 * @brief Gets the "nodisplay" value.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 The box should not be listed by the livebox list app
 * @retval 0 Box should be listed
 * @pre Livebox tag includes "nodisplay" attribute.
 */
extern int livebox_service_nodisplay(const char *lbid);

/**
 * @brief Gets the "ABI" of given package.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval abi String which is allocated in the heap
 * @retval @c NULL Failed to get ABI of given livebox
 * @post Returned string must be free'd manually.
 */
extern char *livebox_service_abi(const char *lbid);

/**
 * @brief Checks the status of the livebox.
 * @details Currently this API is not implemented. It just returns 1 all the time.
 * @remarks This API is not implemented. It will always return 1.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 Enabled
 * @retval 0 Disabled
 */
extern int livebox_service_is_enabled(const char *lbid);

/**
 * @brief Gets the script file of livebox.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL Not specified script file
 * @pre Livebox should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see livebox_service_lb_script_group()
 */
extern char *livebox_service_lb_script_path(const char *lbid);

/**
 * @brief Gets the script group of livebox.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval Group Name of livebox
 * @retval @c NULL If there is no group defined
 * @pre Livebox should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see livebox_service_lb_script_path()
 */
extern char *livebox_service_lb_script_group(const char *lbid);

/**
 * @brief Gets the script file path of given livebox package.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL No specified script file for PD layout
 * @post Returned string must be free'd manually.
 * @see livebox_service_pd_script_group()
 */
extern char *livebox_service_pd_script_path(const char *lbid);

/**
 * @brief Gets the group name for script file to load it.
 * @param[in] lbid Livebox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval string Script group name
 * @retval @c NULL No script path
 * @post Returned string must be free'd manually.
 * @see livebox_service_pd_script_path()
 */
extern char *livebox_service_pd_script_group(const char *lbid);

/**
 * @brief Gets the supported size list.
 * @param[in] lbid Livebox AppId
 * @param[in] cnt Count of array w and h
 * @param[in] w Width array
 * @param[in] h Height array
 * @param[out] cnt Count of array w and h
 * @param[out] w Width array
 * @param[out] h Height array
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS If succeed to get supported size list
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @see livebox_service_get_supported_size_types()
 */
extern int livebox_service_get_supported_sizes(const char *lbid, int *cnt, int *w, int *h);

/**
 * @brief Gets the supported size list of given pkgid.
 * @param[in] lbid Livebox AppId
 * @param[in] cnt Size of types array
 * @param[out] cnt Result count of types array
 * @param[out] types Array of types
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @retval #LB_STATUS_SUCCESS Successfully done
 * @see livebox_service_get_supported_sizes()
 */
extern int livebox_service_get_supported_size_types(const char *lbid, int *cnt, int *types);

/**
 * @brief Gets the category list of given cluster.
 * @param[in] cluster Cluster name
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS Successfully done
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @see livebox_service_enumerate_cluster_list()
 */
extern int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/**
 * @brief Gets the cluster list.
 * @param[in] cb Callback function for retrieving the cluster list
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_IO Failed to access DB
 * @retval count Count of category items
 * @see livebox_service_enumerate_category_list()
 */
extern int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);

/**
 * @brief Initializes the Livebox service API.
 * @details Open the DB file.\n
 *    You don't need to call this if you are using livebox service API shortly.\n
 *    But if you are using livebox service API while whole life of your S/W,
 *    using this, initate the livebox service is more benefical to you.\n
 *    This API will prepare the DB operation, if you don't initiate the livebox service,
 *    every API which are related with DB operation will open DB and close it before return from it.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS Succeed to initialize
 * @retval #LB_STATUS_ERROR_IO Failed to access a DB
 * @see livebox_service_fini()
 */
extern int livebox_service_init(void);

/**
 * @brief Finalizes the livebox service API.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS Succeed to finalize
 * @retval #LB_STATUS_ERROR_IO Failed to close the DB (access failed to DB)
 * @pre Livebox_service_init.
 * @see livebox_service_init()
 */
extern int livebox_service_fini(void);

/**
 * @brief Creates a handle for getting the package list.
 * @details
 *    If you want get the record one by one from DB, use this.\n
 *    This function will create a iterator.\n
 *    Then you can get the records one by one, but there is no backward iterator.\n
 *    You can only get a forward iterator.\n
 *    After calling this function the iterator will be moved to the next record automatically.
 * @remarks
 *    If you call this function again using created pkglist handle, it will be reset.
 *    So you can get records from the first one again.
 * @param[in] lbid Livebox AppId
 * @param[in] handle @c NULL if you call this first, or it will be reset
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return handle
 * @retval @c NULL If it fails
 * @retval handle If it successfully create the package list iterator
 * @see livebox_service_pkglist_destroy()
 */
extern struct pkglist_handle *livebox_service_pkglist_create(const char *lbid, struct pkglist_handle *handle);

/**
 * @brief Gets the lbid & package name & is_prime flag.
 * @param[in] handle Handle which is created by livebox_service_pkglist_create() function
 * @param[out] lbid  Livebox Id
 * @param[out] pkgname Package Id which includes liveboxes
 * @param[out] is_prime If the returned lbid is primary, this will be 1 or 0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_SUCCESS Successfully get the record
 * @retval #LB_STATUS_ERROR_INVALID Invalid argument
 * @retval #LB_STATUS_ERROR_NOT_EXIST Reach to the end of result set. you can rewind the iterator call livebox_service_pkglist_create() again with current handle
 * @retval #LB_STATUS_ERROR_MEMORY Not enough memory
 * @post You must release the lbid, pkgname manually.
 * @see livebox_service_pkglist_create()
 * @see livebox_service_pkglist_destroy()
 */
extern int livebox_service_get_pkglist_item(struct pkglist_handle *handle, char **lbid, char **pkgname, int *is_prime);

/**
 * @brief Destroys the iterator of pkglist.
 * @param[in] handle Package list handle
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #LB_STATUS_ERROR_INVALID Invalid handle
 * @retval #LB_STATUS_SUCCESS Successfully destroyed
 * @pre Handle must be created by livebox_service_pkglist_create().
 * @post You have not to use the handle again after destroy it.
 * @see livebox_service_pkglist_create()
 */
extern int livebox_service_pkglist_destroy(struct pkglist_handle *handle);

/**
 * @brief Getting the activated instance count.
 * @param[in] lbid Livebox Id, if you want to get whole instnaces list, use NULL.
 * @param[in] cluster Cluster name if you don't know what this is, use NULL.
 * @param[in] category Sub-cluster(category) name if you don't know what this is, use NULL.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return count of instances
 * @retval #LB_STATUS_ERROR_INVALID Invalid parameter
 * @retval #LB_STATUS_ERROR_FAULT Unrecorvarable error occurred
 * @retval count Positive value including ZERO, Count of activated instances on viewers
 */
extern int livebox_service_get_instance_count(const char *lbid, const char *cluster, const char *category);

/**
 * @brief Getting the max count of creatable instances.
 * @param[in] lbid Livebox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int count of boxes
 * @retval 0 Instance count is not limited
 * @retval >0 Instance count is limited to >0
 * @retval #LB_STATUS_ERROR_INVALID invalid parameter
 * @retval #LB_STATUS_ERROR_IO Unable to access DB
 * @see livebox_service_get_instance_count()
 */
extern int livebox_service_max_instance_count(const char *lbid);

/*@
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

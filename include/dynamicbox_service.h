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

#ifndef __DYNAMICBOX_SERVICE_H
#define __DYNAMICBOX_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file dynamic-service.h
 * @brief  This file declares API of libdynamic-service library
 */

/**
 * @addtogroup CAPI_DYNAMICBOX_SERVICE_MODULE
 * @{
 */

/**
 * @brief Definition for maximum number of supported dynamicbox size type.
 */
#define DBOX_NR_OF_SIZE_LIST 13

#define DBOX_CH_IDX(a, idx) (((char *)(a))[(idx)])
#define DBOX_IS_INHOUSE(abi)	((DBOX_CH_IDX(abi, 0) == 'c' || DBOX_CH_IDX(abi, 0) == 'C') && (DBOX_CH_IDX(abi, 1) == '\0' || ((DBOX_CH_IDX(abi, 1) == 'p' || DBOX_CH_IDX(abi, 1) == 'P') && (DBOX_CH_IDX(abi, 2) == 'p' || DBOX_CH_IDX(abi, 2) == 'P') && DBOX_CH_IDX(abi, 3) == '\0')))

/**
 * @brief Enumeration for list of supporting dynamicbox size types.
 */
enum dynamicbox_size_type {
	DBOX_SIZE_TYPE_1x1 = 0x0001, /**< 175x175 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_2x1 = 0x0002, /**< 354x175 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_2x2 = 0x0004, /**< 354x354 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x1 = 0x0008, /**< 712x175 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x2 = 0x0010, /**< 712x354 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x3 = 0x0020, /**< 712x533 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x4 = 0x0040, /**< 712x712 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x5 = 0x0080, /**< 712x891 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x6 = 0x0100, /**< 712x1070 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_EASY_1x1 = 0x1000, /**< 224x215 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_EASY_3x1 = 0x2000, /**< 680x215 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_EASY_3x3 = 0x4000, /**< 680x653 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_0x0 = 0x0800, /**< 720x1280 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_UNKNOWN = 0xFFFF /**< Error */
};

/**
 * @brief Enumeration for script event of dynamicbox.
 * @details
 * This enumeration values will be used for script plugin of data-provider-master.
 * Master will send down these accessibility events to the script plugin.
 */
enum dynamicbox_script_event {
	DBOX_SCRIPT_ACCESS_EVENT	= 0x01000000, /**< Mask value */
	DBOX_SCRIPT_MOUSE_EVENT		= 0x02000000, /**< Mask value */
	DBOX_SCRIPT_KEY_EVENT		= 0x04000000, /**< Mask value */

	DBOX_SCRIPT_ACCESS_HIGHLIGHT	= DBOX_SCRIPT_ACCESS_EVENT | 0x00000001, /**< Highlight */
	DBOX_SCRIPT_ACCESS_HIGHLIGHT_NEXT	= DBOX_SCRIPT_ACCESS_EVENT | 0x00000002, /**< Move Highlight focus to the next object */
	DBOX_SCRIPT_ACCESS_HIGHLIGHT_PREV	= DBOX_SCRIPT_ACCESS_EVENT | 0x00000004, /**< Move Highlight focus to the prev object */
	DBOX_SCRIPT_ACCESS_ACTIVATE	= DBOX_SCRIPT_ACCESS_EVENT | 0x00000008, /**< Activate the highlighted object */
	DBOX_SCRIPT_ACCESS_ACTION		= DBOX_SCRIPT_ACCESS_EVENT | 0x00000010, /**< Do specified action for the highlighted object */
	DBOX_SCRIPT_ACCESS_SCROLL		= DBOX_SCRIPT_ACCESS_EVENT | 0x00000020, /**< Scroll operation */
	DBOX_SCRIPT_ACCESS_UNHIGHLIGHT	= DBOX_SCRIPT_ACCESS_EVENT | 0x00000040, /**< Remove highlight */

	DBOX_SCRIPT_MOUSE_DOWN		= DBOX_SCRIPT_MOUSE_EVENT | 0x00001000, /**< Mouse down */
	DBOX_SCRIPT_MOUSE_MOVE		= DBOX_SCRIPT_MOUSE_EVENT | 0x00002000, /**< Mouse move */
	DBOX_SCRIPT_MOUSE_UP		= DBOX_SCRIPT_MOUSE_EVENT | 0x00004000, /**< Mouse up */
	DBOX_SCRIPT_MOUSE_IN		= DBOX_SCRIPT_MOUSE_EVENT | 0x00008000, /**< Mouse in */
	DBOX_SCRIPT_MOUSE_OUT		= DBOX_SCRIPT_MOUSE_EVENT | 0x00010000, /**< Mouse out */

	DBOX_SCRIPT_MOUSE_ON_SCROLL	= DBOX_SCRIPT_MOUSE_EVENT | 0x00020000, /**< If the box in in scroller and it is scrolled */
	DBOX_SCRIPT_MOUSE_OFF_SCROLL	= DBOX_SCRIPT_MOUSE_EVENT | 0x00040000, /**< If the box is in scroller but the scroll is stopped */
	DBOX_SCRIPT_MOUSE_ON_HOLD		= DBOX_SCRIPT_MOUSE_EVENT | 0x00080000, /**< Even though the box gets mouse_up event, the click event will not be generated */
	DBOX_SCRIPT_MOUSE_OFF_HOLD	= DBOX_SCRIPT_MOUSE_EVENT | 0x00100000, /**< Generate the click event if the mouse_up event occurred as normal */

	DBOX_SCRIPT_KEY_DOWN		= DBOX_SCRIPT_KEY_EVENT | 0x00020000, /**< Key pressed */
	DBOX_SCRIPT_KEY_UP		= DBOX_SCRIPT_KEY_EVENT | 0x00040000, /**< Key released */
	DBOX_SCRIPT_KEY_FOCUS_IN		= DBOX_SCRIPT_KEY_EVENT | 0x00080000, /**< Key focus in */
	DBOX_SCRIPT_KEY_FOCUS_OUT 	= DBOX_SCRIPT_KEY_EVENT | 0x00100000, /**< Key focus out */
};

/**
 * @brief Enumeration for result of accessibility event processing.
 * @details Reference the libprovider & libdynamicbox-viewer.
 */
enum dynamicbox_access_status {
	DBOX_ACCESS_STATUS_ERROR = 0x80000000,	/**< Mask value */
	DBOX_ACCESS_STATUS_DONE = 0x00000000,	/**< Successfully finished */
	DBOX_ACCESS_STATUS_FIRST,			/**< Reach to the first item */
	DBOX_ACCESS_STATUS_LAST,			/**< Reach to the last item */
	DBOX_ACCESS_STATUS_READ			/**< TTS is done */
};

enum dynamicbox_key_status {
	DBOX_KEY_STATUS_ERROR = 0x80000000,	/**< Key operation is failed */
	DBOX_KEY_STATUS_DONE = 0x00000000,	/**< Key operation is successfully done */
	DBOX_KEY_STATUS_FIRST,			/**< Focusable object item reaches to the first in it */
	DBOX_KEY_STATUS_LAST,			/**< Focusable object item reaches to the last in it */
};

enum dynamicbox_delete_type {
	DBOX_DELETE_PERMANENTLY = 0x01,	/**< The dynamicbox is removed from the homescreen temporary */
	DBOX_DELETE_TEMPORARY = 0x02,	/**< The dynamicbox is removed from the homescreen by user permanently */
	DBOX_DELETE_INVALID = 0xff,	/**< Unknown event type */
};

enum dynamicbox_pd_close_type {
	DBOX_CLOSE_PD_NORMAL = 0x00,	/**< PD is closed normally */
	DBOX_CLOSE_PD_TIMEOUT = 0x01,	/**< PD is closed because of timeout, there is no response */
	DBOX_CLOSE_PD_FAULT = 0x02,	/**< PD is closed because of unrecoverable error */
	DBOX_CLOSE_PD_INVALID = 0xff,	/**< Unknown event type */
};

/**
 * @brief Package list handle.
 */
struct pkglist_handle;

/**
 * @brief Gets the pixel size of given size type.
 * @details
 *  Size types would be\n
 *  #DBOX_SIZE_TYPE_1x1\n
 *  #DBOX_SIZE_TYPE_2x1\n
 *  #DBOX_SIZE_TYPE_2x2\n
 *  #DBOX_SIZE_TYPE_4x1\n
 *  #DBOX_SIZE_TYPE_4x2\n
 *  #DBOX_SIZE_TYPE_4x3\n
 *  #DBOX_SIZE_TYPE_4x4\n
 *  #DBOX_SIZE_TYPE_4x5\n
 *  #DBOX_SIZE_TYPE_4x6\n
 *  #DBOX_SIZE_TYPE_0x0\n
 *  #DBOX_SIZE_TYPE_EASY_1x1\n
 *  #DBOX_SIZE_TYPE_EASY_3x1\n
 *  #DBOX_SIZE_TYPE_EASY_3x3.
 * @param[in] type Size type
 * @param[out] width Pixel size width
 * @param[out] height Pixel size height
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully done
 * @see dynamicbox_size_type()
 * @see dynamicbox_service_size_type()
 */
extern int dynamicbox_service_get_size(int type, int *width, int *height);

/**
 * @brief Gets the size type for given pixel size.
 * @details
 *  Returnable size types are\n
 *  #DBOX_SIZE_TYPE_1x1\n
 *  #DBOX_SIZE_TYPE_2x1\n
 *  #DBOX_SIZE_TYPE_2x2\n
 *  #DBOX_SIZE_TYPE_4x1\n
 *  #DBOX_SIZE_TYPE_4x2\n
 *  #DBOX_SIZE_TYPE_4x3\n
 *  #DBOX_SIZE_TYPE_4x4\n
 *  #DBOX_SIZE_TYPE_4x5\n
 *  #DBOX_SIZE_TYPE_4x6\n
 *  #DBOX_SIZE_TYPE_0x0\n
 *  #DBOX_SIZE_TYPE_EASY_1x1\n
 *  #DBOX_SIZE_TYPE_EASY_3x1\n
 *  #DBOX_SIZE_TYPE_EASY_3x3\n
 *  or\n
 *  #DBOX_SIZE_TYPE_UNKNOWN for error.
 * @param[in] width Pixel size width
 * @param[in] height Pixel size height
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_SIZE_TYPE_[EASY_]WxH Size type of given pixel size
 * @retval #DBOX_SIZE_TYPE_UNKNOWN If the given pixel size is not valid
 * @see dynamicbox_size_type()
 * @see dynamicbox_service_get_size()
 */
extern int dynamicbox_service_size_type(int width, int height);

/**
 * @brief Supports the mouse event of dynamicbox content.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 If the box requires mouse event, A viewer must has to feed the mouse event to the box instance
 * @retval 0 If the box doesn't requires mouse event
 * @see dynamicbox_service_touch_effect()
 * @see dynamicbox_service_need_frame()
 */
extern int dynamicbox_service_mouse_event(const char *dboxid, int size_type);

/**
 * @brief Requires touch effect.
 * @details If this API returns true(1), the viewer should make touch effect when a user click the dynamicbox.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] size_type Size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 If the box requires touch effect, A viewer should make the touch effect, but it is just recomendation.
 * @retval 0 If the box doesn't requires touch effect, the box will make touch effect itself
 * @see dynamicbox_service_mouse_event()
 * @see dynamicbox_service_need_frame()
 */
extern int dynamicbox_service_touch_effect(const char *dboxid, int size_type);

/**
 * @brief Requires decoration frame.
 * @details If this API returns true(1), the viewer should make decoration border on the dynamicbox content.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] size_type Size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 If the box requires frame for decorating its contents
 * @retval 0 If the box doesn't requires frame
 * @see dynamicbox_service_mouse_event()
 * @see dynamicbox_service_touch_effect()
 */
extern int dynamicbox_service_need_frame(const char *dboxid, int size_type);

/**
 * @brief Triggers the update event for given dynamicbox instance.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] cluster Cluster name. Default @c NULL
 * @param[in] category Category name, Default @c NULL
 * @param[in] content New content information, Default @c NULL
 * @param[in] force 1 if you want to update your dynamicbox even if the provider is paused or 0. 0 is default
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return int type
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_CANCEL Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * @retval #DBOX_STATUS_ERROR_MEMORY Memory is not enough to make request
 * @retval #DBOX_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #DBOX_STATUS_ERROR_NONE Successfully requested
 * @see dynamicbox_service_trigger_update()
 */
extern int dynamicbox_service_trigger_update(const char *dboxid, const char *instance_id, const char *cluster, const char *category, const char *content, int force);

/**
 * @brief Changes the update period of given dynamicbox instance.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] instance_id Dynamicbox instance id
 * @param[in] period New update period in sec
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully changed(requested)
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #DBOX_STATUS_ERROR_MEMORY Not enough memory
 */
extern int dynamicbox_service_change_period(const char *dboxid, const char *instance_id, double period);

/**
 * @brief Gets synchronous package list.
 * @details
 *        callback (appid, dboxid, is_prime)\n
 *        pkgid == Package ID (pkgname)\n
 *        dboxid = Dynamicbox AppId\n
 *        is_prime = 1 if the dynamicbox is default one for associated application package\n
 *        If the callback returns negative value, the list crawling will be stopped.
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval count Count of dynamicbox packages
 * @see dynamicbox_service_get_pkglist_by_pkgid()
 */
extern int dynamicbox_service_get_pkglist(int (*cb)(const char *pkgid, const char *dboxid, int is_prime, void *data), void *data);

/**
 * @brief Gets synchronous uiapp list.
 * @details
 *        Callback (appid, data)\n
 *        This function will retrieve all UI Apps in a package which has given dynamicbox appid(dboxid).\n
 *        If you need to get all ui-app list, using a dynamicbox appid, this function is able to help you.
 * @param[in] dboxid Dynamicbox App Id
 * @param[in] cb Callback function
 * @param[in] data Callback Data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Status success
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_MEMORY Memory error
 * @retval #DBOX_STATUS_ERROR_FAULT
 */
extern int dynamicbox_service_get_applist(const char *dboxid, void (*cb)(const char *dboxid, const char *appid, void *data), void *data);

/**
 * @brief Gets the MAIN application Id of given dynamicbox package Id.
 * @param[in] dboxid Dynamicbox Package Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL If it fails to get main application Id (UI-APPID)
 * @retval appid Main application Id
 */
extern char *dynamicbox_service_mainappid(const char *dboxid);

/**
 * @brief Gets synchronous package list.
 * @details
 *	  callback (dboxid, is_prime)\n
 *	  dboxid == Dynamicbox AppId\n
 *	  is_prime = 1 if the dynamicbox is default one for selected package\n
 *      If the callback returns negative value, the list crawling will be stopped.
 * @param[in] pkgid Package Id (Not the UI App Id)
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval int Count of dynamicbox packages
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @see dynamicbox_service_get_pkglist()
 */
extern int dynamicbox_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *dboxid, int is_prime, void *data), void *data);

/**
 * @brief Gets synchronous package list.
 * @details
 *	  callback (dboxid)\n
 *	  dboxid == Dynamicbox AppId\n
 *        If the callback returns negative value, the list crawling will be stopped
 * @param[in] category Name of category
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int count
 * @retval Count of dynamicbox packages
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @see dynamicbox_service_get_pkglist_by_pkgid()
 */
extern int dynamicbox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *dboxid, void *data), void *data);

/**
 * @brief Gets the dboxid of a primary dynamicbox using given dboxid or pkgid or UI appid.
 * @param[in] id Dynamicbox Id or Package Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL Failed to get primary dboxid
 * @retval dboxid Primary dynamicbox Id. which is allocated in the heap
 * @pre Must be released returned string by manually.
 * @see dynamicbox_service_appid()
 */
extern char *dynamicbox_service_pkgname(const char *dboxid);

/**
 * @brief Checks the primary flag of given dynamicbox Id.
 * @param[in] dboxid Dynamicbox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 0 If is not a primary
 * @retval 1 If it is a primary dynamicbox
 */
extern int dynamicbox_service_is_primary(const char *dboxid);

/**
 * @brief Get the category using given dboxid.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char *
 * @retval @c NULL Failed to get primary dboxid
 * @retval category Category string which is allocated in the heap.
 * @pre Must be released returned string by manually
 * @post N/A
 * @see dynamicbox_service_pkgname()
 */
extern char *dynamicbox_service_category(const char *dboxid);

/**
 * @brief Gets the name of a dynamicbox (provider name == dynamicbox appid), you have to release the return value after use it.
 * @details
 *    OSP dynamicbox has provider process for each dynamicbox instances.\n
 *    To get the provider's package name, you can use this API.\n
 *    If the given dboxid is inhouse dynamicbox, the return string will be the same with given argument but it is allocated in the heap.\n
 *    So you have to free it if you don't need it anymore.
 * @param[in] dboxid Dynamicbox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL Failed to get provider name
 * @retval dboxid Dynamicbox AppId which is allocated on the heap
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_provider_name(const char *dboxid);

/**
 * @brief Gets the appId of setup app which is specified by given dynamicbox Id's manifest.
 * @details This setup app should be launched before adding the dynamicbox to get the content_info.
 * @details
 *    This function should be called before add a dynamicbox.
 *    To determine the content information string.
 * @param[in] dboxid Dynamicbox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval @c NULL There is no setup application
 * @retval appid AppId if exists or @c NULL
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_setup_appid(const char *dboxid);

/**
 * @brief Gets the Package Id (Not the UI App Id) of given dynamicbox, &lt;manifest package="AAA"&gt; tag.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval appid String which is allocated in the heap
 * @retval @c NULL Invalid appid
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_pkgname()
 */
extern char *dynamicbox_service_appid(const char *dboxid);

/**
 * @brief Gives Internationalized name of dynamicbox package.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval name If it fails to get name
 * @retval @c NULL Allocated heap address
 * @post Returned string must be free'd by manually.
 * @see dynamicbox_service_i18n_icon()
 * @see dynamicbox_service_preview()
 */
extern char *dynamicbox_service_i18n_name(const char *dboxid, const char *lang);

/**
 * @brief Gets the preview image path of given size type.
 * @details This function will returns i18nized preview image path.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] size_type Dynamicbox size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval path Preview image path
 * @retval @c NULL There is no preview image file
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_i18n_icon()
 * @see dynamicbox_service_i18n_name()
 */
extern char *dynamicbox_service_preview(const char *dboxid, int size_type);

/**
 * @brief Gets the default content string of the given dynamicbox.
 * @details
 *    If the user defines the default content string in the manifest file (.xml),
 *    this API will return it.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval content Content string
 * @retval @c NULL There is no specified content string
 * @pre Manifest has the default content string. &lt;content&gt;Default content string&lt;content&gt; tag.
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_content(const char *dboxid);

/**
 * @brief Gives Internationalized icon path of given dynamicbox package.
 * @details The user should free the returned string with free().
 * @param[in] dboxid Dynamicbox AppId (It must has to be a dynamicbox package ID. not the UI-APP and the PACKAGE)
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval name Allocated heap address
 * @retval @c NULL Fails to get path of an icon
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_i18n_name()
 * @see dynamicbox_service_preview()
 */
extern char *dynamicbox_service_i18n_icon(const char *pkgid, const char *lang);

/**
 * @brief Gets the "nodisplay" value.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 The box should not be listed by the dynamicbox list app
 * @retval 0 Box should be listed
 * @pre Dynamicbox tag includes "nodisplay" attribute.
 */
extern int dynamicbox_service_nodisplay(const char *dboxid);

/**
 * @brief Gets the "ABI" of given package.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval abi String which is allocated in the heap
 * @retval @c NULL Failed to get ABI of given dynamicbox
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_abi(const char *dboxid);

/**
 * @brief Checks the status of the dynamicbox.
 * @details Currently this API is not implemented. It just returns 1 all the time.
 * @remarks This API is not implemented. It will always return 1.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval 1 Enabled
 * @retval 0 Disabled
 */
extern int dynamicbox_service_is_enabled(const char *dboxid);

/**
 * @brief Gets the script file of dynamicbox.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL Not specified script file
 * @pre Dynamicbox should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see dynamicbox_service_dbox_script_group()
 */
extern char *dynamicbox_service_dbox_script_path(const char *dboxid);

/**
 * @brief Gets the script group of dynamicbox.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval Group Name of dynamicbox
 * @retval @c NULL If there is no group defined
 * @pre Dynamicbox should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see dynamicbox_service_dbox_script_path()
 */
extern char *dynamicbox_service_dbox_script_group(const char *dboxid);

/**
 * @brief Gets the script file path of given dynamicbox package.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL No specified script file for PD layout
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_pd_script_group()
 */
extern char *dynamicbox_service_pd_script_path(const char *dboxid);

/**
 * @brief Gets the group name for script file to load it.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return char * type
 * @retval string Script group name
 * @retval @c NULL No script path
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_pd_script_path()
 */
extern char *dynamicbox_service_pd_script_group(const char *dboxid);

/**
 * @brief Gets the supported size list.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] cnt Count of array w and h
 * @param[in] w Width array
 * @param[in] h Height array
 * @param[out] cnt Count of array w and h
 * @param[out] w Width array
 * @param[out] h Height array
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE If succeed to get supported size list
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @see dynamicbox_service_get_supported_size_types()
 */
extern int dynamicbox_service_get_supported_sizes(const char *dboxid, int *cnt, int *w, int *h);

/**
 * @brief Gets the supported size list of given pkgid.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] cnt Size of types array
 * @param[out] cnt Result count of types array
 * @param[out] types Array of types
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_NONE Successfully done
 * @see dynamicbox_service_get_supported_sizes()
 */
extern int dynamicbox_service_get_supported_size_types(const char *dboxid, int *cnt, int *types);

/**
 * @brief Gets the category list of given cluster.
 * @param[in] cluster Cluster name
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully done
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @see dynamicbox_service_enumerate_cluster_list()
 */
extern int dynamicbox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/**
 * @brief Gets the cluster list.
 * @param[in] cb Callback function for retrieving the cluster list
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval count Count of category items
 * @see dynamicbox_service_enumerate_category_list()
 */
extern int dynamicbox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);

/**
 * @brief Initializes the Dynamicbox service API.
 * @details Open the DB file.\n
 *    You don't need to call this if you are using dynamicbox service API shortly.\n
 *    But if you are using dynamicbox service API while whole life of your S/W,
 *    using this, initate the dynamicbox service is more benefical to you.\n
 *    This API will prepare the DB operation, if you don't initiate the dynamicbox service,
 *    every API which are related with DB operation will open DB and close it before return from it.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Succeed to initialize
 * @retval #DBOX_STATUS_ERROR_IO Failed to access a DB
 * @see dynamicbox_service_fini()
 */
extern int dynamicbox_service_init(void);

/**
 * @brief Finalizes the dynamicbox service API.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Succeed to finalize
 * @retval #DBOX_STATUS_ERROR_IO Failed to close the DB (access failed to DB)
 * @pre Dynamicbox_service_init.
 * @see dynamicbox_service_init()
 */
extern int dynamicbox_service_fini(void);

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
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] handle @c NULL if you call this first, or it will be reset
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return handle
 * @retval @c NULL If it fails
 * @retval handle If it successfully create the package list iterator
 * @see dynamicbox_service_pkglist_destroy()
 */
extern struct pkglist_handle *dynamicbox_service_pkglist_create(const char *dboxid, struct pkglist_handle *handle);

/**
 * @brief Gets the dboxid & package name & is_prime flag.
 * @param[in] handle Handle which is created by dynamicbox_service_pkglist_create() function
 * @param[out] dboxid  Dynamicbox Id
 * @param[out] pkgname Package Id which includes dynamicboxes
 * @param[out] is_prime If the returned dboxid is primary, this will be 1 or 0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully get the record
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_NOT_EXIST Reach to the end of result set. you can rewind the iterator call dynamicbox_service_pkglist_create() again with current handle
 * @retval #DBOX_STATUS_ERROR_MEMORY Not enough memory
 * @post You must release the dboxid, pkgname manually.
 * @see dynamicbox_service_pkglist_create()
 * @see dynamicbox_service_pkglist_destroy()
 */
extern int dynamicbox_service_get_pkglist_item(struct pkglist_handle *handle, char **dboxid, char **pkgname, int *is_prime);

/**
 * @brief Destroys the iterator of pkglist.
 * @param[in] handle Package list handle
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.info
 * @return int type
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid handle
 * @retval #DBOX_STATUS_ERROR_NONE Successfully destroyed
 * @pre Handle must be created by dynamicbox_service_pkglist_create().
 * @post You have not to use the handle again after destroy it.
 * @see dynamicbox_service_pkglist_create()
 */
extern int dynamicbox_service_pkglist_destroy(struct pkglist_handle *handle);

/**
 * @brief Getting the activated instance count.
 * @param[in] dboxid Dynamicbox Id, if you want to get whole instnaces list, use NULL.
 * @param[in] cluster Cluster name if you don't know what this is, use NULL.
 * @param[in] category Sub-cluster(category) name if you don't know what this is, use NULL.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/core/dynamicbox.service
 * @return count of instances
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid parameter
 * @retval #DBOX_STATUS_ERROR_FAULT Unrecorvarable error occurred
 * @retval count Positive value including ZERO, Count of activated instances on viewers
 */
extern int dynamicbox_service_get_instance_count(const char *dboxid, const char *cluster, const char *category);
/*@
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

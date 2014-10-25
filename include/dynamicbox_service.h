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
 * @since_tizen 2.3
 */

/**
 * @addtogroup CAPI_DYNAMICBOX_SERVICE_MODULE
 * @{
 */

/**
 * @brief Definition for maximum number of supported dynamicbox size type.
 * @since_tizen 2.3
 */
#define DBOX_NR_OF_SIZE_LIST 13

#define DBOX_CH_IDX(a, idx) (((char *)(a))[(idx)])
#define DBOX_IS_INHOUSE(abi)	((DBOX_CH_IDX(abi, 0) == 'c' || DBOX_CH_IDX(abi, 0) == 'C') && (DBOX_CH_IDX(abi, 1) == '\0' || ((DBOX_CH_IDX(abi, 1) == 'p' || DBOX_CH_IDX(abi, 1) == 'P') && (DBOX_CH_IDX(abi, 2) == 'p' || DBOX_CH_IDX(abi, 2) == 'P') && DBOX_CH_IDX(abi, 3) == '\0')))

/**
 * @brief Enumeration for list of supporting dynamicbox size types.
 * @since_tizen 2.3
 */
typedef enum dynamicbox_size_type {
	DBOX_SIZE_TYPE_1x1 = 0x0001,      /**< 175x175 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_2x1 = 0x0002,      /**< 354x175 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_2x2 = 0x0004,      /**< 354x354 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x1 = 0x0008,      /**< 712x175 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x2 = 0x0010,      /**< 712x354 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x3 = 0x0020,      /**< 712x533 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x4 = 0x0040,      /**< 712x712 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x5 = 0x0080,      /**< 712x891 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_4x6 = 0x0100,      /**< 712x1070 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_EASY_1x1 = 0x1000, /**< 224x215 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_EASY_3x1 = 0x2000, /**< 680x215 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_EASY_3x3 = 0x4000, /**< 680x653 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_0x0 = 0x0800,      /**< 720x1280 based on 720x1280 resolution */
	DBOX_SIZE_TYPE_UNKNOWN = 0xFFFF   /**< Error */
} dynamicbox_size_type_e;

/**
 * @brief Enumeration for result of accessibility event processing.
 * @details Reference the libprovider & libdynamicbox-viewer.
 * @since_tizen 2.3
 */
typedef enum dynamicbox_access_status {
	DBOX_ACCESS_STATUS_ERROR = 0x80000000,  /**< Mask value */
	DBOX_ACCESS_STATUS_DONE = 0x00000000,   /**< Successfully finished */
	DBOX_ACCESS_STATUS_FIRST,               /**< Reach to the first item */
	DBOX_ACCESS_STATUS_LAST,                /**< Reach to the last item */
	DBOX_ACCESS_STATUS_READ                 /**< TTS is done */
} dynamicbox_access_status_e;

/**
 * @brief Key event handling result status.
 * @since_tizen 2.3
 */
typedef enum dynamicbox_key_status {
	DBOX_KEY_STATUS_ERROR = 0x80000000, /**< Key operation is failed */
	DBOX_KEY_STATUS_DONE = 0x00000000,  /**< Key operation is successfully done */
	DBOX_KEY_STATUS_FIRST,              /**< Focusable object item reaches to the first in it */
	DBOX_KEY_STATUS_LAST,               /**< Focusable object item reaches to the last in it */
} dynamicbox_key_status_e;

/**
 * @brief Delete type for dynamicbox delete operation.
 * @since_tizen 2.3
 */
typedef enum dynamicbox_delete_type {
	DBOX_DELETE_PERMANENTLY = 0x01, /**< The dynamicbox is removed from the homescreen temporary */
	DBOX_DELETE_TEMPORARY = 0x02,   /**< The dynamicbox is removed from the homescreen by user permanently */
	DBOX_DELETE_INVALID = 0xff,     /**< Unknown event type */
} dynamicbox_delete_type_e;

/**
 * @brief Request type for closing Glance Bar
 * @since_tizen 2.3
 */
typedef enum dynamicbox_gbar_close_type {
	DBOX_CLOSE_GBAR_NORMAL = 0x00,  /**< Glance Bar is closed normally */
	DBOX_CLOSE_GBAR_TIMEOUT = 0x01, /**< Glance Bar is closed because of timeout, there is no response */
	DBOX_CLOSE_GBAR_FAULT = 0x02,   /**< Glance Bar is closed because of unrecoverable error */
	DBOX_CLOSE_GBAR_INVALID = 0xff, /**< Unknown event type */
} dynamicbox_gbar_close_type_e;

/**
 * @brief Type of dynamicbox content sharing method
 * @since_tizen 2.3
 */
typedef enum dynamicbox_dbox_type {
	DBOX_TYPE_NONE = 0x0, /**< Undefined */
	DBOX_TYPE_SCRIPT,     /**< Script base */
	DBOX_TYPE_FILE,       /**< File base */
	DBOX_TYPE_TEXT,       /**< Text base */
	DBOX_TYPE_BUFFER,     /**< Buffer base */
	DBOX_TYPE_UIFW        /**< UIFW supported type */
} dynamicbox_dbox_type_e;

/**
 * @brief Type of glance bar content sharing method
 * @since_tizen 2.3
 */
typedef enum dynamicbox_gbar_type {
	GBAR_TYPE_NONE = 0x0, /**< Undefined */
	GBAR_TYPE_SCRIPT,     /**< Script base */
	GBAR_TYPE_TEXT,       /**< Text base */
	GBAR_TYPE_BUFFER,     /**< Buffer base */
	GBAR_TYPE_UIFW        /**< UIFW supported type */
} dynamicbox_gbar_type_e;

typedef enum dynamicbox_destroy_type {
	DBOX_DESTROY_TYPE_DEFAULT = 0x00,   /**< Deleted */
	DBOX_DESTROY_TYPE_UPGRADE = 0x01,   /**< Deleted for upgrading */
	DBOX_DESTROY_TYPE_UNINSTALL = 0x02, /**< Deleted by uninstalling */
	DBOX_DESTROY_TYPE_TERMINATE = 0x03, /**< Deleted for reboot device */
	DBOX_DESTROY_TYPE_FAULT = 0x04,     /**< Deleted by system-fault */
	DBOX_DESTROY_TYPE_TEMPORARY = 0x05, /**< Temporarly deleted, will be created again */
	DBOX_DESTROY_TYPE_UNKNOWN = 0x06    /**< Undefined reason */
} dynamicbox_destroy_type_e; /**< Delete type */

/**
 * @brief Package list handle.
 * @since_tizen 2.3
 */
typedef struct dynamicbox_pkglist_handle *dynamicbox_pkglist_h;

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
 * @since_tizen 2.3
 * @param[in] type Size type
 * @param[out] width Pixel size width
 * @param[out] height Pixel size height
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully done
 * @see dynamicbox_size_type()
 * @see dynamicbox_service_size_type()
 */
extern int dynamicbox_service_get_size(dynamicbox_size_type_e type, int *width, int *height);

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
 * @since_tizen 2.3
 * @param[in] width Pixel size width
 * @param[in] height Pixel size height
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_SIZE_TYPE_[EASY_]WxH Size type of given pixel size
 * @retval #DBOX_SIZE_TYPE_UNKNOWN If the given pixel size is not valid
 * @see dynamicbox_size_type()
 * @see dynamicbox_service_get_size()
 */
extern dynamicbox_size_type_e dynamicbox_service_size_type(int width, int height);

/**
 * @brief Supports the mouse event of dynamicbox content.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] size_type Size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] size_type Size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval 1 If the box requires frame for decorating its contents
 * @retval 0 If the box doesn't requires frame
 * @see dynamicbox_service_mouse_event()
 * @see dynamicbox_service_touch_effect()
 */
extern int dynamicbox_service_need_frame(const char *dboxid, int size_type);

/**
 * @brief Triggers the update event for given dynamicbox instance.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] cluster Cluster name. Default @c NULL
 * @param[in] category Category name, Default @c NULL
 * @param[in] content New content information, Default @c NULL
 * @param[in] force 1 if you want to update your dynamicbox even if the provider is paused or 0. 0 is default
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] instance_id Dynamicbox instance id
 * @param[in] period New update period in sec
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox App Id
 * @param[in] cb Callback function
 * @param[in] data Callback Data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox Package Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] pkgid Package Id (Not the UI App Id)
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] category Name of category
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int count
 * @retval Count of dynamicbox packages
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @see dynamicbox_service_get_pkglist_by_pkgid()
 */
extern int dynamicbox_service_get_pkglist_by_category(const char *category, int (*cb)(const char *dboxid, void *data), void *data);

/**
 * @brief Gets the id of a primary dynamicbox using given (dbox or package or UI app) Id. If a given id is a dynamicbox id, check its existence. and if it is package or ui-app id, then find the primary dynamicbox in its package.
 * @since_tizen 2.3
 * @param[in] id Dynamic Box Id or Package Id or UI App Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval @c NULL Failed to get primary dboxid
 * @retval dboxid Primary dynamicbox Id. which is allocated in the heap
 * @pre Must be released returned string by manually.
 * @see dynamicbox_service_package_id()
 */
extern char *dynamicbox_service_dbox_id(const char *id);

/**
 * @brief Checks the primary flag of given dynamicbox Id.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval 0 If is not a primary
 * @retval 1 If it is a primary dynamicbox
 */
extern int dynamicbox_service_is_primary(const char *dboxid);

/**
 * @brief Get the category using given dboxid.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 *    Dynamicbox has provider process for each dynamicbox instances.\n
 *    To get the provider's package name, you can use this API.\n
 *    If the given dboxid is inhouse dynamicbox, the return string will be the same with given argument but it is allocated in the heap.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval @c NULL Failed to get provider name
 * @retval dboxid Dynamicbox AppId which is allocated on the heap
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_provider_name(const char *dboxid);

/**
 * @brief Gets the appId of setup app which is specified by given dynamicbox Id's manifest.
 * @details
 *    This setup app should be launched before adding the dynamicbox to get the content_info.\n
 *    This function should be called before add a dynamicbox.\n
 *    To determine the content information string.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval @c NULL There is no setup application
 * @retval appid AppId if exists or @c NULL
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_setup_appid(const char *dboxid);

/**
 * @brief Gets the Package Id (Not the UI App Id) of given dynamicbox, &lt;manifest package="AAA"&gt; tag.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval appid String which is allocated in the heap
 * @retval @c NULL Invalid appid
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_pkgname()
 */
extern char *dynamicbox_service_package_id(const char *dboxid);

/**
 * @brief Gives Internationalized name of dynamicbox package.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] size_type Dynamicbox size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 *    If the user defines the default content string in the manifest file (.xml),\n
 *    this API will return it.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId (It must has to be a dynamicbox package ID. not the UI-APP and the PACKAGE)
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval 1 The box should not be listed by the dynamicbox list app
 * @retval 0 Box should be listed
 * @pre Dynamicbox tag includes "nodisplay" attribute.
 */
extern int dynamicbox_service_nodisplay(const char *dboxid);

/**
 * @brief Gets the "ABI" of given package.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval abi String which is allocated in the heap
 * @retval @c NULL Failed to get ABI of given dynamicbox
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_abi(const char *dboxid);

/**
 * @brief Checks the status of the dynamicbox.
 * @details Currently this API is not implemented. It just returns 1 all the time.
 * @since_tizen 2.3
 * @remarks This API is not implemented. It will always return 1.
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval 1 Enabled
 * @retval 0 Disabled
 */
extern int dynamicbox_service_is_enabled(const char *dboxid);

/**
 * @brief Gets the script file of dynamicbox.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL No specified script file for Glance Bar layout
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_gbar_script_group()
 */
extern char *dynamicbox_service_gbar_script_path(const char *dboxid);

/**
 * @brief Gets the group name for script file to load it.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval string Script group name
 * @retval @c NULL No script path
 * @post Returned string must be free'd manually.
 * @see dynamicbox_service_gbar_script_path()
 */
extern char *dynamicbox_service_gbar_script_group(const char *dboxid);

/**
 * @brief Gets the supported size list.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] cnt Count of array w and h
 * @param[in] w Width array
 * @param[in] h Height array
 * @param[out] cnt Count of array w and h
 * @param[out] w Width array
 * @param[out] h Height array
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE If succeed to get supported size list
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @see dynamicbox_service_get_supported_size_types()
 */
extern int dynamicbox_service_get_supported_sizes(const char *dboxid, int *cnt, int *w, int *h);

/**
 * @brief Gets the supported size list of given pkgid.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] cnt Size of types array
 * @param[out] cnt Result count of types array
 * @param[out] types Array of types
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_NONE Successfully done
 * @see dynamicbox_service_get_supported_sizes()
 */
extern int dynamicbox_service_get_supported_size_types(const char *dboxid, int *cnt, int *types);

/**
 * @brief Gets the category list of given cluster.
 * @since_tizen 2.3
 * @param[in] cluster Cluster name
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully done
 * @retval #DBOX_STATUS_ERROR_IO Failed to access DB
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @see dynamicbox_service_enumerate_cluster_list()
 */
extern int dynamicbox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/**
 * @brief Gets the cluster list.
 * @since_tizen 2.3
 * @param[in] cb Callback function for retrieving the cluster list
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Succeed to initialize
 * @retval #DBOX_STATUS_ERROR_IO Failed to access a DB
 * @see dynamicbox_service_fini()
 */
extern int dynamicbox_service_init(void);

/**
 * @brief Finalizes the dynamicbox service API.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
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
 * @since_tizen 2.3
 * @remarks
 *    If you call this function again using created pkglist handle, it will be reset.
 *    So you can get records from the first one again.
 * @param[in] dboxid Dynamicbox AppId
 * @param[in] handle @c NULL if you call this first, or it will be reset
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return handle
 * @retval @c NULL If it fails
 * @retval handle If it successfully create the package list iterator
 * @see dynamicbox_service_pkglist_destroy()
 */
extern dynamicbox_pkglist_h dynamicbox_service_pkglist_create(const char *dboxid, dynamicbox_pkglist_h handle);

/**
 * @brief Gets the dboxid & package name & is_prime flag.
 * @since_tizen 2.3
 * @param[in] handle Handle which is created by dynamicbox_service_pkglist_create() function
 * @param[out] dboxid  Dynamicbox Id
 * @param[out] pkgname Package Id which includes dynamicboxes
 * @param[out] is_prime If the returned dboxid is primary, this will be 1 or 0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_NONE Successfully get the record
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid argument
 * @retval #DBOX_STATUS_ERROR_NOT_EXIST Reach to the end of result set. you can rewind the iterator call dynamicbox_service_pkglist_create() again with current handle
 * @retval #DBOX_STATUS_ERROR_MEMORY Not enough memory
 * @post You must release the dboxid, pkgname manually.
 * @see dynamicbox_service_pkglist_create()
 * @see dynamicbox_service_pkglist_destroy()
 */
extern int dynamicbox_service_get_pkglist_item(dynamicbox_pkglist_h handle, char **dboxid, char **pkgname, int *is_prime);

/**
 * @brief Destroys the iterator of pkglist.
 * @since_tizen 2.3
 * @param[in] handle Package list handle
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return int type
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid handle
 * @retval #DBOX_STATUS_ERROR_NONE Successfully destroyed
 * @pre Handle must be created by dynamicbox_service_pkglist_create().
 * @post You have not to use the handle again after destroy it.
 * @see dynamicbox_service_pkglist_create()
 */
extern int dynamicbox_service_pkglist_destroy(dynamicbox_pkglist_h handle);

/**
 * @brief Getting the activated instance count.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox Id, if you want to get whole instnaces list, use NULL.
 * @param[in] cluster Cluster name if you don't know what this is, use NULL.
 * @param[in] category Sub-cluster(category) name if you don't know what this is, use NULL.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return count of instances
 * @retval #DBOX_STATUS_ERROR_INVALID Invalid parameter
 * @retval #DBOX_STATUS_ERROR_FAULT Unrecorvarable error occurred
 * @retval count Positive value including ZERO, Count of activated instances on viewers
 */
extern int dynamicbox_service_get_instance_count(const char *dboxid, const char *cluster, const char *category);

/**
 * @internal
 * @brief Gets the path of the plug-in module.
 * @since_tizen 2.3
 * @param[in] dboxid Dynamicbox AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char * type
 * @retval path String which is allocated on the heap
 * @retval @c NULL No libexec attribute
 * @post Returned string must be free'd manually.
 */
extern char *dynamicbox_service_libexec(const char *dboxid);

/**
 * @internal
 * @brief Find the lbid using its libexec
 * @details only if the dynamicbox should use the "libexec" attribute in its "<dynamicbox>" tag
 * @since_tizen 2.3
 * @remarks Only usable for inhouse dynamicboxes
 * @param[in] libexec so filename
 * @privlevel public
 * @privilege %http://tizen.org/privilege/dynamicbox
 * @return char *
 * @retval @c NULL if it fails to get pkgname
 * @retval address heap address of pkgname
 * @post return'd string should be released by "free()"
 */
extern char *dynamicbox_service_dbox_id_by_libexec(const char *libexec);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

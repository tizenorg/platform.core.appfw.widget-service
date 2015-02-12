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

#ifndef __WIDGET_SERVICE_H
#define __WIDGET_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file dynamic-service.h
 * @brief  This file declares API of libdynamic-service library
 * @since_tizen 2.4
 */

/**
 * @addtogroup CAPI_WIDGET_SERVICE_MODULE
 * @{
 */

/**
 * @brief Index of buffer
 * @since_tizen 2.4
 */
#define WIDGET_PRIMARY_BUFFER -1
#define WIDGET_UNKNOWN_BUFFER -2

/**
 * @brief Definition for maximum number of supported widget size type.
 * @since_tizen 2.4
 */
#define WIDGET_NR_OF_SIZE_LIST 13

#define WIDGET_CH_IDX(a, idx) (((char *)(a))[(idx)])
#define WIDGET_IS_INHOUSE(abi)    ((WIDGET_CH_IDX(abi, 0) == 'c' || WIDGET_CH_IDX(abi, 0) == 'C') && (WIDGET_CH_IDX(abi, 1) == '\0' || ((WIDGET_CH_IDX(abi, 1) == 'p' || WIDGET_CH_IDX(abi, 1) == 'P') && (WIDGET_CH_IDX(abi, 2) == 'p' || WIDGET_CH_IDX(abi, 2) == 'P') && WIDGET_CH_IDX(abi, 3) == '\0')))

/**
 * @brief Enumeration for list of supporting widget size types.
 * @since_tizen 2.4
 */
typedef enum widget_size_type {
	WIDGET_SIZE_TYPE_1x1 = 0x0001,      /**< 175x175 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_2x1 = 0x0002,      /**< 354x175 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_2x2 = 0x0004,      /**< 354x354 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_4x1 = 0x0008,      /**< 712x175 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_4x2 = 0x0010,      /**< 712x354 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_4x3 = 0x0020,      /**< 712x533 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_4x4 = 0x0040,      /**< 712x712 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_4x5 = 0x0080,      /**< 712x891 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_4x6 = 0x0100,      /**< 712x1070 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_EASY_1x1 = 0x1000, /**< 224x215 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_EASY_3x1 = 0x2000, /**< 680x215 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_EASY_3x3 = 0x4000, /**< 680x653 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_0x0 = 0x0800,      /**< 720x1280 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_UNKNOWN = 0xFFFF   /**< Error */
} widget_size_type_e;

/**
 * @internal
 * @brief Enumeration for result of accessibility event processing.
 * @details Reference the libprovider & libwidget-viewer.
 * @since_tizen 2.4
 */
typedef enum widget_access_status {
	WIDGET_ACCESS_STATUS_ERROR = 0x80000000,  /**< Mask value */
	WIDGET_ACCESS_STATUS_DONE = 0x00000000,   /**< Successfully finished */
	WIDGET_ACCESS_STATUS_FIRST,               /**< Reach to the first item */
	WIDGET_ACCESS_STATUS_LAST,                /**< Reach to the last item */
	WIDGET_ACCESS_STATUS_READ                 /**< TTS is done */
} widget_access_status_e;

/**
 * @internal
 * @brief Key event handling result status.
 * @since_tizen 2.4
 */
typedef enum widget_key_status {
	WIDGET_KEY_STATUS_ERROR = 0x80000000, /**< Key operation is failed */
	WIDGET_KEY_STATUS_DONE = 0x00000000,  /**< Key operation is successfully done */
	WIDGET_KEY_STATUS_FIRST,              /**< Focusable object item reaches to the first in it */
	WIDGET_KEY_STATUS_LAST,               /**< Focusable object item reaches to the last in it */
} widget_key_status_e;

/**
 * @brief Delete type for widget delete operation.
 * @since_tizen 2.4
 */
typedef enum widget_delete_type {
	WIDGET_DELETE_PERMANENTLY = 0x01, /**< The widget is removed from the homescreen temporary */
	WIDGET_DELETE_TEMPORARY = 0x02,   /**< The widget is removed from the homescreen by user permanently */
	WIDGET_DELETE_INVALID = 0xff,     /**< Unknown event type */
} widget_delete_type_e;

/**
 * @brief Request type for closing Glance Bar
 * @since_tizen 2.4
 */
typedef enum widget_gbar_close_type {
	WIDGET_CLOSE_GBAR_NORMAL = 0x00,  /**< Glance Bar is closed normally */
	WIDGET_CLOSE_GBAR_TIMEOUT = 0x01, /**< Glance Bar is closed because of timeout, there is no response */
	WIDGET_CLOSE_GBAR_FAULT = 0x02,   /**< Glance Bar is closed because of unrecoverable error */
	WIDGET_CLOSE_GBAR_INVALID = 0xff, /**< Unknown event type */
} widget_gbar_close_type_e;

/**
 * @internal
 * @brief Type of widget content sharing method
 * @since_tizen 2.4
 */
typedef enum widget_widget_type {
	WIDGET_TYPE_NONE = 0x0, /**< Undefined */
	WIDGET_TYPE_SCRIPT,     /**< Script base */
	WIDGET_TYPE_FILE,       /**< File base */
	WIDGET_TYPE_TEXT,       /**< Text base */
	WIDGET_TYPE_BUFFER,     /**< Buffer base */
	WIDGET_TYPE_UIFW        /**< UIFW supported type */
} widget_widget_type_e;

/**
 * @internal
 * @brief Type of glance bar content sharing method
 * @since_tizen 2.4
 */
typedef enum widget_gbar_type {
	GBAR_TYPE_NONE = 0x0, /**< Undefined */
	GBAR_TYPE_SCRIPT,     /**< Script base */
	GBAR_TYPE_TEXT,       /**< Text base */
	GBAR_TYPE_BUFFER,     /**< Buffer base */
	GBAR_TYPE_UIFW        /**< UIFW supported type */
} widget_gbar_type_e;

/**
 * @brief Destroy type of widget instance
 * @since_tizen 2.4
 */
typedef enum widget_destroy_type {
	WIDGET_DESTROY_TYPE_DEFAULT = 0x00,   /**< Deleted */
	WIDGET_DESTROY_TYPE_UPGRADE = 0x01,   /**< Deleted for upgrading */
	WIDGET_DESTROY_TYPE_UNINSTALL = 0x02, /**< Deleted by uninstalling */
	WIDGET_DESTROY_TYPE_TERMINATE = 0x03, /**< Deleted for reboot device */
	WIDGET_DESTROY_TYPE_FAULT = 0x04,     /**< Deleted by system-fault */
	WIDGET_DESTROY_TYPE_TEMPORARY = 0x05, /**< Temporarly deleted, will be created again */
	WIDGET_DESTROY_TYPE_UNKNOWN = 0x06    /**< Undefined reason */
} widget_destroy_type_e; /**< Delete type */

/**
 * @brief
 * Text signal & Content event uses this data structure.
 * @since_tizen 2.4
 */
typedef struct widget_event_info {
	struct _pointer {
		double x; /**< X value of current mouse(touch) position */
		double y; /**< Y value of current mouse(touch) position */
		int down; /**< Is it pressed(1) or not(0) */
	} pointer;

	struct _part {
		double sx; /**< Pressed object's left top X */
		double sy; /**< Pressed object's left top Y */
		double ex; /**< Pressed object's right bottom X */
		double ey; /**< Pressed object's right bottom Y */
	} part;
} *widget_event_info_t;

/**
 * @brief widget Buffer Handler
 * @since_tizen 2.4
 */
typedef struct widget_buffer *widget_buffer_h;

/**
 * @brief widget Buffer Event information
 * @since_tizen 2.4
 */
typedef struct widget_buffer_event_data *widget_buffer_event_data_t;

/**
 * @brief widget Buffer Handler
 * @since_tizen 2.4
 */
typedef struct widget_buffer *widget_buffer_h;

/**
 * @internal
 * @brief This enumeration values should be sync'd with libwidget interface. (only for inhouse widget)
 * @since_tizen 2.4
 */
typedef enum widget_buffer_event {
	WIDGET_BUFFER_EVENT_ENTER, /**< get the focus */
	WIDGET_BUFFER_EVENT_LEAVE, /**< lost the focus */
	WIDGET_BUFFER_EVENT_DOWN, /**< Touch down */
	WIDGET_BUFFER_EVENT_MOVE, /**< Touch move */
	WIDGET_BUFFER_EVENT_UP, /**< Touch up */

	WIDGET_BUFFER_EVENT_KEY_DOWN, /**< Key pressed */
	WIDGET_BUFFER_EVENT_KEY_UP, /**< Key release */
	WIDGET_BUFFER_EVENT_KEY_FOCUS_IN, /**< Focus in */
	WIDGET_BUFFER_EVENT_KEY_FOCUS_OUT, /**< Focus out */

	WIDGET_BUFFER_EVENT_ACCESS_HIGHLIGHT, /**< Accessibility Highlight event */
	WIDGET_BUFFER_EVENT_ACCESS_HIGHLIGHT_NEXT, /**< Accessibility Highlight Next event */
	WIDGET_BUFFER_EVENT_ACCESS_HIGHLIGHT_PREV, /**< Accessibility Highlight Prev event */
	WIDGET_BUFFER_EVENT_ACCESS_ACTIVATE, /**< Accessibility Activate event */
	WIDGET_BUFFER_EVENT_ACCESS_ACTION_UP, /**< Accessibility Action Up event */
	WIDGET_BUFFER_EVENT_ACCESS_ACTION_DOWN, /**< Accessibility Action Down event */
	WIDGET_BUFFER_EVENT_ACCESS_SCROLL_UP, /**< Accessibility Scroll Mouse Up event */
	WIDGET_BUFFER_EVENT_ACCESS_SCROLL_MOVE, /**< Accessibility Scroll Mouse Move event */
	WIDGET_BUFFER_EVENT_ACCESS_SCROLL_DOWN, /**< Accessibility Scroll Mouse Down event */
	WIDGET_BUFFER_EVENT_ACCESS_UNHIGHLIGHT, /**< Accessibility Unhighlight event */

	WIDGET_BUFFER_EVENT_ON_HOLD,    /**< To prevent from generating mouse clicked event */
	WIDGET_BUFFER_EVENT_OFF_HOLD, /**< Disable the mouse hold event */
	WIDGET_BUFFER_EVENT_ON_SCROLL, /**< Enable the scroll flag */
	WIDGET_BUFFER_EVENT_OFF_SCROLL, /**< Disable the scroll flag */

	WIDGET_BUFFER_EVENT_ACCESS_VALUE_CHANGE, /**< */
	WIDGET_BUFFER_EVENT_ACCESS_MOUSE, /**< give mouse event to highlight object */
	WIDGET_BUFFER_EVENT_ACCESS_BACK, /**< go back to a previous view ex: pop naviframe item */
	WIDGET_BUFFER_EVENT_ACCESS_OVER, /**< mouse over an object */
	WIDGET_BUFFER_EVENT_ACCESS_READ, /**< highlight an object */
	WIDGET_BUFFER_EVENT_ACCESS_ENABLE, /**< enable highlight and read ability */
	WIDGET_BUFFER_EVENT_ACCESS_DISABLE /**< disable highlight and read ability */
} widget_buffer_event_e;

/**
 * @internal
 * @brief Dynamic Box Buffer Event Data
 * @since_tizen 2.4
 */
typedef struct widget_buffer_event_data {
	widget_buffer_event_e type; /**< Event type */
	double timestamp; /**< Timestamp */

	union input_data {
		struct mouse {
			int x; /**< Touch X coordinate */
			int y; /**< Touch Y coordinate */
		} pointer;

		struct access {
			int x; /**< Accessibility event X coordinate */
			int y; /**< Accessibility event Y coordinate */
			unsigned int mouse_type; /**< 0: down, 1: move, 2: up | 0: cur, 1: next, 2: prev, 3: off */
			unsigned int action_type; /**< reserved for protocol */
			unsigned int action_by; /**< reserved for protocol */
			int cycle; /**< reserved for protocol */
		} access;

		unsigned int keycode; /**< Key code value */
	} info;
} *widget_buffer_event_data_t;

/**
 * @internal
 * @brief Package list handle.
 * @since_tizen 2.4
 */
typedef struct widget_pkglist_handle *widget_pkglist_h;

/**
 * @brief Gets the pixel size of given size type.
 * @details
 *  Size types would be\n
 *  #WIDGET_SIZE_TYPE_1x1\n
 *  #WIDGET_SIZE_TYPE_2x1\n
 *  #WIDGET_SIZE_TYPE_2x2\n
 *  #WIDGET_SIZE_TYPE_4x1\n
 *  #WIDGET_SIZE_TYPE_4x2\n
 *  #WIDGET_SIZE_TYPE_4x3\n
 *  #WIDGET_SIZE_TYPE_4x4\n
 *  #WIDGET_SIZE_TYPE_4x5\n
 *  #WIDGET_SIZE_TYPE_4x6\n
 *  #WIDGET_SIZE_TYPE_0x0\n
 *  #WIDGET_SIZE_TYPE_EASY_1x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x3.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] type Size type
 * @param[out] width Pixel size width
 * @param[out] height Pixel size height
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully done
 * @see widget_size_type()
 * @see widget_service_size_type()
 */
extern int widget_service_get_size(widget_size_type_e type, int *width, int *height);

/**
 * @brief Gets the size type for given pixel size.
 * @details
 *  Returnable size types are\n
 *  #WIDGET_SIZE_TYPE_1x1\n
 *  #WIDGET_SIZE_TYPE_2x1\n
 *  #WIDGET_SIZE_TYPE_2x2\n
 *  #WIDGET_SIZE_TYPE_4x1\n
 *  #WIDGET_SIZE_TYPE_4x2\n
 *  #WIDGET_SIZE_TYPE_4x3\n
 *  #WIDGET_SIZE_TYPE_4x4\n
 *  #WIDGET_SIZE_TYPE_4x5\n
 *  #WIDGET_SIZE_TYPE_4x6\n
 *  #WIDGET_SIZE_TYPE_0x0\n
 *  #WIDGET_SIZE_TYPE_EASY_1x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x3\n
 *  or\n
 *  #WIDGET_SIZE_TYPE_UNKNOWN for error.
 * @since_tizen 2.4
 * @param[in] width Pixel size width
 * @param[in] height Pixel size height
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_SIZE_TYPE_[EASY_]WxH Size type of given pixel size
 * @retval #WIDGET_SIZE_TYPE_UNKNOWN If the given pixel size is not valid, widget_last_status() will returns reason of failure.
 * @see widget_size_type()
 * @see widget_service_get_size()
 */
extern widget_size_type_e widget_service_size_type(int width, int height);

/**
 * @brief Supports the mouse event of widget content.
 * @details This function will returns true/false, but even if this fails to access database to retrieve information,
 *          This will returns 0. in that case, you can check it using widget_last_status() function.
 *	    If there is an error, it will returns proper error code.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid widget AppId
 * @return int type
 * @retval 1 If the box requires mouse event, A viewer must has to feed the mouse event to the box instance
 * @retval 0 If the box doesn't requires mouse event, In this case, you can check whether it is error or not using widget_last_status() function.
 * @see widget_service_touch_effect()
 * @see widget_service_need_frame()
 * @see widget_last_status()
 */
extern int widget_service_mouse_event(const char *widgetid, int size_type);

/**
 * @brief Requires touch effect.
 * @details If this API returns true(1), the viewer should make touch effect when a user click the widget.
 *          This function returns 1 even if it fails to retrieve information from Database.
 *	    So if you need validate the information whether it is correct or not, you can use widget_last_status() function.
 *          Even if this function returns 1, It is recommended to check last status of this function call.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid widget AppId
 * @param[in] size_type Size type
 * @return int type
 * @retval 1 If the box requires touch effect, A viewer should make the touch effect, but it is just recomendation.
 * @retval 0 If the box doesn't requires touch effect, the box will make touch effect itself
 * @see widget_service_mouse_event()
 * @see widget_service_need_frame()
 * @see widget_last_status()
 */
extern int widget_service_touch_effect(const char *widgetid, int size_type);

/**
 * @brief Requires decoration frame.
 * @details If this API returns true(1), the viewer should make decoration border on the widget content.
 *          If this function returns 0, you can validate it using widget_last_status() function.
 *          If something goes wrong, so this fails to retrieve information, you can check reason why it fails to get it
 *          using widget_last_status() function.        
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid widget AppId
 * @param[in] size_type Size type
 * @return int type
 * @retval 1 If the box requires frame for decorating its contents
 * @retval 0 If the box doesn't requires frame
 * @see widget_service_mouse_event()
 * @see widget_service_touch_effect()
 * @see widget_last_status()
 */
extern int widget_service_need_frame(const char *widgetid, int size_type);

/**
 * @brief Triggers the update event for given widget instance.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] cluster Cluster name. Default @c NULL
 * @param[in] category Category name, Default @c NULL
 * @param[in] content New content information, Default @c NULL
 * @param[in] force 1 if you want to update your widget even if the provider is paused or 0. 0 is default
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.provider
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_CANCEL Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * @retval #WIDGET_STATUS_ERROR_MEMORY Memory is not enough to make request
 * @retval #WIDGET_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully requested
 * @see widget_service_trigger_update()
 */
extern int widget_service_trigger_update(const char *widgetid, const char *instance_id, const char *cluster, const char *category, const char *content, int force);

/**
 * @brief Changes the update period of given widget instance.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @param[in] instance_id widget instance id
 * @param[in] period New update period in sec
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.provider
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully changed(requested)
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_FAULT Failed to create a request packet
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_STATUS_ERROR_MEMORY Not enough memory
 */
extern int widget_service_change_period(const char *widgetid, const char *instance_id, double period);

/**
 * @internal
 * @brief Gets synchronous package list.
 * @details
 *        callback (appid, widgetid, is_prime)\n
 *        pkgid == Package ID (pkgname)\n
 *        widgetid = widget AppId\n
 *        is_prime = 1 if the widget is default one for associated application package\n
 *        If the callback returns negative value, the list crawling will be stopped.
 * @since_tizen 2.4
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @retval count Count of widget packages
 * @see widget_service_get_pkglist_by_pkgid()
 */
extern int widget_service_get_pkglist(int (*cb)(const char *pkgid, const char *widgetid, int is_prime, void *data), void *data);

/**
 * @internal
 * @brief Gets synchronous uiapp list.
 * @details
 *        Callback (appid, data)\n
 *        This function will retrieve all UI Apps in a package which has given widget appid(widgetid).\n
 *        If you need to get all ui-app list, using a widget appid, this function is able to help you.
 * @since_tizen 2.4
 * @param[in] widgetid widget App Id
 * @param[in] cb Callback function
 * @param[in] data Callback Data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE Status success
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_MEMORY Memory error
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_STATUS_ERROR_FAULT Unrecoverable error occurred
 */
extern int widget_service_get_applist(const char *widgetid, void (*cb)(const char *widgetid, const char *appid, void *data), void *data);

/**
 * @brief Gets the MAIN application Id of given widget package Id.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid widget Package Id
 * @return char * type
 * @retval @c NULL If it fails to get main application Id (UI-APPID), widget_last_status() will returns reason of failure.
 * @retval appid Main application Id
 * @see widget_last_status()
 */
extern char *widget_service_mainappid(const char *widgetid);

/**
 * @internal
 * @brief Gets synchronous package list.
 * @details
 *      callback (widgetid, is_prime)\n
 *      widgetid == widget AppId\n
 *      is_prime = 1 if the widget is default one for selected package\n
 *      If the callback returns negative value, the list crawling will be stopped.
 * @since_tizen 2.4
 * @param[in] pkgid Package Id (Not the UI App Id)
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval int Count of widget packages
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_pkglist()
 */
extern int widget_service_get_pkglist_by_pkgid(const char *pkgid, int (*cb)(const char *widgetid, int is_prime, void *data), void *data);

/**
 * @internal
 * @brief Gets synchronous package list.
 * @details
 *      callback (widgetid)\n
 *      widgetid == widget AppId\n
 *        If the callback returns negative value, the list crawling will be stopped
 * @since_tizen 2.4
 * @param[in] category Name of category
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int count
 * @retval Count of widget packages
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_pkglist_by_pkgid()
 */
extern int widget_service_get_pkglist_by_category(const char *category, int (*cb)(const char *widgetid, void *data), void *data);

/**
 * @brief Gets the id of a primary widget using given (widget or package or UI app) Id. If a given id is a widget id, check its existence. and if it is package or ui-app id, then find the primary widget in its package.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] id Dynamic Box Id or Package Id or UI App Id
 * @return char * type
 * @retval @c NULL Failed to get primary widgetid, widget_last_status() will returns reason of failure.
 * @retval widgetid Primary widget Id. which is allocated in the heap
 * @pre Must be released returned string by manually.
 * @see widget_service_package_id()
 */
extern char *widget_service_widget_id(const char *id);

/**
 * @internal
 * @brief Checks the primary flag of given widget Id.
 * @since_tizen 2.4
 * @param[in] widgetid widget Id
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval 0 If is not a primary, widget_last_status() will returns reason of failure if it fails.
 * @retval 1 If it is a primary widget
 */
extern int widget_service_is_primary(const char *widgetid);

/**
 * @internal
 * @brief Get the category using given widgetid.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char *
 * @retval @c NULL Failed to get primary widgetid, widget_last_status() will returns reason of failure if it fails.
 * @retval category Category string which is allocated in the heap.
 * @pre Must be released returned string by manually
 * @post N/A
 * @see widget_service_widget_id()
 */
extern char *widget_service_category(const char *widgetid);

/**
 * @brief Gets the name of a widget (provider name == widget appid), you have to release the return value after use it.
 * @details
 *    widget has provider process for each widget instances.\n
 *    To get the provider's package name, you can use this API.\n
 *    If the given widgetid is inhouse widget, the return string will be the same with given argument but it is allocated in the heap.
 * @since_tizen 2.4
 * @privlevel N/P
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @param[in] widgetid widget Id
 * @retval @c NULL Failed to get provider name, widget_last_status() will returns reason of failure if it fails.
 * @retval widgetid widget AppId which is allocated on the heap
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_provider_name(const char *widgetid);

/**
 * @brief Gets the appId of setup app which is specified by given widget Id's manifest.
 * @details
 *    This setup app should be launched before adding the widget to get the content_info.\n
 *    This function should be called before add a widget.\n
 *    To determine the content information string.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid widget Id
 * @return char * type
 * @retval @c NULL There is no setup application or error occurred, you can check it using widget_last_status()
 * @retval appid AppId if exists or @c NULL
 * @post Returned string must be free'd manually.
 * @see widget_last_status()
 */
extern char *widget_service_setup_appid(const char *widgetid);

/**
 * @internal
 * @brief Gets the Package Id (Not the UI App Id) of given widget, &lt;manifest package="AAA"&gt; tag.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval appid String which is allocated in the heap
 * @retval @c NULL Invalid appid, widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_widget_id()
 */
extern char *widget_service_package_id(const char *widgetid);

/**
 * @internal
 * @brief Gives Internationalized name of widget package.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval name If it fails to get name
 * @retval @c NULL Allocated heap address, widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd by manually.
 * @see widget_service_i18n_icon()
 * @see widget_service_preview()
 */
extern char *widget_service_i18n_name(const char *widgetid, const char *lang);

/**
 * @internal
 * @brief Gets the preview image path of given size type.
 * @details This function will returns i18nized preview image path.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @param[in] size_type widget size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval path Preview image path
 * @retval @c NULL There is no preview image file, widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_i18n_icon()
 * @see widget_service_i18n_name()
 */
extern char *widget_service_preview(const char *widgetid, int size_type);

/**
 * @internal
 * @brief Gets the default content string of the given widget.
 * @details
 *    If the user defines the default content string in the manifest file (.xml),\n
 *    this API will return it.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval content Content string
 * @retval @c NULL There is no specified content string, or widget_last_status() will returns reason of failure if it fails.
 * @pre Manifest has the default content string. &lt;content&gt;Default content string&lt;content&gt; tag.
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_content(const char *widgetid);

/**
 * @brief Gives Internationalized icon path of given widget package.
 * @details The user should free the returned string with free().
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId (It must has to be a widget package ID. not the UI-APP and the PACKAGE)
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval name Allocated heap address
 * @retval @c NULL Fails to get path of an icon, widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_i18n_name()
 * @see widget_service_preview()
 */
extern char *widget_service_i18n_icon(const char *pkgid, const char *lang);

/**
 * @internal
 * @brief Gets the "nodisplay" value.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval 1 The box should not be listed by the widget list app
 * @retval 0 Box should be listed, widget_last_status() will returns reason of failure if it fails.
 * @pre widget tag includes "nodisplay" attribute.
 */
extern int widget_service_nodisplay(const char *widgetid);

/**
 * @internal
 * @brief Gets the "ABI" of given package.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval abi String which is allocated in the heap
 * @retval @c NULL Failed to get ABI of given widget, widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_abi(const char *widgetid);

/**
 * @internal
 * @brief Checks the status of the widget.
 * @details Currently this API is not implemented. It just returns 1 all the time.
 * @since_tizen 2.4
 * @remarks This API is not implemented. It will always return 1.
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval 1 Enabled
 * @retval 0 Disabled
 */
extern int widget_service_is_enabled(const char *widgetid);

/**
 * @internal
 * @brief Gets the script file of widget.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL Not specified script file, widget_last_status() will returns reason of failure if it fails.
 * @pre widget should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see widget_service_widget_script_group()
 */
extern char *widget_service_widget_script_path(const char *widgetid);

/**
 * @internal
 * @brief Gets the script group of widget.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval Group Name of widget
 * @retval @c NULL If there is no group defined, or widget_last_status() will returns reason of failure if it fails.
 * @pre widget should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see widget_service_widget_script_path()
 */
extern char *widget_service_widget_script_group(const char *widgetid);

/**
 * @internal
 * @brief Gets the script file path of given widget package.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL No specified script file for Glance Bar layout, or widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_gbar_script_group()
 */
extern char *widget_service_gbar_script_path(const char *widgetid);

/**
 * @internal
 * @brief Gets the group name for script file to load it.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval string Script group name
 * @retval @c NULL No script path, or widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_gbar_script_path()
 */
extern char *widget_service_gbar_script_group(const char *widgetid);

/**
 * @internal
 * @brief Gets the supported size list.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @param[in] cnt Count of array w and h
 * @param[in] w Width array
 * @param[in] h Height array
 * @param[out] cnt Count of array w and h
 * @param[out] w Width array
 * @param[out] h Height array
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE If succeed to get supported size list
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_supported_size_types()
 */
extern int widget_service_get_supported_sizes(const char *widgetid, int *cnt, int *w, int *h);

/**
 * @internal
 * @brief Gets the supported size list of given pkgid.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @param[in] cnt Size of types array
 * @param[out] cnt Result count of types array
 * @param[out] types Array of types
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully done
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_supported_sizes()
 */
extern int widget_service_get_supported_size_types(const char *widgetid, int *cnt, int *types);

/**
 * @internal
 * @brief Gets the category list of given cluster.
 * @since_tizen 2.4
 * @param[in] cluster Cluster name
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully done
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_enumerate_cluster_list()
 */
extern int widget_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/**
 * @internal
 * @brief Gets the cluster list.
 * @since_tizen 2.4
 * @param[in] cb Callback function for retrieving the cluster list
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @retval count Count of category items
 * @see widget_service_enumerate_category_list()
 */
extern int widget_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);

/**
 * @internal
 * @brief Initializes the widget service API.
 * @details Open the DB file.\n
 *    You don't need to call this if you are using widget service API shortly.\n
 *    But if you are using widget service API while whole life of your S/W,
 *    using this, initate the widget service is more benefical to you.\n
 *    This API will prepare the DB operation, if you don't initiate the widget service,
 *    every API which are related with DB operation will open DB and close it before return from it.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE Succeed to initialize
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to access a DB
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_fini()
 */
extern int widget_service_init(void);

/**
 * @internal
 * @brief Finalizes the widget service API.
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE Succeed to finalize
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR Failed to close the DB (access failed to DB)
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission denied
 * @pre widget_service_init.
 * @see widget_service_init()
 */
extern int widget_service_fini(void);

/**
 * @internal
 * @brief Creates a handle for getting the package list.
 * @details
 *    If you want get the record one by one from DB, use this.\n
 *    This function will create a iterator.\n
 *    Then you can get the records one by one, but there is no backward iterator.\n
 *    You can only get a forward iterator.\n
 *    After calling this function the iterator will be moved to the next record automatically.
 * @since_tizen 2.4
 * @remarks
 *    If you call this function again using created pkglist handle, it will be reset.
 *    So you can get records from the first one again.
 * @param[in] widgetid widget AppId
 * @param[in] handle @c NULL if you call this first, or it will be reset
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return handle
 * @retval @c NULL If it fails, widget_last_status() will returns reason of failure if it fails.
 * @retval handle If it successfully create the package list iterator
 * @see widget_service_pkglist_destroy()
 */
extern widget_pkglist_h widget_service_pkglist_create(const char *widgetid, widget_pkglist_h handle);

/**
 * @internal
 * @brief Gets the widgetid & package name & is_prime flag.
 * @since_tizen 2.4
 * @param[in] handle Handle which is created by widget_service_pkglist_create() function
 * @param[out] widgetid  widget Id
 * @param[out] pkgname Package Id which includes widgetes
 * @param[out] is_prime If the returned widgetid is primary, this will be 1 or 0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully get the record
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_STATUS_ERROR_NOT_EXIST Reach to the end of result set. you can rewind the iterator call widget_service_pkglist_create() again with current handle
 * @retval #WIDGET_STATUS_ERROR_MEMORY Not enough memory
 * @post You must release the widgetid, pkgname manually.
 * @see widget_service_pkglist_create()
 * @see widget_service_pkglist_destroy()
 */
extern int widget_service_get_pkglist_item(widget_pkglist_h handle, char **widgetid, char **pkgname, int *is_prime);

/**
 * @internal
 * @brief Destroys the iterator of pkglist.
 * @since_tizen 2.4
 * @param[in] handle Package list handle
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return int type
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid handle
 * @retval #WIDGET_STATUS_ERROR_NONE Successfully destroyed
 * @pre Handle must be created by widget_service_pkglist_create().
 * @post You have not to use the handle again after destroy it.
 * @see widget_service_pkglist_create()
 */
extern int widget_service_pkglist_destroy(widget_pkglist_h handle);

/**
 * @internal
 * @brief Getting the activated instance count.
 * @since_tizen 2.4
 * @param[in] widgetid widget Id, if you want to get whole instnaces list, use NULL.
 * @param[in] cluster Cluster name if you don't know what this is, use NULL.
 * @param[in] category Sub-cluster(category) name if you don't know what this is, use NULL.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return count of instances
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_STATUS_ERROR_FAULT Unrecorvarable error occurred
 * @retval count Positive value including ZERO, Count of activated instances on viewers
 */
extern int widget_service_get_instance_count(const char *widgetid, const char *cluster, const char *category);

/**
 * @internal
 * @brief Gets the path of the plug-in module.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval path String which is allocated on the heap
 * @retval @c NULL No libexec attribute, or widget_last_status() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_libexec(const char *widgetid);

/**
 * @internal
 * @brief Find the lbid using its libexec
 * @details only if the widget should use the "libexec" attribute in its "<widget>" tag
 * @since_tizen 2.4
 * @remarks Only usable for inhouse widgetes
 * @param[in] libexec so filename
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char *
 * @retval @c NULL if it fails to get pkgname, widget_last_status() will returns reason of failure.
 * @retval address heap address of pkgname
 * @post return'd string should be released by "free()"
 */
extern char *widget_service_widget_id_by_libexec(const char *libexec);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

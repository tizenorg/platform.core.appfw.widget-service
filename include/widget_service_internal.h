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

#ifndef WIDGET_SERVICE_INTERNAL_H_
#define WIDGET_SERVICE_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief Index of buffer
 * @since_tizen 2.3.1
 */
#define WIDGET_PRIMARY_BUFFER -1
#define WIDGET_UNKNOWN_BUFFER -2

/**
 * @internal
 * @brief Definition for maximum number of supported widget size type.
 * @since_tizen 2.3.1
 */
#define WIDGET_NR_OF_SIZE_LIST 13

#define WIDGET_CH_IDX(a, idx) (((char *)(a))[(idx)])
#define WIDGET_IS_INHOUSE(abi)    ((WIDGET_CH_IDX(abi, 0) == 'c' || WIDGET_CH_IDX(abi, 0) == 'C') && (WIDGET_CH_IDX(abi, 1) == '\0' || ((WIDGET_CH_IDX(abi, 1) == 'p' || WIDGET_CH_IDX(abi, 1) == 'P') && (WIDGET_CH_IDX(abi, 2) == 'p' || WIDGET_CH_IDX(abi, 2) == 'P') && WIDGET_CH_IDX(abi, 3) == '\0')))

#define WIDGET_EVENT_GBAR_UPDATED 2 /**< Contents of the given pd is updated */
#define WIDGET_EVENT_GBAR_EXTRA_UPDATED 3

#define WIDGET_EVENT_GROUP_CHANGED 6 /**< Group (Cluster/Sub-cluster) information is changed */
#define WIDGET_EVENT_PINUP_CHANGED 7 /**< PINUP status is changed */

#define WIDGET_EVENT_GBAR_SIZE_CHANGED 10 /**< Glance Bar size is changed */
#define WIDGET_EVENT_GBAR_CREATED 11 /**< If a Glance Bar is created even if you didn't call the widget_create_glance_bar API */
#define WIDGET_EVENT_GBAR_DESTROYED 12 /**< If a Glance Bar is destroyed even if you didn't call the widget_destroy_glance_bar API */
#define WIDGET_EVENT_HOLD_SCROLL 13 /**< If the screen should be freezed */
#define WIDGET_EVENT_RELEASE_SCROLL 14 /**< If the screen can be scrolled */
#define WIDGET_EVENT_WIDGET_UPDATE_BEGIN 15 /**< widget content update is started */
#define WIDGET_EVENT_WIDGET_UPDATE_END 16 /**< widget content update is finished */
#define WIDGET_EVENT_GBAR_UPDATE_BEGIN 17 /**< Glance Bar content update is started */
#define WIDGET_EVENT_GBAR_UPDATE_END 18 /**< Glance Bar content update is finished */
#define WIDGET_EVENT_UPDATE_MODE_CHANGED 19 /**< widget Update mode is changed */
#define WIDGET_EVENT_REQUEST_CLOSE_GBAR 20 /**< widget requests to close the Glance Bar */

#define WIDGET_EVENT_WIDGET_EXTRA_BUFFER_CREATED 22 /**< WIDGET Extra Buffer created event */
#define WIDGET_EVENT_GBAR_EXTRA_BUFFER_CREATED 23 /**< GBAR Extra Buffer created event */
#define WIDGET_EVENT_WIDGET_EXTRA_BUFFER_DESTROYED 24 /**< WIDGET Extra Buffer destroyed event */
#define WIDGET_EVENT_GBAR_EXTRA_BUFFER_DESTROYED 25 /**< WIDGET Extra Buffer destroyed event */

/**
 * @internal
 * @brief specify the source of input event
 * @since_tizen 2.3.1
 */
typedef enum input_event_source {
	INPUT_EVENT_SOURCE_VIEWER = 0x00,
	INPUT_EVENT_SOURCE_NODE = 0x01
} input_event_source_e;

/**
 * @internal
 * @brief widget Buffer Handler
 * @since_tizen 2.3.1
 */
typedef struct widget_buffer *widget_buffer_h;

/**
 * @internal
 * @brief Request type for closing Glance Bar
 * @since_tizen 2.3.1
 */
typedef enum widget_gbar_close_type {
	WIDGET_CLOSE_GBAR_NORMAL = 0x00,  /**< Glance Bar is closed normally */
	WIDGET_CLOSE_GBAR_TIMEOUT = 0x01, /**< Glance Bar is closed because of timeout, there is no response */
	WIDGET_CLOSE_GBAR_FAULT = 0x02,   /**< Glance Bar is closed because of unrecoverable error */
	WIDGET_CLOSE_GBAR_INVALID = 0xff, /**< Unknown event type */
} widget_gbar_close_type_e;

/**
 * @internal
 * @brief Delete type for widget delete operation.
 * @since_tizen 2.3.1
 */
typedef enum widget_delete_type {
	WIDGET_DELETE_PERMANENTLY = 0x01, /**< The widget is removed from the homescreen temporary */
	WIDGET_DELETE_TEMPORARY = 0x02,   /**< The widget is removed from the homescreen by user permanently */
	WIDGET_DELETE_INVALID = 0xff,     /**< Unknown event type */
} widget_delete_type_e;

/**
 * @internal
 * @brief
 * @since_tizen 2.3.1
 */
typedef enum widget_pre_callback {
	WIDGET_PRE_CREATE_CALLBACK = 1,
	WIDGET_PRE_DESTROY_CALLBACK = 2,
	WIDGET_PRE_RESIZE_CALLBACK = 3,
	WIDGET_PRE_ORIENTATION_CALLBACK = 4,
	WIDGET_PRE_CALLBACK_COUNT = 5
} widget_pre_callback_e;

typedef int (*widget_pre_callback_t)(const char *id, void *data);

/**
 * @internal
 * @brief Enumeration for result of accessibility event processing.
 * @details Reference the libprovider & libwidget-viewer.
 * @since_tizen 2.3.1
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
 * @since_tizen 2.3.1
 */
typedef enum widget_key_status {
	WIDGET_KEY_STATUS_ERROR = 0x80000000, /**< Key operation is failed */
	WIDGET_KEY_STATUS_DONE = 0x00000000,  /**< Key operation is successfully done */
	WIDGET_KEY_STATUS_FIRST,              /**< Focusable object item reaches to the first in it */
	WIDGET_KEY_STATUS_LAST,               /**< Focusable object item reaches to the last in it */
} widget_key_status_e;

/**
 * @internal
 * @brief Type of widget content sharing method
 * @since_tizen 2.3.1
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
 * @since_tizen 2.3.1
 */
typedef enum widget_gbar_type {
	GBAR_TYPE_NONE = 0x0, /**< Undefined */
	GBAR_TYPE_SCRIPT,     /**< Script base */
	GBAR_TYPE_TEXT,       /**< Text base */
	GBAR_TYPE_BUFFER,     /**< Buffer base */
	GBAR_TYPE_UIFW        /**< UIFW supported type */
} widget_gbar_type_e;

/**
 * @internal
 * @brief Destroy type of widget instance
 * @since_tizen 2.3.1
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
 * @internal
 * @brief This enumeration values should be sync'd with libwidget interface. (only for inhouse widget)
 * @since_tizen 2.3.1
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
	WIDGET_BUFFER_EVENT_ACCESS_DISABLE, /**< disable highlight and read ability */

	WIDGET_BUFFER_EVENT_FRAME_SKIP_CLEARED /**< Frame skip is cleared */
} widget_buffer_event_e;

/**
 * @internal
 * @brief widget Buffer Event Data
 * @since_tizen 2.3.1
 */
typedef struct widget_buffer_event_data {
	widget_buffer_event_e type; /**< Event type */
	double timestamp; /**< Timestamp */

	union input_data {
		struct mouse {
			int x; /**< Touch X coordinate */
			int y; /**< Touch Y coordinate */
			input_event_source_e source; /**< Where comes this event from */
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
 * @brief Package list handle.
 * @since_tizen 2.3.1
 */
typedef struct widget_list_handle *widget_list_h;

/* widget_status_e will be replaced with widget_error_e */
typedef enum widget_status  {
    WIDGET_STATUS_ERROR_NONE = TIZEN_ERROR_NONE,
    WIDGET_STATUS_ERROR = TIZEN_ERROR_WIDGET,
    WIDGET_STATUS_ERROR_INVALID_PARAMETER = WIDGET_STATUS_ERROR | 0x0001,
    WIDGET_STATUS_ERROR_FAULT = WIDGET_STATUS_ERROR | 0x0002,
    WIDGET_STATUS_ERROR_OUT_OF_MEMORY = WIDGET_STATUS_ERROR | 0x0004,
    WIDGET_STATUS_ERROR_EXIST = WIDGET_STATUS_ERROR | 0x0008,
    WIDGET_STATUS_ERROR_BUSY = WIDGET_STATUS_ERROR | 0x0010,
    WIDGET_STATUS_ERROR_PERMISSION_DENIED = WIDGET_STATUS_ERROR | 0x0020,
    WIDGET_STATUS_ERROR_ALREADY = WIDGET_STATUS_ERROR | 0x0040,
    WIDGET_STATUS_ERROR_CANCEL = WIDGET_STATUS_ERROR | 0x0080,
    WIDGET_STATUS_ERROR_IO_ERROR = WIDGET_STATUS_ERROR | 0x0100,
    WIDGET_STATUS_ERROR_NOT_EXIST = WIDGET_STATUS_ERROR | 0x0200,
    WIDGET_STATUS_ERROR_TIMEOUT = WIDGET_STATUS_ERROR | 0x0400,
    WIDGET_STATUS_ERROR_NOT_IMPLEMENTED = WIDGET_STATUS_ERROR | 0x0800,
    WIDGET_STATUS_ERROR_NO_SPACE = WIDGET_STATUS_ERROR | 0x1000,
    WIDGET_STATUS_ERROR_DISABLED = WIDGET_STATUS_ERROR | 0x2000
} widget_status_e;

/**
 * @brief Text signal & Content event uses this data structure.
 * @since_tizen 2.3.1
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
} *widget_event_info_s;

/**
 * @brief Names of text signals
 * @since_tizen 2.3.1
 * @see #widget_text_signal_s
 */
#define WIDGET_TEXT_SIGNAL_NAME_EDIT_MODE_ON    "edit,on"   /**< Text signal for edit mode on*/
#define WIDGET_TEXT_SIGNAL_NAME_EDIT_MODE_OFF   "edit,off"  /**< Text signal for edit mode off*/

/**
 * @brief Text signal information
 * @since_tizen 2.3.1
 * @see #WIDGET_TEXT_SIGNAL_NAME_EDIT_MODE_ON
 * @see #WIDGET_TEXT_SIGNAL_NAME_EDIT_MODE_OFF
 * @see #widget_event_info_s
 */
typedef struct widget_text_signal {
    const char *signal_name;    /**< A name of a text signal */
    const char *source;         /**< A source name of this text signal */
    struct {
        double sx;              /**< X-axis value of left-top corner for this text signal */
        double sy;              /**< Y-axis value of left-top corner for this text signal */
        double ex;              /**< X-axis value of right-bottom corner for this text signal */
        double ey;              /**< Y-axis value of right-bottom corner for this text signal  */
    } geometry;                 /**< Region information of this text signal */
} *widget_text_signal_s;

/**
 * @internal
 * @brief Gets the name of the provider (provider name == widget appid), you have to release the return value after use it.
 * @details
 *    widget has provider process for each widget instances.\n
 *    To get the provider's package name, you can use this API.\n
 *    If the given widgetid is inhouse widget, the return string will be the same with given argument but it is allocated in the heap.
 * @since_tizen 2.3.1
 * * @return char * type
 * @param[in] widgetid widget Id
 * @retval @c NULL Failed to get provider name, get_last_result() will returns reason of failure if it fails.
 * @retval widgetid widget AppId which is allocated on the heap
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_get_provider_name(const char *widgetid);

/**
 * @internal
 * @brief Gets synchronous uiapp list.
 * @details
 *        Callback (appid, data)\n
 *        This function will retrieve all UI Apps in a package which has given widget appid(widgetid).\n
 *        If you need to get all ui-app list, using a widget appid, this function is able to help you.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @param[in] cb Callback function
 * @param[in] data Callback Data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE Status success
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_MEMORY Memory error
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_FAULT Unrecoverable error occurred
 */
extern int widget_service_get_applist(const char *widgetid, void (*cb)(const char *widgetid, const char *appid, void *data), void *data);

/**
 * @internal
 * @brief Checks the primary flag of given widget Id.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return the primary flag of given widget id.
 * @retval 0 If is not a primary, get_last_result() will returns reason of failure if it fails.
 * @retval 1 If it is a primary widget
 */
extern int widget_service_is_primary(const char *widgetid);

/**
 * @internal
 * @brief Gets the default content string of the given widget.
 * @details
 *    If the user defines the default content string in the manifest file (.xml),\n
 *    this API will return it.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval content Content string
 * @retval @c NULL There is no specified content string, or get_last_result() will returns reason of failure if it fails.
 * @pre Manifest has the default content string. &lt;content&gt;Default content string&lt;content&gt; tag.
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_get_content_string(const char *widgetid);

/**
 * @internal
 * @brief Gets the "ABI" of given package.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval abi String which is allocated in the heap
 * @retval @c NULL Failed to get ABI of given widget, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_get_abi(const char *widgetid);

/**
 * @internal
 * @brief Checks the status of the widget.
 * @details Currently this API is not implemented. It just returns 1 all the time.
 * @since_tizen 2.3.1
 * @remarks This API is not implemented. It will always return 1.
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return the status of the widget.
 * @retval 1 Enabled
 * @retval 0 Disabled
 */
extern int widget_service_is_enabled(const char *widgetid);

/**
 * @internal
 * @brief Gets the script file of widget.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL Not specified script file, get_last_result() will returns reason of failure if it fails.
 * @pre widget should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see widget_service_widget_script_group()
 */
extern char *widget_service_get_widget_script_path(const char *widgetid);

/**
 * @internal
 * @brief Gets the script group of widget.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval Group Name of widget
 * @retval @c NULL If there is no group defined, or get_last_result() will returns reason of failure if it fails.
 * @pre widget should be developed as script type.
 * @post Return'd string must be free'd manually.
 * @see widget_service_widget_script_path()
 */
extern char *widget_service_get_widget_script_group(const char *widgetid);

/**
 * @internal
 * @brief Gets the script file path of given widget package.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval string Script file path
 * @retval @c NULL No specified script file for Glance Bar layout, or get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_gbar_script_group()
 */
extern char *widget_service_get_gbar_script_path(const char *widgetid);

/**
 * @internal
 * @brief Gets the group name for script file to load it.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval string Script group name
 * @retval @c NULL No script path, or get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_gbar_script_path()
 */
extern char *widget_service_get_gbar_script_group(const char *widgetid);

/**
 * @internal
 * @brief Gets the category list of given cluster.
 * @since_tizen 2.3.1
 * @param[in] cluster Cluster name
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE Successfully done
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_enumerate_cluster_list()
 */
extern int widget_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/**
 * @internal
 * @brief Gets the cluster list.
 * @since_tizen 2.3.1
 * @param[in] cb Callback function for retrieving the cluster list
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
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
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE Succeed to initialize
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access a DB
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_fini()
 */
extern int widget_service_init(void);

/**
 * @internal
 * @brief Finalizes the widget service API.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE Succeed to finalize
 * @retval #WIDGET_ERROR_IO_ERROR Failed to close the DB (access failed to DB)
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @pre widget_service_init.
 * @see widget_service_init()
 */
extern int widget_service_fini(void);

/**
 * @internal
 * @brief Gets the activated instance count.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application, if you want to get whole instances list, use NULL.
 * @param[in] cluster Cluster name if you don't know what this is, use NULL.
 * @param[in] category Sub-cluster(category) name if you don't know what this is, use NULL.
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return count of instances
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_FAULT Unrecorvarable error occurred
 * @retval count Positive value including ZERO, Count of activated instances on viewers
 */
extern int widget_service_get_instance_count(const char *widgetid, const char *cluster, const char *category);

/**
 * @internal
 * @brief Gets the path of the plug-in module.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char * type
 * @retval path String which is allocated on the heap
 * @retval @c NULL No libexec attribute, or get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_get_libexec(const char *widgetid);

/**
 * @internal
 * @brief Find the lbid using its libexec
 * @details only if the widget should use the "libexec" attribute in its "<widget>" tag
 * @since_tizen 2.3.1
 * @remarks Only usable for inhouse widgetes
 * @param[in] libexec so filename
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char *
 * @retval @c NULL if it fails to get pkgname, get_last_result() will returns reason of failure.
 * @retval address heap address of pkgname
 * @post return'd string should be released by "free()"
 */
extern char *widget_service_get_widget_id_by_libexec(const char *libexec);

/**
 * @internal
 * @brief Gets the base file path of the current widget
 * @since_tizen 2.3.1
 * @param[out] base_file_path base file path
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid handle
*/
extern char *widget_service_get_base_file_path(const char *widget_id);

/**
 * @brief Get the category using given widgetid.
 * @since_tizen 2.3.1
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return char *
 * @retval @c NULL Failed to get primary widgetid, get_last_result() will returns reason of failure if it fails.
 * @retval category Category string which is allocated in the heap.
 * @pre Must be released returned string by manually
 * @post N/A
 * @see widget_service_widget_id()
 */
extern char *widget_service_get_category(const char *widgetid);

/**
 * @brief Gets synchronous package list.
 * @details
 *      callback (widgetid)\n
 *      widgetid == widget AppId\n
 *        If the callback returns negative value, the list crawling will be stopped
 * @since_tizen 2.3.1
 * @param[in] category Name of category
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return int count
 * @retval Count of widget packages
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see #widget_service_get_widget_list_by_pkgid
 */
extern int widget_service_get_widget_list_by_category(const char *category, int (*cb)(const char *widgetid, void *data), void *data);

/**
 * @brief Creates a handle for getting the package list.
 * @details
 *    If you want get the record one by one from DB, use this.\n
 *    This function will create a iterator.\n
 *    Then you can get the records one by one, but there is no backward iterator.\n
 *    You can only get a forward iterator.\n
 *    After calling this function the iterator will be moved to the next record automatically.
 * @since_tizen 2.3.1
 * @remarks
 *    If you call this function again using created pkglist handle, it will be reset.
 *    So you can get records from the first one again.
 * @param[in] widgetid appid of widget application
 * @param[in] handle @c NULL if you call this first, or it will be reset
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return handle
 * @retval @c NULL If it fails, get_last_result() will returns reason of failure if it fails.
 * @retval handle If it successfully create the package list iterator
 * @see widget_service_pkglist_destroy()
 */
extern widget_list_h widget_service_create_widget_list(const char *widgetid, widget_list_h handle);

/**
 * @brief Gets the widgetid & package name & is_prime flag.
 * @since_tizen 2.3.1
 * @param[in] handle Handle which is created by widget_service_pkglist_create() function
 * @param[out] widgetid  widget Id
 * @param[out] pkgname Package Id which includes widgetes
 * @param[out] is_prime If the returned widgetid is primary, this will be 1 or 0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE Successfully get the record
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_NOT_EXIST Reach to the end of result set. you can rewind the iterator call widget_service_pkglist_create() again with current handle
 * @retval #WIDGET_ERROR_MEMORY Not enough memory
 * @post You must release the widgetid, pkgname manually.
 * @see widget_service_pkglist_create()
 * @see widget_service_pkglist_destroy()
 */
extern int widget_service_get_item_from_widget_list(widget_list_h handle, char **widgetid, char **pkgname, int *is_prime);

/**
 * @brief Destroys the iterator of pkglist.
 * @since_tizen 2.3.1
 * @param[in] handle Package list handle
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid handle
 * @retval #WIDGET_ERROR_NONE Successfully destroyed
 * @pre Handle must be created by widget_service_pkglist_create().
 * @post You have not to use the handle again after destroy it.
 * @see widget_service_pkglist_create()
 */
extern int widget_service_destroy_widget_list(widget_list_h handle);

/**
 * @brief Get the maximum creatable count of widgets\n
 *        Widget devleoper should describe the maximum count of createable widget instances to its Package manifest file.
 * @since_tizen 2.3.1
 * @param[in] widget_id widget application id
 * @privlevel public
 * @privlege %http://tizen.org/privilege/widget.viewer
 * @return count positive value on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_ERROR_IO_ERROR Input/Output error (failed to access database)
 * @retval #WIDGET_ERROR_FAULT Unrecorvarable error occurred
 */
extern int widget_service_get_widget_max_count(const char *widget_id);

#ifdef __cplusplus
}
#endif

#endif /* WIDGET_SERVICE_INTERNAL_H_ */

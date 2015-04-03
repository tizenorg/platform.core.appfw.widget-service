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

#include <tizen_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file widget-service.h
 * @brief  This file declares API of libwidget-service library
 * @since_tizen 2.3.1
 */

/**
 * @addtogroup CAPI_WIDGET_SERVICE_MODULE
 * @{
 */

/**
 * @brief Enumeration for list of supporting widget size types.
 * @since_tizen 2.3.1
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
	WIDGET_SIZE_TYPE_FULL = 0x0800,      /**< 720x1280 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_UNKNOWN = 0xFFFF
} widget_size_type_e;

/**
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
 * @brief Enumeration for widget event type.
 * @details These events will be sent from the provider.
 * @since_tizen 2.3.1
 */
typedef enum widget_event_type {
    WIDGET_EVENT_WIDGET_UPDATED = 0,                      /**< Contents of the given widget is updated */
    WIDGET_EVENT_WIDGET_EXTRA_UPDATED = 1,
    WIDGET_EVENT_CREATED = 4,                            /**< A new widget is created */
    WIDGET_EVENT_DELETED = 5,                            /**< A widget is deleted */
    WIDGET_EVENT_PERIOD_CHANGED = 8,                     /**< Update period is changed */
    WIDGET_EVENT_WIDGET_SIZE_CHANGED = 9,                /**< widget size is changed */
    WIDGET_EVENT_EXTRA_INFO_UPDATED = 21,                /**< Extra information is updated */
    WIDGET_EVENT_IGNORED = 0xFF                   /**< Request is ignored */
} widget_event_type_e;

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
 *  #WIDGET_SIZE_TYPE_FULL\n
 *  #WIDGET_SIZE_TYPE_EASY_1x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x3.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] type Size type
 * @param[out] width Pixel size width
 * @param[out] height Pixel size height
 * @return #WIDGET_ERROR_NONE on success, 
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_NONE Successfully done
 * @see widget_size_type()
 * @see widget_service_get_size_type()
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
 *  #WIDGET_SIZE_TYPE_FULL\n
 *  #WIDGET_SIZE_TYPE_EASY_1x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x1\n
 *  #WIDGET_SIZE_TYPE_EASY_3x3\n
 *  or\n
 *  #WIDGET_SIZE_TYPE_UNKNOWN for error.
 * @since_tizen 2.3.1
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] width Pixel size width
 * @param[in] height Pixel size height
 * @param[out] size_type Widget size type\n
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter was given.
 * @see widget_size_type()
 * @see widget_service_get_size()
 */
extern int widget_service_get_size_type(int width, int height, widget_size_type_e *size_type);

/**
 * @brief Gets the need of mouse event for the given widget.
 * @details This function gets need of mouse event for the given widget from database.\n
 * 	The value of mouse event requirement means... \n
 * 		true : The widget requires mouse event. A viewer must has to feed the mouse event to the widget instance\n
 * 		false : The widget doesn't require mouse event.\n
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widget_id widget app id
 * @param[in] size_type widget size type
 * @param[out] need_of_mouse_event the need of mouse event
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter was given.
 * @retval #WIDGET_ERROR_IO_ERROR Some error occurs on opening DB file.
 * @see widget_size_type_e
 * @see widget_service_get_need_of_touch_effect()
 * @see widget_service_get_need_of_frame()
 */
extern int widget_service_get_need_of_mouse_event(const char *widget_id, widget_size_type_e size_type, bool *need_of_mouse_event);

/**
 * @brief Gets the need of touch effect for the given widget.
 * @details This API gets the need of touch effect for the given widget from database.\n
 *          The value of the need of touch effect means... \n
 *          true : A viewer is required to make the touch effect when a user click the widget, but it is just recommendation.\n
 *          false : A viewer is not required to make the touch effect, the box will make touch effect itself.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widget_id appid of widget provider
 * @param[in] size_type Size type
 * @param[out] need_of_touch_event the need of touch effect
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter was given.
 * @retval #WIDGET_ERROR_IO_ERROR Some error occurs on opening DB file.
 * @see widget_size_type_e
 * @see widget_service_get_need_of_mouse_event()
 * @see widget_service_get_need_of_frame()
  */
extern int widget_service_get_need_of_touch_effect(const char *widget_id, widget_size_type_e size_type, bool *need_of_touch_event);

/**
 * @brief Gets the need of decoration frame for the given widget.
 * @details This API gets the need of decoration frame for the given widget from database.\n
 *          The value of the need of touch effect means... \n
 *          true : the viewer should make decoration frame on outside of the widget.
 *          false : no need to amke decoration frame.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widget_id widget app id
 * @param[in] size_type Size type
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter was given.
 * @retval #WIDGET_ERROR_IO_ERROR Some error occurs on opening DB file.
 * @see widget_service_get_need_of_mouse_event()
 * @see widget_service_get_need_of_touch_effect()
 */
extern int widget_service_get_need_of_frame(const char *widget_id, widget_size_type_e size_type, bool *need_of_frame);

/**
 * @brief Triggers the update event for given widget instance.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] content New content information, Default @c NULL
 * @param[in] force 1 if you want to update your widget even if the provider is paused or 0. 0 is default
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.provider
 * @feature http://tizen.org/feature/shell.appwidget
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_CANCELED Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * @retval #WIDGET_ERROR_MEMORY Memory is not enough to make request
 * @retval #WIDGET_ERROR_FAULT Failed to create a request packet
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_NONE Successfully requested
 * @see widget_service_trigger_update()
 */
extern int widget_service_trigger_update(const char *widgetid, const char *instance_id, const char *content, int force);

/**
 * @brief Changes the update period of given widget instance.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @param[in] instance_id widget instance id
 * @param[in] period New update period in sec
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.provider
 * @feature http://tizen.org/feature/shell.appwidget
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE Successfully changed(requested)
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_FAULT Failed to create a request packet
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_MEMORY Not enough memory
 */
extern int widget_service_change_period(const char *widgetid, const char *instance_id, double period);


/**
 * @brief Callback function for getting result of widget_service_get_widget_list
 * @since_tizen 2.3.1
 * @param[in] pkgid package id
 * @param[in] widgetid widget app id
 * @param[in] is_prime will be sent as 1 if the widget is a default widget
 * @param[in] data user data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return WIDGET_ERROR_NONE to continue with the next iteration of the loop, other error values to break out of the loop
 * @see #widget_service_get_pkglist
 */
typedef int (*widget_list_cb)(const char *pkgid, const char *widgetid, int is_prime, void *data);

/**
 * @brief Gets a list of all widgets.
 * @since_tizen 2.3.1
 * @param[in] cb Callback function
 * @param[in] data user data for callback function
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval count Count of widget packages
 * @see #widget_service_get_widget_list_by_pkgid
 * @see #widget_service_get_widget_list_cb
 */
extern int widget_service_get_widget_list(widget_list_cb cb, void *data);

/**
 * @brief Gets the MAIN application Id of given widget package Id.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid widget Package Id
 * @return char * type
 * @retval @c NULL If it fails to get main application Id (UI-APPID), get_last_result() will returns reason of failure.
 * @retval appid Main application Id
 * @see #get_last_result
 */
extern char *widget_service_get_main_app_id(const char *widgetid);

/**
 * @brief Callback function for getting result of widget_service_get_widget_list_by_pkgid
 * @since_tizen 2.3.1
 * @param[in] widgetid widget app id
 * @param[in] is_prime will be sent as 1 if the widget is a default widget
 * @param[in] data user data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return WIDGET_ERROR_NONE to continue with the next iteration of the loop, other error values to break out of the loop
 * @see #widget_service_get_widget_list_by_pkgid
 */
typedef int (*widget_list_by_pkgid_cb)(const char *widgetid, int is_prime, void *data);

/**
 * @brief Gets a list of widgets included in the given package id
 * @since_tizen 2.3.1
 * @param[in] pkgid Package Id (Not the UI App Id)
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return #WIDGET_ERROR_NONE on success,
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval int Count of widget packages
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see #widget_service_get_widget_list
 * @see #widget_service_get_widget_list_by_pkgid_cb
 */
extern int widget_service_get_widget_list_by_pkgid(const char *pkgid, widget_list_by_pkgid_cb cb, void *data);

/**
 * @brief Gets the id of a widget by the given id of package or UI app.
 *        If there are two or more widgets in the package, this function returns the id of the primary widget.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] id Package Id or UI App Id
 * @return char * type
 * @retval @c NULL Failed to get primary widgetid, get_last_result() will returns reason of failure.
 * @retval widgetid Primary widget Id. which is allocated in the heap
 * @pre Must be released returned string by manually.
 * @see widget_service_package_id()
 */
extern char *widget_service_get_widget_id(const char *id);

/**
 * @brief Gets the appId of setup app which is specified by given widget Id's manifest.
 * @details
 *    This setup app should be launched before adding the widget to get the content_info.\n
 *    This function should be called before add a widget.\n
 *    To determine the content information string.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @param[in] widgetid appid of widget provider
 * @return char * type
 * @retval @c NULL There is no setup application or error occurred, you can check it using get_last_result()
 * @retval appid AppId if exists or @c NULL
 * @post Returned string must be free'd manually.
 * @see get_last_result()
 */
extern char *widget_service_get_app_id_of_setup_app(const char *widgetid);

/**
 * @brief Gets the package id of the given widget.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval appid String which is allocated in the heap
 * @retval @c NULL Invalid appid, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_widget_id()
 */
extern char *widget_service_get_package_id(const char *widgetid);

/**
 * @brief Gives the name of the given widget.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval name If it fails to get name
 * @retval @c NULL Allocated heap address, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd by manually.
 * @see widget_service_i18n_icon()
 * @see widget_service_preview()
 */
extern char *widget_service_get_name(const char *widgetid, const char *lang);

/**
 * @brief Gets the preview image path of given size type.
 * @details This function will return the preview image path.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @param[in] size_type widget size type
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval path Preview image path
 * @retval @c NULL There is no preview image file, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_i18n_icon()
 * @see widget_service_i18n_name()
 */
extern char *widget_service_get_preview_image_path(const char *widgetid, int size_type);

/**
 * @brief Gets icon path of given widget package.
 * @details The user should free the returned string with free().
 * @since_tizen 2.3.1
 * @param[in] pkgid package id of widget provider
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @retval name Allocated heap address
 * @retval @c NULL Fails to get path of an icon, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_i18n_name()
 * @see widget_service_preview()
 */
extern char *widget_service_get_icon(const char *pkgid, const char *lang);

/**
 * @brief Gets the "nodisplay" value.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return the "nodisplay" value of given widget.
 * @retval 1 The box should not be listed by the widget list app
 * @retval 0 Box should be listed, get_last_result() will returns reason of failure if it fails.
 * @pre widget tag includes "nodisplay" attribute.
 */
extern int widget_service_get_nodisplay(const char *widgetid);

/**
 * @brief Gets the supported size list.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget provider
 * @param[out] cnt Count of array w and h
 * @param[out] w Width array
 * @param[out] h Height array
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return #WIDGET_ERROR_NONE on success, 
 *          otherwise an error code (see #WIDGET_ERROR_XXX) on failure
 * @retval #WIDGET_ERROR_NONE If succeed to get supported size list
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_supported_size_types()
 */
extern int widget_service_get_supported_sizes(const char *widgetid, int *cnt, int **w, int **h);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

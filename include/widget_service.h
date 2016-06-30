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

#include <tizen.h>
#include <bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file widget_service.h
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
	WIDGET_SIZE_TYPE_FULL = 0x0800,     /**< 720x1280 based on 720x1280 resolution */
	WIDGET_SIZE_TYPE_UNKNOWN = 0xFFFF   /**< Error case */
} widget_size_type_e;

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
 * @param[in] type Size type
 * @param[out] width Pixel size width
 * @param[out] height Pixel size height
 * @return 0 on success, otherwise a negative error value
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
 * @param[in] width Pixel size width
 * @param[in] height Pixel size height
 * @param[out] size_type Widget size type\n
 * @return 0 on success, otherwise a negative error value
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
 * @param[in] widget_id appid of widget application
 * @param[in] size_type widget size type
 * @param[out] need_of_mouse_event the need of mouse event
 * @return 0 on success, otherwise a negative error value
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
 * @param[in] widget_id appid of widget application
 * @param[in] size_type Size type
 * @param[out] need_of_touch_event the need of touch effect
 * @return 0 on success, otherwise a negative error value
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
 * @param[in] widget_id appid of widget application
 * @param[in] size_type Size type
 * @param[out] need_of_frame the need of decoration frame
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid parameter was given.
 * @retval #WIDGET_ERROR_IO_ERROR Some error occurs on opening DB file.
 * @see widget_service_get_need_of_mouse_event()
 * @see widget_service_get_need_of_touch_effect()
 */
extern int widget_service_get_need_of_frame(const char *widget_id, widget_size_type_e size_type, bool *need_of_frame);

/**
 * @brief Triggers the update event for given widget instance.
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[in] instance_id Set @c NULL if you don't know what the Id is. Then every instance of given pkgname will be triggered its update event
 * @param[in] b bundle data will be passed to the widget application via widget_update handler, default is @c NULL
 * @param[in] force 1 if you want to update your widget even if the provider is paused or 0. 0 is default
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_CANCELED Provider is paused, so this update request is canceld.(ignored), if you want to make update forcely, use force=1
 * @retval #WIDGET_ERROR_OUT_OF_MEMORY Memory is not enough to make request
 * @retval #WIDGET_ERROR_FAULT Failed to create a request packet
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_NONE Successfully requested
 * @see widget_service_trigger_update()
 */
extern int widget_service_trigger_update(const char *widget_id, const char *instance_id, bundle *b, int force);

/**
 * @brief Changes the update period of given widget instance.
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[in] instance_id widget instance id
 * @param[in] period New update period in sec
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE Successfully changed(requested)
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_FAULT Failed to create a request packet
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_OUT_OF_MEMORY Not enough memory
 */
extern int widget_service_change_period(const char *widget_id, const char *instance_id, double period);


/**
 * @brief Callback function for getting result of widget_service_get_widget_list
 * @since_tizen 2.3.1
 * @param[in] pkgid package id
 * @param[in] widget_id appid of widget application
 * @param[in] is_prime will be sent as 1 if the widget is a default widget
 * @param[in] data user data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return WIDGET_ERROR_NONE to continue with the next iteration of the loop, other error values to break out of the loop
 * @see #widget_service_get_widget_list
 */
typedef int (*widget_list_cb)(const char *pkgid, const char *widget_id, int is_prime, void *data);

/**
 * @brief Gets a list of all widgets.
 * @since_tizen 2.3.1
 * @param[in] cb Callback function
 * @param[in] data user data for callback function
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval count Count of widget packages
 * @see #widget_service_get_widget_list_by_pkgid
 * @see #widget_list_cb
 */
extern int widget_service_get_widget_list(widget_list_cb cb, void *data);

/**
 * @brief Gets the MAIN application Id of given widget package Id.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @remarks Result string should be freed by free()
 * @param[in] widget_id appid of widget application
 * @return char * type
 * @retval @c NULL If it fails to get main application Id (UI-APPID), get_last_result() will returns reason of failure.
 * @retval appid Main application Id
 * @see #get_last_result
 */
extern char *widget_service_get_main_app_id(const char *widget_id);

/**
 * @brief Callback function for getting result of widget_service_get_widget_list_by_pkgid
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[in] is_prime will be sent as 1 if the widget is a default widget
 * @param[in] data user data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return WIDGET_ERROR_NONE to continue with the next iteration of the loop, other error values to break out of the loop
 * @see #widget_service_get_widget_list_by_pkgid
 */
typedef int (*widget_list_by_pkgid_cb)(const char *widget_id, int is_prime, void *data);

/**
 * @brief Gets a list of widgets included in the given package id
 * @since_tizen 2.3.1
 * @param[in] pkgid Package Id (Not the UI App Id)
 * @param[in] cb Callback function
 * @param[in] data Callback data
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval int Count of widget packages
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see #widget_service_get_widget_list
 * @see #widget_list_by_pkgid_cb
 */
extern int widget_service_get_widget_list_by_pkgid(const char *pkgid, widget_list_by_pkgid_cb cb, void *data);

/**
 * @brief Gets the id of a widget by the given id of package or UI app.
 *        If there are two or more widgets in the package, this function returns the id of the primary widget.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @remarks Result string should be freed by free()
 * @param[in] id Package Id or UI App Id
 * @return char * type
 * @retval @c NULL Failed to get primary widget id, get_last_result() will returns reason of failure.
 * @retval widget_id Primary widget Id.
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
 * @remarks Result string should be freed by free()
 * @param[in] widget_id appid of widget application
 * @return char * type
 * @retval @c NULL There is no setup application or error occurred, you can check it using get_last_result()
 * @retval appid AppId if exists or @c NULL
 * @post Returned string must be free'd manually.
 * @see get_last_result()
 */
extern char *widget_service_get_app_id_of_setup_app(const char *widget_id);

/**
 * @brief Gets the package id of the given widget.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @remarks Result string should be freed by free()
 * @param[in] widget_id appid of widget application
 * @return char * type
 * @retval appid String which is allocated in the heap
 * @retval @c NULL Invalid appid, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see widget_service_widget_id()
 */
extern char *widget_service_get_package_id(const char *widget_id);

/**
 * @brief Gets the name of the given widget.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @remarks Result string should be freed by free()
 * @param[in] widget_id appid of widget application
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @return char * type
 * @retval name If it fails to get name
 * @retval @c NULL Allocated heap address, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd by manually.
 * @see widget_service_i18n_icon()
 * @see widget_service_preview()
 */
extern char *widget_service_get_name(const char *widget_id, const char *lang);

/**
 * @brief Gets the preview image path of given size type.
 * @details This function will return the preview image path.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @remarks Result string should be freed by free()
 * @param[in] widget_id appid of widget application
 * @param[in] size_type widget size type
 * @return char * type
 * @retval path Preview image path
 * @retval @c NULL There is no preview image file, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see #widget_service_get_icon
 * @see #widget_service_get_name
 */
extern char *widget_service_get_preview_image_path(const char *widget_id, widget_size_type_e size_type);

/**
 * @brief Gets icon path of given widget package.
 * @details The user should free the returned string with free().
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @remarks Result string should be freed by free()
 * @param[in] pkgid package id of widget application
 * @param[in] lang Locale(en-us, ko-kr, ...), if it is @c NULL, function will use the system locale automatically
 * @return char * type
 * @retval name Allocated heap address
 * @retval @c NULL Fails to get path of an icon, get_last_result() will returns reason of failure if it fails.
 * @post Returned string must be free'd manually.
 * @see #widget_service_get_name
 * @see #widget_service_get_preview_image_path
 */
extern char *widget_service_get_icon(const char *pkgid, const char *lang);

/**
 * @brief Gets the "nodisplay" value
 * @since_tizen 2.3.1
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @param[in] widget_id Appid of widget application
 * @privlevel public
 * @return The "nodisplay" value of given widget
 * @retval 1 The box should not be listed by the widget list app
 * @retval 0 Box should be listed, get_last_result() will returns reason of failure if it fails
 * @pre Widget tag includes "nodisplay" attribute
 */
extern int widget_service_get_nodisplay(const char *widget_id);

/**
 * @brief Gets the supported size list.
 * @since_tizen 2.3.1
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @param[in] widget_id appid of widget application
 * @param[out] cnt Count of array w and h
 * @param[out] w Width array
 * @param[out] h Height array
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NONE If succeed to get supported size list
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_supported_size_types()
 */
extern int widget_service_get_supported_sizes(const char *widget_id, int *cnt, int **w, int **h);

/**
 * @brief Gets the supported size list of given widget id.
 * @since_tizen 2.3.1
 * @param[in] widgetid appid of widget application
 * @param[in] cnt Size of types array
 * @param[out] cnt Result count of types array
 * @param[out] types Array of types
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_NONE Successfully done
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see widget_service_get_supported_sizes()
 */
extern int widget_service_get_supported_size_types(const char *widgetid, int *cnt, int **types);

/**
 * @brief Callback function for getting result of widget_service_get_widget_instance_list
 * @since_tizen 2.3.1
 * @param[in] widget_id widget app id
 * @param[in] widget_instance_id widget instance id
 * @param[in] data user data
 * @return WIDGET_ERROR_NONE to continue with the next iteration of the loop, other error values to break out of the loop
 * @see #widget_service_get_widget_instance_list
 */
typedef int (*widget_instance_list_cb)(const char *widget_id, const char *instance_id, void *data);

/**
 * @brief Gets widget instances of given widget_id.
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[in] cb Callback function
 * @param[in] data user data for callback function
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_NOT_SUPPORTED Not supported
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_NOT_EXIST Event handler callback function is not exist
 * @see #widget_instance_list_cb
 */
extern int widget_service_get_widget_instance_list(const char *widget_id, widget_instance_list_cb cb, void *data);

/**
 * @brief Enumerations for life cycle event of widgets
 * @since_tizen 2.3.1
 * @see widget_lifecycle_event_cb
 */
typedef enum widget_lifecycle_event {
	WIDGET_LIFE_CYCLE_EVENT_CREATE  = 1,    /**< The widget is created */
	WIDGET_LIFE_CYCLE_EVENT_DESTROY = 2,    /**< The widget is destroyed */
	WIDGET_LIFE_CYCLE_EVENT_PAUSE   = 3,    /**< The widget is paused */
	WIDGET_LIFE_CYCLE_EVENT_RESUME  = 4,    /**< The widget is resumed */
	WIDGET_LIFE_CYCLE_EVENT_MAX     = 5
} widget_lifecycle_event_e;

/**
 * @brief Called when a widget is created or destroyed
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[in] lifecycle_event type of event
 * @param[in] widget_instance_id widget instance id
 * @param[in] data user data
 * @return 0 on success, otherwise a negative error value
 * @see #widget_service_set_lifecycle_event_cb
 */
typedef int (*widget_lifecycle_event_cb)(const char *widget_id, widget_lifecycle_event_e lifecycle_event, const char *widget_instance_id, void *data);

/**
 * @brief Registers event handler callback function for life-cycle events of widgets
 * @since_tizen 2.3.1
 * @privlevel public
 * @param[in] widget_id appid of widget application
 * @param[in] cb Callback function
 * @param[in] data user data
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @see #widget_lifecycle_event_cb
 * @see #widget_service_unset_lifecycle_event_cb
 */
extern int widget_service_set_lifecycle_event_cb(const char *widget_id, widget_lifecycle_event_cb cb, void *data);

/**
 * @brief Unregisters event handler callback function for life-cycle events of widgets
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[out] user_data user callback data
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @retval #WIDGET_ERROR_PERMISSION_DENIED Permission denied
 * @retval #WIDGET_ERROR_NOT_EXIST Event handler callback function is not exist
 * @see #widget_service_set_lifecycle_event_cb
 */
extern int widget_service_unset_lifecycle_event_cb(const char *widget_id, void **user_data);

/**
 * @brief Gets content of the widget instance
 * @since_tizen 2.3.1
 * @param[in] widget_id appid of widget application
 * @param[in] widget_instance_id widget instance id
 * @param[out] b bundle(content) data of the given widget instance, it should be released by caller.
 * @return 0 on success, otherwise a negative error value
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access DB
 * @retval #WIDGET_ERROR_INVALID_PARAMETER Invalid argument
 * @post caller should have to release the bundle
 */
extern int widget_service_get_content_of_widget_instance(const char *widget_id, const char *widget_instance_id, bundle **b);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

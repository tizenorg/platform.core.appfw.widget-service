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
 * @since_tizen 2.4
 */
#define WIDGET_PRIMARY_BUFFER -1
#define WIDGET_UNKNOWN_BUFFER -2

/**
 * @internal
 * @brief Definition for maximum number of supported widget size type.
 * @since_tizen 2.4
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
 * @brief Delete type for widget delete operation.
 * @since_tizen 2.4
 */
typedef enum widget_delete_type {
	WIDGET_DELETE_PERMANENTLY = 0x01, /**< The widget is removed from the homescreen temporary */
	WIDGET_DELETE_TEMPORARY = 0x02,   /**< The widget is removed from the homescreen by user permanently */
	WIDGET_DELETE_INVALID = 0xff,     /**< Unknown event type */
} widget_delete_type_e;

/**
 * @internal
 * @brief Get the category using given widgetid.
 * @since_tizen 2.4
 * @param[in] widgetid widget AppId
 * @privlevel public
 * @privilege %http://tizen.org/privilege/widget.viewer
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char *
 * @retval @c NULL Failed to get primary widgetid, get_last_result() will returns reason of failure if it fails.
 * @retval category Category string which is allocated in the heap.
 * @pre Must be released returned string by manually
 * @post N/A
 * @see widget_service_widget_id()
 */
extern char *widget_service_get_category(const char *widgetid);

/**
 * @internal
 * @brief Gets the name of the provider (provider name == widget appid), you have to release the return value after use it.
 * @details
 *    widget has provider process for each widget instances.\n
 *    To get the provider's package name, you can use this API.\n
 *    If the given widgetid is inhouse widget, the return string will be the same with given argument but it is allocated in the heap.
 * @since_tizen 2.4
 * @privlevel N/P
 * @feature http://tizen.org/feature/shell.appwidget
 * @return char * type
 * @param[in] widgetid widget Id
 * @retval @c NULL Failed to get provider name, get_last_result() will returns reason of failure if it fails.
 * @retval widgetid widget AppId which is allocated on the heap
 * @post Returned string must be free'd manually.
 */
extern char *widget_service_get_provider_name(const char *widgetid);

#ifdef __cplusplus
}
#endif

#endif /* WIDGET_SERVICE_INTERNAL_H_ */

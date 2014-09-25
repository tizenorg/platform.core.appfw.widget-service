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

#ifndef __DYNAMICBOX_BUFFER_H
#define __DYNAMICBOX_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file dynamicbox_buffer.h
 * @brief This file describe the buffer ADT and event definitions
 */

/**
 * @addtogroup CAPI_DYNAMICBOX_SERVICE_MODULE
 * @{
 */

enum dynamicbox_fb_type { /*!< Must have to be sync with libprovider, liblivebox-viewer */
	DBOX_FB_TYPE_FILE,
	DBOX_FB_TYPE_SHM,
	DBOX_FB_TYPE_PIXMAP,
	DBOX_FB_TYPE_ERROR
};

/**
 * @internal
 * @brief This enumeration values should be sync'd with libdynamicbox interface. (only for inhouse dynamicbox)
 * @since_tizen 2.3
 */
enum dynamicbox_buffer_event {
	DBOX_BUFFER_EVENT_ENTER, /**< get the focus */
	DBOX_BUFFER_EVENT_LEAVE, /**< lost the focus */
	DBOX_BUFFER_EVENT_DOWN, /**< Touch down */
	DBOX_BUFFER_EVENT_MOVE, /**< Touch move */
	DBOX_BUFFER_EVENT_UP, /**< Touch up */

	DBOX_BUFFER_EVENT_KEY_DOWN, /**< Key pressed */
	DBOX_BUFFER_EVENT_KEY_UP, /**< Key release */
	DBOX_BUFFER_EVENT_KEY_FOCUS_IN, /**< Focus in */
	DBOX_BUFFER_EVENT_KEY_FOCUS_OUT, /**< Focus out */

	DBOX_BUFFER_EVENT_ACCESS_HIGHLIGHT, /**< Accessibility Highlight event */
	DBOX_BUFFER_EVENT_ACCESS_HIGHLIGHT_NEXT, /**< Accessibility Highlight Next event */
	DBOX_BUFFER_EVENT_ACCESS_HIGHLIGHT_PREV, /**< Accessibility Highlight Prev event */
	DBOX_BUFFER_EVENT_ACCESS_ACTIVATE, /**< Accessibility Activate event */
	DBOX_BUFFER_EVENT_ACCESS_ACTION_UP, /**< Accessibility Action Up event */
	DBOX_BUFFER_EVENT_ACCESS_ACTION_DOWN, /**< Accessibility Action Down event */
	DBOX_BUFFER_EVENT_ACCESS_SCROLL_UP, /**< Accessibility Scroll Mouse Up event */
	DBOX_BUFFER_EVENT_ACCESS_SCROLL_MOVE, /**< Accessibility Scroll Mouse Move event */
	DBOX_BUFFER_EVENT_ACCESS_SCROLL_DOWN, /**< Accessibility Scroll Mouse Down event */
	DBOX_BUFFER_EVENT_ACCESS_UNHIGHLIGHT, /**< Accessibility Unhighlight event */

	DBOX_BUFFER_EVENT_ON_HOLD,	/**< To prevent from generating mouse clicked event */
	DBOX_BUFFER_EVENT_OFF_HOLD, /**< Disable the mouse hold event */
	DBOX_BUFFER_EVENT_ON_SCROLL, /**< Enable the scroll flag */
	DBOX_BUFFER_EVENT_OFF_SCROLL, /**< Disable the scroll flag */

	DBOX_BUFFER_EVENT_ACCESS_VALUE_CHANGE, /**< */
	DBOX_BUFFER_EVENT_ACCESS_MOUSE, /**< give mouse event to highlight object */
	DBOX_BUFFER_EVENT_ACCESS_BACK, /**< go back to a previous view ex: pop naviframe item */
	DBOX_BUFFER_EVENT_ACCESS_OVER, /**< mouse over an object */
	DBOX_BUFFER_EVENT_ACCESS_READ, /**< highlight an object */
	DBOX_BUFFER_EVENT_ACCESS_ENABLE, /**< enable highlight and read ability */
	DBOX_BUFFER_EVENT_ACCESS_DISABLE /**< disable highlight and read ability */
};

/**
 * @internal
 * @brief Dynamic Box Buffer Event Data
 * @since_tizen 2.3
 */
typedef struct dynamicbox_buffer_event_data {
	enum dynamicbox_buffer_event type; /**< Event type */
	double timestamp; /**< Timestamp */

	union _input_data {
		struct _mouse {
			int x; /**< Touch X coordinate */
			int y; /**< Touch Y coordinate */
		} pointer;

		struct _access {
			int x; /**< Accessibility event X coordinate */
			int y; /**< Accessibility event Y coordinate */
			unsigned int mouse_type; /**< 0: down, 1: move, 2: up | 0: cur, 1: next, 2: prev, 3: off */
			unsigned int action_type; /**< reserved for protocol */
			unsigned int action_by; /**< reserved for protocol */
			int cycle; /**< reserved for protocol */
		} access;

		unsigned int keycode; /**< Key code value */
	} info;
} *dynamicbox_buffer_event_data_t;


typedef struct dynamicbox_fb { /*!< Must has to be sync with slave & provider */
	enum {
		DBOX_FB_STATE_CREATED = 0x00beef00,
		DBOX_FB_STATE_DESTROYED = 0x00dead00
	} state;
	enum dynamicbox_fb_type type;
	int refcnt;
	void *info;
	char data[];
} *dynamicbox_fb_t;

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

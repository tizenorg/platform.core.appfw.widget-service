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

#ifndef __WIDGET_SCRIPT_H
#define __WIDGET_SCRIPT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file widget_buffer.h
 * @brief This file describe the buffer ADT and event definitions
 */

/**
 * @addtogroup CAPI_WIDGET_SERVICE_MODULE
 * @{
 */

/**
 * @internal
 * @brief Enumeration for script event of widget.
 * @details
 * This enumeration values will be used for script plugin of data-provider-master.
 * Master will send down these accessibility events to the script plugin.
 * @since_tizen 2.3.1
 */
typedef enum widget_script_event {
    WIDGET_SCRIPT_ACCESS_EVENT          = 0x01000000, /**< Mask value */
    WIDGET_SCRIPT_MOUSE_EVENT           = 0x02000000, /**< Mask value */
    WIDGET_SCRIPT_KEY_EVENT             = 0x04000000, /**< Mask value */

    WIDGET_SCRIPT_ACCESS_HIGHLIGHT      = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000001, /**< Highlight */
    WIDGET_SCRIPT_ACCESS_HIGHLIGHT_NEXT = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000002, /**< Move Highlight focus to the next object */
    WIDGET_SCRIPT_ACCESS_HIGHLIGHT_PREV = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000004, /**< Move Highlight focus to the prev object */
    WIDGET_SCRIPT_ACCESS_ACTIVATE       = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000008, /**< Activate the highlighted object */
    WIDGET_SCRIPT_ACCESS_ACTION         = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000010, /**< Do specified action for the highlighted object */
    WIDGET_SCRIPT_ACCESS_SCROLL         = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000020, /**< Scroll operation */
    WIDGET_SCRIPT_ACCESS_UNHIGHLIGHT    = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000040, /**< Remove highlight */
    WIDGET_SCRIPT_ACCESS_VALUE_CHANGE   = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000080, /* TODO: deprecate this */
    WIDGET_SCRIPT_ACCESS_MOUSE          = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000100, /* give mouse event to highlight object */
    WIDGET_SCRIPT_ACCESS_BACK           = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000200, /* go back to a previous view ex: pop naviframe item */
    WIDGET_SCRIPT_ACCESS_OVER           = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000400, /* mouse over an object */
    WIDGET_SCRIPT_ACCESS_READ           = WIDGET_SCRIPT_ACCESS_EVENT | 0x00000800, /* highlight an object */
    WIDGET_SCRIPT_ACCESS_ENABLE         = WIDGET_SCRIPT_ACCESS_EVENT | 0x00001000, /* enable highlight and read ability */
    WIDGET_SCRIPT_ACCESS_DISABLE        = WIDGET_SCRIPT_ACCESS_EVENT | 0x00002000, /* disable highlight and read ability */
    
    WIDGET_SCRIPT_MOUSE_DOWN            = WIDGET_SCRIPT_MOUSE_EVENT | 0x00001000, /**< Mouse down */
    WIDGET_SCRIPT_MOUSE_MOVE            = WIDGET_SCRIPT_MOUSE_EVENT | 0x00002000, /**< Mouse move */
    WIDGET_SCRIPT_MOUSE_UP              = WIDGET_SCRIPT_MOUSE_EVENT | 0x00004000, /**< Mouse up */
    WIDGET_SCRIPT_MOUSE_IN              = WIDGET_SCRIPT_MOUSE_EVENT | 0x00008000, /**< Mouse in */
    WIDGET_SCRIPT_MOUSE_OUT             = WIDGET_SCRIPT_MOUSE_EVENT | 0x00010000, /**< Mouse out */
    
    WIDGET_SCRIPT_MOUSE_ON_SCROLL       = WIDGET_SCRIPT_MOUSE_EVENT | 0x00020000, /**< If the box in in scroller and it is scrolled */
    WIDGET_SCRIPT_MOUSE_OFF_SCROLL      = WIDGET_SCRIPT_MOUSE_EVENT | 0x00040000, /**< If the box is in scroller but the scroll is stopped */
    WIDGET_SCRIPT_MOUSE_ON_HOLD         = WIDGET_SCRIPT_MOUSE_EVENT | 0x00080000, /**< Even though the box gets mouse_up event, the click event will not be generated */
    WIDGET_SCRIPT_MOUSE_OFF_HOLD        = WIDGET_SCRIPT_MOUSE_EVENT | 0x00100000, /**< Generate the click event if the mouse_up event occurred as normal */
    
    WIDGET_SCRIPT_KEY_DOWN              = WIDGET_SCRIPT_KEY_EVENT | 0x00020000, /**< Key pressed */
    WIDGET_SCRIPT_KEY_UP                = WIDGET_SCRIPT_KEY_EVENT | 0x00040000, /**< Key released */
    WIDGET_SCRIPT_KEY_FOCUS_IN          = WIDGET_SCRIPT_KEY_EVENT | 0x00080000, /**< Key focus in */
    WIDGET_SCRIPT_KEY_FOCUS_OUT         = WIDGET_SCRIPT_KEY_EVENT | 0x00100000, /**< Key focus out */
} widget_script_event_e;


#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

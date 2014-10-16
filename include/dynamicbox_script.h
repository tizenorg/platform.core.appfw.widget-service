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

#ifndef __DYNAMICBOX_SCRIPT_H
#define __DYNAMICBOX_SCRIPT_H

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

/**
 * @internal
 * @brief Enumeration for script event of dynamicbox.
 * @details
 * This enumeration values will be used for script plugin of data-provider-master.
 * Master will send down these accessibility events to the script plugin.
 * @since_tizen 2.3
 */
typedef enum dynamicbox_script_event {
    DBOX_SCRIPT_ACCESS_EVENT          = 0x01000000, /**< Mask value */
    DBOX_SCRIPT_MOUSE_EVENT           = 0x02000000, /**< Mask value */
    DBOX_SCRIPT_KEY_EVENT             = 0x04000000, /**< Mask value */

    DBOX_SCRIPT_ACCESS_HIGHLIGHT      = DBOX_SCRIPT_ACCESS_EVENT | 0x00000001, /**< Highlight */
    DBOX_SCRIPT_ACCESS_HIGHLIGHT_NEXT = DBOX_SCRIPT_ACCESS_EVENT | 0x00000002, /**< Move Highlight focus to the next object */
    DBOX_SCRIPT_ACCESS_HIGHLIGHT_PREV = DBOX_SCRIPT_ACCESS_EVENT | 0x00000004, /**< Move Highlight focus to the prev object */
    DBOX_SCRIPT_ACCESS_ACTIVATE       = DBOX_SCRIPT_ACCESS_EVENT | 0x00000008, /**< Activate the highlighted object */
    DBOX_SCRIPT_ACCESS_ACTION         = DBOX_SCRIPT_ACCESS_EVENT | 0x00000010, /**< Do specified action for the highlighted object */
    DBOX_SCRIPT_ACCESS_SCROLL         = DBOX_SCRIPT_ACCESS_EVENT | 0x00000020, /**< Scroll operation */
    DBOX_SCRIPT_ACCESS_UNHIGHLIGHT    = DBOX_SCRIPT_ACCESS_EVENT | 0x00000040, /**< Remove highlight */
    DBOX_SCRIPT_ACCESS_VALUE_CHANGE   = DBOX_SCRIPT_ACCESS_EVENT | 0x00000080, /* TODO: deprecate this */
    DBOX_SCRIPT_ACCESS_MOUSE          = DBOX_SCRIPT_ACCESS_EVENT | 0x00000100, /* give mouse event to highlight object */
    DBOX_SCRIPT_ACCESS_BACK           = DBOX_SCRIPT_ACCESS_EVENT | 0x00000200, /* go back to a previous view ex: pop naviframe item */
    DBOX_SCRIPT_ACCESS_OVER           = DBOX_SCRIPT_ACCESS_EVENT | 0x00000400, /* mouse over an object */
    DBOX_SCRIPT_ACCESS_READ           = DBOX_SCRIPT_ACCESS_EVENT | 0x00000800, /* highlight an object */
    DBOX_SCRIPT_ACCESS_ENABLE         = DBOX_SCRIPT_ACCESS_EVENT | 0x00001000, /* enable highlight and read ability */
    DBOX_SCRIPT_ACCESS_DISABLE        = DBOX_SCRIPT_ACCESS_EVENT | 0x00002000, /* disable highlight and read ability */
    
    DBOX_SCRIPT_MOUSE_DOWN            = DBOX_SCRIPT_MOUSE_EVENT | 0x00001000, /**< Mouse down */
    DBOX_SCRIPT_MOUSE_MOVE            = DBOX_SCRIPT_MOUSE_EVENT | 0x00002000, /**< Mouse move */
    DBOX_SCRIPT_MOUSE_UP              = DBOX_SCRIPT_MOUSE_EVENT | 0x00004000, /**< Mouse up */
    DBOX_SCRIPT_MOUSE_IN              = DBOX_SCRIPT_MOUSE_EVENT | 0x00008000, /**< Mouse in */
    DBOX_SCRIPT_MOUSE_OUT             = DBOX_SCRIPT_MOUSE_EVENT | 0x00010000, /**< Mouse out */
    
    DBOX_SCRIPT_MOUSE_ON_SCROLL       = DBOX_SCRIPT_MOUSE_EVENT | 0x00020000, /**< If the box in in scroller and it is scrolled */
    DBOX_SCRIPT_MOUSE_OFF_SCROLL      = DBOX_SCRIPT_MOUSE_EVENT | 0x00040000, /**< If the box is in scroller but the scroll is stopped */
    DBOX_SCRIPT_MOUSE_ON_HOLD         = DBOX_SCRIPT_MOUSE_EVENT | 0x00080000, /**< Even though the box gets mouse_up event, the click event will not be generated */
    DBOX_SCRIPT_MOUSE_OFF_HOLD        = DBOX_SCRIPT_MOUSE_EVENT | 0x00100000, /**< Generate the click event if the mouse_up event occurred as normal */
    
    DBOX_SCRIPT_KEY_DOWN              = DBOX_SCRIPT_KEY_EVENT | 0x00020000, /**< Key pressed */
    DBOX_SCRIPT_KEY_UP                = DBOX_SCRIPT_KEY_EVENT | 0x00040000, /**< Key released */
    DBOX_SCRIPT_KEY_FOCUS_IN          = DBOX_SCRIPT_KEY_EVENT | 0x00080000, /**< Key focus in */
    DBOX_SCRIPT_KEY_FOCUS_OUT         = DBOX_SCRIPT_KEY_EVENT | 0x00100000, /**< Key focus out */
} dynamicbox_script_event_e;

/**
 * @internal
 * @brief
 * Text signal & Content event uses this data structure.
 * @since_tizen 2.3
 */
typedef struct dynamicbox_event_info {
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
} *dynamicbox_event_info_t;

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

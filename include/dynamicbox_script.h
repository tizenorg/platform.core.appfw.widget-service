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
 * @brief
 * Text signal & Content event uses this data structure.
 * @since_tizen 2.3
 */
struct dynamicbox_event_info {
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
};

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

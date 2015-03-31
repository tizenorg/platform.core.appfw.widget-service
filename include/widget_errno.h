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

#ifndef __WIDGET_ERRNO_H
#define __WIDGET_ERRNO_H

#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file widget_errno.h
 * @brief This file declares API of libwidget-service library
 */

/**
 * @addtogroup CAPI_WIDGET_SERVICE_MODULE
 * @{
 */


#define TIZEN_ERROR_WIDGET -0x02FE0000


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
 * @brief Enumeration for the result status of widget operation.
 * @since_tizen 2.4
 */
typedef enum widget_error
{
	WIDGET_ERROR_NONE = TIZEN_ERROR_NONE, /**< Operation is successfully completed */
	WIDGET_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid function parameter */
	WIDGET_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	WIDGET_ERROR_RESOURCE_BUSY = TIZEN_ERROR_RESOURCE_BUSY, /**< Device or resource busy */
	WIDGET_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	WIDGET_ERROR_CANCELED = TIZEN_ERROR_CANCELED, /**< Operation Canceled */
	WIDGET_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< I/O error */
	WIDGET_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Time out */
	WIDGET_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	WIDGET_ERROR_FILE_NO_SPACE_ON_DEVICE = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on device */
	WIDGET_ERROR_FAULT = TIZEN_ERROR_WIDGET | 0x0001, /**< Fault - Unable to recover from the error */
	WIDGET_ERROR_ALREADY_EXIST = TIZEN_ERROR_WIDGET | 0x0002, /**< Already exists */
	WIDGET_ERROR_ALREADY_STARTED = TIZEN_ERROR_WIDGET | 0x0004, /**< Operation is already started */
	WIDGET_ERROR_NOT_EXIST = TIZEN_ERROR_WIDGET | 0x0008, /**< Not exists */
	WIDGET_ERROR_DISABLED = TIZEN_ERROR_WIDGET | 0x0010,  /**< Disabled */
} widget_error_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */

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

/**
 * @brief Enumeration for the result status of widget operation.
 * @since_tizen 2.3
 */
typedef enum widget_status {
    WIDGET_STATUS_ERROR_NONE = TIZEN_ERROR_NONE, /**< Operation is successfully completed */
    WIDGET_STATUS_ERROR = TIZEN_ERROR_WIDGET, /**< This will be OR'd with other specific error value */
    WIDGET_STATUS_ERROR_INVALID_PARAMETER = WIDGET_STATUS_ERROR | 0x0001, /**< Invalid request */
    WIDGET_STATUS_ERROR_FAULT = WIDGET_STATUS_ERROR | 0x0002, /**< Fault - Unable to recover from the error */
    WIDGET_STATUS_ERROR_OUT_OF_MEMORY = WIDGET_STATUS_ERROR | 0x0004, /**< Memory is not enough to do this operation */
    WIDGET_STATUS_ERROR_EXIST = WIDGET_STATUS_ERROR | 0x0008, /**< Already exists */
    WIDGET_STATUS_ERROR_BUSY = WIDGET_STATUS_ERROR | 0x0010, /**< Busy so the operation is not started(accepted), try again */
    WIDGET_STATUS_ERROR_PERMISSION_DENIED = WIDGET_STATUS_ERROR | 0x0020, /**< Permission error */
    WIDGET_STATUS_ERROR_ALREADY = WIDGET_STATUS_ERROR | 0x0040, /**< Operation is already started */
    WIDGET_STATUS_ERROR_CANCEL = WIDGET_STATUS_ERROR | 0x0080, /**< Operation is canceled */
    WIDGET_STATUS_ERROR_IO_ERROR = WIDGET_STATUS_ERROR | 0x0100, /**< I/O Error */
    WIDGET_STATUS_ERROR_NOT_EXIST = WIDGET_STATUS_ERROR | 0x0200, /**< Not exists */
    WIDGET_STATUS_ERROR_TIMEOUT = WIDGET_STATUS_ERROR | 0x0400, /**< Timeout */
    WIDGET_STATUS_ERROR_NOT_IMPLEMENTED = WIDGET_STATUS_ERROR | 0x0800, /**< Operation is not implemented */
    WIDGET_STATUS_ERROR_NO_SPACE = WIDGET_STATUS_ERROR | 0x1000, /**< No space to operate */
    WIDGET_STATUS_ERROR_DISABLED = WIDGET_STATUS_ERROR | 0x2000 /**< Disabled */
} widget_status_e;

/**
 * @brief Definition for macro function to check whether given code value indicates error or not.
 * @since_tizen 2.3
 */
#define WIDGET_STATUS_IS_ERROR(s)    (!!((s) & WIDGET_STATUS_ERROR))

/**
 * @internal
 * @brief Set the status for the last operation
 * @param[in] status widget_status_e status
 * @since_tizen 2.3
 * @return void
 * @see widget_last_status()
 */
extern void widget_set_last_status(widget_status_e status);

/**
 * @brief Get the last error status
 * @since_tizen 2.3
 * @return int widget error status
 * @retval #WIDGET_STATUS_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #WIDGET_STATUS_ERROR_NONE successfully operated
 * @retval #WIDGET_STATUS_ERROR_OUT_OF_MEMORY Memory is not enough
 * @retval #WIDGET_STATUS_ERROR_ALREADY Already exists or operated
 * @retval #WIDGET_STATUS_ERROR_BUSY Too busy to handles request, try it again
 * @retval #WIDGET_STATUS_ERROR_FAULT Fault - Unable to recover from the error
 * @retval #WIDGET_STATUS_ERROR_EXIST Already exists
 * @retval #WIDGET_STATUS_ERROR_PERMISSION_DENIED Permission error
 * @retval #WIDGET_STATUS_ERROR_CANCEL Operation is canceled
 * @retval #WIDGET_STATUS_ERROR_IO_ERROR I/O Error
 * @retval #WIDGET_STATUS_ERROR_NOT_EXIST Not exists
 * @retval #WIDGET_STATUS_ERROR_TIMEOUT Timeout
 * @retval #WIDGET_STATUS_ERROR_NOT_IMPLEMENTED Operation is not implemented
 * @retval #WIDGET_STATUS_ERROR_NO_SPACE No space to operate
 * @retval #WIDGET_STATUS_ERROR_DISABLED Disabled
 */
extern widget_status_e widget_last_status(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */

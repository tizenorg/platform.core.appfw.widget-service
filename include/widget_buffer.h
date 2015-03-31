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

#ifndef __WIDGET_BUFFER_H
#define __WIDGET_BUFFER_H

#include <widget_service_internal.h>

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
 * @brief Buffer type of widget
 */
typedef enum widget_fb_type { /*!< Must have to be sync with libwidget-provider, libwidget-viewer */
    WIDGET_FB_TYPE_FILE,
    WIDGET_FB_TYPE_SHM,
    WIDGET_FB_TYPE_PIXMAP,
    WIDGET_FB_TYPE_ERROR
} widget_fb_type_e;

/**
 * @internal
 * @brief ADT for widget Buffer
 * @since_tizen 2.3
 */
typedef struct widget_fb { /*!< Must has to be sync with slave & provider */
    enum widget_fb_state {
        WIDGET_FB_STATE_CREATED = 0x00beef00,
        WIDGET_FB_STATE_DESTROYED = 0x00dead00
    } state;
    widget_fb_type_e type;
    int refcnt;
    void *info;
    char data[];
} *widget_fb_t;

/**
 * @internal
 * @brief This enumeration value has to be sync'd with the libwidget interface. (only for inhouse widget)
 * @since_tizen 2.3
 */
typedef enum widget_target_type {
    WIDGET_TYPE_WIDGET, /**< widget */
    WIDGET_TYPE_GBAR, /**< Glance Bar */
    WIDGET_TYPE_ERROR /**< Error */
} widget_target_type_e;

/**
 * @internal
 * @brief widget Frame Buffer Info
 * @since_tizen 2.3
 */
struct fb_info {
    char *id;
    int w;
    int h;
    int bufsz;
    void *buffer;

    int pixels;
    int handle;

    void *gem;
};

/**
 * @internal
 * @brief Locking type - Read/Write
 * @since_tizen 2.3
 */
typedef enum widget_lock_type {
    WIDGET_LOCK_READ = 0x01,
    WIDGET_LOCK_WRITE = 0x02,
} widget_lock_type_e;

/**
 * @internal
 * @brief Locking info
 * @since_tizen 2.3
 */
typedef struct widget_lock_info {
    char *filename;
    int fd;
    widget_lock_type_e type;
} *widget_lock_info_t;


/**
 * @internal
 * @brief widget Buffer structure
 * @since_tizen 2.3
 */
typedef struct widget_buffer {
    enum {
        BUFFER_INITIALIZED = 0x0b0e0e0f,
        BUFFER_CREATED = 0x00beef00,
        BUFFER_DESTROYED = 0x00dead00,
    } state;

    widget_target_type_e type;

    union {
        int fd; /* File handle(descriptor) */
        int id; /* SHM handle(id) */
    } handle;

    char *pkgname;
    char *id;
    int width;
    int height;
    int pixel_size;
    int auto_align;
    int frame_skip; /**< To skip the first few frames to prevent from unexpected buffer clear */

    struct fb_info *fb;

    int (*handler)(struct widget_buffer *info, struct widget_buffer_event_data *event_info, void *data);
    void *data;

    void *user_data;

    unsigned int *extra_buffer;

    widget_lock_info_t lock_info;
} *widget_buffer_h;

/**
 * @internal
 * @brief Create a lock instance
 * @param[in] uri Instance URI
 * @param[in] type widget_target_type_e, WIDGET or GBAR
 * @param[in] option Read or Write
 * @return widget_lock_info_t
 * @retval NULL if it fails to create a lock, proper error code will be set on last_status
 * @retval info Lock information handler
 * @see widget_service_destroy_lock()
 * @see widget_service_acquire_lock()
 * @see widget_service_release_lock()
 */
extern widget_lock_info_t widget_service_create_lock(const char *uri, widget_target_type_e type, widget_lock_type_e option);

/**
 * @internal
 * @brief Destroy a lock instance
 * @param[in] info Lock information handler
 * @return status
 * @retval #WIDGET_ERROR_INVALID_PARAMETER invalid paramter
 * @retval #WIDGET_ERROR_IO_ERROR Failed to manage the lock file
 * @retval #WIDGET_ERROR_NONE Successfully destroyed
 */
extern int widget_service_destroy_lock(widget_lock_info_t info);

/**
 * @internal
 * @brief Acquire a lock instance
 * @param[in] info Lock information handler
 * @return status
 * @retval #WIDGET_ERROR_INVALID_PARAMETER invalid paramter
 * @retval #WIDGET_ERROR_NONE Successfully destroyed
 */
extern int widget_service_acquire_lock(widget_lock_info_t info);

/**
 * @internal
 * @brief Acquire a lock instance
 * @param[in] info Lock information handler
 * @return status
 * @retval #WIDGET_ERROR_INVALID_PARAMETER invalid paramter
 * @retval #WIDGET_ERROR_NONE Successfully destroyed
 */
extern int widget_service_release_lock(widget_lock_info_t info);

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

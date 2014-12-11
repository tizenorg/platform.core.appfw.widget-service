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

/**
 * @internal
 * @brief Buffer type of dynamicbox
 */
typedef enum dynamicbox_fb_type { /*!< Must have to be sync with libprovider, liblivebox-viewer */
    DBOX_FB_TYPE_FILE,
    DBOX_FB_TYPE_SHM,
    DBOX_FB_TYPE_PIXMAP,
    DBOX_FB_TYPE_ERROR
} dynamicbox_fb_type_e;

/**
 * @internal
 * @brief ADT for Dynamicbox Buffer
 * @since_tizen 2.3
 */
typedef struct dynamicbox_fb { /*!< Must has to be sync with slave & provider */
    enum dynamicbox_fb_state {
        DBOX_FB_STATE_CREATED = 0x00beef00,
        DBOX_FB_STATE_DESTROYED = 0x00dead00
    } state;
    dynamicbox_fb_type_e type;
    int refcnt;
    void *info;
    char data[];
} *dynamicbox_fb_t;

/**
 * @internal
 * @brief This enumeration value has to be sync'd with the libdynamicbox interface. (only for inhouse dynamicbox)
 * @since_tizen 2.3
 */
typedef enum dynamicbox_target_type {
    DBOX_TYPE_DBOX, /**< Dynamicbox */
    DBOX_TYPE_GBAR, /**< Glance Bar */
    DBOX_TYPE_ERROR /**< Error */
} dynamicbox_target_type_e;

/**
 * @internal
 * @brief Dynamic Box Frame Buffer Info
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
typedef enum dynamicbox_lock_type {
    DBOX_LOCK_READ = 0x01,
    DBOX_LOCK_WRITE = 0x02,
} dynamicbox_lock_type_e;

/**
 * @internal
 * @brief Locking info
 * @since_tizen 2.3
 */
typedef struct dynamicbox_lock_info {
    char *filename;
    int fd;
    dynamicbox_lock_type_e type;
} *dynamicbox_lock_info_t;


/**
 * @internal
 * @brief Dynamic Box Buffer structure
 * @since_tizen 2.3
 */
typedef struct dynamicbox_buffer {
    enum {
        BUFFER_INITIALIZED = 0x0b0e0e0f,
        BUFFER_CREATED = 0x00beef00,
        BUFFER_DESTROYED = 0x00dead00,
    } state;

    dynamicbox_target_type_e type;

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

    int (*handler)(struct dynamicbox_buffer *info, struct dynamicbox_buffer_event_data *event_info, void *data);
    void *data;

    void *user_data;

    unsigned int *extra_buffer;

    dynamicbox_lock_info_t lock_info;
} *dynamicbox_buffer_h;

/**
 * @internal
 * @brief Create a lock instance
 * @param[in] uri Instance URI
 * @param[in] type dynamicbox_target_type_e, DBOX or GBAR
 * @param[in] option Read or Write
 * @return dynamicbox_lock_info_t
 * @retval NULL if it fails to create a lock, proper error code will be set on last_status
 * @retval info Lock information handler
 * @see dynamicbox_service_destroy_lock()
 * @see dynamicbox_service_acquire_lock()
 * @see dynamicbox_service_release_lock()
 */
extern dynamicbox_lock_info_t dynamicbox_service_create_lock(const char *uri, dynamicbox_target_type_e type, dynamicbox_lock_type_e option);

/**
 * @internal
 * @brief Destroy a lock instance
 * @param[in] info Lock information handler
 * @return status
 * @retval #DBOX_STATUS_ERROR_INVALID_PARAMETER invalid paramter
 * @retval #DBOX_STATUS_ERROR_IO_ERROR Failed to manage the lock file
 * @retval #DBOX_STATUS_ERROR_NONE Successfully destroyed
 */
extern int dynamicbox_service_destroy_lock(dynamicbox_lock_info_t info);

/**
 * @internal
 * @brief Acquire a lock instance
 * @param[in] info Lock information handler
 * @return status
 * @retval #DBOX_STATUS_ERROR_INVALID_PARAMETER invalid paramter
 * @retval #DBOX_STATUS_ERROR_NONE Successfully destroyed
 */
extern int dynamicbox_service_acquire_lock(dynamicbox_lock_info_t info);

/**
 * @internal
 * @brief Acquire a lock instance
 * @param[in] info Lock information handler
 * @return status
 * @retval #DBOX_STATUS_ERROR_INVALID_PARAMETER invalid paramter
 * @retval #DBOX_STATUS_ERROR_NONE Successfully destroyed
 */
extern int dynamicbox_service_release_lock(dynamicbox_lock_info_t info);

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

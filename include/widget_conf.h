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

#ifndef __WIDGET_SERVICE_CONF_H
#define __WIDGET_SERVICE_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief Load a configuration file to the memory
 * @since_tizen 2.3.1
 * @return int
 * @retval #WIDGET_ERROR_NONE Successfully loaded
 * @retval #WIDGET_ERROR_IO_ERROR Failed to access the configuration file
 * @see widget_conf_reset()
 */
extern int widget_conf_load(void);

/**
 * @internal
 * @brief Reset configuration data to its default values
 * @since_tizen 2.3.1
 * @return void
 * @see widget_conf_init()
 */
extern void widget_conf_reset(void);

/**
 * @internal
 * @brief setup the configuration data using default values
 * @since_tizen 2.3.1
 * @return void
 * @see widget_conf_reset()
 */
extern void widget_conf_init(void);

/**
 * @internal
 * @brief Get the flag of configuration loaded status
 * @since_tizen 2.3.1
 * @return int 1 if it is loaded or 0
 * @see widget_conf_load()
 */
extern const int const widget_conf_is_loaded(void);

/**
 * @internal
 * @brief configuration value getter
 * @detail If your service provider intended to check the window stack directly,
 *         Refer this flag, if this returns 1, you should monitor the window stack
 *         to send the pause/resume event and it should be managed manually.
 * @remarks this is just a recommendation to implement service provider,
 *          so the service provider's behaviour is depeneds on its implementation.
 * @since_tizen 2.3.1
 * @return int 1 if the xmonitor should be enabled or 0
 */
extern const int const widget_conf_use_xmonitor(void);

/**
 * @internal
 * @brief configuration value getter for emergency disk mount information
 * @detail if the system has not enough space to operate itself, the master provider allocate memory for
 *         its temporary storage. to save some files.
 * @remarks this is only used for master service provider.
 *          all other service provider should not need to know this configuration value.
 *          but this API support to get current configuration.
 * @since_tizen 2.3.1
 * @return const char * const
 * @retval string emergency disk mount information includes options
 * @retval @c NULL if there is no configuration information
 */
extern const char * const widget_conf_emergency_disk(void);

/**
 * @internal
 * @brief if the master service provider should need to check the lcd status to pause/resume the service providers,
 *        this function will returns 1, or 0.
 * @since_tizen 2.3.1
 * @return int
 * @retval 1 need to care the lcd status by master service provider
 * @retval 0 do not care it even if the lcd is off/on, do not send any pause/resume event to the service providers.
 */
extern const int const widget_conf_check_lcd(void);

/**
 * @internal
 * @brief select the option whether use the event time of each event data from device or not.
 *        if this returns 1, the master service provider should keep its timestamp of each event data,
 *        or use the logical timestamp that is get by service provider.
 * @since_tizen 2.3.1
 * @return int
 * @retval 1 use the timestamp of event data
 * @retval 0 use the system timestamp instead of event data of device
 */
extern const int const widget_conf_use_event_time(void);

/**
 * @internal
 * @brief Get the configuration value for AUTO Alignment of canvas buffer.
 * @details
 *      If the UIFW does not support the STRIDE information for rendering frame,
 *      This function will returns 1, then the widget will automatically aligning the buffer.
 * @since_tizen 2.3.1
 * @return status of auto alignment.
 * @retval 1 Content will be automatically aligned by widget.
 * @retval 0 Content will be aligned by UIFW, using stride information.
 */
extern const int const widget_conf_auto_align(void);

/**
 * @internal
 * @brief this returns formatted string that represents service list to be enabled
 *        format is "[service name1],[service name2],..."\n
 *        Candidate services are\n
 *        [widget]\n
 *        [badge]\n
 *        [notification]\n
 *        [utility]\n
 *        [shortcut]\n
 * @since_tizen 2.3.1
 * @return string
 * @retval NULL if there is no defined list. you should treat this as a default service list
 */
extern const char * const widget_conf_services(void);

/**
 * @internal
 * @brief Get the configuration value of "use_sw_backend"
 *        if this returns 1, the provider should not use the Graphics buffer.
 * @since_tizen 2.3.1
 * @return int
 * @retval 1 true Use the S/W buffer such as heap instead of graphics buffer
 * @retval 0 false You can choose S/W buffer or Graphics buffer
 * @see widget_conf_provider_method()
 */
extern const int const widget_conf_use_sw_backend(void);

/**
 * @internal
 * @brief Content sharing method for buffer type widget (or gbar)
 *        it could be "pixmap", "shm", "file"
 *        in case of pixmap, the master service provider will try to allocate a resource from graphics sub system (ex, Xorg)
 *        in case of shm, it will try to allocate shared memory.
 *        in case of file, it will create a dummy file to write and read frame data from/to it.
 * @since_tizen 2.3.1
 * @return const char * const
 * @retval "pixmap" use the pixmap
 * @retval "shm" use the shared memory
 * @retval "file" use a file
 */
extern const char * const widget_conf_provider_method(void);

/**
 * @internal
 * @brief if a debug mode is enabled, master will not launch the service provider directly.
 *        instead of it, the master will permanently waiting the launch of service provider.
 *        so the developer should launch the service provider with correct bundle data.
 *        and it should send the "hello" message to the master provider
 * @since_tizen 2.3.1
 * @return int
 * @retval 1 master will turn on the debug mode
 * @retval 0 master will turn off the debug mode
 */
extern const int const widget_conf_debug_mode(void);

/**
 * @internal
 * @brief If the overwrite content is enabled, the provider will not prepare to prevent from output file overwriting.
 *	  To do it, the provider will renaming the output file right after it is created.
 * @since_tizen 2.3.1
 * @return int bool
 * @retval 1 Overwrite content
 * @retval 0 Prevent from overwriting content
 */
extern const int const widget_conf_overwrite_content(void);

/**
 * @internal
 */
extern const int const widget_conf_com_core_thread(void);

/**
 * @internal
 */
extern const unsigned int const widget_conf_base_width(void);

/**
 * @internal
 */
extern const unsigned int const widget_conf_base_height(void);

/**
 * @internal
 */
extern const double const widget_conf_minimum_period(void);

/**
 * @internal
 */
extern const int const widget_conf_default_pixels(void);

/**
 * @internal
 */
extern const char * const widget_conf_default_script(void);

/**
 * @internal
 */
extern const char * const widget_conf_default_abi(void);

/**
 * @internal
 */
extern const char * const widget_conf_default_gbar_group(void);

/**
 * @internal
 */
extern const double const widget_conf_default_period(void);

/**
 * @internal
 */
extern const double const widget_conf_default_packet_time(void);

/**
 * @internal
 */
extern const char * const widget_conf_default_content(void);

/**
 * @internal
 */
extern const char * const widget_conf_default_title(void);

/**
 * @internal
 */
extern const unsigned long const widget_conf_minimum_space(void);

/**
 * @internal
 */
extern const char * const widget_conf_replace_tag(void);

/**
 * @internal
 */
extern const double const widget_conf_slave_ttl(void);

/**
 * @internal
 */
extern const double const widget_conf_slave_activate_time(void);

/**
 * @internal
 */
extern const double const widget_conf_slave_relaunch_time(void);

/**
 * @internal
 */
extern const int const widget_conf_slave_relaunch_count(void);

/**
 * @internal
 */
extern const int const widget_conf_max_log_line(void);

/**
 * @internal
 */
extern const int const widget_conf_max_log_file(void);

/**
 * @internal
 */
extern const unsigned long const widget_conf_sqlite_flush_max(void);

/**
 * @internal
 */
extern const char * const widget_conf_db_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_reader_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_always_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_log_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_script_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_script_port(void);

/**
 * @internal
 */
extern const char * const widget_conf_share_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_input_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_root_path(void);

/**
 * @internal
 */
extern const char * const widget_conf_path(void);

/**
 * @internal
 */
extern const double const widget_conf_ping_time(void);

/**
 * @internal
 */
extern const int const widget_conf_slave_max_load(void);

/**
 * @internal
 * @brief Get the configuration value for premultiplied alpha.
 * @details
 *    If the system uses the premultiplied alpha for content,
 *    This function will returns 1.
 * @since_tizen 2.3.1
 * @remarks This function only works with the inhouse provider.
 * @return status of premultiplied alpha.
 * @retval 1 Content rendered with premultiplied alpha.
 * @retval 0 Content doesn't rendered with premultiplied alpha.
 */
extern const int const widget_conf_premultiplied_alpha(void);

/**
 * @internal
 */
extern const double const widget_conf_gbar_request_timeout(void);

/**
 * @internal
 */
extern const double const widget_conf_scale_width_factor(void);

/**
 * @internal
 */
extern const double const widget_conf_scale_height_factor(void);

/**
 * @internal
 */
extern const char * const widget_conf_launch_key_name(void);

/**
 * @internal
 */
extern const char * const widget_conf_launch_key_secured(void);

/**
 * @internal
 */
extern const char * const widget_conf_launch_key_abi(void);

/**
 * @internal
 */
extern const char * const widget_conf_launch_key_hw_acceleration(void);

/**
 * @internal
 */
extern const char * const widget_conf_empty_content(void);

/**
 * @internal
 */
extern const char * const widget_conf_empty_title(void);

/**
 * @internal
 */
extern const int const widget_conf_extra_buffer_count(void);

/**
 * @internal
 */
extern const int const widget_conf_use_gettimeofday(void);

/**
 * @internal
 */
extern const int const widget_conf_slave_event_boost_off(void);

/**
 * @internal
 */
extern const int const widget_conf_slave_event_boost_on(void);

/**
 * @internal
 */
extern const double const widget_conf_event_filter(void);

/**
 * @internal
 */
extern const int const widget_conf_slave_limit_to_ttl(void);

/**
 * @internal
 */
extern const int const widget_conf_frame_skip(void);

/**
 * @internal
 */
extern const int const widget_conf_slave_auto_cache_flush(void);

/**
 * @internal
 */
extern const char * const widget_conf_category_list(void);

extern double widget_conf_fault_detect_in_time(void);
extern int widget_conf_fault_detect_count(void);
extern int widget_conf_reactivate_on_pause(void);
extern const char * const widget_conf_app_abi(void);

#define WIDGET_CONF_BASE_W               widget_conf_base_width()
#define WIDGET_CONF_BASE_H               widget_conf_base_height()

#define WIDGET_CONF_EMERGENCY_DISK       widget_conf_emergency_disk()
#define WIDGET_CONF_SCALE_WIDTH_FACTOR   widget_conf_scale_width_factor()
#define WIDGET_CONF_SCALE_HEIGHT_FACTOR  widget_conf_scale_height_factor()

#define WIDGET_CONF_USE_SW_BACKEND       widget_conf_use_sw_backend()
#define WIDGET_CONF_PROVIDER_METHOD      widget_conf_provider_method()
#define WIDGET_CONF_DEBUG_MODE           widget_conf_debug_mode()
#define WIDGET_CONF_OVERWRITE_CONTENT    widget_conf_overwrite_content()
#define WIDGET_CONF_COM_CORE_THREAD      widget_conf_com_core_thread()

#define WIDGET_CONF_MINIMUM_PERIOD       widget_conf_minimum_period()

#define WIDGET_CONF_DEFAULT_SCRIPT       widget_conf_default_script()
#define WIDGET_CONF_DEFAULT_ABI          widget_conf_default_abi()
#define WIDGET_CONF_DEFAULT_GBAR_GROUP   widget_conf_default_gbar_group()
#define WIDGET_CONF_DEFAULT_PERIOD       widget_conf_default_period()
#define WIDGET_CONF_DEFAULT_PIXELS       widget_conf_default_pixels()
#define WIDGET_CONF_PRIORITY_NO_CHANGE   -1.0f

#define WIDGET_CONF_BUNDLE_SLAVE_NAME    widget_conf_launch_key_name()
#define WIDGET_CONF_BUNDLE_SLAVE_SECURED widget_conf_launch_key_secured()
#define WIDGET_CONF_BUNDLE_SLAVE_ABI     widget_conf_launch_key_abi()
#define WIDGET_CONF_BUNDLE_SLAVE_HW_ACCELERATION widget_conf_launch_key_hw_acceleration()
#define WIDGET_CONF_PACKET_TIME          widget_conf_default_packet_time()
#define WIDGET_CONF_CONTENT_NO_CHANGE    widget_conf_empty_content()
#define WIDGET_CONF_TITLE_NO_CHANGE      widget_conf_empty_title()
#define WIDGET_CONF_DEFAULT_TITLE        widget_conf_default_title()
#define WIDGET_CONF_DEFAULT_CONTENT      widget_conf_default_content()
#define WIDGET_CONF_MINIMUM_SPACE        widget_conf_minimum_space()

#define WIDGET_CONF_IMAGE_PATH           widget_conf_share_path()
#define WIDGET_CONF_SCRIPT_PATH          widget_conf_script_path()
#define WIDGET_CONF_SCRIPT_PORT_PATH     widget_conf_script_port()
#define WIDGET_CONF_CONF_PATH            widget_conf_path()
#define WIDGET_CONF_ROOT_PATH            widget_conf_root_path()
#define WIDGET_CONF_LOG_PATH             widget_conf_log_path()
#define WIDGET_CONF_READER_PATH          widget_conf_reader_path()
#define WIDGET_CONF_ALWAYS_PATH          widget_conf_always_path()
#define WIDGET_CONF_INPUT_PATH           widget_conf_input_path()

#define WIDGET_CONF_REPLACE_TAG_APPID    widget_conf_replace_tag()
#define WIDGET_CONF_SLAVE_TTL            widget_conf_slave_ttl()
#define WIDGET_CONF_SLAVE_ACTIVATE_TIME  widget_conf_slave_activate_time()
#define WIDGET_CONF_SLAVE_RELAUNCH_TIME  widget_conf_slave_relaunch_time()
#define WIDGET_CONF_SLAVE_RELAUNCH_COUNT widget_conf_slave_relaunch_count()

#define WIDGET_CONF_MAX_LOG_LINE         widget_conf_max_log_line()
#define WIDGET_CONF_MAX_LOG_FILE         widget_conf_max_log_file()
#define WIDGET_CONF_CATEGORY_LIST        widget_conf_category_list()

#define WIDGET_CONF_SQLITE_FLUSH_MAX     widget_conf_sqlite_flush_max()
#define WIDGET_CONF_DBFILE               widget_conf_db_path()

#define WIDGET_CONF_GBAR_REQUEST_TIMEOUT widget_conf_gbar_request_timeout()

#define WIDGET_CONF_SLAVE_MAX_LOAD       widget_conf_slave_max_load()
#define WIDGET_CONF_DEFAULT_PING_TIME    widget_conf_ping_time()
#define WIDGET_CONF_PREMULTIPLIED_COLOR  widget_conf_premultiplied_alpha()
#define WIDGET_CONF_SERVICES             widget_conf_services()
#define WIDGET_CONF_EXTRA_BUFFER_COUNT   widget_conf_extra_buffer_count()

#define WIDGET_CONF_SERVICE_WIDGET   "[widget]"
#define WIDGET_CONF_SERVICE_NOTIFICATION "[notification]"
#define WIDGET_CONF_SERVICE_BADGE        "[badge]"
#define WIDGET_CONF_SERVICE_SHORTCUT     "[shortcut]"
#define WIDGET_CONF_SERVICE_UTILITY      "[utility]"
#define WIDGET_CONF_SERVICE_FILE         "[file]"

#define WIDGET_CONF_PAUSED_FILE          "/tmp/.live.paused"

#define WIDGET_CONF_USE_XMONITOR         widget_conf_use_xmonitor()
#define WIDGET_CONF_AUTO_ALIGN           widget_conf_auto_align()
#define WIDGET_CONF_USE_EVENT_TIME       widget_conf_use_event_time()
#define WIDGET_CONF_CHECK_LCD            widget_conf_check_lcd()

#define WIDGET_CONF_USE_GETTIMEOFDAY     widget_conf_use_gettimeofday()

#define WIDGET_CONF_DELAY_TIME           0.0000001f
#define WIDGET_CONF_DEFAULT_CLUSTER      "user,created"
#define WIDGET_CONF_MINIMUM_REACTIVATION_TIME 10

#define WIDGET_CONF_SLAVE_EVENT_BOOST_ON  widget_conf_slave_event_boost_on()
#define WIDGET_CONF_SLAVE_EVENT_BOOST_OFF widget_conf_slave_event_boost_off()
#define WIDGET_CONF_EVENT_FILTER	      widget_conf_event_filter()
#define WIDGET_CONF_SLAVE_LIMIT_TO_TTL    widget_conf_slave_limit_to_ttl()
#define WIDGET_CONF_FRAME_SKIP              widget_conf_frame_skip()
#define WIDGET_CONF_SLAVE_AUTO_CACHE_FLUSH  widget_conf_slave_auto_cache_flush()
#define WIDGET_CONF_FAULT_DETECT_COUNT      widget_conf_fault_detect_count()
#define WIDGET_CONF_FAULT_DETECT_IN_TIME    widget_conf_fault_detect_in_time()
#define WIDGET_CONF_REACTIVATE_ON_PAUSE     widget_conf_reactivate_on_pause()
#define WIDGET_CONF_APP_ABI                 widget_conf_app_abi()

#if !defined(VCONFKEY_MASTER_STARTED)
#define VCONFKEY_MASTER_STARTED    "memory/data-provider-master/started"
#endif

#if !defined(VCONFKEY_MASTER_RESTART_COUNT)
#define VCONFKEY_MASTER_RESTART_COUNT    "memory/private/data-provider-master/restart_count"
#endif

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */

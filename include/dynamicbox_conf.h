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

#ifndef __DYNAMICBOX_SERVICE_CONF_H
#define __DYNAMICBOX_SERVICE_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @internal
 * @brief Load a configuration file to the memory
 * @since_tizen 2.3
 * @return int
 * @retval #DBOX_STATUS_ERROR_NONE Successfully loaded
 * @retval #DBOX_STATUS_ERROR_IO_ERROR Failed to access the configuration file
 * @see dynamicbox_conf_reset()
 */
extern int dynamicbox_conf_load(void);

/**
 * @internal
 * @brief Reset configuration data to its default values
 * @since_tizen 2.3
 * @return void
 * @see dynamicbox_conf_init()
 */
extern void dynamicbox_conf_reset(void);

/**
 * @internal
 * @brief setup the configuration data using default values
 * @since_tizen 2.3
 * @return void
 * @see dynamicbox_conf_reset()
 */
extern void dynamicbox_conf_init(void);

/**
 * @internal
 * @brief Get the flag of configuration loaded status
 * @since_tizen 2.3
 * @return int 1 if it is loaded or 0
 * @see dynamicbox_conf_load()
 */
extern const int const dynamicbox_conf_is_loaded(void);

/**
 * @internal
 * @brief configuration value getter
 * @detail If your service provider intended to check the window stack directly,
 *         Refer this flag, if this returns 1, you should monitor the window stack
 *         to send the pause/resume event and it should be managed manually.
 * @remarks this is just a recommendation to implement service provider,
 *          so the service provider's behaviour is depeneds on its implementation.
 * @since_tizen 2.3
 * @return int 1 if the xmonitor should be enabled or 0
 */
extern const int const dynamicbox_conf_use_xmonitor(void);
extern const char * const dynamicbox_conf_emergency_disk(void);
extern const int const dynamicbox_conf_check_lcd(void);
extern const int const dynamicbox_conf_use_event_time(void);

/**
 * @internal
 * @brief Get the configuration value for AUTO Alignment of canvas buffer.
 * @details
 *      If the UIFW does not support the STRIDE information for rendering frame,
 *      This function will returns 1, then the dynamicbox will automatically aligning the buffer.
 * @since_tizen 2.3
 * @return status of auto alignment.
 * @retval 1 Content will be automatically aligned by dynamicbox.
 * @retval 0 Content will be aligned by UIFW, using stride information.
 */
extern const int const dynamicbox_conf_auto_align(void);

/**
 * @internal
 * @brief this returns formatted string that represents service list to be enabled
 *        format is "[service name1],[service name2],..."\n
 *        Candidate services are\n
 *        [dynamicbox]\n
 *        [badge]\n
 *        [notification]\n
 *        [utility]\n
 *        [shortcut]\n
 * @return string
 * @retval NULL if there is no defined list. you should treat this as a default service list
 */
extern const char * const dynamicbox_conf_services(void);

/**
 * @internal
 * @brief Get the configuration value of "use_sw_backend"
 *        if this returns 1, the provider should not use the Graphics buffer.
 * @return int
 * @retval 1 true Use the S/W buffer such as heap instead of graphics buffer
 * @retval 0 false You can choose S/W buffer or Graphics buffer
 * @see dynamicbox_conf_provider_method()
 */
extern const int const dynamicbox_conf_use_sw_backend(void);

extern const char * const dynamicbox_conf_provider_method(void);
extern const int const dynamicbox_conf_debug_mode(void);
extern const int const dynamicbox_conf_overwrite_content(void);
extern const int const dynamicbox_conf_com_core_thread(void);
extern const unsigned int const dynamicbox_conf_base_width(void);
extern const unsigned int const dynamicbox_conf_base_height(void);
extern const double const dynamicbox_conf_minimum_period(void);
extern const int const dynamicbox_conf_default_pixels(void);
extern const char * const dynamicbox_conf_default_script(void);
extern const char * const dynamicbox_conf_default_abi(void);
extern const char * const dynamicbox_conf_default_gbar_group(void);
extern const double const dynamicbox_conf_default_period(void);
extern const double const dynamicbox_conf_default_packet_time(void);
extern const char * const dynamicbox_conf_default_content(void);
extern const char * const dynamicbox_conf_default_title(void);
extern const unsigned long const dynamicbox_conf_minimum_space(void);
extern const char * const dynamicbox_conf_replace_tag(void);
extern const double const dynamicbox_conf_slave_ttl(void);
extern const double const dynamicbox_conf_slave_activate_time(void);
extern const double const dynamicbox_conf_slave_relaunch_time(void);
extern const int const dynamicbox_conf_slave_relaunch_count(void);
extern const int const dynamicbox_conf_max_log_line(void);
extern const int const dynamicbox_conf_max_log_file(void);
extern const unsigned long const dynamicbox_conf_sqlite_flush_max(void);
extern const char * const dynamicbox_conf_db_path(void);
extern const char * const dynamicbox_conf_reader_path(void);
extern const char * const dynamicbox_conf_always_path(void);
extern const char * const dynamicbox_conf_log_path(void);
extern const char * const dynamicbox_conf_script_path(void);
extern const char * const dynamicbox_conf_script_port(void);
extern const char * const dynamicbox_conf_share_path(void);
extern const char * const dynamicbox_conf_input_path(void);
extern const char * const dynamicbox_conf_root_path(void);
extern const char * const dynamicbox_conf_path(void);
extern const double const dynamicbox_conf_ping_time(void);
extern const int const dynamicbox_conf_slave_max_load(void);

/**
 * @internal
 * @brief Get the configuration value for premultiplied alpha.
 * @details
 *	If the system uses the premultiplied alpha for content,
 *	This function will returns 1.
 * @since_tizen 2.3
 * @remarks This function only works with the inhouse provider.
 * @return status of premultiplied alpha.
 * @retval 1 Content rendered with premultiplied alpha.
 * @retval 0 Content doesn't rendered with premultiplied alpha.
 */
extern const int const dynamicbox_conf_premultiplied_alpha(void);
extern const double const dynamicbox_conf_gbar_request_timeout(void);
extern const double const dynamicbox_conf_scale_width_factor(void);
extern const double const dynamicbox_conf_scale_height_factor(void);
extern const char * const dynamicbox_conf_launch_key_name(void);
extern const char * const dynamicbox_conf_launch_key_secured(void);
extern const char * const dynamicbox_conf_launch_key_abi(void);
extern const char * const dynamicbox_conf_empty_content(void);
extern const char * const dynamicbox_conf_empty_title(void);

#define DYNAMICBOX_CONF_BASE_W               dynamicbox_conf_base_width()
#define DYNAMICBOX_CONF_BASE_H               dynamicbox_conf_base_height()

#define DYNAMICBOX_CONF_EMERGENCY_DISK       dynamicbox_conf_emergency_disk()
#define DYNAMICBOX_CONF_SCALE_WIDTH_FACTOR   dynamicbox_conf_scale_width_factor()
#define DYNAMICBOX_CONF_SCALE_HEIGHT_FACTOR  dynamicbox_conf_scale_height_factor()

#define DYNAMICBOX_CONF_USE_SW_BACKEND       dynamicbox_conf_use_sw_backend()
#define DYNAMICBOX_CONF_PROVIDER_METHOD      dynamicbox_conf_provider_method()
#define DYNAMICBOX_CONF_DEBUG_MODE           dynamicbox_conf_debug_mode()
#define DYNAMICBOX_CONF_OVERWRITE_CONTENT    dynamicbox_conf_overwrite_content()
#define DYNAMICBOX_CONF_COM_CORE_THREAD      dynamicbox_conf_com_core_thread()

#define DYNAMICBOX_CONF_MINIMUM_PERIOD       dynamicbox_conf_minimum_period()

#define DYNAMICBOX_CONF_DEFAULT_SCRIPT       dynamicbox_conf_default_script()
#define DYNAMICBOX_CONF_DEFAULT_ABI          dynamicbox_conf_default_abi()
#define DYNAMICBOX_CONF_DEFAULT_GBAR_GROUP   dynamicbox_conf_default_gbar_group()
#define DYNAMICBOX_CONF_DEFAULT_PERIOD       dynamicbox_conf_default_period()
#define DYNAMICBOX_CONF_DEFAULT_PIXELS       dynamicbox_conf_default_pixels()
#define DYNAMICBOX_CONF_PRIORITY_NO_CHANGE   -1.0f

#define DYNAMICBOX_CONF_BUNDLE_SLAVE_NAME    dynamicbox_conf_launch_key_name()
#define DYNAMICBOX_CONF_BUNDLE_SLAVE_SECURED dynamicbox_conf_launch_key_secured()
#define DYNAMICBOX_CONF_BUNDLE_SLAVE_ABI     dynamicbox_conf_launch_key_abi()
#define DYNAMICBOX_CONF_PACKET_TIME          dynamicbox_conf_default_packet_time()
#define DYNAMICBOX_CONF_CONTENT_NO_CHANGE    dynamicbox_conf_empty_content()
#define DYNAMICBOX_CONF_TITLE_NO_CHANGE      dynamicbox_conf_empty_title()
#define DYNAMICBOX_CONF_DEFAULT_TITLE        dynamicbox_conf_default_title()
#define DYNAMICBOX_CONF_DEFAULT_CONTENT      dynamicbox_conf_default_content()
#define DYNAMICBOX_CONF_MINIMUM_SPACE        dynamicbox_conf_minimum_space()

#define DYNAMICBOX_CONF_IMAGE_PATH           dynamicbox_conf_share_path()
#define DYNAMICBOX_CONF_SCRIPT_PATH          dynamicbox_conf_script_path()
#define DYNAMICBOX_CONF_SCRIPT_PORT_PATH     dynamicbox_conf_script_port()
#define DYNAMICBOX_CONF_CONF_PATH            dynamicbox_conf_path()
#define DYNAMICBOX_CONF_ROOT_PATH            dynamicbox_conf_root_path()
#define DYNAMICBOX_CONF_LOG_PATH             dynamicbox_conf_log_path()
#define DYNAMICBOX_CONF_READER_PATH          dynamicbox_conf_reader_path()
#define DYNAMICBOX_CONF_ALWAYS_PATH          dynamicbox_conf_always_path()
#define DYNAMICBOX_CONF_INPUT_PATH           dynamicbox_conf_input_path()

#define DYNAMICBOX_CONF_REPLACE_TAG_APPID    dynamicbox_conf_replace_tag()
#define DYNAMICBOX_CONF_SLAVE_TTL            dynamicbox_conf_slave_ttl()
#define DYNAMICBOX_CONF_SLAVE_ACTIVATE_TIME  dynamicbox_conf_slave_activate_time()
#define DYNAMICBOX_CONF_SLAVE_RELAUNCH_TIME  dynamicbox_conf_slave_relaunch_time()
#define DYNAMICBOX_CONF_SLAVE_RELAUNCH_COUNT dynamicbox_conf_slave_relaunch_count()

#define DYNAMICBOX_CONF_MAX_LOG_LINE         dynamicbox_conf_max_log_line()
#define DYNAMICBOX_CONF_MAX_LOG_FILE         dynamicbox_conf_max_log_file()

#define DYNAMICBOX_CONF_SQLITE_FLUSH_MAX     dynamicbox_conf_sqlite_flush_max()
#define DYNAMICBOX_CONF_DBFILE               dynamicbox_conf_db_path()

#define DYNAMICBOX_CONF_GBAR_REQUEST_TIMEOUT dynamicbox_conf_gbar_request_timeout()

#define DYNAMICBOX_CONF_SLAVE_MAX_LOAD       dynamicbox_conf_slave_max_load()
#define DYNAMICBOX_CONF_DEFAULT_PING_TIME    dynamicbox_conf_ping_time()
#define DYNAMICBOX_CONF_PREMULTIPLIED_COLOR  dynamicbox_conf_premultiplied_alpha()
#define DYNAMICBOX_CONF_SERVICES             dynamicbox_conf_services()

#define DYNAMICBOX_CONF_SERVICE_DYNAMICBOX   "[dynamicbox]"
#define DYNAMICBOX_CONF_SERVICE_NOTIFICATION "[notification]"
#define DYNAMICBOX_CONF_SERVICE_BADGE        "[badge]"
#define DYNAMICBOX_CONF_SERVICE_SHORTCUT     "[shortcut]"
#define DYNAMICBOX_CONF_SERVICE_UTILITY      "[utility]"
#define DYNAMICBOX_CONF_SERVICE_FILE         "[file]"

#define DYNAMICBOX_CONF_PAUSED_FILE          "/tmp/.live.paused"

#define DYNAMICBOX_CONF_USE_XMONITOR         dynamicbox_conf_use_xmonitor()
#define DYNAMICBOX_CONF_AUTO_ALIGN           dynamicbox_conf_auto_align()
#define DYNAMICBOX_CONF_USE_EVENT_TIME       dynamicbox_conf_use_event_time()
#define DYNAMICBOX_CONF_CHECK_LCD            dynamicbox_conf_check_lcd()

#define DYNAMICBOX_CONF_DELAY_TIME           0.0000001f
#define DYNAMICBOX_CONF_DEFAULT_CLUSTER      "user,created"
#define DYNAMICBOX_CONF_MINIMUM_REACTIVATION_TIME 10

#if !defined(VCONFKEY_MASTER_STARTED)
#define VCONFKEY_MASTER_STARTED	"memory/data-provider-master/started"
#endif

#if !defined(VCONFKEY_MASTER_RESTART_COUNT)
#define VCONFKEY_MASTER_RESTART_COUNT	"memory/private/data-provider-master/restart_count"
#endif

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */

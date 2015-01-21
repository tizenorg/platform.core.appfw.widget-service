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

#ifndef __DYNAMICBOX_CMD_LIST_H
#define __DYNAMICBOX_CMD_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file dynamicbox_cmd_list.h
 * @brief This file declares the list of command Id that are used for communicating with provider & viewer
 *        Max length of CMD is 24 (PACKET_MAX_CMD from packet.h)
 */

/**
 * @addtogroup CAPI_DYNAMICBOX_SERVICE_MODULE
 * @{
 */

/**
 * @internal
 * @brief Command list for
 *        Provider to Master,
 *        or Master to Viewer
 */
#define CMD_UPDATED            0x00000001   /**< DBOX Updated */
#define CMD_DBOX_UPDATED       0x00000001   /**< Alias of CMD_UPDATED */
#define CMD_DESC_UPDATED       0x00000101   /**< GBAR Updated */
#define CMD_GBAR_UPDATED       0x00000101   /**< Alias of CMD_DESC_UPDATED */
#define CMD_EXTRA_UPDATED      0x00000201
#define CMD_EXTRA_INFO         0x00000301   /**< Extra information is updated */
#define CMD_DELETED            0x00000401   /**< DBox is deleted */
#define CMD_FAULTED            0x00000501   /**< DBox is faulted */
#define CMD_FAULT_PACKAGE      0x00000501   /**< Faulted package */
#define CMD_SCROLL             0x00000601
#define CMD_DBOX_UPDATE_BEGIN  0x00000701
#define CMD_DBOX_UPDATE_END    0x00000801
#define CMD_GBAR_UPDATE_BEGIN  0x00000901
#define CMD_GBAR_UPDATE_END    0x00000A01
#define CMD_ACCESS_STATUS      0x00000B01
#define CMD_KEY_STATUS         0x00000C01
#define CMD_CLOSE_GBAR         0x00000D01

#define CMD_STR_UPDATED           "updated"
#define CMD_STR_DBOX_UPDATED      "dbox_updated"
#define CMD_STR_DESC_UPDATED      "desc_updated"
#define CMD_STR_GBAR_UPDATED      "gbar_updated"
#define CMD_STR_EXTRA_UPDATED     "extra_updated"
#define CMD_STR_EXTRA_INFO        "extra_info"
#define CMD_STR_DELETED           "deleted"
#define CMD_STR_FAULTED           "faulted"
#define CMD_STR_FAULT_PACKAGE     "fault_package"
#define CMD_STR_SCROLL            "scroll"
#define CMD_STR_DBOX_UPDATE_BEGIN "dbox_update_begin"
#define CMD_STR_DBOX_UPDATE_END   "dbox_update_end"
#define CMD_STR_GBAR_UPDATE_BEGIN "gbar_update_begin"
#define CMD_STR_GBAR_UPDATE_END   "gbar_update_end"
#define CMD_STR_ACCESS_STATUS     "access_status"
#define CMD_STR_KEY_STATUS        "key_status"
#define CMD_STR_CLOSE_GBAR        "close_gbar"

/**
 * @internal
 * @brief Command list for
 *      Provder to Master.
 */
#define CMD_CALL               0x00000E01
#define CMD_RET                0x00000F01
#define CMD_ACQUIRE_BUFFER     0x00001001
#define CMD_RESIZE_BUFFER      0x00001101
#define CMD_RELEASE_BUFFER     0x00001201
#define CMD_HELLO              0x00001301
#define CMD_PING               0x00001401
#define CMD_CTRL               0x00001501
#define CMD_ACQUIRE_XBUFFER    0x00001601 // eXtra Buffer
#define CMD_RELEASE_XBUFFER    0x00001701 // eXtra Buffer

#define CMD_STR_CALL            "call"
#define CMD_STR_RET             "ret"
#define CMD_STR_ACQUIRE_BUFFER  "acquire_buffer"
#define CMD_STR_RESIZE_BUFFER   "resize_buffer"
#define CMD_STR_RELEASE_BUFFER  "release_buffer"
#define CMD_STR_HELLO           "hello"
#define CMD_STR_PING            "ping"
#define CMD_STR_CTRL            "ctrl"
#define CMD_STR_ACQUIRE_XBUFFER "acquire_xbuffer"
#define CMD_STR_RELEASE_XBUFFER "release_xbuffer"


/**
 * @internal
 * @brief Master to Viewer
 */
#define CMD_GBAR_CREATED       0x00000E01
#define CMD_GBAR_DESTROYED     0x00000F01
#define CMD_CREATED            0x00001001
#define CMD_GROUP_CHANGED      0x00001101
#define CMD_PERIOD_CHANGED     0x00001201
#define CMD_SIZE_CHANGED       0x00001301
#define CMD_RESULT_PINUP       0x00001401   /**< To Viewer */
#define CMD_RESULT_UPDATE_MODE 0x00001501   /**< To Viewer */
#define CMD_DBOX_CREATE_XBUF   0x00001601
#define CMD_GBAR_CREATE_XBUF   0x00001701
#define CMD_DBOX_DESTROY_XBUF  0x00001801
#define CMD_GBAR_DESTROY_XBUF  0x00001901
#define CMD_UPDATE_ID          0x00001A01

#define CMD_STR_GBAR_CREATED       "gbar_created"
#define CMD_STR_GBAR_DESTROYED     "gbar_destroyed"
#define CMD_STR_CREATED            "created"
#define CMD_STR_GROUP_CHANGED      "group_changed"
#define CMD_STR_PERIOD_CHANGED     "period_changed"
#define CMD_STR_SIZE_CHANGED       "size_changed"
#define CMD_STR_RESULT_PINUP       "pinup"
#define CMD_STR_RESULT_UPDATE_MODE "update_mode"
#define CMD_STR_DBOX_CREATE_XBUF   "dbox_create_xbuf"
#define CMD_STR_GBAR_CREATE_XBUF   "gbar_create_xbuf"
#define CMD_STR_DBOX_DESTROY_XBUF  "dbox_destroy_xbuf"
#define CMD_STR_GBAR_DESTROY_XBUF  "gbar_destroy_xbuf"
#define CMD_STR_UPDATE_ID          "update_id"

/**
 * @internal
 * @brief Following list should be started after the common list of client_cmd_list
 *        Master to Provider
 */
#define CMD_GBAR_SHOW      0x00003B01
#define CMD_GBAR_HIDE      0x00003C01
#define CMD_DBOX_PAUSE     0x00003D01
#define CMD_DBOX_RESUME    0x00003E01
#define CMD_SCRIPT         0x00003F01
#define CMD_RENEW          0x00004001
#define CMD_PINUP          0x00004101
#define CMD_UPDATE_CONTENT 0x00004201
#define CMD_PAUSE          0x00004301
#define CMD_RESUME         0x00004401
#define CMD_DISCONNECT     0x00004501

#define CMD_STR_GBAR_SHOW      "gbar_show"
#define CMD_STR_GBAR_HIDE      "gbar_hide"
#define CMD_STR_DBOX_PAUSE     "dbox_pause"
#define CMD_STR_DBOX_RESUME    "dbox_resume"
#define CMD_STR_SCRIPT         "script"
#define CMD_STR_RENEW          "renew"
#define CMD_STR_PINUP          "pinup"
#define CMD_STR_UPDATE_CONTENT "update_content"
#define CMD_STR_PAUSE          "pause"
#define CMD_STR_RESUME         "resume"
#define CMD_STR_DISCONNECT     "disconnect"

/**
 * @brief liveinfo to master
 */
#define CMD_INFO_HELLO         0x00000001
#define CMD_INFO_SLAVE_LIST    0x00000101
#define CMD_INFO_PKG_LIST      0x00000201
#define CMD_INFO_INST_LIST     0x00000301
#define CMD_INFO_SLAVE_CTRL    0x00000401
#define CMD_INFO_PKG_CTRL      0x00000501
#define CMD_INFO_MASTER_CTRL   0x00000601

#define CMD_STR_INFO_HELLO       "liveinfo_hello"
#define CMD_STR_INFO_SLAVE_LIST  "slave_list"
#define CMD_STR_INFO_PKG_LIST    "pkg_list"
#define CMD_STR_INFO_INST_LIST   "inst_list"
#define CMD_STR_INFO_SLAVE_CTRL  "slave_ctrl"
#define CMD_STR_INFO_PKG_CTRL    "pkg_ctrl"
#define CMD_STR_INFO_MASTER_CTRL "master_ctrl"

/**
 * @internal
 * @brief Viewer to maser
 *        or Master to provider
 */
#define CMD_GBAR_MOUSE_MOVE       0x00000001
#define CMD_DBOX_MOUSE_MOVE       0x00000101
#define CMD_GBAR_MOUSE_DOWN       0x00000201
#define CMD_GBAR_MOUSE_UP         0x00000301
#define CMD_DBOX_MOUSE_DOWN       0x00000401
#define CMD_DBOX_MOUSE_UP         0x00000501
#define CMD_GBAR_MOUSE_ENTER      0x00000601
#define CMD_GBAR_MOUSE_LEAVE      0x00000701
#define CMD_DBOX_MOUSE_ENTER      0x00000801
#define CMD_DBOX_MOUSE_LEAVE      0x00000901
#define CMD_DBOX_MOUSE_ON_SCROLL  0x00000A01
#define CMD_DBOX_MOUSE_OFF_SCROLL 0x00000B01
#define CMD_GBAR_MOUSE_ON_SCROLL  0x00000C01
#define CMD_GBAR_MOUSE_OFF_SCROLL 0x00000D01
#define CMD_DBOX_MOUSE_ON_HOLD    0x00000E01
#define CMD_DBOX_MOUSE_OFF_HOLD   0x00000F01
#define CMD_GBAR_MOUSE_ON_HOLD    0x00001001
#define CMD_GBAR_MOUSE_OFF_HOLD   0x00001101
#define CMD_CLICKED               0x00001201
#define CMD_TEXT_SIGNAL           0x00001301
#define CMD_DELETE                0x00001401
#define CMD_RESIZE                0x00001501
#define CMD_NEW                   0x00001601
#define CMD_SET_PERIOD            0x00001701
#define CMD_CHANGE_GROUP          0x00001801
#define CMD_GBAR_MOVE             0x00001901
#define CMD_GBAR_ACCESS_HL        0x00001A01
#define CMD_GBAR_ACCESS_ACTIVATE  0x00001B01
#define CMD_GBAR_ACCESS_ACTION    0x00001C01
#define CMD_GBAR_ACCESS_SCROLL    0x00001D01
#define CMD_GBAR_ACCESS_VALUE_CHANGE 0x00001E01
#define CMD_GBAR_ACCESS_MOUSE     0x00001F01
#define CMD_GBAR_ACCESS_BACK      0x00002001
#define CMD_GBAR_ACCESS_OVER      0x00002101
#define CMD_GBAR_ACCESS_READ      0x00002201
#define CMD_GBAR_ACCESS_ENABLE    0x00002301
#define CMD_DBOX_ACCESS_HL        0x00002401
#define CMD_DBOX_ACCESS_ACTIVATE  0x00002501
#define CMD_DBOX_ACCESS_ACTION    0x00002601
#define CMD_DBOX_ACCESS_SCROLL    0x00002701
#define CMD_DBOX_ACCESS_VALUE_CHANGE 0x00002801
#define CMD_DBOX_ACCESS_MOUSE     0x00002901
#define CMD_DBOX_ACCESS_BACK      0x00002A01
#define CMD_DBOX_ACCESS_OVER      0x00002B01
#define CMD_DBOX_ACCESS_READ      0x00002C01
#define CMD_DBOX_ACCESS_ENABLE    0x00002D01
#define CMD_DBOX_KEY_DOWN         0x00002E01
#define CMD_DBOX_KEY_UP           0x00002F01
#define CMD_DBOX_KEY_FOCUS_IN     0x00003001
#define CMD_DBOX_KEY_FOCUS_OUT    0x00003101
#define CMD_GBAR_KEY_DOWN         0x00003201
#define CMD_GBAR_KEY_UP           0x00003301
#define CMD_GBAR_KEY_FOCUS_IN     0x00003401
#define CMD_GBAR_KEY_FOCUS_OUT    0x00003501
#define CMD_UPDATE_MODE           0x00003601
#define CMD_DBOX_MOUSE_SET        0x00003701
#define CMD_DBOX_MOUSE_UNSET      0x00003801
#define CMD_GBAR_MOUSE_SET        0x00003901
#define CMD_GBAR_MOUSE_UNSET      0x00003A01

#define CMD_STR_GBAR_MOUSE_MOVE          "gbar_mouse_move"
#define CMD_STR_DBOX_MOUSE_MOVE          "dbox_mouse_move"
#define CMD_STR_GBAR_MOUSE_DOWN          "gbar_mouse_down"
#define CMD_STR_GBAR_MOUSE_UP            "gbar_mouse_up"
#define CMD_STR_DBOX_MOUSE_DOWN          "dbox_mouse_down"
#define CMD_STR_DBOX_MOUSE_UP            "dbox_mouse_up"
#define CMD_STR_GBAR_MOUSE_ENTER         "gbar_mouse_enter"
#define CMD_STR_GBAR_MOUSE_LEAVE         "gbar_mouse_leave"
#define CMD_STR_DBOX_MOUSE_ENTER         "dbox_mouse_enter"
#define CMD_STR_DBOX_MOUSE_LEAVE         "dbox_mouse_leave"
#define CMD_STR_DBOX_MOUSE_ON_SCROLL     "dbox_mouse_on_scroll"
#define CMD_STR_DBOX_MOUSE_OFF_SCROLL    "dbox_mouse_off_scroll"
#define CMD_STR_GBAR_MOUSE_ON_SCROLL     "gbar_mouse_on_scroll"
#define CMD_STR_GBAR_MOUSE_OFF_SCROLL    "gbar_mouse_off_scroll"
#define CMD_STR_DBOX_MOUSE_ON_HOLD       "dbox_mouse_on_hold"
#define CMD_STR_DBOX_MOUSE_OFF_HOLD      "dbox_mouse_off_hold"
#define CMD_STR_GBAR_MOUSE_ON_HOLD       "gbar_mouse_on_hold"
#define CMD_STR_GBAR_MOUSE_OFF_HOLD      "gbar_mouse_off_hold"
#define CMD_STR_CLICKED                  "clicked"
#define CMD_STR_TEXT_SIGNAL              "text_signal"
#define CMD_STR_DELETE                   "delete"
#define CMD_STR_RESIZE                   "resize"
#define CMD_STR_NEW                      "new"
#define CMD_STR_SET_PERIOD               "set_period"
#define CMD_STR_CHANGE_GROUP             "change_group"
#define CMD_STR_GBAR_MOVE                "gbar_move"
#define CMD_STR_GBAR_ACCESS_HL           "gbar_acc_hl"
#define CMD_STR_GBAR_ACCESS_ACTIVATE     "gbar_acc_activate"
#define CMD_STR_GBAR_ACCESS_ACTION       "gbar_acc_action"
#define CMD_STR_GBAR_ACCESS_SCROLL       "gbar_acc_scroll"
#define CMD_STR_GBAR_ACCESS_VALUE_CHANGE "gbar_acc_val_change"
#define CMD_STR_GBAR_ACCESS_MOUSE        "gbar_acc_mouse"
#define CMD_STR_GBAR_ACCESS_BACK         "gbar_acc_back"
#define CMD_STR_GBAR_ACCESS_OVER         "gbar_acc_over"
#define CMD_STR_GBAR_ACCESS_READ         "gbar_acc_read"
#define CMD_STR_GBAR_ACCESS_ENABLE       "gbar_acc_enable"
#define CMD_STR_DBOX_ACCESS_HL           "dbox_acc_hl"
#define CMD_STR_DBOX_ACCESS_ACTIVATE     "dbox_acc_activate"
#define CMD_STR_DBOX_ACCESS_ACTION       "dbox_acc_action"
#define CMD_STR_DBOX_ACCESS_SCROLL       "dbox_acc_scroll"
#define CMD_STR_DBOX_ACCESS_VALUE_CHANGE "dbox_acc_val_change"
#define CMD_STR_DBOX_ACCESS_MOUSE        "dbox_acc_mouse"
#define CMD_STR_DBOX_ACCESS_BACK         "dbox_acc_back"
#define CMD_STR_DBOX_ACCESS_OVER         "dbox_acc_over"
#define CMD_STR_DBOX_ACCESS_READ         "dbox_acc_read"
#define CMD_STR_DBOX_ACCESS_ENABLE       "dbox_acc_enable"
#define CMD_STR_DBOX_KEY_DOWN            "dbox_key_down"
#define CMD_STR_DBOX_KEY_UP              "dbox_key_up"
#define CMD_STR_DBOX_KEY_FOCUS_IN        "dbox_key_focus_in"
#define CMD_STR_DBOX_KEY_FOCUS_OUT       "dbox_key_focus_out"
#define CMD_STR_GBAR_KEY_DOWN            "gbar_key_down"
#define CMD_STR_GBAR_KEY_UP              "gbar_key_up"
#define CMD_STR_GBAR_KEY_FOCUS_IN        "gbar_key_focus_in"
#define CMD_STR_GBAR_KEY_FOCUS_OUT       "gbar_key_focus_out"
#define CMD_STR_UPDATE_MODE              "update_mode"
#define CMD_STR_DBOX_MOUSE_SET		 "dbox_mouse_set"
#define CMD_STR_DBOX_MOUSE_UNSET	 "dbox_mouse_unset"
#define CMD_STR_GBAR_MOUSE_SET		 "gbar_mouse_set"
#define CMD_STR_GBAR_MOUSE_UNSET	 "gbar_mouse_unset"

/**
 * @internal
 * @brief viewer to master
 */
#define CMD_CHANGE_VISIBILITY     0x00003B01
#define CMD_DBOX_ACQUIRE_PIXMAP   0x00003C01
#define CMD_DBOX_RELEASE_PIXMAP   0x00003D01
#define CMD_GBAR_ACQUIRE_PIXMAP   0x00003E01
#define CMD_GBAR_RELEASE_PIXMAP   0x00003F01
#define CMD_ACQUIRE               0x00004001
#define CMD_RELEASE               0x00004101
#define CMD_PINUP_CHANGED         0x00004201
#define CMD_CREATE_GBAR           0x00004301
#define CMD_DESTROY_GBAR          0x00004401
#define CMD_ACTIVATE_PACKAGE      0x00004501
#define CMD_SUBSCRIBE             0x00004601
#define CMD_UNSUBSCRIBE           0x00004701
#define CMD_DELETE_CLUSTER        0x00004801
#define CMD_DELETE_CATEGORY       0x00004901
#define CMD_REFRESH_GROUP         0x00004A01
#define CMD_UPDATE                0x00004B01
#define CMD_DBOX_KEY_SET          0x00004C01
#define CMD_DBOX_KEY_UNSET        0x00004D01
#define CMD_GBAR_KEY_SET          0x00004E01
#define CMD_GBAR_KEY_UNSET        0x00004F01
#define CMD_CLIENT_PAUSED         0x00005001
#define CMD_CLIENT_RESUMED        0x00005101
#define CMD_DBOX_ACQUIRE_XPIXMAP  0x00005201
#define CMD_GBAR_ACQUIRE_XPIXMAP  0x00005301
#define CMD_SUBSCRIBE_CATEGORY    0x00005401
#define CMD_UNSUBSCRIBE_CATEGORY  0x00005501

#define CMD_STR_CHANGE_VISIBILITY   "change,visibility"
#define CMD_STR_DBOX_ACQUIRE_PIXMAP "dbox_acquire_pixmap"
#define CMD_STR_DBOX_RELEASE_PIXMAP "dbox_release_pixmap"
#define CMD_STR_GBAR_ACQUIRE_PIXMAP "gbar_acquire_pixmap"
#define CMD_STR_GBAR_RELEASE_PIXMAP "gbar_release_pixmap"
#define CMD_STR_ACQUIRE             "acquire"
#define CMD_STR_RELEASE             "release"
#define CMD_STR_PINUP_CHANGED       "pinup_changed"
#define CMD_STR_CREATE_GBAR         "create_gbar"
#define CMD_STR_DESTROY_GBAR        "destroy_gbar"
#define CMD_STR_ACTIVATE_PACKAGE    "activate_package"
#define CMD_STR_SUBSCRIBE           "subscribe" /* pid, cluster, sub-cluster */
#define CMD_STR_UNSUBSCRIBE         "unsubscribe" /* pid, cluster, sub-cluster */
#define CMD_STR_DELETE_CLUSTER      "delete_cluster"
#define CMD_STR_DELETE_CATEGORY     "delete_category"
#define CMD_STR_REFRESH_GROUP       "refresh_group"
#define CMD_STR_UPDATE              "update"
#define CMD_STR_DBOX_KEY_SET        "dbox_key_set"
#define CMD_STR_DBOX_KEY_UNSET      "dbox_key_unset"
#define CMD_STR_GBAR_KEY_SET        "gbar_key_set"
#define CMD_STR_GBAR_KEY_UNSET      "gbar_key_unset"
#define CMD_STR_CLIENT_PAUSED       "client_paused"
#define CMD_STR_CLIENT_RESUMED      "client_resumed"
#define CMD_STR_DBOX_ACQUIRE_XPIXMAP "dbox_acquire_xpixmap"
#define CMD_STR_GBAR_ACQUIRE_XPIXMAP "gbar_acquire_xpixmap"
#define CMD_STR_SUBSCRIBE_CATEGORY   "subscribe,category"
#define CMD_STR_UNSUBSCRIBE_CATEGORY "unsubscribe,category"

/**
 * @internal
 * @brief Service API to Master
 */
#define CMD_SERVICE_UPDATE        0x00000001
#define CMD_SERVICE_CHANGE_PERIOD 0x00000101
#define CMD_SERVICE_INST_CNT      0x00000201

#define CMD_STR_SERVICE_UPDATE        "service_update"
#define CMD_STR_SERVICE_CHANGE_PERIOD "service_change_period"
#define CMD_STR_SERVICE_INST_CNT      "service_inst_cnt"

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

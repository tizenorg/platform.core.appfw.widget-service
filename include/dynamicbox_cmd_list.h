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
 */

/**
 * @addtogroup CAPI_DYNAMICBOX_SERVICE_MODULE
 * @{
 */

/**
 * @brief Command list for
 *        Provider to Master,
 *        or Master to Viewer
 */
#define CMD_UPDATED            0x00000001   /**< DBOX Updated */
#define CMD_DBOX_UPDATED       0x00000001   /**< Alias of CMD_UPDATED */
#define CMD_DESC_UPDATED       0x00000101   /**< GBAR Updated */
#define CMD_GBAR_UPDATED       0x00000101   /**< Alias of CMD_DESC_UPDATED */
#define CMD_EXTRA_INFO         0x00000201   /**< Extra information is updated */
#define CMD_DELETED            0x00000301   /**< DBox is deleted */
#define CMD_FAULTED            0x00000401   /**< DBox is faulted */
#define CMD_FAULT_PACKAGE      0x00000401   /**< Faulted package */
#define CMD_SCROLL             0x00000501
#define CMD_DBOX_UPDATE_BEGIN  0x00000601
#define CMD_DBOX_UPDATE_END    0x00000701
#define CMD_GBAR_UPDATE_BEGIN  0x00000801
#define CMD_GBAR_UPDATE_END    0x00000901
#define CMD_ACCESS_STATUS      0x00000A01
#define CMD_KEY_STATUS         0x00000B01
#define CMD_CLOSE_GBAR         0x00000C01

/**
 * @brief Command list for
 *	  Provder to Master.
 */
#define CMD_CALL               0x00000D01
#define CMD_RET                0x00000E01
#define CMD_ACQUIRE_BUFFER     0x00000F01
#define CMD_RESIZE_BUFFER      0x00001001
#define CMD_RELEASE_BUFFER     0x00001101
#define CMD_HELLO              0x00001201
#define CMD_PING               0x00001301
#define CMD_CTRL               0x00001401

/**
 * @brief Master to Viewer
 */
#define CMD_GBAR_CREATED       0x00000D01
#define CMD_GBAR_DESTROYED     0x00000E01
#define CMD_CREATED            0x00000F01
#define CMD_GROUP_CHANGED      0x00001001
#define CMD_PERIOD_CHANGED     0x00001101
#define CMD_SIZE_CHANGED       0x00001201
#define CMD_RESULT_PINUP       0x00001301   /**< To Viewer */
#define CMD_RESULT_UPDATE_MODE 0x00001401   /**< To Viewer */

/**
 * @brief Following list should be started after the common list of client_cmd_list
 *        Master to Viewer
 */
#define CMD_GBAR_SHOW          0x00003701
#define CMD_GBAR_HIDE          0x00003801
#define CMD_DBOX_PAUSE         0x00003901
#define CMD_DBOX_RESUME        0x00003A01
#define CMD_SCRIPT             0x00003B01
#define CMD_RENEW              0x00003C01
#define CMD_PINUP              0x00003D01
#define CMD_UPDATE_CONTENT     0x00003E01
#define CMD_PAUSE              0x00003F01
#define CMD_RESUME             0x00004001
#define CMD_DISCONNECT         0x00004101

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

/**
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

/**
 * @brief viewer to master
 */
#define CMD_DBOX_MOUSE_SET        0x00003701
#define CMD_DBOX_MOUSE_UNSET      0x00003801
#define CMD_GBAR_MOUSE_SET        0x00003901
#define CMD_GBAR_MOUSE_UNSET      0x00003A01
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

/**
 * @brief Service API to Master
 */
#define CMD_SERVICE_UPDATE        0x00000001
#define CMD_SERVICE_CHANGE_PERIOD 0x00000101
#define CMD_SERVICE_INST_CNT      0x00000201

#ifdef __cplusplus
}
#endif

#endif

/* End of a file */

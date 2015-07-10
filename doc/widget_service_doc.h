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

#ifndef __WIDGET_SERVICE_DOC_H__
#define __WIDGET_SERVICE_DOC_H__

/**
 * @defgroup CAPI_WIDGET_FRAMEWORK Widget
 * @ingroup CAPI_APPLICATION_FRAMEWORK
 */

/**
 * @defgroup CAPI_WIDGET_SERVICE_MODULE Widget Service
 * @brief To get the information of installed widgets
 * @ingroup CAPI_WIDGET_FRAMEWORK 
 * @section CAPI_WIDGET_SERVICE_MODULE_HEADER Required Header
 *   \#include <widget_service.h>
 *   \#include <widget_errno.h>
 * @section CAPI_WIDGET_SERVICE_MODULE_OVERVIEW Overview
<H1>1. widget service API</H1>
Tizen supports various kinds of API set for retrieving information of widgets.
Every widgets are installed via package manager utilities.
The information of installed widgets are stored in the system DB.

In some cases, application developer needs to send update request to some widgets.
To make it possible, this module provides related API set.
But the API will only works if the widget and application are packaged in one.

<H2>1.1 APIs for retrieving information of widgets</H2>

To get the information for installed widgets.

<H2>1.2 APIs for communicating with running widget instances</H2>

To trigger the update event or send the updated content string to the widget.

<H1>2. Size of widget</H1>

Widget has predefined size types.
1x1 is a square so its pixel size of width and height must has to be same.
2x2, 4x4 also sqaure.

2x1, 4x1, 4x2, 4x3 is a rectangle. so its pixel size of width & height should not be same.
pixel size of width should be long than height.

Pixel size can be differ between different devices.
It depends on its screen resolution.

So widget developer should care various kinds of devices (has different screen resolution).

<TABLE>
<TR>
<TD>Size Type</TD><TD>Size in pixels</TD><TD>Supported profile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_1x1</TD><TD> - (SQUARE, depends on screen resolution) </TD><TD>No profile - This size type is not used. just defined for specifying a default cell size</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_2x1</TD><TD> - (RECTANGLE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_2x2</TD><TD> - (SQUARE, depends on screen resolution) </TD><TD>Mobile, Wearable (Default size)</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x1</TD><TD> - (RECTANGLE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x2</TD><TD> - (RECTANGLE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x3</TD><TD> - (RECTANGLE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x4</TD><TD> - (SQUARE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
</TABLE>

Widget also works for Easy Mode.
For the Easy Mode, there are different size types.
Called WIDGET_SIZE_TYPE_EASY_NxM

<TABLE>
<TR>
<TD>Size Type</TD><TD>Size in pixels</TD><TD>Supported profile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_EASY_1x1</TD><TD> - (SQUARE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_EASY_3x1</TD><TD> - (RECTANGLE, depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_EASY_3x3</TD><TD> - (SQARE - depends on screen resolution) </TD><TD>Mobile</TD>
</TR>
</TABLE>

However the Easy Mode is not a mandatory requirements for viewer application development.
So you will not be able to create your widgets using Easy Mode size types.
But if you are developing the homescreen(or application which is related with widgets displaying),
You should care these types too.

 *
 */

#endif

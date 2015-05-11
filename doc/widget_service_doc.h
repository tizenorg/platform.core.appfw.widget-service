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
 * @defgroup WIDGET_SERVICE_MODULE widget-service
 * @brief To get the information of installed widgets
 * @ingroup CAPI_WIDGET_FRAMEWORK 
 * @section WIDGET_SERVICE_MODULE_HEADER Required Header
 *   \#include <widget_service.h>
 *   \#include <widget_errno.h>
 * @section WIDGET_SERVICE_MODULE_OVERVIEW Overview
<H1>1. widget service API</H1>
Tizen supports various kinds of API set for retrieving information of widgets.
Every widgets are installed via package manager utilities.
The information of installed widgets are stored in the system DB.

In some cases, application developer needs to send update request to its widget.
To make it possible, this module providers such kinds of API set.

Those APIs are for communicating with master service provider called data-provider-master.
When it receives request from applications, it will find the proper service provider called data-provider-slave or application widget Provider.
If master finds proper service provider, it will forward the requests to them.
Then each widget application can get update request.

<H2>1.1 APIs for retrieving information of widgets</H2>

To get the information for installed widgets.

<H2>1.2 APIs for communicating with running widget instances</H2>

To trigger the update event or send the updated content string to the widget.

<H1>2. Size of widget</H1>

widget has predefined size types.

<TABLE>
<TR>
<TD>Size Type</TD><TD>Size in pixels</TD><TD>Supported profile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_1x1</TD><TD> - </TD><TD>Mobile, Wearable - Mandatory size</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_2x1</TD><TD> - </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_2x2</TD><TD> - </TD><TD>Mobile, Wearable</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x1</TD><TD> - </TD><TD>Mobile - Extension</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x2</TD><TD> - </TD><TD>Mobile - Since 2.3</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x3</TD><TD> - </TD><TD>Mobile - Extension</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x4</TD><TD> - </TD><TD>Mobile, Kiran - Since 2.3</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x5</TD><TD> - </TD><TD>Mobile - Extension</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_4x6</TD><TD> - </TD><TD>Mobile - Extension</TD>
</TR>
</TABLE>

widget also handles Easy Mode.
For the easy mode, there are different size types.
Called WIDGET_SIZE_TYPE_EASY_NxM

<TABLE>
<TR>
<TD>Size Type</TD><TD>Size in pixels</TD><TD>Supported profile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_EASY_1x1</TD><TD> - </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_EASY_3x1</TD><TD> - </TD><TD>Mobile</TD>
</TR>
<TR>
<TD>WIDGET_SIZE_TYPE_EASY_3x3</TD><TD> - </TD><TD>Mobile</TD>
</TR>
</TABLE>

However the easy mode is not officially supported.
So you will not be able to create your widgets using easy mode size types.
If you are developing the homescreen(or application which is related with widgets),
You should care these types too.

 *
 */

#endif

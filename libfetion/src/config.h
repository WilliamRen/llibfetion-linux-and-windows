/***************************************************************************
 *   Copyright (C) 2010 by programmeboy                                    *
 *   programmeboy@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.                                        *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             * 
 ***************************************************************************/

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "commdef.h"
#include "mem.h"

#define FX_SYS_CONF_QUERY_URL "http://nav.fetion.com.cn/nav/getsystemconfig.aspx"

#define FX_LOGIN_TYPE_PHONE_NUM_CONFIG   "mobile-no"
#define FX_LOGIN_TYPE_SID				 "sid"
#define FX_LOGIN_TYPE_PHONE_NUM			 "mobileno"


#define FX_QUERY_SYS_CONF   "<config>"                                                      \
                            "<user %s=\"%s\" />"                                     \
                            "<client type=\"PC\" version=\"3.6.2000\" platform=\"W5.1\" />" \
                            "<servers version=\"0\" />"                                     \
                            "<service-no version=\"0\" />"                                  \
                            "<parameters version=\"0\" />"                                  \
                            "<hints version=\"0\" />"                                       \
                            "<http-applications version=\"0\" />"                           \
                            "<client-config version=\"0\" />"                               \
                            "</config>"

#define FX_SSI_FORMAT   "%s?%s=%s"                                                    \
                        "&domains=fetion.com.cn"											\
                        "&v4digest-type=1"                                                  \
                        "&v4digest=%s"

#define FX_SSI_FORMAT1   "%s?%s=%s"                                                    \
						"&domains=fetion.com.cn&pid=%s&pic=%s&algorithm=%s"					\
						"&v4digest-type=1"                                                  \
                        "&v4digest=%s"


#define FX_QUERY_PIC	"http://nav.fetion.com.cn/nav/GetPicCodeV4.aspx?algorithm=%s"
/*function definition*/

FX_RET_CODE fx_get_sys_conf( __in char* sz_phone_num, \
                             __in __out PMEM_STRUCT mem  );

FX_RET_CODE fx_get_user_conf( __in PSYS_CONF_DATA sys_data, \
                              __in __out PMEM_STRUCT mem );

FX_RET_CODE fx_get_verify_pic( __in PSYS_CONF_DATA sys_data, \
							  __in char* algorithm,
                              __in __out PMEM_STRUCT mem );

FX_RET_CODE fx_get_user_conf_ver( __in PSYS_CONF_DATA sys_data, \
								 __in char* sz_chid, __in char* sz_code, __in char* sz_a,
                              __in __out PMEM_STRUCT mem );

#endif // CONFIG_H_INCLUDED

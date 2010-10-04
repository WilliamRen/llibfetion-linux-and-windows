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

/*! \file commdef.h
 *  \brief 
 *  \author programmeboy programmebot@gmail.com
 *  \date 2,10,2010   22:50  
 */

#ifndef COMMDEF_H_INCLUDE
#define COMMDEF_H_INCLUDE

/*macro definition*/

#define __in
#define __out

#define FX_ERROR_OK         0
#define FX_ERROR_INIT       100
#define FX_ERROR_FILE       101
#define FX_ERROR_NOINITIAL  102
#define FX_ERROR_CURL       103
#define FX_ERROR_XMLPARSE   104
#define FX_ERROR_MEM        105
#define FX_ERROR_SOCKET     106
#define FX_ERROR_THREAD     107
#define FX_ERROR_UNKOWN     200

typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

#ifdef __WIN32__
typedef unsigned int uint;
#endif 

typedef int FX_RET_CODE;

enum make_type
{
	FX_BUILD_LOGIN_1,
	FX_BUILD_LOGIN_2,
	FX_BUILD_GET_PERSON_INFO,
	FX_BUILD_GET_CONTECT,
	FX_BUILD_SEND_MSG,
	FX_BUILD_SEND_TO_SELF
};

typedef struct _user_data
{
	char sz_phone_num[20];
	char sz_password[20];
	
}USER_DATA, *PUSER_DATA;

typedef struct _sys_conf_data
{
	USER_DATA user_data;
    char sz_user_conf_url[256];
    char sz_sipc_proxy[100];
	
}SYS_CONF_DATA, *PSYS_CONF_DATA;

typedef struct _login_data
{
    int nstatu_code;
    int  nstatu_user;
    char sz_uri[50];
    char sz_phone_num[20];
    char sz_user_id[20];
	
}LOGIN_DATA, *PLOGIN_DATA;

typedef struct _auth_dlg_helper
{
	int n_callid;
	int n_cseq;
	char phone_num[20];
	char uri[50];
	char user_id[20];
	char user_pwd[20];
	char machine_code[33];

}AUTH_DLG_HELPER, *PAUTH_DLG_HELPER;

typedef struct _chat_dlg_helper
{
	int n_callid;
	int n_cseq;
	char user_id[20];
	char user_sip[32];
	
}CHAT_DLG_HELPER, *PCHAT_DLG_HELPER;

#endif
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
#pragma warning( disable : 4996 )
#pragma warning( disable : 4005 )
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
    char sz_uri[20];
	char sz_uri_full[50];
    char sz_phone_num[20];
    char sz_user_id[20];
	
}LOGIN_DATA, *PLOGIN_DATA;

typedef struct _USER_STATUS
{
	char	sz_uri[50];
	char	sz_sid[20];
	char	sz_nick_name[50];
	char	sz_impresa[256];
	char	sz_phone_num[20];
	int		portrait;
	struct _presence
	{
		int  n_base;
		char desc[64];
		char device_type[16];
		char device_caps[16];

	}PRESENCE;

}USER_STATUS,*PUSER_STATUS;

typedef struct _CONTACT_LIST
{
	int		id;
	char	sz_uri[50];
	char	sz_user_id[20];
	char	sz_local_name[50];
	int		n_group_list_id;
	int		n_relation_status;
	int		n_feike_read_version;
	int		n_online_notify;
	USER_STATUS user_status;
	struct _CONTACT_LIST* next;

}CONTACT_LIST,*PCONTACT_LIST;

typedef struct _GROUP_LIST
{
	int		n_group_id;
	char	sz_group_name[50];
	PCONTACT_LIST p_contact;	
	struct _GROUP_LIST*	next;

}GROUP_LIST, *PGROUP_LIST;

#endif
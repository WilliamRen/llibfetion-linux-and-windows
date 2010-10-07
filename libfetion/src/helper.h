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

#ifndef HELPER_H_INCLUDE
#define HELPER_H_INCLUDE

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
	char my_uri[20];
	char dst_uri[50];
	
}CHAT_DLG_HELPER, *PCHAT_DLG_HELPER;

typedef struct _chat_dlg_helper_list
{
	PAUTH_DLG_HELPER p_helper;
	struct _chat_dlg_helper_list* next;
	
}CHAT_DLG_HELPER_LIST, *PCHAT_DLG_HELPER_LIST;

typedef struct _keeplive_dlg_helper
{
	int n_callid;
	int n_cseq;
	char uri[50];
	
}KEEPLIVE_DLG_HELPER, *PKEEPLIVE_DLG_HELPER;

typedef struct _dlg_helper
{
	PAUTH_DLG_HELPER p_auth;
	PCHAT_DLG_HELPER_LIST p_chat_list;
}DLG_HELPER, *PDLG_HELPER;

#endif
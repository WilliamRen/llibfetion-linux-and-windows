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

#ifndef SIPC_H_INCLUDE
#define SIPC_H_INCLUDE

#include "../src/sip/SIPC-4/libsip/include/sip.h"

#define MAX_SIP_RECV_LENGTH		1024
#define FETION_DOMAIN			"fetion.com.cn"

typedef struct _SIPC_MESG
{
	sip_message_t* msg;
	struct _SIPC_MESG * next;

}SIPC_MSG, *PSIPC_MSG;

/*#define SIP_AUTH_BODY	"<args>" \
						"<device accept-language=\"default\" machine-code=\"%s\" />" \
						"<caps value=\"3FF\" />" \
						"<events value=\"7F\" />" \
						"<user-info mobile-no=\"%s\" user-id=\"%s\">" \
						"<personal version=\"0\" attributes=\"v4default;alv2-version;alv2-warn\" />"\
						"<custom-config version=\"0\" />"\
						"<contact-list version=\"0\" buddy-attributes=\"v4default\" />" \
						"</user-info>" \
						"<credentials domains=\"fetion.com.cn;m161.com.cn;www.ikuwa.cn;games.fetion.com.cn;turn.fetion.com.cn\" />" \
						"<presence><basic value=\"400\" desc=\"\" /><extendeds /></presence></args>"*/
#define SIP_AUTH_BODY "<args><device accept-language=\"default\" machine-code=\"%s\" /><caps value=\"3FF\" /><events value=\"7F\" /><user-info mobile-no=\"%s\" user-id=\"%s\"><personal version=\"0\" attributes=\"v4default;alv2-version;alv2-warn\" /><custom-config version=\"0\" /><contact-list version=\"0\" buddy-attributes=\"v4default\" /></user-info><credentials domains=\"fetion.com.cn;m161.com.cn;www.ikuwa.cn;games.fetion.com.cn;turn.fetion.com.cn\" /><presence><basic value=\"400\" desc=\"\" /><extendeds /></presence></args>"

void fx_sip_msg_append( PSIPC_MSG msglist, PSIPC_MSG msg );
FX_RET_CODE fx_sip_recv( int sock, PSIPC_MSG* msg_list_out );
void fx_sip_loop_print( PSIPC_MSG msg_list );
int fx_sip_get_callid();
int fx_sip_increase_callid();
void fx_sip_msg_list_free( PSIPC_MSG msg_list );
int fx_sip_generate_auth_req( __in PAUTH_DLG_HELPER p_auth_helper, __out char** auth_req);
int fx_sip_generate_auth_resp( __in PAUTH_DLG_HELPER p_auth_helper,
							   __in char* key, __in char* nonce,
							   __out char** auth_req);

#endif
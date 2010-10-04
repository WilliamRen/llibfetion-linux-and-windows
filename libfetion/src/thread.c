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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>

#ifdef __WIN32__
#include <windows.h>
#endif

#include "commdef.h"
#include "sipc.h"
#include "log.h"
#include "crypto.h"
#include "protocol.h"
#include "fxsocket.h"
#include "thread.h"


#define LOGIN_STEP2     "R fetion.com.cn SIP-C/4.0\r\n" \
	"F: 879534138\r\n" \
	"I: 1\r\n" \
	"Q: 2 R\r\n" \
	"A: Digest algorithm=\"SHA1-sess-v4\",response=\"%s\"\r\n" \
	"L: 527\r\n\r\n" \
	"<args><device accept-language=\"default\" machine-code=\"2F6E7CD33AA1F6928E69DEDD7D6C50B1\" /><caps value=\"3FF\" /><events value=\"7F\" /><user-info mobile-no=\"15210281153\" user-id=\"639717376\"><personal version=\"0\" attributes=\"v4default;alv2-version;alv2-warn\" /><custom-config version=\"0\" /><contact-list version=\"0\" buddy-attributes=\"v4default\" /></user-info><credentials domains=\"fetion.com.cn;m161.com.cn;www.ikuwa.cn;games.fetion.com.cn;turn.fetion.com.cn\" /><presence><basic value=\"400\" desc=\"\" /><extendeds /></presence></args>"

extern SYS_CONF_DATA g_sys_conf;
extern PLOGIN_DATA g_login_data;

void* thread_sip_recv( void* lparam )
{
	int socket = (int)lparam;
	fd_set fd_read;
	int ret = 0;
	PSIPC_MSG msg_list = NULL;
	PSIPC_MSG p_msg_tmp = NULL;
	
	while ( 1 )
	{
		FD_ZERO( &fd_read );
		FD_SET( (uint)socket, &fd_read );
		ret = select( socket+1, &fd_read, NULL, NULL, NULL );
		if (ret == -1 || ret == 0) {
			log_string( "Error.. to read socket" );
			return NULL;
		}
		if ( !FD_ISSET(socket, &fd_read) ) {
#ifdef __WIN32__
			Sleep( 100 );
#else
			sleep( 1 );
#endif
			continue;
		}
		
		fx_sip_recv( socket, &msg_list );
#ifdef _DEBUG
		fx_sip_loop_print( msg_list );
#endif
		p_msg_tmp = msg_list;
		while ( p_msg_tmp != NULL )
		{
			if ( strcmp( p_msg_tmp->msg->startline->status_desc, "Unauthoried" ) == 0 )
			{
				char* sz_nonce = NULL, *sz_key = NULL, *sz_RSA = NULL;
				char sz_response[1024] = {0};
				int n_ret = 0;
				
				FX_RET_CODE ret = FX_ERROR_OK;
				
				char* sz_test = NULL;
				sip_message_to_str( p_msg_tmp->msg, &sz_test );
				
				sz_nonce = fx_get_nonce( sz_test );
				sz_key = fx_get_key( sz_test );
				
				ret = fx_generate_response( sz_key, sz_nonce, g_login_data->sz_user_id, \
					g_sys_conf.user_data.sz_password, &sz_RSA );
				
				sprintf( sz_response, LOGIN_STEP2, sz_RSA );
				
				log_string( "len = %d:%s", strlen( sz_response ), sz_response );
				
				n_ret = fx_socket_send( socket, sz_response, strlen(sz_response) );
				if ( n_ret == -1 ){
					log_string( "fx_login:send data to server error!" );
					return NULL;
				}
				
				sip_free( sz_test );
				free( sz_RSA );
				free( sz_nonce );
				free( sz_key );
			}
			p_msg_tmp = p_msg_tmp->next;
		}
	}
}

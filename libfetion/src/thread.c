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

#ifdef __WIN32__
#include <windows.h>
#include <winsock2.h>
#endif

#include "commdef.h"
#include "sipc.h"
#include "log.h"
#include "crypto.h"
#include "protocol.h"
#include "fxsocket.h"
#include "process.h"
#include "thread.h"

extern SYS_CONF_DATA g_sys_conf;
extern LOGIN_DATA g_login_data;

/** \fn void* thread_recv( void* lparam )
  * \brief the thread for recv data
  * \param lparam parameter
  * \return NULL
  */
//#ifdef __WIN32__
//DWORD WINAPI thread_sip_recv( void* lparam )
//#else
void* thread_sip_recv( void* lparam )
//#endif
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
			Sleep( 1000 );
#else
			sleep( 1 );
#endif
			continue;
		}

		fx_sip_recv( socket, &msg_list );
#ifdef __WIN32__
		Sleep( 50 );
#endif
#ifdef _DEBUG
		/*fx_sip_loop_print( msg_list );*/
#endif
		p_msg_tmp = msg_list;
		while ( p_msg_tmp != NULL )
		{
			dispatch_sip_recv( p_msg_tmp->msg );
			p_msg_tmp = p_msg_tmp->next;
		}

		fx_sip_msg_list_free( msg_list );
		msg_list = NULL;
	}
	return 0;
}
//#ifdef __WIN32__
//DWORD WINAPI thread_sip_keeplive( void* lparam )
//#else
void* thread_sip_keeplive( void* lparam )
//#endif
{
	int socket = (int)lparam;

	while( 1 )
	{
		char* sz_keeplive = NULL;
		KEEPLIVE_DLG_HELPER kp_help;
		int n_ret = 0;


		/*
		 *	init helper
		 */

		kp_help.n_callid = 1;
		kp_help.n_cseq = 1;
		strcpy( kp_help.uri, g_login_data.sz_uri );

		if ( fx_sip_generate_keeplive( &kp_help, &sz_keeplive ) != FX_ERROR_OK )
		{
			return NULL;
		}

		/*
		 *	send the package
		 */

		log_string( "==in thread_sip_keeplive fx_socket_send==" );
		n_ret = fx_socket_send( socket, sz_keeplive, strlen(sz_keeplive) );
		if ( n_ret == -1 ){
			log_string( "fx_login:send data to server error!" );
			return NULL;
		}

		free( sz_keeplive );

#ifdef __WIN32__
		Sleep( 200 * 1000 );
#else
		sleep( 200 );
#endif
	}
	return 0;
}

void* thread_sip_keep_connection_busy( void* lparam )
{
	int socket = (int)lparam;

	while( 1 )
	{
		char* sz_keeplive = NULL;
		KEEPLIVE_DLG_HELPER kp_help;
		int n_ret = 0;


		/*
		 *	init helper
		 */

		kp_help.n_callid = fx_sip_increase_callid();
		kp_help.n_cseq = 1;
		strcpy( kp_help.uri, g_login_data.sz_uri );

		if ( fx_sip_generate_keep_connection_busy( &kp_help, &sz_keeplive ) != FX_ERROR_OK )
		{
			return NULL;
		}

		/*
		 *	send the package
		 */

		log_string( "==in thread_sip_keeplive fx_socket_send==" );
		n_ret = fx_socket_send( socket, sz_keeplive, strlen(sz_keeplive) );
		if ( n_ret == -1 ){
			log_string( "fx_login:send data to server error!" );
			return NULL;
		}

		free( sz_keeplive );

#ifdef __WIN32__
		Sleep( 1000 );
#else
		sleep( 200 );
#endif
	}
	return 0;
}

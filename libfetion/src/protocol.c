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

/*! \file protocol.c
 *  \brief 里面封装了sip协议
 */

#include <stdio.h>
#include <time.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/rsa.h>

#include "commdef.h"
#include "config.h"
#include "fxsocket.h"
#include "login.h"
#include "sipc.h"
#include "utf8.h"
#include "log.h"
#include "protocol.h"

#define FX_MAX_PACKAGE_SIZE 1024

extern LOGIN_DATA g_login_data;

char* fx_generate_cnonce()
{
	char* sz_conce = (char*)malloc( 33 );
	srand( (unsigned)time( NULL ) );
	memset( sz_conce, 0, 33 );
	sprintf( sz_conce, "%04X%04X%04X%04X%04X%04X%04X%04X", \
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF );
	return sz_conce;
}

/** \fn 
	\brief 
	\param 
	\return 
*/

FX_RET_CODE fx_send_msg_to_yourself( int socket, char* msg )
{
	
	CHAT_DLG_HELPER dlg_helper = {0};
	char* sz_sip_msg = NULL;
	FX_RET_CODE n_ret = 0;

	/*
	 *	first we should convert the ascii to utf8
	 *  and in linux it is not necessary
	 */
	
	char* sz_convert_msg = NULL;

#ifdef __WIN32__
	sz_convert_msg = ansi_to_utf8( msg );
#else
	sz_convert_msg = msg;
#endif
	
	/*
	 *  init helper	
	 */
	
	strcpy( dlg_helper.dst_uri, g_login_data.sz_uri_full );
	strcpy( dlg_helper.my_uri, g_login_data.sz_uri );
	dlg_helper.n_callid = fx_sip_increase_callid();
	dlg_helper.n_cseq = 1;

	/*
	 *	generate the sip package
	 */
	
	n_ret = fx_sip_generate_send_msg_yourself( &dlg_helper, sz_convert_msg, &sz_sip_msg );
	if ( FX_ERROR_OK != n_ret )
	{
		log_string( "fx_sip_generate_send_msg_yourself error\n" );
#ifdef __WIN32__
		free( sz_convert_msg );
#endif
		return n_ret;
	}
	
	/*
	 *	send the package
	 */
	
	n_ret = fx_socket_send( socket, sz_sip_msg, strlen(sz_sip_msg) );
	if ( n_ret == -1 ){
		log_string( "fx_login:send data to server error!" );
		return FX_ERROR_SOCKET;
	}
	
	
	/*
	 *	free resource
	 */
	
	free( sz_sip_msg );
#ifdef __WIN32__
	free( sz_convert_msg );
#endif
	return FX_ERROR_OK;
}

FX_RET_CODE fx_get_buddies_status( int socket )
{
	char* sz_sip_msg = NULL;
	FX_RET_CODE n_ret = 0;
	
	SUB_DLG_HELPER sub_helper = {0};

	sub_helper.n_callid = fx_sip_increase_callid();
	sub_helper.n_cseq = 1;
	strcpy( sub_helper.uri, g_login_data.sz_uri );
	
	/*
	 *	generate the sip package
	 */
	
	n_ret = fx_sip_generate_get_user_status( &sub_helper, &sz_sip_msg );
	if ( FX_ERROR_OK != n_ret )
	{
		log_string( "fx_sip_generate_send_msg_yourself error\n" );
		return n_ret;
	}

	n_ret = fx_socket_send( socket, sz_sip_msg, strlen(sz_sip_msg) );
	if ( n_ret == -1 ){
		log_string( "fx_login:send data to server error!" );
		return FX_ERROR_SOCKET;
	}
	
	
	/*
	 *	free resource
	 */
	
	free( sz_sip_msg );

	return FX_ERROR_OK;
}
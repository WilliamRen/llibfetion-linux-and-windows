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
#include <winsock2.h>
#else
#define SOCKET_ERROR -1
#endif

#include "commdef.h"
#include "fxsocket.h"
#include "log.h"
#include "protocol.h"
#include "crypto.h"
#include "helper.h"
#include "sipc.h"

//#ifdef __WIN32__
//#pragma comment( lib, "../src/sip/SIPC-4/vc6/LIB_Debug/libsipc4d.lib" )
//#endif

int call_id = 1;

int fx_sip_get_callid()
{
	return call_id;
}
int fx_sip_increase_callid()
{
	call_id ++;
	return call_id;
}

void fx_sip_msg_list_free( PSIPC_MSG msg_list )
{
	PSIPC_MSG pos = msg_list;
	while ( pos != NULL )
	{
		PSIPC_MSG p_temp = pos->next;
		sip_message_free( pos->msg );
		free( pos );
		pos = p_temp;
	}
}

void fx_sip_msg_append( PSIPC_MSG msglist, PSIPC_MSG msg )
{
	PSIPC_MSG pos = msglist;
	while(pos != NULL)
	{
		if(pos->next == NULL)
		{
			pos->next = msg;
			break;
		}
		pos = pos->next;
	}
}

void fx_sip_loop_print( PSIPC_MSG msg_list )
{
	PSIPC_MSG pos = msg_list;
	log_string( "====================log_loop_start========================" );
	while(pos != NULL)
	{
		char* sz_tmp = NULL;
		sip_message_to_str( pos->msg, &sz_tmp );

		printf( "%s\n", sz_tmp );
		log_string( sz_tmp );

		sip_free( sz_tmp );
		pos = pos->next;
	}
	log_string( "====================log_loop_end========================" );
}

FX_RET_CODE fx_sip_recv( int sock, PSIPC_MSG* msg_list_out )
{

	/*
	 *	有时候服务器可能发送两个sip包.所以我们必须处理这种情况
	 *	这里一共有这几种情况
	 *	一次收到1个包.全
	 *  一次收到1个包.并且这个包不全
	 *  一次收到两个包.全
	 *  一次收到两个包.且第二个不全
	 *  一次收到多个包.全.
	 *  一次收到多个包.不全.
	 *
	 */

	char* sz_recv = (char*)malloc( MAX_SIP_RECV_LENGTH + 1 );
	int n_ret = 0, n_sip_ret = LIBSIP_SUCCESS, n_body = 0;
	PSIPC_MSG msg_list = NULL;
	char* sz_tmp_recv = NULL;

	/*
	 *	init return
	 */

	*msg_list_out = NULL;

	memset( sz_recv, 0, MAX_SIP_RECV_LENGTH + 1 );

	/*
	 *	recv buffer
	 */

	n_ret = fx_socket_recv( sock, sz_recv, MAX_SIP_RECV_LENGTH );
	if ( n_ret == SOCKET_ERROR ){
		log_string( "recv error!" );
		return FX_ERROR_SOCKET;
	}

	sz_tmp_recv = sz_recv;

	while ( 1 )
	{
		sip_message_t* p_msg = NULL;
		msg_list = (PSIPC_MSG)malloc( sizeof( SIPC_MSG ) );
		msg_list->msg = NULL;
		msg_list->next = NULL;

		sip_message_init( &p_msg );
		n_sip_ret = sip_message_parse( p_msg, sz_tmp_recv );

		if ( n_ret == MAX_SIP_RECV_LENGTH &&
			 ( n_sip_ret == LIBSIP_NOT_FOUND_HEAD_END || \
			   n_sip_ret == LIBSIP_BODY_NOT_COMPLETE) ){

			/*
			 *	recv the last buffer
			 */

			char sz_tmp[MAX_SIP_RECV_LENGTH + 1] = {0};
			char* sz_tmp_buf = NULL;
			int n_len = 0;

			n_ret = fx_socket_recv( sock, sz_tmp, MAX_SIP_RECV_LENGTH );
			if ( n_ret == SOCKET_ERROR ){
				log_string( "recv error!" );
				return FX_ERROR_SOCKET;
			}

			/*
			 *	add the buffer to the tail
			 */

			n_len = strlen( sz_tmp_recv ) + n_ret + 1;
			sz_tmp_buf = (char*)malloc( n_len );
			memset( sz_tmp_buf, 0, n_len );
			strcpy( sz_tmp_buf, sz_tmp_recv );
			strcat( sz_tmp_buf, sz_tmp );

			/*
			 *	no use free it
			 */

			free( sz_tmp_recv );
			free( msg_list );

			sz_tmp_recv = sz_tmp_buf;

			sip_message_free( p_msg );
			continue;
		}

		/*
		 *	invalid sip package
		 */

		if ( n_ret <= MAX_SIP_RECV_LENGTH && \
			 (n_sip_ret == LIBSIP_NOT_FOUND_HEAD_END || \
			  n_sip_ret == LIBSIP_BODY_NOT_COMPLETE ) )
		{
			free( sz_tmp_recv );
			free( msg_list );
			sip_message_free( p_msg );
			break;
		}

		/*
		 *	the package is ok
		 */

		if ( LIBSIP_IS_SUCCESS( n_sip_ret ) ){

			/*
			 *	add the msg to list tail
			 */

			msg_list->msg = p_msg;
			if ( *msg_list_out == NULL ){
				*msg_list_out = msg_list;
			}else{
				fx_sip_msg_append( *msg_list_out, msg_list );
			}

			/*
			 *	check the n_sip_ret
			 */

			if ( n_sip_ret == LIBSIP_SUCCESS ){

				/*
				 *	just return
				 */

				free( sz_tmp_recv );

				log_string( "==success==" );

				break;

			}else if ( n_sip_ret == LIBSIP_BODY_BEYOND ){

				/*
				 *	have the next message
				 */


				/*
				 *	first we shuld get the first message's hole length
				 */

				int n_head_len  = 0, n_body_len = 0, n_next = 0;
				char* sz_ctrllflf = NULL, *sz_next_msg = NULL;


				log_string( "=LIBSIP_BODY_BEYOND=" );

				/*
				 *	这里肯定能得到,不然sip_message_parse就错了
				 */

				sz_ctrllflf = strstr( sz_tmp_recv, CTRLLFLF );
				n_head_len = strlen( sz_tmp_recv ) - strlen( sz_ctrllflf ) + 4/*for CTRLLFLF*/;

				n_body_len = sip_message_get_body_length( p_msg );

				n_next = strlen( sz_tmp_recv ) - n_head_len - n_body_len;
				sz_next_msg = (char*)malloc( n_next + 1 );
				memset( sz_next_msg, 0, n_next + 1 );

				/*
				 *	set the next msg buffer
				 */

				strcpy( sz_next_msg, sz_tmp_recv + n_head_len + n_body_len );

				free( sz_tmp_recv );


				/*
				 *	set sz_tmp_recv for do next
				 */

				sz_tmp_recv = sz_next_msg;
				continue;

			}
		}else
			break;

	}
	return FX_ERROR_OK;
}

int fx_sip_generate_auth_req( __in PAUTH_DLG_HELPER p_auth_helper, __out char** auth_req)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_cnonce_t* cnonce = NULL;
	sip_client_t*	client = NULL;
	sip_message_t* message = NULL;
	char sz_num[10] = {0};
	char* sz_tmp = NULL;
	int n_ret = 0;


	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "R", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_auth_helper->uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */
#ifdef __WIN32__
	itoa( p_auth_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_auth_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_auth_helper->n_cseq, "R" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	cnonce
	 */

	sz_tmp = fx_generate_cnonce();
	sip_common_init( &cnonce );
	sip_common_set_all( cnonce, sz_tmp );
	free( sz_tmp );
	sip_message_set_common( &(message->cnonce), cnonce );

	/*
	 *	client
	 */

	sip_client_init( &client );
	sip_client_set_all( client, "pc", "4.0.2510" );
	sip_message_set_client( message, client );

	n_ret = sip_message_to_str( message, auth_req );

	sip_message_free( message );

	/*
	 *	add the sceq
	 */

	p_auth_helper->n_cseq++;

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;

}

int fx_sip_generate_auth_resp( __in PAUTH_DLG_HELPER p_auth_helper,
							   __in char* key, __in char* nonce,
							   __out char** auth_req)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_authorization_t* authorization = NULL;
	sip_context_length_t* context_len = NULL;
	sip_message_t* message = NULL;
	char* sz_response = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;
	char sz_body[1024] = {0}, sz_num[10] = {0};

	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "R", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_auth_helper->uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

#ifdef __WIN32__
	itoa( p_auth_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_auth_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_auth_helper->n_cseq, "R" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	authorization
	 */

		/*
		 *	get response
		 */

	n_ret = fx_generate_response( key, nonce, p_auth_helper->user_id, \
								p_auth_helper->user_pwd, &sz_response );
	if ( n_ret != FX_ERROR_OK )
	{
		sip_message_free( message );
		return n_ret;
	}

		/*
		 *	do generate authorization
		 */

	sip_authorization_init( &authorization );
	sip_authorization_set_digest_all( authorization, sz_response, "SHA1-sess-v4" );
	sip_message_set_authorization( message, authorization );


	sprintf( sz_body, SIP_AUTH_BODY, p_auth_helper->machine_code, \
				p_auth_helper->phone_num, p_auth_helper->user_id );
	//strcpy( sz_body, SIP_AUTH_BODY );

	/*
	 *	context length
	 */

#ifdef __WIN32__
	itoa( strlen( sz_body ), sz_num, 10 );
#else
    sprintf( sz_num, "%d", strlen( sz_body) );
#endif

	sip_common_init( &context_len );
	sip_common_set_all( context_len, sz_num );
	sip_message_set_common( &(message->context_len), context_len );

	/*
	 *	body
	 */

	sip_message_set_body( message, sz_body );

	n_ret = sip_message_to_str( message, auth_req );

	sip_message_free( message );

	/*
	 *	add the sceq
	 */

	p_auth_helper->n_cseq++;

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;
}


int fx_sip_generate_keep_connection_busy( __in PKEEPLIVE_DLG_HELPER p_keeplive_helper, __out char** auth_req)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_event_t* event = NULL;
	sip_context_length_t* context_len = NULL;
	sip_message_t* message = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;
	char sz_num[10] = {0};


	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "O", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_keeplive_helper->uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */
#ifdef __WIN32__
	itoa( p_keeplive_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_keeplive_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_keeplive_helper->n_cseq, "O" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	event
	 */

	sip_common_init( &event );
	sip_common_set_all( event, "KeepConnectionBusy" );
	sip_message_set_common( &(message->event), event );

	n_ret = sip_message_to_str( message, auth_req );

	sip_message_free( message );

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;

}

int fx_sip_generate_keeplive( __in PKEEPLIVE_DLG_HELPER p_keeplive_helper, __out char** auth_req)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_event_t* event = NULL;
	sip_context_length_t* context_len = NULL;
	sip_message_t* message = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;
	char sz_num[10] = {0};


	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "R", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_keeplive_helper->uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

#ifdef __WIN32__
	itoa( p_keeplive_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_keeplive_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_keeplive_helper->n_cseq, "R" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	event
	 */

	sip_common_init( &event );
	sip_common_set_all( event, "KeepAlive" );
	sip_message_set_common( &(message->event), event );

	/*
	 *	context length
	 */

#ifdef __WIN32__
	itoa( strlen( SIP_KEEPLIVE_BODY ), sz_num, 10 );
#else
    sprintf( sz_num, "%d", strlen( SIP_KEEPLIVE_BODY ) );
#endif
	sip_common_init( &context_len );
	sip_common_set_all( context_len, sz_num );
	sip_message_set_common( &(message->context_len), context_len );

	/*
	 *	body
	 */


	sip_message_set_body( message, SIP_KEEPLIVE_BODY );

	n_ret = sip_message_to_str( message, auth_req );

	sip_message_free( message );

	/*
	 *	add the sceq
	 */

	p_keeplive_helper->n_cseq++;

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;

}

int fx_sip_generate_send_msg_yourself( __in PCHAT_DLG_HELPER p_helper, __in char* msg,
									   __out char** sip_msg)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_event_t* event = NULL;
	sip_context_length_t* context_len = NULL;
	sip_message_t* message = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;
	char sz_num[10] = {0};

	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "M", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_helper->my_uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

#ifdef __WIN32__
	itoa( p_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_helper->n_cseq, "M" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	To
	 */

	sip_message_set_to_str( message, p_helper->dst_uri );

	/*
	 *	event
	 */

	sip_common_init( &event );
	sip_common_set_all( event, "SendCatSMS" );
	sip_message_set_common( &(message->event), event );

	/*
	 *	context length
	 */

#ifdef __WIN32__
	itoa( strlen( msg ), sz_num, 10 );
#else
    sprintf( sz_num, "%d", strlen( msg ) );
#endif
	sip_common_init( &context_len );
	sip_common_set_all( context_len, sz_num );
	sip_message_set_common( &(message->context_len), context_len );

	/*
	 *	body
	 */


	sip_message_set_body( message, msg );

	n_ret = sip_message_to_str( message, sip_msg );

	sip_message_free( message );

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;

}

int fx_sip_generate_get_user_status( __in PSUB_DLG_HELPER p_helper, __out char** sip_msg )
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_event_t* event = NULL;
	sip_context_length_t* context_len = NULL;
	sip_message_t* message = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;
	char sz_num[10] = {0};

	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "SUB", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_helper->uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

#ifdef __WIN32__
	itoa( p_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, 1, "SUB" );
	sip_message_set_cseq( message, cseq );


	/*
	 *	event
	 */

	sip_common_init( &event );
	sip_common_set_all( event, "PresenceV4" );
	sip_message_set_common( &(message->event), event );

	/*
	 *	context length
	 */

#ifdef __WIN32__
	itoa( strlen( SIP_USER_STATUS_BODY ), sz_num, 10 );
#else
    sprintf( sz_num, "%d", strlen( SIP_USER_STATUS_BODY ) );
#endif
	sip_common_init( &context_len );
	sip_common_set_all( context_len, sz_num );
	sip_message_set_common( &(message->context_len), context_len );

	/*
	 *	body
	 */


	sip_message_set_body( message, SIP_USER_STATUS_BODY );

	n_ret = sip_message_to_str( message, sip_msg );

	sip_message_free( message );

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;
}


int fx_sip_generate_send_msg_other(	   __in PCHAT_DLG_HELPER p_helper, __in char* msg,
									   __out char** sip_msg)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_event_t* event = NULL;
	sip_context_type_t* context_type;
	sip_context_length_t* context_len = NULL;
	sip_support_list_t* support_list = NULL;
	sip_support_t* support = NULL;
	sip_message_t* message = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;
	char sz_num[10] = {0};

	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "M", FETION_DOMAIN );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_helper->my_uri );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

#ifdef __WIN32__
	itoa( p_helper->n_callid, sz_num, 10 );
#else
    sprintf( sz_num, "%d", p_helper->n_callid );
#endif
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_helper->n_cseq, "M" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	To
	 */

	sip_message_set_to_str( message, p_helper->dst_uri );

	/*
	 *	context type
	 */

	sip_common_init( &context_type );
	sip_common_set_all( context_type, "text/plain" );
	sip_message_set_common( &(message->context_type), context_type );

	/*
	 *	TODO: support list
	 */

	support_list = (sip_support_list_t*)sip_malloc( sizeof(sip_support_list_t) );
	sip_common_init( &support );
	sip_common_set_all( support, "SaveHistory" );
	support_list->sip_k = support;
	support_list->next = NULL;
	message->support_list = support_list;

	/*
	 *	event
	 */

	sip_common_init( &event );
	sip_common_set_all( event, "CatMsg" );
	sip_message_set_common( &(message->event), event );

	/*
	 *	context length
	 */

#ifdef __WIN32__
	itoa( strlen( msg ), sz_num, 10 );
#else
    sprintf( sz_num, "%d", strlen(msg) );
#endif
	sip_common_init( &context_len );
	sip_common_set_all( context_len, sz_num );
	sip_message_set_common( &(message->context_len), context_len );

	/*
	 *	body
	 */


	sip_message_set_body( message, msg );

	n_ret = sip_message_to_str( message, sip_msg );

	sip_message_free( message );

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;

}

int fx_sip_generate_resp_recv_msg( __in sip_message_t* message_recv, __out char** sz_out )
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_message_t* message = NULL;
	FX_RET_CODE n_ret = FX_ERROR_OK;

	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_resp_all( start_line, "200", "OK" );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, message_recv->from->element );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

	sip_common_init( &call_id );
	sip_common_set_all( call_id, message_recv->call_id->element );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, message_recv->cseq->number, message_recv->cseq->method );
	sip_message_set_cseq( message, cseq );

	n_ret = sip_message_to_str( message, sz_out );
	sip_message_free( message );

	if ( n_ret == LIBSIP_SUCCESS )
		return FX_ERROR_OK;
	else
		return FX_ERROR_UNKOWN;

}

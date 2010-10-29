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

/*! \file login.c
 *  \brief the file include some code that login to server
 */

#include <stdio.h>
#include <stdlib.h>
#ifndef __WIN32__
#include <unistd.h>
#endif
#include <string.h>
#include <pthread.h>

#ifdef __WIN32__
#include <windows.h>
#endif

#include "initial.h"
#include "config.h"
#include "mem.h"
#include "fxsocket.h"
#include "protocol.h"
#include "crypto.h"
#include "log.h"
#include "thread.h"
#include "sipc.h"
#include "helper.h"
#include "utf8.h"
#include "xml.h"
#include "login.h"
#include "mutex.h"


extern SYS_CONF_DATA g_sys_conf;
LOGIN_DATA g_login_data = {0};
extern DLG_HELPER g_dlg_helper;
extern PGROUP_LIST g_contact_list;

pthread_t g_recv_thread_id = {0};
pthread_t g_keeplive_thread_id = {0};
pthread_t g_keepbusy_thread_id = {0};

int g_socket = 0;

PGROUP_LIST fx_get_group_list()
{
	return g_contact_list;
}

FX_RET_CODE fx_login( __in PLOGIN_DATA l_data, __out PGROUP_LIST* p_group_list )
{

	int n_ret = 0, socket;
    char* sz_proxy = NULL, *sz_find = NULL, *sz_pack = NULL;
	char* sz_contact_list = NULL;
    char sz_ip[20] = {0};
    MEM_STRUCT mem_recv = {0};
	PSIPC_MSG sip_msg_list = NULL;
    ushort u_port = 0;
	g_dlg_helper.p_auth = (PAUTH_DLG_HELPER)malloc( sizeof(AUTH_DLG_HELPER) );

	/*
	 *	get the login data
	 */

	memcpy( &g_login_data, l_data, sizeof(LOGIN_DATA) );


	/*
	 *	get the server ip
	 */


    sz_proxy = g_sys_conf.sz_sipc_proxy;
    sz_find = strchr( sz_proxy, ':' );
    if ( sz_find == NULL ){
        log_string( "sz_roxy format error!" );
        return FX_ERROR_UNKOWN;
    }
    u_port = atoi( (char*)(sz_find+1) );
    memcpy( sz_ip, sz_proxy, (int)sz_find-(int)sz_proxy );

    /*
     *	create socket
     */


    socket = fx_socket_create( TCP, NULL, 0);
    if ( socket == -1 ){
        log_string( "create socket error!\n" );
        return FX_ERROR_SOCKET;
    }

	g_socket = socket;

    /*
     *  connect to the fetion server
     */

    n_ret = fx_socket_connect( socket, sz_ip, u_port);
    if ( n_ret == -1 ){
        log_string( "fx_login:connect to server error!" );
        return FX_ERROR_SOCKET;
    }


	log_string( "==init auth dlg helper==" );

	/*
	 *	init auth dlg helper
	 */

	strcpy( g_dlg_helper.p_auth->machine_code, "2F6E7CD33AA1F6928E69DEDD7D6C50B1" );
	strcpy( g_dlg_helper.p_auth->phone_num, l_data->sz_phone_num );
	strcpy( g_dlg_helper.p_auth->user_id, l_data->sz_user_id );
	strcpy( g_dlg_helper.p_auth->user_pwd, g_sys_conf.user_data.sz_password );
	strcpy( g_dlg_helper.p_auth->uri, l_data->sz_uri );
	g_dlg_helper.p_auth->n_callid = fx_sip_get_callid();
	g_dlg_helper.p_auth->n_cseq = 1;

	/*
	 *	generate response and send it
	 */

	if ( fx_sip_generate_auth_req( g_dlg_helper.p_auth, &sz_pack ) != FX_ERROR_OK )
	{
		log_string( "fx_login:fx_sip_generate_auth_req error!\n" );
		return FX_ERROR_UNKOWN;
	}

	log_string( "==send data to server 1==" );

	/*
	 *	send data to server
	 */

	n_ret = fx_socket_send( socket, sz_pack, strlen(sz_pack) );
    if ( n_ret == -1 ){
        log_string( "fx_login:send data to server error!" );
        return FX_ERROR_SOCKET;
    }

	free( sz_pack );
	sz_pack = NULL;

	log_string( "==recv buffer from server==" );

	/*
	 *	recv buffer from server
	 */

	fx_sip_recv( socket, &sip_msg_list );
	if ( atoi( sip_msg_list->msg->startline->status_code ) != SIP_UNAUTHORIZED )
	{
		log_string( "fx_login error!\n" );
		return FX_ERROR_UNKOWN;
	}

	log_string( "==generate response package then send it==" );

	/*
	 *	generate response package then send it
	 */

	if ( fx_sip_generate_auth_resp( g_dlg_helper.p_auth, sip_msg_list->msg->www_authenticate->key, \
									sip_msg_list->msg->www_authenticate->nonce, &sz_pack ) != FX_ERROR_OK )
	{
		log_string( "fx_login:fx_sip_generate_auth_resp error!\n" );
		return FX_ERROR_UNKOWN;
	}

	log_string( "==send data to server2==" );

	/*
	 *	send data to server
	 */

	n_ret = fx_socket_send( socket, sz_pack, strlen(sz_pack) );
    if ( n_ret == -1 ){
        log_string( "fx_login:send data to server error!" );
        return FX_ERROR_SOCKET;
    }

	free( sz_pack );

	/*
	 *	free the msg list above
	 */

	fx_sip_msg_list_free( sip_msg_list );
	sip_msg_list = NULL;

	log_string( "recv  data from server2" );

	/*
	 *	here the sleep i don't understand why, but it is necessary
	 */

//#ifdef __WIN32__
//	Sleep( 500 );
//#else
//    sleep(1);
//#endif

	/*
	 *	recv buffer from server
	 */


	fx_sip_recv( socket, &sip_msg_list );
	if ( sip_msg_list->msg->startline->status_code == NULL || \
		(sip_msg_list->msg->startline->status_code != NULL && \
		atoi( sip_msg_list->msg->startline->status_code ) != SIP_OK) )
	{
		log_string( "fx_login failed!\n" );
		return FX_ERROR_UNKOWN;
	}

	/*
	 *	parse contact list
	 */

	fx_parse_contact_list( sip_msg_list->msg->body, p_group_list );

	fx_sip_msg_list_free( sip_msg_list );
	sip_msg_list = NULL;


	/*
	 *	update buddiy status
	 */


	fx_get_buddies_status( socket );

	/*
     *  create new thread to recv data from server
     */

	log_string( "==pthread_create thread_sip_recv==" );
    if ( pthread_create( &g_recv_thread_id, NULL, thread_sip_recv, \
            (void*)socket ) != 0 ){
        log_string( "fx_login:create receive thread error!" );
        return FX_ERROR_THREAD;
    }

#ifdef __WIN32__
	Sleep( 1000 );
#else
	Sleep( 1 );
#endif

	/*
	 *	create keeplive thread
	 */

	log_string( "==pthread_create thread_sip_keeplive==" );
	if ( pthread_create( &g_keeplive_thread_id, NULL, thread_sip_keeplive, \
        (void*)socket ) != 0 ){
		log_string( "fx_login:create keeplive thread error!" );
		return FX_ERROR_THREAD;
    }

#ifdef __WIN32__
	Sleep( 500 );
#else
	usleep( 500 * 1000 );
#endif
    //pthread_cancel( g_recv_thread_id );
	//thread_sip_keep_connection_busy
    return FX_ERROR_OK;

}

int fx_get_socket()
{
	return g_socket;
}

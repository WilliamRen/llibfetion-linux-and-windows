
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
#include "login.h"


extern SYS_CONF_DATA g_sys_conf;
PLOGIN_DATA g_login_data = NULL;

pthread_t g_recv_thread_id = {0};

#define ASE_KEY "4A026855890197CFDF768597D07200B346F3D676411C6F87368B5C2276DCEDD2"

/*only for test*/


/** \fn void* thread_recv( void* lparam )
  * \brief the thread for recv data
  * \param lparam parameter
  * \return NULL
  */

// void* thread_recv( void* lparam )
// {
//     int socket = (int)lparam;
//     MEM_STRUCT mem = {0};
// 
//     while( 1 ){
// 

//         /*

//          *  接收数据

//          */

// 
//         if( fx_socket_recv2( socket, &mem ) == -1 ){
//             log_string( "thread_recv error!\n" );
//             break;
//             //return NULL;
//         }
// 
//         /*
//          *
//          */
// 
//         if( strstr( (char*)(mem.mem_ptr), "Unauthoried" ) ){
// 
//             char* sz_nonce = fx_get_nonce( (char*)(mem.mem_ptr) );
//             char* sz_key = fx_get_key( (char*)(mem.mem_ptr) );
//             char sz_response[1024] = {0};

// 			char* sz_RSA = NULL;

// 			int n_ret = 0;

// 			

// 			FX_RET_CODE ret = FX_ERROR_OK;

// 
//             ret = fx_generate_response( sz_key, sz_nonce, g_login_data->sz_user_id, \

// 					g_sys_conf.user_data.sz_password, &sz_RSA );
// 
//             sprintf( sz_response, LOGIN_STEP2, sz_RSA );
// 
//             log_string( "len = %d:%s", strlen( sz_response ), sz_response );
// 
//             n_ret = fx_socket_send( socket, sz_response, strlen(sz_response) );
//             if ( n_ret == -1 ){
//                 log_string( "fx_login:send data to server error!" );
//                 return NULL;
//             }
// 
//             free( sz_RSA );
//             free( sz_nonce );
//             free( sz_key );
//         }
// 
//         myfree( &mem );
// 
// 
//     }
// 
//     return NULL;
// }

FX_RET_CODE fx_login( PLOGIN_DATA l_data  )
{
    /*将sz_sipc_proxy中的IP和端口分别存放，看着挺麻烦的*/
    int n_ret = 0, socket;
    char* sz_proxy = NULL;
    char* sz_find = NULL;
    char* sz_pack = NULL;
    char sz_ip[20] = {0};
    MEM_STRUCT mem_recv = {0};

    ushort u_port = 0;
	
	g_login_data = l_data;

    sz_proxy = g_sys_conf.sz_sipc_proxy;
    sz_find = strchr( sz_proxy, ':' );
    if ( sz_find == NULL ){
        log_string( "sz_roxy format error!" );
        return FX_ERROR_UNKOWN;
    }
    u_port = atoi( (char*)(sz_find+1) );
    memcpy( sz_ip, sz_proxy, (int)sz_find-(int)sz_proxy );
    /*create socket*/
    socket = fx_socket_create( TCP, NULL, 0);
    if ( socket == -1 ){
        log_string( "create socket error!\n" );
        return FX_ERROR_SOCKET;
    }

    /*
     *  connect to the fetion server
     */

    n_ret = fx_socket_connect( socket, sz_ip, u_port);
    if ( n_ret == -1 ){
        log_string( "fx_login:connect to server error!" );
        return FX_ERROR_SOCKET;
    }

    /*
     *  create new thread to recv data from server
     */

    if ( pthread_create( &g_recv_thread_id, NULL, thread_sip_recv, \
            (void*)socket ) != 0 ){
        log_string( "fx_login:create recevice thread error!" );
        return FX_ERROR_THREAD;
    }

    /*
     *  build the login package (login_1)
     */

    sz_pack = fx_pro_build_package( FX_BUILD_LOGIN_1, l_data );
    if ( sz_pack == NULL ){
        log_string( "fx_login:build package error!" );
        return FX_ERROR_UNKOWN;
    }

    /*
     *  send the login data to server
     */

    n_ret = fx_socket_send( socket, sz_pack, strlen(sz_pack) );
    if ( n_ret == -1 ){
        log_string( "fx_login:send data to server error!" );
        return FX_ERROR_SOCKET;
    }
	
#ifdef __WIN32__
	//Sleep( 100 * 1000 );
	while ( 1 )
	{
	}
#else
    sleep( 50 );
#endif
    pthread_cancel( g_recv_thread_id );

    return FX_ERROR_OK;

}

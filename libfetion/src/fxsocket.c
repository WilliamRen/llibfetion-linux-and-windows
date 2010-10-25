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

/*! \file fxsocket.c
 *  \brief this file include the code of socket operation, copy from myqq
 */

#include <stdio.h>

#ifdef __WIN32__
#include <winsock2.h>
#include <wininet.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "mem.h"
#include "fxsocket.h"

#ifdef __WIN32__
static WSADATA wsa_data;
#endif

void fx_socket_init()
{
#ifdef __WIN32__
	int ret = WSAStartup(MAKEWORD(2,1), &wsa_data);
	if( ret != 0 )
	{
		log_string("qqsocket_init:failed in WSAStartup");
	}
	log_string( "WSA Startup." );
#endif
}
void fx_socket_end()
{
#ifdef __WIN32__
	WSACleanup();
#endif
}

int tcp_keep_alive(int socketfd)
{
	int keepAlive = 1;
	int keepIdle = 10;
	int keepInterval = 5;
	int keepCount = 5;

	if(setsockopt(socketfd , SOL_SOCKET , SO_KEEPALIVE
		,(void*)&keepAlive,sizeof(keepAlive)) == -1){
		log_string("set SO_KEEPALIVE failed\n");
		return -1;
	}

#ifdef __WIN32__
	if(setsockopt(socketfd , IPPROTO_TCP, SO_KEEPALIVE
		,(void *)&keepAlive,sizeof(keepAlive)) == -1){
		log_string("set TCP_KEEPALIVE failed\n");
		return -1;
	}
#else
	if(setsockopt(socketfd , SOL_TCP, TCP_KEEPIDLE
		,(void *)&keepIdle,sizeof(keepIdle)) == -1){
		log_string("set TCP_KEEPIDEL failed\n");
		return -1;
	}

	if(setsockopt(socketfd , SOL_TCP, TCP_KEEPINTVL
		,(void *)&keepInterval,sizeof(keepInterval)) == -1){
		log_string("set TCP_KEEPINTVL failed\n");
		return -1;
	}

	if(setsockopt(socketfd , SOL_TCP, TCP_KEEPCNT
		,(void *)&keepCount,sizeof(keepCount)) == -1){
		log_string("set TCP_KEEPCNT failed\n");
		return -1;
	}
#endif
	return 1;
}

/** \fn int fx_socket_create( int type, char* ip, ushort port )
 *  \brief 创建套接字
 *  \param type 创建的类型, 包括tcp和udp
 *  \param ip ip地址
 *  \param port 端口
 *  \return 如果成功返回创建的套接字的句柄,否则返回-1
 */

int fx_socket_create( int type, char* ip, ushort port )
{
	int fd = 0;
	struct sockaddr_in addr;
	switch( type )
	{
	case TCP:
		fd = socket( PF_INET, SOCK_STREAM, 0 );
		memset( &addr, 0, sizeof(struct sockaddr_in) );
		addr.sin_family = PF_INET;
		if( ip )
			netaddr_set( ip, &addr );
		else
			addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons( port );
		if( bind( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in) ) < 0 )
		{
			log_string("bind tcp socket error!");
			fx_socket_close( fd );
			return -1;
		}
		break;
	case UDP:
		fd = socket( PF_INET, SOCK_DGRAM, IPPROTO_IP );
		memset( &addr, 0, sizeof(struct sockaddr) );
		addr.sin_family = PF_INET;
		if( ip )
			netaddr_set( ip, &addr );
		else
			addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons( port );
		if( bind( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in) ) < 0 )
		{
			log_string("bind udp socket error!");
			fx_socket_close( fd );
			return -1;
		}
		break;
	}
	return fd;
}

void fx_socket_close( int fd )
{
#ifdef __WIN32__
	closesocket( fd );
#else
	close( fd );
#endif
}

/** \fn int fx_socket_connect( int fd, char* ip, ushort port )
 *  \brief connect to server
 *  \param fd handle of socket we create
 *  \param ip ip address
 *  \param port port
 *  \return if successfull return 0 otherwise return -1
 */

int fx_socket_connect( int fd, char* ip, ushort port )
{
	struct sockaddr_in addr;
	int max_recv = 0 ;
	memset( &addr, 0, sizeof(struct sockaddr_in) );
	addr.sin_family = PF_INET;
	netaddr_set( ip, &addr );
	addr.sin_port = htons( port );


	/*
	 *	set the max recv buffer
	 */

#ifdef __WIN32__
	max_recv = SO_MAX_MSG_SIZE;
#else
	max_recv = MAX_RECV_BUF_SIZE;
	setsockopt(fd , SOL_SOCKET , SO_RCVBUF , (const char*)&max_recv , sizeof(max_recv));
#endif
	if( connect( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0 )
	{
		log_string("qqsocket connect failed.");
		return -1;
	}
	return 0;
}

int fx_socket_connect2( int fd, uint ip, ushort port )
{
	struct sockaddr_in addr;
	int max_recv = 0;

	memset( &addr, 0, sizeof(struct sockaddr_in) );
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = htonl( ip );
	addr.sin_port = htons( port );

	/*
	 *	set the max recv buffer
	 */

#ifdef __WIN32__
	max_recv = SO_MAX_MSG_SIZE;
#else
	max_recv = MAX_RECV_BUF_SIZE;
	setsockopt(fd , SOL_SOCKET , SO_RCVBUF , (const char*)&max_recv , sizeof(max_recv));
#endif

	if( connect( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0 )
	{
		log_string("qqsocket connect failed.");
		return -1;
	}
	return 0;
}

void netaddr_set( char* name, struct sockaddr_in* addr )
{
	if( (addr->sin_addr.s_addr = inet_addr( name ) ) == -1 )
	{
		//it's not an IP.
		//not an ip, maybe a domain
		struct hostent *host;
		host = gethostbyname( name );
		if( host )
		{
			addr->sin_addr.s_addr = *(size_t*) host->h_addr_list[0];
		}else{
			log_string("Failed to get ip by" );
		}
	}
}

int fx_socket_send( int fd, uchar* buf, size_t size )
{
	int ret;
	size_t rest;
	rest = size;
	while( rest > 0 )
	{
		ret = send( fd, (char*)buf, rest, 0);
		if(ret <= 0 )
		{
			return ret;
		}
		rest -= ret;
		buf += ret;
	}
	return size;
}

int fx_socket_recv( int fd, uchar* buf, size_t size )
{
	return recv( fd, (char*)buf, size, 0 );
}

#define MAX_RECV_PER    1024
int fx_socket_recv2( int fd, PMEM_STRUCT mem )
{
    int ret = 0;
    char sz_recv[MAX_RECV_PER];

    while( 1 )
    {
        memset( sz_recv, 0, MAX_RECV_PER );
        ret = recv( fd, sz_recv, MAX_RECV_PER, 0);
        if ( ret == -1 ){
            log_string( "fx_socket_recv2:recv data error!" );
            return ret;

        }
        if ( ret == 0 ){
            break;

        }


        /*

         *  allocate memory for recv data

         */


        if ( ret > 0 ){
            mem->mem_ptr = myrealloc( mem->mem_ptr, mem->size + ret + 1 );
            if (mem->mem_ptr) {
                memcpy(&(mem->mem_ptr[mem->size]), sz_recv, ret);
                mem->size += ret;
                mem->mem_ptr[mem->size] = 0;
            }
            if ( ret < MAX_RECV_PER ){
                break;

            }

        }

    }
    return mem->size;
}


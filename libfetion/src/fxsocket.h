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

#ifndef FXSOCKET_H_INCLUDED
#define FXSOCKET_H_INCLUDED

#include "commdef.h"
#include "mem.h"

#define MAX_RECV_BUF_SIZE	61140

#define UDP 0
#define TCP 1

int fx_socket_create( int type, char* ip, ushort port );
int fx_socket_connect( int fd, char* ip, ushort port );
void netaddr_set( char* name, struct sockaddr_in* addr );
int fx_socket_send( int fd, uchar* buf, size_t size );
int fx_socket_recv( int fd, uchar* buf, size_t size );
int fx_socket_recv2( int fd, PMEM_STRUCT mem );
void fx_socket_close( int fd );

#endif // FXSOCKET_H_INCLUDED

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

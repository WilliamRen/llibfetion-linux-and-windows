#ifndef FXSOCKET_H_INCLUDED
#define FXSOCKET_H_INCLUDED

#define UDP 0
#define TCP 1

typedef unsigned char uchar;
typedef unsigned short ushort;

int fx_socket_create( int type, char* ip, ushort port );
int fx_socket_connect( int fd, char* ip, ushort port );
void netaddr_set( char* name, struct sockaddr_in* addr );
int fx_socket_send( int fd, uchar* buf, size_t size );
int fx_socket_recv( int fd, uchar* buf, size_t size );

#endif // FXSOCKET_H_INCLUDED
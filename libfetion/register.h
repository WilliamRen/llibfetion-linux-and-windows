
/*! \file register.h
	\brief	register.cpp的头文件包含所一些函数和变量的定义	
	\note	一些算法就在这个模块里面定义
*/

#ifndef __REGISTER__H__
#define __REGISTER__H__

/** \def FX_REGISTER_TYPE
	\brief  自定义类型,作用是叫make_packet函数构造什么样的数据包
*/

#define FX_REGISTER_TYPE	int

/** \def FX_GET_RESPONSE_TYPE
	\brief 自定义类型,指出了要获得服务器返回的数据中的什么数据

*/

#define FX_GET_RESPONSE_TYPE int

#define FX_GET_RESPONSE_CODE	1
#define FX_GET_RESPONSE_CNONCE	2
#define FX_GET_RESPONSE_ARG		3


char*	fx_get_server_response(char c_mark );
int		fx_get_response_header_length( char* sz_response );
int		fx_get_arg_length( char*	sz_response );
int		fx_get_response_code( char* sz_response );
char*	fx_get_response_nonce( char* sz_response );
char*	fx_get_response_arg( char* sz_response );
char*	fx_make_response( char* sz_sid, char* sz_pwd, char* sz_domain,\
		       char* sz_nonce,  char* sz_cnonce );
char*	fx_make_cnonce();

LPSTR ConvertGBKToUtf8( LPSTR strGBK ) ;

LPSTR ConvertUtf8ToGBK( LPSTR strUtf8 ) ;
#endif
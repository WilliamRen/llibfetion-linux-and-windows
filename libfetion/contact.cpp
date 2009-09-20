
/*! \file contact.cpp
	\brief 这是conteact的代码文件，其中包括了获得本人信息,还有活的联系人列表的代码
		   以及筛选的正确的回复包的代码
*/

#include "g_libfetion.h"
#include "register.h"

extern  SOCKET	g_socket;

extern  GROUP_LIST* g_p_group_list;

char*	fx_select_correct_response( char* sz_recv_data );

/** \fn LPVOID		fx_get_personal_info()
	\brief	这个函数就是获得本人的信息
*/

LPVOID		fx_get_personal_info()
{
	int nRet = 0;
	
	
	char* sz_send_data = fx_make_packet( FX_MAKE_PACKET_GET_PERSONAL_INFO, NULL );
	
	nRet = send( g_socket, sz_send_data, strlen( sz_send_data ) + 1, 0 );
	if ( nRet == SOCKET_ERROR )
	{
		TRACE( "send get personal info packet error!\n" );
		return NULL;
	}
	
	char*  sz_recv_data = fx_get_server_response( 'S' );

	sz_recv_data = fx_select_correct_response( 'S', sz_recv_data );
	if ( sz_recv_data == NULL )
	{
		TRACE( "得到用户信息失败!\n" );
		return NULL;
	}

	char*  sz_arg = fx_get_response_arg( sz_recv_data );
	TRACE( "arg = %s\n", sz_arg );

	//delete sz_send_data;
	//delete sz_recv_data;
	return sz_arg;
}

/** \fn FX_RET_CODE			fx_get_contact( LPVOID* p_struct )
	\brief 这个函数是用来获得联系人列表的这里用的事已链表的形式来保存联系人信息的
	\param p_struct 这个就是空链表的指针
*/

FX_RET_CODE			fx_get_contact( LPVOID* p_struct )
{
	int nRet = 0;
	char* sz_send_data = fx_make_packet( FX_MAKE_PACKET_GET_CONTACT, NULL );

	nRet = send( g_socket, sz_send_data, strlen( sz_send_data ) + 1, 0 );

	if ( nRet == SOCKET_ERROR )
	{
		TRACE( "send get contact packet error!\n" );
		return 1;
	}
	char*	sz_recv_data = fx_get_server_response( 'S' );

	sz_recv_data = fx_select_correct_response( 'S', sz_recv_data );
	if ( sz_recv_data == NULL )
	{
		TRACE( "得到连接人列表失败!\n" );
		return 1;
	}

	char*	sz_arg = fx_get_response_arg( sz_recv_data );


	//
	// 解析xml数据
	//
	
	fx_parse_xml( FX_PARSE_XML_CONTACT_DATA, sz_arg, *p_struct );
	*p_struct = g_p_group_list;

	//delete sz_recv_data;
	//delete sz_send_data;
	return FX_ERROR_OK;
	
}

/** \fn char*	fx_select_correct_response( char c_cmd ,char* sz_recv_data )
	\brief 这个函数是用来过滤飞信的系统消息的,这个系统消息特别恶心呀
	\param c_cmd 这是用来表示数据包的头部的第一个字母也就是协议的命令字
	\param sz_recv_data	这是接受到得数据
	\return	返回的是过滤后的数据
*/

char*	fx_select_correct_response( char c_cmd ,char* sz_recv_data )
{
	if ( sz_recv_data[0] != c_cmd )
	{
		
		//
		// 我们首先来计算头和body得长度看下面是否还跟着一个包
		//
		
		int n_ret_len = strlen( sz_recv_data );
		int n_fisrt_len = fx_get_response_header_length( sz_recv_data ) + \
			fx_get_arg_length( sz_recv_data );
		
		if ( n_ret_len > n_fisrt_len )
		{
			
			//
			// 说明还有一个包
			//
			
			if ( sz_recv_data[n_fisrt_len] == 'S' )
			{
				
				//
				// 是回复包
				//
				
				return sz_recv_data + n_fisrt_len;
			}
		}
		else
			return NULL;
	}
	return sz_recv_data;
}
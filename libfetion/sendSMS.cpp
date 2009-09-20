
/*! \file sendSMS.cpp
	\brief 这个文件中实现了发送信息给自己和发送信息给别人的代码.
*/

#include "g_libfetion.h"
#include "libfetion.h"
#include "register.h"

extern GETLOGINDATA g_struc_get_login_data;
extern  SOCKET	g_socket;

/** \fn FX_RET_CODE	fx_send_sms_to_yourself( char* sz_massage )
	\brief 顾名思义这就是想自己发送信息的函数
	\param 是将要发送信息的内容
	\return	发送成功返回 FX_ERROR_OK 失败返回 FX_ERROR_SEND_SMS
*/

FX_RET_CODE	fx_send_sms_to_yourself( char* sz_massage )
{
	PMAKE_PACKET_SEND_SMS	p_send_data = new MAKE_PACKET_SEND_SMS;
	strcpy( p_send_data->sz_uri, g_struc_get_login_data.sz_uri );
	strcpy( p_send_data->sz_message, ConvertGBKToUtf8( (LPSTR)sz_massage ) );
	char*	sz_send_data = fx_make_packet( FX_MAKE_PACKET_SEND_SMS_SELF, p_send_data );
	TRACE( sz_send_data );
	int nRet = 0;
	nRet = send( g_socket, sz_send_data, strlen( sz_send_data ), 0 );
	char*	sz_recv_data = fx_get_server_response( 'S' );
	sz_recv_data = fx_select_correct_response( 'S', sz_recv_data );
	int n_code = fx_get_response_code( sz_recv_data );
	if ( n_code == FX_SEND_SMS_SUCCESSED )
	{
		TRACE( "send to yourself OK!\n" );
		return FX_ERROR_OK;
	}
	return FX_ERROR_SEND_SMS;
}

/** \fn FX_RET_CODE fx_send_sms_to_others( char* sz_uri, char* sz_massage )
	\brief 顾名思义这就是向别人发送信息的函数
	\param sz_uri 是发信人的uri可以在获得联系人的函数中获得
	\param sz_massage 是将要发送信息的内容
	\return	发送成功返回 FX_ERROR_OK 失败返回 FX_ERROR_SEND_SMS
*/

FX_RET_CODE fx_send_sms_to_others( char* sz_uri, char* sz_massage )
{
	PMAKE_PACKET_SEND_SMS	p_send_data = new MAKE_PACKET_SEND_SMS;
	strcpy( p_send_data->sz_uri, sz_uri );
	strcpy( p_send_data->sz_message, ConvertGBKToUtf8( (LPSTR)sz_massage ) );
	char*	sz_send_data = fx_make_packet( FX_MAKE_PACKET_SEND_SMS, p_send_data );
	TRACE( sz_send_data );
	int nRet = 0;
	nRet = send( g_socket, sz_send_data, strlen( sz_send_data ), 0 );
	char*	sz_recv_data = fx_get_server_response( 'S' );
	sz_recv_data = fx_select_correct_response( 'S', sz_recv_data );
	int n_code = fx_get_response_code( sz_recv_data );
	if ( n_code == FX_SEND_SMS_SUCCESSED || n_code == 200 )
	{
		TRACE( "send to other OK!\n" );
		return FX_ERROR_OK;
	}
	return FX_ERROR_SEND_SMS;
}
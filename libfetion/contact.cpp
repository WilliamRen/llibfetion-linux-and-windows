
/*! \file contact.cpp
	\brief ����conteact�Ĵ����ļ������а����˻�ñ�����Ϣ,���л����ϵ���б�Ĵ���
		   �Լ�ɸѡ����ȷ�Ļظ����Ĵ���
*/

#include "g_libfetion.h"
#include "register.h"

extern  SOCKET	g_socket;

extern  GROUP_LIST* g_p_group_list;

char*	fx_select_correct_response( char* sz_recv_data );

/** \fn LPVOID		fx_get_personal_info()
	\brief	����������ǻ�ñ��˵���Ϣ
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
		TRACE( "�õ��û���Ϣʧ��!\n" );
		return NULL;
	}

	char*  sz_arg = fx_get_response_arg( sz_recv_data );
	TRACE( "arg = %s\n", sz_arg );

	//delete sz_send_data;
	//delete sz_recv_data;
	return sz_arg;
}

/** \fn FX_RET_CODE			fx_get_contact( LPVOID* p_struct )
	\brief ������������������ϵ���б�������õ������������ʽ��������ϵ����Ϣ��
	\param p_struct ������ǿ������ָ��
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
		TRACE( "�õ��������б�ʧ��!\n" );
		return 1;
	}

	char*	sz_arg = fx_get_response_arg( sz_recv_data );


	//
	// ����xml����
	//
	
	fx_parse_xml( FX_PARSE_XML_CONTACT_DATA, sz_arg, *p_struct );
	*p_struct = g_p_group_list;

	//delete sz_recv_data;
	//delete sz_send_data;
	return FX_ERROR_OK;
	
}

/** \fn char*	fx_select_correct_response( char c_cmd ,char* sz_recv_data )
	\brief ����������������˷��ŵ�ϵͳ��Ϣ��,���ϵͳ��Ϣ�ر����ѽ
	\param c_cmd ����������ʾ���ݰ���ͷ���ĵ�һ����ĸҲ����Э���������
	\param sz_recv_data	���ǽ��ܵ�������
	\return	���ص��ǹ��˺������
*/

char*	fx_select_correct_response( char c_cmd ,char* sz_recv_data )
{
	if ( sz_recv_data[0] != c_cmd )
	{
		
		//
		// ��������������ͷ��body�ó��ȿ������Ƿ񻹸���һ����
		//
		
		int n_ret_len = strlen( sz_recv_data );
		int n_fisrt_len = fx_get_response_header_length( sz_recv_data ) + \
			fx_get_arg_length( sz_recv_data );
		
		if ( n_ret_len > n_fisrt_len )
		{
			
			//
			// ˵������һ����
			//
			
			if ( sz_recv_data[n_fisrt_len] == 'S' )
			{
				
				//
				// �ǻظ���
				//
				
				return sz_recv_data + n_fisrt_len;
			}
		}
		else
			return NULL;
	}
	return sz_recv_data;
}
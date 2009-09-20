
/*! \file register.cpp
	\brief ����ļ���ʵ����ע�����������Ĵ���.
	\note  �����ע�Ტ����ע���û���ע��,��ָ���߷��������½��
	\par   ��Ҫ���㷨����md5��sha1
*/

#include "g_libfetion.h"
#include "register.h"

//
// ʹ��ǰ�涨��ı���
//

extern char g_sz_user_pwd[20];
extern GETLOGINDATA g_struc_get_login_data;
extern SOCKET	g_socket;

//
// �Լ���ȫ�ֱ���
//

char	sz_cnonce[32];

int		g_ret_code = 0;

/** \fn FX_RET_CODE	fx_registe_to_server()
	\brief	��Ҫ����ע����������
*/

FX_RET_CODE	fx_registe_to_server( char*	sz_server_ip )
{
	
	//
	// ��ʼ���ӷ�����
	//
	
	g_socket = socket( AF_INET, SOCK_STREAM, 0 );
	
	//
	// ����IP��ַ�Լ��˿�
	//
	
	SOCKADDR_IN addrsrv;
	addrsrv.sin_addr.S_un.S_addr = inet_addr( sz_server_ip );
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(8080);
	
	//
	// ���ӷ�����
	//
	
	int nRet = 0;
	
	nRet = connect( g_socket, (SOCKADDR*)&addrsrv, sizeof(SOCKADDR) );
	if ( nRet !=0 )
	{
		printf( "connect server error!\n" );
		return FX_ERROR_REGISTER;
	}
	
	//
	// ��һ���ȷ���ע�������
	//
	
	char*	sz_send_data = fx_make_packet( FX_MAKE_PACKET_REGISTER_STEP_1, NULL );
	TRACE( "%s\n", sz_send_data );

	nRet = send( g_socket, sz_send_data, strlen( sz_send_data ) + 1, 0 );
	if ( nRet == SOCKET_ERROR )
	{
		TRACE( "send data!\n" );
		return FX_ERROR_REGISTER;
	}
	char* sz_recv_data = fx_get_server_response('S');
	if ( fx_get_response_code( sz_recv_data ) != 401 )
	{
		TRACE( "error return code!\n" );
		return FX_ERROR_REGISTER;
	}
	char* sz_nonce = fx_get_response_nonce( sz_recv_data );
	if ( sz_nonce == NULL )
	{
		TRACE( "get nonce error!\n" );
		return FX_ERROR_REGISTER;
	}
	char* sz_cnonce = fx_make_cnonce();
	char* sz_response = fx_make_response( g_struc_get_login_data.sz_sid, \
										  g_sz_user_pwd,
										  g_struc_get_login_data.sz_domain, \
										  sz_nonce, 
										  sz_cnonce
										 );
	//delete sz_recv_data;
	//
	// �ڶ���
	//
	
	PMAKE_PACKET_DATA_STEP2	p_data = new MAKE_PACKET_DATA_STEP2;
	memset( p_data, 0, sizeof( MAKE_PACKET_DATA_STEP2 ) );
	strcpy( p_data->sz_response, sz_response );
	strcpy( p_data->sz_cnonce, sz_cnonce );

	
	sz_send_data = fx_make_packet( FX_MAKE_PACKET_REGISTER_STEP_2, p_data );

	nRet = send( g_socket, sz_send_data, strlen( sz_send_data ) + 1, 0 );
	if ( nRet == SOCKET_ERROR )
	{
		TRACE( "send data!\n" );
		return FX_ERROR_REGISTER;
	}
	sz_recv_data = fx_get_server_response('S');
	int n_code = fx_get_response_code( sz_recv_data );
	if ( n_code == 400 )
	{
		TRACE( "���ݰ���ʽ����!\n" );
		return FX_ERROR_REGISTER;
	}
	else if ( n_code == 401 )
	{
		TRACE( "�������!\n" );
		return FX_ERROR_REGISTER;
	}
	else if ( n_code == 404 )
	{
		TRACE( "�û�������!\n" );
		return FX_ERROR_REGISTER;
	}
	if ( n_code == 200 )
	{
		TRACE( "��½�ɹ�!" );
		return FX_ERROR_OK;
	}

	return FX_ERROR_OK;
}

/** \fn	LPVOID	fx_get_server_response( FX_GET_RESPONSE_TYPE n_get_type )
	\brief	��÷������������ݵ�������Ϣ,����cnonce,��ϵ���б��xml����
	\param	������Ҫ������ݵ�������
*/

char*	fx_get_server_response( char c_mark )
{
	
	//
	// ֱ��recv����
	//
	
		
		//
		// �����Ƚ���1024�ֽڵ�����Ȼ���ж�����մ�С
		// ���С��1024��˵���ǽ�������,���=1024��ô
		// �������������п϶�����һ���ֶ�"L:"�����������
		// �����ĺ������ARG���ݵĴ�С,�󲿷�ARG���ݾ���XML��ʽ��
		//
		
	char	sz_recv[1024];
	char*	sz_recv_all = NULL;
	int		nRet = 0;
	memset( sz_recv, 0, 1024 );

	nRet = recv( g_socket, sz_recv, 1024, 0 );
	if ( nRet == SOCKET_ERROR )
	{
		TRACE( "�������ݴ���\n" );
		return NULL;
	}

	//
	// ���=1024��ô���Ƿ���Э��ͷ�õ����ݳ��ȼ�������
	//
	
	if ( nRet == 1024 )
	{
		
		int n_total_len = 0;
		//
		// ���ж��Ƿ���ȷ�Ļظ�����������ʣ�೤��
		//
		if ( sz_recv[0] != c_mark )
		{

				n_total_len = fx_get_response_header_length( sz_recv ) + \
						 fx_get_arg_length( sz_recv );
				char* sz_recv_1 = fx_select_correct_response( c_mark, sz_recv );
				int n_total_len_1 = fx_get_response_header_length( sz_recv_1 ) + \
						 fx_get_arg_length( sz_recv_1 );
				n_total_len += n_total_len_1;
			
		}
		else
		{	
			n_total_len = fx_get_response_header_length( sz_recv ) + \
						 fx_get_arg_length( sz_recv );
		}
		
		//
		// Ϊ���ݷ������ڴ�
		//
		
		sz_recv_all = new char[n_total_len+1];
		if ( !sz_recv_all )
		{
			TRACE( "�����ڴ�ʧ��!\n" );
			return NULL;
		}

		memset( sz_recv_all, 0, n_total_len + 1 );
		
		//
		// ����ǰ���յ���1024�ֽڸ��Ƶ����ڴ���
		//
		
		memcpy( sz_recv_all, sz_recv, 1024 );
		
		//
		// ����ʣ�µ�����
		//

		int n_ret_temp = 0;
		nRet = 0;
		while ( n_ret_temp < ( n_total_len - 1024 ) )
		{
			nRet = recv( g_socket, (char*)( sz_recv_all + 1024 + n_ret_temp ),\
										1024, 0 );
			n_ret_temp += nRet;
		}
		
	}
	else
	{
		sz_recv_all = new char[nRet+1];
		memset( sz_recv_all, 0, nRet+1 );
		memcpy( sz_recv_all, sz_recv, nRet );
		sz_recv_all[nRet+1] = 0;
	}
	return sz_recv_all;
}

/** \fn int		fx_get_response_header_length( char* sz_response )
	\brief		�õ�����������Э��ͷ�ĳ���
	\param		sz_response �Ƿ��������ص�����
*/

int		fx_get_response_header_length( char* sz_response )
{
	char*	sz_find = NULL;	
	sz_find = strstr( sz_response, "\r\n\r\n" );
	if ( sz_find != NULL )
		return sz_find - sz_response + 4;
	else
		return 0;
}

/** \fn int		fx_get_arg_length( char*	sz_response )
	\brief	���ǵõ�ARG��Ϣ�ĳ���
	\param	sz_response �Ƿ��������ص�����
*/

int		fx_get_arg_length( char*	sz_response )
{
	char* sz_find = NULL;
	sz_find = strstr ( sz_response, "L: " );
	if ( sz_find != NULL )
	{
		char*	sz_find_temp = strstr( sz_find, "\r\n" );
		char	sz_num[10];
		memset(sz_num, 0, 10 );

		memcpy( sz_num, sz_find + 3, sz_find_temp - sz_find - 3 );
		return atoi( sz_num );
	}
	else
		return 0;
}

/** \fn int		fx_get_response_code( char* sz_response )
	\brief	���ǵõ��������ķ���ֵ
	\param	sz_response �Ƿ������ķ�������
	\return ���ط��������ش���
*/

int		fx_get_response_code( char* sz_response )
{
	char* sz_find = strchr( sz_response, ' ' );
	if ( sz_find != NULL )
	{
		char sz_code[5];
		memset( sz_code, 0, 5 );
		char* sz_find_temp = strchr( (char*)( sz_find + 1 ), ' ' );
		memcpy( sz_code, sz_find + 1, sz_find_temp - sz_find - 1 );
		return atoi( sz_code );
	}
	else
		return 0;
}

/** \fn char*	fx_get_response_nonce( char* sz_response )
	\brief	���ǵõ��������ķ��ص�nonce
	\param	sz_response �Ƿ������ķ�������
	\return	����nonce
*/

char*	fx_get_response_nonce( char* sz_response )
{
	char* sz_find = strstr( sz_response, "nonce" );
	char* sz_find_nonce = new char[50];
	char* sz_nonce_temp = sz_find_nonce;
	memset( sz_find_nonce, 0, 50 );
	if ( sz_find != NULL )
	{
		sz_find += ( strlen( "nonce=\"" ));
		while ( *sz_find != '\"' )
		{
			*sz_nonce_temp = *sz_find;
			sz_nonce_temp++;
			sz_find++;
		}
		*sz_nonce_temp = '\0';
		return sz_find_nonce; 
	}
	else
		return NULL;
}

/** \fn char*	fx_get_response_arg( char* sz_response )
	\brief	���ǵõ��������ķ��ص�arg
	\param	sz_response �Ƿ������ķ�������
	\return ����arg
*/

char*	fx_get_response_arg( char* sz_response )
{
	int n_arg_len = fx_get_arg_length( sz_response );
	char* sz_arg = new char[n_arg_len+1];
	memset( sz_arg, 0, n_arg_len+1 );
	memcpy( sz_arg, (char*)(sz_response + \
			fx_get_response_header_length( sz_response ) ), n_arg_len );
	return sz_arg;
}

/** \fn char*	fx_make_response( char* sz_sid, char* sz_pwd, char* sz_domain,\
		       char* sz_nonce,  char* sz_cnonce )
	\brief	��������ĵ�������㷨
	\param	sz_sid �û���sid �����ڵ�¼ʱ��õ������еõ�
	\param	sz_pwd �û������� �����ڵ�¼ʱ��õ������еõ�
	\param	sz_domain ���ŵ�domian ����fetion.com.cn �����ڵ�¼ʱ��õ������еõ�
	\param	sz_nonce ��¼ʱ���������ص�nonce
	\param	sz_cnonce �����Լ�������ɵ�����
	\return ���ؼ��ܺ������
*/

char*	fx_make_response( char* sz_sid, char* sz_pwd, char* sz_domain,\
		       char* sz_nonce,  char* sz_cnonce )
{
	CString str_un_encode_key;
	CString str_encode_key;
	CString strH1;
	CString strH2;
	CString str_response;
	BYTE*	b_encode_key;

	////////////////////////////////////////////////////////////////////////

	str_un_encode_key.Format( "%s:%s:%s", sz_sid, sz_domain, sz_pwd );
	
	//
	// asc to utf8
	//
	
	//ConvertGBKToUtf8( str_un_encode_key );

	char* sz_utf8_encode_key = NULL;
	sz_utf8_encode_key = ConvertGBKToUtf8( str_un_encode_key.GetBuffer(0) );

	printf( sz_utf8_encode_key );

	//
	// MD5����
	//
	
	MD5Init();
	MD5Update( (BYTE*)sz_utf8_encode_key, strlen( sz_utf8_encode_key ) );
	b_encode_key = MD5Final();
	CString strtemp;

	BYTE b_un_encode_h1[100];
	memset( b_un_encode_h1, 0, 100 );
	memcpy( b_un_encode_h1, b_encode_key, 16 );
	b_un_encode_h1[16] = ':';
	b_un_encode_h1[49] = ':';
	memcpy( (BYTE*)(b_un_encode_h1 + 17), sz_nonce, 32 );
	memcpy( (BYTE*)(b_un_encode_h1 + 50), sz_cnonce, 32 );

	
	//
	// �ٽ���MD5����
	//
	
	MD5Init();
	MD5Update( b_un_encode_h1, strlen( (char*)b_un_encode_h1 ) );
	b_encode_key = MD5Final();
	for ( int i = 0; i < 16; i++ )
	{
		strtemp.Format( "%02X", b_encode_key[i] );
		strH1 += strtemp;
	}

	///////////////////////////////////////////////////////////////////

	str_encode_key.Empty();
	str_encode_key.Format( "REGISTER:%s", sz_sid );
	

	//
	// �ٴ�asc to utf8
	//

	sz_utf8_encode_key = ConvertGBKToUtf8( str_encode_key.GetBuffer(0) );

	//
	// �ٽ���MD5����
	//
	
	MD5Init();
	MD5Update( (BYTE*)sz_utf8_encode_key, strlen( sz_utf8_encode_key ) );
	b_encode_key = MD5Final();
	for (  i = 0; i < 16; i++ )
	{
		strtemp.Format( "%02X", b_encode_key[i] );
		strH2 += strtemp;
	}

	/////////////////////////////////////////////////////////////////

	str_encode_key.Empty();
	str_encode_key.Format( "%s:%s:%s", strH1, sz_nonce, strH2 );
	
	//
	// �ٴν���ת��
	//
	
	sz_utf8_encode_key = ConvertGBKToUtf8( str_encode_key.GetBuffer(0) );

	//
	// �ٽ���MD5����
	//
	
	MD5Init();
	MD5Update( (BYTE*)sz_utf8_encode_key, strlen( sz_utf8_encode_key ) );
	b_encode_key = MD5Final();
	for ( i = 0; i < 16; i++ )
	{
		strtemp.Format( "%02X", b_encode_key[i] );
		str_response += strtemp;
	}
	printf( "str_response = %s\n", str_response );

	return str_response.GetBuffer( 0 );

}

/** \fn char* fx_make_cnonce()
	\brief ����cnonce,���������ʱ�õ�
	\return ���ɵ�cnonce
*/

char* fx_make_cnonce()
{
	srand( (unsigned)time( NULL ) );
	char* sz_conce = new char[50];
	memset( sz_conce, 0, 50 );
	sprintf( sz_conce, "%04X%04X%04X%04X%04X%04X%04X%04X", \
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF );
	return sz_conce;
}

/** \fn LPSTR ConvertGBKToUtf8( LPSTR strGBK ) 
	\brief ��ascIIת����utf8��ʽ
	\return ת�����utf8��ʽ���ַ���
*/

LPSTR ConvertGBKToUtf8( LPSTR strGBK ) 
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPSTR)strGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset( wszUtf8, 0, len * 2 + 2 );
	MultiByteToWideChar( CP_ACP, 0, (LPSTR)strGBK, -1, wszUtf8, len );
	
	len = WideCharToMultiByte( CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL );
	char *szUtf8=new char[len + 1];
	memset( szUtf8, 0, len + 1 );
	WideCharToMultiByte ( CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL );
	
	return szUtf8;
}

/** \fn LPSTR ConvertUtf8ToGBK( LPSTR strUtf8 ) 
	\brief ��utf8ת����ascII��ʽ
	\return ת�����ascII��ʽ���ַ���
*/

LPSTR ConvertUtf8ToGBK( LPSTR strUtf8 ) 
{
	int len = MultiByteToWideChar( CP_UTF8, 0, (LPSTR)strUtf8, -1, NULL, 0 );
	unsigned short * wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPSTR)strUtf8, -1, wszGBK, len);
	
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);
	
	return szGBK;
	
}

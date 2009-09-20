
/*! \file libfetion.cpp
	\brief ����libfetion�Ĵ����ļ������а����˳�ʼ��,��½,
		   ����xml,���з�װ���ݰ�,�����������һЩ����
*/

#include "g_libfetion.h"
#include "register.h"

//
// ȫ�ֱ���
//


/** \var SOCKET	g_socket
	\brief �����������ӷ�������socket,�󲿷ֹ��������϶��������
*/

SOCKET	g_socket;


/** \var CURL* g_curl
	\brief ����libcurl��,����https������.
*/

CURL*	g_curl = NULL;

/** \var g_struc_get_login_data
	\brief �����˴ӷ�������õ���Ϣ�ṹ���ָ��
*/

GETLOGINDATA g_struc_get_login_data;

char	g_sz_user_pwd[20];

GROUP_LIST* g_p_group_list;

/** \fn FX_RET_CODE		fx_init()
	\brief ������ʼ��libfetion.
		   ���������ʼ����com����,libcurl,socket��.
	\note  ��������Ὣ�ڷ�����������ݱ����ڳ���Ŀ¼�µ�
		   get.xml�ļ�����,cookies������cookie.txt��
	\warning ��ʹ��libfetion֮ǰ�����ʼ����
	\return FX_RET_CODE �����˷��ش���

*/


FX_RET_CODE	fx_init()
{
	
	//
	// ���ȳ�ʼ��libcurl
	//
	
	g_curl = curl_easy_init();
	if ( !g_curl )
	{
		TRACE( "��ʼ��libcurlʧ��!\n" );
		return FX_ERROR_INIT;
	}

	
	//
	// ��ʼ��socket
	//
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		TRACE( "WSAStartup error!\n" );
		return FX_ERROR_INIT;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		 HIBYTE( wsaData.wVersion ) != 1 )
	{
		WSACleanup();
		return FX_ERROR_INIT; 
	}
	
	//
	// ��ʼ��com�ӿ�
	//
	
	

	return FX_ERROR_OK;

}

/** \fn FX_RET_CODE fx_login( LPTSTR sz_phone_num, LPTSTR sz_password  )
	\brief ��������������������½�Ĵ���
	\param sz_phone_num ������ֻ��ţ���sid��½�Ļ�Ϊ������
			��һ���汾������
	\param sz_password ����ķ������룬����������ȷ������
	\return FX_RET_CODE �����˷��ش���
	\note	�����������ʹ����libcurl���������¾�

*/

FX_RET_CODE		fx_login( LPTSTR sz_phone_num, LPTSTR sz_password, \
									 PGETLOGINDATA p_get_user_data )
{
	CURLcode	curl_ret;
	char		sz_login_url[256];
	curl_slist*	struc_headers = NULL;
	memset( sz_login_url, 0, 256 );

	//
	// ��ʽ����ַ
	//
	
	sprintf( sz_login_url, "%s?mobileno=%s&pwd=%s", \
			FX_LOGIN_URL, sz_phone_num, sz_password );
	
	
	//
	// ���û������뱣��
	//
	
	memset( g_sz_user_pwd, 0, 20 );
	strcpy( g_sz_user_pwd, sz_password );

	//
	// ����д���ļ�
	//
	
	FILE* pfile_xml = fopen( "get.xml", "w+" );
	if ( !pfile_xml )
	{
		TRACE( "���ļ� get.xmlʧ��!\n" );
		return FX_ERROR_LOGIN;
	}

	
	//
	// �޸�Э��ͷ
	//
	
	struc_headers = curl_slist_append( struc_headers, \
									"Accept: Agent-007" );
	
	//
	// ����libcurl
	//
	
	curl_easy_setopt( g_curl, CURLOPT_COOKIEJAR, "cookie.txt" );
	curl_easy_setopt( g_curl, CURLOPT_URL, sz_login_url );
	curl_easy_setopt( g_curl, CURLOPT_HTTPHEADER, struc_headers );
	curl_easy_setopt( g_curl, CURLOPT_CONNECTTIMEOUT, 8 );			//��ʱ8��
	curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, pfile_xml );		//�����д���ļ�
	
	//
	// ִ��libcurl
	//
	
	curl_ret = curl_easy_perform( g_curl );
	if ( curl_ret != CURLE_OK )
	{
		TRACE( "ִ��curlʧ��!\n" );
		return FX_ERROR_LOGIN;
	}
	
	//
	// �ر�����
	//
	
	fclose( pfile_xml );

	//
	// ����xml�ļ�
	//
		
		//
		// �����Ƚ�xml�ļ����ݶ����ڴ�
		//
		
		int		n_file_size = 0;
		char*	sz_read_xml_file = NULL;
		pfile_xml = fopen( "get.xml", "r" );
		fseek( pfile_xml, 0, SEEK_END );
		n_file_size = ftell( pfile_xml );
		sz_read_xml_file = new  char[n_file_size+1];
		memset( sz_read_xml_file, 0, n_file_size + 1 );
		fseek( pfile_xml, 0, SEEK_SET );
		fread( sz_read_xml_file, sizeof(char), n_file_size, pfile_xml );
		fclose( pfile_xml );
	
	//
	// ��ʼ����
	//

	if( fx_parse_xml( FX_PAESE_XML_LOGIN_DATA, sz_read_xml_file, \
		p_get_user_data ) != FX_ERROR_OK )
	{
		TRACE( "����xml�ļ�ʧ��\n" );
		return FX_ERROR_LOGIN;
	}
#ifdef _DEBUG
	TRACE( "uri = %s\n", g_struc_get_login_data.sz_uri );
	TRACE( "sid = %s\n",g_struc_get_login_data.sz_sid );
	TRACE( "domian = %s\n", g_struc_get_login_data.sz_domain );
#endif // _DEBUG
	return FX_ERROR_OK;

}

/** \fn FX_RET_CODE		fx_parse_xml( FX_PARSE_XML_STEP n_step_code, LPVOID	p_struct )
	\brief	����xml�ļ������е���Ϣ�����ڽṹ����
	\param  n_step_code ���������ǽ���������XML
	\param	p_xml_data  ��Ҫ������xml����
	\param	p_struct	��������������ָ��
	\return	FX_RET_CODE �Զ���״̬��
	\note	������ʹ��tinyxml��,��������Щ�����¾�
*/

FX_RET_CODE		fx_parse_xml( FX_PARSE_XML_STEP n_step_code, LPVOID p_xml_data, LPVOID	p_struct )
{
		
		//
		// �����ж��ǽ���ʲô����
		//
		
		TiXmlDocument	xml_root_doc;
		TiXmlNode*		xml_node = NULL;
		TiXmlAttribute*	xml_attribute = NULL;

		switch ( n_step_code )
		{
		case FX_PAESE_XML_LOGIN_DATA:
			{
				
				//
				// Ϊ�ṹ�����ռ�
				//
				
				PGETLOGINDATA p_login_data = (PGETLOGINDATA)p_struct;
				
				//
				// ����
				//
				
				xml_root_doc.Parse( (char*)p_xml_data );
				
				//
				// �õ���һ���ӽڵ�
				//
				
				xml_node = xml_root_doc.RootElement()->FirstChild();
				
				//
				// ѭ��ÿһ���ӽڵ�
				//
				
				while( xml_node )
				{
					if ( strcmp( xml_node->Value(), "user" ) == 0 )
					{
						xml_attribute = xml_node->ToElement()->FirstAttribute();
						while ( xml_attribute )
						{
							if ( strcmp( xml_attribute->Name(), "uri" ) == 0 )
							{
								
								//
								// ���ṹ��
								//
								
								strcpy( p_login_data->sz_uri, xml_attribute->Value() );
								TRACE( "uri = %s\n", p_login_data->sz_uri );
								
								//
								// ��������uri�ֱ�õ�sid��domain
								//
								char* szfind = strchr( xml_attribute->Value(), '@' );
								memcpy( p_login_data->sz_sid, ( xml_attribute->Value() + 4 ),\
										szfind - xml_attribute->Value() - 4 );
								TRACE( "sid = %s\n",p_login_data->sz_sid );
								memcpy( p_login_data->sz_domain, szfind + 1, \
										strchr( xml_attribute->Value(), ';' ) - szfind - 1 );
								TRACE( "domian = %s\n", p_login_data->sz_domain );

							}
							else if ( strcmp( xml_attribute->Name(), "mobile-no" ) == 0 )
							{
								strcpy( p_login_data->sz_phone_num, xml_attribute->Value() );
							}
							else if ( strcmp( xml_attribute->Name(), "user-status" ) == 0 )
							{
								p_login_data->nstatu_user = atoi( xml_attribute->Value() );
							}
							else if ( strcmp( xml_attribute->Name(), "user-id" ) == 0 )
							{
								strcpy( p_login_data->sz_user_id, xml_attribute->Value() );
							}
							xml_attribute = xml_attribute->Next();
						}
					}
					xml_node = xml_node->NextSibling();
				}
				
				//
				// copy to our struct
				//
				
				memcpy( &g_struc_get_login_data, p_login_data, sizeof( GETLOGINDATA ) );
				break;
			}
		case FX_PARSE_XML_CONTACT_DATA:
			{
				
				TiXmlDocument	xmlDoc;
				TiXmlElement*	p_root_element = NULL;
				TiXmlNode*		p_node = NULL;
				TiXmlNode*		p_node_ = NULL;
				TiXmlAttribute*		xml_attribute = NULL;
				TiXmlElement*	P_child_element;
				
				xmlDoc.Parse( (char*)p_xml_data );
				p_root_element = xmlDoc.RootElement();
				p_node = p_root_element->FirstChild();
				p_node_ = p_node->FirstChild();  // <contacts>
				while ( p_node_ )
				{

					P_child_element = p_node_->FirstChildElement();	// <buddy-lists>
					while ( P_child_element )
					{
						if ( strcmp( P_child_element->Value(), "buddy-list" ) == 0 )
						{
							GROUP_LIST* p_struct = new GROUP_LIST;
							memset( p_struct, 0, sizeof( GROUP_LIST ) );
							xml_attribute = P_child_element->FirstAttribute();
							while ( xml_attribute )
							{
								if ( strcmp( xml_attribute->Name(), "id" ) == 0 )
								{
									p_struct->n_group_id = atoi( xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name() , "name" ) == 0 )
								{
									strcpy( p_struct->sz_group_name, \
											ConvertUtf8ToGBK( (LPSTR)xml_attribute->Value()) );
								}
								xml_attribute = xml_attribute ->Next();
							}
							add_group_info_to_link( p_struct );
						}
						else if ( strcmp( P_child_element->Value(), "buddy" ) == 0 )
						{
							CONTACT_LIST* p_struct = new CONTACT_LIST;
							memset( p_struct, 0, sizeof( CONTACT_LIST ) );
							p_struct->b_mobile = false;
							xml_attribute = P_child_element->FirstAttribute();
							while ( xml_attribute )
							{
								if ( strcmp( xml_attribute->Name(), "uri" ) == 0 )
								{
									strcpy( p_struct->sz_uri, xml_attribute->Value() );	
								}
								else if ( strcmp( xml_attribute->Name(), "user-id" ) == 0 )
								{
									strcpy( p_struct->sz_user_id, xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "local-name" ) == 0 )
								{
									strcpy( p_struct->sz_local_name, \
											ConvertUtf8ToGBK( (LPSTR)xml_attribute->Value() )  );
								}
								else if ( strcmp( xml_attribute->Name(), "buddy-lists" ) == 0 )
								{
									p_struct->n_group_list_id = atoi( xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "relation-status" ) == 0 )
								{
									p_struct->n_relation_status = atoi( xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "feike-read-version" ) == 0 )
								{
									p_struct->n_feike_read_version = atoi( xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "online-notify" ) == 0 )
								{
									p_struct->n_online_notify = atoi( xml_attribute->Value() );
								}
								xml_attribute = xml_attribute ->Next();
							}
							add_contect_info_to_link( p_struct );
						}
						else if ( strcmp( P_child_element->Value(), "mobile-buddy" ) == 0 )
						{
							CONTACT_LIST* p_struct = new CONTACT_LIST;
							memset( p_struct, 0, sizeof( CONTACT_LIST ) );
							p_struct->b_mobile = true;
							xml_attribute = P_child_element->FirstAttribute();
							while ( xml_attribute )
							{
								if ( strcmp( xml_attribute->Name(), "uri" ) == 0 )
								{
									strcpy( p_struct->sz_uri, xml_attribute->Value() );	
								}
								else if ( strcmp( xml_attribute->Name(), "user-id" ) == 0 )
								{
									strcpy( p_struct->sz_user_id, xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "local-name" ) == 0 )
								{
									strcpy( p_struct->sz_local_name, \
										 ConvertUtf8ToGBK(( LPSTR)xml_attribute->Value())  );
								}
								else if ( strcmp( xml_attribute->Name(), "buddy-lists" ) == 0 )
								{
									p_struct->n_group_list_id = atoi( xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "relation-status" ) == 0 )
								{
									p_struct->n_relation_status = atoi( xml_attribute->Value() );
								}
								else if ( strcmp( xml_attribute->Name(), "online-notify" ) == 0 )
								{
									p_struct->n_online_notify = atoi( xml_attribute->Value() );
								}
								xml_attribute = xml_attribute ->Next();
							}
							add_contect_info_to_link( p_struct );
						}
						P_child_element = P_child_element->NextSiblingElement();
					}
					p_node_ = p_node_->NextSibling();
				}
				
			}
			break;
		default:
			break;
		}
		p_struct = g_p_group_list;
		delete p_xml_data;
		return FX_ERROR_OK;
}

/** \fn char* fx_get_sid()
	\brief ����»�ñ��˵�sid�º���
	\return ����sid
*/

char* fx_get_sid()
{
	return g_struc_get_login_data.sz_sid;
}

/** \fn FX_RET_CODE	fx_close()
	\brief ����˳�libfetionʱ�ĺ���
	\return FX_RET_CODE�����Ϸ���OK
*/

FX_RET_CODE	fx_close()
{
		if ( g_curl )
		{
			curl_easy_cleanup( g_curl );
		}
		if ( g_socket )
		{
			closesocket( g_socket );
			WSACleanup();
		}
		if ( g_p_group_list )
		{
			clean_link();
		}
		return FX_ERROR_OK; 
}


/** \fn char* fx_make_packet( FX_REGISTER_TYPE n_registe_type )
	\brief	�����������ݰ�
	\param	n_registe_type ���ݰ�������
*/

char* fx_make_packet( FX_MAKE_PACKET_TYPE n_registe_type, LPVOID p_data )
{
	CString str_packet;
	CString str_temp;
	static int	n_call_id = 0;

	//
	// ��������˵��һ��,������͵����,���������ÿ����������Լ�������
	// ���ڿ��ͬѧ�������Լ�дһ����ͨ�õĺ���
	//
	
	switch( n_registe_type )
	{
	case FX_MAKE_PACKET_REGISTER_STEP_1:
		{
			
			//
			//	the packet for register request
			//
			
			// ----------------step 1---------------------
			str_packet = "R fetion.com.cn SIP-C/2.0\r\n";
			// ----------------step 2---------------------
			str_temp.Format( "F: %s\r\n", g_struc_get_login_data.sz_sid );
			str_packet += str_temp;
			// ----------------step 3---------------------
			str_temp.Empty();
			n_call_id = fx_get_next_call();
			str_temp.Format( "I: %d \r\n", n_call_id );
			str_packet += str_temp;
			// ----------------step 4--------------------
			str_packet += "Q: 1 R\r\n";
			// ----------------step 5--------------------
			str_temp.Format( "L: %d\r\n\r\n", strlen( FX_LOGIN_REQUEST_ARG ) + 1 );
			str_packet += str_temp;
			// ----------------step 6---------------------
			str_packet += FX_LOGIN_REQUEST_ARG;

			return str_packet.GetBuffer( 0 );
			
		}
		break;
	case FX_MAKE_PACKET_REGISTER_STEP_2:
		{
			
			//
			// the packet for the password requst of server
			//
			
			PMAKE_PACKET_DATA_STEP2	p_make_data = (PMAKE_PACKET_DATA_STEP2) p_data;

			// ------------------step 1------------------------
			str_packet = "R fetion.com.cn SIP-C/2.0\r\n";
			// ------------------step 2-------------------------
			str_temp.Format( "F: %s\r\n", g_struc_get_login_data.sz_sid );
			str_packet += str_temp;
			// ------------------step 3-------------------------
			str_temp.Empty();
			str_temp.Format( "I: %d \r\n", n_call_id );
			str_packet += str_temp;
			// ------------------step 4--------------------------
			str_packet += "Q: 2 R\r\n";
			// ------------------step 5--------------------------
			str_temp.Empty();
			str_temp.Format( "A: Digest response=\"%s\",cnonce=\"%s\"\r\n",\
							p_make_data->sz_response, p_make_data->sz_cnonce );
			str_packet += str_temp;
			// ------------------step 6--------------------------
			str_temp.Format( "L: %d\r\n\r\n", strlen( FX_LOGIN_REQUEST_ARG ) + 1 );
			str_packet += str_temp;
			// ------------------step 7--------------------------
			str_packet += FX_LOGIN_REQUEST_ARG;
			
			return str_packet.GetBuffer( 0 );

		}
		break;
	case FX_MAKE_PACKET_GET_PERSONAL_INFO:
		{
			// ------------------step 1------------------------
			str_packet = "S fetion.com.cn SIP-C/2.0\r\n";
			// ------------------step 2-------------------------
			str_temp.Empty();
			str_temp.Format( "F: %s\r\n", g_struc_get_login_data.sz_sid );
			str_packet += str_temp;
			// ----------------step 3---------------------
			str_temp.Empty();
			n_call_id = fx_get_next_call();
			str_temp.Format( "I: %d \r\n", n_call_id );
			str_packet += str_temp;
			// ----------------step 4--------------------
			str_packet += "Q: 1 S\r\n";
			// ------------------step 5--------------------------
			str_packet += "N: GetPersonalInfo\r\n";
			// ------------------step 6--------------------------
			str_temp.Empty();
			str_temp.Format( "L: %d\r\n\r\n", strlen( FX_GET_PERSONAL_INFO ) + 1 );
			str_packet += str_temp;
			// ------------------step 7--------------------------
			str_packet += FX_GET_PERSONAL_INFO;
			
			return str_packet.GetBuffer( 0 );
		}
		break;
	case FX_MAKE_PACKET_GET_CONTACT:
		{
			// ------------------step 1------------------------
			str_packet = "S fetion.com.cn SIP-C/2.0\r\n";
			// ------------------step 2-------------------------
			str_temp.Empty();
			str_temp.Format( "F: %s\r\n", g_struc_get_login_data.sz_sid );
			str_packet += str_temp;
			// ----------------step 3---------------------------
			str_temp.Empty();
			n_call_id = fx_get_next_call();
			str_temp.Format( "I: %d \r\n", n_call_id );
			str_packet += str_temp;
			// ----------------step 4---------------------------
			str_packet += "Q: 1 S\r\n";
			// ------------------step 5--------------------------
			str_packet += "N: GetContactList\r\n";
			// ------------------step 6--------------------------
			str_temp.Empty();
			str_temp.Format( "L: %d\r\n\r\n", strlen( FX_GETCONTACT_ARG )+1 );
			str_packet += str_temp;
			// ------------------step 7--------------------------
			str_packet += FX_GETCONTACT_ARG;
			
			return str_packet.GetBuffer( 0 );
		}
		break;
	case FX_MAKE_PACKET_SEND_SMS_SELF:
		{
			PMAKE_PACKET_SEND_SMS	p_send_sms = (PMAKE_PACKET_SEND_SMS) p_data;
			// ------------------step 1------------------------
			str_packet = "M fetion.com.cn SIP-C/2.0\r\n";
			// ------------------step 2-------------------------
			str_temp.Empty();
			str_temp.Format( "F: %s\r\n", g_struc_get_login_data.sz_sid );
			str_packet += str_temp;
			
			// ----------------step 3---------------------
			str_temp.Empty();
			n_call_id = fx_get_next_call();
			str_temp.Format( "I: %d \r\n", n_call_id );
			str_packet += str_temp;
			// ----------------step 4---------------------------
			str_packet += "Q: 1 M\r\n";
			// ------------------step 5--------------------------
			str_temp.Empty();
			str_temp.Format( "T: %s\r\n", p_send_sms->sz_uri );
			str_packet += str_temp;
			// ------------------step 6--------------------------
			str_packet += "N: SendCatSMS\r\n";
			// ------------------step 7--------------------------
			str_temp.Empty();
			str_temp.Format( "L: %d\r\n\r\n", strlen( p_send_sms->sz_message ) );
			str_packet += str_temp;
			// ------------------step 8--------------------------
			str_packet += p_send_sms->sz_message;

			return str_packet.GetBuffer( 0 );
		}
		break;
	case FX_MAKE_PACKET_SEND_SMS:
		{
			PMAKE_PACKET_SEND_SMS	p_send_sms = (PMAKE_PACKET_SEND_SMS) p_data;
			// ------------------step 1------------------------
			str_packet = "M fetion.com.cn SIP-C/2.0\r\n";
			// ------------------step 2-------------------------
			str_temp.Empty();
			str_temp.Format( "F: %s\r\n", g_struc_get_login_data.sz_sid );
			str_packet += str_temp;		
			// ----------------step 3---------------------
			str_temp.Empty();
			n_call_id = fx_get_next_call();
			str_temp.Format( "I: %d \r\n", n_call_id );
			str_packet += str_temp;
			// ----------------step 4---------------------------
			str_packet += "Q: 2 M\r\n";
			// ------------------step 5--------------------------
			str_temp.Empty();
			str_temp.Format( "T: %s\r\n", p_send_sms->sz_uri );
			str_packet += str_temp;
			// ------------------step 6-------------------------
			str_packet += "C: text/plain\r\n";
			// ------------------step 7--------------------------
			str_packet += "K: SaveHistory\r\n";
			// ------------------step 8--------------------------
			str_packet += "N: CatMsg\r\n";
			// ------------------step 9--------------------------
			str_temp.Empty();
			str_temp.Format( "L: %d\r\n\r\n", strlen( p_send_sms->sz_message ) );
			str_packet += str_temp;
			// ------------------step 10--------------------------
			str_packet += p_send_sms->sz_message;
			
			return str_packet.GetBuffer( 0 );
		}
	default:
		break;
	}
	return NULL;
}

/** \fn int		fx_get_next_call()
	\brief ���������ݰ���I�����ݵĺ���

*/

int		fx_get_next_call()
{
	static	int	n_call = 0;
	return ++n_call;
}

/** \fn void add_group_info_to_link( GROUP_LIST* p_group )
	\brief ���ǽ�����Ϣ���뵽������
	\param p_group ��Ҫ������������Ա�ṹ��
*/

void add_group_info_to_link( GROUP_LIST* p_group )
{
	if ( g_p_group_list == NULL )
	{
		g_p_group_list = p_group;
	}
	else
	{
		GROUP_LIST* p_temp = g_p_group_list;
		while ( p_temp->p_next != NULL )
		{
			p_temp = p_temp->p_next;
		}
		p_temp->p_next = p_group;
	}
}

/** \fn void add_contect_info_to_link(  CONTACT_LIST* p_contact )
	\brief ���ǽ���ϵ����Ϣ���뵽������
	\param p_group ��Ҫ�����������ϵ�˽ṹ��
*/

void add_contect_info_to_link(  CONTACT_LIST* p_contact )
{
	if ( g_p_group_list == NULL )
		return;
	GROUP_LIST* p_group = g_p_group_list;
	while ( p_group )
	{
		if ( p_group->n_group_id == p_contact->n_group_list_id )
		{
			if ( p_group->p_contact == NULL )
			{
				p_group->p_contact = p_contact;
			}
			else
			{
				CONTACT_LIST* p_list_contact = p_group->p_contact;
				while ( p_list_contact->p_next )
				{
					p_list_contact = p_list_contact->p_next;
				}
				p_list_contact->p_next = p_contact;
			}
		}
		p_group = p_group->p_next;
	}
}

/** \fn void clean_link( )
	\brief �������
*/

void clean_link( )
{
	GROUP_LIST* p_group = g_p_group_list;
	while( p_group )
	{
		CONTACT_LIST* p_contact = p_group->p_contact;
		if ( p_group )
		{
			while ( p_contact )
			{
				delete p_contact;
				p_contact = p_contact ->p_next;
			}
		}
		delete p_group;
		p_group = p_group->p_next;
	}
}

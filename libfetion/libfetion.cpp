
/*! \file libfetion.cpp
	\brief 这是libfetion的代码文件，其中包括了初始化,登陆,
		   解析xml,还有封装数据包,和链表操作的一些内容
*/

#include "g_libfetion.h"
#include "register.h"

//
// 全局变量
//


/** \var SOCKET	g_socket
	\brief 这是用于连接服务器的socket,大部分工作基本上都由他完成
*/

SOCKET	g_socket;


/** \var CURL* g_curl
	\brief 用于libcurl的,连接https服务器.
*/

CURL*	g_curl = NULL;

/** \var g_struc_get_login_data
	\brief 保存了从服务器获得的消息结构体的指针
*/

GETLOGINDATA g_struc_get_login_data;

char	g_sz_user_pwd[20];

GROUP_LIST* g_p_group_list;

/** \fn FX_RET_CODE		fx_init()
	\brief 用来初始化libfetion.
		   函数里面初始化了com引用,libcurl,socket等.
	\note  这个函数会将在服务器获得数据保存在程序目录下的
		   get.xml文件里面,cookies保存在cookie.txt中
	\warning 在使用libfetion之前必须初始化。
	\return FX_RET_CODE 定义了返回代码

*/


FX_RET_CODE	fx_init()
{
	
	//
	// 首先初始化libcurl
	//
	
	g_curl = curl_easy_init();
	if ( !g_curl )
	{
		TRACE( "初始化libcurl失败!\n" );
		return FX_ERROR_INIT;
	}

	
	//
	// 初始化socket
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
	// 初始化com接口
	//
	
	

	return FX_ERROR_OK;

}

/** \fn FX_RET_CODE fx_login( LPTSTR sz_phone_num, LPTSTR sz_password  )
	\brief 这个函数包括向服务器登陆的代码
	\param sz_phone_num 是你的手机号，用sid登陆的还为开发，
			下一个版本将会有
	\param sz_password 是你的飞信密码，必须输入正确的密码
	\return FX_RET_CODE 定义了返回代码
	\note	这个函数里面使用了libcurl向其作者致敬

*/

FX_RET_CODE		fx_login( LPTSTR sz_phone_num, LPTSTR sz_password, \
									 PGETLOGINDATA p_get_user_data )
{
	CURLcode	curl_ret;
	char		sz_login_url[256];
	curl_slist*	struc_headers = NULL;
	memset( sz_login_url, 0, 256 );

	//
	// 格式化网址
	//
	
	sprintf( sz_login_url, "%s?mobileno=%s&pwd=%s", \
			FX_LOGIN_URL, sz_phone_num, sz_password );
	
	
	//
	// 将用户的密码保存
	//
	
	memset( g_sz_user_pwd, 0, 20 );
	strcpy( g_sz_user_pwd, sz_password );

	//
	// 设置写的文件
	//
	
	FILE* pfile_xml = fopen( "get.xml", "w+" );
	if ( !pfile_xml )
	{
		TRACE( "打开文件 get.xml失败!\n" );
		return FX_ERROR_LOGIN;
	}

	
	//
	// 修改协议头
	//
	
	struc_headers = curl_slist_append( struc_headers, \
									"Accept: Agent-007" );
	
	//
	// 设置libcurl
	//
	
	curl_easy_setopt( g_curl, CURLOPT_COOKIEJAR, "cookie.txt" );
	curl_easy_setopt( g_curl, CURLOPT_URL, sz_login_url );
	curl_easy_setopt( g_curl, CURLOPT_HTTPHEADER, struc_headers );
	curl_easy_setopt( g_curl, CURLOPT_CONNECTTIMEOUT, 8 );			//超时8秒
	curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, pfile_xml );		//将结果写入文件
	
	//
	// 执行libcurl
	//
	
	curl_ret = curl_easy_perform( g_curl );
	if ( curl_ret != CURLE_OK )
	{
		TRACE( "执行curl失败!\n" );
		return FX_ERROR_LOGIN;
	}
	
	//
	// 关闭所有
	//
	
	fclose( pfile_xml );

	//
	// 解析xml文件
	//
		
		//
		// 我们先将xml文件数据读入内存
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
	// 开始解析
	//

	if( fx_parse_xml( FX_PAESE_XML_LOGIN_DATA, sz_read_xml_file, \
		p_get_user_data ) != FX_ERROR_OK )
	{
		TRACE( "解析xml文件失败\n" );
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
	\brief	解析xml文件将其中的信息保存在结构体中
	\param  n_step_code 用来定义是解析怎样的XML
	\param	p_xml_data  是要解析的xml数据
	\param	p_struct	是用来保存结果的指针
	\return	FX_RET_CODE 自定义状态码
	\note	这里面使用tinyxml库,这里向那些作者致敬
*/

FX_RET_CODE		fx_parse_xml( FX_PARSE_XML_STEP n_step_code, LPVOID p_xml_data, LPVOID	p_struct )
{
		
		//
		// 首先判断是解析什么数据
		//
		
		TiXmlDocument	xml_root_doc;
		TiXmlNode*		xml_node = NULL;
		TiXmlAttribute*	xml_attribute = NULL;

		switch ( n_step_code )
		{
		case FX_PAESE_XML_LOGIN_DATA:
			{
				
				//
				// 为结构体分配空间
				//
				
				PGETLOGINDATA p_login_data = (PGETLOGINDATA)p_struct;
				
				//
				// 分析
				//
				
				xml_root_doc.Parse( (char*)p_xml_data );
				
				//
				// 得到第一个子节点
				//
				
				xml_node = xml_root_doc.RootElement()->FirstChild();
				
				//
				// 循环每一个子节点
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
								// 填充结构体
								//
								
								strcpy( p_login_data->sz_uri, xml_attribute->Value() );
								TRACE( "uri = %s\n", p_login_data->sz_uri );
								
								//
								// 接下来拆开uri分别得到sid和domain
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
	\brief 这个事获得本人的sid德函数
	\return 返回sid
*/

char* fx_get_sid()
{
	return g_struc_get_login_data.sz_sid;
}

/** \fn FX_RET_CODE	fx_close()
	\brief 这个退出libfetion时的函数
	\return FX_RET_CODE基本上返回OK
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
	\brief	就是制作数据包
	\param	n_registe_type 数据包的类型
*/

char* fx_make_packet( FX_MAKE_PACKET_TYPE n_registe_type, LPVOID p_data )
{
	CString str_packet;
	CString str_temp;
	static int	n_call_id = 0;

	//
	// 这里我想说明一点,我这里偷懒了,所以我这对每个请求包都自己构造了
	// 有勤快的同学可以子自己写一个更通用的函数
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
	\brief 是用于数据包中I的数据的函数

*/

int		fx_get_next_call()
{
	static	int	n_call = 0;
	return ++n_call;
}

/** \fn void add_group_info_to_link( GROUP_LIST* p_group )
	\brief 这是将组信息加入到链表中
	\param p_group 是要加入链表的组成员结构体
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
	\brief 这是将联系人信息加入到链表中
	\param p_group 是要加入链表的联系人结构体
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
	\brief 清除链表
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

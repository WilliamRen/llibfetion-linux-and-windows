/*! \file g_libfetion.h
	\brief ���ǹ���ʹ�õ�ͷ�ļ�
*/
#ifndef __G_LIBFETION__H__
#define __G_LIBFETION__H__

#pragma warning ( disable : 4006 )

#include <afx.h>
#include <winsock2.h>
#include <tinyxml.h>
#include <curl\curl.h>
#include <cryptohash.h>
#include "libfetion.h"


#pragma comment ( lib, "libcurl.lib" )
#pragma comment ( lib, "wldap32.lib" )
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "cryptohash.lib" )
#pragma comment ( lib, "tinyxml.lib" )
/** \note ���������ӿ⼰�书��
	\par	\b cryptohash.lib ���ܣ�<e>��������˴������㷨�����������õ�ֻ��MD5��SHA1����BASE64</e>
	\par	\b tinyxml.lib ���ܣ�<e>���ṩ�˽���xml�ļ������ݵĹ���</e>
	\par	\b libcurl.lib ����: <e>���ṩ����https���������ӵĹ���</e>
	\par	\b wldap32.lib ����: <e>����libcurl.lib������������</e>
	\par	\b ws2_32.lib  ����: <e>΢��Ŀ�,�ṩ��socket��֧��</e>

	\warning ���ڶ����ṩ�����ӿ�,��ô����涨������ʱ�ⶼ��Mutithreaded Dll
			 ��Ȼ�ᷢ����಻��Ԥ�ϵĴ���

*/


/** \def FX_LOGIN_URL
	\brief �����˷��ŵ�½����������ַ
*/

#define FX_LOGIN_URL		"http://nav.fetion.com.cn/ssiportal/SSIAppSignIn.aspx"

/** \def FX_LOGIN_URL
	\brief �����˷��ŵ�½�����������ݰ��ĺ�׺
*/

#define FX_LOGIN_REQUEST_ARG	"<args>"\
									"<device type=\"PC\" version=\"284571220\" client-version=\"3.3.0370\" />"\
									"<caps value=\"simple-im;im-session;temp-group;personal-group\" />"\
									"<events value=\"contact;permission;system-message;personal-group\" />"\
									"<user-info attributes=\"all\" />"\
									"<presence>"\
										"<basic value=\"400\" desc=\"\" />"\
									"</presence>"\
								"</args>"

/** \def FX_LOGIN_URL
	\brief �����˷��Ż�ñ�����Ϣ�����ݰ��ĺ�׺
*/

#define FX_GET_PERSONAL_INFO	"<args>"\
									"<personal attributes=\"all\" />"\
									"<services version=\"\" attributes=\"all\" />"\
									"<quota attributes=\"all\" />"\
								"</args>"

/** \def FX_LOGIN_URL
	\brief �����˻����ϵ�˵����ݰ��ĺ�׺
*/

#define FX_GETCONTACT_ARG		"<args>"\
									"<contacts>"\
										"<buddy-lists />"\
										"<buddies attributes=\"all\" />"\
										"<mobile-buddies attributes=\"all\" />"\
										"<chat-friends />"\
										"<blacklist />"\
										"<allow-list />"\
									"</contacts>"\
								"</args>"

#define FX_MAKE_PACKET_TYPE int


/** \def FX_REGISTER_STEP_1
	\brief ע������������һ��
*/

#define FX_MAKE_PACKET_REGISTER_STEP_1		1

/** \def FX_REGISTER_STEP_1
	\brief ע�����������ڶ���
*/

#define	FX_MAKE_PACKET_REGISTER_STEP_2		2

/** \def FX_MAKE_PACKET_REGISTER_STEP_2
	\brief ע����������������
*/
#define FX_MAKE_PACKET_REGISTER_STEP_3		3

/** \def FX_MAKE_PACKET_GET_PERSONAL_INFO
	\brief ������ø�����Ϣ�����ݰ�
*/

#define FX_MAKE_PACKET_GET_PERSONAL_INFO	4

/** \def FX_MAKE_PACKET_GET_CONTACT
	\brief �����������ϵ���б�����ݰ�
*/

#define FX_MAKE_PACKET_GET_CONTACT			5

/** \def FX_MAKE_PACKET_SEND_SMS_SELF
	\brief ����������Ϣ���Լ������ݰ�
*/

#define FX_MAKE_PACKET_SEND_SMS_SELF		6

/** \def FX_MAKE_PACKET_SEND_SMS_SELF
	\brief ����������Ϣ���Լ������ݰ�
*/

#define FX_MAKE_PACKET_SEND_SMS				7

/** \def FX_SEND_SMS_SUCCESSED
	\brief ���巢����Ϣ�ɹ����������ص���Ϣ����
*/

#define FX_SEND_SMS_SUCCESSED				280

/** \struct _MAKE_PACKET_DATA_STEP2
	\brief ����ǰ���respone��cnonce�ýṹ��
	�����뿴Դ����
*/

typedef struct _MAKE_PACKET_DATA_STEP2
{
	char	sz_response[33];
	char	sz_cnonce[33];

}MAKE_PACKET_DATA_STEP2,*PMAKE_PACKET_DATA_STEP2;

/** \def MAX_SMS_LENGTH	500
	\brief ������������Ϣ����
*/

#define MAX_SMS_LENGTH	500

/** \struct _MAKE_PACKET_SEND_SMS
	\brief ����Ƿ�����Ϣʱ����ϵ����Ϣ�Ľṹ��
	�����뿴Դ����
*/
typedef struct _MAKE_PACKET_SEND_SMS
{
	char	sz_uri[50];
	char	sz_message[MAX_SMS_LENGTH];
}MAKE_PACKET_SEND_SMS, *PMAKE_PACKET_SEND_SMS;

FX_RET_CODE	fx_parse_xml( FX_PARSE_XML_STEP n_step_code, LPVOID p_xml_data, LPVOID p_struct );
char*		fx_make_packet( FX_MAKE_PACKET_TYPE n_registe_type, LPVOID	p_data );
int			fx_get_next_call();
void		add_group_info_to_link( GROUP_LIST* p_group );
void		add_contect_info_to_link( CONTACT_LIST* p_contact );
void		clean_link();
char*		fx_select_correct_response( char c_cmd ,char* sz_recv_data );


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __G_LIBFETION__H__

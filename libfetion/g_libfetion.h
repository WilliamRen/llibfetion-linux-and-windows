/*! \file g_libfetion.h
	\brief 这是公共使用的头文件
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
/** \note 下面是链接库及其功能
	\par	\b cryptohash.lib 功能：<e>里面包含了大量的算法，我们这里用的只有MD5和SHA1还有BASE64</e>
	\par	\b tinyxml.lib 功能：<e>他提供了解析xml文件或数据的功能</e>
	\par	\b libcurl.lib 功能: <e>它提供了向https服务器连接的功能</e>
	\par	\b wldap32.lib 功能: <e>是用libcurl.lib必须加入这个库</e>
	\par	\b ws2_32.lib  功能: <e>微软的库,提供了socket的支持</e>

	\warning 由于都是提供的链接库,那么这里规定其运行时库都是Mutithreaded Dll
			 不然会发生许多不可预料的错误

*/


/** \def FX_LOGIN_URL
	\brief 定义了飞信登陆服务器的网址
*/

#define FX_LOGIN_URL		"http://nav.fetion.com.cn/ssiportal/SSIAppSignIn.aspx"

/** \def FX_LOGIN_URL
	\brief 定义了飞信登陆服务器的数据包的后缀
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
	\brief 定义了飞信获得本人信息的数据包的后缀
*/

#define FX_GET_PERSONAL_INFO	"<args>"\
									"<personal attributes=\"all\" />"\
									"<services version=\"\" attributes=\"all\" />"\
									"<quota attributes=\"all\" />"\
								"</args>"

/** \def FX_LOGIN_URL
	\brief 定义了获得联系人的数据包的后缀
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
	\brief 注册至服务器第一步
*/

#define FX_MAKE_PACKET_REGISTER_STEP_1		1

/** \def FX_REGISTER_STEP_1
	\brief 注册至服务器第二步
*/

#define	FX_MAKE_PACKET_REGISTER_STEP_2		2

/** \def FX_MAKE_PACKET_REGISTER_STEP_2
	\brief 注册至服务器第三步
*/
#define FX_MAKE_PACKET_REGISTER_STEP_3		3

/** \def FX_MAKE_PACKET_GET_PERSONAL_INFO
	\brief 制作获得个人信息的数据包
*/

#define FX_MAKE_PACKET_GET_PERSONAL_INFO	4

/** \def FX_MAKE_PACKET_GET_CONTACT
	\brief 制作获得俩联系人列表的数据包
*/

#define FX_MAKE_PACKET_GET_CONTACT			5

/** \def FX_MAKE_PACKET_SEND_SMS_SELF
	\brief 制作发送信息给自己的数据包
*/

#define FX_MAKE_PACKET_SEND_SMS_SELF		6

/** \def FX_MAKE_PACKET_SEND_SMS_SELF
	\brief 制作发送信息给自己的数据包
*/

#define FX_MAKE_PACKET_SEND_SMS				7

/** \def FX_SEND_SMS_SUCCESSED
	\brief 定义发送信息成功服务器返回的信息代号
*/

#define FX_SEND_SMS_SUCCESSED				280

/** \struct _MAKE_PACKET_DATA_STEP2
	\brief 这个是包含respone和cnonce得结构体
	更多请看源代码
*/

typedef struct _MAKE_PACKET_DATA_STEP2
{
	char	sz_response[33];
	char	sz_cnonce[33];

}MAKE_PACKET_DATA_STEP2,*PMAKE_PACKET_DATA_STEP2;

/** \def MAX_SMS_LENGTH	500
	\brief 定义了最大的消息长度
*/

#define MAX_SMS_LENGTH	500

/** \struct _MAKE_PACKET_SEND_SMS
	\brief 这个是发送消息时的联系人信息的结构体
	更多请看源代码
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

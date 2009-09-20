#include <winsock2.h>
#include <curl\curl.h>
#include "libfetion.h"

//
// 结构体
//

/** \struct _struc_get_login_data libftrion.h "libfetion.h"
	\brief 这个结构体是在从服务器返回的信息中提取出来的.
	
	更多的信息请看源代码
*/
typedef struct _struc_get_login_data
{
	int		nstatu_code;
	char	sz_sid[20];
	char	sz_uri[50];
	char	sz_domain[20];
	char	sz_phone_num[20];
	char	sz_user_id[20];
	int		nstatu_user;
	
}GETLOGINDATA,*PGETLOGINDATA;


/** \def FX_LOGIN_URL
	\brief 定义了飞信登陆服务器的网址

*/

#define FX_LOGIN_URL		"http://nav.fetion.com.cn/ssiportal/SSIAppSignIn.aspx"

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

PGETLOGINDATA g_struc_get_login_data;

FX_RET_CODE		fx_parse_xml( FX_PARSE_XML_STEP n_step_code, LPVOID p_xml_data, LPVOID	p_struct );


/*! \file libfetion.h
	\brief 这是libfetion的头文件
*/

#ifndef __LIBFETION__H__
#define __LIBFETION__H__

/** \def FX_ERROR_OK
	\brief 函数成功返回
*/

#define FX_ERROR_OK			0

/** \def FX_ERROR_INIT
	\brief 初始化失败
*/

#define FX_ERROR_INIT		1

/** \def FX_ERROR_SOCKET
	\brief socket操作失败
*/

#define FX_ERROR_SOCKET		2

/** \def FX_ERROR_LOGIN
	\brief 定义登陆失败

*/

#define  FX_ERROR_LOGIN		3

/** \def FX_ERROR_REGISTER
	\brief 定义注册至服务器时失败
*/

#define FX_ERROR_REGISTER	4

#define FX_ERROR_SEND_SMS	5

/** \def FX_RET_CODE
      \brief 用来定义函数返回的结果
*/

#define FX_RET_CODE			int

/** \def FX_PARSE_XML_STEP
	\brief 用来定义是解析怎样的XML
*/

#define FX_PARSE_XML_STEP	int

/** \def FX_PAESE_XML_LOGIN_DATA
	\brief 用来定义解析登录时保存的xml文件
	
*/

#define FX_PAESE_XML_LOGIN_DATA			1

/** \def FX_PARSE_XML_CONTACT_DATA
	\brief 用来解析获得联系人时的xml数据

*/

#define FX_PARSE_XML_CONTACT_DATA		2

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

/** \struct _CONTACT_LIST libftrion.h "libfetion.h"
	\brief 这个结构体是联系人的信息.
	更多的信息请看源代码
*/

struct CONTACT_LIST
{
	char			sz_uri[50];
	char			sz_user_id[20];
	char			sz_local_name[50];
	int				n_group_list_id;
	int				n_relation_status;
	int				n_feike_read_version;
	int				n_online_notify;
	bool			b_mobile;
	CONTACT_LIST*	p_next;
};

/** \struct _CONTACT_LIST libftrion.h "libfetion.h"
	\brief 这个结构体是组成员的信息.
	更多的信息请看源代码
*/

struct GROUP_LIST
{
	int				n_group_id;
	char			sz_group_name[50];
	CONTACT_LIST*	p_contact;	
	GROUP_LIST*		p_next;
};


extern "C" FX_RET_CODE		fx_init();

extern "C" FX_RET_CODE		fx_login( LPTSTR sz_phone_num, LPTSTR sz_password, \
										PGETLOGINDATA p_get_user_data );

extern "C" FX_RET_CODE		fx_get_contact( LPVOID* p_struct );

extern "C" char*			fx_get_sid();

extern "C" FX_RET_CODE		fx_close();

extern "C" LPVOID			fx_get_personal_info();

extern "C" FX_RET_CODE		fx_registe_to_server( char*	sz_server_ip );

extern "C" FX_RET_CODE		fx_send_sms_to_yourself( char* sz_massage );

extern "C" FX_RET_CODE		fx_send_sms_to_others( char* sz_uri, char* sz_massage );

#endif __LIBFETION__H__


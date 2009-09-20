#include <winsock2.h>
#include <curl\curl.h>

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

extern "C" FX_RET_CODE		fx_init();

extern "C" FX_RET_CODE		fx_login( LPTSTR sz_phone_num, LPTSTR sz_password  );

/** \fn FX_RET_CODE		fx_get_contact()
	\brief 这个函数的功能是用于在服务器上获得联系人列表
	\warning 必须安全的使用这个函数
	\return FX_RET_CODE 定义了返回代码

*/
extern "C" FX_RET_CODE		fx_get_contact();

#endif __LIBFETION__H__


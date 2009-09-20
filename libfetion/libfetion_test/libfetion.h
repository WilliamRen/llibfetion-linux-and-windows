#include <winsock2.h>
#include <curl\curl.h>

/*! \file libfetion.h
	\brief ����libfetion��ͷ�ļ�
*/

#ifndef __LIBFETION__H__
#define __LIBFETION__H__

/** \def FX_ERROR_OK
	\brief �����ɹ�����
*/

#define FX_ERROR_OK			0

/** \def FX_ERROR_INIT
	\brief ��ʼ��ʧ��
*/

#define FX_ERROR_INIT		1

/** \def FX_ERROR_SOCKET
	\brief socket����ʧ��
*/

#define FX_ERROR_SOCKET		2

/** \def FX_ERROR_LOGIN
	\brief �����½ʧ��

*/

#define  FX_ERROR_LOGIN		3

/** \def FX_RET_CODE
      \brief �������庯�����صĽ��
*/

#define FX_RET_CODE			int

/** \def FX_PARSE_XML_STEP
	\brief ���������ǽ���������XML
*/

#define FX_PARSE_XML_STEP	int

/** \def FX_PAESE_XML_LOGIN_DATA
	\brief �������������¼ʱ�����xml�ļ�
	
*/

#define FX_PAESE_XML_LOGIN_DATA			1

/** \def FX_PARSE_XML_CONTACT_DATA
	\brief �������������ϵ��ʱ��xml����

*/

#define FX_PARSE_XML_CONTACT_DATA		2

extern "C" FX_RET_CODE		fx_init();

extern "C" FX_RET_CODE		fx_login( LPTSTR sz_phone_num, LPTSTR sz_password  );

/** \fn FX_RET_CODE		fx_get_contact()
	\brief ��������Ĺ����������ڷ������ϻ����ϵ���б�
	\warning ���밲ȫ��ʹ���������
	\return FX_RET_CODE �����˷��ش���

*/
extern "C" FX_RET_CODE		fx_get_contact();

#endif __LIBFETION__H__


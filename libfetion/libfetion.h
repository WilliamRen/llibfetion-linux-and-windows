
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

/** \def FX_ERROR_REGISTER
	\brief ����ע����������ʱʧ��
*/

#define FX_ERROR_REGISTER	4

#define FX_ERROR_SEND_SMS	5

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

//
// �ṹ��
//

/** \struct _struc_get_login_data libftrion.h "libfetion.h"
	\brief ����ṹ�����ڴӷ��������ص���Ϣ����ȡ������.
	
	�������Ϣ�뿴Դ����
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
	\brief ����ṹ������ϵ�˵���Ϣ.
	�������Ϣ�뿴Դ����
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
	\brief ����ṹ�������Ա����Ϣ.
	�������Ϣ�뿴Դ����
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


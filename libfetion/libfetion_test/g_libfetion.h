#include <winsock2.h>
#include <curl\curl.h>
#include "libfetion.h"

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


/** \def FX_LOGIN_URL
	\brief �����˷��ŵ�½����������ַ

*/

#define FX_LOGIN_URL		"http://nav.fetion.com.cn/ssiportal/SSIAppSignIn.aspx"

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

PGETLOGINDATA g_struc_get_login_data;

FX_RET_CODE		fx_parse_xml( FX_PARSE_XML_STEP n_step_code, LPVOID p_xml_data, LPVOID	p_struct );

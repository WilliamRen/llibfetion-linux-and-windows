/*++

Copyright (c) 2009  PG-Soft LAB 


File Name:

	libfetion_test.cpp
			
Abstract:

	libfetion���Գ���

Author:

	ProgrammeBoy       ProgrammeBoy@163.com         8/6/2009


Revision History:

	None

--*/

#include <stdio.h>

/*//
// �������mfc֧��
//

#include <afx.h>*/
#include <windows.h>
#include "..\\libfetion.h"

//
// ʹ���Լ�д��libfetion��
//

#pragma comment ( lib , "..\\Debug\\libfetion.lib" )
void print_all();
GROUP_LIST* g_p_link = NULL;

void main()
{
	FX_RET_CODE		ret = 0;
	
	//
	// ��ʼ��libfetion
	//
	
	ret = fx_init();
	if ( ret != FX_ERROR_OK )
	{
		printf( "initialize libfetion error!\n" );
		return;
	}
	
	//
	// �õ���¼ʱ���������ص���Ϣ
	//
	
	PGETLOGINDATA	p_get_user_data = new GETLOGINDATA;
	memset( p_get_user_data, 0, sizeof( GETLOGINDATA ) );
	fx_login( "�������ֻ���", "����������", p_get_user_data );
	
	//
	// ע����������
	//
	
	ret = fx_registe_to_server( "221.176.31.33" );
	if ( ret != FX_ERROR_OK )
	{
		printf( "register to server error!\n" );
		return;
	}
	printf( "register OK!\n" );
	
	//
	// �ӷ������õ����˵���Ϣ
	//
	
	char* sz_personal_info = (char*)fx_get_personal_info();
	if ( sz_personal_info == NULL )
	{
		printf( "get personal info error!\n" );
		fx_close();
		return;
	}
	printf( "personal info = %s\n", sz_personal_info );
	
	//
	// �õ���ϵ���б�
	//
	
	ret = fx_get_contact( (LPVOID*)&g_p_link );
	if ( ret != FX_ERROR_OK || g_p_link == NULL )
	{
		printf( "get contact error!\n" );
		fx_close();
		return;
	}
	
	//
	// ��ӡ����ϵ���б�
	//
	
	print_all();
	
	//
	// �����Ÿ��Լ�
	//

	ret = fx_send_sms_to_yourself( "�ҵ�" );
	if ( ret != FX_ERROR_OK )
	{
		printf( "send SMS to myself error!\n" );
		fx_close();
		return;
	}
	printf( "send SMS to myself ok!\n" );

	
	//
	// �����Ÿ�����..
	//
	
	ret = fx_send_sms_to_others( "sip:879534138@fetion.com.cn;p=12203", \
								 "���Ǹ����Ų���.!�ٺ�" );
	if ( ret != FX_ERROR_OK )
	{
		printf( "send SMS to others error!\n" );
		fx_close();
		return;
	}
	printf( "send SMS to others ok!\n" );
	
	//
	// �ر�libfetion
	//
	
	fx_close();
}

//
// ��ӡ����ϵ���б�
//

void print_all()
/*++

Routine Description:

	������g_p_link�д�ӡ����ϵ�˵���Ϣ

Arguments:

	None

Return Value:

	None

--*/
{
	if ( g_p_link == NULL )
		return;
	GROUP_LIST* p_group = g_p_link;
	while ( p_group )
	{
		printf( "group id : %d   group name : %s\n", \
			p_group->n_group_id, \
			p_group->sz_group_name );
		CONTACT_LIST* p_contact = p_group->p_contact; 
		while ( p_contact )
		{
			
			if ( p_contact->b_mobile == true )
			{
				printf( "\t\turi = %s\n", p_contact->sz_uri );
				printf( "\t\tuser-id = %s\n", p_contact->sz_user_id );
				printf( "\t\tlocal-name = %s\n", p_contact->sz_local_name );
				printf( "\t\trelation-status = %d\n", p_contact->n_relation_status );
				printf( "\t\tonline-notify = %d\n", p_contact->n_online_notify );
			}
			else
			{
				printf( "\t\turi = %s\n", p_contact->sz_uri );
				printf( "\t\tuser-id = %s\n", p_contact->sz_user_id );
				printf( "\t\tlocal-name = %s\n", p_contact->sz_local_name );
				printf( "\t\trelation-status = %d\n", p_contact->n_relation_status );
				printf( "\t\tfeike-read-version = %d\n", p_contact->n_feike_read_version );
				printf( "\t\tonline-notify = %d\n", p_contact->n_online_notify );
			}
			p_contact = p_contact->p_next;
		}
		p_group = p_group->p_next;
	}
}
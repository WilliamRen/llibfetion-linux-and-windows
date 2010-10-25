#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../libsip/include/sip_def.h"
#include "../libsip/include/sip_cseq.h"
#include "../libsip/include/sip_to.h"
#include "../libsip/include/sip_authorization.h"
#include "../libsip/include/sip_www_authenticate.h"
#include "../libsip/include/sip_startline.h"
#include "../libsip/include/sip_client.h"

#include "../libsip/include/sip_message.h"

#define __in
#define __out


#define TEST_URL "sip:588955498@fetion.com.cn;p=737"
#define TEST_CSEQ "12 CN"
#define TEST_AUTH "Digest algorithm=\"SHA1-sess-v4\",response=\"67d4703ba6a1cb\""
#define TEST_W "Digest algorithm=\"SHA1-sess-v4\",nonce=\"660702E04DB7BC69666436271C4D9F7B\",key=\"AD3D7038\",signature=\"D528EC02689AD6D4656CC\""
#define TEST_HEAD "R fetion.com.cn SIP-C/4.0"
#define TEST_HEAD1 "SIP-C/4.0 401 Unauthoried"
#define TEST_CLIENT "type=\"pc\",version=\"4.0.2510\""

#define TEST_MESSAGE  "SIP-C/4.0 401 Unauthoried\r\n" \
                      "F: 879534138\r\n" \
                      "I: 1\r\n" \
					  "K: 11\r\n" \
					  "K: 22\r\n" \
					  "K: 33\r\n" \
					  "C: text/plain\r\n" \
					  "N: CatMsg\r\n" \
                      "CN: 123456\r\n" \
                      "Q: 1 R\r\n" \
					  "D: oct, 2010 01:01:01\r\n" \
					  "AL: daskds\r\n" \
                      "W: Digest algorithm=\"SHA1-sess-v4\",nonce=\"660702E04DB7BC69666436271C4D9F7B\",key=\"AD3D7038\",signature=\"D528E11\"\r\n" \
                      "A: TICKS auth=\"2025669365.557049008\"\r\n" \
					  "\r\n" \
					  "SIP-C/4.0 401 Unauthoried\r\n" \
					  "F: 879534138\r\n" \
					  "CN: 123456\r\n" \
					  "Q: 1 R\r\n" \
					  "\r\n"



typedef struct _auth_dlg_helper
{
	int n_callid;
	int n_cseq;
	char phone_num[20];
	char user_id[20];
	char user_pwd[20];
	char machine_code[32];

}AUTH_DLG_HELPER, *PAUTH_DLG_HELPER;

char* fx_generate_cnonce()
{
	char* sz_conce = (char*)malloc( 33 );
	srand( (unsigned)time( NULL ) );
	memset( sz_conce, 0, 33 );
	sprintf( sz_conce, "%04X%04X%04X%04X%04X%04X%04X%04X", \
		rand() & 0xFFFF, rand() & 0xFFFF,\
		rand() & 0xFFFF, rand() & 0xFFFF,\
		rand() & 0xFFFF, rand() & 0xFFFF,\
		rand() & 0xFFFF, rand() & 0xFFFF );
	return sz_conce;
}


int fx_sip_generate_auth_req( __in PAUTH_DLG_HELPER p_auth_helper, __out char** auth_req)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_cnonce_t* cnonce = NULL;
	sip_client_t*	client = NULL;
	sip_support_list_t* support_list = NULL;
	sip_support_t* support = NULL;
	sip_message_t* message = NULL;
	char sz_num[10] = {0};
	char* sz_tmp = NULL;
	int n_ret = 0;


	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "R", "fetion.com.cn" );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_auth_helper->user_id );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

	//itoa( p_auth_helper->n_callid, sz_num, 10 );
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_auth_helper->n_cseq, "R" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	cnonce
	 */

	sz_tmp = fx_generate_cnonce();
	sip_common_init( &cnonce );
	sip_common_set_all( cnonce, sz_tmp );
	free( sz_tmp );
	sip_message_set_common( &(message->cnonce), cnonce );

	/*
	 *	TODO: support list
	 */

	support_list = (sip_support_list_t*)sip_malloc( sizeof(sip_support_list_t) );
	sip_common_init( &support );
	sip_common_set_all( support, "SaveHistory" );
	support_list->sip_k = support;
	support_list->next = NULL;
	message->support_list = support_list;

	/*
	 *	client
	 */

	sip_client_init( &client );
	sip_client_set_all( client, "pc", "4.0.2510" );
	sip_message_set_client( message, client );

	n_ret = sip_message_to_str( message, auth_req );

	sip_message_free( message );

	/*
	 *	add the sceq
	 */

	p_auth_helper->n_cseq++;

	if ( n_ret == LIBSIP_SUCCESS )
		return 0;
	else
		return -1;

}

#define SIP_AUTH_BODY	"<args>" \
	"<device accept-language=\"default\" machine-code=\"%s\" />" \
	"<caps value=\"3FF\" />" \
	"<events value=\"7F\" />" \
	"<user-info mobile-no=\"%s\" user-id=\"%s\">" \
	"<personal version=\"0\" attributes=\"v4default;alv2-version;alv2-warn\" />"\
	"<custom-config version=\"0\" />"\
	"<contact-list version=\"0\" buddy-attributes=\"v4default\" />" \
	"</user-info>" \
	"<credentials domains=\"fetion.com.cn;m161.com.cn;www.ikuwa.cn;games.fetion.com.cn;turn.fetion.com.cn\" />" \
						"<presence><basic value=\"400\" desc=\"\" /><extendeds /></presence></args>"

int fx_sip_generate_auth_resp( __in PAUTH_DLG_HELPER p_auth_helper,
							   __in char* key, __in char* nonce,
							   __out char** auth_req)
{
	sip_startline_t* start_line = NULL;
	sip_from_t* from = NULL;
	sip_call_id_t* call_id = NULL;
	sip_cseq_t* cseq = NULL;
	sip_authorization_t* authorization = NULL;
	sip_context_length_t* context_len = NULL;
	sip_message_t* message = NULL;
	char* sz_response = NULL;
	int n_ret = 0;
	char sz_body[1024] = {0}, sz_num[10] = {0};

	/*
	 *	init message
	 */

	sip_message_init( &message );

	/*
	 *	start line
	 */

	sip_startline_init( &start_line );
	sip_start_set_line_req_all( start_line, "R", "fetion.com.cn" );
	sip_message_set_startline( message, start_line );

	/*
	 *	from
	 */

	sip_common_init( &from );
	sip_common_set_all( from, p_auth_helper->user_id );
	sip_message_set_common( &(message->from), from );

	/*
	 *	call id
	 */

//	itoa( p_auth_helper->n_callid, sz_num, 10 );
	sip_common_init( &call_id );
	sip_common_set_all( call_id, sz_num );
	sip_message_set_common( &(message->call_id), call_id );

	/*
	 *	cseq
	 */

	sip_cseq_init( &cseq );
	sip_cseq_set_all( cseq, p_auth_helper->n_cseq, "R" );
	sip_message_set_cseq( message, cseq );

	/*
	 *	authorization
	 */

		/*
		 *	get response
		 */

	/*n_ret = fx_generate_response( key, nonce, p_auth_helper->user_id, \
								p_auth_helper->user_pwd, &sz_response );
	if ( n_ret != FX_ERROR_OK )
	{
		sip_message_free( message );
		return n_ret;
	}*/

		/*
		 *	do generate authorization
		 */

	sip_authorization_init( &authorization );
	sip_authorization_set_digest_all( authorization, "217237473437473423923928392839283929329121", "SHA1-sess-v4" );
	sip_message_set_authorization( message, authorization );

	/*
	 *	body
	 */

	sprintf( sz_body, SIP_AUTH_BODY, p_auth_helper->machine_code, \
				p_auth_helper->phone_num, p_auth_helper->user_id );

	sip_message_set_body( message, sz_body );

	n_ret = sip_message_to_str( message, auth_req );

	sip_message_free( message );

	/*
	 *	add the sceq
	 */

	p_auth_helper->n_cseq++;

	if ( n_ret == LIBSIP_SUCCESS )
		return 0;
	else
		return -1;
}

int main()
{

    sip_message_t* msg;
    char* sz_test = NULL;

	int n_ret = 0;

    //msg = (sip_message_t*)sip_malloc( sizeof( sip_message_t ) );

	sip_message_init( &msg );


	n_ret = sip_message_parse( msg, TEST_MESSAGE );
    if ( !LIBSIP_IS_SUCCESS( n_ret ) )

    {

		printf( "parse error!\n" );

		return 0;

    }
//
//
// 	if ( n_ret == LIBSIP_BODY_BEYOND )
//
// 	{
//
// // 		int n_head_len  = 0, n_body_len = 0, n_next = 0;
//
// // 		char* sz_ctrllflf = NULL, *sz_next_msg = NULL;
//
// // 		char* sz_tmp_recv = TEST_MESSAGE;
//
// //
//
// //
//
// // 		/*
//
// // 		 *	这里肯定能得到,不然sip_message_parse就错了
//
// // 		 */
//
// //
//
// // 		sz_ctrllflf = strstr( sz_tmp_recv, CTRLLFLF );
//
// // 		n_head_len = strlen( sz_tmp_recv ) - strlen( sz_ctrllflf ) + 4/*for CTRLLFLF*/;
//
// //
//
// // 		n_body_len = sip_message_get_body_length( msg );
//
// //
//
// // 		n_next = strlen( sz_tmp_recv ) - n_head_len - n_body_len;
//
// // 		sz_next_msg = (char*)malloc( n_next + 1 );
//
// // 		memset( sz_next_msg, 0, n_next + 1 );
//
// //
//
// // 		/*
//
// // 		 *	set the next msg buffer
//
// // 		 */
//
// //
//
// // 		strcpy( sz_next_msg, sz_tmp_recv + n_head_len + n_body_len );
//
//
//
// 	}
//
//
//
// 	printf( "len = %d \n", sip_message_get_body_length( msg ) );
//
//
    sip_message_to_str( msg, &sz_test );

    printf( "%s\n", sz_test );


	sip_message_free( msg );


    sip_free( sz_test );
// 	PAUTH_DLG_HELPER p_auth_helper = (PAUTH_DLG_HELPER)malloc( sizeof(AUTH_DLG_HELPER) );
// 	char* sz_pack = NULL;
//
// 	memset( p_auth_helper, 0, sizeof(AUTH_DLG_HELPER) );
// 	strcpy( p_auth_helper->machine_code, "2F6E7CD33AA1F6928E69DEDD7D6C50B1" );
// 	strcpy( p_auth_helper->phone_num, "15210281153" );
// 	strcpy( p_auth_helper->user_id, "123456" );
// 	strcpy( p_auth_helper->user_pwd, "5211314" );
// 	p_auth_helper->n_callid = 1;
// 	p_auth_helper->n_cseq = 1;
// 	if ( fx_sip_generate_auth_req( p_auth_helper, &sz_pack ) != 0 )
// 	{
// 		return -1;
// 	}
// 	sip_free( sz_pack );
// 	if ( fx_sip_generate_auth_resp( p_auth_helper, "213283273617362732", "126728367826473243", &sz_pack ) != 0 )
// 	{
// 		return -1;
// 	}
// 	sip_free( sz_pack );
    return 0;
}

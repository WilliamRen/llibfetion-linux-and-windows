#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "libsip/include/sip_def.h"
#include "libsip/include/sip_cseq.h"
#include "libsip/include/sip_to.h"
#include "libsip/include/sip_authorization.h"
#include "libsip/include/sip_www_authenticate.h"
#include "libsip/include/sip_startline.h"
#include "libsip/include/sip_client.h"

#include "libsip/include/sip_message.h"

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
                      "CN: 123456\r\n" \
                      "Q: 1 R\r\n" \
                      "W: Digest algorithm=\"SHA1-sess-v4\",nonce=\"660702E04DB7BC69666436271C4D9F7B\",key=\"AD3D7038\",signature=\"D528E11\"\r\n" \
                      "A: TICKS auth=\"2025669365.557049008\"\r\n" \
					  "\r\n" \
					  "SIP-C/4.0 401 Unauthoried\r\n" \
					  "F: 879534138\r\n" \
					  "CN: 123456\r\n" \
					  "Q: 1 R\r\n" \
					  "\r\n"



int main()
{
    /*sip_to_t* to;
    char* sz_str = NULL;

    sip_to_init( &to );
    sip_to_parse( to, TEST_URL );

    printf( "uid=%s\nhost=%s\np=%s\n", to->uid, to->host, to->p_value );

    if( LIBSIP_SUCCESS == sip_to_to_str( to, &sz_str )){

        printf( "str=%s\n", sz_str );
        sip_free( sz_str );
    }
    sip_to_free( to );*/
    /*sip_cseq_t* cseq;
    char* sz_str = NULL;

    sip_cseq_init( &cseq );
    sip_cseq_parse( cseq, TEST_CSEQ );

    printf( "number=%d\nmethod=%s\n", cseq->number, cseq->method );

    if( LIBSIP_SUCCESS == sip_cseq_to_str( cseq, &sz_str )){
        printf( "str=%s\n", sz_str );
        sip_free( sz_str );
    }
    sip_cseq_free( cseq );*/
    /*sip_authorization_t* authorization;
    char* sz_str = NULL;

    sip_authorization_init( &authorization );
    sip_authorization_parse( authorization, TEST_AUTH );

    printf( "type=%s\nalgorithm=%s\nresponse=%s\n", \
            authorization->auth_type, authorization->algorithm, \
            authorization->response );

    if( LIBSIP_SUCCESS == sip_authorization_to_str( authorization, &sz_str )){
        printf( "str=%s\n", sz_str );
        sip_free( sz_str );
    }
    sip_authorization_free( authorization );*/
    /*sip_www_authenticate_t* www_authenticate;
    char* sz_str = NULL;

    sip_www_authenticate_init( &www_authenticate );
    sip_www_authenticate_parse( www_authenticate, TEST_W );

    printf( "type=%s\nalgorithm=%s\nnonce=%s\nkey=%s\nsignature=%s\n", \
            www_authenticate->auth_type, www_authenticate->algorithm, \
            www_authenticate->nonce, www_authenticate->key, \
            www_authenticate->signature );

    if( LIBSIP_SUCCESS == sip_www_authenticate_to_str( www_authenticate, &sz_str )){
        printf( "str=%s\n", sz_str );
        sip_free( sz_str );
    }
    sip_www_authenticate_free( www_authenticate );*/

    /*sip_startline_t* start;
    char* sz_str = NULL;

    sip_startline_init( &start );

    sip_startline_parse( start, TEST_HEAD1 );

    printf( "version=%s\nmethod=%s\ncode=%s\ndesc=%s\nhost=%s\n", \
            start->version, start->method, start->status_code, \
            start->status_desc, start->host );
    if( LIBSIP_SUCCESS == sip_startline_to_str( start, &sz_str )){
        printf( "str=%s\n", sz_str );
        sip_free( sz_str );
    }
    sip_startline_free( start );*/
    /*sip_client_t* client;
    char* sz_str = NULL;

    sip_client_init( &client );

    sip_client_parse( client, TEST_CLIENT );

    printf( "version=%s\ntype=%s\n", client->version, client->type );
    if( LIBSIP_SUCCESS == sip_client_to_str( client, &sz_str )){
        printf( "str=%s\n", sz_str );
        sip_free( sz_str );
    }
    sip_client_free( client );*/

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
	
	if ( n_ret == LIBSIP_BODY_BEYOND )
	{
					
// 		int n_head_len  = 0, n_body_len = 0, n_next = 0;
// 		char* sz_ctrllflf = NULL, *sz_next_msg = NULL;
// 		char* sz_tmp_recv = TEST_MESSAGE;
// 
// 		
// 		/*
// 		 *	这里肯定能得到,不然sip_message_parse就错了
// 		 */
// 		
// 		sz_ctrllflf = strstr( sz_tmp_recv, CTRLLFLF );
// 		n_head_len = strlen( sz_tmp_recv ) - strlen( sz_ctrllflf ) + 4/*for CTRLLFLF*/;
// 
// 		n_body_len = sip_message_get_body_length( msg );
// 		
// 		n_next = strlen( sz_tmp_recv ) - n_head_len - n_body_len;
// 		sz_next_msg = (char*)malloc( n_next + 1 );
// 		memset( sz_next_msg, 0, n_next + 1 );
// 		
// 		/*
// 		 *	set the next msg buffer
// 		 */
// 		
// 		strcpy( sz_next_msg, sz_tmp_recv + n_head_len + n_body_len );

	}

	printf( "len = %d \n", sip_message_get_body_length( msg ) );

    sip_message_to_str( msg, &sz_test );
	
    printf( "%s\n", sz_test );

	sip_message_free( msg );

    sip_free( sz_test );

    return 0;
}

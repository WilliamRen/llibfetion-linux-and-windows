
#include <stdio.h>
#include <stdlib.h>

#include "sip_cseq.h"
#include "sip_parser.h"

#define TEST_A "Digest algorithm=\"SHA1-sess-v4\",response=\"67d470c29dd561efe467c400d17e69111378e6478089c6208633f6aefb2bed11b08084535a2c396a4a30651ac3598f96dbd5c0419ffb4a82684225931cddd75ec4d70d4038408ec63bc5833150d60eeabb51263ba6a1cbd4fd1d4fc36234bafe2284dbb7a274a09d2ccde5c98fe2980a757e92ff0b385af150c667973dafbd95\""
#define TEST_W "Digest algorithm=\"SHA1-sess-v4\",nonce=\"660702E04DB7BC69666436271C4D9F7B\",key=\"AD3D7038\",signature=\"D528EC02689AD6D4656CC\""
#define TEST_URL "sip:588955498@fetion.com.cn;p=737"
#define TEST_FORM "15565345"
#define TEST_TO "sip:588955498@fetion.com.cn;p=737"
#define TEST_CALL_ID "12"

#define TEST_MESSAGE  "SIP-C/4.0 401 Unauthoried\r\n"\
                      "F: 879534138\r\n"\
                      "I: 1\r\n"\
                      "CN: 123456\r\n" \
                      "Q: 1 R\r\n"\
                      "W: Digest algorithm=\"SHA1-sess-v4\",nonce=\"660702E04DB7BC69666436271C4D9F7B\",key=\"AD3D7038\",signature=\"D528E11\"\r\n"\
                      "L: 13\r\n" \
                      "\r\n\r\n"\
                      "<test></test>"
int main()
{
    /* test for "A"
    char sz[] = TEST;
    char* sz_str = NULL;

    osip_authorization_t* p_sipauth = NULL;

    osip_authorization_init( &p_sipauth );

    osip_authorization_parse( p_sipauth, sz );

    printf( "response %s\n", p_sipauth->response );
    printf( "type %s\n", p_sipauth->auth_type );
    printf( "algorithm %s\n", p_sipauth->algorithm );

    if( OSIP_SUCCESS == osip_authorization_to_str( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }*/

    /* Test for "W"
    char sz[] = TEST_W;
    char* sz_str = NULL;

    osip_www_authenticate_t* p_sipauth = NULL;
    osip_www_authenticate_init( &p_sipauth );

    osip_www_authenticate_parse( p_sipauth, sz );

    printf( "key %s\n", p_sipauth->key );
    printf( "signature %s\n", p_sipauth->signature );
    printf( "type %s\n", p_sipauth->auth_type );
    printf( "algorithm %s\n", p_sipauth->algorithm );

    if( OSIP_SUCCESS == osip_www_authenticate_to_str( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }*/
    /*TEST_URL
    char sz[] = TEST_URL;
    char* sz_str = NULL;

    osip_uri_t* p_sipauth = NULL;
    osip_uri_init( &p_sipauth );

    osip_uri_parse( p_sipauth, sz );

    printf( "scheme %s\n", p_sipauth->scheme );
    printf( "username %s\n", p_sipauth->username );
    printf( "password %s\n", p_sipauth->password );
    printf( "host %s\n", p_sipauth->host );
    printf( "port %s\n", p_sipauth->port );


    if( OSIP_SUCCESS == osip_uri_to_str( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }
    */

    /*TEST for "F\CN"
    char sz[] = TEST_FORM;
    char* sz_str = NULL;

    osip_from_c_t* p_sipauth = NULL;
    osip_from_init_c( &p_sipauth );

    osip_from_parse_c( p_sipauth, sz );

    printf( "uid %s\n", p_sipauth->uid );

    if( OSIP_SUCCESS == osip_from_to_str_c( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }*/
    /*TEST "T"
    char sz[] = TEST_TO;
    char* sz_str = NULL;

    osip_to_t* p_sipauth = NULL;
    osip_to_init( &p_sipauth );

    osip_to_parse( p_sipauth, sz );

    printf( "display name %s\n", p_sipauth->displayname );
    printf( "scheme %s\n", p_sipauth->url->scheme );
    printf( "username %s\n", p_sipauth->url->username );
    printf( "password %s\n", p_sipauth->url->password );
    printf( "host %s\n", p_sipauth->url->host );
    printf( "port %s\n", p_sipauth->url->port );


    if( OSIP_SUCCESS == osip_to_to_str( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }*/

    /*TEST "I"
    char sz[] = TEST_CALL_ID;
    char* sz_str = NULL;

    osip_call_id_t* p_sipauth = NULL;
    osip_call_id_init( &p_sipauth );

    osip_call_id_parse( p_sipauth, sz );

    printf( "number %s\n", p_sipauth->number );
    printf( "host %s\n", p_sipauth->host );

    if( OSIP_SUCCESS == osip_call_id_to_str( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }*/

    char sz[] = TEST_MESSAGE;
    char* sz_str = NULL;
    size_t len = 0;

    parser_init();

    osip_message_t* p_sipauth = NULL;
    osip_message_init( &p_sipauth );

    osip_message_parse( p_sipauth, sz, strlen(TEST_MESSAGE) );

    //printf( "number %s\n", p_sipauth->number );
    //printf( "host %s\n", p_sipauth->host );

    if( OSIP_SUCCESS == osip_message_to_str( p_sipauth, &sz_str, &len )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }

    return 0;
}

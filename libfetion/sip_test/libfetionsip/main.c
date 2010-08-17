
#include <stdio.h>
#include <stdlib.h>

#include "sip_cseq.h"
#include "sip_parser.h"

#define TEST_A "Digest algorithm=\"SHA1-sess-v4\",response=\"67d470c29dd561efe467c400d17e69111378e6478089c6208633f6aefb2bed11b08084535a2c396a4a30651ac3598f96dbd5c0419ffb4a82684225931cddd75ec4d70d4038408ec63bc5833150d60eeabb51263ba6a1cbd4fd1d4fc36234bafe2284dbb7a274a09d2ccde5c98fe2980a757e92ff0b385af150c667973dafbd95\""
#define TEST_W "Digest algorithm=\"SHA1-sess-v4\",nonce=\"660702E04DB7BC69666436271C4D9F7B\",key=\"AD3D7038\",signature=\"D528EC02689AD6D4656CC\""
#define TEST_URL "sip:588955498@fetion.com.cn;p=737"
#define TEST_FORM "15565345"
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

    char sz[] = TEST_FORM;
    char* sz_str = NULL;

    osip_from_t* p_sipauth = NULL;
    osip_from_init( &p_sipauth );

    osip_from_parse( p_sipauth, sz );

    //printf( "scheme %s\n", p_sipauth->scheme );
    //printf( "username %s\n", p_sipauth->username );
    //printf( "password %s\n", p_sipauth->password );
    //printf( "host %s\n", p_sipauth->host );
    printf( "displayname %s\n", p_sipauth->displayname );

    if( OSIP_SUCCESS == osip_from_to_str( p_sipauth, &sz_str )){
        printf( "str = %s\n", sz_str );
        osip_free( sz_str );
    }
    return 0;
}

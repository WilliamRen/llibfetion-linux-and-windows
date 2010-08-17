
#include <stdio.h>
#include <stdlib.h>

#include "sip_cseq.h"
#include "sip_parser.h"

#define TEST "Digest algorithm=\"SHA1-sess-v4\",response=\"67d470c29dd561efe467c400d17e69111378e6478089c6208633f6aefb2bed11b08084535a2c396a4a30651ac3598f96dbd5c0419ffb4a82684225931cddd75ec4d70d4038408ec63bc5833150d60eeabb51263ba6a1cbd4fd1d4fc36234bafe2284dbb7a274a09d2ccde5c98fe2980a757e92ff0b385af150c667973dafbd95\""
int main()
{
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
    }

    return 0;
}


/*! \file protocol.c
 *  \brief 里面封装了sip协议
 */

#include <stdio.h>

#include <openssl/sha.h>

#include "config.h"
#include "fxsocket.h"
#include "login.h"
#include "protocol.h"

#define FX_MAX_PACKAGE_SIZE 1024

#define  MAKE_LABLE_RM  "%c fetion.com.cn SIP-C/2.0\r\n"
#define  MAKE_LABLE_NUM "%c: %d\r\n"
#define  MAKE_LABLE_Q   "Q: %d %c\r\n"
#define  MAKE_LABLE_X   "%c: %s\r\n"

int fx_get_next_call()
{
	static	int	n_call = 0;
	return ++n_call;
}

char* fx_pro_build_package( int n_type, void* l_data )
{
    static int n_call_id;
    char* sz_pack = (char*)malloc(FX_MAX_PACKAGE_SIZE);

    memset( sz_pack, 0, FX_MAX_PACKAGE_SIZE );
    /*make package*/
    switch ( n_type )
    {
        //struct login_data* data = (struct login_data*)l_data;
        char sz_tmp[512] = {0};
    	case FX_BUILD_LOGIN_1:
            {
                n_call_id = fx_get_next_call();
                /*--1--*/
                sprintf( sz_tmp, MAKE_LABLE_RM, 'R' );
                strcat( sz_pack, sz_tmp );
                /*--2--*/
                sprintf( sz_tmp, MAKE_LABLE_X, 'F', ((struct login_data*)l_data)->sz_uri );
                strcat( sz_pack, sz_tmp );
                /*--3--*/
                sprintf( sz_tmp, MAKE_LABLE_NUM, 'I', n_call_id );
                strcat( sz_pack, sz_tmp );
                /*--4--*/
                sprintf( sz_tmp, MAKE_LABLE_Q, 1, 'R' );
                strcat( sz_pack, sz_tmp );
                /*--5--*/
                sprintf( sz_tmp, MAKE_LABLE_NUM, 'L', strlen(FX_LOGIN_DATA) );
                strcat( sz_pack, sz_tmp );
                strcat( sz_pack, "\r\n" );
                /*--6--*/
                strcat( sz_pack, FX_LOGIN_DATA );
            }
    		break;
        case FX_BUILD_LOGIN_2:
            break;
        case FX_BUILD_GET_PERSON_INFO:
            break;
    	default:
    		break;
    }
    return sz_pack;
}

char* fx_ssi_get_v4digest_1( char* sz_password )
{
    char sz_encode_src[128] = {0};
    SHA_CTX sc;
    int i = 0;
    unsigned char hash[20] = {0};
    char* szSHA1 = (char*)malloc(SHA1_DIGESTSIZE*2+2);

    memset( szSHA1, 0, SHA1_DIGESTSIZE*2+2 );
    sprintf( sz_encode_src, FX_SSI_V4DIGEST_1, sz_password );

    /*
     *  do hash
     */

    SHA1_Init( &sc );
    SHA1_Update( &sc, sz_encode_src, strlen( sz_encode_src ));
    SHA1_Final(hash, &sc);

    /*
     *  format resoult
     */

    for (i=0; i < 20; i++){
        char sz_num[5] = {0};
        sprintf ( sz_num, "%02X", (int)hash[i]);
        strcat( szSHA1, sz_num );
    }
    if( strlen( szSHA1 ) == 0 ){
            free( szSHA1 );
            return NULL;
    }
    else
        return szSHA1;
}

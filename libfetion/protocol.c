
/*! \file protocol.c
 *  \brief 里面封装了sip协议
 */

#include <stdio.h>
#include <time.h>

#include <openssl/sha.h>
#include <openssl/rsa.h>

#include "config.h"
#include "fxsocket.h"
#include "login.h"
#include "protocol.h"

#define FX_MAX_PACKAGE_SIZE 1024

#define  MAKE_LABLE_RM  "%c fetion.com.cn SIP-C/4.0\r\n"
#define  MAKE_LABLE_NUM "%c: %d\r\n"
#define  MAKE_LABLE_Q   "Q: %d %c\r\n"
#define  MAKE_LABLE_X   "%c: %s\r\n"
#define  MAKE_LABLE_CN  "CN: %s\r\n"
#define  MAKE_LABLE_CL  "CL: type=\"pc\",version=\"4.0.2510\"\r\n"

char g_pw_v4[30] = {0};

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
                char* sz_cnonce = fx_generate_cnonce();
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
                sprintf( sz_tmp, MAKE_LABLE_CN, sz_cnonce );
                strcat( sz_pack, sz_tmp );
                free( sz_cnonce );
                /*--6--*/
                //sprintf( sz_tmp, MAKE_LABLE_NUM, 'L', strlen(FX_LOGIN_DATA) );
                strcat( sz_pack, MAKE_LABLE_CL );
                strcat( sz_pack, "\r\n" );
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
    else{
        strcpy( g_pw_v4, szSHA1 );
        return szSHA1;
    }
}

int fx_generate_response( char* sz_key, char* sz_nonce, char* sz_ase_key )
{
    char sz_module[256+1] = {0};
    char sz_exponent[6+1] = {0};
    char sz_SHA1[1024] ={0};

    if( sz_key == NULL || sz_nonce == NULL || sz_ase_key == NULL )
        return -1;
    /*first we should get the moudle and the publicExponent*/
    if( strlen( sz_key ) != 262 )
        return -1;

    memcpy( sz_module, sz_key, 256 );
    memcpy( sz_exponent, sz_key+256, 6 );

    /*do RSA*/
    {
        int ret = 0, flen = 0;
        BIGNUM *bnn, *bne, *bnd;
        unsigned char in[1024] = {0};
        unsigned char *out;

        sprintf( in, "%s%s%s", g_pw_v4, sz_nonce, sz_ase_key );

         bnn = BN_new();
         bne = BN_new();
        // bnd = BN_new();
         BN_hex2bn(&bnn, sz_module);
         BN_set_word(bne, sz_exponent);
        // BN_hex2bn(&bnd, PRIVATE_EXPONENT);

         RSA *r = RSA_new();
         r->n = bnn;
         r->e = bne;
        // r->d = bnd;

         flen = RSA_size(r);

         out = (char *)malloc(flen);
         bzero(out, flen);

         ret = RSA_public_encrypt(flen, in, out, r,   RSA_NO_PADDING);
         if (ret < 0)
         {
            printf("Encrypt failed!\n");
            return 1;
         }

         //printf("Size:%d\n", ret);
         //printf("ClearText:%s\n", in);
         //printf("CipherText(Hex):\n");
         int i;
         for (i=0; i<ret; i++)
         {
            char sz_num[5] = {0};
            sprintf ( sz_num, "%02X", (int)out[i]);
            strcat( sz_SHA1, sz_num );
         }
         printf("%s\n", sz_SHA1);
         log_string( sz_SHA1 );
        //free(out);

         RSA_free(r);
    }

    return 0;
}

char* fx_generate_cnonce()
{
	srand( (unsigned)time( NULL ) );
	char* sz_conce = (char*)malloc( 33 );
	memset( sz_conce, 0, 33 );
	sprintf( sz_conce, "%04X%04X%04X%04X%04X%04X%04X%04X", \
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF,\
				rand() & 0xFFFF, rand() & 0xFFFF );
	return sz_conce;
}

char* fx_get_nonce( char* sz_data )
{
    char* sz_find = NULL;
    if( NULL == sz_data )
        return NULL;
    sz_find = strstr( sz_data, "nonce" );
	char* sz_find_nonce = (char*)malloc(50);
	char* sz_nonce_temp = sz_find_nonce;
	memset( sz_find_nonce, 0, 50 );
	if ( sz_find != NULL )
	{
		sz_find += ( strlen( "nonce=\"" ));
		while ( *sz_find != '\"' )
		{
			*sz_nonce_temp = *sz_find;
			sz_nonce_temp++;
			sz_find++;
		}
		*sz_nonce_temp = '\0';
		return sz_find_nonce;
	}
	else
		return NULL;
}

char* fx_get_key( char* sz_data )
{
    char* sz_find = NULL;
    if( NULL == sz_data )
        return NULL;
    sz_find = strstr( sz_data, "key" );
	char* sz_find_nonce = (char*)malloc(512);
	char* sz_nonce_temp = sz_find_nonce;
	memset( sz_find_nonce, 0, 512 );
	if ( sz_find != NULL )
	{
		sz_find += ( strlen( "key=\"" ));
		while ( *sz_find != '\"' )
		{
			*sz_nonce_temp = *sz_find;
			sz_nonce_temp++;
			sz_find++;
		}
		*sz_nonce_temp = '\0';
		return sz_find_nonce;
	}
	else
		return NULL;
}

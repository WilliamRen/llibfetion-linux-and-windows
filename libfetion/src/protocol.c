
/*! \file protocol.c
 *  \brief 里面封装了sip协议
 */

#include <stdio.h>
#include <time.h>
#include <string.h>

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

unsigned char* strtohex(char* in , int* len);
char* hextostr(unsigned char* in , int len);
char* hash_password_v2(char* userid , char* passwordhex);

char* hash_password_v1( unsigned char* b0 , int b0len, const unsigned char* password , int psdlen)
{
	unsigned char* dst = (unsigned char*)malloc(b0len + psdlen + 1);
	unsigned char tmp[20];
	char* res;
	SHA_CTX ctx;

	memset(tmp , 0 , sizeof(tmp));
	memset(dst , 0 , b0len + psdlen + 1);
	memcpy(dst , b0 , b0len);
	memcpy(dst + b0len , password , psdlen);

	SHA1_Init(&ctx);
	SHA1_Update(&ctx , dst , b0len + psdlen );
	SHA1_Final(tmp , &ctx);
	free(dst);
	res = hextostr(tmp , 20);
	return res;
}
char* hash_password_v2(char* userid , char* passwordhex)
{
	int id = atoi(userid);
	char* res;
	unsigned char* bid = (unsigned char*)(&id);
	unsigned char ubid[4];
	int bpsd_len;
	unsigned char* bpsd = strtohex(passwordhex , &bpsd_len);
	memcpy(ubid , bid , 4);
	res = hash_password_v1(ubid , sizeof(id) , bpsd , bpsd_len);
	free(bpsd);
	return res;
}
char* hash_password_v4(char* userid , char* password)
{
	char* domain = "fetion.com.cn:";
	char *res , *dst;
	unsigned char* udomain = (unsigned char*)malloc(strlen(domain));
	unsigned char* upassword = (unsigned char*)malloc(strlen(password));
	memset(udomain , 0 , strlen(domain));
	memcpy(udomain , (unsigned char*)domain , strlen(domain));
	memset(upassword , 0 , strlen(password));
	memcpy(upassword , (unsigned char*)password , strlen(password));
	res = hash_password_v1(udomain , strlen(domain) , upassword , strlen(password));
	free(udomain);
	free(upassword);
	if(userid == NULL || strlen(userid) == 0)
	{
		return res;
	}
	dst = hash_password_v2(userid , res);
	free(res);
	return dst;
}

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

char* fx_generate_response( char* sz_key, char* sz_nonce, char* sz_ase_key )
{
    char sz_module[256+1] = {0};
    char sz_exponent[6+1] = {0};
    char* sz_SHA1 = (char*)malloc( 1024 );

    memset( sz_SHA1, 0, 1024 );

    if( sz_key == NULL || sz_nonce == NULL || sz_ase_key == NULL )
        return NULL;
    /*first we should get the moudle and the publicExponent*/
    if( strlen( sz_key ) != 262 )
        return NULL;

    memcpy( sz_module, sz_key, 256 );
    memcpy( sz_exponent, sz_key+256, 6 );

    /*do RSA*/
    {
        int ret = 0, flen = 0, flen1 = 0;
        BIGNUM *bnn, *bne;
        unsigned char in[1024] = {0};
        unsigned char *out;

        //sprintf( in, "%s%s%s", g_pw_v4, sz_nonce, sz_ase_key );

        int n1 = 0, n2 = 0, n3 = strlen( sz_nonce );

        char* sz_pw = hash_password_v4( "639717376", "qiupeng501");

        unsigned char* p_byte = strtohex( sz_pw, &n1 );
        unsigned char* p_byte1= strtohex( sz_ase_key, &n2 );

		RSA *r = NULL;
		char* sz = NULL;
        //hex_str_2_byte( g_pw_v4, p_byte, &n1 );
        //hex_str_2_byte( sz_ase_key, p_byte1, &n2 );

        //in = (unsigned char*)malloc( 1024 );

        //memcpy( in, p_byte, n1 );
        //memcpy( in + n1, sz_nonce, n3);
        memcpy( in, sz_nonce, n3);
        memcpy( in + n3, p_byte, n1 );
        memcpy( in + n1 + n3, p_byte1, n2 );

         bnn = BN_new();
         bne = BN_new();
        // bnd = BN_new();
         BN_hex2bn(&bnn, sz_module);
        // BN_set_word(bne, sz_exponent);
         BN_hex2bn(&bne, sz_exponent);
        // BN_hex2bn(&bnd, PRIVATE_EXPONENT);

         r = RSA_new();
         r->n = bnn;
         r->e = bne;
         r->d = NULL;

         flen = RSA_size(r);
         //flen1 = strlen(in);
         flen1 = n1 + n2 + n3;

         out = (char *)malloc(flen);
         memset(out, 0, flen);

         ret = RSA_public_encrypt(flen1, in, out, r,   RSA_PKCS1_PADDING);
         if (ret < 0)
         {
            printf("Encrypt failed!\n");
            return NULL;
         }

        sz = hextostr( out, ret);
        strcpy( sz_SHA1, sz );
        free( sz );
        RSA_free(r);
    }

    return sz_SHA1;
}

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

char* fx_get_nonce( char* sz_data )
{
    char* sz_find = NULL, *sz_find_nonce = NULL, *sz_nonce_temp = NULL;
    if( NULL == sz_data )
        return NULL;
    sz_find = strstr( sz_data, "nonce" );
	sz_find_nonce = (char*)malloc(50);
	sz_nonce_temp = sz_find_nonce;
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
    char* sz_find = NULL, *sz_find_nonce = NULL, *sz_nonce_temp = NULL;
    if( NULL == sz_data )
        return NULL;
    sz_find = strstr( sz_data, "key" );
	sz_find_nonce = (char*)malloc(512);
	sz_nonce_temp = sz_find_nonce;
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
unsigned char* strtohex(char* in , int* len)
{
	unsigned char* out = (unsigned char*)malloc(strlen(in)/2 );
	int i = 0 , j = 0 , k = 0 ,length = 0;
	char tmp[3] = { 0 };
	memset(out , 0 , strlen(in) / 2);
	while(i < (int)strlen(in))
	{
		tmp[k++] = in[i++];
		tmp[k] = '\0';
		if(k == 2)
		{
			out[j++] = (unsigned char)strtol(tmp , (char**)NULL , 16);
			k = 0;
			length ++;
		}
	}
	if(len != NULL )
		*len = length;
	return out;
}

char* hextostr(unsigned char* in , int len)
{
	char* res = (char*)malloc(len * 2 + 1);
	int i = 0;
	memset(res , 0 , len * 2 + 1);
	while(i < len)
	{
		sprintf(res + i * 2 , "%02x" , in[i]);
		i ++;
	};
	i = 0;
	while(i < (int)strlen(res))
	{
		res[i] = toupper(res[i]);
		i ++;
	};
	return res;
}

char* generate_aes_key()
{
        char* key = (char*)malloc(65);
        memset( key , 0 , 65 );
        sprintf( key , "%04X%04X%04X%04X%04X%04X%04X"
                       "%04X%04X%04X%04X%04X%04X%04X%04X%04X" ,
                        rand() & 0xFFFF , rand() & 0xFFFF ,
                        rand() & 0xFFFF , rand() & 0xFFFF ,
                        rand() & 0xFFFF , rand() & 0xFFFF ,
                        rand() & 0xFFFF , rand() & 0xFFFF ,
                        rand() & 0xFFFF , rand() & 0xFFFF ,
                        rand() & 0xFFFF , rand() & 0xFFFF ,
                        rand() & 0xFFFF , rand() & 0xFFFF,
                        rand() & 0xFFFF , rand() & 0xFFFF );
        return key;
}

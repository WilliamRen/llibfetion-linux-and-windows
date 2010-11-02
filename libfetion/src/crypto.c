/***************************************************************************
 *   Copyright (C) 2010 by programmeboy                                    *
 *   programmeboy@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.                                        *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*! \file crypto.c
 *  \brief 
 *  \author programmeboy programmebot@gmail.com
 *  \date 2,10,2010   22:22  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>

#include "commdef.h"
#include "crypto.h"
#include "log.h"


/** \fn int hex_str_2_byte( __in char* sz_hex, __out byte* p_byte )
	\brief convert hex string to byte array
	\param sz_hex the hex string
	\param p_byte the out byte array
	\return out length
*/

int hex_str_2_byte( __in char* sz_hex, __out byte** p_byte )
{
	int i = 0, j = 0, k = 0, length = 0, out_len = 0;
	char tmp[3] = { 0 };
	
	out_len = strlen( sz_hex ) / 2;
	*p_byte = (byte*)malloc( out_len );
	memset( *p_byte, 0, out_len );

	while( i < (int)strlen( sz_hex ) )
	{
		tmp[k++] = sz_hex[i++];
		tmp[k] = '\0';
		if( k == 2 )
		{
			(*p_byte)[j++] = (byte)strtol( tmp ,(char**)NULL, 16 );
			k = 0;
			length ++;
		}
	}
	return length;
}

/** \fn int byte_2_hex_str( __in byte* p_byte, __out char* sz_hex, 
				   __out int n_len )
	\brief convert byte array to hex string
	\param p_byte the out byte array
	\param sz_hex the out hex string
	\return out length
*/

int byte_2_hex_str( __in byte* p_byte, __out char** sz_hex, \
				    __out int n_len )
{
	int i = 0, n_out_len = 0;

	n_out_len = n_len * 2 + 1;
	*sz_hex = (char*)malloc( n_out_len );
	memset( *sz_hex, 0, n_len * 2 + 1 );

	while(i < n_len)
	{
		sprintf( *sz_hex + i * 2 , "%02X" , (int)p_byte[i]);
		i ++;
	}
	return strlen( *sz_hex );
}

int decode_base64( __in const char* in, __out char** buffer, __out int* len)
{
	unsigned int t = 0 , c = 0;
	int n = 0;
	unsigned char out[3];
	unsigned char inp[4];
	
	n = strlen(in);
	if(n % 4 != 0)
	{
		return 0;
	}
	n = n / 4 * 3;
	if(len != NULL)
		*len = n;
	*buffer = (char*)malloc(n);
	memset(*buffer , 0 , n);
	while(1)
	{
		memset(inp , 0 , 4);
		memset(out , 0 , 3);
		memcpy(inp , in + c , 4);
		c += 4;
		n = EVP_DecodeBlock(out , inp , 4 );
		memcpy( *buffer + t , out, n);
		t += n;
		if(c >= strlen(in))
			break;
	}
	return 1;
}

/** \fn
	\brief 
	\param 
	\return 
*/

FX_RET_CODE fx_ssi_get_v4digest_1( __in byte* sz_fd1, __in int n_fd1_len, 
								   __in byte* sz_fd2, __in int n_fd2_len,
								   __out byte** hex_out )
{
    SHA_CTX sc;
    int i = 0, n_ret = 0;
	byte* sz_encode_src = (byte*)malloc( n_fd1_len + n_fd2_len );

	*hex_out = (byte*)malloc( SHA1_DIGESTSIZE );

    memset( *hex_out, 0, SHA1_DIGESTSIZE );
	memset( sz_encode_src, 0, n_fd1_len + n_fd2_len );
	memcpy( sz_encode_src, sz_fd1, n_fd1_len );
	memcpy( sz_encode_src + n_fd1_len, sz_fd2, n_fd2_len );

    /*
     *  do hash
     */

    SHA1_Init( &sc );
    SHA1_Update( &sc, sz_encode_src, n_fd1_len + n_fd2_len );
    SHA1_Final( *hex_out, &sc );

	return FX_ERROR_OK;
}


/** \fn 
	\brief 
	\param 
	\return 
*/

int do_hash_padding( __in char* sz_key, __in byte* hex_in, 
					 __in int n_in_len, __out char** sz_hash )
{
	
    char sz_module[FX_RSA_MODULE_LEN+1] = {0};
    char sz_exponent[FX_RSA_EXPNONT_LEN+1] = {0};
	
	int ret = 0, flen = 0;
    BIGNUM *bnn, *bne;
	byte* out;
	RSA *r = NULL;

	/*
	 *	get the value of module/exponent from sz_key
	 */
	
	memcpy( sz_module, sz_key, FX_RSA_MODULE_LEN );
	memcpy( sz_exponent, sz_key + FX_RSA_MODULE_LEN, FX_RSA_EXPNONT_LEN );
	
	/*
	 *	do hash 
	 */
	
	bnn = BN_new();
	bne = BN_new();
	
	BN_hex2bn(&bnn, sz_module);
	BN_hex2bn(&bne, sz_exponent);
	
	r = RSA_new();
	r->n = bnn;
	r->e = bne;
	r->d = NULL;
	
    flen = RSA_size(r);
	out = (byte*)malloc( flen );
	memset( out, 0, flen );

	ret = RSA_public_encrypt( n_in_len, hex_in, out, r, RSA_PKCS1_PADDING );
	if (ret < 0)
	{
		printf("Encrypt failed!\n");
		log_string( "Encrypt failed!" );
		return FX_ERROR_UNKOWN;
    }

	return byte_2_hex_str( out, sz_hash, ret );
		
}

/** \fn 
	\brief 
	\param 
	\return 
*/

void generate_aes_key( __out char** sz_aes_key )
{
	*sz_aes_key = (char*)malloc(65);
	memset( *sz_aes_key , 0 , 65 );
	sprintf( *sz_aes_key , "%04X%04X%04X%04X%04X%04X%04X%04X"
						   "%04X%04X%04X%04X%04X%04X%04X%04X" ,
		rand() & 0xFFFF , rand() & 0xFFFF ,
		rand() & 0xFFFF , rand() & 0xFFFF ,
		rand() & 0xFFFF , rand() & 0xFFFF ,
		rand() & 0xFFFF , rand() & 0xFFFF ,
		rand() & 0xFFFF , rand() & 0xFFFF ,
		rand() & 0xFFFF , rand() & 0xFFFF ,
		rand() & 0xFFFF , rand() & 0xFFFF,
		rand() & 0xFFFF , rand() & 0xFFFF );
}

/** \fn 
	\brief 
	\param 
	\return 
*/

FX_RET_CODE fx_generate_response( __in char* sz_key, __in char* sz_nonce,		 \
								  __in char* sz_user_id, __in char* sz_password, \
								  __out char** sz_response )
{	
	
	FX_RET_CODE ret = FX_ERROR_OK;
	int n_ret = 0, n_user_id = 0, n_total_len = 0, n_nonce_len = strlen( sz_nonce );
	byte* hex_step1 = NULL, *hex_step2 = NULL, *hex_aes_key = NULL, *hex_in = NULL;
	char* sz_aes_key = NULL;
	
	if ( sz_key == NULL || sz_nonce == NULL || sz_user_id == NULL || \
		 sz_password == NULL )
	{
		return FX_ERROR_UNKOWN;
	}

	/*
	 *	step 1: sha1( domain:password )
	 */
	
	ret = fx_ssi_get_v4digest_1( (byte*)FX_SHA1_DOMAIN, strlen( FX_SHA1_DOMAIN ), \
								 (byte*)sz_password, strlen( sz_password ), &hex_step1 );
	if ( FX_ERROR_OK != ret )
	{
		return ret;
	}
	
	/*
	 *	step 2: sha1( hex(user_id) + $(step 1) )
	 */
	
	n_user_id = atoi( sz_user_id );
	
	ret = fx_ssi_get_v4digest_1( (byte*)&n_user_id, sizeof( int ), \
								 hex_step1, SHA1_DIGESTSIZE, &hex_step2 );
	if ( FX_ERROR_OK != ret )
	{
		free( hex_step1 );
		return ret;
	}
	
	free( hex_step1 );

	/*
	 * step 3: rsa( nonce + $(hex_step2) + hex(sz_ase_key) )	
	 */
	
	generate_aes_key( &sz_aes_key );
	n_ret = hex_str_2_byte( sz_aes_key, &hex_aes_key );
	
	if ( n_ret <= 0 )
	{
		free( hex_step2 );
		return FX_ERROR_UNKOWN;
	}
	
	n_total_len = n_nonce_len + SHA1_DIGESTSIZE + n_ret;
	hex_in = (byte*)malloc( n_total_len );
	memset( hex_in, 0, n_total_len );

	memcpy( hex_in, sz_nonce, n_nonce_len );
	memcpy( hex_in + n_nonce_len, hex_step2, SHA1_DIGESTSIZE );
	memcpy( hex_in + n_nonce_len + SHA1_DIGESTSIZE, hex_aes_key, n_ret );

	n_ret = do_hash_padding( sz_key, hex_in, n_total_len, sz_response );
	if ( n_ret <= 0 )
	{
		free( hex_step2 );
		return FX_ERROR_UNKOWN;
	}

	free( hex_step2 );
	free( sz_aes_key );
	return FX_ERROR_OK;
}
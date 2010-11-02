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

#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

#define SHA1_DIGESTSIZE			(160/8)
#define FX_SSI_V4DIGEST_1		"fetion.com.cn:%s"

#define FX_RSA_MODULE_LEN		256
#define FX_RSA_EXPNONT_LEN		6
#define FX_SHA1_DOMAIN			"fetion.com.cn:"


int hex_str_2_byte( __in char* sz_hex, __out byte** p_byte );
int byte_2_hex_str( __in byte* p_byte, __out char** sz_hex, \
				    __out int n_len );

FX_RET_CODE fx_generate_response( __in char* sz_key, __in char* sz_nonce,		 \
								  __in char* sz_user_id, __in char* sz_password, \
								  __out char** sz_response );
void generate_aes_key( __out char** sz_aes_key );
int do_hash_padding( __in char* sz_key, __in byte* hex_in,	\
					 __in int n_in_len, __out char** sz_hash );
FX_RET_CODE fx_ssi_get_v4digest_1( __in byte* sz_fd1, __in int n_fd1_len,	\
								   __in byte* sz_fd2, __in int n_fd2_len,	\
								   __out byte** hex_out );

int decode_base64( __in const char* in, __out char** buffer, __out int* len);

#endif
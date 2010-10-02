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


#endif
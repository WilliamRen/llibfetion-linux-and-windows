#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#define FX_SSI_V4DIGEST_1   "fetion.com.cn:%s"
#define SHA1_DIGESTSIZE     (160/8)

enum make_type
{
    FX_BUILD_LOGIN_1,
    FX_BUILD_LOGIN_2,
    FX_BUILD_GET_PERSON_INFO,
    FX_BUILD_GET_CONTECT,
    FX_BUILD_SEND_MSG,
    FX_BUILD_SEND_TO_SELF
};

char* fx_pro_build_package( int n_type, void* l_data );
char* fx_ssi_get_v4digest_1( char* sz_password );
char* fx_generate_cnonce();
char* fx_get_nonce( char* sz_data );
char* fx_get_key( char* sz_data );

#endif // PROTOCOL_H_INCLUDED

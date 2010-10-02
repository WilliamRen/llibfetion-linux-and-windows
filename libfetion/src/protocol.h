#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

char* fx_pro_build_package( int n_type, void* l_data );
char* fx_generate_cnonce();
char* fx_get_nonce( char* sz_data );
char* fx_get_key( char* sz_data );

#endif // PROTOCOL_H_INCLUDED

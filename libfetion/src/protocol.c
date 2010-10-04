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

/*! \file protocol.c
 *  \brief 里面封装了sip协议
 */

#include <stdio.h>
#include <time.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/rsa.h>

#include "commdef.h"
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
                sprintf( sz_tmp, MAKE_LABLE_X, 'F', ((PLOGIN_DATA)l_data)->sz_uri );
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

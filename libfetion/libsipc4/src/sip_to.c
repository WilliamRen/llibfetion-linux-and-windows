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
 ***************************************************************************/

#include <stdio.h>
#include <string.h>

#include "../include/sip_def.h"
#include "../include/sip_to.h"


int
sip_to_init( sip_to_t** to )
{
    *to = (sip_to_t*)sip_malloc( sizeof( sip_to_t ) );
    if ( NULL == *to )
    	return LIBSIP_NOMEM;

    /*
     *  initialize the sip_to_t
     */

    (*to)->uid      = NULL;
    (*to)->host     = NULL;
    (*to)->p_value  = NULL;

    return LIBSIP_SUCCESS;
}

void
sip_to_free( sip_to_t* to )
{
    if ( NULL == to )
    	return;

    /*
     *  free the all element
     */

    if( to->uid != NULL )
        sip_free( to->uid );
    if ( to->host != NULL )
        sip_free( to->host );
    if ( to->p_value != NULL )
    	sip_free( to->p_value );

    /*
     *  free sip_to_t
     */

    sip_free( to );
}

int
sip_to_parse( sip_to_t* to, const char* value )
{

    /*
     *  check paramters
     */

    if ( NULL == to || NULL == value )
    	return LIBSIP_BADPARAMETER;

    /*
     *  first we find the 'sip:'
     */
    {
        char* sz_at  = strchr( value, '@' );
        char* sz_p   = strstr( value, ";p=" );
        size_t len = 0;

        /*
         *  check this
         */

        if ( 0 != strncmp( value, "sip:", 4 ) || \
             NULL == sz_at                    || \
             NULL == sz_p )
        	return LIBSIP_SYNTAXERROR;

        len = sz_at - (value + 4);
        to->uid = (char*)sip_malloc( len + 1 );
        if( NULL == to->uid )
            return LIBSIP_NOMEM;
        sip_strncpy( to->uid, value + 4, len );

        /*
         *  get the host and p value
         */

        {

            /*
             *  get the end of the value
             */

            char* sz_end = (char*)value + strlen(value);

            len = sz_p - sz_at - 1;
            to->host = (char*)sip_malloc( len + 1 );
            if( NULL == to->host )
                return LIBSIP_NOMEM;
            sip_strncpy( to->host, sz_at + 1, len );

            /*
             *  get p value
             */

            len = sz_end - sz_p - 3;
            to->p_value = (char*)sip_malloc( len + 1 );
            if( NULL == to->p_value )
                return LIBSIP_NOMEM;
            sip_strncpy( to->p_value, sz_p + 3, len );
        }
        /*
        //
        //   if have p value
        //

        if ( NULL != sz_p ){

            //
            //  get host with p value
            //

            len = sz_p - sz_at - 1;
            to->host = (char*)sip_malloc( len + 1 );
            if( NULL == to->host )
                return LIBSIP_NOMEM;
            sip_strncpy( to->host, sz_at + 1, len );

            //
            //  get p value
            //

            len = strlen( value ) - sz_p - 3;
            to->p_value = (char*)sip_malloc( len + 1 );
            if( NULL == to->p_value )
                return LIBSIP_NOMEM;
            sip_strncpy( to->p_value, sz_p + 3, len );

        }else{

            len = strlen( value ) - sz_at - 1;
            to->host = (char*)sip_malloc( len );
            if ( NULL == to->host )
            	return LIBSIP_NOMEM;
            sip_strncpy( to->host, sz_at + 1, len );
        }*/
    }

    return LIBSIP_SUCCESS;
}

int
sip_to_to_str( sip_to_t* to, char** dest )
{
    size_t len = 0;

    /*
     *  check paramters
     */

    if( NULL == to || NULL == to->uid           || \
        NULL == to->host || NULL == to->p_value )
        return LIBSIP_BADPARAMETER;

    /*
     *  get the len of destination string.
     *  sample: "sip:588955498@fetion.com.cn;p=737"
     */

    len = strlen( to->uid ) + strlen( to->host ) + \
          strlen( to->p_value ) + 4 /*'sip:'*/   + \
          3 /*;p=*/ + 1 /*@*/;
    *dest = (char*)sip_malloc( len + 1 );
    if ( NULL == *dest )
    	return LIBSIP_NOMEM;

    /*
     *  format the string
     */

    sprintf( *dest, "sip:%s@%s;p=%s", to->uid, to->host, \
             to->p_value );
    return LIBSIP_SUCCESS;
}

void
sip_to_set_uid( sip_to_t* to, const char* uid )
{
    to->uid = (char*)uid;
}

void
sip_to_set_host( sip_to_t* to, const char* host )
{
    to->host = (char*)host;
}

void
sip_to_set_pvalue( sip_to_t* to, const char* p_value )
{
    to->p_value = (char*)p_value;
}

void sip_to_set_all( sip_to_t* to, const char* uid, \
					 const char* host, const char* p_value )
{
	if ( to->uid != NULL )
		sip_free( to->uid );
	to->uid = sip_strdup( uid );
	if ( to->host != NULL )
		sip_free( to->host );
	to->host = sip_strdup( host );
	if ( to->p_value != NULL )
		sip_free( to->p_value );
	to->p_value = sip_strdup( p_value );

}
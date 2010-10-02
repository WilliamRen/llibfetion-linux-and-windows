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
#include "../include/sip_startline.h"

int
sip_startline_init( sip_startline_t** startline )
{
    *startline = (sip_startline_t*)sip_malloc( sizeof(sip_startline_t) );
    if( NULL == *startline )
        return LIBSIP_NOMEM;

    /*
     *  initialize
     */

    (*startline)->method        = NULL;
    (*startline)->status_code   = NULL;
    (*startline)->host          = NULL;
    (*startline)->version       = NULL;
    (*startline)->status_desc   = NULL;

    return LIBSIP_SUCCESS;
}

void
sip_startline_free( sip_startline_t* startline )
{
    if ( NULL == startline )
    	return;
    if ( NULL != startline->method )
    	sip_free( startline->method );
    if ( NULL != startline->status_code )
    	sip_free( startline->status_code );
    if ( NULL != startline->host )
        sip_free( startline->host );
    if ( NULL != startline->version )
        sip_free( startline->version );
    if ( NULL != startline->status_desc )
        sip_free( startline->status_desc );
    sip_free( startline );
}


/*
 *  请求包
 */

int
sip_startline_parse_req( sip_startline_t* startline, const char* value )
{
    char* space = strchr( value, ' ' );
    char* space1 = NULL;
    char* sz_end = NULL;

    if ( NULL == space )
    	return LIBSIP_SYNTAXERROR;
    if ( space - value < 1 )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  find the next space
     */

    space1 = strchr( space + 1, ' ' );
    if ( NULL == space1 )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  get end
     */

    sz_end = value + strlen( value );

    /*
     *  copy method
     */

    startline->method = (char*)sip_malloc( space - value + 1 );
    if ( NULL == startline->method )
    	return LIBSIP_NOMEM;

    sip_strncpy( startline->method, value, space - value );

    /*
     *  copy host
     */

    startline->host = (char*)sip_malloc( space1 - space );
    if ( NULL == startline->host ){
    	sip_startline_free( startline );
    	return LIBSIP_NOMEM;
    }

    sip_strncpy( startline->host, space + 1, space1 - space - 1 );

    /*
     *  copy version
     */

    startline->version = (char*)sip_malloc( sz_end - space1 );
    if ( NULL == startline->version ){
    	sip_startline_free( startline );
    	return LIBSIP_NOMEM;
    }

    sip_strncpy( startline->version, space1 + 1, sz_end - space1 - 1 );

    return LIBSIP_SUCCESS;
}

/*
 *  应答包
 */

int
sip_startline_parse_resp( sip_startline_t* startline, const char* value )
{
    char* space = strchr( value, ' ' );
    char* space1 = NULL;
    char* sz_end = NULL;

    if ( NULL == space )
    	return LIBSIP_SYNTAXERROR;
    if ( space - value < 1 )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  find the next space
     */

    space1 = strchr( space + 1, ' ' );
    if ( NULL == space1 )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  get end
     */

    sz_end = value + strlen( value );

    /*
     *  copy version
     */

    startline->version = (char*)sip_malloc( space - value + 1 );
    if ( NULL == startline->version )
    	return LIBSIP_NOMEM;

    sip_strncpy( startline->version, value, space - value );

    /*
     *  copy status_code
     */

    startline->status_code = (char*)sip_malloc( space1 - space );
    if ( NULL == startline->status_code ){
    	sip_startline_free( startline );
    	return LIBSIP_NOMEM;
    }

    sip_strncpy( startline->status_code, space + 1, space1 - space - 1 );

    /*
     *  copy status_desc
     */

    startline->status_desc = (char*)sip_malloc( sz_end - space1 );
    if ( NULL == startline->status_desc ){
    	sip_startline_free( startline );
    	return LIBSIP_NOMEM;
    }

    sip_strncpy( startline->status_desc, space1 + 1, sz_end - space1 - 1 );

    return LIBSIP_SUCCESS;
}

int
sip_startline_parse( sip_startline_t* startline, const char* value )
{
    int is_resp = 0;
    if ( NULL == startline || NULL == value )
    	return LIBSIP_BADPARAMETER;
    if ( 0 == strncmp( value, SIP_VERSION, 9 ) )
    	return sip_startline_parse_resp( startline, value );
    else
        return sip_startline_parse_req( startline, value );
}

int
sip_startline_to_str( sip_startline_t* startline, char** dest )
{
    size_t len = 0;
    if ( NULL == startline )
    	return LIBSIP_BADPARAMETER;

    /*
     *  request
     */

    if( NULL != startline->method ){
        if ( NULL == startline->host || \
             NULL == startline->version )
        	return LIBSIP_SYNTAXERROR;

        /*
         *  get length
         */

        len = strlen( startline->method ) + 1 /*space*/ + \
              strlen( startline->host ) + 1   /*space*/ + \
              strlen( startline->version );

        /*
         *  allocate memory
         */

        *dest = (char*)sip_malloc( len + 1 );
        if ( NULL == *dest )
        	return LIBSIP_NOMEM;
        /*
         *  format string
         */

        sprintf( *dest, "%s %s %s", startline->method, \
                                     startline->host,   \
                                     startline->version );
        return LIBSIP_SUCCESS;
    }else{

        /*
         *  respose
         */

        if ( NULL == startline->version     ||\
             NULL == startline->status_code ||\
             NULL == startline->status_desc )
        	return LIBSIP_SYNTAXERROR;

        /*
         *  get length
         */

        len = strlen( startline->version ) + 1 /*space*/ + \
              strlen( startline->status_code ) + 1 /*space*/ + \
              strlen( startline->status_desc );

        /*
         *  allocate memory
         */

        *dest = (char*)sip_malloc( len + 1 );
        if ( NULL == *dest )
        	return LIBSIP_NOMEM;

        /*
         *  format string
         */

        sprintf( *dest, "%s %s %s", startline->version, \
                                    startline->status_code, \
                                    startline->status_desc );

        return LIBSIP_SUCCESS;
    }
}

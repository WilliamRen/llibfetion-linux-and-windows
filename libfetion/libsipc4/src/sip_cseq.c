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
#include <stdlib.h>
#include <string.h>

#include "../include/sip_def.h"
#include "../include/sip_cseq.h"

int
sip_cseq_init( sip_cseq_t** cseq )
{
    /*
     *  allocate memory for cseq
     */

    *cseq = (sip_cseq_t*)sip_malloc( sizeof(sip_cseq_t) );
    if ( NULL == *cseq )
    	return LIBSIP_NOMEM;

    /*
     *  initialize the element
     */

    (*cseq)->number = 0;
    (*cseq)->method = NULL;
    return LIBSIP_SUCCESS;
}

void
sip_cseq_free( sip_cseq_t* cseq )
{

    /*
     *  check the paramter
     */

    if ( NULL == cseq )
    	return;
    if( NULL != cseq->method )
        sip_free( cseq->method );
    sip_free( cseq );

}

int
sip_cseq_parse( sip_cseq_t* cseq, const char* value )
{
    char* sz_method = NULL;
    const char* sz_end = NULL;

    /*
     *  check the paramter
     */

    if ( NULL == cseq   || \
         NULL == value  || \
         value[0] == '\0')
    	return LIBSIP_BADPARAMETER;

    /*
     *  first we should find the ' '
     */

    sz_method = strchr( value, ' ' );
    if ( NULL == sz_method )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  get the end of string
     */

    sz_end = value + strlen(value);
    if ( (sz_method - value + 1) < 2 )
    	return LIBSIP_SYNTAXERROR;
    {
        char sz_num[10] = {0};
        sip_strncpy( sz_num, value, sz_method - value );

        /*
         *  set number
         */

         cseq->number = atoi( sz_num );
    }

    if (sz_end - sz_method + 1 < 2)
        return LIBSIP_SYNTAXERROR;

    /*
     *  allocate memory for cseq->method
     */

    cseq->method = (char *)sip_malloc (sz_end - sz_method + 1);
    if ( NULL == cseq->method )
    	return LIBSIP_NOMEM;

    sip_strncpy( cseq->method, sz_method + 1, sz_end - sz_method );
    return LIBSIP_SUCCESS;
}

void
sip_cseq_set_number( sip_cseq_t* cseq, int number )
{
    /*
     *  note - This function must called after "sip_cseq_init"
     *  be called
     */

    cseq->number = number;
}

void
sip_cseq_set_method( sip_cseq_t* cseq, char* method )
{
    /*
     *  note - This function must called after "sip_cseq_init"
     *  be called
     */

    cseq->method = method;
}

int sip_cseq_to_str( sip_cseq_t* cseq, char** dest )
{

    /*
     *  check the paramter
     */

    if ( NULL == cseq || 0 == cseq->number || \
         NULL == cseq->method )
    	return LIBSIP_BADPARAMETER;
    {
        /*
         * get the max length of the string. here i
         * define the max length of number to 10
         */

        size_t len = strlen( cseq->method ) + 10 + 2;
        *dest = (char*)sip_malloc( len );
        if ( NULL == *dest )
        	return LIBSIP_NOMEM;
        sprintf( *dest, "%d %s", cseq->number, cseq->method );

        return LIBSIP_SUCCESS;
    }

}
void sip_cseq_set_all( sip_cseq_t* cseq, int num, const char* method )
{
	if ( cseq == NULL )
		return;
	cseq->number = num;
	if ( cseq->method != NULL )
		sip_free( cseq->method );
	cseq->method = sip_strdup( method );
}




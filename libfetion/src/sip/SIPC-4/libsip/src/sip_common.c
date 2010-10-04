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
 **************************************************************************/

#include <stdio.h>
#include <string.h>

#include "../include/sip_def.h"
#include "../include/sip_common.h"

int
sip_common_init( sip_common_t** common )
{

    /*
     *  allocate memory for common
     */

    *common = (sip_common_t*)sip_malloc( sizeof(sip_common_t) );
    if( NULL == *common )
        return LIBSIP_NOMEM;

    /*
     *  empty the element
     */

    (*common)->element = NULL;
    return LIBSIP_SUCCESS;
}

void
sip_common_free( sip_common_t* common )
{
    if( NULL == common )
        return;

    /*
     *  free the element
     */

    if( NULL != common->element )
        sip_free( common->element );
    sip_free( common );
}

int
sip_common_parse( sip_common_t* common, const char* value )
{
    size_t len = 0;

    /*
     *  check paramters
     */

    if( NULL == common || \
        NULL == value  || \
        value[0] == '\0' )
        return LIBSIP_BADPARAMETER;
    len = strlen( value );

    /*
     *  allocate memory for element
     */

    common->element = (char*)sip_malloc( len+1 );

    /*
     *  because the common have only one element
     *  so the just copy the value to the element
     */

    sip_strncpy( common->element, value, len );
    return LIBSIP_SUCCESS;
}

void
sip_common_set_element( sip_common_t* common, const char* element )
{
    /*
     *  note - This function must called after "sip_common_init"
     *  be called
     */

    common->element = (char*)element;
}

int sip_common_to_str( sip_common_t* common, char** dest )
{
    size_t len = 0;

    /*
     *  check the paramter
     */

    if ( NULL == common || NULL == common->element )
    	return LIBSIP_BADPARAMETER;

    len = strlen( common->element );

    /*
     *  allocate memory
     */

    *dest = (char*)sip_malloc( len + 1);

    /*
     *  copy the string
     */

    sip_strncpy( *dest, common->element, len );
    return LIBSIP_SUCCESS;
}








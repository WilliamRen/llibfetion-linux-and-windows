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

#ifndef SIP_WWW_AUTHENTICATE_H_INCLUDED
#define SIP_WWW_AUTHENTICATE_H_INCLUDED

typedef struct sip_www_authenticate
{
    char* auth_type;
    char* algorithm;
    char* nonce;
    char* key;
	char* ver_type;
    char* signature;

}sip_www_authenticate_t;

/** \fn
	\brief
	\param
	\return
*/

int sip_www_authenticate_init( sip_www_authenticate_t** www_authenticate );

/** \fn
	\brief
	\param
	\return
*/

void sip_www_authenticate_free( sip_www_authenticate_t* www_authenticate );

/** \fn
	\brief
	\param
	\return
*/

int sip_www_authenticate_parse( sip_www_authenticate_t* www_authenticate, \
                                const char* value );

/** \fn
	\brief
	\param
	\return
*/

int sip_www_authenticate_to_str( sip_www_authenticate_t* www_authenticate, \
                                 char** dest );

#endif // SIP_WWW_AUTHENTICATE_H_INCLUDED

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

/** \file sip_authorization.h
	\brief This file contant a list operation for 'A' of the SIP-C/4.0
           header
*/

#ifndef SIP_AUTHORIZATION_H_INCLUDED
#define SIP_AUTHORIZATION_H_INCLUDED

#define SIP_AUTHORIZATION_DIGEST_SZ			"Digest"
#define SIP_AUTHORIZATION_CS_SZ				"CS"
#define SIP_AUTHORIZATION_TICKS_SZ			"TICKS"

enum AUTHORIZATION{
		SIP_AUTHORIZATION_DIGEST = 0,
		SIP_AUTHORIZATION_CS,
		SIP_AUTHORIZATION_TICKS,
		SIP_AUTHORIZATION_UNKOWN
};

typedef struct sip_authorization
{
    char* auth_type;
    char* algorithm;
    char* response;
	char* address;
	char* credential;
	char* auth;

}sip_authorization_t;

/** \fn
	\brief
	\param
	\return
*/

int sip_authorization_init( sip_authorization_t** authorization );

/** \fn
	\brief
	\param
	\return
*/

void sip_authorization_free( sip_authorization_t* authorization );

/** \fn
	\brief
	\param
	\return
*/

int sip_authorization_parse( sip_authorization_t* authorization, const char* value );

/** \fn
	\brief
	\param
	\return
*/

int sip_authorization_to_str( sip_authorization_t* authorization, char** dest );


/** \fn 
	\brief 
	\param 
	\return 
*/

int sip_authorization_get_type( char* sz_type );

#endif // SIP_AUTHORIZATION_H_INCLUDED

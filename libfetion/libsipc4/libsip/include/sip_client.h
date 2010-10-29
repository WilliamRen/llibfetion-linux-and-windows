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

/** \file sip_client.h
	\brief for "CL"
*/

#ifndef SIP_CLIENT_H_INCLUDED
#define SIP_CLIENT_H_INCLUDED

typedef struct sip_client
{
    char* type;
    char* version;

}sip_client_t;

/** \fn
	\brief
	\param
	\return
*/

int sip_client_init( sip_client_t** client );

/** \fn
	\brief
	\param
	\return
*/

void sip_client_free( sip_client_t* client );

/** \fn
	\brief
	\param
	\return
*/

int sip_client_parse( sip_client_t* client, const char* value );

/** \fn
	\brief
	\param
	\return
*/

int sip_client_to_str( sip_client_t* client, char** dest );

void sip_client_set_all( sip_client_t* client, const char* type, const char* version );

#endif // SIP_CLIENT_H_INCLUDED



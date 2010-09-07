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

/** \file sip_to.h
	\brief This file contant a list operation for 'T' of the SIP-C/4.0
           header
*/

#ifndef SIP_TO_H_INCLUDED
#define SIP_TO_H_INCLUDED

typedef struct sip_uri
{
    char* uid;
    char* host;
    char* p_value;
}sip_uri_t;

typedef sip_uri_t sip_to_t;

int sip_to_init( sip_to_t** to );

void sip_to_free( sip_to_t* to );

int sip_to_parse( sip_to_t* to, const char* value );

void sip_to_set_uid( sip_to_t* to, const char* uid );

void sip_to_set_host( sip_to_t* to, const char* host );

void sip_to_set_pvalue( sip_to_t* to, const char* p_value );

int sip_to_to_str( sip_to_t* to, char** dest );

#endif // SIP_TO_H_INCLUDED

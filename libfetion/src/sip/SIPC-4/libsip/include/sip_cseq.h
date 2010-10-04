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

/*! \file sip_cseq.h
	\brief This file contant a list operation for cseq of the SIP-C/4.0
           header
*/

#ifndef SIP_CSEQ_H_INCLUDED
#define SIP_CSEQ_H_INCLUDED

typedef struct sip_cseq
{
    int number;
    char* method;

}sip_cseq_t;

/** \fn int sip_cseq_init( sip_cseq_t** cseq )
	\brief Allocate a cseq element.
	\param cseq The element to work on.
	\return see sip_def.h
*/

int sip_cseq_init( sip_cseq_t** cseq );

/** \fn void sip_cseq_free( sip_cseq_t* cseq )
	\brief Free a cseq element
	\param cseq The element to work on.
*/

void sip_cseq_free( sip_cseq_t* cseq );

/** \fn int sip_cseq_parse( sip_cseq_t* cseq, const char* value )
	\brief Parse the cseq element from a string
	\param cseq  The element to work on.
	\return see sip_def.h
*/

int sip_cseq_parse( sip_cseq_t* cseq, const char* value );

/** \fn int sip_cseq_set_number( sip_cseq_t* cseq, int number )
	\brief Set the number of cseq
	\param cseq The element to work on.
*/

void sip_cseq_set_number( sip_cseq_t* cseq, int number );

/** \fn int sip_cseq_set_method( sip_cseq_t* cseq, char* method )
	\brief Set the method of cseq
	\param cseq The element to work on.
*/

void sip_cseq_set_method( sip_cseq_t* cseq, char* method );


/** \fn int sip_cseq_to_str( sip_cseq_t* cseq, char** dest )
	\brief Get a string representation of a cseq element.
	\param cseq The element to work on.
	\param dest A pointer on the new allocated string.
	\return see sip_def.h
*/

int sip_cseq_to_str( sip_cseq_t* cseq, char** dest );

void sip_cseq_set_all( sip_cseq_t* cseq, int num, const char* method );

#endif // SIP_CSEQ_H_INCLUDED



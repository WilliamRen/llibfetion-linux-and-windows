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

/*! \file sip_common.h
	\brief 这个文件中包含了对于具有类似属性method进行处理.
           例如F和T都只有一个element.The method include
           'F' 'I' 'CN' 'L' 'E' 'N'

*/

#ifndef SIP_COMMON_H_INCLUDE
#define SIP_COMMON_H_INCLUDE

typedef struct sip_common
{
  char* element;

}sip_common_t;

typedef sip_common_t sip_cn_t;
typedef sip_common_t sip_call_id_t;
typedef sip_common_t sip_cnonce_t;
typedef sip_common_t sip_from_t;
typedef sip_common_t sip_context_length_t;
typedef sip_common_t sip_context_type_t;
typedef sip_common_t sip_context_encode_t;
typedef sip_common_t sip_expires_t;
typedef sip_common_t sip_event_t;


/*
 *	add for K
 *  sometime the recv package have a lot of k so we
 */

typedef struct sip_support_list
{
	sip_common_t* sip_k;
	struct sip_support_list* next;

}sip_support_list_t;

/** \fn int sip_common_init( sip_common_t** common )
	\brief Allocate a common element.
	\param common The element to work on.
	\return see sip_def.h
*/

int sip_common_init( sip_common_t** common );

/** \fn void sip_common_free( sip_common_t* common )
	\brief Free the common element
	\param common The element to work on
*/

void sip_common_free( sip_common_t* common );

/** \fn int sip_common_parse( sip_common_t* common, const char* value )
	\brief Parse the common element from a string
	\param common The element to work on
	\param value The string to parse
	\return see sip_def.h
*/

int sip_common_parse( sip_common_t* common, const char* value );

/** \fn int sip_common_set_element( sip_common_t* common, const char* element )
	\brief set the element of the common
	\param common The element to work on
	\param element the element value
*/

void sip_common_set_element( sip_common_t* common, const char* element );

/** \fn int sip_common_to_str( sip_comment_t* common, char** dest )
	\brief Get a string representation of a common element.
	\param common The element to work on.
	\param dest A pointer on the new allocated string.
	\return see sip_def.h
*/

int sip_common_to_str( sip_common_t* common, char** dest );

/** \fn 
	\brief 
	\param 
	\return 
*/

int sip_support_list_init( sip_support_list_t* sip_k );

/** \fn 
	\brief 
	\param 
	\return 
*/

void sip_support_list_free( sip_support_list_t* sip_k );

/** \fn 
	\brief 
	\param 
	\return 
*/

void sip_support_list_append( sip_support_list_t* sip_k, sip_support_list_t* add );

#define sip_common_set_all	sip_common_set_element

#endif/*SIP_COMMON_H_INCLUDE*/








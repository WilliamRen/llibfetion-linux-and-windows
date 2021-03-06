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

/** \file sip_message.h
	\author programmeboy
	\date 2010-08-22-10.38
	\brief for all
*/

#ifndef SIP_MESSAGE_H_INCLUDED
#define SIP_MESSAGE_H_INCLUDED

#include "sip.h"

typedef struct sip_message
{
    sip_startline_t* startline;              /*Head*/
    sip_from_t* from;                        /*'F'*/
    sip_call_id_t* call_id;                  /*'I'*/
    sip_cnonce_t* cnonce;                    /*'CN'*/
    sip_context_length_t* context_len;       /*'L'*/
	sip_context_type_t* context_type;		 /*'C'*/
    sip_context_encode_t* context_encode;    /*'E'*/
    sip_cseq_t* cseq;                        /*'Q'*/
    sip_client_t* client;                    /*'CL'*/
    sip_to_t* to;                            /*'T'*/
    sip_expires_t* expires;                  /*'X'*/
    sip_event_t*   event;                    /*'N'*/
    sip_authorization_t* authorization;      /*'A'*/
	sip_authorization_t* authorization_ver;  /*'A'*/
    sip_www_authenticate_t* www_authenticate;/*'W'*/
	sip_date_t* date;
	sip_support_list_t* support_list;		 /*'K'*/
    char* body;
}sip_message_t;

#define CTRLLF      "\r\n"
#define CTRLLFLF    "\r\n\r\n"

int
sip_message_set_common_str(sip_common_t** common, const char *value);

int
sip_message_set_cseq_str(sip_message_t* sip, const char *value);

int
sip_message_set_www_authenticate_str(sip_message_t* sip, const char *value);

int
sip_message_set_authorization_str(sip_message_t* sip, const char *value);

void
sip_message_set_authorization_ver(sip_message_t* sip, sip_authorization_t* authorization_ver);

int
sip_message_set_to_str(sip_message_t* sip, const char *value);

int
sip_message_set_client_str(sip_message_t* sip, const char *value);

int
sip_message_set_startline_str(sip_message_t* sip, const char *value);

int
sip_message_parse( sip_message_t* message, const char *value );

int
sip_message_to_str( sip_message_t* message, char** dest );

void
sip_message_free( sip_message_t* message );

int 
sip_message_init( sip_message_t** message );

int
sip_message_get_body_length( sip_message_t* message );

void
sip_message_set_common(sip_common_t** common, sip_common_t* common_set);

void
sip_message_set_cseq(sip_message_t* sip, sip_cseq_t* cseq );

void
sip_message_set_www_authenticate(sip_message_t* sip, sip_www_authenticate_t* www_authenticate);

void
sip_message_set_authorization(sip_message_t* sip, sip_authorization_t* authorization);

void
sip_message_set_to(sip_message_t* sip, sip_to_t* to);

void
sip_message_set_client(sip_message_t* sip, sip_client_t* client);

void
sip_message_set_startline(sip_message_t* sip, sip_startline_t* startline);

void
sip_message_set_body(sip_message_t* sip, const char* body );


#endif // SIP_MESSAGE_H_INCLUDED

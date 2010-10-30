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

#ifndef SIP_STARTLINE_H_INCLUDED
#define SIP_STARTLINE_H_INCLUDED

typedef struct sip_startline
{
    char* method;
    char* version;
    char* status_code;
    char* host;
    char* status_desc;

}sip_startline_t;

int sip_startline_init( sip_startline_t** startline );

void sip_startline_free( sip_startline_t* startline );

int sip_startline_parse( sip_startline_t* startline, const char* value );

int sip_startline_to_str( sip_startline_t* startline, char** dest );

void sip_start_set_line_req_all( sip_startline_t* startline, char* method, \
							 char* host );
void sip_start_set_line_resp_all( sip_startline_t* startline, char* statu_code, \
							  char* status_desc );

#endif // SIP_STARTLINE_H_INCLUDED

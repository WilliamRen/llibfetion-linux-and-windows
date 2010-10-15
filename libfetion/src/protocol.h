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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             * 
 ***************************************************************************/

#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#include "commdef.h"
#include "../src/sip/SIPC-4/libsip/include/sip.h"

char* fx_generate_cnonce();
FX_RET_CODE fx_send_msg_to_yourself( int socket, char* msg );
FX_RET_CODE fx_get_buddies_status( int socket );

FX_RET_CODE fx_send_msg_to_other( int socket, char* msg, __in int n_id );

FX_RET_CODE fx_send_msg_resp( int socket, sip_message_t* message );

#endif // PROTOCOL_H_INCLUDED

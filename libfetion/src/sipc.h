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

#ifndef SIPC_H_INCLUDE
#define SIPC_H_INCLUDE

#include "../src/sip/SIPC-4/libsip/include/sip.h"

#define MAX_SIP_RECV_LENGTH		1024

typedef struct _SIPC_MESG
{
	sip_message_t* msg;
	struct _SIPC_MESG * next;

}SIPC_MSG, *PSIPC_MSG;

void fx_sip_msg_append( PSIPC_MSG msglist, PSIPC_MSG msg );
FX_RET_CODE fx_sip_recv( int sock, PSIPC_MSG* msg_list_out );
void fx_sip_loop_print( PSIPC_MSG msg_list );

#endif
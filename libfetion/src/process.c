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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commdef.h"
#include "utf8.h"
#include "process.h"

int dispatch_sip_recv( sip_message_t* message )
{
	
	/*
	 *	判断是请求包还是应答包
	 */
	
	if ( message->startline->method != NULL )
	{
		char* sz_method = message->startline->method;

		/*
		 *	request package
		 */
		
		if ( strcmp( sz_method, "M" ) == 0 )
		{
			
			/*
			 *	recv message
			 */
			
			return process_sip_message( message );

		}
		else if ( strcmp( sz_method, "BN" ) == 0 )
		{
			
			/*
			 *	notification message
			 */
			
			return process_sip_notification( message );

		}
		else if ( strcmp( sz_method, "I" ) == 0 )
		{
			
			/*
			 *	invitation message
			 */

			return process_sip_invitation( message );
			
		}
		else if ( strcmp( sz_method, "IN" ) == 0 )
		{
			
			/*
			 *	TODO 
			 */
			
		}

	}
	else
	{
		
		/*
		 *	a response package
		 */

		
		/*
		 *	TODO
		 */
		
		
	}
	return FX_ERROR_OK;
}

int process_sip_message( sip_message_t* message )
{
	
	/*
	 *	just print message
	 */
	
	if ( strcmp( message->context_type->element, "text/plain" ) == 0 )
	{
		char* sz_tmp = utf8_to_ansi( message->body );
		printf( "%s ==> %s\n", message->from->element, sz_tmp );
		free( sz_tmp );
	}
	else if ( strcmp( message->context_type->element, "text/html-fragment" ) == 0 )
	{
		char* sz_tmp = utf8_to_ansi( message->body );
		printf( "xml message %s ==> %s\n", message->from->element, message->body );
		free( sz_tmp );
	}

	return FX_ERROR_OK;
}

int process_sip_notification( sip_message_t* message )
{
	return FX_ERROR_OK;
}

int process_sip_invitation( sip_message_t* message )
{
	return FX_ERROR_OK;
}
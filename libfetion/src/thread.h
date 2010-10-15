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

#ifndef THREAD_H_INCLUDE
#define THREAD_H_INCLUDE

/** \fn 
	\brief 
	\param 
	\return 
*/

//void* thread_sip_recv( void* lparam );
//void* thread_sip_keeplive( void* lparam );

//#ifdef __WIN32__
//DWORD WINAPI thread_sip_recv( void* lparam );
//#else
void* thread_sip_recv( void* lparam );
//#endif

//#ifdef __WIN32__
//DWORD WINAPI thread_sip_keeplive( void* lparam );
//#else
void* thread_sip_keeplive( void* lparam );
//#endif

void* thread_sip_keep_connection_busy( void* lparam );

#endif
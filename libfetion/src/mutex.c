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
#ifndef __WIN32__
#include <unistd.h>
#endif
#include <string.h>
#include <pthread.h>

#ifdef __WIN32__
#include <windows.h>
#endif

pthread_mutex_t g_group_list_mutex = PTHREAD_MUTEX_INITIALIZER;

int fx_get_group_list_mutex_lock()
{
	return pthread_mutex_lock( &g_group_list_mutex );
}

int fx_get_group_list_mutex_unlock()
{
	return pthread_mutex_unlock( &g_group_list_mutex );
}

void fx_get_group_list_mutex_init()
{
	pthread_mutex_init( &g_group_list_mutex, NULL );
}

void fx_get_group_list_mutex_free()
{
	pthread_mutex_destroy( &g_group_list_mutex );
}

pthread_mutex_t fx_get_group_list_mutex()
{
	return g_group_list_mutex;
}
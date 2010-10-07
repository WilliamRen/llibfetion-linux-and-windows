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

/*! \file   main.c
 *  \brief  this file include some code of initialization
 *  \author programmeboy programmebot@gmail.com
 *  \date   15:45:16 2010-3-3
 */

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <curl/curl.h>

#include "initial.h"
#include "log.h"

CURL* g_curl;

/** \fn FX_RET_CODE fx_init()
	\brief the function initialize the libcurl for use later
	\return if the function successds, return FX_ERROR_OK
            otherwise return FX_ERROR_INIT
*/
FX_RET_CODE fx_init()
{
#ifdef __WIN32__
#ifdef PTW32_STATIC_LIB
	if(!pthread_win32_process_attach_np())
		return FX_ERROR_INIT;
#endif
	log_init();
	return FX_ERROR_OK;
#endif
#ifdef _WINDOWS_
    /*todo here add the code run on windows*/
#endif
}

FX_RET_CODE fx_curl_init()
{
	g_curl = curl_easy_init();
	if( !g_curl ){
		log_string( "fx_init:initialize error!" );
		return FX_ERROR_INIT;
	}
	return FX_ERROR_OK;
}
void fx_curl_close()
{
	if ( g_curl )
	{
		curl_easy_cleanup( g_curl );
		g_curl = NULL;
	}
	
}

void fx_close()
{
#ifdef __WIN32__
#ifdef PTW32_STATIC_LIB
	pthread_win32_process_detach_np();
#endif
#endif
}


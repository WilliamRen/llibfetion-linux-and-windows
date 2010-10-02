
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
#endif
	log_init();
    g_curl = curl_easy_init();
    if( !g_curl ){
        log_string( "fx_init:initialize error!" );
        return FX_ERROR_INIT;
    }
    return FX_ERROR_OK;
#ifdef _WINDOWS_
    /*todo here add the code run on windows*/
#endif
}

void fx_close()
{
#ifdef __WIN32__
#ifdef PTW32_STATIC_LIB
	pthread_win32_process_detach_np();
#endif
#endif
}


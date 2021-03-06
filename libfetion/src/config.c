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

/*! \file config.cpp
 *  \brief this file include the code of
 *         get config file from fetion server
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

/*private include file*/
#include "log.h"
#include "initial.h"
#include "mem.h"
#include "config.h"
#include "crypto.h"
#include "protocol.h"

/*declare the extern variable*/
extern CURL* g_curl;


static
int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
	const char *text;
	
	switch (type) {
	case CURLINFO_TEXT:
		fprintf(stdout, "== Info: %s", data);
	default: /* in case a new one is introduced to shock us */ 
		return 0;
		
	case CURLINFO_HEADER_OUT:
		text = "=> Send header";
		break;
	case CURLINFO_DATA_OUT:
		text = "=> Send data";
		break;
	case CURLINFO_SSL_DATA_OUT:
		text = "=> Send SSL data";
		break;
	case CURLINFO_HEADER_IN:
		text = "<= Recv header";
		break;
	case CURLINFO_DATA_IN:
		text = "<= Recv data";
		break;
	case CURLINFO_SSL_DATA_IN:
		text = "<= Recv SSL data";
		break;
	}
	
	printf( "%s\n%s", text, data );

	return 0;
}


/** \fn
 *  \brief
 *  \param
 *  \return
 */

FX_RET_CODE fx_get_sys_conf( __in char* sz_phone_num, \
                             __in __out PMEM_STRUCT mem  )
{
    CURLcode curl_ret = 0;
    char sz_send_data[512] = {0};
	
	/*
	 *	initial
	 */
	
	
	//fx_curl_init();
    if ( g_curl == NULL ){
        log_string( "fx_get_user_conf:libfetion need initial" );
    	return FX_ERROR_NOINITIAL;
    }

    /*
	 * format the string
	 */
	
	if ( strlen( sz_phone_num ) == 11 ){
		sprintf( sz_send_data, FX_QUERY_SYS_CONF, FX_LOGIN_TYPE_PHONE_NUM_CONFIG, sz_phone_num );
	}else{
		sprintf( sz_send_data, FX_QUERY_SYS_CONF, FX_LOGIN_TYPE_SID, sz_phone_num );
	}
	
    
    /*if the paramter mem == ?*/
    if ( mem == NULL ){
    	log_string( "fx_get_sys_conf:the mem is null" );
    	return FX_ERROR_MEM;
    }
    /*set libcurl*/
    curl_easy_setopt( g_curl, CURLOPT_URL, FX_SYS_CONF_QUERY_URL );
	curl_easy_setopt( g_curl, CURLOPT_USERAGENT, "IIC2.0/pc 3.6.2000");
    curl_easy_setopt( g_curl, CURLOPT_POSTFIELDS, sz_send_data );
    curl_easy_setopt( g_curl, CURLOPT_POSTFIELDSIZE, strlen(sz_send_data));
    curl_easy_setopt( g_curl, CURLOPT_WRITEFUNCTION, write_mem_call_back );
    curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, (void*)mem );
    /*execute curl*/
    curl_ret = curl_easy_perform( g_curl );
    if( curl_ret != CURLE_OK ){
        log_string( "fx_get_user_conf:exec curl error!" );
        return FX_ERROR_CURL;
    }
	
	/*
	 *	reset curl
	 */
	
	curl_easy_reset( g_curl );
	//fx_curl_close();
	/*next we should parse the config file*/
    return FX_ERROR_OK;
}

/** \fn FX_RET_CODE fx_get_user_conf( __in char* sz_username, __in char* sz_password, \
 *                                    __in __out PMEM_STRUCT mem  )
 *  \brief get the config-file of the user specifed by sz_username
 *  \param sz_username the user's phone number
 *  \param sz_password the password
 *  \param mem the data we get from config-file
 *  \return FX_RET_CODE see more at error code definition
 */

FX_RET_CODE fx_get_user_conf( __in PSYS_CONF_DATA sys_data, \
                              __in __out PMEM_STRUCT mem )
{
    CURLcode curl_ret = 0;
    char sz_login_url[256] = {0};
	struct curl_slist* cookies = NULL, *cookies_temp = NULL;
    char* sz_digest1 = NULL;
	byte* hex_digest1 = NULL;
	FX_RET_CODE ret = FX_ERROR_OK;
	int n_ret = 0;
	
	/*
	 *	initial
	 */
	
	
	//fx_curl_init();

    if ( g_curl == NULL ){
    	log_string( "fx_get_user_conf:libfetion need initial" );
    	return FX_ERROR_NOINITIAL;
    }

    /*get the hash string*/
    ret = fx_ssi_get_v4digest_1( (byte*)FX_SHA1_DOMAIN, strlen( FX_SHA1_DOMAIN ), \
		(byte*)(sys_data->user_data.sz_password), strlen( sys_data->user_data.sz_password ), &hex_digest1 );
	if ( FX_ERROR_OK != ret )
	{
		log_string( "fx_get_user_conf:libfetion fx_ssi_get_v4digest_1 error!" );
    	return FX_ERROR_NOINITIAL;
	}
	n_ret = byte_2_hex_str( hex_digest1, &sz_digest1, SHA1_DIGESTSIZE );
    if( n_ret <= 0 ){
        log_string( "get diget1 error!" );
        return FX_ERROR_UNKOWN;
    }
	
    /*format the url for login request*/
	if ( strlen( sys_data->user_data.sz_phone_num ) == 11 )
	{
		sprintf( sz_login_url, FX_SSI_FORMAT, \
			sys_data->sz_user_conf_url, \
			FX_LOGIN_TYPE_PHONE_NUM, sys_data->user_data.sz_phone_num, \
            sz_digest1 );
	}else{
		sprintf( sz_login_url, FX_SSI_FORMAT, \
			sys_data->sz_user_conf_url, \
			 FX_LOGIN_TYPE_SID, sys_data->user_data.sz_phone_num, \
             sz_digest1 );
	}

    free( sz_digest1 );

    /*if the paramter mem == ?*/
    if ( mem == NULL ){
    	log_string( "fx_get_sys_conf:the mem is null" );
    	return FX_ERROR_MEM;
    }
    /*set libcurl*/
    curl_easy_setopt( g_curl, CURLOPT_URL, sz_login_url );
	curl_easy_setopt( g_curl, CURLOPT_USERAGENT, "IIC2.0/pc 3.6.2000");
    curl_easy_setopt( g_curl, CURLOPT_CONNECTTIMEOUT, 20 ); /*time out 8s*/
    curl_easy_setopt( g_curl, CURLOPT_WRITEFUNCTION, write_mem_call_back );
	curl_easy_setopt( g_curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt( g_curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, (void*)mem );
    curl_easy_setopt( g_curl, CURLOPT_COOKIEFILE, "" );
	/*curl_easy_setopt( g_curl, CURLOPT_DEBUGFUNCTION, my_trace);
	curl_easy_setopt( g_curl, CURLOPT_DEBUGDATA, &n_ret);
	curl_easy_setopt( g_curl, CURLOPT_VERBOSE, 1 );*/
    /*execute curl*/
    curl_ret = curl_easy_perform( g_curl );
    if( curl_ret != CURLE_OK ){
        log_string( "fx_get_user_conf:exec curl error!" );
        return FX_ERROR_CURL;
    }
	
	/*
	 *	get cookies
	 */
	//curl_easy_setopt( g_curl, CURLOPT_COOKIELIST, "ALL" );
	curl_ret = curl_easy_getinfo( g_curl, CURLINFO_COOKIELIST, &cookies );
	if ( curl_ret != CURLE_OK ){
		log_string( "fx_get_user_conf:curl_easy_getinfo error!" );
        fx_curl_close();
		return FX_ERROR_OK;
	}
	
	/*
	 *	get ssic
	 */
	
	cookies_temp = cookies;
	while( cookies_temp )
	{
		if ( strstr( cookies_temp->data, "ssic" ) != NULL )
		{
			//printf( "cookies = %s\n", cookies_temp->data );
			strcpy( sys_data->sz_cookie, cookies_temp->data );
			break;
		}
		cookies_temp = cookies_temp->next;
	}
	
	/*
	 *	clean up curl
	 */

	curl_slist_free_all( cookies );
	
	/*
	 *	reset curl
	 */
	
	curl_easy_reset( g_curl );

	//fx_curl_close();
    /*next we should parse the config file*/
    return FX_ERROR_OK;
}

FX_RET_CODE fx_get_user_conf_ver( __in PSYS_CONF_DATA sys_data, \
				__in char* sz_chid, __in char* sz_code, __in char* sz_a,
                              __in __out PMEM_STRUCT mem )
{
    CURLcode curl_ret = 0;
    char sz_login_url[256] = {0};
	struct curl_slist* cookies = NULL, *cookies_temp = NULL;
    char* sz_digest1 = NULL;
	byte* hex_digest1 = NULL;
	FX_RET_CODE ret = FX_ERROR_OK;
	int n_ret = 0;
	
	/*
	 *	initial
	 */
	
	
	//fx_curl_init();

    if ( g_curl == NULL ){
    	log_string( "fx_get_user_conf:libfetion need initial" );
    	return FX_ERROR_NOINITIAL;
    }

    /*get the hash string*/
    ret = fx_ssi_get_v4digest_1( (byte*)FX_SHA1_DOMAIN, strlen( FX_SHA1_DOMAIN ), \
		(byte*)(sys_data->user_data.sz_password), strlen( sys_data->user_data.sz_password ), &hex_digest1 );
	if ( FX_ERROR_OK != ret )
	{
		log_string( "fx_get_user_conf:libfetion fx_ssi_get_v4digest_1 error!" );
    	return FX_ERROR_NOINITIAL;
	}
	n_ret = byte_2_hex_str( hex_digest1, &sz_digest1, SHA1_DIGESTSIZE );
    if( n_ret <= 0 ){
        log_string( "get diget1 error!" );
        return FX_ERROR_UNKOWN;
    }
	
    /*format the url for login request*/
	if ( strlen( sys_data->user_data.sz_phone_num ) == 11 )
	{
		
		/*
		 *	login with phone number
		 */
		
		sprintf( sz_login_url, FX_SSI_FORMAT1, \
				 sys_data->sz_user_conf_url, \
				 FX_LOGIN_TYPE_PHONE_NUM, \
				 sys_data->user_data.sz_phone_num, \
				 sz_chid, sz_code, sz_a, \
				 sz_digest1 );
	}else{
		
		/*
		 *	login with sid
		 */
		
		sprintf( sz_login_url, FX_SSI_FORMAT1, \
				 sys_data->sz_user_conf_url, \
				 FX_LOGIN_TYPE_SID,	\
				 sys_data->user_data.sz_phone_num, \
				 sz_chid, sz_code, sz_a, \
				 sz_digest1 );
	}


    free( sz_digest1 );

    /*if the paramter mem == ?*/
    if ( mem == NULL ){
    	log_string( "fx_get_sys_conf:the mem is null" );
    	return FX_ERROR_MEM;
    }
    /*set libcurl*/
    curl_easy_setopt( g_curl, CURLOPT_URL, sz_login_url );
    curl_easy_setopt( g_curl, CURLOPT_CONNECTTIMEOUT, 20 ); /*time out 8s*/
	curl_easy_setopt( g_curl, CURLOPT_USERAGENT, "IIC2.0/pc 3.6.2000");
    curl_easy_setopt( g_curl, CURLOPT_WRITEFUNCTION, write_mem_call_back );
	curl_easy_setopt( g_curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt( g_curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, (void*)mem );
    curl_easy_setopt( g_curl, CURLOPT_COOKIEFILE, "" );
	/*curl_easy_setopt( g_curl, CURLOPT_DEBUGFUNCTION, my_trace);
	curl_easy_setopt( g_curl, CURLOPT_DEBUGDATA, &n_ret);
	curl_easy_setopt( g_curl, CURLOPT_VERBOSE, 1 );*/
    /*execute curl*/
    curl_ret = curl_easy_perform( g_curl );
    if( curl_ret != CURLE_OK ){
        log_string( "fx_get_user_conf:exec curl error!" );
        return FX_ERROR_CURL;
    }
	
	/*
	 *	get cookies
	 */
	//curl_easy_setopt( g_curl, CURLOPT_COOKIELIST, "ALL" );
	curl_ret = curl_easy_getinfo( g_curl, CURLINFO_COOKIELIST, &cookies );
	if ( curl_ret != CURLE_OK ){
		log_string( "fx_get_user_conf:curl_easy_getinfo error!" );
        fx_curl_close();
		return FX_ERROR_OK;
	}
	
	/*
	 *	get ssic
	 */
	
	cookies_temp = cookies;
	while( cookies_temp )
	{
		if ( strstr( cookies_temp->data, "ssic" ) != NULL )
		{
			strcpy( sys_data->sz_cookie, cookies_temp->data );
			break;
		}
		cookies_temp = cookies_temp->next;
	}
	
	/*
	 *	clean up curl
	 */
	
	curl_slist_free_all( cookies );
	
	/*
	 *	reset curl
	 */
	
	curl_easy_reset( g_curl );

    /*next we should parse the config file*/
    return FX_ERROR_OK;
}

FX_RET_CODE fx_get_verify_pic( __in PSYS_CONF_DATA sys_data, \
							   __in char* algorithm,
                               __in __out PMEM_STRUCT mem )
{
	CURLcode curl_ret = 0;
    char sz_send_data[512] = {0};
	
	/*
	 *	initial
	 */
	
    if ( g_curl == NULL ){
        log_string( "fx_get_user_conf:libfetion need initial" );
    	return FX_ERROR_NOINITIAL;
    }

    /*
	 * format the string
	 */

    sprintf( sz_send_data, FX_QUERY_PIC, algorithm );
    
	/*
	 * if the paramter mem == ?
	 */

    if ( mem == NULL ){
    	log_string( "fx_get_sys_conf:the mem is null" );
    	return FX_ERROR_MEM;
    }

    /*
	 * set libcurl
	 */

    curl_easy_setopt( g_curl, CURLOPT_URL, sz_send_data );
	curl_easy_setopt( g_curl, CURLOPT_USERAGENT, "IIC2.0/pc 3.6.2000");
	if ( strlen( sys_data->sz_cookie ) != 0 ){
		curl_easy_setopt( g_curl, CURLOPT_COOKIE, sys_data->sz_cookie );
	}
    curl_easy_setopt( g_curl, CURLOPT_WRITEFUNCTION, write_mem_call_back );
    curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, (void*)mem );
    
	/*
	 * execute curl
	 */

    curl_ret = curl_easy_perform( g_curl );
    if( curl_ret != CURLE_OK ){
        log_string( "fx_get_user_conf:exec curl error!" );
        return FX_ERROR_CURL;
    }
	
	/*
	 *	reset curl
	 */
	
	curl_easy_reset( g_curl );
	
	/*
	 * next we should parse the config file
	 */

    return FX_ERROR_OK;
}
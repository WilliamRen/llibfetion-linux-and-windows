
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
#include "protocol.h"


/*declare the extern variable*/
extern CURL* g_curl;

/** \fn
 *  \brief
 *  \param
 *  \return
 */

FX_RET_CODE fx_get_sys_conf( __in char* sz_phone_num, \
                             __in __out struct mem_struct* mem  )
{
    CURLcode curl_ret = 0;
    char sz_send_data[512] = {0};

    struct curl_slist* head = NULL;
    if ( g_curl == NULL ){
        log_string( "fx_get_user_conf:libfetion need initial" );
    	return FX_ERROR_NOINITIAL;
    }
    /*format the string*/
    sprintf( sz_send_data, FX_QUERY_SYS_CONF, sz_phone_num );
    /*if the paramter mem == ?*/
    if ( mem == NULL ){
    	log_string( "fx_get_sys_conf:the mem is null" );
    	return FX_ERROR_MEM;
    }
    /*set protocal header*/
    head = curl_slist_append( head, "Accept: Agent-007" );
    /*set libcurl*/
    curl_easy_setopt( g_curl, CURLOPT_URL, FX_SYS_CONF_QUERY_URL );
    curl_easy_setopt( g_curl, CURLOPT_HTTPHEADER, head );
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
    /*next we should parse the config file*/
    return FX_ERROR_OK;
}

/** \fn FX_RET_CODE fx_get_user_conf( __in char* sz_username, __in char* sz_password, \
 *                                    __out struct login_data* l_data )
 *  \brief get the config-file of the user specifed by sz_username
 *  \param sz_username the user's phone number
 *  \param sz_password the password
 *  \param l_data the data we get from config-file
 *  \return FX_RET_CODE see more at error code definition
 */

FX_RET_CODE fx_get_user_conf( __in struct sys_conf_data* sys_data, \
                              __in __out struct mem_struct* mem )
{
    CURLcode curl_ret = 0;
    char sz_login_url[256] = {0};
    struct curl_slist* head = NULL;
    char* sz_digest1 = NULL;

    if ( g_curl == NULL ){
    	log_string( "fx_get_user_conf:libfetion need initial" );
    	return FX_ERROR_NOINITIAL;
    }

    /*get the hash string*/
    sz_digest1 = fx_ssi_get_v4digest_1( sys_data->user_data.sz_password );
    if( sz_digest1 == NULL ){
        log_string( "get diget1 error!" );
        return FX_ERROR_UNKOWN;
    }
    /*format the url for login request*/
    sprintf( sz_login_url, FX_SSI_FORMAT, \
             sys_data->sz_user_conf_url, sys_data->user_data.sz_phone_num, \
             sz_digest1 );

    free( sz_digest1 );

    /*if the paramter mem == ?*/
    if ( mem == NULL ){
    	log_string( "fx_get_sys_conf:the mem is null" );
    	return FX_ERROR_MEM;
    }
    /*set the head of protocol*/
    head = curl_slist_append( head, "Accept: Agent-007" );
    /*set libcurl*/
    curl_easy_setopt( g_curl, CURLOPT_URL, sz_login_url );
    curl_easy_setopt( g_curl, CURLOPT_HTTPHEADER, head );
    curl_easy_setopt( g_curl, CURLOPT_CONNECTTIMEOUT, 8 ); /*time out 8s*/
    curl_easy_setopt( g_curl, CURLOPT_WRITEFUNCTION, write_mem_call_back );
    curl_easy_setopt( g_curl, CURLOPT_WRITEDATA, (void*)mem );
    curl_easy_setopt( g_curl, CURLOPT_COOKIEJAR, "cookie.txt" );
    /*execute curl*/
    curl_ret = curl_easy_perform( g_curl );
    if( curl_ret != CURLE_OK ){
        log_string( "fx_get_user_conf:exec curl error!" );
        return FX_ERROR_CURL;
    }
    /*next we should parse the config file*/
    return FX_ERROR_OK;
}

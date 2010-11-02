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

#include <stdio.h>
#include <string.h>

#include "../include/sip_def.h"
#include "../include/sip_authorization.h"

int
sip_authorization_init( sip_authorization_t** authorization )
{
    *authorization = (sip_authorization_t*)sip_malloc( sizeof(sip_authorization_t) );
    if ( NULL == *authorization )
    	return LIBSIP_NOMEM;

    /*
     *  initialize
     */

    (*authorization)->auth_type = NULL;
    (*authorization)->algorithm = NULL;
    (*authorization)->response  = NULL;
	(*authorization)->address  = NULL;
	(*authorization)->credential  = NULL;
	(*authorization)->auth  = NULL;
	(*authorization)->ver_type  = NULL;
	(*authorization)->chid  = NULL;

    return LIBSIP_SUCCESS;
}

void
sip_authorization_free( sip_authorization_t* authorization )
{
    if ( NULL == authorization )
    	return;

    /*
     *  free memeber
     */

    if ( NULL != authorization->auth_type )
        sip_free( authorization->auth_type );
    if ( NULL != authorization->algorithm )
    	sip_free( authorization->algorithm );
    if ( NULL != authorization->response )
    	sip_free( authorization->response );
	if ( NULL != authorization->address )
    	sip_free( authorization->address );
	if ( NULL != authorization->credential )
    	sip_free( authorization->credential );
	if ( NULL != authorization->auth )
    	sip_free( authorization->auth );
	if ( NULL != authorization->ver_type )
        sip_free( authorization->ver_type );
	if ( NULL != authorization->chid )
        sip_free( authorization->chid );

    /*
     *  free authorization
     */

    sip_free( authorization );
}

int
sip_authorization_parse( sip_authorization_t* authorization, const char* value )
{

    /*
     *  example : Digest algorithm="SHA1-sess-v4",response="67d4703ba6a1cb"
     */

    const char* space = NULL;
    const char* next = NULL;
    int i = 0, n_type = SIP_AUTHORIZATION_UNKOWN;

    if ( NULL == authorization || NULL == value )
    	return LIBSIP_BADPARAMETER;

    space = strchr( value, ' ' );
    if ( NULL == space )
    	return LIBSIP_SYNTAXERROR;
    if ( space - value < 1 )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  get type
     */

    authorization->auth_type = (char*)sip_malloc( space - value + 1);
    if ( NULL == authorization->auth_type )
    	return LIBSIP_NOMEM;
    sip_strncpy( authorization->auth_type, value, space - value );
	
	n_type = sip_authorization_get_type( authorization->auth_type );

    for (;;){

      int parse_ok = 0;
	  
	  if ( SIP_AUTHORIZATION_DIGEST == n_type ){
		  /*
		   *  get filed
		   */

		  i = __sip_quoted_string_set( "response", space, &(authorization->response), &next );
		  if ( i != 0 )
			return i;
		  if ( next == NULL )
			return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if ( next != space ){
			  space = next;
			  parse_ok++;
		  }

		  /*
		   *  next
		   */

		  i = __sip_quoted_string_set( "algorithm", space, &(authorization->algorithm), &next );
		  if ( i != 0 )
			return i;
		  if ( next == NULL )
			return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }
	  }else if ( SIP_AUTHORIZATION_VERIFY == n_type ){
		  /*
		   *  get filed
		   */

		  i = __sip_quoted_string_set( "response", space, &(authorization->response), &next );
		  if ( i != 0 )
			return i;
		  if ( next == NULL )
			return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if ( next != space ){
			  space = next;
			  parse_ok++;
		  }

		  /*
		   *  next
		   */

		  i = __sip_quoted_string_set( "algorithm", space, &(authorization->algorithm), &next );
		  if ( i != 0 )
			return i;
		  if ( next == NULL )
			return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }

		  /*
		   *  next
		   */

		  i = __sip_quoted_string_set( "type", space, &(authorization->ver_type), &next );
		  if ( i != 0 )
			return i;
		  if ( next == NULL )
			return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }

		  /*
		   *  next
		   */

		  i = __sip_quoted_string_set( "chid", space, &(authorization->chid), &next );
		  if ( i != 0 )
			return i;
		  if ( next == NULL )
			return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }

	  }else if ( SIP_AUTHORIZATION_CS == n_type ){
		  /*
		   *	get filed 
		   */
		  
		  i = __sip_quoted_string_set( "address", space, &(authorization->address), &next );
		  if ( i != 0 )
			  return i;
		  if ( next == NULL )
			  return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }

		  /*
		   *	next 
		   */
		  
		  i = __sip_quoted_string_set( "credential", space, &(authorization->credential), &next );
		  if ( i != 0 )
			  return i;
		  if ( next == NULL )
			  return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }
	  }else if ( SIP_AUTHORIZATION_TICKS == n_type ){
		  /*
		   *	get filed 
		   */
		  
		  i = __sip_quoted_string_set( "auth", space, &(authorization->auth), &next );
		  if ( i != 0 )
			  return i;
		  if ( next == NULL )
			  return LIBSIP_SUCCESS;                  /* end of header detected! */
		  else if (next != space){
			  space = next;
			  parse_ok++;
		  }
	  }
	  
      /* nothing was recognized:
       * here, we should handle a list of unknown tokens where:
       * token1 = ( token2 | quoted_text ) */
      /* TODO */

      if ( 0 == parse_ok ){

          const char *quote1, *quote2, *tmp;

          /*
           * CAUTION
           * parameter not understood!!! I'm too lazy to handle IT
           * let's simply bypass it
           */

          if ( strlen( space ) < 1)
            return LIBSIP_SUCCESS;
          tmp = strchr( space + 1, ',' );
          if ( tmp == NULL )                    /* it was the last header */
            return LIBSIP_SUCCESS;
          quote1 = __sip_quote_find( space );
          if ( (quote1 != NULL) && (quote1 < tmp) ){

              quote2 = __sip_quote_find( quote1 + 1 );
              if ( quote2 == NULL )
                return LIBSIP_SYNTAXERROR;      /* bad header format... */
              if ( tmp < quote2 )               /* the comma is inside the quotes! */
                space = strchr( quote2, ',' );
              else
                space = tmp;
              if ( space == NULL )              /* it was the last header */
                return LIBSIP_SUCCESS;
          } else
            space = tmp;
          /* continue parsing... */
      }
  }
  return LIBSIP_SUCCESS;                        /* ok */
}

int
sip_authorization_to_str( sip_authorization_t* authorization, char** dest )
{
  size_t len;
  char *tmp;
  int n_type = SIP_AUTHORIZATION_UNKOWN;

  *dest = NULL;

  /*
   * IMS requirement: send authorization like in:
   * Digest uri="sip:sip.antisip.com", username="joe", response=""
   */

  if ((authorization == NULL) || (authorization->auth_type == NULL))
    return LIBSIP_BADPARAMETER;
  if ( authorization->address != NULL && \
	   authorization->algorithm != NULL && \
	   authorization->auth == NULL )
	  return LIBSIP_BADPARAMETER;

  len = strlen (authorization->auth_type) + 1;
  
  n_type = sip_authorization_get_type( authorization->auth_type );

  switch ( n_type )
  {
  case SIP_AUTHORIZATION_DIGEST:
	  {
		  if (authorization->response != NULL)
			 len = len + 11 + strlen(authorization->response) + 2 /*for ""*/;
		  len = len + 2;
		  if (authorization->algorithm != NULL)
			len = len + strlen(authorization->algorithm) + 12 + 2 /*for ""*/;
	  }
	  break;
  case SIP_AUTHORIZATION_CS:
	  {
		  if (authorization->credential != NULL)
			len = len + strlen(authorization->credential) + 14 + 2 /*for ""*/;
		  len = len + 2;/*for \r\n*/
		  if (authorization->address != NULL)
			len = len + strlen(authorization->address) + 10 + 2 /*for ""*/;
	  }
	  break;
  case SIP_AUTHORIZATION_TICKS:
	  {
		  if ( authorization->auth != NULL )
		  {
			  len = len + strlen( authorization->auth ) + 7 + 2;
		  }
	  }
	  break;
  case SIP_AUTHORIZATION_VERIFY:
	  {
		  if (authorization->response != NULL)
			 len = len + 11 + strlen(authorization->response) + 2 /*for ""*/;
		  len = len + 2;
		  if (authorization->algorithm != NULL)
			len = len + strlen(authorization->algorithm) + 12 + 2 /*for ""*/;
		  if ( authorization->ver_type != NULL )
			len = len + strlen(authorization->ver_type) + 8 + 2 /*for ""*/;
		  if ( authorization->chid != NULL )
			len = len + strlen(authorization->chid) + 8 + 2 /*for ""*/;
	  }
	  break;
  default:
	  break;

  }
  /*
   *  allocate memory
   */

  tmp = (char*)sip_malloc( len );
  if (tmp == NULL)
    return LIBSIP_NOMEM;
  *dest = tmp;

  tmp = sip_str_append(tmp, authorization->auth_type);

  /*
   * for sip-c
   */
  
  switch( n_type )
  {
  case SIP_AUTHORIZATION_DIGEST:
	  {
		  if (authorization->algorithm != NULL){
			  
			  tmp = sip_strn_append(tmp, " algorithm=\"", 12);
			  tmp = sip_str_append(tmp, authorization->algorithm);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
		  if (authorization->response != NULL){
			  
			  tmp = sip_strn_append(tmp, ",response=\"", 11);
			  tmp = sip_str_append(tmp, authorization->response);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
	  }
	  break;
  case SIP_AUTHORIZATION_CS:
	  {
		  if (authorization->address != NULL){
			  
			  tmp = sip_strn_append(tmp, " address=\"", 10);
			  tmp = sip_str_append(tmp, authorization->address);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
		  if (authorization->credential != NULL){
			  
			  tmp = sip_strn_append(tmp, ",credential=\"", 13);
			  tmp = sip_str_append(tmp, authorization->credential);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
	  }
	  break;
  case SIP_AUTHORIZATION_TICKS:
	  {
		  if (authorization->auth != NULL){
			  
			  tmp = sip_strn_append(tmp, " auth=\"", 7);
			  tmp = sip_str_append(tmp, authorization->auth);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
	  }
	  break;
  case SIP_AUTHORIZATION_VERIFY:
	  {
		  if (authorization->algorithm != NULL){
			  
			  tmp = sip_strn_append(tmp, " algorithm=\"", 12);
			  tmp = sip_str_append(tmp, authorization->algorithm);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
		  if ( authorization->ver_type != NULL )
		  {
			  tmp = sip_strn_append(tmp, ",type=\"", 7 );
			  tmp = sip_str_append(tmp, authorization->ver_type);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
		  if (authorization->response != NULL){
			  
			  tmp = sip_strn_append(tmp, ",response=\"", 11);
			  tmp = sip_str_append(tmp, authorization->response);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
		  if (authorization->chid != NULL){
			  
			  tmp = sip_strn_append(tmp, ",chid=\"", 7);
			  tmp = sip_str_append(tmp, authorization->chid);
			  tmp = sip_strn_append(tmp, "\"", 1);
		  }
	  }
	  break;
  default:
	  break;
  }
  return LIBSIP_SUCCESS;
}

int sip_authorization_get_type( char* sz_type )
{
	if ( strcmp( sz_type, SIP_AUTHORIZATION_DIGEST_SZ ) == 0 ){
		return SIP_AUTHORIZATION_DIGEST;
	}else if ( strcmp( sz_type, SIP_AUTHORIZATION_CS_SZ ) == 0 ){
		return SIP_AUTHORIZATION_CS;
	}else if ( strcmp( sz_type, SIP_AUTHORIZATION_TICKS_SZ ) == 0 ){
		return SIP_AUTHORIZATION_TICKS;
	}else if ( strcmp( sz_type, SIP_AUTHORIZATION_VERIFY_SZ ) == 0 ){
		return SIP_AUTHORIZATION_VERIFY;
	}else{
		return SIP_AUTHORIZATION_UNKOWN;
	}
}

void sip_authorization_set_type( sip_authorization_t* authorization, const char* auth_type )
{
	if ( authorization->auth_type != NULL )
	{
		sip_free( authorization->auth_type );
	}
	authorization->auth_type = sip_strdup( auth_type );
}

void sip_authorization_set_address( sip_authorization_t* authorization, const char* address )
{
	if ( authorization->address != NULL )
	{
		sip_free( authorization->address );
	}
	authorization->address = sip_strdup( address );
}

void sip_authorization_set_algorithm( sip_authorization_t* authorization, const char* algorithm )
{
	if ( authorization->algorithm != NULL )
	{
		sip_free( authorization->algorithm );
	}
	authorization->algorithm = sip_strdup( algorithm );
}

void sip_authorization_set_auth( sip_authorization_t* authorization, const char* auth )
{
	if ( authorization->auth != NULL )
	{
		sip_free( authorization->auth );
	}
	authorization->auth = sip_strdup( auth );
}

void sip_authorization_set_credential( sip_authorization_t* authorization, const char* credential )
{
	if ( authorization->credential != NULL )
	{
		sip_free( authorization->credential );
	}
	authorization->credential = sip_strdup( credential );
}

void sip_authorization_set_ver_type( sip_authorization_t* authorization, const char* ver_type )
{
	if ( authorization->ver_type != NULL )
	{
		sip_free( authorization->ver_type );
	}
	authorization->ver_type = sip_strdup( ver_type );
}

void sip_authorization_set_chid( sip_authorization_t* authorization, const char* chid )
{
	if ( authorization->chid != NULL )
	{
		sip_free( authorization->chid );
	}
	authorization->chid = sip_strdup( chid );
}

void sip_authorization_set_response( sip_authorization_t* authorization, const char* response )
{
	if ( authorization->response != NULL )
	{
		sip_free( authorization->response );
	}
	authorization->response = sip_strdup( response );
}

void sip_authorization_set_digest_all( sip_authorization_t* authorization, const char* response, \
									   const char* algorithm )
{
	sip_authorization_set_type( authorization, "Digest" );
	sip_authorization_set_response( authorization, response );
	sip_authorization_set_algorithm( authorization, algorithm );
}

void sip_authorization_set_verity_all( sip_authorization_t* authorization, const char* response, \
									   const char* algorithm, char* ver_type, char* chid )
{
	sip_authorization_set_type( authorization, "Verify" );
	sip_authorization_set_response( authorization, response );
	sip_authorization_set_algorithm( authorization, algorithm );
	sip_authorization_set_ver_type(authorization, ver_type);
	sip_authorization_set_chid(authorization, chid);
}
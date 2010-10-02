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
    int i = 0;

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

    for (;;){

      int parse_ok = 0;

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

      //i = __osip_token_set ("algorithm", space, &(authorization->algorithm), &next);
      i = __sip_quoted_string_set( "algorithm", space, &(authorization->algorithm), &next );
      if ( i != 0 )
        return i;
      if ( next == NULL )
        return LIBSIP_SUCCESS;                  /* end of header detected! */
      else if (next != space){
          space = next;
          parse_ok++;
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

  *dest = NULL;

  /*
   * IMS requirement: send authorization like in:
   * Digest uri="sip:sip.antisip.com", username="joe", response=""
   */

  if ((authorization == NULL) || (authorization->auth_type == NULL))
    return LIBSIP_BADPARAMETER;

  len = strlen (authorization->auth_type) + 1;
  if (authorization->response != NULL)
    len = len + 11 + strlen(authorization->response)+2 /*for ""*/;
  len = len + 2;
  if (authorization->algorithm != NULL)
    len = len + strlen(authorization->algorithm) + 12 + 2 /*for ""*/;

  /*
   *  allocate memory
   */

  tmp = (char*)sip_malloc( len );
  if (tmp == NULL)
    return LIBSIP_NOMEM;
  *dest = tmp;

  tmp = sip_str_append(tmp, authorization->auth_type);

  /*
   * for sip-c only have two members
   */

  if (authorization->algorithm != NULL){

      tmp = sip_strn_append(tmp, " algorithm=\"", 12);
      tmp = sip_str_append(tmp, authorization->algorithm);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  if (authorization->response != NULL){

      tmp = sip_strn_append(tmp, ", response=\"", 12);
      tmp = sip_str_append(tmp, authorization->response);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  return LIBSIP_SUCCESS;
}

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
#include "../include/sip_www_authenticate.h"

int
sip_www_authenticate_init( sip_www_authenticate_t** www_authenticate )
{
    *www_authenticate = (sip_www_authenticate_t*)\
                        sip_malloc( sizeof(sip_www_authenticate_t) );
    if ( NULL == *www_authenticate )
    	return LIBSIP_NOMEM;

    /*
     *  initialize
     */

    (*www_authenticate)->auth_type = NULL;
    (*www_authenticate)->algorithm = NULL;
    (*www_authenticate)->key       = NULL;
    (*www_authenticate)->nonce     = NULL;
    (*www_authenticate)->signature = NULL;

    return LIBSIP_SUCCESS;
}

void
sip_www_authenticate_free( sip_www_authenticate_t* www_authenticate )
{
    if ( NULL == www_authenticate )
    	return;

    /*
     *  free memeber
     */

    if ( NULL != www_authenticate->auth_type )
        sip_free( www_authenticate->auth_type );
    if ( NULL != www_authenticate->algorithm )
    	sip_free( www_authenticate->algorithm );
    if ( NULL != www_authenticate->key )
    	sip_free( www_authenticate->key );
    if ( NULL != www_authenticate->nonce )
    	sip_free( www_authenticate->nonce );
    if ( NULL != www_authenticate->signature )
    	sip_free( www_authenticate->signature );

    /*
     *  free www_authenticate
     */

    sip_free( www_authenticate );
}

int
sip_www_authenticate_parse( sip_www_authenticate_t* www_authenticate, \
                                const char* value )
{

    /*
     *  example : Digest algorithm="SHA1-sess-v4",response="67d4703ba6a1cb"
     */

    const char* space = NULL;
    const char* next = NULL;
    int i = 0;

    if ( NULL == www_authenticate || NULL == value )
    	return LIBSIP_BADPARAMETER;

    space = strchr( value, ' ' );
    if ( NULL == space )
    	return LIBSIP_SYNTAXERROR;
    if ( space - value < 1 )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  get type
     */

    www_authenticate->auth_type = (char*)sip_malloc( space - value + 1);
    if ( NULL == www_authenticate->auth_type )
    	return LIBSIP_NOMEM;
    sip_strncpy( www_authenticate->auth_type, value, space - value );

    for (;;){

      int parse_ok = 0;

      /*
       *  get filed
       */

      i = __sip_quoted_string_set( "algorithm", space, &(www_authenticate->algorithm), &next );
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
      i = __sip_quoted_string_set( "nonce", space, &(www_authenticate->nonce), &next );
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

      i = __sip_quoted_string_set( "key", space, &(www_authenticate->key), &next );
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

      i = __sip_quoted_string_set( "signature", space, &(www_authenticate->signature), &next );
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

int sip_www_authenticate_to_str( sip_www_authenticate_t* www_authenticate, \
                                 char** dest )
{
  size_t len;
  char *tmp;

  *dest = NULL;

  /*
   * IMS requirement: send www_authenticate like in:?
   */

  if ((www_authenticate == NULL) || (www_authenticate->auth_type == NULL))
    return LIBSIP_BADPARAMETER;

  len = strlen (www_authenticate->auth_type) + 1;
  if (www_authenticate->nonce != NULL)
    len = len + 11 + strlen(www_authenticate->nonce) + 2/*for "" */;
  len = len + 2;
  if (www_authenticate->algorithm != NULL)
    len = len + strlen(www_authenticate->algorithm) + 12 + 2/*for "" */;
  if(www_authenticate->key != NULL)
    len = len + strlen( www_authenticate->key ) + 6;
  if(www_authenticate->signature!=NULL)
    len = len + strlen( www_authenticate->signature ) + 12 + 2/*for "" */;

  /*
   *  allocate memory
   */

  tmp = (char*)sip_malloc( len );
  if (tmp == NULL)
    return LIBSIP_NOMEM;
  *dest = tmp;

  tmp = sip_str_append(tmp, www_authenticate->auth_type);

  /*
   * for sip-c only have two members
   */

  if (www_authenticate->algorithm != NULL){

      tmp = sip_strn_append(tmp, " algorithm=\"", 12);
      tmp = sip_str_append(tmp, www_authenticate->algorithm);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  if (www_authenticate->nonce != NULL){

      tmp = sip_strn_append(tmp, ", nonce=\"", 9);
      tmp = sip_str_append(tmp, www_authenticate->nonce);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  if (www_authenticate->key != NULL){

      tmp = sip_strn_append(tmp, ", key=\"", 7);
      tmp = sip_str_append(tmp, www_authenticate->key);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  if (www_authenticate->signature != NULL){

      tmp = sip_strn_append(tmp, ", signature=\"", 13);
      tmp = sip_str_append(tmp, www_authenticate->signature);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  return LIBSIP_SUCCESS;
}

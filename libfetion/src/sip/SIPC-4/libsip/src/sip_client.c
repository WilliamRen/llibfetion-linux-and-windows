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
#include "../include/sip_client.h"

int
sip_client_init( sip_client_t** client )
{
    *client = (sip_client_t*)sip_malloc( sizeof( sip_client_t ) );
    if ( NULL == *client )
    	return LIBSIP_NOMEM;
    (*client)->type     = NULL;
    (*client)->version  = NULL;
    return LIBSIP_SUCCESS;
}

void
sip_client_free( sip_client_t* client )
{
    if ( NULL == client )
    	return;
    if ( NULL != client->type )
        sip_free( client->type );
    if ( NULL != client->version )
        sip_free( client->version );
    sip_free( client );
}

int
sip_client_parse( sip_client_t* client, const char* value )
{
    const char* space = value;
    const char* next = NULL;
    int i = 0;

    if ( NULL == client || NULL == value )
    	return LIBSIP_BADPARAMETER;

    for (;;){

      int parse_ok = 0;

      /*
       *  get filed
       */

      i = __sip_quoted_string_set( "type", space, &(client->type), &next );
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

      i = __sip_quoted_string_set( "version", space, &(client->version), &next );
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
sip_client_to_str( sip_client_t* client, char** dest )
{
  size_t len;
  char *tmp;

  *dest = NULL;


  if ( client == NULL || client->type == NULL || \
       client->version == NULL)
    return LIBSIP_BADPARAMETER;

  len = strlen (client->type) + 1;
  if (client->version != NULL)
    len = len + 5 + strlen(client->type) + 2 /*for ""*/;
  len = len + 2;
  if (client->version != NULL)
    len = len + strlen(client->version) + 10 + 2/*for ""*/;

  /*
   *  allocate memory
   */

  tmp = (char*)sip_malloc( len );
  if (tmp == NULL)
    return LIBSIP_NOMEM;
  *dest = tmp;

  /*
   * for sip-c only have two members
   */

  if (client->type != NULL){

      tmp = sip_strn_append(tmp, "type=\"", 6);
      tmp = sip_str_append(tmp, client->type);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  if (client->version != NULL){

      tmp = sip_strn_append(tmp, ", version=\"", 11);
      tmp = sip_str_append(tmp, client->version);
      tmp = sip_strn_append(tmp, "\"", 1);
  }
  return LIBSIP_SUCCESS;
}


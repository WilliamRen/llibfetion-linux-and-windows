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
#include <stdlib.h>
#include <string.h>

#include "../include/sip_message.h"


int
sip_message_set_startline_str(sip_message_t* sip, const char *value)
{
  int i = 0;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  if ( NULL != sip->startline )
    return LIBSIP_SYNTAXERROR;
  i = sip_startline_init( &(sip->startline) );
  if (i != 0)
    return i;
  i = sip_startline_parse( sip->startline, value );
  if (i != 0){
      sip_startline_free (sip->startline);
      sip->startline = NULL;
      return i;
  }
  return LIBSIP_SUCCESS;
}

int
sip_message_set_client_str(sip_message_t* sip, const char *value)
{
  int i = 0;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  if ( NULL != sip->client )
    return LIBSIP_SYNTAXERROR;
  i = sip_client_init( &(sip->client) );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_client_parse( sip->client, value );
  if (i != LIBSIP_SUCCESS){
      sip_client_free (sip->client);
      sip->client = NULL;
      return i;
  }
  return LIBSIP_SUCCESS;
}

int
sip_message_set_to_str(sip_message_t* sip, const char *value)
{
  int i = 0;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  if ( NULL != sip->to )
    return LIBSIP_SYNTAXERROR;
  i = sip_to_init( &(sip->to) );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_to_parse( sip->to, value );
  if (i != LIBSIP_SUCCESS){
      sip_to_free (sip->to);
      sip->to = NULL;
      return i;
  }
  return LIBSIP_SUCCESS;
}

int
sip_message_set_authorization_str(sip_message_t* sip, const char *value)
{
  int i = 0;
  sip_authorization_t* auth  = NULL;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  /*if ( NULL != sip->authorization )
    return LIBSIP_SYNTAXERROR;
  i = sip_authorization_init( &(sip->authorization) );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_authorization_parse( sip->authorization, value );
  if (i != LIBSIP_SUCCESS){
      sip_authorization_free (sip->authorization);
      sip->authorization = NULL;
      return i;
  }*/

  i = sip_authorization_init( &auth );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_authorization_parse( auth, value );
  if (i != LIBSIP_SUCCESS){
      sip_authorization_free (auth);
      sip->authorization = NULL;
      return i;
  }
  if ( strcmp( auth->auth_type, "Verify" ) == 0 )
	  sip->authorization_ver = auth;
  else
	sip->authorization = auth;
  
  return LIBSIP_SUCCESS;
}

int
sip_message_set_www_authenticate_str(sip_message_t* sip, const char *value)
{
  int i = 0;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  if ( NULL != sip->www_authenticate )
    return LIBSIP_SYNTAXERROR;
  i = sip_www_authenticate_init( &(sip->www_authenticate) );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_www_authenticate_parse( sip->www_authenticate, value );
  if (i != LIBSIP_SUCCESS){
      sip_www_authenticate_free (sip->www_authenticate);
      sip->www_authenticate = NULL;
      return i;
  }
  return LIBSIP_SUCCESS;
}

int
sip_message_set_cseq_str(sip_message_t* sip, const char *value)
{
  int i = 0;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  if ( NULL != sip->cseq )
    return LIBSIP_SYNTAXERROR;
  i = sip_cseq_init( &(sip->cseq) );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_cseq_parse( sip->cseq, value );
  if (i != LIBSIP_SUCCESS){
      sip_cseq_free (sip->cseq);
      sip->cseq = NULL;
      return i;
  }
  return LIBSIP_SUCCESS;
}

int
sip_message_set_common_str(sip_common_t** common, const char *value)
{
  int i = 0;

  if ( NULL == value || value[0] == '\0')
    return LIBSIP_SUCCESS;

  if ( NULL != *common )
    return LIBSIP_SYNTAXERROR;
  i = sip_common_init( common );
  if (i != LIBSIP_SUCCESS)
    return i;
  i = sip_common_parse( *common, value );
  if (i != LIBSIP_SUCCESS){
      sip_common_free(*common);
      *common = NULL;
      return i;
  }
  return LIBSIP_SUCCESS;
}

int
sip_message_get_line( const char* value, char** line, char** next )
{
    char* sz_ctrlf = NULL;
    if ( NULL == value )
    	return LIBSIP_BADPARAMETER;
    sz_ctrlf = strstr( value, CTRLLF );

    /*
     *  for the end of header
     */

    if ( NULL == sz_ctrlf){
        return LIBSIP_SYNTAXERROR;
    }
    *line = (char*)sip_malloc( sz_ctrlf - value + 1 );
    if ( NULL == *line )
    	return LIBSIP_NOMEM;
    sip_strncpy( *line, value, sz_ctrlf - value);
    *next = sz_ctrlf + 2;
    if ( (*next)[0] == '\r' && (*next)[1] == '\n' &&\
         (*next)[2] == '\r' && (*next)[3] == '\n'){
        *next = NULL;
    }
    return LIBSIP_SUCCESS;
}

int
sip_common_to_hole_string( sip_common_t* common, const char* type, char** dest )
{
    int n_ret = 0;
    char* sz_temp;
    int n_len = 0;
    int n_total = strlen( *dest );

    /*
     *  check paramter
     */

    if ( NULL == common )
    	return LIBSIP_SUCCESS;
    if ( NULL == type )
    	return LIBSIP_BADPARAMETER;

    /*
     *  call common_2_str
     */

    n_ret = sip_common_to_str( common, &sz_temp );
    if ( LIBSIP_SUCCESS != n_ret )
    	return n_ret;

    /*
     *  get the length of the string
     */

    n_len = strlen( sz_temp ) + 1 + strlen( type )/*for 'X: '*/ + 2/*for \r\n*/;
    n_total += n_len;

    /*
     *  realloc memory of dest
     */

    *dest = (char*)sip_realloc( *dest, n_total );
    strcat( *dest, type );
    strcat( *dest, sz_temp );
    strcat( *dest, "\r\n" );

    /*
     *  free sz_temp
     */

    sip_free( sz_temp );

    return LIBSIP_SUCCESS;
}

int
sip_message_get_body_length( sip_message_t* message )
{
	int n_len = 0;
	sip_context_length_t* sip_len = message->context_len;
	if ( sip_len != NULL ){
		n_len = atoi( sip_len->element );
	}
	return n_len;
}

int
sip_message_to_str( sip_message_t* message, char** dest )
{
    int n_len = 0;
    int n_total = 0;
    int n_ret = 0;
    char* sz_temp = NULL;

    /****************************************************
                        startline
    *****************************************************/
    /*
     *  check the paramter
     */

    if ( NULL == message || NULL == message->startline )
    	return LIBSIP_BADPARAMETER;

    /*
     *  add first line
     */

    n_ret = sip_startline_to_str( message->startline, &sz_temp );
    if ( LIBSIP_SUCCESS != n_ret )
    	return LIBSIP_SYNTAXERROR;

    /*
     *  get the length of the string of startline. just call strlen() is ok
     */

    n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/;
    n_total += n_len;

    /*
     *  allocate memory for the dest string
     */

    *dest = (char*)sip_malloc( n_total );

    /*
     *  copy the string of startline 2 dest string
     */

    strcpy( *dest, sz_temp );
    strcat( *dest, "\r\n" );

    /*
     *  free sz_temp
     */

    sip_free( sz_temp );

    /****************************************************
     *                      I: CN: X:                   *
     ****************************************************/

    n_ret = sip_common_to_hole_string( message->call_id, "I: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return LIBSIP_SYNTAXERROR;


    /****************************************************
     *                       Q:                         *
     ****************************************************/

    if ( NULL != message->cseq ){

        n_ret = sip_cseq_to_str( message->cseq, &sz_temp );
        if ( LIBSIP_SUCCESS != n_ret )
        	return LIBSIP_SYNTAXERROR;

        n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/ + 3/*for 'Q: '*/;
        n_total = strlen( *dest ) + n_len;

        *dest = (char*)sip_realloc( *dest, n_total );

        strcat( *dest, "Q: " );
        strcat( *dest, sz_temp );
        strcat( *dest, "\r\n" );

        sip_free( sz_temp );

    }

	/****************************************************
     *                         CN: X:                   *
     ****************************************************/

	n_ret = sip_common_to_hole_string( message->from, "F: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return LIBSIP_SYNTAXERROR;

    /****************************************************
     *                         CN: X:                   *
     ****************************************************/

	n_ret = sip_common_to_hole_string( message->cnonce, "CN: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
		return LIBSIP_SYNTAXERROR;
	
    n_ret = sip_common_to_hole_string( message->expires, "X: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return LIBSIP_SYNTAXERROR;

    /****************************************************
     *                       T:                         *
     ****************************************************/

    if ( NULL != message->to ){

        n_ret = sip_to_to_str( message->to, &sz_temp );
        if ( LIBSIP_SUCCESS != n_ret )
        	return LIBSIP_SYNTAXERROR;

        n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/ + 3/*for 'Q: '*/;
        n_total = strlen( *dest ) + n_len;

        *dest = (char*)sip_realloc( *dest, n_total );

        strcat( *dest, "T: " );
        strcat( *dest, sz_temp );
        strcat( *dest, "\r\n" );

        sip_free( sz_temp );

    }

	/****************************************************
     *                       C:                         *
     ****************************************************/	

	n_ret = sip_common_to_hole_string( message->context_type, "C: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return LIBSIP_SYNTAXERROR;

	/****************************************************
     *                       K:                         *
     ****************************************************/

	if ( message->support_list != NULL )
	{
		
		/*
		 *	loop the k list
		 */
		
		sip_support_list_t* pos = message->support_list;
		while ( pos )
		{
			n_ret = sip_common_to_hole_string( pos->sip_k, "K: ", dest );
			if ( LIBSIP_SUCCESS != n_ret )
    			return n_ret;
			pos = pos->next;
		}

	}

    /****************************************************
     *                       N:                         *
     ****************************************************/

    n_ret = sip_common_to_hole_string( message->event, "N: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return n_ret;

    /****************************************************
     *                       A:                         *
     ****************************************************/

    if ( NULL != message->authorization ){

        n_ret = sip_authorization_to_str( message->authorization, &sz_temp );
        if ( LIBSIP_SUCCESS != n_ret )
        	return LIBSIP_SYNTAXERROR;

        n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/ + 3/*for 'Q: '*/;
        n_total = strlen( *dest ) + n_len;

        *dest = (char*)sip_realloc( *dest, n_total );

        strcat( *dest, "A: " );
        strcat( *dest, sz_temp );
        strcat( *dest, "\r\n" );

        sip_free( sz_temp );

    }
	if ( NULL != message->authorization_ver ){
		
        n_ret = sip_authorization_to_str( message->authorization_ver, &sz_temp );
        if ( LIBSIP_SUCCESS != n_ret )
			return LIBSIP_SYNTAXERROR;
		
        n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/ + 3/*for 'Q: '*/;
        n_total = strlen( *dest ) + n_len;
		
        *dest = (char*)sip_realloc( *dest, n_total );
		
        strcat( *dest, "A: " );
        strcat( *dest, sz_temp );
        strcat( *dest, "\r\n" );
		
        sip_free( sz_temp );
		
    }
    /****************************************************
     *                       W:                         *
     ****************************************************/

    if ( NULL != message->www_authenticate ){

        n_ret = sip_www_authenticate_to_str( message->www_authenticate, &sz_temp );
        if ( LIBSIP_SUCCESS != n_ret )
        	return n_ret;

        n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/ + 3/*for 'Q: '*/;
        n_total = strlen( *dest ) + n_len;

        *dest = (char*)sip_realloc( *dest, n_total );

        strcat( *dest, "W: " );
        strcat( *dest, sz_temp );
        strcat( *dest, "\r\n" );

        sip_free( sz_temp );
    }

    /****************************************************
     *                       CL:                        *
     ****************************************************/

    if ( NULL != message->client ){

        n_ret = sip_client_to_str( message->client, &sz_temp );
        if ( LIBSIP_SUCCESS != n_ret )
        	return n_ret;

        n_len = strlen( sz_temp ) + 1 + 2/*for \r\n*/ + 4/*for 'Q: '*/;
        n_total = strlen( *dest ) + n_len;

        *dest = (char*)sip_realloc( *dest, n_total );

        strcat( *dest, "CL: " );
        strcat( *dest, sz_temp );
        strcat( *dest, "\r\n" );

        sip_free( sz_temp );
    }

    /****************************************************
     *                       L:                         *
     ****************************************************/

    n_ret = sip_common_to_hole_string( message->context_len, "L: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return n_ret;
	
	 /****************************************************
     *                       D:                         *
     ****************************************************/

    n_ret = sip_common_to_hole_string( message->date, "D: ", dest );
    if ( LIBSIP_SUCCESS != n_ret )
    	return n_ret;


	n_len = strlen( *dest ) + 2 + 1;
	*dest = (char*)sip_realloc( *dest, n_len );
	strcat( *dest, "\r\n" );

    /****************************************************
     *                       body:                      *
     ****************************************************/

    /*
     *  add body
     */

    if ( NULL != message->body ){

        /*
         *  get length of body
         */

        n_len = strlen( message->body )/*for \r\n*/ + 1;
        n_total = strlen( *dest ) + n_len;

        *dest = (char*)sip_realloc( *dest, n_total );

        /*
         *  for last line of header
         */


        strcat( *dest, message->body );

    }

    return LIBSIP_SUCCESS;
}

int sip_message_init( sip_message_t** message )
{
	*message = (sip_message_t*)sip_malloc( sizeof( sip_message_t ) );
	if ( NULL == *message )
    	return LIBSIP_NOMEM;
	/*
     *  initialize
     */

    (*message)->authorization	 = NULL;
	(*message)->authorization_ver= NULL;
    (*message)->body			= NULL;
    (*message)->call_id			= NULL;
	(*message)->client			= NULL;
	(*message)->cnonce			= NULL;
	(*message)->context_encode  = NULL;
	(*message)->context_type	= NULL;
	(*message)->context_len		= NULL;
    (*message)->cseq			= NULL;
    (*message)->event			= NULL;
	(*message)->expires			= NULL;
	(*message)->from			= NULL;
	(*message)->startline		= NULL;
	(*message)->to				= NULL;
    (*message)->www_authenticate= NULL;
	(*message)->support_list	= NULL;
	(*message)->date			= NULL;

	return LIBSIP_SUCCESS;
}

void
sip_message_free( sip_message_t* message )
{
    if ( message->startline )
    	sip_startline_free( message->startline );
    if ( message->from )
        sip_common_free( message->from );
    if ( message->call_id )
        sip_common_free( message->call_id );
	if ( message->cseq )
		sip_cseq_free( message->cseq );
    if ( message->cnonce )
        sip_common_free( message->cnonce );
    if ( message->context_len )
        sip_common_free( message->context_len );
	if ( message->context_type )
        sip_common_free( message->context_type );
    if ( message->context_encode )
        sip_common_free( message->context_encode );
    if ( message->client )
        sip_client_free( message->client );
    if ( message->to )
        sip_to_free( message->to );
    if ( message->expires )
        sip_common_free( message->expires );
    if ( message->event )
        sip_common_free( message->event );
    if ( message->authorization )
        sip_authorization_free( message->authorization );
	if ( message->authorization_ver )
        sip_authorization_free( message->authorization_ver );
    if ( message->www_authenticate )
        sip_www_authenticate_free( message->www_authenticate );
    if ( message->body )
    	sip_free( message->body );
	if ( message->support_list )
		sip_support_list_free( message->support_list );
	if ( message->date )
        sip_common_free( message->date );

	sip_free( message );
}

int
sip_message_parse( sip_message_t* message, const char *value )
{
    char* sz_line = NULL;
    char* sz_next = NULL;
    char* sz_temp = (char*)value;
    char* sz_body_pos = NULL;
    int n_ret = LIBSIP_SUCCESS;

    if ( NULL == value || NULL == message )
    	return LIBSIP_BADPARAMETER;

    /*
     *
     */

    sz_body_pos = strstr( value, CTRLLFLF );
    if ( NULL == sz_body_pos )
    	return LIBSIP_NOT_FOUND_HEAD_END;

    /*
     *  get first line
     */

    n_ret = sip_message_get_line( sz_temp, &sz_line, &sz_next );
    if ( LIBSIP_SUCCESS != n_ret )
    	return n_ret;

    /*
     *  parse startline
     */

    n_ret = sip_message_set_startline_str( message, sz_line );
    if ( LIBSIP_SUCCESS != n_ret )
    	return n_ret;

    sip_free( sz_line );

    sz_temp = sz_next;

    /*
     *  parse headers
     */

    while( 1 ){

        n_ret = sip_message_get_line( sz_temp, &sz_line, &sz_next );
        if( LIBSIP_SUCCESS != n_ret ){
            break;
        }
		
		/*
		 *	at the end
		 */
		
		if ( strlen( sz_line ) == 0 )
		{
			n_ret = LIBSIP_SUCCESS;
			break;
		}
		
        if( NULL == strchr( sz_line, ':' ) ){
            n_ret = LIBSIP_SYNTAXERROR;
        }

        /*
         *  parse
         */

        switch( sz_line[0] ){
        case 'A':
            {
                if ( sz_line[1] == 'L' )
                {
					
					/*
					 *	TODO for AL 
					 */
					
                }
				else if ( sz_line[1] == ':' )
                {
					n_ret = sip_message_set_authorization_str( message, sz_line + 3 );
					if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
                }
            }
            break;
        case 'F':
            {
                n_ret = sip_message_set_common_str( &(message->from), sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'T':
            {
                n_ret = sip_message_set_to_str( message, sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'C':
            {
                if ( sz_line[1] == 'N' ){
                    n_ret = sip_message_set_common_str( &(message->cnonce), sz_line + 4 );
                    if( LIBSIP_SUCCESS != n_ret )
                        return n_ret;
                }else if ( sz_line[1] == ':' )
                {
					n_ret = sip_message_set_common_str( &(message->context_type), sz_line + 3 );
                    if( LIBSIP_SUCCESS != n_ret )
                        return n_ret;
                }
                
            }
            break;
        case 'I':
            {
                n_ret = sip_message_set_common_str( &(message->call_id), sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'Q':
            {
                n_ret = sip_message_set_cseq_str( message, sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'W':
            {
                n_ret = sip_message_set_www_authenticate_str( message, sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'M':
            {

            }
            break;
		case 'K':
			{
				sip_common_t* sip_k = NULL;
				sip_support_list_t* support_list = (sip_support_list_t*)\
									sip_malloc( sizeof(sip_support_list_t) );
				n_ret = sip_message_set_common_str( &sip_k, sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;

				support_list->sip_k = sip_k;
				support_list->next = NULL;
				if ( message->support_list == NULL ){
					message->support_list = support_list;
				}else{
					sip_support_list_append( message->support_list, support_list );
				}
			}
			break;
        case 'X':
            {
                n_ret = sip_message_set_common_str( &(message->expires), sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'L':
            {
                n_ret = sip_message_set_common_str( &(message->context_len), sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'N':
            {
                n_ret = sip_message_set_common_str( &(message->event), sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'D':
            {
                n_ret = sip_message_set_common_str( &(message->date), sz_line + 3 );
                if( LIBSIP_SUCCESS != n_ret )
                    return n_ret;
            }
            break;
        case 'E':
            {

            }
            break;
        case 'S':
            {

            }
            break;
        default:
            break;
        }
        sip_free( sz_line );

        sz_temp = sz_next;

        /*
         *  if end?
         */

        if ( sz_next[0] == '\r' && sz_next[1] == '\n' ){
			
			/*
			 *	get the body
			 */
			
			int n_body_len = 0, n_body_len_real = 0;
			
			n_body_len = sip_message_get_body_length( message );

			n_body_len_real = strlen( sz_next + 2 );
			
			/*
			 *	if ok!
			 */
			
			if ( n_body_len == n_body_len_real ){
				if ( n_body_len != 0 )
				{
					message->body = (char*)sip_malloc( n_body_len + 1);
					strcpy( message->body, sz_next + 2 );
				}
			}
			if ( n_body_len > n_body_len_real ){
				n_ret = LIBSIP_BODY_NOT_COMPLETE;
			}
			if ( n_body_len < n_body_len_real ){
				if ( n_body_len != 0 )
				{
					message->body = (char*)sip_malloc( n_body_len + 1);
					strncpy( message->body, sz_next + 2, n_body_len );
				}
				n_ret = LIBSIP_BODY_BEYOND;
			}

        	break;
        }
    }


    return n_ret;
}


void
sip_message_set_common(sip_common_t** common, sip_common_t* common_set)
{
	if ( *common != NULL )
	{
		sip_common_free( *common );
	}
	*common = common_set;
}

void
sip_message_set_cseq(sip_message_t* sip, sip_cseq_t* cseq )
{
	if ( sip->cseq != NULL )
	{
		sip_cseq_free( sip->cseq );
	}
	sip->cseq = cseq;
}

void
sip_message_set_body(sip_message_t* sip, const char* body )
{
	if ( sip->body != NULL )
	{
		sip_free( sip->body );
	}
	sip->body = sip_strdup( body );
}

void
sip_message_set_www_authenticate(sip_message_t* sip, sip_www_authenticate_t* www_authenticate)
{
	if ( sip->www_authenticate != NULL )
	{
		sip_www_authenticate_free( sip->www_authenticate );
	}
	sip->www_authenticate = www_authenticate;
}

void
sip_message_set_authorization(sip_message_t* sip, sip_authorization_t* authorization)
{
	if ( sip->authorization != NULL )
	{
		sip_authorization_free( sip->authorization );
	}
	sip->authorization = authorization;
}

void
sip_message_set_authorization_ver(sip_message_t* sip, sip_authorization_t* authorization_ver)
{
	if ( sip->authorization_ver != NULL )
	{
		sip_authorization_free( sip->authorization_ver );
	}
	sip->authorization_ver = authorization_ver;
}

void
sip_message_set_to(sip_message_t* sip, sip_to_t* to)
{
	if ( sip->to != NULL )
	{
		sip_to_free( sip->to );
	}
	sip->to = to;
}

void
sip_message_set_client(sip_message_t* sip, sip_client_t* client)
{
	if ( sip->client != NULL )
	{
		sip_client_free( sip->client );
	}
	sip->client = client;
}

void
sip_message_set_startline(sip_message_t* sip, sip_startline_t* startline)
{
	if ( sip->startline != NULL )
	{
		sip_startline_free( sip->startline );
	}
	sip->startline = startline;
}

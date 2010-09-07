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

/*! \file sip_def.h
 	\brief 这个文件中包含了一些公用定义
 */

#ifndef SIP_DEF_H_INCLUDED
#define SIP_DEF_H_INCLUDED

#define LIBSIP_SUCCESS               0
#define LIBSIP_UNDEFINED_ERROR      -1
#define LIBSIP_BADPARAMETER         -2
#define LIBSIP_WRONG_STATE          -3
#define LIBSIP_NOMEM                -4
#define LIBSIP_SYNTAXERROR          -5
#define LIBSIP_NOTFOUND             -6
#define LIBSIP_API_NOT_INITIALIZED  -7

#define SIP_VERSION                 "SIP-C/4.0"

#define SIP_TRYING                        100
#define SIP_RINGING                       180
#define SIP_CALL_IS_BEING_FORWARDED       181
#define SIP_QUEUED                        182
#define SIP_SESSION_PROGRESS              183
#define SIP_OK                            200
#define SIP_ACCEPTED                      202
#define SIP_MULTIPLE_CHOICES              300
#define SIP_MOVED_PERMANENTLY             301
#define SIP_MOVED_TEMPORARILY             302
#define SIP_USE_PROXY                     305
#define SIP_ALTERNATIVE_SERVICE           380
#define SIP_BAD_REQUEST                   400
#define SIP_UNAUTHORIZED                  401
#define SIP_PAYMENT_REQUIRED              402
#define SIP_FORBIDDEN                     403
#define SIP_NOT_FOUND                     404
#define SIP_METHOD_NOT_ALLOWED            405
#define SIP_406_NOT_ACCEPTABLE            406
#define SIP_PROXY_AUTHENTICATION_REQUIRED 407
#define SIP_REQUEST_TIME_OUT              408
#define SIP_GONE                          410
#define SIP_REQUEST_ENTITY_TOO_LARGE      413
#define SIP_REQUEST_URI_TOO_LARGE         414
#define SIP_UNSUPPORTED_MEDIA_TYPE        415
#define SIP_UNSUPPORTED_URI_SCHEME        416
#define SIP_BAD_EXTENSION                 420
#define SIP_EXTENSION_REQUIRED            421
#define SIP_INTERVAL_TOO_BRIEF            423
#define SIP_TEMPORARILY_UNAVAILABLE       480
#define SIP_CALL_TRANSACTION_DOES_NOT_EXIST 481
#define SIP_LOOP_DETECTED                 482
#define SIP_TOO_MANY_HOPS                 483
#define SIP_ADDRESS_INCOMPLETE            484
#define SIP_AMBIGUOUS                     485
#define SIP_BUSY_HERE                     486
#define SIP_REQUEST_TERMINATED            487
#define SIP_NOT_ACCEPTABLE_HERE           488
#define SIP_BAD_EVENT                     489
#define SIP_REQUEST_PENDING               491
#define SIP_UNDECIPHERABLE                493
#define SIP_INTERNAL_SERVER_ERROR         500
#define SIP_NOT_IMPLEMENTED               501
#define SIP_BAD_GATEWAY                   502
#define SIP_SERVICE_UNAVAILABLE           503
#define SIP_SERVER_TIME_OUT               504
#define SIP_VERSION_NOT_SUPPORTED         505
#define SIP_MESSAGE_TOO_LARGE             513
#define SIP_BUSY_EVRYWHERE                600
#define SIP_DECLINE                       603
#define SIP_DOES_NOT_EXIST_ANYWHERE       604
#define SIP_606_NOT_ACCEPTABLE            606

/** is the status code informational */
#define SIP_IS_SIP_INFO(x)         (((x) >= 100)&&((x) < 200))
/** is the status code OK ?*/
#define SIP_IS_SIP_SUCCESS(x)      (((x) >= 200)&&((x) < 300))
/** is the status code a redirect */
#define SIP_IS_SIP_REDIRECT(x)     (((x) >= 300)&&((x) < 400))
/** is the status code a error (client or server) */
#define SIP_IS_SIP_ERROR(x)        (((x) >= 400)&&((x) < 600))
/** is the status code a client error  */
#define SIP_IS_SIP_CLIENT_ERROR(x) (((x) >= 400)&&((x) < 500))
/** is the status code a server error  */
#define SIP_IS_SIP_SERVER_ERROR(x) (((x) >= 500)&&((x) < 600))

/** \fn
	\brief
	\param
	\return
*/

void *
sip_realloc (void *ptr, size_t size);

/** \fn
	\brief
	\param
	\return
*/

void
sip_free (void *ptr);

/** \fn
	\brief
	\param
	\return
*/

void *
sip_malloc (size_t size);

/** \fn
	\brief
	\param
	\return
*/

char *
sip_strncpy (char *dest, const char *src, size_t length);

/** \fn
	\brief
	\param
	\return
*/

int
__sip_quoted_string_set ( const char *name, const char *str, \
                          char **result, const char **next );

/** \fn
	\brief
	\param
	\return
*/

char *
__sip_quote_find (const char *qstring);

/** \fn
	\brief
	\param
	\return
*/

int
sip_strncasecmp (const char *s1, const char *s2, size_t len);

/** \fn
	\brief
	\param
	\return
*/

int
sip_strcasecmp (const char *s1, const char *s2);

/** \fn
	\brief
	\param
	\return
*/

char *
sip_str_append (char *dst, const char *src);

/** \fn
	\brief
	\param
	\return
*/

char *
sip_strn_append (char *dst, const char *src, size_t len);

int
__sip_token_set (const char *name, const char *str, char **result,
                  const char **next);

#endif // SIP_DEF_H_INCLUDED

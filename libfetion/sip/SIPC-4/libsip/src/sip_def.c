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

void *
sip_realloc (void *ptr, size_t size)
{
  return realloc (ptr, size);
}

void
sip_free (void *ptr)
{
  if (ptr == NULL)
    return;
  free (ptr);
}

void *
sip_malloc (size_t size)
{
  void *ptr = malloc (size);

  if (ptr != NULL)
    memset (ptr, 0, size);
  return ptr;
}

char *
sip_strncpy (char *dest, const char *src, size_t length)
{
  strncpy (dest, src, length);
  dest[length] = '\0';
  return dest;
}

int
__sip_quoted_string_set (const char *name, const char *str,
                          char **result, const char **next)
{
  *next = str;
  if (*result != NULL)
    return LIBSIP_SUCCESS;                      /* already parsed */
  *next = NULL;
  while ((' ' == *str) || ('\t' == *str) || (',' == *str))
    if (*str)
      str++;
    else
      return LIBSIP_SYNTAXERROR;                /* bad header format */

  if (strlen (str) <= strlen (name))
    return LIBSIP_SYNTAXERROR;                  /* bad header format... */
  if (sip_strncasecmp (name, str, strlen (name)) == 0){

      const char *quote1;
      const char *quote2;
      const char *tmp;
      const char *hack = strchr (str, '=');

      if (hack == NULL)
        return LIBSIP_SYNTAXERROR;

      while (' ' == *(hack - 1))        /* get rid of extra spaces */
        hack--;
      if ((size_t) (hack - str) != strlen (name)){

          *next = str;
          return LIBSIP_SUCCESS;
      }

      quote1 = __sip_quote_find (str);
      if (quote1 == NULL)
        return LIBSIP_SYNTAXERROR;              /* bad header format... */
      quote2 = __sip_quote_find (quote1 + 1);
      if (quote2 == NULL)
        return LIBSIP_SYNTAXERROR;              /* bad header format... */
      if (quote2 - quote1 == 1){

          /* this is a special case! The quote contains nothing! */
          /* example:   Digest opaque="",cnonce=""               */
          /* in this case, we just forget the parameter... this  */
          /* this should prevent from user manipulating empty    */
          /* strings */

          tmp = quote2 + 1;     /* next element start here */
          for (; *tmp == ' ' || *tmp == '\t'; tmp++){
          }
          for (; *tmp == '\n' || *tmp == '\r'; tmp++){
          }                     /* skip LWS */
          *next = NULL;
          if (*tmp == '\0')     /* end of header detected */
            return LIBSIP_SUCCESS;
          if (*tmp != '\t' && *tmp != ' ')
            *next = tmp;
          else{                     /* it is: skip it... */
              for (; *tmp == ' ' || *tmp == '\t'; tmp++){
              }
              if (*tmp == '\0')     /* end of header detected */
                return LIBSIP_SUCCESS;
              *next = tmp;
          }
          return LIBSIP_SUCCESS;
      }
      *result = (char *) sip_malloc (quote2 - quote1 + 3);
      if (*result == NULL)
        return LIBSIP_NOMEM;

      /*
       *  copy
       */

      //sip_strncpy (*result, quote1, quote2 - quote1 + 1);
      sip_strncpy (*result, quote1 + 1, quote2 - quote1 - 1);
      tmp = quote2 + 1;         /* next element start here */
      for (; *tmp == ' ' || *tmp == '\t'; tmp++){
      }
      for (; *tmp == '\n' || *tmp == '\r'; tmp++){
      }                       /* skip LWS */
      *next = NULL;
      if (*tmp == '\0')         /* end of header detected */
        return LIBSIP_SUCCESS;
      if (*tmp != '\t' && *tmp != ' ')
        /* LWS here ? */
        *next = tmp;
      else{                       /* it is: skip it... */
          for (; *tmp == ' ' || *tmp == '\t'; tmp++){
          }
          if (*tmp == '\0')     /* end of header detected */
            return LIBSIP_SUCCESS;
          *next = tmp;
      }
  }else
    *next = str;                /* wrong header asked! */
  return LIBSIP_SUCCESS;
}

char *
__sip_quote_find (const char *qstring)
{
  char *quote;

  quote = strchr (qstring, '"');
  if (quote == qstring)         /* the first char matches and is not escaped... */
    return quote;

  if (quote == NULL)
    return NULL;                /* no quote at all... */

  /* this is now the nasty cases where '"' is escaped
     '" jonathan ros \\\""'
     |                  |
     '" jonathan ros \\"'
     |                |
     '" jonathan ros \""'
     |                |
     we must count the number of preceeding '\' */
  {
    int i = 1;

    for (;;){

        if (0 == strncmp (quote - i, "\\", 1))
          i++;
        else{
            if (i % 2 == 1)             /* the '"' was not escaped */
              return quote;

            /* else continue with the next '"' */
            quote = strchr (quote + 1, '"');
            if (quote == NULL)
              return NULL;
            i = 1;
        }
        if (quote - i == qstring - 1){

            /* special case where the string start with '\' */
            if (*qstring == '\\')
              i++;                      /* an escape char was not counted */
            if (i % 2 == 0)             /* the '"' was not escaped */
              return quote;
            else{                       /* else continue with the next '"' */
                qstring = quote + 1;    /* reset qstring because
                                           (*quote+1) may be also == to '\\' */
                quote = strchr (quote + 1, '"');
                if (quote == NULL)
                  return NULL;
                i = 1;
            }

        }
    }
    return NULL;
  }
}

int
sip_strncasecmp (const char *s1, const char *s2, size_t len)
{
  if (len == 0)
    return LIBSIP_SUCCESS;
  while ((len > 0) && (tolower (*s1) == tolower (*s2)))
    {
      len--;
      if ((len == 0) || (*s1 == '\0') || (*s2 == '\0'))
        break;
      s1++;
      s2++;
    }
  return tolower (*s1) - tolower (*s2);

}

int
sip_strcasecmp (const char *s1, const char *s2)
{
  while ((*s1 != '\0') && (tolower (*s1) == tolower (*s2)))
    {
      s1++;
      s2++;
    }
  return (tolower (*s1) - tolower (*s2));
}

char *
sip_str_append (char *dst, const char *src)
{
  while (*src != '\0')
    {
      *dst = *src;
      src++;
      dst++;
    }
  *dst = '\0';
  return dst;
}

char *
sip_strn_append (char *dst, const char *src, size_t len)
{
  memmove ((void *) dst, (void *) src, len);
  dst += len;
  *dst = '\0';
  return dst;
}

void *
osip_realloc (void *ptr, size_t size)
{
  if ( NULL == ptr )
  	return malloc( size );
  return realloc (ptr, size);
}

int
__sip_token_set (const char *name, const char *str, char **result,
                  const char **next)
{
  const char *beg;
  const char *tmp;

  *next = str;
  if (*result != NULL)
    return LIBSIP_SUCCESS;                        /* already parsed */
  *next = NULL;

  beg = strchr (str, '=');
  if (beg == NULL)
    return LIBSIP_SYNTAXERROR;                    /* bad header format... */

  if (strlen (str) < 6)
    return LIBSIP_SUCCESS;                        /* end of header... */

  while ((' ' == *str) || ('\t' == *str) || (',' == *str))
    if (*str)
      str++;
    else
      return LIBSIP_SYNTAXERROR;                  /* bad header format */

  if (sip_strncasecmp (name, str, strlen (name)) == 0){

      const char *end;

      end = strchr (str, ',');
      if (end == NULL)
        end = str + strlen (str);                 /* This is the end of the header */

      if (end - beg < 2)
        return LIBSIP_SYNTAXERROR;
      *result = (char *)sip_malloc (end - beg);
      if (*result == NULL)
        return LIBSIP_NOMEM;
      sip_strncpy(*result, beg + 1, end - beg - 1);

      /* make sure the element does not contain more parameter */
      tmp = (*end) ? (end + 1) : end;
      for (; *tmp == ' ' || *tmp == '\t'; tmp++){
      }
      for (; *tmp == '\n' || *tmp == '\r'; tmp++){
      }                         /* skip LWS */
      *next = NULL;
      if (*tmp == '\0')         /* end of header detected */
        return LIBSIP_SUCCESS;
      if (*tmp != '\t' && *tmp != ' ')
        /* LWS here ? */
        *next = tmp;
      else
      {                        /* it is: skip it... */
          for (; *tmp == ' ' || *tmp == '\t'; tmp++){
          }
          if (*tmp == '\0')     /* end of header detected */
            return LIBSIP_SUCCESS;
          *next = tmp;
     }
  }else
    *next = str;                /* next element start here */
  return LIBSIP_SUCCESS;
}

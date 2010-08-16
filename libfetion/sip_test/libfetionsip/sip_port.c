
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sip_port.h"

char *
osip_clrncpy (char *dst, const char *src, size_t len)
{
  const char *pbeg;
  const char *pend;
  char *p;
  size_t spaceless_length;

  if (src == NULL)
    return NULL;

  /* find the start of relevant text */
  pbeg = src;
  while ((' ' == *pbeg) || ('\r' == *pbeg) || ('\n' == *pbeg) || ('\t' == *pbeg))
    pbeg++;


  /* find the end of relevant text */
  pend = src + len - 1;
  while ((' ' == *pend) || ('\r' == *pend) || ('\n' == *pend) || ('\t' == *pend))
    {
      pend--;
      if (pend < pbeg)
        {
          *dst = '\0';
          return dst;
        }
    }

  /* if pend == pbeg there is only one char to copy */
  spaceless_length = pend - pbeg + 1;   /* excluding any '\0' */
  memmove (dst, pbeg, spaceless_length);
  p = dst + spaceless_length;

  /* terminate the string and pad dest with zeros until len */
  do
    {
      *p = '\0';
      p++;
      spaceless_length++;
    }
  while (spaceless_length < len);

  return dst;
}


char *
osip_str_append (char *dst, const char *src)
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

/* ---For better performance---
   Same as above, only this time we know the length */
char *
osip_strn_append (char *dst, const char *src, size_t len)
{
  memmove ((void *) dst, (void *) src, len);
  dst += len;
  *dst = '\0';
  return dst;
}

void *
osip_realloc (void *ptr, size_t size)
{
  return realloc (ptr, size);
}

void
osip_free (void *ptr)
{
  if (ptr == NULL)
    return;
  free (ptr);
}

void *
osip_malloc (size_t size)
{
  void *ptr = malloc (size);

  if (ptr != NULL)
    memset (ptr, 0, size);
  return ptr;
}

int
osip_strncasecmp (const char *s1, const char *s2, size_t len)
{
  if (len == 0)
    return OSIP_SUCCESS;
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
osip_strcasecmp (const char *s1, const char *s2)
{
  while ((*s1 != '\0') && (tolower (*s1) == tolower (*s2)))
    {
      s1++;
      s2++;
    }
  return (tolower (*s1) - tolower (*s2));
}

#include <limits.h>

int
osip_atoi (const char *number)
{
  int i;

  if (number == NULL)
    return OSIP_UNDEFINED_ERROR;
  i = strtol (number, (char **) NULL, 10);
  if (i == LONG_MIN || i == LONG_MAX)
    return OSIP_UNDEFINED_ERROR;
  return i;
#ifdef __WIN32__
  return atoi (number);
#endif
}

char *
osip_strncpy (char *dest, const char *src, size_t length)
{
  strncpy (dest, src, length);
  dest[length] = '\0';
  return dest;
}

char *
osip_strdup (const char *ch)
{
  char *copy;
  size_t length;

  if (ch == NULL)
    return NULL;
  length = strlen (ch);
  copy = (char *) osip_malloc (length + 1);
  if (copy==NULL)
	  return NULL;
  osip_strncpy (copy, ch, length);
  return copy;
}

/* in quoted-string, many characters can be escaped...   */
/* __osip_quote_find returns the next quote that is not escaped */
char *
__osip_quote_find (const char *qstring)
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

    for (;;)
      {
        if (0 == strncmp (quote - i, "\\", 1))
          i++;
        else
          {
            if (i % 2 == 1)     /* the '"' was not escaped */
              return quote;

            /* else continue with the next '"' */
            quote = strchr (quote + 1, '"');
            if (quote == NULL)
              return NULL;
            i = 1;
          }
        if (quote - i == qstring - 1)
          /* example: "\"john"  */
          /* example: "\\"jack" */
          {
            /* special case where the string start with '\' */
            if (*qstring == '\\')
              i++;              /* an escape char was not counted */
            if (i % 2 == 0)     /* the '"' was not escaped */
              return quote;
            else
              {                 /* else continue with the next '"' */
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
#define OSIP_TRACE osip_trace
int
osip_trace (char *fi, int li, osip_trace_level_t level, FILE * f, char *chfr, ...)
{
    return 1;
}

int
osip_tolower (char *word)
{
/*#if defined(HAVE_CTYPE_H) && !defined (_WIN32_WCE)

  for (; *word; word++)
    *word = (char) tolower (*word);
#else*/
  size_t i;
  size_t len = strlen (word);

  for (i = 0; i <= len - 1; i++)
    {
      if ('A' <= word[i] && word[i] <= 'Z')
        word[i] = word[i] + 32;
    }
//#endif
  return OSIP_SUCCESS;
}

int
osip_clrspace (char *word)
{
  char *pbeg;
  char *pend;
  size_t len;

  if (word == NULL)
    return OSIP_UNDEFINED_ERROR;
  if (*word == '\0')
    return OSIP_SUCCESS;
  len = strlen (word);

  pbeg = word;
  while ((' ' == *pbeg) || ('\r' == *pbeg) || ('\n' == *pbeg) || ('\t' == *pbeg))
    pbeg++;

  pend = word + len - 1;
  while ((' ' == *pend) || ('\r' == *pend) || ('\n' == *pend) || ('\t' == *pend))
    {
      pend--;
      if (pend < pbeg)
        {
          *word = '\0';
          return OSIP_SUCCESS;
        }
    }

  /* Add terminating NULL only if we've cleared room for it */
  if (pend + 1 <= word + (len - 1))
    pend[1] = '\0';

  if (pbeg != word)
    memmove (word, pbeg, pend - pbeg + 2);

  return OSIP_SUCCESS;
}

unsigned long
osip_hash (const char *str)
{
  unsigned int hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c;

  return hash & 0xFFFFFFFFu;
}

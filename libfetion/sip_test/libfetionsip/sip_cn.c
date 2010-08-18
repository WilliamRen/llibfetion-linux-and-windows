
#include <stdlib.h>
#include <stdio.h>

#include "sip_port.h"
#include "sip_message.h"
#include "sip_parser.h"

int
osip_message_set_cn (osip_message_t * sip, const char *hvalue)
{
  int i;

  if (hvalue == NULL || hvalue[0] == '\0')
    return OSIP_SUCCESS;

  if (sip->cn != NULL)
    return OSIP_SYNTAXERROR;
  i = osip_cn_init (&(sip->cn));
  if (i != 0)
    return i;
  sip->message_property = 2;
  i = osip_cn_parse(sip->cn, hvalue);
  if (i != 0)
    {
      osip_cn_free (sip->cn);
      sip->cn = NULL;
      return i;
    }
  return OSIP_SUCCESS;
}


#ifndef MINISIZE

osip_cn_t *
osip_message_get_cn (const osip_message_t * sip)
{
  return sip->cn;
}
#endif

int
osip_cn_init (osip_cn_t ** cnonce)
{

    /*
     *  call osip_from_init_c directly
     */

    return osip_from_init_c( (osip_from_c_t**)cnonce );
}

void
osip_cn_free (osip_cn_t * cnonce)
{
    osip_from_free_c( (osip_from_c_t*)cnonce );
}

int
osip_cn_parse (osip_cn_t * cnonce, const char *hvalue)
{
    return osip_from_parse_c( (osip_from_c_t*)cnonce, hvalue );
}

int
osip_cn_to_str (const osip_cn_t * cnonce, char **dest)
{
    return osip_from_to_str_c( (osip_from_c_t*)cnonce, dest );
}

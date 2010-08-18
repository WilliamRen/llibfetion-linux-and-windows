/*
  The oSIP library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001,2002,2003,2004,2005,2006,2007 Aymeric MOIZARD jack@atosc.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef _OSIP_FROM_H_
#define _OSIP_FROM_H_

#include "sip_list.h"
#include "sip_uri.h"

typedef struct osip_from osip_from_t;

struct osip_from
{
    char *displayname;
    osip_uri_t *url;
    osip_list_t gen_params;
};

typedef struct osip_from_c osip_from_c_t;
struct osip_from_c
{
    char* uid;
};

#ifdef __cplusplus
extern "C"
{
#endif

  int osip_from_init (osip_from_t ** header);

  void osip_from_free (osip_from_t * header);

  int osip_from_parse (osip_from_t * header, const char *hvalue);

  int osip_from_to_str (const osip_from_t * header, char **dest);

  int osip_from_clone (const osip_from_t * header, osip_from_t ** dest);

  void osip_from_set_displayname (osip_from_t * header, char *value);

  char *osip_from_get_displayname (osip_from_t * header);

  void osip_from_set_url (osip_from_t * header, osip_uri_t * url);

  osip_uri_t *osip_from_get_url (osip_from_t * header);

  int osip_from_param_get (osip_from_t * header, int pos, osip_generic_param_t ** dest);

#define osip_from_param_add(header,name,value)      osip_generic_param_add((&(header)->gen_params),name,value)

#define osip_from_param_get_byname(header,name,dest) osip_generic_param_get_byname((&(header)->gen_params),name,dest)

#define osip_from_get_tag(header,dest)    osip_generic_param_get_byname((&(header)->gen_params), "tag",dest)

#define osip_from_set_tag(header,value)     osip_generic_param_add((&(header)->gen_params), osip_strdup("tag"),value)

#ifndef DOXYGEN
  int osip_from_compare (osip_from_t * header1, osip_from_t * header2);
#endif

  int osip_from_tag_match (osip_from_t * from1, osip_from_t * from2);

  /*
   * add by programmeboy
   */

  int osip_from_init_c (osip_from_c_t ** header);

  void osip_from_free_c (osip_from_c_t * header);

  int osip_from_parse_c (osip_from_c_t * header, const char *hvalue);

  int osip_from_to_str_c (const osip_from_c_t * header, char **dest);

  //int osip_from_tag_match_c (osip_from_c_t * from1, osip_from_c_t * from2);

#ifdef __cplusplus
}
#endif

#endif

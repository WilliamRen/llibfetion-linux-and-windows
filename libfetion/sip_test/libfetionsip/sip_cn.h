#ifndef SIP_CN_H_INCLUDED
#define SIP_CN_H_INCLUDED

#include "sip_from.h"

typedef osip_from_c_t osip_cn_t;

int
osip_cn_init (osip_cn_t ** cnonce);

void
osip_cn_free (osip_cn_t * cnonce);

int
osip_cn_parse (osip_cn_t * cnonce, const char *hvalue);

int
osip_cn_to_str (const osip_cn_t * cnonce, char **dest);

#endif // SIP_CN_H_INCLUDED

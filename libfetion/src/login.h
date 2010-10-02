#ifndef LOGIN_H_INCLUDED
#define LOGIN_H_INCLUDED

#include "commdef.h"

#define FX_LOGIN_DATA       "<args>"                                                                    \
                            "<device type=\"PC\" version=\"284571220\" client-version=\"3.3.0370\" />"  \
                            "<caps value=\"simple-im;im-session;temp-group;personal-group\" />"         \
                            "<events value=\"contact;permission;system-message;personal-group\" />"     \
                            "<user-info attributes=\"all\" />"                                          \
                            "<presence>"                                                                \
                            "<basic value=\"400\" desc=\"\" />"                                         \
                            "</presence>"                                                               \
                            "</args>"

FX_RET_CODE fx_login( PLOGIN_DATA l_data  );
#endif // LOGIN_H_INCLUDED

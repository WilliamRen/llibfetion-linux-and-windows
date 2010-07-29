#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define __in
#define __out

#define FX_SYS_CONF_QUERY_URL "http://nav.fetion.com.cn/nav/getsystemconfig.aspx"

#define FX_QUERY_SYS_CONF   "<config>"                                                      \
                            "<user mobile-no=\"%s\" />"                                     \
                            "<client type=\"PC\" version=\"3.6.2000\" platform=\"W5.1\" />" \
                            "<servers version=\"0\" />"                                     \
                            "<service-no version=\"0\" />"                                  \
                            "<parameters version=\"0\" />"                                  \
                            "<hints version=\"0\" />"                                       \
                            "<http-applications version=\"0\" />"                           \
                            "<client-config version=\"0\" />"                               \
                            "</config>"

#define FX_SSI_FORMAT   "%s?mobileno=%s"                                                    \
                        "&domains=fetion.com.cn%3bm161.com.cn%3bwww.ikuwa.cn"               \
                        "&v4digest-type=1"                                                  \
                        "&v4digest=%s"
#include "initial.h"

typedef struct sys_conf_data
{
    struct _user_data
    {
        char sz_phone_num[20];
        char sz_password[20];
    }user_data;
    char sz_user_conf_url[256];
    char sz_sipc_proxy[100];

}sys_conf_data;

typedef struct login_data
{
    int nstatu_code;
    int  nstatu_user;
    char sz_uri[50];
    char sz_phone_num[20];
    char sz_user_id[20];

}login_data;

/*function definition*/

FX_RET_CODE fx_get_sys_conf( __in char* sz_phone_num, \
                             __in __out struct mem_struct* mem  );

FX_RET_CODE fx_get_user_conf( __in struct sys_conf_data* sys_data, \
                              __in __out struct mem_struct* mem );

#endif // CONFIG_H_INCLUDED

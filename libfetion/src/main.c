
/*! \file main.c
 *  \brief
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "initial.h"
#include "log.h"
#include "config.h"
#include "xml.h"
#include "mem.h"
#include "login.h"

extern FILE* g_log_file;
struct sys_conf_data g_sys_conf = {0};

/** \fn
 *  \brief
 *  \param
 *  \return
 */

void init_data( char* sz_phone_num, char* sz_password )
{
    strcpy( g_sys_conf.user_data.sz_phone_num, sz_phone_num );
    strcpy( g_sys_conf.user_data.sz_password, sz_password  );
}

int main()
{
    struct login_data l_data = {0};
    struct mem_struct mem_sys = {0};
    struct mem_struct mem_user = {0};

    FX_RET_CODE fx_ret;

    init_data( "1521028XXXX", "XXXX" );

    fx_ret = fx_init();
    if( fx_ret != FX_ERROR_OK){
        printf( "intialize error\n" );
        return fx_ret;
    }
    fx_ret = fx_get_sys_conf( g_sys_conf.user_data.sz_phone_num, &mem_sys );
    if ( fx_ret != FX_ERROR_OK ){
        return fx_ret;
    }
    fx_ret = fx_parse_sys_conf( &mem_sys, &g_sys_conf );
    if( fx_ret != FX_ERROR_OK){
        printf( "parse system config file error\n" );
        return fx_ret;
    }
    fx_ret = fx_get_user_conf( &g_sys_conf, &mem_user );
    if( fx_ret != FX_ERROR_OK){
        printf( "get user config file error\n" );
        return fx_ret;
    }
    fx_ret = fx_parse_user_conf( &mem_user, &l_data );
    if( fx_ret != FX_ERROR_OK){
        printf( "parse config file error\n" );
        return fx_ret;
    }
    fx_ret = fx_login( &l_data );
    fclose(g_log_file);
    return 0;
}


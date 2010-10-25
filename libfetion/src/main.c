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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
#include "protocol.h"
#include "sipc.h"
#include "login.h"

#ifdef __WIN32__
#include <windows.h>
#endif

extern FILE* g_log_file;
SYS_CONF_DATA g_sys_conf = {0};
PGROUP_LIST g_contact_list = NULL;


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
    LOGIN_DATA l_data = {0};
    MEM_STRUCT mem_sys = {0};
    MEM_STRUCT mem_user = {0};

    FX_RET_CODE fx_ret;

    init_data( "152102XXXX", "XXXX" );

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
	log_string( "=start login=" );
    fx_ret = fx_login( &l_data, &g_contact_list );

	printf( "wait for update buddies list\n" );
#ifdef __WIN32__
	Sleep( 2000 );
#endif
	/*
	 *	into command module
	 */


	printf( "\t\t\tlibfetion v1.0 by programmeboy\n" );

	while ( 1 )
	{
		char sz_msg[1024] = {0};
		int socket = fx_get_socket();
		int n_current_chat = -1;

		printf( ">>" );
		gets( sz_msg );

		/*
		 *	print contact list
		 */

		if ( strcmp( sz_msg, "print" ) == 0 )
		{
			print_group_list( g_contact_list );
		}
		else if ( memcmp( sz_msg, "TO", 2 ) == 0 )
		{
			char sz_cmd[10] = {0};
			char sz_msg1[100] = {0};
			int i = 0;

			sscanf( sz_msg, "%s %d %s", sz_cmd, &i, sz_msg1 );

			n_current_chat = i;

			fx_send_msg_to_other( socket, sz_msg1, i );

		}
		else if ( strcmp( sz_msg, "CLOSE" ) == 0 )
		{
			if ( n_current_chat != -1 )
			{
				fx_chat_dlg_helper_item_free_by_id( n_current_chat );
			}
		}
		else
		{
			log_string( "==start send msg to myself==" );
			fx_send_msg_to_yourself( socket, sz_msg );
			log_string( "==end send msg to myself==" );
		}

#ifdef __WIN32__
		Sleep( 10 );
#endif
	}

    fclose(g_log_file);
	fx_close();
    return 0;
}


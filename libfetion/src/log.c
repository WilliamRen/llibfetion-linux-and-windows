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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*include file for file operation*/
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

FILE* g_log_file = NULL;

/** \fn void log_init()
 *  \brief initialize the logging function
 *  \return
 */

void log_init()
{
    if (g_log_file != NULL)
    	return;
    g_log_file = fopen( "log_fetion.log", "a+" );
    if ( g_log_file == NULL ){
#ifdef _DEBUG
    	printf( "fetion: create log file error!\n" );
#endif
    }

}

/** \fn void log_string( char* sz_log )
 *  \brief logging a string to log_file
 *  \param sz_log the log string
 *  \return
 */

void log_string( char* sz_fmt, ... )
{
    time_t timep;
    struct tm* t_p;
    char sz_time[100] = {0};
    char sz_log_full[1024] = {0};

    va_list args;
    char sz_sprintf[1024] = {0};

    va_start( args, sz_fmt );
#ifdef __WIN32__
	_vsnprintf( sz_sprintf, 1024, sz_fmt, args );
#else
	vsnprintf( sz_sprintf, 1024, sz_fmt, args );
#endif
    va_end( args );

    sz_sprintf[1023] = 0;
    /*look at the g_file_log isn't NULL*/
    if (g_log_file == NULL){
#ifdef _DEBUG
    	printf( "you should call log_init() to initialize!\n" );
#endif
        return;
    }
    /*get time*/
    time( &timep );
    t_p = localtime( &timep );
    sprintf( sz_time, "%d:%d:%d %d,%d,%d", t_p->tm_sec, t_p->tm_min, \
             t_p->tm_hour, t_p->tm_mday, t_p->tm_mon + 1, t_p->tm_year + 1900);
    sprintf( sz_log_full, "%s: %s\n", sz_time, sz_sprintf );
    /*write to file*/
    fwrite( sz_log_full, sizeof(char), strlen( sz_log_full ), g_log_file );
    fflush(g_log_file);
}






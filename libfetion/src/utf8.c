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


#ifdef __WIN32__
#include <windows.h>
#else
#include <iconv.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utf8.h"
#include "log.h"


#ifdef __WIN32__
void utf8_to_gb ( char* src, char* dst, int len )
{
	int ret = 0;
	WCHAR* strA;
	int i= MultiByteToWideChar ( CP_UTF8, 0 , src, -1, NULL, 0 );
	if( i<=0 ){
		log_string("MultiByteToWideChar ERROR."); return;
	}
	strA = malloc( i*2 );
	MultiByteToWideChar ( CP_UTF8 , 0 , src, -1, strA , i);
	i= WideCharToMultiByte(CP_ACP,0,strA,-1,NULL,0,NULL,NULL);
	if( len >= i ){
		ret = WideCharToMultiByte (CP_ACP,0,strA,-1,dst,i,NULL,NULL);
		dst[i] = 0;
	}
	if( ret<=0 ){
		free( strA ); return;
	}
	free( strA );
}
void gb_to_utf8 ( char* src, char* dst, int len )
{
	int ret = 0;
	WCHAR* strA;
	int i= MultiByteToWideChar ( CP_ACP, 0 , src, -1, NULL, 0 );
	if( i<=0 ){
		log_string("MultiByteToWideChar ERROR."); return;
	}
	strA = malloc( i*2 );
	MultiByteToWideChar ( CP_ACP , 0 , src, -1, strA , i);
	i= WideCharToMultiByte(CP_UTF8,0,strA,-1,NULL,0,NULL,NULL);
	if( len >= i ){
		ret = WideCharToMultiByte (CP_UTF8,0,strA,-1,dst,i,NULL,NULL);
		dst[i] = 0;
	}
	if( ret<=0 ){
		free( strA ); return;
	}
	free( strA );
}

/** \fn LPSTR utf8_to_ansi( LPSTR strUtf8 )
    \brief 将utf8转化成ascII格式
    \return 转化后的ascII格式的字符串
*/

char* utf8_to_ansi( char* strUtf8 ) 
{
    int len = MultiByteToWideChar( CP_UTF8, 0, (LPSTR)strUtf8, -1, NULL, 0 );
    wchar_t* wszGBK = (wchar_t*)malloc( len * 2 + 2 );
	char* szGBK = NULL;

    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPSTR)strUtf8, -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    szGBK=(char*)malloc(len + 1);
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

    free( wszGBK );
    return szGBK;

}

/** \fn char* ansi_to_utf8( char* sz_ansi )
    \brief 将ansi编码转换成utf-8的格式
    \param sz_ansi ansi编码的字符串
    \return utf-8的字符串.
    \note 这里的返回值,用户必须自己调用free()来释放他
*/

char* ansi_to_utf8( char* sz_ansi )
{
    int len = MultiByteToWideChar(CP_ACP, 0, (LPSTR)sz_ansi, -1, NULL,0);
    wchar_t* wszUtf8 = (wchar_t*)malloc(len * 2 + 2);
	char* szUtf8 = NULL;

    memset( wszUtf8, 0, len * 2 + 2 );
    MultiByteToWideChar( CP_ACP, 0, (LPSTR)sz_ansi, -1, (LPWSTR)wszUtf8, len );

    len = WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL );
    szUtf8 = (char*)malloc(len + 1);
    memset( szUtf8, 0, len + 1 );
    WideCharToMultiByte ( CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL );

    free( wszUtf8 );
    return szUtf8;
}

#else   //Linux
void utf8_to_gb( char* src, char* dst, int len )
{
	int ret = 0;
	uint inlen = strlen( src );
	uint outlen = len;
	char* inbuf = src;
	char* outbuf = dst;
	iconv_t cd;
	cd = iconv_open( "GBK", "UTF-8" );
	if ( cd != (iconv_t)-1 ){
		ret = iconv( cd, &inbuf, &inlen, &outbuf, &outlen );
		if( ret != 0 )
			printf("iconv failed err" );
		iconv_close( cd );
	}
}
void gb_to_utf8( char* src, char* dst, int len )
{
	int ret = 0;
	uint inlen = strlen( src );
	uint outlen = len;
	char* inbuf = src;
	char* outbuf = dst;
	iconv_t cd;
	cd = iconv_open( "UTF-8", "GBK" );
	if ( cd != (iconv_t)-1 ){
		ret = iconv( cd, &inbuf, &inlen, &outbuf, &outlen );
		if( ret != 0 )
			printf("iconv failed err" );
		iconv_close( cd );
	}
}
#endif


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

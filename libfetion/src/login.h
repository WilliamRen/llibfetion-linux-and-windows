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

FX_RET_CODE fx_login( __in PLOGIN_DATA l_data, __out PGROUP_LIST* p_group_list );
int fx_get_socket();
PGROUP_LIST fx_get_group_list();

int fx_get_group_list_mutex_lock();
int fx_get_group_list_mutex_unlock();
void fx_get_group_list_mutex_init();
void fx_get_group_list_mutex_free();

#endif // LOGIN_H_INCLUDED

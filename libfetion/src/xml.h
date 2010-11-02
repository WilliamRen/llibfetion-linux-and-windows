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

#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED

#include "commdef.h"
#include "mem.h"

FX_RET_CODE fx_parse_user_conf( __in  PMEM_STRUCT mem, \
                                __out PLOGIN_DATA l_data );
FX_RET_CODE fx_parse_sys_conf( __in  PMEM_STRUCT mem, \
                               __out PSYS_CONF_DATA sys_data );

void contact_list_free( PCONTACT_LIST p_contact );
void group_list_free( PGROUP_LIST p_group );
void contact_list_append( PCONTACT_LIST p_contact, PCONTACT_LIST p_add );
void group_list_append( PGROUP_LIST p_group, PGROUP_LIST p_add );
void print_group_list( __in PGROUP_LIST p_group );
void print_whois( __in int id );

FX_RET_CODE fx_parse_contact_list( __in const char* sz_xml, __out PGROUP_LIST* p_contact_list );
FX_RET_CODE fx_parse_event( __in char* sz_xml, __out PGROUP_LIST* p_contact_list );

PCONTACT_LIST fx_find_contact_by_sip( __in PGROUP_LIST p_group, __in char* sip );
FX_RET_CODE fx_parse_query_pic( __in  PMEM_STRUCT mem, __out char** sz_chid );

#endif // XML_H_INCLUDED

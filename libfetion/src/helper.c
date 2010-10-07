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
#include <stdlib.h>
#include <string.h>

#include "commdef.h"
#include "helper.h"

DLG_HELPER g_dlg_helper = {0};
extern PGROUP_LIST g_contact_list;

PCONTACT_LIST fx_contact_list_find_by_id( __in int n_id )
{
	PGROUP_LIST p_group = g_contact_list;
	PCONTACT_LIST p_ret = NULL;
	while( p_group )
	{
		PCONTACT_LIST p_temp = p_group->p_contact;
		while ( p_temp )
		{
			if ( p_temp->id == n_id )
			{
				p_ret = p_temp;
				break;
			}
			p_temp = p_temp->next;
		}
		if ( p_ret != NULL )
			break;
		p_group = p_group->next;
	}
	return p_ret;
}

PCHAT_DLG_HELPER_LIST fx_chat_dlg_find_by_uri( __in char* sz_uri )
{
	PCHAT_DLG_HELPER_LIST p_temp = g_dlg_helper.p_chat_list;
	while( p_temp )
	{
		if ( strcmp( p_temp->p_helper.dst_uri, sz_uri ) == 0 )
		{
			break;
		}
		p_temp = p_temp->next;
	}
	return p_temp;
}

PCHAT_DLG_HELPER_LIST fx_chat_dlg_find_by_id( __in int n_id )
{
	PCHAT_DLG_HELPER_LIST p_temp = g_dlg_helper.p_chat_list;
	
	/*
	 *	first we should find the PCONTACT_LIST by id
	 */
	
	PCONTACT_LIST p_chat = fx_contact_list_find_by_id( n_id );
	if ( p_chat == NULL )
	{
		return NULL;
	}

	while( p_temp )
	{
		if ( strcmp( p_temp->p_helper.dst_uri, p_chat->sz_uri ) == 0 )
		{
			break;
		}
		p_temp = p_temp->next;
	}
	return p_temp;
}

void fx_chat_dlg_helper_append( PCHAT_DLG_HELPER_LIST p_helper, PCHAT_DLG_HELPER_LIST p_add )
{
	PCHAT_DLG_HELPER_LIST pos = p_helper;
	while(pos != NULL)
	{
		if(pos->next == NULL)
		{
			pos->next = p_add;
			break;
		}
		pos = pos->next;
	}
}

void fx_chat_dlg_helper_free( PCHAT_DLG_HELPER_LIST p_helper )
{
	PCHAT_DLG_HELPER_LIST pos = p_helper;
	while ( pos != NULL )
	{
		PCHAT_DLG_HELPER_LIST p_temp = pos->next;
		free( pos );
		pos = p_temp;
	}
}

void fx_chat_dlg_helper_item_free( __in char* sz_uri )
{
	
	PCHAT_DLG_HELPER_LIST p_temp = g_dlg_helper.p_chat_list;
	PCHAT_DLG_HELPER_LIST p_next = NULL;

	/*
	 *	first
	 */
	
	if ( strcmp( p_temp->p_helper.dst_uri, sz_uri ) == 0 )
	{
		p_next = p_temp->next;
		free( p_temp );
		g_dlg_helper.p_chat_list = p_next;
	}
	else
	{
		while ( p_temp )
		{
			p_next = p_temp->next;
			if ( p_next )
			{
				if ( strcmp( p_next->p_helper.dst_uri, sz_uri ) == 0 )
				{
					PCHAT_DLG_HELPER_LIST p_next1 = p_next->next;
					free( p_next );
					p_temp->next = p_next1;
					
				}
			}
			p_temp = p_temp->next;
		}
	}
}

void fx_chat_dlg_helper_item_free_by_id( __in int n_id )
{
	PCONTACT_LIST p_chat = fx_contact_list_find_by_id( n_id );
	if ( p_chat == NULL )
	{
		return;
	}
	fx_chat_dlg_helper_item_free( p_chat->sz_uri );
}
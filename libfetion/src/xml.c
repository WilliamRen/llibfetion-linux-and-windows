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

/*! \file xml.c
 *  \brief this file include some code of parse xml file
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "commdef.h"
#include "initial.h"
#include "log.h"
#include "login.h"
#include "crypto.h"
#include "config.h"
#include "mem.h"
#include "utf8.h"
#include "xml.h"
#include "helper.h"
#include "mutex.h"

#ifdef __WIN32__

/*
 *	i don't know where the xmlReadMemory defined
 */

extern xmlDocPtr xmlReadMemory(const char * buffer, int size, const char * URL, const char * encoding, int options);
#endif

/** \fn
 *  \brief
 *  \param
 *  \return
 */

FX_RET_CODE fx_parse_sys_conf( __in  PMEM_STRUCT mem, \
                               __out PSYS_CONF_DATA sys_data )
{
    xmlDocPtr xml_doc;
    xmlNodePtr node_root = NULL, node_child = NULL;

    /*load the xml file*/
    xml_doc = xmlParseMemory( mem->mem_ptr, mem->size );
    if (xml_doc == NULL){
        log_string( "fx_parse_conf:parse xml buffer error!" );
        return FX_ERROR_XMLPARSE;
    }
    /*get root element*/
    node_root = xmlDocGetRootElement(xml_doc);
    /*step 2, servers*/
    node_child = node_root->children;
    /*skip servers,get*/
    node_child = node_child->children;
    while( node_child ){
        if (xmlStrcmp(node_child->name, BAD_CAST("sipc-proxy")) == 0){
            char* sz_temp = xmlNodeGetContent( node_child );
            memcpy( sys_data->sz_sipc_proxy, sz_temp, strlen(sz_temp) );
        }
        else if ( xmlStrcmp(node_child->name, BAD_CAST("ssi-app-sign-in-v2")) == 0 ){
            char* sz_temp = xmlNodeGetContent( node_child );
            memcpy( sys_data->sz_user_conf_url, sz_temp, strlen(sz_temp) );
        }
        node_child = node_child->next;
    }
    /*free it*/
    xmlFreeDoc(xml_doc);

	/*
	 *	check it
	 */

	if ( strlen( sys_data->sz_sipc_proxy ) <= 0 || \
		 strlen( sys_data->sz_user_conf_url ) <= 0 )
	{
		return FX_ERROR_UNKOWN;
	}
    return FX_ERROR_OK;
}

FX_RET_CODE fx_parse_query_pic( __in  PMEM_STRUCT mem, __out char** sz_chid )
{
	xmlDocPtr xml_doc;
    xmlNodePtr node_root = NULL,node_child = NULL;
	char* sz_attr = NULL, *p_pic = NULL, *base64de = NULL;
	int n_len = 0;

	*sz_chid = NULL;


	xml_doc = xmlParseMemory(mem->mem_ptr , mem->size);
	if (xml_doc == NULL){
        log_string( "fx_parse_conf:parse xml buffer error!" );
        return FX_ERROR_XMLPARSE;
    }
	node_child = xmlDocGetRootElement(xml_doc);
	node_child = node_child->xmlChildrenNode;
	
	/*
	 *	get chid of pic
	 */
	
	sz_attr = (char*)xmlGetProp(node_child , BAD_CAST "id");
	*sz_chid = (char*)malloc( strlen(sz_attr) + 1 );
	memset( *sz_chid, 0,  strlen(sz_attr) + 1 );
	strcpy( *sz_chid, sz_attr );
	
	/*
	 *	get the length of encode string of pic
	 */
	
	p_pic = (char*)xmlGetProp(node_child , BAD_CAST "pic");
	
	/*
	 *	decode base64 buffer
	 */
	
	decode_base64( p_pic, &base64de, &n_len );
	{
		
		/*
		 *	write to file
		 */
		
		FILE* pfile = fopen( "pic.gif", "w+b" );
		fwrite( base64de, sizeof(char), n_len, pfile );
		fclose( pfile );

	}
	xmlFreeDoc(xml_doc);
	return FX_ERROR_OK;
}

/** \fn
 *  \brief
 *  \param
 *  \return
 */

FX_RET_CODE fx_parse_user_conf( __in  PMEM_STRUCT mem, \
                                __out PLOGIN_DATA l_data )
{
    xmlDocPtr xml_doc;
    xmlNodePtr node_root = NULL,node_child = NULL;

    /*load the xml file*/
    //xml_doc = xmlParseMemory( mem->mem_ptr, mem->size );
	xml_doc = xmlReadMemory(mem->mem_ptr , mem->size, NULL , "UTF-8" , 1);
    if (xml_doc == NULL){
        log_string( "fx_parse_conf:parse xml buffer error!" );
        return FX_ERROR_XMLPARSE;
    }
    /*get root element*/
    node_root = xmlDocGetRootElement(xml_doc);
    /*step 1, get the return code*/
    if(xmlHasProp(node_root, BAD_CAST("status-code"))){
        xmlChar* sz_attr = xmlGetProp(node_root, BAD_CAST("status-code"));
        /*save to struct*/
        l_data->nstatu_code = atoi((char*)sz_attr);
#ifdef _DEBUG
        //printf("get status-code is %s \n", sz_attr);
#endif
    }else{
        log_string("fx_parse_conf:can't find the status-code!");
        return FX_ERROR_XMLPARSE;
    }
	if ( l_data->nstatu_code == 200 )
	{
		/*step 2, get the uri*/
		node_child = node_root->children;
		/*first we should find the "uri" element*/
		while(node_child != NULL){
			if (xmlStrcmp(node_child->name, BAD_CAST("user")) == 0){
				break;
			}
			node_child = node_child->next;
		}
		if (node_child != NULL){
			/*found*/
			/*get uri*/
			if(xmlHasProp(node_child, BAD_CAST("uri"))){
				char sz_num[20] = {0};
				int i = 0;
				char* sz_first = NULL;
				char* sz_next = NULL;
				
				xmlChar* sz_attr = xmlGetProp(node_child, BAD_CAST("uri"));
				/*save to struct*/
				/*we should do some operation for uri*/
				sz_next  = strchr( sz_attr, '@' );
				sz_first = strchr( sz_attr, ':' );
				if ( sz_next == NULL || sz_first == NULL ){
					log_string( "fx_parse_conf:can't find the uri" );
					return FX_ERROR_XMLPARSE;
				}
				memcpy(l_data->sz_uri, (char*)(sz_first+1), (int)sz_next-(int)sz_first-1 );
				memcpy( l_data->sz_uri_full, sz_attr, strlen((char*)sz_attr) );
			}else{
				log_string("fx_parse_conf:can't find the uri!");
				return FX_ERROR_XMLPARSE;
			}
			/*get mobile-no*/
			if(xmlHasProp(node_child, BAD_CAST("mobile-no"))){
				xmlChar* sz_attr = xmlGetProp(node_child, BAD_CAST("mobile-no"));
				/*save to struct*/
				memcpy(l_data->sz_phone_num, sz_attr, strlen((char*)sz_attr) );
			}else{
				log_string("fx_parse_conf:can't find the mobile-no!");
				return FX_ERROR_XMLPARSE;
			}
			/*get user-status*/
			if(xmlHasProp(node_child, BAD_CAST("user-status"))){
				xmlChar* sz_attr = xmlGetProp(node_child, BAD_CAST("user-status"));
				/*save to struct*/
				l_data->nstatu_user = atoi((char*)sz_attr);
			}else{
				log_string("fx_parse_conf:can't find the user-status!");
				return FX_ERROR_XMLPARSE;
			}
			/*get user-id*/
			if(xmlHasProp(node_child, BAD_CAST("user-id"))){
				xmlChar* sz_attr = xmlGetProp(node_child, BAD_CAST("user-id"));
				/*save to struct*/
				memcpy(l_data->sz_user_id, sz_attr, strlen((char*)sz_attr) );
			}else{
				log_string("fx_parse_conf:can't find the user-id!");
				return FX_ERROR_XMLPARSE;
			}
		}else{
			log_string("fx_parse_conf:can't find the user element!");
			return FX_ERROR_XMLPARSE;
		}
	}else{
		/*step 2, get the algorithmuri*/
		node_child = node_root->children;
		/*first we should find the "verification" element*/
		while(node_child != NULL){
			if (xmlStrcmp(node_child->name, BAD_CAST("verification")) == 0){
				break;
			}
			node_child = node_child->next;
		}
		if (node_child != NULL){
			if(xmlHasProp(node_child, BAD_CAST("algorithm"))){
				xmlChar* sz_attr = xmlGetProp(node_child, BAD_CAST("algorithm"));
				/*save to struct*/
				memcpy(l_data->sz_algorithm, sz_attr, strlen((char*)sz_attr) );
			}else{
				log_string("fx_parse_conf:can't find the algorithm!");
				return FX_ERROR_XMLPARSE;
			}
			if(xmlHasProp(node_child, BAD_CAST("type"))){
				xmlChar* sz_attr = xmlGetProp(node_child, BAD_CAST("type"));
				/*save to struct*/
				memcpy(l_data->sz_type, sz_attr, strlen((char*)sz_attr) );
			}else{
				log_string("fx_parse_conf:can't find the type!");
				return FX_ERROR_XMLPARSE;
			}
		}	
	}
	
    /*free it*/
    xmlFreeDoc(xml_doc);
    return FX_ERROR_OK;
}

void contact_list_append( PCONTACT_LIST p_contact, PCONTACT_LIST p_add )
{
	PCONTACT_LIST pos = p_contact;
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

void group_list_append( PGROUP_LIST p_group, PGROUP_LIST p_add )
{
	PGROUP_LIST pos = p_group;
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

void contact_list_free( PCONTACT_LIST p_contact )
{
	PCONTACT_LIST pos = p_contact;
	while ( pos != NULL )
	{
		PCONTACT_LIST p_temp = pos->next;
		free( pos );
		pos = p_temp;
	}
}

void group_list_free( PGROUP_LIST p_group )
{
	PGROUP_LIST pos = p_group;
	while ( pos != NULL )
	{
		PGROUP_LIST p_temp = pos->next;
		contact_list_free( pos->p_contact );
		free( pos );
		pos = p_temp;
	}
}

PCONTACT_LIST fx_find_contact_by_sip( __in PGROUP_LIST p_group, __in char* sip )
{
	PGROUP_LIST p_temp = p_group;
	PCONTACT_LIST p_ret = NULL;

	if ( p_group == NULL || sip == NULL )
	{
		return NULL;
	}
	/*
	 *	lock
	 */

	fx_get_group_list_mutex_lock();

	while ( p_temp )
	{
		PCONTACT_LIST p_temp_contact = p_temp->p_contact;

		/*
		 *	then print
		 */

		while ( p_temp_contact )
		{
			if ( strcmp( p_temp_contact->sz_uri, sip ) == 0 )
			{
				p_ret = p_temp_contact;
				break;
			}
			p_temp_contact = p_temp_contact->next;
		}
		p_temp = p_temp->next;
	}

	/*
	 *	unlock
	 */

	fx_get_group_list_mutex_unlock();

	return p_ret;

}

void print_whois( __in int id )
{
	PCONTACT_LIST p_contact = NULL;
	
	/*
	 *	lock 
	 */
	
	fx_get_group_list_mutex_lock();
	p_contact = fx_contact_list_find_by_id( id );
	if ( p_contact == NULL )
	{
		fx_get_group_list_mutex_unlock();
		return;
	}
	printf( "*****************************************\n" );
	printf( "\tid = %d\n", p_contact->id );
	printf( "\turi = %s\n", p_contact->sz_uri );
	printf( "\tlocal name = %s\n", p_contact->sz_local_name );
	printf( "\tgroup id = %d\n", p_contact->n_group_list_id );
	printf( "\tsid = %s\n", p_contact->user_status.sz_sid );
	printf( "\tphone number = %s\n", p_contact->user_status.sz_phone_num );
	printf( "\tnick name = %s\n", p_contact->user_status.sz_nick_name );
	printf( "\timpresa = %s\n", p_contact->user_status.sz_impresa );
	printf( "\tonline stat = %d\n", p_contact->user_status.PRESENCE.n_base );
	printf( "\tdevice type = %s\n", p_contact->user_status.PRESENCE.device_type );
	printf( "\tdevice caps = %s\n", p_contact->user_status.PRESENCE.device_caps );
	printf( "*****************************************\n" );
	fx_get_group_list_mutex_unlock();

}
void print_group_list( __in PGROUP_LIST p_group )
{
	PGROUP_LIST p_temp = p_group;

	/*
	 *	lock
	 */

	fx_get_group_list_mutex_lock();
	
	printf( "*****************************************\n" );

	while ( p_temp )
	{
		PCONTACT_LIST p_temp_contact = p_temp->p_contact;

		/*
		 *	first print group list
		 */

		printf( "%s:\n", p_temp->sz_group_name );

		/*
		 *	then print
		 */

		while ( p_temp_contact )
		{
			if ( strlen( p_temp_contact->sz_local_name ) != 0 )
			{
				printf( "\t%d %-20s", p_temp_contact->id, p_temp_contact->sz_local_name );
			}
			else
			{
				if ( strlen( p_temp_contact->user_status.sz_nick_name ) != 0 )
				{
					printf( "\t%d %-20s", p_temp_contact->id, p_temp_contact->user_status.sz_nick_name );
				}
				else
				{
					printf( "\t%d %-20s", p_temp_contact->id, p_temp_contact->user_status.sz_sid );
				}
			}
			printf( "\tbase_code = %d\n", p_temp_contact->user_status.PRESENCE.n_base );
			p_temp_contact = p_temp_contact->next;
		}
		p_temp = p_temp->next;
	}
	printf( "*****************************************\n" );
	fx_get_group_list_mutex_unlock();
}

FX_RET_CODE fx_parse_contact_list( __in const char* sz_xml, __out PGROUP_LIST* p_contact_list )
{
	xmlNodePtr node_root = NULL,node_child = NULL, node_group = NULL, node_buddy = NULL;
	xmlNodePtr node_contact = NULL;
	xmlDocPtr p_doc = xmlParseMemory( sz_xml, strlen( sz_xml ) );
	int i = 1;

	node_root = xmlDocGetRootElement( p_doc );

	/*
	 *	循环root's child得到user_info
	 */

	node_child = node_root->children;
	while ( node_child )
	{
		if (xmlStrcmp(node_child->name, BAD_CAST("user-info")) == 0){
            break;
        }
        node_child = node_child->next;
	}

	/*
	 *	循环user-info得到contact list
	 */

	node_child = node_child->children;
	while( node_child )
	{
		if (xmlStrcmp(node_child->name, BAD_CAST("contact-list")) == 0){
            break;
        }
        node_child = node_child->next;
	}

	node_contact = node_child;

	if ( node_contact == NULL )
	{
		return FX_ERROR_XMLPARSE;
	}
	/*
	 *	循环contact list得到组信息.
	 */

	node_child = node_contact->children;
	while( node_child )
	{
		if (xmlStrcmp(node_child->name, BAD_CAST("buddy-lists")) == 0){
            break;
        }
        node_child = node_child->next;
	}

	node_group = node_child;

	if ( node_group == NULL )
	{
		return FX_ERROR_XMLPARSE;
	}

	/*
	 *	循环组信息添加到链表
	 */

	node_child = node_group->children;
	while ( node_child )
	{
		PGROUP_LIST p_group = (PGROUP_LIST)malloc( sizeof(GROUP_LIST) );
		memset( p_group, 0, sizeof(GROUP_LIST) );
		if ( xmlStrcmp(node_child->name, BAD_CAST("buddy-list")) == 0 )
		{
			if ( xmlHasProp( node_child, BAD_CAST( "id" ) ) )
			{
				xmlChar* sz_attr = NULL;
				sz_attr = xmlGetProp(node_child, BAD_CAST("id"));
				p_group->n_group_id = atoi( (char*)sz_attr );
			}
			if ( xmlHasProp( node_child, BAD_CAST( "name" ) ) )
			{
				xmlChar* sz_attr = NULL;
				char* sz_asc = NULL;

				sz_attr = xmlGetProp(node_child, BAD_CAST("name"));

            #ifdef __WIN32__
				sz_asc = utf8_to_ansi( (char*)sz_attr );
            #else
                sz_asc = sz_attr;
            #endif
				strcpy( p_group->sz_group_name, sz_asc );
				free( sz_asc );
			}
			if ( *p_contact_list == NULL )
				*p_contact_list = p_group;
			else
			group_list_append( *p_contact_list, p_group );
		}

		/*
		 *	next
		 */

		node_child = node_child->next;
	}
	
	/*
	 *	group == 0 的情况
	 */
	
	{
		PGROUP_LIST p_group = (PGROUP_LIST)malloc( sizeof(GROUP_LIST) );
		memset( p_group, 0, sizeof(GROUP_LIST) );
		
		p_group->n_group_id = 0;
		strcpy( p_group->sz_group_name, "not grouped" );

		if ( *p_contact_list == NULL )
			*p_contact_list = p_group;
		else
			group_list_append( *p_contact_list, p_group );
	}

	/*
	 *	循环contact 得到buddies
	 */

	node_child = node_contact->children;
	while( node_child )
	{
		if (xmlStrcmp(node_child->name, BAD_CAST("buddies")) == 0){
            break;
        }
        node_child = node_child->next;
	}

	node_buddy = node_child;

	if ( node_buddy == NULL )
	{
		return FX_ERROR_XMLPARSE;
	}

	/*
	 *	循环 buddy 添加到链表
	 */

	node_child = node_buddy->children;

	while ( node_child )
	{
		if ( xmlStrcmp(node_child->name, BAD_CAST("b")) == 0 )
		{
			PCONTACT_LIST p_contact = (PCONTACT_LIST)malloc( sizeof(CONTACT_LIST) );
			PGROUP_LIST p_temp = *p_contact_list;

			memset( p_contact, 0, sizeof(CONTACT_LIST) );
			if ( xmlHasProp( node_child, BAD_CAST( "i" ) ) )
			{
				xmlChar* sz_attr = NULL;

				sz_attr = xmlGetProp(node_child, BAD_CAST("i"));

				strcpy( p_contact->sz_user_id, (char*)sz_attr );
			}
			if ( xmlHasProp( node_child, BAD_CAST( "u" ) ) )
			{
				xmlChar* sz_attr = NULL;

				sz_attr = xmlGetProp(node_child, BAD_CAST("u"));
				strcpy( p_contact->sz_uri, (char*)sz_attr );
			}
			if ( xmlHasProp( node_child, BAD_CAST( "n" ) ) )
			{
				xmlChar* sz_attr = NULL;
				char* sz_asc = NULL;

				sz_attr = xmlGetProp(node_child, BAD_CAST("n"));
            #ifdef __WIN32__
				sz_asc = utf8_to_ansi( (char*)sz_attr );
            #else
                sz_asc = sz_attr;
            #endif
				strcpy( p_contact->sz_local_name, (char*)sz_asc );
				free( sz_asc );
			}
			if ( xmlHasProp( node_child, BAD_CAST( "o" ) ) )
			{
				xmlChar* sz_attr = NULL;
				sz_attr = xmlGetProp(node_child, BAD_CAST("o"));
				p_contact->n_online_notify = atoi( (char*)sz_attr );
			}
			if ( xmlHasProp( node_child, BAD_CAST( "r" ) ) )
			{
				xmlChar* sz_attr = NULL;
				sz_attr = xmlGetProp(node_child, BAD_CAST("r"));
				p_contact->n_relation_status= atoi( (char*)sz_attr );
			}
			if ( xmlHasProp( node_child, BAD_CAST( "l" ) ) )
			{
				xmlChar* sz_attr = NULL;
				sz_attr = xmlGetProp(node_child, BAD_CAST("l"));
				
				/*
				 *	for buddy-lists is null
				 */
				
				if ( strlen( sz_attr ) == 0 )
					p_contact->n_group_list_id = 0;
				else
					p_contact->n_group_list_id = atoi( (char*)sz_attr );
			}

			p_contact->id = i;
			i++;

			/*
			 *	add to group list
			 */

			while ( p_temp )
			{
				if ( p_temp->n_group_id == p_contact->n_group_list_id )
				{
					/*
					 *	add to list
					 */

					if ( p_temp->p_contact == NULL )
						p_temp->p_contact = p_contact;
					else
						contact_list_append( p_temp->p_contact, p_contact );
				}
				p_temp = p_temp->next;
			}
		}

		/*
		 *	next
		 */

		node_child = node_child->next;
	}

	return FX_ERROR_OK;
}

FX_RET_CODE fx_parse_event( __in char* sz_xml, __out PGROUP_LIST* p_contact_list )
{
	xmlNodePtr node_root = NULL,node_child = NULL, node_person = NULL;
	xmlDocPtr p_doc = xmlParseMemory( sz_xml, strlen( sz_xml ) );

	/*
	 *	here use thread to parse the list so we should lock it
	 */

	fx_get_group_list_mutex_lock();

	/*
	 *	events
	 */

	node_root = xmlDocGetRootElement( p_doc );

	/*
	 *	event
	 */

	node_child = node_root->children;

	while( node_child )
	{
		if (xmlStrcmp(node_child->name, BAD_CAST("event")) == 0){
            break;
        }
        node_child = node_child->next;
	}

	if ( node_child == NULL )
	{
		fx_get_group_list_mutex_unlock();
		return FX_ERROR_XMLPARSE;
	}
	/*
	 *	contacts
	 */

	node_child = node_child->children;
	while( node_child )
	{
		if (xmlStrcmp(node_child->name, BAD_CAST("contacts")) == 0){
            break;
        }
        node_child = node_child->next;
	}

	if ( node_child == NULL )
	{
		fx_get_group_list_mutex_unlock();
		return FX_ERROR_XMLPARSE;
	}

	node_child = node_child->children;

	/*
	 *	loop contact;
	 */

	while ( node_child )
	{
		if ( xmlStrcmp(node_child->name, BAD_CAST("c")) == 0  )
		{
			if ( xmlHasProp( node_child, BAD_CAST( "id" ) ) )
			{
				xmlChar* sz_attr = NULL;
				PGROUP_LIST p_tmp = *p_contact_list;
				PCONTACT_LIST p_dst = NULL;

				sz_attr = xmlGetProp(node_child, BAD_CAST("id"));

				/*
				 *	去PGROUP_LIST里面比较USERID去
				 */

				while( p_tmp )
				{
					PCONTACT_LIST p_contact = p_tmp->p_contact;
					while ( p_contact )
					{
						if ( strcmp( p_contact->sz_user_id, sz_attr ) == 0 )
						{
							p_dst = p_contact;
							break;
						}
						p_contact = p_contact->next;
					}
					if ( p_dst != NULL )
						break;
					p_tmp = p_tmp->next;
				}

				if ( p_dst != NULL )
				{
					node_person = node_child->children;

					while( node_person )
					{
						if ( xmlStrcmp(node_person->name, BAD_CAST("p")) == 0 )
						{
							if ( xmlHasProp( node_person, BAD_CAST( "sid" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("sid"));

								strcpy( p_dst->user_status.sz_sid, (char*)sz_attr );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "su" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("su"));
								strcmp( p_dst->user_status.sz_uri, (char*)sz_attr );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "n" ) ) )
							{
								xmlChar* sz_attr = NULL;
								char* sz_asc = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("n"));
                            #ifdef __WIN32__
								sz_asc = utf8_to_ansi( (char*)sz_attr );
                            #else
                                sz_asc = sz_attr;
                            #endif
								strcpy( p_dst->user_status.sz_nick_name, (char*)sz_asc );
								log_string( "get the nick name of %s userid %s\n", sz_asc, p_dst->sz_uri );
								free( sz_asc );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "i" ) ) )
							{
								xmlChar* sz_attr = NULL;
								char* sz_asc = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("i"));
                            #ifdef __WIN32__
								sz_asc = utf8_to_ansi( (char*)sz_attr );
                            #else
                                sz_asc = sz_attr;
                            #endif
								strcpy( p_dst->user_status.sz_impresa, (char*)sz_asc );
								free( sz_asc );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "m" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("m"));
								
								strcpy( p_dst->user_status.sz_phone_num, (char*)sz_attr );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "sms" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("sms"));
								//printf( "\tsms = %s\n", (char*)sz_attr );
							}

						}
						else if ( xmlStrcmp(node_person->name, BAD_CAST("pr")) == 0 )
						{
							if ( xmlHasProp( node_person, BAD_CAST( "b" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("b"));
								if ( sz_attr != NULL )
								{
#ifdef _DEBUG
									log_string( "get the base code %s\n", sz_attr );
#endif
									p_dst->user_status.PRESENCE.n_base = atoi( (char*)sz_attr );
								}
							}
							if ( xmlHasProp( node_person, BAD_CAST( "d" ) ) )
							{
								xmlChar* sz_attr = NULL;
								char* sz_asc = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("d"));
                            #ifdef __WIN32__
								sz_asc = utf8_to_ansi( (char*)sz_attr );
							#else
                                sz_asc = sz_attr;
							#endif
								strcpy( p_dst->user_status.PRESENCE.desc, sz_asc );
								free( sz_asc );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "dt" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("dt"));
								strcpy( p_dst->user_status.PRESENCE.device_type, sz_attr );
							}
							if ( xmlHasProp( node_person, BAD_CAST( "dc" ) ) )
							{
								xmlChar* sz_attr = NULL;
								sz_attr = xmlGetProp(node_person, BAD_CAST("dc"));
								strcpy( p_dst->user_status.PRESENCE.device_caps, sz_attr );
							}
						}

						/*
						 *	next
						 */

						node_person = node_person->next;
					}

				}
			}

		}

		/*
		 *	next
		 */

		node_child = node_child->next;

	}

	/*
	 *	unlock it
	 */

	fx_get_group_list_mutex_unlock();

	return FX_ERROR_OK;
}

/*! \file xml.c
 *  \brief this file include some code of parse xml file
 */

#include <stdio.h>
#include <stdlib.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "initial.h"
#include "log.h"
#include "config.h"
#include "mem.h"
#include "xml.h"

/** \fn
 *  \brief
 *  \param *  \return
 */

FX_RET_CODE fx_parse_sys_conf( __in  struct mem_struct* mem, \
                               __out struct sys_conf_data* sys_data )
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
            printf( "%s\n", sz_temp );
        }
        else if ( xmlStrcmp(node_child->name, BAD_CAST("ssi-app-sign-in-v2")) == 0 ){
            char* sz_temp = xmlNodeGetContent( node_child );
            memcpy( sys_data->sz_user_conf_url, sz_temp, strlen(sz_temp) );
            printf( "%s\n", sz_temp );
        }
        node_child = node_child->next;
    }
    /*free it*/
    xmlFreeDoc(xml_doc);
    return FX_ERROR_OK;
}

/** \fn
 *  \brief
 *  \param
 *  \return
 */

FX_RET_CODE fx_parse_user_conf( __in  struct mem_struct* mem, \
                                __out struct login_data* l_data )
{
    xmlDocPtr xml_doc;
    xmlNodePtr node_root = NULL,node_child = NULL;

    /*load the xml file*/
    xml_doc = xmlParseMemory( mem->mem_ptr, mem->size );
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
        printf("get status-code is %s \n", sz_attr);
#endif
    }else{
        log_string("fx_parse_conf:can't find the status-code!");
        return FX_ERROR_XMLPARSE;
    }
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
    /*free it*/
    xmlFreeDoc(xml_doc);
    return FX_ERROR_OK;
}







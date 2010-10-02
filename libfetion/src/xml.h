#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED

FX_RET_CODE fx_parse_user_conf( __in  PMEM_STRUCT mem, \
                                __out PLOGIN_DATA l_data );
FX_RET_CODE fx_parse_sys_conf( __in  PMEM_STRUCT mem, \
                               __out PSYS_CONF_DATA sys_data );

#endif // XML_H_INCLUDED

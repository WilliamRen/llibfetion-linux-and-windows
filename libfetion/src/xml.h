#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED

FX_RET_CODE fx_parse_user_conf( __in  struct mem_struct* mem, \
                                __out struct login_data* l_data );
FX_RET_CODE fx_parse_sys_conf( __in  struct mem_struct* mem, \
                               __out struct sys_conf_data* sys_data );

#endif // XML_H_INCLUDED

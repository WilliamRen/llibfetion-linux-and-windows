#ifndef SIP_PORT_H_INCLUDED
#define SIP_PORT_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif


    /*
     *  some thing
     */

    char *
    osip_clrncpy (char *dst, const char *src, size_t len);

    char *
    osip_str_append (char *dst, const char *src);

    char *
    osip_strn_append (char *dst, const char *src, size_t len);

    void
    osip_free (void *ptr);

#ifdef __cplusplus
}
#endif

#define OSIP_SUCCESS               0
#define OSIP_UNDEFINED_ERROR      -1
#define OSIP_BADPARAMETER         -2
#define OSIP_WRONG_STATE          -3
#define OSIP_NOMEM                -4
#define OSIP_SYNTAXERROR          -5
#define OSIP_NOTFOUND             -6
#define OSIP_API_NOT_INITIALIZED  -7

#define OSIP_NO_NETWORK           -10
#define OSIP_PORT_BUSY            -11
#define OSIP_UNKNOWN_HOST         -12

#define OSIP_DISK_FULL            -30
#define OSIP_NO_RIGHTS            -31
#define OSIP_FILE_NOT_EXIST       -32

#define OSIP_TIMEOUT              -50
#define OSIP_TOOMUCHCALL          -51
#define OSIP_WRONG_FORMAT         -52
#define OSIP_NOCOMMONCODEC        -53


#endif // SIP_PORT_H_INCLUDED

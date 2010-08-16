#ifndef SIP_PORT_H_INCLUDED
#define SIP_PORT_H_INCLUDED

#include "sip_const.h"
#include "sip_list.h"

#ifdef __cplusplus
extern "C"
{
#endif


    /**************************/
/* LOG&DEBUG support      */
/**************************/

#define LOG_TRUE  1
#define LOG_FALSE 0
/* levels */
  typedef enum _trace_level
  {
    TRACE_LEVEL0 = 0,
#define OSIP_FATAL    0
    TRACE_LEVEL1 = 1,
#define OSIP_BUG      1
    TRACE_LEVEL2 = 2,
#define OSIP_ERROR    2
    TRACE_LEVEL3 = 3,
#define OSIP_WARNING  3
    TRACE_LEVEL4 = 4,
#define OSIP_INFO1    4
    TRACE_LEVEL5 = 5,
#define OSIP_INFO2    5
    TRACE_LEVEL6 = 6,
#define OSIP_INFO3    6
    TRACE_LEVEL7 = 7,
#define OSIP_INFO4    7
    END_TRACE_LEVEL = 8
  }
  osip_trace_level_t;


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

    char *
    __osip_quote_find (const char *qstring);

    #define OSIP_TRACE
    int
    osip_trace (char *fi, int li, osip_trace_level_t level, FILE * f, char *chfr, ...);

    int
    osip_tolower (char *word);

    int
    osip_clrspace (char *word);

    unsigned long
    osip_hash (const char *str);

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

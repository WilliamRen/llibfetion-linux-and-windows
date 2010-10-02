#ifndef INITIAL_H_INCLUDED
#define INITIAL_H_INCLUDED

typedef int FX_RET_CODE;

/*macro definition*/

#define FX_ERROR_OK         0
#define FX_ERROR_INIT       100
#define FX_ERROR_FILE       101
#define FX_ERROR_NOINITIAL  102
#define FX_ERROR_CURL       103
#define FX_ERROR_XMLPARSE   104
#define FX_ERROR_MEM        105
#define FX_ERROR_SOCKET     106
#define FX_ERROR_THREAD     107
#define FX_ERROR_UNKOWN     200

//#define FX_LOGIN_URL		"https://uid.fetion.com.cn/ssiportal/SSIAppSignIn.aspx" \
//                            "?mobileno=%s&pwd=%s"

/*function definition*/

FX_RET_CODE fx_init();

#endif // INITIAL_H_INCLUDED

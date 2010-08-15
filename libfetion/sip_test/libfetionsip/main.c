#include <stdio.h>
#include <stdlib.h>

#include "sip_cseq.h"

int main()
{
    char sz_test[] = "1 R\r\n";

    osip_cseq_t* cseq = NULL;
    osip_cseq_init( &cseq );
    if( cseq == NULL )
        return -1;
    osip_cseq_parse( cseq, sz_test );

    printf( "number:%s method:%s\n", cseq->number, cseq->method );

    osip_cseq_free( cseq );

    return 0;
}

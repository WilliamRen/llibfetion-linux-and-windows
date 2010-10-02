
/*! \file mem.c
 *  \brief 其中包含了libcurl中的writefunction的回调函数的实现
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "mem.h"

/** \fn void* myrealloc(void *ptr, size_t size)
 *  \brief reallocate memory for a specied
 *  \param
 *  \return
 */

void* myrealloc(void *ptr, size_t size)
{
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

void myfree( PMEM_STRUCT mem )
{
    if( mem ){
        if( mem->mem_ptr ){
			free( mem->mem_ptr );
			memset( mem, 0, sizeof( MEM_STRUCT ) );
        }
    }
}

/** \fn size_t write_mem_call_back(void *ptr, size_t size, \
                            size_t nmemb, void *data)
 *  \brief this is the call back function of libcurl for WRITEFUNCTION
 *  \param ptr the data we received
 *  \param size the size of ptr
 *  \param nmemb 粒度
 *  \param data the buffer or FILE*
 *  \return buffer real size
 */

size_t write_mem_call_back(void *ptr, size_t size, \
                            size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  PMEM_STRUCT mem = (PMEM_STRUCT)data;

  mem->mem_ptr = (char*)myrealloc(mem->mem_ptr, mem->size + realsize + 1);
  if (mem->mem_ptr) {
    memcpy(&(mem->mem_ptr[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->mem_ptr[mem->size] = 0;
  }
  return realsize;
}

#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

typedef struct _mem_struct
{
    size_t size;
    char* mem_ptr;
	
}MEM_STRUCT, *PMEM_STRUCT;

void* myrealloc(void *ptr, size_t size);
size_t write_mem_call_back(void *ptr, size_t size, \
                            size_t nmemb, void *data);
void myfree( PMEM_STRUCT mem );

#endif // MEM_H_INCLUDED

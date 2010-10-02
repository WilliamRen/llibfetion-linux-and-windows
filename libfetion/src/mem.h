#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

typedef struct mem_struct
{
    size_t size;
    char* mem_ptr;

}mem_struct;

void* myrealloc(void *ptr, size_t size);
size_t write_mem_call_back(void *ptr, size_t size, \
                            size_t nmemb, void *data);

#endif // MEM_H_INCLUDED

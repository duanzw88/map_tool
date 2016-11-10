#ifndef __MEM_H
#define __MEM_H

#include "except.h"

extern const Except_T Mem_Failed;

extern void *mem_alloc(long nbytes,const char *file,int line);
extern void *mem_calloc(long count,long nbytes,const char *file,int line);
extern void mem_free(void *ptr,const char *file,int line);
extern void *mem_resize(void *ptr,long nbytes,const char *file,int line);
void mem_leak(void (*apply)(const void *ptr,
                            long size,
                            const char *file,
                            int line,
                            void *cl),
                void *cl);
#define ALLOC(nbytes) mem_alloc((nbytes),__FILE__,__LINE__)
#define CALLOC(count,nbytes) mem_calloc((count),(nbytes),__FILE__,__LINE__)

//NEW接口仅适用于要分配的字节数在编译时已知
#define NEW(p) 		((p) = ALLOC((long)sizeof *(p)))
#define NEW0(p) 	((p) = CALLOC(1,(long)sizeof *(p)))
#define FREE(ptr)	((void)(mem_free((ptr),__FILE__,__LINE__),(ptr) = 0))
#define RESIZE(ptr,nbytes) ((ptr) = mem_resize((ptr),(nbytes),__FILE__,__LINE__))


#endif

#ifndef __ARENA_H
#define __ARENA_H
#include "except.h"

#define T arena_t
typedef struct T *T;

extern const Except_T Arena_NewFailed;
extern const Except_T Arena_Failed;


/**
 * 创建一个内存池，如果该函数无法分配内存池，则引发Arena_NewFailed异常
 * @return  返回指向新建内存池的一个不透明指针
 */
extern T 	arena_new(void);

/**
 * 释放与*ap内存池相关联的内存，即释放内存池本身，并将*ap清零
 * @param ap 内存池
 */
extern void arena_dispose(T *ap);
//内存池分配函数
/**
 * 在内存池中分配内存块
 * @param  arena  目标内存池
 * @param  nbytes 需要分配的新块的大小
 * @param  file   调用分配函数的文件名
 * @param  line   调用分配函数的行号
 * @return        新内存块的第一个字节的指针
 */
extern void *arena_alloc(T arena,long nbytes,const char *file,int line);

extern void *arena_calloc(T arena,long count,long nbytes,const char *file,int line);
//释放内存池中所有的内存
extern void arena_free(T arena);

#define ARENA_ALLOC(a,len) arena_alloc((a),(len),__FILE__,__LINE__)
#define ARENA_CALLOC(a,count,len) arena_calloc((a),(count),(len),__FILE__,__LINE__)

#undef T

#endif

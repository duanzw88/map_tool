#ifndef __GETWORD_H
#define __GETWORD_H

#include <stdio.h>

/**
*
* 从文件中fp中读取下一个单词，将其作为0结尾的字符串存储到buf[0..size-1]中，并返回1
* 当达到文件末尾而无法读取单词时，返回0
* 函数first和rest测试字符是否属于first和rest集合
* 一个单词是一个连续的字符序列，其起始字符用first函数测试会返回非0值，后接字符用rest测试时将返回非0值
*/
extern int getword(FILE *fp,char *buf,int size,int first(int c),int rest(int c));

#endif

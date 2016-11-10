#ifndef __ATOM_H
#define __ATOM_H

/**
 * 获取参数原子的长度
 * @param  str 参数原子
 * @return     返回str的长度
 */
extern 		 int 	atom_length	(const char *str);
extern		 void	atom_link_length();

/**
 * 用字符串和长度新建一个原子
 * @param  str 原子的字符串序列
 * @param  len str的长度
 * @return     [description]
 */
extern const char   *atom_new	(const char *str,int len);
/**
 * 用字符串新建一个原子
 * @param  str 字符串
 * @return     [description]
 */
extern const char	*atom_string(const char *str);

/**
 * 把整数作为字符串当成原子
 * @param  n 整数参数
 * @return   返回对应于以字符串表示长整数n的样子
 */
extern const char	*atom_int	(long n);
#endif

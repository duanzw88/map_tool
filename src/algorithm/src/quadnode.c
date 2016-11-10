

#include "quadnode.h"

#define T quadnode_t

#define QUADNODE_NW 	0
#define QUADNODE_NE 	1
#define QUADNODE_SW 	2
#define QUADNODE_SE 	3
#define QUADNODE_MAX	4

typedef struct arect
{
	double left;
	double top;
	double right;
	double bottom;
};
struct alist
{
	struct alist *next;
	void 		 *data;
};
struct T
{
	int 			split_flag;
	T 				children[QUADNODE_MAX];
	T 				parent;
	struct arect 	rect;
	struct alist 	*list;
};

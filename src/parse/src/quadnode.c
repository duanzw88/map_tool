#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#include "quadnode.h"

#define T  quadnode_t

#define THRESHOLD 8
/**
 * 子空间莫顿编码
 */
#define NODE_I      3
#define NODE_II     1
#define NODE_III    2
#define NODE_IV     4
#define NODE_SELF   0

struct alist
{
    struct alist *next;
    void    *data;
};
struct T
{
    int         splited;        //本空间节点是否已经划分过子空间
    int         count;          //本空间节点要素列表存放的要素数目
    envelop_t   subspace[5];    //本空间节点的子空间
    T           children[5];    //本空间节点的子节点
    struct alist *featurelist;  //本空间节点要素列表
};

static void _split_space(T qn);
static int  _space_index(T qn,envelop_t box);
static void _insert_to_subspace(T qn,int spaceindex,struct alist *al);
static void _insert(T qn,struct alist *al);
T    quadnode_new(double left,double top,double right,double bottom)
{
    T qn;

    NEW0(qn);
    qn->splited = 0;
    qn->count = 0;
    qn->featurelist = NULL;

    qn->subspace[NODE_SELF] = envelop_new(left,top,right,bottom);
    qn->subspace[NODE_I]    = envelop_new((left + right)/2.0, top, right, (top + bottom)/2.0);
    qn->subspace[NODE_II]   = envelop_new(left,top,(left + right)/2.0,(top + bottom)/2.0);
    qn->subspace[NODE_III]  = envelop_new(left,(top + bottom)/2.0,(left + right)/2.0,bottom);
    qn->subspace[NODE_IV]   = envelop_new((left + right)/2.0,(top + bottom)/2.0,right,bottom);

    qn->children[NODE_SELF] = qn;
    qn->children[NODE_I]    = NULL;
    qn->children[NODE_II]   = NULL;
    qn->children[NODE_III]  = NULL;
    qn->children[NODE_IV]   = NULL;

    return qn;
}
T    quadnode_new_space(envelop_t space)
{
    assert(space);
    return quadnode_new(envelop_get_left(space),envelop_get_top(space),envelop_get_right(space),envelop_get_bottom(space));
}

void     quadnode_insert(T qn,feature_t f)
{
    assert(qn);
    assert(f);

    struct alist *featurewrap;
    featurewrap = (struct alist *)ALLOC(sizeof(*featurewrap));
    featurewrap->next = NULL;
    featurewrap->data = f;

    _insert(qn,featurewrap);
}
void     quadnode_remove(T qn,feature_t f)
{

}

int      quadnode_contains_check_point(T qn,double x,double y)
{
    int i;
    struct alist *list;

    assert(qn);

    list = qn->featurelist;
    while(list)
    {
        feature_t f = (feature_t)list->data;
        envelop_t e = feature_get_extent(f);
        if(envelop_contains_point(e,x,y))
        {
            return 1;
        }
        list = list->next;
    }

    //递归检查子节点
    for(i = 1; i < 5; i++)
    {
        if(envelop_contains_point(qn->subspace[i],x,y))
        {
            if(qn->children[i] && quadnode_contains_check_point(qn->children[i],x,y))
            {
                return 1;
            }
        }
    }

    return 0;
}
int      quadnode_contains_intersect_check_box(T qn,envelop_t checkbox)
{
    int i;
    struct alist *list;

    assert(qn);
    list = qn->featurelist;

    while (list)
    {
        feature_t f = (feature_t)list->data;
        envelop_t e = feature_get_extent(f);
        if(envelop_intersect(e,checkbox))
        {
            return 1;
        }
        list = list->next;
    }

    for(i = 1; i < 5; i++)
    {
        if(envelop_intersect(qn->subspace[i],checkbox))
        {
            if(qn->children[i] && quadnode_contains_intersect_check_box(qn->children[i],checkbox))
            {
                return 1;
            }
        }
    }

    return 0;
}

int      quadnode_search(T qn,envelop_t searchbox,double maxdistance,int maxcount,seq_t retlist)
{
    int count,i;
    double maxdistsq;
    struct alist *list;

    assert(qn);
    assert(searchbox);
    assert(retlist);
    assert(maxdistance >= 0);
    assert(maxcount >= 0);

    count = maxcount - seq_length(retlist);
    if(count <= 0)
    {
        return 0;
    }

    maxdistsq = maxdistance * maxdistance;

    for(i = 1; i < 5; i++)
    {
        if(envelop_intersect(qn->subspace[i],searchbox) && qn->children[i])
        {
            int ret = quadnode_search(qn->children[i],searchbox,maxdistance,maxcount,retlist);
            if(!ret)
            {
                return 0;
            }
        }
    }

    list = qn->featurelist;
    while(list)
    {
        feature_t f = (feature_t)list->data;
        envelop_t e = feature_get_extent(f);

        double distsq = envelop_distance_sq(searchbox,e);
        if(distsq <= maxdistsq && count > 0)
        {
            seq_addend(retlist,f);
            count--;
            if(count <= 0)
            {
                return 0;
            }
        }
        list = list->next;
    }

    return 1;
}

static void _split_space(T qn)
{
    struct alist *tmplist;
    struct alist *list;
    struct alist *next;

    assert(qn);
    qn->count = 0;
    qn->splited = 1;
    tmplist = NULL;
    list = qn->featurelist;

    while(list)
    {
        feature_t f = (feature_t)list->data;
        envelop_t e = feature_get_extent(f);

        next = list->next;

        int spaceindex = _space_index(qn,e);
        if(NODE_SELF == spaceindex)
        {
            list->next = tmplist;
            tmplist = list;
            qn->count += 1;
        }
        else
        {
            _insert_to_subspace(qn,spaceindex,list);
        }
        list = next;
    }
    qn->featurelist = tmplist;
}

static int _space_index(T qn,envelop_t box)
{
    int i;
    for(i = 1; i < 5; i++)
    {
        if(envelop_contains(qn->subspace[i],box))
        {
            return i;
        }
    }

    return NODE_SELF;
}
static void _insert(T qn,struct alist *al)
{
    if(!qn->splited && qn->count < THRESHOLD)
    {
        al->next = qn->featurelist;
        qn->featurelist = al;
        qn->count++;
    }
    else
    {
        if(!qn->splited)
        {
            _split_space(qn);
        }

        feature_t f = (feature_t)al->data;
        envelop_t e = feature_get_extent(f);
        int space_index = _space_index(qn,e);
        if(NODE_SELF == space_index)
        {
            al->next = qn->featurelist;
            qn->featurelist = al;
            qn->count++;
        }
        else
        {
            _insert_to_subspace(qn,space_index,al);
        }
    }
}

static void _insert_to_subspace(T qn,int spaceindex,struct alist *alist)
{
    T child;

    child = qn->children[spaceindex];
    if(!child)
    {
        child = quadnode_new_space(qn->subspace[spaceindex]);
        qn->children[spaceindex] = child;
    }
    _insert(child,alist);
}

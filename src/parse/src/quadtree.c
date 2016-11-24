
#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#include "sparsearray.h"
#include "quadtree.h"


#define T quadtree_t
struct T
{
    quadnode_t root;
    sparsearray_t featurelist;
};

T quadtree_new(double left,double top,double right,double bottom)
{
    T qt;
    NEW0(qt);

    qt->root = quadnode_new(left,top,right,bottom);
    qt->featurelist = sparsearray_new(1024);

    return qt;
}
T          quadtree_new_space(envelop_t extent)
{
    T qt;
    NEW0(qt);

    qt->root = quadnode_new_space(extent);
    qt->featurelist = sparsearray_new(1024);

    return qt;
}
void       quadtree_free(T qt)
{
    assert(qt);
    sparsearray_free(&(qt->featurelist));
}
feature_t  quadtree_get(T qt,int id)
{
    assert(qt);
    assert(id >= 0);

    return (feature_t)sparsearray_get(qt->featurelist,id);
}
int        quadtree_get_count(T qt)
{
    assert(qt);

    return sparsearray_length(qt->featurelist);
}
feature_t  quadtree_get_by_index(T qt,int index)
{
    assert(qt);
    assert(index >= 0 && index < sparsearray_length(qt->featurelist));

    return sparsearray_get_at(qt->featurelist,index);
}
void       quadtree_insert(T qt,feature_t f)
{
    assert(qt);
    assert(f);

    sparsearray_put(qt->featurelist,feature_get_id(f),f);
    quadnode_insert(qt->root,f);
}

int        quadtree_contains_point(T qt,double x,double y)
{
    assert(qt);
    return quadnode_contains_check_point(qt->root,x,y);
}
int        quadtree_intersect(T qt,double left,double top,double right,double bottom)
{
    int retval;

    envelop_t e;
    assert(qt);
    e = envelop_new(left,top,right,bottom);
    retval = quadtree_intersect_by_envelop(qt,e);

    envelop_free(&e);

    return retval;
}
int        quadtree_intersect_by_envelop(T qt,envelop_t e)
{
    assert(qt);
    assert(e);

    return quadnode_contains_intersect_check_box(qt->root,e);
}

seq_t      quadtree_search(T qt,double left,double top,double right,double bottom,double maxdistance,int maxcount,seq_t retlist)
{
    assert(qt);

    seq_t retval;
    envelop_t e;

    e = envelop_new(left,top,right,bottom);
    retval = quadtree_search_by_envelop(qt,e,maxdistance,maxcount,retlist);

    envelop_free(&e);

    return retval;
}
seq_t      quadtree_search_by_envelop(T qt,envelop_t searchbox,double maxdistance,int maxcount,seq_t retlist)
{
    assert(qt);
    assert(searchbox);

    if(NULL == retlist)
    {
        retlist = seq_new(32);
    }

    quadnode_search(qt->root,searchbox,maxdistance,maxcount,retlist);

    return retlist;
}

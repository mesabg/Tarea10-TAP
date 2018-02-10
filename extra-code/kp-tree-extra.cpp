//KDTNode template class declaration
KDTEMPLATE class KDNode
{
//members 
public:
    int axis ;        //split dimension
    Xtype x[SD];        //N-dimensional point 
    unsigned int id ;
    bool checked ;        //flag needed for recursive parent check

//member functions
public:
    KDNode(Xtype* x0, int split_axis);  //contructor

    KDNODE*    Insert(Xtype* x);
    KDNODE*    FindParent(Xtype* x0);

    KDNODE* Parent ;
    KDNODE* Left ;
    KDNODE* Right ;
};



KDTEMPLATE
KDNODE*    KDNODE::FindParent(Xtype* x0)
{
    KDNODE* parent ;
    KDNODE* next = this ;
    int split ;
    while(next)
    {
        split = next->axis  ;
        parent = next ;
        if(x0[split] > next->x[split])
            next = next->Right ;
        else
            next = next->Left ;
    }
    return parent ;
}


KDTEMPLATE
KDNODE*    KDNODE::Insert(Xtype* p)
{
    KDNODE* parent = FindParent(p);
    if(equal(p, parent->x, SD))
        return NULL ;

    KDNODE* newNode = new KDNODE(p, parent->axis +1 < SD? parent->axis+1:0);
    newNode->Parent = parent ;

    if(p[parent->axis] > parent->x[parent->axis])
    {
        parent->Right = newNode ;
        newNode->orientation = 1 ;
    }
    else
    {
        parent->Left = newNode ;
        newNode->orientation = 0 ;
    }

    return newNode ;
}



KDTEMPLATE
KDNODE* KDTREE::find_nearest(Xtype* x)
{
    if(!Root)
    return NULL ;

    checked_nodes = 0;

    KDNODE* parent = Root->FindParent(x);
    nearest_neighbour = parent ;
    d_min = distance2(x, parent->x, SD); ;

    if(equal(x, parent->x, SD))
        return nearest_neighbour ;

    search_parent(parent, x);
    uncheck();

    return nearest_neighbour ;
}


KDTEMPLATE
KDNODE* KDTREE::search_parent(KDNODE* parent, Xtype* x)
{
    for(int k=0; k<SD; k++)
    {
        x_min[k] = x_max[k] = 0;
        max_boundary[k] = min_boundary[k] = 0;
    }
    n_boundary = 0;

    Xtype dx;
    KDNODE* search_root = parent ;
    while(parent && n_boundary != 2*SD)
    {    
       check_subtree(parent, x);
       search_root = parent ;
       parent = parent->Parent ;
    }

    return search_root ;
}
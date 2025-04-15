#ifndef TREE_H
#define TREE_H

struct Node
{   
    int   type   ;
    long  data   ;
    Node* left   ;
    Node* right  ;
    Node* pointer;
    Node* parent ;
};

struct Tree
{
    int   size;
    Node* root;
};

enum Type
{
    KNOT      = 0,
    LEAF      = 1,
    VARIABLES = 2,
};

#endif /*TREE_H*/
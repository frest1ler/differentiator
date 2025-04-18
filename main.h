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

struct Info_about_text
{
    size_t max_number_line;
    size_t size_text      ;
    char*  text           ;
    char** ptr_line       ;
};

enum Type
{
    KNOT      = 0,
    LEAF      = 1,
    VARIABLES = 2,
};

#endif /*TREE_H*/
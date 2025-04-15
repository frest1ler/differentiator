#ifndef TREE_CREATE_H
#define TREE_CREATE_H

#include "main.h"

const int LEFT      =    0;
const int RIGHT     =    1;
const int POISON    = -333;

Node* get_pointer_node(                                                         );
Node* node_ctor       (long value, void* parent, int* type                      );
Node* go_left         (Tree* tree, Node* node, int* add_el, FILE * point_to_file); 
Node* go_back         (Node* node, Tree* tree                                   );

Tree* ctor_tree       (                                                         );

void  node_destroy    (Node* node                                               );
void  insert          (Tree* tree, int value                                    );
void  bypass          (Tree* tree, FILE * point_to_file                         );
void  bypass_destroy  (Tree* tree                                               );

void* get_pointer     (size_t element_count, size_t element_size                );

#endif /*TREE_CREATE_H*/
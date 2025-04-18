#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_create.h"
#include "dump.h"
#include "differentiator_function.h"

int checking_tree_size(Tree* tree);

Node* get_pointer_node() 
{
    Node* node = (Node*)calloc(1, sizeof(Node));

    if (node == NULL){
        printf("Memory allocation failed : node\n");
        return NULL;
    }

    return node;
}

Node* node_ctor(long value, void* parent, int* type)
{
    Node* node = get_pointer_node();

    node->data         = value        ;
    node->parent       = (Node*)parent;
    node->pointer      = node         ;
    node->type         = *type        ;
    node->parent->type = KNOT         ;

    //printf("\nnode_ctor\n");
    //printf("data=%s\nptr=%p\nparent=%p\n\n", value, node, parent);

    return node;
}

void node_destroy(Node* node) 
{
    if (node == NULL){
        printf("NULL pointer node\n");
        return;
    }

    node->parent->type = LEAF;

    if (node->parent->left == node){
        node->parent->left = NULL;
    }
    else{
        node->parent->right = NULL;
    }

    node->left =    NULL;
    node->right =   NULL;
    node->parent =  NULL;
    node->pointer = NULL;

    free(node);
}

Tree* ctor_tree()
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));

    if (tree == NULL){
        printf("Memory allocation failed: Tree\n");
        return NULL;
    }
    Node* node = get_pointer_node();

    node->pointer = node       ;
    node->parent  = (Node*)tree;
    tree->root    = node       ;
    node->type    = KNOT       ;

    return tree;
}


Node* go_back(Node* node, Tree* tree) 
{   
    Node* old_node = node;

    node = node->parent;

    while(node != tree->root)
    {
        if (node->right != NULL && node->right != old_node){
            //printf("1after_back node=%p\n", node->right);
            return node->right;
        }
        old_node = node        ;
        node     = node->parent;
    }
    //printf("2after_back node=%p\n", node);
    if(node == tree->root){
        node = node->right;
    }
    //printf("3after_back node=%p\n", node);

    return node;
}

Node* go_left(Tree* tree, Node* node, int* found_size, FILE * point_to_file) 
{
    if (node == NULL){
        return NULL; 
    }

    //printf("node = %p right = %p left = %p\n", node, node->right, node->left);  

    if (node->right == NULL && node->left == NULL && node == node->parent->right){
        //printf("case1\n");
        dump_node(node, point_to_file);
        //debug_print_node(node);
        (*found_size)++;
        //printf("found_size = %d\n", *found_size);
    }
    else if ((node != tree->root) &&
             (node == node->parent->right)){
        //printf("case2\n");
        if (*found_size == tree->size - 1){
            (*found_size)++;
            return node;
        }
        dump_node(node, point_to_file);
        //debug_print_node(node);
        (*found_size)++;
        //printf("found_size = %d\n", *found_size);
    }
    while ((node->right != NULL || node->left != NULL) && (*found_size <= tree->size - 1))
    {   
        //printf("case3\n");
        while (node->left != NULL && (*found_size <= tree->size - 1)){   
            //debug_print_node(node);
            node = node->left;

            dump_node(node, point_to_file); 
            (*found_size)++;
        }
        //printf("found_size = %d\n", *found_size);
        if (node->right != NULL && (*found_size <= tree->size - 1)){
            //debug_print_node(node);
            node = node->right;
            dump_node(node, point_to_file);
            (*found_size)++;
        }
        //printf("found_size = %d\n", *found_size);
    }
    return node;
}

Node* go_left_destroy(Node* node) 
{
    if (node == NULL){
        return NULL;
    }

    while (node->right != NULL || node->left != NULL)
    {   
        while (node->left != NULL){
            debug_print_node(node);
        }

        if (node->right != NULL){
            debug_print_node(node);
            node = node->right;
        }
    }
    return node;
}

void bypass_destroy(Tree* tree)
{
    if (tree == NULL || tree->root == NULL){
        return; // Проверка на NULL
    }

    Node* dest_node  = 0         ;  
    Node* node       = tree->root;

    while (tree->size > 1) 
    {
        node = go_left_destroy(node);

        dest_node = node;

        tree->size --;

        node = tree->root;

        node_destroy(dest_node);
    }
    if (tree != NULL)
    {
        if (tree->root != NULL){
            free(tree->root);
        }
        free(tree);
    }
}

void bypass(Tree* tree, FILE * point_to_file)
{
    if (tree == NULL || tree->root == NULL){
        return;
    }

    int found_size = 0;    
    Node* node = tree->root;

    if (checking_tree_size(tree)){
        return;
    }
    //printf("tree_size=%d", tree->size);
    while (found_size < tree->size) 
    {   
        //printf("found_size = %d", found_size);
        node = go_left(tree, node, &found_size, point_to_file);
        //printf("after_left\n");
        //debug_print_node(node);
        node = go_back(node, tree);
    }
    //printf("end_bypass\n");
}

void* get_pointer(size_t element_count, size_t element_size) 
{
    void* pointer = calloc(element_count, element_size);

    if (pointer == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for array.\n");
        exit(EXIT_FAILURE);
    }
    return pointer;
}

int checking_tree_size(Tree* tree)
{
    if (tree->root->left ==NULL && tree->root->right == NULL && tree->size == 1){
        return 1;
    }
    return 0;
}

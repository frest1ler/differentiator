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

Node* node_ctor(int value, void* parent)
{
    Node* node = get_pointer_node();

    node->data         = value        ;
    node->parent       = (Node*)parent;
    node->pointer      = node         ;
    node->type         = LEAF         ;
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

    if (node->parent->left ==  node){
        node->parent->left =  NULL;
    }
    else{
        node->parent->right = NULL;
    }

    node->left =     NULL;
    node->right =    NULL;
    node->parent =   NULL;
    node->pointer =  NULL;

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
    node->type    = LEAF       ;

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

Node* go_left(Node* node, int* add_el, FILE * point_to_file) 
{
    if (node == NULL){
        return NULL; 
    }

    //printf("node = %p right = %p left = %p\n", node, node->right, node->left);  

    if (node->right == NULL && node->left == NULL && node == node->parent->right){
        dump_node(node, point_to_file);
        //printf("hlt\n");
        //printf("node=%p\ndata=%s\nparent=%p\nptr=%p\nleft=%p\nright=%p\n", node, node->data, node->parent, node->pointer, node->left, node->right);
        (*add_el)++;
    }
    else if (node->left != NULL || node->right != NULL && node == node->parent->right){
        dump_node(node, point_to_file);
        //printf("hltx2\n");
        //printf("node=%p\ndata=%s\nparent=%p\nptr=%p\nleft=%p\nright=%p\n", node, node->data, node->parent, node->pointer, node->left, node->right);
        (*add_el)++;
    }
    while (node->right != NULL || node->left != NULL)
    {   
        while (node->left != NULL){   
            //printf("\ndata=%s\nptr=%p\nparent=%p\nleft=%p\nright=%p\n", node->data, node->pointer, node->parent, node->left, node->right);
            //printf("node->left->data=(%s)\n", node->left->data);
            node = node->left;

            dump_node(node, point_to_file); 
            (*add_el)++;
        }

        if (node->right != NULL){
            node = node->right;
            dump_node(node, point_to_file);
            (*add_el)++;
        }
    }
    return node;
}

Node* go_left_destroy(Node* node, Tree* tree) 
{
    if (node == NULL){
        return NULL;
    }

    while (node->right != NULL || node->left != NULL)
    {   
        while (node->left != NULL){
            printf("node=%p\ndata=%s\nparent=%p\nptr=%p\nleft=%p\nright=%p\n", node, node->data, node->parent, node->pointer, node->left, node->right);   
            node = node->left;
        }

        if (node->right != NULL){
            printf("node=%p\ndata=%s\nparent=%p\nptr=%p\nleft=%p\nright=%p\n", node, node->data, node->parent, node->pointer, node->left, node->right);
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
        node = go_left_destroy(node, tree);

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

    while (found_size < tree->size) 
    {   
        //printf("go_left, size = %d\n", found_size);

        node = go_left(node, &found_size, point_to_file);

        //printf("go_back, size = %d\n", found_size);
        //printf("exp=%d, have=%d\n", tree->size, found_size);
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

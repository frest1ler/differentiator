#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dump.h"
#include "tree_create.h"
#include "differentiator_function.h"

void dump_first_node(Tree* tree, FILE * point_to_file);
void dump_node_data (Node* node, FILE * point_to_file);

void dump(Tree* tree, const char* fname)
{   
    assert(tree      );
    assert(tree->root);

    FILE * point_to_file = fopen(fname, "w");

    if (!point_to_file){
        printf("The file does not open\n");

        exit(EXIT_FAILURE);
    }
    
    fprintf(point_to_file, "digraph G\n {rankdir=TB;\n root [label=\"Header tree %p\" shape=box];\n", tree);
    
    dump_first_node(tree, point_to_file);

    bypass(tree, point_to_file);  

    fprintf(point_to_file, "\n}\n}");

    fclose(point_to_file);
}

void dump_node(Node* node, FILE * point_to_file)
{   
    fprintf(point_to_file, "subgraph cluster_A_left {\nlabel=\"Левое облачко A1\";\n");
    fprintf(point_to_file, "style=dotted;\nnode [shape=record];\n");
    //debug_print_node(node);
    fprintf(point_to_file, "a%p [label=\"{Type %d | Parent %p | Ptr %p | Data ", node, node->type, node->parent, node);
    
    dump_node_data(node, point_to_file);

    fprintf(point_to_file, " | {Left %p | Right %p }}\"];\n", node->left, node->right);

    if ((node->parent)->left == node){
    fprintf(point_to_file, "a%p -> a%p [label=\"Left\" dir=forward];\n}\n", node->parent, node);
    }
    else{
    fprintf(point_to_file, "a%p -> a%p [label=\"Right\" dir=forward];\n}\n", node->parent, node);
    }
}

void dump_first_node(Tree* tree, FILE * point_to_file)
{   
    //printf("dump_first_node\n");
    if (tree == NULL || tree->root == NULL){
        printf("dump_first_node: tree == NULL || tree->root == NULL\n");
        return;
    }
    debug_print_node(tree->root);

    fprintf(point_to_file, "subgraph cluster_A {label=\"Облачко A\";style=dashed;node [shape=record];\n");
    fprintf(point_to_file, "a%p [label=\"{Type %d | Parent %p | Ptr %p | Data ",
            tree->root, tree->root->type, tree, tree->root);
    
    dump_node_data(tree->root, point_to_file);

    fprintf(point_to_file, " | {Left %p | Right %p }}\"];\n root -> a%p;", tree->root->left, tree->root->right, tree->root);
}

void dump_node_data(Node* node, FILE * point_to_file)
{
    if (node->type != LEAF)
    {   
        if (node->type == VARIABLES){
            fprintf(point_to_file, "%s", (char*)node->data);    
        }
        else if (node->data == ADD){
            fprintf(point_to_file, "add");
        }
        else if (node->data == SUB){
            fprintf(point_to_file, "sub");
        }
        else if (node->data == MUL){
            fprintf(point_to_file, "mul");
        }
        else if (node->data == DIV){
            fprintf(point_to_file, "div");
        }
    }
    else
    {
        fprintf(point_to_file, "%ld", node->data);    
    }
}
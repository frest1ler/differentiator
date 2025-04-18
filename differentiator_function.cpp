#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include "differentiator_function.h"
#include "tree_create.h"

void  perform_operation         (Node* node                              );
void  scanf_if_variable         (Node* node, long* value_l, long* value_r);
Node* go_left_decide            (Node* node                              ); 
long  transfer_argument         (char* ptr, int* type                    );
long  perform_math_operation    (Node* node, long value_l, long value_r  );
char  determining_last_index_row(Info_about_text* info, size_t* size     );
int   check_data                (char* ptr                               );
int   check_data_to_symbols     (char* ptr                               );

void info_dtor(Info_about_text* info)
{
    if (info == NULL){
        printf("info = NULL\n");
        
        return;
    }

    if (info->text != NULL){
        free(info->text);
    }
    if (info->ptr_line != NULL){
        free(info->ptr_line);
    }
    free(info);
}

int check_data(char* ptr)
{
    if (strcmp(ptr, "add") == 0){
        return ADD;
    }
    if (strcmp(ptr, "sub") == 0){
        return SUB;
    }
    if (strcmp(ptr, "mul") == 0){
        return MUL;
    }
    if (strcmp(ptr, "div") == 0){
        return DIV;
    }
    return 0;
}

int check_data_to_symbols(char* ptr)
{
    if (isdigit(ptr[0]) == 0){
        return 1;
    }
    return 0;
}

long transfer_argument(char* ptr, int* type)
{
    int check    = check_data(ptr);
    long argument = 0;

    if (check != 0){
        argument = (long)check;
    }
    else
    {
        if (check_data_to_symbols(ptr) == 1){
            *type = VARIABLES;
            argument = (long)ptr;

            printf("variables: %s\narg=%ld\n", ptr, argument);
        }
        else{
            printf("leaf: %s\n", ptr);
            *type = LEAF;

            argument = atoi(ptr);
        }
    }
    return argument;
}

char determining_last_index_row(Info_about_text* info, size_t* size)
{
    char symbol = 0;

    while (*size < info->size_text && info->text[*size] != '(' && info->text[*size] != ')' 
               && info->text[*size] != ';' && info->text[*size] != '\r' && info->text[*size] != '\0') 
    {
        (*size)++;
    }
    symbol = info->text[*size]; 

    return symbol;   
}

void insert_from_file(Info_about_text* info, Tree* tree) 
{   
    if (!info || !tree){
        printf("Invalid input: info or tree is NULL\n");
        return;
    }
    read_commands(info);

    Node*  parent              = tree->root;
    Node*  node                = NULL      ;
    size_t index_last_sring    = 0         ;
    long   argument            = 0         ;
    char   pr_symbol           = 0         ;
    char   symbol              = 0         ;
    int    type                = 0         ;

    for (size_t size = 0; size < info->size_text; size++) 
    {   
        symbol = determining_last_index_row(info, &size);  //printf("string: %s\n", info->text + index_last_sring);

        if (size < info->size_text && 
            (symbol == '(' && (pr_symbol == 0 || pr_symbol == '(')) || 
            (pr_symbol == '(' && symbol ==';') ||
            (pr_symbol == ')' && symbol == ';'))
        {   
            info->text[size] = '\0';
            //printf("case1\n");
            if (strcmp(info->text + index_last_sring, "\0") != 0)
            {   
                if (node == NULL)
                {
                    node = tree->root;
                    argument = transfer_argument(info->text + index_last_sring, &type);
                    node->data = argument;
                    //debug_print_node(node);
                }
                else
                {   
                    argument = transfer_argument(info->text + index_last_sring, &type);
                    node = node_ctor(argument, parent, &type);
                    //debug_print_node(node);
                    parent->left = node;   
                }
                if (symbol == '('){
                parent = node;
                }
                tree->size++;
            }
            else if (pr_symbol == ')' && symbol == ';'){
                parent = parent->parent;
            }
        } 
        else if (size < info->size_text && pr_symbol == ';' && (symbol == '(' || symbol == ')')) 
        {   
            info->text[size] = '\0';
            //printf("case2\n");
            //printf("(%s) = case 3\n", info->text + index_last_sring);

            if (strcmp(info->text + index_last_sring, "\0") != 0)
            {   
                argument = transfer_argument(info->text + index_last_sring, &type);
                node = node_ctor(argument, parent, &type);
                //debug_print_node(node);
                parent->right = node;
                tree->size++;

            }
            if (symbol == '('){
                parent = node;
            }
        }
        else if (size < info->size_text && symbol == ')' && pr_symbol == ')')
        {
            info->text[size] = '\0';
            //printf("case3\n");
            if (parent != tree->root){    
                parent = parent->parent;
            }
        }
        pr_symbol = symbol;
        index_last_sring = size + 1;
    }
}

int get_input()
{   
    int answer = -1;

    while((scanf("%d", &answer)) != 1 && (answer != 0 && answer != 1))
    {
        skip_buffer();
        printf("Input error. Try again\a\n");
    }
    skip_buffer();

    return answer;
}

void skip_buffer()
{
    int symbol = 0;

	do
	{
        symbol = getchar();
	} while (symbol != '\n' && symbol != EOF);
}

void decide(Tree* tree)
{
    if (tree == NULL || tree->root == NULL){
        return; 
    }
  
    Node* node   = tree->root;

    //debug_print_node(node);
    
    while (tree->size > 1) 
    {
        node = go_left_decide(node);

        perform_operation(node);

        if (node == tree->root){
            printf("result = %ld\n", node->data);
        }
        tree->size -=2;
        
        node = tree->root;
    }
}

Node* go_left_decide(Node* node) 
{
    if (node == NULL){
        return NULL; 
    }

    if (node->left->type == LEAF && node->right->type == KNOT){
        node = node->right;
    }

    while (node->left != NULL || node->right != NULL)
    {   
        while (node->left != NULL){   
            //debug_print_node(node);
    
            node = node->left;
            //debug_print_node(node);
        }

        if (node->right != NULL){
            node = node->right;
        }
    }
    //debug_print_node(node);
    //printf("end_left_decide\n");
    return node->parent;
}

void perform_operation(Node* node)
{   
    //printf("\n\nperform_operation\n");
    if (node == NULL){
        printf("node == NULL\n");
        return;
    }
    if (node->type != KNOT){
        printf("node->data->type != KNOT\n");
        return;
    }
    long value_l = 0; 
    long value_r = 0; 

    scanf_if_variable(node, &value_l, &value_r);

    long value = perform_math_operation(node, value_l, value_r);
    
    node->data = value;

    node_destroy(node->left );
    node_destroy(node->right);
}

void debug_print_node(Node* node)
{
    if (node == NULL){
        printf("node == NULL\n");
    }
    else
    {
        printf("\ntype=%d\ndata=%ld\nptr=%p\nparent=%p\nleft=%p\nright=%p\n", 
        node->type, node->data, node->pointer, node->parent, node->left, node->right);    
    }
}

void scanf_if_variable(Node* node, long* value_l, long* value_r)
{
    if (node->left->type == VARIABLES ){
        printf("enter value:\n");
        scanf("%ld", value_l);
    }
    else{
        *value_l = node->left->data;     
    }
    if (node->right->type == VARIABLES){
        printf("enter value:\n");
        scanf("%ld", value_r);
    }
    else{
        *value_r = node->right->data;     
    }
}

long perform_math_operation(Node* node, long value_l, long value_r)
{   
    long value = 0;

    if (node->data == ADD){
        value = value_l + value_r;
    }
    else if (node->data == SUB){
        value = value_l - value_r;
    }
    else if (node->data == MUL){
        value = value_l * value_r;
    }
    else if (node->data == DIV){
        value = value_l / value_r;
    }
    return value;
}
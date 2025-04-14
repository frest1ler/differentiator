#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "differentiator_function.h"
#include "tree_create.h"

static void search_new_line(Info_about_text* info);
static void count_number_lines(Info_about_text* info);
void        calculate_array_size(Info_about_text *info, const char* fname);

int         checking_variable(char* ptr);

void read_commands(Info_about_text* info)
{
    assert(info);

    const char* fname = "TREE_INITIAL_DATA.txt"; 

    FILE * point_to_file = fopen(fname, "rb");

    if (!point_to_file) 
    {
        printf("The file %s does not open\n", fname);

        exit(EXIT_FAILURE);
    }

    calculate_array_size(info, fname);

    int number_successfully_read_chars = fread(info->text, 1, info->size_text, point_to_file);

    if (number_successfully_read_chars + 1 != info->size_text)
    {
        printf("ERROR: fread didn't read enough chars\n"
               "info->size_text = %d\n"
               "number_successfully_read_chars = %d\n",
               info->size_text, number_successfully_read_chars);

        exit(EXIT_FAILURE);
    }
    fclose(point_to_file);
    printf("%s\n", info->text);
    info->text[info->size_text - 1] = '\0';

    return;
}

void init_pointer_array(Info_about_text* info)
{
    assert(info);

    count_number_lines(info);

    search_new_line(info);
}

void calculate_array_size(Info_about_text *info, const char* fname)
{
    assert(info);
    assert(fname);

    struct stat data_for_file = {};

    stat(fname, &data_for_file);

    info->size_text = data_for_file.st_size + 1;

    if (!info->size_text)
    {
        printf("size_text = 0\n");

        exit(EXIT_FAILURE);
    }
    info->text = (char*)calloc(info->size_text, sizeof(char));

    if (info->text == NULL)
    {
        printf("ERROR: calculate_array_size; text = NULL\n");

        exit(EXIT_FAILURE);
    }
}

static void search_new_line(Info_about_text* info)
{
    if (!info)
    {
        printf("structure transfer error\n");

        exit(EXIT_FAILURE);
    }
    assert(info->text);

    info->ptr_line = (char**)calloc(info->max_number_line, sizeof(char*));

    char symbol         = 0;
    size_t line_element = 0;
    size_t number_line  = 1;

    info->ptr_line[0] = info->text;
    
    for(int i = 0; i < info->size_text - 1; i++)
    {

        if (info->text[i] == '\0')
        {
            info->ptr_line[number_line] = (info->text + i + 1);

            number_line++;
        }
    }
}

static void count_number_lines(Info_about_text* info)
{
    assert(info);

    info->max_number_line = 1;

    char symbol = 0;

    for(int line_element = 0; (symbol = info->text[line_element]) &&
        line_element < info->size_text; line_element++)
    {
        if (symbol == '\n')
        {
            info->max_number_line++;
            info->text[line_element] = '\0';
        }
    }
}

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

int transfer_argument(char* ptr, Node* node)
{
    int check    = check_data(ptr);
    int argument = 0;
    int type     = 0;

    if (check != 0){
        argument = check;

    }
    else
    {
        argument = atoi(ptr);

        if (checking_variable(ptr) == 1){
            node->type = VARIABLES;
        }
        else{
            node->type = LEAF;
        }
    }
    return argument;
}

int checking_variable(char* ptr)
{
    int i = 0;

    while(ptr[i] != '\0'){
        i++;
    }
    if (ptr[i - 1] == ':'){
        return 1;
    }
    return 0;
}

void insert_from_file(Info_about_text* info, Tree* tree) 
{   
    if (!info || !tree){
        printf("Invalid input: info or tree is NULL\n");
        return;
    }
    read_commands(info);

    Node* parent              = tree->root;
    Node* node                = NULL      ;
    char  pr_symbol           = 0         ;
    char  symbol              = 0         ;
    int   index_last_sring    = 0         ;
    int   argument            = 0         ;

    for (int size = 0; size < info->size_text; size++) 
    {
        while (size < info->size_text && info->text[size] != '(' && info->text[size] != ')' 
               && info->text[size] != ';' && info->text[size] != '\r' && info->text[size] != '\0') 
        {
            size++;
        }
        symbol = info->text[size];  

        if (size < info->size_text && 
            (symbol == '(' && (pr_symbol == 0 || pr_symbol == '(')) || 
            (pr_symbol == '(' && symbol ==';') ||
            (pr_symbol == ')' && symbol == ';'))
        {   
            info->text[size] = '\0';

            if (strcmp(info->text + index_last_sring, "\0") != 0)
            {
                //printf("%s = case 1\n", info->text + index_last_sring);
                if (node == NULL)
                {
                    node = tree->root;
                    argument = transfer_argument(info->text + index_last_sring, node);
                    node->data = argument;
                    //debug_print_node(node);
                }
                else
                {   
                    argument = transfer_argument(info->text + index_last_sring, node);
                    node = node_ctor(argument, parent);
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

            //printf("(%s) = case 3\n", info->text + index_last_sring);

            if (strcmp(info->text + index_last_sring, "\0") != 0)
            {   
                argument = transfer_argument(info->text + index_last_sring, node);

                node = node_ctor(argument, parent);
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

    Node* parent = NULL      ;  
    Node* node   = tree->root;

    debug_print_node(node);
    
    while (tree->size > 1) 
    {
        node = go_left_decide(node);

        perform_operation(node);

        if (node == tree->root){
            printf("result = %d\n", node->data);
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
            debug_print_node(node);
    
            node = node->left;
            debug_print_node(node);
        }

        if (node->right != NULL){
            node = node->right;
        }
    }
    debug_print_node(node);
    printf("end_left_decide\n");
    return node->parent;
}

void perform_operation(Node* node)
{   
    printf("\n\nperform_operation\n");
    if (node == NULL){
        printf("node == NULL\n");
        return;
    }
    if (node->type != KNOT){
        printf("node->data->type != KNOT\n");
        return;
    }
    int value_l = node->left->data ; 
    int value_r = node->right->data; 
    int value   = 0                ;

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
    printf("value=%d\n", value);
    node->data = value;

    node_destroy(node->left );
    node_destroy(node->right);
}

void debug_print_node(Node* node)
{
    assert(node);

    printf("\ntype=%d\ndata=%d\nptr=%p\nparent=%p\nleft=%p\nright=%p\n", 
    node->type, node->data, node->pointer, node->parent, node->left, node->right);
}
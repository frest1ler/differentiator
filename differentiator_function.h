#ifndef AKINATOR_FUNCTION_H
#define AKINATOR_FUNCTION_H

const int COMMAND_LENGTH = 50;
const int MAX_LEVEL_TREE = 10;

#include "main.h"
#include "read_from_file.h"

void  decide                (Tree* tree                              );
void  info_dtor             (Info_about_text* info                   );
void  insert_from_file      (Info_about_text* info, Tree* tree       );
void  perform_operation     (Node* node                              );
void  debug_print_node      (Node* node                              );
void  scanf_if_variable     (Node* node, long* value_l, long* value_r);
void  skip_buffer           (                                        );

Node* go_left_decide        (Node* node                              ); 

int   get_input             (                                        );
int   check_data            (char* ptr                               );
int   check_data_to_symbols (char* ptr                               );

long  transfer_argument     (char* ptr, int* type                    );
long  perform_math_operation(Node* node, long value_l, long value_r  );

enum Action
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
};

#endif /*AKINATOR_FUNCTION_H*/
#ifndef AKINATOR_FUNCTION_H
#define AKINATOR_FUNCTION_H

const int COMMAND_LENGTH = 50;
const int MAX_LEVEL_TREE = 10;

#include "main.h"
#include "read_from_file.h"

void  decide          (Tree* tree                       );
void  info_dtor       (Info_about_text* info            );
void  insert_from_file(Info_about_text* info, Tree* tree);
void  debug_print_node(Node* node                       );
void  skip_buffer     (                                 );

int   get_input       (                                 );

enum Action
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
};

#endif /*AKINATOR_FUNCTION_H*/
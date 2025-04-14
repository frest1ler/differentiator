#include <stdio.h>
#include "main.h"
#include "dump.h"
#include "tree_create.h"
#include "differentiator_function.h"

int main()
{
    Tree*            tree = ctor_tree();
    Info_about_text* info = (Info_about_text*)get_pointer(1, sizeof(Info_about_text));

    printf("f_dump\n");

    dump(tree, "data/bata.dot");

    insert_from_file(info, tree);
    
    printf("s_dump\n");

    dump(tree, "data/bata2.dot");

    printf("decide\n");
    decide(tree);

    dump(tree, "data/bata3.dot");
    
    //dump(tree, "data/bata4.dot");

    //dump(tree, "data/bata3.dot");

    printf("destroy\n");

    bypass_destroy(tree);

    info_dtor(info);
    
    return 0;
}

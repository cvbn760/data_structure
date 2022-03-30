#include <stdio.h>
#include "tree.h"

int main() {
    Tree *tree = new_tree(DECIMAL_ELEM, STRING_ELEM);

    set_tree(tree, decimal(50), string("A"));
    set_tree(tree, decimal(40), string("B"));

//    del_tree(tree, decimal(40));

    int64_t x = 15;
    if (in_tree(tree, decimal(x))) {
        printf("%lf\n", get_tree(tree, decimal(x)).real);
    }

    print_tree_as_list(tree);
    free_tree(tree);
    return 0;
}

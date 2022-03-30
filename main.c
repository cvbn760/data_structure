#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "tree.h"
#include "tree.c"
//#include "hash_bable.h"
#include <stdio.h>
#include <stdlib.h>

//
//#include "number571/tree.h"

int main(void) {
//    HashTab *hashtab = new_hashtab(10000, STRING_ELEM, STRING_ELEM);
//
//    set_hashtab(hashtab, string("A"), string("dg"));
//    set_hashtab(hashtab, string("B"), string("gfdfdsg"));
//    set_hashtab(hashtab, string("C"), string("gdsg"));
//    set_hashtab(hashtab, string("D"), string("tr9ieu"));
//
//    print_hashtab(hashtab);
//    free_hashtab(hashtab);

    Tree *tree = new_tree(STRING_ELEM, STRING_ELEM);

    char *kOne = "keyOne";
    char *kTwo = "keyTwo";

    char *vOne = "valOne";
    char *vTwo = "valTwo";

    set_tree(tree, kOne, vOne);
    set_tree(tree, kTwo, vTwo);

//    del_tree(tree, string(40));

    if (in_tree(tree, kTwo)) {
        printf("%s\n", get_tree(tree, kTwo));
    }

    print_tree_as_list(tree);
    free_tree(tree);
    return 0;
}

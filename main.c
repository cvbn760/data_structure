#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "data_structure/tree.h"
#include "data_structure/tree.c"
#include "data_structure/hash_table.h"
#include "data_structure/hash_table.c"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    Tree *tree = newTree(STRING_ELEM, STRING_ELEM);

    char *a = "key1";
    char *b = "key2";
    char *c = "key3";
    char *d = "key4";

    char *e = "val1";
    char *f = "val2";
    char *g = "val3";
    char *h = "val4";

    addElementTree(tree, a, e);
    addElementTree(tree, b, f);
    addElementTree(tree, c, g);
    addElementTree(tree, d, h);

    deleteByKeyTree(tree, c);

//    if (containsElementTree(tree, d)) {
//        log_i("%lf\n", getElementTree(tree, d));
//    }

    printTree(tree);
    freeTree(tree);

    printf("\nnext hash table\n");

    HashTab *hashtab = newHT(200, STRING_ELEM, STRING_ELEM);

    addElementHT(hashtab, a, e);
    addElementHT(hashtab, b, f);
    addElementHT(hashtab, c, g);
    addElementHT(hashtab, d, h);

    printHT(hashtab);

    printf("%d", containsElementHT(hashtab, a));
    freeHT(hashtab);
    return 0;
}

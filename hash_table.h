//
// Created by klokov on 30.03.2022.
//

#ifndef TELITERA_HASH_TABLE_H
#define TELITERA_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

typedef struct HashTab {
    struct {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    size_t size;
    Tree **table;
} HashTab;

extern HashTab *newHT(size_t size, vtype_tree_t key, vtype_tree_t value);

extern void freeHT(HashTab *hashtab);

extern value_tree_t getElementHT(HashTab *hashtab, char *key);

extern void addElementHT(HashTab *hashtab, char *key, char *value);

extern void deleteByKeyHT(HashTab *hashtab, char *key);

extern int containsElementHT(HashTab *hashtab, char *key);

extern void printHT(HashTab *hashtab);

#endif //TELITERA_HASH_TABLE_H

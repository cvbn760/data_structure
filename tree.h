//
// Created by klokov on 29.03.2022.
//

#ifndef FLASHCONST_TREE_H
#define FLASHCONST_TREE_H
#pragma once

#include <stdint.h>
//#include "m2mb/m2mb_types.h"

typedef enum {
    DECIMAL_ELEM,
    REAL_ELEM,
    STRING_ELEM,
} vtype_tree_t;

typedef union {
//    INT64 decimal;
//    FLOAT64 real;
    char *string;
} value_tree_t;

typedef struct tree_node {
    struct {
        value_tree_t key;
        value_tree_t value;
    } data;
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *parent;
} tree_node;

typedef struct Tree {
    struct {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    struct tree_node *node;
} Tree;

extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value);
extern void free_tree(Tree *tree);

extern value_tree_t get_tree(Tree *tree, char *key);
extern void set_tree(Tree *tree, char *key, char *value);
extern void del_tree(Tree *tree, char *key);
extern _Bool in_tree(Tree *tree, char *key);

//extern void *decimal(INT64 x);
extern void *string(char *x);
//extern void *real(FLOAT64 x);

extern void print_tree(Tree *tree);
extern void print_tree_as_list(Tree *tree);
#endif //FLASHCONST_TREE_H

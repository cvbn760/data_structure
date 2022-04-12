//
// Created by klokov on 29.03.2022.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "../instruments/char_service.h"
#include "../instruments/char_service.c"

static tree_node *_new_node(char *key, char *value);

static void addForNode(Tree *tree,tree_node *node, char *key, char *value);

static void setKey(tree_node *node, char *key);

static void setValue(tree_node *node, char *value);

static void deleteNode(tree_node *node);

static void printNodeAsList(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);

static void printNode(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);

static void printNodeContnt(tree_node *node);

static tree_node *getNodeIfExist(tree_node *node, vtype_tree_t tkey, char *key);

static tree_node *_del1_tree(Tree *tree, vtype_tree_t tkey, char *key);

static void _del2_tree(Tree *tree, tree_node *node);

static void _del3_tree(tree_node *node);

static char *toStringNode(tree_node *node);
/**
 * Конструктор экземпляра структуры "Tree"
 * @param key типы хранимых ключей
 * @param value типы хранимых значений
 * @return
 */
extern Tree *newTree(vtype_tree_t key, vtype_tree_t value) {
    // Проверка типа ключа
    if (key != STRING_ELEM) {
        printf("%s\n", "key type not supported");
        return NULL;
    }
    // Провекта типа значения
    if (value != STRING_ELEM) {
        printf("%s\n", "value type not supported");
        return NULL;
    }
    // Выделение памяти
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    tree->type.key = key;
    tree->type.value = value;
    tree->root = NULL;
    tree->numberNodes = 0;
    return tree;
}

/**
 * Освободить память. Удалить дерево
 * @param tree дерево
 */
extern void freeTree(Tree *tree) {
    deleteNode(tree->root);
    free(tree);
}

/**
 * Содержит ли дерево элемент данным ключем
 * @param tree дерево
 * @param key ключ
 * @return (1 - true / 0 - false)
 */
extern int containsElementTree(Tree *tree, char *key) {
    return getNodeIfExist(tree->root, tree->type.key, key) != NULL;
}

/**
 * Достать из элемент по его ключу
 * @param tree адрес дерева
 * @param key адрес ключа
 * @return значение элемента
 */
extern value_tree_t getElementTree(Tree *tree, char *key) {
    tree_node *node = getNodeIfExist(tree->root, tree->type.key, key);
    if (node == NULL) {
        printf("value undefined\n");
        value_tree_t none;
        return none;
    }
    return node->data.value;
}

/**
 * Добавить в дерево новый узел
 * @param tree адрес дерева
 * @param key адрес ключа узла
 * @param value адрес значения узла
 */
extern void addElementTree(Tree *tree, char *key, char *value) {
    if (key == NULL || value == NULL){
        return;
    }
    if (tree->root == NULL) {
        tree->root = _new_node(key, value);
        tree->numberNodes++;
        return;
    }
    addForNode(tree,tree->root, key, value);
}

/**
 * Удалить из дерева элемент по его ключу
 * @param tree адрес дерева
 * @param key адрес ключа
 */
extern void deleteByKeyTree(Tree *tree, char *key) {
    tree_node *node = _del1_tree(tree, tree->type.key, key);
    if (node == NULL) {
        return;
    }
    if (node->left != NULL && node->right != NULL) {
        _del3_tree(node);
        tree->numberNodes--;
        return;
    }
    _del2_tree(tree, node);
    return;
}

/**
 * Распе
 * @param tree
 */
extern void printTreeAsList(Tree *tree) {
    printNodeAsList(tree->root, tree->type.key, tree->type.value);
}

extern void printTree(Tree *tree) {
    printNode(tree->root, tree->type.key, tree->type.value);
}


static tree_node *_new_node(char *key, char *value) {
    tree_node *node = (tree_node *) malloc(sizeof(tree_node));
    setKey(node, key);
    setValue(node, value);
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// Установить ключ для ноды
static void setKey(tree_node *node, char *key) {
    node->data.key.string = key;
}

// Установить значение для ноды
static void setValue(tree_node *node, char *value) {
    node->data.value.string = value;
}

// Добавить потомка для ноды
static void addForNode(Tree *tree,tree_node *node, char *key, char *value) {
    int cond = 0;
    cond = strcmp(key, node->data.key.string);
    if (cond > 0) {
        if (node->right == NULL) {
            node->right = _new_node(key, value);
            node->right->parent = node;
            tree->numberNodes++;
        } else {
            addForNode(tree,node->right, key, value);
        }
    } else if (cond < 0) {
        if (node->left == NULL) {
            node->left = _new_node(key, value);
            node->left->parent = node;
            tree->numberNodes++;
        } else {
            addForNode(tree,node->left, key, value);
        }
    } else {
        setValue(node, value);
    }
}

static tree_node *getNodeIfExist(tree_node *node, vtype_tree_t tkey, char *key) {
    int cond = 0;
    if (node == NULL) {
        return NULL;
    }
    cond = strcmp(key, node->data.key.string);
    if (cond > 0) {
        return getNodeIfExist(node->right, tkey, key);
    } else if (cond < 0) {
        return getNodeIfExist(node->left, tkey, key);
    }
    return node;
}

static tree_node *_del1_tree(Tree *tree, vtype_tree_t tkey, char *key) {
    tree_node *node = tree->root;
    node = getNodeIfExist(node, tkey, key);
    if (node == NULL) {
        return NULL;
    }
    if (node->left != NULL || node->right != NULL) {
        return node;
    }
    tree_node *parent = node->parent;
    if (parent == NULL) {
        tree->root = NULL;
    } else if (parent->left == node) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    free(node);
    tree->numberNodes--;
    return NULL;
}

static void _del2_tree(Tree *tree, tree_node *node) {
    tree_node *parent = node->parent;
    tree_node *temp;
    if (node->right != NULL) {
        temp = node->right;
    } else {
        temp = node->left;
    }
    if (parent == NULL) {
        tree->root = temp;
    } else if (parent->left == node) {
        parent->left = temp;
    } else {
        parent->right = temp;
    }
    temp->parent = parent;
    free(node);
    tree->numberNodes--;
}

static void _del3_tree(tree_node *node) {
    tree_node *ptr = node->right;
    while (ptr->left != NULL) {
        ptr = ptr->left;
    }
    node->data.key = ptr->data.key;
    node->data.value = ptr->data.value;
    tree_node *parent = ptr->parent;
    if (parent->left == ptr) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    free(ptr);
}

static void printNodeContnt(tree_node *node) {
    printf("[%s => %s]", node->data.key.string, node->data.value.string);
}

extern char *toString(tree_node *node){
    if (node == NULL){
        return multConc(1,"");
    }
    char *leftNode = toString(node->left);
    char *currNode = toStringNode(node);
    char *rightNode = toString(node->right);
    char *res = multConc(3, leftNode, rightNode, currNode);
    free(leftNode);
    free(currNode);
    free(rightNode);
    return res;
}

static char *toStringNode(tree_node *node){
    if (node == NULL){
        return multConc(1,"");
    }
    return multConc(4, node->data.key.string, "=",node->data.value.string, "\r\n");
}

static void printNode(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    if (node == NULL) {
        printf("null");
        return;
    }
    printf("(");
    printNode(node->left, tkey, tvalue);
    printNodeContnt(node);
    printNode(node->right, tkey, tvalue);
    printf(")");
}

static void printNodeAsList(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    if (node == NULL) {
        return;
    }
    printNodeAsList(node->left, tkey, tvalue);
    printNodeContnt(node);
    printNodeAsList(node->right, tkey, tvalue);
}

/**
 * Удалить узел дерева
 * @param node адрес узла
 */
static void deleteNode(tree_node *node) {
    if (node == NULL) {
        return;
    }
    deleteNode(node->left);
    deleteNode(node->right);
    free(node);
}
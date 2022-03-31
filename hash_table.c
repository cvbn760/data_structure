//
// Created by klokov on 30.03.2022.
//

#include "hash_table.h"

static uint32_t hashCode(char *s, size_t size);

/**
 * Конструктор для новой таблицы
 * @param size размер таблицы
 * @param key тип хранимых ключей
 * @param value тип хранимых значений
 * @return ссылку на экземпляр структуры HashTab
 */
extern HashTab *newHT(size_t size, vtype_tree_t key, vtype_tree_t value) {
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
    // Выделение памяти для таблицы
    HashTab *hashtab = (HashTab *) malloc(sizeof(HashTab));
    hashtab->table = (Tree **) malloc(size * sizeof(Tree));
    for (size_t i = 0; i < size; ++i) {
        hashtab->table[i] = newTree(key, value);
    }
    hashtab->size = size;
    hashtab->type.key = key;
    hashtab->type.value = value;
    return hashtab;
}

/**
 * Удаляем из таблицы элемент по его ключу
 * @param hashtab адрес таблицы
 * @param key адрес ключа
 */
extern void deleteByKeyHT(HashTab *hashtab, char *key) {
    uint32_t hash;
    switch (hashtab->type.key) {
        case STRING_ELEM:
            hash = hashCode(key, hashtab->size);
            break;
    }
    deleteByKeyTree(hashtab->table[hash], key);
}

/**
 * Содержит ли таблица элемент c требуемым ключем
 * @param hashtab адрес таблицы
 * @param key адрес ключа
 * @return (1 - true / 0 - false)
 */
extern int containsElementHT(HashTab *hashtab, char *key) {
    uint32_t hash;
    int result;
    switch (hashtab->type.key) {
        case STRING_ELEM:
            hash = hashCode(key, hashtab->size);
            break;
    }
    result = containsElementTree(hashtab->table[hash], key);
    return result;
}

/**
 * Достать из таблицы элемент по его ключу
 * @param hashtab адрес таблицы
 * @param key адрес ключа
 * @return значение элемента
 */
extern value_tree_t getElementHT(HashTab *hashtab, char *key) {
    uint32_t hash;
    value_tree_t result;
    switch (hashtab->type.key) {
        case STRING_ELEM:
            hash = hashCode(key, hashtab->size);
            break;
    }
    result = getElementTree(hashtab->table[hash], key);
    return result;
}

/**
 * Добавить элемент в таблицу.
 * @param hashtab хештаблица
 * @param key ключ элемента
 * @param value значение элемента
 */
extern void addElementHT(HashTab *hashtab, char *key, char *value) {
    uint32_t hash;
    switch (hashtab->type.key) {
        case STRING_ELEM:
            hash = hashCode(key, hashtab->size);
            break;
    }
    addElementTree(hashtab->table[hash], key, value);
}

/**
 * Освободить память выделенную под таблицу. (Удалить таблицу)
 * @param hashtab хештаблица
 */
extern void freeHT(HashTab *hashtab) {
    for (size_t i = 0; i < hashtab->size; ++i) {
        freeTree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

/**
 * Вывести содержимое таблицы в консоль
 * @param hashtab хештаблица
 */
extern void printHT(HashTab *hashtab) {
    for (size_t i = 0; i < hashtab->size; ++i) {
        if (hashtab->table[i]->root == NULL) {
            continue;
        }
        printTreeAsList(hashtab->table[i]);
    }
}

/**
 * Приватный метод. Считает хешкод для переданной строки
 * @param s адрес строки
 * @param size размер таблицы
 * @return хешкод
 */
static uint32_t hashCode(char *s, size_t size) {
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % size;
}
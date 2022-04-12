//
// Created by klokov on 05.04.2022.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include "char_service.h"

/**
 * Склеивает две строки. В куче выделяет память под новую строку.
 * Удалять потом из памяти нужно самому.
 * @param str1 адрес первой строки
 * @param str2 адрес второй строки
 * @return адрес результирующей строки
 */
extern char* strconc(char *str1, char* str2){
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2 + 1);
    return result;
}

/**
 * Множественная конкатенация строк.
 * Чистить память после результирующей строки нужно самому.
 * @param count количество строк.
 * @param ... адреса строк.
 * @return адрес результирующей строки.
 */
extern char *multConc(int count,...){
    va_list args;
    va_start(args, count);
    char *currentStr;
    char *prevRes = NULL;
    char *currRes = NULL;
    for(int i=count; i--; ){
        currentStr = va_arg(args, char *);
        if(currRes == NULL){
            currRes = strconc("", currentStr);
            prevRes = strconc("", currRes);
        } else{
            if (i == (count - 2)){
                free(currRes);
            }
            currRes = strconc(prevRes, currentStr);
            free(prevRes);                          // Освобождаем prevData
            prevRes = malloc(strlen(currRes) + 1); // Переписываем prevData
            strcpy(prevRes, currRes);
            if(i != 0){
                free(currRes);
            }
        }
    }
    free(prevRes);
    va_end(args);
    return currRes;
}

/**
 * Убирает из строки нечитаемые символы
 * @param s адрес строки
 */
extern void trim(char *s) {
    int i = 0, j;
    while ((s[i] == ' ') || (s[i] == '\t') || (s[i] == '\n') || (s[i] == '\r')) {
        i++;
    }
    if (i > 0) {
        for (j = 0; j < strlen(s); j++) {
            s[j] = s[j + i];
        }
        s[j] = '\0';
    }

    i = strlen(s) - 1;
    while ((s[i] == ' ') || (s[i] == '\t') || (s[i] == '\n') || (s[i] == '\r')) {
        i--;
    }
    if (i < (strlen(s) - 1)) {
        s[i + 1] = '\0';
    }
}

// Проверка пустой строки
extern int isEmpty(char *str){
    if(str == NULL || strlen(str) < 1){
        return 1;
    }
    return 0;
}
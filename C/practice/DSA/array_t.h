#ifndef ARRAY_T_H // Include guard start
#define ARRAY_T_H

#include <stdio.h>
#include <stdlib.h>

/* 
 * Type definition for the dynamic array. 
 */
 typedef struct {
    int *data;
    int size;
    int capacity;
 } array_t;

/*
 * Free the dynamic array memory
 */
static inline void array_t_free(array_t *arr) {
    free(arr->data);
    free(arr);
}

/*
 * Expand array capacity
 */
static inline int array_t_expand(array_t *arr) {
    arr->capacity *= 2;
    int *temp = realloc(arr->data, sizeof(int) * arr->capacity);
    if (!temp) {
        return 1;
    }
    arr->data = temp;
    return 0;
}

/*
 * Reduce array capacity
 */
static inline void array_t_reduce(array_t *arr) {
    if (arr->size <= arr->capacity / 4) {
        arr->capacity = arr->capacity / 2;
        arr->data = realloc(arr->data, sizeof(int) * arr->capacity);
    }
}

/*
 * Initialize the dynamic array
 */
array_t *array_t_init();

/*
 * Insert value at index
 */
int array_t_add(array_t *arr, int index, int value);

/*
 * Delete value at index
 */
int array_t_delete(array_t *arr, int index);

/*
 * Print the array
 */
void array_t_print(const array_t *arr);

#endif

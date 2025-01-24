#ifndef ARRAY_T_H // Include guard start
#define ARRAY_T_H

/*
* Error codes macros 
*/
#define ARRAY_T_OK 0
#define ARRAY_T_ERR_INDEX 1
#define ARRAY_T_ERR_MEM 2

#include <stdio.h>
#include <stdlib.h>

/* 
 * Type definition for the dynamic array. 
 */
 typedef struct {
    /*
     * A block of bytes.
     */ 
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
 } array_t;

/*
 * Free the dynamic array memory
 */
static inline int array_t_free(array_t *arr) {
    if (!arr) return ARRAY_T_ERR_MEM;
    free(arr->data);
    free(arr);
    return ARRAY_T_OK;
}

/*
 * Expand array capacity
 */
static inline int array_t_expand(array_t *arr) {
    if (!arr) return ARRAY_T_ERR_MEM;
    arr->capacity *= 2;
    int *temp = realloc(arr->data, sizeof(int) * arr->capacity);
    if (!temp) {
        return ARRAY_T_ERR_MEM;
    }
    arr->data = temp;
    return ARRAY_T_OK;
}

/*
 * Reduce array capacity
 */
static inline int array_t_reduce(array_t *arr) {
    if (!arr) return ARRAY_T_ERR_MEM;
    if (arr->size <= arr->capacity / 4) {
        arr->capacity = arr->capacity / 2;
        int *temp = realloc(arr->data, sizeof(int) * arr->capacity);
        if (!temp) {
            return ARRAY_T_ERR_MEM;
        }
        arr->data = temp;
    }
    return ARRAY_T_OK;
}

/*
 * Initialize the dynamic array
 */
array_t *array_t_init(size_t elem_size, size_t initial_capacity);

/*
 * Insert value at index
 */
int array_t_add(array_t *arr, size_t index, const void *elem);

/*
 * Push a value at the end of the array
 */
int array_t_push(array_t *arr, int value);

/*
 * Get value at index
 */
int array_t_get(const array_t *arr, int index, int *outValue);

/*
 * Delete the last value of the array
 */
int array_t_pop(array_t *arr);

/*
 * Delete value at index
 */
int array_t_delete(array_t *arr, int index);

/*
 * Print the array
 */
int array_t_print(const array_t *arr);

#endif

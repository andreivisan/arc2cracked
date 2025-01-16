#include "array_t.h"

array_t *array_t_init() {
    array_t *arr = malloc(sizeof(array_t));
    if (!arr) return NULL;
    arr->capacity = 8;
    arr->data = malloc(sizeof(int) * arr->capacity);
    if (!arr->data) {
        array_t_free(arr);
        return NULL;
    }
    arr->size = 0;
    return arr;
}

int array_t_add(array_t *arr, int index, int val) {
    if (!arr) return ARRAY_T_ERR_MEM;
    if (index < 0 || index > arr->size) return ARRAY_T_ERR_INDEX;
    if (arr->size == arr->capacity) {
        int ret = array_t_expand(arr);
        if (ret != ARRAY_T_OK) return ret;
    }
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i-1];
    }
    arr->data[index] = val;
    arr->size += 1;
    return ARRAY_T_OK;
}

int array_t_push(array_t *arr, int value) {
    if (!arr) return ARRAY_T_ERR_MEM;
    if (arr->size == arr->capacity) {
        int ret = array_t_expand(arr);
        if (ret != ARRAY_T_OK) return ret;
    }
    arr->data[arr->size] = value;
    arr->size++;
    return ARRAY_T_OK;
}

int array_t_get(const array_t *arr, int index, int *outValue) {
    if (!arr || !outValue) return ARRAY_T_ERR_MEM;
    if (index < 0 || index >= arr->size) return ARRAY_T_ERR_INDEX;
    *outValue = arr->data[index];
    return ARRAY_T_OK;
}

int array_t_pop(array_t *arr) {
   if (!arr) return ARRAY_T_ERR_MEM;
   if (arr->size == 0) return ARRAY_T_ERR_INDEX;
   arr->size--;
   return ARRAY_T_OK;
}

int array_t_delete(array_t *arr, int index) {
    if (!arr) return ARRAY_T_ERR_MEM;
    if (index < 0 || index >= arr->size) return ARRAY_T_ERR_INDEX;
    for (int i = index; i < arr-> size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    } 
    arr->size -= 1;
    int ret = array_t_reduce(arr);
    if (ret != ARRAY_T_OK) return ret;
    return ARRAY_T_OK;
}

int array_t_print(const array_t *arr) {
    if (!arr) return ARRAY_T_ERR_MEM;
    for (int i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return ARRAY_T_OK;
}


#include "array_t.h"

int main() {
    array_t *arr = array_t_init();
    array_t_add(arr, 0, 10);
    array_t_add(arr, 1, 7);
    array_t_add(arr, 1, 11);
    array_t_add(arr, 2, 16);
    array_t_print(arr);
    printf("array size: %d\n", arr->size);
    array_t_delete(arr, 2);
    printf("array size: %d\n", arr->size);
    array_t_print(arr);
    array_t_free(arr);
    return 0;
}
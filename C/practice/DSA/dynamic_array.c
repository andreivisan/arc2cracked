#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;

DynamicArray *init() {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    arr->capacity = 8;
    arr->data = malloc(sizeof(int) * arr->capacity);
    arr->size = 0;
    return arr;
}

int main() {
    DynamicArray *arr = init();
    free(arr->data);
    free(arr);
    return 0;
}

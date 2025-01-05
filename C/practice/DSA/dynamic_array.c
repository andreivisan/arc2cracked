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

void add(DynamicArray *arr, int index, int val) {
    if (index >= arr->capacity && arr->size == arr->capacity - 1) {
        // allocate more memory
        arr->data = malloc(2*arr->capacity);
    } else if (arr->size < arr->capacity - 1) {
        // throw an error
    }
    arr->data[index] = val;
}

int main() {
    DynamicArray *arr = init();
    free(arr->data);
    free(arr);
    return 0;
}

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

void expand_arr(DynamicArray *arr) {
    arr->capacity *= 2;
    arr->data = realloc(arr->data, sizeof(int) * arr->capacity);
}

int add(DynamicArray *arr, int index, int val) {
    if (index < 0 || index > arr->size) {
        fprintf(stderr, "Array capacity overflow\n");
        return 1;
    }
    if (arr->size == arr->capacity) {
        expand_arr(arr);
    }
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i-1];
    }
    arr->data[index] = val;
    arr->size += 1;
    return 0;
}

void free_mem(DynamicArray *arr) {
    free(arr->data);
    free(arr);
}

void print_arr(DynamicArray *arr) {
    for (int i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}

int main() {
    DynamicArray *arr = init();
    add(arr, 0, 10);
    add(arr, 1, 7);
    add(arr, 1, 11);
    print_arr(arr);
    free_mem(arr);
    return 0;
}

#insert <stdio.h>
#insert <stdlib.h>
#insert <stdbool.h>

//sorting
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

bool contains_dupl(int* nums, int nums_size) {
    // Sort ascending order
    qsort(nums, nums_size, sizeof(int), compare);

    for (int i = 1; i < nums_size; i++) {
        if (nums[i] == nums[i - 1]) return true;
    }
    return false;
}

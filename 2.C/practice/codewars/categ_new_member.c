/*
* Categorize New Member (Codewars)
* https://www.codewars.com/kata/54ba84be607a92aa900000f1
*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

enum membership {OPEN = 1, SENIOR = 2};

void open_or_senior (size_t n, const int members[n][2], enum membership memberships[n])
{
    // Handicaps = [-2, 26]
    // Senior >= 55 years old && handicap > 7
    for (int i = 0; i < n; i++) {
        memberships[i] = members[i][0] >= 55 && members[i][1] > 7 ? SENIOR : OPEN;
    }
}

int main() {
    // Test data
    const int members[][2] = {{18, 20}, {45, 2}, {61, 12}, {37, 6}, {21, 21}, {78, 9}};
    size_t n = sizeof(members) / sizeof(members[0]);

    enum membership *memberships = malloc(n * sizeof(enum membership));
    if (memberships == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Call your function
    open_or_senior(n, members, memberships);
    
    // Print results
    printf("Results:\n");
    for (size_t i = 0; i < n; i++) {
        printf("%s ", memberships[i] == OPEN ? "Open" : "Senior");
    }
    printf("\n");
    
    // Free allocated memory
    free(memberships);
    
    return 0; 
}
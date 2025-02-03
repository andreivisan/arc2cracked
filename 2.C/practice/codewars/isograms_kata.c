/*
* ============
*   ISOGRAMS
* ============
* An isogram is a word that has no repeating letters, consecutive or non-consecutive. 
* Implement a function that determines whether a string that contains only letters is an isogram. 
* Assume the empty string is an isogram. Ignore letter case.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool is_isogram (const char *string) {
	char seen[26] = {0};
    int len = strlen(string);
    for (int i = 0; i < len; i++) {
        char c = tolower(string[i]) - 'a';
        if (seen[c]) return false;
        seen[c] = 1;
    }
    return string[0] == 0 || true;
}

int main(void) {
    printf("%d\n", is_isogram("Dermatoglyphics")); // prints 1 (true)
    printf("%d\n", is_isogram("aba"));             // prints 0 (false)
    printf("%d\n", is_isogram("moOse"));           // prints 0 (false)
    return 0;
}
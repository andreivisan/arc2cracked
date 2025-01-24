#import <stdio.h>
#import <stdbool.h>
#import <string.h>

#define MAX_SIZE 10000

bool is_valid(char* s) {
    char stack[MAX_SIZE];
    int top = -1;

    for(int i = 0; i < strlen(s); i++) {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
            stack[++top] = s[i];
        } else {
            if (top == -1) return false;
            if ((s[i] == ')' && stack[top] == '(') ||
                (s[i] == ']' && stack[top] == '[') ||
                (s[i] == '}' && stack[top] == '{')) {
                    top--;
            } else {
                return false;
            }
        }
    }

    return top == -1;
}

int main() {
    bool valid_par = is_valid("([{}])");
    printf("is valid: %d\n", valid_par);
    return 0;
}
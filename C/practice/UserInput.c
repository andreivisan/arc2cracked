#include <stdio.h>

int main() {
    char name[25];
    int age;
    
    printf("\nWhat is your name? ");
    // read input from user
    scanf("%s", &name);
    
    printf("\nHow old are you? ");
    scanf("%d", &age);

    printf("\nHello %s, how are you?", name);
    printf("\nYou are %d years old", age);
    return 0;
}
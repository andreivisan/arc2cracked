#include <stdio.h>
#include <string.h>

int main() {
    char name[25];
    int age;
    
    printf("\nWhat is your name? ");
    // read input from user
    // scanf("%s", &name);
    // if the user needs to enter a space, use fgets
    // name - the var , 25 - the size of the var, stdin - the input source
    fgets(name, 25, stdin);
    // remove the newline character from the end of the string added by fgets
    name[strlen(name) - 1] = '\0';
    
    printf("\nHow old are you? ");
    scanf("%d", &age);

    printf("\nHello %s, how are you?", name);
    printf("\nYou are %d years old", age);
    return 0;
}
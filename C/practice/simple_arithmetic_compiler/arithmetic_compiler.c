#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType token_type;
    int value; // Only valid when token_type is TOKEN_NUMBER
} Token;
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

Token *tokenize(const char *input) {
    Token *tokens = malloc(256 * sizeof(Token));
    int index = 0;
    while (input) {
        if (isdigit(input)) {
            tokens[index].token_type = TOKEN_NUMBER;
            tokens[index].value = strtol(input, &input, 10);
            index++;
        } else {
            switch(*input) {
                case '+':
                    tokens[index].token_type = TOKEN_PLUS;
                    input++;
                    index++;
                    break;
                case '-':
                    tokens[index].token_type = TOKEN_MINUS;
                    input++;
                    index++;
                    break;
                default:
                    input++;
                    break; 
            }
        }
    }
    tokens[index].token_type = TOKEN_END;
    return tokens;
}
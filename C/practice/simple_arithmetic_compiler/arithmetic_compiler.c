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
        if (isdigit(*input)) {
            tokens[index].token_type = TOKEN_NUMBER;
            /*
             * strtol expects a char** for endptr.
             * If input is a char*, &input is char**
             */
            tokens[index].value = strtol(input, (char**)&input, 10);
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

int parse(Token *tokens, int *result) {
    int index = 0;
    if (tokens[index].token_type != TOKEN_NUMBER) {
        return 0; // Invalid expression
    }
    *result = tokens[index].value;
    index++;
    while (tokens[index].token_type != TOKEN_END) {
        if (tokens[index].token_type == TOKEN_PLUS) {
            index++;
            if (tokens[index].token_type != TOKEN_NUMBER) {
                return 0; // Invalid expression
            }
            *result += tokens[index].value;
        } else if (tokens[index].token_type == TOKEN_MINUS) {
            index++;
            if (tokens[index].token_type != TOKEN_NUMBER) {
                return 0; // Invalid expression
            }
            *result -= tokens[index].value; 
        }
        index++;
    }
    return 1;
}

int main() {
    return 0;
}
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
    while (*input) {
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

void generate_assembly(Token *tokens) {
    int position = 0;
    printf("Assembly-like Instructions:\n");
    if (tokens[position].token_type == TOKEN_NUMBER) {
        printf("LOAD %d\n", tokens[position].value);
        position++;
    }
    while (tokens[position].token_type != TOKEN_END) {
        if (tokens[position].token_type == TOKEN_PLUS) {
            position++;
            if (tokens[position].token_type == TOKEN_NUMBER) {
                printf("ADD %d\n", tokens[position].value);
            }
        } else if (tokens[position].token_type == TOKEN_MINUS) {
            position++;
            if (tokens[position].token_type == TOKEN_NUMBER) {
                printf("SUB %d\n", tokens[position].value);
            }
        }
        position++;
    }
}

int main() {
    char input[256];
    printf("Enter an arithmetic expression: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    Token* tokens = tokenize(input);
    int result;
    if (parse(tokens, &result)) {
        printf("Result: %d\n", result);
        generate_assembly(tokens);
    } else {
        printf("Invalid expression!\n");
    }
    
    free(tokens); // Remember to free allocated memory
    return 0;
}
#ifndef KK_JSON_H
#define KK_JSON_H

#include <stddef.h>
#include <stdio.h>

typedef void (*json_value_callback)(const char *value, size_t length, void *agent_response);

typedef struct {
    int in_string;
    int escape_next;
    int depth;
    int array_depth;
    char current_key[1024];
    size_t key_pos;
    int is_parsing_key;
    int parsing_response_value;
    char value_buffer[4096];
    size_t value_pos;
    json_value_callback callback;
    int found_response;
    void *agent_response;
} JsonParser;

void json_parser_init(JsonParser *parser, json_value_callback callback, void *agent_response);
void json_parse(JsonParser *parser, const char *chunk, size_t length);

#endif
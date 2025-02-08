#include "kk_json.h"
#include <string.h>
#include <ctype.h>

void json_parser_init(JsonParser *parser, json_value_callback callback, void *agent_response) {
    memset(parser, 0, sizeof(JsonParser));
    parser->callback = callback;
    parser->agent_response = agent_response;
}

static void handle_json_value(JsonParser *parser) {
    if (parser->found_response && parser->depth == 1) {
        // Null-terminate the value_buffer.
        if (parser->value_pos < sizeof(parser->value_buffer)) {
            parser->value_buffer[parser->value_pos] = '\0';
        }
        parser->callback(parser->value_buffer, parser->value_pos, parser->agent_response);
    }
    parser->value_pos = 0;
}

void json_parse(JsonParser *parser, const char *chunk, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        char curr_chunk = chunk[i];
        if (parser->escape_next) {
            if (parser->value_pos < sizeof(parser->value_buffer)-1) {
                switch (curr_chunk) {
                    case '"':  parser->value_buffer[parser->value_pos++] = '"'; break;
                        case '\\': parser->value_buffer[parser->value_pos++] = '\\'; break;
                        case '/':  parser->value_buffer[parser->value_pos++] = '/'; break;
                        case 'b':  parser->value_buffer[parser->value_pos++] = '\b'; break;
                        case 'f':  parser->value_buffer[parser->value_pos++] = '\f'; break;
                        case 'n':  parser->value_buffer[parser->value_pos++] = '\n'; break;
                        case 'r':  parser->value_buffer[parser->value_pos++] = '\r'; break;
                        case 't':  parser->value_buffer[parser->value_pos++] = '\t'; break;
                        case 'u':  /* Simple unicode handling (basic) */
                            if (i + 4 < length) {
                                // Skip unicode bytes for simplicity
                                i += 4;
                                parser->value_buffer[parser->value_pos++] = '?';
                            }
                            break;
                        default: parser->value_buffer[parser->value_pos++] = curr_chunk; break;
                }
            }
            parser->escape_next = 0;
            continue;
        }
        if (parser->in_string) {
            if (curr_chunk == '\\') {
                parser->escape_next = 1;
            } else if (curr_chunk == '"') {
                parser->in_string = 0;
                if (parser->is_parsing_key) {
                    parser->current_key[parser->key_pos] = '\0';
                    if (strcmp(parser->current_key, "response") == 0) {
                        parser->found_response = 1;
                    }
                    parser->key_pos = 0;
                    parser->is_parsing_key = 0;
                } else if (parser->parsing_response_value) {
                    handle_json_value(parser);
                    parser->parsing_response_value = 0;
                    parser->found_response = 0;
                }
            } else {
                if (parser->is_parsing_key) {  // Building key
                    if (parser->key_pos < sizeof(parser->current_key)-1) {
                        parser->current_key[parser->key_pos++] = curr_chunk;
                    }
                } else if (parser->parsing_response_value) {
                    if (parser->value_pos < sizeof(parser->value_buffer) - 1) {
                        parser->value_buffer[parser->value_pos++] = curr_chunk;
                    }
                }
            }
        } else {
            switch(curr_chunk) {
                case '"':
                    if (parser->is_parsing_key) parser->key_pos = 0;
                    parser->in_string = 1;    
                    break;
                case '{':
                    parser->depth++;
                    parser->is_parsing_key = 1;
                    break;
                case '}':
                    parser->depth--;
                    break;
                case '[':
                    parser->array_depth++;
                    break;
                case ']':
                    parser->array_depth--;
                    break;
                case ':':
                    if (parser->depth == 1 && !parser->array_depth) {
                        if (parser->found_response) {
                            parser->parsing_response_value = 1;
                            parser->value_pos = 0;
                        }
                        parser->is_parsing_key = 0;
                    }
                    break;
                case ',':
                    if (parser->depth == 1 && !parser->array_depth) {
                        // After a comma, expect a new key.
                        parser->is_parsing_key = 1;
                    }
                    break;
            }
        }
    }
}
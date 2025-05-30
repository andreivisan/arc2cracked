#ifndef OLLAMA_CLIENT_H
#define OLLAMA_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct ResponseBuffer {
    char *data;
    size_t size;
    size_t parsed_offset;
    size_t allocated_size;
    char *parsed_response;
} ResponseBuffer;

size_t write_callback(void *contents, size_t size, size_t no_memb, void *user_data);
int parse_ollama_response(ResponseBuffer *buffer);
ResponseBuffer* call_ollama(const char *model, const char *prompt);

#endif
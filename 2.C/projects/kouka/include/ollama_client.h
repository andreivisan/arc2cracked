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
} ResponseBuffer;

size_t write_callback(void *contents, size_t size, size_t no_memb, void *user_data);
void call_ollama(const char *prompt);

#endif
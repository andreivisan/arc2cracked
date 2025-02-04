#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct ResponseBuffer {
    char *data;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t no_memb, void *user_data);
void call_ollama();
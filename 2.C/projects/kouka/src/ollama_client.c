#include "ollama_client.h"

size_t write_callback(void *contents, size_t size, size_t no_memb, void *user_data)
{
    size_t real_size = size * no_memb;
    struct ResponseBuffer *buffer = (struct ResponseBuffer *)user_data;
    char *ptr = realloc(buffer->data, buffer->size + real_size + 1);
    if(!ptr) return 0;
    buffer->data = ptr;
    memcpy(&(buffer->data[buffer->size]), contents, real_size);
    buffer->size += real_size;
    buffer->data[buffer->size] = 0;
    return real_size;
}





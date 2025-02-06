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

void call_ollama(const char *prompt) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return;
    }
    ResponseBuffer buffer = {0};
    char json_data[1024];
    snprintf(json_data, sizeof(json_data), "{\"model\":\"qwen2.5-coder:32b\",\"prompt\":\"%s\"}", prompt);
    // initialize Ollama request headers
    struct curl_slist *headers = NULL;
}





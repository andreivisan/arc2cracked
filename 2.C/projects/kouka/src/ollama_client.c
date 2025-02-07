#include "common.h"
#include "kk_json.h"
#include "ollama_client.h"

size_t write_callback(void *contents, size_t size, size_t no_memb, void *user_data)
{
    size_t real_size = size * no_memb;
    ResponseBuffer *buffer = (ResponseBuffer *)user_data;
    // Reallocate buffer
    char *ptr = realloc(buffer->data, buffer->size + real_size + 1);
    if(!ptr) return 0;
    buffer->data = ptr;
    memcpy(&(buffer->data[buffer->size]), contents, real_size);
    buffer->size += real_size;
    buffer->data[buffer->size] = '\0';
    // Parse ONLY the new data
    parse_ollama_response(buffer);
    return real_size;
}

static void response_callback(const char *value, size_t length) {
    for (size_t i = 0; i < length; i++) {
        fputc(value[i], stdout);
    }
}

int parse_ollama_response(ResponseBuffer *buffer) {
    static JsonParser parser;
    static int parser_initialized = 0;
    if (!parser_initialized) {
        json_parser_init(&parser, response_callback);
        parser_initialized = 1;
    }
    // Process only new data since last parse
    char *new_data_start = buffer->data + buffer->parsed_offset;
    size_t new_data_length = buffer->size - buffer->parsed_offset;
    json_parse(&parser, new_data_start, new_data_length);
    // Update parsed offset to mark this data as processed
    buffer->parsed_offset = buffer->size;
    return STATUS_SUCCESS;
}

void call_ollama(const char *model, const char *prompt) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return;
    }
    ResponseBuffer buffer = {0};
    char json_data[1024];
    snprintf(json_data, sizeof(json_data), "{\"model\":\"%s\",\"prompt\":\"%s\"}", model, prompt);
    // initialize Ollama request headers
    struct curl_slist *headers = NULL;
    // add headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // curl set options
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    // perform request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
    }
    // cleanup
    free(buffer.data);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

int main() {
    setbuf(stdout, NULL);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    call_ollama("qwen2.5-coder:14b", "Please give me the structure of a Java Spring Web application in JSON format");
    curl_global_cleanup();
    return 0;
}





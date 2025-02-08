#include "common.h"
#include "kk_json.h"
#include "ollama_client.h"

size_t write_callback(void *contents, size_t size, size_t no_memb, void *ai_response) {
    if (!contents || !ai_response) { 
        fprintf(stderr, "NULL pointer in write callback\n");
        return 0;
    }
    size_t real_size = size * no_memb;
    ResponseBuffer *buffer = (ResponseBuffer *)ai_response;
    if (buffer->size + real_size + 1> buffer->allocated_size) {
        size_t new_alloc = (buffer->allocated_size >0) ? buffer->allocated_size * 2 : 4096;
        while (buffer->size + real_size + 1> new_alloc) {
            new_alloc *= 2;
        }
        char *ptr = realloc(buffer->data, new_alloc);
        if (!ptr) {
            fprintf(stderr, "Failed to reallocate memory\n");
            return 0;
        }
        buffer->data = ptr;
        buffer->allocated_size = new_alloc;
    }
    memcpy(&(buffer->data[buffer->size]), contents, real_size);
    buffer->size += real_size;
    buffer->data[buffer->size] = '\0';

    if (parse_ollama_response(buffer) != STATUS_SUCCESS) {
        fprintf(stderr, "Failed to parse response\n");
    }
    return real_size;
}

static void response_callback_store(const char *value, size_t length, void *agent_response) {
    if (!value || !agent_response) {  // Add NULL checks
        fprintf(stderr, "NULL pointer in callback\n");
        return;
    }
    ResponseBuffer *buffer = (ResponseBuffer *)agent_response;
    // If parsed response is not yet set, allocate it
    if (buffer->parsed_response == NULL) {
        buffer->parsed_response = (char *)malloc(length + 1);
        if (!buffer->parsed_response) {
            fprintf(stderr, "Failed to allocate memory for parsed response\n");
            return;
        }
        memcpy(buffer->parsed_response, value, length);
        buffer->parsed_response[length] = '\0';
    } else {
        // Otherwise, append the new data
        size_t old_len = strlen(buffer->parsed_response);
        char *new_ptr = realloc(buffer->parsed_response, old_len + length + 1);
        if (!new_ptr) {
            fprintf(stderr, "Failed to reallocate memory for parsed response\n");
            return;
        }
        buffer->parsed_response = new_ptr;
        memcpy(buffer->parsed_response + old_len, value, length);
        buffer->parsed_response[old_len + length] = '\0';
    }
}

int parse_ollama_response(ResponseBuffer *buffer) {
    static JsonParser parser;
    static int parser_initialized = 0;
    if (!parser_initialized) {
        json_parser_init(&parser, response_callback_store, buffer);
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

ResponseBuffer* call_ollama(const char *model, const char *prompt) {
    // First allocate the response buffer
    ResponseBuffer *buffer = malloc(sizeof(ResponseBuffer));
    if (!buffer) {
        fprintf(stderr, "Failed to allocate response buffer\n");
        return NULL;
    }

    // Initialize buffer fields
    buffer->data = malloc(4096);  // Initial 4KB allocation
    if (!buffer->data) {
        fprintf(stderr, "Failed to allocate buffer data\n");
        free(buffer);
        return NULL;
    }
    buffer->allocated_size = 4096;
    buffer->size = 0;
    buffer->parsed_offset = 0;
    buffer->parsed_response = NULL;
    buffer->data[0] = '\0';

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl: ensure curl_global_init() was called\n");
        free(buffer->data);
        free(buffer);
        return NULL;
    }
   
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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    // perform request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(buffer->data);
        free(buffer);
        return NULL;
    }
    // cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return buffer;
}

int main() {
    // Initialize CURL globally FIRST
    CURLcode curl_init_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (curl_init_result != CURLE_OK) {
        fprintf(stderr, "Failed to initialize CURL globally: %s\n", 
                curl_easy_strerror(curl_init_result));
        return 1;
    }

    setbuf(stdout, NULL);  // Unbuffer stdout
    ResponseBuffer *response = call_ollama("qwen2.5-coder:32b", "Please give me the structure of a Java Spring Web application in JSON format");

    if (!response) {
        fprintf(stderr, "No response buffer created\n");
        curl_global_cleanup();
        return 1;
    }

    if (response->parsed_response) {
        printf("Parsed response: %s\n", response->parsed_response);
    } else {
        printf("No parsed response available\n");
    }
    
    // Clean up
    if (response->data) {
        free(response->data);
    }
    if (response->parsed_response) {
        free(response->parsed_response);
    }
    free(response);
    curl_global_cleanup();

    return 0;
}
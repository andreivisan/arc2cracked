#include "common.h"
#include "ollama_client.h"

size_t write_callback(void *contents, size_t size, size_t no_memb, void *user_data)
{
    size_t real_size = size * no_memb;
    ResponseBuffer *buffer = (ResponseBuffer *)user_data;
    // Reallocate buffer->data
    char *ptr = realloc(buffer->data, buffer->size + real_size + 1);
    if(!ptr) return 0;
    buffer->data = ptr;
    // Copy the newly arrived chunk into the end of buffer->data
    memcpy(&(buffer->data[buffer->size]), contents, real_size);
    buffer->size += real_size;
    buffer->data[buffer->size] = '\0';
    parse_ollama_response(buffer);
    return real_size;
}

int parse_ollama_response(ResponseBuffer *buffer) {
    // Now parse only newly added text
    // i.e. from buffer->data + buffer->parsed_offset up to buffer->data + buffer->size
    char *start = buffer->data + buffer->parsed_offset;
    char *end   = buffer->data + buffer->size;
    const char *search_key = "response\":\"";
    char *found = NULL;
    while ((found = strstr(start, search_key)) && found < end) {
        found += strlen(search_key);
        // Find the closing quote
        char *closing_quote = strchr(found, '"');
        if (!closing_quote || closing_quote > end) {
            break;
        }
        // Print the substring [found, closing_quote)
        for (char *p = found; p < closing_quote; p++) {
            fputc(*p, stdout);
            fflush(stdout);
        }
        // advance 'start' so we keep searching after this closing quote
        start = closing_quote + 1;
    }
    // Update parsed_offset so next time we skip all the old data
    // We only update if 'start' advanced beyond the old offset
    buffer->parsed_offset = start - buffer->data;
    return STATUS_SUCCESS;
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
    curl_global_init(CURL_GLOBAL_DEFAULT);
    call_ollama("Please give me the structure of a Java jar application in JSON format");
    curl_global_cleanup();
    return 0;
}





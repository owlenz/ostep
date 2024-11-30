#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memstrct {
  char *memory;
  size_t size;
} MemoryStruct;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  MemoryStruct *userp) {
  size_t realsize = size * nmemb;
  userp->memory = realloc(userp->memory, userp->size + realsize + 1);
  if (userp->memory == NULL) {
    printf("Not enough memory!\n");
    return 0;
  }
  memcpy(&(userp->memory[userp->size]), contents, realsize);
  userp->size += realsize;
  userp->memory[userp->size] = '\0';
  return realsize;
}

int main() {
  CURL *curl;
  CURLcode res;

  MemoryStruct chunk;

  chunk.memory = malloc(1);
  chunk.size = 0;
  curl_global_init(CURL_GLOBAL_ALL); // Initialize curl
  curl = curl_easy_init();           // Initialize a curl session

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    } else {
      printf("data: %s", chunk.memory);
    }
  }

  // Cleanup
  free(chunk.memory);
  curl_easy_cleanup(curl);
  return 0;
}

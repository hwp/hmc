// main.c
// Main Program for HWP Media Center
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "utils.h"

#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *buffer, size_t size, size_t nmemb, void *dstr) {
  d_string* str = (d_string*) dstr;
  dstr_ncat(str, buffer, size * nmemb);
  return nmemb;
}

int main(int argc, char** argv) {
  CURL* curl;
  CURLcode res;

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);

    d_string* content = dstr_alloc();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, content);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    printf("size=%d, cap=%d\n%s", content->size, content->cap, content->str);

    dstr_free(content);
    curl_easy_cleanup(curl);
  }

  return 0;
}


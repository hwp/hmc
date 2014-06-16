// utils.c
// Utility Functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define DSTR_INIT_CAP 1024;

d_string* dstr_alloc(void) {
  d_string* s = malloc(sizeof(d_string));
  if (s == NULL) {
    fprintf(stderr, "Failed to allocate memory, (%s:%d)", __FILE__, __LINE__);
    return NULL;
  }

  s->cap = DSTR_INIT_CAP;
  s->size = 0;

  s->str = malloc(sizeof(char) * (s->cap + 1));
  if(s->str == NULL) {
    free(s);
    fprintf(stderr, "Failed to allocate memory, (%s:%d)", __FILE__, __LINE__);
    return NULL;
  }
  s->str[s->size] = '\0';
  
  return s;
}

void dstr_free(d_string* str) {
  free(str->str);
  free(str);
}

ssize_t dstr_cat(d_string* dest, const char* src) {
  return dstr_ncat(dest, src, strlen(src));
}

ssize_t dstr_ncat(d_string* dest, const char* src, size_t n) {
  if (n + dest->size > dest->cap) {
    do {
      dest->cap *= 2;
    } while (n + dest->size > dest->cap);

    char* nstr = malloc(sizeof(char) * (dest->cap + 1));
    if (nstr == NULL) {
      fprintf(stderr, "Failed to allocate memory, (%s:%d)", __FILE__, __LINE__);
      return -1;
    }
    strcpy(nstr, dest->str);
    free(dest->str);
    dest->str = nstr;
  }
  strncat(dest->str, src, n);
  dest->size += n;
  return n;
}

ssize_t dstr_app(d_string* dest, char src) {
  if (src == '\0') {
    return 0;
  }

  char buf[1];
  buf[0] = src;
  return dstr_ncat(dest, buf, 1);
}

void dstr_clear(d_string* str) {
  str->size = 0;
  str->str[0] = '\0';
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *dstr) {
  d_string* str = (d_string*) dstr;
  dstr_ncat(str, buffer, size * nmemb);
  return nmemb;
}

d_string* url_request(const char* url) {
  CURL* curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl == NULL) {
    fprintf(stderr, "Failed to initiate CURL.\n");
    return NULL;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "hwp"); 

  d_string* content = dstr_alloc();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, content);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "Failed to connect: %s\n", curl_easy_strerror(res));
  }

  curl_easy_cleanup(curl);

  return content;
}


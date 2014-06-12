// utils.c
// Utility Functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "utils.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define DSTR_INIT_CAP 1024;

d_string* dstr_alloc() {
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


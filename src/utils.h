// utils.h
// Utility Functions
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>

/**
 * Dynamic String
 */
typedef struct {
  size_t cap;
  size_t size;
  char* str;
} d_string;

/**
 * Queue Element
 */
typedef struct {
  element* next;
  void* value;
} element;

typedef struct {
  element* first;
  element* last;
} queue;

/**
 * Get an instantce of dynamic string.
 */
d_string* dstr_alloc(void);

/**
 * Free a dynamic string.
 */
void dstr_free(d_string* str);

/**
 * Concatenate a c string to a dynamic string.
 */
ssize_t dstr_cat(d_string* dest, const char* src);
ssize_t dstr_ncat(d_string* dest, const char* src, size_t n);
ssize_t dstr_app(d_string* dest, char src);

/**
 * Clear the content.
 */
void dstr_clear(d_string* str);

queue* queue_alloc(void);
void queue_free(queue* q);

/**
 * data cannot be NULL;
 */
void queue_push(queue* q, void* data);

/**
 * @return data, or NULL if no more element in queue;
 */
void* queue_pop(queue* q);

/**
 * Get content form an URL.
 * @return a new dynamic string, which should be freed after use.
 */
d_string* url_request(const char* url);

#endif  // UTILS_H_


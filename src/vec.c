/** 
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "vec.h"

#define VEC_MIN_CAPACITY 4


void vec_expand_(char **data, int *length, int *capacity, int memsz) {
  if (*length + 1 > *capacity) {
    if (*capacity == 0) {
      *capacity = VEC_MIN_CAPACITY;
    } else {
      *capacity <<= 1;
    }
    *data = realloc(*data, *capacity * memsz);
  }
}


void vec_reserve_(char **data, int *length, int *capacity, int memsz, int n) {
  if (n > *capacity) {
    *capacity = n;
    *data = realloc(*data, *capacity * memsz);
  }
}


void vec_compact_(char **data, int *length, int *capacity, int memsz) {
  if (*length == 0) {
    free(*data);
    *data = NULL;
    *capacity = 0;
    return;
  }
  *capacity = *length;
  *data = realloc(*data, *capacity * memsz);
}


void vec_splice_(char **data, int *length, int *capacity, int memsz,
                 int start, int count
) {
  memmove(*data + start * memsz,
          *data + (start + count) * memsz,
          (*length - start - count) * memsz);
}


void vec_insert_(char **data, int *length, int *capacity, int memsz,
                 int idx
) {
  vec_expand_(data, length, capacity, memsz);
  memmove(*data + (idx + 1) * memsz,
          *data + idx * memsz,
          (*length - idx) * memsz);
}


void vec_swap_(char **data, int *length, int *capacity, int memsz,
               int idx1, int idx2 
) {
  char *tmp;
  vec_expand_(data, length, capacity, memsz);
  tmp = *data + *length * memsz;
  memcpy(tmp, *data + (idx1 * memsz), memsz);
  memcpy(*data + (idx1 * memsz), *data + (idx2 * memsz), memsz);
  memcpy(*data + (idx2 * memsz), tmp, memsz);
}


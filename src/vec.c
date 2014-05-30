/** 
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "vec.h"

#define VEC_MIN_CAPACITY 4

#define vec_absindex(idx, len) (idx) < 0 ? (len) + (idx) : (idx)


void vec_expand_(char **members, int *length, int *capacity, int memsz) {
  if (*length + 1 > *capacity) {
    if (*capacity == 0) {
      *capacity = VEC_MIN_CAPACITY;
    } else {
      *capacity <<= 1;
    }
    *members = realloc(*members, *capacity * memsz);
  }
}


void vec_compact_(char **members, int *length, int *capacity, int memsz) {
  if (*length == 0) {
    free(*members);
    *members = NULL;
    *capacity = 0;
    return;
  }
  *capacity = *length;
  *members = realloc(*members, *capacity * memsz);
}


void vec_splice_(char **members, int *length, int *capacity, int memsz,
                 int start, int count
) {
  start = vec_absindex(start, *length);
  memmove(*members + start * memsz,
          *members + (start + count) * memsz,
          (*length - start - count) * memsz);
}


void vec_insert_(char **members, int *length, int *capacity, int memsz,
                 int index
) {
  index = vec_absindex(index, *length);
  vec_expand_(members, length, capacity, memsz);
  memmove(*members + (index + 1) * memsz,
          *members + index * memsz,
          (*length - index) * memsz);
}


void vec_swap_(char **members, int *length, int *capacity, int memsz,
               int idx1, int idx2 
) {
  char *tmp;
  idx1 = vec_absindex(idx1, *length);
  idx2 = vec_absindex(idx2, *length);
  vec_expand_(members, length, capacity, memsz);
  tmp = *members + *length * memsz;
  memcpy(tmp, *members + (idx1 * memsz), memsz);
  memcpy(*members + (idx1 * memsz), *members + (idx2 * memsz), memsz);
  memcpy(*members + (idx2 * memsz), tmp, memsz);
}


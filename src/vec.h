/** 
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define VEC_VERSION "0.0.1"


#define vec_unpack_(v)\
  (char**)&(v)->members, &(v)->length, &(v)->capacity, sizeof(*(v)->members)

#define vec_absindex_(idx, len) (idx) < 0 ? (len) + (idx) : (idx)


#define vec_t(T)\
  struct { T *members; int length, capacity; }


#define vec_init(v)\
  memset((v), 0, sizeof(*(v)))


#define vec_deinit(v)\
  ( free((v)->members),\
    vec_init(v) ) 


#define vec_push(v, val)\
  ( vec_expand_(vec_unpack_(v)),\
    (v)->members[(v)->length++] = (val) )


#define vec_pop(v)\
  (v)->members[--(v)->length]


#define vec_splice(v, start, count)\
  ( vec_splice_(vec_unpack_(v), start, count),\
    (v)->length -= (count) )


#define vec_insert(v, index, val)\
  ( vec_insert_(vec_unpack_(v), index),\
    (v)->members[index] = (val),\
    (v)->length++ )
    

#define vec_sort(v, fn)\
  qsort((v)->members, (v)->length, sizeof(*(v)->members), fn)


#define vec_swap(v, idx1, idx2)\
  vec_swap_(vec_unpack_(v), idx1, idx2)


#define vec_truncate(v, len)\
  ((v)->length = (len) < (v)->length ? (len) : (v)->length)


#define vec_clear(v)\
  ((v)->length = 0)


#define vec_first(v)\
  (v)->members[0]


#define vec_last(v)\
  (v)->members[(v)->length - 1]

 
#define vec_compact(v)\
  vec_compact_(vec_unpack_(v))


#define vec_extend(v, v2)\
  do {\
    int i__;\
    for (i__ = 0; i__ < (v2)->length; i__++) {\
      vec_push((v), (v2)->members[i__]);\
    }\
  } while (0)


#define vec_find(v, val, idx)\
  do {\
    for ((idx) = 0; (idx) < (v)->length; (idx)++) {\
      if ((v)->members[(idx)] == (val)) break;\
    }\
    if ((idx) == (v)->length) (idx) = -1;\
  } while (0)


#define vec_remove(v, val)\
  do {\
    int idx__;\
    vec_find(v, val, idx__);\
    if (idx__ != -1) vec_splice(v, idx__, 1);\
  } while (0)


#define vec_reverse(v)\
  do {\
    int i__ = (v)->length / 2;\
    while (i__--) {\
      vec_swap((v), i__, -(i__ + 1));\
    }\
  } while (0)


#define vec_foreach(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = 0;\
        (iter) < (v)->length && (((var) = (v)->members[(iter)]), 1);\
        ++(iter))


#define vec_foreach_rev(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = (v)->length - 1;\
        (iter) >= 0 && (((var) = (v)->members[(iter)]), 1);\
        --(iter))


#define vec_foreach_ptr(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = 0;\
        (iter) < (v)->length && (((var) = &(v)->members[(iter)]), 1);\
        ++(iter))


#define vec_foreach_ptr_rev(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = (v)->length - 1;\
        (iter) >= 0 && (((var) = &(v)->members[(iter)]), 1);\
        --(iter))



void vec_expand_(char **members, int *length, int *capacity, int memsz);
void vec_compact_(char **members, int *length, int *capacity, int memsz);
void vec_splice_(char **members, int *length, int *capacity, int memsz,
                 int start, int count);
void vec_insert_(char **members, int *length, int *capacity, int memsz,
                 int index);
void vec_swap_(char **members, int *length, int *capacity, int memsz,
               int idx1, int idx2);


typedef vec_t(void*) vec_void_t;
typedef vec_t(char*) vec_str_t;
typedef vec_t(int) vec_int_t;
typedef vec_t(char) vec_char_t;
typedef vec_t(float) vec_float_t;
typedef vec_t(double) vec_double_t;

#endif

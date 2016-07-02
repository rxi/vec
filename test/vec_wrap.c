
#include "../src/vec.h"
#include "vec_wrap.h"

#define vec_t(T)\
  struct { T *data; int length, capacity; }


struct test_vec *
_vec_init(struct test_vec *v) {
  vec_init(v);
  return v;
}

struct test_vec *
_vec_deinit(struct test_vec *v) {
  vec_deinit(v);
  return v;
}


struct test_vec *
_vec_push(struct test_vec *v, int val) {
  vec_push(v, val);
  return v;
}


int _vec_pop(struct test_vec *v) {
  return vec_pop(v);
}

struct test_vec *
_vec_splice(struct test_vec *v, int start, int count) {
  vec_splice(v, start, count);
  return v;
}

struct test_vec *
_vec_swapsplice(struct test_vec *v, int start, int count) {
  vec_swapsplice(v, start, count);
  return v;
}


int _vec_insert(struct test_vec *v, int idx, int val) {
  return vec_insert(v, idx, val);
}


struct test_vec *
_vec_sort(struct test_vec *v, int (*fn)(const void*, const void*)) {
  vec_sort(v, fn);
  return v;
}


struct test_vec * _vec_swap(struct test_vec *v, int idx1, int idx2) {
  vec_swap(v, idx1, idx2);
  return v;
}


struct test_vec * _vec_truncate(struct test_vec *v, int len) {
  vec_truncate(v, len);
  return v;
}


struct test_vec * _vec_clear(struct test_vec *v) {
  vec_clear(v);
  return v;
}


int _vec_first(struct test_vec *v) {
  return vec_first(v);
}


int _vec_last(struct test_vec *v) {
  return vec_last(v);
}


struct test_vec * _vec_reserve(struct test_vec *v, int n) {
  vec_reserve(v, n);
  return v;
}


struct test_vec * _vec_compact(struct test_vec *v) {
  vec_compact(v);
  return v;
}

struct test_vec * _vec_pusharr(struct test_vec *v, int *arr, int count) {
  vec_pusharr(v, arr, count);
  return v;
}


struct test_vec * _vec_extend(struct test_vec *v, struct test_vec *v2) {
  vec_extend(v, v2);
  return v;
}

/* struct test_vec * vec_find(v, val, idx) { */
/* } */


struct test_vec * _vec_remove(struct test_vec *v, int val) {
  vec_remove(v, val);
  return v;
}


struct test_vec * _vec_reverse(struct test_vec *v) {
  vec_reverse(v);
  return v;
}

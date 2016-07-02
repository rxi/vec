
#ifndef VEC_WRAP_H
#define VEC_WRAP_H

struct test_vec {
  int *data;
  int length;
  int capacity;
};

struct test_vec *_vec_clear(struct test_vec *v);
struct test_vec *_vec_compact(struct test_vec *v);
struct test_vec *_vec_deinit(struct test_vec *v);
struct test_vec *_vec_extend(struct test_vec *v, struct test_vec *v2);
int _vec_first(struct test_vec *v);
struct test_vec *_vec_init(struct test_vec *v);
int _vec_insert(struct test_vec *v, int idx, int val);
int _vec_last(struct test_vec *v);
int _vec_pop(struct test_vec *v);
struct test_vec *_vec_pusharr(struct test_vec *v, int *arr, int count);
struct test_vec *_vec_push(struct test_vec *v, int val);
struct test_vec *_vec_remove(struct test_vec *v, int val);
struct test_vec *_vec_reserve(struct test_vec *v, int n);
struct test_vec *_vec_reverse(struct test_vec *v);
struct test_vec *_vec_sort(struct test_vec *v, int (*fn)(const void *, const void*));
struct test_vec *_vec_splice(struct test_vec *v, int start, int count);
struct test_vec *_vec_swapsplice(struct test_vec *v, int start, int count);
struct test_vec *_vec_swap(struct test_vec *v, int idx1, int idx2);
struct test_vec *_vec_truncate(struct test_vec *v, int len);

#endif /* VEC_WRAP_H */

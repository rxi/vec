#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"


#define test_section(desc)\
  do {\
    printf("-- %s\n", desc);\
  } while (0)

#define test_assert(res)\
  do {\
    int pass = res;\
    printf("line %d : %s\n", __LINE__, pass ? "PASS" : "FAIL");\
    if (pass) { pass_count++; } else { fail_count++; }\
  } while (0)


#define test_print_res()\
  do {\
    printf("------------------------------------------------------------\n");\
    printf("-- Results:   %3d Total    %3d Passed    %3d Failed       --\n",\
           pass_count + fail_count, pass_count, fail_count);\
    printf("------------------------------------------------------------\n");\
  } while (0)

int pass_count = 0;
int fail_count = 0;


int intptrcmp(const void *a_, const void *b_) {
  const int *a = a_, *b = b_;
  return *a < *b ? -1 : *a > *b;
}



int main(void) {

  { test_section("vec_push");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 1000; i++) vec_push(&v, i * 2);
    test_assert(v.members[1] == 2);
    test_assert(v.members[999] == 999 * 2);
    vec_deinit(&v);
  }

  { test_section("vec_pop");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 123);
    vec_push(&v, 456);
    vec_push(&v, 789);
    test_assert(vec_pop(&v) == 789);
    test_assert(vec_pop(&v) == 456);
    test_assert(vec_pop(&v) == 123);
    vec_deinit(&v);
  }

  { test_section("vec_splice");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 1000; i++) vec_push(&v, i);
    vec_splice(&v, 0, 10);
    test_assert(v.members[0] == 10);
    vec_splice(&v, 10, 10);
    test_assert(v.members[10] == 30);
    vec_splice(&v, -50, 50);
    test_assert(v.members[v.length - 1] == 949);
    vec_deinit(&v);
  }

  { test_section("vec_insert");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 1000; i++) vec_insert(&v, 0, i);
    test_assert(v.members[0] == 999);
    test_assert(v.members[v.length - 1] == 0);
    vec_insert(&v, 10, 123);
    test_assert(v.members[10] == 123);
    test_assert(v.length == 1001);
    vec_insert(&v, -2, 678);
    test_assert(v.members[999] == 678);
    vec_deinit(&v);
  }

  { test_section("vec_sort");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 3);
    vec_push(&v, -1);
    vec_push(&v, 0);
    vec_sort(&v, intptrcmp);
    test_assert(v.members[0] == -1);
    test_assert(v.members[1] == 0);
    test_assert(v.members[2] == 3);
    vec_deinit(&v);
  }

  { test_section("vec_swap");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 'a');
    vec_push(&v, 'b');
    vec_push(&v, 'c');
    vec_swap(&v, 0, 2);
    test_assert(v.members[0] == 'c' && v.members[2] == 'a');
    vec_swap(&v, 0, 1);
    test_assert(v.members[0] == 'b' && v.members[1] == 'c');
    vec_swap(&v, 1, 2);
    test_assert(v.members[1] == 'a' && v.members[2] == 'c');
    vec_deinit(&v);
  }

  { test_section("vec_truncate");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 1000; i++) vec_push(&v, 0);
    vec_truncate(&v, 10000);
    test_assert(v.length == 1000);
    vec_truncate(&v, 900);
    test_assert(v.length == 900);
    vec_deinit(&v);
  }

  { test_section("vec_clear");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 1);
    vec_push(&v, 2);
    vec_clear(&v);
    test_assert(v.length == 0);
    vec_deinit(&v);
  }

  { test_section("vec_first");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 0xcafef00d);
    vec_push(&v, 0);
    test_assert(vec_first(&v) == 0xcafef00d);
    vec_deinit(&v);
  }

  { test_section("vec_last");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 0);
    vec_push(&v, 0xcafef00d);
    test_assert(vec_last(&v) == 0xcafef00d);
    vec_deinit(&v);
  }

  { test_section("vec_reserve");
    vec_int_t v;
    vec_init(&v);
    vec_reserve(&v, 100);
    test_assert(v.capacity == 100);
    vec_reserve(&v, 50);
    test_assert(v.capacity == 100);
    vec_deinit(&v);
    vec_init(&v);
    vec_push(&v, 123);
    vec_push(&v, 456);
    vec_reserve(&v, 200);
    test_assert(v.capacity == 200);
    vec_deinit(&v);
  }

  { test_section("vec_compact");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 1000; i++) vec_push(&v, 0);
    vec_truncate(&v, 3);
    vec_compact(&v);
    test_assert(v.length == v.capacity);
    vec_deinit(&v);
  }

  { test_section("vec_extend");
    vec_int_t v, v2;
    vec_init(&v);
    vec_init(&v2);
    vec_push(&v, 12);
    vec_push(&v, 34);
    vec_push(&v2, 56);
    vec_push(&v2, 78);
    vec_extend(&v, &v2);
    test_assert(v.members[0] == 12 && v.members[1] == 34 &&
                v.members[2] == 56 && v.members[3] == 78);
    test_assert(v.length == 4);
    vec_deinit(&v);
    vec_deinit(&v2);
  }

  { test_section("vec_find");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 26; i++) vec_push(&v, 'a' + i);
    vec_find(&v, 'a', i);
    test_assert(i == 0);
    vec_find(&v, 'z', i);
    test_assert(i == 25);
    vec_find(&v, 'd', i);
    test_assert(i == 3);
    vec_find(&v, '_', i);
    test_assert(i == -1);
    vec_deinit(&v);
  }

  { test_section("vec_remove");
    vec_int_t v;
    vec_init(&v);
    int i;
    for (i = 0; i < 26; i++) vec_push(&v, 'a' + i);
    vec_remove(&v, '_');
    test_assert(v.length == 26);
    vec_remove(&v, 'c');
    test_assert(v.members[0] == 'a' && v.members[1] == 'b' &&
                v.members[2] == 'd' && v.members[3] == 'e');
    test_assert(v.length == 25);
    vec_deinit(&v);
  }

  { test_section("vec_reverse");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 'a');
    vec_push(&v, 'b');
    vec_push(&v, 'c');
    vec_push(&v, 'd');
    vec_reverse(&v);
    test_assert(v.length == 4);
    test_assert(v.members[0] == 'd' && v.members[1] == 'c' &&
                v.members[2] == 'b' && v.members[3] == 'a');
    vec_deinit(&v);
  }

  { test_section("vec_foreach");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 19);
    vec_push(&v, 31);
    vec_push(&v, 47);
    int i, x;
    int count = 0, acc = 1;
    vec_foreach(&v, x, i) {
      acc *= (x + count);
      count++;
    }
    test_assert(acc == (19 + 0) * (31 + 1) * (47 + 2));
    vec_deinit(&v);
  }

  { test_section("vec_foreach_rev");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 19);
    vec_push(&v, 31);
    vec_push(&v, 47);
    int i, x;
    int count = 0, acc = 1;
    vec_foreach_rev(&v, x, i) {
      acc *= (x + count);
      count++;
    }
    test_assert(acc == (19 + 2) * (31 + 1) * (47 + 0));
    vec_deinit(&v);
  }

  { test_section("vec_foreach_ptr");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 19);
    vec_push(&v, 31);
    vec_push(&v, 47);
    int i, *x;
    int count = 0, acc = 1;
    vec_foreach_ptr(&v, x, i) {
      acc *= (*x + count);
      count++;
    }
    test_assert(acc == (19 + 0) * (31 + 1) * (47 + 2));
    vec_deinit(&v);
  }

  { test_section("vec_foreach_ptr_rev");
    vec_int_t v;
    vec_init(&v);
    vec_push(&v, 19);
    vec_push(&v, 31);
    vec_push(&v, 47);
    int i, *x;
    int count = 0, acc = 1;
    vec_foreach_ptr_rev(&v, x, i) {
      acc *= (*x + count);
      count++;
    }
    test_assert(acc == (19 + 2) * (31 + 1) * (47 + 0));
    vec_deinit(&v);
  }

  test_print_res();

  return 0;
}

# vec
A type-safe dynamic array implementation for C. 


## Installation 
The [vec.c](src/vec.c?raw=1) and [vec.h](src/vec.h?raw=1) files can be dropped
into an existing C project and compiled along with it.


## Usage
Before using a vector it should first be initialised using the `vec_init()`
function.
```c
vec_int_t v;
vec_init(&v);
vec_push(&v, 123);
vec_push(&v, 456);
```

To access the elements of the vector directly the vector's `data` field can be
used.
```c
printf("%d\n", v.data[1]); /* Prints the value at index 1 */
```

The current length of the vector is stored in the `length` field of the vector
```c
printf("%d\n", v.length); /* Prints the length of the vector */
```

When you are done with the vector the `vec_deinit()` function should be called
on it. This will free any memory the vector allocated during use.
```c
vec_deinit(&v);
```


## Types
vec.h provides the following predefined vector types:

| Contained Type  | Type name     |
|-----------------|---------------|
| void*           | vec_void_t    |
| char*           | vec_str_t     |
| int             | vec_int_t     |
| char            | vec_char_t    |
| float           | vec_float_t   |
| double          | vec_double_t  |

To define a new vector type the `vec_t()` macro should be used:
```c
/* Creates the type uint_vec_t for storing unsigned ints */
typedef vec_t(unsigned int) uint_vec_t;
```


## Functions
All vector functions are macro functions. The parameter `v` in each function
should be a pointer to the vec struct which the operation is to be performed
on.

### vec\_t(T)
Creates a vec struct for containing values of type `T`.
```c
/* Typedefs the struct `fp_vec_t` as a container for type FILE* */
typedef vec_t(FILE*) fp_vec_t;
```

### vec\_init(v)
Initialises the vector, this must be called before the vector can be used. 

### vec\_deinit(v)
Deinitialises the vector, freeing the memory the vector allocated during use;
this should be called when we're finished with a vector.

### vec\_push(v, val)
Pushes a value to the end of the vector. Returns 0 if the operation was
successful, otherwise -1 is returned and the vector remains unchanged.

### vec\_pop(v)
Removes and returns the value at the end of the vector.

### vec\_splice(v, start, count)
Removes the number of values specified by `count`, starting at the index
`start`.
```c
vec_splice(&v, 2, 4); /* Removes the values at indices 2, 3, 4 and 5 */
```

### vec\_swapsplice(v, start, count)
Removes the number of values specified by `count`, starting at the index
`start`; the removed values are replaced with the last `count` values of the
vector. This does not preserve ordering but is O(1).

### vec\_insert(v, idx, val)
Inserts the value `val` at index `idx` shifting the elements after the index
to make room for the new value.
```c
/* Inserts the value 123 at the beginning of the vec */
vec_insert(&v, 0, 123);
```
Returns 0 if the operation was successful, otherwise -1 is returned and the
vector remains unchanged.

### vec\_sort(v, fn)
Sorts the values of the vector; `fn` should be a qsort-compatible compare
function.

### vec\_swap(v, idx1, idx2)
Swaps the values at the indices `idx1` and `idx2` with one another.

### vec\_truncate(v, len)
Truncates the vector's length to `len`. If `len` is greater than the vector's
current length then no change is made.

### vec\_clear(v)
Clears all values from the vector reducing the vector's length to 0.

### vec\_first(v)
Returns the first value in the vector. This should not be used on an empty
vector.

### vec\_last(v)
Returns the last value in the vector. This should not be used on an empty
vector.

### vec\_reserve(v, n)
Reserves capacity for at least `n` elements in the given vector;  if `n` is
less than the current capacity then `vec_reserve()` does nothing. Returns 0 if
the operation was successful, otherwise -1 is returned and the vector remains
unchanged.

### vec\_compact(v)
Reduces the vector's capacity to the smallest size required to store its
current number of values. Returns 0 if the operation is successful, otherwise
-1 is returned and the vector remains unchanged.

### vec\_pusharr(v, arr, count)
Pushes the contents of the array `arr` to the end of the vector. `count` should
be the number of elements in the array.

### vec\_extend(v, v2)
Appends the contents of the `v2` vector to the `v` vector.

### vec\_find(v, val, idx)
Finds the first occurrence of the value `val` in the vector. `idx` should be an
int where the value's index will be written; `idx` is set to -1 if `val` could
not be found in the vector.

### vec\_remove(v, val)
Removes the first occurrence of the value `val` from the vector. If the `val`
is not contained in the vector then `vec_remove()` does nothing.

### vec\_reverse(v)
Reverses the order of the vector's values in place. For example, a vector
containing `4, 5, 6` would contain `6, 5, 4` after reversing.

### vec\_foreach(v, var, iter)
Iterates the values of the vector from the first to the last. `var` should be a
variable of the vector's contained type where the value will be stored with
each iteration. `iter` should be an int used to store the index during
iteration.
```c
/* Iterates and prints the value and index of each value in the float vec */
int i; float val;
vec_foreach(&v, val, i) {
  printf("%d : %f\n", i, val);
}
```

### vec\_foreach\_rev(v, var, iter)
Iterates the values of the vector from the last to the first. See
`vec_foreach()`

### vec\_foreach\_ptr(v, var, iter)
Iterates the value pointers of the vector from first to last. `var` should be a
variable of the vector's contained type's pointer. See `vec_foreach()`.
```c
/* Iterates and prints the value and index of each value in the float vector */
int i; float *val;
vec_foreach_ptr(&v, val, i) {
  printf("%d : %f\n", i, *val);
}
```

### vec\_foreach\_ptr\_rev(v, var, iter)
Iterates the value pointers of the vector from last to first. See
`vec_foreach_ptr()`


## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.

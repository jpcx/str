# str

## Information

 - The type alias `str` describes a char pointer to the following memory layout:
     ```c
     #include "str.h"

     // typedef char *str;
     str s = str_new("yay"); ----------------------> | returned char *
     | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
     |  size_t capacity = 3  |   size_t length = 3   | y | a | y |\0 |
     | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
     
     str_free(&s);
     ```
  - `str` utilizes a storage mechanic similar to [antirez/sds](https://github.com/antirez/sds).
    A header behind the returned `char *` stores state information.

- All constructors return a `str`
  - All `str` instances must be freed using `str_free(str *)`.

- All manipulators are void and accept a `str *`

- This library uses `malloc` and `free` by default but
  may be customized using `STR_CONFIG_MALLOC` and `STR_CONFIG_FREE`
  ```c
  #define STR_CONFIG_MALLOC my_cool_stack_allocator
  #define STR_CONFIG_FREE   my_cool_stack_deallocator
  #include "str.h"
  ```

- All strings managed by this library are null terminated

-----

- Some manipulator names are trailed by an underscore '_'.
  This is used to label variants of string manipulators
  that accept a const str instead of a `const char*`.

- Any function that accepts a str or a `str *` must only be
  used with str pointers (created by the str constructors).

  __passing a non-str ptr to these functions will read adjacent memory!__

-----

- All symbols are namespaced under `STR_CONFIG_NAMESPACE [default=str]`
    If necessary, define before inclusion; all names will change
    ```c
    #define STR_CONFIG_NAMESPACE string
    #include "str.h"
    ...
    string s = string_new("There!);  // default: str s = str_new("There!");
    string_prepend(s, "Hi ");        // default: str_prepend(s, "Hi ");
    ```

## Errors

- `malloc` errors during construction will return `NULL`, and
                  during manipulation will not modify the string.
- `errno` is not modified by this library; changes are preserved

## Testing

- A simple Makefile is included for testing.
  run `make test` to test the library.

## Usage

- str should be able to be included in any C or C++ project.

- Tested with gcc 10.1.1 and clang 10.0.0 on x64 Linux
  - C89, C90, C99, C11, C18, C2x
  - C++98, C++03, C++11, C++14, C++17, C++20
- Not tested with MSVC, but it should be compatible.

- Please let me know if there are any compatibility issues.

## Illustration

```c
  str s = str_alloc(3);         // [ \0,?,?,? ]               <- alloc
  str_emplace(&s, "bar", 0);    // [ b,a,r,\0 ]
  str_grow(&s, 3);              // [ b,a,r,\0,?,?,? ]         <- alloc
  str_prepend(&s, "foo");       // [ f,o,o,b,a,r,\0 ]
  str_append(&s, "baz!");       // [ f,o,o,b,a,r,b,a,z,!,\0 ] <- alloc

  printf("%s\n", s);            // foobarbaz!
  str_free(&s); // <- free

  ...

  str a = str_new("foo");       // [ f,o,o,\0 ]               <- alloc
  str b = str_dup(a);           // [ f,o,o,\0 ]               <- alloc
  str_emplace(&b, "bar", 0);    // [ b,a,r,\0 ]
  str_append_(&a, b);           // [ f,o,o,b,a,r,\0 ]         <- alloc

  str_free(&a); // <- free
  str_free(&b); // <- free

  ...

  str s = str_new("babble");    // [ b,a,b,b,l,e,\0 ]         <- alloc

  printf("%lu\n", str_len(&a)); // 6
  printf("%lu\n", str_cap(&a)); // 6

  str_grow(&s, 3);              // [ b,a,b,b,l,e,\0,?,?,? ]   <- alloc

  printf("%lu\n", str_len(&a)); // 6
  printf("%lu\n", str_cap(&a)); // 9

  str_shrink(&a, 6);            // [ b,a,b,\0]                <- alloc

  printf("%lu\n", str_len(&a)); // 3
  printf("%lu\n", str_cap(&a)); // 3

  str_free(&s); // <- free
```

## Synopsis

_note: all functions are static inline_

### Construction

```c
str    str_alloc   (size_t cap)                 : create a str with capacity cap
str    str_dup     (const str s)                : duplicate str storage (alloc)
str    str_new     (const char *s)              : record length, allocate, copy
str    str_sub     (const char *s, size_t len)  : copy up to len chars from s
```

### Properties

```c
int    str_avail     (const str s, size_t cap)  : true if capacity is available
size_t str_cap       (const str s)              : retrieve the capacity
char * str_end       (const str s)              : pointer to the null terminator
size_t str_len       (const str s)              : retrieve the length
void * str_mbegin    (str s)                    : ptr to allocated memory begin
void * str_mend      (str s)                    : ptr to allocated memory end
size_t str_msize     (const str s)              : size of allocated memory
str    str_mstr      (void *m)                  : str pointer from membegin
```

### Manipulation

```c
// concatenate //
void   str_append    (str *a, const char *b)    : append to a
void   str_append_   (str *a, const str b)
void   str_prepend   (str *b, const char *a)    : prepend to b
void   str_prepend_  (str *b, const str a)

//  transform  //
void   str_emplace   (str *s, const char *ins,  : overwrite at idx
                      size_t idx)
void   str_emplace_  (str *s, const str ins,
                      size_t idx)
void   str_insert    (str *s, const char *ins,  : insert before s[idx]
                      size_t idx)
void   str_insert_   (str *s, const str ins,
                      size_t idx)

//   format    //
void   str_cpad      (str *s, size_t len)       : center pad to reach len
void   str_lpad      (str *s, size_t len)       : left pad to reach len
void   str_rpad      (str *s, size_t len)       : right pad to reach len
void   str_trim      (str *s)                   : trim leading and trailing
                                                  whitespace [no realloc]

//   manage    //
void   str_clear     (str *s)                   : zero len, term [no realloc]
void   str_fit       (str *s, size_t min_cap)   : resize if capacity < min_cap
void   str_grow      (str *s, size_t delta)     : grow string capacity by delta
void   str_realloc   (str *s, size_t cap)       : resize string [null if needed]
void   str_shrink    (str *s, size_t delta)     : shrink cap [null if needed]
void   str_shrinkfit (str *s)                   : shrink to len if cap > len
```

### Destruction

```c
void   str_free      (str *s)                   : free owned string, nullify ptr
```

## Contribution

Contribution is welcome; please make a pull request.
I'd like to keep this library simple, but please let me know if you have any
useful feature ideas, security fixes, or portable optimizations.

Features are sorted alphabetically by category;
feature order is preserved throughout the project.

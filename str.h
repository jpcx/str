#ifndef STR_STR_H_INCLUDED
#define STR_STR_H_INCLUDED
/* /////////////////////////////////////////////////////////////////////////////
//                ___
//              ,--.'|_            str: C string management header [1.0.0]
//              |  | :,'   __  ,-. Copyright (C) 2020 Justin Collier
//    .--.--.   :  : ' : ,' ,'/ /|
//   /  /    '.;__,'  /  '  | |' | - - - - - - - - - - - - - - - - - - -
//  |  :  /`./|  |   |   |  |   ,'
//  |  :  ;_  :__,'| :   '  :  /   This program is free software: you can
//   \  \    `. '  : |__ |  | '    redistribute it and/or modify it under the
//    `----.   \|  | '.'|;  : |    terms of the GNU General Public License
//   /  /`--'  /;  :    ;|  , ;    as published by the Free Software Foundation,
//  '--'.     / |  ,   /  ---'     either version 3 of the License, or (at your
//    `--'---'   ---`-'            option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the internalied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//                                                                             /
//  You should have received a copy of the GNU General Public License         //
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.   ///
///////////////////////////////////////////////////////////////////////////// */

/*
                                    Synopsis
                                    --------

note: all functions are static inline

 - - -                        ~ ~ construction ~ ~                        - - -

str    str_alloc   (size_t cap)                 : create a str with capacity cap
str    str_dup     (const str s)                : duplicate str storage (alloc)
str    str_new     (const char *s)              : record length, allocate, copy
str    str_sub     (const char *s, size_t len)  : copy up to len chars from s

 - - -                         ~ ~ properties ~ ~                         - - -

int    str_avail     (const str s, size_t cap)  : true if capacity is available
size_t str_cap       (const str s)              : retrieve the capacity
char * str_end       (const str s)              : pointer to the null terminator
size_t str_len       (const str s)              : retrieve the length
void * str_mbegin    (str s)                    : ptr to allocated memory begin
void * str_mend      (str s)                    : ptr to allocated memory end
size_t str_msize     (const str s)              : size of allocated memory
str    str_mstr      (void *m)                  : str pointer from mbegin

 - - -                        ~ ~ manipulation ~ ~                        - - -

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

 - - -                        ~ ~ destruction ~ ~                         - - -

void   str_free      (str *s)                   : free owned string, nullify ptr

*/

#ifdef __cplusplus
extern "C" {
#include <cctype>
#include <cstdlib>
#include <cstring>
#else
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#endif

/*.----------------------------------------------------------------------------,
 /                               detail macros                               */

/*                                   config                                   */

/*                                                     */ /* clang-format off */

#ifndef   STR_CONFIG_NAMESPACE
/** defines the prefix of all defined C symbols [default `str`]
 *  if necessary, `#define STR_CONFIG_NAMESPACE [name]` before inclusion */
#  define STR_CONFIG_NAMESPACE str
#else
#  define STR_DETAIL_USING_CUSTOM_NAMESPACE
#endif

#ifndef   STR_CONFIG_MALLOC
#  define STR_CONFIG_MALLOC malloc
#else
#  define STR_DETAIL_USING_CUSTOM_MALLOC
#endif

#ifndef   STR_CONFIG_FREE
#  define STR_CONFIG_FREE free
#else
#  define STR_DETAIL_USING_CUSTOM_FREE
#endif

/*                                preprocessor                                */

/** Cat. */
#define STR_DETAIL_PP_CAT(a, b)   STR_DETAIL_PP_CAT_X(a, b)
#define STR_DETAIL_PP_CAT_X(a, b) a##b

/*                               project setup                                */

/** refers to a namespaced library function name [eg `str_free`] */
#define STR_DETAIL_NS_FN(name) \
  STR_DETAIL_PP_CAT(STR_CONFIG_NAMESPACE, STR_DETAIL_PP_CAT(_, name))

#define STR_VERSION_MAJOR 1
#define STR_VERSION_MINOR 0
#define STR_VERSION_PATCH 0

/* thanks to cxong/tinydir for this inline definition logic */
#ifdef _MSC_VER
#  define STR_FUNCTION static __inline
#elif !defined __STDC_VERSION__ || __STDC_VERSION__ < 199901L
#  define STR_FUNCTION static __inline__
#else
#  define STR_FUNCTION static inline
#endif

/** temporary definitions for project readability; undefined at end of file */
#ifdef STR_DETAIL_USING_CUSTOM_NAMESPACE
#  define str           STR_CONFIG_NAMESPACE
#  define str_alloc     STR_DETAIL_NS_FN(alloc)
#  define str_dup       STR_DETAIL_NS_FN(dup)
#  define str_new       STR_DETAIL_NS_FN(new)
#  define str_sub       STR_DETAIL_NS_FN(sub)
#  define str_avail     STR_DETAIL_NS_FN(avail)
#  define str_cap       STR_DETAIL_NS_FN(cap)
#  define str_end       STR_DETAIL_NS_FN(end)
#  define str_len       STR_DETAIL_NS_FN(len)
#  define str_mbegin    STR_DETAIL_NS_FN(mbegin)
#  define str_mend      STR_DETAIL_NS_FN(mend)
#  define str_msize     STR_DETAIL_NS_FN(msize)
#  define str_mstr      STR_DETAIL_NS_FN(mstr)
#  define str_append    STR_DETAIL_NS_FN(append)
#  define str_append_   STR_DETAIL_NS_FN(append_)
#  define str_prepend   STR_DETAIL_NS_FN(prepend)
#  define str_prepend_  STR_DETAIL_NS_FN(prepend_)
#  define str_emplace   STR_DETAIL_NS_FN(emplace)
#  define str_emplace_  STR_DETAIL_NS_FN(emplace_)
#  define str_insert    STR_DETAIL_NS_FN(insert)
#  define str_insert_   STR_DETAIL_NS_FN(insert_)
#  define str_cpad      STR_DETAIL_NS_FN(cpad)
#  define str_lpad      STR_DETAIL_NS_FN(lpad)
#  define str_rpad      STR_DETAIL_NS_FN(rpad)
#  define str_trim      STR_DETAIL_NS_FN(trim)
#  define str_clear     STR_DETAIL_NS_FN(clear)
#  define str_fit       STR_DETAIL_NS_FN(fit)
#  define str_grow      STR_DETAIL_NS_FN(grow)
#  define str_realloc   STR_DETAIL_NS_FN(realloc)
#  define str_shrink    STR_DETAIL_NS_FN(shrink)
#  define str_shrinkfit STR_DETAIL_NS_FN(shrinkfit)
#  define str_free      STR_DETAIL_NS_FN(free)
#endif

/*                                                     */ /* clang-format on  */

/*                           implementation helpers                           */

/** defines the size of the memory block given a capacity */
#define STR_DETAIL_MEMORY_SIZE(cap) \
  sizeof(size_t) * 2 + sizeof(char) * (cap + 1)

/** shifts a char* to the left by n */
#define STR_DETAIL_SHIFT_LEFT(cstr, len, n) \
  {                                         \
    size_t i_;                              \
    for (i_ = 0; i_ < len; ++i_)            \
      (cstr)[i_ - n] = (cstr)[i_];          \
  }

/** shifts a char* to the right by n */
#define STR_DETAIL_SHIFT_RIGHT(cstr, len, n) \
  {                                          \
    size_t i_;                               \
    for (i_ = len + 1; i_ > 0; --i_)         \
      (cstr)[(i_ - 1) + n] = (cstr)[i_ - 1]; \
  }

/** assigns len to its memory location */
#define STR_DETAIL_SET_LEN(str, len) *(((size_t *)(str)) - 1) = len

/** assigns cap to its memory location */
#define STR_DETAIL_SET_CAP(str, cap) *(((size_t *)(str)) - 2) = cap

/*.----------------------------------------------------------------------------,
 /                                 type alias                                */

typedef char *str;

/*.----------------------------------------------------------------------------,
 /                                declarations                               */

/*                                construction                                */

/** create a str with capacity cap */
STR_FUNCTION str
str_alloc(size_t cap);
/** duplicate str storage (alloc) */
STR_FUNCTION str
str_dup(const str s);
/** record length, allocate, copy */
STR_FUNCTION str
str_new(const char *s);
/** copy up to len chars from s */
STR_FUNCTION str
str_sub(const char *s, size_t len);

/*                                 properties                                 */

/** true if capacity is available */
STR_FUNCTION int
str_avail(const str s, size_t cap);
/** retrieve the capacity */
STR_FUNCTION size_t
str_cap(const str s);
/** pointer to the null terminator */
STR_FUNCTION char *
str_end(const str s);
/** retrieve the length */
STR_FUNCTION size_t
str_len(const str s);
/** ptr to allocated memory begin */
STR_FUNCTION void *
str_mbegin(str s);
/** ptr to allocated memory end */
STR_FUNCTION void *
str_mend(str s);
/** size of allocated memory */
STR_FUNCTION size_t
str_msize(const str s);
/** str pointer from mbegin */
STR_FUNCTION str
str_mstr(void *m);

/*                                manipulation                                */

/** append chars to a */
STR_FUNCTION void
str_append(str *a, const char *b);
/** append str to a */
STR_FUNCTION void
str_append_(str *a, const str b);
/** prepend chars to b */
STR_FUNCTION void
str_prepend(str *b, const char *a);
/** prepend str to b */
STR_FUNCTION void
str_prepend_(str *b, const str a);

/** overwrite chars in a string */
STR_FUNCTION void
str_emplace(str *a, const char *b, size_t idx);
/** overwrite chars in a string */
STR_FUNCTION void
str_emplace_(str *a, const str b, size_t idx);
/** insert chars before s[idx] */
STR_FUNCTION void
str_insert(str *s, const char *ins, size_t idx);
/** insert str before s[idx] */
STR_FUNCTION void
str_insert_(str *s, const str ins, size_t idx);

/** center pad to reach len */
STR_FUNCTION void
str_cpad(str *s, size_t len);
/** left pad to reach len */
STR_FUNCTION void
str_lpad(str *s, size_t len);
/** right pad to reach len */
STR_FUNCTION void
str_rpad(str *s, size_t len);
/** trim leading and trailing whitespace */
STR_FUNCTION void
str_trim(str *s);

/** zero len, term [no realloc] */
STR_FUNCTION void
str_clear(str *a);
/** resize if capacity < min_cap */
STR_FUNCTION void
str_fit(str *a, size_t min_cap);
/** grow string capacity by delta */
STR_FUNCTION void
str_grow(str *a, size_t delta);
/** resize string [null if needed] */
STR_FUNCTION void
str_realloc(str *s, size_t cap);
/** shrink cap [null if needed] */
STR_FUNCTION void
str_shrink(str *a, size_t delta);
/** resize to len */
STR_FUNCTION void
str_shrinkfit(str *s);

/*                                destruction                                 */

/** free owned string, nullify ptr */
STR_FUNCTION void
str_free(str *s);

/*.----------------------------------------------------------------------------,
 /                                definitions                                */

/*                                construction                                */

/** create a str with capacity cap */
STR_FUNCTION str
str_alloc(size_t cap) {
  void *o = STR_CONFIG_MALLOC(STR_DETAIL_MEMORY_SIZE(cap));
  if (o == NULL)
    return NULL;
  str s = str_mstr(o);
  STR_DETAIL_SET_CAP(s, cap);
  STR_DETAIL_SET_LEN(s, 0);
  s[0] = '\0';
  return s;
}

/** duplicate str storage (alloc) */
STR_FUNCTION str
str_dup(const str s) {
  void *o = STR_CONFIG_MALLOC(str_msize(s));
  if (o == NULL)
    return NULL;
  memcpy(o, str_mbegin(s), str_msize(s));
  return str_mstr(o);
}

/** record length, allocate, copy */
STR_FUNCTION str
str_new(const char *s) {
  size_t len = strlen(s);
  str v      = str_alloc(len);
  if (v == NULL)
    return NULL;
  strcpy(v, s);
  STR_DETAIL_SET_LEN(v, len);
  return v;
}

/** copy up to len chars from s */
STR_FUNCTION str
str_sub(const char *s, size_t len) {
  str v = str_alloc(len);
  if (v == NULL)
    return NULL;
  size_t i;
  for (i = 0; i < len && s[i] != '\0'; ++i)
    v[i] = s[i];
  v[i] = '\0';
  STR_DETAIL_SET_LEN(v, i);
  return v;
}

/*                                 properties                                 */

/** true if capacity is available */
STR_FUNCTION int
str_avail(const str s, size_t cap) {
  return str_cap(s) >= cap;
}

/** retrieve the capacity */
STR_FUNCTION size_t
str_cap(const str s) {
  return *(((size_t *)s) - 2);
}

/** pointer to the null terminator */
STR_FUNCTION char *
str_end(const str s) {
  return &s[str_len(s)];
}

/** retrieve the length */
STR_FUNCTION size_t
str_len(const str s) {
  return *(((size_t *)s) - 1);
}

/** ptr to allocated memory begin */
STR_FUNCTION void *
str_mbegin(const str s) {
  return (size_t *)s - 2;
}

/** ptr to allocated memory end */
STR_FUNCTION void *
str_mend(const str s) {
  return s + str_cap(s);
}

/** size of allocated memory */
STR_FUNCTION size_t
str_msize(const str s) {
  return STR_DETAIL_MEMORY_SIZE(str_cap(s));
}

/** str pointer from mbegin */
STR_FUNCTION str
str_mstr(void *m) {
  return (str)((size_t *)(m) + 2);
}

/*                                manipulation                                */

/** append chars to a */
STR_FUNCTION void
str_append(str *a, const char *b) {
  size_t alen = str_len(*a);
  size_t blen = strlen(b);
  str_fit(a, alen + blen);
  strcpy(&(*a)[alen], b);
  STR_DETAIL_SET_LEN(*a, alen + blen);
}

/** append str to a */
STR_FUNCTION void
str_append_(str *a, const str b) {
  size_t alen = str_len(*a);
  size_t blen = str_len(b);
  str_fit(a, alen + blen);
  strcpy(&(*a)[alen], b);
  STR_DETAIL_SET_LEN(*a, alen + blen);
}

/** prepend chars to b */
STR_FUNCTION void
str_prepend(str *b, const char *a) {
  size_t blen = str_len(*b);
  size_t alen = strlen(a);
  str_fit(b, alen + blen);
  STR_DETAIL_SHIFT_RIGHT(*b, blen, alen);
  memcpy((*b), a, alen);
  STR_DETAIL_SET_LEN(*b, alen + blen);
}

/** prepend str to b */
STR_FUNCTION void
str_prepend_(str *b, const str a) {
  size_t blen = str_len(*b);
  size_t alen = str_len(a);
  str_fit(b, alen + blen);
  STR_DETAIL_SHIFT_RIGHT(*b, blen, alen);
  memcpy((*b), a, alen);
  STR_DETAIL_SET_LEN(*b, alen + blen);
}

/** overwrite chars in a string */
STR_FUNCTION void
str_emplace(str *s, const char *ins, size_t idx) {
  size_t inslen = strlen(ins);
  str_fit(s, idx + inslen);
  memcpy(&(*s)[idx], ins, inslen);
  if (idx + inslen > str_len(*s)) {
    (*s)[idx + inslen] = '\0';
    STR_DETAIL_SET_LEN(*s, idx + inslen);
  }
}

/** overwrite chars in a string */
STR_FUNCTION void
str_emplace_(str *s, const str ins, size_t idx) {
  size_t inslen = str_len(ins);
  str_fit(s, idx + inslen);
  memcpy(&(*s)[idx], ins, inslen);
  if (idx + inslen > str_len(*s)) {
    (*s)[idx + inslen] = '\0';
    STR_DETAIL_SET_LEN(*s, idx + inslen);
  }
}

/** insert chars before s[idx] */
STR_FUNCTION void
str_insert(str *s, const char *ins, size_t idx) {
  size_t slen   = str_len(*s);
  size_t inslen = strlen(ins);
  str_fit(s, slen + inslen);
  STR_DETAIL_SHIFT_RIGHT(&(*s)[idx], slen - idx, inslen);
  memcpy(&(*s)[idx], ins, inslen);
  STR_DETAIL_SET_LEN(*s, slen + inslen);
}

/** insert str before s[idx] */
STR_FUNCTION void
str_insert_(str *s, const str ins, size_t idx) {
  size_t slen   = str_len(*s);
  size_t inslen = str_len(ins);
  str_fit(s, slen + inslen);
  STR_DETAIL_SHIFT_RIGHT(&(*s)[idx], slen - idx, inslen);
  memcpy(&(*s)[idx], ins, inslen);
  STR_DETAIL_SET_LEN(*s, slen + inslen);
}

/** center pad to reach len */
STR_FUNCTION void
str_cpad(str *s, size_t len) {
  size_t slen = str_len(*s);
  if (slen < len) {
    str_fit(s, len);
    size_t mid = (len - slen) / 2;
    STR_DETAIL_SHIFT_RIGHT(*s, slen, mid);
    memset(*s, ' ', mid);
    memset(&(*s)[mid + slen], ' ', len - mid);
    (*s)[len] = '\0';
    STR_DETAIL_SET_LEN(*s, len);
  }
}

/** left pad to reach len */
STR_FUNCTION void
str_lpad(str *s, size_t len) {
  size_t slen = str_len(*s);
  if (slen < len) {
    str_fit(s, len);
    STR_DETAIL_SHIFT_RIGHT(*s, slen, len - slen);
    memset(*s, ' ', len - slen);
    STR_DETAIL_SET_LEN(*s, len);
  }
}

/** right pad to reach len */
STR_FUNCTION void
str_rpad(str *s, size_t len) {
  size_t slen = str_len(*s);
  if (slen < len) {
    str_fit(s, len);
    memset(&(*s)[slen], ' ', len - slen);
    (*s)[len] = '\0';
    STR_DETAIL_SET_LEN(*s, len);
  }
}

/** trim leading and trailing whitespace */
STR_FUNCTION void
str_trim(str *s) {
  size_t slen = str_len(*s);
  size_t beg;
  size_t end;
  for (beg = 0; beg < slen && isspace((*s)[beg]); ++beg)
    ;
  for (end = slen; end > 0 && isspace((*s)[end - 1]); --end)
    ;

  if (beg > 0)
    STR_DETAIL_SHIFT_LEFT(&(*s)[beg], end - beg, beg);

  if (beg > 0 || end < slen) {
    (*s)[end - beg] = '\0';
    STR_DETAIL_SET_LEN(*s, end - beg);
  }
}

/** zero len, term [no realloc] */
STR_FUNCTION void
str_clear(str *s) {
  (*s)[0] = '\0';
  STR_DETAIL_SET_LEN(*s, 0);
}

/** resize if capacity < min_cap */
STR_FUNCTION void
str_fit(str *a, size_t min_cap) {
  if (str_cap(*a) < min_cap)
    str_realloc(a, min_cap);
}

/** grow string capacity by delta */
STR_FUNCTION void
str_grow(str *s, size_t delta) {
  str_realloc(s, str_cap(*s) + delta);
}

/** resize string [reallocates and null terminates] */
STR_FUNCTION void
str_realloc(str *s, size_t cap) {
  void *v = STR_CONFIG_MALLOC(STR_DETAIL_MEMORY_SIZE(cap));

  if (v == NULL)
    return;

  memcpy(v, str_mbegin(*s), STR_DETAIL_MEMORY_SIZE(cap));
  str_free(s);

  *s = str_mstr(v);
  STR_DETAIL_SET_CAP(*s, cap);

  if (cap < str_len(*s)) {
    (*s)[cap] = '\0';
    STR_DETAIL_SET_LEN(*s, cap);
  }
}

/** shrink cap [null if needed] */
STR_FUNCTION void
str_shrink(str *s, size_t delta) {
  str_realloc(s, str_cap(*s) - delta);
}

/** shrink to len if cap > len */
STR_FUNCTION void
str_shrinkfit(str *s) {
  size_t slen = str_len(*s);
  if (slen < str_cap(*s))
    str_realloc(s, slen);
}

/*                                destruction                                 */

/** free owned string, nullify ptr */
STR_FUNCTION void
str_free(str *s) {
  STR_CONFIG_FREE(str_mbegin(*s));
  *s = NULL;
}

/*                                                     */ /* clang-format off */

#undef STR_DETAIL_PP_CAT
#undef STR_DETAIL_PP_CAT_X
#undef STR_DETAIL_NS_FN
#undef STR_FUNCTION

#ifdef STR_DETAIL_USING_CUSTOM_NAMESPACE
#  undef str
#  undef str_alloc
#  undef str_dup
#  undef str_new
#  undef str_sub
#  undef str_avail
#  undef str_cap
#  undef str_end
#  undef str_len
#  undef str_mbegin
#  undef str_mend
#  undef str_msize
#  undef str_mstr
#  undef str_append
#  undef str_append_
#  undef str_prepend
#  undef str_prepend_
#  undef str_emplace
#  undef str_emplace_
#  undef str_insert
#  undef str_insert_
#  undef str_cpad
#  undef str_lpad
#  undef str_rpad
#  undef str_trim
#  undef str_clear
#  undef str_fit
#  undef str_grow
#  undef str_realloc
#  undef str_shrink
#  undef str_shrinkfit
#  undef str_free
#endif

#ifdef   STR_DETAIL_USING_CUSTOM_NAMESPACE
#  undef STR_DETAIL_USING_CUSTOM_NAMESPACE
#else
#  undef STR_CONFIG_NAMESPACE
#endif

#ifdef   STR_DETAIL_USING_CUSTOM_MALLOC
#  undef STR_DETAIL_USING_CUSTOM_MALLOC
#else
#  undef STR_CONFIG_MALLOC
#endif

#ifdef   STR_DETAIL_USING_CUSTOM_FREE
#  undef STR_DETAIL_USING_CUSTOM_FREE
#else
#  undef STR_CONFIG_FREE
#endif

#undef STR_DETAIL_MEMORY_SIZE
#undef STR_DETAIL_SHIFT_RIGHT
#undef STR_DETAIL_SHIFT_LEFT
#undef STR_DETAIL_SET_LEN
#undef STR_DETAIL_SET_CAP
/*                                                     */ /* clang-format on  */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

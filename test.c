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

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef IS_NAMESPACE_TEST
#define STR_CONFIG_NAMESPACE xyz
#define TEST_REPORT_NS       xyz
#else
#define TEST_REPORT_NS str
#endif

/*                                                     */ /* clang-format off */
/* allocation test detail */
#ifdef IS_ALLOCATION_TEST
static size_t last_alloc_sz  = SIZE_MAX;
static void  *last_alloc_ptr = NULL;
static void  *last_freed_ptr = NULL;
static void  *tracked_ptr    = NULL;
void *track_alloc(size_t n) {
  last_alloc_sz  = n;
  last_alloc_ptr = malloc(n);
  return last_alloc_ptr;
}
void  track_free(void *p)   { last_freed_ptr = p;  free(p); }
#define STR_CONFIG_MALLOC track_alloc
#define STR_CONFIG_FREE   track_free
#define RESET_TRACKING    last_alloc_sz  = SIZE_MAX; \
                          last_alloc_ptr = NULL;     \
                          last_freed_ptr = NULL;     \
                          tracked_ptr    = NULL
#define ASSERT_ALLOC(cap, str)                        \
    assert(last_alloc_sz ==                           \
    (sizeof(size_t) * 2 + sizeof(char) * ((cap) + 1)) \
    && last_alloc_ptr == str_mbegin(str)              \
    && last_alloc_ptr != NULL)
#define ASSERT_NO_ALLOC   assert(last_alloc_sz     == SIZE_MAX \
                                 && last_alloc_ptr == NULL)
/* note- must use TRACK_STR or assign to tracked_ptr */
#define ASSERT_FREE       assert(                         \
                            last_freed_ptr == tracked_ptr \
                            && last_freed_ptr != NULL)
#define ASSERT_NO_FREE    assert(last_freed_ptr == NULL)
#define TRACK_STR(str)    tracked_ptr = str_mbegin(str)
#else
/* dummy macros for non-allocation testing */
#define RESET_TRACKING
#define ASSERT_ALLOC(sz, str)
#define ASSERT_NO_ALLOC  
#define ASSERT_FREE
#define ASSERT_NO_FREE
#define TRACK_STR(str)
#endif
/*                                                     */ /* clang-format on  */

#include "str.h"

/*.----------------------------------------------------------------------------,
 /                                test detail                                */

#define PP_CAT(a, b)        PP_CAT_X(a, b)
#define PP_CAT_X(a, b)      a##b
#define PP_STRINGIZE(...)   PP_STRINGIZE_X(__VA_ARGS__)
#define PP_STRINGIZE_X(...) #__VA_ARGS__

#define TEST(feature)      static void PP_CAT(test_, feature)()
#define ASSERT_STREQ(a, b) assert(strcmp(a, b) == 0)
#define ASSERT_EQ(a, b)    assert(a == b)
#define ASSERT_NEQ(a, b)   assert(a != b)
#define ASSERT_TRUE(c)     assert(c == 1)
#define ASSERT_FALSE(c)    assert(c == 0)

#ifndef IS_ALLOCATION_TEST
#define RUN_TEST(feature)   \
  PP_CAT(test_, feature)(); \
  printf("PASS " PP_STRINGIZE(TEST_REPORT_NS) "_" #feature "\n")
#else
#define RUN_TEST(feature)   \
  PP_CAT(test_, feature)(); \
  printf("PASS ALLOCATION " PP_STRINGIZE(TEST_REPORT_NS) "_" #feature "\n")
#endif
#define TEST_MAIN int main(void)

#define NS_FN(name) PP_CAT(STR_CONFIG_NAMESPACE, PP_CAT(_, name))

#define ASSERT_STR_PROPS(str, streq, cap) \
  ASSERT_STREQ(str, streq);               \
  ASSERT_EQ(str_cap(str), cap);           \
  ASSERT_EQ(str_len(str), strlen(streq))

/* simplify namespace testing */
#ifdef IS_NAMESPACE_TEST
#define str           STR_CONFIG_NAMESPACE
#define str_alloc     NS_FN(alloc)
#define str_dup       NS_FN(dup)
#define str_new       NS_FN(new)
#define str_sub       NS_FN(sub)
#define str_avail     NS_FN(avail)
#define str_cap       NS_FN(cap)
#define str_end       NS_FN(end)
#define str_len       NS_FN(len)
#define str_mbegin    NS_FN(mbegin)
#define str_mend      NS_FN(mend)
#define str_msize     NS_FN(msize)
#define str_mstr      NS_FN(mstr)
#define str_append    NS_FN(append)
#define str_append_   NS_FN(append_)
#define str_prepend   NS_FN(prepend)
#define str_prepend_  NS_FN(prepend_)
#define str_emplace   NS_FN(emplace)
#define str_emplace_  NS_FN(emplace_)
#define str_insert    NS_FN(insert)
#define str_insert_   NS_FN(insert_)
#define str_cpad      NS_FN(cpad)
#define str_lpad      NS_FN(lpad)
#define str_rpad      NS_FN(rpad)
#define str_trim      NS_FN(trim)
#define str_clear     NS_FN(clear)
#define str_fit       NS_FN(fit)
#define str_grow      NS_FN(grow)
#define str_realloc   NS_FN(realloc)
#define str_shrink    NS_FN(shrink)
#define str_shrinkfit NS_FN(shrinkfit)
#define str_free      NS_FN(free)

#endif

/*.----------------------------------------------------------------------------,
 /                                   tests                                   */

/*  Feature Testing                                     Allocation Testing    */
/*  ---------------                                     ------------------    */

TEST(alloc) {
  {
    /*                                                 */ RESET_TRACKING;
    str s = str_alloc(3);
    ASSERT_EQ(s[0], '\0');
    ASSERT_STR_PROPS(s, "", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
  {
    /*                                                 */ RESET_TRACKING;
    str s = str_alloc(0);
    ASSERT_EQ(s[0], '\0');
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
}

TEST(dup) {
  {
    str s = str_new("foobar");
    /*                                                 */ RESET_TRACKING;
    str d = str_dup(s);
    ASSERT_NEQ(s, d); /* assert new pointer */
    ASSERT_EQ(d[6], '\0');
    ASSERT_STR_PROPS(s, d, str_cap(d));
    /*                                                 */ ASSERT_ALLOC(6, d);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
    str_free(&d);
  }
  {
    str s = str_new("");
    /*                                                 */ RESET_TRACKING;
    str d = str_dup(s);
    ASSERT_NEQ(s, d); /* assert new pointer */
    ASSERT_EQ(d[0], '\0');
    ASSERT_STR_PROPS(s, d, str_cap(d));
    /*                                                 */ ASSERT_ALLOC(0, d);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
    str_free(&d);
  }
}

TEST(new) {
  {
    /*                                                 */ RESET_TRACKING;
    str s = str_new("foo");
    ASSERT_EQ(s[3], '\0');
    ASSERT_STR_PROPS(s, "foo", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
  {
    /*                                                 */ RESET_TRACKING;
    str s = str_new("");
    ASSERT_EQ(s[0], '\0');
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
}

TEST(sub) {
  {
    const char *text = "invest in education";
    /*                                                 */ RESET_TRACKING;
    str inv = str_sub(text, 6);
    ASSERT_STR_PROPS(inv, "invest", 6);
    ASSERT_NEQ(inv, &text[0]);
    /*                                                 */ ASSERT_ALLOC(6, inv);
    /*                                                 */ ASSERT_NO_FREE;
    /*                                                 */ RESET_TRACKING;
    str in = str_sub(&text[7], 2);
    ASSERT_STR_PROPS(in, "in", 2);
    ASSERT_NEQ(in, &text[7]);
    /*                                                 */ ASSERT_ALLOC(2, in);
    /*                                                 */ ASSERT_NO_FREE;
    /*                                                 */ RESET_TRACKING;
    str edu = str_sub(&text[10], 9);
    ASSERT_STR_PROPS(edu, "education", 9);
    ASSERT_NEQ(in, &text[10]);
    /*                                                 */ ASSERT_ALLOC(9, edu);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&inv);
    str_free(&in);
    str_free(&edu);
  }
  { /* null-terminated early stop */
    const char *foo = ":)";
    /*                                                 */ RESET_TRACKING;
    str s = str_sub(foo, 10);
    ASSERT_STR_PROPS(s, ":)", 10);
    /*                                                 */ ASSERT_ALLOC(10, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
  { /* empty string */
    const char *foo = "";
    /*                                                 */ RESET_TRACKING;
    str s = str_sub(foo, 10);
    ASSERT_STR_PROPS(s, "", 10);
    /*                                                 */ ASSERT_ALLOC(10, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
  { /* empty sub */
    const char *foo = "hi";
    /*                                                 */ RESET_TRACKING;
    str s = str_sub(foo, 0);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_NO_FREE;
    str_free(&s);
  }
}

TEST(avail) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    ASSERT_TRUE(str_avail(s, 0));
    ASSERT_FALSE(str_avail(s, 1));
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_grow(&s, 1);
    /*                                                 */ RESET_TRACKING;
    ASSERT_TRUE(str_avail(s, 0));
    ASSERT_TRUE(str_avail(s, 1));
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(cap) {
  str s = str_new("foo");
  {
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_cap(s), 3);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_realloc(&s, 4);
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_cap(s), 4);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(end) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_end(s), s);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_emplace(&s, "foo", 0);
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_end(s), s + 3);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(len) {
  str s = str_new("foo");
  {
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_len(s), 3);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(mbegin) {
  str s = str_alloc(0);
  {
    void *m = (size_t *)s - 2;
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_mbegin(s), m);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_realloc(&s, 4);
    m = (size_t *)s - 2;
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_mbegin(s), m);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(mend) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_mend(s), (void *)s);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_realloc(&s, 6);
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_mend(s), (void *)(s + 6));
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(msize) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_msize(s), sizeof(size_t) * 2 + sizeof(char));
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_realloc(&s, 6);
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_msize(s), sizeof(size_t) * 2 + sizeof(char) * 7);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(mstr) {
  str s = str_alloc(0);
  {
    void *m = (size_t *)s - 2;
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_mstr(m), s);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_realloc(&s, 6);
    m = (size_t *)s - 2;
    /*                                                 */ RESET_TRACKING;
    ASSERT_EQ(str_mstr(m), s);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

#define STR_APPEND_TEST(str_append_fn, foo, bar, baz, isms, blank)            \
  str s = str_alloc(0);                                                       \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_append_fn(&s, foo);                                                     \
  ASSERT_STR_PROPS(s, "foo", 3);                                              \
  /*                                                   */ ASSERT_ALLOC(3, s); \
  /*                                                   */ ASSERT_FREE;        \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_append_fn(&s, bar);                                                     \
  ASSERT_STR_PROPS(s, "foobar", 6);                                           \
  /*                                                   */ ASSERT_ALLOC(6, s); \
  /*                                                   */ ASSERT_FREE;        \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_append_fn(&s, baz);                                                     \
  ASSERT_STR_PROPS(s, "foobarbaz", 9);                                        \
  /*                                                   */ ASSERT_ALLOC(9, s); \
  /*                                                   */ ASSERT_FREE;        \
  str_realloc(&s, 13);                                                        \
  /*                                                   */ RESET_TRACKING;     \
  str_append_fn(&s, isms);                                                    \
  ASSERT_STR_PROPS(s, "foobarbazisms", 13);                                   \
  str_append_fn(&s, blank);                                                   \
  ASSERT_STR_PROPS(s, "foobarbazisms", 13);                                   \
  /*                                                   */ ASSERT_NO_ALLOC;    \
  /*                                                   */ ASSERT_NO_FREE;     \
  str_free(&s)

TEST(append) {
  STR_APPEND_TEST(str_append, "foo", "bar", "baz", "isms", "");
}

TEST(append_) {
  str foo   = str_new("foo");
  str bar   = str_new("bar");
  str baz   = str_new("baz");
  str isms  = str_new("isms");
  str blank = str_new("");

  STR_APPEND_TEST(str_append_, foo, bar, baz, isms, blank);

  str_free(&foo);
  str_free(&bar);
  str_free(&baz);
  str_free(&isms);
  str_free(&blank);
}

#define STR_PREPEND_TEST(str_prepend_fn, bar, foo, more, blank)               \
  str s = str_alloc(0);                                                       \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_prepend_fn(&s, bar);                                                    \
  ASSERT_STR_PROPS(s, "bar", 3);                                              \
  /*                                                   */ ASSERT_ALLOC(3, s); \
  /*                                                   */ ASSERT_FREE;        \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_prepend_fn(&s, foo);                                                    \
  ASSERT_STR_PROPS(s, "foobar", 6);                                           \
  /*                                                   */ ASSERT_ALLOC(6, s); \
  /*                                                   */ ASSERT_FREE;        \
  str_realloc(&s, 11);                                                        \
  /*                                                   */ RESET_TRACKING;     \
  str_prepend_fn(&s, more);                                                   \
  ASSERT_STR_PROPS(s, "more foobar", 11);                                     \
                                                                              \
  str_prepend_fn(&s, blank);                                                  \
  ASSERT_STR_PROPS(s, "more foobar", 11);                                     \
  /*                                                   */ ASSERT_NO_ALLOC;    \
  /*                                                   */ ASSERT_NO_FREE;     \
  str_free(&s)

TEST(prepend) {
  STR_PREPEND_TEST(str_prepend, "bar", "foo", "more ", "");
}

TEST(prepend_) {
  str bar   = str_new("bar");
  str foo   = str_new("foo");
  str more  = str_new("more ");
  str blank = str_new("");

  STR_PREPEND_TEST(str_prepend_, bar, foo, more, blank);

  str_free(&bar);
  str_free(&foo);
  str_free(&more);
  str_free(&blank);
}

#define STR_EMPLACE_TEST(str_emplace_fn, few, foo, do_, bar, blank)           \
  str s = str_alloc(0);                                                       \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_emplace_fn(&s, few, 0);                                                 \
  ASSERT_STR_PROPS(s, "few", 3);                                              \
  /*                                                   */ ASSERT_ALLOC(3, s); \
  /*                                                   */ ASSERT_FREE;        \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  str_emplace_fn(&s, foo, 0);                                                 \
  ASSERT_STR_PROPS(s, "foo", 3);                                              \
  /*                                                   */ ASSERT_NO_ALLOC;    \
  /*                                                   */ ASSERT_NO_FREE;     \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_emplace_fn(&s, do_, 3);                                                 \
  ASSERT_STR_PROPS(s, "foodo", 5);                                            \
  /*                                                   */ ASSERT_ALLOC(5, s); \
  /*                                                   */ ASSERT_FREE;        \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  /*                                                   */ TRACK_STR(s);       \
  str_emplace_fn(&s, bar, 4);                                                 \
  ASSERT_STR_PROPS(s, "foodbar", 7);                                          \
  /*                                                   */ ASSERT_ALLOC(7, s); \
  /*                                                   */ ASSERT_FREE;        \
                                                                              \
  /*                                                   */ RESET_TRACKING;     \
  str_emplace_fn(&s, blank, 0);                                               \
  ASSERT_STR_PROPS(s, "foodbar", 7);                                          \
  /*                                                   */ ASSERT_NO_ALLOC;    \
  /*                                                   */ ASSERT_NO_FREE;     \
  str_free(&s)

TEST(emplace) {
  STR_EMPLACE_TEST(str_emplace, "few", "foo", "do", "bar", "");
}

TEST(emplace_) {
  str few   = str_new("few");
  str foo   = str_new("foo");
  str do_   = str_new("do");
  str bar   = str_new("bar");
  str blank = str_new("");

  STR_EMPLACE_TEST(str_emplace_, few, foo, do_, bar, blank);

  str_free(&few);
  str_free(&foo);
  str_free(&do_);
  str_free(&bar);
  str_free(&blank);
}

#define STR_INSERT_TEST(str_insert_fn, sentence, this, is, a, blank)           \
  str s = str_alloc(0);                                                        \
  /*                                                   */ RESET_TRACKING;      \
  /*                                                   */ TRACK_STR(s);        \
  str_insert_fn(&s, sentence, 0);                                              \
  ASSERT_STR_PROPS(s, "sentence", 8);                                          \
  /*                                                   */ ASSERT_ALLOC(8, s);  \
  /*                                                   */ ASSERT_FREE;         \
                                                                               \
  /*                                                   */ RESET_TRACKING;      \
  /*                                                   */ TRACK_STR(s);        \
  str_insert_fn(&s, this, 0);                                                  \
  ASSERT_STR_PROPS(s, "thissentence", 12);                                     \
  /*                                                   */ ASSERT_ALLOC(12, s); \
  /*                                                   */ ASSERT_FREE;         \
  str_realloc(&s, 16);                                                         \
  /*                                                   */ RESET_TRACKING;      \
  str_insert_fn(&s, is, 4);                                                    \
  /*                                                   */ ASSERT_NO_ALLOC;     \
  /*                                                   */ ASSERT_NO_FREE;      \
  ASSERT_STR_PROPS(s, "this is sentence", 16);                                 \
  /*                                                   */ RESET_TRACKING;      \
  /*                                                   */ TRACK_STR(s);        \
  str_insert_fn(&s, a, 8);                                                     \
  /*                                                   */ ASSERT_ALLOC(18, s); \
  /*                                                   */ ASSERT_FREE;         \
  ASSERT_STR_PROPS(s, "this is a sentence", 18);                               \
  /*                                                   */ RESET_TRACKING;      \
  str_insert_fn(&s, blank, 0);                                                 \
  ASSERT_STR_PROPS(s, "this is a sentence", 18);                               \
  /*                                                   */ ASSERT_NO_ALLOC;     \
  /*                                                   */ ASSERT_NO_FREE;      \
  str_free(&s)

TEST(insert) {
  STR_INSERT_TEST(str_insert, "sentence", "this", " is ", "a ", "");
}

TEST(insert_) {
  str sentence = str_new("sentence");
  str this_    = str_new("this");
  str is       = str_new(" is ");
  str a        = str_new("a ");
  str blank    = str_new("");

  STR_INSERT_TEST(str_insert_, sentence, this_, is, a, blank);

  str_free(&sentence);
  str_free(&this_);
  str_free(&is);
  str_free(&a);
  str_free(&blank);
}

TEST(cpad) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_cpad(&s, 3);
    ASSERT_STR_PROPS(s, "   ", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_FREE;
    str_emplace(&s, "<--->", 0);
    str_realloc(&s, 11);
    /*                                                 */ RESET_TRACKING;
    str_cpad(&s, 11);
    ASSERT_STR_PROPS(s, "   <--->   ", 11);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;

    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_cpad(&s, 14);
    ASSERT_STR_PROPS(s, "    <--->     ", 14);
    /*                                                 */ ASSERT_ALLOC(14, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(lpad) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_lpad(&s, 3);
    ASSERT_STR_PROPS(s, "   ", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_FREE;
    str_append(&s, "<----");
    str_realloc(&s, 11);
    /*                                                 */ RESET_TRACKING;
    str_lpad(&s, 11);
    ASSERT_STR_PROPS(s, "      <----", 11);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;

    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_lpad(&s, 14);
    ASSERT_STR_PROPS(s, "         <----", 14);
    /*                                                 */ ASSERT_ALLOC(14, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(rpad) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_rpad(&s, 3);
    ASSERT_STR_PROPS(s, "   ", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_FREE;
    str_prepend(&s, "---->");
    str_realloc(&s, 11);
    /*                                                 */ RESET_TRACKING;
    str_rpad(&s, 11);
    ASSERT_STR_PROPS(s, "---->      ", 11);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;

    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_rpad(&s, 14);
    ASSERT_STR_PROPS(s, "---->         ", 14);
    /*                                                 */ ASSERT_ALLOC(14, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(trim) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    str_trim(&s);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_append(&s, ":)");
    /*                                                 */ RESET_TRACKING;
    str_trim(&s);
    ASSERT_STR_PROPS(s, ":)", 2);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_emplace(&s, " :)", 0);
    /*                                                 */ RESET_TRACKING;
    str_trim(&s);
    ASSERT_STR_PROPS(s, ":)", 3);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_emplace(&s, ":)  ", 0);
    /*                                                 */ RESET_TRACKING;
    str_trim(&s);
    ASSERT_STR_PROPS(s, ":)", 4);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_emplace(&s, "  :)  ", 0);
    /*                                                 */ RESET_TRACKING;
    str_trim(&s);
    ASSERT_STR_PROPS(s, ":)", 6);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(clear) {
  str s = str_new("foo");
  {
    /*                                                 */ RESET_TRACKING;
    str_clear(&s);
    ASSERT_STR_PROPS(s, "", 3);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
  }
  str_free(&s);
}

TEST(fit) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_fit(&s, 4);
    ASSERT_STR_PROPS(s, "", 4);
    /*                                                 */ ASSERT_ALLOC(4, s);
    /*                                                 */ ASSERT_FREE;

    /*                                                 */ RESET_TRACKING;
    str_fit(&s, 4);
    ASSERT_STR_PROPS(s, "", 4);
    str_fit(&s, 3);
    ASSERT_STR_PROPS(s, "", 4);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_emplace(&s, "foo", 0);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_fit(&s, 6);
    ASSERT_STR_PROPS(s, "foo", 6);
    /*                                                 */ ASSERT_ALLOC(6, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(grow) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_grow(&s, 1);
    ASSERT_STR_PROPS(s, "", 1);
    /*                                                 */ ASSERT_ALLOC(1, s);
    /*                                                 */ ASSERT_FREE;
    str_emplace(&s, "foo", 0);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_grow(&s, 2);
    ASSERT_STR_PROPS(s, "foo", 5);
    /*                                                 */ ASSERT_ALLOC(5, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(realloc) {
  str s = str_alloc(3);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_realloc(&s, 3);
    ASSERT_STR_PROPS(s, "", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_FREE;

    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_realloc(&s, 11);
    ASSERT_STR_PROPS(s, "", 11);
    /*                                                 */ ASSERT_ALLOC(11, s);
    /*                                                 */ ASSERT_FREE;
    str_emplace(&s, "hey! hello!", 0);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_realloc(&s, 4);
    ASSERT_STR_PROPS(s, "hey!", 4); /* shrink and nullterm */
    /*                                                 */ ASSERT_ALLOC(4, s);
    /*                                                 */ ASSERT_FREE;

    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_realloc(&s, 0);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(shrink) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_shrink(&s, 0);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_FREE;
    str_append(&s, "foobar");
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_shrink(&s, 3);
    ASSERT_STR_PROPS(s, "foo", 3);
    /*                                                 */ ASSERT_ALLOC(3, s);
    /*                                                 */ ASSERT_FREE;

    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_shrink(&s, 3);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_FREE;
    str_append(&s, "foo");
    str_grow(&s, 6);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_shrink(&s, 3);
    ASSERT_STR_PROPS(s, "foo", 6);
    /*                                                 */ ASSERT_ALLOC(6, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(shrinkfit) {
  str s = str_alloc(0);
  {
    /*                                                 */ RESET_TRACKING;
    str_shrinkfit(&s);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_NO_FREE;
    str_append(&s, "foobar");

    str_grow(&s, 10);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_shrinkfit(&s);
    ASSERT_STR_PROPS(s, "foobar", 6);
    /*                                                 */ ASSERT_ALLOC(6, s);
    /*                                                 */ ASSERT_FREE;
    str_clear(&s);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_shrinkfit(&s);
    ASSERT_STR_PROPS(s, "", 0);
    /*                                                 */ ASSERT_ALLOC(0, s);
    /*                                                 */ ASSERT_FREE;
  }
  str_free(&s);
}

TEST(free) {
  {
    str s = str_alloc(0);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_free(&s);
    ASSERT_EQ(s, NULL);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_FREE;
  }
  {
    str s = str_alloc(3);
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_free(&s);
    ASSERT_EQ(s, NULL);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_FREE;
  }
  {
    str s = str_new("finished testing! :D");
    /*                                                 */ RESET_TRACKING;
    /*                                                 */ TRACK_STR(s);
    str_free(&s);
    ASSERT_EQ(s, NULL);
    /*                                                 */ ASSERT_NO_ALLOC;
    /*                                                 */ ASSERT_FREE;
  }
}

/*.----------------------------------------------------------------------------,
 /                                    main                                   */

TEST_MAIN {
  RUN_TEST(alloc);
  RUN_TEST(dup);
  RUN_TEST(new);
  RUN_TEST(sub);
  RUN_TEST(avail);
  RUN_TEST(cap);
  RUN_TEST(end);
  RUN_TEST(len);
  RUN_TEST(mbegin);
  RUN_TEST(mend);
  RUN_TEST(msize);
  RUN_TEST(mstr);
  RUN_TEST(append);
  RUN_TEST(append_);
  RUN_TEST(prepend);
  RUN_TEST(prepend_);
  RUN_TEST(emplace);
  RUN_TEST(emplace_);
  RUN_TEST(insert);
  RUN_TEST(insert_);
  RUN_TEST(cpad);
  RUN_TEST(lpad);
  RUN_TEST(rpad);
  RUN_TEST(trim);
  RUN_TEST(clear);
  RUN_TEST(fit);
  RUN_TEST(grow);
  RUN_TEST(realloc);
  RUN_TEST(shrink);
  RUN_TEST(shrinkfit);
  RUN_TEST(free);
  return 0;
}

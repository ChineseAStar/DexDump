/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Common defines for all Dalvik code.
 */
#ifndef DALVIK_COMMON_H_
#define DALVIK_COMMON_H_

#ifndef LOG_TAG
# define LOG_TAG "dalvikvm"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#if defined(HAVE_ENDIAN_H)
# include <endian.h>
#else /*not HAVE_ENDIAN_H*/
# define __BIG_ENDIAN 4321
# define __LITTLE_ENDIAN 1234
# if defined(HAVE_LITTLE_ENDIAN)
#  define __BYTE_ORDER __LITTLE_ENDIAN
# elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define __BYTE_ORDER __LITTLE_ENDIAN
# else
#  define __BYTE_ORDER __BIG_ENDIAN
# endif
#endif /*not HAVE_ENDIAN_H*/

#define XSTR(x) STR(x)
#define STR(x) #x
#pragma message "The value of ABC: " XSTR(__BYTE_ORDER)

#if !defined(NDEBUG) && defined(WITH_DALVIK_ASSERT)
# undef assert
# define assert(x) \
    ((x) ? ((void)0) : (ALOGE("ASSERT FAILED (%s:%d): %s", \
        __FILE__, __LINE__, #x), *(int*)39=39, (void)0) )
#endif

#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

#define LIKELY(exp) (__builtin_expect((exp) != 0, true))
#define UNLIKELY(exp) (__builtin_expect((exp) != 0, false))

#define ALIGN_UP(x, n) (((size_t)(x) + (n) - 1) & ~((n) - 1))
#define ALIGN_DOWN(x, n) ((size_t)(x) & -(n))
#define ALIGN_UP_TO_PAGE_SIZE(p) ALIGN_UP(p, SYSTEM_PAGE_SIZE)
#define ALIGN_DOWN_TO_PAGE_SIZE(p) ALIGN_DOWN(p, SYSTEM_PAGE_SIZE)

#define CLZ(x) __builtin_clz(x)

/*
 * If "very verbose" logging is enabled, make it equivalent to ALOGV.
 * Otherwise, make it disappear.
 *
 * Define this above the #include "Dalvik.h" to enable for only a
 * single file.
 */
/* #define VERY_VERBOSE_LOG */
#if defined(VERY_VERBOSE_LOG)
# define LOGVV      ALOGV
# define IF_LOGVV() IF_ALOGV()
#else
# define LOGVV(...) ((void)0)
# define IF_LOGVV() if (false)
#endif


/*
 * These match the definitions in the VM specification.
 */
typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;

/*
 * Storage for primitive types and object references.
 *
 * Some parts of the code (notably object field access) assume that values
 * are "left aligned", i.e. given "JValue jv", "jv.i" and "*((s4*)&jv)"
 * yield the same result.  This seems to be guaranteed by gcc on big- and
 * little-endian systems.
 */
struct Object;

union JValue {
#if defined(HAVE_LITTLE_ENDIAN)
    u1      z;
    s1      b;
    u2      c;
    s2      s;
    s4      i;
    s8      j;
    float   f;
    double  d;
    Object* l;
#endif
#if defined(HAVE_BIG_ENDIAN)
    struct {
        u1    _z[3];
        u1    z;
    };
    struct {
        s1    _b[3];
        s1    b;
    };
    struct {
        u2    _c;
        u2    c;
    };
    struct {
        s2    _s;
        s2    s;
    };
    s4      i;
    s8      j;
    float   f;
    double  d;
    void*   l;
#endif
};

#define OFFSETOF_MEMBER(t, f)         \
  (reinterpret_cast<char*>(           \
     &reinterpret_cast<t*>(16)->f) -  \
   reinterpret_cast<char*>(16))

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

#endif  // DALVIK_COMMON_H_

#if defined(WIN32) && !defined(PRId64)
#define PRId64 "I64d"
#endif

#define android_testLog(prio, tag) (1)

#ifndef IF_ALOGE
#define IF_ALOGE() IF_ALOG(prio, tag)
#endif

#ifndef IF_ALOG
#define IF_ALOG(priority, tag) \
    if (android_testLog(ANDROID_##priority, tag))
#endif

#if 1
/*
 * Pretend we have the Android logging macros.  These are replaced by the
 * Android logging implementation.
 */
#define ANDROID_LOG_DEBUG 3
#define ALOGV(...)    LOG_PRI(2, 0, __VA_ARGS__)
#define ALOGD(...)    LOG_PRI(3, 0, __VA_ARGS__)
#define ALOGI(...)    LOG_PRI(4, 0, __VA_ARGS__)
#define ALOGW(...)    LOG_PRI(5, 0, __VA_ARGS__)
#define ALOGE(...)    LOG_PRI(6, 0, __VA_ARGS__)
#define MIN_LOG_LEVEL   2
/*
            dvmFprintf(stdout, "%s:%-4d ", __FILE__, __LINE__);     \
            dvmFprintf(stdout, __VA_ARGS__);                        \
*/
#define LOG_PRI(priority, tag, ...) do {                            \
        if (priority >= MIN_LOG_LEVEL) {                            \
            fprintf(stdout, "%s:%-4d ", __FILE__, __LINE__);     \
            fprintf(stdout, __VA_ARGS__);                        \
        }                                                           \
    } while(0)
#else
# include "utils/Log.h"
#endif

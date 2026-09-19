// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Common Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_COMMON_H__
#define __AFW_COMMON_H__

/* Need to include doxygen groups. */
#include "afw_doxygen.h"


/**
 * @addtogroup afw_common
 * @{
 */

/**
 * @file afw_common.h
 * @brief Common types, macros, and includes used throughout AFW.
 *
 * Early shared substrate: C library includes, opaque typedefs, defines,
 * and complete structs that must exist before the generated interfaces.
 * Included by afw_interface.h. Do not include this header directly.
 *
 * What to include:
 *
 * You are writing                         | Include
 * --------------------------------------- | -------
 * libafw C under src/afw/                 | afw_internal.h only
 * Extension, command, or application C    | afw.h only
 * A module header                         | afw_interface.h; if that is
 *                                         | not enough, afw_minimal.h
 *
 * Do not include a peer afw_*.h from a module header. A pointer to
 * another type uses the opaque afw_*_t (afw_common_opaques.h and
 * generated afw_interface_opaques.h). Put a complete struct in this
 * header when it is embedded in another early struct, or a similar
 * special case. Otherwise the struct stays in its module header.
 *
 * Header stack (each line includes the one above):
 *
 *     afw_common.h  (plus opaques and generated interface opaques)
 *     afw_interface.h     generated contracts and call macros
 *     afw_minimal.h       bootstrap set; include order is resolved here
 *     afw.h               C umbrella for extensions, commands, and apps
 *     afw_internal.h      libafw C only
 *
 * Headers in the minimal set include only afw_interface.h from AFW.
 * They must not include afw_minimal.h. If one of them needs another
 * member of the set (for example xctx needs vector macros), add that
 * include to afw_minimal.h before the header that needs it.
 *
 * afw_minimal.h is for other headers, not a thinner API for C files.
 */

/*
 * Compile-time debug probes (runtime flags still off unless set).
 * Written to debug_fd as ">debug eval|lock|pool ...", not via afw_trace.
 *   AFW_DEBUG_EVALUATION  evaluation stack push/pop
 *   AFW_DEBUG_LOCK        lock obtain/release
 *   AFW_DEBUG_POOL        prefix {pool,size} before user pointer on
 *                         heap/tracker; always checked on free.
 *                         Freed USER is filled so a dangling inf
 *                         faults on any vtable access.
 *                         debug:pool:detail is the alloc/free firehose.
 * `afwdev build --cdev` and `--fulldev` define all three. Otherwise:
 *   afwdev build --define AFW_DEBUG_POOL
 */

/* Common standard headers. */
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#if !defined(_WIN32) && !defined(WIN32)
#include <sys/types.h>
#endif

 
/*
 * Adaptive Framework common opaque typedefs are contained in
 * afw_common_opaques.h.
 */
#include "afw_common_opaques.h"

/*
 * Adaptive Framework interface opaque typedefs are contained in
 * afw_interface_opaques.h.
 */
#include "afw_interface_opaques.h"

/*
 * Atomic.
 *
 * On Windows, volatile has atomic semantics; otherwise require C11 or higher
 * and atomic primitives and types.
 */
#if defined(_WIN32) || defined(WIN32)
#define AFW_ATOMIC volatile
#define AFW_WINDOWS 1
#else
#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#define AFW_HAS_ATOMIC
#include <stdatomic.h>
#define AFW_ATOMIC _Atomic
#else
#error C11 or higher with atomic primitives and types is required to compile Adaptive Framework
#endif
#endif

/*
 * Public C API declare/define helpers (formerly generated afw_declare_helpers.h).
 * Platform declspec + calling convention for the libafw shared library surface.
 * Internal AFW_DECLARE_INTERNAL* macros are no longer used in-tree.
 */

/**
 * AFW_BEGIN_DECLARES goes before declares and AFW_END_DECLARES at end in
 * afw*.h files.
 */
#ifdef __cplusplus
#define AFW_BEGIN_DECLARES extern "C" {
#define AFW_END_DECLARES }
#else
#define AFW_BEGIN_DECLARES
#define AFW_END_DECLARES
#endif
#if defined(WIN32)
#error afw is not currently supported on Windows
#endif

/* #defines for declspec and calling convention. */
#if defined(DOXYGEN) || !defined(WIN32)
#define AFW_DECLSPEC_DECLARE extern
#define AFW_DECLSPEC_DEFINE
#define AFW_CALLING_CONVENTION
#define AFW_CALLING_CONVENTION_ELLIPSIS
#elif defined(AFW_DECLARE_STATIC)
#define AFW_DECLSPEC_DECLARE extern
#define AFW_DECLSPEC_DEFINE
#define AFW_CALLING_CONVENTION __stdcall
#define AFW_CALLING_CONVENTION_ELLIPSIS __cdecl
#elif defined(AFW_DECLARE_EXPORT)
#define AFW_DECLSPEC_DECLARE extern __declspec(dllexport)
#define AFW_DECLSPEC_DEFINE __declspec(dllexport)
#define AFW_CALLING_CONVENTION __stdcall
#define AFW_CALLING_CONVENTION_ELLIPSIS __cdecl
#else
#define AFW_DECLSPEC_DECLARE extern __declspec(dllimport)
#define AFW_DECLSPEC_DEFINE __declspec(dllimport)
#define AFW_CALLING_CONVENTION __stdcall
#define AFW_CALLING_CONVENTION_ELLIPSIS __cdecl
#endif

/**
 * @brief Declare a public afw function.
 * @param type of return value.
 *
 * There must be a corresponding AFW_DEFINE() in a /src/afw/ source .c file.
 */
#define AFW_DECLARE(type) \
AFW_DECLSPEC_DECLARE \
type \
AFW_CALLING_CONVENTION

/**
 * @brief Declare a public afw function with variable arguments.
 * @param type of return value.
 */
#define AFW_DECLARE_ELLIPSIS(type) \
AFW_DECLSPEC_DECLARE \
type \
AFW_CALLING_CONVENTION_ELLIPSIS

/**
 * @brief Declare a public afw const variable.
 * @param type of variable
 */
#define AFW_DECLARE_CONST_DATA(type) \
AFW_DECLSPEC_DECLARE \
const type

/**
 * @brief Define a public afw function.
 * @param type of return value.
 */
#define AFW_DEFINE(type) \
AFW_DECLSPEC_DEFINE \
type \
AFW_CALLING_CONVENTION

/**
 * @brief Define a public afw function with variable arguments.
 * @param type of return value.
 */
#define AFW_DEFINE_ELLIPSIS(type) \
AFW_DECLSPEC_DEFINE \
type \
AFW_CALLING_CONVENTION_ELLIPSIS

/**
 * @brief Define a public afw const variable.
 * @param type of variable
 */
#define AFW_DEFINE_CONST_DATA(type) \
AFW_DECLSPEC_DEFINE \
const type

#define AFW_CALLBACK \
AFW_CALLING_CONVENTION

#define AFW_CALLBACK_ELLIPSIS \
AFW_CALLING_CONVENTION_ELLIPSIS

#ifdef WIN32
#define AFW_DEFINE_DSO(type) __declspec(dllexport) type
#else
#define AFW_DEFINE_DSO(type) type
#endif

#ifdef AFW_DISABLE_INLINE
#define AFW_HAS_INLINE 0
#define AFW_INLINE
#else
#define AFW_HAS_INLINE 1
#define AFW_INLINE inline
#endif

/** AFW Inline. */
#define AFW_STATIC_INLINE static AFW_INLINE

#define AFW_DECLARE_STATIC(type) static type

#if defined(DOXYGEN)
#define AFW_DEFINE_STATIC_INLINE(type) type
#else
#define AFW_DEFINE_STATIC_INLINE(type) AFW_STATIC_INLINE type
#endif

/* Include template headers. */
#include "afw_associative_array_template.h"

/**
 * @brief Macro to avoid unused variable warning
 * 
 * This avoids GCC warning unused-variable when placed before a variable.
 * 
 * Example:
 * 
 * AFW_COMPILER_ANNOTATION_UNUSED afw_boolean_t this_FINALLY_ENTERED = false;
 */
#ifdef __GNUC__
#define AFW_COMPILER_ANNOTATION_UNUSED __attribute__((unused))
#else
#define AFW_COMPILER_ANNOTATION_UNUSED
#endif

/**
 * @brief Macro to mark a variable as nonnull to get around a Clang warning
 * 
 * This avoids Clang warning that variable might be NULL when placed before a
 * variable.
 * 
 * Example:
 * 
 * AFW_COMPILER_ANNOTATION_NONNULL afw_xctx_t *xctx;
 */
#ifdef __clang__
#define AFW_COMPILER_ANNOTATION_NONNULL __attribute__((nonnull))
#else
#define AFW_COMPILER_ANNOTATION_NONNULL
#endif

/**
 * @deprecated Do not use in new or changed code.
 *
 * **Do not use.** Expands to C `assert()` and can **abort the process**.
 * That is unsafe under a long-running server / request (takes down the
 * worker). Use `AFW_THROW_ERROR_Z` / `AFW_THROW_ERROR_FZ` /
 * `AFW_COMPILE_THROW_ERROR_*` so failures participate in `AFW_TRY` /
 * `AFW_CATCH` / `AFW_FINALLY` and the request can fail cleanly. There is
 * intentionally **no** `AFW_ABORT` (or similar) helper — do not invent
 * process-abort wrappers.
 *
 * Kept only until remaining call sites are removed; new code must not add
 * `AFW_ASSERT`.
 *
 * @fixme Remove when no remaining call sites.
 */
#define AFW_ASSERT(e) assert(e)

/** @fixme */
#ifndef offsetof
#define offsetof(type, member)  __builtin_offsetof (type, member)
#endif

AFW_BEGIN_DECLARES

/** Adaptive Framework's core adapter id. */
#define AFW_ADAPTER_ID "afw"

/** @brief Evaluate x and quote the results. */
#define AFW_STRINGIFY(x) AFW_STRINGIFY_x(x)
#define AFW_STRINGIFY_x(x) #x

/** @brief file:line */
#define AFW__FILE_LINE__ __FILE__ ":" AFW_STRINGIFY(__LINE__)

/** @brief size_t. */
typedef size_t afw_size_t;

/** @brief off_t. */
#ifdef AFW_WINDOWS
typedef long long afw_off_t;
#else
typedef off_t afw_off_t;
#endif

/** @brief 8-bit signed integer. */
typedef int8_t afw_int8_t;

/** @brief 16-bit signed integer. */
typedef int16_t afw_int16_t;

/** @brief 32-bit signed integer. */
typedef int32_t afw_int32_t;

/** @brief Min int32. */
#define AFW_INT32_MIN -2147483647 - 1

/** @brief Max int32. */
#define AFW_INT32_MAX 2147483647

/** @brief 64-bit signed integer. */
typedef int64_t afw_int64_t;

/** @brief 8-bit unsigned integer. */
typedef uint8_t afw_uint8_t;

/** @brief 16-bit unsigned integer. */
typedef uint16_t afw_uint16_t;

/** @brief Max uint32. */
#define AFW_UINT16_MAX 65536

/** @brief 32-bit unsigned integer. */
typedef uint32_t afw_uint32_t;

/** @brief Max uint32. */
#define AFW_UINT32_MAX 4294967296

/** @brief 64-bit unsigned integer. */
typedef uint64_t afw_uint64_t;

/** @brief 32-bit float. */
typedef float afw_float32_t;

/** @brief 64-bit float. */
typedef double afw_float64_t;

/** @brief Normal float. */
typedef double afw_float_t;

/** @brief Normal AFW number is double. */
typedef double afw_double_t;

/** @brief Unicode code point. */
typedef afw_int32_t afw_code_point_t;

/**
 * @brief 8-bit unsigned memory unit.
 *
 * CHAR_BIT is 8. There is no separate "byte that might not be 8 bits."
 * UTF-8 code units stay `afw_utf8_octet_t` (`char`) for pointer signedness.
 */
typedef unsigned char afw_octet_t;

/**
 *  @brief 8 bits of utf-8 codepoint.
 * 
 * This is one 8-bit octet of a UTF-8 encoded character.
 * 
 * String literals in c are "char *" and "NUL terminated strings" are
 * often declared as "char *". Because of this, afw_utf8_octet_t is also
 * "char" to avoid "differ in signedness" warnings when compiled using
 * gcc with CFLAGS -Werror=pointer-sign active.
 * 
 * Depending on the compiler, "char" can be considered signed or unsigned
 * so avoid using comparisons like (>= 0) to determine if an octet is ASCII.
 * Even specifying "signed char" or "unsigned char" instead of "char" in gcc
 * can cause "differ in signedness" warnings.
 * 
 * First      | Last       |          |          |          |
 * code point | code point |  Octet 1 |  Octet 2 |  Octet 3 |  Octet 4
 * -----------|------------|----------|----------|----------|---------
 *     U+0000 |     U+007F | 0xxxxxxx |        - |        - |        -
 *     U+0080 |     U+07FF | 110xxxxx | 10xxxxxx |        - |        -
 *     U+0800 |     U+FFFF | 1110xxxx | 10xxxxxx | 10xxxxxx |        -
 *    U+10000 |   U+10FFFF | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx 
 */
typedef char afw_utf8_octet_t;

/**
 * @brief Get Unicode code point callback.
 * @param data from provider of callback.
 * @param xctx of caller to callback
 * @return -1 if end of file, otherwise a Unicode code point.
 */
typedef afw_code_point_t (*afw_code_point_get_cb_t) (
    void *data, afw_xctx_t *xctx);


/**
 * @brief Get an utf-8 octet (8 bits).
 * @param octet cursor.
 * @param data from provider of callback.
 * @param xctx of caller to callback
 * @return -1 if end of file, otherwise 0.
 */
typedef int (*afw_utf8_octet_get_cb_t) (
    afw_utf8_octet_t *octet,
    void *data, afw_xctx_t *xctx);


/** @brief String is NUL (0) terminate.
 * 
 * For function and methods that have length parameters for strings
 * and document that -1 can be used if string is NUL (0) terminated,
 * this symbol can be used.
 */
#define AFW_UTF8_Z_LEN -1


/**
 * @brief this is the maximum number of digits that can be produced by
 * afw_integer_t plus negative sign plus null terminator.
 * Largest negative 64 bit int is -9223372036854775808
 * Largest positive 64 bit int is  9223372036854775807
 */
#define AFW_INTEGER_MAX_BUFFER 21


/**
 * @brief largest afw_integer_t
 */
#define AFW_INTEGER_MAX  INT64_MAX

/**
 * @brief largest afw_integer_t quoted
 */
#define AFW_INTEGER_Q_MAX "9223372036854775807"

/**
 * @brief smallest afw_integer_t
 */
#define AFW_INTEGER_MIN INT64_MIN

/**
 * @brief smallest afw_integer_t quoted
 */
#define AFW_INTEGER_Q_MIN "-9223372036854775808"

/**
 * @brief largest afw_integer_t value that a double can hold.
 */
#define AFW_INTEGER_MAX_SAFE_DOUBLE 9007199254740991

/**
 * @brief smallest afw_integer_t value that a double can hold.
 */
#define AFW_INTEGER_MIN_SAFE_DOUBLE -9007199254740991

/**
 * @brief Tests integer to be safely held in a double.
 * @param integer to test.
 * @return boolean result.
 */
#define AFW_INTEGER_IS_SAFE_DOUBLE(integer) \
    ((integer) <= AFW_INTEGER_MAX_SAFE_DOUBLE && \
    ((integer) >= AFW_INTEGER_MIN_SAFE_DOUBLE)


/**
 * @brief typedef for big signed int.
 */
typedef int64_t afw_integer_t;

/**
 * @brief Format string specifier used for afw_integer_t.
 */
#define AFW_INTEGER_FMT "%" PRId64

/**
 * @brief Format string specifier used for afw_integer_t without %.
 * 
 * This can be used if additional specifiers are needed. For example,
 * printf("The number is %0" AFW_INTEGER_FMT_NO_PERCENT "\n", number);
 */
#define AFW_INTEGER_FMT_NO_PERCENT PRId64

/**
 * @brief afw_integer_t max digits.
 */
#define AFW_INTEGER_MAX_DIGITS 19

/**
 * @brief afw_integer_t max digits in quotes.
 */
#define AFW_Q_INTEGER_MAX_DIGITS "19"

/**
 * @brief Format string specifier used for afw_size_t.
 */
#define AFW_SIZE_T_FMT "%zu"

/**
 * @brief Format string specifier used for afw_size_t without %.
 * 
 * This can be used if additional specifiers are needed. For example,
 * printf("The size is %0" AFW_SIZE_T_FMT_NO_PERCENT "\n", size);
 */
#define AFW_SIZE_T_FMT_NO_PERCENT "zu"

/**
 * @brief afw_size_t max.
 */
#define AFW_SIZE_T_MAX SIZE_MAX

/**
 * @brief afw_size_t max digits.
 */
#define AFW_SIZE_T_MAX_DIGITS 19

/**
 * @brief afw_size_t max digits in quotes.
 */
#define AFW_Q_SIZE_T_MAX_DIGITS "19"

/**
 * @brief this is the maximum number of digits that can be produced by
 * afw_size_t plus null terminator.
 */
#define AFW_SIZE_T_MAX_BUFFER AFW_SIZE_T_MAX_DIGITS + 1



/**
 * @brief typedef for big unsigned int.
 */
typedef uint64_t afw_unsigned_integer_t;


/** Boolean value. */
typedef _Bool afw_boolean_t;


/** AFW boolean true.
 *
 * True can be any value other than 0.
 *
 * Only use AFW_TRUE for setting a true value.  Normal C boolean testing
 * should be used for checking.
 */
#define AFW_TRUE true


/** AFW boolean false.
 *
 * False will always be 0.
 *
 * Normal C boolean testing can be used for checking.
 */
#define AFW_FALSE false


/** The end-of-line marker for HTTP protocol elements. */
#define AFW_CRLF "\x0d\x0a"


/** The strlen of the end-of-line marker for HTTP protocol elements. */
#define AFW_CRLF_STRLEN 2


/**
 * @brief Compile Type Map.
 * @param XX macro
 *
 * There must be and afw_s_ with each name in map.
 */
#define AFW_COMPILE_TYPE_MAP(XX)                                                \
                                                                                \
    XX(error,                                                                   \
        string, NULL,                                                           \
        "This is an error")                                                     \
                                                                                \
    XX(json,                                                                    \
        string,                                                                 \
        &afw_function_definition_compile_json,                                  \
        "Compile strict json")                                                  \
                                                                                \
    XX(regexp,                                                                  \
        regexp,                                                                 \
        &afw_function_definition_compile_regexp,                                \
        "Parses a regular expression conforming to XML Schemas Part 2 "         \
        "Datatype Appendix F and builds an automata suitable for testing "      \
        "strings against that regular expression ")                             \
                                                                                \
    XX(relaxed_json,                                                            \
        string,                                                                 \
        &afw_function_definition_compile_relaxed_json,                          \
        "1) Allows inline and block comments\n"                                 \
        "2) Allows property names to be unquoted if they start with '_',\n"     \
        "   '$', 'a-z', 'A-Z', or char > 127 followed by any of those or\n"     \
        "   '0' - '9'.\n"                                                       \
        "3) Allows Infinity, -Infinity, NaN, and -NaN for number values.\n"     \
        "4) Allows a trailing comma in Object and List.")                       \
                                                                                \
    XX(script,                                                                  \
        script,                                                                 \
        &afw_function_definition_compile_script,                                \
        "See production Script in src/afw/doc/syntax/index.html or "            \
        "src/afw/generated/ebnf/syntax.ebnf for syntax.")                       \
                                                                                \
    XX(template,                                                                \
        template,                                                               \
        &afw_function_definition_compile_template,                              \
        "See production Template in src/afw/doc/syntax/index.html or "          \
        "src/afw/generated/ebnf/syntax.ebnf for syntax.")                       \
                                                                                \
    XX(test_script,                                                             \
        script,                                                                 \
        &afw_function_definition_compile_script,                                \
        "See production TestScript in src/afw/doc/syntax/index.html or "        \
        "src/afw/generated/ebnf/syntax.ebnf for syntax.")                       \
                                                                                \
    XX(xpathExpression,                                                         \
        xpathExpression,                                                        \
        &afw_function_definition_compile_xpathExpression,                       \
        "XML Path Language (XPath) expression")                                 \
 
/**
 * @brief Compile type enum
 */
typedef enum afw_compile_type_e {
#define XX(name, data_type, compile_function, description) \
    afw_compile_type_ ## name,
    AFW_COMPILE_TYPE_MAP(XX)
#undef XX
    afw_compile_type_count
} afw_compile_type_t;


/**
 * @brief Data type instance: clone value unmanaged into dest p.
 *
 * Full payload copy (including utf8/memory octets). Permanents as-is.
 * Do not release the source.
 */
typedef const afw_value_t *
(*afw_data_type_clone_value_unmanaged_t)(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Data type instance: clone value managed in p->managed_p.
 *
 * Already managed: get_reference. Unmanaged: create_managed.
 * Permanents as-is. Do not release the source.
 */
typedef const afw_value_t *
(*afw_data_type_clone_value_managed_t)(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Struct for memory pointer and size.
 *
 * IMPORTANT:  This must match up with afw_utf8_t since it is accepted to
 *            cast from afw_utf8_t to afw_memory_t.  Don't cast the other way
 *            around unless you are positive ptr point to NFC Unicode --
 *            use afw_utf8_from_memory() instead.
 */
typedef struct afw_memory_s {
    const afw_octet_t *ptr;
    afw_size_t size;
} afw_memory_t;


/**
 * @brief NFC normalized UTF-8 null terminated string.
 *
 * All UTF-8 strings MUST be NFC normalized.  Doing otherwise can cause
 * unintended minor or major errors.
 *
 * Use the afw_utf8_z_create* functions in afw_utf8.h or an ascii literal.
 * 
 * If 0x00 must be represented in a string, use afw_utf8_t instead.
 * 
 * @see afw_utf8_octet_t for more information.
 */
typedef afw_utf8_octet_t afw_utf8_z_t;


/**
 * @brief NFC normalized UTF-8 string.
 *
 * IMPORTANT:  This must match up with afw_utf8_t since it is accepted to
 *            cast from afw_utf8_t to afw_memory_t.  Don't cast the other way
 *            around unless you are positive ptr point to NFC Unicode --
 *            use afw_utf8_from_memory() instead.
 *
 * All UTF-8 strings MUST be NFC normalized.  Doing otherwise can cause
 * unintended minor or major errors.
 *
 * See RFC 5198 and http://unicode.org/reports/tr15/
 *
 * This string is not null terminated, so it can contain NUL characters. The
 * number of bytes in the string is stored in len and a pointer to the first
 * byte in s.
 * 
 * @see afw_utf8_octet_t for more information.
 */
typedef struct afw_utf8_s {
    const afw_utf8_octet_t *s;
    afw_size_t len;
} afw_utf8_t;



/**
 * @brief Internal utf8 + external C string (same pointer).
 *
 * Use when the bytes are already 0-terminated (C literal, `z_create`
 * result). `.s` is length-prefixed NFC; `.s_z` is the external C string.
 * No interior 0 in `.s.len` bytes. There is no create for this type.
 * Generated strings use `afw_s_*` / `afw_z_*` instead.
 */
typedef union afw_utf8_utf8_z_s {
    afw_utf8_t s;
    const afw_utf8_z_t *s_z;
} afw_utf8_utf8_z_t;



/**
 * @brief Array of strings. 
 */
typedef struct afw_utf8_array_s {
    afw_integer_t count;
    union {
        afw_utf8_t *first;
        afw_utf8_t (*array)[];
    };
} afw_utf8_array_t;


/**
 * @brief Trusted C string-literal initializer (no NFC check).
 *
 * Example:
 *
 * static const afw_utf8_t hello = AFW_UTF8_LITERAL("Hello World");
 *
 * ASCII (including \\n, \\t, \\\\) is always valid UTF-8 and already NFC.
 * Invalid only via \\xNN that is not UTF-8, or a non-UTF-8 source file.
 * AFW does not support EBCDIC. Those are programmer errors, same as a
 * bad pointer. Runtime / unknown bytes go through afw_utf8_set /
 * afw_utf8_create, not this macro.
 */
#define AFW_UTF8_LITERAL(A_STRING) {A_STRING, sizeof(A_STRING) - 1}


/**
 * @brief Format string specifier used for afw_utf8_t with libc.
 *
 * libc `%.*s` (int len, char *). Stops at an interior 0. Use with
 * `fprintf` and `AFW_UTF8_FMT_ARG`. AFW printf uses `%ku` (one
 * `const afw_utf8_t *`; NULL is empty).
 */
#define AFW_UTF8_FMT "%.*s"

/**
 * @brief Quoted libc format specifier for afw_utf8_t.
 */
#define AFW_UTF8_FMT_Q "'%.*s'"


/** @brief Format string used for source location (AFW printf). */
#define AFW_UTF8_CONTEXTUAL_LABEL_FMT "%ku> "

/**
 * @brief Convenience Macro for use with AFW_UTF8_FMT to specify arg.
 * @param A_STRING a (const afw_utf8_t *) string.
 *
 * The argument for " AFW_UTF8_FMT " is an int length (not size_t) followed by
 * a comma and const char *.  
 */
#define AFW_UTF8_FMT_ARG(A_STRING) \
    (int)(A_STRING)->len, (const char *)(A_STRING)->s


/**
 * @brief Macro to produce prefix for AFW messages.
 */
#define AFW_MESSAGE_PREFIX __FILE__ ":" AFW_STRINGIFY(__LINE__) " "


/**
 * @brief UTF-8 encoding.
 */
#define AFW_UTF8_ENCODING "UTF-8"


/**
 * @brief A Unicode codepoint.  Range is 0 to 0x10ffff.  Other values are
 *       illegal Unicode codepoints.
 */
typedef uint32_t afw_u_cp_t;


/**
 * @brief this implementation uses UTF-8.
 */
typedef char *afw_uri_t;


/** Log priority mask. See afw_log.h for more information. */
typedef int afw_log_priority_mask_t;


/** Adaptive Framework Return Code. */
typedef int afw_rc_t;


/**
 * @brief Typedef for name/value pair.
 */
typedef struct afw_name_value_s {
    const afw_utf8_t *name;
    const afw_value_t *value;
} afw_name_value_t;


/**
 * @brief Typedef for key/string pair.
 */
typedef struct afw_key_string_s {
    const afw_utf8_t *key;
    const afw_utf8_t *string;
} afw_key_string_t;


/**
 * @brief Key/string pair, each an internal utf8 + external C string.
 *
 * Same contract as `afw_utf8_utf8_z_t` on both sides. VFS map uses this.
 */
typedef struct afw_key_z_string_z_s {
    union {
        afw_utf8_t key;
        const afw_utf8_z_t *key_z;
    };
    union {
        afw_utf8_t string;
        const afw_utf8_z_t *string_z;
    };
} afw_key_z_string_z_t;


/**
 * @brief Typedef for meta variable in afw_object interface.
 *
 * Information in this struct should be accessed using functions declared in
 * afw_object_meta.h -- do not access directly.
 */
typedef struct afw_object_meta_s {

    /**
     * @brief Meta object.
     *
     * This is the meta object associated with this object.  See
     * afw_object_meta.h for more information.
     */
    const afw_object_t *meta_object;

    /**
     * @brief Embedding object.
     *
     * Embedding object or NULL if this is an entity object.
     */
    const afw_object_t *embedding_object;

    /**
     * @brief Object id or property name.
     *
     * If embedding_object is NULL, this is the entity's objectId.  If
     * embedding_object is not NULL, this is the propertyName of this object
     * in that object.
     *
     * Use afw_object_meta_get_object_id() to access an entity's objectId,
     * which will throw an error if the object is embedded.  Use
     * afw_object_meta_get_property_name() to access an object's property name,
     * which will throw an error if this object is an entity.
     */
    const afw_utf8_t *id;

    /**
     * @brief Object type object URI or NULL.
     *
     * This is the URI of the object type object.  If this is just an objectId,
     * the URI is the object URI with the last two path segments replaced by
     * /_AdaptiveObjectType_/ followed by this objectId.
     *
     * Use these functions to access:
     *     . afw_object_meta_get_object_type_id()
     *     . afw_object_meta_get_object_type_path()
     *     . afw_object_meta_get_object_type_uri()
     */
    const afw_utf8_t *object_type_uri;

    /**
     * @brief Object path or NULL.
     *
     * This is the URI of the object.  At a minimum, this is the relative URI of
     * the object within the current adaptive framework environment:
     *
     * /<adapter id>/<object type id>/<object id>
     *
     * Use these functions to access:
     *     . afw_object_meta_get_id()
     *     . afw_object_meta_get_path()
     *     . afw_object_meta_get_uri()
     */
    const afw_utf8_t *object_uri;

} afw_object_meta_t;



/** @brief Error code map.
 *
 * Each row is: id, error_allow_in_response, http_response_code, description.
 *
 * id: Token pasted as afw_error_code_<id> and exposed as _AdaptiveError_.id.
 * error_allow_in_response: If false, HTTP response omits the error object.
 * http_response_code: Status for an uncaught error on an HTTP request.
 * description: Phrase after the status number (e.g. "404 Not Found").
 *
 * Scripts should branch on id, not numeric errorCode. On this beta line the
 * numeric order may change when the map is reviewed; prefer id.
 *
 * Layout: none / general / throw first, then script/language, then request
 * HTTP, then host. general is the default when nothing more specific fits.
 *
 * none                     - No error has occurred.
 *
 * general                  - Default error. Use this unless a more specific
 *                           code (especially one with a different HTTP
 *                           status) applies.
 *
 * throw                    - Adaptive Script throw statement.
 *
 * assertion_failed         - assert() failed.
 *
 * argument_error           - Adaptive function argument error (including
 *                           things like integer divide by zero).
 *
 * conversion_error         - Conversion to a data type failed.
 *
 * undefined_value          - Required value was undefined.
 *
 * syntax                   - Adaptive source parse / compile syntax error.
 *
 * HTTP-shaped ids follow IANA assigned codes a server or extension may
 * issue (RFC 9110 plus later registry entries). Adaptive extras that share
 * an HTTP number stay next to that number (query_too_complex and
 * request_syntax with 400; read_only with 403; terminating with 503).
 * Existing Adaptive names are kept (authentication_required, denied,
 * unsupported_accept, client_timeout, payload_too_large,
 * unsupported_content, method_not_supported). Not included: 1xx,
 * deprecated 305/306, obsolete 510, temporary 104.
 *
 * bad_request              - Something about the request was bad.
 *
 * query_too_complex        - Query is too complex for this adapter/index.
 *
 * request_syntax           - Error parsing the HTTP request body / path.
 *
 * authentication_required  - Subject authentication required.
 *
 * denied                   - Caller is not authorized. See error data.
 *
 * read_only                - Something is write protected. See error data.
 *
 * not_found                - A named resource was not found. Do not throw
 *                           this when "missing" is a normal get (property
 *                           miss → NULL / found=false). Do throw for a
 *                           named adapter / object / URI / file the
 *                           caller asked for (open_file ENOENT).
 *
 * method_not_allowed       - HTTP method is not allowed for this resource.
 *
 * unsupported_accept       - None of the Accept types are supported.
 *
 * client_timeout           - Timed out waiting on the client.
 *
 * conflict                 - Conflict such as resource in use.
 *
 * length_required          - Request requires a content length.
 *
 * payload_too_large        - Request or result exceeds a configured limit.
 *                           HTTP phrase is IANA "Content Too Large".
 *
 * unsupported_content      - Request Content-Type is not supported.
 *
 * im_a_teapot              - Historical 418 (IANA lists 418 as unused).
 *
 * memory                   - Not enough memory to continue.
 *
 * coding_error             - Internal coding error.
 *
 * method_not_supported     - This implementation does not support the
 *                           method or capability.
 *
 * terminating              - Server is terminating; stop starting more work
 *                           (e.g. mid retrieve at an object boundary).
 *                           HTTP 503; service_unavailable is the generic 503.
 *
 * client_closed            - Client closed the connection.
 *
 */
#define AFW_ERROR_CODE_MAP(XX)                                                  \
    XX(none,                               true,  200, "OK"                               )\
    XX(general,                            true,  500, "General Error"                    )\
    XX(throw,                              true,  400, "Statement throw encountered"      )\
    XX(assertion_failed,                   true,  400, "Assertion failed"                 )\
    XX(argument_error,                     true,  400, "Argument Error"                   )\
    XX(conversion_error,                   true,  400, "Conversion Error"                 )\
    XX(undefined_value,                    true,  400, "Undefined Value"                  )\
    XX(syntax,                             true,  400, "Syntax Error"                     )\
    XX(created,                            true,  201, "Created"                          )\
    XX(accepted,                           true,  202, "Accepted"                         )\
    XX(no_content,                         false, 204, "No Content"                       )\
    XX(partial_content,                    true,  206, "Partial Content"                  )\
    XX(multi_status,                       true,  207, "Multi-Status"                     )\
    XX(multiple_choices,                   true,  300, "Multiple Choices"                 )\
    XX(moved_permanently,                  true,  301, "Moved Permanently"                )\
    XX(moved_temporarily,                  true,  302, "Found"                            )\
    XX(see_other,                          true,  303, "See Other"                        )\
    XX(not_modified,                       false, 304, "Not Modified"                     )\
    XX(temporary_redirect,                 true,  307, "Temporary Redirect"               )\
    XX(permanent_redirect,                 true,  308, "Permanent Redirect"               )\
    XX(bad_request,                        true,  400, "Bad Request"                      )\
    XX(query_too_complex,                  true,  400, "Query Too Complex"                )\
    XX(request_syntax,                     true,  400, "Request Syntax Error"             )\
    XX(authentication_required,            true,  401, "Authentication Needed"            )\
    XX(payment_required,                   true,  402, "Payment Required"                 )\
    XX(denied,                             true,  403, "Forbidden - Access Denied"        )\
    XX(read_only,                          true,  403, "Forbidden - Read Only"            )\
    XX(not_found,                          true,  404, "Not Found"                        )\
    XX(method_not_allowed,                 true,  405, "Method Not Allowed"               )\
    XX(unsupported_accept,                 false, 406, "Unsupported Content Type Requested")\
    XX(proxy_authentication_required,      true,  407, "Proxy Authentication Required"    )\
    XX(client_timeout,                     true,  408, "Request Timeout"                  )\
    XX(conflict,                           true,  409, "Conflict"                         )\
    XX(gone,                               true,  410, "Gone"                             )\
    XX(length_required,                    true,  411, "Content Length Required"          )\
    XX(precondition_failed,                true,  412, "Precondition Failed"              )\
    XX(payload_too_large,                  true,  413, "Content Too Large"                )\
    XX(uri_too_long,                       true,  414, "URI Too Long"                     )\
    XX(unsupported_content,                true,  415, "Unsupported Media Type"           )\
    XX(range_not_satisfiable,              true,  416, "Range Not Satisfiable"            )\
    XX(expectation_failed,                 true,  417, "Expectation Failed"               )\
    XX(im_a_teapot,                        true,  418, "I'm a Teapot"                     )\
    XX(misdirected_request,                true,  421, "Misdirected Request"              )\
    XX(unprocessable_content,              true,  422, "Unprocessable Content"            )\
    XX(locked,                             true,  423, "Locked"                           )\
    XX(failed_dependency,                  true,  424, "Failed Dependency"                )\
    XX(too_early,                          true,  425, "Too Early"                        )\
    XX(upgrade_required,                   true,  426, "Upgrade Required"                 )\
    XX(precondition_required,              true,  428, "Precondition Required"            )\
    XX(too_many_requests,                  true,  429, "Too Many Requests"                )\
    XX(request_header_fields_too_large,    true,  431, "Request Header Fields Too Large"  )\
    XX(unavailable_for_legal_reasons,      true,  451, "Unavailable For Legal Reasons"    )\
    XX(memory,                             true,  500, "Memory Error"                     )\
    XX(coding_error,                       true,  500, "Internal Coding Error"            )\
    XX(method_not_supported,               true,  501, "Method Not Supported"             )\
    XX(bad_gateway,                        true,  502, "Bad Gateway"                      )\
    XX(service_unavailable,                true,  503, "Service Unavailable"              )\
    XX(terminating,                        true,  503, "Server Terminating"               )\
    XX(gateway_timeout,                    true,  504, "Gateway Timeout"                  )\
    XX(http_version_not_supported,         true,  505, "HTTP Version Not Supported"       )\
    XX(variant_also_negotiates,            true,  506, "Variant Also Negotiates"          )\
    XX(insufficient_storage,               true,  507, "Insufficient Storage"             )\
    XX(loop_detected,                      true,  508, "Loop Detected"                    )\
    XX(network_authentication_required,    true,  511, "Network Authentication Required"  )\
    XX(client_closed,                      false, 000, "Client Closed Connection"         )\

/** Adaptive Framework error codes enum. */
typedef enum afw_error_code_e {
    afw_error_code_is_not_specified = 0,
    
#define XX(id, error_allow_in_response, http_response_code,  description) \
    afw_error_code_ ## id,
    AFW_ERROR_CODE_MAP(XX)
#undef XX

    /**
     * The Adaptive framework will not use an error code higher than this.
     * Error codes higher than this can be assigned by users of Adaptive
     * Framework.  Use this symbol plus a value for user error codes, since
     * its value may possibly change someday.
     */
    afw_error_code_highest_afw = 1000

} afw_error_code_t;



/**
 * @brief Typedef for pool cleanup functions.
 * @param data supplied on cleanup register.
 * @param data2 supplied on cleanup register.
 * @param p being cleaned up.
 * @param xctx of caller to cleanup.
 */
typedef void (*afw_pool_cleanup_function_p_t)(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Typedef for callback to get a qualified variable from a stack entry.
 * @param entry Qualifier stack entry.
 * @param name Name of variable.
 * @param xctx of caller.
 * @return Non-NULL if this frame defines @p name (use permanent singletons
 *    afw_value_undefined / afw_value_null for present nullish values); C NULL
 *    if @p name is not defined on this frame.
 *
 * Used as get_cb on afw_xctx_qualifier_stack_entry_t. The stack walks matching
 * visible frames newest → oldest and uses the first non-NULL return (first
 * defining frame wins). Do not return C NULL for a present undefined value —
 * return afw_value_undefined (permanent singleton, pointer identity).
 *
 * This contract is for **qualifier frames** only. Unqualified lexical `let` /
 * params use scope symbol slots (see afw_xctx_get_optionally_qualified_variable
 * and afw_xctx_scope_symbol_exists_by_name); do not assume get_cb rules apply
 * to bare names.
 */
typedef const afw_value_t *
(*afw_xctx_get_variable_cb_t)(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx);


/**
 * @brief Typedef for callback to contribute variables into a snapshot object.
 * @param entry Qualifier stack entry.
 * @param object Accumulator object; only add property names not already present.
 * @param include_untrusted See afw_xctx_qualifier_object_create(); most
 *    contributes ignore this (frame selection is done by the snapshot builder).
 * @param xctx of caller.
 *
 * Used as contribute_cb on afw_xctx_qualifier_stack_entry_t for qualifier() /
 * qualifiers() listing (issue #9). Not for the hot qualifier::name path.
 * Values should match what get_cb would return.
 */
typedef void
(*afw_xctx_contribute_variables_cb_t)(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_object_t *object,
    afw_boolean_t include_untrusted,
    afw_xctx_t *xctx);


/** @brief Log levels.  See afw_log.h for more information. */    
typedef enum afw_log_priority_e {

    afw_log_priority_invalid   = -1,   /** Invalid log level          */

    /* These go to the configured log. */
    afw_log_priority_min       = 0,    /** Min syslog log priority level. */
    afw_log_priority_emerg     = 0,    /** System unusable                */
    afw_log_priority_alert     = 1,    /** Immediate action required.     */
    afw_log_priority_crit      = 2,    /** Critical errors.               */
    afw_log_priority_err       = 3,    /** Errors.                        */
    afw_log_priority_warning   = 4,    /** Warnings.                      */
    afw_log_priority_notice    = 5,    /** Normal but significant.        */
    afw_log_priority_info      = 6,    /** Informational.                 */
    afw_log_priority_debug     = 7,    /** Debug message.                 */
    afw_log_priority_max       = 7,    /** Max syslog log priority level. */
    
    /* These go to the configured trace destination. */
    afw_log_priority_trace_min = 8,    /** Min trace priority level       */
    afw_log_priority_trace1    = 8,    /** Trace level 1 message.         */
    afw_log_priority_trace2    = 9,    /** Trace level 2 message.         */
    afw_log_priority_trace3    = 10,   /** Trace level 3 message.         */
    afw_log_priority_trace4    = 11,   /** Trace level 4 message.         */
    afw_log_priority_trace5    = 12,   /** Trace level 5 message.         */
    afw_log_priority_trace6    = 13,   /** Trace level 6 message.         */
    afw_log_priority_trace7    = 14,   /** Trace level 7 message.         */
    afw_log_priority_trace8    = 15,   /** Trace level 8 message.         */
    afw_log_priority_trace_max = 15    /** Max trace priority level       */
    
} afw_log_priority_t;


/** @brief Typedef for afw_adapter_journal get_entry options enum.
 *
 * The options use zero of more of these parameters:
 *
 * parameter    | description
 * ------------ | ----------- 
 * entry_cursor | The objectId of a journal entry.
 * consumer_id  | The objectId of an _AdaptiveProvisioningPeer_ object.
 * limit        | The maximum number of entries that will be scanned for an entry where the consumerFilter expression in the associated _AdaptiveProvisioningPeer_ object evaluates to true. 
 *
 * Depending on option, get_entry() sets zero or more of these properties in
 * the response:
 *
 * property     | description
 * ------------ | -----------
 * entry        | If a journal entry is retrieved, get_entry() will set this Object property to the entry.  If the option is a "get next" option and there are no applicable entries to return, this property will remain unset.
 * entryCursor  | If a journal entry is retrieved, get_entry() will set this Sting property to its objectId, also know as its entryCursor.
 * reissue      | If the object retrieved is a reissue of one previously retrieved, this Boolean property will be set to true.
 */
typedef enum afw_adapter_journal_option_e {

    /**
     * @brief afw_adapter_journal get_entry option get_first
     *
     * Get first journal entry. Parameters entry_cursor, consumer_id, and
     * limit are ignored.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * there is a first entry to return.
     */
    afw_adapter_journal_option_get_first,
 
    /**
     * @brief afw_adapter_journal get_entry option get_by_cursor
     *
     * Get journal entry specified by entry_cursor parameter.  Parameters
     * consumer_id and limit are ignored.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * there is an entry to retrieve.  If an entry with the supplied
     * entryCursor does not exist, a not_found error is thrown.
     */
    afw_adapter_journal_option_get_by_cursor,
    
    /**
     * @brief afw_adapter_journal get_entry option get_next_after_cursor
     *
     * Get the next journal entry after the one specified by the entry_cursor
     * parameter.  Parameters consumer_id and limit are ignored.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * there is next entry to retrieve.
     */
    afw_adapter_journal_option_get_next_after_cursor,
    
    /**
     * @brief afw_adapter_journal get_entry option get_next_for_consumer
     *
     * Get the next journal entry for a consumer referenced by the
     * consumer_id parameter.  The entry_cursor parameter is ignored.
     * The limit parameter specifies the maximum number of entries 
     * to scan for an applicable entry for consumer before returning.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * an applicable entry is retrieved.  Property "reissue" will be set as
     * described below.
     * 
     * The properties of the _AdaptiveProvisioningPeer_ object associated
     * with the consumer_id are used in the following way:
     * 
     * The consumerFilter expression is used to determine if an entry is
     * applicable.
     * 
     * If consumeCursor property exists, return that entry at that cursor
     * again immediately with a "reissue" property added and set to true.
     *
     * Journal entries are scanned beginning at the entry at the cursor
     * in the advanceCursor property.  If the advanceCursor property is not
     * present, the scan begins after the cursor in currentCursor.  If
     * neither are present, the scan begins at the start of the journal.
     * 
     * If an applicable entry is found, properties consumeStart and 
     * consumeCursor are set, advanceCursor is removed, and the entry is
     * returned.  Method mark_entry_consumed() will remove these properties.
     *
     * If no applicable entry is found, advanceCursor is set to the last
     * entry scanned.
     */
    afw_adapter_journal_option_get_next_for_consumer,
    
    /**
     * @brief afw_adapter_journal get_entry option
     *     get_next_for_consumer_after_cursor
     *
     * Get the next journal entry for a consumer referenced by the
     * consumer_id after the one specified by the entry_cursor parameter. The
     * limit parameter specifies the maximum number of entries to scan for an
     * applicable entry for consumer before returning.
     * 
     * This option will set response properties "entry" and "entryCursor" if
     * an applicable entry is retrieved.
     * 
     * The properties of the _AdaptiveProvisioningPeer_ object associated
     * with the consumer_id are used in the following way:
     * 
     * The consumerFilter expression is used to determine if an entry is
     * applicable.
     * 
     * Unlike option get_next_for_consumer, no other properties are
     * referenced or modified.
     */
    afw_adapter_journal_option_get_next_for_consumer_after_cursor,
 
    /**
     * @brief afw_adapter_journal get_entry option advance_cursor_for_consumer
     *
     * Update the advance cursor for a consumer referenced by the
     * consumer_id parameter.  The entry_cursor parameter is ignored.
     * The limit parameter specifies the maximum number of entries 
     * to scan for an applicable entry for consumer before returning.
     * NULL is always returned.
     *
     * There are no response properties set by this function.
     * 
     * The properties of the _AdaptiveProvisioningPeer_ object associated
     * with the consumer_id are used in the following way:
     * 
     * The consumerFilter expression is used to determine if an entry is
     * applicable.
     * 
     * Journal entries are scanned beginning at the entry at the cursor
     * in the advanceCursor property.  If the advanceCursor property is not
     * present, the scan begins after the cursor in currentCursor.  If
     * neither are present, the scan begins at the start of the journal.
     *
     * If an new applicable entry is found or if the limit is met, the
     * advanceCursor property is set to the currently scanned entry's cursor.
     */
    afw_adapter_journal_option_advance_cursor_for_consumer
    
} afw_adapter_journal_option_t;


/** 
 * @brief Typedef for afw_adapter_session_object callback.
 * @param object pointer or NULL.
 * @param context supplied with callback
 * @param xctx of caller
 * @return true if the callback function requests that it not be called again.
 *
 * The purpose of returning true from a callback function is to short circuit
 * a retrieve request when the callback has determined that it is "complete".
 * Unless the callback has a special purpose like this, it should always
 * return false so that it will continue to called until there are no more
 * objects.
 * 
 * This is the callback passed to afw_adapter_session_retrieve_objects() and
 * afw_adapter_session_get_object().
 * 
 * For afw_adapter_session_retrieve_objects(), it is called each time an object
 * is retrieved and once with an object pointer when finished.
 *
 * For afw_adapter_session_get_object(), it is called once, either with the
 * object retrieved or NULL if not found.
 *
 * The callback function should call afw_object_release() on object once
 * finished with it.
 */
typedef afw_boolean_t
(*afw_object_cb_t)(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx);
 

/** 
 * @brief Typedef for value callback.
 * @param value void pointer or NULL.
 * @param context supplied with callback
 * @param xctx of caller
 * @return true if the callback function requests that it not be called again.
 */
typedef afw_boolean_t
(*afw_value_cb_t)(
    const void *value,
    void *context,
    afw_xctx_t *xctx);
 

/**
 * @brief Typedef for read callback function.
 * @param context supplied with callback.
 * @param buffer where bytes will be read into.
 * @param size is the number of bytes to read. A value of -1 reads a line.
 * @param more_to_read indicates that there is possibly more input to read.
 * @param p is pool to use if resources are needed.
 * @param xctx of caller.
 * @return number of bytes read.
 */
typedef afw_size_t
(*afw_read_cb_t)(
    void *context,
    const void * buffer,
    afw_size_t size,
    afw_boolean_t *more_to_read,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Typedef for write callback function.
 * @param context supplied with callback
 * @param buffer containing bytes to written.
 * @param size is the number of bytes to write.
 * @param p is pool to use if resources are needed.
 * @param xctx of caller.
 * @return number of bytes written.
 */
typedef afw_size_t
(*afw_write_cb_t)(
    void *context,
    const void * buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx);
    
/*
 * @brief Runtime value accessor function.
 * @param property is associated afw_runtime_object_map_property_t *.
 * @param internal value.
 * @param p pool to use.
 * @param xctx xctx of caller.
 */
typedef const afw_value_t *
(*afw_runtime_value_accessor_t) (
    const afw_runtime_object_map_property_t * property,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Typedef for function execute functions.
 * @param x Function execute struct.
 * @return (const afw_value_t *) value
 */
typedef const afw_value_t *
(*afw_function_execute_cb_t) (afw_function_execute_t *x);



/**
 * @brief Typedef for function thunk execute functions.
 * @param function Evaluated argv[0] and if polymorphic, specific instance.
 * @param argc The number of function parameters (does not include argv[0]).
 * @param argv The function to call argv[0] followed by function parameters.
 * @param p to use.
 * @param xctx execution context.
 * @return (const afw_value_t *) value
 */
typedef const afw_value_t *
(*afw_function_thunk_execute_t) (
    const afw_value_function_thunk_t *function,
    afw_size_t argc,
    const afw_value_t *const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Typedef for function arg check functions.
 * @param x The function execute struct.
 * @param argc The number of function parameters (does not include argv[0]).
 * @param argv The function to call argv[0] followed by function parameters.
 * @param p The memory pool allocated by the expression compiler.
 * @param xctx of the caller.
 * @return NULL if check passed or reason if it didn't.
 *
 * This function does any addition argument checking that may be needed
 * and is only called when a expression is being compiled, NOT each time a
 * request is processed.
 */
typedef const afw_utf8_t *
(*afw_function_arg_check_t)(
    afw_function_execute_t *x,
    afw_size_t argc,
    const afw_value_t * argv[],
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/** @brief Map used for afw_service_startup_t enum. */
#define AFW_SERVICE_STARTUP_MAP(XX)                                             \
    XX(disabled,    "Service is disabled and cannot be started."               )\
    XX(permanent,   "Service is permanently started and can not be stopped."   )\
    XX(manual,      "Service is started and stopped manually."                 )\
    XX(immediate,   "Service is started immediately and can be stopped and "    \
                    "started manually."    )\

/**
 * @brief Typedef for service startup type enum.
 * 
 * This is used by the afw_service interface definition.  See
 * afw_service.h for more information.
 */
typedef enum afw_service_startup_e {

#define XX(id, desc) afw_service_startup_ ## id,
    AFW_SERVICE_STARTUP_MAP(XX)
#undef XX

    afw_service_startup_invalid
} afw_service_startup_t;

/** @brief Map used for afw_service_status_t enum. */
#define AFW_SERVICE_STATUS_MAP(XX)                                              \
    XX(error,    "Service is not running because of an error."                 )\
    XX(disabled, "Service is disabled."                                        )\
    XX(ready_to_start, "Service is ready to start."                            )\
    XX(starting, "Service is starting."                                        )\
    XX(running,  "Service is running."                                         )\
    XX(stopping, "Service is finishing active work and releasing resources."   )\
    XX(stopped,  "Service is stopped and can be started manually."             )\
    XX(restarting,"Service is restarting."                                     )\

/**
 * @brief Typedef for service status enum.
 * 
 * This is used by the afw_service interface definition.  See
 * afw_service.h for more information.
 */
typedef enum afw_service_status_e {

#define XX(id, desc) afw_service_status_ ## id,
    AFW_SERVICE_STATUS_MAP(XX)
#undef XX

    afw_service_status_invalid
} afw_service_status_t;


/** @brief Thread start calling convention (empty on POSIX). */
#define AFW_THREAD_FUNCTION /* empty */

/** @brief Typedef for thread start function. */
typedef void
*(AFW_THREAD_FUNCTION *afw_thread_function_t)
    (const afw_thread_t *thread, void *arg);

/** @brief Typedef for union of all cType. */
typedef union afw_c_types_u {

    afw_integer_t   integer;
    afw_integer_t   integer_p;

    afw_int8_t      int8;
    afw_int8_t    * int8_p;

    afw_uint8_t     uint8;
    afw_uint8_t   * uint8_p;

    afw_int16_t     int16;
    afw_int16_t   * int16_p;

    afw_uint16_t    uint16;
    afw_uint16_t  * uint16_p;

    afw_int32_t     int32;
    afw_int32_t   * int32_p;

    afw_uint32_t    uint32;
    afw_uint32_t  * uint32_p;

    afw_int64_t     int64;
    afw_int64_t   * int64_p;

    afw_uint64_t    uint64;
    afw_uint64_t  * uint64_p;

    afw_array_t    * list_p;

    afw_float32_t   float32;
    afw_float32_t * float32_p;

    afw_float64_t   float64;
    afw_float64_t * float64_p;

    afw_float_t     floating;
    afw_float_t   * floating_p;

    afw_object_t  * object_p;

    afw_utf8_t    string;
    afw_utf8_t  * string_p;

} afw_c_types_t;


/**
 * @brief Time with offset (old)
 *
 * This typedef is used when time needs to be local time and a UTC offset
 * instead of UTC time.
 */
typedef struct afw_time_with_offset_s {
    afw_integer_t time;
    afw_integer_t offset;
} afw_time_with_offset_t;


/**
 * @brief yearMonth duration
 *
 * If negative duration all members will/MUST be negative or zero.
 */
typedef struct afw_yearMonthDuration_s {
    afw_int32_t  years;
    afw_int32_t  months;
} afw_yearMonthDuration_t;


/**
 * @brief dayTime duration
 * 
 * If negative duration, all members will/MUST be negative or zero.
 */
typedef struct afw_dayTimeDuration_s {
    afw_int32_t days;
    afw_int32_t hours;
    afw_int32_t minutes;
    afw_int32_t seconds;
    afw_int32_t microseconds;
} afw_dayTimeDuration_t;


/**
 * @brief Time duration 
 */
typedef struct afw_time_duration_s {
    afw_yearMonthDuration_t yearMonth;
    afw_dayTimeDuration_t dayTime;
} afw_time_duration_t;


/**
 * @brief time zone
 */
typedef struct afw_time_zone_s {
    /**
     * Hours (-14 - +14).  If +14 or -14, minutes must be 0.
     */
    afw_int8_t hours;

    /**
     * Minutes (0 - 59 or -1).  -1 indicates no time zone, in which case,
     * hours must be 0.
     */
    afw_int8_t minutes;
} afw_time_zone_t;


/**
 * @brief date with no time zone.
 */
typedef struct afw_date_no_time_zone_s {

    /** Year including century. Negative is BC. No 0? */
    afw_int32_t year;

    /** Month of year (1-12). */
    afw_uint8_t month;

    /** Day day of month (1-31).  Month limited to days in month. */ 
    afw_uint8_t day;

} afw_date_no_time_zone_t;


/**
 * @brief date with time zone.
 */
typedef struct afw_date_s {
    
    /** Date. */
    afw_date_no_time_zone_t date;

    /** Time zone. */
    afw_time_zone_t time_zone;

} afw_date_t;


/**
 * @brief time with no time zone.
 */
typedef struct afw_time_no_time_zone_s {

    /** Hour (0-24).  If hour is 24 then other values must be 0. */
    afw_uint8_t hour;

    /** Minute (0-59). */
    afw_uint8_t minute;

    /** Second (0-60+). 60 or more is allowed for leap seconds. */ 
    afw_uint8_t second;

    /** Microsecond (0-999999). */ 
    afw_uint32_t microsecond;

} afw_time_no_time_zone_t;


/**
 * @brief time with time zone.
 */
typedef struct afw_time_s {

    /** Time */
    afw_time_no_time_zone_t time;

    /** Time zone. */
    afw_time_zone_t time_zone;

} afw_time_t;


/**
 * @brief date, time, and time zone.
 */
typedef struct afw_dateTime_s {

    /** Date. */
    afw_date_no_time_zone_t date;

    /** time */
    afw_time_no_time_zone_t time;

    /** Time zone. */
    afw_time_zone_t time_zone;

} afw_dateTime_t;


/**
 * @brief Typedef for service wrapper start call back.
 *
 * See afw_service.h for more information.
 */
typedef void
(*afw_service_wrapper_start_cb)(
    void *data, afw_xctx_t *xctx);


/**
 * @brief Typedef for service wrapper stop call back.
 *
 * See afw_service.h for more information.
 */
typedef void
(*afw_service_wrapper_stop_cb)(
    void *data, afw_xctx_t *xctx);




/*
 * Data-type typedefs (`afw_value_hexBinary_t`, …). After the complete
 * internals in this header (`afw_yearMonthDuration_t`, …). This
 * generated file includes afw_minimal.h, so it cannot sit next to
 * afw_interface_opaques.h.
 */
#include "afw_data_type_typedefs.h"

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_COMMON_H__ */

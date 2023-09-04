// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Common Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMMON_H__
#define __AFW_COMMON_H__

/* Need to include doxygen groups. */
#include "afw_doxygen.h"


/**
 * @defgroup afw_common Common
 * @ingroup afw_c_api_public
 *
 * Common to all Adaptive Framework applications
 *
 * @{
 */

/**
 * @file afw_common.h
 * @brief Adaptive Framework Common Header.
 * 
 * This header contains `#include`s for common headers that do not have other
 * Adaptive Framework dependencies, typedefs required by afw_interface.h,
 * as well as other `#define`s and typedefs that are common to all Adaptive
 * Framework applications.
 *
 * This header is included by afw_interface.h, afw_minimal.h, and afw.h, there
 * is no need to include it directly.
 *
 * The major Adaptive Framework headers are arranged as follows:
 *
 * Major AFW header        | description
 * ----------------------- | -----------
 * afw_common.h            | This header.  Included by afw_interface.h, afw_minimal.h, and afw.h.
 * afw_common_opaques.h    | Contains common opaque typedefs. Included by afw_common.h.
 * afw_interface.h         | Generated AFW interface header.  Can be included in all headers other than opaque only ones.
 * afw_interface_opaques.h | Generated AFW interface opaques.  Included by afw_common.h.
 * afw_minimal.h           | Contains `#include`s for some core headers that only `#include` afw_interface.h.  Can be included in headers that are not included by afw_minimal.h.
 * afw.h                   | Contains `#include`s for most core headers and will generally be included in all Adaptive Framework .c files.  
 */

/* For pool debugging use  -D AFW_POOL_DEBUG */
/* For lock debugging use -D AFW_LOCK_DEBUG */

/* Adaptive Framework uses Apache Portable Runtime. */
#include <apr_general.h>
#include <apr_tables.h>
#include <apr_strings.h>
#include <apr_time.h>
#include <apr_hash.h>
#include <apr_buckets.h>
#include <apr_atomic.h>
#include <apr_thread_proc.h>
#include <apr_thread_rwlock.h>


/* Common standard headers. */
#include <assert.h>
#include <limits.h>
#include <setjmp.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

 
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

/* Include generated declare helpers. */
#include "afw_declare_helpers.h"

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

/** @fixme Remove this as soon as no source contains it. */
#define AFW_ASSERT(e) assert(e)

/** @fixme */
#ifndef offsetof
#define offsetof(type, member)  __builtin_offsetof (type, member)
#endif

AFW_BEGIN_DECLARES

/** Adaptive Framework's core adaptor id. */
#define AFW_ADAPTOR_ID "afw"

/** @brief Evaluate x and quote the results. */
#define AFW_STRINGIFY(x) AFW_STRINGIFY_x(x)
#define AFW_STRINGIFY_x(x) #x

/** @brief file:line */
#define AFW__FILE_LINE__ __FILE__ ":" AFW_STRINGIFY(__LINE__)

/** @brief size_t. */
typedef apr_size_t afw_size_t;

/** @brief off_t. */
typedef apr_off_t afw_off_t;

/** @brief 8-bit signed integer. */
typedef int8_t afw_int8_t;

/** @brief 16-bit signed integer. */
typedef apr_int16_t afw_int16_t;

/** @brief 32-bit signed integer. */
typedef apr_int32_t afw_int32_t;

/** @brief Min int32. */
#define AFW_INT32_MIN -2147483647 - 1

/** @brief Max int32. */
#define AFW_INT32_MAX 2147483647

/** @brief 64-bit signed integer. */
typedef apr_int64_t afw_int64_t;

/** @brief 8-bit unsigned integer. */
typedef uint8_t afw_uint8_t;

/** @brief 16-bit unsigned integer. */
typedef apr_uint16_t afw_uint16_t;

/** @brief Max uint32. */
#define AFW_UINT16_MAX 65536

/** @brief 32-bit unsigned integer. */
typedef apr_uint32_t afw_uint32_t;

/** @brief Max uint32. */
#define AFW_UINT32_MAX 4294967296

/** @brief 64-bit unsigned integer. */
typedef apr_uint64_t afw_uint64_t;

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

/** * @brief A byte of memory (unsigned). */
typedef unsigned char afw_byte_t;

/** * @brief 8 bits (unsigned). */
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
#define AFW_INTEGER_MAX  APR_INT64_MAX

/**
 * @brief largest afw_integer_t quoted
 */
#define AFW_INTEGER_Q_MAX "9223372036854775807"

/**
 * @brief smallest afw_integer_t
 */
#define AFW_INTEGER_MIN APR_INT64_MIN

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
typedef apr_int64_t afw_integer_t;

/**
 * @brief Format string specifier used for afw_integer_t.
 */
#define AFW_INTEGER_FMT "%" APR_INT64_T_FMT

/**
 * @brief Format string specifier used for afw_integer_t without %.
 * 
 * This can be used if additional specifiers are needed. For example,
 * printf("The number is %0" AFW_INTEGER_FMT_NO_PERCENT "\n", number);
 */
#define AFW_INTEGER_FMT_NO_PERCENT APR_INT64_T_FMT

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
#define AFW_SIZE_T_FMT "%" APR_SIZE_T_FMT

/**
 * @brief Format string specifier used for afw_size_t without %.
 * 
 * This can be used if additional specifiers are needed. For example,
 * printf("The size is %0" AFW_SIZE_T_FMT_NO_PERCENT "\n", size);
 */
#define AFW_SIZE_T_FMT_NO_PERCENT APR_SIZE_T_FMT

/**
 * @brief afw_size_t max.
 */
#define AFW_SIZE_T_MAX APR_SIZE_MAX

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
typedef apr_uint64_t afw_unsigned_integer_t;


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
    XX(expression,                                                              \
        expression,                                                             \
        &afw_function_definition_compile_expression,                            \
        "See production Expression in src/afw/doc/syntax/index.html or "        \
        "src/afw/generated/ebnf/syntax.ebnf for syntax.")                       \
                                                                                \
    XX(hybrid,                                                                  \
        hybrid,                                                                 \
        &afw_function_definition_compile_hybrid,                                \
        "See production Hybrid in src/afw/doc/syntax/index.html or "            \
        "src/afw/generated/ebnf/syntax.ebnf for syntax.")                       \
                                                                                \
    XX(json,                                                                    \
        string,                                                                 \
        &afw_function_definition_compile_json,                                  \
        "Compile strict json")                                                  \
                                                                                \
    XX(parenthesized_expression,                                                \
        expression,                                                             \
        &afw_function_definition_compile_expression,                            \
        "See production ParenthesizedExpression in "                            \
        "src/afw/doc/syntax/index.html "                                        \
        "or src/afw/generated/ebnf/syntax.ebnf for syntax.")                    \
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
        "3) Allows Infinity, -Infinity, NaN, and -NaN for number values.\n"    \
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
        "XML Path Language (XPath) expression")
 
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
 * @brief Struct for memory pointer and size.
 *
 * IMPORTANT:  This must match up with afw_utf8_t since it is accepted to
 *            cast from afw_utf8_t to afw_memory_t.  Don't cast the other way
 *            around unless you are positive ptr point to NFC Unicode --
 *            use afw_utf8_from_raw() instead.
 */
typedef struct afw_memory_s {
    const afw_byte_t *ptr;
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
 *            use afw_utf8_from_raw() instead.
 *
 * All UTF-8 strings MUST be NFC normalized.  Doing otherwise can cause
 * unintended minor or major errors.
 *
 * See RFC 5198 and http://unicode.org/reports/tr15/
 *
 * This string is not null terminated, so it can contain NUL characters. The
 * number of bytes in the sting is stored in len and a pointer to the first
 * byte in s.
 * 
 * @see afw_utf8_octet_t for more information.
 */
typedef struct afw_utf8_s {
    const afw_utf8_octet_t *s;
    afw_size_t len;
} afw_utf8_t;



/**
 * @brief NFC normalized UTF-8 string accessible as afw_utf8_t or afw_utf8_z_t
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
 * @brief String literal initializer.
 *
 * Example of use:
 *
 * static const afw_utf8_t hello = AFW_UTF8_LITERAL("Hello World");
 *
 */
#define AFW_UTF8_LITERAL(A_STRING) {A_STRING, sizeof(A_STRING) - 1}


/**
 * @brief Format string specifier used for afw_utf8_t.
 */
#define AFW_UTF8_FMT "%.*s"

/**
 * @brief Format quoted string specifier used for afw_utf8_t.
 */
#define AFW_UTF8_FMT_Q "'%.*s'"


/** @brief Format string used for source location separator. */
#define AFW_UTF8_CONTEXTUAL_LABEL_FMT_SEP "> "


/** @brief Format string used for source location. */
#define AFW_UTF8_CONTEXTUAL_LABEL_FMT \
AFW_UTF8_FMT AFW_UTF8_CONTEXTUAL_LABEL_FMT_SEP

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
 * @brief Convenience Macro for use with AFW_UTF8_FMT to specify optional arg.
 * @param A_STRING a (const afw_utf8_t *) string or NULL.
 *
 * The argument for " AFW_UTF8_FMT " is an int length (not size_t) followed by
 * a comma and const char *.  
 */
#define AFW_UTF8_FMT_OPTIONAL_ARG(A_STRING) \
    (A_STRING) ? (int)(A_STRING)->len : 0, \
    (A_STRING) ? (const char *)(A_STRING)->s : ""


/**
 * @brief Convenience Macro for use with AFW_UTF8_FMT to specify optional arg.
 * @param A_STRING a (const afw_utf8_t *) string or undefined.
 *
 * The argument for " AFW_UTF8_FMT " is an int length (not size_t) followed by
 * a comma and const char *.  If NULL, `<undefined>` is supplied.
 */
#define AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(A_STRING) \
    (A_STRING) ? (int)(A_STRING)->len : 0, \
    (A_STRING) ? (const char *)(A_STRING)->s : "<undefined>"


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
 * @brief Struct for afw_stack_t typedef
 */
struct afw_stack_s {
    const afw_pool_t *p;
    void *first;
    void *top;
    void *end;
};


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
 * @brief Typedef for key/string pair that have both utf8 and utf8_z.
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
 * @brief Define a struct for a stack with the specified entry type.
 * @param struct_name of struct.
 * @param entry_type typedef name.
 *
 * The member variables must correspond to afw_stack_s, but with the specified
 * entry_type instead of void for the type of first, top, and end.
 */
#define AFW_STACK_STRUCT(struct_name, entry_type) \
struct struct_name { \
    union { \
        afw_stack_t pub; \
        struct { \
            const afw_pool_t *p; \
            entry_type *first; \
            entry_type *top; \
            entry_type *end; \
        }; \
    }; \
}

/**
 * @brief afw_utf8_t stack struct.
 */
AFW_STACK_STRUCT(afw_utf8_stack_s, afw_utf8_t);

/**
 * @brief const afw_utf8_t * stack struct.
 */
AFW_STACK_STRUCT(afw_const_utf8_a_stack_s, const afw_utf8_t *);

/**
 * @brief const afw_utf8_t * stack struct.
 */
AFW_STACK_STRUCT(afw_name_value_stack_s, afw_name_value_t);


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
     * /<adaptor id>/<object type id>/<object id>
     *
     * Use these functions to access:
     *     . afw_object_meta_get_id()
     *     . afw_object_meta_get_path()
     *     . afw_object_meta_get_uri()
     */
    const afw_utf8_t *object_uri;

} afw_object_meta_t;



/** @brief Error code map.
 * IMPORTANT>>> Do not change the order of these entries.  The order must match
 * the order of the error codes in afw_error_code_t as originally released since
 * the internal number will be externally compile in.
 *
 * none                     - No error has occurred.
 *
 * general                  - A general error has occurred that does not match
 *                           a more specific error code.
 *
 * evaluate                 - A error occurred while evaluating an adaptive
 *                           value.
 *
 * bad_request              - Something about the request was bad.
 *
 * query_to_complex         - The query is too complex for the particular
 *                           adaptor and/or index.
 *
 * request_syntax           - A error occurred while parsing the request.
 *
 * objects_needed           - This error will not occur if the object whose
 *                           URI is in error.additional is available in the
 *                           xctx's cache.  Function
 *                           afw_object_resolve_instance() uses this error
 *                           code when object is NULL and the object with the
 *                           URI passed is not in the cache.
 *
 * authentication_required  - Subject authentication required.
 *
 * denied                   - The caller is not authorized to perform the
 *                           request. See error.additional for more
 *                           information.
 *
 * read_only                - Something is read only.  See error.additional
 *                           for an indication of what is write protected.
 *
 * not_found                - Something was not found.  If "not found" is a
 *                           normal condition that might happen in a function
 *                           or method, returning NULL or using a parameter
 *                           should communicate "not found" instead of
 *                           throwing an error with this error code.  For
 *                           example, function afw_object_get_property() sets
 *                           its "found" parameter to AFW_FALSE and
 *                           afw_object_resolve_instance() return NULL to
 *                           indicate "not found".
 *
 * method_not_allowed       - Method is not allowed for this resource.
 *
 * unsupported_accept       - None of the request's acceptable response
 *                           content types are supported.
 *
 * client_time_out          - Timed out waiting on client.
 *
 * conflict                 - Conflict such as resourse in use.
 *
 * length_required          - Request requires content length.
 * 
 * unsupported_content_type - The request's content type is not supported.
 *
 * memory                   - There is not enough memory available to continue.
 *
 * syntax                   - A server side syntax error occurred.
 *
 * method_not_supported     - The implementation of this interface does not
 *                           support this method.
 *
 * client_closed            - Client closed connection.
 *
 *
 * Map has id, error_allow_in_response, http_response_code, and description.
 * 
 * id: Id of error code
 * error_allow_in_response: If an error is thrown with this id, it's allowed
 *     for the HTTP response to include the error object.
 * http_response_code: The HTTP response code to use if this error is thrown.
 * description: A very short description that will be included in after the
 *     http_response_code in the status message.
 *
 */
#define AFW_ERROR_CODE_MAP(XX)                                                  \
    XX(none,                   true,  200, "OK"                                )\
    XX(throw,                  true,  400, "Statement throw encountered"       )\
    XX(general,                true,  500, "General Error"                     )\
    XX(bad_request,            true,  400, "Bad Request"                       )\
    XX(query_too_complex,      true,  400, "Query Too Complex"                 )\
    XX(request_syntax,         true,  400, "Request Syntax Error"              )\
    XX(objects_needed,         true,  400, "Objects Needed To Complete Request")\
    XX(authentication_required,true,  401, "Authentication Needed"             )\
    XX(denied,                 true,  403, "Forbidden - Access Denied"         )\
    XX(read_only,              true,  403, "Forbidden - Read Only"             )\
    XX(not_found,              true,  404, "Not Found"                         )\
    XX(method_not_allowed,     true,  405, "Method Not Allowed"                )\
    XX(unsupported_accept,     false, 406, "Unsupported Content Type Requested")\
    XX(client_time_out,        true,  408, "Request Timeout"                   )\
    XX(im_a_teapot,            true,  418, "I'm a Teapot"                      )\
    XX(conflict,               true,  409, "Conflict"                          )\
    XX(length_required,        true,  411, "Content Length Required"           )\
    XX(payload_too_large,      true,  413, "Payload too large"                 )\
    XX(unsupported_content,    true,  415, "Unsupported Content Type"          )\
    XX(memory,                 true,  500, "Memory Error"                      )\
    XX(syntax,                 true,  500, "Server Side Syntax Error"          )\
    XX(method_not_supported,   true,  501, "Method Not Supported"              )\
    XX(client_closed,          false, 000, "Client Closed Connection"          )\
    XX(assertion_failed,       true,  400, "Assertion failed"                  )\
    XX(cast_error,             true,  400, "Adaptive Type Cast Error"          )\
    XX(arg_error,              true,  400, "Adaptive Function Arg Error"       )\
    XX(evaluate,               true,  400, "Evaluation error"                  )\
    XX(undefined,              true,  400, "Undefined value"                   )\
    XX(code,                   true,  500, "Clearly an internal coding error"  )\

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
 * @brief Typedef for function to get a qualified variable.
 * @param entry Qualifier stack entry.
 * @param name Name of variable.
 * @param xctx of caller.
 * @return value or NULL if not found.
 *
 * The stack is searched from newest to oldest.
 */
typedef const afw_value_t *
(*afw_xctx_get_variable_t)(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
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


/** @brief Typedef for afw_adaptor_journal get_entry options enum.
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
typedef enum afw_adaptor_journal_option_e {

    /**
     * @brief afw_adaptor_journal get_entry option get_first
     *
     * Get first journal entry. Parameters entry_cursor, consumer_id, and
     * limit are ignored.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * there is a first entry to return.
     */
    afw_adaptor_journal_option_get_first,
 
    /**
     * @brief afw_adaptor_journal get_entry option get_by_cursor
     *
     * Get journal entry specified by entry_cursor parameter.  Parameters
     * consumer_id and limit are ignored.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * there is an entry to retrieve.  If an entry with the supplied
     * entryCursor does not exist, a not_found error is thrown.
     */
    afw_adaptor_journal_option_get_by_cursor,
    
    /**
     * @brief afw_adaptor_journal get_entry option get_next_after_cursor
     *
     * Get the next journal entry after the one specified by the entry_cursor
     * parameter.  Parameters consumer_id and limit are ignored.
     *
     * This option will set response properties "entry" and "entryCursor" if
     * there is next entry to retrieve.
     */
    afw_adaptor_journal_option_get_next_after_cursor,
    
    /**
     * @brief afw_adaptor_journal get_entry option get_next_for_consumer
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
    afw_adaptor_journal_option_get_next_for_consumer,
    
    /**
     * @brief afw_adaptor_journal get_entry option
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
    afw_adaptor_journal_option_get_next_for_consumer_after_cursor,
 
    /**
     * @brief afw_adaptor_journal get_entry option advance_cursor_for_consumer
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
    afw_adaptor_journal_option_advance_cursor_for_consumer
    
} afw_adaptor_journal_option_t;


/** 
 * @brief Typedef for afw_adaptor_session_object callback.
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
 * This is the callback passed to afw_adaptor_session_retrieve_objects() and
 * afw_adaptor_session_get_object().
 * 
 * For afw_adaptor_session_retrieve_objects(), it is called each time an object
 * is retrieved and once with an object pointer when finished.
 *
 * For afw_adaptor_session_get_object(), it is called once, either with the
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


/** @brief Uses apr_thread_mutex_t asis.  See afw_thread.h. */
#define afw_thread_mutex_t apr_thread_mutex_t

/** @brief Uses apr_thread_mutex_t asis. */
#define afw_thread_mutex_create apr_thread_mutex_create

/** @brief Uses apr_thread_mutex_t asis. */
#define afw_thread_mutex_lock apr_thread_mutex_lock

/** @brief Uses apr_thread_mutex_trylock asis. */
#define afw_thread_mutex_trylock apr_thread_mutex_trylock

/** @brief Uses apr_thread_mutex_unlock asis. */
#define afw_thread_mutex_unlock apr_thread_mutex_unlock

/** @brief Uses apr_thread_mutex_destroy asis. */
#define afw_thread_mutex_destroy apr_thread_mutex_destroy

/** @brief Uses APR_THREAD_FUNC as AFW_THREAD_FUNCTION. */
#define AFW_THREAD_FUNCTION APR_THREAD_FUNC

/** @brief Typedef for thread start function. */
typedef void
*(AFW_THREAD_FUNCTION *afw_thread_function_t)
    (const afw_thread_t *thread, void *arg);

/** @brief Typedef for afw_thread_attr. */
typedef struct afw_thread_attr_s afw_thread_attr_t;

/** @brief Default for afw_environment_t evaluation_stack_initial_count. */
#define AFW_ENVIRONMENT_DEFAULT_EVALUATION_STACK_INITIAL_COUNT 100

/** @brief Default for afw_environment_t evaluation_stack_maximum_count. */
#define AFW_ENVIRONMENT_DEFAULT_EVALUATION_STACK_MAXIMUM_COUNT 200


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
    apr_time_t time;
    apr_time_t offset;
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


/** @brief Struct for typedef afw_environment_t defined in afw_common.h. */
struct afw_environment_s {

    /** @brief Pool used to hold environment. */
    const afw_pool_t *p;

    /** @brief Program name. */
    union {
        afw_utf8_t program_name;
        const afw_utf8_z_t * program_name_z;
    };

    /** @brief Environment variables at environment create. */
    const afw_object_t *initial_environment_variables;

    /** @brief Adaptive framework core adaptor. */
    const afw_adaptor_t *afw_adaptor;

    /** @brief Adaptor for application.confAdaptorId or NULL. */
    const afw_adaptor_t *conf_adaptor;

    /** @brief The id of the application. */
    afw_utf8_t application_id;

    /** @brief Application object - /afw/_AdaptiveApplication_/current. */
    const afw_object_t *application_object;

    /** brief Application qualified variables objects of compiled variables. */
    const afw_object_t *application_qualified_variables;
    
    /** @brief  rootFilePaths - /afw/_AdaptiveApplication_/current/rootFilePaths. */
    const afw_object_t *root_file_paths;

    /** @brief Custom layout adaptor or NULL. */
    const afw_utf8_t *layout_adaptor_id;

    /** @brief Director log.  This log will direct to other logs. */
    const afw_log_t *log;

    /** @brief Open file descriptor used for debug writes. Default stderr. */
    FILE *debug_fd;

    /** @brief Open file descriptor used for writing error output. Default stderr. */
    FILE *stderr_fd;

    /** @brief Open file descriptor used for writing standard output. Default stdout. */
    FILE *stdout_fd;

    /** @brief Double infinity */
    afw_double_t infinity;

    /** @brief Double infinity value */
    const afw_value_t *infinity_value;

    /** @brief Double minus infinity */
    afw_double_t minus_infinity;

    /** @brief Double minus infinity value */
    const afw_value_t *minus_infinity_value;

    /** @brief Double NaN */
    afw_double_t NaN;

    /** @brief Double NaN value */
    const afw_value_t *NaN_value;

    /** @brief Used by authorization. */
    const afw_authorization_control_t *authorization_control;

    /** @brief Used by function execution. */
    const afw_function_environment_t *function_environment;

    /** @brief initial_count used to create xctx's evaluation stack. */
    afw_size_t evaluation_stack_initial_count;

    /** @brief maximum_count used to create xctx's evaluation stack. */
    afw_size_t evaluation_stack_maximum_count;

    /** @brief Copy of director log's mask for short circuit tests. */
    afw_log_priority_mask_t log_mask;

    /** @brief Used to give unique number for pool. */
    AFW_ATOMIC afw_integer_t pool_number;

    /** @brief Indicates that environment is terminating. */
    afw_boolean_t terminating;


    /*---------- LOCKS ----------*/

    /** @brief Lock for whole environment. */
    const afw_lock_t *environment_lock;
    
    /** @brief Lock used internal to afw_pool.c. */
    const afw_lock_t *multithreaded_pool_lock;
    
    /** @brief Lock for protecting changes to adaptor id anchors. */
    const afw_lock_t *adaptor_id_anchor_lock;
    
    /**
     * @brief Lock for protecting changes to authorization handler id
     *     anchors.
     *
     * Read/write lock is needed because anchor chain can be reordered
     * while processing handlers.
     */
    const afw_lock_rw_t *authorization_handler_id_anchor_rw_lock;

    /**
     * @brief Lock for protecting changes to active log list.
     */
    const afw_lock_t *active_log_list_lock;

    /**
     * @brief Lock for protecting changes to flags (internal to afw_flag.c).
     */
    const afw_lock_t *flags_lock;


    /*---------- FLAGS ----------*/

    /** @brief The number of flags allocated in global_flags. */
    AFW_ATOMIC afw_size_t flags_count_allocated;

    /** @brief The number of flags registered. */
    AFW_ATOMIC afw_size_t flags_count_registered;

    /**
     * @brief Default flags array indexed by flag_index.
     *
     * Always get a copy of flag_count_registered and/or flag_count_allocated
     * first.  This may change to a larger set of flags if allocated is not
     * large enough to hold a new flag registration.
     *
     * Normally this will not be accessed directly since this pointer is copied
     * to xctx->flags.
     */
    AFW_ATOMIC const afw_boolean_t *default_flags;

    /**
     * @brief Flag struct indexed by flag_index.
     *
     * Always get a copy of flag_count_registered and/or flag_count_allocated
     * first.  This may change to a larger set of flags if allocated is not
     * large enough to hold a new flag registration.
     */
    const afw_flag_t * AFW_ATOMIC const *flag_by_index;

    /** @brief Flag index of compile:noImplicitAny. */
    afw_size_t flag_index_compile_noImplicitAny_active;

    /** @brief Flag index of compile:noOptimize. */
    afw_size_t flag_index_compile_noOptimize_active;

    /** @brief Flag index of debug:function_active. */
    afw_size_t flag_index_debug_function_active;

    /** @brief Flag index of debug:function_active:detail. */
    afw_size_t flag_index_debug_function_active_detail;

    /** @brief Flag index of debug:pool. */
    afw_size_t flag_index_debug_pool;

    /** @brief Flag index of debug:pool:detail. */
    afw_size_t flag_index_debug_pool_detail;

    /** @brief Flag index of response:error:backtrace. */
    afw_size_t flag_index_response_error_backtrace;

    /** @brief Flag index of response:error:backtraceEvaluation. */
    afw_size_t flag_index_response_error_backtraceEvaluation;

    /** @brief Flag index of response:error:contextual. */
    afw_size_t flag_index_response_error_contextual;

    /** @brief Flag index of response:error:hasAdditionalDetail. */
    afw_size_t flag_index_response_error_hasAdditionalDetail;

    /** @brief Flag index of trace:authorization:check. */
    afw_size_t flag_index_trace_authorization_check;

    /** @brief Flag index of trace:authorization:check:bypass. */
    afw_size_t flag_index_trace_authorization_check_bypass;

    /** @brief Flag index of trace:authorization:check:detail. */
    afw_size_t flag_index_trace_authorization_check_detail;

    /** @brief Flag index of trace:authorization:decision. */
    afw_size_t flag_index_trace_authorization_decision;

    /** @brief Flag index of trace:authorization:decision:detail. */
    afw_size_t flag_index_trace_authorization_decision_detail;

    /** @brief Flag index of trace:evaluation:detail. */
    afw_size_t flag_index_trace_evaluation_detail;

    /** @brief Flag index of trace:request. */
    afw_size_t flag_index_trace_request;

    /** @brief Flag index of trace:request:detail. */
    afw_size_t flag_index_trace_request_detail;

};


/** @brief Struct for typedef afw_xctx_t defined in afw_common.h. */
struct afw_xctx_s {

    /**
     * Default pool or execution context (xctx).
     */
    const afw_pool_t * p;

    /**
     * The execution context (xctx) name or type.
     */
    const afw_utf8_t * name;

    /**
     * The execution context (xctx) parent xctx.
     */
    afw_xctx_t * parent;

    /**
     * Adaptive Framework Environment. This points to the same environment as
     * all other execution contexts in the same Adaptive Framework application.
     */
    const afw_environment_t * env;

    /**
     * Thread associate with xctx or NULL if base xctx.
     */
    const afw_thread_t * thread;

    /**
     * Request instance associated with xctx or NULL.
     */
    const afw_request_t * request;

    /**
     * A UUID to identify this xctx.
     */
    const afw_utf8_t * uuid;

    /**
     * The execution context (xctx) properties. This is an untyped object.
     */
    const afw_object_t * properties;

    /**
     * Authorization mode value. This contains the value from one of the
     * afw_authorization_mode_id_*_value variables from afw_authorization_h.
     */
    const afw_value_t * mode;

    /**
     * Anchor for steams available in xctx. See afw_stream.h.
     */
    const afw_stream_anchor_t * stream_anchor;

    /**
     * A counter writers can increment and use to help identify the sequence
     * of writes.
     */
    afw_integer_t write_sequence;

    /**
     * Private data used by xctx implementation.
     */
    void * priv;

    /**
     * Error.
     */
    afw_error_t * error;

    /**
     * Current try.
     */
    afw_try_t * current_try;

    /**
     * Runtime objects for xctx.
     */
    const afw_runtime_objects_t * runtime_objects;

    /**
     * The number of the scopes created.
     */
    afw_size_t scope_count;

    /**
     * The execution context (xctx) runtime scope stack. Entries are
     * const afw_xctx_scope_t *.
     */
    apr_array_header_t * scope_stack;

    /**
     * This is set each time a result is produced while evaluated an adaptive
     * script. Once evaluate of a script is complete, this is the final
     * return value.
     */
    const afw_value_t * evaluation_result;

    /**
     * The execution context (xctx) evaluation stack.
     */
    afw_xctx_evaluation_stack_t * evaluation_stack;

    /**
     * The execution context (xctx) qualifier stack. Entries are
     * const afw_xctx_qualifier_stack_entry_t *.
     */
    const afw_xctx_qualifier_stack_t * qualifier_stack;

    /**
     * Internal struct used by adaptors for this xctx. May be NULL.
     */
    afw_adaptor_xctx_internal_t * adaptor_xctx_internal;

    /**
     * The execution context (xctx) cache.
     */
    afw_adaptor_internal_cache_t * cache;

    /**
     * The local dateTime when execution context was created.
     */
    afw_dateTime_t local_dateTime_when_created;

    /**
     * The UTC/Zulu dateTime when execution context was created.
     */
    afw_dateTime_t utc_dateTime_when_created;

    /**
     * The number of flags.
     */
    afw_size_t flags_count;

    /**
     * Array of boolean flags. The size is flag_count.
     */
    const afw_boolean_t * flags;

    /**
     * This indicates that xctx->flags is a mutable copy for env->flags.
     * 
     * During xctx create, xctx->flags is set to env->flags. If flags change
     * for an xctx, xctx->flags is set to a mutable copy of env->flags and this
     * variable is set to true.
     */
    afw_boolean_t flags_is_mutable_copy;

    /**
     * Error function for libxml2.
     */
    void * libxml2_error_func;

    /**
     * If true, evaluates should only used secure context variables.
     * Use AFW_XCTX_SECURE_BEGIN and AFW_XCTX_SECURE_END for afw_xctx.h to
     * modify this variable.
     */
    afw_boolean_t secure;
};


AFW_END_DECLARES

/** @} */

#endif /* __AFW_COMMON_H__ */

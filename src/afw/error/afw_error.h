// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Error Handling Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ERROR_H__
#define __AFW_ERROR_H__

#include "afw_interface.h"
#include <stdio.h>

/**
 * @defgroup afw_error Error
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework error
 *
 * Typedef afw_error_code_t is defined in afw_common.h
 * @{
 */

/**
 * @file afw_error.h
 * @brief Adaptive Framework error handling header.
 */

AFW_BEGIN_DECLARES

/** @brief Not applicable rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_NULL NULL

/**
 * @brief APR rv source id.
 *
 * Registered in afw_environment_register_core.c.
 */
#define AFW_ERROR_RV_SOURCE_ID_Z_apr "apr"

/**
 * @brief ICU rv source id.
 *
 * Registered in afw_environment_register_core.c.
 */
#define AFW_ERROR_RV_SOURCE_ID_Z_icu "icu"


/**
 * @brief Adaptive Framework Error Context.
 */
struct afw_error_context_s {
    afw_error_context_t *next;
    const afw_utf8_t *info;
};



/**
 * @brief Adaptive Framework Error.
 *
 * All but xctx is cleared on AFW_ERROR_CLEAR_PARTIAL().
 */
struct afw_error_s {
    afw_xctx_t *xctx;

    /*
     * IMPORTANT AFW_ERROR_CLEAR_PARTIAL() will clear from code up to
     * decode_rv_wa.
     */

    /** @brief Error code. */
    afw_error_code_t code;

    /** @brief File:line in source error was thrown. */
    const afw_utf8_z_t *source_z;

    /** @brief Message */
    const afw_utf8_z_t *message_z;

    /** @brief Set before throw to supply a error object "data" property. */
    const afw_value_t *data;

    /** @brief Contextual information or NULL. */
    const afw_compile_value_contextual_t *contextual;

    /** @brief This is the source of the non-zero rv. */
    const afw_utf8_z_t * rv_source_id_z;

    /** @brief Human readable decode of rv. */
    const afw_utf8_z_t * rv_decoded_z;

    /** @brief If not memory error and afw_os_backtrace() supplies one. */
    const afw_utf8_t *backtrace;

    /**
     * @brief If syntax error, this is cursor when parse error occurred or 0.
     * 
     * This is a UTF-8 octet cursor at the time the syntax error was discovered.
     * 
     * Use this as an offset in parser_source if it is available.
     */
    afw_size_t parser_cursor;

    /**
     * @brief If syntax error, this is partial/full source or NULL.
     * 
     * If this is Not NULL, this is the compiler source that was available at
     * the time of the syntax error.
     */
    const afw_utf8_t *parser_source;

    /** @brief If non-zero, this is rc, rv, or any int value related to error. */
    int rv;

    /** @brief Recursive error. */
    afw_boolean_t recursive_error;

    /** @brief Recursive error in finally. */
    afw_boolean_t recursive_error_in_finally;

    /*IMPORTANT AFW_ERROR_CLEAR_PARTIAL() will clear up to decode_rv_wa. */

    /** @brief Place to optionally hold rv_decoded_z. */
    afw_utf8_z_t decode_rv_wa[23];

    /** @brief Makes sure there is a zero terminator after decode_rv_wa. */
    afw_utf8_z_t decode_rv_wa_safety_zero_terminator;

    /** @brief Place to optionally hold message_z. */
    afw_utf8_z_t message_wa[255];

    /** @brief Makes sure there is a zero terminator after message_wa. */
    afw_utf8_z_t message_wa_safety_zero_terminator;
};

/** @fixme AFW_ERROR_RETHROW doesn't restore evaluation_stack. */

/* @brief Macro used to clear an error between code and decode_rv_wa. */
#define AFW_ERROR_CLEAR_PARTIAL(__ERROR_) \
memset( \
    (char *)&((__ERROR_)->code), \
    0, \
    ( \
        (char *)&((__ERROR_)->decode_rv_wa[0]) - \
        (char *)&((__ERROR_)->code) \
    ) \
);

/**
 * @brief error footprint
 *
 * This is used with AFW_ERROR_FOOTPRINT and AFW_THROW_ERROR_FOOTPRINT* macros
 *
 */
typedef struct afw_error_footprint_s {
    const afw_utf8_z_t *z;
    const afw_utf8_z_t *source_z;
} afw_error_footprint_t;

/**
 * @brief Set error footprint
 * @param _footprint_ is a afw_u8z_z literal associated with footprint
 *
 * This macro will set footprint.z to _footprint_ and footprint.source_z to
 * AFW__FILE_LINE__.
 *
 * Variable with the specific name footprint of type afw_error_footprint_t
 * must already be defined.
 *
 * afw_error_footprint_t footprint;
 */ 
#define AFW_ERROR_FOOTPRINT(_footprint_) \
    footprint.z = _footprint_; \
    footprint.source_z = AFW__FILE_LINE__;

/**
 * Macro used to set error and return.
 *
 * This macro should normally only be used when an xctx is not available.
 * AFW_THROW related macros should be used instead.
 *
 */
#define AFW_THROW_UNHANDLED_ERROR(unhandled_error, _ERROR, _CODE, _RV_SOURCE_ID, _RV, _MESSAGE_Z) \
do { \
    (_ERROR)->code = afw_error_code_ ## _CODE; \
    (_ERROR)->source_z = AFW__FILE_LINE__;\
    (_ERROR)->rv_source_id_z = #_RV_SOURCE_ID; \
    (_ERROR)->rv = _RV; \
    (_ERROR)->rv_decoded_z = NULL; \
    (_ERROR)->message_z = _MESSAGE_Z; \
    longjmp((unhandled_error)->throw_jmp_buf, afw_error_code_ ##_CODE); \
} while (0);


/**
 * Macro used to test if error->code is afw_error_code_none.
 *
 * This macro should normally only be used when an xctx is not available.
 * AFW_THROW related macros should be used instead.
 */
#define AFW_SUCCESS(__ERROR_) ((__ERROR_)->code == afw_error_code_none)

/**
 * Macro used to test if error->code is not afw_error_code_none.    
 *
 * This macro should normally only be used when an xctx is not available.
 * AFW_THROW related macros should be used instead.
 */
#define AFW_ERROR_OCCURRED(__ERROR_) ((__ERROR_)->code != afw_error_code_none)

/** Used by AFW_TRY and in afw_xctx_t. */
struct afw_try_s {
    jmp_buf throw_jmp_buf;
    struct afw_try_s *prev;
};


AFW_DECLARE(void)
afw_error_rv_set_z(
    afw_error_code_t code,
    const afw_utf8_z_t *rv_source_id_z,
    int related_rv,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_error_set_z(
    afw_error_code_t code,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx);

AFW_DECLARE_ELLIPSIS(void)
afw_error_rv_set_fz(
    afw_error_code_t code,
    const afw_utf8_z_t *rv_source_id_z,
    int related_rv,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z, ...);

AFW_DECLARE_ELLIPSIS(void)
afw_error_set_fz(
    afw_error_code_t code,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z, ...);

AFW_DECLARE(void)
afw_error_rv_set_vz(
    afw_error_code_t code,
    const afw_utf8_z_t *rv_source_id_z,
    int related_rv,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * format_z, va_list ap,
    afw_xctx_t *xctx);

AFW_DECLARE(void
) afw_error_set_vz(
    afw_error_code_t code,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * format_z, va_list ap,
    afw_xctx_t *xctx);

/**
 * Macro used to throw an error.  The error should already be set using
 * macro AFW_SET_ERROR().
 *
 * Always follow with a semicolon ( AFW_THROW(xctx); ).
 */
#define AFW_THROW(xctx) \
    longjmp(((xctx)->current_try->throw_jmp_buf), (xctx)->error->code)


/**
 * @brief Macro used to set error and 0 rv in xctx and throw it.
 * @param code to be appended to afw_error_code_
 * @param message_z error message.
 * @param xctx of caller.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_Z(code, message_z, xctx) \
do { \
    afw_error_set_z(afw_error_code_ ## code, \
        AFW__FILE_LINE__, message_z, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)


/**
 * @brief Macro used to set error and rv in xctx and throw it.
 * @param code to be appended to afw_error_code_
 * @param rv_source_id to append to AFW_ERROR_RV_SOURCE_ID_Z_
 * @param rv associated with rv_source_id_z
 * @param message_z error message.
 * @param xctx of caller.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_RV_Z(code, rv_source_id, rv, message_z, xctx) \
do { \
    afw_error_rv_set_z(afw_error_code_ ## code, \
        AFW_ERROR_RV_SOURCE_ID_Z_ ## rv_source_id, rv, \
        AFW__FILE_LINE__, message_z, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)


/**
 * @brief Macro used to set error and 0 rv in xctx and throw it.
 * @param code to be appended to afw_error_code_
 * @param xctx of caller.
 * @param format_z format for error message
 * @param ... for format_z
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_FZ(code, xctx, format_z, ...) \
do { \
    afw_error_set_fz(afw_error_code_ ## code, \
        AFW__FILE_LINE__, xctx, format_z, __VA_ARGS__); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)


/**
 * @brief Macro used to set error and rv in xctx and throw it.
 * @param code to be appended to afw_error_code_
 * @param rv_source_id to append to AFW_ERROR_RV_SOURCE_ID_Z_
 * @param rv associated with rv_source_id
 * @param xctx of caller.
 * @param format_z format for error message
 * @param ... for format_z
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_RV_FZ(code, rv_source_id, rv, xctx, format_z, ...) \
do { \
    afw_error_rv_set_fz(afw_error_code_ ## code, \
        AFW_ERROR_RV_SOURCE_ID_Z_ ## rv_source_id, rv, \
        AFW__FILE_LINE__, xctx, format_z, __VA_ARGS__); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)

/**
 * @brief Macro used to set error and 0 rv in xctx and throw it.
 * @param code to be appended to afw_error_code_
 * @param format_z format for error message
 * @param ap arguments for format_z
 * @param xctx of caller.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_VZ(code, format_z, ap, xctx) \
do { \
    afw_error_set_vz(afw_error_code_ ## code, \
        AFW__FILE_LINE__, format_z, ap, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)

/**
 * @brief Macro used to set error and rv in xctx and throw it.
 * @param code to be appended to afw_error_code_
 * @param rv_source_id to append to AFW_ERROR_RV_SOURCE_ID_Z_
 * @param rv associated with rv_source_id
 * @param format_z format for error message
 * @param ap arguments for format_z
 * @param xctx of caller.
 */
#define AFW_THROW_ERROR_RV_VZ(code, rv_source_id, rv, format_z, ap, xctx) \
do { \
    afw_error_rv_set_vz(afw_error_code_ ## code, \
        AFW_ERROR_RV_SOURCE_ID_Z_ ## rv_source_id, rv, \
        AFW__FILE_LINE__, format_z, ap, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)

/**
 * @brief Macro used to set error and 0 rv in xctx using line number in
 *   footprint then throw it.
 * @param code to be appended to afw_error_code_
 * @param message_z error message.
 * @param xctx of caller.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_FOOTPRINT_Z(code, message_z, xctx) \
do { \
    afw_error_set_z(afw_error_code_ ## code, \
        footprint.source_z, message_z, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)


/**
 * @brief Macro used to set error and rv in xctx using line number in
 *    footprint then throw it.
 * @param code to be appended to afw_error_code_
 * @param line_number of footprint
 * @param rv_source_id to append to AFW_ERROR_RV_SOURCE_ID_Z_
 * @param rv associated with rv_source_id_z
 * @param message_z error message.
 * @param xctx of caller.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_FOOTPRINT_RV_Z(code, rv_source_id, rv, message_z, xctx) \
do { \
    afw_error_rv_set_z(afw_error_code_ ## code, \
        AFW_ERROR_RV_SOURCE_ID_Z_ ## rv_source_id, rv, \
        footprint.source_z, message_z, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)


/**
 * @brief Macro used to set error and 0 rv in xctx using line number in
 *  footprint then throw it.
 * @param code to be appended to afw_error_code_
 * @param line_number of footprint
 * @param xctx of caller.
 * @param format_z format for error message
 * @param ... for format_z
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_FOOTPRINT_FZ(code, xctx, format_z, ...) \
do { \
    afw_error_set_fz(afw_error_code_ ## code, \
        footprint.source_z, xctx, format_z, __VA_ARGS__); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)


/**
 * @brief Macro used to set error and rv in xctx using line number in
 *   footprint then throw it.
 * @param code to be appended to afw_error_code_
 * @param line_number of footprint
 * @param rv_source_id to append to AFW_ERROR_RV_SOURCE_ID_Z_
 * @param rv associated with rv_source_id
 * @param xctx of caller.
 * @param format_z format for error message
 * @param ... for format_z
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_FOOTPRINT_RV_FZ(code, rv_source_id, rv, xctx, format_z, ...) \
do { \
    afw_error_rv_set_fz(afw_error_code_ ## code, \
        AFW_ERROR_RV_SOURCE_ID_Z_ ## rv_source_id, rv, \
        footprint.source_z, xctx, format_z, __VA_ARGS__); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)

/**
 * @brief Macro used to set error and 0 rv in xctx using line number in
 *   footprint then throw it.
 * @param code to be appended to afw_error_code_
 * @param line_number of footprint
 * @param format_z format for error message
 * @param ap arguments for format_z
 * @param xctx of caller.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_ERROR_FOOTPRINT_VZ(code, format_z, ap, xctx) \
do { \
    afw_error_set_vz(afw_error_code_ ## code, \
        footprint.source_z, format_z, ap, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)

/**
 * @brief Macro used to set error and rv in xctx using line number in
 *   footprint then throw it.
 * @param code to be appended to afw_error_code_
 * @param line_number of footprint
 * @param rv_source_id to append to AFW_ERROR_RV_SOURCE_ID_Z_
 * @param rv associated with rv_source_id
 * @param format_z format for error message
 * @param ap arguments for format_z
 * @param xctx of caller.
 */
#define AFW_THROW_ERROR_FOOTPRINT_RV_VZ(code, rv_source_id, rv, format_z, ap, xctx) \
do { \
    afw_error_rv_set_vz(afw_error_code_ ## code, \
        AFW_ERROR_RV_SOURCE_ID_Z_ ## rv_source_id, rv, \
        footprint.source_z, format_z, ap, xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_ ## code)); \
} while (0)

/**
 * Macro used to set memory error in xctx and throw it.
 *
 * Always follow with a semicolon;
 */
#define AFW_THROW_MEMORY_ERROR(xctx) \
do { \
    afw_error_set_z(afw_error_code_memory, \
        AFW__FILE_LINE__, "Memory error", xctx); \
    longjmp(((xctx)->current_try->throw_jmp_buf), (afw_error_code_memory)); \
} while (0)

/**
 * @internal
 * @brief Macro used inside afw_environment.c to catch unhandle errors.
 *
 * This macro is normally only used in afw_environment.c to catch errors
 * thrown in an xctx that are not handled.  This should normally not occur
 * and will usually result in termination.
 *
 * Example:
 *
 *  afw_try_t unhandled_error;
 *
 *  AFW_ERROR_INTERNAL_ON_UNHANDLED(unhandled_error) {
 *
 *      ...                 Body executed on unhandled error.
 *
 *      abort();
 *  }
 */
#define AFW_ERROR_INTERNAL_ON_UNHANDLED(__TRY_) \
    (__TRY_).prev = NULL;\
    if ((setjmp((__TRY_).throw_jmp_buf)) != 0)


/**
 * @brief Copy an error struct.
 * @param to must be sizeof(afw_error_t)
 * @param from is address of error to copy.
 *
 * The from error struct is copied to to and the to->message_z and
 * to->rv_decoded_z is adjusted if they point to the corresponding
 * wa in from.
 */
#define AFW_ERROR_COPY(to, from) \
    memcpy((to), (from), sizeof(afw_error_t)); \
    if ((from)->message_z == &(from)->message_wa[0]) { \
        (to)->message_z = &(to)->message_wa[0]; \
    } \
    if ((from)->rv_decoded_z == &(from)->decode_rv_wa[0]) { \
        (to)->rv_decoded_z = &(to)->decode_rv_wa[0]; \
    }


/**
 * @brief Access the thrown error. See AFW_TRY.
 *
 * This should only be called in the body of AFW_CATCH and AFW_CATCH_UNHANDLED.
 */
#define AFW_ERROR_THROWN ((const afw_error_t *)&this_THROWN_ERROR)

/**
 * @brief Begin an AFW TRY block.
 * 
 * The variable name of the current xctx must be named xctx.
 * 
 * AFW_ERROR_THROWN can be used to access a copy of the thrown error struct
 * (const afw_error_t *) in AFW_CATCH and AFW_CATCH_UNHANDLED.
 *
 * Variable names  this_ERROR_COPIED, this_THROWN_ERROR, this_TOP_OFFSET, and 
 * this_TRY are used internal by AFW TRY block macros.
 *
 * --- WARNING  ---
 *
 * DO NOT use return, goto, or any other means of exiting TRY block, TRY_CATCH*
 * blocks or AFW_FINALLY block, other than the associated AFW_ macros or the
 * xctx's current_try will not be set correctly and AFW_FINALLY might be
 * skipped.
 *
 *
 * AFW_TRY and associated macros are used as follows:
 *
 * AFW_TRY {                (Required) Beginning of TRY block, where "xctx"
 *                         is the current xctx's pointer variable.
 *
 *    ...                  Body of TRY.
 *
 *    break;               (optional) goes to AFW_FINALLY if present or
 *                         ENDTRY if not.
 *
 * }
 * 
 * AFW_CATCH(error_code) {  (Optional) Zero or more AFW_CATCH macros with
 *                         error_code of type afw_error_codet.
 *
 *    ...                  Body of CATCH.
 *
 *    // AFW_ERROR_THROWN can be used to access error.
 *    // example: puts(AFW_ERROR_THROWN->message_z) 
 *
 *    break;               (optional) goes to AFW_FINALLY if present or
 *                         ENDTRY if not.
 *
 *    AFW_ERROR_RETHROW;  (Optional) Marks error as unhandled and breaks.
 *                         AFW_ENDTRY will rethrow error.
 *
 * }
 *
 * AFW_CATCH_UNHANDLED {    (Optional) AFW_CATCH_UNHANDLED macro.
 *
 *    ...                  Body of CATCH executed for errors not handled by
 *                         previous AFW_CATCH().
 *
 *    // AFW_ERROR_THROWN can be used to access error.
 *    // example: puts(AFW_ERROR_THROWN->message_z)
 *
 *    break;               (optional) goes to AFW_FINALLY if present or
 *                         ENDTRY if not.
 *
 *    AFW_ERROR_RETHROW;  (Optional) Marks error as unhandled and breaks.
 *                         AFW_ENDTRY will rethrow error.
 *
 * }
 *
 * AFW_FINALLY {            (Optional) AFW_FINALLY macro.
 *
 *    ...                  Body of FINALLY is executed after the body of the
 *                         AFW_TRY and bodies of AFW_CATCH* macros, regardless
 *                         of whether an error has occurred.
 * 
 *    AFW_ERROR_MARK_CAUGHT;(Optional) Marks error as handled even if not caught
 *
 *    break;               (optional) goes to AFW_ENDTRY
 *
 * }
 *
 * AFW_ENDTRY;              (Required) End of TRY block.  The xctx's current_try
 *                         will be set to its value before entering the TRY
 *                         block.  If error is unhandled, it will be rethrown.
 *
 */
#define AFW_TRY \
do {\
\
    afw_size_t this_TOP_OFFSET; \
    afw_try_t this_TRY; \
    afw_error_t this_THROWN_ERROR; \
    afw_boolean_t this_ERROR_OCCURRED = false; \
    afw_boolean_t this_ERROR_CAUGHT = false; \
    AFW_POSSIBLY_UNUSED_VARIABLE afw_boolean_t this_FINALLY_ENTERED = false; \
    this_TRY.prev = xctx->current_try;\
    xctx->current_try = &this_TRY;\
    this_TOP_OFFSET = xctx->evaluation_stack->top - \
        xctx->evaluation_stack->first; \
    do { \
        if (setjmp(this_TRY.throw_jmp_buf) != 0) { \
            AFW_ERROR_COPY(&this_THROWN_ERROR, xctx->error); \
            AFW_ERROR_CLEAR_PARTIAL(xctx->error); \
            if (this_ERROR_OCCURRED) { \
                this_THROWN_ERROR.recursive_error = true; \
                this_ERROR_CAUGHT = false; \
                break; \
            } \
            this_ERROR_OCCURRED = true; \
        } \
        else { \
            do

/**
 * @brief Catch a particular error that occurs in a AFW_TRY block.
 *
 * The xctx's current_try is set to its value before entering the AFW_TRY
 * block, so errors thrown in this AFW_CATCH block will be handle by the
 * previous try;
 */
#define AFW_CATCH( __CODE_ ) \
            while(0); \
            break; \
        } \
        if (xctx->error->code == (afw_error_code_ ## __CODE_)) \
        { \
            this_ERROR_CAUGHT = true; \
            do

/**
 * @brief Catch an unhandled error that occurs in a AFW_TRY block.
 *
 * The xctx's current_try is set to its value before entering the AFW_TRY
 * block, so errors thrown in this AFW_CATCH_UNHANDLED block will be handle by
 * the previous try;
 */
#define AFW_CATCH_UNHANDLED \
            while(0); \
            break; \
        } \
        { \
            this_ERROR_CAUGHT = true; \
            do

/**
 * @brief Always executed regardless of error.
 *
 * The body of AFW_FINALLY is executed after the body of the AFW_TRY and
 * bodies of AFW_CATCH* macros, regardless of whether an error has occurred.
 *
 * The xctx's current_try is set to its value before entering the AFW_TRY
 * block, so errors thrown in this AFW_FINALLY block will be handle by
 * the previous try;
 */
#define AFW_FINALLY \
            while(0); \
        } \
    } while(0); \
    do { \
        if (this_FINALLY_ENTERED) { \
            this_THROWN_ERROR.recursive_error_in_finally = true; \
            break; \
        } \
        this_FINALLY_ENTERED = true; \
        { \
            do

/**
 * @brief Ends an AFW try block.
 *
 * Ends a TRY bock.  If error was not handled, it will be rethrown so previous
 * try can hand it.  If error was handled, the xctx's error is cleared.
 *
 * The xctx's current_try is set to its value before entering the AFW_TRY
 * block, so errors thrown after this AFW_ENDTRY block will be handle by
 * the previous try;
 *
 * Always follow AFW_ENDTRY with a semicolon.
 */
#define AFW_ENDTRY \
            while(0); \
        } \
    } while(0); \
    xctx->current_try = this_TRY.prev; \
    if (this_ERROR_OCCURRED && !this_ERROR_CAUGHT) { \
        AFW_ERROR_COPY(xctx->error, &this_THROWN_ERROR); \
        longjmp(xctx->current_try->throw_jmp_buf, this_THROWN_ERROR.code); \
    }\
    xctx->evaluation_stack->top = xctx->evaluation_stack->first + \
        this_TOP_OFFSET; \
} while (0)


/**
 * @brief Use an AFW_ERROR_MARK_CAUGHT in AFW_FINALLY block to mark error as
 *   unconditionally unhandled even if not caught.
 *
 * The AFW_ENDTRY will not rethrow the error even if not caught.
 *
 * Always follow with a semicolon (AFW_ERROR_MARK_CAUGHT;).
 */
#define AFW_ERROR_MARK_CAUGHT \
    this_ERROR_CAUGHT = true;

/**
 * @brief Use in an AFW_CATCH or AFW_CATCH_UNHANDLED block to mark error
 *   as not caught and break
 *
 * The AFW_ENDTRY will rethrow the error.
 *
 * Always follow with a semicolon (AFW_ERROR_RETHROW;).
 */
#define AFW_ERROR_RETHROW \
    this_ERROR_CAUGHT = false; \
    break;

/**
 * @brief Create an object with error info in specified pool.
 * @param error info.
 * @param p to use for object.
 * @param xctx of caller.
 * @return object with error.
 *
 * This function leaves xctx->error unchanged if it is successful.
 */
AFW_DECLARE(const afw_object_t *)
afw_error_to_object(
    const afw_error_t *error,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Add error info to existing object using specified pool.
 * @param object to set error properties in.
 * @param error info.
 * @param xctx of caller.
 *
 * This function leaves xctx->error unchanged if it is successful.
 */
AFW_DECLARE(void)
afw_error_add_to_object(
    const afw_object_t *object,
    const afw_error_t *error,
    afw_xctx_t *xctx);



/**
 * @brief Determine if the error object for code is allowed in HTTP response.
 * @param code error code.
 * @return boolean result.
 */
AFW_DECLARE(afw_boolean_t)
afw_error_allow_in_response(afw_error_code_t code);



/**
 * @brief Print error.
 * @param fp open FILE.
 * @param error to print.
 * @return negative if error writing to fp.
 */
AFW_DECLARE(int)
afw_error_print(FILE *fp, const afw_error_t *error);



/**
 * @brief Print error when xctx is available.
 * @param fp open FILE.
 * @param error to print.
 * @param p to use if needed.
 * @param xctx of caller.
 * @return negative if error writing to fp.
 *
 * An evaluation backtrace will be printed if available when flag
 * response:error:backtraceEvaluation is on.
 */
AFW_DECLARE(int)
afw_error_print_with_xctx(
    FILE *fp,
    const afw_error_t *error,
    const afw_pool_t *p,
    afw_xctx_t *xctx);
    

/**
 * @brief Write error to environment log.
 * @param priority level.
 * @param error to write.
 * @param xctx of caller.
 *
 * This function leaves xctx->error unchanged if it is successful.
 */
AFW_DECLARE(void)
afw_error_write_log(afw_log_priority_t priority, const afw_error_t *error, 
    afw_xctx_t *xctx);


/**
 * @brief Build message in xctx error message_wa and return pointer.
 * @param xctx of caller.
 * @param format Format string.
 * @param ... List of arguments need by format string.
 * @return pointer to message_wa in xctx->error.
 *
 * This function leaves xctx->error unchanged if it is successful.
 *
 * This is for use with AFW_ERROR related macros for formatted message.
 * Messages are built in error's message_wa which is pre-allocated.
 */
AFW_DECLARE_ELLIPSIS(const afw_utf8_z_t *)
afw_error_message(afw_xctx_t *xctx, const afw_utf8_z_t *format, ...);


/**
 * @brief Build message in xctx error message_wa and return pointer.
 * @param format Format string.
 * @param ap Argument list initialized by va_start used by format string.
 * @param xctx of caller.
 * @return pointer to message_wa in xctx->error.
 *
 * This function leaves xctx->error unchanged if it is successful.
 *
 * This is for use with AFW_ERROR related macros for formatted message.
 * Messages are built in error's message_wa which is pre-allocated.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_error_message_vz(const afw_utf8_z_t *format, va_list ap,
    afw_xctx_t *xctx);


/**
 * @brief Returns value of error->source_z after last '/ 'or '\'.
 * @param error struct;
 * @return pointer past last '/' or '\'.
 *
 * Some compilers return the full path name for __FILE__, but we normally
 * just want to display the part of path after last slash.
 */
#define afw_error_source_file(error) \
    afw_utf8_z_source_file((error)->source_z)


/**
 * @brief Returns http status for error.
 * @param error struct.
 * @return http status.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_error_http_status(const afw_error_t *error);


/**
 * @brief Returns error->code id.
 * @param error struct.
 * @return description.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_error_code_id_z(const afw_error_t *error);



/**
 * @brief Convert error to utf8.
 * @param error struct.
 * @param p to use.
 * @param xctx of caller.
 * @return error as a string.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_error_to_utf8(
    const afw_error_t *error,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ERROR_H__ */

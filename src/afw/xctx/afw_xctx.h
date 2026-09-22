// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Execution Context (xctx) Implementation Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_XCTX_H__
#define __AFW_XCTX_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_xctx
 * @{
 */

/**
 * @file afw_xctx.h
 * @brief Execution context (xctx): scopes, stack, and statement_flow helpers.
 *
 * See @ref afw_xctx. An `afw_xctx_t` is a unit of work (request, eval, …).
 * Scopes use ST heaps (4k chunks, inherit managed_p) for automatic cleanup; statement_flow drives
 * break/continue/return/rethrow without C++ exceptions.
 */


AFW_BEGIN_DECLARES

/**
 * @brief Execution context (`afw_xctx_t`): scopes, stack, statement_flow.
 *
 * Unit of work for evaluation and requests. See group afw_xctx.
 * Opaque typedef is in afw_common_opaques.h.
 */
struct afw_xctx_s {

    /**
     * This xctx's pool. ST heap, managed_p = self
     * (`afw_pool_heap_create_as_managed_p`). create_managed(xctx->p)
     * allocates here. Evaluation `{ }` scopes inherit this dest.
     */
    const afw_pool_t *p;

    /**
     * The execution context (xctx) name or type.
     */
    const afw_utf8_t *name;

    /**
     * The execution context (xctx) parent xctx.
     */
    afw_xctx_t *parent;

    /**
     * Adaptive Framework Environment. This points to the same environment as
     * all other execution contexts in the same Adaptive Framework application.
     */
    const afw_environment_t *env;

    /**
     * Associated AFW thread. Always set: base xctx has type `base`
     * (no pthread). Nested xctx copies the parent pointer.
     */
    const afw_thread_t *thread;

    /**
     * Thread pool_bytes_in_use copied at xctx create. Usage is
     * thread current minus this (`afw_xctx_pool_bytes_in_use` in
     * afw_thread.h).
     */
    afw_size_t snap_pool_bytes_in_use;

    /**
     * Thread pool_chunk_bytes copied at xctx create.
     */
    afw_size_t snap_pool_chunk_bytes;

    /**
     * Request instance associated with xctx or NULL.
     */
    const afw_request_t *request;

    /**
     * A UUID to identify this xctx.
     */
    const afw_utf8_t *uuid;

    /**
     * The execution context (xctx) properties. This is an untyped object.
     */
    const afw_object_t *properties;

    /**
     * Authorization mode value. This contains the value from one of the
     * afw_authorization_mode_id_*_value variables from afw_authorization_h.
     */
    const afw_value_t *mode;

    /**
     * Anchor for steams available in xctx. See afw_stream.h.
     */
    const afw_stream_anchor_t *stream_anchor;

    /**
     * Error.
     */
    afw_error_t *error;

    /**
     * Nested throws still being processed. 0 is off. Last
     * pool release/destroy waits until this is 0 again.
     */
    afw_size_t error_processing_count;

    /**
     * Current try.
     */
    afw_try_t * current_try;

    /**
     * The execution context (xctx) evaluation stack.
     *
     * Public for AFW_TRY rewind. May move later.
     */
    afw_xctx_evaluation_stack_t *evaluation_stack;

    /**
     * The number of flags.
     */
    afw_size_t flags_count;

    /**
     * Array of boolean flags. The size is flag_count.
     */
    const afw_boolean_t *flags;

    /**
     * This indicates that xctx->flags is a mutable copy for env->flags.
     *
     * During xctx create, xctx->flags is set to env->flags. If flags change
     * for an xctx, xctx->flags is set to a mutable copy of env->flags and this
     * variable is set to true.
     */
    afw_boolean_t flags_is_mutable_copy;

    /**
     * If true, evaluates should only used secure context variables.
     * Use AFW_XCTX_SECURE_BEGIN and AFW_XCTX_SECURE_END for afw_xctx.h to
     * modify this variable.
     */
    afw_boolean_t secure;

#ifdef AFW_XCTX_INTERNAL_MEMBERS
#include "afw_xctx_internal_members.h"
#endif

};

/** Name of base xctx. */
#define AFW_XCTX_Q_NAME_BASE "base"

/** Name of base xctx. */
#define AFW_XCTX_s_NAME_BASE afw_self_s_base

/** Name of request thread xctx. */
#define AFW_XCTX_Q_NAME_REQUEST_THREAD "request_thread"

/** Name of request thread xctx. */
#define AFW_XCTX_s_NAME_REQUEST_THREAD afw_self_s_request_thread

/** Name of request session xctx. */
#define AFW_XCTX_Q_NAME_REQUEST_SESSION "request_session"

/** Name of request session xctx. */
#define AFW_XCTX_s_NAME_REQUEST_SESSION afw_self_s_request_session


/**
 * @brief Release xctx.
 * @param instance of xctx.
 * @param xctx of caller.
 */


/**
 * @brief Create an Adaptive Framework xctx.
 * @param name of xctx.
 * @param number that is unique within the life of this Adaptive
 *    Framework application for this xctx name.
 * @param xctx of caller.  This will be xctx's parent.
 *
 * Call each time a new xctx is needed with a shorter lifetime.  For
 * example, a server xctx with a shorter lifetime request_session
 * xctx.  Calling the release method will release all resources
 * allocated in the xctx including any children xctx's.  Call
 * afw_environment_create() to create the initial xctx.
 *
 * The xctx authorization mode is set to user.  Use
 * AFW_XCTX_AUTHORIZATION_MODE_BEGIN/END to change it.
 */
AFW_DECLARE(afw_xctx_t *)
afw_xctx_create(
    const afw_utf8_t *name, afw_integer_t number, afw_xctx_t *xctx);



/**
 * @brief Release an Adaptive Framework xctx.
 * @param instance of xctx.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_xctx_release(
    const afw_xctx_t *instance,
    afw_xctx_t *xctx);

/**
 * @brief Throw payload_too_large if a resource cap would be exceeded.
 * @param xctx of caller.
 * @param extra_eval_slots slots about to be pushed (0 skips eval-stack).
 *
 * Checks evaluation-stack count, request-thread ST asked-for vs
 * limitRequestPoolBytes, and C-stack remaining vs
 * limitCStackHeadroomBytes. 0 on a cap is unlimited. Pool bytes
 * are not enforced on the CLI base thread unless application
 * conf set limitRequestPoolBytes.
 */
AFW_DECLARE(void)
afw_xctx_check_resource_limits(
    afw_xctx_t *xctx, afw_size_t extra_eval_slots);



/**
 * @brief Test for environment terminating.
 * @param xctx of caller.
 * @return true if environment terminating.
 */
#define afw_xctx_environment_is_terminating(_xctx) \
    ((_xctx)->env->terminating)


/**
 * @brief If the environment is terminating, throw terminating (HTTP 503).
 * @param xctx of caller.
 *
 * Use at object / work-unit boundaries in long I/O loops so in-flight work
 * stops starting more work during graceful shutdown. Requires AFW throw
 * macros (e.g. via afw_error.h / afw_minimal.h).
 */
#define AFW_XCTX_THROW_IF_TERMINATING(_xctx) \
    do { \
        if (afw_xctx_environment_is_terminating(_xctx)) { \
            AFW_THROW_ERROR_Z(terminating, \
                "Server is terminating", (_xctx)); \
        } \
    } while (0)


/**
 * @brief Macro to allocate cleared memory in xctx's lifetime pool.
 * @param size of memory to allocate.
 * @param xctx of caller.
 * @return Pointer to memory allocated.
 */
#define afw_xctx_calloc(_size, _xctx) \
    afw_pool_calloc((_xctx)->p, (_size), (_xctx))


/**
 * @brief Macro to allocate cleared memory for type in xctx's lifetime pool.
 * @param type to allocate.
 * @param xctx of caller.
 * @return Pointer to memory allocated cast to pointer of type.
 *
 * This is a helper macro to call afw_xctx_calloc() to allocate
 * memory for a specified type and cast the return pointer to
 * a pointer to that type.
 */
#define afw_xctx_calloc_type(_type, _xctx) \
    (_type *) afw_pool_calloc((_xctx)->p, sizeof(_type), (_xctx))


/**
 * @brief Macro to allocate uncleared memory in xctx's lifetime pool.
 * @param size of memory to allocate.
 * @param xctx of caller.
 * @return Pointer to memory allocated.
 */
#define afw_xctx_malloc(_size, _xctx) \
    afw_pool_malloc((_xctx)->p, (_size), (_xctx))


/**
 * @brief Macro to allocate uncleared memory for type in xctx's lifetime pool.
 * @param type to allocate.
 * @param xctx of caller.
 * @return Pointer to memory allocated cast to pointer of type.
 *
 * This is a helper macro to call afw_pool_malloc() to allocate
 * memory for a specified type and cast the return pointer to a pointer to
 * that type.
 */
#define afw_xctx_malloc_type(_type, _xctx) \
    (_type *) afw_pool_malloc((_xctx)->p, sizeof(_type), (_xctx))


/**
 * @brief Optionally free memory allocated from xctx's lifetime pool.
 * @param address returned by afw_xctx_malloc/calloc.
 * @param size passed to malloc/calloc.
 * @param xctx of caller.
 *
 * Same pool as afw_xctx_malloc (`xctx->p`).
 */
#define afw_xctx_free(_address, _size, _xctx) \
    afw_pool_free_memory((_xctx)->p, (_address), (_size), (_xctx))


/**
 * @brief Optionally free a typed allocation from xctx's lifetime pool.
 * @param address returned by afw_xctx_malloc_type/calloc_type.
 * @param type allocated.
 * @param xctx of caller.
 */
#define afw_xctx_free_type(_address, _type, _xctx) \
    afw_pool_free_memory((_xctx)->p, (_address), sizeof(_type), (_xctx))


/**
 * @brief Begin a section this can only use secure context variables.
 * @param xctx of caller.
 *
 * The variable name of the current xctx must be named xctx.
 *
 * Usage:
 *
 *  AFW_XCTX_SECURE_BEGIN {
 *     ... code that can only use secure context variables
 *  }
 *  AFW_XCTX_SECURE_END;
 */
#define AFW_XCTX_SECURE_BEGIN \
do { \
    afw_boolean_t this_SCOPE_save_secure = xctx->secure; \
    xctx->secure = true; \
    AFW_TRY


/**
 * @brief End a section this can only use secure context variables.
 */
#define AFW_XCTX_SECURE_END \
    AFW_FINALLY { \
        xctx->secure = this_SCOPE_save_secure; \
    } \
    AFW_ENDTRY; \
} while(0)


/**
 * @brief Macro to begin an authorization mode change section.
 * @param modeId (core, intermediate, or user).
 *
 * The variable name of the current xctx must be named xctx.
 *
 * Usage:
 *
 * AFW_XCTX_AUTHORIZATION_MODE_BEGIN(intermediate) {
 *    ... code
 * }
 * AFW_XCTX_AUTHORIZATION_MODE_END;
 */
#define AFW_XCTX_AUTHORIZATION_MODE_BEGIN(_modeId) \
    const afw_value_t *this_PREVIOUS_MODE = xctx->mode; \
    xctx->mode = afw_authorization_mode_id_ ## _modeId ## _value; \
AFW_TRY


/**
 * @brief Macro to end an authorization xctx mode change section.
 *
 * See AFW_XCTX_AUTHORIZATION_MODE_BEGIN for usage.
 */
#define AFW_XCTX_AUTHORIZATION_MODE_END \
AFW_FINALLY { \
    xctx->mode =  this_PREVIOUS_MODE; \
} \
AFW_ENDTRY



/*
 * Scope frame macros and functions are in afw_pool_scope_internal.h.
 * Core only. The public name afw_pool_scope_t stays an incomplete type.
 */


/* ----------------------------------------------------------------------------

    Execution Context (xctx) statement flow.
    
---------------------------------------------------------------------------- */

/**
 * @brief Set the xctx statement flow.
 * @param flow to set.
 * @param xctx of caller.
 */
#define afw_xctx_statement_flow_set(_flow, _xctx) \
    ((afw_xctx_t *)_xctx)->statement_flow = (_flow)

/**
 * @brief Set the xctx statement flow to <type>
 * @param type is afw_xctx_statement_flow_<type> of flow to set.
 * @param xctx of caller.
 */
#define afw_xctx_statement_flow_set_type(_type, _xctx) \
    ((afw_xctx_t *)_xctx)->statement_flow = \
        afw_xctx_statement_flow_ ## _type
/**
 * @brief Get the xctx statement flow
 * @param xctx of caller.
 * @return flow.
 */
#define afw_xctx_statement_flow_get(_xctx) \
    (((afw_xctx_t *)_xctx)->statement_flow)

/**
 * @brief Test if xctx statement flow <type>
 * @param type is afw_xctx_statement_flow_<type> of flow to set.
 * @param xctx of caller.
 * @return boolean result of test.
 */
#define afw_xctx_statement_flow_is_type(_type, _xctx) \
    (((afw_xctx_t *)_xctx)->statement_flow == \
        afw_xctx_statement_flow_ ## _type)

/**
 * @brief Test if xctx statement flow is one that should leave loop or switch
 * @param xctx of caller.
 * @return boolean result of test.
 * 
 * This is true if the statement flow is break, return, and rethrow but false
 * for continue and sequential.
 */
#define afw_xctx_statement_flow_is_leave(_xctx) \
    (((afw_xctx_t *)_xctx)->statement_flow >= \
        afw_xctx_statement_flow_ge_is_leave)

/**
 * @brief Reset xctx statement flow break and continue to sequential
 * @param xctx of caller.
 */
#define afw_xctx_statement_flow_reset_break_and_continue(_xctx) \
    if (((afw_xctx_t *)_xctx)->statement_flow <= \
        afw_xctx_statement_flow_ge_is_leave) { \
            afw_xctx_statement_flow_set_type(sequential, _xctx); \
            ((afw_xctx_t *)_xctx)->statement_flow_label = NULL; \
    }

/**
 * @brief Reset xctx statement flow except rethrow to sequential
 * @param xctx of caller.
 * 
 * This should be used at the end of script function evaluation, template
 * evaluation, and all eval() adaptive functions.
 */
#define afw_xctx_statement_flow_reset_all_except_rethrow(_xctx) \
    if (!afw_xctx_statement_flow_is_type(rethrow, _xctx)) { \
        afw_xctx_statement_flow_set_type(sequential, _xctx); \
        ((afw_xctx_t *)_xctx)->statement_flow_label = NULL; \
    }

/**
 * @brief Get the running Adaptive Script result.
 * @param xctx of caller.
 * @return Current script result, or undefined if none has been written.
 */
#define afw_xctx_script_result_get(_xctx) \
    ((_xctx)->script_result \
        ? (_xctx)->script_result \
        : afw_value_undefined)

/**
 * @brief Set the running Adaptive Script result.
 * @param v result value. Void and NULL are not stored.
 * @param xctx of caller.
 *
 * Scope deactivate writes last_result here (not void). Nested
 * evaluate that must not change the caller's last saves and restores
 * the pointer.
 */
#define afw_xctx_script_result_set(_v, _xctx) \
    afw_xctx_script_result_set_value((_v), (_xctx))


/**
 * @brief Assign into the current hidden result slot.
 * @param value to store. Void and NULL are not stored.
 * @param xctx of caller.
 *
 * Same protocol as a named slot (`get_assignable` new, `release` old).
 */
AFW_DECLARE(void)
afw_xctx_script_result_set_value(
    const afw_value_t *value,
    afw_xctx_t *xctx);



/* ----------------------------------------------------------------------------

    Execution Context (xctx) Evaluation stack.
    
---------------------------------------------------------------------------- */


/**
 * @brief The execution context (xctx) evaluation stack entry.
 *
 * The xctx evaluation stack is used to detection recursive loops and
 * to produce a backtrace of evaluate.
 *
 * If entry_id points to afw_s_parameter, the previous entry in the
 * stack is parameter_number.  In all other cases the entry is value.
 */
struct afw_xctx_evaluation_stack_entry_s {
    union {
        const afw_utf8_t *entry_id;
        afw_size_t parameter_number;
        const afw_value_t *value;
    };
};


AFW_VECTOR_STRUCT(afw_xctx_evaluation_stack_s,
    afw_xctx_evaluation_stack_entry_t);

#define AFW_XCTX_EVALUATION_STACK_LAST(_xctx) \
    (&(_xctx)->evaluation_stack->entries[ \
        (_xctx)->evaluation_stack->count - 1])


#ifdef AFW_DEBUG_EVALUATION
#define AFW_XCTX_DEBUG_EVALUATION_PRINT(_xctx, _op_z, _extra_fmt_z, ...) \
do { \
    if ((_xctx) && (_xctx)->env && (_xctx)->env->debug_fd && \
        afw_flag_is_active( \
            (_xctx)->env->flag_index_debug_evaluation, (_xctx))) \
    { \
        if (afw_flag_is_active( \
                (_xctx)->env->flag_index_debug_evaluation_detail, (_xctx))) \
        { \
            fprintf((_xctx)->env->debug_fd, \
                ">debug eval %s" _extra_fmt_z " (%s)\n", \
                _op_z, ##__VA_ARGS__, \
                afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        } \
        else { \
            fprintf((_xctx)->env->debug_fd, \
                ">debug eval %s (%s)\n", \
                _op_z, afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        } \
        fflush((_xctx)->env->debug_fd); \
    } \
} while (0)
#endif


/**
 * @brief Push VALUE onto execution stack.
 * @param VALUE
 * @param xctx of caller.
 * @return Don't use.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_push_value(_VALUE, _xctx) \
do { \
    const afw_value_t *_afw_eval_push_value = (_VALUE); \
    const char *_afw_eval_inf_s = "-"; \
    int _afw_eval_inf_len = 1; \
    if (_afw_eval_push_value && _afw_eval_push_value->inf) { \
        _afw_eval_inf_s = (const char *) \
            _afw_eval_push_value->inf->rti.implementation_id.s; \
        _afw_eval_inf_len = (int) \
            _afw_eval_push_value->inf->rti.implementation_id.len; \
        if (!_afw_eval_inf_s) { \
            _afw_eval_inf_s = "-"; \
            _afw_eval_inf_len = 1; \
        } \
    } \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
        "push_value", \
        " value %p inf " AFW_UTF8_FMT, \
        (const void *)_afw_eval_push_value, \
        _afw_eval_inf_len, _afw_eval_inf_s); \
    afw_xctx_check_resource_limits((_xctx), 1); \
    afw_vector_push_index_impl( \
        &(_xctx)->evaluation_stack->internal, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->value = \
        _afw_eval_push_value; \
} while (0)
#else
#define afw_xctx_evaluation_stack_push_value(_VALUE, _xctx) \
    do { \
        afw_xctx_check_resource_limits((_xctx), 1); \
        afw_vector_push_index_impl( \
            &(_xctx)->evaluation_stack->internal, (_xctx)); \
        AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->value = (_VALUE); \
    } while (0)
#endif


/**
 * @brief Push PARAMETER_NUMBER onto execution stack.
 * @param PARAMETER_NUMBER
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_push_parameter_number(\
    PARAMETER_NUMBER, _xctx) \
do { \
    afw_size_t _afw_eval_push_pn = (PARAMETER_NUMBER); \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
        "push_parameter_number", \
        " n " AFW_SIZE_T_FMT, \
        _afw_eval_push_pn); \
    afw_xctx_check_resource_limits((_xctx), 2); \
    afw_vector_push_index_impl( \
        &(_xctx)->evaluation_stack->internal, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->parameter_number = \
        _afw_eval_push_pn; \
    afw_vector_push_index_impl( \
        &(_xctx)->evaluation_stack->internal, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->entry_id = \
        afw_s_parameter_number; \
} while (0)
#else
#define afw_xctx_evaluation_stack_push_parameter_number(\
    PARAMETER_NUMBER, _xctx) \
do { \
    afw_xctx_check_resource_limits((_xctx), 2); \
    afw_vector_push_index_impl( \
        &(_xctx)->evaluation_stack->internal, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->parameter_number = \
        (PARAMETER_NUMBER); \
    afw_vector_push_index_impl( \
        &(_xctx)->evaluation_stack->internal, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->entry_id = \
        afw_s_parameter_number; \
} while (0)
#endif


/**
 * @brief Pop top PARAMETER_NUMBER or VALUE off execution stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_pop(_xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
        "pop", ""); \
    if (AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->entry_id == \
        afw_s_parameter_number) { \
        afw_vector_pop((_xctx)->evaluation_stack, (_xctx)); \
    } \
    afw_vector_pop((_xctx)->evaluation_stack, (_xctx)); \
} while (0)
#else
#define afw_xctx_evaluation_stack_pop(_xctx) \
do { \
    if (AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->entry_id == \
        afw_s_parameter_number) { \
        afw_vector_pop((_xctx)->evaluation_stack, (_xctx)); \
    } \
    afw_vector_pop((_xctx)->evaluation_stack, (_xctx)); \
} while (0)
#endif


/**
 * @brief Pop top VALUE off execution stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_pop_value(_xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
        "pop_value", ""); \
    afw_xctx_evaluation_stack_pop_value_impl(_xctx); \
} while (0)
#else
#define afw_xctx_evaluation_stack_pop_value(_xctx) \
    afw_xctx_evaluation_stack_pop_value_impl(_xctx)
#endif



/**
 * @brief Pop top PARAMETER_NUMBER off execution stack.
 * @param VALUE evaluated parameter, or NULL.
 * @param xctx of caller.
 *
 * Use only when top is the parameter-number marker. Pops the marker
 * and writes VALUE into the number slot. To take the pair off with
 * nothing to keep, use afw_xctx_evaluation_stack_pop().
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_pop_parameter_number(_VALUE, _xctx) \
do { \
    const afw_value_t *_afw_pop_pn_value = (_VALUE); \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
        "pop_parameter_number", ""); \
    afw_vector_pop((_xctx)->evaluation_stack, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->value = _afw_pop_pn_value; \
} while (0)
#else
#define afw_xctx_evaluation_stack_pop_parameter_number(_VALUE, _xctx) \
do { \
    afw_vector_pop((_xctx)->evaluation_stack, (_xctx)); \
    AFW_XCTX_EVALUATION_STACK_LAST(_xctx)->value = (_VALUE); \
} while (0)
#endif


/**
 * @brief Rewind evaluation stack to saved_top.
 * @param save_count entry count to restore to.
 * @param xctx of caller.
 *
 * Used by AFW_ENDTRY and restore_top.
 */
AFW_DECLARE(void)
afw_xctx_evaluation_stack_rewind(
    afw_size_t save_count,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_xctx_evaluation_stack_pop_value_impl(
    afw_xctx_t *xctx);


/**
 * @brief Save top of stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_save_top(_xctx) \
afw_size_t evaluation_stack_save_top = \
(_xctx)->evaluation_stack->count; \
AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
    "save_top", "")
#else
#define afw_xctx_evaluation_stack_save_top(_xctx) \
afw_size_t evaluation_stack_save_top = \
(_xctx)->evaluation_stack->count
#endif


/**
 * @brief Restore top of stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_restore_top(_xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((_xctx), \
        "restore_top", ""); \
    afw_xctx_evaluation_stack_rewind( \
        evaluation_stack_save_top, (_xctx)); \
} while (0)
#else
#define afw_xctx_evaluation_stack_restore_top(_xctx) \
afw_xctx_evaluation_stack_rewind(evaluation_stack_save_top, _xctx)
#endif


/* ----------------------------------------------------------------------------

    Execution Context (xctx) Qualifiers and Qualified Variables
    
---------------------------------------------------------------------------- */


/**
 * @brief Get an optionally qualified variable value.
 * @param qualifier of variable, or NULL / empty for unqualified lexical name.
 * @param name of variable.
 * @param xctx of caller.
 * @return value pointer, or C NULL (see below — meaning depends on path).
 *
 * **Unqualified** (no qualifier): looks up a lexical symbol by name and
 * returns the **slot contents**. After scope create / set_value, bound slots
 * use afw_value_undefined rather than C NULL for empty values, so a NULL
 * return normally means **not bound**. Still prefer
 * afw_pool_scope_symbol_exists_by_name() / get_value_address_by_name() for
 * existence (issue #131; script variable_exists / variable_get).
 *
 * **Qualified** (`qualifier::name`): walks matching visible frames newest →
 * oldest. First frame whose get_cb returns non-NULL wins (including
 * afw_value_undefined / afw_value_null). C NULL from get_cb means not defined
 * on that frame (keep walking). Overall NULL means not defined on any frame.
 * See afw_xctx_get_variable_cb_t — do not return C NULL for present undefined.
 */
AFW_DECLARE(const afw_value_t *)
afw_xctx_get_optionally_qualified_variable(
    const afw_utf8_t *qualifier,
    const afw_utf8_t *name,
    afw_xctx_t *xctx);

/** Struct for xctx qualifier stack entry. */
struct afw_xctx_qualifier_stack_entry_s {

    /** @brief Pool used while processing entry. */
    const afw_pool_t *p;

    /** @brief qualifier or len=0 if unqualified. */
    afw_utf8_t qualifier;

    /** @brief qualifier_object.  This may be NULL now. */
    const afw_object_t *qualifier_object;

    /** @brief Get a variable by name (hot path). */
    afw_xctx_get_variable_cb_t get_cb;

    /**
     * @brief Contribute known variables into a snapshot object (slow path).
     * Skip names already present on the object (first wins). Required.
     */
    afw_xctx_contribute_variables_cb_t contribute_cb;

    /** @brief Data that will be passed to get/contribute callbacks. */
    void *data;

    /** @brief Work area used by callbacks (e.g. CB variable table). */
    void *wa;

    /** @brief Secure access to this qualifier is allowed. */
    afw_boolean_t secure;

};



/**
 * @brief Get qualifier stack top.
 * @param xctx of caller.
 * @return Stack top index.
 */
AFW_DECLARE(int)
afw_xctx_qualifier_stack_top_get(
    afw_xctx_t *xctx);



/**
 * @brief Set stack top index.
 * @param top returned from corresponding
 *   afw_xctx_qualifier_stack_top_get().
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_xctx_qualifier_stack_top_set(int top, afw_xctx_t *xctx);



/**
 * @brief Push qualifiers object on to stack.
 * @param context_object
 * @param secure access to this qualifier is allowed.
 * @param p used while evaluating variable values.
 * @param xctx of caller.
 *
 * Context object is an object whose properties are objects with a name of a
 * qualifier.  Each of the qualifier objects contains properties that are
 * variable name/adaptive value pairs.
 *
 * Never specify secure true if source of context is a client. A secure
 * context is used for access control and other server side configured
 * evaluates.
 *
 * See afw_xctx_qualifier_stack_qualifier_push() for use pattern.
 */
AFW_DECLARE(void)
afw_xctx_qualifier_stack_qualifiers_object_push(
    const afw_object_t *context_object,
    afw_boolean_t secure,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Push qualifier on to stack.
 * @param qualifier or NULL.
 * @param qualifier_object optional object of properties (may be NULL).
 * @param secure access to this qualifier is allowed.
 * @param get_cb get variable by name (hot path).
 * @param contribute_cb contribute variables into a snapshot object (required;
 *    used by qualifier()/qualifiers() listing — issue #9).
 * @param data to be passed to callbacks (often ctx).
 * @param p used while evaluating variable values.
 * @param xctx of caller.
 * @return New qualifier stack entry.
 *
 * get_cb is the fastest way to get a known variable for this qualifier.
 * contribute_cb must list that frame's variables into a memory object without
 * overwriting existing property names (first wins).
 *
 * Never specify secure true if source of context is a client. A secure
 * context is used for access control and other server side configured
 * evaluates.
 *
 * This function should use AFW_TRY to restore top.
 *
 * top = afw_xctx_qualifier_stack_top_get(xctx);
 * AFW_TRY (xctx) {
 *  afw_xctx_qualifier_stack_qualifier_push(...);
 *  ... do something
 * }
 * AFW_FINALLY {
 *    afw_xctx_qualifier_stack_top_set(top, xctx);
 * }
 *
 */
AFW_DECLARE(afw_xctx_qualifier_stack_entry_t *)
afw_xctx_qualifier_stack_qualifier_push(
    const afw_utf8_t *qualifier,
    const afw_object_t *qualifier_object,
    afw_boolean_t secure,
    afw_xctx_get_variable_cb_t get_cb,
    afw_xctx_contribute_variables_cb_t contribute_cb,
    void * data,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Push qualifier object on to stack.
 * @param qualifier_name or NULL.
 * @param qualifier_object whose properties will be variables.
 * @param secure access to this qualifier is allowed.
 * @param p used while evaluating variable values.
 * @param xctx of caller.
 *
 * Never specify secure true if source of context is a client. A secure
 * context is used for access control and other server side configured
 * evaluates.
 *
 * See afw_xctx_qualifier_stack_qualifier_push() for use pattern.
 */
AFW_DECLARE(void)
afw_xctx_qualifier_stack_qualifier_object_push(
    const afw_utf8_t *qualifier_name,
    const afw_object_t *qualifier_object,
    afw_boolean_t secure,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a fresh snapshot object of active variables for a qualifier.
 * @param qualifier name (e.g. environment, current).
 * @param include_untrusted Default false: same frame visibility as
 *    qualifier::name get right now. When xctx is secure, true uses the same
 *    visibility as running less secure (trusted and untrusted frames). When
 *    xctx is not secure, true and false are the same.
 * @param p to use.
 * @param xctx of caller.
 * @return New memory object (never a live view), or NULL if no matching
 *    visible stack entry for that qualifier (nullish to scripts). Walks most
 *    recent → older; every matching visible entry contributes (most recent
 *    wins per property name). Empty object after contribute is still an object.
 */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifier_object_create(
    const afw_utf8_t *qualifier,
    afw_boolean_t include_untrusted,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a fresh snapshot of all active qualifiers and their variables.
 * @param include_untrusted Same meaning as afw_xctx_qualifier_object_create().
 * @param p to use.
 * @param xctx of caller.
 * @return New memory object of qualifier name → variables object.
 */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifiers_object_create(
    afw_boolean_t include_untrusted,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_XCTX_H__ */

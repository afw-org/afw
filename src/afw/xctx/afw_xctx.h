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
 * Scopes use heap trackers for automatic cleanup; statement_flow drives
 * break/continue/return/rethrow without C++ exceptions. Struct layout is
 * in `afw_common.h`; this header is the public helper surface.
 */


AFW_BEGIN_DECLARES

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
 * @brief Test for environment terminating.
 * @param xctx of caller.
 * @return true if environment terminating.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_xctx_environment_is_terminating(afw_xctx_t *xctx)
{
    return xctx->env->terminating;
}


/**
 * @brief If the environment is terminating, throw terminating (HTTP 503).
 * @param xctx of caller.
 *
 * Use at object / work-unit boundaries in long I/O loops so in-flight work
 * stops starting more work during graceful shutdown. Requires AFW throw
 * macros (e.g. via afw_error.h / afw_minimal.h).
 */
#define AFW_XCTX_THROW_IF_TERMINATING(xctx) \
    do { \
        if (afw_xctx_environment_is_terminating(xctx)) { \
            AFW_THROW_ERROR_Z(terminating, \
                "Server is terminating", (xctx)); \
        } \
    } while (0)


/**
 * @brief Macro to allocate cleared memory in xctx's lifetime pool.
 * @param size of memory to allocate.
 * @param xctx of caller.
 * @return Pointer to memory allocated.
 */
#define afw_xctx_calloc(size, xctx) \
    afw_pool_calloc((xctx)->p, (size), (xctx))


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
#define afw_xctx_calloc_type(type, xctx) \
    (type *) afw_pool_calloc((xctx)->p, sizeof(type), (xctx))


/**
 * @brief Macro to allocate uncleared memory in xctx's lifetime pool.
 * @param size of memory to allocate.
 * @param xctx of caller.
 * @return Pointer to memory allocated.
 */
#define afw_xctx_malloc(size, xctx) \
    afw_pool_malloc((xctx)->p, (size), (xctx))


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
#define afw_xctx_malloc_type(type, xctx) \
    (type *) afw_pool_malloc((xctx)->p, sizeof(type), (xctx))


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
#define AFW_XCTX_AUTHORIZATION_MODE_BEGIN(modeId) \
    const afw_value_t *this_PREVIOUS_MODE = xctx->mode; \
    xctx->mode = afw_authorization_mode_id_ ## modeId ## _value; \
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



/* ----------------------------------------------------------------------------

    Execution Context (xctx) Scope
    
---------------------------------------------------------------------------- */


/**
 * @brief The struct for an execution context frame.
 */
struct afw_xctx_scope_s {
    const afw_pool_t *p;
    const afw_value_block_t *block;
    const afw_xctx_scope_t *parent_lexical_scope;
    afw_size_t reference_count;
    afw_size_t scope_number;
    /*
     * Last-release walk can re-enter via a closure that holds this
     * scope. Skip a nested release while tearing down.
     */
    afw_boolean_t destroying;
    /*
     * When this struct is created by afw_xctx_scope_create(), it will be
     * allocated large enough to hold block->symbol_count symbol_values.
     */ 
    const afw_value_t *symbol_values[1];
};



/**
 * @brief Get current scope.
 * @param xctx of caller.
 * @return Current scope.
 */
#define afw_xctx_scope_current(xctx) \
    ((xctx->scope_stack->nelts > 0) \
    ? ((const afw_xctx_scope_t **)xctx->scope_stack->elts) \
        [xctx->scope_stack->nelts - 1] \
    : NULL)



/**
 * @brief Create a new scope.
 * @param block associated with this scope.
 * @param parent_lexical_scope of this scope or NULL for first one.
 * @param xctx of caller.
 * @return New xctx scope.
 *
 * Function afw_xctx_scope_create() is used to create a new scope for the
 * supplied block. Symbol value slots start as the permanent
 * **afw_value_undefined** singleton (not C NULL) so a bound name always has a
 * value pointer; see afw_xctx_scope_symbol_exists_by_name and issue #131.
 * afw_xctx_scope_symbol_set_value() also stores that singleton when given
 * C NULL.
 *
 * If a parent_lexical_scope is specified, it's reference count will be
 * incremented. The block depth of the block supplied must be 1 more than the
 * block depth of the parent_lexical_scope's block depth.
 *
 * If parent_lexical_scope is NULL, the block's depth must be 0.
 *
 * This newly created scope has a reference count of 0 when first created. This
 * reference count is incremented by functions afw_xctx_scope_activate() and
 * afw_xctx_scope_get_reference(), as well as a call to afw_xctx_scope_create()
 * with this scope specified as its parent_lexical_scope.
 *
 * The reference count is decreased by calls to afw_xctx_scope_deactivate(),
 * afw_xctx_scope_release() and afw_xctx_scope_unwind(). When the reference
 * count reaches 0, or it is already 0 because it's never been referenced, this
 * scope's pool is released and afw_xctx_scope_release() is call for the
 * parent_lexical_scope, if there is one.
 *
 * More detail on how scopes work:
 *
 * An empty scope stack is created when the xctx is created and destroyed when
 * the xctx is destroyed. This scope stack is a stack of pointers to scope
 * structs of the currently active scopes in order of their activation.
 *
 * The current scope, which can be retrieve by calling afw_xctx_scope_current(),
 * is at the top of the scope stack.
 *
 * The scope stack is maintained by calls to function afw_xctx_scope_activate(),
 * which pushes a scope onto the scope stack and increments its reference count,
 * paired with calls to afw_xctx_scope_deactivate() which pops a scope off the
 * scope stack and calls afw_xctx_stack_release() for it.
 *
 * Function afw_xctx_scope_rewind(), used in 'catch' and 'finally', calls
 * afw_xctx_scope_deactivate() on all of the scopes down to the current scope at
 * the time 'try' was entered.
 *
 * The evaluate for a compiled value always pushes a NULL on the scope stack
 * before evaluating its root value then makes sure the NULL is still there in
 * the same position and removes it when the evaluation is complete. This causes
 * the evaluation of the root value to begin with a current scope of NULL, which
 * will cause it's first scope to be lexical scope lexical depth 0.
 *
 * Symbols (variables, parameters, etc.) go in and out of scope. The scope
 * struct has a C array of values for the symbols in the scope. A symbol has a
 * lexical scope depth and index into the corresponding scope's symbol values
 * array, which is determined at compile time. The depth of the current scope's
 * block minus the lexical scope depth of a symbol determines how many times the
 * scope parent_lexical_scope pointer must be dereferenced to find the scope
 * containing the symbol's value.
 *
 * When a closure binding is created, afw_xctx_scope_get_reference() is called
 * on its enclosing pool. When the closure binding goes out of scope, a
 * corresponding afw_xctx_scope_release() is called.
 *
 * The afw_xctx_scope_symbol_*() functions are used to get and set symbol
 * values.
 */
AFW_DECLARE(const afw_xctx_scope_t *)
afw_xctx_scope_create(
    const afw_value_block_t *block,
    const afw_xctx_scope_t *parent_lexical_scope,
    afw_xctx_t *xctx);


/**
 * @brief Clone a scope.
 * @param original_scope to clone.
 * @param xctx of caller.
 *
 * This function calls afw_xctx_scope_create() and `add_reference`s each
 * original symbol into the new scope (same protocol as assign). The
 * hidden result is not on the scope and is not copied.
 *
 * This function was originally needed to support the incrementor of 'for'
 * statements since each increment needs its own copy of variables to support
 * closure semantics.
 */
AFW_DECLARE(const afw_xctx_scope_t *)
afw_xctx_scope_clone(
    const afw_xctx_scope_t *original_scope,
    afw_xctx_t *xctx);



/**
 * @brief Activate scope.
 * @param scope to activate as the current scope.
 * @param xctx of caller.
 * 
 * Call this after afw_xctx_scope_create() or afw_xctx_scope_clone() and when
 * there is a need to switch to a different containing lexical scope.
 */
AFW_DECLARE(void)
afw_xctx_scope_activate(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx);



/**
 * @brief Add a reference to a scope.
 * @param scope to be referenced. 
 * @param xctx of caller.
 * @return scope.
 */
AFW_DECLARE(const afw_xctx_scope_t *)
afw_xctx_scope_get_reference(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Deactivate scope.
 * @param scope to deactivate that must be the current scope.
 * @param xctx of caller.
 *
 * Deactivate is done automatically when a afw_xctx_scope_release() is called
 * for a scope so only use this when afw_xctx_scope_activate() is called at
 * times other than paired after a afw_xctx_scope_create_and_activate(). One place this
 * happens is in call_script_function evaluate when there are no parameters but
 * there is a need to switch to the enclosing lexical scope.
 */
AFW_DECLARE(void)
afw_xctx_scope_deactivate(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx);



/**
 * @brief Release current scope.
 * @param scope must match afw_xctx_scope_current(xctx)
 * @param xctx of caller.
 * 
 * Decrement the reference count. On last release, walk slots (`release`
 * each), then the parent lexical scope, then this scope's pool.
 */
AFW_DECLARE(void)
afw_xctx_scope_release(
    const afw_xctx_scope_t *scope, afw_xctx_t *xctx);



/**
 * @brief Unwind the scope stack down to but not including the specified scope.
 * @param scope to unwind down to
 * @param xctx of caller.
 * 
 * This pops and releases all of the scopes in the scope stack down to but not
 * including the specified scope.  This is used to unwind the scope stack when
 * an error occurs.
 */
AFW_DECLARE(void)
afw_xctx_scope_unwind(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Get the address where the value of a symbol is stored within the
 *     current scope chain.
 * @param symbol whose value address is to be returned.
 * @param scope to start search from.
 * @param xctx of caller.
 * @return value address (never NULL on success).
 *
 * An error is thrown if the symbol's value location is not found. This most
 * likely is caused by a compile error.
 *
 * Non-NULL address means the symbol is bound. Slot contents are normally the
 * permanent undefined singleton until assigned (or an explicit value); C NULL
 * in a slot is legacy — treat with afw_value_is_undefined().
 */
AFW_DECLARE(const afw_value_t **)
afw_xctx_scope_symbol_get_value_address(
    const afw_value_block_symbol_t *symbol,
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Get the address where the value of a named symbol is stored within the
 *     current scope chain.
 * @param symbol_name of symbol whose value address is to be returned.
 * @param xctx of caller.
 * @return value address, or NULL if no symbol with that name is bound.
 *
 * NULL return means the name is **not bound**. Non-NULL means bound; *address
 * is the current value (undefined singleton until assigned). Script
 * variable_exists uses this distinction (issue #131). Prefer this or
 * afw_xctx_scope_symbol_exists_by_name() over truthiness-testing a get
 * result from the unqualified get path alone.
 */
AFW_DECLARE(const afw_value_t **)
afw_xctx_scope_symbol_get_value_address_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx);



/**
 * @brief Get the value of a symbol in the current scope chain.
 * @param symbol to get value of.
 * @param xctx of caller.
 * @return value pointer (undefined singleton until assigned; use
 *     afw_value_is_undefined).
 *
 * An error is thrown if the symbol's value location is not found. Does not mean
 * “missing symbol” when the value is undefined.
 */
AFW_DECLARE(const afw_value_t *)
afw_xctx_scope_symbol_get_value(
    const afw_value_block_symbol_t *symbol,
    afw_xctx_t *xctx);



/**
 * @brief Get the value of a named symbol in the current scope chain.
 * @param symbol_name of value to get.
 * @param xctx of caller.
 * @return value pointer (undefined singleton until assigned).
 *
 * An error is thrown if the name is not bound. Undefined value is not “missing.”
 */
AFW_DECLARE(const afw_value_t *)
afw_xctx_scope_symbol_get_value_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx);



/**
 * @brief True if the named lexical symbol is bound in the current scope chain.
 * @param symbol_name of symbol to check.
 * @param xctx of caller.
 * @return true if a symbol with that name exists (any value, including
 *     the undefined singleton).
 *
 * This is the C-side “variable_exists” for unqualified names: **bound**, not
 * “value is non-nullish.” Does not consult the qualifier stack (use
 * afw_xctx_get_optionally_qualified_variable for qualifier::name presence via
 * get_cb contract).
 */
AFW_DECLARE(afw_boolean_t)
afw_xctx_scope_symbol_exists_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx);



/**
 * @brief Set the value of a symbol in the current scope chain.
 * @param symbol to set value of.
 * @param value to set.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_xctx_scope_symbol_set_value(
    const afw_value_block_symbol_t *symbol,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Set the value of a named symbol in the current scope chain.
 * @param symbol_name of value to set.
 * @param value to set.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_xctx_scope_symbol_set_value_by_name(
    const afw_utf8_t *symbol_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/* ----------------------------------------------------------------------------

    Execution Context (xctx) statement flow.
    
---------------------------------------------------------------------------- */

/**
 * @brief Set the xctx statement flow.
 * @param flow to set.
 * @param xctx of caller.
 */
#define afw_xctx_statement_flow_set(flow, xctx) \
    ((afw_xctx_t *)xctx)->statement_flow = (flow)

/**
 * @brief Set the xctx statement flow to <type>
 * @param type is afw_xctx_statement_flow_<type> of flow to set.
 * @param xctx of caller.
 */
#define afw_xctx_statement_flow_set_type(type, xctx) \
    ((afw_xctx_t *)xctx)->statement_flow = \
        afw_xctx_statement_flow_ ## type
/**
 * @brief Get the xctx statement flow
 * @param xctx of caller.
 * @return flow.
 */
#define afw_xctx_statement_flow_get(xctx) \
    (((afw_xctx_t *)xctx)->statement_flow)

/**
 * @brief Test if xctx statement flow <type>
 * @param type is afw_xctx_statement_flow_<type> of flow to set.
 * @param xctx of caller.
 * @return boolean result of test.
 */
#define afw_xctx_statement_flow_is_type(type, xctx) \
    (((afw_xctx_t *)xctx)->statement_flow == \
        afw_xctx_statement_flow_ ## type)

/**
 * @brief Test if xctx statement flow is one that should leave loop or switch
 * @param xctx of caller.
 * @return boolean result of test.
 * 
 * This is true if the statement flow is break, return, and rethrow but false
 * for continue and sequential.
 */
#define afw_xctx_statement_flow_is_leave(xctx) \
    (((afw_xctx_t *)xctx)->statement_flow >= \
        afw_xctx_statement_flow_ge_is_leave)

/**
 * @brief Reset xctx statement flow break and continue to sequential
 * @param xctx of caller.
 */
#define afw_xctx_statement_flow_reset_break_and_continue(xctx) \
    if (((afw_xctx_t *)xctx)->statement_flow <= \
        afw_xctx_statement_flow_ge_is_leave) { \
            afw_xctx_statement_flow_set_type(sequential, xctx); \
            ((afw_xctx_t *)xctx)->statement_flow_label = NULL; \
    }

/**
 * @brief Reset xctx statement flow except rethrow to sequential
 * @param xctx of caller.
 * 
 * This should be used at the end of script function evaluation, template
 * evaluation, and all eval() adaptive functions.
 */
#define afw_xctx_statement_flow_reset_all_except_rethrow(xctx) \
    if (!afw_xctx_statement_flow_is_type(rethrow, xctx)) { \
        afw_xctx_statement_flow_set_type(sequential, xctx); \
        ((afw_xctx_t *)xctx)->statement_flow_label = NULL; \
    }

/**
 * @brief Get the running Adaptive Script result.
 * @param xctx of caller.
 * @return Current script result, or undefined if none has been written.
 */
#define afw_xctx_script_result_get(xctx) \
    ((xctx)->script_result \
        ? (xctx)->script_result \
        : afw_value_undefined)

/**
 * @brief Set the running Adaptive Script result.
 * @param v result value (NULL is stored as undefined).
 * @param xctx of caller.
 *
 * Assignment and return write this. Most other statements do not.
 */
#define afw_xctx_script_result_set(v, xctx) \
    afw_xctx_script_result_set_value((v), (xctx))


/**
 * @brief Assign into the current hidden result slot.
 * @param value to store (NULL becomes undefined).
 * @param xctx of caller.
 *
 * Same protocol as a named slot (`release` old, `add_reference` new).
 */
AFW_DECLARE(void)
afw_xctx_script_result_set_value(
    const afw_value_t *value,
    afw_xctx_t *xctx);


/**
 * @brief Restore a parked caller hidden result after a nested activation.
 * @param saved caller `script_result` pointer.
 * @param saved_active caller `script_result_active`.
 * @param saved_written caller `script_result_written`.
 * @param donate true to park the callee hold for the caller to store;
 *    false to `release` a nested write (for init/increment).
 * @param xctx of caller.
 *
 * Not an assign into the caller slot. Call/block last-`release` must
 * already have run so named slots are walked while the callee hidden
 * result still holds the return.
 */
AFW_DECLARE(void)
afw_xctx_script_result_restore(
    const afw_value_t *saved,
    afw_boolean_t saved_active,
    afw_boolean_t saved_written,
    afw_boolean_t donate,
    afw_xctx_t *xctx);



/* ----------------------------------------------------------------------------

    Execution Context (xctx) Evaluation stack.
    
---------------------------------------------------------------------------- */


/**
 * @brief Set the xctx evaluation result.
 * @param value to set.
 * @param xctx of caller.
 *
 * This is called while evaluating an adaptive script each time a result is
 * produced to set xctx->evaluation_result. Once evaluate of a script is
 * complete, this is the final return value.
 *
 * This function will call the afw_value_release() of the previous
 * xctx->evaluation_result. If you need to keep the previous evaluation_result,
 * call afw_value_clone_or_reference() to get a reference to it before calling this
 * function.
 */
AFW_DECLARE(void)
afw_xctx_evaluation_result_set(
    afw_xctx_t *xctx,
    const afw_value_t *value);


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


AFW_STACK_STRUCT(afw_xctx_evaluation_stack_s,
    afw_xctx_evaluation_stack_entry_t);


#ifdef AFW_DEBUG_EVALUATION
#include <stdio.h>
#include "afw_utf8.h"

#define AFW_XCTX_DEBUG_EVALUATION_PRINT(xctx, op_z, extra_fmt_z, ...) \
do { \
    if ((xctx) && (xctx)->env && (xctx)->env->debug_fd && \
        afw_flag_is_active( \
            (xctx)->env->flag_index_debug_evaluation, (xctx))) \
    { \
        if (afw_flag_is_active( \
                (xctx)->env->flag_index_debug_evaluation_detail, (xctx))) \
        { \
            fprintf((xctx)->env->debug_fd, \
                ">debug eval %s" extra_fmt_z " (%s)\n", \
                op_z, ##__VA_ARGS__, \
                afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        } \
        else { \
            fprintf((xctx)->env->debug_fd, \
                ">debug eval %s (%s)\n", \
                op_z, afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        } \
        fflush((xctx)->env->debug_fd); \
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
#define afw_xctx_evaluation_stack_push_value(VALUE, xctx) \
do { \
    const afw_value_t *_afw_eval_push_value = (VALUE); \
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
    AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
        "push_value", \
        " value %p inf " AFW_UTF8_FMT, \
        (const void *)_afw_eval_push_value, \
        _afw_eval_inf_len, _afw_eval_inf_s); \
    afw_stack_push_direct((xctx)->evaluation_stack, (xctx)); \
    (xctx)->evaluation_stack->top->value = _afw_eval_push_value; \
} while (0)
#else
#define afw_xctx_evaluation_stack_push_value(VALUE, xctx) \
    afw_stack_push_direct(xctx->evaluation_stack, xctx); \
    (xctx)->evaluation_stack->top->value = VALUE
#endif


/**
 * @brief Push PARAMETER_NUMBER onto execution stack.
 * @param PARAMETER_NUMBER
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_push_parameter_number( \
    PARAMETER_NUMBER, xctx) \
do { \
    afw_size_t _afw_eval_push_pn = (PARAMETER_NUMBER); \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
        "push_parameter_number", \
        " n " AFW_SIZE_T_FMT, \
        _afw_eval_push_pn); \
    afw_stack_push_direct((xctx)->evaluation_stack, (xctx)); \
    (xctx)->evaluation_stack->top->parameter_number = _afw_eval_push_pn; \
    afw_stack_push_direct((xctx)->evaluation_stack, (xctx)); \
    (xctx)->evaluation_stack->top->entry_id = afw_s_parameter_number; \
} while (0)
#else
#define afw_xctx_evaluation_stack_push_parameter_number( \
    PARAMETER_NUMBER, xctx) \
    afw_stack_push_direct(xctx->evaluation_stack, xctx); \
    (xctx)->evaluation_stack->top->parameter_number =PARAMETER_NUMBER; \
    afw_stack_push_direct(xctx->evaluation_stack, xctx); \
    (xctx)->evaluation_stack->top->entry_id = afw_s_parameter_number
#endif


/**
 * @brief Pop top PARAMETER_NUMBER or VALUE off execution stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_pop(xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
        "pop", ""); \
    if ((xctx)->evaluation_stack->top->entry_id == afw_s_parameter_number) { \
        afw_stack_pop((xctx)->evaluation_stack, (xctx)); \
    } \
    afw_stack_pop((xctx)->evaluation_stack, (xctx)); \
} while (0)
#else
#define afw_xctx_evaluation_stack_pop(xctx) \
if (xctx->evaluation_stack->top->entry_id == afw_s_parameter_number) { \
    afw_stack_pop(xctx->evaluation_stack, xctx); \
} \
afw_stack_pop(xctx->evaluation_stack, xctx)
#endif


/**
 * @brief Pop top VALUE off execution stack.
 * @param xctx of caller.
 * 
 * Use only when you're positive top of stack is VALUE.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_pop_value(xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
        "pop_value", ""); \
    afw_stack_pop((xctx)->evaluation_stack, (xctx)); \
} while (0)
#else
#define afw_xctx_evaluation_stack_pop_value(xctx) \
afw_stack_pop(xctx->evaluation_stack, xctx)
#endif



/**
 * @brief Pop top PARAMETER_NUMBER off execution stack.
 * @param xctx of caller.
 *
 * Use only when you're positive top of stack is PARAMETER_NUMBER.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_pop_parameter_number(xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
        "pop_parameter_number", ""); \
    afw_stack_pop((xctx)->evaluation_stack, (xctx)); \
    afw_stack_pop((xctx)->evaluation_stack, (xctx)); \
} while (0)
#else
#define afw_xctx_evaluation_stack_pop_parameter_number(xctx) \
afw_stack_pop(xctx->evaluation_stack, xctx); \
afw_stack_pop(xctx->evaluation_stack, xctx)
#endif


/**
 * @brief Save top of stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_save_top(xctx) \
afw_xctx_evaluation_stack_entry_t *evaluation_stack_save_top = \
(xctx)->evaluation_stack->top; \
AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
    "save_top", "")
#else
#define afw_xctx_evaluation_stack_save_top(xctx) \
afw_xctx_evaluation_stack_entry_t *evaluation_stack_save_top = \
xctx->evaluation_stack->top
#endif


/**
 * @brief Restore top of stack.
 * @param xctx of caller.
 */
#ifdef AFW_DEBUG_EVALUATION
#define afw_xctx_evaluation_stack_restore_top(xctx) \
do { \
    AFW_XCTX_DEBUG_EVALUATION_PRINT((xctx), \
        "restore_top", ""); \
    (xctx)->evaluation_stack->top = evaluation_stack_save_top; \
} while (0)
#else
#define afw_xctx_evaluation_stack_restore_top(xctx) \
xctx->evaluation_stack->top = evaluation_stack_save_top
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
 * afw_xctx_scope_symbol_exists_by_name() / get_value_address_by_name() for
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

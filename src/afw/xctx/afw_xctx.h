// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Execution Context (xctx) Implementation Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_XCTX_H__
#define __AFW_XCTX_H__

#include "afw_interface.h"

/**
 * @defgroup afw_xctx Execution context (xctx)
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework Execution Context (xctx).
 *
 * @{
 */

/**
 * @file afw_xctx.h
 * @brief Adaptive Framework Execution Context (xctx) Implementation Header.
 */

AFW_BEGIN_DECLARES

/** Name of base xctx. */
#define AFW_XCTX_Q_NAME_BASE "base"

/** Name of base xctx. */
#define AFW_XCTX_s_NAME_BASE afw_s_base

/** Name of request thread xctx. */
#define AFW_XCTX_Q_NAME_REQUEST_THREAD "request_thread"

/** Name of request thread xctx. */
#define AFW_XCTX_s_NAME_REQUEST_THREAD afw_s_request_thread

/** Name of request session xctx. */
#define AFW_XCTX_Q_NAME_REQUEST_SESSION "request_session"

/** Name of request session xctx. */
#define AFW_XCTX_s_NAME_REQUEST_SESSION afw_s_request_session

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
 * allocated in the xctx including any children xctxs.  Call
 * afw_environment_create() to create the initial xctx.
 *
 * The xctx authorization mode is set to user.  Use
 * AFW_XCTX_AUTHORIZATION_MODE_BEGIN/END to change it.
 */
AFW_DECLARE(afw_xctx_t *)
afw_xctx_create(
    const afw_utf8_t *name, afw_integer_t number, afw_xctx_t *xctx);


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
const afw_value_t *this_PREVIOUS_MODE = \
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
    int local_top; /** @fixme Will be removed.*/
    afw_xctx_scope_t *previous_scope;
    afw_xctx_scope_t *previous_static_scope;
    afw_size_t scope_number;
    afw_size_t symbol_count;
    /* This is often used as argv so symbols[0] may not be a symbol. */
    const afw_value_t *symbol_values[1];
};



/**
 * @brief Begin begin a scope.
 * @param block associated with this scope.
 * @param xctx of caller.
 * @return New xctx scope.
 */
AFW_DECLARE(afw_xctx_scope_t *)
afw_xctx_scope_begin(
    const afw_value_block_t *block,
    afw_xctx_t *xctx);


/**
 * @brief Begin begin a scope for closure.
 * @param xctx of caller.
 * @return Parameter to pass to end_stack_frame.
 */
AFW_DECLARE(void)
afw_xctx_scope_closure_begin(
    const afw_xctx_scope_t *enclosure_scope,
    afw_xctx_t *xctx);


/**
 * @brief Jump to a scope.
 * @param scope returned from corresponding to afw_xctx_scope_begin().
 * @param xctx of caller.
 * 
 * This will normally only be used at the beginning of AFW_CATCH* and
 * AFW_FINALLY to set the scope to the one associated with the AFW_TRY without
 * releasing it in the case where an adaptive value is going to be evaluated
 * that needs to be in that scope. Then the associated AFW_FINALLY should call
 * afw_xctx_scope_release() before it ends to release the scope.
 */
AFW_DEFINE(void)
afw_xctx_scope_jump_to(
    const afw_xctx_scope_t *scope, afw_xctx_t *xctx);


/**
 * @brief Release scope.
 * @param scope returned from corresponding to afw_xctx_scope_begin().
 * @param xctx of caller.
 * 
 * This will release all resources allocated in the scope including and
 * children. This is a release, not a destroy, so if there is a referenced
 * closure for any scope, it will just have its reference count decremented.
 */
AFW_DECLARE(void)
afw_xctx_scope_release(
    const afw_xctx_scope_t *scope, afw_xctx_t *xctx);


/**
 * @brief Defined a dynamic variable in current xctx frame.
 * @param name Name of variable.
 * @param value Value to set.
 * @param xctx of caller.
 * @return value or NULL if not found.
 *
 * Dynamic variables are ones that are not known at compile time.  They
 * are defined in the current xctx frame.  If the variable is already
 * defined in the current xctx frame, an error is thrown.
 */
AFW_DECLARE(void)
afw_xctx_scope_dynamic_variable_define(const afw_utf8_t *name,
    const afw_value_t *value, afw_xctx_t *xctx);


/**
 * @brief Set a dynamic variable in xctx.
 * @param name Name of variable.
 * @param value Value to set.
 * @param xctx of caller.
 * @return value or NULL if not found.
 *
 * The most recently defined named variable in xctx will be set to the
 * value.  If the variable is not found or const an error is thrown.
 */
AFW_DECLARE(void)
afw_xctx_scope_dynamic_variable_set(const afw_utf8_t *name,
    const afw_value_t *value, afw_xctx_t *xctx);



/* ----------------------------------------------------------------------------

    Execution Context (xctx) Evaluation Stack
    
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


AFW_STACK_STRUCT(afw_xctx_evaluation_stack_s,
    afw_xctx_evaluation_stack_entry_t);


/**
 * @brief Push VALUE onto execution stack.
 * @param VALUE.
 * @param xctx of caller.
 * @return Don't use.
 */
#define afw_xctx_evaluation_stack_push_value(VALUE, xctx) \
    afw_stack_push_direct(xctx->evaluation_stack, xctx); \
    (xctx)->evaluation_stack->top->value = VALUE


/**
 * @brief Push PARAMETER_NUMBER onto execution stack.
 * @param PARAMETER_NUMBER.
 * @param xctx of caller.
 */
#define afw_xctx_evaluation_stack_push_parameter_number( \
    PARAMETER_NUMBER, xctx) \
    afw_stack_push_direct(xctx->evaluation_stack, xctx); \
    (xctx)->evaluation_stack->top->parameter_number =PARAMETER_NUMBER; \
    afw_stack_push_direct(xctx->evaluation_stack, xctx); \
    (xctx)->evaluation_stack->top->entry_id = &afw_s_parameter_number


/**
 * @brief Pop top PARAMETER_NUMBER or VALUE off execution stack.
 * @param xctx of caller.
 */
#define afw_xctx_evaluation_stack_pop(xctx) \
if (xctx->evaluation_stack->top->entry_id == &afw_s_parameter_number) { \
    afw_stack_pop(xctx->evaluation_stack, xctx); \
} \
afw_stack_pop(xctx->evaluation_stack, xctx)


/**
 * @brief Pop top VALUE off execution stack.
 * @param xctx of caller.
 * 
 * Use only when you're positive top of stack is VALUE.
 */
#define afw_xctx_evaluation_stack_pop_value(xctx) \
afw_stack_pop(xctx->evaluation_stack, xctx)



/**
 * @brief Pop top PARAMETER_NUMBER off execution stack.
 * @param xctx of caller.
 *
 * Use only when you're positive top of stack is PARAMETER_NUMBER.
 */
#define afw_xctx_evaluation_stack_pop_parameter_number(xctx) \
afw_stack_pop(xctx->evaluation_stack, xctx); \
afw_stack_pop(xctx->evaluation_stack, xctx)


/**
 * @brief Save top of stack.
 * @param xctx of caller.
 */
#define afw_xctx_evaluation_stack_save_top(xctx) \
afw_xctx_evaluation_stack_entry_t *evaluation_stack_save_top = \
xctx->evaluation_stack->top


/**
 * @brief Save top of stack.
 * @param xctx of caller.
 */
#define afw_xctx_evaluation_stack_restore_top(xctx) \
xctx->evaluation_stack->top = evaluation_stack_save_top


/* ----------------------------------------------------------------------------

    Execution Context (xctx) Qualifier and Qualified Variables
    
---------------------------------------------------------------------------- */

/**
 * @brief Get a variable from xctx stack.
 * @param qualifier of variable.
 * @param name of variable.
 * @param xctx of caller.
 * @return value or NULL if not found.
 *
 * The stack is searched from newest to oldest.
 */
AFW_DECLARE(const afw_value_t *)
afw_xctx_get_qualified_variable(
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

    /** @brief Get routine or NULL if get not allowed. */
    afw_xctx_get_variable_t get;

    /** @brief Data that will be passed to get/set. */
    void *data;

    /** @brief Work area used by get/set. */
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
 * @param context_object.
 * @param secure access to this qualifier is allowed.
 * @param p used while evaluating variable values.
 * @param xctx of caller.
 * @return Stack top.
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
 * @param qualifier_object.
 * @param secure access to this qualifier is allowed.
 * @param get routine or NULL.
 * @param data to be passed to get/set.
 * @param p used while evaluating variable values.
 * @param xctx of caller.
 * @return New qualifier stack entry.
 * 
 * The "get" function is the fastest way to get the value of a known
 * variable for this pushed qualifier.  The qualifier_object is used
 * when the get_next_property() method is needed.
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
    afw_xctx_get_variable_t get,
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
 * @return Stack top.
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
 * @brief Create object to access active variables for a qualifier.
 * @param qualifier_name.
 * @param for_testing
 * @param p to use.
 * @param xctx of caller.
 * @return qualifier object.
 */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifier_object_create(
    const afw_utf8_t *qualifier,
    afw_boolean_t for_testing,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create object to access active qualified variables.
 * @param for_testing
 * @param p to use.
 * @param xctx of caller.
 * @return qualifier object.
 */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifiers_object_create(
    afw_boolean_t for_testing,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_XCTX_H__ */

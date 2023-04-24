// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Stack Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_STACK_H__
#define __AFW_STACK_H__

#include "afw_interface.h"

/**
 * @defgroup afw_stack Stack
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework Stack.
 * 
 * @{
 */

/**
 * @file afw_stack.h
 * @brief Adaptive Framework stack support
 */

AFW_BEGIN_DECLARES

/**
 * @brief Create a stack for the specified typedef.
 * @param typedef_name (See AFW_STACK_STRUCT macro in afw_common.h)
 * @param initial_count
 * @param maximum_count or 0 if no limit
 * @param create_subpool_pool if true.
 * @param p for stack
 * @param xctx of caller
 * @return pointer to created stack of type typedef_name *.
 */
#define afw_stack_create(typedef_name, \
    initial_count, maximum_count, create_subpool_pool, p, xctx) \
(typedef_name *)afw_stack_create_impl( \
    sizeof(*(((typedef_name *)0))->first), \
    initial_count, maximum_count, create_subpool_pool, p, xctx);



/**
 * @brief Create a stack implementation.
 * @param entry_size
 * @param initial_count
 * @param maximum_count or 0 for no limit.
 * @param create_subpool_pool if true.
 * @param p for stack.
 * @param xctx of caller.
 * @return stack
 *
 * This is normally called by macro afw_stack_create().
 */
AFW_DECLARE(afw_stack_t *)
afw_stack_create_impl(
    afw_size_t entry_size,
    afw_size_t initial_count,
    afw_size_t maximum_count,
    afw_boolean_t create_subpool_pool,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Release stack implementation.
 * @param instance of AFW_STACK_STRUCT().
 * @param xctx of caller.
 */
#define afw_stack_release(instance, xctx) \
afw_stack_release_impl( &((instance)->pub), xctx)


/**
 * @brief Release stack implementation.
 * @param instance
 * @param xctx of caller.
 * 
 * This is normally called by macro afw_stack_release().
 */
AFW_DECLARE(void)
afw_stack_release_impl(
    afw_stack_t *instance,
    afw_xctx_t *xctx);


/**
 * @brief Copy stack.
 * @param instance of AFW_STACK_STRUCT().
 * @param count is place to return count.
 * @param ptr is place to return pointer to copy.
 * @param p to use for result.
 * @param xctx of caller.
 */
#define afw_stack_copy(instance, count, ptr, p, xctx) \
afw_stack_copy_impl( &((instance)->pub), count, ptr, p, xctx)


/**
 * @brief Copy stack implementation.
 * @param instance
 * @param count is place to return count or NULL.
 * @param ptr is place to return pointer to copy.
 * @param p to use for result.
 * @param xctx of caller.
 *
 * This is normally called by macro afw_stack_copy().
 */
AFW_DECLARE(void)
afw_stack_copy_impl(
    afw_stack_t *instance,
    afw_size_t *count,
    const void ***ptr,
    const afw_pool_t *p,
    afw_xctx_t *xctx);
 
 
/**
 * @brief Copy and release stack.
 * @param instance of AFW_STACK_STRUCT().
 * @param count is place to return count or NULL.
 * @param ptr is place to return pointer to copy.
 * @param p to use for result.
 * @param xctx of caller.
 */
#define afw_stack_copy_and_release(instance, count, ptr, p, xctx) \
afw_stack_copy(instance, (count), (const void ***)(ptr), (p), (xctx)); \
afw_stack_release(instance, (xctx))


/**
 * @brief Extend stack implementation.
 * @param instance
 * @param xctx of caller.
 * 
 * This is normally called by a afw_stack_push*() macro.
 */
AFW_DECLARE(void)
afw_stack_extend_impl(
    afw_stack_t * instance,
    afw_xctx_t *xctx);


/**
 * @brief Determine is a stack is empty.
 * @param instance.
 *
 * stack->top will point to uninitialize storage.  If needed
 * the stack's extend will be called, so stack first and end
 * may change after afw_stack_push*().
 */
#define afw_stack_is_empty(instance) \
    ((instance)->top < (instance)->first)


/**
 * @brief Decrement stack->top to location of previous entry.
 * @param instance.
 * @param xctx of caller.
 *
 * An error will be thrown on underflow.
 */
#define afw_stack_pop(instance, xctx) \
    if ((instance)->top < ((instance)->first)) { \
        AFW_THROW_ERROR_Z(general, "stack underflow", xctx); \
    } \
    ((instance)->top)--;
 
  
/**
 * @brief Number of entries in stack.
 * @param instance.
 * @param xctx of caller.
 * @return number of entries.
 */
#define afw_stack_count(instance, xctx) \
    ((instance)->top + 1 - (instance)->first))


/**
 * @brief Increment stack->top to location of next entry and returns *top.
 * @param instance of AFW_STACK_STRUCT().
 * @param xctx of caller.
 * @return stack->top.
 *
 * stack->top will point to uninitialize storage.  If needed
 * the stack's extend will be called, so stack first and end
 * may change after afw_stack_push*().
 */
#define afw_stack_push(instance, xctx) \
    ((instance)->top)++; \
    if ((instance)->top >= ((instance)->end)) { \
        afw_stack_extend_impl(&((instance)->pub), xctx); \
    } \
    *((instance)->top)


/**
 * @brief Increment stack->top to location of next entry and returns top.
 * @param instance of AFW_STACK_STRUCT().
 * @param xctx of caller.
 * @return Don't count return. Use (instance)->top.
 *
 * stack->top will point to uninitialize storage.  If needed
 * the stack's extend will be called, so stack first and end
 * may change after afw_stack_push*().
 */
#define afw_stack_push_direct(instance, xctx) \
    ((instance)->top)++; \
    if ((instance)->top >= ((instance)->end)) { \
        afw_stack_extend_impl(&((instance)->pub), xctx); \
    }


/**
 * @brief Increment stack->top to location of next entry and get entry.
 * @param instance of AFW_STACK_STRUCT().
 * @param xctx of caller.
 * @return stack->top.
 *
 * stack->top will point to uninitialize storage.  If needed
 * the stack's extend will be called, so stack first and end
 * may change after afw_stack_push*().
 */
#define afw_stack_push_and_get_entry(instance, entry, xctx) \
    afw_stack_push_direct(instance, xctx); \
    entry = (instance)->top


/**
 * @brief Increment stack->top to location of next entry.
 * @param instance.
 *
 * stack->top will point to uninitialize storage.  If needed
 * the stack's extend will be called, so stack first and end
 * may change after afw_stack_push*().
 */
#define afw_stack_set_empty(instance) \
    (instance)->top = ((instance)->first) - 1


/**
 * @internal
 * @brief Internal function called during xctx creation.
 * @param xctx to set evaluation stack in.
 */
AFW_DEFINE_INTERNAL(void)
afw_stack_internal_set_qualifier_stack(
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Internal function called during xctx creation.
 * @param xctx to set evaluation stack in.
 */
AFW_DEFINE_INTERNAL(void)
afw_stack_internal_set_evaluation_stack(
    afw_xctx_t *xctx);
    
    
AFW_END_DECLARES

/** @} */

#endif /* __AFW_STACK_H__ */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Atomic Operation Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ATOMIC_H__
#define __AFW_ATOMIC_H__

#include "afw_interface.h"

/**
 * @defgroup afw_atomic Atomic
 * @ingroup afw_c_api_public
 *
 * Atomic functions
*
* @{
 */

/**
 * @file afw_atomic.h
 * @brief Adaptive Framework atomic operations
 */

AFW_BEGIN_DECLARES

/**
 * @brief Integer atomic compare and swap
 * @param mem pointer to afw_uint32_t to compare and swap
 * @param expected value.
 * @param desired value.
 * @return true if successful.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_atomic_uint32_cas(
    AFW_ATOMIC afw_uint32_t *mem,
    afw_uint32_t expected, afw_uint32_t desired)
{
#ifdef AFW_WINDOWS
    LONG initial;
    initial = InterlockedCompareExchange(mem, desired, expected);
    return initial == expected;
#else
    return atomic_compare_exchange_strong(mem, &expected, desired);
#endif
}



/**
 * @brief Integer atomic decrement
 * @param mem pointer to afw_integer_t to compare and swap
 * @param expected value.
 * @param desired value.
 * @return true if successful.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_atomic_integer_cas(
    AFW_ATOMIC afw_integer_t *mem,
    afw_integer_t expected, afw_integer_t desired)
{
#ifdef AFW_WINDOWS
    LONG64 initial;
    initial = InterlockedCompareExchange64(mem, desired, expected);
    return initial == expected;
#else
    return atomic_compare_exchange_strong(mem, &expected, desired);
#endif
}


/**
 * @brief 32-bit atomic decrement
 * @param mem pointer to 32-bits to decrement
 */
AFW_DEFINE_STATIC_INLINE(afw_uint32_t)
afw_atomic_uint32_decrement(
    AFW_ATOMIC afw_uint32_t *mem)
{
#ifdef AFW_WINDOWS
    return apr_atomic_dec32(mem);
#else
    return --(*mem);
#endif
}


/**
 * @brief Integer atomic decrement
 * @param mem pointer to afw_integer_t to decrement
 */
AFW_DEFINE_STATIC_INLINE(afw_integer_t)
afw_atomic_integer_decrement(
    AFW_ATOMIC afw_integer_t *mem)
{
#ifdef AFW_WINDOWS
    return InterlockedDecrement64(mem);
#else
    return --(*mem);
#endif
}


/**
 * @brief 32-bit atomic increment
 * @param mem pointer to 32-bits to increment
 */
AFW_DEFINE_STATIC_INLINE(afw_uint32_t)
afw_atomic_uint32_increment(
    AFW_ATOMIC afw_uint32_t *mem)
{
#ifdef AFW_WINDOWS
    return apr_atomic_inc32(mem);
#else
    return  ++(*mem);
#endif
}

/**
 * @brief Integer atomic increment
 * @param mem pointer to afw_integer_t to increment
 */
AFW_DEFINE_STATIC_INLINE(afw_integer_t)
afw_atomic_integer_increment(
    AFW_ATOMIC afw_integer_t *mem)
{
#ifdef AFW_WINDOWS
    return InterlockedIncrement64(mem);
#else
    return ++(*mem);
#endif
}

AFW_END_DECLARES

/** @} */

#endif /* __AFW_ATOMIC_H__ */

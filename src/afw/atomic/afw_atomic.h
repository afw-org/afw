// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Atomic Operation Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ATOMIC_H__
#define __AFW_ATOMIC_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_atomic
 * @{
 */

/**
 * @file afw_atomic.h
 * @brief Atomic load/store and compare-exchange helpers.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Integer atomic compare and swap
 * @param mem pointer to afw_uint32_t to compare and swap
 * @param expected value.
 * @param desired value.
 * @return true if successful.
 *
 * `expected` is by value so the caller is not updated (C11 CAS
 * would otherwise write the found value back).
 */
AFW_DECLARE(afw_boolean_t)
afw_atomic_uint32_cas(
    AFW_ATOMIC afw_uint32_t *mem,
    afw_uint32_t expected, afw_uint32_t desired);

/**
 * @brief Integer atomic compare and swap
 * @param mem pointer to afw_integer_t to compare and swap
 * @param expected value.
 * @param desired value.
 * @return true if successful.
 */
AFW_DECLARE(afw_boolean_t)
afw_atomic_integer_cas(
    AFW_ATOMIC afw_integer_t *mem,
    afw_integer_t expected, afw_integer_t desired);

#ifdef AFW_WINDOWS

/** @brief 32-bit atomic decrement */
#define afw_atomic_uint32_decrement(_mem) \
    ((afw_uint32_t)InterlockedDecrement((LONG *)(_mem)))

/** @brief Integer atomic decrement */
#define afw_atomic_integer_decrement(_mem) \
    (InterlockedDecrement64(_mem))

/** @brief 32-bit atomic increment */
#define afw_atomic_uint32_increment(_mem) \
    ((afw_uint32_t)InterlockedIncrement((LONG *)(_mem)))

/** @brief Integer atomic increment */
#define afw_atomic_integer_increment(_mem) \
    (InterlockedIncrement64(_mem))

#else

/** @brief 32-bit atomic decrement */
#define afw_atomic_uint32_decrement(_mem) (--(*(_mem)))

/** @brief Integer atomic decrement */
#define afw_atomic_integer_decrement(_mem) (--(*(_mem)))

/** @brief 32-bit atomic increment */
#define afw_atomic_uint32_increment(_mem) (++(*(_mem)))

/** @brief Integer atomic increment */
#define afw_atomic_integer_increment(_mem) (++(*(_mem)))

#endif

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ATOMIC_H__ */

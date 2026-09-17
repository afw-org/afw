// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Atomic Operation Implementation
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_atomic.c
 * @brief Atomic compare-exchange (expected is by value).
 */

#include "afw_internal.h"


AFW_DEFINE(afw_boolean_t)
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


AFW_DEFINE(afw_boolean_t)
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

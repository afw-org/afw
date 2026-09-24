// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework sort
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_SORT_H__
#define __AFW_SORT_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_sort
 * @{
 */

/**
 * @file afw_sort.h
 * @brief Sort a pointer list by name.
 *
 * The name does not pick an algorithm. The order matches
 * afw_binary_search_compare_names() for values, and the same
 * shared-prefix-then-length order for UTF-8.
 */

/**
 * @brief Compare two elements.
 * @param a element pointer from the array.
 * @param b element pointer from the array.
 * @param data caller data.
 * @return negative if a is before b, 0 if equal, positive if a is after b.
 */
typedef int (*afw_sort_compare_t)(
    const void *a,
    const void *b,
    void *data);


/**
 * @brief Sort with a compare callback.
 * @param base array of element pointers.
 * @param count number of elements.
 * @param compare called with the pointers stored in base.
 * @param data passed to compare.
 *
 * The smaller side is recursed and the larger side is looped, so the
 * depth stays logarithmic.
 */
AFW_DECLARE(void)
afw_sort(
    const void **base,
    afw_size_t count,
    afw_sort_compare_t compare,
    void *data);


/**
 * @brief Sort by a `const afw_utf8_t *` at the start of each element.
 * @param base array of pointers.
 * @param count number of elements.
 */
AFW_DECLARE(void)
afw_sort_by_name(
    const void **base,
    afw_size_t count);


/**
 * @brief Sort by a `const afw_value_t *` at the start of each element.
 * @param base array of pointers.
 * @param count number of elements.
 */
AFW_DECLARE(void)
afw_sort_by_name_value(
    const void **base,
    afw_size_t count);


/**
 * @brief Sort by a `const afw_utf8_t *` at an offset.
 * @param base array of pointers.
 * @param count number of elements.
 * @param offset byte offset of the `const afw_utf8_t *` field.
 */
AFW_DECLARE(void)
afw_sort_by_name_with_offset(
    const void **base,
    afw_size_t count,
    afw_size_t offset);


/**
 * @brief Sort by a `const afw_value_t *` at an offset.
 * @param base array of pointers.
 * @param count number of elements.
 * @param offset byte offset of the `const afw_value_t *` field.
 */
AFW_DECLARE(void)
afw_sort_by_name_value_with_offset(
    const void **base,
    afw_size_t count,
    afw_size_t offset);

/** @} */

#endif /* __AFW_SORT_H__ */

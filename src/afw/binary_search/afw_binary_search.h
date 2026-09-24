// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework binary search
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_BINARY_SEARCH_H__
#define __AFW_BINARY_SEARCH_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_binary_search
 * @{
 */

/**
 * @file afw_binary_search.h
 * @brief Binary search of a name-sorted pointer list.
 *
 * Each element points at a struct whose first field is
 * `const afw_value_t *name`. The names are strings, sorted by
 * UTF-8 bytes. Generate emits that order. The runtime does not sort.
 */

/**
 * @brief Find an element by string name.
 * @param base array of pointers. Each points at a struct whose first
 *    field is `const afw_value_t *name`.
 * @param count number of elements. The NULL terminator is not included.
 * @param name lookup name. A value that is not a string does not match.
 * @return the element pointer, or NULL if absent.
 */
AFW_DECLARE(const void *)
afw_binary_search_by_name(
    const void * const *base,
    afw_size_t count,
    const afw_value_t *name);

/** @} */

#endif /* __AFW_BINARY_SEARCH_H__ */

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
 * `const afw_value_t *name`. Generate emits const property names
 * sorted by UTF-8 bytes. The runtime does not sort.
 *
 * The compare is the shared prefix, then the length. A fixed-size
 * scalar uses sizeof(internal) on both sides, so that is a memory
 * compare of internal. afw_utf8_t and afw_memory_t are pointer then
 * size, so the compare uses those bytes.
 */

/**
 * @brief Find an element by UTF-8 name.
 * @param base array of pointers. Each points at a struct whose first
 *    field is `const afw_value_t *name`.
 * @param count number of elements. The NULL terminator is not included.
 * @param name lookup bytes. The list names are strings.
 * @return the element pointer, or NULL if absent.
 */
AFW_DECLARE(const void *)
afw_binary_search_by_name(
    const void * const *base,
    afw_size_t count,
    const afw_utf8_t *name);


/**
 * @brief Find an element by scalar name value.
 * @param base array of pointers. Each points at a struct whose first
 *    field is `const afw_value_t *name`.
 * @param count number of elements. The NULL terminator is not included.
 * @param name lookup value. It must be an evaluated scalar, and the
 *    stored name must be the same data type.
 * @return the element pointer, or NULL if absent.
 */
AFW_DECLARE(const void *)
afw_binary_search_by_name_value(
    const void * const *base,
    afw_size_t count,
    const afw_value_t *name);

/** @} */

#endif /* __AFW_BINARY_SEARCH_H__ */

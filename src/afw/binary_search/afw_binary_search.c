// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework binary search
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_binary_search.c
 * @brief Binary search of a name-sorted pointer list.
 */

#include "afw_internal.h"

/* The looked-up name as UTF-8, or NULL if it is not a string. */
static const afw_utf8_t *
impl_name_utf8(const afw_value_t *name)
{
    if (!afw_value_is_string(name)) {
        return NULL;
    }
    return &((const afw_value_string_t *)name)->internal;
}

/* Name field at the start of an element. */
static const afw_utf8_t *
impl_element_name_utf8(const void *element)
{
    const afw_value_t *name;

    name = *(const afw_value_t * const *)element;
    return impl_name_utf8(name);
}

/*
 * Implementation of afw_binary_search_by_name().
 */
AFW_DEFINE(const void *)
afw_binary_search_by_name(
    const void * const *base,
    afw_size_t count,
    const afw_value_t *name)
{
    const afw_utf8_t *key;
    afw_size_t low;
    afw_size_t high;

    key = impl_name_utf8(name);
    if (!base || !key || count == 0) {
        return NULL;
    }

    low = 0;
    high = count;
    while (low < high) {
        afw_size_t mid;
        const afw_utf8_t *element_name;
        int cmp;

        mid = low + ((high - low) / 2);
        element_name = impl_element_name_utf8(base[mid]);
        if (!element_name) {
            return NULL;
        }
        cmp = afw_utf8_compare(key, element_name);
        if (cmp == 0) {
            return base[mid];
        }
        if (cmp < 0) {
            high = mid;
        }
        else {
            low = mid + 1;
        }
    }

    return NULL;
}

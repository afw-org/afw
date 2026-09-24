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
#include <string.h>

static const afw_utf8_t impl_ctype_utf8 =
    AFW_UTF8_LITERAL("afw_utf8_t");
static const afw_utf8_t impl_ctype_memory =
    AFW_UTF8_LITERAL("afw_memory_t");

/* Shared prefix, then the shorter length. */
static int
impl_compare_memory(const afw_memory_t *a, const afw_memory_t *b)
{
    afw_size_t n;
    int cmp;

    n = (a->size < b->size) ? a->size : b->size;
    if (n > 0 && a->ptr && b->ptr) {
        cmp = memcmp(a->ptr, b->ptr, n);
        if (cmp != 0) {
            return cmp;
        }
    }
    if (a->size < b->size) {
        return -1;
    }
    if (a->size > b->size) {
        return 1;
    }
    return 0;
}

/* Scalar internal as pointer and size. False if not an evaluated scalar. */
static afw_boolean_t
impl_scalar_memory(
    const afw_value_t *value, afw_memory_t *memory)
{
    const afw_data_type_t *type;
    const afw_utf8_t *utf8;

    if (!value || !value->inf) {
        return false;
    }
    type = value->inf->is_evaluated_of_data_type;
    if (!type || !type->scalar) {
        return false;
    }

    /*
     * afw_utf8_t and afw_memory_t are pointer then size. Other scalars
     * hold internal immediately after the value header.
     */
    if (afw_utf8_equal(&type->cType, &impl_ctype_utf8) ||
        afw_utf8_equal(&type->cType, &impl_ctype_memory))
    {
        utf8 = (const afw_utf8_t *)
            ((const char *)value + sizeof(afw_value_t));
        memory->ptr = (const afw_octet_t *)utf8->s;
        memory->size = utf8->len;
        return true;
    }

    memory->ptr = (const afw_octet_t *)
        ((const char *)value + sizeof(afw_value_t));
    memory->size = type->c_type_size;
    return true;
}

/* Name value at the start of an element. */
static const afw_value_t *
impl_element_name(const void *element)
{
    return *(const afw_value_t * const *)element;
}

static const void *
impl_search(
    const void * const *base,
    afw_size_t count,
    const afw_memory_t *key,
    const afw_data_type_t *key_type)
{
    afw_size_t low;
    afw_size_t high;

    if (!base || !key || count == 0) {
        return NULL;
    }

    low = 0;
    high = count;
    while (low < high) {
        afw_size_t mid;
        const afw_value_t *name;
        const afw_data_type_t *name_type;
        afw_memory_t element;
        int cmp;

        mid = low + ((high - low) / 2);
        name = impl_element_name(base[mid]);
        if (!name || !name->inf) {
            return NULL;
        }
        name_type = name->inf->is_evaluated_of_data_type;
        if (name_type != key_type) {
            return NULL;
        }
        if (!impl_scalar_memory(name, &element)) {
            return NULL;
        }
        cmp = impl_compare_memory(key, &element);
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

/*
 * Implementation of afw_binary_search_compare_names().
 */
AFW_DEFINE(int)
afw_binary_search_compare_names(
    const afw_value_t *a,
    const afw_value_t *b)
{
    const afw_data_type_t *type_a;
    const afw_data_type_t *type_b;
    afw_memory_t memory_a;
    afw_memory_t memory_b;

    if (a == b) {
        return 0;
    }
    type_a = (a && a->inf) ? a->inf->is_evaluated_of_data_type : NULL;
    type_b = (b && b->inf) ? b->inf->is_evaluated_of_data_type : NULL;
    if (type_a != type_b) {
        if (!type_a) {
            return -1;
        }
        if (!type_b) {
            return 1;
        }
        if ((const char *)type_a < (const char *)type_b) {
            return -1;
        }
        return 1;
    }
    if (!impl_scalar_memory(a, &memory_a) ||
        !impl_scalar_memory(b, &memory_b))
    {
        return 0;
    }
    return impl_compare_memory(&memory_a, &memory_b);
}


/*
 * Implementation of afw_binary_search_by_name().
 */
AFW_DEFINE(const void *)
afw_binary_search_by_name(
    const void * const *base,
    afw_size_t count,
    const afw_utf8_t *name)
{
    afw_memory_t key;

    if (!name) {
        return NULL;
    }
    key.ptr = (const afw_octet_t *)name->s;
    key.size = name->len;
    return impl_search(base, count, &key, afw_data_type_string);
}

/*
 * Implementation of afw_binary_search_by_name_value().
 */
AFW_DEFINE(const void *)
afw_binary_search_by_name_value(
    const void * const *base,
    afw_size_t count,
    const afw_value_t *name)
{
    const afw_data_type_t *type;
    afw_memory_t key;

    if (!name || !name->inf) {
        return NULL;
    }
    type = name->inf->is_evaluated_of_data_type;
    if (!impl_scalar_memory(name, &key)) {
        return NULL;
    }
    return impl_search(base, count, &key, type);
}

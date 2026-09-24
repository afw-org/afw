// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework sort
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_sort.c
 * @brief Sort a pointer list by name.
 */

#include "afw_internal.h"
#include <string.h>

typedef enum {
    impl_kind_utf8,
    impl_kind_value
} impl_kind_t;

typedef struct {
    impl_kind_t kind;
    afw_size_t offset;
} impl_state_t;

/* Shared prefix, then the shorter length. Matches the search. */
static int
impl_compare_utf8(const afw_utf8_t *a, const afw_utf8_t *b)
{
    afw_size_t n;
    int cmp;

    if (a == b) {
        return 0;
    }
    if (!a) {
        return -1;
    }
    if (!b) {
        return 1;
    }
    n = (a->len < b->len) ? a->len : b->len;
    if (n > 0 && a->s && b->s) {
        cmp = memcmp(a->s, b->s, n);
        if (cmp != 0) {
            return cmp;
        }
    }
    if (a->len < b->len) {
        return -1;
    }
    if (a->len > b->len) {
        return 1;
    }
    return 0;
}

static int
impl_compare(const void *a, const void *b, const impl_state_t *state)
{
    const char *pa;
    const char *pb;

    pa = (const char *)a + state->offset;
    pb = (const char *)b + state->offset;
    if (state->kind == impl_kind_value) {
        return afw_binary_search_compare_names(
            *(const afw_value_t * const *)pa,
            *(const afw_value_t * const *)pb);
    }
    return impl_compare_utf8(
        *(const afw_utf8_t * const *)pa,
        *(const afw_utf8_t * const *)pb);
}

static void
impl_swap(const void **base, afw_size_t i, afw_size_t j)
{
    const void *tmp;

    tmp = base[i];
    base[i] = base[j];
    base[j] = tmp;
}

static void
impl_insertion(
    const void **base, afw_size_t count, const impl_state_t *state)
{
    afw_size_t i;

    for (i = 1; i < count; i++) {
        const void *value;
        afw_size_t j;

        value = base[i];
        j = i;
        while (j > 0 && impl_compare(base[j - 1], value, state) > 0) {
            base[j] = base[j - 1];
            j--;
        }
        base[j] = value;
    }
}

/* Median of three, left in the last slot. */
static void
impl_median_of_three(
    const void **base, afw_size_t count, const impl_state_t *state)
{
    afw_size_t mid;
    afw_size_t last;

    mid = count / 2;
    last = count - 1;
    if (impl_compare(base[0], base[mid], state) > 0) {
        impl_swap(base, 0, mid);
    }
    if (impl_compare(base[0], base[last], state) > 0) {
        impl_swap(base, 0, last);
    }
    if (impl_compare(base[mid], base[last], state) > 0) {
        impl_swap(base, mid, last);
    }
    impl_swap(base, mid, last);
}

static void
impl_sort(const void **base, afw_size_t count, const impl_state_t *state)
{
    const void *pivot;
    afw_size_t i;
    afw_size_t j;

    if (count < 8) {
        impl_insertion(base, count, state);
        return;
    }

    impl_median_of_three(base, count, state);
    pivot = base[count - 1];
    i = 0;
    for (j = 0; j + 1 < count; j++) {
        if (impl_compare(base[j], pivot, state) < 0) {
            impl_swap(base, i, j);
            i++;
        }
    }
    impl_swap(base, i, count - 1);
    impl_sort(base, i, state);
    impl_sort(base + i + 1, count - i - 1, state);
}

static void
impl_sort_kind(
    const void **base,
    afw_size_t count,
    impl_kind_t kind,
    afw_size_t offset)
{
    impl_state_t state;

    if (!base || count < 2) {
        return;
    }
    state.kind = kind;
    state.offset = offset;
    impl_sort(base, count, &state);
}

/*
 * Implementation of afw_sort_by_name().
 */
AFW_DEFINE(void)
afw_sort_by_name(
    const void **base,
    afw_size_t count)
{
    impl_sort_kind(base, count, impl_kind_utf8, 0);
}

/*
 * Implementation of afw_sort_by_name_value().
 */
AFW_DEFINE(void)
afw_sort_by_name_value(
    const void **base,
    afw_size_t count)
{
    impl_sort_kind(base, count, impl_kind_value, 0);
}

/*
 * Implementation of afw_sort_by_name_with_offset().
 */
AFW_DEFINE(void)
afw_sort_by_name_with_offset(
    const void **base,
    afw_size_t count,
    afw_size_t offset)
{
    impl_sort_kind(base, count, impl_kind_utf8, offset);
}

/*
 * Implementation of afw_sort_by_name_value_with_offset().
 */
AFW_DEFINE(void)
afw_sort_by_name_value_with_offset(
    const void **base,
    afw_size_t count,
    afw_size_t offset)
{
    impl_sort_kind(base, count, impl_kind_value, offset);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework vector (growable contiguous elements)
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_vector.c
 * @brief Growable contiguous elements allocated from an AFW pool.
 */

#include "afw_internal.h"


#define IMPL_SELF(internal) ((afw_vector_t *)(internal))


static afw_size_t
impl_bytes(
    afw_size_t entry_size,
    afw_size_t n,
    afw_xctx_t *xctx)
{
    if (n != 0 && entry_size > AFW_SIZE_T_MAX / n) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    return entry_size * n;
}


static void
impl_grow_to(
    afw_vector_t *self,
    afw_size_t min_allocated,
    afw_xctx_t *xctx)
{
    afw_size_t new_allocated;
    afw_size_t new_bytes;
    afw_size_t used_bytes;
    afw_size_t old_bytes;
    void *new_entries;
    void *old_entries;

    if (min_allocated <= self->allocated) {
        return;
    }

    new_allocated = self->allocated;
    if (new_allocated == 0) {
        new_allocated = 8;
    }
    while (new_allocated < min_allocated) {
        if (new_allocated > AFW_SIZE_T_MAX / 2) {
            new_allocated = min_allocated;
            break;
        }
        new_allocated *= 2;
    }
    if (new_allocated < min_allocated) {
        new_allocated = min_allocated;
    }

    new_bytes = impl_bytes(self->entry_size, new_allocated, xctx);
    new_entries = afw_pool_calloc(self->p, new_bytes, xctx);

    old_entries = self->entries;
    if (old_entries && self->count > 0) {
        used_bytes = impl_bytes(self->entry_size, self->count, xctx);
        memcpy(new_entries, old_entries, used_bytes);
    }
    if (old_entries) {
        old_bytes = impl_bytes(self->entry_size, self->allocated,
            xctx);
        afw_pool_free_memory(self->p, old_entries, old_bytes, xctx);
    }

    self->entries = new_entries;
    self->allocated = new_allocated;
}



/* Create an untyped vector. */
AFW_DEFINE(afw_vector_t *)
afw_vector_create_impl(
    afw_size_t entry_size,
    afw_size_t initial_allocated,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;
    afw_size_t bytes;

    if (entry_size == 0) {
        AFW_THROW_ERROR_Z(argument_error,
            "vector entry size must be greater than 0",
            xctx);
    }

    self = afw_pool_calloc_type(p, afw_vector_t, xctx);
    self->p = p;
    self->entry_size = entry_size;
    self->count = 0;
    self->allocated = initial_allocated;

    if (initial_allocated > 0) {
        bytes = impl_bytes(entry_size, initial_allocated, xctx);
        self->entries = afw_pool_calloc(p, bytes, xctx);
    }

    return self;
}



/* Grow so allocated is at least min_allocated. */
AFW_DEFINE(void)
afw_vector_ensure_impl(
    const afw_vector_t *internal,
    afw_size_t min_allocated,
    afw_xctx_t *xctx)
{
    impl_grow_to(IMPL_SELF(internal), min_allocated, xctx);
}



/* Grow capacity for push. */
AFW_DEFINE(void)
afw_vector_extend_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;
    afw_size_t min_allocated;

    self = IMPL_SELF(internal);
    min_allocated = self->count + 1;
    impl_grow_to(self, min_allocated, xctx);
}



/* Append one slot. Return its index. */
AFW_DEFINE(afw_size_t)
afw_vector_push_index_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;
    afw_size_t index;

    self = IMPL_SELF(internal);
    index = self->count;
    impl_grow_to(self, index + 1, xctx);
    self->count = index + 1;
    return index;
}



/* Remove the last slot. Return its index. */
AFW_DEFINE(afw_size_t)
afw_vector_pop_index_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;

    self = IMPL_SELF(internal);
    if (self->count == 0) {
        AFW_THROW_ERROR_Z(general,
            "vector underflow", xctx);
    }
    self->count--;
    return self->count;
}



/* Make a hole at index and increment count. */
AFW_DEFINE(afw_size_t)
afw_vector_insert_impl(
    const afw_vector_t *internal,
    afw_size_t index,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;
    afw_octet_t *entries;
    afw_size_t nmove;

    self = IMPL_SELF(internal);
    if (index > self->count) {
        AFW_THROW_ERROR_Z(argument_error,
            "vector insert index is past the end",
            xctx);
    }

    impl_grow_to(self, self->count + 1, xctx);
    entries = (afw_octet_t *)self->entries;
    if (index < self->count) {
        nmove = self->count - index;
        memmove(
            entries + ((index + 1) * self->entry_size),
            entries + (index * self->entry_size),
            impl_bytes(self->entry_size, nmove, xctx));
    }
    self->count++;
    return index;
}



/* Remove the element at index and close the gap. */
AFW_DEFINE(void)
afw_vector_remove_impl(
    const afw_vector_t *internal,
    afw_size_t index,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;
    afw_octet_t *entries;
    afw_size_t nmove;

    self = IMPL_SELF(internal);
    if (index >= self->count) {
        AFW_THROW_ERROR_Z(argument_error,
            "vector remove index is out of range",
            xctx);
    }

    entries = (afw_octet_t *)self->entries;
    nmove = self->count - index - 1;
    if (nmove > 0) {
        memmove(
            entries + (index * self->entry_size),
            entries + ((index + 1) * self->entry_size),
            impl_bytes(self->entry_size, nmove, xctx));
    }
    self->count--;
}



/* Copy used elements into a new vector in p. */
AFW_DEFINE(afw_vector_t *)
afw_vector_copy_impl(
    const afw_vector_t *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_vector_t *copy;
    afw_size_t used_bytes;

    copy = afw_vector_create_impl(internal->entry_size,
        internal->count, p, xctx);
    copy->count = internal->count;
    if (internal->count > 0) {
        used_bytes = impl_bytes(internal->entry_size,
            internal->count, xctx);
        memcpy(copy->entries, internal->entries, used_bytes);
    }
    return copy;
}



/* Copy used entries to an exact-sized block in p. */
AFW_DEFINE(void)
afw_vector_copy_entries_impl(
    const afw_vector_t *internal,
    afw_size_t *count,
    void **ptr,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t used_bytes;

    if (count) {
        *count = internal->count;
    }
    if (ptr) {
        *ptr = NULL;
    }
    if (internal->count == 0 || !ptr) {
        return;
    }

    used_bytes = impl_bytes(internal->entry_size, internal->count,
        xctx);
    *ptr = afw_pool_malloc(p, used_bytes, xctx);
    memcpy(*ptr, internal->entries, used_bytes);
}



/* Free the work vector header and entry chunk. */
AFW_DEFINE(void)
afw_vector_release_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx)
{
    afw_vector_t *self;
    const afw_pool_t *p;
    afw_size_t entries_bytes;

    self = IMPL_SELF(internal);
    p = self->p;
    if (self->entries) {
        entries_bytes = impl_bytes(self->entry_size, self->allocated,
            xctx);
        afw_pool_free_memory(p, self->entries, entries_bytes, xctx);
        self->entries = NULL;
        self->allocated = 0;
        self->count = 0;
    }
    afw_pool_free_memory(p, self, sizeof(afw_vector_t), xctx);
}

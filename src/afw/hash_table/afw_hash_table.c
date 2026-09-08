// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework hash table (key → pointer)
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_hash_table.c
 * @brief Byte-key to pointer map allocated from an AFW pool.
 */

#include "afw_internal.h"


#define IMPL_SELF(internal) ((afw_hash_table_t *)(internal))

#define IMPL_INITIAL_ALLOCATED 16

typedef struct impl_entry_s impl_entry_t;

struct impl_entry_s {
    impl_entry_t *next;
    afw_size_t hash;
    const void *key;
    afw_size_t klen;
    const void *value;
};


static afw_size_t
impl_hash(
    const void *key,
    afw_size_t klen)
{
    const unsigned char *p;
    afw_size_t hash;
    afw_size_t i;

    hash = 0;
    p = (const unsigned char *)key;
    for (i = 0; i < klen; i++) {
        hash = hash * 33 + p[i];
    }
    return hash;
}


static impl_entry_t **
impl_buckets(afw_hash_table_t *self)
{
    return (impl_entry_t **)self->buckets;
}


static impl_entry_t *
impl_find(
    afw_hash_table_t *self,
    const void *key,
    afw_size_t klen,
    afw_size_t hash,
    impl_entry_t ***link)
{
    impl_entry_t **b;
    impl_entry_t *e;

    b = impl_buckets(self) + (hash & (self->allocated - 1));
    if (link) {
        *link = b;
    }
    for (e = *b; e; e = e->next) {
        if (e->hash == hash &&
            e->klen == klen &&
            (klen == 0 || memcmp(e->key, key, klen) == 0))
        {
            return e;
        }
        if (link) {
            *link = &e->next;
        }
    }
    return NULL;
}


static impl_entry_t *
impl_alloc_entry(
    afw_hash_table_t *self,
    afw_xctx_t *xctx)
{
    impl_entry_t *e;

    e = (impl_entry_t *)self->free_entries;
    if (e) {
        self->free_entries = e->next;
        memset(e, 0, sizeof(impl_entry_t));
        return e;
    }
    return afw_pool_calloc_type(self->p, impl_entry_t, xctx);
}


static void
impl_recycle_entry(
    afw_hash_table_t *self,
    impl_entry_t *e)
{
    e->next = (impl_entry_t *)self->free_entries;
    e->key = NULL;
    e->value = NULL;
    e->klen = 0;
    e->hash = 0;
    self->free_entries = e;
}


static void
impl_grow(
    afw_hash_table_t *self,
    afw_xctx_t *xctx)
{
    impl_entry_t **old_buckets;
    impl_entry_t **new_buckets;
    impl_entry_t *e;
    impl_entry_t *next;
    afw_size_t old_allocated;
    afw_size_t new_allocated;
    afw_size_t i;
    afw_size_t mask;

    old_allocated = self->allocated;
    if (old_allocated > AFW_SIZE_T_MAX / 2) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    new_allocated = old_allocated * 2;
    new_buckets = afw_pool_calloc(self->p,
        new_allocated * sizeof(impl_entry_t *), xctx);

    old_buckets = impl_buckets(self);
    mask = new_allocated - 1;
    for (i = 0; i < old_allocated; i++) {
        for (e = old_buckets[i]; e; e = next) {
            next = e->next;
            e->next = new_buckets[e->hash & mask];
            new_buckets[e->hash & mask] = e;
        }
    }

    afw_pool_free_memory(self->p, old_buckets,
        old_allocated * sizeof(impl_entry_t *), xctx);

    self->buckets = new_buckets;
    self->allocated = new_allocated;
}


static void
impl_index_prefetch(
    afw_hash_table_index_t *index)
{
    afw_hash_table_t *self;
    impl_entry_t **buckets;
    impl_entry_t *e;
    afw_size_t i;

    e = (impl_entry_t *)index->entry;
    if (!e) {
        index->next = NULL;
        index->next_bucket = 0;
        return;
    }
    if (e->next) {
        index->next = e->next;
        index->next_bucket = index->bucket;
        return;
    }
    self = IMPL_SELF(index->ht);
    buckets = impl_buckets(self);
    for (i = index->bucket + 1; i < self->allocated; i++) {
        if (buckets[i]) {
            index->next = buckets[i];
            index->next_bucket = i;
            return;
        }
    }
    index->next = NULL;
    index->next_bucket = self->allocated;
}


static void
impl_index_from_bucket(
    afw_hash_table_index_t *index,
    afw_size_t start_bucket)
{
    afw_hash_table_t *self;
    impl_entry_t **buckets;
    afw_size_t i;

    self = IMPL_SELF(index->ht);
    buckets = impl_buckets(self);
    for (i = start_bucket; i < self->allocated; i++) {
        if (buckets[i]) {
            index->bucket = i;
            index->entry = buckets[i];
            impl_index_prefetch(index);
            return;
        }
    }
    index->bucket = self->allocated;
    index->entry = NULL;
    index->next = NULL;
    index->next_bucket = self->allocated;
}



/* Create an untyped hash table. */
AFW_DEFINE(afw_hash_table_t *)
afw_hash_table_create_impl(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_hash_table_t *self;

    self = afw_pool_calloc_type(p, afw_hash_table_t, xctx);
    self->p = p;
    self->count = 0;
    self->allocated = IMPL_INITIAL_ALLOCATED;
    self->buckets = afw_pool_calloc(p,
        IMPL_INITIAL_ALLOCATED * sizeof(impl_entry_t *), xctx);
    self->free_entries = NULL;

    return self;
}



/* Get the value for key, or NULL. */
AFW_DEFINE(void *)
afw_hash_table_get_impl(
    const afw_hash_table_t *internal,
    const void *key,
    afw_size_t klen)
{
    afw_hash_table_t *self;
    impl_entry_t *e;

    if (!internal || !internal->buckets || internal->count == 0) {
        return NULL;
    }
    self = IMPL_SELF(internal);
    e = impl_find(self, key, klen, impl_hash(key, klen), NULL);
    return e ? (void *)e->value : NULL;
}



/* Set or delete the value for key. */
AFW_DEFINE(void)
afw_hash_table_set_impl(
    const afw_hash_table_t *internal,
    const void *key,
    afw_size_t klen,
    const void *value,
    afw_xctx_t *xctx)
{
    afw_hash_table_t *self;
    impl_entry_t *e;
    impl_entry_t **slot;
    afw_size_t hash;

    self = IMPL_SELF(internal);
    hash = impl_hash(key, klen);
    e = impl_find(self, key, klen, hash, &slot);

    if (!value) {
        if (e) {
            *slot = e->next;
            impl_recycle_entry(self, e);
            self->count--;
        }
        return;
    }

    if (e) {
        e->value = value;
        return;
    }

    if (self->count >= (self->allocated / 4) * 3) {
        impl_grow(self, xctx);
    }

    slot = impl_buckets(self) +
        (hash & (self->allocated - 1));
    e = impl_alloc_entry(self, xctx);
    e->hash = hash;
    e->key = key;
    e->klen = klen;
    e->value = value;
    e->next = *slot;
    *slot = e;
    self->count++;
}



/* Position index at the first entry (or end). */
AFW_DEFINE(void)
afw_hash_table_first_impl(
    const afw_hash_table_t *internal,
    afw_hash_table_index_t *index)
{
    index->ht = internal;
    index->bucket = 0;
    index->entry = NULL;
    index->next = NULL;
    index->next_bucket = 0;
    if (!internal || !internal->buckets || internal->count == 0) {
        return;
    }
    impl_index_from_bucket(index, 0);
}



/* Advance index to the next entry (or end). */
AFW_DEFINE(void)
afw_hash_table_next_impl(
    afw_hash_table_index_t *index)
{
    index->entry = index->next;
    index->bucket = index->next_bucket;
    impl_index_prefetch(index);
}



/* Fill key/klen/value from the current index entry. */
AFW_DEFINE(afw_boolean_t)
afw_hash_table_this_impl(
    const afw_hash_table_index_t *index,
    const void **key,
    afw_size_t *klen,
    void **value)
{
    impl_entry_t *e;

    e = (impl_entry_t *)index->entry;
    if (!e) {
        return false;
    }
    if (key) {
        *key = e->key;
    }
    if (klen) {
        *klen = e->klen;
    }
    if (value) {
        *value = (void *)e->value;
    }
    return true;
}



/* Recycle all entries. Capacity is kept. */
AFW_DEFINE(void)
afw_hash_table_clear_impl(
    const afw_hash_table_t *internal,
    afw_xctx_t *xctx)
{
    afw_hash_table_t *self;
    impl_entry_t **buckets;
    impl_entry_t *e;
    impl_entry_t *next;
    afw_size_t i;

    (void)xctx;
    self = IMPL_SELF(internal);
    buckets = impl_buckets(self);
    if (!buckets) {
        return;
    }
    for (i = 0; i < self->allocated; i++) {
        for (e = buckets[i]; e; e = next) {
            next = e->next;
            impl_recycle_entry(self, e);
        }
        buckets[i] = NULL;
    }
    self->count = 0;
}



static void
impl_free_entry_list(
    const afw_pool_t *p,
    impl_entry_t *e,
    afw_xctx_t *xctx)
{
    impl_entry_t *next;

    for (; e; e = next) {
        next = e->next;
        afw_pool_free_memory(p, e, sizeof(impl_entry_t),
            xctx);
    }
}



/* Free the work header, buckets, and entries. */
AFW_DEFINE(void)
afw_hash_table_release_impl(
    const afw_hash_table_t *internal,
    afw_xctx_t *xctx)
{
    afw_hash_table_t *self;
    const afw_pool_t *p;
    impl_entry_t **buckets;
    afw_size_t i;

    self = IMPL_SELF(internal);
    p = self->p;
    buckets = impl_buckets(self);
    if (buckets) {
        for (i = 0; i < self->allocated; i++) {
            impl_free_entry_list(p, buckets[i], xctx);
        }
        afw_pool_free_memory(p, buckets,
            self->allocated * sizeof(impl_entry_t *), xctx);
        self->buckets = NULL;
        self->allocated = 0;
        self->count = 0;
    }
    impl_free_entry_list(p, (impl_entry_t *)self->free_entries,
        xctx);
    self->free_entries = NULL;
    afw_pool_free_memory(p, self, sizeof(afw_hash_table_t),
        xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Shared pool lifetime (heap and tracker).
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool.c
 * @brief Shared pool lifetime: parent/child, RC, cleanups, accounting.
 *
 * Heap store is `afw_pool_heap.c`. Tracker store is
 * `afw_pool_tracker.c`. A pool is a heap unless it is a tracker.
 *
 * Glossary — two layers; do not mix:
 *
 *   afw_memory_region — thread-owned. Not a pool. get/free whole
 *   aligned allocations.
 *   Heap store — a living pool carving USER out of chunks it holds.
 *
 *   pool — a heap or a tracker.
 *   heap — owns chunks; malloc/free_memory happen here.
 *   tracker — gets bytes from an ancestor heap and remembers them.
 *   free_memory only marks.
 *   scope — heap used for `{ }`. Smaller chunk_min (4k), plus
 *   last-release delay on throw.
 *
 *   chunk — `afw_pool_chunk_t`. One posix_memalign allocation the
 *   heap holds. Linked from first_chunk. Typical sizes: 4k (scope)
 *   or 64k (`xctx->p`).
 *   region — the `void *` from `afw_memory_region_get()`. The heap
 *   uses that pointer as a chunk. Same bytes, two names: region at
 *   the thread, chunk once the heap owns it.
 *   page — hardware MMU page, or the 4096 posix_memalign alignment
 *   (`AFW_POOL_CHUNK_ALIGN`). Not a type name. Not a chunk or a
 *   region.
 *
 *   USER — pointer malloc returns. size on malloc/free_memory is
 *   always USER size.
 *   block — one malloc/free_memory unit inside a chunk (USER plus
 *   any prefix). Overlay when freed: `afw_pool_free_node_t`. Not an
 *   Adaptive Script block (`afw_value_block`).
 *   prefix — bytes before USER. Heap: [chunk*][USER]. Debug heap:
 *   [chunk*…][size][pool][USER]. Tracker: [next][size][USER].
 *   free node — `afw_pool_free_node_t` written on a freed block.
 *
 *   free list — two lists. (1) Heap `free_memory_head`: freed
 *   blocks inside live chunks. (2) memory_region: whole
 *   regions/chunks after the heap is destroyed.
 *   bump / remaining — unused tail of current_chunk. Not a block
 *   until malloc takes it or it is put on the heap free list.
 *   current_chunk — chunk the bump is carving.
 *   first_chunk — this heap's chunk list. Extra chunks are
 *   prepended.
 *   chunk_min — smallest chunk this heap will get. 0 = env default
 *   (64k). Scope uses compile/4k.
 *   asked-for vs chunk_bytes — sum of malloc sizes vs bytes in
 *   chunks still held.
 *   containing — `chunk *` on the block (which chunk this block is
 *   in). Not a walk of first_chunk.
 *   coalesce — merge two adjacent free blocks in the same chunk.
 *   first-fit — walk the heap free list; take the first block big
 *   enough.
 *   LIFO — push at the list head. memory_region free() does this
 *   for regions. The heap free list is also LIFO.
 *
 *   parent / child — pool lifetime only. Store is the ancestor
 *   heap.
 *   managed_p — dest pool for managed values. Not a store kind.
 *
 * Script vs pool (same English word):
 *   block — pool: malloc unit in a chunk. Script: `afw_value_block`
 *   (`{ }` frame).
 *   scope — pool: the pool for `{ }`. Script: running frame /
 *   qualifier stack.
 */

#include "afw_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define AFW_POOL_SELF_T afw_pool_internal_self_t


void
afw_pool_internal_env_add_bytes(afw_environment_t *env, afw_size_t n)
{
    env->pool_bytes_in_use += n;
    if (env->pool_bytes_in_use > env->peak_pool_bytes_in_use) {
        env->peak_pool_bytes_in_use = env->pool_bytes_in_use;
    }
}


void
afw_pool_internal_env_add_chunks(afw_environment_t *env, afw_size_t n)
{
    env->pool_chunk_bytes += n;
    if (env->pool_chunk_bytes > env->peak_pool_chunk_bytes) {
        env->peak_pool_chunk_bytes = env->pool_chunk_bytes;
    }
}


void
afw_pool_internal_thread_add_bytes(const afw_thread_t *thread, afw_size_t n)
{
    afw_thread_t *t;

    if (!thread || n == 0) {
        return;
    }
    t = (afw_thread_t *)thread;
    t->pool_bytes_in_use += n;
    if (t->pool_bytes_in_use > t->peak_pool_bytes_in_use) {
        t->peak_pool_bytes_in_use = t->pool_bytes_in_use;
    }
}


void
afw_pool_internal_thread_sub_bytes(const afw_thread_t *thread, afw_size_t n)
{
    if (!thread || n == 0) {
        return;
    }
    ((afw_thread_t *)thread)->pool_bytes_in_use -= n;
}


void
afw_pool_internal_thread_add_chunks(const afw_thread_t *thread, afw_size_t n)
{
    afw_thread_t *t;

    if (!thread || n == 0) {
        return;
    }
    t = (afw_thread_t *)thread;
    t->pool_chunk_bytes += n;
    if (t->pool_chunk_bytes > t->peak_pool_chunk_bytes) {
        t->peak_pool_chunk_bytes = t->pool_chunk_bytes;
    }
}


void
afw_pool_internal_thread_sub_chunks(const afw_thread_t *thread, afw_size_t n)
{
    if (!thread || n == 0) {
        return;
    }
    ((afw_thread_t *)thread)->pool_chunk_bytes -= n;
}


afw_boolean_t
afw_pool_internal_is_multithreaded(const afw_pool_t *p)
{
    const afw_pool_internal_inf_implementation_specific_t *spec;

    if (!p || !p->inf) {
        return false;
    }
    spec = p->inf->rti.implementation_specific;
    return spec && spec->is_multithreaded;
}


afw_boolean_t
afw_pool_internal_counts_on_thread(const afw_pool_internal_self_t *self)
{
    if (!self->thread) {
        return false;
    }
    return !afw_pool_internal_is_multithreaded(&self->pub);
}

void
afw_pool_internal_assign_pool_number(afw_pool_internal_self_t *self)
{
    afw_thread_t *thread;

    if (!self->thread) {
        return;
    }
    thread = (afw_thread_t *)self->thread;
    self->pool_number = ++thread->pool_number;
}


void
afw_pool_internal_account_alloc(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated += consumed;
    if (xctx && xctx->env) {
        afw_pool_internal_env_add_bytes((afw_environment_t *)xctx->env, consumed);
    }
    if (afw_pool_internal_counts_on_thread(self)) {
        afw_pool_internal_thread_add_bytes(self->thread, consumed);
    }
}

void
afw_pool_internal_account_free(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated -= consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -= consumed;
    }
    if (afw_pool_internal_counts_on_thread(self)) {
        afw_pool_internal_thread_sub_bytes(self->thread, consumed);
    }
}


void
afw_pool_internal_account_destroy(afw_pool_internal_self_t *self, afw_xctx_t *xctx)
{
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -=
            self->bytes_allocated;
    }
    if (afw_pool_internal_counts_on_thread(self)) {
        afw_pool_internal_thread_sub_bytes(self->thread, self->bytes_allocated);
    }
    self->bytes_allocated = 0;
}

static void
impl_add_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_get_reference(&parent->pub, xctx);

    child->parent = parent;
    child->next_sibling = parent->first_child;
    parent->first_child = child;
}


static void
impl_unlink_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *prev;
    afw_pool_internal_self_t *sibling;

    (void)xctx;
    for (prev = NULL, sibling = parent->first_child;
        sibling;
        prev = sibling, sibling = sibling->next_sibling)
    {
        if (sibling == child) {
            if (!prev) {
                parent->first_child = sibling->next_sibling;
            }
            else {
                prev->next_sibling = sibling->next_sibling;
            }
            child->next_sibling = NULL;
            return;
        }
    }
}

void
afw_pool_internal_link_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    if (afw_pool_internal_is_multithreaded(&parent->pub)) {
        IMPL_MULTITHREADED_LOCK_BEGIN(parent) {
            impl_add_child(parent, child, xctx);
        }
        IMPL_MULTITHREADED_LOCK_END;
    }
    else {
        impl_add_child(parent, child, xctx);
    }
}

#ifdef AFW_DEBUG_POOL
void
afw_pool_internal_debug_prefix_set(
    AFW_POOL_SELF_T *self,
    void *user,
    afw_size_t size)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)user -
        sizeof(afw_pool_debug_prefix_t));
    pre->size = size;
    pre->pool = &self->pub;
}

afw_boolean_t
afw_pool_internal_debug_prefix_ok(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)address -
        sizeof(afw_pool_debug_prefix_t));
    return pre->pool == &self->pub && pre->size == size;
}

void
afw_pool_internal_debug_check_prefix(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)address -
        sizeof(afw_pool_debug_prefix_t));
    if (pre->pool != &self->pub) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: pool does not match allocation",
            xctx);
    }
    if (pre->size != size) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: size does not match allocation",
            xctx);
    }
}

void
afw_pool_internal_debug_poison_user(void *user, afw_size_t size)
{
    afw_size_t i;
    afw_size_t n;
    afw_size_t *w;
    unsigned char *b;
    unsigned char *end;
    afw_size_t poison;

    if (!user || size == 0) {
        return;
    }
    poison = AFW_POOL_DEBUG_POISON;
    w = (afw_size_t *)user;
    n = size / sizeof(afw_size_t);
    for (i = 0; i < n; i++) {
        w[i] = poison;
    }
    b = (unsigned char *)user + n * sizeof(afw_size_t);
    end = (unsigned char *)user + size;
    i = 0;
    while (b < end) {
        *b++ = (unsigned char)(poison >>
            (8 * (i % sizeof(afw_size_t))));
        i++;
    }
}
#endif

/*
 * Run cleanup callbacks only. Storage stays so sibling callbacks can
 * still value_release tracker-allocated managed headers.
 */
void
afw_pool_internal_run_cleanups(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    /*
     * Detach the list first. A callback may last-release this pool
     * (closure drops its scope); that must not walk the same list.
     */
    e = self->first_cleanup;
    self->first_cleanup = NULL;
    for (; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }
}


void
afw_pool_internal_unlink_from_parent(
    AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;

    parent = self->parent;
    if (!parent) {
        return;
    }
    if (afw_pool_internal_is_multithreaded(&parent->pub)) {
        IMPL_MULTITHREADED_LOCK_BEGIN(parent) {
            impl_unlink_child(parent, self, xctx);
        }
        IMPL_MULTITHREADED_LOCK_END;
    }
    else {
        impl_unlink_child(parent, self, xctx);
    }
}

void
afw_pool_internal_mark_destroying(AFW_POOL_SELF_T *self)
{
    afw_pool_internal_self_t *child;

    self->destroying = true;
    for (child = self->first_child; child; child = child->next_sibling) {
        afw_pool_internal_mark_destroying(child);
    }
}


void
afw_pool_internal_destroy_children(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    while (self->first_child) {
        afw_pool_internal_self_t *child;

        child = self->first_child;
        afw_pool_destroy(&child->pub, xctx);
        if (self->first_child == child) {
            impl_unlink_child(self, child, xctx);
        }
    }
}


void
afw_pool_internal_run_child_cleanups(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *child;
    afw_pool_internal_self_t *next;

    child = self->first_child;
    while (child) {
        next = child->next_sibling;
        afw_pool_run_cleanups(&child->pub, xctx);
        child = next;
    }
}


const afw_pool_t *
afw_pool_internal_release_common(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx,
    void (*teardown)(AFW_POOL_SELF_T *self, afw_xctx_t *xctx))
{
    if (--(self->reference_count) == 0) {
        if (self->destroying) {
            afw_pool_internal_run_cleanups(self, xctx);
            return NULL;
        }
        if (self->first_child) {
            AFW_THROW_ERROR_Z(general,
                "Pool last-release with children remaining", xctx);
        }
        afw_pool_internal_run_cleanups(self, xctx);
        teardown(self, xctx);
        return NULL;
    }
    return &self->pub;
}


/*
 * Implementation of method get_reference for interface afw_pool.
 */
void
afw_pool_internal_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "get_reference");

    self->reference_count++;
}

/*
 * Implementation of method register_cleanup for interface afw_pool.
 */
void
afw_pool_internal_register_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "register_cleanup %p %p",
        data, cleanup);

    /* Allocate entry which will also make sure its ok to use pool. */
    e = afw_pool_calloc_type(&self->pub, afw_pool_cleanup_t, xctx);

    /* Add entry to front of list of cleanup functions. */
    e->data = data;
    e->data2 = data2;
    e->cleanup = cleanup;
    e->next_cleanup = self->first_cleanup;
    self->first_cleanup = e;
}

/*
 * Implementation of method deregister_cleanup for interface afw_pool.
 */
void
afw_pool_internal_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e, *prev;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "deregister_cleanup %p %p",
        data, cleanup);

    /* Search for entry and remove. */
    for (prev = (afw_pool_cleanup_t *)& self->first_cleanup,
        e = self->first_cleanup;
        e; prev = e, e = e->next_cleanup)
    {
        if (e->data == data && e->data2 == data2 && e->cleanup == cleanup) {
            prev->next_cleanup = e->next_cleanup;
            afw_pool_free_memory(&self->pub, e,
                sizeof(afw_pool_cleanup_t), xctx);
            break;
        }
    }
}


afw_boolean_t
afw_pool_internal_is_tracker(const afw_pool_t *p)
{
    const afw_pool_internal_inf_implementation_specific_t *spec;

    if (!p || !p->inf) {
        return false;
    }
    spec = p->inf->rti.implementation_specific;
    return spec && spec->is_tracker;
}


afw_boolean_t
afw_pool_internal_is_heap(const afw_pool_t *p)
{
    return p && !afw_pool_internal_is_tracker(p);
}


void *
afw_pool_internal_malloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    if (!instance) {
        return NULL;
    }
    return afw_pool_malloc_no_throw(instance, size, xctx);
}


void *
afw_pool_internal_calloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    if (!instance) {
        return NULL;
    }
    return afw_pool_calloc_no_throw(instance, size, xctx);
}

void
afw_pool_internal_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const AFW_POOL_SELF_T *self = (const AFW_POOL_SELF_T *)pool;
    const afw_pool_internal_self_t *child;
    const afw_pool_internal_heap_self_t *heap;
    int i;
    afw_size_t chunk_count;
    afw_size_t chunk_bytes;

    (void)xctx;
    chunk_count = 0;
    chunk_bytes = 0;
    if (afw_pool_internal_is_heap(pool)) {
        heap = afw_pool_heap_internal_as_heap(self);
        chunk_count = heap->chunk_count;
        chunk_bytes = heap->chunk_bytes;
    }
    for (i = 0; i < indent; i++) {
        printf("  ");
    }
    printf(
        "thread " AFW_INTEGER_FMT " pool " AFW_INTEGER_FMT
        " in_use " AFW_SIZE_T_FMT
        " chunks " AFW_SIZE_T_FMT " " AFW_SIZE_T_FMT
        " refs " AFW_INTEGER_FMT
        " parent " AFW_INTEGER_FMT "\n",
        self->thread ? self->thread->thread_number : (afw_integer_t)0,
        self->pool_number,
        self->bytes_allocated,
        chunk_count,
        chunk_bytes,
        self->reference_count,
        self->parent ? self->parent->pool_number : (afw_integer_t)0);

    for (child = self->first_child; child; child = child->next_sibling) {
        afw_pool_internal_print_debug_info(indent + 2, &child->pub, xctx);
    }
}

AFW_DEFINE(afw_size_t)
afw_pool_bytes_allocated(const afw_pool_t *instance)
{
    if (!instance) {
        return 0;
    }
    return ((const afw_pool_internal_self_t *)instance)->bytes_allocated;
}

static afw_size_t
impl_subtree_bytes_allocated(const afw_pool_internal_self_t *self)
{
    const afw_pool_internal_self_t *child;
    afw_size_t n;

    n = self->bytes_allocated;
    for (child = self->first_child; child; child = child->next_sibling) {
        n += impl_subtree_bytes_allocated(child);
    }
    return n;
}


static afw_size_t
impl_subtree_chunk_bytes(const afw_pool_internal_self_t *self)
{
    const afw_pool_internal_self_t *child;
    afw_size_t n;

    n = 0;
    if (afw_pool_internal_is_heap(&self->pub)) {
        n = afw_pool_heap_internal_as_heap(self)->chunk_bytes;
    }
    for (child = self->first_child; child; child = child->next_sibling) {
        n += impl_subtree_chunk_bytes(child);
    }
    return n;
}


AFW_DEFINE(afw_size_t)
afw_pool_subtree_bytes_allocated(const afw_pool_t *instance)
{
    if (!instance) {
        return 0;
    }
    return impl_subtree_bytes_allocated(
        (const afw_pool_internal_self_t *)instance);
}


AFW_DEFINE(afw_size_t)
afw_pool_subtree_chunk_bytes(const afw_pool_t *instance)
{
    if (!instance) {
        return 0;
    }
    return impl_subtree_chunk_bytes(
        (const afw_pool_internal_self_t *)instance);
}


/* Release the value registered with afw_pool_release_value_at_cleanup(). */
static void
impl_release_value_at_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    afw_value_release((const afw_value_t *)data, xctx);
}


AFW_DEFINE(afw_boolean_t)
afw_pool_is_value_release_registered(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_cleanup_t *e;

    (void)xctx;
    if (!value || !p) {
        return false;
    }
    self = (afw_pool_internal_self_t *)p;
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        if (e->cleanup == impl_release_value_at_cleanup &&
            e->data == (void *)value &&
            e->data2 == NULL)
        {
            return true;
        }
    }
    return false;
}


/* Release a value when a pool is destroyed. */
AFW_DEFINE(void)
afw_pool_release_value_at_cleanup(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value) {
        return;
    }
    /* Permanents / compile literals: nothing to release. */
    if (!value->inf || !value->inf->optional_release) {
        return;
    }
    if (afw_pool_is_value_release_registered(value, p, xctx)) {
        return;
    }
    afw_pool_register_cleanup(p, (void *)value, NULL,
        impl_release_value_at_cleanup, xctx);
}

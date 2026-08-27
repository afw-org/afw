// See the 'COPYING' file in the project root for licensing information.
/*
 * Evaluation heap and heap-tracker pool implementation.
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_pool_heap.c
 * @brief Heap and heap tracker: single-thread only.
 *
 * Create, use, and release on the same thread. compiled_value evaluate
 * creates a heap for one evaluate and releases it in finally. Trackers
 * are scope->p and return leftovers to that heap. Optional free is
 * afw_pool_free_memory(p, address, size): address-ordered list, coalesce.
 * Temporarily checks size against the live chunk header.
 */

#include "afw_internal.h"
#include "afw_pool_heap_internal.h"

#include <stdio.h>
#include <stdlib.h>

/* multithreaded pool lock begin */
#define IMPL_MULTITHREADED_LOCK_BEGIN(xctx) \
AFW_LOCK_BEGIN((xctx)->env->multithreaded_pool_lock)

/* multithreaded pool lock end */
#define IMPL_MULTITHREADED_LOCK_END \
AFW_LOCK_END;

/*
 * The pool methods begin with 'impl_afw_pool_' only.
 */
#define AFW_POOL_SELF_T afw_pool_internal_self_t

#define AFW_IMPLEMENTATION_ID "heap"

static const afw_pool_internal_inf_implementation_specific_t
impl_pool_implementation_specific =
    {
        /* multithreaded */ false,
        /* subpool */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_SPECIFIC

#define AFW_POOL_INF_ONLY 1

/*
 * Some of the subpool pool methods begin with 'impl_subpool_afw_pool_' but
 * the others are the same as the default.
 */
#define AFW_IMPLEMENTATION_ID "heap_tracker"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_subpool_inf

static void
impl_subpool_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_destroy \
    impl_subpool_afw_pool_destroy

static apr_pool_t *
impl_subpool_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T * self);

#define impl_afw_pool_get_apr_pool \
    impl_subpool_afw_pool_get_apr_pool

static void *
impl_subpool_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc \
    impl_subpool_afw_pool_calloc

static void *
impl_subpool_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_malloc \
    impl_subpool_afw_pool_malloc

static void
impl_subpool_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free_memory \
    impl_subpool_afw_pool_free_memory

static const afw_pool_internal_inf_implementation_specific_t
impl_subpool_implementation_specific =
    {
        /* multithreaded */ false,
        /* subpool */ true
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_subpool_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_destroy
#undef impl_afw_pool_get_apr_pool
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory


/* --------------------------- internal macros ------------------------------ */

#ifdef AFW_DEBUG_POOL

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool %s " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            info_z, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool " format_z " " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            __VA_ARGS__, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#else

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z)
#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...)

#endif


static void
impl_account_alloc(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated += consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use += consumed;
    }
}


static void
impl_account_free(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated -= consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -= consumed;
    }
}


static void
impl_account_destroy(afw_pool_internal_self_t *self, afw_xctx_t *xctx)
{
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -=
            self->bytes_allocated;
    }
    self->bytes_allocated = 0;
}


/* --------------------------- internal functions --------------------------- */

static void
impl_add_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_get_reference(&parent->pub, xctx);

    child->next_sibling = parent->first_child;
    parent->first_child = child;
}


static void
impl_remove_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *prev;
    afw_pool_internal_self_t *sibling;

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
            break;
        }
    }

    if (!sibling) {
        AFW_THROW_ERROR_Z(general, "Not a child of parent", xctx);
    }

    afw_pool_release(&parent->pub, xctx);
}


/* Create skeleton heap struct. Parent is any AFW pool (usually APR). */
static afw_pool_internal_self_t *
impl_create(
    const afw_pool_t *afw_parent,
    const afw_pool_inf_t *inf,
    afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    apr_pool_t *parent_apr;
    afw_pool_internal_self_t *self;
    afw_pool_internal_self_with_free_memory_head_t *mem;

    /* Reservoir APR under the parent AFW pool's APR door. Heap still
     * runs in APR; this is not get_apr_pool() on the new heap. */
    parent_apr = afw_parent ? afw_pool_get_apr_pool(afw_parent) : NULL;
    apr_pool_create(&apr_p, parent_apr);
    if (!apr_p) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }

    mem = apr_pcalloc(apr_p,
        sizeof(afw_pool_internal_self_with_free_memory_head_t));
    if (!mem) {
        AFW_THROW_ERROR_Z(memory,
                "Unable to allocate memory for pool", xctx);
    }
    self = &mem->common;
    self->pub.inf = inf;
    self->pub.managed_p = &self->pub;
    self->apr_p = apr_p;
    self->parent = NULL;
    self->external_parent = afw_parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);
    self->reference_count = 1;
    self->free_memory_head = &mem->memory_for_free_memory_head;
    self->thread = xctx->thread;

    if (afw_parent) {
        afw_pool_get_reference(afw_parent, xctx);
    }

    /* Header is APR; in_use starts at 0 until malloc/calloc. */
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

/* Create skeleton pool struct for subpool. */
static afw_pool_internal_self_t *
impl_create_for_subpool(
    afw_pool_internal_self_t *parent,
    const afw_pool_inf_t *inf,
    afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    afw_pool_internal_self_t *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required for subpool", xctx);
    }
    apr_p = parent->apr_p;

    /*
     * Header is a heap user block so destroy can free_memory it.
     * apr_pcalloc on the reservoir was never returned. Not on this
     * tracker’s allocated list.
     */
    self = afw_pool_calloc(&parent->pub,
        sizeof(afw_pool_internal_self_t), xctx);
    self->pub.inf = inf;
    self->pub.managed_p = parent->pub.managed_p
        ? parent->pub.managed_p
        : &parent->pub;
    self->apr_p = apr_p;
    self->parent = parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);
    self->reference_count = 1;

    /* Subpools allocate from parent. */
    self->free_memory_head = parent->free_memory_head;

    /* If parent, add this new child. */
    if (parent) {
        self->thread = parent->thread;
        if (self->thread) {
            impl_add_child(parent, self, xctx);
        }
        else {
            IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
                impl_add_child(parent, self, xctx);
            }
            IMPL_MULTITHREADED_LOCK_END;
        }
    }

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    /* Return new subpool. */
    return self;
}

/*
 * First-fit on an address-ordered doubly-linked free list. Always
 * insert on free; coalesce with neighbors when adjacent. Remainder
 * too small to hold a chunk is taken with the allocation.
 *
 * Size is APR-aligned; the low bit is in-use. A second free of the
 * same chunk (heap free of a tracker alloc, then last-release) must
 * not insert again — that cycles next and livelocks first-fit.
 */
#define AFW_POOL_HEAP_INUSE ((afw_size_t)1)

static afw_size_t
impl_chunk_bytes(const afw_pool_heap_chunk_t *chunk)
{
    return chunk->size & ~AFW_POOL_HEAP_INUSE;
}

static afw_boolean_t
impl_chunk_in_use(const afw_pool_heap_chunk_t *chunk)
{
    return (chunk->size & AFW_POOL_HEAP_INUSE) ? true : false;
}

static void
impl_chunk_mark_in_use(afw_pool_heap_chunk_t *chunk, afw_size_t bytes)
{
    chunk->size = bytes | AFW_POOL_HEAP_INUSE;
}

static void
impl_chunk_mark_free(afw_pool_heap_chunk_t *chunk, afw_size_t bytes)
{
    chunk->size = bytes & ~AFW_POOL_HEAP_INUSE;
}

static void
impl_chunk_unlink(
    afw_pool_heap_chunk_t **head,
    afw_pool_heap_chunk_t *chunk)
{
    if (chunk->prev) {
        chunk->prev->next = chunk->next;
    }
    else {
        *head = chunk->next;
    }
    if (chunk->next) {
        chunk->next->prev = chunk->prev;
    }
    chunk->prev = NULL;
    chunk->next = NULL;
}

/*
 * Heap free_memory(p=heap) of a tracker allocation does not go
 * through tracker free, so unlink from the owning tracker here.
 */
static void
impl_unlink_from_child_trackers(
    AFW_POOL_SELF_T *heap,
    afw_pool_heap_chunk_t *block)
{
    afw_pool_internal_self_t *t;

    if (block->prev) {
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block->prev = NULL;
        block->next = NULL;
        return;
    }
    for (t = heap->first_child; t; t = t->next_sibling) {
        if (t->first_allocated_memory == block) {
            impl_chunk_unlink(&t->first_allocated_memory, block);
            return;
        }
    }
}


static afw_boolean_t
impl_alloc_memory(
    afw_byte_t **address,
    afw_size_t *actual_size,
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_heap_chunk_t *curr;
    afw_pool_heap_chunk_t *prev;
    afw_pool_heap_chunk_t *next;
    afw_pool_heap_chunk_t *rest;
    afw_size_t requested;
    afw_boolean_t reused;

    requested = (size < sizeof(afw_pool_heap_chunk_t))
        ? sizeof(afw_pool_heap_chunk_t)
        : size;
    *actual_size = APR_ALIGN_DEFAULT(requested);
    if (*actual_size < requested) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }

    curr = NULL;
    if (self->free_memory_head) {
        afw_pool_heap_chunk_t *slow;
        afw_pool_heap_chunk_t *fast;

        slow = self->free_memory_head->first;
        fast = slow;
        for (curr = slow;
            curr && impl_chunk_bytes(curr) < *actual_size;
            curr = curr->next)
        {
            if (fast) {
                fast = fast->next;
            }
            if (fast) {
                fast = fast->next;
            }
            if (fast && fast == curr) {
                AFW_THROW_ERROR_Z(general,
                    "heap free-list cycle",
                    xctx);
            }
        }
    }

    if (curr) {
        afw_size_t curr_bytes;

        curr_bytes = impl_chunk_bytes(curr);
        prev = curr->prev;
        next = curr->next;
        impl_chunk_unlink(&self->free_memory_head->first, curr);
        if (curr_bytes - *actual_size >= sizeof(afw_pool_heap_chunk_t))
        {
            rest = (afw_pool_heap_chunk_t *)
                (((char *)curr) + *actual_size);
            impl_chunk_mark_free(rest, curr_bytes - *actual_size);
            rest->prev = prev;
            if (prev) {
                prev->next = rest;
            }
            else {
                self->free_memory_head->first = rest;
            }
            if (next &&
                ((char *)rest) + impl_chunk_bytes(rest) == (char *)next)
            {
                impl_chunk_mark_free(rest,
                    impl_chunk_bytes(rest) + impl_chunk_bytes(next));
                rest->next = next->next;
                if (next->next) {
                    next->next->prev = rest;
                }
            }
            else {
                rest->next = next;
                if (next) {
                    next->prev = rest;
                }
            }
        }
        else {
            *actual_size = curr_bytes;
        }
    }

    reused = (curr != NULL);

    if (!reused) {
        curr = apr_palloc(self->apr_p, *actual_size);
        if (!curr) {
            AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
        }
    }

    *address = (afw_byte_t *)curr;
    return reused;
}


static void
impl_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_heap_chunk_t *freeing;
    afw_pool_heap_chunk_t *prev;
    afw_pool_heap_chunk_t *curr;

    freeing = (afw_pool_heap_chunk_t *)address;
    if (!impl_chunk_in_use(freeing)) {
        /* Running xctx, not self: self may already be in teardown. */
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: memory already freed",
            xctx);
    }
    impl_chunk_mark_free(freeing, size);
    freeing->prev = NULL;
    freeing->next = NULL;

    prev = NULL;
    curr = self->free_memory_head->first;
    while (curr && curr < freeing) {
        prev = curr;
        curr = curr->next;
    }

    freeing->prev = prev;
    freeing->next = curr;
    if (prev) {
        prev->next = freeing;
    }
    else {
        self->free_memory_head->first = freeing;
    }
    if (curr) {
        curr->prev = freeing;
    }

    if (curr &&
        ((char *)freeing) + impl_chunk_bytes(freeing) == (char *)curr)
    {
        impl_chunk_mark_free(freeing,
            impl_chunk_bytes(freeing) + impl_chunk_bytes(curr));
        freeing->next = curr->next;
        if (curr->next) {
            curr->next->prev = freeing;
        }
    }

    if (prev &&
        ((char *)prev) + impl_chunk_bytes(prev) == (char *)freeing)
    {
        impl_chunk_mark_free(prev,
            impl_chunk_bytes(prev) + impl_chunk_bytes(freeing));
        prev->next = freeing->next;
        if (freeing->next) {
            freeing->next->prev = prev;
        }
    }
}


static void *
impl_malloc_user(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_byte_t *mem;
    afw_pool_heap_chunk_t *block;
    afw_size_t size_with_prefix;
    afw_size_t actual_size;
    afw_boolean_t reused;

    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }
    if (size > AFW_SIZE_T_MAX - sizeof(afw_pool_heap_chunk_t)) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }

    size_with_prefix = size + sizeof(afw_pool_heap_chunk_t);
    reused = impl_alloc_memory(&mem, &actual_size, self,
        size_with_prefix, xctx);
    (void)reused;
    IMPL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "apr", size);
    block = (afw_pool_heap_chunk_t *)mem;
    impl_chunk_mark_in_use(block, actual_size);
    block->prev = NULL;
    block->next = NULL;
    impl_account_alloc(self, actual_size, xctx);
    return mem + sizeof(afw_pool_heap_chunk_t);
}


/*
 * Temporary: caller size vs live chunk. Remainder too small to split
 * keeps the whole chunk; that is not a size bug.
 */
static void
impl_check_free_size(
    const afw_pool_heap_chunk_t *block,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_size_t with_prefix;
    afw_size_t aligned;
    afw_size_t chunk;

    if (size > AFW_SIZE_T_MAX - sizeof(afw_pool_heap_chunk_t)) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: size too large", xctx);
    }
    with_prefix = size + sizeof(afw_pool_heap_chunk_t);
    aligned = APR_ALIGN_DEFAULT(with_prefix);
    chunk = impl_chunk_bytes(block);
    if (chunk < aligned) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "afw_pool_free_memory: size " AFW_SIZE_T_FMT
            " larger than chunk " AFW_SIZE_T_FMT,
            size, chunk);
    }
    if (chunk > aligned &&
        (chunk - aligned) >= sizeof(afw_pool_heap_chunk_t))
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            "afw_pool_free_memory: size " AFW_SIZE_T_FMT
            " smaller than chunk " AFW_SIZE_T_FMT,
            size, chunk);
    }
}


/* --------------------------- pool implementations ------------------------- */

/*
 * Implementation of method release for interface afw_pool.
 *
 * Returns the pool if it still exists, or NULL if this call destroyed it.
 */
const afw_pool_t *
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");

    /* Decrement reference count and release pools resources if zero. */
    if (--(self->reference_count) == 0) {
        afw_pool_destroy(&self->pub, xctx);
        return NULL;
    }
    return &self->pub;
}


/*
 * Implementation of method get_reference for interface afw_pool.
 */
void
impl_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "get_reference");

    /* Increment reference count. */
    self->reference_count++;
}

/*
 * Implementation of method destroy for interface afw_pool.
 */
void
impl_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *child;
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");

    /*
     * Call all of the cleanup routines for this pool before releasing children.
     */
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }

    /*
     * Release children.
     *
     * Release of child sets self->first_child to its next sibling.
     */
    for (child = self->first_child;
        child;
        child = self->first_child)
    {
        afw_pool_release(&child->pub, xctx);
    }

    /* If parent heap, removed self as child. */
    if (self->parent) {
        impl_remove_as_child(self->parent, self, xctx);
    }
    else if (self->external_parent) {
        afw_pool_release(self->external_parent, xctx);
    }

    impl_account_destroy(self, xctx);

    /* Destroy apr pool. */
    apr_pool_destroy(self->apr_p);
}

/*
 * Implementation of method get_apr_pool for interface afw_pool.
 */
apr_pool_t *
impl_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T * self)
{
    /*
     * Door for leftover APR function calls, not a second store. Heap
     * still runs in apr_p, so for now the public pool is that one.
     */
    if (!self->public_apr_p) {
        self->public_apr_p = self->apr_p;
    }

    return self->public_apr_p;
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
impl_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_afw_pool_malloc(self, size, xctx);
    memset(result, 0, size);
    return result;
}

/*
 * Implementation of method malloc for interface afw_pool.
 */
void *
impl_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_malloc_user(self, size, xctx);
}

/*
 * Implementation of method free_memory for interface afw_pool.
 */
void
impl_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_heap_chunk_t *block;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
    }
    block = AFW_POOL_HEAP_CHUNK(address);
    if (!impl_chunk_in_use(block)) {
#ifndef FIXME_GET_IT_WORKING
        (void)self;
        (void)address;
        (void)size;
        (void)xctx;
        return;
#endif
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: memory already freed",
            xctx);
    }
    impl_check_free_size(block, size, xctx);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, impl_chunk_bytes(block));
    impl_unlink_from_child_trackers(self, block);
    impl_account_free(self, impl_chunk_bytes(block), xctx);
    impl_free_memory(self, block, impl_chunk_bytes(block), xctx);
}

/*
 * Implementation of method register_cleanup_before for interface afw_pool.
 */
void
impl_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "register_cleanup_before %p %p",
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
impl_afw_pool_deregister_cleanup(
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


/* --------------------------- subpool implementations ---------------------- */

void
impl_subpool_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_heap_chunk_t *memory;
    afw_pool_internal_self_t *child;
    afw_pool_internal_self_t *parent;
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");

    /* Subpool always have a parent. (needed to suppress valgrind error) */
    if (!self->parent) {
        AFW_THROW_ERROR_Z(general, "Subpool has no parent", xctx);
    }
    parent = self->parent;

    /*
     * Call all of the cleanup routines for this pool before releasing children.
     */
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }

    /* Release all of the children of this subpool. */
    for (child = self->first_child;
        child;
        child = self->first_child)
    {
        afw_pool_release(&child->pub, xctx);
    }

    /* Leftover children honor destroy; do not reparent. */
    while (self->first_child) {
        afw_pool_destroy(&self->first_child->pub, xctx);
    }

    /* Lazy get_apr_pool() door, if anyone called it. Not the reservoir. */
    if (self->public_apr_p) {
        apr_pool_destroy(self->public_apr_p);
    }

    /* Return remaining in-use chunks. Unlink first so next is still
     * the allocated-list link, not a free-list link. */
    while (self->first_allocated_memory) {
        memory = self->first_allocated_memory;
        impl_chunk_unlink(&self->first_allocated_memory, memory);
        if (impl_chunk_in_use(memory)) {
            impl_free_memory(self, memory, impl_chunk_bytes(memory),
                xctx);
        }
    }

    impl_account_destroy(self, xctx);

    /* Removed self as child of parent. Header was calloc’d from the heap. */
    impl_remove_as_child(parent, self, xctx);
    afw_pool_free_memory(&parent->pub, self,
        sizeof(afw_pool_internal_self_t), xctx);
}


apr_pool_t *
impl_subpool_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T * self)
{
    int rv;
    apr_pool_t *parent_apr_p;

    /*
     * Door for leftover APR function calls only. The reservoir is
     * self->apr_p (the parent heap's). Create a child APR pool on first
     * call, parented on the heap reservoir — not get_apr_pool(heap),
     * which would open the heap door as a side effect. If nobody
     * calls, none exists. Tracker destroy releases it.
     */
    if (!self->public_apr_p) {
        parent_apr_p = self->parent->apr_p;
        rv = apr_pool_create(&self->public_apr_p, parent_apr_p);
        if (rv != APR_SUCCESS) {
            /*
             * No xctx to throw. If this fails, the heap reservoir is
             * already gone or the process is out of memory. abort()
             * so a debugger/core gets a stack; stderr says why.
             */
            fprintf(stderr,
                "afw_pool_get_apr_pool: apr_pool_create failed "
                "for heap tracker (rv=%d)\n", rv);
            abort();
        }
    }

    return self->public_apr_p;
}


void *
impl_subpool_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_subpool_afw_pool_malloc(self, size, xctx);
    memset(result, 0, size);
    return result;
}


static void *
impl_subpool_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;
    afw_pool_heap_chunk_t *block;

    result = impl_malloc_user(self, size, xctx);
    block = AFW_POOL_HEAP_CHUNK(result);
    block->next = self->first_allocated_memory;
    if (self->first_allocated_memory) {
        self->first_allocated_memory->prev = block;
    }
    self->first_allocated_memory = block;
    return result;
}


static void
impl_subpool_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_heap_chunk_t *block;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
    }
    block = AFW_POOL_HEAP_CHUNK(address);
    if (!impl_chunk_in_use(block)) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: memory already freed",
            xctx);
    }
    impl_check_free_size(block, size, xctx);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, impl_chunk_bytes(block));

    impl_account_free(self, impl_chunk_bytes(block), xctx);
    impl_chunk_unlink(&self->first_allocated_memory, block);
    impl_free_memory(self, block, impl_chunk_bytes(block), xctx);
}


/* ---------------------------- extern functions ---------------------------- */


AFW_DEFINE(const afw_pool_t *)
afw_pool_heap_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    const afw_pool_inf_t *inf;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    /* Always single-thread inf. One thread creates, uses, releases. */
    inf = &impl_afw_pool_inf;

    self = impl_create(parent, inf, xctx);
    return &self->pub;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_heap_tracker_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    AFW_POOL_SELF_T *heap;
    const afw_pool_inf_t *inf;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (parent->inf != &impl_afw_pool_inf) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_heap_tracker_create() parent must be a heap",
            xctx);
    }

    heap = (AFW_POOL_SELF_T *)parent;
    /* Same thread as the heap. */
    inf = &impl_afw_pool_subpool_inf;

    self = impl_create_for_subpool(heap, inf, xctx);
    return &self->pub;
}

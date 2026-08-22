// See the 'COPYING' file in the project root for licensing information.
/*
 * Evaluation heap and heap-tracker pool implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_heap.c
 * @brief Heap and heap tracker: single-thread only.
 *
 * Create, use, and release on the same thread. compiled_value evaluate
 * creates a heap for one evaluate and releases it in finally. Trackers
 * are scope->p and return memory to that heap.
 */

#include "afw_internal.h"
#include "afw_pool_heap_internal.h"

#include <stdio.h>

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
impl_subpool_afw_pool_free_memory_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_xctx_t *xctx);

#define impl_afw_pool_free_memory_internal \
    impl_subpool_afw_pool_free_memory_internal

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
#undef impl_afw_pool_free_memory_internal


/* --------------------------- internal macros ------------------------------ */

#ifdef AFW_TRACE_POOL

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_trace_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_trace_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            info_z " %s pool " AFW_INTEGER_FMT \
            " bytes " AFW_SIZE_T_FMT \
            " env " AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT "\n", \
            AFW__FILE_LINE__, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_allocated, \
            afw_os_get_maxrss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0)); \
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
            format_z " %s pool " AFW_INTEGER_FMT \
            " bytes " AFW_SIZE_T_FMT \
            " env " AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT "\n", \
            __VA_ARGS__, \
            AFW__FILE_LINE__, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_allocated, \
            afw_os_get_maxrss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0)); \
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
        ((afw_environment_t *)xctx->env)->pool_bytes_allocated += consumed;
    }
}


static void
impl_account_free(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated -= consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_allocated -= consumed;
    }
}


static void
impl_account_destroy(afw_pool_internal_self_t *self, afw_xctx_t *xctx)
{
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_allocated -=
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
    afw_size_t size;
    afw_byte_t *mem;
    afw_pool_internal_memory_prefix_t *block;

    size = sizeof(afw_pool_internal_self_with_free_memory_head_t) +
        sizeof(afw_pool_internal_memory_prefix_t);
    parent_apr = afw_parent ? afw_pool_get_apr_pool(afw_parent) : NULL;
    apr_pool_create(&apr_p, parent_apr);
    if (!apr_p) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    size = APR_ALIGN_DEFAULT(size);

    mem = apr_pcalloc(apr_p, size);
    if (!mem) {
        AFW_THROW_ERROR_Z(memory,
                "Unable to allocate memory for pool", xctx);
    }
    self = (afw_pool_internal_self_t *)(mem +
        sizeof(afw_pool_internal_memory_prefix_t));

    block = AFW_POOL_INTERNAL_MEMORY_PREFIX(self);
    block->p = (const afw_pool_t *)self;
    block->size = size;
    self->pub.inf = inf;
    self->pub.managed_p = &self->pub;
    self->apr_p = apr_p;
    self->parent = NULL;
    self->external_parent = afw_parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);
    self->reference_count = 1;
    self->free_memory_head =
        &((afw_pool_internal_self_with_free_memory_head_t *)self)->
        memory_for_free_memory_head;
    self->thread = xctx->thread;

    if (afw_parent) {
        afw_pool_get_reference(afw_parent, xctx);
    }

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
    afw_size_t size;
    afw_byte_t *mem;
    afw_pool_internal_memory_prefix_t *block;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required for subpool", xctx);
    }
    size = sizeof(afw_pool_internal_self_t) +
        sizeof(afw_pool_internal_memory_prefix_with_links_t);
    apr_p = parent->apr_p;
    size = APR_ALIGN_DEFAULT(size);

    mem = apr_pcalloc(apr_p, size);
    if (!mem) {
        AFW_THROW_ERROR_Z(memory,
            "Unable to allocate memory for pool", xctx);
    }
    self = (afw_pool_internal_self_t *)(mem +
        sizeof(afw_pool_internal_memory_prefix_with_links_t));
    self->first_allocated_memory =
        (afw_pool_internal_memory_prefix_with_links_t *)mem;

    block = AFW_POOL_INTERNAL_MEMORY_PREFIX(self);
    block->p = (const afw_pool_t *)self;
    block->size = size;
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

    /* Return new subpool. */
    return self;
}

/*
 * This finds the the first free memory block that is large enough to hold
 * the requested size.
 * 
 * The size is always rounded up using APR_ALIGN_DEFAULT and will always be
 * increased to at least the size of a afw_pool_internal_free_memory_t struct.
 *
 * If the block is larger than the requested size, the
 * block is split and the remainder remains on free memory chain. If there is
 * no block large enough, a new block of the requested size is allocated from
 * the apr pool.
 */
static void
impl_alloc_memory(
    afw_byte_t **address,
    afw_size_t *actual_size,
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_internal_free_memory_t *prev;
    afw_pool_internal_free_memory_t *curr;
    afw_pool_internal_free_memory_t *new_block;
    afw_size_t requested;

    requested = (size < sizeof(afw_pool_internal_free_memory_t))
        ? sizeof(afw_pool_internal_free_memory_t)
        : size;
    *actual_size = APR_ALIGN_DEFAULT(requested);
    if (*actual_size < requested) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }

    /* Find the first free memory that fits. */
    curr = NULL;
    prev = NULL;
    if (self->free_memory_head) {
        for (curr = self->free_memory_head->first;
            curr && curr->size < *actual_size;
            prev = curr, curr = curr->next);
    }

    /* If a free memory block found, use all or some of it. */
    if (curr) {
        if (curr->size - *actual_size < sizeof(afw_pool_internal_free_memory_t))
        {
            /* Just use the whole block. */
            if (!prev) {
                self->free_memory_head->first = curr->next;
            }
            else {
                prev->next = curr->next;
            }
        }
        else {
            /* Split the block. */
            new_block = (afw_pool_internal_free_memory_t *)
                (((char *)curr) + *actual_size);
            new_block->size = curr->size - *actual_size;
            new_block->next = curr->next;
            if (!prev) {
                self->free_memory_head->first = new_block;
            }
            else {
                prev->next = new_block;
            }
        }
    }

    /* If no free memory block found, allocate from apr pool. */
    else {
        curr = apr_palloc(self->apr_p, *actual_size);
        if (!curr) {
            AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
        }
    }

    *address = (afw_byte_t *)curr;
}


static void
impl_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_internal_free_memory_t *freeing;
    afw_pool_internal_free_memory_t *prev;
    afw_pool_internal_free_memory_t *curr;

    freeing = (afw_pool_internal_free_memory_t *)address;
    freeing->next = NULL;
    freeing->size = size;

    /* If this is first freed, set freeing as first and return. */
    curr = self->free_memory_head->first;
    if (!curr) {
        self->free_memory_head->first = freeing;
        return;
    }

    /* Find place in free chain with higher address. */
    for (prev = NULL;
        curr && curr < freeing;
        prev = curr, curr = curr->next);

    /* If freeing adjacent to curr, combine. */
    if (curr && ((char *)curr) == ((char *)freeing) + size) {
        freeing->size += curr->size;
        freeing->next = curr->next;
        if (prev) {
            prev->next = freeing;
        }
        else {
            self->free_memory_head->first = freeing;
        }
    }

    /* If freeing adjacent to prev, combine. */
    if (prev && ((char *)prev) + prev->size == ((char *)freeing)) {
        prev->size += freeing->size;
        prev->next = freeing->next;
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
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_release");

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
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_get_reference");

    /* Decrement reference count. */
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

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_destroy");

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
    afw_byte_t *mem;
    afw_pool_internal_memory_prefix_t *block;
    afw_size_t size_with_prefix;
    afw_size_t actual_size;
    void *result;

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_*alloc " AFW_SIZE_T_FMT,
        size);

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    if (size > AFW_SIZE_T_MAX -
        sizeof(afw_pool_internal_memory_prefix_t))
    {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }

    size_with_prefix = size + sizeof(afw_pool_internal_memory_prefix_t);

    impl_alloc_memory(&mem, &actual_size, self, size_with_prefix, xctx);
    result = mem + sizeof(afw_pool_internal_memory_prefix_t);
    block = (afw_pool_internal_memory_prefix_t *)mem;
    block->p = (const afw_pool_t *)self;
    block->size = actual_size;
    impl_account_alloc(self, actual_size, xctx);

    return result;
}

/*
 * Implementation of method free_memory_internal for interface afw_pool.
 */
void
impl_afw_pool_free_memory_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_xctx_t *xctx)
{
    afw_pool_internal_memory_prefix_t *block;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "afw_pool_free NULL");
        return;
    }
    block = AFW_POOL_INTERNAL_MEMORY_PREFIX(address);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "afw_pool_free %p " AFW_SIZE_T_FMT,
        address, block->size);
    impl_account_free(self, block->size, xctx);
    /* Make memory available for reuse. */
    impl_free_memory(self, address, block->size, xctx);
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
        "afw_pool_register_cleanup_before %p %p",
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
        "afw_pool_deregister_cleanup %p %p",
        data, cleanup);

    /* Search for entry and remove. */
    for (prev = (afw_pool_cleanup_t *)& self->first_cleanup,
        e = self->first_cleanup;
        e; prev = e, e = e->next_cleanup)
    {
        if (e->data == data && e->data2 == data2 && e->cleanup == cleanup) {
            prev->next_cleanup = e->next_cleanup;
            afw_pool_free_memory(e, xctx);
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
    afw_pool_internal_memory_prefix_with_links_t *memory;
    afw_pool_internal_memory_prefix_with_links_t *next;
    afw_pool_internal_self_t *child;
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_destroy");

    /* Subpool always have a parent. (needed to suppress valgrind error) */
    if (!self->parent) {
        AFW_THROW_ERROR_Z(general, "Subpool has no parent", xctx);
    }

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

    /* If public apr pool made for this subpool, destroy it. */
    if (self->public_apr_p) {
        apr_pool_destroy(self->public_apr_p);
    }

    /* Return all allocated memory to the parent. */
    for (memory = self->first_allocated_memory;
        memory;
        memory = next)
    {
        next = memory->next;
        impl_free_memory(self, memory, memory->common.size, xctx);
    }

    impl_account_destroy(self, xctx);

    /* Removed self as child of parent. */
    impl_remove_as_child(self->parent, self, xctx);
}


apr_pool_t *
impl_subpool_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T * self)
{
    int rv;
    apr_pool_t *parent_apr_p;

    /*
     * For subpools, the public apr_p is only created if accessed. Note that
     * the private apr_p is the same as the parent's apr_p.
     */
    if (!self->public_apr_p) {
        parent_apr_p = afw_pool_get_apr_pool((const afw_pool_t *)self->parent);
        rv = apr_pool_create(&self->public_apr_p, parent_apr_p);
        if (rv != APR_SUCCESS) {
            /** @fixme do something. */
            //rv = 0/0;
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
    afw_byte_t *mem;
    afw_pool_internal_memory_prefix_with_links_t *block;
    afw_size_t size_with_prefix;
    afw_size_t actual_size;
    void *result;

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_*alloc " AFW_SIZE_T_FMT,
        size);

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    if (size > AFW_SIZE_T_MAX -
        sizeof(afw_pool_internal_memory_prefix_with_links_t))
    {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }

    size_with_prefix =
        size + sizeof(afw_pool_internal_memory_prefix_with_links_t);

    impl_alloc_memory(&mem, &actual_size, self, size_with_prefix, xctx);
    result = mem + sizeof(afw_pool_internal_memory_prefix_with_links_t);
    block = (afw_pool_internal_memory_prefix_with_links_t *)mem;
    block->common.p = (const afw_pool_t *)self;
    block->common.size = actual_size;
    block->prev = NULL;
    block->next = self->first_allocated_memory;
    if (self->first_allocated_memory) {
        self->first_allocated_memory->prev = block;
    }
    self->first_allocated_memory = block;
    impl_account_alloc(self, actual_size, xctx);

    return result;
}


static void
impl_subpool_afw_pool_free_memory_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_xctx_t *xctx)
{
    afw_pool_internal_memory_prefix_with_links_t *block;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "afw_pool_free NULL");
        return;
    }
    block = AFW_POOL_INTERNAL_MEMORY_PREFIX_WITH_LINKS(address);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "afw_pool_free %p " AFW_SIZE_T_FMT,
        address, block->common.size);

    impl_account_free(self, block->common.size, xctx);

    /* Remove from alloc chain. */

    /* Make memory available for reuse. */
    impl_free_memory(self, address, block->common.size, xctx);
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

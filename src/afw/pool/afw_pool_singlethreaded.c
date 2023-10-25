// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Singlethreaded pool implementation.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_pool_singlethreaded.c
 * @brief Adaptive Frameworksinglethreaded pool implementation.
 */

#include "afw_internal.h"



#ifdef AFW_POOL_DEBUG
#undef afw_pool_create
#undef afw_pool_internal_create_thread
#endif

/* Declares and rti/inf defines for interface afw_pool. */
#define AFW_IMPLEMENTATION_ID "afw_pool_singlethreaded"
#define AFW_POOL_SELF_T afw_pool_internal_singlethreaded_self_t
#include "afw_pool_impl_declares.h"

/** @fixme If pool is multithread, use a lock when changing memory. */
#define IMPL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define IMPL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    const afw_utf8_t *trace; \
    if (afw_flag_is_active(xctx->env->IMPL_DEBUG_LEVEL_##level, xctx)) \
    { \
        trace = afw_os_backtrace(0, -1, xctx); \
        afw_debug_write_fz(NULL, source_z, xctx, \
            "pool " AFW_INTEGER_FMT " " \
            info_z \
            ": before " AFW_SIZE_T_FMT \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " single" \
            "%s" \
            AFW_UTF8_FMT, \
            self->common.pool_number, \
            (self->bytes_allocated), \
            (self->reference_count), \
            (afw_integer_t)((self->common.parent) ? self->common.parent->pool_number : 0), \
            (char *)((trace) ? "\n" : ""), \
            (int)((trace) ? (int)trace->len : 0), \
            (const char *)((trace) ? (const char *)trace->s : "") \
            ); \
    } \
} while (0) \


#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
do { \
    const afw_utf8_t *trace; \
    if (afw_flag_is_active(xctx->env->IMPL_DEBUG_LEVEL_##level, xctx)) \
    { \
        trace = afw_os_backtrace(0, -1, xctx); \
        afw_debug_write_fz(NULL, source_z, xctx, \
            "pool " AFW_INTEGER_FMT " " \
            format_z \
            ": before " AFW_SIZE_T_FMT \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " single" \
            "%s" \
            AFW_UTF8_FMT, \
            self->common.pool_number, \
            __VA_ARGS__, \
            (self->bytes_allocated), \
            (self->reference_count), \
            (afw_integer_t)((self->common.parent) ? self->common.parent->pool_number : 0), \
            (char *)((trace) ? "\n" : ""), \
            (int)((trace) ? (int)trace->len : 0), \
            (const char *)((trace) ? (const char *)trace->s : "") \
            ); \
    } \
} while (0) \


AFW_DEFINE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    /* Create skeleton pool stuct. */
    self = (AFW_POOL_SELF_T *)afw_pool_internal_create(
        (afw_pool_internal_common_self_t *)parent,
        &impl_afw_pool_inf, sizeof(AFW_POOL_SELF_T), xctx);
    self->reference_count = 1;

    /* Parent is required. */
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
 
    /* If thread specific parent pool, this one is as well for same thread. */
    if (((AFW_POOL_SELF_T *)parent)->thread) {
        self->thread = xctx->thread;
    }

    /* Return new pool. */
    return &self->common.pub;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create_debug(
    const afw_pool_t *parent,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    AFW_POOL_SELF_T *self = 
        (AFW_POOL_SELF_T *)afw_pool_create(parent, xctx);

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_create " AFW_INTEGER_FMT,
        ((const AFW_POOL_SELF_T *)parent)->common.pool_number);

    return &self->common.pub;
}



/* Create a new thread specific pool for xctx. */
AFW_DEFINE(afw_thread_t *)
afw_pool_internal_create_thread(
    afw_size_t size,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    afw_thread_t *thread;

    if (size == -1 || size < sizeof(afw_thread_t)) {
        size = sizeof(afw_thread_t);
    }
    /* Create skeleton pool stuct. */
    self = (AFW_POOL_SELF_T *)afw_pool_internal_create(
        NULL,
        &impl_afw_pool_inf, sizeof(AFW_POOL_SELF_T), xctx);
    self->reference_count = 1;
    thread = apr_pcalloc(self->common.apr_p, size);
    self->thread = thread;
    thread->p = (const afw_pool_t *)self;

    return thread;
}



/* Debug version of create a new thread specific pool for xctx. */
AFW_DEFINE(afw_thread_t *)
afw_pool_internal_create_thread_debug(
    afw_size_t size,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    afw_thread_t *thread =
        afw_pool_internal_create_thread(size, xctx);
    const AFW_POOL_SELF_T *self =
        (const AFW_POOL_SELF_T *)thread->p;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_internal_create_thread " AFW_SIZE_T_FMT,
        size);

    return thread;
}



/*
 * Implementation of method release for interface afw_pool.
 */
void
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* If instance is NULL, just return. */
    /** @fixme This should probably go away. */
    if (!self) {
        return;
    }

    //printf("pool " AFW_INTEGER_FMT " release refs " AFW_SIZE_T_FMT "\n",
    //    self->pool_number, self->reference_count);

    /* Decrement reference count and release pools resources if zero. */
    if (--(self->reference_count) == 0) {
        afw_pool_destroy(&self->common.pub, xctx);
    }
}

/*
 * Implementation of method get_reference for interface afw_pool.
 */
void
impl_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* If instance is NULL, just return. */
    if (!self) {
        return;
    }

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
    afw_pool_internal_common_self_t *child;
    afw_pool_cleanup_t *e;

    /* If instance is NULL, just return. */
    if (!self) {
        return;
    }

    /*
     * Call all of the cleanup routines for this pool before releasing children.
     */
    for (e = self->common.first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->common.pub, xctx);
    }

    /*
     * Release children.
     *
     * Release of child sets self->common.first_child to its next sibling.
     */
    for (child = self->common.first_child;
        child;
        child = self->common.first_child)
    {
        afw_pool_release(&child->pub, xctx);
    }

    /* If parent, removed self as child. */
    if (self->common.parent) {
        afw_pool_internal_remove_as_child(
            self->common.parent, &self->common, xctx);
    }

    /* Destroy apr pool. */
    apr_pool_destroy(self->common.apr_p);
}

/*
 * Implementation of method get_apr_pool for interface afw_pool.
 */
apr_pool_t *
impl_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T * self)
{
    int rv;

    if (!self->common.apr_p) {
        rv = apr_pool_create(&self->common.apr_p,
            (self->common.parent) ? self->common.parent->apr_p : NULL);
        if (rv != APR_SUCCESS) {
            /** @fixme do something. */
            //rv = 0/0;
        }
    }

    return self->common.apr_p;
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

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    result = apr_pcalloc(self->common.apr_p, size);
    self->bytes_allocated += size;

    if (!result) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error.", xctx);
    }

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
    void *result;

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    result = apr_palloc(self->common.apr_p, size);
    self->bytes_allocated += size;

    if (!result) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error.", xctx);
    }

    return result;
}

/*
 * Implementation of method free for interface afw_pool.
 */
void
impl_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

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

    /* Allocate entry which will also make sure its ok to use pool. */
    e = afw_pool_calloc_type(&self->common.pub, afw_pool_cleanup_t, xctx);

    /* Add entry to front of list of cleanup functions. */
    e->data = data;
    e->data2 = data2;
    e->cleanup = cleanup;
    e->next_cleanup = self->common.first_cleanup;
    self->common.first_cleanup = e;
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

    /* Search for entry and remove. */
    for (prev = (afw_pool_cleanup_t *)& self->common.first_cleanup,
        e = self->common.first_cleanup;
        e; prev = e, e = e->next_cleanup)
    {
        if (e->data == data && e->data2 == data2 && e->cleanup == cleanup) {
            prev->next_cleanup = e->next_cleanup;
            break;
        }
    }
}

/*
 * Implementation of method release_debug for interface afw_pool.
 */
void
impl_afw_pool_release_debug(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_release");

    impl_afw_pool_release(self, xctx);
}

/*
 * Implementation of method get_reference for interface afw_pool.
 */
void
impl_afw_pool_get_reference_debug(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_get_reference");

    impl_afw_pool_get_reference(self, xctx);
}

/*
 * Implementation of method destroy_debug for interface afw_pool.
 */
void
impl_afw_pool_destroy_debug(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_destroy");

    impl_afw_pool_destroy(self, xctx);
}


/*
 * Implementation of method calloc_debug for interface afw_pool.
 */
void *
impl_afw_pool_calloc_debug(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_calloc " AFW_SIZE_T_FMT,
        size);

    return impl_afw_pool_calloc(self, size, xctx);
}

/*
 * Implementation of method malloc_debug for interface afw_pool.
 */
void *
impl_afw_pool_malloc_debug(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_malloc " AFW_SIZE_T_FMT,
        size);

    return impl_afw_pool_malloc(self, size, xctx);
}

/*
 * Implementation of method free_debug for interface afw_pool.
 */
void
impl_afw_pool_free_debug(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_free " AFW_SIZE_T_FMT,
        size);

    impl_afw_pool_free(self, address, size, xctx);
}

/*
 * Implementation of method register_cleanup_before_debug for interface
 * afw_pool.
 */
void
impl_afw_pool_register_cleanup_before_debug(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_register_cleanup_before %p %p",
        data, cleanup);

    impl_afw_pool_register_cleanup_before(self, data, data2, cleanup, xctx);
}

/*
 * Implementation of method deregister_cleanup_debug for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup_debug(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_deregister_cleanup_debug %p %p",
        data, cleanup);

    impl_afw_pool_deregister_cleanup(self, data, data2, cleanup, xctx);
}


void afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    if (pool->inf != &impl_afw_pool_inf) {
        printf("Not correct pool inf\n");
        return;
    }
    const AFW_POOL_SELF_T *self = (const AFW_POOL_SELF_T *)pool;

    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    printf(
        "pool " AFW_INTEGER_FMT " " AFW_SIZE_T_FMT " refs " AFW_INTEGER_FMT
        " parent " AFW_INTEGER_FMT "\n",
        self->common.pool_number,
        (self->bytes_allocated),
        (self->reference_count),
        (afw_integer_t)
            ((self->common.parent) &&
                self->common.parent->pub.inf == &impl_afw_pool_inf
                ? self->common.parent->pool_number
                : 0)
        );

    // for (int i = 0; i < indent; i++) {
    //     printf("  ");
    // }
    // printf("Siblings:\n");
    // for (const AFW_POOL_SELF_T *sibling = self->common.next_sibling;
    //     sibling;
    //     sibling = sibling->next_sibling)
    // {
    //     afw_pool_print_debug_info(indent + 2, &sibling->pub, xctx);
    // }

    for (
        const afw_pool_internal_common_self_t *child = self->common.first_child;
        child;
        child = child->next_sibling)
    {
        afw_pool_print_debug_info(indent + 2, &child->pub, xctx);
    }

}

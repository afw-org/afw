// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Multithreaded pool implementation.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_pool_multithreaded.c
 * @brief Adaptive Framework multithreaded pool implementation.
 */

#include "afw_internal.h"



#ifdef AFW_POOL_DEBUG
#undef afw_pool_create_multithreaded
#endif

#define AFW_IMPLEMENTATION_ID "afw_pool_multithreaded"
#define AFW_POOL_SELF_T afw_pool_internal_multithreaded_self_t
#include "afw_pool_impl_declares.h"

/* multithreaded pool lock begin */
#define IMPL_MULTITHREADED_LOCK_BEGIN(xctx) \
AFW_LOCK_BEGIN((xctx)->env->multithreaded_pool_lock)

/* multithreaded pool lock end */
#define IMPL_MULTITHREADED_LOCK_END \
AFW_LOCK_END;

AFW_DEFINE(const afw_pool_t *)
afw_pool_internal_create_base_pool()
{
    apr_pool_t *apr_p;
    AFW_POOL_SELF_T *self;

    /* Create new pool for environment and initial xctx. */
    apr_pool_create(&apr_p, NULL);
    if (!apr_p) {
        return NULL;
    };

    /* Allocate self. */
    self = apr_pcalloc(apr_p,
        sizeof(AFW_POOL_SELF_T));
    if (!self) {
        return NULL;
    }
    self->common.pub.inf = &impl_afw_pool_inf;
    self->common.apr_p = apr_p;
    self->common.name = afw_s_base;
    self->common.pool_number = 1;
    self->reference_count = 1;

    return &self->common.pub;
}


/* Create a new multithreaded pool. */
AFW_DEFINE(const afw_pool_t *)
afw_pool_create_multithreaded(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    if (parent) {
        if (parent != xctx->env->p &&
            parent->inf != &impl_afw_pool_inf)
        {
            AFW_THROW_ERROR_Z(general,
                "Parent pool must be base or multithreaded", xctx);
        }
    }
    else {
        parent = xctx->env->p;
    }

    /* Create skeleton pool stuct. */
    self = (AFW_POOL_SELF_T *)afw_pool_internal_create(
        (afw_pool_internal_common_self_t *)parent,
        &impl_afw_pool_inf, sizeof(AFW_POOL_SELF_T), xctx);
    self->reference_count = 1;

    return &self->common.pub;
}


/* Create a new multithreaded pool. */
AFW_DEFINE(const afw_pool_t *)
afw_pool_create_multithreaded_debug(
    const afw_pool_t *parent,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    AFW_POOL_SELF_T *self =
        (AFW_POOL_SELF_T *)
        afw_pool_create_multithreaded(parent, xctx);

    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_create_multithreaded " AFW_INTEGER_FMT,
        (self->common.parent) ? self->common.parent->pool_number : 0);

    return &self->common.pub;
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

    /* Decrement reference count and release pools resources if zero. */
    if (afw_atomic_integer_decrement(&self->reference_count) == 0) {
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
    afw_atomic_integer_increment(&self->reference_count);
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
     * Call all of the cleanup routines for this pool before destroying
     * children.
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
    AFW_POOL_SELF_T *self)
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

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = apr_pcalloc(self->common.apr_p, size);
        self->bytes_allocated += size;
    }
    IMPL_MULTITHREADED_LOCK_END;

    if (!result) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
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

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = apr_palloc(self->common.apr_p, size);
        self->bytes_allocated += size;
    }
    IMPL_MULTITHREADED_LOCK_END;

    if (!result) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
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
    AFW_THROW_ERROR_Z(general, "Method not implemented", xctx);
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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_release");

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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_get_reference");

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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_destroy");

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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(detail,
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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(detail,
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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(detail,
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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(minimal,
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
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_deregister_cleanup_debug %p %p",
        data, cleanup);

    impl_afw_pool_deregister_cleanup(self, data, data2, cleanup, xctx);
}

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
#include "afw_pool_impl_declares.h"

/* multithreaded pool lock begin */
#define IMPL_MULTITHREADED_LOCK_BEGIN(xctx) \
AFW_LOCK_BEGIN((xctx)->env->multithreaded_pool_lock)

/* multithreaded pool lock end */
#define IMPL_MULTITHREADED_LOCK_END \
AFW_LOCK_END;


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
            "pool %" AFW_INTEGER_FMT " " \
            info_z \
            ": before %" AFW_SIZE_T_FMT \
            " refs %" AFW_INTEGER_FMT \
            " parent %" AFW_INTEGER_FMT \
            " multi" \
            "%s" \
            "%" AFW_UTF8_FMT, \
            self->pool_number, \
            (self->bytes_allocated), \
            (self->reference_count), \
            (afw_integer_t)((self->parent) ? self->parent->pool_number : 0), \
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
            "pool %" AFW_INTEGER_FMT " " \
            format_z \
            ": before %" AFW_SIZE_T_FMT \
            " refs %" AFW_INTEGER_FMT \
            " parent %" AFW_INTEGER_FMT \
            " multi" \
            "%s" \
            "%" AFW_UTF8_FMT, \
            self->pool_number, \
            __VA_ARGS__, \
            (self->bytes_allocated), \
            (self->reference_count), \
            (afw_integer_t)((self->parent) ? self->parent->pool_number : 0), \
            (char *)((trace) ? "\n" : ""), \
            (int)((trace) ? (int)trace->len : 0), \
            (const char *)((trace) ? (const char *)trace->s : "") \
            ); \
    } \
} while (0) \


static void
impl_add_child(afw_pool_internal_multithreaded_self_t *parent,
    afw_pool_internal_multithreaded_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_add_reference(&parent->pub, xctx);

    child->next_sibling = parent->first_child;
    parent->first_child = child;
}


static void
impl_remove_child(afw_pool_internal_multithreaded_self_t *parent,
    afw_pool_internal_multithreaded_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *prev;
    afw_pool_internal_multithreaded_self_t *sibling;

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


/* Create skeleton pool struct. */
static afw_pool_internal_multithreaded_self_t *
impl_pool_create(afw_pool_internal_multithreaded_self_t *parent, afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    afw_pool_internal_multithreaded_self_t *self;

    apr_pool_create(&apr_p, (parent) ? parent->apr_p : NULL);
    if (!apr_p) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self = apr_pcalloc(apr_p, sizeof(afw_pool_internal_multithreaded_self_t));
    if (!self) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self->pub.inf = &impl_afw_pool_inf;
    self->apr_p = apr_p;
    self->reference_count = 1;
    self->parent = parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);

    /* If parent, add this new child. */
    if (parent) {
        impl_add_child(parent, self, xctx);
    }

    /* Return new pool. */
    return self;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_internal_create_base_pool()
{
    apr_pool_t *apr_p;
    afw_pool_internal_multithreaded_self_t *self;

    /* Create new pool for environment and initial xctx. */
    apr_pool_create(&apr_p, NULL);
    if (!apr_p) {
        return NULL;
    };

    /* Allocate self. */
    self = apr_pcalloc(apr_p,
        sizeof(afw_pool_internal_multithreaded_self_t));
    if (!self) {
        return NULL;
    }
    self->pub.inf = &impl_afw_pool_inf;
    self->apr_p = apr_p;
    self->name = &afw_s_base;
    self->pool_number = 1;
    self->reference_count = 1;

    return (const afw_pool_t *)self;
}


/* Create a new multithreaded pool. */
AFW_DEFINE(const afw_pool_t *)
afw_pool_create_multithreaded(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self;

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

    self = impl_pool_create((afw_pool_internal_multithreaded_self_t *)parent, xctx);

    return (const afw_pool_t *)self;
}


/* Create a new multithreaded pool. */
AFW_DEFINE(const afw_pool_t *)
afw_pool_create_multithreaded_debug(
    const afw_pool_t *parent,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)
        afw_pool_create_multithreaded(parent, xctx);

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_create_multithreaded %" AFW_INTEGER_FMT,
        (self->parent) ? self->parent->pool_number : 0);

    return (const afw_pool_t *)self;
}


/*
 * Implementation of method release for interface afw_pool.
 */
void
impl_afw_pool_release(
    const afw_pool_t * instance,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    /* If instance is NULL, just return. */
    /** @fixme This should probably go away. */
    if (!instance) return;

    /* Decrement reference count and release pools resources if zero. */
    if (afw_atomic_integer_decrement(&self->reference_count) == 0) {
        afw_pool_destroy(instance, xctx);
    }
}

/*
 * Implementation of method add_reference for interface afw_pool.
 */
void
impl_afw_pool_add_reference(
    const afw_pool_t * instance,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self;

    /* If instance is NULL, just return. */
    if (!instance) return;

    /* Decrement reference count. */
    self = (afw_pool_internal_multithreaded_self_t *)instance;
    afw_atomic_integer_increment(&self->reference_count);
}

/*
 * Implementation of method destroy for interface afw_pool.
 */
void
impl_afw_pool_destroy(
    const afw_pool_t * instance,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;
    afw_pool_internal_multithreaded_self_t *child;
    afw_pool_cleanup_t *e;

    /* If instance is NULL, just return. */
    if (!instance) return;

    /*
     * Call all of the cleanup routines for this pool before destroying
     * children.
     */
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, instance, xctx);
    }

    /*
     * Destroy children.
     *
     * Release of child sets self->first_child to its next sibling.
     */
    for (child = self->first_child;
        child;
        child = self->first_child)
    {
        afw_pool_destroy((const afw_pool_t *)child, xctx);
    }

    /* If parent, removed self as child. */
    if (self->parent) {
        impl_remove_child(self->parent, self, xctx);
    }

    /* Destroy apr pool. */
    apr_pool_destroy(self->apr_p);
}

/*
 * Implementation of method get_apr_pool for interface afw_pool.
 */
apr_pool_t *
impl_afw_pool_get_apr_pool(
    const afw_pool_t * instance)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;
    int rv;

    if (!self->apr_p) {
        rv = apr_pool_create(&self->apr_p,
            (self->parent) ? self->parent->apr_p : NULL);
        if (rv != APR_SUCCESS) {
            /** @fixme do something. */
            //rv = 0/0;
        }
    }

    return self->apr_p;
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
impl_afw_pool_calloc(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;
    void *result;

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = apr_pcalloc(self->apr_p, size);
        self->bytes_allocated += size;
    }
    IMPL_MULTITHREADED_LOCK_END;

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
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;
    void *result;

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = apr_palloc(self->apr_p, size);
        self->bytes_allocated += size;
    }
    IMPL_MULTITHREADED_LOCK_END;

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
    const afw_pool_t * instance,
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
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;
    afw_pool_cleanup_t *e;

    /* Allocate entry which will also make sure its ok to use pool. */
    e = afw_pool_calloc_type(instance, afw_pool_cleanup_t, xctx);

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
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_internal_multithreaded_self_t *self = (afw_pool_internal_multithreaded_self_t *)instance;
    afw_pool_cleanup_t *e, *prev;

    /* Search for entry and remove. */
    for (prev = (afw_pool_cleanup_t *)& self->first_cleanup,
        e = self->first_cleanup;
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
    const afw_pool_t * instance,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_release");

    impl_afw_pool_release(instance, xctx);
}

/*
 * Implementation of method add_reference for interface afw_pool.
 */
void
impl_afw_pool_add_reference_debug(
    const afw_pool_t * instance,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_add_reference");

    impl_afw_pool_add_reference(instance, xctx);
}

/*
 * Implementation of method destroy_debug for interface afw_pool.
 */
void
impl_afw_pool_destroy_debug(
    const afw_pool_t * instance,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_destroy");

    impl_afw_pool_destroy(instance, xctx);
}


/*
 * Implementation of method calloc_debug for interface afw_pool.
 */
void *
impl_afw_pool_calloc_debug(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_calloc %" AFW_SIZE_T_FMT,
        size);

    return impl_afw_pool_calloc(instance, size, xctx);
}

/*
 * Implementation of method malloc_debug for interface afw_pool.
 */
void *
impl_afw_pool_malloc_debug(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_malloc %" AFW_SIZE_T_FMT,
        size);

    return impl_afw_pool_malloc(instance, size, xctx);
}

/*
 * Implementation of method free_debug for interface afw_pool.
 */
void
impl_afw_pool_free_debug(
    const afw_pool_t * instance,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_free %" AFW_SIZE_T_FMT,
        size);

    impl_afw_pool_free(instance, address, size, xctx);
}

/*
 * Implementation of method register_cleanup_before_debug for interface
 * afw_pool.
 */
void
impl_afw_pool_register_cleanup_before_debug(
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_register_cleanup_before %p %p",
        data, cleanup);

    impl_afw_pool_register_cleanup_before(instance, data, data2, cleanup, xctx);
}

/*
 * Implementation of method deregister_cleanup_debug for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup_debug(
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * source_z)
{
    const afw_pool_internal_multithreaded_self_t *self =
        (afw_pool_internal_multithreaded_self_t *)instance;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_deregister_cleanup_debug %p %p",
        data, cleanup);

    impl_afw_pool_deregister_cleanup(instance, data, data2, cleanup, xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Singlethreaded pool implementation.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_pool.c
 * @brief Adaptive Framework pool implementations.
 * 
 * All implementations release all memory when the pool is destroyed.
 * 
 * This c file implements the afw_pool interface for the following pool types:
 * - unmanaged - Method free() is ignored.
 * - managed - Memory is managed in blocks and freed memory is available for
 *             reuse.
 * - subpool - Memory is allocated in parent pool. Method free() adds the free
 *             memory back to the parent pool. Any memory in the subpool that
 *             is not explicitly freed will be added back to parent pool.
 * - multithreaded -         This is a thread-safe managed pool that is
 *                           protected with a mutex.
 * - multithreaded_subpool - This is a thread-safe subpool protected with a
 *                           mutex.
 */

#include "afw_internal.h"

/* multithreaded pool lock begin */
#define IMPL_MULTITHREADED_LOCK_BEGIN(xctx) \
AFW_LOCK_BEGIN((xctx)->env->multithreaded_pool_lock)

/* multithreaded pool lock end */
#define IMPL_MULTITHREADED_LOCK_END \
AFW_LOCK_END;

/* Declares and rti/inf defines for interface afw_pool for unmanaged pool. */
#define AFW_POOL_SELF_T afw_pool_internal_self_t

#define AFW_IMPLEMENTATION_ID "unmanaged"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_unmanaged_inf
#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL

#define AFW_POOL_INF_ONLY 1

/* Declares and rti/inf defines for interface afw_pool for managed pool. */
#define AFW_IMPLEMENTATION_ID "managed"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_managed_inf

static void
impl_managed_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_release \
    impl_managed_afw_pool_release

static void *
impl_managed_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc \
    impl_managed_afw_pool_calloc

static void *
impl_managed_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
   
#define impl_afw_pool_malloc \
    impl_managed_afw_pool_malloc

static void
impl_managed_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free \
    impl_managed_afw_pool_free

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_pool_release
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free


/* Declares and rti/inf defines for interface afw_pool for subpool pool. */
#define AFW_IMPLEMENTATION_ID "subpool"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_subpool_inf

static void
impl_subpool_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_release \
    impl_subpool_afw_pool_release

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
impl_subpool_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free \
    impl_subpool_afw_pool_free

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_pool_release
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free


/* Declares and rti/inf defines for interface afw_pool for multithreaded pool.*/
#define AFW_IMPLEMENTATION_ID "multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_multithreaded_inf

static void
impl_multithreaded_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_release \
    impl_multithreaded_afw_pool_release

static void
impl_multithreaded_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_get_reference \
    impl_multithreaded_afw_pool_get_reference

static void
impl_multithreaded_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_destroy \
    impl_multithreaded_afw_pool_destroy

static apr_pool_t *
impl_multithreaded_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self);

#define impl_afw_pool_get_apr_pool \
    impl_multithreaded_afw_pool_get_apr_pool


static void *
impl_multithreaded_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc \
    impl_multithreaded_afw_pool_calloc

static void *
impl_multithreaded_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
   
#define impl_afw_pool_malloc \
    impl_multithreaded_afw_pool_malloc

static void
impl_multithreaded_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free \
    impl_multithreaded_afw_pool_free

static void
impl_multithreaded_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx);

#define impl_afw_pool_register_cleanup_before \
    impl_multithreaded_afw_pool_register_cleanup_before

static void
impl_multithreaded_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx);

#define impl_afw_pool_deregister_cleanup \
    impl_multithreaded_afw_pool_deregister_cleanup
    
#include "afw_pool_impl_declares.h"

#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_destroy
#undef impl_afw_pool_get_apr_pool
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free
#undef impl_afw_pool_register_cleanup_before
#undef impl_afw_pool_deregister_cleanup


/* Declares and rti/inf defines for interface afw_pool for multithreaded_subpool pool.*/
#define AFW_IMPLEMENTATION_ID "multithreaded_subpool"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_multithreaded_subpool_inf

static void
impl_multithreaded_subpool_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_release \
    impl_multithreaded_subpool_afw_pool_release

static void
impl_multithreaded_subpool_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_get_reference \
    impl_multithreaded_subpool_afw_pool_get_reference

static void
impl_multithreaded_subpool_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_destroy \
    impl_multithreaded_subpool_afw_pool_destroy

static apr_pool_t *
impl_multithreaded_subpool_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self);

#define impl_afw_pool_get_apr_pool \
    impl_multithreaded_subpool_afw_pool_get_apr_pool


static void *
impl_multithreaded_subpool_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc \
    impl_multithreaded_subpool_afw_pool_calloc

static void *
impl_multithreaded_subpool_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
   
#define impl_afw_pool_malloc \
    impl_multithreaded_subpool_afw_pool_malloc

static void
impl_multithreaded_subpool_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free \
    impl_multithreaded_subpool_afw_pool_free

static void
impl_multithreaded_subpool_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx);

#define impl_afw_pool_register_cleanup_before \
    impl_multithreaded_subpool_afw_pool_register_cleanup_before

static void
impl_multithreaded_subpool_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx);

#define impl_afw_pool_deregister_cleanup \
    impl_multithreaded_subpool_afw_pool_deregister_cleanup
    
#include "afw_pool_impl_declares.h"

#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_destroy
#undef impl_afw_pool_get_apr_pool
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free
#undef impl_afw_pool_register_cleanup_before
#undef impl_afw_pool_deregister_cleanup

/* Debug macros only add code if AFW_POOL_DEBUG is defined. */
#ifdef AFW_POOL_DEBUG

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z)
#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...)

#else

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    const afw_utf8_t *trace; \
    if (afw_flag_is_active( \
        xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        trace = afw_os_backtrace(0, -1, xctx); \
        afw_debug_write_fz(NULL, AFW__FILE_LINE__, xctx, \
            "pool " AFW_INTEGER_FMT " " \
            info_z \
            ": before " AFW_SIZE_T_FMT \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            "%s" \
            AFW_UTF8_FMT, \
            self->pool_number, \
            (self->bytes_allocated), \
            (self->reference_count), \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : \
                0), \
            (char *)((trace) ? "\n" : ""), \
            (int)((trace) ? (int)trace->len : 0), \
            (const char *)((trace) ? (const char *)trace->s : "") \
            ); \
    } \
} while (0) \

#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
do { \
    const afw_utf8_t *trace; \
    if (afw_flag_is_active( \
        xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        trace = afw_os_backtrace(0, -1, xctx); \
        afw_debug_write_fz(NULL, AFW__FILE_LINE__, xctx, \
            "pool " AFW_INTEGER_FMT " " \
            format_z \
            ": before " AFW_SIZE_T_FMT \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            "%s" \
            AFW_UTF8_FMT, \
            self->pool_number, \
            __VA_ARGS__, \
            (self->bytes_allocated), \
            (self->reference_count), \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number \
                : 0), \
            (char *)((trace) ? "\n" : ""), \
            (int)((trace) ? (int)trace->len : 0), \
            (const char *)((trace) ? (const char *)trace->s : "") \
            ); \
    } \
} while (0) \

#endif



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


/* Create skeleton pool struct. */
static afw_pool_internal_self_t *
impl_create(
    afw_pool_internal_self_t *parent,
    const afw_pool_inf_t *inf,
    afw_size_t instance_size,
    afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    afw_pool_internal_self_t *self;

    apr_pool_create(&apr_p, (parent) ? parent->apr_p : NULL);
    if (!apr_p) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self = apr_pcalloc(apr_p, instance_size);
    if (!self) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self->pub.inf = inf;
    self->apr_p = apr_p;
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
afw_pool_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    /* Parent is required. */
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    /* Create skeleton pool stuct. */
    self = (AFW_POOL_SELF_T *)impl_create(
        (afw_pool_internal_self_t *)parent,
        &afw_pool_unmanaged_inf, sizeof(AFW_POOL_SELF_T), xctx);
    self->reference_count = 1;
 
    /* If thread specific parent pool, this one is as well for same thread. */
    if (((AFW_POOL_SELF_T *)parent)->thread) {
        self->thread = xctx->thread;
    }

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_create " AFW_INTEGER_FMT,
        ((const AFW_POOL_SELF_T *)parent)->pool_number);

    /* Return new pool. */
    return &self->pub;
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
    self = (AFW_POOL_SELF_T *)impl_create(
        NULL,
        &afw_pool_unmanaged_inf, sizeof(AFW_POOL_SELF_T), xctx);
    self->reference_count = 1;
    thread = apr_pcalloc(self->apr_p, size);
    self->thread = thread;
    thread->p = (const afw_pool_t *)self;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_internal_create_thread " AFW_SIZE_T_FMT,
        size);

    return thread;
}

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
    self->pub.inf = &afw_pool_multithreaded_inf;
    self->apr_p = apr_p;
    self->name = afw_s_base;
    self->pool_number = 1;
    self->reference_count = 1;

    return &self->pub;
}


/* Create a new multithreaded pool. */
AFW_DEFINE(const afw_pool_t *)
afw_pool_create_multithreaded(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    if (parent) {
        if (parent != xctx->env->p &&
            parent->inf != &afw_pool_multithreaded_inf)
        {
            AFW_THROW_ERROR_Z(general,
                "Parent pool must be base or multithreaded", xctx);
        }
    }
    else {
        parent = xctx->env->p;
    }

    /* Create skeleton pool stuct. */
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {

        self = (AFW_POOL_SELF_T *)impl_create(
            (afw_pool_internal_self_t *)parent,
            &afw_pool_multithreaded_inf, sizeof(AFW_POOL_SELF_T), xctx);
        self->reference_count = 1;
    }
    IMPL_MULTITHREADED_LOCK_END;   

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_create_multithreaded " AFW_INTEGER_FMT,
        (self->parent) ? self->parent->pool_number : 0);

    return &self->pub;
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

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_release");

    //printf("pool " AFW_INTEGER_FMT " release refs " AFW_SIZE_T_FMT "\n",
    //    self->pool_number, self->reference_count);

    /* Decrement reference count and release pools resources if zero. */
    if (--(self->reference_count) == 0) {
        afw_pool_destroy(&self->pub, xctx);
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

    /* If instance is NULL, just return. */
    if (!self) {
        return;
    }

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

    /* If parent, removed self as child. */
    if (self->parent) {
        impl_remove_as_child(self->parent, self, xctx);
    }

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
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_calloc " AFW_SIZE_T_FMT,
        size);

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    result = apr_pcalloc(self->apr_p, size);
    self->bytes_allocated += size;

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

    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_malloc " AFW_SIZE_T_FMT,
        size);

    /* Don't allow allocate for a size of 0. */
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    result = apr_palloc(self->apr_p, size);
    self->bytes_allocated += size;

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
    IMPL_PRINT_DEBUG_INFO_FZ(detail,
        "afw_pool_free " AFW_SIZE_T_FMT,
        size);

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
            break;
        }
    }
}


void afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    if (pool->inf != &afw_pool_unmanaged_inf) {
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
        self->pool_number,
        (self->bytes_allocated),
        (self->reference_count),
        (afw_integer_t)
            ((self->parent) &&
                self->parent->pub.inf == &afw_pool_unmanaged_inf
                ? self->parent->pool_number
                : 0)
        );

    // for (int i = 0; i < indent; i++) {
    //     printf("  ");
    // }
    // printf("Siblings:\n");
    // for (const AFW_POOL_SELF_T *sibling = self->next_sibling;
    //     sibling;
    //     sibling = sibling->next_sibling)
    // {
    //     afw_pool_print_debug_info(indent + 2, &sibling->pub, xctx);
    // }

    for (
        const afw_pool_internal_self_t *child = self->first_child;
        child;
        child = child->next_sibling)
    {
        afw_pool_print_debug_info(indent + 2, &child->pub, xctx);
    }

}

/* --------------------------- managed implementations ---------------------- */

void
impl_managed_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    
}


void *
impl_managed_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return NULL;
}


static void *
impl_managed_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return NULL;
}


static void
impl_managed_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    
}


/* --------------------------- subpool implementations ------------------------ */

void
impl_subpool_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    
}


void *
impl_subpool_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return NULL;
}


static void *
impl_subpool_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return NULL;
}


static void
impl_subpool_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    
}


/* ----------------------------multithreaded implementations ---------------- */

void
impl_multithreaded_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


apr_pool_t *
impl_multithreaded_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self)
{
    apr_pool_t *result;

    //FIXME IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_afw_pool_get_apr_pool(self);
    //FIXME }
    //FIXME MPL_MULTITHREADED_LOCK_END;

    return result;
}


void *
impl_multithreaded_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_afw_pool_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;

    return result;
}

void *
impl_multithreaded_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_afw_pool_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;

    return result;
}
   
void
impl_multithreaded_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_free(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_register_cleanup_before(self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_deregister_cleanup(self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;

}

/* ----------------------------multithreaded subpool implementations -------- */

void
impl_multithreaded_subpool_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_subpool_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_subpool_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_subpool_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


apr_pool_t *
impl_multithreaded_subpool_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self)
{
    apr_pool_t *result;

    //FIXME IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_afw_pool_get_apr_pool(self);
    //FIXME }
    //FIXME MPL_MULTITHREADED_LOCK_END;

    return result;
}


void *
impl_multithreaded_subpool_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_subpool_afw_pool_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;

    return result;
}

void *
impl_multithreaded_subpool_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_subpool_afw_pool_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;

    return result;
}
   
void
impl_multithreaded_subpool_afw_pool_free(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_subpool_afw_pool_free(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_subpool_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_register_cleanup_before(self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

void
impl_multithreaded_subpool_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_deregister_cleanup(self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;

}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework pool implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_apr.c
 * @brief Parked wrap-APR pool (not wired).
 *
 * Live allocations still use afw_pool.c (develop prefix/subpool). This
 * implementation is kept to design a hybrid later: wrap-APR for request /
 * managed object lifetime; something cheaper than an APR pool per script
 * scope. Do not call these create functions from production paths yet.
 *
 * Two infs, one struct. Destroy is apr_pool_destroy. Individual free is
 * a no-op. No prefixes, no first-fit, no create_subpool.
 */

#include "afw_internal.h"
#include "afw_pool_apr_internal.h"

#include <stdio.h>

#define IMPL_MULTITHREADED_LOCK_BEGIN(xctx) \
AFW_LOCK_BEGIN((xctx)->env->multithreaded_pool_lock)

#define IMPL_MULTITHREADED_LOCK_END \
AFW_LOCK_END;

#define AFW_POOL_SELF_T afw_pool_apr_self_t

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    const afw_utf8_t *trace; \
    if (xctx && xctx->env && \
        afw_flag_is_active( \
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
} while (0)

#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
do { \
    const afw_utf8_t *trace; \
    if (xctx && xctx->env && \
        afw_flag_is_active( \
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
                ? self->parent->pool_number : \
                0), \
            (char *)((trace) ? "\n" : ""), \
            (int)((trace) ? (int)trace->len : 0), \
            (const char *)((trace) ? (const char *)trace->s : "") \
            ); \
    } \
} while (0)


/* Core methods: declared here so link/create can call them without
 * going through inf (nested multi lock). Full bodies after declares. */
static const afw_pool_t *
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

static void
impl_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

static void
impl_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);


static void
impl_assert_thread(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    if (self->thread && xctx && xctx->thread &&
        self->thread != xctx->thread)
    {
        AFW_THROW_ERROR_Z(general,
            "Thread-specific pool used from another thread", xctx);
    }
}


/*
 * Doubly-linked child list. add holds the parent; unlink drops that
 * hold unless the parent is already destroying (parent walks children
 * with destroy, not release).
 */
static void
impl_link_child(
    afw_pool_apr_self_t *parent,
    afw_pool_apr_self_t *child)
{
    child->parent = parent;
    child->prev_sibling = NULL;
    child->next_sibling = parent->first_child;
    if (parent->first_child) {
        parent->first_child->prev_sibling = child;
    }
    parent->first_child = child;
}


static void
impl_unlink_child(
    afw_pool_apr_self_t *parent,
    afw_pool_apr_self_t *child)
{
    if (child->prev_sibling) {
        child->prev_sibling->next_sibling = child->next_sibling;
    }
    else {
        parent->first_child = child->next_sibling;
    }
    if (child->next_sibling) {
        child->next_sibling->prev_sibling = child->prev_sibling;
    }
    child->prev_sibling = NULL;
    child->next_sibling = NULL;
    child->parent = NULL;
}


static void
impl_add_child(
    afw_pool_apr_self_t *parent,
    afw_pool_apr_self_t *child,
    afw_xctx_t *xctx)
{
    /* Core get_reference: caller holds the multi lock if parent is multi. */
    impl_afw_pool_get_reference(parent, xctx);
    impl_link_child(parent, child);
}


static afw_pool_apr_self_t *
impl_create(
    apr_pool_t *parent_apr,
    const afw_pool_inf_t *inf,
    afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    afw_pool_apr_self_t *self;

    apr_pool_create(&apr_p, parent_apr);
    if (!apr_p) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self = apr_pcalloc(apr_p, sizeof(afw_pool_apr_self_t));
    if (!self) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self->pub.inf = inf;
    self->pub.managed_p = &self->pub;
    self->apr_p = apr_p;
    self->reference_count = 1;
    if (xctx && xctx->env) {
        self->pool_number = (afw_integer_t)afw_atomic_integer_increment(
            &((afw_environment_t *)xctx->env)->pool_number);
    }

    return self;
}


#define AFW_IMPLEMENTATION_ID "pool"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_inf
#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL


/*
 * Implementation of method release for interface afw_pool.
 */
const afw_pool_t *
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_assert_thread(self, xctx);
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_release");

    if (--(self->reference_count) == 0) {
        impl_afw_pool_destroy(self, xctx);
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
    impl_assert_thread(self, xctx);
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_get_reference");
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
    afw_pool_apr_self_t *parent;
    const afw_pool_t *afw_parent;
    afw_boolean_t parent_destroying;
    afw_pool_cleanup_t *e;

    if (!self || self->destroying) {
        return;
    }
    impl_assert_thread(self, xctx);
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "afw_pool_destroy");
    self->destroying = true;

    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }

    while (self->first_child) {
        impl_afw_pool_destroy(self->first_child, xctx);
    }

    parent = self->parent;
    afw_parent = self->afw_parent;
    parent_destroying = parent && parent->destroying;
    if (parent) {
        impl_unlink_child(parent, self);
    }

    /* self is allocated in apr_p; do not use self after this. */
    apr_pool_destroy(self->apr_p);

    if (parent && !parent_destroying) {
        impl_afw_pool_release(parent, xctx);
    }
    else if (afw_parent) {
        afw_pool_release(afw_parent, xctx);
    }
}


/*
 * Implementation of method get_apr_pool for interface afw_pool.
 */
apr_pool_t *
impl_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self)
{
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

    impl_assert_thread(self, xctx);
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }
    if (APR_ALIGN_DEFAULT(size) < size) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    result = apr_pcalloc(self->apr_p, size);
    if (!result) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error.", xctx);
    }
    self->bytes_allocated += size;
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

    impl_assert_thread(self, xctx);
    if (size == 0) {
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }
    if (APR_ALIGN_DEFAULT(size) < size) {
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    result = apr_palloc(self->apr_p, size);
    if (!result) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error.", xctx);
    }
    self->bytes_allocated += size;
    return result;
}


/*
 * Implementation of method free_memory_internal for interface afw_pool.
 *
 * No-op this pass. Optional reuse will not use a live-allocation prefix.
 */
void
impl_afw_pool_free_memory_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)address;
    (void)xctx;
}


/*
 * Implementation of method register_cleanup_before for interface afw_pool.
 */
void
impl_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    impl_assert_thread(self, xctx);
    e = apr_pcalloc(self->apr_p, sizeof(afw_pool_cleanup_t));
    if (!e) {
        AFW_THROW_ERROR_Z(memory, "Allocate memory error.", xctx);
    }
    e->cleanup = cleanup;
    e->data = data;
    e->data2 = data2;
    e->next_cleanup = self->first_cleanup;
    self->first_cleanup = e;
}


/*
 * Implementation of method deregister_cleanup for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;
    afw_pool_cleanup_t *prev;

    impl_assert_thread(self, xctx);
    for (prev = NULL, e = self->first_cleanup;
        e;
        prev = e, e = e->next_cleanup)
    {
        if (e->cleanup == cleanup && e->data == data && e->data2 == data2) {
            if (prev) {
                prev->next_cleanup = e->next_cleanup;
            }
            else {
                self->first_cleanup = e->next_cleanup;
            }
            return;
        }
    }
}


/* --- multithreaded wrappers ------------------------------------------- */

static const afw_pool_t *
impl_mt_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}


static void
impl_mt_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


static void
impl_mt_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


static void *
impl_mt_afw_pool_calloc(
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


static void *
impl_mt_afw_pool_malloc(
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


static void
impl_mt_afw_pool_free_memory_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)address;
    (void)xctx;
}


static void
impl_mt_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_register_cleanup_before(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


static void
impl_mt_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


#define impl_afw_pool_release \
    impl_mt_afw_pool_release
#define impl_afw_pool_get_reference \
    impl_mt_afw_pool_get_reference
#define impl_afw_pool_destroy \
    impl_mt_afw_pool_destroy
#define impl_afw_pool_calloc \
    impl_mt_afw_pool_calloc
#define impl_afw_pool_malloc \
    impl_mt_afw_pool_malloc
#define impl_afw_pool_free_memory_internal \
    impl_mt_afw_pool_free_memory_internal
#define impl_afw_pool_register_cleanup_before \
    impl_mt_afw_pool_register_cleanup_before
#define impl_afw_pool_deregister_cleanup \
    impl_mt_afw_pool_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_multithreaded_inf
#define AFW_POOL_INF_ONLY 1
#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_POOL_INF_ONLY
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory_internal
#undef impl_afw_pool_register_cleanup_before
#undef impl_afw_pool_deregister_cleanup


static afw_boolean_t
impl_is_this_impl(const afw_pool_t *p)
{
    return p &&
        (p->inf == &impl_afw_pool_inf ||
         p->inf == &impl_afw_pool_multithreaded_inf);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    AFW_POOL_SELF_T *apr_parent;
    const afw_pool_inf_t *inf;
    apr_pool_t *parent_apr;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    parent_apr = afw_pool_get_apr_pool(parent);

    if (impl_is_this_impl(parent)) {
        apr_parent = (AFW_POOL_SELF_T *)parent;
        inf = apr_parent->thread
            ? &impl_afw_pool_inf
            : &impl_afw_pool_multithreaded_inf;
        if (apr_parent->thread) {
            self = impl_create(parent_apr, inf, xctx);
            self->thread = apr_parent->thread;
            impl_add_child(apr_parent, self, xctx);
        }
        else {
            IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
                self = impl_create(parent_apr, inf, xctx);
                self->thread = apr_parent->thread;
                impl_add_child(apr_parent, self, xctx);
            }
            IMPL_MULTITHREADED_LOCK_END;
        }
    }
    else {
        /* Parent is another impl (heap). Single-threaded child. */
        inf = &impl_afw_pool_inf;
        self = impl_create(parent_apr, inf, xctx);
        self->thread = xctx->thread;
        self->afw_parent = parent;
        afw_pool_get_reference(parent, xctx);
    }

    self->pub.managed_p = parent->managed_p
        ? parent->managed_p
        : &self->pub;

    return &self->pub;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create_as_managed_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;

    p = afw_pool_create(parent, xctx);
    ((afw_pool_t *)p)->managed_p = p;
    return p;
}


AFW_DEFINE(afw_thread_t *)
afw_pool_create_thread(
    afw_size_t size,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    afw_thread_t *thread;

    if (size == (afw_size_t)-1 || size < sizeof(afw_thread_t)) {
        size = sizeof(afw_thread_t);
    }

    self = impl_create(NULL, &impl_afw_pool_inf, xctx);
    thread = apr_pcalloc(self->apr_p, size);
    self->thread = thread;
    thread->p = (const afw_pool_t *)self;
    self->pub.managed_p = &self->pub;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "afw_pool_create_thread " AFW_SIZE_T_FMT,
        size);

    return thread;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_internal_create_base_pool()
{
    apr_pool_t *apr_p;
    AFW_POOL_SELF_T *self;

    apr_pool_create(&apr_p, NULL);
    if (!apr_p) {
        return NULL;
    }
    self = apr_pcalloc(apr_p, sizeof(afw_pool_apr_self_t));
    if (!self) {
        return NULL;
    }
    self->pub.inf = &impl_afw_pool_multithreaded_inf;
    self->pub.managed_p = &self->pub;
    self->apr_p = apr_p;
    self->name = afw_s_base;
    self->pool_number = 1;
    self->reference_count = 1;
    return &self->pub;
}


AFW_DEFINE(void)
afw_pool_free_memory(
    void *address,
    afw_xctx_t *xctx)
{
    (void)address;
    (void)xctx;
}


void
afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const AFW_POOL_SELF_T *self = (const AFW_POOL_SELF_T *)pool;
    const afw_pool_apr_self_t *child;
    int i;

    (void)xctx;
    for (i = 0; i < indent; i++) {
        printf("  ");
    }
    printf(
        "pool " AFW_INTEGER_FMT " " AFW_SIZE_T_FMT " refs " AFW_INTEGER_FMT
        " parent " AFW_INTEGER_FMT "\n",
        self->pool_number,
        self->bytes_allocated,
        self->reference_count,
        self->parent ? self->parent->pool_number : (afw_integer_t)0);

    for (child = self->first_child; child; child = child->next_sibling) {
        afw_pool_print_debug_info(indent + 2, &child->pub, xctx);
    }
}

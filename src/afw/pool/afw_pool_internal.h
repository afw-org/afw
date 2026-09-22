// See the 'COPYING' file in the project root for licensing information.
/*
 * Shared pool internals.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_INTERNAL_H__
#define __AFW_POOL_INTERNAL_H__

#include "afw_interface.h"
#include <stdint.h>

/**
 * @file afw_pool_internal.h
 * @brief Shared pool internals (`afw_pool.c`).
 *
 * Implementation is split:
 * - `afw_pool.c` / this header - shared lifetime (parent/child, RC,
 *   cleanups, accounting, debug)
 * - `afw_pool_heap.c` / `afw_pool_heap_internal.h` - heap store,
 *   scope delay, heap/scope infs
 * - `afw_pool_tracker.c` / `afw_pool_tracker_internal.h` -
 *   tracker allocated list, tracker infs
 *
 * A pool is a heap unless it is a tracker. Parent/child is lifetime
 * only. Common prefix is `afw_pool_internal_self_t`.
 *
 * USER `size` is always the malloc/free_memory argument. With
 * AFW_DEBUG_POOL, [size][pool] is immediately before USER.
 */

AFW_BEGIN_DECLARES

typedef struct afw_pool_internal_inf_implementation_specific_s {
    afw_boolean_t is_multithreaded;
    afw_boolean_t is_tracker;
} afw_pool_internal_inf_implementation_specific_t;

#ifdef AFW_DEBUG_POOL
/** Immediately before USER on heap and tracker. */
typedef struct afw_pool_debug_prefix_s {
    afw_size_t size;
    const afw_pool_t *pool;
} afw_pool_debug_prefix_t;
#define AFW_POOL_DEBUG_PREFIX_BYTES sizeof(afw_pool_debug_prefix_t)
/*
 * USER fill on free. First word is a value `inf` pointer: non-canonical
 * on x86-64 so any `value->inf->…` faults, not only optional_release.
 */
#ifdef __LP64__
#define AFW_POOL_DEBUG_POISON \
    ((afw_size_t)0x0BADF00D0BADF00DULL)
#else
#define AFW_POOL_DEBUG_POISON ((afw_size_t)0x0BADF00Du)
#endif
#else
#define AFW_POOL_DEBUG_PREFIX_BYTES ((afw_size_t)0)
#endif

/**
 * Common prefix of heap and tracker self. First field of both.
 */
typedef struct afw_pool_internal_self_s
afw_pool_internal_self_t;

struct afw_pool_internal_self_s {

    afw_pool_t pub;

    /** @brief Debug id: thread->pool_number at create. */
    afw_integer_t pool_number;

    /**
     * @brief Creating / owning AFW thread, including MT and base.
     *
     * Identity and ST byte counts. MT asked-for / chunks stay off
     * thread->pool_bytes_in_use.
     */
    const afw_thread_t *thread;

    /**
     * @brief AFW parent. Listed on first_child.
     *
     * Linking does not keep the parent alive. get_reference on this
     * child does, once per hold past the create reference.
     */
    afw_pool_internal_self_t *parent;

    /** @brief First child (heap or tracker). */
    afw_pool_internal_self_t *first_child;

    /** @brief Next sibling. */
    afw_pool_internal_self_t *next_sibling;

    /** @brief First cleanup function. */
    afw_pool_cleanup_t *first_cleanup;

    /**
     * @brief Reference count.
     *
     * Starts at 1 on create; get_reference / release. The create
     * reference does not pin the parent. Each get_reference does.
     */
    afw_integer_t reference_count;

    /**
     * @brief Parent references taken by get_reference on this child.
     *
     * Dropped one at a time by release. Not used for the create reference.
     */
    afw_integer_t parent_pins;

    /** @brief Outstanding malloc/calloc (minus free/destroy). */
    afw_size_t bytes_allocated;

    /**
     * @brief This destroy is in progress.
     *
     * Children unlink without releasing this parent.
     */
    afw_boolean_t destroying;
};


#define afw_pool_internal_region(_self) \
    ((_self)->thread ? (_self)->thread->memory_region : NULL)

/*
 * MT methods lock the pool's thread region (recursive so get/free
 * inside malloc are fine). ST get/free do not lock. Uses xctx from
 * the enclosing function.
 */
#define IMPL_MULTITHREADED_LOCK_BEGIN(_pool) \
const afw_memory_region_t *_this_region = \
    ((_pool)->thread \
        ? (_pool)->thread->memory_region : NULL); \
if (_this_region) { \
    afw_memory_region_lock(_this_region, xctx); \
} \
AFW_TRY

#define IMPL_MULTITHREADED_LOCK_END \
AFW_FINALLY { \
    if (_this_region) { \
        afw_memory_region_unlock(_this_region, xctx); \
    } \
} \
AFW_ENDTRY


#ifdef AFW_DEBUG_POOL

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(_level, _info_z) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##_level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool %s thread " AFW_INTEGER_FMT \
            " pool " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " chunks " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            _info_z, \
            self->thread ? self->thread->thread_number : \
                (afw_integer_t)0, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            (afw_size_t)xctx->env->peak_pool_bytes_in_use, \
            (afw_size_t)xctx->env->pool_chunk_bytes, \
            (afw_size_t)xctx->env->peak_pool_chunk_bytes, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#define IMPL_PRINT_DEBUG_INFO_FZ(_level, _format_z, ...) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##_level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool " _format_z " thread " AFW_INTEGER_FMT \
            " pool " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " chunks " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            __VA_ARGS__, \
            self->thread ? self->thread->thread_number : \
                (afw_integer_t)0, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            (afw_size_t)xctx->env->peak_pool_bytes_in_use, \
            (afw_size_t)xctx->env->pool_chunk_bytes, \
            (afw_size_t)xctx->env->peak_pool_chunk_bytes, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

void
afw_pool_internal_debug_prefix_set(
    afw_pool_internal_self_t *self,
    void *user,
    afw_size_t size);

afw_boolean_t
afw_pool_internal_debug_prefix_ok(
    afw_pool_internal_self_t *self,
    void *address,
    afw_size_t size);

void
afw_pool_internal_debug_check_prefix(
    afw_pool_internal_self_t *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

void
afw_pool_internal_debug_poison_user(void *user, afw_size_t size);

#else

#define IMPL_PRINT_DEBUG_INFO_Z(_level, _info_z)
#define IMPL_PRINT_DEBUG_INFO_FZ(_level, _format_z, ...)
#define afw_pool_internal_debug_prefix_set(_self, _user, _size) ((void)0)
#define afw_pool_internal_debug_prefix_ok(_self, _address, _size) (true)
#define afw_pool_internal_debug_check_prefix(_self, _address, _size, _xctx) ((void)0)
#define afw_pool_internal_debug_poison_user(_user, _size) ((void)0)

#endif


void
afw_pool_internal_env_add_bytes(afw_environment_t *env, afw_size_t n);

void
afw_pool_internal_env_add_chunks(afw_environment_t *env, afw_size_t n);

void
afw_pool_internal_thread_add_bytes(const afw_thread_t *thread, afw_size_t n);

void
afw_pool_internal_thread_sub_bytes(const afw_thread_t *thread, afw_size_t n);

void
afw_pool_internal_thread_add_chunks(const afw_thread_t *thread, afw_size_t n);

void
afw_pool_internal_thread_sub_chunks(const afw_thread_t *thread, afw_size_t n);

afw_boolean_t
afw_pool_internal_is_multithreaded(const afw_pool_t *p);

afw_boolean_t
afw_pool_internal_counts_on_thread(const afw_pool_internal_self_t *self);

void
afw_pool_internal_assign_pool_number(afw_pool_internal_self_t *self);

void
afw_pool_internal_account_alloc(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx);

void
afw_pool_internal_account_free(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx);

void
afw_pool_internal_account_destroy(afw_pool_internal_self_t *self, afw_xctx_t *xctx);

void
afw_pool_internal_link_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx);

void
afw_pool_internal_unlink_from_parent(
    afw_pool_internal_self_t *self, afw_xctx_t *xctx);

void
afw_pool_internal_run_cleanups(
    afw_pool_internal_self_t *self, afw_xctx_t *xctx);

void
afw_pool_internal_mark_destroying(afw_pool_internal_self_t *self);

void
afw_pool_internal_destroy_children(
    afw_pool_internal_self_t *self, afw_xctx_t *xctx);

void
afw_pool_internal_run_child_cleanups(
    afw_pool_internal_self_t *self, afw_xctx_t *xctx);

const afw_pool_t *
afw_pool_internal_release_common(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx,
    void (*teardown)(afw_pool_internal_self_t *self, afw_xctx_t *xctx));

void
afw_pool_internal_get_reference(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx);

void
afw_pool_internal_register_cleanup(
    afw_pool_internal_self_t *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx);

void
afw_pool_internal_deregister_cleanup(
    afw_pool_internal_self_t *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx);

afw_boolean_t
afw_pool_internal_is_heap(const afw_pool_t *p);

afw_boolean_t
afw_pool_internal_is_tracker(const afw_pool_t *p);

void
afw_pool_internal_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);

/**
 * Allocate without throwing. Env-create / xctx-init window only.
 * xctx may be NULL.
 */
void *
afw_pool_internal_malloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx);

void *
afw_pool_internal_calloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx);

#define afw_pool_internal_calloc_type_unhandled(_instance, _type, _xctx) \
    (_type *) afw_pool_internal_calloc_unhandled(_instance, sizeof(_type), _xctx)

AFW_END_DECLARES

#endif /* __AFW_POOL_INTERNAL_H__ */

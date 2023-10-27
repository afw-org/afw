// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool internal header.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_POOL_INTERNAL_H__
#define __AFW_POOL_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_pool_internal Pool
 * @ingroup afw_c_api_internal
 *
 * Pool internal API
 *
 * @{
 */

/**
 * @file afw_pool.h
 * @brief Adaptive Framework memory pool internal header.
 */

AFW_BEGIN_DECLARES


typedef struct afw_pool_internal_memory_block_s
afw_pool_internal_memory_block_t;

/*
 * Memory block.
 *
 * This is used to keep track of memory blocks in a pool. The blocks are kept
 * in two singly linked list, one for free blocks and one for allocated. The
 * size in the block does not include the size of the struct itself. The size
 * is either the size of the free memory block or the size of the allocated
 * memory block, depending on the list the block is in.
 */
struct afw_pool_internal_memory_block_s {
    struct afw_pool_internal_memory_block_t *next;
    afw_size_t size;
};


typedef struct afw_pool_internal_self_s
afw_pool_internal_self_t;

struct afw_pool_internal_self_s {

    afw_pool_t pub;

    /*NOTE: Keep pool_number just after pub for debugging. */
    /** @brief Unique number for pool. */
    afw_integer_t pool_number;

    /** @brief Associated apr pool or NULL if it has not been created. */
    apr_pool_t *apr_p;

    /** @brief Optional pool name. */
    const afw_utf8_t *name;

    /** @brief Parent pool of this pool. */
    afw_pool_internal_self_t *parent;

    /* @brief First subpool of this pool. */
    afw_pool_internal_self_t * AFW_ATOMIC first_child;

    /* @brief Next sibling of this pool. */
    afw_pool_internal_self_t * AFW_ATOMIC next_sibling;

    /** @brief First cleanup function. */
    afw_pool_cleanup_t *first_cleanup;

    /* First free block. */
    afw_pool_internal_memory_block_t *first_free_block;

    /* First allocated block. */
    afw_pool_internal_memory_block_t *first_allocated_block;

    /**
     * @brief Pools reference count.
     *
     * This starts at 1 on create and is incremented and decremented
     * by afw_pool_get_reference() and afw_pool_release().
     */
    afw_integer_t reference_count;

    /** @brief Bytes allocated via afw_pool_malloc()/afw_pool_calloc(). */
    afw_size_t bytes_allocated;

    /**
     * @brief Thread associated with a thread specific pool.
     *
     * If this is not NULL, this pool is thread specific and can only be
     * accessed by this thread.
     */
    const afw_thread_t *thread;
};


AFW_DECLARE_INTERNAL(void)
afw_pool_internal_add_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child, afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(void)
afw_pool_internal_remove_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx);


/* Create skeleton pool struct. */
AFW_DECLARE_INTERNAL(afw_pool_internal_self_t *)
afw_pool_internal_create(
    afw_pool_internal_self_t *parent,
    const afw_pool_inf_t *inf,
    afw_size_t instance_size,
    afw_xctx_t *xctx);
 

/**
 * @internal
 * @brief Allocate base pool.
 * @return Pointer to base pool or NULL if there is an error.
 *
 * This should only be used in afw_environment_create().
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_internal_create_base_pool();


/**
 * @internal
 * @brief Create thread struct in new thread specific pool with p  set.
 * @param size of thread struct or -1 if sizeof(afw_thread_t) should be used.
 * @param xctx of caller.
 * @return new thread struct with p set.
 *
 * This should only be called from afw_thread_create().
 */
AFW_DECLARE(afw_thread_t *)
afw_pool_internal_create_thread(
    afw_size_t size,
    afw_xctx_t *xctx);


void afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);



#ifdef AFW_POOL_DEBUG

#define AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(level,info_z)
#define AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(level,format_z,...)

#else

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(level,info_z) \
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
            " " AFW_IMPLEMENTATION_ID " " \
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

#define AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
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
            " " AFW_IMPLEMENTATION_ID " " \
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

AFW_END_DECLARES

/** @} */

#endif /* __AFW_POOL_INTERNAL_H__ */

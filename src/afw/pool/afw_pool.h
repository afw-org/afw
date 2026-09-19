// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool support header.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_H__
#define __AFW_POOL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_pool
 * @{
 */

/**
 * @file afw_pool.h
 * @brief Adaptive Framework memory pool support header.
 *
 * See the @ref afw_pool group (defined in afw_doxygen.h) for the mental model.
 *
 * Key invariants:
 * - A pool is a heap unless it is a tracker. A tracker gets memory
 *   from a heap and tracks it. Destroy returns the chain to the
 *   ancestor heap. free_memory on a tracker marks; it does not
 *   return the block until destroy or garbage_collect.
 *   Multithreaded heap is lock wrappers. The heap owns posix_memalign
 *   chunks (4k-aligned, 64k minimum). Not a third AFW pool kind.
 * - Parent/child is lifetime only (last-release throws if children
 *   remain). Store is the ancestor heap. Trackers may parent other
 *   trackers.
 * - One ST job heap per xctx (`afw_pool_heap_create_as_managed_p`).
 *   Evaluation `{ }` uses `afw_pool_scope_create` (ST heap, 4k
 *   chunks, inherits managed_p, last-release delay while a script
 *   throw is handled).
 *   Closures pin the inner scope; the xctx heap outlives the outer
 *   `{ }`.
 * - `afw_pool_create()` of a ST parent (xctx->p or tracker) is a
 *   tracker (not a scope). Of an MT parent, an MT heap that
 *   **inherits** managed_p. `env->p` is the process MT job heap.
 *   Things you start (conf, server, log, adapter) use
 *   `afw_pool_multithread_create_as_managed_p(env->p)`. Compile
 *   units use `afw_pool_heap_create` (own chunks, inherit
 *   managed_p; optional smaller chunk_min).
 * - Job heaps set `managed_p = self` (`*_as_managed_p` and the
 *   base/env pool). Unmarked heap/MT create and scopes inherit.
 *   Trackers inherit. Managed values allocate in `p->managed_p`.
 *   Do not change managed_p mid-eval. Request xctx:
 *   `xctx->p->managed_p` is `xctx->p`.
 * - Two numbers: asked-for (`bytes_allocated` /
 *   `env->pool_bytes_in_use`) vs chunks (`chunk_bytes` /
 *   `env->pool_chunk_bytes`). Env also keeps high-water
 *   `peak_pool_bytes_in_use` / `peak_pool_chunk_bytes`. Adaptive
 *   `pool_bytes_in_use()` vs `process_rss()`. This xctx:
 *   `afw_pool_subtree_*` on `xctx->p`.
 * - Last-release: decrement; if 0 and children remain, throw; else
 *   callbacks, unchain, free this store, release parent. Does not
 *   call destroy. "Children remaining" is a leaked child.
 * - destroy: storage-only (must not fail). Call `run_cleanups`
 *   first if callbacks must run (`xctx_release` does both).
 * - `afw_pool_release_delayed()`: postorder last-release delayed
 *   pools. ENDTRY after a caught error.
 * - `env->p` is process lifetime (valgrind still reachable is
 *   intended).
 *
 * Debug:
 * - Build `--cdev` / `--fulldev` defines `AFW_DEBUG_POOL`. Prefix
 *   {pool,size} on free; poison `0x0BADF00D` so a dangling inf
 *   faults. Runtime `debug:pool` / `debug:pool:detail` on a short
 *   run, not a soak. gdb `afw_pool_print_debug_info(0, xctx->p,
 *   xctx)`.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Round a byte size up to the heap posix_memalign page.
 * @param size bytes. 0 is unchanged (unlimited or "use default").
 * @return 0 if size is 0; otherwise at least one page, multiple of
 *    the page.
 *
 * Use this when storing env chunk_min / compile_chunk_min /
 * xctx_chunk_min and byte limits. Heap create then does not redo
 * the round.
 */
AFW_DECLARE(afw_size_t)
afw_pool_round_up_chunk_size(afw_size_t size);

/** @brief Typedef for registered cleanup functions. */
typedef struct afw_pool_cleanup_s afw_pool_cleanup_t;


/** @brief Struct for registered cleanup functions. */
struct afw_pool_cleanup_s {

    /* *** next_cleanup MUST BE FIRST *** */
    /** @brief Next cleanup function. */
    afw_pool_cleanup_t *next_cleanup;

    /** @brief Cleanup function. */
    afw_pool_cleanup_function_p_t cleanup;

    /** @brief Parameter data passed to function. */
    void *data;

    /** @brief Parameter data2 passed to function. */
    void *data2;
};


/**
 * @brief Create a new pool.
 * @param parent of new pool.
 * @param xctx of caller.
 * @return new pool.
 *
 * Tracker if the parent is a single-thread heap or a tracker.
 * Multithreaded heap if the parent is multithreaded (inherits
 * managed_p). Use multithread_create_as_managed_p for a job heap.
 *
 * env->p is a multithreaded job heap. xctx->p is a single-thread
 * job heap (`afw_pool_heap_create_as_managed_p`). Thread-specific
 * heaps are not safe from another thread.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a single-thread heap that inherits managed_p.
 * @param parent of new pool (may be multithreaded env/base).
 * @param chunk_min minimum posix_memalign size; 0 = env->chunk_min.
 * @param xctx of caller.
 * @return new pool.
 *
 * Own chunks. Compile units and other bulk-free children use this.
 * afw_pool_create() of the result is a tracker.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_heap_create(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);


/**
 * @brief Create a single-thread job heap (managed_p = self).
 * @param parent of new pool (may be multithreaded env/base).
 * @param chunk_min minimum posix_memalign size; 0 = env->chunk_min.
 * @param xctx of caller.
 * @return new pool.
 *
 * xctx->p and thread heaps use this.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_heap_create_as_managed_p(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);


/**
 * @brief Create a multithreaded heap that inherits managed_p.
 * @param parent must be a multithreaded heap (usually env->p).
 * @param xctx of caller.
 * @return new pool.
 *
 * Child MT store, not a new job heap.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_multithread_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a multithreaded job heap (managed_p = self).
 * @param parent must be a multithreaded heap (usually env->p).
 * @param xctx of caller.
 * @return new pool.
 *
 * For things you start: conf, server, log, adapter. Work from a
 * request xctx on a pool that outlives that request.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_multithread_create_as_managed_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a tracker.
 * @param parent heap or tracker.
 * @param xctx of caller.
 * @return tracker. managed_p is the ancestor heap.
 *
 * Single-thread only, same thread as the parent. No last-release
 * delay on throw. For evaluation `{ }`, use afw_pool_scope_create().
 * The tracker header is a parent-pool block (`free_memory` on
 * destroy).
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_tracker_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a scope pool (evaluation `{ }`).
 * @param parent heap or tracker (or another scope).
 * @param xctx of caller.
 * @return ST heap, compile-sized (4k) chunks, inherits managed_p.
 *
 * Last-release is delayed while error_processing_count > 0 so CATCH
 * can still use values from this `{ }`. ENDTRY calls
 * afw_pool_release_delayed().
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_scope_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Outstanding malloc/calloc on this pool (asked-for).
 *
 * Trackers and heaps each count their own allocs. Not chunk RSS.
 */
AFW_DECLARE(afw_size_t)
afw_pool_bytes_allocated(const afw_pool_t *instance);


/**
 * @brief posix_memalign bytes still held (heap only; 0 on a tracker).
 */
AFW_DECLARE(afw_size_t)
afw_pool_chunk_bytes(const afw_pool_t *instance);


/**
 * @brief Number of chunks (heap only; 0 on a tracker).
 */
AFW_DECLARE(afw_size_t)
afw_pool_chunk_count(const afw_pool_t *instance);


/**
 * @brief Asked-for bytes for this pool and descendants.
 *
 * Single-thread trees only (xctx->p). For process total use
 * env->pool_bytes_in_use.
 */
AFW_DECLARE(afw_size_t)
afw_pool_subtree_bytes_allocated(const afw_pool_t *instance);


/**
 * @brief Chunk bytes for this pool and descendant heaps.
 *
 * Single-thread trees only (xctx->p). For process total use
 * env->pool_chunk_bytes.
 */
AFW_DECLARE(afw_size_t)
afw_pool_subtree_chunk_bytes(const afw_pool_t *instance);


/**
 * @brief Create thread struct in new thread specific pool with p set.
 * @param size of thread struct or -1 if sizeof(afw_thread_t) should be used.
 * @param xctx of caller.
 * @return new thread struct with p set.
 *
 * Internal. Only called from afw_thread_create(). Does not start a
 * pthread; it creates the thread-specific pool and the thread struct
 * in that pool.
 */
AFW_DECLARE(afw_thread_t *)
afw_pool_thread_create(
    afw_size_t size,
    afw_xctx_t *xctx);


/**
 * @brief Macro to allocate cleared memory to hold type in pool.
 * @param instance of pool.
 * @param type to allocate.
 * @param xctx of caller.
 * @return pointer to memory allocated cast to pointer of type.
 *
 * This is a helper macro to call afw_pool_calloc() to allocate
 * memory for a specified type and cast the return pointer to
 * a pointer to that type.
 */
#define afw_pool_calloc_type(instance, type, xctx) \
    (type *) afw_pool_calloc(instance, sizeof(type), xctx)


/**
 * @brief Macro to allocate uncleared memory to hold type in pool.
 * @param instance of pool.
 * @param type to allocate.
 * @param xctx of caller.
 * @return pointer to memory allocated cast to pointer of type.
 *
 * This is a helper macro to call afw_pool_malloc() to allocate
 * memory for a specified type and cast the return pointer to a pointer
 * to that type.
 */
#define afw_pool_malloc_type(instance, type, xctx) \
    (type *) afw_pool_malloc(instance, sizeof(type), xctx)


/**
 * @brief Optionally free a typed allocation from a pool.
 * @param instance of pool.
 * @param address returned by afw_pool_malloc/calloc.
 * @param type allocated.
 * @param xctx of caller.
 */
#define afw_pool_free_memory_type(instance, address, type, xctx) \
    afw_pool_free_memory(instance, address, sizeof(type), xctx)


/**
 * @brief Release a value when a pool is destroyed.
 * @param value to release at cleanup. NULL is a no-op.
 * @param p pool whose destroy runs the release.
 * @param xctx of caller.
 *
 * Registers `afw_pool_register_cleanup()` so
 * `afw_value_release()` runs at last-release or run_cleanups.
 * Does not add a reference; the caller already holds `value`
 * (or otherwise owns a matching release).
 *
 * No-op if `value` has no optional_release (the callback would do
 * nothing). A second register of the same value on this same `p` is
 * a no-op (already handled).
 *
 * Managed values (RC, including closures) may be registered on any
 * p: they stay alive while referenced. The callback drops that hold
 * when p last-releases or run_cleanups.
 */
AFW_DECLARE(void)
afw_pool_release_value_at_cleanup(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief True if this value already has a cleanup release on p.
 */
AFW_DECLARE(afw_boolean_t)
afw_pool_is_value_release_registered(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_POOL_H__ */

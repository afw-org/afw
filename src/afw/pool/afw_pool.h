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
 *   chunks (4k-aligned; default floor 64k when chunk_min is 0).
 *   Not a third AFW pool kind.
 * - Parent/child is lifetime only. Store is the ancestor heap.
 *   Trackers may parent other trackers. A heap or tracker pins its
 *   parent only while an extra hold is outstanding. The create
 *   reference does not. A thread pool (afw_pool_thread_create)
 *   uses impl_afw_pool_thread_inf: one parent hold from
 *   create until teardown, and get_reference does not pin that
 *   parent. At 0, destroy
 *   children that never pinned this pool, then free this store.
 *   A referenced child still linked is an error.
 * - One ST job heap per xctx (thread handoff off env->p, including
 *   base). A `{ }` frame is the scope pool (`afw_pool_scope_create`;
 *   top parent is the evaluate dest, nested parent is the parent
 *   scope pool). A scope holds its parent from create until
 *   teardown, and its pool count stays at 1 so throw-path delay
 *   still sees a last release. Blank pool, no block:
 *   `afw_pool_scope_allocate`. Heap/scope/tracker follow parent
 *   ST/MT. Closures hold the inner scope; the xctx heap outlives
 *   the outer `{ }`.
 * - `afw_pool_create()` is a heap like the parent (ST or MT lock
 *   wrappers), **inherits** managed_p. Tracker is
 *   `afw_pool_tracker_create()`. `env->p` is the process MT job heap.
 *   Things you start (conf, server, log, adapter) use
 *   `afw_pool_multithread_create_as_managed_p(env->p, chunk_min)`.
 *   0 is env->default_chunk_min (64k). Those callers pass
 *   small_chunk_min (4k). Compile units use
 *   `afw_pool_heap_create` (own chunks, inherit managed_p;
 *   optional smaller chunk_min).
 * - `managed_p` is a pool property. Unmarked heap/MT create,
 *   trackers, and `scope_create` **inherit** `parent->managed_p`.
 *   `*_as_managed_p` sets `managed_p = self` (this pool is a
 *   managed dest). Managed create/clone/`get_assignable` use
 *   dest `p->managed_p`. Do not change `managed_p` mid-eval.
 *   Request xctx: `xctx->p->managed_p` is `xctx->p`.
 * - Two numbers: asked-for (`bytes_allocated` /
 *   `env->pool_bytes_in_use`) vs chunks (`chunk_bytes` /
 *   `env->pool_chunk_bytes`). Env also keeps high-water
 *   `peak_pool_bytes_in_use` / `peak_pool_chunk_bytes`. Adaptive
 *   `pool_bytes_in_use()` vs `process_rss()`. This xctx:
 *   `afw_pool_subtree_*` on `xctx->p`.
 * - destroy: storage-only (must not fail). Call `run_cleanups`
 *   first if callbacks must run (`xctx_release` does both).
 * - `afw_pool_heap_internal_release_delayed()`: last-release scopes delayed
 *   while error_processing_count > 0. ENDTRY after a caught error.
 * - `env->p` is process lifetime (valgrind still reachable is
 *   intended).
 *
 * Debug:
 * - Build `--cdev` / `--fulldev` defines `AFW_DEBUG_POOL`. Prefix
 *   {pool,size} on free; poison `0x0BADF00D` so a dangling inf
 *   faults. Runtime `debug:pool` / `debug:pool:detail` on a short
 *   run, not a soak. gdb `afw_pool_internal_print_debug_info(0, xctx->p,
 *   xctx)`.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Round a byte size up to the heap posix_memalign page.
 * @param size bytes. 0 is unchanged (unlimited or "use default").
 * @return 0 if size is 0; otherwise at least one page, multiple of
 *    the page.
 *
 * Use this when storing env chunk_min / small_chunk_min /
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
 * Heap like the parent: ST heap or MT heap (lock wrappers).
 * Inherits managed_p. Use afw_pool_tracker_create() for a tracker.
 * Use *_as_managed_p for a job heap.
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
 * @param chunk_min minimum posix_memalign size; 0 = env->default_chunk_min.
 * @param xctx of caller.
 * @return new pool.
 *
 * Own chunks. Compile units and other bulk-free children use this.
 * ST/MT follows the parent.
 *
 * Inherits parent->managed_p, so create_managed(this) still
 * allocates on the ancestor dest. Use heap_create_as_managed_p
 * only when this pool's lifetime is the managed dest.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_heap_create(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);


/**
 * @brief Create a single-thread heap with managed_p = self.
 * @param parent of new pool (may be multithreaded env/base).
 * @param chunk_min minimum posix_memalign size; 0 = env->default_chunk_min.
 * @param xctx of caller.
 * @return new pool.
 *
 * This pool is a managed dest: create_managed(p) with this p (or a
 * child that inherited) allocates here; last-release of this pool
 * free_memorys those headers.
 *
 * Use when the pool outlives the work that holds managed values:
 * xctx->p, a thread heap. Do not use for a compile unit or `{ }`
 * (those inherit so managed values survive the child).
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_heap_create_as_managed_p(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);


/**
 * @brief Create a multithreaded heap that inherits managed_p.
 * @param parent must be a multithreaded heap (usually env->p).
 * @param chunk_min minimum posix_memalign size; 0 = env->default_chunk_min.
 * @param xctx of caller.
 * @return new pool.
 *
 * Child MT store. Inherits parent->managed_p. Use
 * multithread_create_as_managed_p when this pool is the dest.
 * 0 is the process floor (64k), same as heap_create. Pass
 * small_chunk_min (4k) for a small child.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_multithread_create(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);


/**
 * @brief Create a multithreaded heap with managed_p = self.
 * @param parent must be a multithreaded heap (usually env->p).
 * @param chunk_min minimum posix_memalign size; 0 = env->default_chunk_min.
 * @param xctx of caller.
 * @return new pool.
 *
 * This pool is a managed dest (same rule as
 * heap_create_as_managed_p). For things you start that outlive a
 * request: conf, server, log, adapter. Their `p` is this pool.
 * Those callers pass small_chunk_min (4k). 0 is the process
 * floor (64k). A single allocation larger than the floor still
 * gets a chunk that fits.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_multithread_create_as_managed_p(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);


/**
 * @brief Create a tracker.
 * @param parent heap or tracker.
 * @param xctx of caller.
 * @return tracker. managed_p is the ancestor heap.
 *
 * If the parent is multithreaded, methods are lock wrappers on
 * the same region mutex as MT heaps. No last-release delay on
 * throw. For evaluation `{ }`, use afw_pool_scope_create().
 * The tracker header is a parent-pool block (`free_memory` on
 * destroy).
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_tracker_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Allocate a scope pool with no block.
 * @param parent heap or tracker (or another scope).
 * @param xctx of caller.
 * @return The scope pool (`afw_pool_scope_t` is that pool).
 *
 * A `{ }` frame is afw_pool_scope_create(). This entry is the pool
 * alone: no symbols, reference count 0. Last-release is delayed
 * while error_processing_count > 0. Inherits managed_p.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_scope_allocate(
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
#define afw_pool_calloc_type(_instance, _type, _xctx) \
    (_type *) afw_pool_calloc(_instance, sizeof(_type), _xctx)


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
#define afw_pool_malloc_type(_instance, _type, _xctx) \
    (_type *) afw_pool_malloc(_instance, sizeof(_type), _xctx)


/**
 * @brief Optionally free a typed allocation from a pool.
 * @param instance of pool.
 * @param address returned by afw_pool_malloc/calloc.
 * @param type allocated.
 * @param xctx of caller.
 */
#define afw_pool_free_memory_type(_instance, _address, _type, _xctx) \
    afw_pool_free_memory(_instance, _address, sizeof(_type), _xctx)


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
 * when p last-releases or run_cleanups. Evaluate of a compiled
 * script/template registers the result here on dest p.
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

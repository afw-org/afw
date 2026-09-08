// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework vector (growable contiguous elements)
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#ifndef __AFW_VECTOR_H__
#define __AFW_VECTOR_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_vector
 * @{
 */

/**
 * @file afw_vector.h
 * @brief Growable contiguous elements; typesafe overlay on one impl.
 *
 * See @ref afw_vector.
 *
 * C replacement for apr_array: header stays put, entries points at
 * the current chunk, growth copies to a larger chunk. Callers hold
 * the header. Pointers into entries are invalid after grow.
 *
 * Declare a typed overlay, then use shared macros. Element type lives
 * on the variable, not restated at each push.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Untyped vector header.
 *
 * Layout must match the typed overlay from AFW_VECTOR_STRUCT.
 * Call sites use the typed fields. Impl functions take
 * const afw_vector_t * (like an interface instance) and cast
 * to non-const internally.
 */
struct afw_vector_s {
    const afw_pool_t *p;
    afw_size_t entry_size;
    afw_size_t count;
    afw_size_t allocated;
    void *entries;
};

typedef struct afw_vector_s afw_vector_t;

/**
 * @brief Declare a vector struct with a typed entries pointer.
 * @param struct_name of the struct.
 * @param entry_type element type (entries is entry_type *).
 *
 * Fields must match afw_vector_s, with entries typed. internal is
 * the untyped impl view (const, like an interface instance).
 * Writes go through the typed fields or the impl cast.
 */
#define AFW_VECTOR_STRUCT(struct_name, entry_type) \
struct struct_name { \
    union { \
        const afw_vector_t internal; \
        struct { \
            const afw_pool_t *p; \
            afw_size_t entry_size; \
            afw_size_t count; \
            afw_size_t allocated; \
            entry_type *entries; \
        }; \
    }; \
}

/**
 * @brief Create a vector for the specified typedef.
 * @param typedef_name from AFW_VECTOR_STRUCT plus typedef.
 * @param initial_allocated capacity (count starts at 0). 0 is ok.
 * @param p pool for header and element storage.
 * @param xctx of caller.
 * @return pointer of type typedef_name *.
 *
 * Entry size is sizeof(*entries) of the typedef.
 */
#define afw_vector_create(typedef_name, \
    initial_allocated, p, xctx) \
((typedef_name *)afw_vector_create_impl( \
    sizeof(*(((typedef_name *)0)->entries)), \
    initial_allocated, p, xctx))

/**
 * @brief Create an untyped vector.
 * @param entry_size size of one entry. Must be > 0.
 * @param initial_allocated capacity. Count starts at 0.
 * @param p pool for header and element storage.
 * @param xctx of caller.
 * @return vector header.
 *
 * Normally called by afw_vector_create().
 */
AFW_DECLARE(afw_vector_t *)
afw_vector_create_impl(
    afw_size_t entry_size,
    afw_size_t initial_allocated,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Grow so allocated is at least min_allocated.
 * @param internal untyped header.
 * @param min_allocated capacity to guarantee.
 * @param xctx of caller.
 *
 * No-op if already large enough. Does not change count.
 */
AFW_DECLARE(void)
afw_vector_ensure_impl(
    const afw_vector_t *internal,
    afw_size_t min_allocated,
    afw_xctx_t *xctx);

/**
 * @brief Grow capacity (at least double).
 * @param internal untyped header.
 * @param xctx of caller.
 *
 * Called when count == allocated.
 */
AFW_DECLARE(void)
afw_vector_extend_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx);

/**
 * @brief Append one slot. Return its index.
 * @param internal untyped header.
 * @param xctx of caller.
 * @return index of the new uninitialized slot.
 */
AFW_DECLARE(afw_size_t)
afw_vector_push_index_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx);

/**
 * @brief Remove the last slot. Return its index.
 * @param internal untyped header.
 * @param xctx of caller.
 * @return index of the popped slot (still in the buffer).
 *
 * Throws on empty.
 */
AFW_DECLARE(afw_size_t)
afw_vector_pop_index_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx);

/**
 * @brief Make a hole at index and increment count.
 * @param internal untyped header.
 * @param index 0 .. count (count appends).
 * @param xctx of caller.
 * @return index (same as passed).
 *
 * Existing elements at and after index move toward the end.
 * Caller fills entries[index]. Throws if index > count.
 */
AFW_DECLARE(afw_size_t)
afw_vector_insert_impl(
    const afw_vector_t *internal,
    afw_size_t index,
    afw_xctx_t *xctx);

/**
 * @brief Remove the element at index and close the gap.
 * @param internal untyped header.
 * @param index 0 .. count-1.
 * @param xctx of caller.
 *
 * Following elements move toward the front. Throws if
 * index >= count. Copy entries[index] first if the value is
 * needed (shift is copy then remove 0).
 */
AFW_DECLARE(void)
afw_vector_remove_impl(
    const afw_vector_t *internal,
    afw_size_t index,
    afw_xctx_t *xctx);

/**
 * @brief Copy used elements into a new vector in p.
 * @param internal untyped header.
 * @param p pool for the copy.
 * @param xctx of caller.
 * @return new vector with the same entry_size and count.
 */
AFW_DECLARE(afw_vector_t *)
afw_vector_copy_impl(
    const afw_vector_t *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Copy used entries to an exact-sized block in p.
 * @param internal untyped header.
 * @param count place for used count, or NULL.
 * @param ptr place for the block pointer, or NULL.
 * @param p pool for the copy.
 * @param xctx of caller.
 *
 * Allocates count * entry_size (or sets *ptr NULL when count is 0).
 * Does not release the work vector.
 */
AFW_DECLARE(void)
afw_vector_copy_entries_impl(
    const afw_vector_t *internal,
    afw_size_t *count,
    void **ptr,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Free the work vector header and entries chunk.
 * @param internal untyped header.
 * @param xctx of caller.
 *
 * No get_reference. Do not use the vector after this.
 */
AFW_DECLARE(void)
afw_vector_release_impl(
    const afw_vector_t *internal,
    afw_xctx_t *xctx);

/**
 * @brief Copy into a typed vector.
 */
#define afw_vector_copy(typedef_name, instance, p, xctx) \
((typedef_name *)afw_vector_copy_impl( \
    &((instance)->internal), (p), (xctx)))

/**
 * @brief Copy used entries to an exact-sized typed block.
 *
 *     afw_vector_copy_entries(v, &n, &out, p, xctx);
 *
 * out is entry_type *.
 */
#define afw_vector_copy_entries(instance, count, ptr, p, xctx) \
    afw_vector_copy_entries_impl(&((instance)->internal), \
        (count), (void **)(ptr), (p), (xctx))

/**
 * @brief Free header and entries chunk. Do not use after.
 */
#define afw_vector_release(instance, xctx) \
    afw_vector_release_impl(&((instance)->internal), xctx)

/**
 * @brief Copy used entries out, then release the work vector.
 */
#define afw_vector_copy_entries_and_release( \
    instance, count, ptr, p, xctx) \
    afw_vector_copy_entries((instance), (count), (ptr), \
        (p), (xctx)); \
    afw_vector_release((instance), (xctx))

/**
 * @brief Ensure capacity on a typed vector. Count unchanged.
 */
#define afw_vector_ensure(instance, min_allocated, xctx) \
    afw_vector_ensure_impl(&((instance)->internal), \
        (min_allocated), (xctx))

/**
 * @brief Append one uninitialized slot. Lvalue of the entry type.
 *
 * instance->entries may move. Do not hold interior pointers
 * across this call. Use as a statement, like afw_stack_push:
 *
 *     afw_vector_push(v, xctx) = value;
 *
 * The impl call is a full statement so grow finishes before
 * entries is used.
 */
#define afw_vector_push(instance, xctx) \
    afw_vector_push_index_impl( \
        &((instance)->internal), xctx); \
    (instance)->entries[(instance)->count - 1]

/**
 * @brief Last entry. Caller must not use on empty.
 */
#define afw_vector_last(instance) \
    ((instance)->entries[(instance)->count - 1])

/**
 * @brief Remove the last element. Throws on empty.
 *
 * Does not yield the value. Peek first:
 *
 *     value = afw_vector_last(v);
 *     afw_vector_pop(v, xctx);
 *
 * The popped slot remains in the buffer until overwritten.
 */
#define afw_vector_pop(instance, xctx) \
    afw_vector_pop_index_impl(&((instance)->internal), xctx)

/**
 * @brief Insert a hole at index. Lvalue of the new slot.
 *
 * index equal to count appends. index must not have side
 * effects (evaluated more than once). Use as a statement:
 *
 *     afw_vector_insert(v, i, xctx) = value;
 */
#define afw_vector_insert(instance, index, xctx) \
    afw_vector_insert_impl( \
        &((instance)->internal), (index), xctx); \
    (instance)->entries[index]

/**
 * @brief Remove the element at index (memmove to close).
 */
#define afw_vector_remove(instance, index, xctx) \
    afw_vector_remove_impl(&((instance)->internal), \
        (index), xctx)

/**
 * @brief Set count to 0. Capacity is kept.
 */
#define afw_vector_clear(instance) \
    ((instance)->count = 0)

AFW_END_DECLARES

/** @} */

#endif /* __AFW_VECTOR_H__ */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework hash table (key → pointer)
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#ifndef __AFW_HASH_TABLE_H__
#define __AFW_HASH_TABLE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_hash_table
 * @{
 */

/**
 * @file afw_hash_table.h
 * @brief Byte-key to pointer map; typesafe overlay on one impl.
 *
 * See @ref afw_hash_table.
 *
 * Header stays put; buckets point at the current array. Growth
 * copies chains to a larger array. Callers hold the header.
 * Keys are not copied; the caller keeps key memory alive. Set
 * value NULL deletes. No get_reference.
 *
 * Declare a typed overlay, then use shared macros. Value type
 * lives on the variable. Get returns void * (assign to a typed
 * pointer).
 */

AFW_BEGIN_DECLARES

/**
 * @brief Untyped hash table header.
 *
 * Layout must match the typed overlay from AFW_HASH_TABLE_STRUCT.
 * Call sites use the typed overlay. Impl functions take
 * const afw_hash_table_t * (like an interface instance) and cast
 * to non-const internally.
 */
struct afw_hash_table_s {
    const afw_pool_t *p;
    afw_size_t count;
    afw_size_t allocated;
    void *buckets;
    void *free_entries;
};

typedef struct afw_hash_table_s afw_hash_table_t;

/**
 * @brief Stack iterator. Do not hold across grow.
 *
 * first() positions at the first entry (or end). this() fills
 * key/klen/value and returns false at end. next() advances.
 * Deleting the current entry (set value NULL) is safe; the
 * iterator prefetches next.
 */
struct afw_hash_table_index_s {
    const afw_hash_table_t *ht;
    afw_size_t bucket;
    void *entry;
    void *next;
    afw_size_t next_bucket;
};

typedef struct afw_hash_table_index_s afw_hash_table_index_t;

/**
 * @brief Declare a hash table struct (overlay on afw_hash_table_s).
 * @param struct_name of the struct.
 * @param value_type stored value type (documentation / typed fields).
 *
 * Fields must match afw_hash_table_s. internal is the untyped
 * impl view (const, like an interface instance).
 */
#define AFW_HASH_TABLE_STRUCT(struct_name, value_type) \
struct struct_name { \
    union { \
        const afw_hash_table_t internal; \
        struct { \
            const afw_pool_t *p; \
            afw_size_t count; \
            afw_size_t allocated; \
            void *buckets; \
            void *free_entries; \
        }; \
    }; \
}

/**
 * @brief Hash table of untyped pointers (usual drop-in).
 */
AFW_HASH_TABLE_STRUCT(afw_void_hash_table_s, void *);
typedef struct afw_void_hash_table_s afw_void_hash_table_t;

/**
 * @brief Create a hash table for the specified typedef.
 * @param typedef_name from AFW_HASH_TABLE_STRUCT plus typedef.
 * @param p pool for header, buckets, and entries.
 * @param xctx of caller.
 * @return pointer of type typedef_name *.
 *
 * Starts with 16 buckets; doubles when load is high.
 */
#define afw_hash_table_create(typedef_name, p, xctx) \
((typedef_name *)afw_hash_table_create_impl((p), (xctx)))

/**
 * @brief Create an untyped hash table.
 * @param p pool for header, buckets, and entries.
 * @param xctx of caller.
 * @return hash table header.
 *
 * Normally called by afw_hash_table_create().
 */
AFW_DECLARE(afw_hash_table_t *)
afw_hash_table_create_impl(
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Get the value for key, or NULL.
 * @param internal untyped header.
 * @param key bytes to match.
 * @param klen length of key in bytes.
 * @return stored pointer, or NULL if absent.
 */
AFW_DECLARE(void *)
afw_hash_table_get_impl(
    const afw_hash_table_t *internal,
    const void *key,
    afw_size_t klen);

/**
 * @brief Set or delete the value for key.
 * @param internal untyped header.
 * @param key bytes. Stored as a pointer; caller keeps it alive.
 * @param klen length of key in bytes.
 * @param value pointer to store, or NULL to delete.
 * @param xctx of caller.
 *
 * Replacing an existing key keeps the original stored key
 * pointer. A new key stores this key pointer.
 */
AFW_DECLARE(void)
afw_hash_table_set_impl(
    const afw_hash_table_t *internal,
    const void *key,
    afw_size_t klen,
    const void *value,
    afw_xctx_t *xctx);

/**
 * @brief Position index at the first entry (or end).
 * @param internal untyped header.
 * @param index caller-owned iterator.
 */
AFW_DECLARE(void)
afw_hash_table_first_impl(
    const afw_hash_table_t *internal,
    afw_hash_table_index_t *index);

/**
 * @brief Advance index to the next entry (or end).
 * @param index iterator from first().
 */
AFW_DECLARE(void)
afw_hash_table_next_impl(
    afw_hash_table_index_t *index);

/**
 * @brief Fill key/klen/value from the current index entry.
 * @param index iterator from first()/next().
 * @param key place for key pointer, or NULL.
 * @param klen place for key length, or NULL.
 * @param value place for value pointer, or NULL.
 * @return true if index is on an entry; false at end.
 */
AFW_DECLARE(afw_boolean_t)
afw_hash_table_this_impl(
    const afw_hash_table_index_t *index,
    const void **key,
    afw_size_t *klen,
    void **value);

/**
 * @brief Recycle all entries. Capacity is kept.
 * @param internal untyped header.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_hash_table_clear_impl(
    const afw_hash_table_t *internal,
    afw_xctx_t *xctx);

/**
 * @brief Free the work header, buckets, and entries.
 * @param internal untyped header.
 * @param xctx of caller.
 *
 * No get_reference. Do not use the table after this.
 */
AFW_DECLARE(void)
afw_hash_table_release_impl(
    const afw_hash_table_t *internal,
    afw_xctx_t *xctx);

/**
 * @brief Get the value for key, or NULL.
 */
#define afw_hash_table_get(instance, key, klen) \
    afw_hash_table_get_impl(&(instance)->internal, \
        (key), (klen))

/**
 * @brief Get the value for a utf8 key, or NULL.
 */
#define afw_hash_table_get_utf8(instance, key) \
    afw_hash_table_get((instance), (key)->s, (key)->len)

/**
 * @brief Set or delete the value for key. NULL value deletes.
 */
#define afw_hash_table_set(instance, key, klen, value, xctx) \
    afw_hash_table_set_impl(&(instance)->internal, \
        (key), (klen), (value), (xctx))

/**
 * @brief Set or delete the value for a utf8 key.
 */
#define afw_hash_table_set_utf8(instance, key, value, xctx) \
    afw_hash_table_set((instance), (key)->s, (key)->len, \
        (value), (xctx))

/**
 * @brief Position index at the first entry (or end).
 */
#define afw_hash_table_first(instance, index) \
    afw_hash_table_first_impl(&(instance)->internal, (index))

/**
 * @brief Advance index to the next entry (or end).
 */
#define afw_hash_table_next(index) \
    afw_hash_table_next_impl(index)

/**
 * @brief Fill key/klen/value. Returns false at end.
 *
 *     afw_hash_table_index_t hi;
 *     const void *key;
 *     afw_size_t klen;
 *     void *value;
 *     for (afw_hash_table_first(ht, &hi);
 *         afw_hash_table_this(&hi, &key, &klen, &value);
 *         afw_hash_table_next(&hi))
 */
#define afw_hash_table_this(index, key, klen, value) \
    afw_hash_table_this_impl((index), \
        (const void **)(key), (klen), (void **)(value))

/**
 * @brief Recycle all entries. Capacity is kept.
 */
#define afw_hash_table_clear(instance, xctx) \
    afw_hash_table_clear_impl(&(instance)->internal, (xctx))

/**
 * @brief Free header, buckets, and entries. Do not use after.
 */
#define afw_hash_table_release(instance, xctx) \
    afw_hash_table_release_impl(&(instance)->internal, (xctx))

AFW_END_DECLARES

/** @} */

#endif /* __AFW_HASH_TABLE_H__ */

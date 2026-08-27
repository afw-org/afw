// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Memory
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_MEMORY_H__
#define __AFW_MEMORY_H__


/**
 * @addtogroup afw_memory
 * @{
 */

#include "afw_minimal.h"

/**
 * @file afw_memory.h
 * @brief Untyped pointer+length (`afw_memory_t`) and byte helpers.
 *
 * Same dest/copy verbs as @ref afw_utf8 (`create` copies, `create_no_copy`
 * points) without NFC. See `designs/c-naming-and-payloads.md`.
 * Pool alloc with `p, xctx` last: `afw_memory_malloc` / `calloc` / `free`
 * below (same job as `afw_pool_*` with instance first).
 */

AFW_BEGIN_DECLARES

/**
 * @brief Copy to preallocated memory of same type.
 * @param to address.
 * @param from address.
 *
 * The size of the type of the variable pointed to by the to parameter is
 * used for the size used by memcpy().
 */
#define afw_memory_copy(to, from) \
memcpy((to), (from), sizeof(*(to)))


/**
 * @brief Clear preallocated memory for sizeof(*(to)).
 * @param to address.
 */
#define afw_memory_clear(to) \
memset((to), 0, sizeof(*(to)))


/**
 * @brief Allocate cleared memory. Pool last with xctx.
 * @param size of memory to allocate.
 * @param p pool.
 * @param xctx of caller.
 * @return Pointer to memory allocated.
 *
 * Same as afw_pool_calloc() with `p, xctx` at the end.
 */
#define afw_memory_calloc(size, p, xctx) \
    afw_pool_calloc((p), (size), (xctx))


/**
 * @brief Allocate uncleared memory. Pool last with xctx.
 * @param size of memory to allocate.
 * @param p pool.
 * @param xctx of caller.
 * @return Pointer to memory allocated.
 */
#define afw_memory_malloc(size, p, xctx) \
    afw_pool_malloc((p), (size), (xctx))


/**
 * @brief Allocate cleared memory for type. Pool last with xctx.
 * @param type to allocate.
 * @param p pool.
 * @param xctx of caller.
 * @return Pointer cast to type *.
 */
#define afw_memory_calloc_type(type, p, xctx) \
    (type *) afw_pool_calloc((p), sizeof(type), (xctx))


/**
 * @brief Allocate uncleared memory for type. Pool last with xctx.
 * @param type to allocate.
 * @param p pool.
 * @param xctx of caller.
 * @return Pointer cast to type *.
 */
#define afw_memory_malloc_type(type, p, xctx) \
    (type *) afw_pool_malloc((p), sizeof(type), (xctx))


/**
 * @brief Optionally free memory. Pool last with xctx.
 * @param address returned by afw_memory_malloc/calloc.
 * @param size passed to malloc/calloc.
 * @param p pool.
 * @param xctx of caller.
 */
#define afw_memory_free(address, size, p, xctx) \
    afw_pool_free_memory((p), (address), (size), (xctx))


/**
 * @brief Optionally free a typed allocation. Pool last with xctx.
 * @param address returned by afw_memory_malloc_type/calloc_type.
 * @param type allocated.
 * @param p pool.
 * @param xctx of caller.
 */
#define afw_memory_free_type(address, type, p, xctx) \
    afw_pool_free_memory((p), (address), sizeof(type), (xctx))


/**
 * @brief Create afw_memory_t in p (copy bytes).
 * @param ptr octets.
 * @param size number of bytes.
 * @param p pool for the struct and the copy.
 * @param xctx of caller.
 * @return Pointer to afw_memory_t.
 *
 * Short name is safe: always copy. No NFC (untyped bytes).
 */
AFW_DEFINE_STATIC_INLINE(const afw_memory_t *)
afw_memory_create(
    const afw_byte_t *ptr,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_t *result;
    afw_byte_t *copy;

    result = afw_pool_malloc_type(p, afw_memory_t, xctx);
    if (size > 0 && ptr) {
        copy = afw_pool_malloc(p, size, xctx);
        memcpy(copy, ptr, size);
        result->ptr = copy;
        result->size = size;
    }
    else {
        result->ptr = NULL;
        result->size = 0;
    }
    return result;
}


/**
 * @brief Create afw_memory_t in p pointing at ptr (no copy).
 * @param ptr octets that must live as long as the result.
 * @param size number of bytes.
 * @param p pool for the struct only.
 * @param xctx of caller.
 */
AFW_DEFINE_STATIC_INLINE(const afw_memory_t *)
afw_memory_create_no_copy(
    const afw_byte_t *ptr,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_t *result;

    result = afw_pool_malloc_type(p, afw_memory_t, xctx);
    result->ptr = ptr;
    result->size = size;
    return result;
}


/**
 * @brief Set a preallocated afw_memory_t (copy bytes into p).
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_memory_set(
    afw_memory_t *to,
    const afw_byte_t *ptr,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_t *created;

    created = afw_memory_create(ptr, size, p, xctx);
    to->ptr = created->ptr;
    to->size = created->size;
}


/**
 * @brief Set a preallocated afw_memory_t pointing at ptr (no copy).
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_memory_set_no_copy(
    afw_memory_t *to,
    const afw_byte_t *ptr,
    afw_size_t size)
{
    to->ptr = ptr;
    to->size = size;
}



/**
 * @brief Duplicate a block of memory into specified pool.
 * @param from From address.
 * @param size Size of memory to duplicate.
 * @param p Pool to use.
 * @param xctx of caller.
 * @return Pointer to memory allocated or NULL is size is 0.
 *
 * The default pool from xctx will be used.
 */
AFW_DEFINE_STATIC_INLINE(void *)
afw_memory_dup(const void *from,
    apr_size_t size, const afw_pool_t *p, afw_xctx_t *xctx)
{
    void *result;
    
    if (size == 0) return NULL;
    result = afw_pool_malloc(p, size, xctx);
    memcpy(result, from, size);
    return result;
}



/** @brief Return value from afw_memory_create_writer(). */
typedef struct {
    const afw_pool_t *p; /* Temporary pool used by memory writer. */
    void *context;       /* Context to pass to callback. */
    afw_write_cb_t callback;   /* Write callback function. */
} afw_memory_writer_t;



/**
 * @brief Encode memory to as base64 string.
 * @param encode is a afw_utf8_t that will be updated with result.
 * @param memory to encode.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_memory_encode_base64(afw_utf8_t *encoded, const afw_memory_t *memory,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Decode memory to a base64 string.
 * @param memory is a afw_memory_t that will be updated with result.
 * @param encoded is a base64 encoded string to decode.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_memory_decode_base64(
    afw_memory_t *memory,
    const afw_utf8_t *encoded,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Encode memory to a printable hex string.
 * @param encode is a afw_utf8_t that will be updated with result.
 * @param memory to encode.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_memory_encode_printable_hex(
    afw_utf8_t *encoded,
    const afw_memory_t *memory,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Decode memory to a printable hex string.
 * @param memory is a afw_memory_t that will be updated with result.
 * @param encoded is a printable hex encoded string to decode.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_memory_decode_printable_hex(
    afw_memory_t *memory,
    const afw_utf8_t *encoded,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a memory writer
 * @param p to use.
 * @param xctx of caller.
 * @return memory writer instance.
 */
AFW_DECLARE(const afw_memory_writer_t *)
afw_memory_create_writer(
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Retrieve memory as one chunk from memory writer and release
 *    writer.
 * @param writer instance of memory writer.
 * @param xctx of caller.
 * @return current total number of octets in buffer.
 */
AFW_DECLARE(afw_size_t)
afw_memory_writer_get_current_size(
    const afw_memory_writer_t *writer, afw_xctx_t *xctx);



/**
 * @brief Retrieve memory as using callback.
 * @param writer instance of memory writer.
 * @param context to pass to callback.
 * @param callback to call.
 * @param xctx of caller.
 * @return total number of octets written to callback.
 */
AFW_DECLARE(afw_size_t)
afw_memory_writer_retrieve_using_callback_and_release(
    const afw_memory_writer_t *writer,
    void *context,
    afw_write_cb_t callback,
    afw_xctx_t *xctx);



/**
 * @brief Retrieve memory as one chunk from memory writer and release writer.
 * @param writer instance of memory writer.
 * @param xctx of caller.
 * @return memory in pool specified on afw_memory_create_writer().
 */
AFW_DECLARE(const afw_memory_t *)
afw_memory_writer_retrieve_and_release(
    const afw_memory_writer_t *writer, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_MEMORY_H__ */

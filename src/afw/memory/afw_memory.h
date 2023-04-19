// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Memory
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_MEMORY_H__
#define __AFW_MEMORY_H__


/**
 * @defgroup afw_memory Memory support
 * @ingroup afw_c_api_public
 *
 * Adaptive framework memory support functions.
 *
 * @{
 */

/**
 * @file afw_memory.h
 * @brief Header file for Adaptive Framework Memory
 */

#include "afw_minimal.h"

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
 * @brief Create a afw_memory_t struct for a ptr and size.
 * @param ptr.
 * @param size
 * @param p to use.
 * @param xctx of caller.
 * @return Pointer to afw_memory_t.
 */
AFW_DEFINE_STATIC_INLINE(const afw_memory_t *)
afw_memory_create(
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

/** @} */

#endif /* __AFW_MEMORY_H__ */

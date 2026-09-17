// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Endian Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ENDIAN_H__
#define __AFW_ENDIAN_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_endian
 * @{
 */

/**
 * @file afw_endian.h
 * @brief Endian conversion helpers for portable binary data.
 */

AFW_BEGIN_DECLARES

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
#define AFW_ENDIAN_IS_BIG \
    ((__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) ? 1 : 0)
#else
#define AFW_ENDIAN_IS_BIG 0
#endif

#define AFW_ENDIAN_SWAP(_X_, _Y_) \
    _X_ ^= _Y_; \
    _Y_ ^= _X_; \
    _X_ ^= _Y_

/** Swap ENDIAN of 8 byte unsigned. */
#define AFW_ENDIAN_SWITCH_64(_N_) \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[0], ((unsigned char *)(_N_))[7]); \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[1], ((unsigned char *)(_N_))[6]); \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[2], ((unsigned char *)(_N_))[5]); \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[3], ((unsigned char *)(_N_))[4]); \

/** Swap ENDIAN of 4 byte unsigned. */
#define AFW_ENDIAN_SWITCH_32(_N_) \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[0], ((unsigned char *)(_N_))[3]); \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[1], ((unsigned char *)(_N_))[2]); \

/** Swap ENDIAN of 2 byte unsigned. */
#define AFW_ENDIAN_SWITCH_16(_N_) \
    AFW_ENDIAN_SWAP(((unsigned char *)(_N_))[0], ((unsigned char *)(_N_))[1]); \


/** Macros for endian. */
#if AFW_ENDIAN_IS_BIG
#define AFW_ENDIAN_IS_LITTLE 0
#define AFW_ENDIAN_NATIVE_TO_LITTLE_64(_N_) AFW_ENDIAN_SWITCH_64(_N_)
#define AFW_ENDIAN_LITTLE_TO_NATIVE_64(_N_) AFW_ENDIAN_SWITCH_64(_N_)
#define AFW_ENDIAN_NATIVE_TO_LITTLE_32(_N_) AFW_ENDIAN_SWITCH_32(_N_)
#define AFW_ENDIAN_LITTLE_TO_NATIVE_32(_N_) AFW_ENDIAN_SWITCH_32(_N_)
#define AFW_ENDIAN_NATIVE_TO_LITTLE_16(_N_) AFW_ENDIAN_SWITCH_16(_N_)
#define AFW_ENDIAN_LITTLE_TO_NATIVE_16(_N_) AFW_ENDIAN_SWITCH_16(_N_)
#define AFW_ENDIAN_NATIVE_TO_BIG_64(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE_64(_N_)
#define AFW_ENDIAN_NATIVE_TO_BIG_32(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE_32(_N_)
#define AFW_ENDIAN_NATIVE_TO_BIG_16(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE_16(_N_)
#else
#define AFW_ENDIAN_IS_LITTLE 1
#define AFW_ENDIAN_NATIVE_TO_LITTLE_64(_N_)
#define AFW_ENDIAN_LITTLE_TO_NATIVE_64(_N_)
#define AFW_ENDIAN_NATIVE_TO_LITTLE_32(_N_)
#define AFW_ENDIAN_LITTLE_TO_NATIVE_32(_N_)
#define AFW_ENDIAN_NATIVE_TO_LITTLE_16(_N_)
#define AFW_ENDIAN_LITTLE_TO_NATIVE_16(_N_)
#define AFW_ENDIAN_NATIVE_TO_BIG_64(_N_) AFW_ENDIAN_SWITCH_64(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE_64(_N_) AFW_ENDIAN_SWITCH_64(_N_)
#define AFW_ENDIAN_NATIVE_TO_BIG_32(_N_) AFW_ENDIAN_SWITCH_32(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE_32(_N_) AFW_ENDIAN_SWITCH_32(_N_)
#define AFW_ENDIAN_NATIVE_TO_BIG_16(_N_) AFW_ENDIAN_SWITCH_16(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE_16(_N_) AFW_ENDIAN_SWITCH_16(_N_)
#endif

#define AFW_ENDIAN_NATIVE_TO_BIG(_N_, _BITS_) AFW_ENDIAN_NATIVE_TO_BIG_ ## _BITS_(_N_)
#define AFW_ENDIAN_BIG_TO_NATIVE(_N_, _BITS_) AFW_ENDIAN_BIG_TO_NATIVE_ ## _BITS_(_N_)

#define AFW_ENDIAN_INCREMENT_BIG_64(_N_) \
    AFW_ENDIAN_BIG_TO_NATIVE_64(_N_); \
    (_N_)++; \
    AFW_ENDIAN_NATIVE_TO_BIG_64(_N_)

#define AFW_ENDIAN_INCREMENT_BIG_32(_N_) \
    AFW_ENDIAN_BIG_TO_NATIVE_32(_N_); \
    (_N_)++; \
    AFW_ENDIAN_NATIVE_TO_BIG_32(_N_)

#define AFW_ENDIAN_INCREMENT_BIG_16(_N_) \
    AFW_ENDIAN_BIG_TO_NATIVE_16(_N_); \
    (_N_)++; \
    AFW_ENDIAN_NATIVE_TO_BIG_16(_N_)

/** @brief 64-bit unsigned big endian integer. */
typedef union {
    unsigned char c[8];
    afw_uint64_t i;
} afw_endian_big_uint64_t;

/** @brief 64-bit unsigned little endian integer. */
typedef union {
    unsigned char c[8];
    afw_uint64_t i;
} afw_endian_little_uint64_t;

/** @brief 64-bit signed big endian integer. */
typedef union {
    unsigned char c[8];
    afw_int64_t i;
} afw_endian_big_int64_t;

/** @brief 64-bit signed little endian integer. */
typedef union {
    unsigned char c[8];
    afw_int64_t i;
} afw_endian_little_int64_t;

/** @brief 32-bit unsigned big endian integer. */
typedef union {
    unsigned char c[4];
    afw_uint32_t i;
} afw_endian_big_uint32_t;

/** @brief 32-bit unsigned little endian integer.  */
typedef union {
    unsigned char c[4];
    afw_uint32_t i;
} afw_endian_little_uint32_t;

/** @brief 32-bit signed big endian integer. */
typedef union {
    unsigned char c[4];
    afw_int32_t i;
} afw_endian_big_int32_t;

/** @brief 32-bit signed little endian integer. */
typedef union {
    unsigned char c[4];
    afw_int32_t i;
} afw_endian_little_int32_t;

/** @brief 16-bit unsigned big endian integer. */
typedef union {
    unsigned char c[2];
    afw_uint16_t i;
} afw_endian_big_uint16_t;

/** @brief 16-bit unsigned little endian integer. */
typedef union {
    unsigned char c[2];
    afw_uint16_t i;
} afw_endian_little_uint16_t;

/** @brief 16-bit signed big endian integer. */
typedef union {
    unsigned char c[2];
    afw_int16_t i;
} afw_endian_big_int16_t;

/** @brief 16-bit signed little endian integer. */
typedef union {
    unsigned char c[2];
    afw_int16_t i;
} afw_endian_little_int16_t;

/**
 * @brief Safe afw_endian_big_uint64_t to native afw_size_t
 * @param big afw_endian_big_uint64_t
 * @return native afw_size_t
 *
 * afw_endian_big_uint64_t may be larger than afw_size_t.  An error will be
 * thrown if the value in big exceeds the value that will fit in afw_size_t. 
 */
AFW_DECLARE(afw_size_t)
afw_endian_safe_big_uint64_to_native_size_t(
    afw_endian_big_uint64_t big, afw_xctx_t *xctx);

/**
 * @brief afw_endian_big_uint64_t to native afw_uint64_t
 * @param big afw_endian_big_uint64_t
 * @return native afw_uint64_t
 */
AFW_DECLARE(afw_uint64_t)
afw_endian_big_to_native_uint64(
    afw_endian_big_uint64_t big);

/**
 * @brief afw_endian_big_int64_t to native afw_int64_t
 * @param big afw_endian_big_uint64_t
 * @return native afw_uint64_t
 */
AFW_DECLARE(afw_int64_t)
afw_endian_big_to_native_int64(
    afw_endian_big_int64_t big);

/**
 * @brief native afw_uint64_t to afw_endian_big_uint64_t
 * @param native afw_uint64_t
 * @return afw_endian_big_uint64_t
 */
AFW_DECLARE(afw_endian_big_uint64_t)
afw_endian_native_to_big_uint64(
    afw_uint64_t native);

/**
 * @brief native afw_int64_t to afw_endian_big_int64_t
 * @param native afw_int64_t
 * @return afw_endian_big_int64_t
 */
AFW_DECLARE(afw_endian_big_int64_t)
afw_endian_native_to_big_int64(
    afw_int64_t native);

/**
 * @brief afw_endian_big_uint32_t to native afw_uint32_t
 * @param big afw_endian_big_uint32_t
 * @return native afw_uint32_t
 */
AFW_DECLARE(afw_uint32_t)
afw_endian_big_to_native_uint32(
    afw_endian_big_uint32_t big);

/**
 * @brief afw_endian_big_int32_t to native afw_int32_t
 * @param big afw_endian_big_uint32_t
 * @return native afw_uint32_t
 */
AFW_DECLARE(afw_int32_t)
afw_endian_big_to_native_int32(
    afw_endian_big_int32_t big);

/**
 * @brief native afw_uint32_t to afw_endian_big_uint32_t
 * @param native afw_uint32_t
 * @return afw_endian_big_uint32_t
 */
AFW_DECLARE(afw_endian_big_uint32_t)
afw_endian_native_to_big_uint32(
    afw_uint32_t native);

/**
 * @brief native afw_int32_t to afw_endian_big_int32_t
 * @param native afw_int32_t
 * @return afw_endian_big_int32_t
 */
AFW_DECLARE(afw_endian_big_int32_t)
afw_endian_native_to_big_int32(
    afw_int32_t native);

/**
 * @brief afw_endian_big_uint16_t to native afw_uint16_t
 * @param big afw_endian_big_uint16_t
 * @return native afw_uint16_t
 */
AFW_DECLARE(afw_uint16_t)
afw_endian_big_to_native_uint16(
    afw_endian_big_uint16_t big);

/**
 * @brief afw_endian_big_int16_t to native afw_int16_t
 * @param big afw_endian_big_uint16_t
 * @return native afw_uint16_t
 */
AFW_DECLARE(afw_int16_t)
afw_endian_big_to_native_int16(
    afw_endian_big_int16_t big);

/**
 * @brief native afw_uint16_t to afw_endian_big_uint16_t
 * @param native afw_uint16_t
 * @return afw_endian_big_uint16_t
 */
AFW_DECLARE(afw_endian_big_uint16_t)
afw_endian_native_to_big_uint16(
    afw_uint16_t native);

/**
 * @brief native afw_int16_t to afw_endian_big_int16_t
 * @param native afw_int16_t
 * @return afw_endian_big_int16_t
 */
AFW_DECLARE(afw_endian_big_int16_t)
afw_endian_native_to_big_int16(
    afw_int16_t native);

/**
 * @brief Safe afw_endian_little_uint64_t to native afw_size_t
 * @param little afw_endian_little_uint64_t
 * @return native afw_size_t
*
 * afw_endian_little_uint64_t may be larger than afw_size_t.  An error will be
 * thrown if the value in little exceeds the value that will fit in afw_size_t.
 */
AFW_DECLARE(afw_size_t)
afw_endian_safe_little_uint64_to_native_size_t(
    afw_endian_little_uint64_t little, afw_xctx_t *xctx);

/**
 * @brief afw_endian_little_uint64_t to native afw_uint64_t
 * @param little afw_endian_little_uint64_t
 * @return native afw_uint64_t
 */
AFW_DECLARE(afw_uint64_t)
afw_endian_little_to_native_uint64(
    afw_endian_little_uint64_t little);

/**
 * @brief afw_endian_little_int64_t to native afw_int64_t
 * @param little afw_endian_little_uint64_t
 * @return native afw_uint64_t
 */
AFW_DECLARE(afw_int64_t)
afw_endian_little_to_native_int64(
    afw_endian_little_int64_t little);

/**
 * @brief native afw_uint64_t to afw_endian_little_uint64_t
 * @param native afw_uint64_t
 * @return afw_endian_little_uint64_t
 */
AFW_DECLARE(afw_endian_little_uint64_t)
afw_endian_native_to_little_uint64(
    afw_uint64_t native);

/**
 * @brief native afw_int64_t to afw_endian_little_int64_t
 * @param native afw_int64_t
 * @return afw_endian_little_int64_t
 */
AFW_DECLARE(afw_endian_little_int64_t)
afw_endian_native_to_little_int64(
    afw_int64_t native);

/**
 * @brief afw_endian_little_uint32_t to native afw_uint32_t
 * @param little afw_endian_little_uint32_t
 * @return native afw_uint32_t
 */
AFW_DECLARE(afw_uint32_t)
afw_endian_little_to_native_uint32(
    afw_endian_little_uint32_t little);

/**
 * @brief afw_endian_little_int32_t to native afw_int32_t
 * @param little afw_endian_little_uint32_t
 * @return native afw_uint32_t
 */
AFW_DECLARE(afw_int32_t)
afw_endian_little_to_native_int32(
    afw_endian_little_int32_t little);

/**
 * @brief native afw_uint32_t to afw_endian_little_uint32_t
 * @param native afw_uint32_t
 * @return afw_endian_little_uint32_t
 */
AFW_DECLARE(afw_endian_little_uint32_t)
afw_endian_native_to_little_uint32(
    afw_uint32_t native);

/**
 * @brief native afw_int32_t to afw_endian_little_int32_t
 * @param native afw_int32_t
 * @return afw_endian_little_int32_t
 */
AFW_DECLARE(afw_endian_little_int32_t)
afw_endian_native_to_little_int32(
    afw_int32_t native);

/**
 * @brief afw_endian_little_uint16_t to native afw_uint16_t
 * @param little afw_endian_little_uint16_t
 * @return native afw_uint16_t
 */
AFW_DECLARE(afw_uint16_t)
afw_endian_little_to_native_uint16(
    afw_endian_little_uint16_t little);

/**
 * @brief afw_endian_little_int16_t to native afw_int16_t
 * @param little afw_endian_little_uint16_t
 * @return native afw_uint16_t
 */
AFW_DECLARE(afw_int16_t)
afw_endian_little_to_native_int16(
    afw_endian_little_int16_t little);

/**
 * @brief native afw_uint16_t to afw_endian_little_uint16_t
 * @param native afw_uint16_t
 * @return afw_endian_little_uint16_t
 */
AFW_DECLARE(afw_endian_little_uint16_t)
afw_endian_native_to_little_uint16(
    afw_uint16_t native);

/**
 * @brief native afw_int16_t to afw_endian_little_int16_t
 * @param native afw_int16_t
 * @return afw_endian_little_int16_t
 */
AFW_DECLARE(afw_endian_little_int16_t)
afw_endian_native_to_little_int16(
    afw_int16_t native);

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ENDIAN_H__ */

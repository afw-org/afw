// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Endian Conversion
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_endian.c
 * @brief Endian conversion helpers (need a temp; not portable as macros).
 */

#include "afw_internal.h"


AFW_DEFINE(afw_size_t)
afw_endian_safe_big_uint64_to_native_size_t(
    afw_endian_big_uint64_t big, afw_xctx_t *xctx)
{
    afw_uint64_t result = big.i;

    /* Convert to native and throw error if it didn't fit afw_size_t. */
    AFW_ENDIAN_BIG_TO_NATIVE_64(&result);
    if (result != (afw_size_t)result) {
        AFW_THROW_ERROR_Z(general,
            "Value in afw_endian_big_uint64_t exceeds the value that will "
            "fit in afw_size_t",
            xctx);
    }

    return (afw_size_t)result;
}

AFW_DEFINE(afw_uint64_t)
afw_endian_big_to_native_uint64(
    afw_endian_big_uint64_t big)
{
    afw_uint64_t result = big.i;
    AFW_ENDIAN_BIG_TO_NATIVE_64(&result);
    return result;
}

AFW_DEFINE(afw_int64_t)
afw_endian_big_to_native_int64(
    afw_endian_big_int64_t big)
{
    afw_int64_t result = big.i;
    AFW_ENDIAN_BIG_TO_NATIVE_64(&result);
    return result;
}

AFW_DEFINE(afw_endian_big_uint64_t)
afw_endian_native_to_big_uint64(
    afw_uint64_t native)
{
    afw_endian_big_uint64_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_BIG_64(&result);
    return result;
}

AFW_DEFINE(afw_endian_big_int64_t)
afw_endian_native_to_big_int64(
    afw_int64_t native)
{
    afw_endian_big_int64_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_BIG_64(&result);
    return result;
}

AFW_DEFINE(afw_uint32_t)
afw_endian_big_to_native_uint32(
    afw_endian_big_uint32_t big)
{
    afw_uint32_t result = big.i;
    AFW_ENDIAN_BIG_TO_NATIVE_32(&result);
    return result;
}

AFW_DEFINE(afw_int32_t)
afw_endian_big_to_native_int32(
    afw_endian_big_int32_t big)
{
    afw_int32_t result = big.i;
    AFW_ENDIAN_BIG_TO_NATIVE_32(&result);
    return result;
}

AFW_DEFINE(afw_endian_big_uint32_t)
afw_endian_native_to_big_uint32(
    afw_uint32_t native)
{
    afw_endian_big_uint32_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_BIG_32(&result);
    return result;
}

AFW_DEFINE(afw_endian_big_int32_t)
afw_endian_native_to_big_int32(
    afw_int32_t native)
{
    afw_endian_big_int32_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_BIG_32(&result);
    return result;
}

AFW_DEFINE(afw_uint16_t)
afw_endian_big_to_native_uint16(
    afw_endian_big_uint16_t big)
{
    afw_uint16_t result = big.i;
    AFW_ENDIAN_BIG_TO_NATIVE_16(&result);
    return result;
}

AFW_DEFINE(afw_int16_t)
afw_endian_big_to_native_int16(
    afw_endian_big_int16_t big)
{
    afw_int16_t result = big.i;
    AFW_ENDIAN_BIG_TO_NATIVE_16(&result);
    return result;
}

AFW_DEFINE(afw_endian_big_uint16_t)
afw_endian_native_to_big_uint16(
    afw_uint16_t native)
{
    afw_endian_big_uint16_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_BIG_16(&result);
    return result;
}

AFW_DEFINE(afw_endian_big_int16_t)
afw_endian_native_to_big_int16(
    afw_int16_t native)
{
    afw_endian_big_int16_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_BIG_16(&result);
    return result;
}

AFW_DEFINE(afw_size_t)
afw_endian_safe_little_uint64_to_native_size_t(
    afw_endian_little_uint64_t little, afw_xctx_t *xctx)
{
    afw_uint64_t result = little.i;

    /* Convert to native and throw error if it didn't fit afw_size_t. */
    AFW_ENDIAN_LITTLE_TO_NATIVE_64(&result);
    if (result != (afw_size_t)result) {
        AFW_THROW_ERROR_Z(general,
            "Value in afw_endian_little_uint64_t exceeds the value that will "
            "fit in afw_size_t",
            xctx);
    }

    return (afw_size_t)result;
}

AFW_DEFINE(afw_uint64_t)
afw_endian_little_to_native_uint64(
    afw_endian_little_uint64_t little)
{
    afw_uint64_t result = little.i;
    AFW_ENDIAN_LITTLE_TO_NATIVE_64(&result);
    return result;
}

AFW_DEFINE(afw_int64_t)
afw_endian_little_to_native_int64(
    afw_endian_little_int64_t little)
{
    afw_int64_t result = little.i;
    AFW_ENDIAN_LITTLE_TO_NATIVE_64(&result);
    return result;
}

AFW_DEFINE(afw_endian_little_uint64_t)
afw_endian_native_to_little_uint64(
    afw_uint64_t native)
{
    afw_endian_little_uint64_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_LITTLE_64(&result);
    return result;
}

AFW_DEFINE(afw_endian_little_int64_t)
afw_endian_native_to_little_int64(
    afw_int64_t native)
{
    afw_endian_little_int64_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_LITTLE_64(&result);
    return result;
}

AFW_DEFINE(afw_uint32_t)
afw_endian_little_to_native_uint32(
    afw_endian_little_uint32_t little)
{
    afw_uint32_t result = little.i;
    AFW_ENDIAN_LITTLE_TO_NATIVE_32(&result);
    return result;
}

AFW_DEFINE(afw_int32_t)
afw_endian_little_to_native_int32(
    afw_endian_little_int32_t little)
{
    afw_int32_t result = little.i;
    AFW_ENDIAN_LITTLE_TO_NATIVE_32(&result);
    return result;
}

AFW_DEFINE(afw_endian_little_uint32_t)
afw_endian_native_to_little_uint32(
    afw_uint32_t native)
{
    afw_endian_little_uint32_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_LITTLE_32(&result);
    return result;
}

AFW_DEFINE(afw_endian_little_int32_t)
afw_endian_native_to_little_int32(
    afw_int32_t native)
{
    afw_endian_little_int32_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_LITTLE_32(&result);
    return result;
}

AFW_DEFINE(afw_uint16_t)
afw_endian_little_to_native_uint16(
    afw_endian_little_uint16_t little)
{
    afw_uint16_t result = little.i;
    AFW_ENDIAN_LITTLE_TO_NATIVE_16(&result);
    return result;
}

AFW_DEFINE(afw_int16_t)
afw_endian_little_to_native_int16(
    afw_endian_little_int16_t little)
{
    afw_int16_t result = little.i;
    AFW_ENDIAN_LITTLE_TO_NATIVE_16(&result);
    return result;
}

AFW_DEFINE(afw_endian_little_uint16_t)
afw_endian_native_to_little_uint16(
    afw_uint16_t native)
{
    afw_endian_little_uint16_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_LITTLE_16(&result);
    return result;
}

AFW_DEFINE(afw_endian_little_int16_t)
afw_endian_native_to_little_int16(
    afw_int16_t native)
{
    afw_endian_little_int16_t result;
    result.i = native;
    AFW_ENDIAN_NATIVE_TO_LITTLE_16(&result);
    return result;
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Safe Cast
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_safe_cast.c
 * @brief Safe integer casts with overflow checks.
 */

#include "afw_internal.h"


AFW_DEFINE(afw_size_t)
afw_safe_cast_off_to_size(
    afw_off_t off, afw_xctx_t *xctx)
{
    afw_size_t result = (afw_size_t)off;

    if (result != off) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

AFW_DEFINE(afw_size_t)
afw_safe_cast_integer_to_size(
    afw_integer_t integer, afw_xctx_t *xctx)
{
    afw_size_t result = (afw_size_t)integer;

    if (result != integer) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

AFW_DEFINE(int)
afw_safe_cast_integer_to_int(
    afw_integer_t integer, afw_xctx_t *xctx)
{
    int result = (int)integer;

    if (result != integer) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

AFW_DEFINE(int)
afw_safe_cast_size_to_int(
    afw_size_t size, afw_xctx_t *xctx)
{
    int result = (int)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

AFW_DEFINE(afw_integer_t)
afw_safe_cast_size_to_integer(
    afw_size_t size, afw_xctx_t *xctx)
{
    afw_integer_t result = (afw_integer_t)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

AFW_DEFINE(afw_int32_t)
afw_safe_cast_size_to_int32(
    afw_size_t size, afw_xctx_t *xctx)
{
    afw_int32_t result = (afw_int32_t)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

AFW_DEFINE(afw_uint32_t)
afw_safe_cast_size_to_uint32(
    afw_size_t size, afw_xctx_t *xctx)
{
    afw_uint32_t result = (afw_uint32_t)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}

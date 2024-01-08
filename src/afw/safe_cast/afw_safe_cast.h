// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Safe Cast Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_SAFE_CAST_H__
#define __AFW_SAFE_CAST_H__

#include "afw_interface.h"
#include <limits.h>


/**
 * @defgroup afw_safe_cast Safe cast
 * @ingroup afw_c_api_public
 *
 * Safe cast between c types.
 *
 * @{
 */

/**
 * @file afw_safe_cast.h
 * @brief Safe cast functions
 */

AFW_BEGIN_DECLARES

/*
 * NOTE:
 *
 * This can be improved later based on the compiler target.  For instance,
 * if two types have the same internal representation, an #ifdef can be
 * used to just assign the value without doing the value test.
 */

/**
 * @brief Safely cast afw_off_t to afw_size_t
 * @param off afw_off_t
 * @return native afw_size_t
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(afw_size_t)
afw_safe_cast_off_to_size(afw_off_t off, afw_xctx_t *xctx)
{
    afw_size_t result = (afw_size_t)off;

    if (result != off) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}


/**
 * @brief Safely cast afw_integer_t to afw_size_t
 * @param integer afw_integer_t
 * @return native afw_size_t
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(afw_size_t)
afw_safe_cast_integer_to_size(afw_integer_t integer, afw_xctx_t *xctx)
{
    afw_size_t result = (afw_size_t)integer;

    if (result != integer) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}



/**
 * @brief Safely cast afw_integer_t to int
 * @param integer afw_integer_t
 * @return native int
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(int)
afw_safe_cast_integer_to_int(afw_integer_t integer, afw_xctx_t *xctx)
{
    int result = (int)integer;

    if (result != integer) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}



/**
 * @brief Safely cast afw_size_t to int
 * @param size afw_size_t
 * @return native int
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(int)
afw_safe_cast_size_to_int(afw_size_t size, afw_xctx_t *xctx)
{
    int result = (int)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}



/**
 * @brief Safely cast afw_size_t to afw_integer_t.
 * @param size afw_size_t
 * @return afw_integer_t
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(afw_integer_t)
afw_safe_cast_size_to_integer(afw_size_t size, afw_xctx_t *xctx)
{
    afw_integer_t result = (afw_integer_t)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}



/**
 * @brief Safely cast afw_size_t to afw_int32_t.
 * @param size afw_size_t
 * @return native int
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(afw_int32_t)
afw_safe_cast_size_to_int32(afw_size_t size, afw_xctx_t *xctx)
{
    afw_int32_t result = (afw_int32_t)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}



/**
 * @brief Safely cast afw_size_t to afw_uint32_t.
 * @param size afw_size_t
 * @return native int
 *
 * The size of these on some systems may be different.
 */
AFW_DEFINE_STATIC_INLINE(afw_uint32_t)
afw_safe_cast_size_to_uint32(afw_size_t size, afw_xctx_t *xctx)
{
    afw_uint32_t result = (afw_uint32_t)size;

    if (result != size) {
        AFW_THROW_ERROR_Z(general, "Type safe error", xctx);
    }

    return result;
}


AFW_END_DECLARES

/** @} */

#endif /* __AFW_SAFE_CAST_H__ */

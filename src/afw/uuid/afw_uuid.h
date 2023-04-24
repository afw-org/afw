// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework UUID Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_UUID_H__
#define __AFW_UUID_H__

#include "afw_minimal.h"
#include "apr_uuid.h"

/**
 * @defgroup afw_uuid UUID
 * @ingroup afw_c_api_public
 *
 * UUID functions
 *
 * @{
 */

/**
 * @file afw_uuid.h
 * @brief Adaptive Framework UUID header
 */

AFW_BEGIN_DECLARES

/* afw_uuid_t is the same as apr_uuid_t. */
typedef apr_uuid_t afw_uuid_t;

/*
 * Standard format UUID string are formatted as:
 *
 * 00112233-4455-6677-8899-AABBCCDDEEFF
 */

/**
 * @brief Generate a new UUID.
 * @param uuid Place to put generated UUID.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_uuid_generate(
    afw_uuid_t *uuid, afw_xctx_t *xctx);


/**
 * @brief Create a UUID.
 * @param xctx of caller.
 * @param p Pool for result.
 * @return uuid
 */
AFW_DECLARE(const afw_uuid_t *)
afw_uuid_create(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a UUID as a standard format UUID utf-8 string value.
 * @param xctx of caller.
 * @param p Pool for result.
 * @return uuid as string value
 */
AFW_DECLARE(const afw_value_t *)
afw_uuid_create_string(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a UUID as a standard format UUID utf-8 string.
 * @param xctx of caller.
 * @param p Pool for result.
 * @return uuid as string
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uuid_create_utf8(
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Convert uuid to a standard format UUID utf-8 string.
 * @param uuid
 * @param p Pool for result.
 * @param xctx of caller.
 * @return uuid as string
 */
AFW_DECLARE(const afw_utf8_t *)
afw_uuid_to_utf8(
    const afw_uuid_t *uuid,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Parse a standard format UUID string to a uuid.
 * @param uuid Place to put parsed UUID.
 * @param s standard format UUID utf-8 string to parse.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_uuid_parse(
    afw_uuid_t *uuid,
    const afw_utf8_t *s,
    afw_xctx_t *xctx);


/**
 * @brief Convert standard format UUID utf-8 string to uuid.
 * @param s standard format UUID utf-8 string.
 * @param p Pool for result.
 * @param xctx of caller.
 * @return uuid
 */
AFW_DECLARE(const afw_uuid_t *)
afw_uuid_from_utf8(
    const afw_utf8_t *s,
    const afw_pool_t *p, afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_UUID_H__ */

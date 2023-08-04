// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptive UUID Support.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_uuid.c
 * @brief AFW UUID Support
 */

#include "afw_internal.h"



/* Generate a new UUID. */
AFW_DEFINE(void)
afw_uuid_generate(
    afw_uuid_t *uuid, afw_xctx_t *xctx)
{
    apr_uuid_get(uuid);
}


/* Create a UUID. */
AFW_DEFINE(const afw_uuid_t *)
afw_uuid_create(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t * uuid;

    uuid = afw_pool_malloc_type(p, afw_uuid_t, xctx);
    apr_uuid_get(uuid);
    return uuid;
}


/* Create a UUID as a standard format UUID string value. */
AFW_DEFINE(const afw_value_t *)
afw_uuid_create_string(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t uuid;
    afw_value_string_t *string;

    string = afw_value_allocate_string(p, xctx);
    string->internal.len = APR_UUID_FORMATTED_LENGTH + 1;
    string->internal.s = afw_pool_malloc(p, string->internal.len,
        xctx);
    apr_uuid_get(&uuid);
    apr_uuid_format((char *)string->internal.s, &uuid);

    return (const afw_value_t *)string;
}


/* Create a UUID as a standard format UUID string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uuid_create_utf8(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t uuid;
    char *buffer;

    buffer = afw_pool_malloc(p, APR_UUID_FORMATTED_LENGTH + 1,
        xctx);
    apr_uuid_get(&uuid);
    apr_uuid_format(buffer, &uuid);

    return afw_utf8_create(buffer, AFW_UTF8_Z_LEN, p, xctx);
}


/* Convert uuid to a standard format UUID string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uuid_to_utf8(
    const afw_uuid_t *uuid,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    char *buffer;

    buffer = afw_pool_malloc(p, APR_UUID_FORMATTED_LENGTH + 1,
        xctx);
    apr_uuid_format(buffer, uuid);

    return afw_utf8_create(buffer, AFW_UTF8_Z_LEN, p, xctx);    
}


/* Parse a standard format UUID string to a uuid. */
AFW_DEFINE(void)
afw_uuid_parse(
    afw_uuid_t *uuid,
    const afw_utf8_t *s,
    afw_xctx_t *xctx)
{
    apr_status_t rv = 0;
    char uuid_str[APR_UUID_FORMATTED_LENGTH + 1];

    if (s->len != APR_UUID_FORMATTED_LENGTH) goto error;
    memcpy(uuid_str, s->s, s->len);
    uuid_str[s->len] = 0;
    rv = apr_uuid_parse(uuid, uuid_str);
    if (rv != APR_SUCCESS) goto error;
    return;

error:
    AFW_THROW_ERROR_RV_Z(general, apr, rv, "Invalid uuid string.", xctx);
}


/* Convert a standard format UUID string to uuid. */
AFW_DEFINE(const afw_uuid_t *)
afw_uuid_from_utf8(
    const afw_utf8_t *s,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t *uuid;
    char uuid_str[APR_UUID_FORMATTED_LENGTH + 1];
    apr_status_t rv = 0;

    if (s->len > APR_UUID_FORMATTED_LENGTH) goto error;
    memcpy(uuid_str, s->s, s->len);
    uuid_str[s->len] = 0;
    uuid = afw_pool_malloc_type(p, afw_uuid_t, xctx);
    rv = apr_uuid_parse(uuid, uuid_str);
    if (rv != APR_SUCCESS) goto error;

    return uuid;
    
error:
    AFW_THROW_ERROR_RV_Z(general, apr, rv, "Invalid uuid string.", xctx);
}

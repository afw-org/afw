// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptive UUID Support.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_uuid.c
 * @brief UUID create, parse, and format helpers.
 */

#include "afw_internal.h"
#include <stdio.h>


static int
impl_hex_nibble(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static void
impl_uuid_format(char *buf, const afw_uuid_t *uuid)
{
    const afw_byte_t *d;

    d = uuid->data;
    sprintf(buf,
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-"
        "%02x%02x%02x%02x%02x%02x",
        d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7],
        d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]);
}

static afw_boolean_t
impl_uuid_parse(afw_uuid_t *uuid, const char *s)
{
    afw_size_t i;
    afw_size_t di;
    int hi;
    int lo;

    static const int dash_at[4] = { 8, 13, 18, 23 };

    di = 0;
    for (i = 0; i < AFW_UUID_FORMATTED_LENGTH; i++) {
        if (i == (afw_size_t)dash_at[0] ||
            i == (afw_size_t)dash_at[1] ||
            i == (afw_size_t)dash_at[2] ||
            i == (afw_size_t)dash_at[3])
        {
            if (s[i] != '-') {
                return false;
            }
            continue;
        }
        hi = impl_hex_nibble(s[i]);
        lo = impl_hex_nibble(s[i + 1]);
        if (hi < 0 || lo < 0) {
            return false;
        }
        uuid->data[di++] = (afw_byte_t)((hi << 4) | lo);
        i++;
    }
    return di == 16;
}

static void
impl_uuid_generate(afw_uuid_t *uuid, afw_xctx_t *xctx)
{
    afw_os_random_bytes(uuid->data, sizeof(uuid->data), xctx);
    /* RFC 4122 version 4 / variant 10. */
    uuid->data[6] = (afw_byte_t)((uuid->data[6] & 0x0f) | 0x40);
    uuid->data[8] = (afw_byte_t)((uuid->data[8] & 0x3f) | 0x80);
}


/* Generate a new UUID. */
AFW_DEFINE(void)
afw_uuid_generate(
    afw_uuid_t *uuid, afw_xctx_t *xctx)
{
    impl_uuid_generate(uuid, xctx);
}


/* Create a UUID. */
AFW_DEFINE(const afw_uuid_t *)
afw_uuid_create(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t * uuid;

    uuid = afw_pool_malloc_type(p, afw_uuid_t, xctx);
    impl_uuid_generate(uuid, xctx);
    return uuid;
}


/* Create a UUID as a standard format UUID string value. */
AFW_DEFINE(const afw_value_t *)
afw_uuid_create_string(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t uuid;
    afw_value_string_t *string;
    char buf[AFW_UUID_FORMATTED_LENGTH + 1];

    impl_uuid_generate(&uuid, xctx);
    impl_uuid_format(buf, &uuid);
    string = afw_value_allocate_unmanaged_string(p, xctx);
    string->internal.len = AFW_UUID_FORMATTED_LENGTH;
    string->internal.s = afw_pool_malloc(p,
        AFW_UUID_FORMATTED_LENGTH + 1, xctx);
    memcpy((char *)string->internal.s, buf,
        AFW_UUID_FORMATTED_LENGTH + 1);

    return &string->pub;
}


/* Create a UUID as a standard format UUID string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uuid_create_utf8(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t uuid;
    char buf[AFW_UUID_FORMATTED_LENGTH + 1];

    impl_uuid_generate(&uuid, xctx);
    impl_uuid_format(buf, &uuid);
    return afw_utf8_create(buf, AFW_UUID_FORMATTED_LENGTH, p, xctx);
}


/* Convert uuid to a standard format UUID string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uuid_to_utf8(
    const afw_uuid_t *uuid,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    char buf[AFW_UUID_FORMATTED_LENGTH + 1];

    impl_uuid_format(buf, uuid);
    return afw_utf8_create(buf, AFW_UUID_FORMATTED_LENGTH, p, xctx);
}


/* Parse a standard format UUID string to a uuid. */
AFW_DEFINE(void)
afw_uuid_parse(
    afw_uuid_t *uuid,
    const afw_utf8_t *s,
    afw_xctx_t *xctx)
{
    char uuid_str[AFW_UUID_FORMATTED_LENGTH + 1];

    if (s->len != AFW_UUID_FORMATTED_LENGTH) {
        AFW_THROW_ERROR_Z(general, "Invalid uuid string.", xctx);
    }
    memcpy(uuid_str, s->s, s->len);
    uuid_str[s->len] = 0;
    if (!impl_uuid_parse(uuid, uuid_str)) {
        AFW_THROW_ERROR_Z(general, "Invalid uuid string.", xctx);
    }
}


/* Convert a standard format UUID string to uuid. */
AFW_DEFINE(const afw_uuid_t *)
afw_uuid_from_utf8(
    const afw_utf8_t *s,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uuid_t *uuid;
    char uuid_str[AFW_UUID_FORMATTED_LENGTH + 1];

    if (s->len > AFW_UUID_FORMATTED_LENGTH) {
        AFW_THROW_ERROR_Z(general, "Invalid uuid string.", xctx);
    }
    memset(uuid_str, 0, sizeof(uuid_str));
    memcpy(uuid_str, s->s, s->len);
    uuid = afw_pool_malloc_type(p, afw_uuid_t, xctx);
    if (!impl_uuid_parse(uuid, uuid_str)) {
        AFW_THROW_ERROR_Z(general, "Invalid uuid string.", xctx);
    }

    return uuid;
}

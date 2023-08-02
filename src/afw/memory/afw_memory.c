// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework memory functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_memory.c
 * @brief Adaptive Framework memory support
 */

#include "afw_internal.h"



/* Indexed by value from impl_base64_value. */
static const afw_utf8_octet_t
impl_base64_digits[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";



/*
 * 0-63 = index into impl_base64_digits
 * 64 = '='
 * 65 = invalid
 */
static const afw_octet_t
impl_base64_value[256] =
{
    /*   0  1  2  3   4  5  6  7   8  9  a  b   c  d  e  f       */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* 0 */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* 1 */
        65,65,65,65, 65,65,65,65, 65,65,65,62, 65,65,65,63, /* 2 */
        52,53,54,55, 56,57,58,59, 60,61,65,65, 65,64,65,65, /* 3 */
        65, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14, /* 4 */
        15,16,17,18, 19,20,21,22, 23,24,25,65, 65,65,65,65, /* 5 */
        65,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40, /* 6 */
        41,42,43,44, 45,46,47,48, 49,50,51,65, 65,65,65,65, /* 7 */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* 8 */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* 9 */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* a */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* b */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* c */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* d */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65, /* e */
        65,65,65,65, 65,65,65,65, 65,65,65,65, 65,65,65,65  /* f */
};



static const char
impl_hex_digits[16] =
{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };



const afw_octet_t
impl_hex_lookup[256] = {
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
0,     1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255, 255, 255, 255, 255,
255, 10,  11,  12,  13,  14,  15,  255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255,  10,  11,  12,  13,  14,  15, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};



#define IMPL_BINARY_BUF_SEGMENT_SIZE 1000

typedef struct impl_memory_buffer_segment_s
impl_memory_buffer_segment_t;

struct impl_memory_buffer_segment_s {
    impl_memory_buffer_segment_t *next;
    afw_size_t used;
    afw_octet_t s[IMPL_BINARY_BUF_SEGMENT_SIZE];
};


typedef struct {
    afw_memory_writer_t pub;
    const afw_pool_t *result_p;
    impl_memory_buffer_segment_t *first;
    impl_memory_buffer_segment_t *last;
} impl_memory_self_t;



static afw_size_t impl_callback(void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_memory_self_t *self = context;
    afw_size_t count;
    const afw_octet_t *s;
    impl_memory_buffer_segment_t *segment;

    for (s = (const afw_octet_t *)buffer, count = size;
        count > 0;
        count--, s++)
    {
        if (self->last->used == IMPL_BINARY_BUF_SEGMENT_SIZE) {
            segment = afw_pool_malloc_type(self->pub.p,
                impl_memory_buffer_segment_t,
                xctx);
            segment->next = NULL;
            segment->used = 0;
            self->last->next = segment;
            self->last = segment;
        }
        self->last->s[self->last->used++] = *s;
    }

    return size;
}




/* Clone an object to a specified pool. */
AFW_DEFINE(const afw_object_t *)
afw_object_create_clone(
    const afw_object_t * object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *new_object = NULL;

    afw_data_type_clone_internal(afw_data_type_object,
        (void *)&new_object, &object, p, xctx);

    return new_object;
}


/* Create a memory object with properties from two other objects. */
AFW_DEFINE(const afw_object_t *) afw_object_create_merged(
    const afw_object_t * primary,
    const afw_object_t * secondary,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_object_t *result;
    const afw_value_t *value;
    const afw_utf8_t *property_name;

    result = afw_object_create_clone(primary, p, xctx);

    /* Merge */
    iterator = NULL;
    do {
        value = afw_object_get_next_property(secondary, &iterator,
            &property_name, xctx);
        if (!value) break;
        if (!afw_object_has_property(primary, property_name, xctx)) {
            value = afw_value_clone(value, p, xctx);
            afw_object_set_property(result, property_name, value, xctx);
        }
    } while (1);

    /* Return merged object. */
    return result;
}



AFW_DEFINE(void)
afw_memory_encode_base64(afw_utf8_t *encoded, const afw_memory_t *memory,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_octet_t *s;
    afw_octet_t d1, d2, d3, d4;
    afw_size_t i;

    encoded->len = ((memory->size + 2) / 3) * 4;
    if (encoded->len == 0) {
        encoded->s = NULL;
        return;
    }

    s = afw_pool_malloc(p, encoded->len, xctx);
    encoded->s = (const afw_utf8_octet_t*)s;

    for (i = 0; i < memory->size; i += 3) {
        d1 = memory->ptr[i] >> 2;
        d2 = (memory->ptr[i] & 0x03) << 4;
        d3 = d4 = 64;
        if ((i + 1) < memory->size) {
            d2 |= memory->ptr[i + 1] >> 4;
            d3 = (memory->ptr[i + 1] & 0x0f) << 2;
            if ((i + 2) < memory->size) {
                d3 |= memory->ptr[i + 2] >> 6;
                d4 = memory->ptr[i + 2] & 0x3f;
            }
        }
        *s++ = impl_base64_digits[d1];
        *s++ = impl_base64_digits[d2];
        *s++ = impl_base64_digits[d3];
        *s++ = impl_base64_digits[d4];
    }
}



AFW_DEFINE(void)
afw_memory_decode_base64(
    afw_memory_t *memory, const afw_utf8_t *encoded,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t non_ws_len, equals;
    const afw_octet_t *c;
    afw_octet_t c1, c2, c3, c4;
    afw_octet_t *b;
    const afw_octet_t *end;

    /* Syntax check and determine number for non ws chars other than '=' */
    for (non_ws_len = equals = 0, c = (const afw_octet_t *)encoded->s, end = c + encoded->len; ; c++)
    {

        if (c >= end) {
            if (non_ws_len % 4 == 0) break;
        }

        else {

            if (afw_ascii_is_whitespace(*c)) {
                continue;
            }

            if (impl_base64_value[*c] <= 63) {
                non_ws_len++;
                continue;
            }

            if (*c == '=') {
                if (non_ws_len % 4 == 3) {
                    if (c + 1 == end) {
                        equals = 1;
                        non_ws_len++;
                        break;
                    }
                }
                else if (non_ws_len % 4 == 2) {
                    if (c + 2 == end && *(c + 1) == '=') {
                        equals = 2;
                        non_ws_len += 2;
                        break;
                    }
                }
            }

        }

        AFW_THROW_ERROR_Z(general, "Invalid base64 encoding", xctx);
    }

    /* Allocate memory for result.  Just return if length is 0. */
    memory->size = (non_ws_len / 4) * 3 - equals;
    if (memory->size == 0) {
        memory->ptr = NULL;
        return;
    }
    b = afw_pool_malloc(p, memory->size, xctx);
    memory->ptr = b;

    /* Decode */
    for (c = (const afw_octet_t *)encoded->s; c < end;)
    {
        for (c1 = *c++; afw_ascii_is_whitespace(c1); c1 = *c++);
        for (c2 = *c++; afw_ascii_is_whitespace(c2); c2 = *c++);
        for (c3 = *c++; afw_ascii_is_whitespace(c3); c3 = *c++);
        for (c4 = *c++; afw_ascii_is_whitespace(c4); c4 = *c++);

        *b++ = (impl_base64_value[c1] << 2) |
            (impl_base64_value[c2] >> 4);

        if (c3 == '=') break;

        *b++ = ((impl_base64_value[c2] << 4) & 0xF0) |
            (impl_base64_value[c3] >> 2);

        if (c4 == '=') break;

        *b++ = ((impl_base64_value[c3] << 6) & 0xC0) |
            (impl_base64_value[c4]);
    }
}



/* Encode memory to a printable hex string. */
AFW_DEFINE(void)
afw_memory_encode_printable_hex(afw_utf8_t *encoded, const afw_memory_t *memory,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const unsigned char *in;
    afw_utf8_octet_t *out;
    afw_size_t count;

    if (memory->size == 0) {
        memset(encoded, 0, sizeof(afw_utf8_t));
        return;
    }

    encoded->len = memory->size * 2;
    encoded->s = out = afw_pool_malloc(p, encoded->len, xctx);

    for (count = memory->size, in = memory->ptr; count > 0; count--, in++) {
        *out = impl_hex_digits[(*in) >> 4];
        out++;
        *out = impl_hex_digits[(*in) & 0x0f];
        out++;
    }
}



/* Encode memory to a printable hex string. */
AFW_DEFINE(void)
afw_memory_decode_printable_hex(
    afw_memory_t *memory, const afw_utf8_t *encoded,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_octet_t *in;
    afw_octet_t *out;
    afw_size_t count;

    /* Length can not be odd. */
    if (encoded->len & 1) goto error;

    if (encoded->len == 0) {
        memset(memory, 0, sizeof(afw_memory_t));
        return;
    }

    count = encoded->len / 2;
    memory->ptr = out = afw_pool_malloc(p, count, xctx);
    memory->size = count;

    for (in = (const afw_octet_t *)encoded->s; count > 0; in += 2, out++, count--)
    {
        if (*in != 0x20 && *in != 0x09 && *in != 0x0a && *in != 0x0d) {
            if (impl_hex_lookup[*in] == 255 || impl_hex_lookup[*(in + 1)] == 255)
                goto error;
            *out = (impl_hex_lookup[*in] << 4) + impl_hex_lookup[*(in + 1)];
        }
    }

    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Invalid printable hex value " AFW_UTF8_FMT ".",
        AFW_UTF8_FMT_ARG(encoded));
}



AFW_DEFINE(const afw_memory_writer_t *)
afw_memory_create_writer(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_memory_self_t *self;
    afw_memory_writer_t *writer;
    const afw_pool_t *self_p;

    self_p = afw_pool_create(p, xctx);
    self = afw_pool_calloc_type(self_p, impl_memory_self_t, xctx);
    writer = (afw_memory_writer_t *)self;
    writer->callback = impl_callback;
    writer->context = self;
    self->pub.p = self_p;
    self->result_p = p;
    self->first = self->last = afw_pool_malloc_type(self_p,
        impl_memory_buffer_segment_t, xctx);
    self->first->next = NULL;
    self->first->used = 0;

    return writer;
}


AFW_DEFINE(afw_size_t)
afw_memory_writer_get_current_size(
    const afw_memory_writer_t *writer, afw_xctx_t *xctx)
{
    impl_memory_self_t *self = (impl_memory_self_t *)writer;
    afw_size_t size;
    impl_memory_buffer_segment_t *segment;

    for (size = 0, segment = self->first; segment; segment = segment->next) {
        size += segment->used;
    }

    return size;
}



AFW_DEFINE(afw_size_t)
afw_memory_writer_retrieve_using_callback_and_release(
    const afw_memory_writer_t *writer,
    void *context,
    afw_write_cb_t callback,
    afw_xctx_t *xctx)
{
    impl_memory_self_t *self = (impl_memory_self_t *)writer;
    impl_memory_buffer_segment_t *segment;
    afw_size_t size;

    for (size = 0, segment = self->first; segment; segment = segment->next)
    {
        callback(context, &segment->s[0], segment->used, self->result_p, xctx);
        size += segment->used;
    }
    afw_pool_release(self->pub.p, xctx);

    return size;
}



AFW_DEFINE(const afw_memory_t *)
afw_memory_writer_retrieve_and_release(
    const afw_memory_writer_t *writer, afw_xctx_t *xctx)
{
    impl_memory_self_t *self = (impl_memory_self_t *)writer;
    afw_memory_t *result;
    impl_memory_buffer_segment_t *segment;
    afw_octet_t *ptr;

    result = afw_pool_calloc_type(self->result_p, afw_memory_t, xctx);

    for (segment = self->first; segment; segment = segment->next) {
        result->size += segment->used;
    }

    if (result->size > 0) {
        result->ptr = ptr = afw_pool_malloc(self->result_p,
            result->size, xctx);
        for (segment = self->first; segment; segment = segment->next) {
            memcpy(ptr, &segment->s[0], segment->used);
            ptr += segment->used;
        }
    }

    afw_pool_release(self->pub.p, xctx);

    return result;
}

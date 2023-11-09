// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW UBJSON convert from value
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_ubjson_from_value.c
 * @brief Convert an AFW value to UBJSON.
 */

#include "afw.h"
#include "afw_endian.h"
#include "afw_ubjson.h"
#include <apr_buckets.h>


typedef struct from_value_wa_s {
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const afw_object_options_t *options;
    afw_boolean_t optimized;
    afw_boolean_t suppressDataType;
    void * context;
    afw_write_cb_t callback;
    int object_depth;
} from_value_wa_t;


AFW_DEFINE_STATIC_INLINE(void) impl_putc(from_value_wa_t *wa, afw_byte_t c)
{
    char ch;

    ch = c;
    wa->callback(wa->context, &ch, 1, wa->p, wa->xctx);
}


AFW_DEFINE_STATIC_INLINE(void) impl_puts(from_value_wa_t *wa,
    const afw_utf8_z_t *s)
{
    wa->callback(wa->context, s, strlen(s), wa->p, wa->xctx);
}


AFW_DEFINE_STATIC_INLINE(void) impl_write(from_value_wa_t *wa,
    const void * buffer, afw_size_t size)
{
    wa->callback(wa->context, buffer, size, wa->p, wa->xctx);
}


static from_value_wa_t * create_from_value_wa(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    from_value_wa_t *wa;

    wa = afw_pool_calloc_type(p, from_value_wa_t, xctx);
    wa->xctx = xctx;
    wa->p = p;

    return wa;
}

static void convert_value_to_ubjson(
    from_value_wa_t * wa,
    const afw_value_t * value);

static void convert_list_to_ubjson(
    from_value_wa_t * wa,
    const afw_array_t * list);

static void convert_object_to_ubjson(
    from_value_wa_t * wa,
    const afw_object_t * obj);

static void convert_string_to_ubjson(
    from_value_wa_t * wa,
    const afw_utf8_t *string,
    afw_boolean_t marker);

static void convert_integer_to_ubjson(
    from_value_wa_t * wa,
    afw_integer_t i);

static void convert_string_to_ubjson(
    from_value_wa_t * wa,
    const afw_utf8_t *string,
    afw_boolean_t marker)
{
    if (string->len == 1 && marker) {
        /* for single-byte strings, treat as CHAR */
        impl_putc(wa, AFW_UBJSON_MARKER_CHAR);
        impl_putc(wa, *(string->s));

    } else {
        /* property names do not need a marker */
        if (marker) {
            impl_putc(wa, AFW_UBJSON_MARKER_STRING);
        }

        convert_integer_to_ubjson(wa, string->len);

        /* may not be necessary, but not sure if APR handles zero byte writes (empty string) */
        if (string->len > 0) {
            impl_write(wa, string->s, string->len);
        } 
    }
}

/* this routine takes a 64-bit integer and fits it into the minimal space */
static void convert_integer_to_ubjson(
    from_value_wa_t * wa,
    afw_integer_t i)
{
    /*
        UBJSON defines 8 numeric value types (to JSON's 1) for allowing
        highly optimized storage/retrieval of numeric values depending on
        the necessary precision; in addition to a number of other more
        optimized representations of JSON values.

        Type              Size         Marker  Length  Payload
        ----------------------------------------------------------------------
        int8              2-bytes      i       No      Yes
        uint8             2-bytes      U       No      Yes
        int16             3-bytes      I       No      Yes
        int32             5-bytes      l       No      Yes
        int64             9-bytes      L       No      Yes
        float32           5-bytes      d       No      Yes
        float64           9-bytes      D       No      Yes
        high-precision    1-byte+      H       Yes     Yes (if non-empty)
     */
    if (i >= -128 && i <= 127) {
        char x = (char)i;

        impl_putc(wa, AFW_UBJSON_MARKER_INT8);
        impl_write(wa, (const char*)&x, sizeof(x));
    }

    else if (i >= 0 && i <= 255) {
        char x = (char)i;

        impl_putc(wa, AFW_UBJSON_MARKER_UINT8);
        impl_write(wa, (const char*)&x, sizeof(x));
    }

    else if (i >= -32768 && i <= 32767) {
        afw_uint16_t x = (afw_uint16_t)i;
        AFW_ENDIAN_NATIVE_TO_BIG_16(&x);

        impl_putc(wa, AFW_UBJSON_MARKER_INT16);
        impl_write(wa, (const char*)&x, sizeof(x));
    }

    else if (i >= -2147483647 && i <= 2147483647) {
        afw_uint32_t x = (afw_uint32_t)i;
        AFW_ENDIAN_NATIVE_TO_BIG_32(&x);

        impl_putc(wa, AFW_UBJSON_MARKER_INT32);
        impl_write(wa, (const char*)&x, sizeof(x));
    }

    else {
        /* this should never happen.  64-bit integers can't exceed this range */
        AFW_THROW_ERROR_Z(general,
            "Unexpected error in UBJSON converting 64-bit integer.", wa->xctx);
    }
}

/* this routine takes a double and turns it into a 64-bit float */
static void convert_double_to_ubjson(
    from_value_wa_t * wa,
    double d)
{
    /* 
        UBJSON defines 8 numeric value types (to JSON's 1) for allowing
        highly optimized storage/retrieval of numeric values depending on 
        the necessary precision; in addition to a number of other more 
        optimized representations of JSON values.

        Type              Size         Marker  Length  Payload
        ----------------------------------------------------------------------
        int8              2-bytes      i       No      Yes
        uint8             2-bytes      U       No      Yes
        int16             3-bytes      I       No      Yes
        int32             5-bytes      l       No      Yes
        int64             9-bytes      L       No      Yes
        float32           5-bytes      d       No      Yes
        float64           9-bytes      D       No      Yes
        high-precision    1-byte+      H       Yes     Yes (if non-empty)
     */

    impl_putc(wa, AFW_UBJSON_MARKER_FLOAT64);

    /** @fixme endian on a double? */
    impl_write(wa, &d, sizeof(d));
}

static void convert_object_to_ubjson(
    from_value_wa_t * wa,
    const afw_object_t * obj)
{
    const afw_iterator_t *property_iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *next;
    const afw_object_t *meta;

    impl_putc(wa, AFW_UBJSON_MARKER_OBJECT);

    (wa->object_depth)++;
 
    property_iterator = NULL;

    if (wa->optimized) {
        /** @fixme, if we can count the entries, then we can
            optimize the object */
    }

    /* If object has meta, convert it first. */
    meta = afw_object_meta_create_accessor_with_options(obj,
        wa->options, wa->p, wa->xctx);
    if (meta) {
        convert_string_to_ubjson(wa, afw_s__meta_, AFW_FALSE);
        convert_object_to_ubjson(wa, meta);
    }

    next = afw_object_get_next_property(obj, &property_iterator, 
        &property_name, wa->xctx);
    if (next) {
        while (1) {
            /* property name */
            convert_string_to_ubjson(wa, property_name, AFW_FALSE);

            /* property value */
            convert_value_to_ubjson(wa, next);
 
            next = afw_object_get_next_property(obj, &property_iterator, 
                &property_name, wa->xctx);

            if (!next)
                break;
        }
    }

    if (!wa->optimized) {
        impl_putc(wa, AFW_UBJSON_MARKER_OBJECT_);
    }

    (wa->object_depth)--;
}


#ifdef __FIXME_REMOVE_OLD_BAG_CODE__
/*
 * AFW Bags allow us to create specialized UBJSON Array Containers.
 * By knowing both the number of elements, and the data type of all
 * elements in the Bag, we can get more efficient storage with the
 * optimized format:
 *
 *   [$][type][#][i][count]
 * 
 * For example, a Bag of 64 Strings starts with:
 *
 *     [$S#i64
 *
 * http://ubjson.org/type-reference/container-types/#optimized-format-example-array
 *
 */
static void convert_bag_to_ubjson(
    from_value_wa_t * wa,
    const afw_value_bag_t * bag)
{
    afw_size_t count;
    afw_value_common_t *entry;
    const afw_data_type_t *data_type;
    const char *value;

    /* write out the Array marker */
    impl_putc(wa, AFW_UBJSON_MARKER_ARRAY);

    if (wa->optimized) {
        /* followed by the type */
        impl_putc(wa, AFW_UBJSON_MARKER_TYPE);

        /* use the data type for ubjson efficiency */
        data_type = bag->data_type;

        if (afw_data_type_is_object(data_type)) {
            impl_putc(wa, AFW_UBJSON_MARKER_OBJECT);
        } else if (afw_data_type_is_list(data_type)) {
            impl_putc(wa, AFW_UBJSON_MARKER_ARRAY);
        } else if (afw_data_type_is_string(data_type)) {
            /** @fixme determine if all are Char? */
            impl_putc(wa, AFW_UBJSON_MARKER_STRING);
        } else if (afw_data_type_is_double(data_type)) {
            /** @fixme determine smallest type */
            AFW_THROW_ERROR_Z(general,
                "Not implemented.  Cannot determine smallest type for UBJSON", wa->xctx);
        }

        /* the bag count can be used to create more efficient ubjson */ 
        count = bag->count;

        /* write out the count */
        impl_putc(wa, AFW_UBJSON_MARKER_COUNT);
        convert_integer_to_ubjson(wa, count);
    }

    entry = afw_value_common_allocate(bag->data_type,
        wa->p, wa->xctx);
    
    /* now we can traverse the values */
    for (value = (const char *)&(bag->internal),
        count = bag->count;
        count > 0;
        value += bag->data_type->c_type_size, 
        count--) 
    {
        memcpy(&entry->internal, value, bag->data_type->c_type_size);
        if (wa->optimized)
            wa->suppressDataType = AFW_TRUE;

        convert_value_to_ubjson(wa, (const afw_value_t *)entry);

        if (wa->optimized)
            wa->suppressDataType = AFW_FALSE;
    }

    if (!wa->optimized) {
        impl_putc(wa, AFW_UBJSON_MARKER_ARRAY_);
    }
}
#endif



static void convert_list_to_ubjson(
    from_value_wa_t * wa,
    const afw_array_t * list)
{
    const afw_iterator_t *list_iterator;
    const afw_value_t *next;

    impl_putc(wa, AFW_UBJSON_MARKER_ARRAY);

    if (wa->optimized) {
        /** @fixme, if we can count the entries, then we can
            optimize the list */
    }

    list_iterator = NULL;
    next = afw_array_get_next_value(list, &list_iterator,
        wa->p, wa->xctx);
    
    while (next) {
        convert_value_to_ubjson(wa, next);
        next = afw_array_get_next_value(list, &list_iterator,
            wa->p, wa->xctx);
    }

    if (!wa->optimized) {
        impl_putc(wa, AFW_UBJSON_MARKER_ARRAY_);
    }
}

static void convert_value_to_ubjson(
    from_value_wa_t * wa,
    const afw_value_t * value)
{
    const afw_utf8_t *string;
    const afw_data_type_t *value_data_type;
    afw_value_info_t info;

    /* Change NULL value pointer to null.  */
    if (!value) {
        value = afw_value_null;
    }

    /* Value must be evaluated already. */
    if (!afw_value_is_defined_and_evaluated(value)) {
        afw_value_get_info(value, &info, wa->p, wa->xctx);
        AFW_THROW_ERROR_FZ(general, wa->xctx,
            "Unevaluated value encountered producing ubjson "
            "(" AFW_UTF8_FMT " " AFW_UTF8_FMT ")",
            AFW_UTF8_FMT_ARG(info.value_inf_id),
            AFW_UTF8_FMT_OPTIONAL_ARG(info.detail)
        );
    }

    /* Get data type. */
    value_data_type = afw_value_get_data_type(value, wa->xctx);

    if (afw_value_is_array(value)) {
        convert_list_to_ubjson(wa, afw_value_as_array(value, wa->xctx));
    }

    else if (afw_value_is_object(value)) {
        convert_object_to_ubjson(wa, afw_value_as_object(value, wa->xctx));
    }

    else if (afw_value_is_defined_and_evaluated(value)) {

        /* Primitive json type is null. */
        if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            AFW_JSON_S_PRIMITIVE_NULL)) 
        {
            impl_putc(wa, AFW_UBJSON_MARKER_NULL);
        }

        /* Primitive json type is string. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            AFW_JSON_S_PRIMITIVE_STRING)) 
        {
            string = afw_value_as_utf8(value, wa->p, wa->xctx);
            if (!string) {
                AFW_THROW_ERROR_Z(general, "Error converting string.", wa->xctx);
            }
            convert_string_to_ubjson(wa, string, AFW_TRUE);
        }

        /* Primitive json type is boolean. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            AFW_JSON_S_PRIMITIVE_BOOLEAN))
        {
            if (((afw_value_boolean_t*)value)->internal)
                impl_putc(wa, AFW_UBJSON_MARKER_TRUE);
            else
                impl_putc(wa, AFW_UBJSON_MARKER_FALSE);
        }

        /* C type is an integer. */
        else if (afw_utf8_equal_utf8_z(&value_data_type->cType, AFW_DATA_TYPE_CTYPE_Q_integer))
        {
            convert_integer_to_ubjson(wa, afw_value_as_integer(value, wa->xctx));
        }

        /* C type is double. */
        else if (afw_utf8_equal_utf8_z(&value_data_type->cType, AFW_DATA_TYPE_CTYPE_Q_double))
        {
            convert_double_to_ubjson(wa, afw_value_as_double(value, wa->xctx));
        }

        else {
            AFW_THROW_ERROR_FZ(general, wa->xctx,
                "Cannot determine suitable encoding for this c type: %.*s", 
                value_data_type->cType.len, value_data_type->cType.s);
        }
    }

    else {
        AFW_THROW_ERROR_Z(general,
            "Value type is invalid for this content-type", wa->xctx);
    }
}


/* Convert a value to json and write it. */
extern void
afw_ubjson_internal_write_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    from_value_wa_t *wa;

    /* Create and initialize workarea and associated resources. */
    wa = create_from_value_wa(p, xctx);
    wa->options = options;
    wa->context = context;
    wa->callback = callback;

    /* Convert object to json. */
    convert_value_to_ubjson(wa, value);
}


/* Convert a value to ubjson. */
const afw_memory_t *
afw_ubjson_from_value(
    const afw_value_t * value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_writer_t *writer;
    const afw_memory_t *raw;

    writer = afw_memory_create_writer(p, xctx);

    afw_ubjson_internal_write_value(value, NULL,
        writer->context, writer->callback, p, xctx);

    raw = afw_memory_writer_retrieve_and_release(writer, xctx);
    return raw;
}

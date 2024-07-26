// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW ubjson conversion functions.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_ubjson.c
 * @brief AFW ubjson conversion functions.
 *
 */

#include "afw.h"
#include "afw_ubjson.h"

/** @fixme:  UTF-8 encoding/decoding. When converting JSON 
            to UBJSON, back to JSON, we lose special characters. */


typedef struct afw_ubjson_parser_s {
    const afw_memory_t *input;
    const unsigned char *ptr;
    size_t cursor;
    const afw_utf8_t *path;
    const afw_object_t *embedding_object;
    const afw_utf8_t *property_name;
    const afw_pool_t *p;
    afw_boolean_t cede_p;
} afw_ubjson_parser_t;


static const afw_value_t * afw_ubjson_parse_value(
    afw_ubjson_parser_t *parser, 
    char type,
    afw_xctx_t *xctx);

static const afw_object_t * afw_ubjson_parse_object(
    afw_ubjson_parser_t *parser,
    afw_xctx_t *xctx);

static const afw_array_t * afw_ubjson_parse_array(
    afw_ubjson_parser_t *parser,
    afw_xctx_t *xctx);

static const afw_utf8_t * afw_ubjson_parse_string(
    afw_ubjson_parser_t *parser,
    afw_xctx_t *xctx);

const afw_value_t * afw_ubjson_parse_number(
    afw_ubjson_parser_t *parser, 
    char type,
    afw_xctx_t *xctx);

size_t afw_ubjson_parse_length(
    afw_ubjson_parser_t *parser,
    afw_xctx_t *xctx);




static unsigned char afw_ubjson_peek_byte(
    afw_ubjson_parser_t *parser, afw_xctx_t *xctx)
{
    if (parser->cursor > parser->input->size) {
        AFW_THROW_ERROR_Z(general, "Error: expected end of document.", xctx);
    }

    return *(parser->ptr);
}

static unsigned char afw_ubjson_next_byte(
    afw_ubjson_parser_t *parser, afw_xctx_t *xctx)
{
    unsigned char c;

    if (parser->cursor > parser->input->size) {
        AFW_THROW_ERROR_Z(general, "Error: expected end of document.", xctx);
    }

    c = *(parser->ptr);

    parser->ptr++;
    parser->cursor++;

    return c;
}

static void afw_ubjson_next_bytes(
    afw_ubjson_parser_t *parser, unsigned char *buf, 
    size_t len, afw_xctx_t *xctx)
{
    if (parser->cursor + len > parser->input->size) {
        AFW_THROW_ERROR_Z(general, "Error: expected end of document.", xctx);
    }

    memcpy(buf, parser->ptr, len);
    parser->ptr += len;
    parser->cursor += len;
}

size_t afw_ubjson_parse_length(
    afw_ubjson_parser_t *parser, afw_xctx_t *xctx)
{
    unsigned char c;
    afw_c_types_t c_type;
    size_t d;

    c = afw_ubjson_next_byte(parser, xctx);

    switch (c) {
        case AFW_UBJSON_MARKER_INT8:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 1, xctx);
            d = c_type.int8;
            break;

        case AFW_UBJSON_MARKER_UINT8:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 1, xctx);
            d = c_type.uint8;
            break;

        case AFW_UBJSON_MARKER_INT16:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 2, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 16);
            d = c_type.int16;
            break;

        case AFW_UBJSON_MARKER_INT32:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 4, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 32);
            d = c_type.int32;
            break;

        default:
            AFW_THROW_ERROR_RV_Z(general, ubjson_marker, c, 
                "Error: invalid numeric type for string length", xctx);
            break;
    }

    return d;
}

const afw_value_t * afw_ubjson_parse_number(
    afw_ubjson_parser_t *parser,
    char type,
    afw_xctx_t *xctx)
{
    afw_c_types_t c_type;
    const afw_value_t *value;

    switch (type) {
        case AFW_UBJSON_MARKER_CHAR:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 1, xctx);
            value = afw_value_create_unmanaged_integer(
                c_type.int8, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_INT8:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 1, xctx);
            value = afw_value_create_unmanaged_integer(
                c_type.int8, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_UINT8:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 1, xctx);
            value = afw_value_create_unmanaged_integer(
                c_type.uint8, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_INT16:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 2, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 16);
            value = afw_value_create_unmanaged_integer(
                c_type.int16, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_INT32:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 4, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 32);
            value = afw_value_create_unmanaged_integer(
                c_type.int32, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_INT64:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 8, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 64);
            value = afw_value_create_unmanaged_integer(
                c_type.int64, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_FLOAT32:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 4, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 32);
            value = afw_value_create_unmanaged_double(
                c_type.float32, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_FLOAT64:
            afw_ubjson_next_bytes(parser, (unsigned char*) &c_type, 8, xctx);
            AFW_ENDIAN_BIG_TO_NATIVE(&c_type, 64);
            value = afw_value_create_unmanaged_double(
                c_type.float64, parser->p, xctx);
            break;

        default:
            AFW_THROW_ERROR_RV_Z(general, ubjson_marker, type,
                "Error: invalid numeric type.", xctx);
            break;
    }

    return value;
}

const afw_utf8_t * afw_ubjson_parse_string(
    afw_ubjson_parser_t *parser, 
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string = NULL;
    size_t len;

    len = afw_ubjson_parse_length(parser, xctx);

    if (parser->cursor + len > parser->input->size) {
        AFW_THROW_ERROR_Z(general, 
            "Error: string length exceeds input.", xctx);
    }

    string = afw_utf8_create_copy((const afw_utf8_octet_t *)parser->ptr, len, parser->p, xctx);
  
    parser->ptr += len;
    parser->cursor += len;

    return string;
}

/**
 * This routine parses an array from UBJSON and turns it into 
 * an Adaptive List.
 * 
 * FIXME: UBJSON supports some optimization tricks for arrays
 *        by providing a count of items, ahead of time, but 
 *        we currently discard this information.
 */
const afw_array_t * afw_ubjson_parse_array(
    afw_ubjson_parser_t *parser, 
    afw_xctx_t *xctx)
{
    const afw_array_t *list;
    const afw_value_t *value;
    char c;
    char type = 0;

    list = afw_array_create_generic(parser->p, xctx);

    c = afw_ubjson_peek_byte(parser, xctx);

    /* check if we have a count or type specified */
    if (c == AFW_UBJSON_MARKER_TYPE) {
        /* optimized type specified */
        type = afw_ubjson_next_byte(parser, xctx);

        c = afw_ubjson_peek_byte(parser, xctx);
        if (c == AFW_UBJSON_MARKER_COUNT) {
            /* optimized count specified */
            afw_ubjson_parse_length(parser, xctx);
        }
    } else if (c == AFW_UBJSON_MARKER_COUNT) {
        /* optimized count specified */
        afw_ubjson_parse_length(parser, xctx);
    }

    while (c != AFW_UBJSON_MARKER_ARRAY_) {
        value = afw_ubjson_parse_value(parser, type, xctx);

        afw_array_add_value(list, value, xctx);

        c = afw_ubjson_peek_byte(parser, xctx);
    }

    afw_ubjson_next_byte(parser, xctx);

    /* Return. */
    return list;
}

/**
 * This routine parses an object from UBJSON and turns it into 
 * an Adaptive Object.
 * 
 * FIXME: UBJSON supports some optimization tricks for objects
 *        by providing a count of items, ahead of time, but 
 *        we currently discard this information.
 */
const afw_object_t * afw_ubjson_parse_object(
    afw_ubjson_parser_t *parser,
    afw_xctx_t *xctx)
{
    const afw_object_t * obj;
    const afw_utf8_t * property_name;
    const afw_value_t *property_value;
    char type = 0;
    char c;
    const afw_object_t *saved_embedding_object;
    const afw_utf8_t *saved_property_name;
    const afw_object_t *_meta_;

    /* Create new memory object.*/
    AFW_OBJECT_CREATE_ENTITY_OR_EMBEDDED(obj,
        parser->embedding_object,
        parser->property_name,
        false, parser->cede_p, parser->p, xctx);
    _meta_ = NULL;

    /*
     * Save parser->embedding_object and set to new object.
     *
     * Save parser->property_name and set to NULL.
     *
     * If entity, default path is supplied by caller, otherwise it's NULL.
     */
    saved_embedding_object = parser->embedding_object;
    parser->embedding_object = obj;
    saved_property_name = parser->property_name;
    parser->property_name = NULL;

    c = afw_ubjson_peek_byte(parser, xctx);

    /* check if we have a count or type specified */
    if (c == AFW_UBJSON_MARKER_TYPE) {
        /* optimized type specified */
        type = afw_ubjson_next_byte(parser, xctx);

        c = afw_ubjson_peek_byte(parser, xctx);
        if (c == AFW_UBJSON_MARKER_COUNT) {
            /* optimized count specified */
            afw_ubjson_parse_length(parser, xctx);
        }
    } else if (c == AFW_UBJSON_MARKER_COUNT) {
        /* optimized count specified */
        afw_ubjson_parse_length(parser, xctx);
    }

    c = afw_ubjson_peek_byte(parser, xctx);
    while (c != AFW_UBJSON_MARKER_OBJECT_) {
        property_name = afw_ubjson_parse_string(parser, xctx);
        parser->property_name = property_name;
        property_value = afw_ubjson_parse_value(parser, type, xctx);

        /* check if this is a meta property */
        if (afw_utf8_equal(property_name, afw_s__meta_)) {
            if (!afw_value_is_object(property_value)) {
                AFW_THROW_ERROR_Z(general, "_meta_ property must be an object", xctx);
            }
            _meta_ = ((const afw_value_object_t *)property_value)->internal;
        }

        /*
         * If not _meta_ property, set property in new object to this
         * value if it's not an object.  Object will already have been
         * added by afw_object_create_embedded().
         */
        else if (!afw_value_is_object(property_value)) {
            afw_object_set_property(obj, property_name, 
                property_value, xctx);
        }

        c = afw_ubjson_peek_byte(parser, xctx);
    }

    afw_ubjson_next_byte(parser, xctx);

    /* Set meta.  Note: must be called after properties are set. */
    afw_object_meta_set_meta_object(obj, _meta_, xctx);

    /* Set parser->embedding_object to previous value and return object. */
    parser->embedding_object = saved_embedding_object;
    parser->property_name = saved_property_name;

    return obj;
}

const afw_value_t * afw_ubjson_parse_value(
    afw_ubjson_parser_t *parser, 
    char type,
    afw_xctx_t *xctx)
{
    const afw_value_t *value = NULL;
    const afw_object_t *obj;
    const afw_array_t *list;
    const afw_utf8_t *string;
    unsigned char c;

    /* if type has been specified, do not look at the input */
    if (type)
        c = type;
    else
        c = afw_ubjson_next_byte(parser, xctx);

    switch (c) {
        /* Null */
        case AFW_UBJSON_MARKER_NULL:
            value = afw_value_null;
            break;

        /* No-op */
        case AFW_UBJSON_MARKER_NOOP:
            value = afw_value_null;
            break;

        /* Boolean true */
        case AFW_UBJSON_MARKER_TRUE:
            value = afw_boolean_v_true;
            break;

        /* Boolean false */
        case AFW_UBJSON_MARKER_FALSE:
            value = afw_boolean_v_false;
            break;

        /* Numeric types */
        case AFW_UBJSON_MARKER_INT8:
        case AFW_UBJSON_MARKER_UINT8:
        case AFW_UBJSON_MARKER_INT16:
        case AFW_UBJSON_MARKER_INT32:
        case AFW_UBJSON_MARKER_INT64:
        case AFW_UBJSON_MARKER_FLOAT32:
        case AFW_UBJSON_MARKER_FLOAT64:
            value = afw_ubjson_parse_number(parser, c, xctx);
            break;

        /* High-precision number */
        case AFW_UBJSON_MARKER_HIGH_PRECISION:
            /** @fixme represent as String?? */
            AFW_THROW_ERROR_Z(general,
                "Error: high-precision numbers not implemented.", xctx);
            break;

        /* Char */
        case AFW_UBJSON_MARKER_CHAR:
            c = afw_ubjson_next_byte(parser, xctx);
            string = afw_utf8_create_copy((const afw_utf8_octet_t *)&c, 1, parser->p, xctx);
            value = afw_value_create_unmanaged_string(string, parser->p, xctx);
            break;

        case AFW_UBJSON_MARKER_STRING:
            string = afw_ubjson_parse_string(parser, xctx);
            value = afw_value_create_unmanaged_string(string, parser->p, xctx);
            break;

        /* Array */
        case AFW_UBJSON_MARKER_ARRAY:
            list = afw_ubjson_parse_array(parser, xctx);
            value = afw_value_create_unmanaged_array(list, parser->p, xctx);
            break;

        /* Object */
        case AFW_UBJSON_MARKER_OBJECT:
            obj = afw_ubjson_parse_object(parser, xctx);
            value = afw_value_create_unmanaged_object(obj, parser->p, xctx);
            break;

        default:
            AFW_THROW_ERROR_RV_Z(general, ubjson_marker, c,
                "Error: unknown value type marker.", xctx);
            break;
    }

    return value;
}

/*
 * Create an adaptive object from ubjson.
 */
const afw_value_t * afw_ubjson_to_value(
    const afw_memory_t *ubjson,
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    afw_ubjson_parser_t parser;

    parser.input = ubjson;
    parser.ptr = parser.input->ptr;
    parser.cursor = 0;
    parser.embedding_object = NULL;
    parser.property_name = NULL;
    parser.p = p;
    parser.path = path;
    parser.cede_p = false;

    value = afw_ubjson_parse_value(&parser, 0, xctx);

    return value;
}


/*
 * Implementation of method raw_to_object of interface afw_content_type.
 */
const afw_object_t *
afw_ubjson_to_object(
    const afw_memory_t *ubjson,
    const afw_utf8_t *source_location,
    const afw_utf8_t * adapter_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_ubjson_parser_t parser;

    parser.input = ubjson;
    parser.ptr = parser.input->ptr;
    parser.cursor = 0;
    parser.embedding_object = NULL;
    parser.property_name = NULL;
    parser.p = (cede_p) ? p : afw_pool_create(p, xctx);
    parser.cede_p = true;
    parser.path = afw_object_path_make(
        adapter_id, object_type_id, object_id, parser.p, xctx);

    /* go ahead and read the first byte, since we assume it's an object. */
    afw_ubjson_next_byte(&parser, xctx);

    /* Parse and return object. */
    return afw_ubjson_parse_object(&parser, xctx);
}

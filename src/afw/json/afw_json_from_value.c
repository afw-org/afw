// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW JSON convert from value
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_json_from_value.c
 * @brief Convert an AFW value to JSON.
 */

#include "afw_internal.h"
#include <apr_buckets.h>

typedef struct from_value_wa_s {
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const afw_object_options_t *options;
    void * context;
    afw_write_cb_t callback;
    afw_size_t indent;
    afw_size_t object_depth;
    afw_boolean_t skip_next_ws;
    afw_boolean_t do_ws;
    afw_boolean_t do_typed_values;
} impl_from_value_wa_t;

static void
impl_put_ws(impl_from_value_wa_t *wa);

static void
impl_put_json_string(
    impl_from_value_wa_t *wa,
    const afw_utf8_t *string);

static void
impl_put_quoted_to_json(
    impl_from_value_wa_t *wa,
    const afw_utf8_t *s);

static void
impl_convert_boolean_to_json(
    impl_from_value_wa_t *wa,
    afw_boolean_t b);

static void
impl_convert_list_to_json(
    impl_from_value_wa_t *wa,
    const afw_array_t *list);

static void
impl_convert_object_to_json(
    impl_from_value_wa_t *wa,
    const afw_object_t *obj);

static void
impl_convert_value_to_json(
    impl_from_value_wa_t *wa,
    const afw_value_t *value);

AFW_DEFINE_STATIC_INLINE(void)
impl_putc(impl_from_value_wa_t *wa, afw_byte_t c)
{
    char ch;

    ch = c;
    wa->callback(wa->context, &ch, 1, wa->p, wa->xctx);
}


AFW_DEFINE_STATIC_INLINE(void)
impl_puts(impl_from_value_wa_t *wa,
    const afw_utf8_z_t *s)
{
    wa->callback(wa->context, s, strlen(s), wa->p, wa->xctx);
}


AFW_DEFINE_STATIC_INLINE(void)
impl_write(impl_from_value_wa_t *wa,
    const void * buffer, afw_size_t size)
{
    wa->callback(wa->context, buffer, size, wa->p, wa->xctx);
}


static void
impl_printf(impl_from_value_wa_t *wa,
    const afw_utf8_z_t *format_z, ...)
{
    va_list arg;
    const afw_utf8_t *s;

    va_start(arg, format_z);
    s = afw_utf8_printf_v(format_z, arg, wa->p, wa->xctx);
    va_end(arg);

    wa->callback(wa->context, s->s, s->len, wa->p, wa->xctx);
}


static void
impl_put_ws(impl_from_value_wa_t *wa)
{
    afw_size_t indent;

    if (wa->do_ws) {
        if (!wa->skip_next_ws) {
            impl_putc(wa, '\n');
            for (indent = 1; indent <= wa->indent; indent++) {
                /* default whitespace indentation to 4 spaces */                
                impl_puts(wa, "    ");
            }
        }
        wa->skip_next_ws = 0;
    }
}


static void
impl_put_json_string(
    impl_from_value_wa_t *wa,
    const afw_utf8_t *string)
{
    afw_utf8_octet_t c;
    afw_size_t len;
    const afw_utf8_octet_t *s;

    impl_putc(wa, '"');

    s = string->s;
    len = string->len;

    while (len > 0) {
        c = *s;

        /* Add an extra backslash if character is backslash or quote. */
        if (c == '\\' || c == '"') {
            impl_putc(wa, '\\');
        }

        /* If not a control character, output the character. */
        if (c >= 32) {
            impl_putc(wa, c);
        }

        /* If \n */
        else if (c == 10) {
            impl_puts(wa, "\\n");
        }

        /* If \r */
        else if (c == 13) {
            impl_puts(wa, "\\r");
        }

        /* If \t */
        else if (c == 9) {
            impl_puts(wa, "\\t");
        }

        /* If \f */
        else if (c == 12) {
            impl_puts(wa, "\\f");
        }

        /* If \b */
        else if (c == 7) {
            impl_puts(wa, "\\b");
        }

        /*
         * If other control character, output as \u followed by character as four
         * byte hex characters.
         */
        else {
            impl_printf(wa, "\\u%04x", c);
        }

        len--;
        s++;
    }

    impl_putc(wa, '"');
}


static void
impl_put_quoted_to_json(
    impl_from_value_wa_t *wa,
    const afw_utf8_t *s)
{
    impl_putc(wa, '"');
    /** @fixme Need to encode?*/
    if (s) {
        impl_write(wa, s->s, s->len);
    }
    impl_putc(wa, '"');
}


static void
impl_convert_boolean_to_json(
    impl_from_value_wa_t *wa,
    afw_boolean_t b)
{
    if (b) {
        impl_puts(wa, AFW_JSON_Q_PRIMITIVE_BOOLEAN_TRUE);
    }
    else {
        impl_puts(wa, AFW_JSON_Q_PRIMITIVE_BOOLEAN_FALSE);
    }
}


static void
impl_convert_list_to_json(
    impl_from_value_wa_t *wa,
    const afw_array_t *list)
{
    const afw_iterator_t *list_iterator;
    const afw_value_t *next;

    /* Put [ and increment indent. */
    impl_putc(wa, '[');
    (wa->indent)++;

    list_iterator = NULL;
    next = afw_array_get_next_value(list, &list_iterator, 
        wa->p, wa->xctx);

    while (next) {
        impl_convert_value_to_json(wa, next);
        next = afw_array_get_next_value(list, &list_iterator,
            wa->p, wa->xctx);
        if (next) {
            impl_putc(wa, ',');
        }
    }

    /* Decrement indent and put ']'. */
    (wa->indent)--;
    impl_put_ws(wa);
    impl_putc(wa, ']');
}



static void
impl_convert_object_to_json(
    impl_from_value_wa_t *wa,
    const afw_object_t *obj)
{
    const afw_iterator_t *property_iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *next;
    afw_boolean_t starting_comma_needed;
    const afw_object_t *meta;

    /* Put { and increment indent. */
    impl_putc(wa, '{');
    (wa->indent)++;
    (wa->object_depth)++;

    /* If object has meta, convert it first. */
    starting_comma_needed = false;
    meta = afw_object_meta_create_accessor_with_options(obj,
        wa->options, wa->p, wa->xctx);
    if (meta) {
        impl_put_ws(wa);
        wa->skip_next_ws = 1;
        if (wa->do_ws) {
            impl_puts(wa, "\"_meta_\": ");
        }
        else {
            impl_puts(wa, "\"_meta_\":");
        }
        impl_put_ws(wa);
        impl_convert_object_to_json(wa, meta);
        starting_comma_needed = true;
    }

    /* Add each object property. */
    property_iterator = NULL;
    next = afw_object_get_next_property(obj, &property_iterator,
        &property_name, wa->xctx);
    if (next) {
        while (1) {
            if (starting_comma_needed) {
                impl_putc(wa, ',');
                starting_comma_needed = false;
            }
            impl_put_ws(wa);
            wa->skip_next_ws = 1;
            impl_put_quoted_to_json(wa, property_name);

            impl_putc(wa, ':');
            if (wa->do_ws) {
                impl_putc(wa, ' ');
            }

            impl_convert_value_to_json(wa, next);
            next = afw_object_get_next_property(obj,
                &property_iterator, &property_name, wa->xctx);
            if (!next) break;
            impl_putc(wa, ',');
        }
    }

    /* Decrement indent and put '}'. */
    (wa->indent)--;
    (wa->object_depth)--;
    impl_put_ws(wa);
    impl_putc(wa, '}');
}


static void
impl_convert_value_to_json(
    impl_from_value_wa_t *wa,
    const afw_value_t *value)
{
    const afw_utf8_t *string;
    const afw_object_t *object;
    const afw_utf8_t *s;
    const afw_data_type_t *value_data_type;
    afw_value_info_t info;

    /* Change undefined value to null.  */
    if (afw_value_is_undefined(value)) {
        value = afw_value_null;
    }

    /* Put whitespace if needed. */
    impl_put_ws(wa);

    /* If doing type values, start typed info. */
    if (wa->do_typed_values) {
        impl_putc(wa, '[');
        (wa->indent)++;
    }

    /* Value must be evaluated already. */
    if (!afw_value_is_defined_and_evaluated(value)) {
        afw_value_get_info(value, &info, wa->p, wa->xctx);
        AFW_THROW_ERROR_FZ(general, wa->xctx,
            "Unevaluated value encountered producing json "
            "(" AFW_UTF8_FMT " " AFW_UTF8_FMT ")",
            AFW_UTF8_FMT_ARG(info.value_inf_id),
            AFW_UTF8_FMT_OPTIONAL_ARG(info.detail)
        );
    }

    /* Get data type. */
    value_data_type = afw_value_get_data_type(value, wa->xctx);

    /* If value is a list, convert list to json. */
    if (afw_value_is_array(value)) {
        impl_convert_list_to_json(wa, ((afw_value_array_t *)value)->internal);
    }

    /* If value is object, convert object to json. */
    else  if (afw_value_is_object(value)) {
        impl_convert_object_to_json(wa,
            ((afw_value_object_t *)value)->internal);
    }

    /* If value is single, process based on jsonPrimitive value of dataType. */
    else {

        /* Primitive json type is null. */
        if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            &AFW_JSON_S_PRIMITIVE_NULL))
        {
            impl_puts(wa, AFW_JSON_Q_PRIMITIVE_NULL);
        }

        /* Primitive json type is string. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            &AFW_JSON_S_PRIMITIVE_STRING))
        {
            string = afw_value_as_utf8(value, wa->p, wa->xctx);
            if (!string) {
                AFW_THROW_ERROR_Z(general, "Error converting string.", wa->xctx);
            }
            impl_put_json_string(wa, string);
        }

        /* Primitive json type is number. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            &AFW_JSON_S_PRIMITIVE_NUMBER))
        {
            if (afw_value_is_integer(value) &&
                /** @fixme (option quoute all || */
                !AFW_INTEGER_IS_SAFE_DOUBLE(
                ((const afw_value_integer_t *)value)->internal))
                )
            {
            string = afw_value_as_utf8(value, wa->p, wa->xctx);
            if (!string) {
                AFW_THROW_ERROR_Z(general, "Error converting string.",
                    wa->xctx);
            }
            impl_put_json_string(wa, string);
            }

            else {
                string = afw_data_type_internal_to_utf8(
                    value_data_type,
                    &((const afw_value_evaluated_t *)value)->internal,
                    wa->p, wa->xctx);
                impl_write(wa, string->s, string->len);
            }
        }

        /* Primitive json type is boolean. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            &AFW_JSON_S_PRIMITIVE_BOOLEAN))
        {
            impl_convert_boolean_to_json(wa,
                ((afw_value_boolean_t *)value)->internal);
        }

        /* If none of above then it's an error. */
        else {
            AFW_THROW_ERROR_Z(general,
                "Value inf is invalid", wa->xctx);
        }
    }

    /* If doing type values, end typed info. */
    if (wa->do_typed_values) {
        impl_putc(wa, ',');
        impl_put_ws(wa);
        if (value_data_type) {
            if (afw_value_is_object(value)) {
                object = ((const afw_value_object_t *)value)->internal;
                s = &afw_s_object;
                if (afw_object_meta_get_object_type_id(object, xctx)) {
                    s = afw_utf8_printf(wa->p, wa->xctx,
                        "object:" AFW_UTF8_FMT,
                        AFW_UTF8_FMT_ARG(
                            afw_object_meta_get_object_type_id(object, xctx))
                    );
                }
                impl_put_json_string(wa, s);
            }
            else {
                impl_put_json_string(wa, &value_data_type->data_type_id);
            }
        }
        else {
            impl_puts(wa, "null");
        }
        (wa->indent)--;
        impl_put_ws(wa);
        impl_putc(wa, ']');
    }
}


/* Convert a value to json and write it. */
void
afw_json_internal_write_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_from_value_wa_t wa;

    /* Initialize workarea and associated resources. */
    afw_memory_clear(&wa);
    wa.xctx = xctx;
    wa.p = p;
    wa.options = options;
    wa.do_ws = AFW_OBJECT_OPTION_IS(options, whitespace);
    wa.do_typed_values = AFW_OBJECT_OPTION_IS(options, typedValues);
    wa.skip_next_ws = 1;
    wa.context = context;
    wa.callback = callback;

    /* Convert object to json. */
    impl_convert_value_to_json(&wa, value);
}


/* Convert a value to json. */
AFW_DEFINE(const afw_utf8_t *)
afw_json_from_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_memory_writer_t *writer;
    const afw_memory_t *raw;

    writer = afw_memory_create_writer(p, xctx);

    afw_json_internal_write_value(value, options,
        writer->context, writer->callback, p, xctx);

    raw = afw_memory_writer_retrieve_and_release(writer, xctx);
    return afw_utf8_from_raw(raw, p, xctx);
}


AFW_DEFINE(void)
afw_json_write_encoded_string(
    const afw_utf8_t *string,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_utf8_octet_t c;
    afw_utf8_octet_t c2[2];
    afw_size_t len;
    const afw_utf8_octet_t *s;

    afw_writer_write_z(writer, "\"", xctx);

    s = string->s;
    len = string->len;

    while (len > 0) {
        c = *s;

        /* Add an extra backslash if character is backslash or quote. */
        if (c == '\\' || c == '"') {
            afw_writer_write_z(writer, "\\", xctx);
        }

        /* If not a control character, output the character. */
        if (c >= 32) {
            afw_writer_write(writer, &c, 1, xctx);
        }

        /* If \n */
        else if (c == 10) {
            afw_writer_write_z(writer, "\\n", xctx);
        }

        /* If \r */
        else if (c == 13) {
            afw_writer_write_z(writer, "\\r", xctx);
        }

        /* If \t */
        else if (c == 9) {
            afw_writer_write_z(writer, "\\t", xctx);
        }

        /* If \f */
        else if (c == 12) {
            afw_writer_write_z(writer, "\\f", xctx);
        }

        /* If \b */
        else if (c == 7) {
            afw_writer_write_z(writer, "\\b", xctx);
        }

        /*
         * If other control character, output as \u followed by character as four
         * byte hex characters.
         */
        else {
            afw_writer_write_z(writer, "\\u00", xctx);
            c2[0] = c / 16 + '0';
            c2[1] = c % 16 + '0';
            afw_writer_write(writer, &c2, 2, xctx);
        }

        len--;
        s++;
    }

    afw_writer_write_z(writer, "\"", xctx);
}



AFW_DEFINE(const afw_utf8_t *)
afw_json_utf8_string_create(
    const afw_utf8_t *string,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_string_t value;
    const afw_utf8_t *result;

    value.inf = &afw_value_evaluated_ia5String_inf;
    afw_memory_copy(&value.internal, string);

    result = afw_json_from_value(
        (const afw_value_t *)&value, NULL, p, xctx);
    
    return result;  
}

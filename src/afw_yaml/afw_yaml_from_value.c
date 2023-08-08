// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW YAML convert from value
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_yaml_from_value.c
 * @brief Convert an AFW value to YAML.
 */

#include "afw.h"
#include "afw_yaml.h"
#include <apr_buckets.h>

typedef struct from_value_wa_s {
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const afw_object_options_t *options;
    afw_boolean_t do_ws;
    int indent;
    afw_boolean_t skip_next_ws;
    void * context;
    afw_write_cb_t callback;
    int object_depth;
} from_value_wa_t;

static void put_ws(
    from_value_wa_t *wa);

static from_value_wa_t * create_from_value_wa(
    const afw_pool_t *p, afw_xctx_t *xctx);

static void put_yaml_string(
    from_value_wa_t *wa,
    const afw_utf8_t *string);

static void convert_string_to_literal_style_yaml(
    from_value_wa_t *wa,
    const afw_utf8_t *string);

static void convert_string_to_yaml(
    from_value_wa_t *wa,
    const afw_value_t *value);

static void convert_integer_to_yaml(
    from_value_wa_t *wa,
    afw_integer_t i);

static void convert_number_to_yaml(
    from_value_wa_t *wa,
    double d);

static void convert_boolean_to_yaml(
    from_value_wa_t *wa,
    afw_boolean_t b);

static void convert_list_to_yaml(
    from_value_wa_t *wa,
    const afw_array_t *list);

static void convert_object_to_yaml(
    from_value_wa_t *wa,
    const afw_object_t *obj);

static void convert_value_to_yaml(
    from_value_wa_t *wa,
    const afw_value_t *value);

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


static void impl_printf(from_value_wa_t *wa,
    const afw_utf8_z_t *format_z, ...)
{
    va_list arg;
    const afw_utf8_t *s;

    va_start(arg, format_z);
    s = afw_utf8_printf_v(format_z, arg, wa->p, wa->xctx);
    va_end(arg);
    wa->callback(wa->context, s->s, s->len, wa->p, wa->xctx);
}


void put_ws(
    from_value_wa_t *wa)
{
    int indent;

    if (wa->do_ws) {
        if (!wa->skip_next_ws) {
            impl_putc(wa, '\n');
            for (indent = 1; indent <= wa->indent; indent++) {
                impl_puts(wa, "  ");
            }
        }
        wa->skip_next_ws = 0;
    }
}

from_value_wa_t * create_from_value_wa(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    from_value_wa_t *wa;

    wa = afw_xctx_calloc_type(from_value_wa_t, xctx);
    wa->xctx = xctx;
    wa->p = p;

    return wa;
}

void put_yaml_string(
    from_value_wa_t *wa,
    const afw_utf8_t *string)
{
    unsigned char c;
    afw_size_t len;
    const afw_utf8_octet_t *s;

    s = string->s;
    len = string->len;

    while (len > 0) {
        c = *s;

        /* If not a control character, output the character. */
        if (c >= 32) {
            impl_putc(wa, c);
        }

        /*
         * If control character, output as /u followed by character as four
         * byte hex characters.
         */
        else {
            impl_printf(wa, "\\u%04x", c);
        }

        len--;
        s++;
    }
}


/*
 * This write a string using the literal block scalar style as described in
 * Chapter 8 at https://yaml.org/spec/1.2.2/#rule-c-indentation-indicator.
 */
void convert_string_to_literal_style_yaml(
    from_value_wa_t *wa,
    const afw_utf8_t *string)
{
    unsigned char c;
    afw_size_t len;
    const afw_utf8_octet_t *s;

    s = string->s;
    len = string->len;

    /* Indicate literal style. */
    impl_putc(wa, '|');

    /* If first char is space, add indentation indicator. */
    if (*s == ' ') {
        impl_printf(wa, "%d", wa->indent);
    }

    /* If string ends with a newline, use KEEP chomping indicator. */
    if (s[len - 1] == '\n') {
        impl_putc(wa, '+');
    }

    /* If string does not ends with a newline, use STRIP chomping indicator. */
    else {
        impl_putc(wa, '-');
    }

    /* Put newline and indentation. */
    put_ws(wa);

    /* Write string. */
    while (len > 0) {
        c = *s;

        /* If \n, line break and add indent. */
        if (c == '\n') {
            put_ws(wa);
        }

        /*
         * If not new line, just write character asis.
         */
        else {
            impl_putc(wa, c);
        }

        /* Increment. */
        len--;
        s++;
    }    
}



/*
 * If the string has newlines, use the literal style. If not, just use JSON's
 * representation.
 */
void convert_string_to_yaml(
    from_value_wa_t *wa,
    const afw_value_t *value)
{
    const afw_utf8_t *string;
    const afw_utf8_octet_t *c, *end;

    string = afw_value_as_utf8(value, wa->xctx->p, wa->xctx);
    if (!string) {
        AFW_THROW_ERROR_Z(general, "Error converting string.", wa->xctx);
    }

    /* If string contains a newline, write as literal style. */
    if (string->len > 0) {
        for (c = string->s, end = c + string->len; c < end; c++)
        {
            if (*c == '\n') {
                convert_string_to_literal_style_yaml(wa, string);
                return;
            }
        }
    }

    /* If there are no newlines, just write as JSON quoted string. */
    string = afw_json_utf8_string_create(string, wa->p, wa->xctx);
    impl_write(wa, string->s, string->len);
}


/*
 * Convert integer.
 */
void convert_integer_to_yaml(
    from_value_wa_t *wa,
    afw_integer_t i)
{
    impl_printf(wa, AFW_INTEGER_FMT, i);
}

/*
 * Since JSON numbers are a valid subset of YAML numbers, we'll just use
 * JSON's representation.
 *
 * FIXME:  I've read that numeric one (1) should be quoted, otherwise
 * it will be parsed as boolean true.
 */
void convert_number_to_yaml(
    from_value_wa_t *wa,
    double d)
{
    /* IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
     *
     * Make sure s is one longer than precision plus 1 for \0 plus a
     * little padding because of implementation differences that may
     * not count leading sign in precision, etc.  For %.23g, 30 is used
     * just to be safe.
     */
    char s[30]; /* 23 + 1 for \0 + some padding for sign, etc. */

    /*
     * %.23g will be able to print minus, plus 16 digits, plus period,
     * plus 'e', plus sign, plus 3 digits.
     *
     * JSON only supports finite numbers.  If not finite, return a string
     * with "NaN", "-INF", or "INF".
     */
    if (afw_number_is_finite(d)) {
        sprintf(s, "%.23G", d);
    }
    else if (afw_number_is_NaN(d)) {
        strcpy(s, "\"" AFW_JSON_Q_NAN "\"");
    }
    else if (d <= DBL_MAX) {
        strcpy(s, "\"" AFW_JSON_Q_MINUS_INFINITY "\"");
    }
    else {
        strcpy(s, "\"" AFW_JSON_Q_INFINITY "\"");
    }
    impl_puts(wa, s);
}

void convert_boolean_to_yaml(
    from_value_wa_t *wa,
    afw_boolean_t b)
{
    if (b) {
        impl_puts(wa,
            AFW_JSON_Q_PRIMITIVE_BOOLEAN_TRUE);
    }
    else {
        impl_puts(wa,
            AFW_JSON_Q_PRIMITIVE_BOOLEAN_FALSE);
    }
}

void convert_list_to_yaml(
    from_value_wa_t *wa,
    const afw_array_t *list)
{
    const afw_iterator_t *list_iterator;
    const afw_value_t *next;

    impl_puts(wa, "- ");


    list_iterator = NULL;
    next = afw_array_get_next_value(list, &list_iterator, wa->p, wa->xctx);

    while (next) {
        (wa->indent)++;
        convert_value_to_yaml(wa, next);
        (wa->indent)--;
        put_ws(wa);
        next = afw_array_get_next_value(list, &list_iterator,
            wa->p, wa->xctx);
        if (next) {
            impl_puts(wa, "- ");
        }
    }
}

void convert_object_to_yaml(
    from_value_wa_t *wa,
    const afw_object_t *obj)
{
    const afw_iterator_t *property_iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *next;
    const afw_object_t *meta;

    /* Get first property. */
    property_iterator = NULL;
    next = afw_object_get_next_property(obj, &property_iterator,
        &property_name, wa->xctx);


    /* If object has meta, convert it first. */
    meta = afw_object_meta_create_accessor_with_options(obj,
        wa->options, wa->p, wa->xctx);
    if (meta) {
        (wa->indent)++;
        put_yaml_string(wa, &afw_s__meta_);
        impl_puts(wa, ": ");
        put_ws(wa);
        convert_object_to_yaml(wa, meta);
        (wa->indent)--;
        if (next) {
            put_ws(wa);
        }
    }


    /* Add each object property. */
    if (next) {
        while (1) {
            (wa->indent)++;
            put_yaml_string(wa, property_name);

            impl_puts(wa, ": ");

            convert_value_to_yaml(wa, next);

            next = afw_object_get_next_property(obj,
                &property_iterator, &property_name, wa->xctx);

            (wa->indent)--;
            if (next)
            put_ws(wa);
            if (!next) {
                break;
            }
        }
    }

}

void convert_value_to_yaml(
    from_value_wa_t *wa,
    const afw_value_t *value)
{
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
            "Unevaluated value encountered producing yaml "
            "(" AFW_UTF8_FMT " " AFW_UTF8_FMT ")",
            AFW_UTF8_FMT_ARG(info.value_inf_id),
            AFW_UTF8_FMT_OPTIONAL_ARG(info.detail)
        );
    }

    /* Get data type. */
    value_data_type = afw_value_get_data_type(value, wa->xctx);

    /* If value is a list, convert list to yaml. */
    if (afw_value_is_array(value)) {
        put_ws(wa);
        convert_list_to_yaml(wa, ((afw_value_array_t *)value)->internal);
    }

    /* If value is object, convert object to yaml. */
    else  if (afw_value_is_object(value)) {
        put_ws(wa);
        convert_object_to_yaml(wa,
            ((afw_value_object_t *)value)->internal);
    }

    /* If value is single, process based on jsonPrimitive value of dataType. */
    else if (afw_value_is_defined_and_evaluated(value)) {

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
            convert_string_to_yaml(wa, value);
        }

        /* Primitive json type is number. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            &AFW_JSON_S_PRIMITIVE_NUMBER))
        {
            if (afw_value_is_integer(value)) {
                convert_integer_to_yaml(wa,
                    ((const afw_value_integer_t *)value)->internal);
            }
            else if (afw_value_is_double(value)) {
                convert_number_to_yaml(wa,
                    ((const afw_value_double_t *)value)->internal);
            }
            else {
                AFW_THROW_ERROR_Z(general,
                    "jsonPrimitive number not supported for this data type",
                    wa->xctx);
            }
        }

        /* Primitive json type is boolean. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            &AFW_JSON_S_PRIMITIVE_BOOLEAN))
        {
            convert_boolean_to_yaml(wa,
                ((afw_value_boolean_t *)value)->internal);
        }
    }

    /* If value is function, evaluated value and convert value to json. */
    else  if (0 /** @fixmeAFW_VALUE_INSTANCE_IS(value,Function)*/) {
        /** @fixme
        evaluated_value = afw_value_evaluate((const afw_value_t *)value,
            wa->p, wa->xctx);
        convert_value_to_yaml(wa, evaluated_value);
         */
    }

    /* If none of above then it's an error. */
    else {
        AFW_THROW_ERROR_Z(general,
            "Value type is invalid", wa->xctx);
    }
}


/* Convert a value to json and write it. */
extern void afw_yaml_internal_write_value(
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
    wa->do_ws = 1;
    wa->skip_next_ws = 0;
    wa->context = context;
    wa->callback = callback;

    /* begin the document */
    impl_puts(wa, "---");
    (wa->indent)++;

    /* Convert object to json. */
    convert_value_to_yaml(wa, value);
}



/* Convert a value to yaml. */
const afw_utf8_t * afw_yaml_from_value(
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_memory_writer_t *writer;
    const afw_memory_t *raw;

    writer = afw_memory_create_writer(p, xctx);

    afw_yaml_internal_write_value(value, NULL,
        writer->context, writer->callback, p, xctx);

    raw = afw_memory_writer_retrieve_and_release(writer, xctx);
    return afw_utf8_from_raw(raw, p, xctx);
}

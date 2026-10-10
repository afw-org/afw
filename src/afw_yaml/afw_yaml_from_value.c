// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW YAML convert from value
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_yaml_from_value.c
 * @brief Serialize adaptive values to YAML text.
 */

#include "afw.h"
#include "afw_yaml.h"

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

AFW_DEFINE_STATIC_INLINE(void) impl_putc(from_value_wa_t *wa, afw_octet_t c)
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

    wa = afw_pool_calloc_type(p, from_value_wa_t, xctx);
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
 * Write a mapping key. A name that YAML would read as itself in plain
 * style (a letter or '_', then letters, digits, '_' or '-', and not a
 * word a YAML 1.1 or 1.2 reader takes as a boolean or null) is written
 * plain; any other is JSON-quoted. Keys were always written plain, so
 * "a #b", "%p", " x", "{a}", or "- x" made a file no reader could load,
 * and "yes" or "1" read back as another type.
 */
static void
impl_put_key(
    from_value_wa_t *wa,
    const afw_utf8_t *key)
{
    static const char * const reserved[] = {
        "y", "n", "yes", "no", "on", "off", "true", "false", "null", NULL
    };
    const afw_utf8_t *quoted;
    afw_boolean_t plain;
    afw_size_t i;
    const char * const *r;
    unsigned char c;

    plain = key->len > 0;
    for (i = 0; plain && i < key->len; i++) {
        c = (unsigned char)key->s[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            c == '_' ||
            (i > 0 && ((c >= '0' && c <= '9') || c == '-'))))
        {
            plain = false;
        }
    }
    for (r = reserved; plain && *r; r++) {
        if (strlen(*r) != key->len) {
            continue;
        }
        for (i = 0; i < key->len; i++) {
            c = (unsigned char)key->s[i];
            if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';
            }
            if (c != (unsigned char)(*r)[i]) {
                break;
            }
        }
        if (i == key->len) {
            plain = false;
        }
    }

    if (plain) {
        put_yaml_string(wa, key);
    }
    else {
        quoted = afw_json_utf8_string_create(key, wa->p, wa->xctx);
        impl_write(wa, quoted->s, quoted->len);
    }
}


/*
 * Whether a string with a line break reads back exactly in the literal
 * block style this writer uses: content lines at the next indentation,
 * no indentation indicator, and strip ("|-", no final line break) or
 * clip ("|", one final line break) chomping. That needs a non-empty
 * line, a first non-empty line that does not start with a space or tab
 * (the reader takes its indentation from it), at most one final line
 * break (keep chomping wrote the indentation after the last line as one
 * more empty line), and no control character but tab (and no NEL, LS or
 * PS, which a YAML 1.1 reader takes as line breaks).
 */
static afw_boolean_t
impl_literal_style_ok(const afw_utf8_t *string)
{
    const afw_utf8_octet_t *c, *end;
    afw_boolean_t at_line_start;
    afw_boolean_t seen_content;
    unsigned char o;

    if (string->len == 0 ||
        (string->len >= 2 && string->s[string->len - 1] == '\n' &&
            string->s[string->len - 2] == '\n'))
    {
        return false;
    }

    at_line_start = true;
    seen_content = false;
    for (c = string->s, end = c + string->len; c < end; c++) {
        o = (unsigned char)*c;
        if (o == '\n') {
            at_line_start = true;
            continue;
        }
        if ((o < 0x20 && o != '\t') || o == 0x7f) {
            return false;
        }
        if (o == 0xc2 && c + 1 < end && (unsigned char)c[1] == 0x85) {
            return false;
        }
        if (o == 0xe2 && c + 2 < end && (unsigned char)c[1] == 0x80 &&
            ((unsigned char)c[2] == 0xa8 || (unsigned char)c[2] == 0xa9))
        {
            return false;
        }
        if (at_line_start && !seen_content && (o == ' ' || o == '\t')) {
            return false;
        }
        at_line_start = false;
        seen_content = true;
    }

    return seen_content;
}


/*
 * Write a string using the literal block scalar style as described in
 * Chapter 8 at https://yaml.org/spec/1.2.2/#rule-c-indentation-indicator.
 * The caller checked impl_literal_style_ok().
 */
void convert_string_to_literal_style_yaml(
    from_value_wa_t *wa,
    const afw_utf8_t *string)
{
    afw_size_t len;
    const afw_utf8_octet_t *s;
    afw_boolean_t clip;

    s = string->s;
    len = string->len;

    /*
     * Clip ("|") keeps one final line break, strip ("|-") none. The final
     * line break is written here, so it is there at the end of a
     * document too; the indentation the next line starts with is an
     * empty line, which clip drops.
     */
    clip = s[len - 1] == '\n';
    impl_puts(wa, clip ? "|" : "|-");
    if (clip) {
        len--;
    }

    /* Each line on its own line at the next indentation. */
    put_ws(wa);
    for (; len > 0; len--, s++) {
        if (*s == '\n') {
            put_ws(wa);
        }
        else {
            impl_putc(wa, *s);
        }
    }
    if (clip) {
        impl_putc(wa, '\n');
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

    string = afw_value_convert_to_utf8(value, wa->xctx->p, wa->xctx);
    if (!string) {
        AFW_THROW_ERROR_Z(general, "Error converting string.", wa->xctx);
    }

    /*
     * If string contains a newline, write as literal style when it reads
     * back exactly; else as a JSON (YAML double-quoted) string.
     */
    if (string->len > 0) {
        for (c = string->s, end = c + string->len; c < end; c++)
        {
            if (*c == '\n') {
                if (impl_literal_style_ok(string)) {
                    convert_string_to_literal_style_yaml(wa, string);
                    return;
                }
                break;
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
 * A finite double in the fewest digits that read back as the same value,
 * written as a YAML float: it always has a '.' (1e10 was written
 * 10000000000 and read back as an integer; -0.0 as -0), and an exponent
 * has a sign ("1.0E+10", which YAML 1.1 readers also take as a float).
 */
static void
impl_double_to_yaml(char *s, double d)
{
    int precision;
    char *e;
    char tail[32];

    for (precision = 1; precision < 17; precision++) {
        sprintf(s, "%.*G", precision, d);
        if (strtod(s, NULL) == d) {
            break;
        }
    }
    if (precision == 17) {
        sprintf(s, "%.17G", d);
    }

    if (!strchr(s, '.')) {
        e = strchr(s, 'E');
        if (e) {
            strcpy(tail, e);
            strcpy(e, ".0");
            strcat(s, tail);
        }
        else {
            strcat(s, ".0");
        }
    }
}


/*
 * Finite doubles are YAML floats in the fewest digits that read back
 * (impl_double_to_yaml). Non-finite values are quoted strings (NaN / INF), matching JSON helpers. YAML 1.2 Core
 * Schema does not treat plain 1 as boolean — no special-case quoting needed.
 */
void convert_number_to_yaml(
    from_value_wa_t *wa,
    double d)
{
    /*
     * At most 17 digits: sign, 17 digits, '.', "E-308", ".0" added by
     * impl_double_to_yaml, and \0 fit in 30.
     */
    char s[30];

    /*
     * JSON only supports finite numbers.  If not finite, return a string
     * with "NaN", "-INF", or "INF".
     */
    if (afw_number_is_finite(d)) {
        impl_double_to_yaml(s, d);
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
    const afw_iterator_old_t *list_iterator;
    const afw_value_t *next;

    list_iterator = NULL;
    next = afw_array_get_next_value(list, &list_iterator, wa->xctx);

    /* Empty array → flow []. */
    if (!next) {
        impl_puts(wa, "[]");
        return;
    }

    while (next) {
        impl_puts(wa, "- ");
        (wa->indent)++;
        convert_value_to_yaml(wa, next);
        (wa->indent)--;
        next = afw_array_get_next_value(list, &list_iterator, wa->xctx);
        if (next) {
            put_ws(wa);
        }
    }
}

void convert_object_to_yaml(
    from_value_wa_t *wa,
    const afw_object_t *obj)
{
    const afw_iterator_old_t *property_iterator;
    const afw_value_t *property_name;
    const afw_value_t *next;
    const afw_object_t *meta;

    /* Get first property. */
    property_iterator = NULL;
    next = afw_object_get_next_property(obj, &property_iterator,
        &property_name, wa->xctx);

    /* If object has meta, convert it first. */
    meta = afw_object_meta_create_accessor_with_options(obj,
        wa->options, wa->p, wa->xctx);

    /* Empty object with no meta → flow {}. */
    if (!meta && !next) {
        impl_puts(wa, "{}");
        return;
    }

    if (meta) {
        (wa->indent)++;
        put_yaml_string(wa, afw_s__meta_);
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
            impl_put_key(wa,
                afw_object_string_property_name_internal(
                    property_name, wa->xctx));

            impl_puts(wa, ": ");

            convert_value_to_yaml(wa, next);

            next = afw_object_get_next_property(obj,
                &property_iterator, &property_name, wa->xctx);

            (wa->indent)--;
            if (next) {
                put_ws(wa);
            }
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
            "(%ku %ku)",
            info.value_inf_id,
            info.detail
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
            AFW_JSON_S_PRIMITIVE_NULL))
        {
            impl_puts(wa, AFW_JSON_Q_PRIMITIVE_NULL);
        }

        /* Primitive json type is string. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            AFW_JSON_S_PRIMITIVE_STRING))
        {
            convert_string_to_yaml(wa, value);
        }

        /* Primitive json type is number. */
        else if (afw_utf8_equal(&value_data_type->jsonPrimitive,
            AFW_JSON_S_PRIMITIVE_NUMBER))
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
            AFW_JSON_S_PRIMITIVE_BOOLEAN))
        {
            convert_boolean_to_yaml(wa,
                ((afw_value_boolean_t *)value)->internal);
        }

        /* Evaluated but not a YAML-encodable primitive. */
        else {
            AFW_THROW_ERROR_FZ(general, wa->xctx,
                "Value data type is not supported for YAML "
                "(%ku)",
                &value_data_type->data_type_id);
        }
    }

    /* If none of above then it's an error. */
    else {
        AFW_THROW_ERROR_Z(general,
            "Value type is invalid", wa->xctx);
    }
}


/* Convert a value to YAML and write it. */
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

    /*
     * Begin the document. A scalar follows "--- " on the same line: "---"
     * must be followed by a space or a line break to start a document
     * ("---42" is the string "---42").
     */
    impl_puts(wa, "---");
    if (!value || (!afw_value_is_array(value) && !afw_value_is_object(value)))
    {
        impl_putc(wa, ' ');
    }
    (wa->indent)++;

    /* Convert value to YAML. */
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
    return afw_utf8_from_memory(raw, p, xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - String Functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_query_criteria.c
 * @brief AFW Query criteria functions.
 */

#include "afw_internal.h"
#include <libxml/xmlregexp.h>



/* ------------------------------------------------------------------------- */
/* Internal parse URL encoded RQL string typedef and declares                */
/* ------------------------------------------------------------------------- */

typedef struct impl_string_parser_s {
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    afw_query_criteria_t *criteria;
    afw_query_criteria_filter_entry_t * last;
    const afw_utf8_z_t *start;
    const afw_utf8_z_t *c;
    afw_utf8_t token;

    /* Token types. */
    enum {
        impl_token_type_unset = 0,
        impl_token_type_delimiter,
        impl_token_type_operation,
        impl_token_type_string,
        impl_token_type_end
    } token_type;

    /*
     * These two flags are needed to deal with '!' not being a delimiter
     * except when expecting an operation.
     */
    afw_boolean_t bang_is_a_delimiter;
    afw_boolean_t expect_an_operation;

} impl_string_parser_t;

static void
impl_parse_string_filter(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static void
impl_parse_string_function(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static void
impl_parse_string_sugar(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static void
impl_parse_string_term(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static void
impl_parse_string_factor(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static afw_query_criteria_filter_entry_t *
impl_parse_string_relation(
    impl_string_parser_t *parser,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static const afw_utf8_t * const *
impl_parse_string_select(impl_string_parser_t *parser);

static const afw_query_criteria_sort_entry_t *
impl_parse_string_sort(impl_string_parser_t *parser);

static const afw_utf8_t *
impl_decode_token(impl_string_parser_t *parser);

static const afw_value_t *
impl_token_to_value(impl_string_parser_t *parser);

static void
impl_get_token(impl_string_parser_t *parser);

static afw_boolean_t
impl_is_delimiter(afw_utf8_octet_t ch);

static const afw_value_t *
impl_parse_string_list_value(impl_string_parser_t *parser);

static void
impl_set_alt(afw_query_criteria_filter_entry_t *entry);




/* ------------------------------------------------------------------------- */
/* Internal parse object typedef and declares                                */
/* ------------------------------------------------------------------------- */

typedef struct impl_AdaptiveQueryCriteria_object_parser_s {
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    afw_query_criteria_t *criteria;
    afw_query_criteria_filter_entry_t *last;
} impl_AdaptiveQueryCriteria_object_parser_t;

static void
impl_AdaptiveQueryCriteria_object_parse_filter(
    impl_AdaptiveQueryCriteria_object_parser_t *parser,
    const afw_object_t *filter_object,
    const afw_query_criteria_filter_entry_t **filter,
    const afw_query_criteria_filter_entry_t **tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false);

static const afw_utf8_t * const *
impl_AdaptiveQueryCriteria_object_parse_select(
    impl_AdaptiveQueryCriteria_object_parser_t *parser,
    const afw_array_t *select);

static const afw_query_criteria_sort_entry_t *
impl_AdaptiveQueryCriteria_object_parse_sort(
    impl_AdaptiveQueryCriteria_object_parser_t *parser,
    const afw_array_t *sort);


/* ------------------------------------------------------------------------- */
/* Internal common typedefs an consts                                        */
/* ------------------------------------------------------------------------- */


typedef struct impl_op_s {
    const afw_utf8_t *name;
    afw_query_criteria_filter_op_id_t op_id;
    afw_boolean_t is_list;
} impl_op_t;

typedef struct impl_rql_op_s {
    const afw_utf8_t *name;
    const impl_op_t *op;
    afw_boolean_t can_be_property;
} impl_rql_op_t;

typedef struct impl_fiql_op_s {
    const afw_utf8_z_t *fiql_z;
    const impl_op_t *op;
} impl_fiql_op_t;


static const impl_op_t
impl_op_and = {
    afw_s_and,
    afw_query_criteria_filter_op_id_and,
    false
};

static const impl_op_t
impl_op_or = {
    afw_s_or,
    afw_query_criteria_filter_op_id_or,
    false
};

static const impl_op_t
impl_op_in = {
    afw_s_in,
    afw_query_criteria_filter_op_id_in,
    true
};

static const impl_op_t
impl_op_out = {
    afw_s_out,
    afw_query_criteria_filter_op_id_out,
    true
};

static const impl_op_t
impl_op_contains = {
    afw_s_contains,
    afw_query_criteria_filter_op_id_contains,
    false
};

static const impl_op_t
impl_op_excludes = {
    afw_s_excludes,
    afw_query_criteria_filter_op_id_excludes,
    false
};

static const impl_op_t
impl_op_match = {
    afw_s_match,
    afw_query_criteria_filter_op_id_match,
    false
};

static const impl_op_t
impl_op_differ = {
    afw_s_differ,
    afw_query_criteria_filter_op_id_differ,
    false
};

static const impl_op_t
impl_op_eq = {
    afw_s_eq,
    afw_query_criteria_filter_op_id_eq,
    false
};

static const impl_op_t
impl_op_lt = {
    afw_s_lt,
    afw_query_criteria_filter_op_id_lt,
    false
};

static const impl_op_t
impl_op_le = {
    afw_s_le,
    afw_query_criteria_filter_op_id_le,
    false
};

static const impl_op_t
impl_op_gt = {
    afw_s_gt,
    afw_query_criteria_filter_op_id_gt,
    false
};

static const impl_op_t
impl_op_ge = {
    afw_s_ge,
    afw_query_criteria_filter_op_id_ge,
    false
};

static const impl_op_t
impl_op_ne = {
    afw_s_ne,
    afw_query_criteria_filter_op_id_ne,
    false
};



static const impl_rql_op_t
impl_rql_op[] = {

    {
        afw_s_and,
        &impl_op_and,
        true
    },

    {
        afw_s_or,
        &impl_op_or,
        true
    },

    {
        afw_s_in,
        &impl_op_in,
        true
    },

    {
        afw_s_out,
        &impl_op_out,
        true
    },

    {
        afw_s_contains,
        &impl_op_contains,
        true
    },

    {
        afw_s_excludes,
        &impl_op_excludes,
        true
    },

    {
        afw_s_match,
        &impl_op_match,
        true
    },

    {
        afw_s_differ,
        &impl_op_differ,
        true
    },

    {
        afw_s_eq,
        &impl_op_eq,
        true
    },

    {
        afw_s_lt,
        &impl_op_lt,
        true
    },

    {
        afw_s_le,
        &impl_op_le,
        true
    },

    {
        afw_s_lte,
        &impl_op_le,
        false
    },

    {
        afw_s_gt,
        &impl_op_gt,
        true
    },

    {
        afw_s_ge,
        &impl_op_ge,
        true
    },

    {
        afw_s_gte,
        &impl_op_ge,
        false
    },

    {
        afw_s_ne,
        &impl_op_ne,
        true
    }
};

/* Operations that can be in a relation. */
static const impl_fiql_op_t
impl_fiql_op[] = {
    {
        "=",
        &impl_op_eq
    },
    {
        "==",
        &impl_op_eq
    },
    {
        "=eq=",
        &impl_op_eq
    },
    {
        "=ne=",
        &impl_op_ne
    },
    {
        "!=",
        &impl_op_ne
    },
    {
        "=lt=",
        &impl_op_lt
    },
    {
        "=le=",
        &impl_op_le
    },
    {
        "=lte=",
        &impl_op_le
    },
    {
        "=gt=",
        &impl_op_gt
    },
    {
        "=ge=",
        &impl_op_ge
    },
    {
        "=gte=",
        &impl_op_ge
    },
    {
        "=match=",
        &impl_op_match
    },
    {
        "=differ=",
        &impl_op_differ
    },
    {
        "=contains=",
        &impl_op_contains
    },
    {
        "=in=",
        &impl_op_in
    },
    {
        "=out=",
        &impl_op_out
    },
    {NULL}
};

/* Delimiters.  Notice last char in string is a space. */
static const afw_utf8_octet_t * impl_delimiter =  "&=<>()|,;+# ";


/* ------------------------------------------------------------------------- */
/* Internal string parser error handling                                     */
/* ------------------------------------------------------------------------- */


/* Parse error. */
static void
impl_string_parser_set_error_z(
    impl_string_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *message_z)
{
    afw_error_set_fz(afw_error_code_syntax,
        source_z, parser->xctx,
        "Query string error at offset +" AFW_SIZE_T_FMT
        ": %s",
        parser->c - parser->start - parser->token.len,
        message_z);
}


/* Parse error. */
static void
impl_string_parser_set_error_fz(
    impl_string_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z, ...)
{
    va_list ap;
    const afw_utf8_z_t *message_z;

    va_start(ap, format_z);
    message_z = afw_utf8_z_printf_v(format_z, ap,
        parser->p, parser->xctx);
    va_end(ap);

    impl_string_parser_set_error_z(parser, source_z, message_z);
}


/*  Macro used to set parse error in xctx and throw it. */
#define IMPL_STRING_THROW_ERROR_Z(message_z) \
do { \
    impl_string_parser_set_error_z(parser, \
        AFW__FILE_LINE__, message_z); \
    longjmp(((parser->xctx)->current_try->throw_jmp_buf), \
        (afw_error_code_syntax)); \
} while (0)


/*  Macro used to set parse error in xctx and throw it. */
#define IMPL_STRING_THROW_ERROR_FZ(format_z, ...) \
do { \
    impl_string_parser_set_error_fz(parser, \
        AFW__FILE_LINE__, format_z, __VA_ARGS__); \
    longjmp(((parser->xctx)->current_try->throw_jmp_buf), \
        (afw_error_code_syntax)); \
} while (0)




/* ------------------------------------------------------------------------- */
/* Internal parse URL encoded RQL string function defines                    */
/* ------------------------------------------------------------------------- */

AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
impl_token_equal(
    impl_string_parser_t *parser, const afw_utf8_t *s)
{
    return afw_utf8_equal(&parser->token, s);
}


AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
impl_token_equal_z(
    impl_string_parser_t *parser, const afw_utf8_z_t *z)
{
    return afw_utf8_equal_utf8_z(&parser->token, z);
}


AFW_DEFINE_STATIC_INLINE(void)
impl_token_copy(
    impl_string_parser_t *parser, afw_utf8_t *to)
{
    memcpy(to, &parser->token, sizeof(afw_utf8_t));
}


/* Determine if character is a delimiter (includes 0).  */
static afw_boolean_t
impl_is_delimiter(afw_utf8_octet_t ch)
{
    const afw_utf8_octet_t * delimiter;

    delimiter = impl_delimiter;
    while (1) {
        if (*delimiter == ch) return true;
        if (*delimiter == 0) break;
        delimiter++;
    }

    return false;
}


/* Get next token.  Token pointer and size are set in parser. */
static void
impl_get_token(impl_string_parser_t *parser)
{
    const afw_utf8_octet_t *c;

    /* Catch if being called after end. */
    if (parser->token_type == impl_token_type_end) {
        IMPL_STRING_THROW_ERROR_Z(
            "call to impl_get_token() after end");
    }

    /* Set token start. */
    parser->token.s = parser->c;

    /* If at end, token length is 0 and return. */
    if (*(parser->c) == 0 || *(parser->c) == '#') {
        parser->token.len = 0;
        parser->token_type = impl_token_type_end;
        return;
    }

    /*
     * If next char is '=', token is either '=' or an operator enclose in '='s.
     * Set token size accordingly to include '='s.  See impl_fiql_op[].
     */
    if (parser->expect_an_operation &&
        (*(parser->c) == '=' || *(parser->c) == '!'))
    {
        parser->token_type = impl_token_type_operation;
        parser->token.len = 1;
        c = parser->c + 1;
        if (*(parser->c) == '=') {
            while (1) {
                if (*c == '=') {
                    parser->token.len = c - parser->c + 1;
                    break;
                }
                if (impl_is_delimiter(*c)) break;
                c++;
            }
        }
        else {
            if (*c == '=') {
                parser->token.len = 2;
            }
        }
    }

    /* Else if cursor points to a delimiter, set token size to 1. */
    else if (impl_is_delimiter(*(parser->c))) {
        parser->token_type = impl_token_type_delimiter;
        parser->token.len = 1;
    }

    /* Else token is characters up to next delimiter. */
    else {
        for (c = parser->c;
            !impl_is_delimiter(*c) &&
            (!parser->bang_is_a_delimiter || *c != '!');
            c++);
        parser->token.len = c - parser->c;
        parser->token_type = impl_token_type_string;
    }

    /* Update cursor past token. */
    parser->c += parser->token.len;

    /* Reset bang_is_a_delimiter and expect_an_operation. */
    parser->bang_is_a_delimiter = false;
    parser->expect_an_operation = false;
}


static const afw_utf8_t *
impl_decode_token(impl_string_parser_t *parser) {
    return afw_uri_decode_create(parser->token.s, parser->token.len,
        parser->p, parser->xctx);
}


static const afw_value_t *
impl_token_to_value(impl_string_parser_t *parser)
{
    const afw_utf8_t *decoded;

    decoded = afw_uri_decode_create(parser->token.s, parser->token.len,
        parser->p, parser->xctx);

    return afw_value_create_unmanaged_string(decoded, parser->p, parser->xctx);
}


static void
impl_set_alt(afw_query_criteria_filter_entry_t *entry)
{
    entry->alt_op_id = entry->op_id;
    entry->alt_not = true;
    switch (entry->op_id) {

    case afw_query_criteria_filter_op_id_ne:
        entry->alt_op_id = afw_query_criteria_filter_op_id_eq;
        break;

    case afw_query_criteria_filter_op_id_ge:
        entry->alt_op_id = afw_query_criteria_filter_op_id_lt;
        break;

    case afw_query_criteria_filter_op_id_gt:
        entry->alt_op_id = afw_query_criteria_filter_op_id_le;
        break;

    case afw_query_criteria_filter_op_id_differ:
        entry->alt_op_id = afw_query_criteria_filter_op_id_match;
        break;

    case afw_query_criteria_filter_op_id_excludes:
        entry->alt_op_id = afw_query_criteria_filter_op_id_contains;
        break;

    case afw_query_criteria_filter_op_id_out:
        entry->alt_op_id = afw_query_criteria_filter_op_id_in;
        break;

    case afw_query_criteria_filter_op_id_and:
    case afw_query_criteria_filter_op_id_or:
        entry->alt_op_id = afw_query_criteria_filter_op_id_na;
    default:
        entry->alt_not = false;
    }
}


/* Parse sort */
static const afw_query_criteria_sort_entry_t *
impl_parse_string_sort(impl_string_parser_t *parser)
{
    afw_query_criteria_sort_entry_t *entry;
    afw_query_criteria_sort_entry_t *previous_entry = NULL;
    afw_query_criteria_sort_entry_t *first_entry = NULL;

    /* Loop processing all sort entries. */
    while (1) {

        /* Allocate memory for entry. */
        entry = afw_pool_calloc_type(parser->p,
            afw_query_criteria_sort_entry_t,
            parser->xctx);

        /* Get next token. */
        impl_get_token(parser);

        /* If the token is '-' or '+', indicate in entry and adjust token. */
        if (*(parser->token.s) == '-') {
            entry->descending = true;
            (parser->token.s)++;
            (parser->token.len)--;
        }
        else if (*(parser->token.s) == '+') {
            impl_get_token(parser);
        }

        /* If encoded '+' or '-', indicate in entry and adjust token. */
        else if (*(parser->token.s) == '%' &&
            parser->token.len > 3 &&
            *(parser->token.s + 1) == '2')
        {
            /* Encoded '+' */
            if (*(parser->token.s + 2) == 'b' ||
                *(parser->token.s + 2) == 'B')
            {
                parser->token.s += 3;
                parser->token.len -= 3;
            }

            /* Encoded '-' */
            else if (*(parser->token.s + 2) == 'd' ||
                *(parser->token.s + 2) == 'D')
            {
                entry->descending = true;
                parser->token.s += 3;
                parser->token.len -= 3;
            }
        }

        /* It's an error if token is not a string. */
        if (parser->token_type != impl_token_type_string) {
            IMPL_STRING_THROW_ERROR_Z("Invalid sort value");
        }

        /* Get property name from token. */
        entry->property_name = impl_decode_token(parser);
        if (parser->criteria->object_type && entry->property_name) {
            entry->pt = afw_object_type_property_type_get(
                parser->criteria->object_type,
                entry->property_name, parser->xctx);
            if (!entry->pt || !entry->pt->allow_query) {
                IMPL_STRING_THROW_ERROR_FZ(
                    "Property " AFW_UTF8_FMT_Q " cannot be queried",
                    AFW_UTF8_FMT_ARG(entry->property_name));
            }
        }

        /** @fixme check for queryable. */

        /* Set first and previous entry pointer. */
        if (!previous_entry) {
            first_entry = entry;
        }
        else {
            previous_entry->next = entry;
        }
        previous_entry = entry;

        /*
         * Get next token.  If it is ')', then finished.  If ',' or ' ',
         * then continue. Anything else is an error.
         */
        impl_get_token(parser);
        if (*(parser->token.s) == ')') break;
        if (*(parser->token.s) == ',' || *(parser->token.s) == ' ') continue;
        IMPL_STRING_THROW_ERROR_Z("Invalid sort value");
    }

    /* Return first entry. */
    return first_entry;
}


/* Parse select */
static const afw_utf8_t * const *
impl_parse_string_select(impl_string_parser_t *parser)
{
    afw_const_utf8_a_stack_t *names;
    const afw_utf8_t *property_name;
    static const afw_utf8_t * const *result;

    /* Make an array to hold zero terminated list of property names. */
    names = afw_stack_create(afw_const_utf8_a_stack_t, 10, 0, true,
        parser->p, parser->xctx);

    /* Loop processing all select entries. */
    while (1) {

        /* Get a token.  It must be a string. */
        impl_get_token(parser);
        if (parser->token_type != impl_token_type_string) {
            IMPL_STRING_THROW_ERROR_Z("Invalid property name");
        }

        /* Get copy of property name from token and push pointer on array. */
        property_name = impl_decode_token(parser);
        /** @fixme check for valid. */
        afw_stack_push(names, parser->xctx) = property_name;

        /*
         * Get next token.  If it is ')', then finished.  If ',' or ' ',
         * then continue. Anything else is an error.
         */
        impl_get_token(parser);
        if (*(parser->token.s) == ')') break;
        if (*(parser->token.s) == ',' || *(parser->token.s) == ' ') continue;
        IMPL_STRING_THROW_ERROR_Z("Invalid select value");
    }

    /* Return NULL terminated list of select names. */
    afw_stack_push(names, parser->xctx) = NULL;

    /* Get copy of stack, release instance, and return copy. */
    afw_stack_copy_and_release(names, NULL, &result,
        parser->p, parser->xctx);
    return result;
}


/* Parse relation list value up to ')'. */
static const afw_value_t *
impl_parse_string_list_value(impl_string_parser_t *parser)
{
    afw_utf8_t sign;
    apr_array_header_t *values;
    const afw_utf8_t *value;
    const afw_array_t *list;
    const afw_value_t *result;

    /** @fixme Change to use new list create function. */
    /* Make an array to hold zero terminated list of values. */
    values = apr_array_make(afw_pool_get_apr_pool(parser->p),
        10, sizeof(afw_utf8_t));

    /* Loop though list values. */
    while (1) {

        /* Get next token. */
        impl_get_token(parser);

        /* If token is '+' or '-', remember sign and get another token. */
        memset(&sign, 0, sizeof(afw_utf8_t));
        if (*(parser->token.s) == '+' || *(parser->token.s) == '-') {
            sign.s = parser->token.s;
            sign.len = parser->token.len;
            impl_get_token(parser);
        }

        /* Token should be a string. */
        if (parser->token_type != impl_token_type_string) {
            IMPL_STRING_THROW_ERROR_Z("Invalid list item in relation");
        }

        /*
         * Get copy of value from token and push pointer on array. If there
         * is a sign, add it to front of value.
         */
        value = impl_decode_token(parser);
        if (sign.s) {
            value = afw_utf8_concat(parser->p, parser->xctx, &sign, value, NULL);
        }
        memcpy(apr_array_push(values), value, sizeof(afw_utf8_t));

        /* Get next token. */
        impl_get_token(parser);

        /* If one of the separator, continue. */
        if (*(parser->token.s) == ',')
        {
            continue;
        }

        /* If next token will be ')', increment cursor and break. */
        if (*(parser->token.s) == ')') {
            break;
        }
    }

    /* Return list of strings. */
    list = afw_array_create_wrapper_for_array(
        values->elts, false, afw_data_type_string,
        values->nelts, parser->p, parser->xctx);
    result = afw_value_create_unmanaged_array(list, parser->p, parser->xctx);
    return result;
}


/* Parse relation */
static afw_query_criteria_filter_entry_t *
impl_parse_string_relation(
    impl_string_parser_t *parser,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    afw_query_criteria_filter_entry_t *entry;
    const impl_fiql_op_t *o;
    const afw_utf8_t *decoded = NULL;
    const xmlChar *s_z;

    /* Allocate memory for entry */
    entry = afw_pool_calloc_type(parser->p, afw_query_criteria_filter_entry_t,
        parser->xctx);
    if (parser->last) {
        parser->last->next = entry;
    }
    parser->last = entry;

    /* Set on_true and on_false in entry to parameter values. */
    entry->on_true = on_true;
    entry->on_false = on_false;

    /* Get property name and check if query allowed. */
    parser->bang_is_a_delimiter = true;
    impl_get_token(parser);
    if (parser->token_type != impl_token_type_string) {
        IMPL_STRING_THROW_ERROR_Z(
            "Invalid or missing property name in relation");
    }
    entry->property_name = impl_decode_token(parser);
    if (parser->criteria->object_type && entry->property_name) {
        entry->pt = afw_object_type_property_type_get(
            parser->criteria->object_type,
            entry->property_name, parser->xctx);
        if (!entry->pt || !entry->pt->allow_query) {
            IMPL_STRING_THROW_ERROR_FZ(
                "Property " AFW_UTF8_FMT_Q " cannot be queried",
                AFW_UTF8_FMT_ARG(entry->property_name));
        }
    }

    /* Get operation. Operation should not be URL encoded. */
    parser->expect_an_operation = true;
    impl_get_token(parser);
    if (parser->token_type != impl_token_type_operation) {
        IMPL_STRING_THROW_ERROR_Z(
            "Invalid or missing operation in relation");
    }

    /* Find entry in impl_fiql_op[] for operation. */
    for (o = &impl_fiql_op[0]; o->fiql_z; o++) {
        if (strlen(o->fiql_z) == parser->token.len &&
            memcmp(parser->token.s, o->fiql_z, parser->token.len) == 0)
        {
            break;
        }
    }
    if (!o->fiql_z) {
        IMPL_STRING_THROW_ERROR_Z(
            "Invalid operation in relation");
    }
    entry->op_name = o->op->name;
    entry->op_id = o->op->op_id;
    impl_set_alt(entry);

    /* Single value */
    if (!o->op->is_list) {
        impl_get_token(parser);
        if (parser->token_type != impl_token_type_string) {
            IMPL_STRING_THROW_ERROR_Z("Invalid value in relation");
        }
        decoded = impl_decode_token(parser);
        entry->value = afw_value_create_unmanaged_string(
            decoded, parser->p, parser->xctx);
    }

    /* List value. */
    else {
        /* Get token and expect it to be '(' */
        impl_get_token(parser);
        if (*(parser->token.s) != '(') {
            IMPL_STRING_THROW_ERROR_Z("Expecting '('  in relation");
        }
        entry->value = impl_parse_string_list_value(parser);
    }

    /* If match, compile expression. */
    if (entry->op_id == afw_query_criteria_filter_op_id_match) {
        s_z = BAD_CAST afw_utf8_to_utf8_z(decoded, parser->p, parser->xctx);
        entry->op_specific = xmlRegexpCompile(s_z);
        if (entry->op_specific == NULL) {
            IMPL_STRING_THROW_ERROR_Z("regexp syntax error");
        }
    }

    /* Return entry. */
    return entry;
}


/* Parse factor */
static void
impl_parse_string_factor(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    if (*(parser->c) == '(') {
        (parser->c)++;
        impl_parse_string_sugar(parser, filter, tree,
            on_true, on_false);
        if (*(parser->c) != ')') {
            IMPL_STRING_THROW_ERROR_Z("missing ')'");
        }
        (parser->c)++;
    }
    else {
        *filter = impl_parse_string_relation(parser, on_true, on_false);
        *tree = *filter;
    }
}


static void
impl_patch_on(
    afw_query_criteria_filter_entry_t *start_entry,
    const afw_query_criteria_filter_entry_t *old_on,
    afw_query_criteria_filter_entry_t *new_on)
{

    afw_query_criteria_filter_entry_t *entry;

    for (entry = (afw_query_criteria_filter_entry_t *)start_entry;
        entry && entry != new_on;
        entry = (afw_query_criteria_filter_entry_t *)entry->next)
    {
        if (entry->on_true == old_on) {
            entry->on_true = new_on;
        }
        if (entry->on_false == old_on) {
            entry->on_false = new_on;
        }
    }

}


/* Parse term */
static void 
impl_parse_string_term(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    afw_query_criteria_filter_entry_t *new_entry;
    afw_query_criteria_filter_entry_t *previous_entry;
    afw_query_criteria_filter_entry_t *new_tree;
    afw_query_criteria_filter_entry_t *previous_normalized;
    afw_query_criteria_filter_entry_t *and_entry;

    impl_parse_string_factor(parser, filter, tree, on_true, on_false);

    if (*(parser->c) == '&' || *(parser->c) == ';') {
        new_entry = *filter;
        and_entry = afw_pool_calloc_type(parser->p,
            afw_query_criteria_filter_entry_t,
            parser->xctx);
        and_entry->op_name = afw_s_and;
        and_entry->op_id = afw_query_criteria_filter_op_id_and;
        and_entry->first_conjunctive_child = *tree;
        new_tree = *tree;
        *tree = and_entry;
        do {
            (parser->c)++;
            previous_entry = new_entry;
            previous_normalized = new_tree;
            impl_parse_string_factor(parser, &new_entry, &new_tree,
                on_true, on_false);
            impl_patch_on(previous_entry, on_true, new_entry);
            previous_normalized->next_conjunctive_sibling = new_tree;
        } while (*(parser->c) == '&' || *(parser->c) == ';');
    }
}


/* Parse sugar */
static void 
impl_parse_string_sugar(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    afw_query_criteria_filter_entry_t *new_entry;
    afw_query_criteria_filter_entry_t *previous_entry;
    afw_query_criteria_filter_entry_t *new_tree;
    afw_query_criteria_filter_entry_t *previous_normalized;
    afw_query_criteria_filter_entry_t *or_entry;

    impl_parse_string_term(parser, filter, tree, on_true, on_false);

    if (*(parser->c) == '|' || *(parser->c) == ',') {
        new_entry = *filter;
        or_entry = afw_pool_calloc_type(
            parser->p,
            afw_query_criteria_filter_entry_t,
            parser->xctx);
        or_entry->op_name = afw_s_or;
        or_entry->op_id = afw_query_criteria_filter_op_id_or;
        or_entry->first_conjunctive_child = *tree;
        new_tree = *tree;
        *tree = or_entry;
        do {
            (parser->c)++;
            previous_entry = new_entry;
            previous_normalized = new_tree;
            impl_parse_string_term(parser, &new_entry, &new_tree,
                on_true, on_false);
            impl_patch_on(previous_entry, on_false, new_entry);
            previous_normalized->next_conjunctive_sibling = new_tree;
        } while (*(parser->c) == '|' || *(parser->c) == ',');
    }
}


static const impl_rql_op_t *
impl_find_rql_op(const afw_utf8_t *op_name)
{
    const impl_rql_op_t *result;

    for (
        result = &impl_rql_op[0];
        (char *)result < (char *)&impl_rql_op[0] + sizeof(impl_rql_op);
        result++)
    {
        if (afw_utf8_equal(op_name, result->name)) {
            return result;
        }
    }

    return NULL;
}


/* Parse tree */
static void
impl_parse_string_function(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    const impl_rql_op_t *rql_op;
    afw_query_criteria_filter_entry_t *entry;
    afw_query_criteria_filter_entry_t *child_entry;
    afw_query_criteria_filter_entry_t *child_tree;
    afw_query_criteria_filter_entry_t *previous_entry;
    afw_query_criteria_filter_entry_t *previous_tree;
    const afw_array_t *list;

    /* Find impl_rql_op_t for this operator. */
    impl_get_token(parser);
    rql_op = impl_find_rql_op(&parser->token);
    if (!rql_op) {
        IMPL_STRING_THROW_ERROR_FZ(
            "Operator " AFW_UTF8_FMT_Q " is not valid",
            AFW_UTF8_FMT_ARG(&parser->token));
    }

    /* Next must be an '('. */
    impl_get_token(parser);
    if (!impl_token_equal_z(parser, "(")) {
        IMPL_STRING_THROW_ERROR_Z("Expecting '(' after operator");
    }

    /* Allocate and initialize new filter entry. */
    entry = afw_pool_calloc_type(
        parser->p,
        afw_query_criteria_filter_entry_t,
        parser->xctx);
    *tree = entry;
    if (parser->last) {
        parser->last->next = entry;
    }
    parser->last = entry;
    entry->op_name = rql_op->op->name;
    entry->op_id = rql_op->op->op_id;
    impl_set_alt(entry);

    /* Process and/or. */
    if (entry->op_id == afw_query_criteria_filter_op_id_and ||
        entry->op_id == afw_query_criteria_filter_op_id_or)
    {
        for (
            previous_entry = previous_tree = NULL
            ;
            ; previous_entry = child_entry, previous_tree = child_tree)
        {
            impl_parse_string_function(parser,
                &child_entry,
                &child_tree,
                on_true, on_false);

            if (!previous_entry) {
                entry->first_conjunctive_child = child_tree;
                *filter = child_entry;
            }
            else {
                previous_tree->next_conjunctive_sibling = child_tree;

                /* If "and", patch previous entry true to new entry. */
                if (entry->op_id == afw_query_criteria_filter_op_id_and) {
                    impl_patch_on(previous_entry, previous_entry->on_true,
                        child_entry);
                }

                /* If "or", patch previous entry false to new entry. */
                else {
                    impl_patch_on(previous_entry, previous_entry->on_false,
                        child_entry);
                }
            }

            /* Next token is ',' to continue or ')' to end. */
            impl_get_token(parser);
            if (impl_token_equal_z(parser, ")")) {
                break;
            }
            if (!impl_token_equal_z(parser, ",")) {
                IMPL_STRING_THROW_ERROR_Z("Expecting ','");
            }
        }
    }

    /* Process comparisons. */
    else {
        entry->on_true = on_true;
        entry->on_false = on_false;
        *filter = entry;

        /* Get property name and make sure query allowed. */
        impl_get_token(parser);
        if (parser->token_type != impl_token_type_string) {
            IMPL_STRING_THROW_ERROR_Z(
                "Expecting property name as first parameter of operator");
        }
        entry->property_name = impl_decode_token(parser);
        if (parser->criteria->object_type && entry->property_name) {
            entry->pt = afw_object_type_property_type_get(
                parser->criteria->object_type,
                entry->property_name, parser->xctx);
            if (!entry->pt || !entry->pt->allow_query) {
                IMPL_STRING_THROW_ERROR_FZ(
                    "Property " AFW_UTF8_FMT_Q " cannot be queried",
                    AFW_UTF8_FMT_ARG(entry->property_name));
            }
        }

        /* Get ',' */
        impl_get_token(parser);
        if (!impl_token_equal_z(parser, ",")) {
            IMPL_STRING_THROW_ERROR_Z("Expecting ','");
        }

        /* Single value */
        if (!rql_op->op->is_list) {
            impl_get_token(parser);
            if (parser->token_type != impl_token_type_string) {
                IMPL_STRING_THROW_ERROR_Z("Invalid value in relation");
            }
            entry->value = impl_token_to_value(parser);
            impl_get_token(parser);
        }

        /* List value. */
        else {
            list = afw_array_create_generic(parser->p, parser->xctx);
            entry->value = afw_value_create_unmanaged_array(
                list, parser->p, parser->xctx);
            for (;;) {
                impl_get_token(parser);
                if (parser->token_type != impl_token_type_string) {
                    IMPL_STRING_THROW_ERROR_Z("Expecting string value");
                }
                afw_array_add_value(list,
                    impl_token_to_value(parser), parser->xctx);

                impl_get_token(parser);
                if (impl_token_equal_z(parser, ")")) {
                    break;
                }
                if (!impl_token_equal_z(parser, ",")) {
                    IMPL_STRING_THROW_ERROR_Z("Expecting ','");
                }
            }

            /* Make sure it ends with ')' */
            if (!impl_token_equal_z(parser, ")")) {
                IMPL_STRING_THROW_ERROR_Z("Expecting ')'");
            }
        }
    }
}


/* Parse filter */
static void
impl_parse_string_filter(
    impl_string_parser_t *parser,
    afw_query_criteria_filter_entry_t ** filter,
    afw_query_criteria_filter_entry_t ** tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    const afw_utf8_z_t *c;

    /* Save cursor. */
    c = parser->c;

    /*
     * If next tokens are a string followed by '(', restore cursor and parser
     * tree.
     */
    impl_get_token(parser);
    if (parser->token_type == impl_token_type_string) {
        impl_get_token(parser);
        if (afw_utf8_equal_utf8_z(&parser->token, "(")) {
            parser->c = c;
            impl_parse_string_function(parser, filter, tree,
                on_true, on_false);
            return;
        }      
    }

    /* Otherwise, restore cursor and parser sugar version. */
    parser->c = c;
    impl_parse_string_sugar(parser, filter, tree, on_true, on_false);
}


/* ------------------------------------------------------------------------- */
/* Internal parse object function defines                                    */
/* ------------------------------------------------------------------------- */

static void
impl_AdaptiveQueryCriteria_object_parse_filter(
    impl_AdaptiveQueryCriteria_object_parser_t *parser,
    const afw_object_t *filter_object,
    const afw_query_criteria_filter_entry_t **filter,
    const afw_query_criteria_filter_entry_t **tree,
    const afw_query_criteria_filter_entry_t *on_true,
    const afw_query_criteria_filter_entry_t *on_false)
{
    const impl_rql_op_t *rql_op;
    afw_query_criteria_filter_entry_t *entry;
    afw_query_criteria_filter_entry_t *child_entry;
    afw_query_criteria_filter_entry_t *child_tree;
    afw_query_criteria_filter_entry_t *previous_entry;
    afw_query_criteria_filter_entry_t *previous_tree;
    const afw_iterator_t *iterator;
    const afw_array_t *filters_list;
    const afw_object_t *child_object;
    const afw_value_t *value;
    const afw_utf8_t *s;

    /* Get "op" property and get its corresponding operator. */
    s = afw_object_old_get_property_as_string(filter_object,
        afw_s_op, parser->xctx);
    if (!s) {
        AFW_THROW_ERROR_Z(general, "Missing \"op\" property", parser->xctx);
    }
    rql_op = impl_find_rql_op(s);
    if (!rql_op) {
        AFW_THROW_ERROR_FZ(general, parser->xctx,
            "Property \"op\" value is not valid: " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(s));
    }

    /* Allocate and initialize new filter entry. */
    entry = afw_pool_calloc_type(
        parser->p,
        afw_query_criteria_filter_entry_t,
        parser->xctx);
    *filter = NULL;
    *tree = entry;
    if (parser->last) {
        parser->last->next = entry;
    }
    parser->last = entry;
    entry->op_name = rql_op->op->name;
    entry->op_id = rql_op->op->op_id;
    impl_set_alt(entry);

    /* Process and/or. */
    if (entry->op_id == afw_query_criteria_filter_op_id_and ||
        entry->op_id == afw_query_criteria_filter_op_id_or)
    {
        filters_list = afw_object_old_get_property_as_array(filter_object,
            afw_s_filters, parser->xctx);
        if (!filters_list) {
            AFW_THROW_ERROR_Z(general,
                "Property \"filters\" for this op",
                parser->xctx);
        }

        for (
            previous_entry = previous_tree = NULL,
            iterator = NULL
            ;
            ; previous_entry = child_entry, previous_tree = child_tree)
        {
            value = afw_array_get_next_value(filters_list, &iterator,
                parser->p, parser->xctx);
            if (!value) {
                break;
            }
            if (!afw_value_is_object(value)) {
                AFW_THROW_ERROR_Z(general,
                    "Property \"filters\" list entries must be objects",
                    parser->xctx);
            }
            child_object = afw_value_as_object(value, parser->xctx);

            impl_AdaptiveQueryCriteria_object_parse_filter(
                parser, child_object,
                (const afw_query_criteria_filter_entry_t **)&child_entry,
                (const afw_query_criteria_filter_entry_t **)&child_tree,
                on_true, on_false);

            if (!previous_entry) {
                entry->first_conjunctive_child = child_tree;
                *filter = child_entry;
            }
            else {
                previous_tree->next_conjunctive_sibling = child_tree;

                /* If "and", patch previous entry true to new entry. */
                if (entry->op_id == afw_query_criteria_filter_op_id_and) {
                    impl_patch_on(previous_entry, previous_entry->on_true,
                        child_entry);
                }                
                
                /* If "or", patch previous entry false to new entry. */
                else {
                    impl_patch_on(previous_entry, previous_entry->on_false,
                        child_entry);
                }
            }
        }
    }

    /* Process comparisons. */
    else {
        entry->on_true = on_true;
        entry->on_false = on_false;
        *filter = entry;
        entry->property_name = afw_object_old_get_property_as_string(
            filter_object, afw_s_property, parser->xctx);
        if (parser->criteria->object_type && entry->property_name) {
            entry->pt = afw_object_type_property_type_get(
                parser->criteria->object_type,
                entry->property_name, parser->xctx);
            if (!entry->pt || !entry->pt->allow_query) {
                AFW_THROW_ERROR_FZ(general, parser->xctx,
                    "Property " AFW_UTF8_FMT_Q " cannot be queried",
                    AFW_UTF8_FMT_ARG(entry->property_name));
            }
        }

        entry->value = afw_object_get_property(
            filter_object, afw_s_value, parser->xctx);
        /** @fixme Make sure this is a single value/list. */
    }
}


static const afw_utf8_t * const *
impl_AdaptiveQueryCriteria_object_parse_select(
    impl_AdaptiveQueryCriteria_object_parser_t *parser,
    const afw_array_t *select)
{
    afw_const_utf8_a_stack_t *names;
    const afw_utf8_t *name;
    const afw_utf8_t * const *result;
    const afw_iterator_t *iterator;
    const afw_value_t *value;

    names = afw_stack_create(afw_const_utf8_a_stack_t, 20, 0, true,
        parser->p, parser->xctx);
    for (iterator = NULL;;) {
        value = afw_array_get_next_value(select, &iterator,
            parser->p, parser->xctx);
        if (!value) {
            break;
        }
        name = afw_value_as_string(value, parser->xctx);
        afw_stack_push(names, parser->xctx) = name;
    }
    afw_stack_push(names, parser->xctx) = NULL;

    /* Get copy of stack, release instance, and return copy. */
    afw_stack_copy_and_release(names, NULL, &result,
        parser->p, parser->xctx);
    return result;
}


static const afw_query_criteria_sort_entry_t *
impl_AdaptiveQueryCriteria_object_parse_sort(
    impl_AdaptiveQueryCriteria_object_parser_t *parser,
    const afw_array_t *sort)
{
    afw_query_criteria_sort_entry_t *result;
    afw_query_criteria_sort_entry_t *prev;
    afw_query_criteria_sort_entry_t *curr;
    const afw_utf8_t *name;
    const afw_iterator_t *iterator;
    const afw_value_t *value;

    for (iterator = NULL, result = NULL, curr = NULL;;)
    {
        value = afw_array_get_next_value(sort, &iterator,
            parser->p, parser->xctx);
        if (!value) {
            break;
        }
        prev = curr;
        curr = afw_pool_calloc_type(parser->p,
            afw_query_criteria_sort_entry_t, parser->xctx);
        if (prev) {
            prev->next = curr;
        }
        else {
            result = curr;
        }
        name = afw_value_as_string(value, parser->xctx);
        if (name->len <= 2 ||
            (*name->s != '+' && *name->s != '-')
            )
        {
            AFW_THROW_ERROR_Z(general,
                "sort property name is missing or does not start with "
                "'+' or '-'",
                parser->xctx);
        }
        curr->descending = *name->s == '-';
        curr->property_name = afw_utf8_create(name->s + 1, name->len - 1,
            parser->p, parser->xctx);
    }
    return result;
}



/* ------------------------------------------------------------------------- */
/* Internal query test function defines                                      */
/* ------------------------------------------------------------------------- */

static afw_boolean_t
impl_compare_value(
    const afw_query_criteria_filter_entry_t *entry,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_boolean_t is_true;
    const void *i1, *i2;
    const afw_iterator_t *iterator;
    const afw_value_t *entry_value;
    const afw_data_type_t *data_type;
    const afw_data_type_t *entry_data_type;
    const afw_array_t *list;
    afw_array_wrapper_for_array_self_t list_for_single_internal;

    /*
     * If not operator contains or match and data type passed does not match,
     * try to convert property value to entry type.
     */
    entry_value = entry->value;
    if (entry->op_id != afw_query_criteria_filter_op_id_match &&
        entry->op_id != afw_query_criteria_filter_op_id_contains)
    {
        if (value->inf != entry_value->inf) {
            AFW_TRY {
                entry_value = afw_value_convert(entry_value,
                    afw_value_get_data_type(value, xctx), false,
                    p, xctx);
            }
            AFW_CATCH_UNHANDLED{
                entry_value = NULL;
            }
            AFW_ENDTRY;
            if (!entry_value) {
                return false;
            }
        }
    }

    /* Set up so process operation can always work with a list. */
    if (afw_value_is_array(value)) {
        list = ((afw_value_array_t *)value)->internal;
        data_type = afw_array_get_data_type(list, xctx);
        /** @fixme will have to deal with data type from value if NULL data_type. */
    }
    else {
        data_type = afw_value_get_data_type(value, xctx);
        AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY(
            &list_for_single_internal,
            &((afw_value_common_t *)value)->internal, false,
            data_type, 1);
        list = (afw_array_t *)&list_for_single_internal;
    }

    /* Process operation. */
    switch (entry->op_id) {

    case afw_query_criteria_filter_op_id_eq:
        i1 = (const void *)&((const afw_value_common_t *)entry_value)->internal;
        for (is_true = false, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i2, xctx);
            if (!i2) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            is_true = afw_data_type_compare_internal(
                afw_value_get_data_type(value, xctx),
                i1, i2, xctx) == 0;
            if (is_true) {
                break;
            }
        }
        break;

    case afw_query_criteria_filter_op_id_ne:
        i1 = (const void *)&((const afw_value_common_t *)entry_value)->internal;
        for (is_true = true, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i2, xctx);
            if (!i2) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            is_true = afw_data_type_compare_internal(
                afw_value_get_data_type(value, xctx),
                i1, i2, xctx) != 0;
            if (!is_true) {
                break;
            }
        }
        break;

    case afw_query_criteria_filter_op_id_lt:
        i1 = (const void *)&((const afw_value_common_t *)entry_value)->internal;
        for (is_true = false, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i2, xctx);
            if (!i2) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            is_true = afw_data_type_compare_internal(
                afw_value_get_data_type(value, xctx),
                i1, i2, xctx) < 0;
            if (is_true) {
                break;
            }
        }
        break;

    case afw_query_criteria_filter_op_id_le:
        i1 = (const void *)&((const afw_value_common_t *)entry_value)->internal;
        for (is_true = false, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i2, xctx);
            if (!i2) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            is_true = afw_data_type_compare_internal(
                afw_value_get_data_type(value, xctx),
                i1, i2, xctx) <= 0;
            if (is_true) {
                break;
            }
        }
        break;

    case afw_query_criteria_filter_op_id_gt:
        i1 = (const void *)&((const afw_value_common_t *)entry_value)->internal;
        for (is_true = false, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i2, xctx);
            if (!i2) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            is_true = afw_data_type_compare_internal(
                afw_value_get_data_type(value, xctx),
                i1, i2, xctx) > 0;
            if (is_true) {
                break;
            }
        }
        break;

    case afw_query_criteria_filter_op_id_ge:
        i1 = (const void *)&((const afw_value_common_t *)entry_value)->internal;
        for (is_true = false, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i2, xctx);
            if (!i2) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            is_true = afw_data_type_compare_internal(
                afw_value_get_data_type(value, xctx),
                i1, i2, xctx) >= 0;
            if (is_true) {
                break;
            }
        }
        break;

    case afw_query_criteria_filter_op_id_in:
        AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
        /** @fixme 
        entry_list = ((const afw_value_array_t *)entry_value)->internal;
        for (is_true = false, iterator = NULL;;) {
            afw_array_get_next_internal(list, &iterator, &entry_data_type, &i1, xctx);
            if (!i1) {
                break;
            }
            if (data_type != entry_data_type) {
                AFW_THROW_ERROR_Z(general, "data type mismatch", xctx);
            }
            for (iterator2 = NULL;;) {
                afw_array_get_next_internal(entry_list, &iterator2, NULL, &i2, xctx);
                if (!i2) {
                    break;
                }
                is_true = afw_data_type_compare_internal(data_type, i1, i2, xctx) == 0;
                if (is_true) {
                    break;
                }
            }
            if (is_true) {
                break;
            }
        }
        break;
         */


    case afw_query_criteria_filter_op_id_match:
        AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
        /** @fixme 
        for (is_true = false;
            count > 0 && !is_true;
            count--, i2 += value->data_type->c_type_size)
        {
            s2 = afw_data_type_internal_to_utf8(value->data_type,
                i2, p, xctx);
            s_z = afw_utf8_to_utf8_z(s2, p, xctx);
            rv = xmlRegexpExec((xmlRegexpPtr)entry->op_id_specific, s_z);
            if (rv == 1) {
                is_true = true;
            }
            else if (rv < 0) {
                AFW_THROW_ERROR_Z(general,
                    "xmlRegexpExec() error", xctx);
            }
        }
        break;
         */

    case afw_query_criteria_filter_op_id_contains:
        AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
        /** @fixme 
        s1 = afw_value_as_utf8(entry->value, p, xctx);
        for (is_true = false;
            count > 0 && !is_true;
            count--, i2 += value->data_type->c_type_size)
        {
            s2 = afw_data_type_internal_to_utf8(value->data_type,
                i2, p, xctx);
            for (c = s2->s, len = s2->len; len >= s1->len; len--, c++) {
                if (memcmp(c, s1->s, s1->len) == 0) {
                    is_true = true;
                    break;
                }
            }
        }
        break;
         */

    default:
        AFW_THROW_ERROR_Z(general,
            "Error occurred processing query filter", xctx);

    }

    return is_true;
}


/* ------------------------------------------------------------------------- */
/* External function defines                                                 */
/* ------------------------------------------------------------------------- */

/*
 *
 * Parse query string.
 *
 * <query-string> ::= ( <sort> | <select> | <filter> ) ( '&' <query-string> )*
 * <sort>         ::= 'sort(' ( '+' | '-' ) <name> ( ',' ( '+' | '-' ) <name> )* ')'
 * <select>       ::= 'select(' <name> ( ',' <name> )* ')'
 * <filter>       ::= <function> |
 *                   <term> # tree if string followed by '('
 * <function>     ::= 'and' '(' <function> ( ',' <function> )* ')' |
 *                   'or' '(' <function> ( ',' <function> )* ')' |
 *                   <operator> '(' <function> |
 *                       ( <name> ',' <literal> | <list> ) # depends on operator
 *                   ')'
 * <sugar>        ::= <term> ( ( '|' | ',' ) <term> )*
 * <term>         ::= <factor> ( ( '&' | ';' ) <factor> )*
 * <factor>       ::= <relation> | ( <sugar> )
 * <relation>     ::= ( <name> <op1> <literal> ) | ( <name> <op2> <list> )
 * <op1>          ::= '=' | '==' | '=eq=' | '!=' | '=ne=' | '=lt=' | '=le=' |
 *                    '=lte=' | '=gt=' | '=ge=' | '=gte=' | '=match=' |
 *                    '=contains='
 * <op2>          ::= '=in='
 * <list>         ::= '(' <literal> ( ',' <literal> )* ')'
 *
 * The delimiters are "&=<>!()|,;+-# \0".  There is a space before \0.
 *
 * In <query-string>: <sort>, <select> and <filter> can only occur once.
 *
 * In <list>, the comma separator can be % encoded.  DStore seems to do this.
 * This may be an issue if <literal> should contain an encoded comma.
 *
 * <name> is a property name.  Dot notation is allowed for referencing
 * properties of embedded objects.
 *s
 * <literal> is a URL encoded string.  All delimiters in the string must be %
 * encoded.
 *
 */
AFW_DEFINE(const afw_query_criteria_t *)
afw_query_criteria_parse_url_encoded_rql_string(
    const afw_utf8_t *url_encoded_rql_string,
    const afw_object_type_t *object_type,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_query_criteria_t *criteria;
    impl_string_parser_t parser;
    const afw_utf8_z_t *queryz;
    afw_query_criteria_filter_entry_t *filter;
    afw_query_criteria_filter_entry_t *tree;

    /* Initialize parser buffer. */
    memset(&parser, 0, sizeof(parser));
    parser.xctx = xctx;
    parser.p = p;

    /* Allocate memory for criteria. */
    criteria = afw_pool_calloc_type(p, afw_query_criteria_t, xctx);
    criteria->object_type = object_type;
    parser.criteria = criteria;

    /* If there is no query string, return. */
    if (!url_encoded_rql_string ||
        url_encoded_rql_string->len == 0 ||
        *(url_encoded_rql_string->s) == '#')
    {
        return criteria;
    }

    /* Parse string. */
    queryz = afw_utf8_z_create(url_encoded_rql_string->s, url_encoded_rql_string->len, p, xctx);
    parser.criteria->origin_query_string = afw_utf8_create(queryz,
        url_encoded_rql_string->len, p, xctx);
    parser.start = parser.criteria->origin_query_string->s;
    parser.c = parser.start;

    if (*(parser.c) != 0) {
        while (1) {

            /* If "sort(", parser sort criteria. It can only occur once. */
            if (afw_utf8_z_starts_with_z(parser.c, "sort(")) {
                if (parser.criteria->first_sort) {
                    AFW_THROW_ERROR_Z(general,
                        "sort() specified multiple times in query string",
                        xctx);
                }
                parser.c += strlen("sort(");
                parser.criteria->first_sort = impl_parse_string_sort(&parser);
            }

            /* If "select(", parser select criteria. It can only occur once. */
            else if (afw_utf8_z_starts_with_z(parser.c, "select(")) {
                if (parser.criteria->select) {
                    AFW_THROW_ERROR_Z(general,
                        "select() specified multiple times in query string",
                        xctx);
                }
                parser.c += strlen("select(");
                parser.criteria->select = impl_parse_string_select(&parser);
            }

            /* If "limit(", parser limit criteria. It can only occur once.
            else if (afw_utf8_z_starts_with_z(parser.c, "select(")) {
                if (parser.criteria->limit) {
                    AFW_THROW_ERROR_Z(general,
                        "limit() specified multiple times in query string",
                        xctx);
                }
                parser.c += strlen("limit(");
                impl_parse_string_limit(&parser);
            }
             */

            /* Anything else must be a filter. */
            else {
                if (parser.criteria->filter) {
                    AFW_THROW_ERROR_Z(general,
                        "Filter in query string is malformed", xctx);
                }
                impl_parse_string_filter(&parser,
                    &filter,
                    &tree,
                    AFW_QUERY_CRITERIA_TRUE,
                    AFW_QUERY_CRITERIA_FALSE);
                parser.criteria->filter = filter;
                parser.criteria->tree = tree;
            }

            /*
             * Get next token.  If it is an '&", continue.  Otherwise, anything
             * except the end of the query sting is an error.
             */
            impl_get_token(&parser);
            if (parser.token_type == impl_token_type_end) {
                break;
            }
            if ((parser.token_type != impl_token_type_delimiter) ||
                (*(parser.token.s) != '&'))
            {
                AFW_THROW_ERROR_Z(general,
                    "query filter is malformed", xctx);
            }
        }
    }

    return criteria;
}



/*  Parse URI encoded query string.  */
AFW_DEFINE(const afw_query_criteria_t *)
afw_query_criteria_parse_AdaptiveQueryCriteria_object(
    const afw_object_t *query_object,
    const afw_object_type_t *object_type,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_query_criteria_t *criteria;
    const afw_object_t *filter_object;
    const afw_iterator_t *iterator;
    const afw_array_t *sort;
    const afw_array_t *select;
    const afw_utf8_t *property_name;
    const afw_utf8_t *url_encoded_rql_string;
    const afw_value_t *value;
    impl_AdaptiveQueryCriteria_object_parser_t parser;

    /* If urlEncodedRQLString property present, process it and return result. */
    url_encoded_rql_string = afw_object_old_get_property_as_string(
        query_object, afw_s_urlEncodedRQLString, xctx);
    if (url_encoded_rql_string) {

        /* Make sure only urlEncodedRQLString is specified. */
        for (iterator = NULL;;) {
            value = afw_object_get_next_property(query_object,
                &iterator, &property_name, xctx);
            if (!value) {
                break;
            }
            if (!afw_utf8_equal(property_name, afw_s_urlEncodedRQLString)) {
                AFW_THROW_ERROR_Z(general,
                    "Property urlEncodedRQLString must be specified alone",
                    xctx);
            }
        }

        /* Parse urlEncodedRQLString and return criteria. */
        return afw_query_criteria_parse_url_encoded_rql_string(
            url_encoded_rql_string, object_type, p, xctx);
    }

    /* Otherwise, process based on filter, select, and sort properties. */
    criteria = afw_pool_calloc_type(p, afw_query_criteria_t, xctx);
    criteria->query_object = query_object;
    criteria->object_type = object_type;
    memset(&parser, 0, sizeof(parser));
    parser.xctx = xctx;
    parser.p = p;
    parser.criteria = criteria;

    for (
        iterator = NULL,
        sort = NULL,
        select = NULL,
        property_name = NULL;
        ;)
    {
        value = afw_object_get_next_property(query_object,
            &iterator, &property_name, xctx);
        if (!value) {  
            break;
        }

        /* filter property */
        if (afw_utf8_equal(property_name, afw_s_filter)) {
            filter_object = afw_value_as_object(value, xctx);
            impl_AdaptiveQueryCriteria_object_parse_filter(&parser,
                filter_object, &criteria->filter, &criteria->tree,
                AFW_QUERY_CRITERIA_TRUE, AFW_QUERY_CRITERIA_FALSE);
        }

        /* select property */
        else if (afw_utf8_equal(property_name, afw_s_select)) {
            select = afw_value_as_array(value, xctx);
            criteria->select = impl_AdaptiveQueryCriteria_object_parse_select(
                &parser, select);
        }

        /* sort property */
        else if (afw_utf8_equal(property_name, afw_s_sort)) {
            sort = afw_value_as_array(value, xctx);
            criteria->first_sort = impl_AdaptiveQueryCriteria_object_parse_sort(
                &parser, sort);
        }

        /* unknown property */
        else {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Unknown query criteria property " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(property_name));
        }
    }

    return criteria;
}


/* Test object against query criteria. */
AFW_DEFINE(afw_boolean_t)
afw_query_criteria_test_object(
    const afw_object_t *obj,
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_query_criteria_filter_entry_t *entry;
    const afw_value_t *value;
    afw_boolean_t is_true;

    /* Check object against filter. No filter always results in true. */
    entry = (criteria) ? criteria->filter : NULL;
    is_true = true; 
    while (entry) {
        value = afw_object_get_property_extended(obj,
            entry->property_name, xctx);

        /* If property does not exists, always fail. */
        if (!value) {
            is_true = false;
            break;
        }

        /* Make sure value is already evaluated. */
        if (!afw_value_is_defined_and_evaluated(value)) {
            AFW_THROW_ERROR_Z(general, "Expecting evaluated value", xctx);
        }

        /* Object values are not supported yet or ever. */
        if (afw_value_is_object(value) || afw_value_is_object(entry->value))
        {
            AFW_THROW_ERROR_Z(general,
                "Object values are not supported in query string",
                xctx);
        }

        /* Set on_true/on_value based on comparison. */
        is_true = impl_compare_value(entry, value, p, xctx);
        if (is_true) {
            if (entry->on_true == AFW_QUERY_CRITERIA_TRUE) {
                break;
            }
            if (entry->on_true == AFW_QUERY_CRITERIA_FALSE){
                is_true = false;
                break;
            }
            entry = entry->on_true;
        }
        else {
            if (entry->on_false == AFW_QUERY_CRITERIA_TRUE) {
                is_true = true;
                break;
            }
            if (entry->on_false == AFW_QUERY_CRITERIA_FALSE){
                break;
            }
            entry = entry->on_false;
        }

    }

    /* Return whether object passes filter. */
    return is_true;
}



/* ------------------------------------------------------------------------- */
/* afw_query_criteria_to_AdaptiveQueryCriteria_object()                      */
/* ------------------------------------------------------------------------- */


static const afw_object_t *
impl_criteria_filter_to_property_value(
    const afw_query_criteria_filter_entry_t *entry,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *filter;
    const afw_query_criteria_filter_entry_t *e;
    const afw_array_t *filters;
    const afw_object_t *o;
    const afw_value_t *v;

    if (!entry)
    {
        return NULL;
    }

    filter = afw_object_create_unmanaged(p, xctx);
    afw_object_set_property_as_string(filter, afw_s_op, entry->op_name, xctx);

    if (entry->op_id == afw_query_criteria_filter_op_id_and ||
        entry->op_id == afw_query_criteria_filter_op_id_or)
    {
        filters = afw_array_create_generic(p, xctx);
        afw_object_set_property_as_array(filter, afw_s_filters, filters, xctx);
        for (e = entry->first_conjunctive_child; e; e = e->next_conjunctive_sibling) {
            o = impl_criteria_filter_to_property_value(e, p, xctx);
            v = afw_value_create_unmanaged_object(o, p, xctx);
            afw_array_add_value(filters, v, xctx);
        }
    }

    else {
        afw_object_set_property_as_string(filter,
            afw_s_property, entry->property_name, xctx);
        afw_object_set_property(filter, afw_s_value, entry->value, xctx);
    }

    return filter;
}


static const afw_array_t *
impl_criteria_select_to_property_value(
    const afw_utf8_t * const *select,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *result;
    const afw_utf8_t * const *e;
    afw_value_string_t *v;

    if (!select) {
        return NULL;
    }

    result = afw_array_create_generic(p, xctx);

    for (e = select; *e; e++) {
        v = afw_value_allocate_unmanaged_string(p, xctx);
        afw_memory_copy(&v->internal, *e);
        afw_array_add_value(result, &v->pub, xctx);
    }

    return result;
}


static const afw_array_t *
impl_criteria_sort_to_property_value(
    const afw_query_criteria_sort_entry_t *sort_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *result;
    const afw_query_criteria_sort_entry_t *e;
    afw_value_string_t *v;
    afw_utf8_octet_t *c;

    if (!sort_entry) {
        return NULL;
    }

    result = afw_array_create_generic(p, xctx);

    for (e = sort_entry; e; e = e->next) {
        v = afw_value_allocate_unmanaged_string(p, xctx);
        v->internal.len = e->property_name->len + 1;
        v->internal.s = c = afw_pool_malloc(p, v->internal.len, xctx);
        *c++ = (e->descending) ? '-' : '+';
        memcpy(c, e->property_name->s, e->property_name->len);
        afw_array_add_value(result, &v->pub, xctx);
    }

    return result;
}



/* Convert query criteria to a _AdaptiveQueryCriteria_ object. */
AFW_DEFINE(const afw_object_t *)
afw_query_criteria_to_AdaptiveQueryCriteria_object(
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_object_t *filter;
    const afw_array_t *select;
    const afw_array_t *sort;

    /* Create object for result. */
    object = afw_object_create_managed(p, xctx);
    afw_object_meta_set_object_type_id(object, afw_s__AdaptiveQueryCriteria_,
        xctx);

    /* If no criteria, return empty object. */
    if (!criteria) {
        return object;
    }

    /* filter */
    filter = impl_criteria_filter_to_property_value(
        criteria->tree, object->p, xctx);
    if (filter) {
        afw_object_set_property_as_object(object,
            afw_s_filter, filter, xctx);
    }

    /* select */
    if (criteria->select) {
        select = impl_criteria_select_to_property_value(
            criteria->select, object->p, xctx);
        afw_object_set_property_as_array(object,
            afw_s_select, select, xctx);
    }

    /* sort */
    if (criteria->first_sort) {
        sort = impl_criteria_sort_to_property_value(
            criteria->first_sort, object->p, xctx);
        afw_object_set_property_as_array(object,
            afw_s_sort, sort, xctx);
    }

    /* Return _AdaptiveQueryCriteria_ object */
    return object;
}



/* ------------------------------------------------------------------------- */
/* afw_query_criteria_to_query_string()                            */
/* ------------------------------------------------------------------------- */

static void
impl_entry_to_query_string(
    const afw_writer_t *w,
    const afw_query_criteria_filter_entry_t *entry,
    afw_boolean_t style_function,
    afw_boolean_t style_long,
    afw_boolean_t style_semi_colon_comma,
    afw_boolean_t need_ampersand,
    afw_boolean_t inner_conjunction,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *value;
    afw_boolean_t first_loop;
    const afw_utf8_z_t *fiql_op;
    const afw_utf8_t *s;
    const impl_rql_op_t *rql_op;
    const afw_array_t *list;
    const afw_query_criteria_filter_entry_t *child;
    afw_boolean_t first_time;
    afw_boolean_t need_close_parenthesis;

    /* Conjunction */
    if (afw_utf8_equal(entry->op_name, afw_s_and) ||
        afw_utf8_equal(entry->op_name, afw_s_or))
    {
        if (style_function) {
            for (child = entry->first_conjunctive_child, first_loop = true;
                child;
                child = child->next_conjunctive_sibling)
            {
                if (first_loop) {
                    if (need_ampersand) {
                        afw_writer_write_z(w, "&", xctx);
                    }
                    first_loop = false;
                    afw_writer_write_utf8(w, entry->op_name, xctx);
                    afw_writer_write_z(w, "(", xctx);
                }
                else {
                    afw_writer_write_z(w, ",", xctx);
                }
                impl_entry_to_query_string(w, child,
                    style_function, style_long,
                    style_semi_colon_comma, false, true , p, xctx);
            }

            afw_writer_write_z(w, ")", xctx);
        }

        else {
            need_close_parenthesis = false;
            fiql_op = afw_utf8_equal(entry->op_name, afw_s_and)
                ? (style_semi_colon_comma ? ";" : "&")
                : (style_semi_colon_comma ? "," : "|");
            for (child = entry->first_conjunctive_child, first_loop = true;
                child;
                child = child->next_conjunctive_sibling)
            {

                if (first_loop) {
                    if (need_ampersand) {
                        afw_writer_write_z(w, "&", xctx);
                    }
                    if (inner_conjunction ||
                        (!inner_conjunction &&
                            afw_utf8_equal(entry->op_name, afw_s_or)))
                    {
                        afw_writer_write_z(w, "(", xctx);
                        need_close_parenthesis = true;
                    }
                }
                else {
                    afw_writer_write_z(w, fiql_op, xctx);
                }

                impl_entry_to_query_string(w, child,
                    style_function, style_long,
                    style_semi_colon_comma, false, true, p, xctx);

                first_loop = false;
            }
            if (need_close_parenthesis) {
                afw_writer_write_z(w, ")", xctx);
            }
        }
    }

    /* Comparision */
    else {
        if (need_ampersand) {
            afw_writer_write_z(w, "&", xctx);
        }

        rql_op = impl_find_rql_op(entry->op_name);
        if (!rql_op || !rql_op->can_be_property) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid op name " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(entry->op_name));
        }
        property_name = afw_uri_encode(entry->property_name,
            AFW_URI_OCTET_UNRESERVED, p, xctx);

        if (style_function) {
            afw_writer_write_utf8(w, entry->op_name, xctx);
            afw_writer_write_z(w, "(", xctx);
            afw_writer_write_utf8(w, property_name, xctx);
            if (rql_op->op->is_list) {
                list = afw_value_as_array(entry->value, xctx);
                for (iterator = NULL;;) {
                    value = afw_array_get_next_value(list, &iterator, p, xctx);
                    if (!value) {
                        break;
                    }
                    afw_writer_write_z(w, ",", xctx);
                    s = afw_value_as_utf8(value, p, xctx);
                    s = afw_uri_encode(s,
                        AFW_URI_OCTET_ENCODE_COMPONENT_VALUE, p, xctx);
                    afw_writer_write_utf8(w, s, xctx);
                }
            }
            else {
                afw_writer_write_z(w, ",", xctx);
                s = afw_value_as_utf8(entry->value, p, xctx);
                s = afw_uri_encode(s,
                    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE, p, xctx);
                afw_writer_write_utf8(w, s, xctx);
            }
            afw_writer_write_z(w, ")", xctx);
        }

        else {
            afw_writer_write_utf8(w, property_name, xctx);
            if (style_long) {
                if (afw_utf8_equal(entry->op_name, afw_s_ge)) {
                    afw_writer_write_z(w, "=gte=", xctx);
                }
                else if (afw_utf8_equal(entry->op_name, afw_s_le)) {
                    afw_writer_write_z(w, "=lte=", xctx);
                }
                else {
                    afw_writer_write_z(w, "=", xctx);
                    afw_writer_write_utf8(w, entry->op_name, xctx);
                    afw_writer_write_z(w, "=", xctx);
                }
            }
            else {
                if (afw_utf8_equal(entry->op_name, afw_s_ne)) {
                    afw_writer_write_z(w, "!=", xctx);
                }
                else {
                    afw_writer_write_z(w, "=", xctx);
                    if (!afw_utf8_equal(entry->op_name, afw_s_eq)) {
                        afw_writer_write_utf8(w, entry->op_name, xctx);
                        afw_writer_write_z(w, "=", xctx);
                    }
                }
            }

            if (rql_op->op->is_list) {
                list = afw_value_as_array(entry->value, xctx);
                for (iterator = NULL, first_time = true;;) {
                    value = afw_array_get_next_value(list, &iterator, p, xctx);
                    if (!value) {
                        break;
                    }
                    if (first_time) {
                        first_time = false;
                        afw_writer_write_z(w, "(", xctx);
                    }
                    else {
                        afw_writer_write_z(w, ",", xctx);
                    }
                    s = afw_value_as_utf8(value, p, xctx);
                    s = afw_uri_encode(s,
                        AFW_URI_OCTET_ENCODE_COMPONENT_VALUE, p, xctx);
                    afw_writer_write_utf8(w, s, xctx);
                    afw_writer_write_z(w, ")", xctx);
                }
            }
            else {
                s = afw_value_as_utf8(entry->value, p, xctx);
                s = afw_uri_encode(s,
                    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE, p, xctx);
                afw_writer_write_utf8(w, s, xctx);
            }
        }
    }
}

/* Convert query criteria to a url encoded rql string. */
AFW_DEFINE(const afw_utf8_t *)
afw_query_criteria_to_query_string(
    const afw_query_criteria_t *criteria,
    afw_query_criteria_style_t style,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_writer_t *w;
    afw_utf8_t ws;
    const afw_utf8_t *result;
    const afw_utf8_t *s;
    afw_boolean_t needs_ampersand, first_inner;
    const afw_utf8_t * const *sa;
    const afw_query_criteria_sort_entry_t *se;
    afw_boolean_t style_function;
    afw_boolean_t style_long;
    afw_boolean_t style_semi_colon_comma;

    /* Return empty string if no query_object. */
    if (!criteria) {
        return afw_s_a_empty_string;
    }

    /* Create writer for result. */
    needs_ampersand = false;
    w = afw_utf8_writer_create(NULL, p, xctx);

    /* select */
    if (criteria->select) {
        for (sa = criteria->select, first_inner = true; *sa; sa++)
        {
            if (first_inner) {
                first_inner = false;
                afw_writer_write_z(w, "select(", xctx);
                needs_ampersand = true;
            }
            else {
                afw_writer_write_z(w, ",", xctx);
            }
            s = afw_uri_encode(*sa,
                AFW_URI_OCTET_UNRESERVED, p, xctx);
            afw_writer_write_utf8(w, s, xctx);
        }
        if (!first_inner) {
            afw_writer_write_z(w, ")", xctx);
        }
    }

    /* sort */
    for (se = criteria->first_sort, first_inner = true; se; se = se->next)
    {
        if (first_inner) {
            first_inner = false;
            if (needs_ampersand) {
                afw_writer_write_z(w, "&", xctx);
            }
            afw_writer_write_z(w, "sort(", xctx);
            needs_ampersand = true;
        }
        else {
            afw_writer_write_z(w, ",", xctx);
        }

        if (se->descending) {
            afw_writer_write_z(w, "-", xctx);
        }
        else {
            afw_writer_write_z(w, "%2b", xctx);
        }
        s = afw_uri_encode(se->property_name,
            AFW_URI_OCTET_UNRESERVED, p, xctx);
        afw_writer_write_utf8(w, s, xctx);
    }
    if (!first_inner) {
        afw_writer_write_z(w, ")", xctx);
    }


    /* filter. */
    if (criteria->filter) {
        style_function = false;
        style_long = false;
        style_semi_colon_comma = false;
        switch (style) {

        case afw_query_criteria_style_operator:
            break;

        case afw_query_criteria_style_operator_long:
            style_long = true;
            break;

        case afw_query_criteria_style_semicolon_comma:
            style_semi_colon_comma = true;
            break;

        case afw_query_criteria_style_function:
            style_function = true;
            break;

        default:
            AFW_THROW_ERROR_Z(general, "invalid value for style parameter", xctx);
        }

        impl_entry_to_query_string(w, criteria->tree,
            style_function, style_long, style_semi_colon_comma,
            needs_ampersand, false, p, xctx);
    }

    /* Return RQL */
    afw_utf8_writer_current_string(w, &ws, xctx);
    result = afw_utf8_clone(&ws, p, xctx);
    afw_writer_release(w, xctx);
    return result;
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Internal
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMPILE_INTERNAL_H__
#define __AFW_COMPILE_INTERNAL_H__

#include "afw.h"

/**
 * @defgroup afw_compile_internal Compile
 * @ingroup afw_c_api_internal
 * 
 * Internal compile API.
 * 
 * @{
 */

/**
 * @file afw_compile_internal.h
 * @brief Adaptive Framework Compiler Internal.
 */

AFW_BEGIN_DECLARES
typedef enum {

    /* Invalid token. */
    afw_compile_token_type_invalid,

    /*
     * End of input. Should only be returned by get_token() after parsing
     * is complete.
     */
    afw_compile_token_type_end,

    /* This token is set on call to afw_compile_get_raw_line_impl() */
    afw_compile_token_type_raw_line,

    /*
     * This can only be returned if afw_compile_get_token_before_eol() is
     * called. It indicates that no token was found before the end of line.
     */
    afw_compile_token_type_eol,

    /* Token types with additional information. See afw_compile_token_t. */
    afw_compile_token_type_boolean,
    afw_compile_token_type_identifier,
    afw_compile_token_type_integer,
    afw_compile_token_type_number,
    afw_compile_token_type_binary_string,
    afw_compile_token_type_utf8_string,
    afw_compile_token_type_template_string,
    afw_compile_token_type_null,
    afw_compile_token_type_undefined,

    /* Miscellaneous symbols. */
    afw_compile_token_type_ampersand,                    /* & */
    afw_compile_token_type_back_slash,                   /* \ */
    afw_compile_token_type_caret,                        /* ^ */
    afw_compile_token_type_colon,                        /* : */
    afw_compile_token_type_comma,                        /* , */
    afw_compile_token_type_dollar_sign,                  /* $ */
    afw_compile_token_type_ellipsis,                     /* ... */
    afw_compile_token_type_fat_arrow,                    /* => */
    afw_compile_token_type_grave,                        /* ` */
    afw_compile_token_type_nullish_coalescing,           /* ?? */
    afw_compile_token_type_optional_chaining,            /* ?. */
    afw_compile_token_type_optional_chaining_thin_arrow, /* ?-> */
    afw_compile_token_type_percent,                      /* % */
    afw_compile_token_type_period,                       /* . */
    afw_compile_token_type_question_mark,                /* ? */
    afw_compile_token_type_semicolon,                    /* ; */
    afw_compile_token_type_slash,                        /* / */
    afw_compile_token_type_thin_arrow,                   /* -> */
    afw_compile_token_type_tilde,                        /* ~ */
    afw_compile_token_type_substitute_start,             /* ${ */
    afw_compile_token_type_vertical_bar,                 /* | */

    /* Unary operators */
    afw_compile_token_type_unary_plus,                   /* + */
    afw_compile_token_type_unary_minus,                  /* - */
    afw_compile_token_type_unary_not,                    /* ! */

    /* Open/close symbols. */
    afw_compile_token_type_open_brace,                   /* { */
    afw_compile_token_type_close_brace,                  /* } */
    afw_compile_token_type_open_parenthesis,             /* ( */
    afw_compile_token_type_close_parenthesis,            /* ) */
    afw_compile_token_type_open_bracket,                 /* [ */
    afw_compile_token_type_close_bracket,                /* ] */
    afw_compile_token_type_open_angle_bracket,           /* < */
    afw_compile_token_type_close_angle_bracket,          /* > */

    /* Arithmetic operators. */
    afw_compile_token_type_add,                          /* + */
    afw_compile_token_type_subtract,                     /* - */
    afw_compile_token_type_multiply,                     /* * */
    afw_compile_token_type_divide,                       /* / */
    afw_compile_token_type_modulus,                      /* % */
    afw_compile_token_type_exponentiation,               /* ** */
    afw_compile_token_type_increment,                    /* ++ */
    afw_compile_token_type_decrement,                    /* -- */

    /* Logical operators. */
    afw_compile_token_type_and,                          /* && */
    afw_compile_token_type_or,                           /* || */

    /* Assignment operators. */
    afw_compile_token_type_equal,                        /* = */
    afw_compile_token_type_plus_equal,                   /* += */
    afw_compile_token_type_minus_equal,                  /* -= */
    afw_compile_token_type_multiply_equal,               /* *= */
    afw_compile_token_type_divide_equal,                 /* /= */
    afw_compile_token_type_modulus_equal,                /* %= */
    afw_compile_token_type_exponentiation_equal,         /* **= */
    afw_compile_token_type_and_equal,                    /* &&= */
    afw_compile_token_type_or_equal,                     /* ||= */
    afw_compile_token_type_nullish_equal,                /* ??= */

    /* Comparison operators. */
    afw_compile_token_type_equal_to,                     /* == */
    afw_compile_token_type_equal_value_and_type,         /* === */
    afw_compile_token_type_not_equal_to,                 /* != */
    afw_compile_token_type_not_equal_value_and_type,     /* !== */
    afw_compile_token_type_less_than,                    /* < */
    afw_compile_token_type_less_than_or_equal_to,        /* <= */
    afw_compile_token_type_greater_than,                 /* > */
    afw_compile_token_type_greater_than_or_equal_to      /* >= */

} afw_compile_internal_token_type_t;


/**
 * @brief Resources that can be shared by multiple compiles.
 *
 * This struct is created by a call to afw_compile_shared_create() and
 * by a call to afw_compile_*() with a shared parameter of NULL.
 *
 * If an afw_compile_*() is called with a non-NULL value, the p parameter
 * must match shared->P.
 *
 * This struct will be destroyed when struct->p is destroyed.
 *
 * A call to afw_compile_shared_release_temp() will release resources that are
 * only needed during calls to afw_compile_*() that compile.
 */
struct afw_compile_internal_shared_s {

    /*
     * This is the pool that all calls to afw_compile_* with a shared
     * parameter pointing to this must use.  If the shared parameter
     * for one of these calls is NULL, a new one will be created.
     *
     * The purpose of this is to allow multiple of compiles that have a
     * p with the same lifetime to share strings, literals, etc.
     *
     * This struct will be destroyed when the p of the original call
     * to a afw_compile_* call with a shared parameter of NULL is
     * destroyed.
     */
    const afw_pool_t *p;

    /*
     * Temporary pool that will be released when
     * afw_compile_shared_release_temp() is called.
     */
    const afw_pool_t *temp_p;

    /*
     * This is a hash table of string literals that can be reused.  The
     * key is the string and the value is the string.  Use
     * afw_compile_get_string_literal() to access;
     */
    apr_hash_t *string_literals;

};



struct afw_compile_internal_token_s {

    afw_compile_internal_token_type_t type;

    afw_size_t token_source_offset;

    afw_size_t token_source_len;

    /* Additional information for some tokens. */
    union {

        /*
         * If type is identifier this is the name and qualifier.  Member
         * identifier includes the identifier including optional qualifier
         * followed by '::'.  If qualifier is not specified,
         * identifier_qualifier.len will be 0.
         *
         */
        struct {
            const afw_utf8_t *identifier;
            const afw_utf8_t *identifier_name;
            const afw_utf8_t *identifier_qualifier;
        };

        /* If type is boolean, this is its boolean value. */
        afw_boolean_t boolean;

        /* If type is integer, this is its integer value. */
        afw_integer_t integer;

        /* If type is null, this is NULL. */
        void *null;

        /* If type is undefined, use identifier to hold 'undefined'. */

        /* If type is number, this is the double value. */
        afw_double_t number;

        /*
         * If type is quoted string, this is the decoded string and quote
         * character (single (') or double (") quote)*/
        struct {
            const afw_utf8_t *string;
            afw_utf8_octet_t string_quote_character;
        };

        /* Used for afw_compile_token_type_raw_line. */
        afw_utf8_t raw_line;
    };

};


#define AFW_COMPILE_MAX_TOKENS 5



/**
 * @brief Contextual information provided in some values.
 *
 * This information is in some afw_value_*_t structs to provide contextual
 * information for errors and debugging.
 */
struct afw_compile_internal_value_contextual_s {

    /** @brief Compiled value this value is part of. */
    const afw_value_compiled_value_t *compiled_value;

    /** @brief Source location. */
    const afw_utf8_t *source_location;

    /** @brief Offset in full source of compiled value to this value. */
    afw_size_t value_offset;

    /** @brief Size in full_source of value source. */
    afw_size_t value_size;
};



struct afw_compile_internal_parser_s {

    /* Pool used for everything returned. */
    const afw_pool_t *p;

    /* Some things still use apr pools. */
    apr_pool_t *apr_p;

    /* The execution context (xctx) of caller to parser. */
    afw_xctx_t *xctx;

    /* Shared resources. */
    const afw_compile_shared_t *shared;

    /* This is the value returned by the compiler. */
    afw_value_compiled_value_t *compiled_value;

    afw_compile_value_contextual_t contextual;

    const afw_utf8_t *passed_source;

    afw_size_t estimated_size;

    afw_utf8_octet_get_cb_t callback;

    void *callback_data;

    /*
     * Full source as it is retrieved.
     *
     * IMPORTANT:  This must continue to point to the same exact memory since
     *             its value is copied into many value structs that exist
     *             after parsing is finished.
     *
     * Only afw_compile_get_code_point_impl() should modify this.
     */
    afw_utf8_t *full_source;

    /* Current cursor within source of next octet. */
    afw_size_t cursor;

    /* If all_eof occurred, this is the total length of the source. */
    afw_size_t cursor_eof;

    /*
     * Only used if callback specified and will include passed_source followed
     * by octets retrieved via callback.
     *
     * Only afw_compile_get_code_point_impl() should directly access this.
     */
    apr_array_header_t *source_buffer;

    /* Used for parsing strings. */
    apr_array_header_t *s;

    /*
     * Temporary array used for building list of values.
     *
     * Each function that uses the array should reset nelts to entry
     * value on return.
     */
    apr_array_header_t *values;

    afw_size_t hybrid_start_offset;

    afw_error_t *error;

    const afw_object_t *embedding_object;

    const afw_utf8_t *property_name;

    afw_compile_token_t token_storage;

    afw_compile_token_t *token;

    afw_size_t cursors[AFW_COMPILE_MAX_TOKENS];

    afw_size_t block_count;

    int cursor_count;

    int current_cursor_index;

    afw_compile_type_t compile_type;

    afw_compile_residual_check_t residual_check;

    afw_boolean_t strict;

    afw_boolean_t cede_p;

    /*
     * This is used for context of operators:
     *   +, -, %, /, **, ||, &&, <, and >
     */
    afw_boolean_t next_can_be_nonunary_operator;

    /* When this is true, grave symbol starts a grave string. */
    afw_boolean_t next_can_be_template_string;

    /* Next identifier is not a special literal (true, NaN, ...) . */
    afw_boolean_t next_identifier_is_not_special_literal;

    afw_boolean_t all_eof;

    afw_boolean_t last_octet_eof;

    afw_boolean_t callback_eof;

    afw_boolean_t scanning_for_residual;

    afw_boolean_t has_called_callback;

    /*
     * Set while parsing object spread to change behavior of
     * AFW_MEMORY_OBJECT_CREATE_ENTITY_OR_EMBEDDED_P()
     */
    afw_boolean_t doing_object_spread;

    /*
     * Set by afw_compile_get_token_before_eol() and reset in
     * afw_compile_get_token_impl().
     */
    afw_boolean_t get_token_before_eol;

    /*
     * Set by afw_compile_skip_ws() when end of line found and
     * parser->get_token_before_eol is on.
     */
    afw_boolean_t get_token_found_eol;

    /* Rethrow only allowed in catch block. */
    afw_boolean_t rethrow_allowed;

    /* break statement allowed in loop can switch. */
    afw_boolean_t break_allowed;

    /* continue statement allowed in loop. */
    afw_boolean_t continue_allowed;

};



/**
 * @brief Assignment type Map.
 * @param id of assignment type
 * @param description of this assignment type
 */
#define AFW_COMPILE_INTERNAL_ASSIGNMENT_TYPE_MAP(XX)                            \
                                                                                \
    XX(assign_only,                                                             \
        "This is an assignment to an existing variable.")                       \
                                                                                \
    XX(const,                                                                   \
        "This is an assignment to a new const variable.")                       \
                                                                                \
    XX(declare_only,                                                            \
        "This is a declare of a new variable without assignment.")              \
                                                                                \
    XX(define_if_needed,                                                        \
        "This is an assignment to a new or existing local variable. ")          \
                                                                                \
    XX(let,                                                                     \
        "This is an assignment to a new local variable. ")                      \
                                                                                \
    XX(reference_only,                                                          \
        "No assignment, just reference.")                                       \
                                                                                \
    XX(use_assignment_targets,                                                  \
        "Use the assignment type from assignment target.")                      \
                                                                                \

/** @brief Enum for assignment types. */
typedef enum {
#define XX(id, description)                                                   \
    afw_compile_assignment_type_ ## id,
    AFW_COMPILE_INTERNAL_ASSIGNMENT_TYPE_MAP(XX)
#undef XX
    afw_compile_assignment_type_max_type
} afw_compile_internal_assignment_type_t;


/**
 * @brief Assignment target type Map.
 * @param id of assignment target type
 * @param description of this assignment target type
 */
#define AFW_COMPILE_INTERNAL_ASSIGNMENT_TARGET_TYPE_MAP(XX)                     \
                                                                                \
    XX(list_destructure,                                                        \
        "This is a list destructure.")                                          \
                                                                                \
    XX(object_destructure,                                                      \
        "This is a object destructure.")                                        \
                                                                                \
    XX(variable_reference,                                                      \
        "This is a variable reference.")                                        \

/** @brief Enum for assignment target types. */
typedef enum {
#define XX(id, description)                                                   \
    afw_compile_assignment_target_type_ ## id,
    AFW_COMPILE_INTERNAL_ASSIGNMENT_TARGET_TYPE_MAP(XX)
#undef XX
    afw_compile_assignment_target_type_max_type
} afw_compile_assignment_target_type_t;



/* See AssignmentTarget */
struct afw_compile_internal_assignment_target_s {
    afw_compile_internal_assignment_type_t assignment_type;
    afw_compile_assignment_target_type_t target_type;
    union {
        const afw_compile_list_destructure_t *list_destructure;
        const afw_compile_object_destructure_t *object_destructure;
        union {
            const afw_value_type_t *variable_type;
            const afw_value_variable_reference_t *variable_reference;
        };
    };
};


/* See AssignmentListDestructureTarget */
struct afw_compile_internal_list_destructure_s {

    /* AssignmentElement or NULL */
    const afw_compile_assignment_element_t *assignment_element;

    /* AssignmentTarget for ... Type? VariableName or NULL  */
    const afw_value_type_t *rest_type;
    const afw_value_t *rest;
};


/* See AssignmentElement */
struct afw_compile_assignment_element_s {
    const afw_compile_assignment_element_t *next;
    const afw_value_type_t *type;
    const afw_value_t *assignment_target;
    const afw_value_t *default_value;
};


/* See AssignmentProperty */
struct afw_compile_internal_assignment_property_s {
    const afw_compile_assignment_property_t *next;
    const afw_value_type_t *type;
    afw_boolean_t is_rename;
    union {
        /* If is_rename is true. */
        struct {
            const afw_utf8_t *property_name;
            const afw_compile_assignment_element_t *assignment_element;
        };
        /* If is_rename is false. */
        struct {
            const afw_value_variable_reference_t *variable_reference;
            const afw_value_t *default_value;
        };
    };
};



/* See AssignmentObjectDestructureTarget */
struct afw_compile_internal_object_destructure_s {

    /* AssignmentProperty or NULL */
    const afw_compile_assignment_property_t *assignment_property;

    /* AssignmentTarget for ...VariableName or NULL  */
    const afw_value_type_t *rest_type;
    const afw_value_t *rest;
};



#define afw_value_contextual_resolve_value_source( \
    value_source, contextual) \
do { \
    if ((contextual)->compiled_value && \
        (contextual)->compiled_value->full_source)  \
    { \
        (value_source)->s = (((contextual)->compiled_value->full_source)->s) + \
            (contextual)->value_offset; \
        (value_source)->len = (contextual)->value_size; \
    } \
    else { \
        memset((value_source), 0, sizeof(afw_utf8_t)); \
    } \
} while (0)



#define afw_compile_token_is(token_type) \
    (parser->token->type == afw_compile_token_type_##token_type)



#define afw_compile_token_is_unqualified_identifier() \
    (parser->token->type == afw_compile_token_type_identifier && \
    !parser->token->identifier_qualifier)



#define afw_compile_token_is_name(string) \
    (afw_compile_token_is_unqualified_identifier() && \
    afw_utf8_equal(parser->token->identifier_name, (string)))



#define afw_compile_token_is_name_z(string_z) \
    (afw_compile_token_is_unqualified_identifier() && \
    afw_utf8_equal_utf8_z(parser->token->identifier_name, (string_z)))


afw_compile_internal_token_type_t
afw_compile_peek_next_token_impl(afw_compile_parser_t *parser);



#define afw_compile_is_at_eof() \
    (parser->last_octet_eof)



#define afw_compile_next_is_at_eof() \
    (parser->last_octet_eof || \
    (parser->all_eof && (parser->cursor == parser->cursor_eof)) )



#define afw_compile_peek_next_token() \
    afw_compile_peek_next_token_impl(parser)



#define afw_compile_peek_next_token_is(token_type) \
    (afw_compile_peek_next_token_impl(parser) == \
        afw_compile_token_type_##token_type)



#define afw_compile_set_contextual() \
    parser->xctx->error->contextual = &parser->contextual; \
    parser->xctx->error->parser_cursor = parser->cursor

//! @fixme    parser->contextual.value_source.s = parser->c; 
//! @fixme    parser->contextual.value_source.len = 0; 


/*  Macro used to set parse error in xctx and throw it. */
#define AFW_COMPILE_THROW_ERROR_Z(message_z) \
do { \
    afw_compile_parse_set_error_z(parser, \
        AFW__FILE_LINE__, message_z); \
    longjmp(((parser->xctx)->current_try->throw_jmp_buf), \
        (afw_error_code_syntax)); \
} while (0)


/*  Macro used to set parse error in xctx and throw it. */
#define AFW_COMPILE_THROW_ERROR_FZ(format_z, ...) \
do { \
    afw_compile_parse_set_error_fz(parser, \
        AFW__FILE_LINE__, format_z, __VA_ARGS__); \
    longjmp(((parser->xctx)->current_try->throw_jmp_buf), \
        (afw_error_code_syntax)); \
} while (0)



/* Check of unexpected end. */
#define AFW_COMPILE_EOF_IS_ERROR() \
if (afw_compile_is_at_eof()) \
    AFW_COMPILE_THROW_ERROR_Z("Unexpected end")



AFW_DECLARE_INTERNAL(afw_code_point_t)
afw_compile_get_code_point_impl(afw_compile_parser_t *parser);

#define afw_compile_get_code_point() \
    afw_compile_get_code_point_impl(parser)

AFW_DECLARE_INTERNAL(afw_code_point_t)
afw_compile_get_unescaped_code_point_impl(afw_compile_parser_t *parser);

#define afw_compile_get_unescaped_code_point() \
    afw_compile_get_unescaped_code_point_impl(parser)


AFW_DECLARE_INTERNAL(afw_boolean_t)
afw_compile_is_reserved_word(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s);


AFW_DECLARE_INTERNAL(afw_boolean_t)
afw_compile_next_raw_starts_with_impl(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s);



#define afw_compile_next_raw_starts_with(s) \
    afw_compile_next_raw_starts_with_impl(parser, s)



AFW_DECLARE_INTERNAL(afw_boolean_t)
afw_compile_next_raw_starts_with_z_impl(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *s_z);



#define afw_compile_next_raw_starts_with_z(s_z) \
    afw_compile_next_raw_starts_with_z_impl(parser, s_z)



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_StatementList(
    afw_compile_parser_t *parser,
    afw_boolean_t end_is_close_brace,
    afw_boolean_t end_is_close_brace_case_or_default,   
    afw_boolean_t can_be_single_return_expression);



AFW_DECLARE_INTERNAL(void)
afw_compile_get_raw_line_impl(
    afw_compile_parser_t *parser,
    afw_utf8_t *line);



#define afw_compile_get_raw_line(line) \
    afw_compile_get_raw_line_impl(parser, line)



AFW_DECLARE_INTERNAL(const afw_utf8_t *)
afw_compile_current_raw_token(
    afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_utf8_t *)
afw_compile_get_string_literal(
    afw_compile_parser_t *parser,
    const afw_utf8_octet_t *s,
    afw_size_t len);



AFW_DECLARE_INTERNAL(void)
afw_compile_get_token_impl(
    afw_compile_parser_t *parser);



/* Get the next token. */
#define afw_compile_get_token() \
    afw_compile_get_token_impl(parser)



/* Get the next token before end of line. */
#define afw_compile_get_token_before_eol() \
    parser->get_token_before_eol = true; \
    afw_compile_get_token_impl(parser)



#define afw_compile_save_offset(save) \
    save = parser->token->token_source_offset



#define afw_compile_get_token_and_save_offset(save) \
    afw_compile_get_token_impl(parser); \
    save = parser->token->token_source_offset



void
afw_compile_reuse_token_impl(afw_compile_parser_t *parser);



/*
 * Set in parse and reset in afw_compile_get_token() to indicate that next
 * token can be an operator.  This is used to determine if '+' and '-' are
 * unary and if '<' and '>' are comparisons.
 */
#define afw_compile_next_can_be_operator() \
    parser->next_can_be_nonunary_operator = true



/*
 * Set in parse and reset in afw_compile_get_token() to indicate that next
 * token can be a grave string.  This is used to determine if '`' is just
 * a grave token or a grave string.
 */
#define afw_compile_next_can_be_template_string() \
    parser->next_can_be_template_string = true


/*
 * Set in parse and reset in afw_compile_get_token() to indicate that next
 * identifier is not be interpreted as a literal. Therefore, true, NaN and other
 * special literals will be returned as an identifier instead of their
 * corresponding value.
 */
#define afw_compile_next_identifier_is_not_special_literal() \
    parser->next_identifier_is_not_special_literal = true


#define afw_compile_reuse_token() \
    afw_compile_reuse_token_impl(parser)



#define afw_compile_save_cursor(save) \
    save = parser->cursor



#define afw_compile_restore_cursor(save) \
do { \
    parser->last_octet_eof = false; \
    parser->cursor = save; \
} while (0)



#define afw_compile_cursor_equal(save) \
    (parser->cursor == save)



#define afw_compile_source_buffer_at(save) \
    (parser->full_source->s + save)



#define afw_compile_source_buffer_length_from(save) \
    (parser->cursor - save)



AFW_DECLARE_INTERNAL(const afw_compile_value_contextual_t *)
afw_compile_create_contextual(
    afw_compile_parser_t *parser,
    afw_size_t start_offset,
    afw_size_t size);


#define afw_compile_create_contextual_to_cursor(start_offset) \
    afw_compile_create_contextual(parser, start_offset, \
        parser->cursor - start_offset)



AFW_DECLARE_INTERNAL(const afw_utf8_t *)
afw_compile_create_source_location_impl(
    afw_compile_parser_t *parser,
    afw_size_t start_offset);



#define afw_compile_create_source_location(start_offset) \
    afw_compile_create_source_location_impl(parser, \
    start_offset)


AFW_DEFINE_STATIC_INLINE(afw_code_point_t)
afw_compile_peek_code_point(afw_compile_parser_t *parser)
{
    afw_size_t cursor;
    afw_code_point_t cp;

    afw_compile_save_cursor(cursor);
    cp = afw_compile_get_code_point();
    afw_compile_restore_cursor(cursor);
    return cp;
}


/* Push cp on parser->s */
AFW_DECLARE_INTERNAL(void)
afw_compile_internal_s_push_code_point(
    afw_compile_parser_t *parser,
    afw_code_point_t cp);



/* Parse error. */
AFW_DECLARE_INTERNAL(void)
afw_compile_parse_set_error_z(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *message_z);



/* Parse error with format. */
AFW_DECLARE_INTERNAL(void)
afw_compile_parse_set_error_fz(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z, ...);



AFW_DECLARE_INTERNAL(afw_value_block_symbol_t *)
afw_compile_parse_get_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name);



AFW_DECLARE_INTERNAL(afw_value_block_symbol_t *)
afw_compile_parse_get_local_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name);



AFW_DECLARE_INTERNAL(afw_value_block_symbol_t *)
afw_compile_parse_add_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name);



AFW_DECLARE_INTERNAL(const afw_value_block_t *)
afw_compile_parse_link_new_value_block(
    afw_compile_parser_t *parser,
    afw_size_t start_offset);



#define afw_compile_parse_pop_value_block(parser) \
    (parser)->compiled_value->current_block = \
        (parser)->compiled_value->current_block->parent_block



/*
 * Create the appropriate value for identifier passed.  Either
 * variable_reference, qualified_variable_reference, or
 * function_definition.
 */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_reference_create(
    afw_compile_parser_t *parser,
    const afw_compile_value_contextual_t *contextual,
    const afw_utf8_t *identifier);



AFW_DECLARE_INTERNAL(void)
afw_compile_parse_check_symbol(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    afw_size_t symbol_cursor);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Assignment(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression);



AFW_DECLARE_INTERNAL(void)
afw_compile_parse_AssignmentBindingTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_value_type_t **type,
    const afw_value_t **value);



AFW_DECLARE_INTERNAL(const afw_compile_list_destructure_t *)
afw_compile_parse_AssignmentListDestructureTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);



AFW_DECLARE_INTERNAL(afw_compile_assignment_element_t *)
afw_compile_parse_AssignmentElement(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentExpression(
    afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_compile_object_destructure_t *)
afw_compile_parse_AssignmentObjectDestructureTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);



AFW_DECLARE_INTERNAL(afw_compile_assignment_property_t *)
afw_compile_parse_AssignmentProperty(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);



/* Parameter was_expression is used to support single return expression */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentStatement(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Comparison(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_EntryFunctionLambdaOrVariableReference(
    afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Equality(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Exponentiation(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Evaluation(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Expression(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTuple(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleAnnotation(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleOperation(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleParameter(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Factor(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_script_function_signature_t *)
afw_compile_parse_FunctionSignature(
    afw_compile_parser_t *parser,
    const afw_value_block_t **block,
    const afw_value_string_t **function_name_value);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_FunctionSignatureAndBody(
    afw_compile_parser_t *parser,
    const afw_value_string_t **function_name_value);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Json(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Literal(
    afw_compile_parser_t *parser,
    afw_boolean_t *is_Literal,
    afw_boolean_t must_be_literal,
    afw_boolean_t scalar_only);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Lambda(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_List(
    afw_compile_parser_t *parser,
    afw_boolean_t allow_expression,
    afw_boolean_t allow_enhanced_literals);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_LogicalExpression(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_LogicalAnd(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_NullishCoalescing(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Object(
    afw_compile_parser_t *parser,
    afw_boolean_t allow_expression,
    afw_boolean_t allow_enhanced_literals);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_OptionalDefineAssignment(
    afw_compile_parser_t *parser);


AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_OptionalDefineTarget(
    afw_compile_parser_t *parser);

AFW_DECLARE_INTERNAL(const afw_value_type_t *)
afw_compile_parse_OptionalType(
    afw_compile_parser_t *parser,
    afw_boolean_t is_return);

AFW_DECLARE_INTERNAL(void)
afw_compile_parse_Parameters(
    afw_compile_parser_t *parser,
    afw_compile_args_t *args);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_ParenthesizedExpression(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Prefixed(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Reference(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Script(
    afw_compile_parser_t *parser);



/* If was_expression is NULL, statement can not be Expression. */
AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Statement(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Substitution(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Template(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_TemplateString(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Term(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_TestScript(
    afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_type_t *)
afw_compile_parse_Type(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Value(afw_compile_parser_t *parser);



AFW_DECLARE_INTERNAL(const afw_value_t *)
afw_compile_parse_Hybrid(afw_compile_parser_t *parser);


AFW_DEFINE_INTERNAL(const afw_value_variable_reference_t *)
afw_compile_parse_variable_reference_create(
    afw_compile_parser_t *parser,
    const afw_compile_value_contextual_t *contextual,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_utf8_t *identifier);


/* Skip white space and comments. */
AFW_DECLARE_INTERNAL(void)
afw_compile_skip_ws(afw_compile_parser_t *parser);



/* Make sure there is no residual data. */
AFW_DECLARE_INTERNAL(void)
afw_compile_check_for_residual(afw_compile_parser_t *parser);


AFW_STACK_STRUCT(afw_compile_internal_args_s, const afw_value_t *);


/* Create instance of args. */
#define afw_compile_args_create(parser) \
afw_stack_create(afw_compile_args_t, 10, 0, true, \
    (parser)->p, (parser)->xctx)



/* Add a value to args. */
#define afw_compile_args_add_value(args, value) \
afw_stack_push(args, (parser)->xctx) = value



/* Get argc and argv from args and release work resources. */
#define afw_compile_args_finalize(args, argc, argv) \
afw_stack_copy_and_release((args), (argc), (argv), \
    (parser)->p, (parser)->xctx)


/* Make sure there is no residual data. */
AFW_DECLARE_INTERNAL(void)
afw_compile_check_for_residual(afw_compile_parser_t *parser);



/* Create a parser. */
AFW_DECLARE_INTERNAL(afw_compile_parser_t *)
afw_compile_lexical_parser_create(
    const afw_utf8_t *source,
    afw_utf8_octet_get_cb_t callback,
    void *callback_data,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    afw_compile_residual_check_t residual_check,
    afw_boolean_t cede_p,
    const afw_value_compiled_value_t *parent,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/* Call when finished with parser. */
AFW_DECLARE_INTERNAL(void)
afw_compile_lexical_parser_finish_and_release(
    afw_compile_parser_t *parser,
    afw_xctx_t *xctx);



#define AFW_COMPILE_THROW_EXPECTING_SEMICOLON \
    AFW_COMPILE_THROW_ERROR_Z("Expecting statement terminator ';'")



#define AFW_COMPILE_ASSERT_TOKEN_IS_SEMICOLON \
    if (!afw_compile_token_is(semicolon)) \
        AFW_COMPILE_THROW_EXPECTING_SEMICOLON



#define AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON \
    afw_compile_get_token(); \
    AFW_COMPILE_ASSERT_TOKEN_IS_SEMICOLON



AFW_END_DECLARES

/** @} */

#endif /* __AFW_COMPILE_INTERNAL_H__ */

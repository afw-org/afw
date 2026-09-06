// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Internal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_COMPILE_INTERNAL_H__
#define __AFW_COMPILE_INTERNAL_H__

#include "afw.h"

/**
 * @addtogroup afw_compile_internal
 * @{
 */

/**
 * @file afw_compile_internal.h
 * @brief Internal compiler state, tokens, macros, and parse declares.
 *
 * Layout: (1) types/enums/structs, (2) hot path macros,
 * (3) support declares + small static inlines that need those
 * declares, (4) production parse declares alphabetically.
 *
 * Usage notes (see section comments too):
 * - Most hot macros assume a local variable named `parser`
 *   (`afw_compile_parser_t *`). Call sites use that name.
 * - Production names match EBNF harvest blocks in the
 *   corresponding `afw_compile_parse_*.c` files.
 * - Compile policy (type_check, etc.) lives on
 *   `compiled_value->compile_policy`, not on the parser. Type-check
 *   helpers use contextual→compiled_value (see `AFW_VALUE_TYPE_CHECK_*`
 *   in afw_value.h).
 */

AFW_BEGIN_DECLARES

/* Token kinds produced by the lexical scanner. */
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
    /*
     * #Name — pound_identifier (no qualifier): author pragmas (#compile)
     * and compiler-internal decompile forms (#block, …). identifier_name is
     * the spelling after '#'; identifier is full "#name".
     * Not a bare '#'; that is pound_sign (token only, no EBNF production).
     */
    afw_compile_token_type_pound_identifier,
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
    /*
     * '#{' one token (not '#' then '{'). Opens CompileTimeSubstitution.
     */
    afw_compile_token_type_compile_time_substitute_start,    /* #{ */
    /*
     * Bare '$' only — not a production by itself; '$' can start Identifier.
     * '${' is evaluation_time_substitute_start.
     */
    afw_compile_token_type_dollar_sign,                      /* $ */
    afw_compile_token_type_evaluation_time_substitute_start, /* ${ */
    afw_compile_token_type_ellipsis,                     /* ... */
    afw_compile_token_type_fat_arrow,                    /* => */
    afw_compile_token_type_grave,                        /* ` */
    afw_compile_token_type_nullish_coalescing,           /* ?? */
    afw_compile_token_type_optional_chaining,            /* ?. */
    afw_compile_token_type_optional_chaining_thin_arrow, /* ?-> */
    afw_compile_token_type_percent,                      /* % */
    afw_compile_token_type_period,                       /* . */
    /*
     * Bare '#' only (not followed by '{' or IdentifierStart). Token only —
     * not an EBNF production; parse rejects unless a production lists it.
     */
    afw_compile_token_type_pound_sign,                   /* # */
    afw_compile_token_type_question_mark,                /* ? */
    afw_compile_token_type_semicolon,                    /* ; */
    afw_compile_token_type_slash,                        /* / */
    afw_compile_token_type_thin_arrow,                   /* -> */
    afw_compile_token_type_tilde,                        /* ~ */
    afw_compile_token_type_vertical_bar,                 /* | */

    /*
     * Unary operators. Same spellings as binary + / - use
     * next_can_be_operator() so the lexer can pick unary vs binary.
     */
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
    /* Angle brackets vs comparisons: also gated by next_can_be_operator(). */
    afw_compile_token_type_open_angle_bracket,           /* < */
    afw_compile_token_type_close_angle_bracket,          /* > */

    /* Arithmetic operators (binary + / - when next_can_be_operator). */
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
     * Interned string values (compile_literal or reusable env strings).
     * Key is octets; value is const afw_value_string_t *. Use
     * afw_compile_get_string_literal().
     */
    apr_hash_t *string_literals;

};


/* One scanned token; parser->token points into parser token storage. */
struct afw_compile_internal_token_s {

    afw_compile_internal_token_type_t type;

    /* Span in full_source for errors / contextual creation. */
    afw_size_t token_source_offset;

    afw_size_t token_source_len;

    /* Additional information for some tokens. */
    union {

        /*
         * If type is identifier or pound_identifier:
         *
         * identifier_name — unqualified name (no '#' for pragmas).
         * identifier_qualifier — for identifier only; empty/NULL for
         *     pound_identifier (#Name is never qualified).
         * identifier — full spelling: optional "qualifier::" + name for
         *     identifier; "#name" for pound_identifier (useful in errors).
         *
         * If identifier has no qualifier, identifier equals identifier_name
         * and identifier_qualifier is empty. For pound_identifier,
         * identifier always includes the leading '#'.
         */
        struct {
            const afw_value_string_t *identifier;
            const afw_value_string_t *identifier_name;
            const afw_value_string_t *identifier_qualifier;
        };

        /* If type is boolean, process permanent true/false. */
        const afw_value_boolean_t *boolean;

        /* If type is integer, permanent or compile_literal. */
        const afw_value_integer_t *integer;

        /* If type is null, this is NULL. */
        void *null;

        /* If type is undefined, use identifier to hold 'undefined'. */

        /* If type is number, permanent (Inf/NaN/-0) or compile_literal. */
        const afw_value_double_t *number;

        /*
         * If type is quoted string, interned compile_literal (or empty
         * permanent) and quote character (single (') or double (")).
         */
        struct {
            const afw_value_string_t *string;
            afw_utf8_octet_t string_quote_character;
        };

        /* Used for afw_compile_token_type_raw_line. */
        afw_utf8_t raw_line;
    };

};


#define AFW_COMPILE_MAX_TOKENS 5

/*
 * Max recursive Type / destructure nesting. Stops C-stack overflow on
 * ((((…)))) types and [[[…]]] patterns. Generous for real scripts.
 */
#define AFW_COMPILE_PARSE_NESTING_MAX 256


/**
 * @brief Source span + owning compiled unit for a value.
 *
 * Attached to many afw_value_*_t structs for errors, decompile, and
 * type-check policy resolution via compiled_value.
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


/* Active loop labels while parsing (issue #62). Innermost first. */
typedef struct afw_compile_loop_label_s {
    const afw_value_string_t *name;
    struct afw_compile_loop_label_s *next;
} afw_compile_loop_label_t;

/*
 * Mutable parse state for one compile. Not public API.
 * Hot macros below expect the local name `parser` for this pointer.
 */
struct afw_compile_internal_parser_s {

    /* Pool used for everything returned. */
    const afw_pool_t *p;

    /* Some things still use apr pools. */
    apr_pool_t *apr_p;

    /* The execution context (xctx) of caller to parser. */
    afw_xctx_t *xctx;

    /* Shared resources. */
    const afw_compile_shared_t *shared;

    /* True if this parser created shared (not caller-passed). */
    afw_boolean_t shared_created;

    /* This is the value returned by the compiler. */
    afw_value_compiled_value_t *compiled_value;

    /**
     * Unit contextual (compiled_value set at parser create). Prefer
     * &parser->contextual for type-check helpers during parse.
     */
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

    const afw_value_t *property_name;

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
     * Recursive Type / destructure nesting (UnionType and
     * AssignmentBindingTarget list/object). See AFW_COMPILE_PARSE_NESTING_MAX.
     */
    afw_size_t parse_nesting;

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

    /* Active loop labels for break/continue Identifier (issue #62). */
    afw_compile_loop_label_t *loop_labels;

    /*
     * Script type/interface name table for this compile (issue #28).
     * Key: type name (afw_utf8_t *), value: const afw_value_type_t *
     * (a reference placeholder; resolved is the body).
     * NULL until first type/interface statement.
     */
    apr_hash_t *script_type_names;

    /**
     * While parsing a function/lambda body: declared return type (may be
     * NULL / any). Used for compile-time return checks (issue #28).
     */
    const afw_value_type_t *current_function_returns;

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
    XX(let,                                                                     \
        "This is an assignment to a new local variable. ")                      \
                                                                                \
    XX(parameter,                                                               \
        "This introduces a function/lambda parameter (or Pattern leaf).")       \
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
    XX(symbol_reference,                                                      \
        "This is a symbol reference.")                                        \

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
    /*
     * Tagged by target_type. For symbol_reference targets, type annotations
     * live on symbol_reference->symbol->type (set at symbol create), not as a
     * second field here. variable_type is only for non-symbol uses if any;
     * it aliases the same storage as symbol_reference — do not set both.
     */
    union {
        const afw_compile_list_destructure_t *list_destructure;
        const afw_compile_object_destructure_t *object_destructure;
        union {
            const afw_value_type_t *variable_type;
            const afw_value_symbol_reference_t *symbol_reference;
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
        /*
         * If is_rename is true: static property_name and/or computed
         * property_name_expr (issue #140 / #38-style [expr] keys). Exactly
         * one of property_name / property_name_expr is non-NULL.
         * property_name_was_string: decompile should quote static name.
         */
        struct {
            const afw_value_t *property_name;
            const afw_value_t *property_name_expr;
            const afw_compile_assignment_element_t *assignment_element;
            afw_boolean_t property_name_was_string;
        };
        /* If is_rename is false. */
        struct {
            const afw_value_symbol_reference_t *symbol_reference;
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

typedef struct afw_compile_parse_StatementList_cb_s {
    void (*func) (
        struct afw_compile_parse_StatementList_cb_s *cb,
        afw_compile_parser_t *parser,
        const afw_value_block_t *block,
        afw_compile_args_t *statements);
} afw_compile_parse_StatementList_cb_t;

/* Work stack for building argument / statement lists while parsing. */
AFW_STACK_STRUCT(afw_compile_internal_args_s, const afw_value_t *);


/*
 * ---------- Hot macros ----------
 *
 * Prefer these over calling *_impl directly from parse code.
 * Unless a macro takes an explicit parser parameter, it refers to a
 * local variable named `parser`.
 *
 * Token check macros take the enum *suffix* only, e.g.
 *   afw_compile_token_is(semicolon)
 * expands to compare against afw_compile_token_type_semicolon.
 */

/* Resolve contextual offset/size into a utf8 view of full_source. */
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

#define afw_compile_token_identifier_name() \
    (&(parser)->token->identifier_name->internal)

#define afw_compile_token_identifier() \
    ((parser)->token->identifier \
        ? &(parser)->token->identifier->internal : NULL)

#define afw_compile_token_identifier_qualifier() \
    ((parser)->token->identifier_qualifier \
        ? &(parser)->token->identifier_qualifier->internal : NULL)

#define afw_compile_token_identifier_name_value() \
    (&(parser)->token->identifier_name->pub)

#define afw_compile_token_identifier_value() \
    ((parser)->token->identifier \
        ? &(parser)->token->identifier->pub : NULL)

#define afw_compile_token_string() \
    (&(parser)->token->string->internal)

#define afw_compile_token_string_value() \
    (&(parser)->token->string->pub)

#define afw_compile_token_integer() \
    ((parser)->token->integer->internal)

#define afw_compile_token_integer_value() \
    (&(parser)->token->integer->pub)

#define afw_compile_token_number() \
    ((parser)->token->number->internal)

#define afw_compile_token_number_value() \
    (&(parser)->token->number->pub)

#define afw_compile_token_boolean() \
    ((parser)->token->boolean->internal)

#define afw_compile_token_boolean_value() \
    (&(parser)->token->boolean->pub)

#define afw_compile_token_is_name(string_value) \
    (afw_compile_token_is_unqualified_identifier() && \
    (parser)->token->identifier_name == \
        (const afw_value_string_t *)(string_value))

#define afw_compile_is_at_eof() \
    (parser->last_octet_eof)

#define afw_compile_next_is_at_eof() \
    (parser->last_octet_eof || \
    (parser->all_eof && (parser->cursor == parser->cursor_eof)) )

/* Look ahead one token without permanently consuming it (get + reuse). */
#define afw_compile_peek_next_token() \
    afw_compile_peek_next_token_impl(parser)

#define afw_compile_peek_next_token_is(token_type) \
    (afw_compile_peek_next_token_impl(parser) == \
        afw_compile_token_type_##token_type)

/* Point xctx error at this unit's contextual / cursor (before throws). */
#define afw_compile_set_contextual() \
    parser->xctx->error->contextual = &parser->contextual; \
    parser->xctx->error->parser_cursor = parser->cursor

/* Set parse error in xctx and longjmp (syntax). Requires active AFW try. */
#define AFW_COMPILE_THROW_ERROR_Z(message_z) \
do { \
    afw_compile_parse_set_error_z(parser, \
        AFW__FILE_LINE__, message_z); \
    afw_error_processing_throw((parser)->xctx, afw_error_code_syntax); \
} while (0)

#define AFW_COMPILE_THROW_ERROR_FZ(format_z, ...) \
do { \
    afw_compile_parse_set_error_fz(parser, \
        AFW__FILE_LINE__, format_z, __VA_ARGS__); \
    afw_error_processing_throw((parser)->xctx, afw_error_code_syntax); \
} while (0)

/*
 * Recursive Type / destructure nesting. Assumes local `parser`.
 * See AFW_COMPILE_PARSE_NESTING_MAX.
 */
#define afw_compile_parse_nesting_enter(parser) \
do { \
    AFW_XCTX_THROW_IF_TERMINATING((parser)->xctx); \
    (parser)->parse_nesting++; \
    if ((parser)->parse_nesting > AFW_COMPILE_PARSE_NESTING_MAX) { \
        AFW_COMPILE_THROW_ERROR_Z( \
            "Type or pattern nesting is too deep"); \
    } \
} while (0)

#define afw_compile_parse_nesting_leave(parser) \
do { \
    if ((parser)->parse_nesting > 0) { \
        (parser)->parse_nesting--; \
    } \
} while (0)

/* Unexpected end of input. */
#define AFW_COMPILE_EOF_IS_ERROR() \
if (afw_compile_is_at_eof()) \
    AFW_COMPILE_THROW_ERROR_Z("Unexpected end")

#define AFW_COMPILE_THROW_EXPECTING_SEMICOLON \
    AFW_COMPILE_THROW_ERROR_Z("Expecting statement terminator ';'")

#define AFW_COMPILE_ASSERT_TOKEN_IS_SEMICOLON \
    if (!afw_compile_token_is(semicolon)) \
        AFW_COMPILE_THROW_EXPECTING_SEMICOLON

#define AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON \
    afw_compile_get_token(); \
    AFW_COMPILE_ASSERT_TOKEN_IS_SEMICOLON

/* Octet/code-point scan (raw source). Prefer macros over *_impl. */
#define afw_compile_get_code_point() \
    afw_compile_get_code_point_impl(parser)

#define afw_compile_get_unescaped_code_point() \
    afw_compile_get_unescaped_code_point_impl(parser)

#define afw_compile_next_raw_starts_with(s) \
    afw_compile_next_raw_starts_with_impl(parser, s)

#define afw_compile_next_raw_starts_with_z(s_z) \
    afw_compile_next_raw_starts_with_z_impl(parser, s_z)

#define afw_compile_get_raw_line(line) \
    afw_compile_get_raw_line_impl(parser, line)

/* Advance: fill parser->token. Also clears next_can_be_* flags. */
#define afw_compile_get_token() \
    afw_compile_get_token_impl(parser)

/* Like get_token, but stop at EOL (may yield token_type_eol). */
#define afw_compile_get_token_before_eol() \
    parser->get_token_before_eol = true; \
    afw_compile_get_token_impl(parser)

#define afw_compile_save_offset(save) \
    save = parser->token->token_source_offset

#define afw_compile_get_token_and_save_offset(save) \
    afw_compile_get_token_impl(parser); \
    save = parser->token->token_source_offset

/*
 * Set before the next get_token so '+' / '-' are binary and '<' / '>'
 * are comparisons (not unary / angle brackets). Cleared by get_token.
 */
#define afw_compile_next_can_be_operator() \
    parser->next_can_be_nonunary_operator = true

/*
 * Next '`' starts a template string rather than a bare grave token.
 * Cleared by get_token.
 */
#define afw_compile_next_can_be_template_string() \
    parser->next_can_be_template_string = true

/*
 * Next identifier is not a special literal (true, NaN, …). Cleared by
 * get_token.
 */
#define afw_compile_next_identifier_is_not_special_literal() \
    parser->next_identifier_is_not_special_literal = true

/* Put current token back so the next get_token returns it again. */
#define afw_compile_reuse_token() \
    afw_compile_reuse_token_impl(parser)

/* Cursor save/restore for speculative scan (code points, not tokens). */
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

#define afw_compile_create_contextual_to_cursor(start_offset) \
    afw_compile_create_contextual(parser, start_offset, \
        parser->cursor - start_offset)

#define afw_compile_create_source_location(start_offset) \
    afw_compile_create_source_location_impl(parser, \
    start_offset)

/* Pop current_block to its parent (after link_new_value_block). */
#define afw_compile_parse_pop_value_block(parser) \
    (parser)->compiled_value->current_block = \
        (parser)->compiled_value->current_block->parent_block

/* Temporary arg/statement list helpers (uses parser for pool/xctx). */
#define afw_compile_args_create(parser) \
afw_stack_create(afw_compile_args_t, 10, 0, true, \
    (parser)->p, (parser)->xctx)

#define afw_compile_args_add_value(args, value) \
afw_stack_push(args, (parser)->xctx) = value

#define afw_compile_args_finalize(args, argc, argv) \
afw_stack_copy_and_release((args), (argc), (argv), \
    (parser)->p, (parser)->xctx)


/*
 * ---------- Support declares ----------
 *
 * Lexical helpers, errors, symbols, and parser lifecycle. Parse code
 * usually goes through the hot macros above rather than *_impl names.
 */

extern afw_octet_t
afw_compile_get_octet(afw_compile_parser_t *parser);

extern afw_code_point_t
afw_compile_get_code_point_impl(afw_compile_parser_t *parser);

extern afw_code_point_t
afw_compile_get_unescaped_code_point_impl(afw_compile_parser_t *parser);

extern afw_boolean_t
afw_compile_is_reserved_word(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s);

extern afw_boolean_t
afw_compile_next_raw_starts_with_impl(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s);

extern afw_boolean_t
afw_compile_next_raw_starts_with_z_impl(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *s_z);

extern void
afw_compile_get_raw_line_impl(
    afw_compile_parser_t *parser,
    afw_utf8_t *line);

extern const afw_utf8_t *
afw_compile_current_raw_token(
    afw_compile_parser_t *parser);

/* Intern string: env reusable value, else compile_literal in parser->p. */
extern const afw_value_string_t *
afw_compile_get_string_literal(
    afw_compile_parser_t *parser,
    const afw_utf8_octet_t *s,
    afw_size_t len);

/* Intern utf8 as a string value (env hit or compile_literal). */
#define afw_compile_intern_utf8_string(utf8) \
    (afw_compile_get_string_literal((parser), (utf8)->s, (utf8)->len))

#define afw_compile_intern_utf8(utf8) \
    (&afw_compile_intern_utf8_string(utf8)->pub)

extern void
afw_compile_get_token_impl(
    afw_compile_parser_t *parser);

extern void
afw_compile_reuse_token_impl(afw_compile_parser_t *parser);

extern afw_compile_internal_token_type_t
afw_compile_peek_next_token_impl(afw_compile_parser_t *parser);

extern const afw_compile_value_contextual_t *
afw_compile_create_contextual(
    afw_compile_parser_t *parser,
    afw_size_t start_offset,
    afw_size_t size);

extern const afw_utf8_t *
afw_compile_create_source_location_impl(
    afw_compile_parser_t *parser,
    afw_size_t start_offset);

/* Push code point onto parser->s while building a string. */
extern void
afw_compile_internal_s_push_code_point(
    afw_compile_parser_t *parser,
    afw_code_point_t cp);

/* Outer AFW_CATCH: attach parse location info to error. */
extern void
afw_compile_parse_embellish_error(
    afw_compile_parser_t *parser,
    afw_error_t *error);

/* Prefer AFW_COMPILE_THROW_ERROR_* macros from parse code. */
extern void
afw_compile_parse_set_error_z(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *message_z);

extern void
afw_compile_parse_set_error_fz(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z, ...);

/* Symbol table helpers for the current block chain. */
extern afw_value_block_symbol_t *
afw_compile_parse_get_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_value_string_t *name);

extern afw_value_block_symbol_t *
afw_compile_parse_get_local_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_value_string_t *name);

extern afw_value_block_symbol_t *
afw_compile_parse_add_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_value_string_t *name);

/* Push a new value block; pair with afw_compile_parse_pop_value_block. */
extern const afw_value_block_t *
afw_compile_parse_link_new_value_block(
    afw_compile_parser_t *parser,
    afw_size_t start_offset);

extern void
afw_compile_parse_check_symbol(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    afw_size_t symbol_cursor);

/*
 * Reference for identifier: symbol_reference,
 * qualified_variable_reference, or function_definition.
 */
extern const afw_value_t *
afw_compile_parse_reference_create(
    afw_compile_parser_t *parser,
    const afw_compile_value_contextual_t *contextual,
    const afw_utf8_t *identifier);

/* Introduce or resolve a local symbol (let/const/parameter/assign). */
extern const afw_value_symbol_reference_t *
afw_compile_parse_variable_reference_create(
    afw_compile_parser_t *parser,
    const afw_compile_value_contextual_t *contextual,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_value_string_t *identifier,
    const afw_value_type_t *type);

/* Skip white space and comments. */
extern void
afw_compile_skip_ws(afw_compile_parser_t *parser);

/* Enforce residual_check after top-level parse. */
extern void
afw_compile_check_for_residual(afw_compile_parser_t *parser);

/* Create / tear down a parser (public compile entry points use these). */
extern afw_compile_parser_t *
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

extern void
afw_compile_lexical_parser_finish_and_release(
    afw_compile_parser_t *parser,
    afw_xctx_t *xctx);

extern void
afw_compile_shared_release_temp(
    const afw_compile_shared_t *shared,
    afw_xctx_t *xctx);

/**
 * @brief Register a script `type` / `interface` name for this compile unit.
 */
extern void
afw_compile_script_type_register(
    afw_compile_parser_t *parser,
    const afw_value_string_t *name,
    const afw_value_type_t *type);

/**
 * @brief Reserve a `type` / `interface` name before parsing its body.
 *
 * Inserts a reference placeholder so the body of this statement can
 * see the name (self-ref). Other names must already be in the table.
 * Caller sets `reference.resolved` to the body.
 */
extern afw_value_type_t *
afw_compile_script_type_reserve(
    afw_compile_parser_t *parser,
    const afw_value_string_t *name);

/**
 * @brief Reject unproductive `type` alias cycles in this compile unit.
 *
 * Call after each `type` / `interface` body is bound, and at end of
 * compile. Names must already be declared; this does not discover later
 * statements.
 */
extern void
afw_compile_script_types_resolve(
    afw_compile_parser_t *parser);

/**
 * @brief Evaluate a script-function formal (script IR only).
 *
 * Not for Adaptive built-in execute bodies (use AFW_FUNCTION_EVALUATE_*).
 * In afw_function_compiler_internal.c; only call_script_function uses this.
 * Runtime typeCheck on unit → strict assignability; else leaf convert.
 */
extern const afw_value_t *
afw_function_script_evaluate_parameter_with_type(
    const afw_value_t *value,
    afw_size_t parameter_number,
    const afw_value_type_t *type,
    const afw_compile_value_contextual_t *contextual,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Bind a value into an assignment target / Pattern (script IR).
 *
 * Pattern parameters and destructure; target is symbol_reference,
 * assignment_target, etc. In afw_function_compiler_internal.c.
 */
extern void
afw_function_script_assign_pattern(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/* Needs get_code_point_impl declared above (via get_code_point macro). */
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


/*
 * ---------- Production declares (alphabetical) ----------
 *
 * One function per grammar production; implementations live in
 * afw_compile_parse_*.c with matching ebnf harvest comment blocks.
 * Prefer declaring new productions here (not as static file-locals)
 * so other parse units can call them.
 */

extern const afw_value_type_t *
afw_compile_parse_ArrayType(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Assignment(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression);

extern void
afw_compile_parse_AssignmentBindingTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_value_type_t **type,
    const afw_value_t **value);

extern afw_compile_assignment_element_t *
afw_compile_parse_AssignmentElement(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);

extern const afw_value_t *
afw_compile_parse_AssignmentExpression(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression,
    afw_boolean_t *just_expression_okay);

extern const afw_compile_list_destructure_t *
afw_compile_parse_AssignmentListDestructureTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);

extern const afw_compile_object_destructure_t *
afw_compile_parse_AssignmentObjectDestructureTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);

extern const afw_value_t *
afw_compile_parse_AssignmentOperation(
    afw_compile_parser_t *parser,
    const afw_value_t *target,
    afw_boolean_t just_expression_okay,
    afw_boolean_t *was_expression);

extern afw_compile_assignment_property_t *
afw_compile_parse_AssignmentProperty(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);

/* Parameter was_expression is used to support single return expression */
extern const afw_value_t *
afw_compile_parse_AssignmentStatement(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression);

extern const afw_value_t *
afw_compile_parse_AssignmentTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type);

extern const afw_value_t *
afw_compile_parse_Comparison(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_CompileTimeSubstitution(afw_compile_parser_t *parser);

/**
 * @brief Parse compiler-internal #Name in statement position.
 * @param parser
 * @return value if the form produces one; throws if unknown or invalid.
 *
 * Current token must be pound_identifier. See
 * afw_compile_parse_compiler_internal.c. Not for ordinary script authoring.
 */
extern const afw_value_t *
afw_compile_parse_CompilerInternalStatement(afw_compile_parser_t *parser);

/**
 * @brief Parse compiler-internal #Name in value/expression position.
 * @param parser
 * @return adaptive value; throws if unknown or invalid.
 *
 * Current token must be pound_identifier. See
 * afw_compile_parse_compiler_internal.c. Not for ordinary script authoring.
 * Decompile/recompile accept path (#block, #script_function, …).
 */
extern const afw_value_t *
afw_compile_parse_CompilerInternalValue(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_EntryFunctionLambdaOrVariableReference(
    afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Equality(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Evaluation(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_EvaluationTimeSubstitution(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Exponentiation(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Expression(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Factor(afw_compile_parser_t *parser);

extern const afw_value_script_function_signature_t *
afw_compile_parse_FunctionSignature(
    afw_compile_parser_t *parser,
    const afw_value_block_t **block,
    const afw_value_string_t **function_name_value,
    const afw_value_type_t **return_type);

extern const afw_value_t *
afw_compile_parse_FunctionSignatureAndBody(
    afw_compile_parser_t *parser,
    const afw_value_string_t **function_name_value,
    const afw_value_type_t **return_type);

extern const afw_value_type_t *
afw_compile_parse_FunctionType(afw_compile_parser_t *parser);

extern const afw_value_type_t *
afw_compile_parse_IntersectionType(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Json(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Lambda(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_List(
    afw_compile_parser_t *parser,
    afw_boolean_t allow_expression,
    afw_boolean_t allow_enhanced_literals);

extern const afw_value_t *
afw_compile_parse_Literal(
    afw_compile_parser_t *parser,
    afw_boolean_t *is_Literal,
    afw_boolean_t must_be_literal,
    afw_boolean_t scalar_only);

extern const afw_value_t *
afw_compile_parse_LogicalAnd(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_LogicalExpression(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_NullishCoalescing(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Object(
    afw_compile_parser_t *parser,
    afw_boolean_t allow_expression,
    afw_boolean_t allow_enhanced_literals);

extern const afw_value_type_t *
afw_compile_parse_ObjectTypeLiteral(afw_compile_parser_t *parser);

/* afw_compile_parse_OptionalDefineTarget should be called first. */
extern const afw_value_t *
afw_compile_parse_OptionalDefineAssignment(
    afw_compile_parser_t *parser,
    const afw_value_t *target,
    const afw_value_t *define_function);

extern const afw_value_t *
afw_compile_parse_OptionalDefineTarget(
    afw_compile_parser_t *parser,
    const afw_value_t **define_function,
    const afw_value_block_t **block);

extern const afw_value_type_t *
afw_compile_parse_OptionalType(
    afw_compile_parser_t *parser,
    afw_boolean_t is_return);

extern void
afw_compile_parse_Parameters(
    afw_compile_parser_t *parser,
    afw_compile_args_t *args);

extern const afw_value_t *
afw_compile_parse_ParenthesizedExpression(afw_compile_parser_t *parser);

extern const afw_value_type_t *
afw_compile_parse_ParenthesizedOrFunctionType(
    afw_compile_parser_t *parser);

/**
 * @brief Parse statement-position #Name (pound_identifier).
 * @param parser
 * @return value if the form produces one; throws if unknown or invalid.
 *
 * Current token must be pound_identifier. Dispatches author-facing policy
 * pragmas (afw_compile_parse_pragma.c) then compiler-internal forms
 * (afw_compile_parse_CompilerInternalStatement).
 */
extern const afw_value_t *
afw_compile_parse_PragmaStatement(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Prefixed(afw_compile_parser_t *parser);

extern const afw_value_type_t *
afw_compile_parse_PrimaryType(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Reference(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Script(
    afw_compile_parser_t *parser,
    afw_boolean_t end_is_close_brace);

/* If was_expression is NULL, statement can not be Expression. */
extern const afw_value_t *
afw_compile_parse_Statement(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression);

/**
 * @brief Parse a StatementList, optionally into an already-open block.
 * @param use_existing_current_block If true, do not create a new block;
 *        use parser->compiled_value->current_block (must already be set,
 *        e.g. catch Pattern symbols introduced before the body). Still
 *        finalizes and pops that block when building a block (not case lists).
 *
 * Pass false for all historical call sites. Catch with a Pattern binding
 * opens the block early, then calls with true so statements share that block.
 */
extern const afw_value_t *
afw_compile_parse_StatementList(
    afw_compile_parser_t *parser,
    afw_compile_parse_StatementList_cb_t *cb,
    afw_boolean_t end_is_close_brace,
    afw_boolean_t end_is_close_brace_case_or_default,   
    afw_boolean_t can_be_single_return_expression,
    afw_boolean_t use_existing_current_block);

extern const afw_value_t *
afw_compile_parse_Substitution(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Template(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_TemplateString(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Term(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_TestScript(
    afw_compile_parser_t *parser);

extern const afw_value_type_t *
afw_compile_parse_TupleType(afw_compile_parser_t *parser);

extern const afw_value_type_t *
afw_compile_parse_Type(afw_compile_parser_t *parser);

extern const afw_value_type_t *
afw_compile_parse_UnionType(afw_compile_parser_t *parser);

extern const afw_value_t *
afw_compile_parse_Value(afw_compile_parser_t *parser);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_COMPILE_INTERNAL_H__ */

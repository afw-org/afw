// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser — pragmas
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_pragma.c
 * @brief Parse #pragma_identifier in statement and value positions.
 *
 * Lex produces pragma_identifier for #Name (see afw_compile_lexical.c).
 * This file owns the two productions that consume that token:
 *
 *   - PragmaStatement — statement position (policy / compiler flags, etc.)
 *   - PragmaValue     — expression/value position (structural IR forms, etc.)
 *
 * On entry the current token is already pragma_identifier. identifier_name is
 * the name without '#'; identifier is the full "#name" (for errors). There is
 * no qualifier. Dispatch on identifier_name; fall through to unknown if no
 * match. Do not look up names in the Adaptive function registry.
 *
 * Structural pragmas (e.g. #block) call afw_value_* create/finalize APIs;
 * contextual/compiled_value bookkeeping is supplied by the parser.
 */

#include "afw_internal.h"



/*
 * Token must already be pragma_identifier. Returns full "#name" for errors.
 */
static const afw_utf8_t *
impl_pragma_full_name(afw_compile_parser_t *parser)
{
    if (!afw_compile_token_is(pragma_identifier)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Internal error: expecting pragma_identifier");
    }

    if (parser->token->identifier) {
        return parser->token->identifier;
    }
    return parser->token->identifier_name;
}


/*
 * Unknown pragma for the given grammar context.
 * with_name_format_z must include one AFW_UTF8_FMT_Q.
 */
static void
impl_pragma_unknown(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *with_name_format_z,
    const afw_utf8_z_t *without_name_z)
{
    const afw_utf8_t *full;

    full = impl_pragma_full_name(parser);
    if (full) {
        AFW_COMPILE_THROW_ERROR_FZ(with_name_format_z,
            AFW_UTF8_FMT_ARG(full));
    }
    else {
        AFW_COMPILE_THROW_ERROR_Z(without_name_z);
    }
}


/*
 * #block( statementExpression, ... )
 *
 * Builds a block value via link + finalize. Arguments are Expressions (same
 * list shape as function Parameters). Matches decompile of block IR:
 * #block(stmt, ...).
 */
static const afw_value_t *
impl_parse_pragma_block(afw_compile_parser_t *parser)
{
    const afw_value_block_t *block;
    const afw_value_t *expr;
    const afw_value_t **argv;
    afw_compile_args_t *args;
    afw_size_t argc;
    afw_size_t start_offset;
    afw_boolean_t had_value;

    start_offset = parser->token->token_source_offset;

    /* Expect '('. */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '(' after #block");
    }

    block = afw_compile_parse_link_new_value_block(parser, start_offset);
    args = afw_compile_args_create(parser);

    /* Expression list like Parameters, '(' already consumed. */
    for (had_value = false;;) {
        afw_compile_get_token();

        if (afw_compile_token_is(close_parenthesis)) {
            break;
        }

        if (afw_compile_token_is(comma)) {
            if (!had_value) {
                afw_compile_args_add_value(args, NULL);
            }
            had_value = false;
            continue;
        }

        afw_compile_reuse_token();
        expr = afw_compile_parse_Expression(parser);
        afw_compile_args_add_value(args, expr);
        had_value = true;
    }

    afw_compile_args_finalize(args, &argc, &argv);
    afw_value_block_finalize(block, argc, argv, parser->xctx);
    afw_compile_parse_pop_value_block(parser);

    return &block->pub;
}


/* True if current pragma_identifier is #block. */
static afw_boolean_t
impl_pragma_is_block(afw_compile_parser_t *parser)
{
    return parser->token->identifier_name &&
        afw_utf8_equal_utf8_z(parser->token->identifier_name, "block");
}



/*ebnf>>>
 *
 *# Statement-position pragma (#Name …). Token is pragma_identifier.
 *
 * PragmaStatement ::=
 *     '#block' '(' ( Expression ( ',' Expression )* )? ')'
 *
 *<<<ebnf*/
/**
 * Parse a pragma in statement position.
 *
 * Example of adding a known pragma (before the unknown fall-through):
 *
 *   if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "typecheck")) {
 *       // parse tail for #typecheck …; apply parser flags; return value
 *   }
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaStatement(afw_compile_parser_t *parser)
{
    /*
     * Known statement pragmas: compare identifier_name and handle here.
     *
     * Structural #block is also allowed as a statement (expression-statement
     * style) so decompile output can recompile as a script.
     */
    if (impl_pragma_is_block(parser)) {
        return impl_parse_pragma_block(parser);
    }

    /*
     * if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "…")) {
     *     …
     *     return result;
     * }
     */

    impl_pragma_unknown(parser,
        "Unknown pragma statement " AFW_UTF8_FMT_Q,
        "Unknown pragma statement");
    return NULL; /* not reached */
}



/*ebnf>>>
 *
 *# Value/expression-position pragma (#Name …). Token is pragma_identifier.
 *# Structural forms match decompile #implementation_id where implemented.
 *
 * PragmaValue ::=
 *     '#block' '(' ( Expression ( ',' Expression )* )? ')'
 *
 *<<<ebnf*/
/**
 * Parse a pragma in value/expression position.
 *
 * Example of adding a known pragma (before the unknown fall-through):
 *
 *   if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "…")) {
 *       // parse arguments; call create API; return value
 *   }
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaValue(afw_compile_parser_t *parser)
{
    if (impl_pragma_is_block(parser)) {
        return impl_parse_pragma_block(parser);
    }

    /*
     * if (afw_utf8_equal_utf8_z(parser->token->identifier_name, "…")) {
     *     …
     *     return result;
     * }
     */

    impl_pragma_unknown(parser,
        "Unknown pragma value " AFW_UTF8_FMT_Q,
        "Unknown pragma value");
    return NULL; /* not reached */
}

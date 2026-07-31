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


/* True if current pragma_identifier name equals z. */
static afw_boolean_t
impl_pragma_name_is(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *name_z)
{
    return parser->token->identifier_name &&
        afw_utf8_equal_utf8_z(parser->token->identifier_name, name_z);
}


/*
 * Map decompile string ("const", "let", …) to assignment_type enum.
 */
static afw_compile_internal_assignment_type_t
impl_assignment_type_from_utf8(
    afw_compile_parser_t *parser,
    const afw_utf8_t *s)
{
#define XX(id, description) \
    if (afw_utf8_equal_utf8_z(s, #id)) { \
        return afw_compile_assignment_type_ ## id; \
    }
    AFW_COMPILE_INTERNAL_ASSIGNMENT_TYPE_MAP(XX)
#undef XX

    AFW_COMPILE_THROW_ERROR_FZ(
        "Invalid assignment type " AFW_UTF8_FMT_Q " in #assignment_target",
        AFW_UTF8_FMT_ARG(s));
}


/*
 * #assignment_target( assignmentKind, variableName )
 *
 * assignmentKind — string Expression, e.g. "const", "let" (matches decompile).
 * variableName   — bare Identifier or string; introduces a binding for
 *                  const/let (not a lookup). Matches decompile of symbol form:
 *                  #assignment_target("const", x)
 */
static const afw_value_t *
impl_parse_pragma_assignment_target(afw_compile_parser_t *parser)
{
    const afw_compile_value_contextual_t *contextual;
    const afw_value_t *kind_value;
    const afw_utf8_t *kind_string;
    const afw_utf8_t *variable_name;
    const afw_value_symbol_reference_t *symbol_reference;
    afw_compile_assignment_target_t *target;
    afw_compile_internal_assignment_type_t assignment_type;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    contextual = afw_compile_create_contextual_to_cursor(start_offset);

    /* Expect '('. */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '(' after #assignment_target");
    }

    /* Kind string Expression. */
    kind_value = afw_compile_parse_Expression(parser);
    if (!afw_value_is_string(kind_value)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "#assignment_target kind must be a string "
            "(\"const\", \"let\", …)");
    }
    kind_string = &((const afw_value_string_t *)kind_value)->internal;
    assignment_type = impl_assignment_type_from_utf8(parser, kind_string);

    /* ','. */
    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting ',' after assignment kind in #assignment_target");
    }

    /*
     * Variable name: Identifier (binding name, not a reference lookup) or
     * string. Do not parse as Expression — that would require a prior
     * declaration for a bare name.
     */
    afw_compile_get_token();
    if (afw_compile_token_is_unqualified_identifier()) {
        variable_name = parser->token->identifier_name;
        if (afw_compile_is_reserved_word(parser, variable_name)) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Variable name can not be a reserved word");
        }
    }
    else if (afw_compile_token_is(utf8_string)) {
        variable_name = parser->token->string;
    }
    else {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting variable name (identifier or string) in "
            "#assignment_target");
    }

    /* ')' */
    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')' after #assignment_target");
    }

    /* Create symbol in current block (const/let add; others require existing). */
    symbol_reference = afw_compile_parse_variable_reference_create(
        parser, contextual, assignment_type, variable_name, NULL);

    target = afw_pool_calloc_type(parser->p,
        afw_compile_assignment_target_t, parser->xctx);
    target->assignment_type = assignment_type;
    target->target_type =
        afw_compile_assignment_target_type_symbol_reference;
    target->variable_type = NULL;
    target->symbol_reference = symbol_reference;

    return afw_value_assignment_target_create(
        contextual, target, parser->p, parser->xctx);
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
     * Structural #block is allowed as a statement so decompile output can
     * recompile as a script. Policy pragmas land here as well.
     */
    if (impl_pragma_name_is(parser, "block")) {
        return impl_parse_pragma_block(parser);
    }

    /*
     * if (impl_pragma_name_is(parser, "typecheck")) {
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
 *     '#block' '(' ( Expression ( ',' Expression )* )? ')' |
 *     '#assignment_target' '(' Expression ',' ( Identifier | String ) ')'
 *
 *<<<ebnf*/
/**
 * Parse a pragma in value/expression position.
 *
 * Example of adding a known pragma (before the unknown fall-through):
 *
 *   if (impl_pragma_name_is(parser, "…")) {
 *       // parse arguments; call create API; return value
 *   }
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaValue(afw_compile_parser_t *parser)
{
    if (impl_pragma_name_is(parser, "block")) {
        return impl_parse_pragma_block(parser);
    }

    if (impl_pragma_name_is(parser, "assignment_target")) {
        return impl_parse_pragma_assignment_target(parser);
    }

    /*
     * if (impl_pragma_name_is(parser, "…")) {
     *     …
     *     return result;
     * }
     */

    impl_pragma_unknown(parser,
        "Unknown pragma value " AFW_UTF8_FMT_Q,
        "Unknown pragma value");
    return NULL; /* not reached */
}

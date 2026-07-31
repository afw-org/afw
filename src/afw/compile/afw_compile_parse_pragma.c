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



/*ebnf>>>
 *
 *# Statement-position pragma (#Name …). Token is pragma_identifier.
 *# Extend the alternatives below as statement pragmas are implemented.
 *
 * PragmaStatement ::=
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaStatement(afw_compile_parser_t *parser)
{
    /* const afw_utf8_t *name = parser->token->identifier_name; */

    /*
     * Known statement pragmas: compare identifier_name and handle here.
     *
     * if (afw_utf8_equal(name, afw_s_…)) {
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
 *# Extend the alternatives below as value/structural pragmas are implemented
 *# (including IR forms that match decompile #implementation_id).
 *
 * PragmaValue ::=
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaValue(afw_compile_parser_t *parser)
{
    /* const afw_utf8_t *name = parser->token->identifier_name; */

    /*
     * Known value pragmas: compare identifier_name and handle here.
     *
     * if (afw_utf8_equal(name, afw_s_…)) {
     *     …
     *     return result;
     * }
     */

    impl_pragma_unknown(parser,
        "Unknown pragma value " AFW_UTF8_FMT_Q,
        "Unknown pragma value");
    return NULL; /* not reached */
}

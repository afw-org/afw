// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser — pragmas
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_pragma.c
 * @brief Parse author-facing # pragma directives (compile-time policy).
 *
 * Lex produces pragma_identifier for #Name (see afw_compile_lexical.c).
 * That token is shared with compiler-private #implementation_id forms
 * (afw_compile_parse_compiler_private.c). This file owns policy pragmas
 * that script writers may use (today: #typecheck).
 *
 * Statement position: PragmaStatement dispatches known policy pragmas, then
 * hands other #Name forms to CompilerPrivateStatement. Value/expression
 * position does not use this file — see CompilerPrivateValue.
 *
 * On entry the current token is already pragma_identifier. identifier_name is
 * the name without '#'; identifier is the full "#name" (for errors). There is
 * no qualifier.
 */

#include "afw_internal.h"



/* True if current pragma_identifier name equals z. */
static afw_boolean_t
impl_pragma_name_is(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *name_z)
{
    return parser->token->identifier_name &&
        afw_utf8_equal_utf8_z(parser->token->identifier_name, name_z);
}


/*ebnf>>>
 *
 *# Policy pragma (compile-time type-check flags for this compile unit).
 *# Token is already pragma_identifier with name typecheck / typeCheck.
 *# Sets xctx compile type-check flags; no runtime value.
 *
 * TypeCheckPragma ::=
 *     '#typecheck'
 *         ( 'off' | 'on' | 'compileOnly' | 'strict' |
 *           'noImplicitAny' | 'strictNullChecks' | ',' )*
 *         ';'
 *
 *<<<ebnf*/
/*
 * #typecheck [ mode ] [ option ... ] ;
 *
 * Mode (optional; default on/full when any checking is requested):
 *   off | false
 *   on | full | true
 *   compileOnly | compileonly | compile
 *
 * Options (zero or more, optional commas):
 *   noImplicitAny
 *   strictNullChecks
 *   strict  — mode full + noImplicitAny + strictNullChecks
 *
 * Bare #typecheck; enables full typeCheck (same as on).
 */
static const afw_value_t *
impl_parse_typecheck_pragma(afw_compile_parser_t *parser)
{
    afw_boolean_t full;
    afw_boolean_t compile_only;
    afw_boolean_t off;
    afw_boolean_t saw_mode;
    afw_boolean_t set_no_implicit_any;
    afw_boolean_t set_strict_null;
    afw_boolean_t set_strict;

    full = true;
    compile_only = false;
    off = false;
    saw_mode = false;
    set_no_implicit_any = false;
    set_strict_null = false;
    set_strict = false;

    afw_compile_get_token();
    while (afw_compile_token_is_unqualified_identifier() ||
        afw_compile_token_is(comma))
    {
        if (afw_compile_token_is(comma)) {
            afw_compile_get_token();
            continue;
        }

        if (afw_compile_token_is_name_z("off") ||
            afw_compile_token_is_name_z("false"))
        {
            off = true;
            full = false;
            compile_only = false;
            saw_mode = true;
        }
        else if (afw_compile_token_is_name_z("on") ||
            afw_compile_token_is_name_z("full") ||
            afw_compile_token_is_name_z("true"))
        {
            full = true;
            off = false;
            compile_only = false;
            saw_mode = true;
        }
        else if (afw_compile_token_is_name_z("compileOnly") ||
            afw_compile_token_is_name_z("compileonly") ||
            afw_compile_token_is_name_z("compile"))
        {
            compile_only = true;
            full = false;
            off = false;
            saw_mode = true;
        }
        else if (afw_compile_token_is_name_z("noImplicitAny") ||
            afw_compile_token_is_name_z("noimplicitany"))
        {
            set_no_implicit_any = true;
        }
        else if (afw_compile_token_is_name_z("strictNullChecks") ||
            afw_compile_token_is_name_z("strictnullchecks"))
        {
            set_strict_null = true;
        }
        else if (afw_compile_token_is_name_z("strict")) {
            /* Bundle: full check + noImplicitAny + strictNullChecks. */
            set_strict = true;
            full = true;
            off = false;
            compile_only = false;
            saw_mode = true;
            set_no_implicit_any = true;
            set_strict_null = true;
        }
        else {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting off, on, compileOnly, noImplicitAny, "
                "strictNullChecks, or strict after #typecheck");
        }
        afw_compile_get_token();
    }

    if (!afw_compile_token_is(semicolon)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ';' after #typecheck");
    }

    /* Clear mode flags first, then set the requested mode. */
    afw_flag_set(afw_s_a_flag_compile_typeCheck, false, parser->xctx);
    afw_flag_set(afw_s_a_flag_compile_typeCheckCompileOnly, false,
        parser->xctx);

    if (off) {
        /* Mode off: also clear related policy flags for a clean slate. */
        afw_flag_set(afw_s_a_flag_compile_noImplicitAny, false,
            parser->xctx);
        afw_flag_set(afw_s_a_flag_compile_strictNullChecks, false,
            parser->xctx);
    }
    else if (compile_only) {
        afw_flag_set(afw_s_a_flag_compile_typeCheckCompileOnly, true,
            parser->xctx);
    }
    else if (full || saw_mode || set_no_implicit_any || set_strict_null ||
        set_strict)
    {
        /*
         * Default and option-only forms enable full typeCheck so options
         * have an active checking mode to apply to.
         */
        afw_flag_set(afw_s_a_flag_compile_typeCheck, true, parser->xctx);
    }

    if (!off) {
        if (set_no_implicit_any) {
            afw_flag_set(afw_s_a_flag_compile_noImplicitAny, true,
                parser->xctx);
        }
        if (set_strict_null) {
            afw_flag_set(afw_s_a_flag_compile_strictNullChecks, true,
                parser->xctx);
        }
    }

    /* No runtime value; statement is side-effect only. */
    return NULL;
}


/*ebnf>>>
 *
 *# Statement-position #Name. Token is already pragma_identifier.
 *# Author policy pragmas first; other # names are compiler-private
 *# (see CompilerPrivateStatement).
 *
 * PragmaStatement ::=
 *     TypeCheckPragma |
 *     CompilerPrivateStatement
 *
 *<<<ebnf*/
/**
 * Parse a #Name in statement position (pragma or compiler-private).
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaStatement(afw_compile_parser_t *parser)
{
    if (impl_pragma_name_is(parser, "typecheck") ||
        impl_pragma_name_is(parser, "typeCheck"))
    {
        return impl_parse_typecheck_pragma(parser);
    }

    /* Non-policy #Name — compiler-private decompile/recompile forms. */
    return afw_compile_parse_CompilerPrivateStatement(parser);
}

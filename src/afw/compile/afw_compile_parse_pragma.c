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
 * ---------------------------------------------------------------------------
 * # design (author vs compiler-private)
 * ---------------------------------------------------------------------------
 *
 * Lex produces pound_identifier for any #Name (see afw_compile_lexical.c).
 * That token is shared by two product surfaces:
 *
 * | Kind | Where | Audience | Role |
 * |------|--------|----------|------|
 * | **Pragma** (this file) | Statement | Script authors | Per-compile policy |
 * | **Compiler-private** | Statement/value | Toolchain only | decompile → recompile |
 *
 * Compiler-private forms (#block, #script_function, …) live in
 * afw_compile_parse_compiler_private.c — not ordinary authoring.
 *
 * Pattern B — author pragmas (current intent):
 *
 *   • The only planned author pragma family is compile policy: **#compile**.
 *   • Operands are short names of compile:* flags (the part after "compile:"),
 *     same spelling as the flag registry / handbook.
 *   • Flags (env / flag_set / host) are **defaults**, snapshotted into the
 *     compiled unit's afw_compile_policy_t at compile start.
 *   • #compile **mutates only that policy** (never afw_flag_set / process
 *     flags). Sibling compiles and the rest of the request keep their defaults.
 *   • Unmentioned knobs keep the snapshotted defaults; mentioned names are
 *     forced on for this unit (except **off**, which forces the type-check
 *     cluster off on the policy).
 *   • Type checks use AFW_VALUE_TYPE_CHECK_*(contextual, xctx): unit policy
 *     when contextual->compiled_value is set, else process flags.
 *   • New compile:* flags that should be script-overridable become new
 *     allow-listed operands — not new top-level # directives.
 *   • If a future need is not compile policy, add a different #DirectiveName;
 *     do not invent #typecheck-style parallel vocabularies.
 *
 * #compile operands currently allow-listed (see impl_parse_compile_pragma):
 *   off | typeCheck | typeCheckCompileOnly | noImplicitAny |
 *   strictNullChecks | strict | noOptimize
 *
 * Examples:
 *   #compile typeCheck;
 *   #compile typeCheck noImplicitAny;
 *   #compile typeCheckCompileOnly;
 *   #compile strict;
 *   #compile off;
 *
 * Retired: #typecheck / #typeCheck and synonym soup (on/true/full/…).
 *
 * Maintainer pad: designs/pragma-hash-design.md
 * ---------------------------------------------------------------------------
 *
 * Statement position: PragmaStatement dispatches known policy pragmas, then
 * hands other #Name forms to CompilerPrivateStatement. Value/expression
 * position does not use this file — see CompilerPrivateValue.
 *
 * On entry the current token is already pound_identifier. identifier_name is
 * the name without '#'; identifier is the full "#name" (for errors). There is
 * no qualifier.
 */

#include "afw_internal.h"



/* True if current pound_identifier name equals z. */
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
 *# Author compile-policy pragma. Token is already pound_identifier "compile".
 *# Operands are compile:* flag short names (and special 'off'). At least one
 *# operand required. Spaces separate tokens; no commas.
 *
 * CompilePragmaOperand ::=
 *     'off' |
 *     'typeCheck' |
 *     'typeCheckCompileOnly' |
 *     'noImplicitAny' |
 *     'strictNullChecks' |
 *     'strict' |
 *     'noOptimize'
 *
 * CompilePragma ::=
 *     '#compile' CompilePragmaOperand+ ';'
 *
 *<<<ebnf*/
/*
 * #compile <operand>… ;
 *
 * Apply per-compile overrides for allow-listed compile:* flag short names.
 * See file header for the # design (Pattern B).
 *
 * Special operand off — force type-check related flags off for this unit
 * (typeCheck, typeCheckCompileOnly, noImplicitAny, strictNullChecks, strict).
 * Does not clear noOptimize.
 *
 * typeCheck vs typeCheckCompileOnly — last one wins if both appear.
 * strict — same expansion as compile:strict (typeCheck + noImplicitAny +
 * strictNullChecks) on the unit policy only.
 */
static const afw_value_t *
impl_parse_compile_pragma(afw_compile_parser_t *parser)
{
    /*
     * Mode operands are mutually exclusive (last wins). Options accumulate
     * unless mode is off (clears type options) or strict (bundle on policy).
     */
    enum {
        mode_none,
        mode_off,
        mode_typeCheck,
        mode_typeCheckCompileOnly,
        mode_strict
    } mode;

    afw_boolean_t saw_operand;
    afw_boolean_t set_no_implicit_any;
    afw_boolean_t set_strict_null;
    afw_boolean_t set_no_optimize;
    afw_compile_policy_t *policy;

    mode = mode_none;
    saw_operand = false;
    set_no_implicit_any = false;
    set_strict_null = false;
    set_no_optimize = false;
    policy = &parser->compiled_value->compile_policy;

    afw_compile_get_token();
    while (afw_compile_token_is_unqualified_identifier()) {
        saw_operand = true;

        if (afw_compile_token_is_name_z("off")) {
            mode = mode_off;
            set_no_implicit_any = false;
            set_strict_null = false;
            /* noOptimize is independent of type-check off. */
        }
        else if (afw_compile_token_is_name_z("typeCheck")) {
            mode = mode_typeCheck;
        }
        else if (afw_compile_token_is_name_z("typeCheckCompileOnly")) {
            mode = mode_typeCheckCompileOnly;
        }
        else if (afw_compile_token_is_name_z("strict")) {
            mode = mode_strict;
            set_no_implicit_any = false;
            set_strict_null = false;
        }
        else if (afw_compile_token_is_name_z("noImplicitAny")) {
            if (mode == mode_off) {
                mode = mode_none;
            }
            if (mode == mode_strict) {
                /* Explicit options after strict: fall back to typeCheck + opts. */
                mode = mode_typeCheck;
            }
            set_no_implicit_any = true;
        }
        else if (afw_compile_token_is_name_z("strictNullChecks")) {
            if (mode == mode_off) {
                mode = mode_none;
            }
            if (mode == mode_strict) {
                mode = mode_typeCheck;
            }
            set_strict_null = true;
        }
        else if (afw_compile_token_is_name_z("noOptimize")) {
            set_no_optimize = true;
        }
        else {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting off, typeCheck, typeCheckCompileOnly, "
                "noImplicitAny, strictNullChecks, strict, or noOptimize "
                "after #compile");
        }
        afw_compile_get_token();
    }

    if (!saw_operand) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting at least one operand after #compile");
    }

    if (!afw_compile_token_is(semicolon)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ';' after #compile");
    }

    /* Mutate unit policy only — never process flags. */
    if (mode == mode_off) {
        policy->type_check = false;
        policy->type_check_compile_only = false;
        policy->no_implicit_any = false;
        policy->strict_null_checks = false;
    }
    else if (mode == mode_strict) {
        policy->type_check_compile_only = false;
        policy->type_check = true;
        policy->no_implicit_any = true;
        policy->strict_null_checks = true;
    }
    else {
        if (mode == mode_typeCheckCompileOnly) {
            policy->type_check = false;
            policy->type_check_compile_only = true;
        }
        else if (mode == mode_typeCheck) {
            policy->type_check_compile_only = false;
            policy->type_check = true;
        }

        if (set_no_implicit_any) {
            policy->no_implicit_any = true;
        }
        if (set_strict_null) {
            policy->strict_null_checks = true;
        }
    }

    if (set_no_optimize) {
        policy->no_optimize = true;
    }

    /* No runtime value; statement is side-effect only. */
    return NULL;
}


/*ebnf>>>
 *
 *# Statement-position #Name. Token is already pound_identifier.
 *# Author policy pragmas first; other # names are compiler-private
 *# (see CompilerPrivateStatement).
 *
 * PragmaStatement ::=
 *     CompilePragma |
 *     CompilerPrivateStatement
 *
 *<<<ebnf*/
/**
 * Parse a #Name in statement position (pragma or compiler-private).
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_PragmaStatement(afw_compile_parser_t *parser)
{
    if (impl_pragma_name_is(parser, "compile")) {
        return impl_parse_compile_pragma(parser);
    }

    /* Non-policy #Name — compiler-private decompile/recompile forms. */
    return afw_compile_parse_CompilerPrivateStatement(parser);
}

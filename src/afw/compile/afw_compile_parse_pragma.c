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
 * # design (author vs compiler-internal)
 * ---------------------------------------------------------------------------
 *
 * Lex produces pound_identifier for any #Name (see afw_compile_lexical.c).
 * That token is shared by two product surfaces:
 *
 * | Kind | Where | Audience | Role |
 * |------|--------|----------|------|
 * | **Pragma** (this file) | Statement | Script authors | Per-compile policy |
 * | **Compiler-internal** | Statement/value | Toolchain only | decompile → recompile |
 *
 * Compiler-internal # forms (#block, #script_function, …) live in
 * afw_compile_parse_compiler_internal.c (with compiler_internal function
 * category IR) — not ordinary authoring.
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
 *   • Mid-unit: #compile may appear as a statement anywhere; policy applies
 *     **from here on** for later parse/checks in this unit (not process-wide).
 *     Constructs already parsed are not re-checked under the new policy.
 *   • Unmentioned knobs keep the snapshotted defaults; mentioned names are
 *     forced on for this unit (except **noTypeCheck**, which forces the
 *     type-check cluster off; does not clear noOptimize).
 *   • Type checks use AFW_VALUE_TYPE_CHECK_*(contextual, xctx): unit policy
 *     when contextual->compiled_value is set, else process flags.
 *     If a flag seems to "do nothing", the unit may have #compile noTypeCheck,
 *     or the flag was set after that unit was already compiled (snapshot).
 *   • New compile:* flags that should be script-overridable become new
 *     allow-listed operands — not new top-level # directives.
 *   • If a future need is not compile policy, add a different #DirectiveName;
 *     do not invent #typecheck-style parallel vocabularies.
 *
 * #compile operands currently allow-listed (see impl_parse_compile_pragma):
 *   noImplicitAny | noOptimize | noTypeCheck | strict |
 *   strictNullChecks | typeCheck | typeCheckCompileOnly
 *
 * Examples:
 *   #compile typeCheck;
 *   #compile typeCheck noImplicitAny;
 *   #compile typeCheckCompileOnly;
 *   #compile strict;
 *   #compile noTypeCheck;
 *
 * Retired: #typecheck / #typeCheck and synonym soup (on/true/full/…).
 *
 * Maintainer pad: designs/pragma-hash-design.md
 * ---------------------------------------------------------------------------
 *
 * Statement position: PragmaStatement dispatches known policy pragmas, then
 * hands other #Name forms to CompilerInternalStatement. Value/expression
 * position does not use this file — see CompilerInternalValue.
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
    return afw_compile_token_identifier_name() &&
        afw_utf8_equal_utf8_z(afw_compile_token_identifier_name(), name_z);
}


/*ebnf>>>
 *
 *# Author compile-policy pragma. Token is already pound_identifier "compile".
 *# Operands are compile:* flag short names (and special 'noTypeCheck').
 *# At least one operand required. Spaces separate tokens; no commas.
 *
 * CompilePragmaOperand ::=
 *     'noImplicitAny' |
 *     'noOptimize' |
 *     'noTypeCheck' |
 *     'strict' |
 *     'strictNullChecks' |
 *     'typeCheck' |
 *     'typeCheckCompileOnly'
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
 * Special operand noTypeCheck — force type-check related policy off for this
 * unit (typeCheck, typeCheckCompileOnly, noImplicitAny, strictNullChecks,
 * strict). Does not clear noOptimize. Named like noOptimize (not bare "off").
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
     * unless mode is noTypeCheck (clears type options) or strict (bundle).
     */
    enum {
        mode_none,
        mode_noTypeCheck,
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

        if (afw_compile_token_is_name_z("noTypeCheck")) {
            mode = mode_noTypeCheck;
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
            if (mode == mode_noTypeCheck) {
                mode = mode_none;
            }
            if (mode == mode_strict) {
                /* Explicit options after strict: fall back to typeCheck + opts. */
                mode = mode_typeCheck;
            }
            set_no_implicit_any = true;
        }
        else if (afw_compile_token_is_name_z("strictNullChecks")) {
            if (mode == mode_noTypeCheck) {
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
                "Expecting typeCheck, typeCheckCompileOnly, "
                "noImplicitAny, strictNullChecks, strict, noOptimize, "
                "or noTypeCheck after #compile");
        }
        afw_compile_get_token();
    }

    if (!saw_operand) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting at least one operand after #compile "
            "(typeCheck, typeCheckCompileOnly, noImplicitAny, "
            "strictNullChecks, strict, noOptimize, or noTypeCheck)");
    }

    if (!afw_compile_token_is(semicolon)) {
        if (afw_compile_token_is_unqualified_identifier()) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Unknown #compile operand (expecting typeCheck, "
                "typeCheckCompileOnly, noImplicitAny, strictNullChecks, "
                "strict, noOptimize, or noTypeCheck, then ';')");
        }
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting ';' after #compile operands");
    }

    /* Mutate unit policy only — never process flags. */
    if (mode == mode_noTypeCheck) {
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
 *# Author policy pragmas first; other # names are compiler-internal
 *# (see CompilerInternalStatement).
 *
 * PragmaStatement ::=
 *     CompilePragma |
 *     CompilerInternalStatement
 *
 *<<<ebnf*/
/**
 * Parse a #Name in statement position (pragma or compiler-internal).
 */
const afw_value_t *
afw_compile_parse_PragmaStatement(afw_compile_parser_t *parser)
{
    if (impl_pragma_name_is(parser, "compile")) {
        return impl_parse_compile_pragma(parser);
    }

    /* Non-policy #Name — compiler-internal decompile/recompile forms. */
    return afw_compile_parse_CompilerInternalStatement(parser);
}

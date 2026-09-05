// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser — compiler-internal # forms
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_compiler_internal.c
 * @brief Parse compiler-internal #implementation_id forms (decompile/recompile).
 *
 * Lex produces pound_identifier for #Name (see afw_compile_lexical.c). That
 * token is shared with author-facing pragmas (afw_compile_parse_pragma.c).
 * This file owns the non-public accept path: forms that decompile() emits so
 * compile() can rebuild the same compiled-value graph.
 *
 * Also owns **compiler literals** (author-usable # names that fold to
 * permanent values, not call forms): #doubleMax, #doubleMin,
 * #doubleMinSubnormal, #doubleEpsilon, #integerMax, #integerMin, #pi, #e,
 * and aliases #infinity / #inf / #minusInfinity / #nan. See afw_value.h.
 *
 * Policy directives (#compile) live in afw_compile_parse_pragma.c.
 *
 * On entry the current token is already pound_identifier. identifier_name is
 * the name without '#'; identifier is the full "#name" (for errors). There is
 * no qualifier. Dispatch on identifier_name; fall through to unknown if no
 * match. Do not look up names in the Adaptive function registry.
 */

#include "afw_internal.h"



/*
 * Token must already be pound_identifier. Returns full "#name" for errors.
 */
static const afw_utf8_t *
impl_compiler_internal_full_name(afw_compile_parser_t *parser)
{
    if (!afw_compile_token_is(pound_identifier)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Internal error: expecting pound_identifier");
    }

    if (afw_compile_token_identifier()) {
        return afw_compile_token_identifier();
    }
    return afw_compile_token_identifier_name();
}


/*
 * Unknown compiler-internal #Name for the given grammar context.
 * with_name_format_z must include one AFW_UTF8_FMT_Q.
 */
static void
impl_compiler_internal_unknown(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *with_name_format_z,
    const afw_utf8_z_t *without_name_z)
{
    const afw_utf8_t *full;

    full = impl_compiler_internal_full_name(parser);
    if (full) {
        AFW_COMPILE_THROW_ERROR_FZ(with_name_format_z,
            AFW_UTF8_FMT_ARG(full));
    }
    else {
        AFW_COMPILE_THROW_ERROR_Z(without_name_z);
    }
}


/* True if current pound_identifier name equals z. */
static afw_boolean_t
impl_compiler_internal_name_is(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *name_z)
{
    return afw_compile_token_identifier_name() &&
        afw_utf8_equal_utf8_z(afw_compile_token_identifier_name(), name_z);
}


/* Compiler literal #Name → permanent value, or NULL if not one. */
static const afw_value_t *
impl_compiler_numeric_constant(afw_compile_parser_t *parser)
{
    static const struct {
        const afw_utf8_z_t *name_z;
        const afw_value_t * const *value;
    } consts[] = {
        { "doubleMax", &afw_value_double_max },
        { "doubleMin", &afw_value_double_min },
        { "doubleMinSubnormal", &afw_value_double_min_subnormal },
        { "doubleEpsilon", &afw_value_double_epsilon },
        { "integerMax", &afw_value_integer_max },
        { "integerMin", &afw_value_integer_min },
        { "pi", &afw_value_double_pi },
        { "e", &afw_value_double_e },
        { "infinity", &afw_value_infinity },
        { "inf", &afw_value_infinity },
        { "minusInfinity", &afw_value_minus_infinity },
        { "nan", &afw_value_NaN }
    };
    afw_size_t i;

    for (i = 0; i < sizeof(consts) / sizeof(consts[0]); i++) {
        if (impl_compiler_internal_name_is(parser, consts[i].name_z)) {
            return *consts[i].value;
        }
    }
    return NULL;
}


/*
 * #closure_binding is emitted by decompile of a live closure. It holds a
 * runtime enclosing scope that cannot be reconstructed from text alone.
 * Recognized so the error is intentional, not "unknown".
 */
static void
impl_compiler_internal_closure_binding_not_recompilable(
    afw_compile_parser_t *parser)
{
    AFW_COMPILE_THROW_ERROR_Z(
        "#closure_binding is runtime-only (closed-over scope) and cannot "
        "be recompiled from decompile text");
}


/*
 * #function_thunk is C-side only (model adapter hooks, etc.). Decompile
 * prints a label; there is no Adaptive body to recompile.
 */
static void
impl_compiler_internal_function_thunk_not_recompilable(
    afw_compile_parser_t *parser)
{
    AFW_COMPILE_THROW_ERROR_Z(
        "#function_thunk is C-side only and cannot be recompiled from "
        "decompile text");
}


/*ebnf>>>
 *
 *# Compiler-internal #block( statementExpression, ... ).
 *# Token is already pound_identifier with name "block".
 *
 * CompilerInternalBlock ::=
 *     '#block' '(' ( Expression ( ',' Expression )* )? ')'
 *
 *<<<ebnf*/
/*
 * Builds a block value via link + finalize. Arguments are Expressions (same
 * list shape as function Parameters). Matches decompile of block:
 * #block(stmt, ...).
 */
static const afw_value_t *
impl_parse_compiler_internal_block(afw_compile_parser_t *parser)
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


/*ebnf>>>
 *
 *# Target shape for #assignment_target (symbol name or destructure Pattern).
 * AssignmentTargetCompilerInternalShape ::=
 *     Identifier |
 *     String |
 *     AssignmentListDestructureTarget |
 *     AssignmentObjectDestructureTarget
 *
 *# Compiler-internal #assignment_target( assignmentKind, Pattern ).
 *# assignmentKind — string Expression, e.g. "const", "let" (matches decompile).
 *
 * CompilerInternalAssignmentTarget ::=
 *     '#assignment_target' '(' Expression ','
 *         AssignmentTargetCompilerInternalShape ')'
 *
 *<<<ebnf*/
/*
 * Examples:
 *   #assignment_target("const", x)
 *   #assignment_target("const", [a, b])
 *   #assignment_target("const", {a, b: c})
 */
static const afw_value_t *
impl_parse_compiler_internal_assignment_target(afw_compile_parser_t *parser)
{
    const afw_compile_value_contextual_t *contextual;
    const afw_value_t *kind_value;
    const afw_value_t *result;
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
     * Pattern: string name is an extra convenience (not surface Script).
     * Otherwise reuse AssignmentTarget (name | [list] | {object}).
     * Do not parse as Expression — bare names would be lookups.
     */
    afw_compile_get_token();
    if (afw_compile_token_is(utf8_string)) {
        variable_name = afw_compile_token_string();
        symbol_reference = afw_compile_parse_variable_reference_create(
            parser, contextual, assignment_type,
            afw_compile_intern_utf8_string(variable_name), NULL);
        target = afw_pool_calloc_type(parser->p,
            afw_compile_assignment_target_t, parser->xctx);
        target->assignment_type = assignment_type;
        target->target_type =
            afw_compile_assignment_target_type_symbol_reference;
        /* Type (if any) is on symbol->type only; see assignment_target union. */
        target->symbol_reference = symbol_reference;
        result = afw_value_assignment_target_create(
            contextual, target, parser->p, parser->xctx);
    }
    else {
        afw_compile_reuse_token();
        result = afw_compile_parse_AssignmentTarget(parser, assignment_type);
    }

    /* ')' */
    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')' after #assignment_target");
    }

    return result;
}



/*ebnf>>>
 *
 *# Compiler-internal type alias. Name is reserved before Type so self-ref
 *# works. Matches decompile of a `type` statement.
 *
 * CompilerInternalType ::=
 *     '#type' '(' String ',' Type ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_type(afw_compile_parser_t *parser)
{
    const afw_compile_value_contextual_t *contextual;
    const afw_value_t *name_value;
    const afw_value_type_t *type;
    afw_value_type_t *placeholder;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    contextual = afw_compile_create_contextual_to_cursor(start_offset);

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '(' after #type");
    }

    name_value = afw_compile_parse_Expression(parser);
    if (!afw_value_is_string(name_value)) {
        AFW_COMPILE_THROW_ERROR_Z("#type name must be a string");
    }
    placeholder = afw_compile_script_type_reserve(parser,
        (const afw_value_string_t *)name_value);

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ',' after #type name");
    }

    type = afw_compile_parse_Type(parser);
    placeholder->reference.resolved = type;
    afw_compile_script_types_resolve(parser);

    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')' after #type Type");
    }

    return afw_value_script_type_declaration_create(
        contextual, (const afw_value_string_t *)name_value, type, false,
        parser->p, parser->xctx);
}



/*ebnf>>>
 *
 *# Compiler-internal interface. Name is reserved before the body.
 *# Further Type arguments are extends bases.
 *
 * CompilerInternalInterface ::=
 *     '#interface' '(' String ',' Type ( ',' Type )* ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_interface(afw_compile_parser_t *parser)
{
    const afw_compile_value_contextual_t *contextual;
    const afw_value_t *name_value;
    const afw_utf8_t *name;
    const afw_value_type_t *body;
    const afw_value_type_t *base;
    afw_value_type_t *type;
    afw_value_type_t *placeholder;
    apr_array_header_t *extends;
    const afw_value_type_t **list;
    afw_size_t i;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    contextual = afw_compile_create_contextual_to_cursor(start_offset);

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '(' after #interface");
    }

    name_value = afw_compile_parse_Expression(parser);
    if (!afw_value_is_string(name_value)) {
        AFW_COMPILE_THROW_ERROR_Z("#interface name must be a string");
    }
    name = &((const afw_value_string_t *)name_value)->internal;
    placeholder = afw_compile_script_type_reserve(parser,
        (const afw_value_string_t *)name_value);

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ',' after #interface name");
    }

    body = afw_compile_parse_Type(parser);
    if (body->kind != afw_value_type_kind_object) {
        AFW_COMPILE_THROW_ERROR_Z(
            "#interface body must be an object type");
    }

    extends = NULL;
    for (;;) {
        afw_compile_get_token();
        if (afw_compile_token_is(close_parenthesis)) {
            break;
        }
        if (!afw_compile_token_is(comma)) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting ',' or ')' in #interface");
        }
        base = afw_compile_parse_Type(parser);
        if (base &&
            base->kind == afw_value_type_kind_reference &&
            base->reference.name &&
            afw_utf8_equal(base->reference.name, name))
        {
            AFW_COMPILE_THROW_ERROR_FZ(
                "Interface " AFW_UTF8_FMT_Q " cannot extend itself",
                AFW_UTF8_FMT_ARG(name));
        }
        if (!extends) {
            extends = apr_array_make(parser->apr_p, 2,
                sizeof(const afw_value_type_t *));
        }
        APR_ARRAY_PUSH(extends, const afw_value_type_t *) = base;
    }

    type = afw_pool_calloc_type(parser->p, afw_value_type_t, parser->xctx);
    type->kind = afw_value_type_kind_object;
    type->object.properties = body->object.properties;
    type->object.interface_name = name;
    if (extends && extends->nelts > 0) {
        type->object.extends_count = (afw_size_t)extends->nelts;
        list = afw_pool_malloc(parser->p,
            sizeof(afw_value_type_t *) * type->object.extends_count,
            parser->xctx);
        for (i = 0; i < type->object.extends_count; i++) {
            list[i] = ((const afw_value_type_t **)extends->elts)[i];
        }
        type->object.extends = list;
    }

    placeholder->reference.resolved = type;
    afw_compile_script_types_resolve(parser);

    return afw_value_script_type_declaration_create(
        contextual, (const afw_value_string_t *)name_value, type, true,
        parser->p, parser->xctx);
}



/*ebnf>>>
 *
 *# Compiler-internal wrapper for array spread entries. Matches decompile.
 *
 * CompilerInternalListExpression ::=
 *     '#list_expression' '(' Expression ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_list_expression(afw_compile_parser_t *parser)
{
    const afw_value_t *internal;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '(' after #list_expression");
    }

    internal = afw_compile_parse_Expression(parser);

    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting ')' after #list_expression");
    }

    return afw_value_create_array_expression(
        afw_compile_create_contextual_to_cursor(start_offset),
        internal, parser->p, parser->xctx);
}


/*ebnf>>>
 *
 *# Runtime return temp. One Expression is the occupant.
 *
 * CompilerInternalFunctionReturnValue ::=
 *     '#function_return_value' '(' Expression ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_function_return_value(
    afw_compile_parser_t *parser)
{
    const afw_value_t *inner;

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '(' after #function_return_value");
    }

    inner = afw_compile_parse_Expression(parser);

    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting ')' after #function_return_value");
    }

    return afw_value_function_return_value_create(
        inner, parser->p, parser->xctx);
}


/*ebnf>>>
 *
 *# Parts are Expressions (string segments and nested #block expressions).
 *# At least one part is required.
 *
 * CompilerInternalTemplateDefinition ::=
 *     '#template_definition' '(' Expression ( ',' Expression )* ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_template_definition(afw_compile_parser_t *parser)
{
    const afw_value_t *expr;
    const afw_value_t **argv;
    afw_compile_args_t *args;
    afw_size_t argc;
    afw_size_t start_offset;
    afw_boolean_t had_value;

    start_offset = parser->token->token_source_offset;

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '(' after #template_definition");
    }

    args = afw_compile_args_create(parser);
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
    if (argc == 0) {
        AFW_COMPILE_THROW_ERROR_Z(
            "#template_definition requires at least one part");
    }

    return afw_value_template_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc, argv, parser->p, parser->xctx);
}


/*ebnf>>>
 *
 *# #script_function( param* , body [, returnType] ) — decompile form.
 *# Param symbols are introduced before the body is parsed.
 *# A name or Pattern is a parameter only when followed by '?', ':', '=',
 *# or ',' (or after '...'); otherwise it starts the body Expression.
 *
 * ScriptFunctionCompilerInternalParamName ::=
 *     Identifier | String
 *
 * ScriptFunctionCompilerInternalNameParameter ::=
 *     ( '...' )? ScriptFunctionCompilerInternalParamName
 *         ( '?' )? ( ':' Type )? ( '=' Expression )?
 *
 * ScriptFunctionCompilerInternalPatternParameter ::=
 *     ( AssignmentListDestructureTarget |
 *       AssignmentObjectDestructureTarget )
 *         ( '?' )? ( '=' Expression )?
 *
 * ScriptFunctionCompilerInternalParameter ::=
 *     ScriptFunctionCompilerInternalNameParameter |
 *     ScriptFunctionCompilerInternalPatternParameter
 *
 * ScriptFunctionCompilerInternalArgs ::=
 *     ( ScriptFunctionCompilerInternalParameter
 *       ( ',' ScriptFunctionCompilerInternalParameter )*
 *       ',' )?
 *     Expression ( ',' Type )?
 *
 * CompilerInternalScriptFunction ::=
 *     '#script_function' '(' ScriptFunctionCompilerInternalArgs ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_script_function(afw_compile_parser_t *parser)
{
    const afw_value_t *body;
    const afw_value_block_t *block;
    const afw_utf8_t *param_name;
    afw_value_script_function_signature_t *signature;
    afw_value_script_function_parameter_t *param;
    afw_value_block_symbol_t *symbol;
    apr_array_header_t *params;
    const afw_value_block_t *enclosing_block;
    afw_size_t start_offset;
    afw_size_t arg_source_offset;
    afw_boolean_t have_body;
    afw_boolean_t is_rest;
    afw_boolean_t is_identifier_name;

    start_offset = parser->token->token_source_offset;
    enclosing_block = parser->compiled_value->current_block;

    signature = afw_pool_calloc_type(parser->p,
        afw_value_script_function_signature_t, parser->xctx);
    params = apr_array_make(parser->apr_p, 4,
        sizeof(afw_value_script_function_parameter_t *));
    body = NULL;
    have_body = false;

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '(' after #script_function");
    }

    /*
     * Always open a parameter block (even with zero params) so the function
     * has a stable signature->block whose parent is the defining scope.
     */
    block = afw_compile_parse_link_new_value_block(parser, start_offset);
    signature->block = block;

    for (;;) {
        afw_compile_get_token();

        if (afw_compile_token_is(close_parenthesis)) {
            if (!have_body) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "#script_function requires a body Expression");
            }
            break;
        }

        if (have_body) {
            /*
             * Optional trailing return Type after body
             * (#script_function(..., #block(...), integer)).
             */
            if (afw_compile_token_is(comma)) {
                signature->returns = afw_compile_parse_Type(parser);
                afw_compile_get_token();
                if (!afw_compile_token_is(close_parenthesis)) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Expecting ')' after #script_function return Type");
                }
                break;
            }
            if (afw_compile_token_is(close_parenthesis)) {
                break;
            }
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting ',' return Type or ')' after #script_function body");
        }

        is_rest = false;
        if (afw_compile_token_is(ellipsis)) {
            is_rest = true;
            afw_compile_get_token();
        }

        is_identifier_name = afw_compile_token_is_unqualified_identifier();
        if (is_identifier_name || afw_compile_token_is(utf8_string)) {
            arg_source_offset = parser->token->token_source_offset;
            if (is_identifier_name) {
                param_name = afw_compile_token_identifier_name();
                if (afw_compile_is_reserved_word(parser, param_name)) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Parameter name can not be a reserved word");
                }
            }
            else {
                param_name = afw_compile_token_string();
            }

            /*
             * Param if followed by '?', ':', '=', or ',' (or rest already).
             * Otherwise this identifier/string starts the body Expression.
             */
            afw_compile_get_token();
            if (is_rest ||
                afw_compile_token_is(question_mark) ||
                afw_compile_token_is(colon) ||
                afw_compile_token_is(equal) ||
                afw_compile_token_is(comma))
            {
                /* Another parameter after rest is not allowed. */
                if (params->nelts > 0) {
                    afw_value_script_function_parameter_t *prev =
                        ((afw_value_script_function_parameter_t **)
                            params->elts)[params->nelts - 1];
                    if (prev->is_rest) {
                        AFW_COMPILE_THROW_ERROR_Z(
                            "Rest parameter must be last");
                    }
                }
                param = afw_pool_calloc_type(parser->p,
                    afw_value_script_function_parameter_t, parser->xctx);
                param->name = afw_compile_intern_utf8_string(param_name);
                param->is_rest = is_rest;
                symbol = afw_compile_parse_add_symbol_entry(parser,
                    param->name);
                symbol->symbol_type = afw_value_block_symbol_type_parameter;
                param->symbol = symbol;

                if (afw_compile_token_is(question_mark)) {
                    param->is_optional = true;
                    afw_compile_get_token();
                }

                if (afw_compile_token_is(colon)) {
                    afw_compile_reuse_token();
                    param->type = afw_compile_parse_OptionalType(parser,
                        false);
                    if (param->type) {
                        afw_memory_copy(&symbol->type, param->type);
                    }
                    afw_compile_get_token();
                }

                if (afw_compile_token_is(equal)) {
                    param->default_value = afw_compile_parse_Expression(
                        parser);
                    param->is_optional = true;
                    afw_compile_get_token();
                }

                if (afw_compile_token_is(comma)) {
                    /*
                     * Rest may be followed by ',' before the body Expression
                     * (rest is last *parameter*, not last argument).
                     */
                    APR_ARRAY_PUSH(params,
                        afw_value_script_function_parameter_t *) = param;
                    continue;
                }

                if (afw_compile_token_is(close_parenthesis)) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "#script_function requires a body Expression");
                }

                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting ',' or ')' in #script_function parameter");
            }

            /* Not a param — body starts at this name (or ... was wrong). */
            if (is_rest) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting parameter name after '...'");
            }
            afw_compile_restore_cursor(arg_source_offset);
            body = afw_compile_parse_Expression(parser);
            have_body = true;
            continue;
        }

        if (is_rest) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting parameter name after '...'");
        }

        /*
         * Pattern parameter: [ … ] or { … } followed by param-ish tokens
         * or ',' before body. If the body itself is a list/object literal,
         * it must not look like a trailing param (no ',' after) — same
         * ambiguity as name params: Pattern is a param when followed by
         * '?', '=', or ','.
         */
        if (afw_compile_token_is(open_bracket) ||
            afw_compile_token_is(open_brace))
        {
            afw_size_t pattern_start = parser->token->token_source_offset;
            afw_compile_reuse_token();
            {
                const afw_value_t *pat;
                afw_value_script_function_parameter_t *pparam;

                pat = afw_compile_parse_AssignmentTarget(parser,
                    afw_compile_assignment_type_parameter);
                afw_compile_get_token();
                if (afw_compile_token_is(question_mark) ||
                    afw_compile_token_is(equal) ||
                    afw_compile_token_is(comma))
                {
                    if (params->nelts > 0) {
                        afw_value_script_function_parameter_t *prev =
                            ((afw_value_script_function_parameter_t **)
                                params->elts)[params->nelts - 1];
                        if (prev->is_rest) {
                            AFW_COMPILE_THROW_ERROR_Z(
                                "Rest parameter must be last");
                        }
                    }
                    pparam = afw_pool_calloc_type(parser->p,
                        afw_value_script_function_parameter_t,
                        parser->xctx);
                    pparam->assignment_target = pat;
                    if (afw_compile_token_is(question_mark)) {
                        pparam->is_optional = true;
                        afw_compile_get_token();
                    }
                    if (afw_compile_token_is(equal)) {
                        pparam->default_value =
                            afw_compile_parse_Expression(parser);
                        pparam->is_optional = true;
                        afw_compile_get_token();
                    }
                    if (afw_compile_token_is(comma)) {
                        APR_ARRAY_PUSH(params,
                            afw_value_script_function_parameter_t *) =
                            pparam;
                        continue;
                    }
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Expecting ',' after Pattern parameter in "
                        "#script_function");
                }
                /* Not a param — body starts with list/object Expression. */
                afw_compile_restore_cursor(pattern_start);
                body = afw_compile_parse_Expression(parser);
                have_body = true;
                continue;
            }
        }

        /* Body Expression that does not start with a name. */
        afw_compile_reuse_token();
        body = afw_compile_parse_Expression(parser);
        have_body = true;
    }

    afw_compile_parse_pop_value_block(parser);

    signature->count = (afw_size_t)params->nelts;
    signature->parameters =
        (const afw_value_script_function_parameter_t **)params->elts;

    return afw_value_script_function_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        enclosing_block, signature,
        signature->returns, signature->count, signature->parameters,
        body, parser->p, parser->xctx);
}


/*ebnf>>>
 *
 *# Switch default-clause marker (unique permanent null). Optional "()".
 *
 * CompilerInternalSwitchDefault ::=
 *     '#switch_default' ( '(' ')' )?
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_switch_default(afw_compile_parser_t *parser)
{
    afw_compile_get_token();
    if (afw_compile_token_is(open_parenthesis)) {
        afw_compile_get_token();
        if (!afw_compile_token_is(close_parenthesis)) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting ')' after #switch_default");
        }
    }
    else {
        afw_compile_reuse_token();
    }
    return afw_value_unique_default_case_value;
}


/*ebnf>>>
 *
 *# Array value of compiled nodes without evaluating them (for for/switch
 *# statement lists). Unlike array(), elements stay as call/block nodes.
 *
 * CompilerInternalStatements ::=
 *     '#statements' '(' ( Expression ( ',' Expression )* )? ')'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_compiler_internal_statements(afw_compile_parser_t *parser)
{
    const afw_value_t *expr;
    const afw_array_t *list;
    afw_boolean_t had_value;

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '(' after #statements");
    }
    list = afw_array_create_unmanaged(parser->p, parser->xctx);
    for (had_value = false;;) {
        afw_compile_get_token();
        if (afw_compile_token_is(close_parenthesis)) {
            break;
        }
        if (afw_compile_token_is(comma)) {
            if (!had_value) {
                afw_array_push_value(list, NULL, parser->xctx);
            }
            had_value = false;
            continue;
        }
        afw_compile_reuse_token();
        expr = afw_compile_parse_Expression(parser);
        afw_array_push_value(list, expr, parser->xctx);
        had_value = true;
    }
    afw_array_set_immutable(list, parser->xctx);
    return afw_value_create_unmanaged_array(
        list, parser->p, parser->xctx);
}


/*ebnf>>>
 *
 *# Statement-position compiler-internal #Name.
 *# Token is already pound_identifier.
 *# #closure_binding / #function_thunk are recognized as known rejects
 *# (always compile errors; not productions — runtime / C-side only).
 *
 * CompilerInternalStatement ::=
 *     CompilerInternalBlock
 *
 *<<<ebnf*/
/**
 * Parse a compiler-internal form in statement position.
 */
const afw_value_t *
afw_compile_parse_CompilerInternalStatement(afw_compile_parser_t *parser)
{
    /* #block is allowed as a statement so decompile output can recompile. */
    if (impl_compiler_internal_name_is(parser, "block")) {
        return impl_parse_compiler_internal_block(parser);
    }

    if (impl_compiler_internal_name_is(parser, "function_return_value")) {
        return impl_parse_compiler_internal_function_return_value(parser);
    }

    if (impl_compiler_internal_name_is(parser, "closure_binding")) {
        impl_compiler_internal_closure_binding_not_recompilable(parser);
        return NULL; /* not reached */
    }

    if (impl_compiler_internal_name_is(parser, "function_thunk")) {
        impl_compiler_internal_function_thunk_not_recompilable(parser);
        return NULL; /* not reached */
    }

    impl_compiler_internal_unknown(parser,
        "Unknown compiler-internal statement " AFW_UTF8_FMT_Q,
        "Unknown compiler-internal statement");
    return NULL; /* not reached */
}


/*ebnf>>>
 *
 *# Value/expression-position #Name.
 *# Token is already pound_identifier.
 *# Compiler-internal forms match decompile #implementation_id.
 *# Compiler literals fold to permanent values (not call forms).
 *# #closure_binding / #function_thunk are recognized but always compile
 *# errors (runtime / C-side only; not recompilable from decompile text).
 *
 * CompilerLiteral ::=
 *     '#doubleMax' | '#doubleMin' | '#doubleMinSubnormal' |
 *     '#doubleEpsilon' | '#integerMax' | '#integerMin' |
 *     '#pi' | '#e' | '#infinity' | '#inf' | '#minusInfinity' | '#nan'
 *
 * CompilerInternalValue ::=
 *     CompilerInternalBlock |
 *     CompilerInternalAssignmentTarget |
 *     CompilerInternalType |
 *     CompilerInternalInterface |
 *     CompilerInternalListExpression |
 *     CompilerInternalScriptFunction |
 *     CompilerInternalTemplateDefinition |
 *     CompilerInternalSwitchDefault |
 *     CompilerInternalStatements |
 *     CompilerInternalFunctionReturnValue |
 *     CompilerLiteral
 *
 *<<<ebnf*/
/**
 * Parse a compiler-internal form in value/expression position.
 */
const afw_value_t *
afw_compile_parse_CompilerInternalValue(afw_compile_parser_t *parser)
{
    const afw_value_t *numeric;

    if (impl_compiler_internal_name_is(parser, "block")) {
        return impl_parse_compiler_internal_block(parser);
    }

    if (impl_compiler_internal_name_is(parser, "assignment_target")) {
        return impl_parse_compiler_internal_assignment_target(parser);
    }

    if (impl_compiler_internal_name_is(parser, "type")) {
        return impl_parse_compiler_internal_type(parser);
    }

    if (impl_compiler_internal_name_is(parser, "interface")) {
        return impl_parse_compiler_internal_interface(parser);
    }

    if (impl_compiler_internal_name_is(parser, "list_expression")) {
        return impl_parse_compiler_internal_list_expression(parser);
    }

    if (impl_compiler_internal_name_is(parser, "function_return_value")) {
        return impl_parse_compiler_internal_function_return_value(parser);
    }

    if (impl_compiler_internal_name_is(parser, "script_function")) {
        return impl_parse_compiler_internal_script_function(parser);
    }

    if (impl_compiler_internal_name_is(parser, "template_definition")) {
        return impl_parse_compiler_internal_template_definition(parser);
    }

    if (impl_compiler_internal_name_is(parser, "closure_binding")) {
        impl_compiler_internal_closure_binding_not_recompilable(parser);
        return NULL; /* not reached */
    }

    if (impl_compiler_internal_name_is(parser, "function_thunk")) {
        impl_compiler_internal_function_thunk_not_recompilable(parser);
        return NULL; /* not reached */
    }

    if (impl_compiler_internal_name_is(parser, "switch_default")) {
        return impl_parse_compiler_internal_switch_default(parser);
    }

    if (impl_compiler_internal_name_is(parser, "statements")) {
        return impl_parse_compiler_internal_statements(parser);
    }

    numeric = impl_compiler_numeric_constant(parser);
    if (numeric) {
        return numeric;
    }

    impl_compiler_internal_unknown(parser,
        "Unknown compiler-internal value " AFW_UTF8_FMT_Q,
        "Unknown compiler-internal value");
    return NULL; /* not reached */
}

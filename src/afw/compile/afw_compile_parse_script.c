// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Script Parser
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_script.c
 * @brief Adaptive Framework Compiler Script Parser.
 */

#include "afw_internal.h"



/** @fixme Several things will need to change when symbol table is in place. */

static const afw_value_t *
impl_function_definition_break =
    &afw_function_definition_break.pub;

static const afw_value_t *
impl_function_definition_continue =
    &afw_function_definition_continue.pub;

static const afw_value_t *
impl_function_definition_rethrow =
    &afw_function_definition_rethrow.pub;




/*
 * See through compile-time wrap_literal_array(...) (issue #17) for pattern
 * element type checks against the constant array argument.
 */
static const afw_value_t *
impl_unwrap_wrap_literal_array(const afw_value_t *value)
{
    const afw_value_call_built_in_function_t *call;

    if (!afw_value_is_call_built_in_function(value)) {
        return value;
    }
    call = (const afw_value_call_built_in_function_t *)value;
    if (!call->function ||
        !afw_utf8_equal_utf8_z(&call->function->functionId->internal,
            "wrap_literal_array") ||
        call->args.argc < 1 || !call->args.argv[1])
    {
        return value;
    }
    return call->args.argv[1];
}



/* Compile-time type check for const/let/assign (issue #28). */
static void
impl_compile_check_list_pattern(
    afw_compile_parser_t *parser,
    const afw_compile_list_destructure_t *ld,
    const afw_value_t *value)
{
    const afw_array_t *arr;
    const afw_compile_assignment_element_t *ae;
    const afw_value_t *elem;
    afw_size_t i;

    /*
     * Evaluates to array (includes wrap_literal_array). Then require cast-safe
     * evaluated array (after unwrap) to open entries.
     */
    if (!AFW_VALUE_EVALUATES_TO_DATA_TYPE(value, array, parser->xctx)) {
        return;
    }
    value = impl_unwrap_wrap_literal_array(value);
    if (!AFW_VALUE_IS_DATA_TYPE(value, array)) {
        return;
    }
    arr = ((const afw_value_array_t *)value)->internal;
    for (i = 0, ae = ld->assignment_element; ae; ae = ae->next, i++) {
        if (!ae->assignment_target || !ae->type ||
            afw_value_type_is_any(ae->type))
        {
            continue;
        }
        elem = afw_array_get_entry_value(arr, i, parser->p, parser->xctx);
        if (!elem) {
            continue;
        }
        afw_value_type_check_compile_assignable(ae->type, elem,
            "list pattern element", &parser->contextual, parser->xctx);
    }
}



/*
 * See through compile-time wrap_literal_object(...) (issue #17) for pattern
 * property type checks against the constant object argument.
 */
static const afw_value_t *
impl_unwrap_wrap_literal_object(const afw_value_t *value)
{
    const afw_value_call_built_in_function_t *call;

    if (!afw_value_is_call_built_in_function(value)) {
        return value;
    }
    call = (const afw_value_call_built_in_function_t *)value;
    if (!call->function ||
        !afw_utf8_equal_utf8_z(&call->function->functionId->internal,
            "wrap_literal_object") ||
        call->args.argc < 1 || !call->args.argv[1])
    {
        return value;
    }
    return call->args.argv[1];
}



static void
impl_compile_check_object_pattern(
    afw_compile_parser_t *parser,
    const afw_compile_object_destructure_t *od,
    const afw_value_t *value)
{
    const afw_object_t *obj;
    const afw_compile_assignment_property_t *ap;
    const afw_value_t *pv;
    const afw_value_type_t *type;
    const afw_utf8_t *name;

    /*
     * Evaluates to object (includes wrap_literal_object call). Then require a
     * cast-safe evaluated object (after unwrap) to walk properties.
     */
    if (!AFW_VALUE_EVALUATES_TO_DATA_TYPE(value, object, parser->xctx)) {
        return;
    }
    value = impl_unwrap_wrap_literal_object(value);
    if (!AFW_VALUE_IS_DATA_TYPE(value, object)) {
        return;
    }
    obj = ((const afw_value_object_t *)value)->internal;

    for (ap = od->assignment_property; ap; ap = ap->next) {
        type = NULL;
        name = NULL;
        if (ap->is_rename) {
            if (ap->property_name_expr) {
                /* Dynamic name: cannot check at compile. */
                continue;
            }
            name = ap->property_name;
            if (ap->assignment_element) {
                type = ap->assignment_element->type;
            }
        }
        else if (ap->symbol_reference && ap->symbol_reference->symbol) {
            name = ap->symbol_reference->symbol->name;
            type = &ap->symbol_reference->symbol->type;
        }
        if (!name || !type || afw_value_type_is_any(type)) {
            continue;
        }
        pv = afw_object_get_property(obj, name, parser->xctx);
        if (!pv) {
            continue;
        }
        afw_value_type_check_compile_assignable(type, pv,
            "object pattern property", &parser->contextual, parser->xctx);
    }
}



static void
impl_compile_check_assign_target(
    afw_compile_parser_t *parser,
    const afw_value_t *target,
    const afw_value_t *value)
{
    const afw_value_assignment_target_t *at;
    const afw_value_type_t *type;

    if (!AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED(
            &parser->contextual, parser->xctx))
    {
        return;
    }
    if (!afw_value_is_assignment_target(target)) {
        return;
    }
    at = (const afw_value_assignment_target_t *)target;

    switch (at->assignment_target->target_type) {
    case afw_compile_assignment_target_type_symbol_reference:
        type = &at->assignment_target->symbol_reference->symbol->type;
        afw_value_type_check_compile_assignable(type, value,
            "assignment", &parser->contextual, parser->xctx);
        break;

    case afw_compile_assignment_target_type_list_destructure:
        impl_compile_check_list_pattern(parser,
            at->assignment_target->list_destructure, value);
        break;

    case afw_compile_assignment_target_type_object_destructure:
        impl_compile_check_object_pattern(parser,
            at->assignment_target->object_destructure, value);
        break;

    case afw_compile_assignment_target_type_max_type:
    default:
        break;
    }
}


/*ebnf>>>
 *
 *#
 *# Must evaluate one of following:
 *#    VariableReference
 *#    ReferenceByIndex
 *#    ReferenceByName
 *
 * AssignmentExpression ::= Expression
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentExpression(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression,
    afw_boolean_t *just_expression_okay)
{
    const afw_value_t *result;
 
    /* Initialize was_expression to false. */
    if (was_expression) {
        *was_expression = false;
    }
    *just_expression_okay = false;

    if (afw_compile_token_is(open_bracket) ||
        afw_compile_token_is(open_brace) ||
        afw_compile_token_is(open_angle_bracket))
    {
        afw_compile_reuse_token();
        result = afw_compile_parse_AssignmentTarget(parser,
            afw_compile_assignment_type_assign_only);
    }
    else {
        afw_compile_reuse_token();
        result = afw_compile_parse_Evaluation(parser);
        if (!result && was_expression) {
            result = afw_compile_parse_Expression(parser);
        }
        if (!result) {
            AFW_COMPILE_THROW_ERROR_Z("Invalid assignment target");
        }
        *just_expression_okay = true;
    }

    return result;
}



/*ebnf>>>
 *
 * OptionalDefineTarget ::=
 *    ( ( 'let' | 'const' )? AssignmentTarget )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_OptionalDefineTarget(
    afw_compile_parser_t *parser,
    const afw_value_t **define_function,
    const afw_value_block_t **block)
{
    const afw_value_t *result;
    afw_size_t start_offset;
    afw_compile_internal_assignment_type_t assignment_type;

    afw_compile_save_cursor(start_offset);

    /* Determine assignment type. */
    afw_compile_get_token();
    if (afw_compile_token_is_name(afw_s_let)) {
        assignment_type = afw_compile_assignment_type_let;
        *define_function = &afw_function_definition_let.pub;
    }
    else if afw_compile_token_is_name(afw_s_const) {
        assignment_type = afw_compile_assignment_type_const;
        *define_function = &afw_function_definition_const.pub;
    }
    else {
        afw_compile_reuse_token();
        assignment_type = afw_compile_assignment_type_assign_only;
        *define_function = NULL;
    }

    /* If there is a let or const, block specified and not NULL, make one. */
    if (define_function && block && !*block) {
        *block = afw_compile_parse_link_new_value_block(
            parser, start_offset);
    }

    /* Parse AssignmentTarget and return result. */
    result = afw_compile_parse_AssignmentTarget(parser, assignment_type);
    return result;
}



/*ebnf>>>
 *
 * OptionalDefineAssignment ::= (
 *     ( ( 'let' | 'const' ) AssignmentTarget '=' Expression ) |
 *     Assignment
 * )
 *
 *<<<ebnf*/
/* Call afw_compile_parse_OptionalDefineTarget() first to get params. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_OptionalDefineAssignment(
    afw_compile_parser_t *parser,
    const afw_value_t *target,
    const afw_value_t *define_function)
{
    const afw_value_t *result;
    const afw_value_t **argv;

    if (!define_function) {
        result = afw_compile_parse_AssignmentOperation(parser,
            target, false, NULL);
    }

    else {

        /* Call appropriate function for const and let. */
        argv = afw_pool_malloc(parser->p,
            sizeof(afw_value_t *) * 4,
            parser->xctx);

        argv[0] = define_function;
        argv[1] = target;
        argv[2] = NULL;
        argv[3] = NULL;

        afw_compile_get_token();
        if (!afw_compile_token_is(equal)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting '='");
        }
        argv[2] = afw_compile_parse_Expression(parser);

        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(
                parser->token->token_source_offset),
            3, argv, true, parser->p, parser->xctx);
    }

    return result;
}



/*ebnf>>>
 *
 * AssignmentOperation ::=
 *    (
 *        ( '++' | '--' ) |
 *        ( 
 *            (
 *              '=' | '+=' | '-=' | '*=' |'/=' | '%=' |
 *              '**=' | '&&=' | '||=' | '??='
 *            )
 *            Expression
 *       )
 *    )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentOperation(
    afw_compile_parser_t *parser,
    const afw_value_t *target,
    afw_boolean_t just_expression_okay,
    afw_boolean_t *was_expression)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_t *function;
    afw_boolean_t expression_is_one;

    expression_is_one = false;
    afw_compile_get_token();
    switch (parser->token->type) {
    case afw_compile_token_type_equal:
        function = NULL;
        break;

    case afw_compile_token_type_plus_equal:
        function = &afw_function_definition_add.pub;
        break;

    case afw_compile_token_type_increment:
        function = &afw_function_definition_add.pub;
        expression_is_one = true;
        break;

    case afw_compile_token_type_minus_equal:
        function = &afw_function_definition_subtract.pub;
        break;

    case afw_compile_token_type_decrement:
        function = &afw_function_definition_subtract.pub;
        expression_is_one = true;
        break;

    case afw_compile_token_type_multiply_equal:
        function = &afw_function_definition_multiply.pub;
        break;

    case afw_compile_token_type_divide_equal:
        function = &afw_function_definition_divide.pub;
        break;

    case afw_compile_token_type_modulus_equal:
        function = &afw_function_definition_mod.pub;
        break;

    case afw_compile_token_type_exponentiation_equal:
        function = &afw_function_definition_pow.pub;
        break;

    case afw_compile_token_type_and_equal:
        function = &afw_function_definition_and.pub;
        break;

    case afw_compile_token_type_or_equal:
        function = &afw_function_definition_or.pub;
        break;

    case afw_compile_token_type_nullish_equal:
        function = &afw_function_definition_nullish_coalescing.pub;
        break;

    default:
        if (just_expression_okay) {
            afw_compile_reuse_token();
            if (was_expression) {
                *was_expression = true;
            }
            return target;
        }
        else {
            AFW_COMPILE_THROW_ERROR_Z("Invalid assignment operator");
        }
    }

    if (function) {
        if (expression_is_one) {
            result = afw_integer_v_one;
        }
        else {
            result = afw_compile_parse_Expression(parser);
        }
        argv = afw_pool_malloc(parser->p,
            sizeof(afw_value_t *) * 3,
            parser->xctx);
        argv[0] = function;
        argv[1] = target;
        argv[2] = result;
        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(
                parser->token->token_source_offset),
                2, argv, true, parser->p, parser->xctx);
    }
    else {
        result = afw_compile_parse_Expression(parser);
    }

    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 3,
        parser->xctx);
    argv[0] = &afw_function_definition_assign.pub;
    argv[1] = target;
    argv[2] = result;
    impl_compile_check_assign_target(parser, target, result);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            2, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * Assignment ::= AssignmentExpression AssignmentOperation
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Assignment(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression)
{
    const afw_value_t *result;
    const afw_value_t *target;
    afw_boolean_t just_expression_okay;

    target = afw_compile_parse_AssignmentExpression(parser,
        was_expression, &just_expression_okay);

    result = afw_compile_parse_AssignmentOperation(parser,
        target, just_expression_okay, was_expression);

    return result;
}



/*ebnf>>>
 *
 * AssignmentStatement ::=
 *    (
 *        ( '(' AssignmentObjectDestructureTarget '=' Expression ')' ) |
 *        Assignment
 *    )
 *    ';'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentStatement(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression)
{
    const afw_value_t *result;
    afw_boolean_t is_object_destructuring;

    /*
     * An object destructure for an assignment statement requires parentheses
     * around the everything up to statement terminator.
     */
    is_object_destructuring = false;
    if (afw_compile_token_is(open_parenthesis)) {
        afw_compile_get_token();
        if afw_compile_token_is(open_brace) {
            is_object_destructuring = true;
        }
        else {
            afw_compile_reuse_token();
        }
    }

    result = afw_compile_parse_Assignment(parser,
        is_object_destructuring ? NULL : was_expression);

    /* Check for close parenthesis if object destructuring. */
    if (is_object_destructuring) {
        afw_compile_get_token();
        if (!afw_compile_token_is(close_parenthesis)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
        }
    }

    if (!was_expression || !*was_expression) {
        AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    }

    return result;
}



/*ebnf>>>
 *
 * BreakStatement ::= 'break' ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_BreakStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    if (!parser->break_allowed) {
        AFW_COMPILE_THROW_ERROR_Z("Misplaced break");
    }

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            0, &impl_function_definition_break, true,
            parser->p, parser->xctx);

    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;

    return result;
}



/*ebnf>>>
 *
 * ConstStatement ::= 'const' AssignmentTarget '=' Expression ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ConstStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;

    result = NULL;
    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 4,
        parser->xctx);

    argv[0] = &afw_function_definition_const.pub;
    argv[1] = afw_compile_parse_AssignmentTarget(parser,
        afw_compile_assignment_type_const);
    argv[2] = NULL;
    argv[3] = NULL;

    afw_compile_get_token();
    if (!afw_compile_token_is(equal)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '='");
    }
    argv[2] = afw_compile_parse_Expression(parser);
    impl_compile_check_assign_target(parser, argv[1], argv[2]);
    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            3, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * InterfaceName ::= Identifier
 *
 * InterfaceStatement ::=
 *     'interface' InterfaceName
 *     ( 'extends' Type ( ',' Type )* )?
 *     ObjectTypeLiteral
 *     ';'
 *
 *# Object body is an ObjectTypeLiteral (see Type productions in expression).
 *# Script-local only; not adaptive object types (issue #28).
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_InterfaceStatement(afw_compile_parser_t *parser)
{
    const afw_utf8_t *name;
    const afw_value_type_t *body;
    const afw_value_type_t *base;
    afw_value_type_t *type;
    apr_array_header_t *extends;
    const afw_value_type_t **list;
    afw_size_t i;
    afw_size_t brace_offset;

    /* 'interface' already consumed as statement keyword. */
    afw_compile_get_token();
    if (!afw_compile_token_is_unqualified_identifier()) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting interface name");
    }
    name = parser->token->identifier_name;

    extends = NULL;
    afw_compile_get_token();
    if (afw_compile_token_is_name_z("extends")) {
        extends = apr_array_make(parser->apr_p, 2,
            sizeof(const afw_value_type_t *));
        for (;;) {
            /* Each base: full Type starting at next token (name). */
            base = afw_compile_parse_Type(parser);
            APR_ARRAY_PUSH(extends, const afw_value_type_t *) = base;
            afw_compile_get_token();
            if (!afw_compile_token_is(comma)) {
                break;
            }
        }
    }

    if (!afw_compile_token_is(open_brace)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '{' in interface declaration");
    }

    /* parse_Type begins with get_token; put '{' back so primary sees it. */
    brace_offset = parser->token->token_source_offset;
    afw_compile_restore_cursor(brace_offset);
    body = afw_compile_parse_Type(parser);

    if (body->kind != afw_value_type_kind_object) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Interface body must be an object type literal");
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

    afw_compile_script_type_register(parser, name, type);

    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    return NULL;
}



/*ebnf>>>
 *
 * TypeVariableName ::= Identifier
 *
 * TypeStatement ::= 'type' TypeVariableName '=' Type ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_TypeStatement(afw_compile_parser_t *parser)
{
    const afw_utf8_t *name;
    const afw_value_type_t *type;

    afw_compile_get_token();
    if (!afw_compile_token_is_unqualified_identifier()) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting type name");
    }
    name = parser->token->identifier_name;

    afw_compile_get_token();
    if (!afw_compile_token_is(equal)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '=' in type alias");
    }

    type = afw_compile_parse_Type(parser);
    afw_compile_script_type_register(parser, name, type);

    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    return NULL;
}



/*ebnf>>>
 *
 * ContinueStatement ::= 'continue' ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ContinueStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    if (!parser->continue_allowed) {
        AFW_COMPILE_THROW_ERROR_Z("Misplaced 'continue'");
    }

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
        0, &impl_function_definition_continue, true,
        parser->p, parser->xctx);

    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;

    return result;
}


/*ebnf>>>
 *
 * DoWhileStatement ::= 'do' Statement 'while' '(' Expression ')' ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_DoWhileStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;
    afw_boolean_t break_allowed;
    afw_boolean_t continue_allowed;

    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = &afw_function_definition_do_while.pub;

    break_allowed = parser->break_allowed;
    continue_allowed = parser->continue_allowed;
    parser->break_allowed = true;
    parser->continue_allowed = true;
    argv[2] = afw_compile_parse_Statement(parser, NULL);
    parser->break_allowed = break_allowed;
    parser->continue_allowed = continue_allowed;

    afw_compile_get_token();
    if (!afw_compile_token_is_name(afw_s_while)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting 'while'");
    }

    /* ( expression ) */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }
    argv[1] = afw_compile_parse_Expression(parser);
        afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
    }

    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, true, parser->p, parser->xctx);
        
    return result;
}



/*ebnf>>>
 *
 * ForStatement ::= 'for'
 *   '(' (
 *       (
 *           ( OptionalDefineAssignment ( ',' OptionalDefineAssignment )* )?
 *           ';' Expression?
 *           ';' ( Assignment ( ',' Assignment )* )?
 *       ) |
 *       (
 *           OptionalDefineTarget 'of' Expression
 *       )
 *   ) ')' Statement
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ForStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_t *value;
    const afw_value_t *target;
    const afw_array_t *list;
    const afw_value_t *define_function;
    const afw_value_block_t *block;
    afw_size_t start_offset;
    afw_boolean_t break_allowed;
    afw_boolean_t continue_allowed;
    afw_boolean_t is_for_of;

    block = NULL;
    is_for_of = false;
    afw_compile_save_cursor(start_offset);

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }
    list = NULL;
    afw_compile_get_token();
    if (!afw_compile_token_is(semicolon)) {
        afw_compile_reuse_token();
        for (;;) {
            /*
             * Get the target using OptionalDefineTarget(). This is needed for
             * the "OptionalDefineTarget 'of' Expression" part of production. If
             * it turns out the OptionalDefineAssignment matches instead, pass
             * this target to OptionalDefineAssignment() since it has already
             * been parsed and the first part is the same.
             */
            target = afw_compile_parse_OptionalDefineTarget(parser,
                &define_function, &block);

            afw_compile_get_token();
            if (afw_compile_token_is_name(afw_s_of)) {
                if (list) {
                    AFW_COMPILE_THROW_ERROR_Z("Not expecting 'of'");
                }
                is_for_of = true;
                break;
            }
            afw_compile_reuse_token();

            value = afw_compile_parse_OptionalDefineAssignment(parser,
                target, define_function);

            if (!list) {
                list = afw_array_create_generic(parser->p, parser->xctx);
            }
            afw_array_push_value(list, value, parser->xctx);
            afw_compile_get_token();
            if (afw_compile_token_is(semicolon)) {
                break;
            }
            if (!afw_compile_token_is(comma)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ',' or ';'");
            }
        }
    }

    if (is_for_of) {
        argv = afw_pool_malloc(parser->p,
            sizeof(afw_value_t *) * 4, parser->xctx);
        argv[0] = &afw_function_definition_for_of.pub;
        argv[1] = target;   
        argv[2] = afw_compile_parse_Expression(parser);
        
        afw_compile_get_token();
        if (!afw_compile_token_is(close_parenthesis)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
        }

        break_allowed = parser->break_allowed;
        continue_allowed = parser->continue_allowed;
        parser->break_allowed = true;
        parser->continue_allowed = true;
        argv[3] = afw_compile_parse_Statement(parser, NULL);
        parser->break_allowed = break_allowed;
        parser->continue_allowed = continue_allowed;

        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            3, argv, true, parser->p, parser->xctx);
    }

    else {
        argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 5, parser->xctx);
        argv[0] = &afw_function_definition_for.pub;
        argv[1] = NULL;
        if (list) {
            argv[1] = afw_value_create_unmanaged_array(
                list, parser->p, parser->xctx);
        }

        /* Expression? ';' */
        argv[2] = NULL;
        afw_compile_get_token();
        if (!afw_compile_token_is(semicolon)) {
            afw_compile_reuse_token();
            argv[2] = afw_compile_parse_Expression(parser);
            afw_compile_get_token();
            if (!afw_compile_token_is(semicolon)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ';'");
            }
        }

        /* ( Assignment ( ',' Assignment )* )? ')' */
        afw_compile_get_token();
        list = NULL;
        if (!afw_compile_token_is(close_parenthesis)) {
            for (;;) {
                value = afw_compile_parse_Assignment(parser, NULL);
                if (!list) {
                    list = afw_array_create_generic(parser->p, parser->xctx);
                }
                afw_array_push_value(list, value, parser->xctx);
                afw_compile_get_token();
                if (afw_compile_token_is(close_parenthesis)) {
                    break;
                }
                if (!afw_compile_token_is(comma)) {
                    AFW_COMPILE_THROW_ERROR_Z("Expecting ',' or ')'");
                }
                afw_compile_get_token();
            }
        }
        argv[3] = NULL;
        if (list) {
            argv[3] = afw_value_create_unmanaged_array(
                list, parser->p, parser->xctx);
        }

        break_allowed = parser->break_allowed;
        continue_allowed = parser->continue_allowed;
        parser->break_allowed = true;
        parser->continue_allowed = true;
        argv[4] = afw_compile_parse_Statement(parser, NULL);
        parser->break_allowed = break_allowed;
        parser->continue_allowed = continue_allowed;

        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            4, argv, true, parser->p, parser->xctx);

    }

    /* If there is a block for let/const finalize it. */
    if (block) {
        argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *), parser->xctx);
        argv[0] = result;
        afw_value_block_finalize(block, 1, argv, parser->xctx);
        result = &block->pub;
        afw_compile_parse_pop_value_block(parser);
    }

    return result;
}



/*ebnf>>>
 *# Named function
 *#
 *# FunctionName must be present for this production to match.
 *
 * FunctionStatement ::= 
 *      'function' ( FunctionName - ReservedWords ) FunctionSignatureAndBody ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_FunctionStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_string_t *function_name_value;
    afw_value_block_symbol_t *symbol;
    const afw_value_type_t *return_type;
    afw_size_t start_offset;

    afw_compile_save_cursor(start_offset);
    afw_compile_get_token();
    if (!afw_compile_token_is_unqualified_identifier()) {
        afw_compile_reuse_token();
        return NULL;
    }
    afw_compile_reuse_token();
    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 4, parser->xctx);
    argv[0] = &afw_function_definition_const.pub;
    argv[2] = afw_compile_parse_FunctionSignatureAndBody(
        parser, &function_name_value, &return_type);
    argv[1] = NULL;
    if (function_name_value) {
        symbol = afw_compile_parse_add_symbol_entry(parser,
            &function_name_value->internal);
        symbol->symbol_type = afw_value_block_symbol_type_function;
        symbol->initial_value = argv[2];
        /*
         * Do not store the return type as symbol->type: the binding holds a
         * function value. Return types live on the script_function_definition
         * (issue #28 typeCheck would reject assigning a function to a
         * return-type slot).
         */
        (void)return_type;
        argv[1] = afw_value_symbol_reference_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            symbol, parser->p, parser->xctx);
    }
    argv[3] = NULL;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        3, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * IfStatement ::= 'if' '(' Expression ')' Statement ( 'else' Statement )?
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_IfStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t *condition;
    const afw_value_t *then;
    const afw_value_t *otherwise;
    const afw_value_t **argv;
    afw_size_t argc;
    afw_size_t start_offset;

    afw_compile_save_cursor(start_offset);

    /* ( expression ) */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }
    condition = afw_compile_parse_Expression(parser);
        afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
    }

    /* statement and optional else statement. */
    then = afw_compile_parse_Statement(parser, NULL);
    otherwise = NULL;

    afw_compile_get_token();
    if (afw_compile_token_is_unqualified_identifier()) {
        if (afw_utf8_equal(parser->token->identifier_name, afw_s_else))
        {
            otherwise = afw_compile_parse_Statement(parser, NULL);
        }
        else {
            afw_compile_reuse_token();
        }
    }
    else {
        afw_compile_reuse_token();
    }

    /* Create call value for if and return it. */
    argc = otherwise ? 3 : 2;
    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * (argc + 1),
        parser->xctx);
    argv[0] = &afw_function_definition_if.pub;
    argv[1] = condition;
    argv[2] = then;
    if (otherwise) {
        argv[3] = otherwise;
    }
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc, argv, true, parser->p, parser->xctx);
    return result;
}



/*ebnf>>>
 *
 * LetStatement ::= 'let' AssignmentTarget ( '=' Expression )? ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_LetStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
 
    result = NULL;
    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 4,
        parser->xctx);

    argv[0] = &afw_function_definition_let.pub;
    argv[1] = afw_compile_parse_AssignmentTarget(parser,
        afw_compile_assignment_type_let);
    argv[2] = NULL;
    argv[3] = NULL;

    afw_compile_get_token();
    if (!afw_compile_token_is(semicolon)) {
        if (!afw_compile_token_is(equal)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting '='");
        }
        argv[2] = afw_compile_parse_Expression(parser);
        impl_compile_check_assign_target(parser, argv[1], argv[2]);
        AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    }

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            3, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 *# Expression is required if inside a function that has a non-void return.
 * ReturnStatement ::= 'return' Expression? ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ReturnStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;

    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 2, parser->xctx);
    argv[0] = &afw_function_definition_return.pub;
    argv[1] = afw_value_undefined;

    afw_compile_get_token();
    if (!afw_compile_token_is(semicolon)) {
        afw_compile_reuse_token();
        argv[1] = afw_compile_parse_Expression(parser);
        AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    }

    /* Compile-time return type check when inside a typed function (issue #28). */
    if (parser->current_function_returns &&
        AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED(
            &parser->contextual, parser->xctx) &&
        argv[1] && !afw_value_is_undefined(argv[1]))
    {
        afw_value_type_check_compile_assignable(
            parser->current_function_returns, argv[1],
            "return", &parser->contextual, parser->xctx);
    }

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        1, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * CaseClause ::= 'case' Expression ':' StatementList?
 * 
 * DefaultClause ::= 'default' ':' StatementList?
 *
 * SwitchStatement ::= 'switch' ParenthesizedExpression
 *     ( 'using' EntryFunctionLambdaOrVariableReference )?
 *     '{'
 *         CaseClause*
 *         DefaultClause?
 *         CaseClause*
 *     '}'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_SwitchStatement(afw_compile_parser_t *parser)
{
    
    const afw_value_t *result;
    const afw_value_t *predicate;
    const afw_value_t *case_expression;
    const afw_value_t *statement_list;
    const afw_value_block_t *block;
    afw_compile_args_t *args;
    afw_size_t argc;
    const afw_value_t **argv;
    afw_size_t start_offset;
    afw_boolean_t break_allowed;
    afw_boolean_t default_encountered;

    afw_compile_save_cursor(start_offset);

    /* All of switch is in a new block. */
    block = afw_compile_parse_link_new_value_block(parser, start_offset);
 
    /* Build variable length args. */
    args = afw_compile_args_create(parser);
    afw_compile_args_add_value(args, &afw_function_definition_switch.pub);

    /* ParenthesizedExpression */
    result = afw_compile_parse_ParenthesizedExpression(parser);

    /* Optional 'using' EntryFunctionLambdaOrVariableReference */
    predicate = &afw_function_definition_eqx.pub;
    afw_compile_get_token();
    if (afw_compile_token_is_name(afw_s_using)) {
        predicate = afw_compile_parse_EntryFunctionLambdaOrVariableReference(
            parser);
        afw_compile_get_token();
    }
    if (!afw_compile_token_is(open_brace)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '{'");
    }

    /* Add predicate and expression to args. */
    afw_compile_args_add_value(args, predicate);
    afw_compile_args_add_value(args, result);

    /* Loop processing clauses. */
    break_allowed = parser->break_allowed;
    parser->break_allowed = true;
    default_encountered = false;
    for (;;) {
        afw_compile_get_token();
        if (afw_compile_token_is(close_brace)) {
            break;
        }
        if (afw_compile_token_is_name(afw_s_case)) {
            case_expression = afw_compile_parse_Expression(parser);
        }
        else if (afw_compile_token_is_name(afw_s_default)) {
            if (default_encountered) {
                AFW_COMPILE_THROW_ERROR_Z("Multiple 'default' clauses");
            }
            default_encountered = true;
            case_expression = afw_value_unique_default_case_value;
        }
        else {
            AFW_COMPILE_THROW_ERROR_Z("Expecting 'case' or 'default'");
        }
        afw_compile_get_token();
        if (!afw_compile_token_is(colon)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting ':'");
        }
        statement_list = afw_compile_parse_StatementList(parser,
            NULL, false, true, false, false);
        afw_compile_args_add_value(args, case_expression);
        afw_compile_args_add_value(args, statement_list);
    }
    parser->break_allowed = break_allowed;

    /* Create switch support function call. */
    afw_compile_args_finalize(args, &argc, &argv);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc - 1, argv, true, parser->p, parser->xctx);

    /* Finalize the block. */
    argv = afw_pool_calloc(parser->p, sizeof(afw_value_t *), parser->xctx);
    *argv = result;
    afw_value_block_finalize(block, 1, argv, parser->xctx);
    result = &block->pub;

    afw_compile_parse_pop_value_block(parser);
  
    return result;
}



/*ebnf>>>
 *
 *# If there is no expression, the exception is rethrow and can only be
 *# contained in a catch block. If there is an expression, it is the message
 *# for the exception to be thrown. If there is a second expression, it
 *# can be any value and will be the value for the "data" property of the
 *# error object.
 *#
 * ThrowStatement ::=  'throw' ( Expression  Expression? )?
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ThrowStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_size_t argc;
    const afw_value_t **argv;
    const afw_compile_value_contextual_t *contextual;

    contextual = afw_compile_create_contextual_to_cursor(parser->cursor);

    /* rethrow if end of statement. */
    afw_compile_get_token();
    if (afw_compile_token_is(semicolon)) {
        if (!parser->rethrow_allowed) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Can only rethrow ('throw;') inside a catch block");
        }
        result = afw_value_call_built_in_function_create(contextual,
            0, &impl_function_definition_rethrow, true,
            parser->p, parser->xctx);
    }

    /* throw. */
    else {
        afw_compile_reuse_token();
        argv = afw_pool_calloc(parser->p, sizeof(afw_value_t *) * 3,
            parser->xctx);
        argv[0] = &afw_function_definition_throw.pub;
        argc = 1;

        /* Message */
        argv[1] = afw_compile_parse_Expression(parser);

        /* Optional data */
        afw_compile_get_token();
        if (!afw_compile_token_is(semicolon)) {
            afw_compile_reuse_token();
            argv[2] = afw_compile_parse_Expression(parser);
            argc = 2;
            AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
        }
    
        /* Create the throw function call. */
        result = afw_value_call_built_in_function_create(
            contextual, argc, argv, true,
            parser->p, parser->xctx);
    }

    return result;
}
 

typedef struct {
    afw_compile_parse_StatementList_cb_t public;
    const afw_utf8_t *error_variable_name;
    const afw_value_t **symbol_reference;
    const afw_compile_value_contextual_t *contextual;
} impl_parse_TryStatement_StatementList_cb_t;

/* Identifier catch only: plant error name after StatementList opens the block. */
static void
impl_parse_TryStatement_StatementList_cb (
    afw_compile_parse_StatementList_cb_t *cb,
    afw_compile_parser_t *parser,
    const afw_value_block_t *block,
    afw_compile_args_t *statements)
{
    impl_parse_TryStatement_StatementList_cb_t *self =
        (impl_parse_TryStatement_StatementList_cb_t *)cb;
 
    *self->symbol_reference = (const afw_value_t *)
        afw_compile_parse_variable_reference_create(parser,
            self->contextual,
            afw_compile_assignment_type_let,
            self->error_variable_name, NULL);
}

/*ebnf>>>
 *
 *#
 *# Catch binding is Identifier or the same list/object Pattern as let/const
 *# (issue #140). Runtime assigns the error object into the binding target.
 *#
 * Catch ::= 'catch' ( '(' ( Identifier | AssignmentTarget ) ')' )? Statement
 * 
 * Finally ::= 'finally' Statement
 *
 * TryStatement ::= 'try' Statement ( Catch | Finally | ( Catch Finally ) )
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_TryStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    impl_parse_TryStatement_StatementList_cb_t cb;
    afw_size_t argc;
    afw_size_t start_offset;
    afw_boolean_t rethrow_allowed;
    afw_boolean_t pattern_binding;

    rethrow_allowed = parser->rethrow_allowed;
    parser->rethrow_allowed = false;
    cb.public.func = NULL;
    pattern_binding = false;
    afw_compile_save_cursor(start_offset);

    argv = afw_pool_calloc(parser->p, sizeof(afw_value_t *) * 5, parser->xctx);
    argv[0] = &afw_function_definition_try.pub;

    /* Body */
    argv[1] = afw_compile_parse_Statement(parser, NULL);
    argc = 1;

    /* Catch */
    afw_compile_get_token();
    if (afw_compile_token_is_name(afw_s_catch)) {
        parser->rethrow_allowed = true;
        argc = 3;
        afw_compile_get_token();
        if (!afw_compile_token_is(open_parenthesis)) {
            afw_compile_reuse_token();
        }
        else {
            argc = 4;
            afw_compile_get_token();
            /*
             * Pattern catch: open catch block first so Pattern leaves land
             * on the same block as body statements. Identifier: keep the
             * historical callback that plants the name when StatementList
             * opens the block.
             */
            if (afw_compile_token_is(open_bracket) ||
                afw_compile_token_is(open_brace))
            {
                /*
                 * Pattern catch (issue #140): open the catch block first so
                 * Pattern leaves are symbols on the same block as the body.
                 * StatementList(..., use_existing_current_block=true) fills
                 * that block; no StatementList callback needed.
                 */
                pattern_binding = true;
                afw_compile_reuse_token();
                (void)afw_compile_parse_link_new_value_block(parser,
                    start_offset);
                argv[4] = afw_compile_parse_AssignmentTarget(parser,
                    afw_compile_assignment_type_let);
            }
            else if (afw_compile_token_is_unqualified_identifier()) {
                /*
                 * Identifier catch: StatementList opens the block, then the
                 * callback plants the error variable on that block (historical
                 * path; decompile of try expects argv[4] as symbol_reference).
                 */
                cb.public.func = impl_parse_TryStatement_StatementList_cb;
                cb.error_variable_name = parser->token->identifier_name;
                cb.contextual = afw_compile_create_contextual_to_cursor(
                    start_offset);
                cb.symbol_reference = &argv[4];
            }
            else {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting identifier or Pattern in catch");
            }
            afw_compile_get_token();
            if (!afw_compile_token_is(close_parenthesis)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
            }
        }

        /* catch always has to have a block, not single statement. */
        afw_compile_get_token();
        if (!afw_compile_token_is(open_brace)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting '{'");
        }
        /*
         * Pattern binding already opened the catch block (symbols live on
         * that block). Identifier binding uses the StatementList callback to
         * plant the name after a new block is opened. use_existing_current_block
         * is only for the Pattern case so we do not nest a second block.
         */
        argv[3] = afw_compile_parse_StatementList(parser,
            (cb.public.func) ? &cb.public : NULL,
            true, false, false,
            /* use_existing_current_block */ pattern_binding);
        parser->rethrow_allowed = false;
    }
    else {
        afw_compile_reuse_token();
    }

    /* Finally */
    afw_compile_get_token();
    if (afw_compile_token_is_name(afw_s_finally)) {
        if (argc == 1) {
            argc = 2;
        }
        /* finally always has to have a block, not single statement. */
        afw_compile_get_token();
        if (!afw_compile_token_is(open_brace)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting '{'");
        }
        argv[2] = afw_compile_parse_StatementList(parser,
            NULL, true, false, false, false);
    }
    else {
        afw_compile_reuse_token();
    }

    /* If neither catch or finally, it's an error. */
    if (argc == 1) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting 'catch' or 'finally'");
    }

    /* Create the try function call. */
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc, argv, true, parser->p, parser->xctx);

    parser->rethrow_allowed = rethrow_allowed;
    return result;
}



/*ebnf>>>
 *
 * WhileStatement ::= 'while' '(' Expression ')' Statement
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_WhileStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;
    afw_boolean_t break_allowed;
    afw_boolean_t continue_allowed;

    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = &afw_function_definition_while.pub;

    /* ( expression ) */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }
    argv[1] = afw_compile_parse_Expression(parser);
        afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
    }  

    break_allowed = parser->break_allowed;
    continue_allowed = parser->continue_allowed;
    parser->break_allowed = true;
    parser->continue_allowed = true;
    argv[2] = afw_compile_parse_Statement(parser, NULL);
    parser->break_allowed = break_allowed;
    parser->continue_allowed = continue_allowed;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 *# This is any Evaluation that compiles to a call adaptive value.
 * EvaluationThatCompilesToCallValue ::= Evaluation
 *
 * CallStatement ::= EvaluationThatCompilesToCallValue
 *
 *# BreakStatement and ContinueStatement can only be in a loop.
 *
 * Block ::= '{' StatementList '}'
 *
 * Statement ::=
 *    ';' |
 *    Block |
 *    AssignmentStatement |
 *    BreakStatement |
 *    CallStatement |
 *    ConstStatement |
 *    ContinueStatement |
 *    DoWhileStatement |
 *    ForStatement |
 *    FunctionStatement |
 *    IfStatement |
 *    InterfaceStatement |
 *    LetStatement |
 *    PragmaStatement |
 *    ReturnStatement |
 *    SwitchStatement |
 *    ThrowStatement |
 *    TryStatement |
 *    TypeStatement |
 *    WhileStatement
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Statement(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression)
{
    const afw_value_t *result;
    afw_boolean_t was_assignment_expression;

    /* Initialize was_expression to false. */
    if (was_expression) {
        *was_expression = false;
    }
 
    /* Get next token. */
    afw_compile_get_token();

    /* If next token is '{', parse Block. */
    if (afw_compile_token_is(open_brace)) {
        result = afw_compile_parse_StatementList(parser,
            NULL, true, false, false, false);
        return result;
    }

    /* If pound_identifier, parse pragma or compiler-internal # form. */
    if (afw_compile_token_is(pound_identifier)) {
        return afw_compile_parse_PragmaStatement(parser);
    }

    /* If not assignment, process statement. */
    result = NULL;
    if (afw_compile_token_is(identifier) &&
        !parser->token->identifier_qualifier)
    {
        if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_let))
        {
            result = impl_parse_LetStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_const))
        {
            result = impl_parse_ConstStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_break))
        {
            result = impl_parse_BreakStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_continue))
        {
            result = impl_parse_ContinueStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_do))
        {
            result = impl_parse_DoWhileStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_for))
        {
            result = impl_parse_ForStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_if))
        {
            result = impl_parse_IfStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_return))
        {
            result = impl_parse_ReturnStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_switch))
        {
            result = impl_parse_SwitchStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_throw))
        {
            result = impl_parse_ThrowStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_try))
        {
            result = impl_parse_TryStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_while))
        {
            result = impl_parse_WhileStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_function))
        {
            result = impl_parse_FunctionStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_interface))
        {
            result = impl_parse_InterfaceStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            afw_s_type))
        {
            result = impl_parse_TypeStatement(parser);
        }
    }

    /*
     * If there is not a result already and this is not an empty statement, see
     * if this is a call or assignment statement by calling
     * afw_compile_parse_AssignmentStatement() with a was_expression parameter
     * to get a value. The result is the first applicable of the following
     * based on this value:
     *
     * 1) If the value is not an expression, the result is an assignment
     *   statement.
     *
     * 2) If the value is a call value followed by a semicolon, the result is a
     *   call statement.
     *
     * 3) If the was_expression parameter is not NULL on the call to
     *   afw_compile_parse_Statement(), it's value is set to true and the
     *   expression is returned.
     *
     * 4) An invalid statement error is thrown.
     *
     */
    if (!result) {
        if (afw_compile_token_is(semicolon)) {
            return NULL;
        }
        was_assignment_expression = false;
        result = afw_compile_parse_AssignmentStatement(parser,
            &was_assignment_expression);
        if (was_assignment_expression)
        {
            if (afw_value_is_any_call(result)) {
                afw_compile_get_token();
                if (afw_compile_token_is(semicolon)) {
                    was_assignment_expression = false;
                }
                else {
                    afw_compile_reuse_token();
                }
            }
        }
        if (was_assignment_expression) {
            if (was_expression) {
                *was_expression = was_assignment_expression;
            }
            else {
                result = NULL;
            }
        }
    }

    /* If no result yet, throw error. */
    if (!result) {
        AFW_COMPILE_THROW_ERROR_Z("Invalid statement");
    }

    return result;
}


/*ebnf>>>
 *
 * StatementList ::= Statement*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_StatementList(
    afw_compile_parser_t *parser,
    afw_compile_parse_StatementList_cb_t *cb,
    afw_boolean_t end_is_close_brace,
    afw_boolean_t end_is_close_brace_case_or_default,   
    afw_boolean_t can_be_single_return_expression,
    afw_boolean_t use_existing_current_block)
{
    const afw_value_t *result;
    const afw_value_t *statement;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    const afw_value_block_t *block;
    const afw_array_t *array;
    afw_size_t argc;
    afw_size_t start_offset;
    afw_boolean_t *was_expression;
    afw_boolean_t was_expression_value;
    afw_boolean_t building_list_not_block;

    args = afw_compile_args_create(parser);
    was_expression_value = false;
    was_expression = (can_be_single_return_expression)
        ? &was_expression_value
        : NULL;

    building_list_not_block = end_is_close_brace_case_or_default;
    block = NULL;

    /* Save starting cursor. */
    afw_compile_save_cursor(start_offset);

    /*
     * Open a new value block, or continue the current one (catch Pattern:
     * symbols already introduced on current_block before the body).
     */
    if (!building_list_not_block) {
        if (use_existing_current_block) {
            block = parser->compiled_value->current_block;
            if (!block) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Internal error: StatementList use_existing_current_block "
                    "with no current block");
            }
        }
        else {
            block = afw_compile_parse_link_new_value_block(parser,
                start_offset);
        }
    }
    else if (use_existing_current_block) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Internal error: use_existing_current_block with case-list mode");
    }

    /* If cb passed, call it now that args and block are set. */
    if (cb) {
        (cb->func)(cb, parser, block, args);
    }

    /* Process statements. */
    for (;;) {

        afw_compile_get_token();
        if (end_is_close_brace_case_or_default) {
            if (afw_compile_token_is(close_brace) ||
                afw_compile_token_is_name(afw_s_case) ||
                afw_compile_token_is_name(afw_s_default))
            {
                afw_compile_reuse_token();
                break;
            }
            else if afw_compile_token_is(end) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting '}', 'case', or 'default'");
            }
        }
        else if (end_is_close_brace) {
            if (afw_compile_token_is(close_bracket) ||
                afw_compile_token_is(close_brace))
            {
                break;
            }
            else if afw_compile_token_is(end) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting '}'");
            }
        }
        else if (afw_compile_token_is(end)) {
            break;
        }
        afw_compile_reuse_token();

        if (was_expression_value) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expression can not be followed by Statement");
        }

        statement = afw_compile_parse_Statement(parser, was_expression);

        /** @fixme I believe these statements can go away. Return sets flow. */
        // if (was_expression_value) {
        //     argv = afw_pool_malloc(parser->p,
        //         sizeof(afw_value_t *) * 2, parser->xctx);
        //     argv[0] = &afw_function_definition_return.pub;
        //     argv[1] = statement;
        //     statement = afw_value_call_built_in_function_create(
        //         afw_compile_create_contextual_to_cursor(start_offset),
        //         1, argv, true, parser->p, parser->xctx);
        // }
        was_expression = NULL;

        if (statement) {
            afw_compile_args_add_value(args, statement);
        }
    }

    /* Finalize args. */
    afw_compile_args_finalize(args, &argc, &argv);

    /* If building statement list, create an array of statements. */
    if (building_list_not_block) {
        array = afw_array_const_create_array_of_values(
            argv, argc, parser->p, parser->xctx);
        result = afw_value_create_unmanaged_array(
            array, parser->p, parser->xctx);
    }

    /*
     * If building a block: when this StatementList opened an empty block
     * (no symbols) whose only statement is itself a block — e.g. recompiling
     * decompile text "#block(...)" as a script — promote the inner block
     * instead of wrapping (#block(#block(...))). Script always starts a
     * top-level block; #block then nested inside it.
     *
     * After promotion, renumber depths for the whole subtree. Nested blocks
     * (and script_function param blocks under them) keep parent links but
     * their old depths would be off by one, breaking scope creation and
     * script_function enclosing-scope resolution.
     */
    else if (
        argc == 1 &&
        argv[0] &&
        afw_value_is_block(argv[0]) &&
        block->symbol_count == 0 &&
        parser->compiled_value->top_block == block)
    {
        afw_value_block_t *inner = (afw_value_block_t *)argv[0];
        afw_value_block_t *b;
        afw_value_block_t *child;
        afw_value_block_t *stack[64];
        afw_size_t sp;

        parser->compiled_value->top_block = inner;
        inner->parent_block = NULL;
        inner->next_sibling_block = NULL;
        /* Outer block is abandoned (never finalized with statements). */
        parser->compiled_value->current_block = NULL;

        /* Depth-first renumber: top=0, children = parent+1. */
        inner->depth = 0;
        stack[0] = inner;
        sp = 1;
        while (sp > 0) {
            b = stack[--sp];
            for (child = (afw_value_block_t *)b->first_child_block;
                child;
                child = (afw_value_block_t *)child->next_sibling_block)
            {
                child->depth = b->depth + 1;
                if (sp < 64) {
                    stack[sp++] = child;
                }
                else {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Block nesting too deep to renumber after #block "
                        "unwrap");
                }
            }
        }

        result = argv[0];
    }

    /* If building block, finalize and set result. */
    else {
        afw_value_block_finalize(block, argc, argv, parser->xctx);
        result = &block->pub;
        afw_compile_parse_pop_value_block(parser);
    }

    /* Return block or list. */
    return result;
}


/*ebnf>>>
 *
 *#
 *# The value returned from a script if the Expression specified on an
 *# evaluated ReturnStatement, the single Expression specified, or null
 *# if none of the above.
 *#
 * Script ::= ScriptShebang? ( Statement* | Expression )
 *
 * ScriptShebang ::= '#!' UnicodeNonControl* 'afw' UnicodeNonControl* '\n'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Script(
    afw_compile_parser_t *parser,
    afw_boolean_t end_is_close_brace)
{
    const afw_value_t *result;
    afw_utf8_t line;

    /*
     * Shebang line must contain afw.  If it also contains test_script, parse
     * using TestScript production.
     */
    if (afw_compile_next_raw_starts_with_z("#!")) {
        afw_compile_get_raw_line(&line);
        if (!afw_utf8_contains(&line, afw_s_afw) &&
            !afw_utf8_contains(&line, afw_s_maluba) &&     /* Easter egg */
            !afw_utf8_contains(&line, afw_s_JeremyScript)) /* Easter egg */
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Shebang line must contain 'afw' to be recognized as an "
                "adaptive script");
        }
        if (afw_utf8_contains(&line, afw_s_a_dash_s_test_script) ||
            afw_utf8_contains(&line, afw_s_a_dash_dash_syntax_test_script))
        {
            return afw_compile_parse_TestScript(parser);
        }
    }

    /* Parse statements and return. */
    result = afw_compile_parse_StatementList(parser,
        NULL, end_is_close_brace, false, true, false);
    return result;
}


static void
impl_test_script_get_next_key_value(
    afw_compile_parser_t *parser,
    const afw_utf8_t **key,
    const afw_utf8_t **string,
    afw_size_t *string_offset,
    afw_size_t *string_length)
{
    afw_utf8_t line;
    afw_utf8_t remaining;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end;
    const afw_utf8_octet_t *start;
    afw_size_t start_cursor, end_cursor;
    int state;

    afw_compile_save_cursor(*string_offset);

    for (*key = NULL, *string = NULL;;) {

        afw_compile_get_raw_line(&line);
        if (afw_compile_is_at_eof()) {
            break;
        }

        if (!afw_utf8_starts_with_z(&line, "//?")) {
            AFW_COMPILE_THROW_ERROR_Z("Line must start with '//?'");
        }

        for (
            state = 0,
            c = line.s + 3,
            start = line.s,
            end = line.s + line.len;
            ;
            c++)
        {
            switch (state) {

            /* Find begin of key */
            case 0:
                if (c >= end) {
                    break;
                }
                if (*c != ' ') {
                    start = c;
                    state = 1;
                }
                break;
       
            /* Find end of key */
            case 1:
                if (c >= end) {
                    AFW_COMPILE_THROW_ERROR_Z("Expecting ':' after key");
                }
                if (*c == ':' || *c == ' ') {
                    *key = afw_utf8_create_copy(start, c - start,
                        parser->p, parser->xctx);
                    state = 2;
                    if (*c == ':') {
                        state=3;
                    }
                }
                break;

            /* Find ':' */
            case 2:
                if (c >= end) {
                    AFW_COMPILE_THROW_ERROR_Z("Expecting ':' after key");
                }
                if (*c == ':') {
                    state = 3;
                }
                else if (*c != ' ') {
                    AFW_COMPILE_THROW_ERROR_Z("Expecting ':' after key");
                }
                break;

            /* Get string. */
            case 3:

                /* If end of line, string is NULL. */
                if (c >= end) {
                    break;
                }

                /* If start of string found, extract it. */
                if (*c != ' ') {
                    start = c;
                    remaining.s = start;
                    remaining.len = end - start;

                    /* If "...", string is next line to end or "//?" line. */
                    if (afw_utf8_starts_with_z(&remaining, "...")) {
                        afw_compile_save_cursor(start_cursor);
                        afw_compile_save_cursor(*string_offset);
                        for (;;) {
                            afw_compile_save_cursor(end_cursor);
                            afw_compile_get_raw_line(&line);
                            if (!line.s) {
                                *string_length = end_cursor - start_cursor;
                                *string = afw_utf8_create_copy(
                                    parser->full_source->s + start_cursor,
                                    *string_length, parser->p, parser->xctx);
                                break;
                            }
                            else if (afw_utf8_starts_with_z(&line, "//?")) {
                                afw_compile_restore_cursor(end_cursor);
                                /* Don't include \n before //? */
                                *string_length = end_cursor - start_cursor;
                                if (*string_length > 0) {
                                    *string_length -= 1;
                                }
                                *string = afw_utf8_create_copy(
                                    parser->full_source->s + start_cursor,
                                    *string_length,  parser->p, parser->xctx);
                                break;
                            }
                        }
                        c = end;
                    }

                    /*
                     * If not "...", string is rest of line with trailing
                     * AFW Whitespace trimmed (Unicode; same idea as
                     * afw_compile_skip_ws / Whitespace production — not only
                     * ASCII space). Leading spaces after ':' are already
                     * skipped when locating start. Use "..." or a future
                     * <<< form when exact edge whitespace matters.
                     */
                    else {
                        afw_size_t value_len;
                        afw_size_t offset;
                        afw_size_t end_trim;
                        afw_code_point_t cp;

                        *string_offset = (afw_size_t)(start -
                            parser->full_source->s);
                        value_len = (afw_size_t)(end - start);
                        offset = 0;
                        end_trim = 0;
                        while (offset < value_len) {
                            cp = afw_utf8_next_code_point(start, &offset,
                                value_len, parser->xctx);
                            if (cp < 0) {
                                AFW_COMPILE_THROW_ERROR_Z(
                                    "Invalid utf-8 in test script key value");
                            }
                            /*
                             * Whitespace production (Zs, tab, VT, FF, ZWNBSP).
                             * Also drop EOL code points if present on the line
                             * (e.g. stray CR).
                             */
                            if (!afw_compile_code_point_is_Whitespace(cp) &&
                                !afw_compile_code_point_is_EOL(cp))
                            {
                                end_trim = offset;
                            }
                        }
                        *string_length = end_trim;
                        *string = afw_utf8_create_copy(start, *string_length,
                            parser->p, parser->xctx);
                        c = end;
                    }
                }
                break;
            }

            /* If at end of line, break. */
            if (c >= end) {
                break;
            }
        }

        /* If key found, finished. */
        if (*key) {
            break;
        }
    }

    if (afw_utf8_equal(*key, afw_s_expect)) {
        if (*string &&
            afw_utf8_starts_with(*string, afw_s_error) &&
            (*string)->len > afw_self_s_error.len &&
            *((*string)->s + afw_self_s_error.len) != ':')
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Must be 'error' by itself or 'error:' immediately "
                "followed by the exact error message expected");
        }
    }
}


/*ebnf>>>
 *
 * TestScript ::= TestScriptShebang? TestScriptDefinition TestDefinition+
 *
 * TestScriptShebang ::= '#!' UnicodeNonControl* 'afw' UnicodeNonControl*
 *     ('-s test_script' | '--syntax test_script' ) |  UnicodeNonControl* '\n'
 *
 *# TestDescription, TestSkip, TestSourceType and TestCustomProperty can occur
 *# in any order
 * TestScriptDefinition ::=
 *     TestScriptBegin
 *     TestDescription?
 *     TestSkip?
 *     TestSourceType?
 *     TestCustomProperty*
 *
 *# TestDescription, TestExpect, TestSkip, TestSource, TestSourceType and
 *# TestUserProperty can occur in any order
 * TestDefinition ::=
 *     TestBegin
 *     TestDescription?
 *     TestExpect
 *     TestSkip?
 *     TestSource
 *     TestSourceType?
 *     TestCustomProperty*
 *
 *# Must start at the beginning of a line
 * TestScriptLineStart ::= ( '//?' '\n')* '//?'
 *
 *# The '//?' of '\n//?' is reused.  The last '/n' is ignored to allow ...
 *# values to not end with a '\n'.
 * TestScriptValue ::= (
 *     ( '...' '\n' UnicodeCodePoint* ( '\n//?' | END ) ) |
 *     ( UnicodeNonControl #x20* '\n' )
 * )
 *
 * TestScriptBegin ::= TestScriptLineStart 'test_script:' TestScriptValue
 *
 * TestBegin ::= TestScriptLineStart 'test:' TestScriptValue
 *
 *# Official case properties (also accepted at file level where listed).
 *# Any other Identifier key is still accepted as TestCustomProperty and
 *# stored as a string on the test object (no compiler enum required).
 * TestCustomProperty ::= Identifier ':' TestScriptValue
 *
 *# What the case checks. For test262-derived cases, stay close to the TC39
 *# description (light Adaptive tweaks OK). Not the place for harness notes.
 * TestDescription ::= TestScriptLineStart 'description:' TestScriptValue
 *
 *# Optional ES vs Adaptive language delta for the construct under test only
 *# (not try/assert wrappers). Harvestable for differences docs later.
 * TestDifferences ::= TestScriptLineStart 'differences:' TestScriptValue
 *
 * TestExpect ::= TestScriptLineStart
 *    'expect:' (
 *        ( 'error' '\n' ) |
 *        ( 'result' TestScriptValue )
 *    )
 *
 *# Skip when the case is not ready to run (or permanently out of scope).
 * TestSkip ::= TestScriptLineStart 'skip:' ( 'true' | 'false' ) '\n'
 *
 *# Why skip is true. Suggested prefixes (convention): Incompatible: never
 *# convert; FIXME: Adaptive should fix/decide; Deferred: later; Harness: rare
 *# runner limit. See src/afw/tests/test262/README.md.
 * TestSkipReason ::= TestScriptLineStart 'skipReason:' TestScriptValue
 *
 *# Default is script or the one specified in TestScriptDefinition
 * TestSourceType ::= TestScriptLineStart 'source_type:'
 *     ( 'json' | 'relaxed_json' | 'script' | 'template' )
 *
 * TestSource ::= TestScriptLineStart 'source:' TestScriptValue
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_TestScript(
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_object_t *test_script_object;
    const afw_array_t *test_list;
    const afw_object_t *test_object;
    const afw_utf8_t *test_script_id;
    const afw_utf8_t *key;
    const afw_utf8_t *string;
    const afw_utf8_t *global_source_type;
    const afw_utf8_t *source_location;
    const afw_utf8_t *expect_location;

    afw_size_t source_line;
    afw_size_t source_column;

    afw_size_t start_offset;
    afw_size_t up_to_tests_offset;

    afw_size_t string_offset;
    afw_size_t string_length;

    afw_utf8_t line;

    /* Save starting cursor. */
    afw_compile_save_cursor(start_offset);
    afw_compile_save_cursor(up_to_tests_offset);

    /* Shebang line must contain afw and -s test_script. */
    if (afw_compile_next_raw_starts_with_z("#!")) {
        afw_compile_get_raw_line(&line);
        if ((!afw_utf8_contains(&line, afw_s_afw) &&
            !afw_utf8_contains(&line, afw_s_JeremyScript) && /* Easter egg */
            !afw_utf8_contains(&line, afw_s_maluba))   /* Easter egg */ ||
            (!afw_utf8_contains(&line, afw_s_a_dash_s_test_script) &&
            !afw_utf8_contains(&line, afw_s_a_dash_dash_syntax_test_script)))
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Shebang line must contain afw and -s[yntax] test_script to "
                "be recognized as an adaptive test script");
        }
    }

    test_script_object = afw_object_create_unmanaged(parser->p, parser->xctx);
    test_list = afw_array_of_create(afw_data_type_object,
        parser->p, parser->xctx);
    afw_object_set_property_as_array(test_script_object,
        afw_s_tests, test_list, parser->xctx);

    /* Process TestScriptDefinition */
    for (global_source_type = NULL, test_script_id = NULL;;)
    {
        afw_compile_save_cursor(up_to_tests_offset);

        impl_test_script_get_next_key_value(parser,
            &key, &string, &string_offset, &string_length);
        if (afw_utf8_equal(key, afw_s_testScript)) {
            test_script_id = string;
        }
        else if (!test_script_id) {
            AFW_COMPILE_THROW_ERROR_Z(
                "'test_script:' must be specified first");
        }
        if (!key || afw_utf8_equal(key, afw_s_test)) {
            break;
        }
        if (afw_utf8_equal(key, afw_s_skip)) {
            if (afw_utf8_equal(string, afw_s_true)) {
                afw_object_set_property(test_script_object,
                    key, afw_boolean_v_true, parser->xctx);
            }
            else if (!afw_utf8_equal(string, afw_s_false)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "'skip:' must be 'true' or 'false'");
            }
        }
        else {
            if (afw_object_has_property(test_script_object, key, parser->xctx))
            {
                AFW_COMPILE_THROW_ERROR_FZ(
                    AFW_UTF8_FMT_Q " already specified",
                    AFW_UTF8_FMT_ARG(key));
            }
            if (afw_utf8_equal(key, afw_s_sourceType)) {
                global_source_type = string;
            }
            afw_object_set_property_as_string(test_script_object,
                key, string, parser->xctx);
        }
    }
    if (!global_source_type) {
        global_source_type = afw_s_script;
        afw_object_set_property_as_string(test_script_object,
            afw_s_sourceType, global_source_type, parser->xctx);
    }
    afw_object_set_property_as_integer(test_script_object,
        afw_s_upToTestsUTF8OctetOffsetInTestScript,
            afw_safe_cast_size_to_integer(up_to_tests_offset, parser->xctx),
            parser->xctx);

    /* Process TestDefinition */
    for (test_object = NULL; ;) {

        if (!key || afw_utf8_equal(key, afw_s_test)) {
            if (test_object) {
                if (!afw_object_has_property(test_object,
                    afw_s_source, parser->xctx))
                {
                    AFW_COMPILE_THROW_ERROR_Z("'source:' missing");
                }
                if (!afw_object_has_property(test_object,
                    afw_s_expect, parser->xctx))
                {
                    AFW_COMPILE_THROW_ERROR_Z("'expect:' missing");
                }
            }
            if (!key) {
                break;
            }
            test_object = afw_object_create_unmanaged(parser->p, parser->xctx);
            afw_array_push_value(test_list,
                afw_value_create_unmanaged_object(
                    test_object, parser->p, parser->xctx),
                parser->xctx);
            afw_object_set_property_as_string(test_object,
                afw_s_test, string, parser->xctx);
        }

        else if (afw_utf8_equal(key, afw_s_skip)) {
            if (afw_utf8_equal(string, afw_s_true)) {
                if (!test_object) {
                    AFW_COMPILE_THROW_ERROR_Z("'test:' missing");
                }
                afw_object_set_property(test_object,
                    key, afw_boolean_v_true, parser->xctx);
            }
            else if (!afw_utf8_equal(string, afw_s_false)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "skip: must be 'true' or 'false'");
            }
        }

        else if (!test_object) {
            AFW_COMPILE_THROW_ERROR_FZ(
                AFW_UTF8_FMT_Q " specified before 'test'",
                AFW_UTF8_FMT_ARG(key));
        }

        else {
            if (afw_object_has_property(test_object, key, parser->xctx)) {
                AFW_COMPILE_THROW_ERROR_FZ(
                    AFW_UTF8_FMT_Q " already specified",
                    AFW_UTF8_FMT_ARG(key));
            }
            afw_object_set_property_as_string(test_object,
                key, string, parser->xctx);
        }

        if (afw_utf8_equal(key, afw_s_expect)) {
            afw_utf8_line_column_of_offset(
                &source_line, &source_column,
                parser->full_source,
                string_offset, 4, parser->xctx);
            expect_location = afw_utf8_printf(parser->p, parser->xctx,
                AFW_UTF8_FMT
                "+" AFW_SIZE_T_FMT
                "[" AFW_SIZE_T_FMT ":" AFW_SIZE_T_FMT "]",
                AFW_UTF8_FMT_ARG(test_script_id),
                string_offset, source_line, source_column);
            afw_object_set_property_as_string(test_object,
                afw_s_expectLocation, expect_location, parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_expectLineNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_line, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_expectColumnNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_column, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_expectCodepointOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_expectCodepointLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_expectUTF8OctetOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_expectUTF8OctetLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
        }

        if (afw_utf8_equal(key, afw_s_source)) {
            afw_utf8_line_column_of_offset(
                &source_line, &source_column,
                parser->full_source,
                string_offset, 4, parser->xctx);
            source_location = afw_utf8_printf(parser->p, parser->xctx,
                AFW_UTF8_FMT
                "+" AFW_SIZE_T_FMT
                "[" AFW_SIZE_T_FMT ":" AFW_SIZE_T_FMT "]",
                AFW_UTF8_FMT_ARG(test_script_id),
                string_offset, source_line, source_column);
            afw_object_set_property_as_string(test_object,
                afw_s_sourceLocation, source_location, parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_sourceLineNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_line, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_sourceColumnNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_column, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_sourceCodepointOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_sourceCodepointLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_sourceUTF8OctetOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                afw_s_sourceUTF8OctetLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
        }

        impl_test_script_get_next_key_value(parser,
            &key, &string, &string_offset, &string_length);
    }

    string = afw_utf8_create_copy(
        parser->full_source->s, parser->full_source->len,
        parser->p, parser->xctx);
    afw_object_set_property_as_string(test_script_object,
        afw_s_source, string, parser->xctx);

    /* Result is a call_test_script value. */
    result = afw_value_call_test_script_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        test_script_object, parser->p, parser->xctx);
    return result;
}

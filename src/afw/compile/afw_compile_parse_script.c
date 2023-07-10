// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Script Parser
 *
 * Copyright (c) 2010-2023 Clemson University
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
    (const afw_value_t *)&afw_function_definition_break;

static const afw_value_t *
impl_function_definition_continue =
    (const afw_value_t *)&afw_function_definition_continue;

static const afw_value_t *
impl_function_definition_rethrow =
    (const afw_value_t *)&afw_function_definition_rethrow;


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_list_of_statements(
    afw_compile_parser_t *parser,
    afw_boolean_t end_is_close_brace,
    afw_boolean_t can_be_single_return_expression)
{
    const afw_value_t *result;
    const afw_value_t *statement;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    const afw_value_block_t *block;
    afw_size_t argc;
    afw_size_t start_offset;
    afw_boolean_t *was_expression;
    afw_boolean_t was_expression_value;

    args = afw_compile_args_create(parser);
    was_expression_value = false;
    was_expression = (can_be_single_return_expression)
        ? &was_expression_value
        : NULL;

    /* Save starting cursor. */
    afw_compile_save_cursor(start_offset);

    /* Make new block and link. */
    block = afw_compile_parse_link_new_value_block(parser, start_offset);

    /* Process statements. */
    for (;;) {

        afw_compile_get_token();
        if (end_is_close_brace) {
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

        if (was_expression_value) {
            argv = afw_pool_malloc(parser->p,
                sizeof(afw_value_t *) * 2, parser->xctx);
            argv[0] = (const afw_value_t *)&afw_function_definition_return;
            argv[1] = statement;
            statement = afw_value_call_built_in_function_create(
                afw_compile_create_contextual_to_cursor(start_offset),
                1, argv, parser->p, parser->xctx);
        }
        was_expression = NULL;

        if (statement) {
            afw_compile_args_add_value(args, statement);
        }
    }

    /* Make block of statements. */
    afw_compile_args_finalize(args, &argc, &argv);
    afw_value_block_finalize(block, argc, argv, parser->xctx);
    result = (const afw_value_t *)block;

    /* Pop block. */
    afw_compile_parse_pop_value_block(parser);

    /* Return block or list. */
    return result;
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
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    result = afw_compile_parse_Expression(parser);

    return result;
}



/*ebnf>>>
 *
 * OptionalDefineTarget ::=
 *    ( ( 'loc' | 'const' ) AssignmentTarget ) |
 *    AssignmentTarget
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_OptionalDefineTarget(
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_compile_internal_assignment_type_t assignment_type;

    /* Determine assignment type. */
    assignment_type = afw_compile_assignment_type_assign_only;
    afw_compile_get_token();
    if (afw_compile_token_is_name(&afw_s_loc)) {
        assignment_type = afw_compile_assignment_type_loc;
    }
    else if afw_compile_token_is_name(&afw_s_const) {
        assignment_type = afw_compile_assignment_type_const;
    }
    else {
        afw_compile_reuse_token();
    }

    /* Parse AssignmentTarget and return result. */
    result = afw_compile_parse_AssignmentTarget(parser, assignment_type);
    return result;
}



/*ebnf>>>
 *
 * OptionalDefineAssignment ::= (
 *     ( ( 'loc' | 'const' ) AssignmentTarget '=' Expression ) |
 *     Assignment
 * )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_OptionalDefineAssignment(
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_compile_internal_assignment_type_t assignment_type;
    const afw_value_t **argv;
    const afw_value_t *function;

    /* Determine assignment type and function_id. */
    if (afw_compile_token_is_name(&afw_s_loc)) {
        assignment_type = afw_compile_assignment_type_loc;
        function = (const afw_value_t *)&afw_function_definition_loc;
    }
    else if afw_compile_token_is_name(&afw_s_const) {
        assignment_type = afw_compile_assignment_type_const;
        function = (const afw_value_t *)&afw_function_definition_const;
    }

    /* If not const or loc, return result of normal Assignment. */
    else {
        return afw_compile_parse_Assignment(parser, NULL);
    }

    /* Call appropriate function for const and loc. */
    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 4,
        parser->xctx);

    argv[0] = function;
    argv[1] = afw_compile_parse_AssignmentTarget(parser, assignment_type);
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
        3, argv, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * Assignment ::=
 *    (
 *        AssignmentExpression
 *        ( '=' | '+=' | '-=' | '*=' |'/=' | '%=' | '**=' | '&&=' | '||=' |
 *            '??=' )
 *        Expression
 *    )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Assignment(
    afw_compile_parser_t *parser,
    afw_boolean_t *was_expression)
{
    const afw_value_t *result;
    const afw_value_t *target;
    const afw_value_t **argv;
    const afw_value_t *function;
    afw_boolean_t just_expression_okay;

    /* Initialize was_expression to false. */
    if (was_expression) {
        *was_expression = false;
    }
    just_expression_okay = false;

    if (afw_compile_token_is(open_bracket) ||
        afw_compile_token_is(open_brace) ||
        afw_compile_token_is(open_angle_bracket))
    {
        afw_compile_reuse_token();
        target = afw_compile_parse_AssignmentTarget(parser,
            afw_compile_assignment_type_assign_only);
    }
    else {
        afw_compile_reuse_token();
        target = afw_compile_parse_Evaluation(parser);
        if (!target && was_expression) {
            target = afw_compile_parse_Expression(parser);
        }
        if (!target) {
            AFW_COMPILE_THROW_ERROR_Z("Invalid assignment target");
        }
        if (was_expression) {
            just_expression_okay = true;
        }
    }

    afw_compile_get_token();
    switch (parser->token->type) {
    case afw_compile_token_type_equal:
        function = NULL;
        break;

    case afw_compile_token_type_plus_equal:
        function = (const afw_value_t *)&afw_function_definition_add;
        break;

    case afw_compile_token_type_minus_equal:
        function = (const afw_value_t *)&afw_function_definition_subtract;
        break;

    case afw_compile_token_type_multiply_equal:
        function = (const afw_value_t *)&afw_function_definition_multiply;
        break;

    case afw_compile_token_type_divide_equal:
        function = (const afw_value_t *)&afw_function_definition_divide;
        break;

    case afw_compile_token_type_modulus_equal:
        function = (const afw_value_t *)&afw_function_definition_mod;
        break;

    case afw_compile_token_type_exponentiation_equal:
        function = (const afw_value_t *)&afw_function_definition_pow;
        break;

    case afw_compile_token_type_and_equal:
        function = (const afw_value_t *)&afw_function_definition_and;
        break;

    case afw_compile_token_type_or_equal:
        function = (const afw_value_t *)&afw_function_definition_or;
        break;

    case afw_compile_token_type_nullish_equal:
        function = (const afw_value_t *)
            &afw_function_definition_nullish_coalescing;
        break;

    default:
        if (just_expression_okay) {
            afw_compile_reuse_token();
            *was_expression = true;
            return target;
        }
        else {
            AFW_COMPILE_THROW_ERROR_Z("Invalid assignment operator");
        }
    }

    if (function) {
        result = afw_compile_parse_Expression(parser);
        argv = afw_pool_malloc(parser->p,
            sizeof(afw_value_t *) * 3,
            parser->xctx);
        argv[0] = function;
        argv[1] = target;
        argv[2] = result;
        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(
                parser->token->token_source_offset),
                2, argv, parser->p, parser->xctx);
    }
    else {
        result = afw_compile_parse_Expression(parser);
    }

    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 3,
        parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_assign;
    argv[1] = target;
    argv[2] = result;
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            2, argv, parser->p, parser->xctx);

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

    /*FIXME Check for inside loop. */

    result = afw_value_call_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            0, &impl_function_definition_break,
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

    argv[0] = (const afw_value_t *)&afw_function_definition_const;
    argv[1] = afw_compile_parse_AssignmentTarget(parser,
        afw_compile_assignment_type_const);
    argv[2] = NULL;
    argv[3] = NULL;

    afw_compile_get_token();
    if (!afw_compile_token_is(equal)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '='");
    }
    argv[2] = afw_compile_parse_Expression(parser);
    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            3, argv, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * InterfaceName ::= Identifier
 * 
 * InterfaceStatement ::= 'interface' InterfaceName
 *      '{' 
 *          ( String | Identifier ) ':' Type
 *          ( ','  ( String | Identifier ) ':' Type )*
 *          ','?
 *      '}' ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_InterfaceStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    AFW_COMPILE_THROW_ERROR_Z("interface statement is not supported yet");

    return result;
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
    const afw_value_t *result;

    AFW_COMPILE_THROW_ERROR_Z("type statement is not supported yet");

    return result;
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

    /*FIXME Check for inside loop. */

    result = afw_value_call_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
        0, &impl_function_definition_continue, parser->p, parser->xctx);

    AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;

    return result;
}


/*ebnf>>>
 *
 * DeclareStatement ::= 'declare' AssignmentTarget ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_DeclareStatement(afw_compile_parser_t *parser)
{
    AFW_COMPILE_THROW_ERROR_Z("Not implemented");
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

    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_do_while;
    argv[2] = afw_compile_parse_Statement(parser, NULL);

    afw_compile_get_token();
    if (!afw_compile_token_is_name(&afw_s_while)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting while");
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
        2, argv, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * ForStatement ::= 'for'
 *   '('
 *       ( OptionalDefineAssignment ( ',' OptionalDefineAssignment )* )?
 *       ';' Expression?
 *       ';' ( Assignment ( ',' Assignment )* )?
 *   ')' Statement
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ForStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_t *value;
    const afw_list_t *list;
    const afw_value_block_t *block;
    afw_size_t start_offset;

    block = NULL;
    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 5, parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_for;

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }

    /* ( OptionalDefineAssignment ( ',' OptionalDefineAssignment )* )? ';' */
    afw_compile_get_token();
    list = NULL;
    if (!afw_compile_token_is(semicolon)) {
        for (;;) {

            /* If there are loc and/or const, a block is needed. */
            if (afw_compile_token_is_name(&afw_s_loc) ||
                afw_compile_token_is_name(&afw_s_const))
            {
                if (!block) {
                    block = afw_compile_parse_link_new_value_block(
                        parser, start_offset);
                }
                value = afw_compile_parse_OptionalDefineAssignment(parser);
            }
            else {
                value = afw_compile_parse_Assignment(parser, NULL);
            }
            if (!list) {
                list = afw_list_create_generic(parser->p, parser->xctx);
            }
            afw_list_add_value(list, value, parser->xctx);
            afw_compile_get_token();
            if (afw_compile_token_is(semicolon)) {
                break;
            }
            if (!afw_compile_token_is(comma)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ',' or ';'");
            }
            afw_compile_get_token();
        }
    }
    argv[1] = NULL;
    if (list) {
        argv[1] = afw_value_create_list(list, parser->p, parser->xctx);
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
                list = afw_list_create_generic(parser->p, parser->xctx);
            }
            afw_list_add_value(list, value, parser->xctx);
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
        argv[3] = afw_value_create_list(list, parser->p, parser->xctx);
    }

    argv[4] = afw_compile_parse_Statement(parser, NULL);

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        4, argv, parser->p, parser->xctx);

    if (block) {
        argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *), parser->xctx);
        argv[0] = result;
        afw_value_block_finalize(block, 1, argv, parser->xctx);
        result = (const afw_value_t *)block;
    }

    return result;
}



/*ebnf>>>
 *
 *# This will probably change when set operators are added.
 *
 * ForeachStatement ::=
 *    'foreach' OptionalDefineTarget 'of' Expression Statement
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_ForeachStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;

    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 4, parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_foreach;
    argv[1] = afw_compile_parse_OptionalDefineTarget(parser);

    afw_compile_get_token();
    if (!afw_compile_token_is_name(&afw_s_of))
    {
        AFW_COMPILE_THROW_ERROR_Z("Expecting 'of'");
    }

    argv[2] = afw_compile_parse_Expression(parser);
    argv[3] = afw_compile_parse_Statement(parser, NULL);

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        3, argv, parser->p, parser->xctx);

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
    afw_size_t start_offset;

    afw_compile_save_cursor(start_offset);
    afw_compile_get_token();
    if (!afw_compile_token_is_unqualified_identifier()) {
        afw_compile_reuse_token();
        return NULL;
    }

    if (afw_compile_is_reserved_word(parser, parser->token->identifier_name))
    {
        AFW_COMPILE_THROW_ERROR_Z("Function name can not be a reserved word");
    }

    afw_compile_parse_add_symbol_entry(parser,
        parser->token->identifier_name);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 4, parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_const;
    argv[1] = afw_value_create_string(parser->token->identifier_name,
        parser->p, parser->xctx);
    argv[2] = afw_compile_parse_FunctionSignatureAndBody(parser);
    argv[3] = NULL;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        3, argv, parser->p, parser->xctx);

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
        if (afw_utf8_equal(parser->token->identifier_name, &afw_s_else))
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
    argv[0] = (const afw_value_t *)&afw_function_definition_if;
    argv[1] = condition;
    argv[2] = then;
    if (otherwise) {
        argv[3] = otherwise;
    }
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc, argv, parser->p, parser->xctx);
    return result;
}



/*ebnf>>>
 *
 * LocStatement ::= 'loc' AssignmentTarget ( '=' Expression )? ';'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_LocStatement(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
 
    result = NULL;
    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 4,
        parser->xctx);

    argv[0] = (const afw_value_t *)&afw_function_definition_loc;
    argv[1] = afw_compile_parse_AssignmentTarget(parser,
        afw_compile_assignment_type_loc);
    argv[2] = NULL;
    argv[3] = NULL;

    afw_compile_get_token();
    if (!afw_compile_token_is(semicolon)) {
        if (!afw_compile_token_is(equal)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting '='");
        }
        argv[2] = afw_compile_parse_Expression(parser);
        AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    }

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset),
            3, argv, parser->p, parser->xctx);

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
    argv[0] = (const afw_value_t *)&afw_function_definition_return;
    argv[1] = afw_value_null;

    afw_compile_get_token();
    if (!afw_compile_token_is(semicolon)) {
        afw_compile_reuse_token();
        argv[1] = afw_compile_parse_Expression(parser);
        AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
    }

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        1, argv, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * SwitchStatement ::= 'switch' ParenthesizedExpression
 *     '{'
 *         ( 'case' Expression ':' Statement )*
 *         ( 'default' ':' Statement )?
 *     '}'
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_SwitchStatement(afw_compile_parser_t *parser)
{
    AFW_COMPILE_THROW_ERROR_Z("Not implemented");
}



/*ebnf>>>
 *
 *# If there is no expression, the exception is rethrow and can only be
 *# contained in a catch block. If there is an expression, it is the message
 *# for the exception to be thrown. If there is a second expression, it
 *# can be any value and will be the value for the "additional" property of the
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
                "Can only rethrow (\"throw;\") inside a catch block");
        }
        result = afw_value_call_create(contextual,
            0, &impl_function_definition_rethrow,
            parser->p, parser->xctx);
    }

    /* throw. */
    else {
        afw_compile_reuse_token();
        argv = afw_pool_calloc(parser->p, sizeof(afw_value_t *) * 3,
            parser->xctx);
        argv[0] = (const afw_value_t *)&afw_function_definition_throw;
        argc = 1;

        /* Message */
        argv[1] = afw_compile_parse_Expression(parser);

        /* Optional additional */
        afw_compile_get_token();
        if (!afw_compile_token_is(semicolon)) {
            afw_compile_reuse_token();
            argv[2] = afw_compile_parse_Expression(parser);
            argc = 2;
            AFW_COMPILE_ASSERT_NEXT_TOKEN_IS_SEMICOLON;
        }
    
        /* Create the throw function call. */
        result = afw_value_call_built_in_function_create(
            contextual, argc, argv,
            parser->p, parser->xctx);
    }

    return result;
}



/*ebnf>>>
 *
 * Catch ::= 'catch' ( '(' Identifier ')' )? Statement
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
    const afw_value_block_t *block;
    afw_size_t start_offset;
    afw_size_t argc;
    afw_boolean_t rethrow_allowed;

    rethrow_allowed = parser->rethrow_allowed;
    parser->rethrow_allowed = false;
    block = NULL;
    afw_compile_save_cursor(start_offset);

    argv = afw_pool_calloc(parser->p, sizeof(afw_value_t *) * 5, parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_try;

    /* Body */
    argv[1] = afw_compile_parse_Statement(parser, NULL);
    argc = 1;

    /* Catch */
    afw_compile_get_token();
    if (afw_compile_token_is_name(&afw_s_catch)) {
        parser->rethrow_allowed = true;
        argc = 3;
        afw_compile_get_token();
        if (!afw_compile_token_is(open_parenthesis)) {
            afw_compile_reuse_token();
        }
        else {
            argc = 4;
            block = afw_compile_parse_link_new_value_block(
                parser, start_offset);
            afw_compile_get_token();
            if (!afw_compile_token_is_unqualified_identifier()) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting identifier");
            }
            argv[4] = (const afw_value_t *)
                afw_compile_parse_variable_reference_create(parser,
                    afw_compile_create_contextual_to_cursor(start_offset),
                    afw_compile_assignment_type_loc,
                    parser->token->identifier_name);
            afw_compile_get_token();
            if (!afw_compile_token_is(close_parenthesis)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
            }
        }
        argv[3] = afw_compile_parse_Statement(parser, NULL);
        parser->rethrow_allowed = false;
    }
    else {
        afw_compile_reuse_token();
    }

    /* Finally */
    afw_compile_get_token();
    if (afw_compile_token_is_name(&afw_s_finally)) {
        if (argc == 1) {
            argc = 2;
        }

        argv[2] = afw_compile_parse_Statement(parser, NULL);
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
        argc, argv, parser->p, parser->xctx);

    /* If there is a catch with variable, create a block. */
    if (block) {
        argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *), parser->xctx);
        argv[0] = result;
        afw_value_block_finalize(block, 1, argv, parser->xctx);
        result = (const afw_value_t *)block;
    }

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

    afw_compile_save_cursor(start_offset);

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = (const afw_value_t *)&afw_function_definition_while;

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

    argv[2] = afw_compile_parse_Statement(parser, NULL);

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, parser->p, parser->xctx);

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
 * Block ::= '{' Statement* '}'
 *
 * Statement ::=
 *    ';' |
 *    Block |
 *    AssignmentStatement |
 *    BreakStatement |
 *    CallStatement |
 *    ConstStatement |
 *    ContinueStatement |
 *    DeclareStatement |
 *    DoWhileStatement |
 *    ForStatement |
 *    ForeachStatement |
 *    FunctionStatement |
 *    IfStatement |
 *    InterfaceStatement |
 *    LocStatement |
 *    ReturnStatement |
 *    SwitchStatement |
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

    /* Get token '{'. */
    if (afw_compile_token_is(open_brace)) {
        result = afw_compile_parse_list_of_statements(parser, true, false);
        return result;
    }
   
    /* If not assignment, process statement. */
    result = NULL;
    if (afw_compile_token_is(identifier) &&
        !parser->token->identifier_qualifier)
    {
        if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_loc))
        {
            result = impl_parse_LocStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_const))
        {
            result = impl_parse_ConstStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_break))
        {
            result = impl_parse_BreakStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_continue))
        {
            result = impl_parse_ContinueStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_do))
        {
            result = impl_parse_DoWhileStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_for))
        {
            result = impl_parse_ForStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_foreach))
        {
            result = impl_parse_ForeachStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_if))
        {
            result = impl_parse_IfStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_return))
        {
            result = impl_parse_ReturnStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_switch))
        {
            result = impl_parse_SwitchStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_throw))
        {
            result = impl_parse_ThrowStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_try))
        {
            result = impl_parse_TryStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_while))
        {
            result = impl_parse_WhileStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_function))
        {
            result = impl_parse_FunctionStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_interface))
        {
            result = impl_parse_InterfaceStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_type))
        {
            result = impl_parse_TypeStatement(parser);
        }
        else if (afw_utf8_equal(parser->token->identifier_name,
            &afw_s_declare))
        {
            result = impl_parse_DeclareStatement(parser);
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
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_utf8_t line;

    /*
     * Shebang line must contain afw.  If it also contains test_script, parse
     * using TestScript production.
     */
    if (afw_compile_next_raw_starts_with_z("#!")) {
        afw_compile_get_raw_line(&line);
        if (!afw_utf8_contains(&line, &afw_s_afw) &&
            !afw_utf8_contains(&line, &afw_s_maluba)) /* Easter egg */
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Shebang line must contain afw to be recognized as an "
                "adaptive script in a hybrid value");
        }
        if (afw_utf8_contains(&line, &afw_s_a_dash_s_test_script) ||
            afw_utf8_contains(&line, &afw_s_a_dash_dash_syntax_test_script))
        {
            return afw_compile_parse_TestScript(parser);
        }
    }

    /* Parse statements and return. */
    result = afw_compile_parse_list_of_statements(parser, false, true);
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
            AFW_COMPILE_THROW_ERROR_Z("Line must start with //?");
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

                    /* If "...", string is next line all all to end or "//?". */
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
                                *string_length = end_cursor - start_cursor - 1;
                                *string = afw_utf8_create_copy(
                                    parser->full_source->s + start_cursor,
                                    *string_length,  parser->p, parser->xctx);
                                break;
                            }
                        }
                        c = end;
                    }

                    /*
                     * If not "..." string is rest of line except for training
                     * whitespace.
                     */
                    else {
                        *string_offset = start - parser->full_source->s;
                        for (c = end - 1; c > start && *c == ' '; c--);
                        *string_length = c - start + 1;
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

    if (afw_utf8_equal(*key, &afw_s_expect)) {
        if (afw_utf8_starts_with(*string, &afw_s_error) &&
            (*string)->len > afw_s_error.len &&
            *((*string)->s + afw_s_error.len) != ':')
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Must be \"error\" by itself or \"error:\" immediately "
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
 *# Identifier should be one that will not collide with standard ones.
 * TestCustomProperty ::= Identifier ':' TestScriptValue
 *
 * TestDescription ::= TestScriptLineStart 'description:' TestScriptValue
 *
 * TestExpect ::= TestScriptLineStart
 *    'expect:' (
 *        ( 'error' '\n' ) |
 *        ( 'result' TestScriptValue )
 *    )
 *
 *# Skip can be use for source that is not ready to test
 * TestSkip ::= TestScriptLineStart 'skip:' 'true' '\n'
 *
 *# Default is script or the one specified in TestScriptDefinition
 * TestSourceType ::= TestScriptLineStart 'source_type:'
 *     ( 'expression' | 'expression_tuple' | 'hybrid' | 'json' |
 *       'parenthesized_expression' | 'relaxed_json' | 'script' |
 *       'template' )
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
    const afw_list_t *test_list;
    const afw_object_t *test_object;
    const afw_utf8_t *test_script_id;
    const afw_utf8_t *key;
    const afw_utf8_t *string;
    const afw_utf8_t *global_source_type;
    const afw_value_t **argv;
    const afw_utf8_t *source_location;
    const afw_utf8_t *expect_location;

    afw_size_t source_line;
    afw_size_t source_column;

    afw_size_t start_offset;

    afw_size_t string_offset;
    afw_size_t string_length;

    afw_utf8_t line;

    /* Save starting cursor. */
    afw_compile_save_cursor(start_offset);

    /* Shebang line must contain afw and -s test_script. */
    if (afw_compile_next_raw_starts_with_z("#!")) {
        afw_compile_get_raw_line(&line);
        if ((!afw_utf8_contains(&line, &afw_s_afw) &&
            !afw_utf8_contains(&line, &afw_s_maluba)) /* Easter egg */ ||
            (!afw_utf8_contains(&line, &afw_s_a_dash_s_test_script) &&
            !afw_utf8_contains(&line, &afw_s_a_dash_dash_syntax_test_script)))
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Shebang line must contain afw and -s[yntax] test_script to "
                "be recognized as an adaptive test script");
        }
    }

    test_script_object = afw_object_create(parser->p, parser->xctx);
    test_list = afw_list_of_create(afw_data_type_object,
        parser->p, parser->xctx);
    afw_object_set_property_as_list(test_script_object,
        &afw_s_tests, test_list, parser->xctx);

    /* Process TestScriptDefinition */
    for (global_source_type = NULL, test_script_id = NULL;;)
    {
        impl_test_script_get_next_key_value(parser,
            &key, &string, &string_offset, &string_length);
        if (afw_utf8_equal(key, &afw_s_testScript)) {
            test_script_id = string;
        }
        else if (!test_script_id) {
            AFW_COMPILE_THROW_ERROR_Z(
                "test_script: must be specified first");
        }
        if (!key || afw_utf8_equal(key, &afw_s_test)) {
            break;
        }
        if (afw_utf8_equal(key, &afw_s_skip)) {
            if (afw_utf8_equal(string, &afw_s_true)) {
                afw_object_set_property(test_script_object,
                    key, afw_value_true, parser->xctx);
            }
            else if (!afw_utf8_equal(string, &afw_s_false)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "skip: must be true or false");
            }
        }
        else {
            if (afw_object_has_property(test_script_object, key, parser->xctx))
            {
                AFW_COMPILE_THROW_ERROR_FZ(
                    "%" AFW_UTF8_FMT ": already specified",
                    AFW_UTF8_FMT_ARG(key));
            }
            if (afw_utf8_equal(key, &afw_s_sourceType)) {
                global_source_type = string;
            }
            afw_object_set_property_as_string(test_script_object,
                key, string, parser->xctx);
        }
    }
    if (!global_source_type) {
        global_source_type = &afw_s_script;
        afw_object_set_property_as_string(test_script_object,
            &afw_s_sourceType, global_source_type, parser->xctx);
    }

    /* Process TestDefinition */
    for (test_object = NULL; ;) {

        if (!key || afw_utf8_equal(key, &afw_s_test)) {
            if (test_object) {
                if (!afw_object_has_property(test_object,
                    &afw_s_source, parser->xctx))
                {
                    AFW_COMPILE_THROW_ERROR_Z("source: missing");
                }
                if (!afw_object_has_property(test_object,
                    &afw_s_expect, parser->xctx))
                {
                    AFW_COMPILE_THROW_ERROR_Z("expect: missing");
                }
            }
            if (!key) {
                break;
            }
            test_object = afw_object_create(parser->p, parser->xctx);
            afw_list_add_value(test_list,
                afw_value_create_object(test_object, parser->p, parser->xctx),
                parser->xctx);
            afw_object_set_property_as_string(test_object,
                &afw_s_test, string, parser->xctx);
        }

        else if (afw_utf8_equal(key, &afw_s_skip)) {
            if (afw_utf8_equal(string, &afw_s_true)) {
                afw_object_set_property(test_object,
                    key, afw_value_true, parser->xctx);
            }
            else if (!afw_utf8_equal(string, &afw_s_false)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "skip: must be true or false");
            }
        }

        else {
            if (afw_object_has_property(test_object, key, parser->xctx)) {
                AFW_COMPILE_THROW_ERROR_FZ(
                    "%" AFW_UTF8_FMT ": already specified",
                    AFW_UTF8_FMT_ARG(key));
            }
            afw_object_set_property_as_string(test_object,
                key, string, parser->xctx);
        }

        if (afw_utf8_equal(key, &afw_s_expect)) {
            afw_utf8_line_column_of_offset(
                &source_line, &source_column,
                parser->full_source,
                string_offset, 4, parser->xctx);
            expect_location = afw_utf8_printf(parser->p, parser->xctx,
                "%" AFW_UTF8_FMT
                "+%" AFW_SIZE_T_FMT
                "[%" AFW_SIZE_T_FMT ":%" AFW_SIZE_T_FMT "]",
                AFW_UTF8_FMT_ARG(test_script_id),
                string_offset, source_line, source_column);
            afw_object_set_property_as_string(test_object,
                &afw_s_expectLocation, expect_location, parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_expectLineNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_line, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_expectColumnNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_column, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_expectCodepointOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_expectCodepointLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_expectUTF8OctetOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_expectUTF8OctetLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
        }

        if (afw_utf8_equal(key, &afw_s_source)) {
            afw_utf8_line_column_of_offset(
                &source_line, &source_column,
                parser->full_source,
                string_offset, 4, parser->xctx);
            source_location = afw_utf8_printf(parser->p, parser->xctx,
                "%" AFW_UTF8_FMT
                "+%" AFW_SIZE_T_FMT
                "[%" AFW_SIZE_T_FMT ":%" AFW_SIZE_T_FMT "]",
                AFW_UTF8_FMT_ARG(test_script_id),
                string_offset, source_line, source_column);
            afw_object_set_property_as_string(test_object,
                &afw_s_sourceLocation, source_location, parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_sourceLineNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_line, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_sourceColumnNumberInTestScript,
                    afw_safe_cast_size_to_integer(source_column, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_sourceCodepointOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_sourceCodepointLengthInTestScript,
                    afw_safe_cast_size_to_integer(string_length, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_sourceUTF8OctetOffsetInTestScript,
                    afw_safe_cast_size_to_integer(string_offset, parser->xctx),
                    parser->xctx);
            afw_object_set_property_as_integer(test_object,
                &afw_s_sourceUTF8OctetLengthInTestScript,
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
        &afw_s_source, string, parser->xctx);

    /* Result is call to test_script_runtime_support() with testScriptObject. */
    argv = afw_pool_malloc(parser->p,
        sizeof(afw_value_t *) * 2, parser->xctx);
    argv[0] = (const afw_value_t *)
        &afw_function_definition_test_script_runtime_support;
    argv[1] = afw_value_create_object(test_script_object,
        parser->p, parser->xctx);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        1, argv, parser->p, parser->xctx);
    return result;
}

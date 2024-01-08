// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Assignment Target Parser
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_assignment_target.c
 * @brief Adaptive Framework Compiler Script Parser.
 */

#include "afw_internal.h"



/*ebnf>>>
 *
 * AssignmentListDestructureTarget ::=
 *   '['
 *        (
 *            ( '...' AssignmentBindingTarget ) |
 *            ( AssignmentElement (',' AssignmentElement )*
 *                ( ',' '...' AssignmentBindingTarget )? )
 *        )
 *   ']'
 *
 *<<<ebnf*/
/* Note: Token '[' is already consumed. */
AFW_DEFINE_INTERNAL(const afw_compile_list_destructure_t *)
afw_compile_parse_AssignmentListDestructureTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type)
{
    afw_compile_list_destructure_t *ld;
    afw_compile_assignment_element_t *e;
    afw_compile_assignment_element_t *new_e;

    ld = afw_pool_calloc_type(parser->p,
        afw_compile_list_destructure_t, parser->xctx);
    afw_compile_get_token();
    if (!afw_compile_token_is(ellipsis)) {
        afw_compile_reuse_token();
        ld->assignment_element = e =
            afw_compile_parse_AssignmentElement(parser, assignment_type);
        for (;;) {
            afw_compile_get_token();
            if (afw_compile_token_is(comma)) {
                afw_compile_get_token();
                if (afw_compile_token_is(ellipsis)) {
                    break;
                }
                afw_compile_reuse_token();
                new_e = afw_compile_parse_AssignmentElement(parser,
                    assignment_type);
                e->next = new_e;
                e = new_e;
            }
            else if (afw_compile_token_is(close_bracket)) {
                break;
            }
            else {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ',' or ']'");
            }
        }
    }

    if (afw_compile_token_is(ellipsis)) {
        afw_compile_parse_AssignmentBindingTarget(parser,
            assignment_type, &ld->rest_type, &ld->rest);
        afw_compile_get_token();
    }

    if (!afw_compile_token_is(close_bracket)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ']'");
    }

    return ld;
}


/*ebnf>>>
 *
 * AssignmentElement ::=
 *    AssignmentBindingTarget ( '=' Expression )?
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(afw_compile_assignment_element_t *)
afw_compile_parse_AssignmentElement(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type)
{
    afw_compile_assignment_element_t *e;

    e = afw_pool_calloc_type(parser->p,
        afw_compile_assignment_element_t, parser->xctx);
    if (afw_compile_peek_next_token_is(comma)) {
        return e;
    }
    afw_compile_parse_AssignmentBindingTarget(parser,
        assignment_type, &e->type, &e->assignment_target);
    afw_compile_get_token();
    if (afw_compile_token_is(equal)) {
        e->default_value = afw_compile_parse_Expression(parser);
    }
    else {
        afw_compile_reuse_token();
    }

    return e;
}



/*ebnf>>>
 *
 * AssignmentObjectDestructureTarget ::=
 *    '{'
 *        (
 *            ( '...' AssignmentBindingTarget ) |
 *            (
 *                AssignmentProperty ( ',' AssignmentProperty )*
 *                ( ',' '...' AssignmentBindingTarget )?
 *            )
 *        )?
 *    '}'
 *
 *<<<ebnf*/
/* Note: Token '{' is already consumed. */
AFW_DEFINE_INTERNAL(const afw_compile_object_destructure_t *)
afw_compile_parse_AssignmentObjectDestructureTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type)
{
    afw_compile_object_destructure_t *od;
    afw_compile_assignment_property_t *ap;
    afw_compile_assignment_property_t *new_ap;

    od = afw_pool_calloc_type(parser->p,
        afw_compile_object_destructure_t, parser->xctx);
    afw_compile_get_token();
    if (!afw_compile_token_is(ellipsis)) {
        afw_compile_reuse_token();
        od->assignment_property = ap =
            afw_compile_parse_AssignmentProperty(parser, assignment_type);
        for (;;) {
            afw_compile_get_token();
            if (afw_compile_token_is(comma)) {
                afw_compile_get_token();
                if (afw_compile_token_is(ellipsis)) {
                    break;
                }
                afw_compile_reuse_token();
                new_ap = afw_compile_parse_AssignmentProperty(parser,
                    assignment_type);
                ap->next = new_ap;
                ap = new_ap;
            }
            else if (afw_compile_token_is(close_brace)) {
                break;
            }
            else {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ',' or '}'");
            }
        }
    }

    if (afw_compile_token_is(ellipsis)) {
        afw_compile_parse_AssignmentBindingTarget(parser,
            assignment_type, &od->rest_type, &od->rest);
        afw_compile_get_token();
    }

    if (!afw_compile_token_is(close_brace)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '}'");
    }

    return od;
}



/*ebnf>>>
 *
 * AssignmentProperty ::=
 *    ( PropertyName ('=' Expression )? ) |
 *    ( PropertyName ( ':' AssignmentElement )? )
 *
 *<<<ebnf*/
/* Note: Token identifier is already consumed and passed as parameter. */
AFW_DEFINE_INTERNAL(afw_compile_assignment_property_t *)
afw_compile_parse_AssignmentProperty(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type)
{
    afw_compile_assignment_property_t *ap;
    const afw_compile_value_contextual_t *contextual;
    const afw_utf8_t *identifier;

/*
    if (afw_compile_token_is(identifier)) {
        name = parser->token->identifier_name;
    }
    else if (afw_compile_token_is(integer)) {
        name = afw_number_integer_to_utf8(
            parser->token->integer,
            parser->p, parser->xctx);
    }
    else {
        AFW_COMPILE_THROW_ERROR_Z("Expecting property name");
    }
 */
    ap = afw_pool_calloc_type(parser->p,
        afw_compile_assignment_property_t, parser->xctx);
    afw_compile_get_token();

    /* Get property name */
    if (!afw_compile_token_is_unqualified_identifier()) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting PropertyName");
    }
    identifier = parser->token->identifier_name;

    /* ( PropertyName ( ':' AssignmentElement )? ) */
    afw_compile_get_token();
    if (afw_compile_token_is(colon)) {
        ap->is_rename = true;
        ap->property_name = identifier;
        ap->assignment_element = afw_compile_parse_AssignmentElement(
            parser, assignment_type);
    }

    /* ( PropertyName ( '=' Expression )? ) */
    else {
        contextual = afw_compile_create_contextual_to_cursor(
            parser->token->token_source_offset);
        ap->symbol_reference = afw_compile_parse_variable_reference_create(
            parser, contextual, assignment_type, identifier, NULL);
        if (afw_compile_token_is(equal)) {
            ap->default_value = afw_compile_parse_Expression(parser);
        }
        else {
            afw_compile_reuse_token();
        }
    }

    return ap;
}



/*ebnf>>>
 *
 *#
 *# Reference is only valid if assignment_type is assign_only
 *#
 * AssignmentBindingTarget ::=
 *    AssignmentListDestructureTarget |
 *    AssignmentObjectDestructureTarget |
 *    ( VariableName - ReservedWords )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(void)
afw_compile_parse_AssignmentBindingTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_value_type_t **type,
    const afw_value_t **value)
{
    const afw_compile_value_contextual_t *contextual;
    afw_compile_assignment_target_t *target;
    const afw_utf8_t *variable_name;

    *type = NULL;
    contextual = afw_compile_create_contextual_to_cursor(
        parser->token->token_source_offset);

    afw_compile_get_token();

    /* List Destructure */
    if (afw_compile_token_is(open_bracket)) {
        target = afw_pool_calloc_type(parser->p,
            afw_compile_assignment_target_t, parser->xctx);
        target->assignment_type = assignment_type;
        target->target_type =
            afw_compile_assignment_target_type_list_destructure;
        target->list_destructure =
            afw_compile_parse_AssignmentListDestructureTarget(
                parser, target->assignment_type);
        *value = afw_value_assignment_target_create(
            contextual, target, parser->p, parser->xctx);
    }

    /* Object Destructure */
    else if (afw_compile_token_is(open_brace)) {
        target = afw_pool_calloc_type(parser->p,
            afw_compile_assignment_target_t, parser->xctx);
        target->assignment_type = assignment_type;
        target->target_type =
            afw_compile_assignment_target_type_object_destructure;
        target->object_destructure =
            afw_compile_parse_AssignmentObjectDestructureTarget(
                parser, target->assignment_type);
        *value = afw_value_assignment_target_create(
            contextual, target, parser->p, parser->xctx);
    }

    /* Special case for handling call from afw_compile_parse_AssignmentTarget */
    else if (assignment_type == afw_compile_assignment_type_assign_only)
    {
        afw_compile_reuse_token();
        *value = afw_compile_parse_Reference(parser);
        if (!*value) {
            AFW_COMPILE_THROW_ERROR_Z("Invalid assignment target");
        }
    }

    /* VariableName */
    else if (afw_compile_token_is_unqualified_identifier() ||
        afw_compile_token_is(open_parenthesis))
    {
        if (!afw_compile_token_is_unqualified_identifier()) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting VariableName");
        }

        variable_name = parser->token->identifier_name;
        if (afw_compile_is_reserved_word(parser, variable_name)) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Variable name can not be a reserved word");
        }
 
        if (assignment_type == afw_compile_assignment_type_let ||
            assignment_type == afw_compile_assignment_type_const)
        {
            *type = afw_compile_parse_OptionalType(parser, false);
        }

        *value = (const afw_value_t *)
            afw_compile_parse_variable_reference_create(
                parser, contextual, assignment_type,
                variable_name, *type);
    }

    else {
        AFW_COMPILE_THROW_ERROR_Z("Invalid assignment target");
    }
}



/*ebnf>>>
 *
 *#
 *# If AssignmentBindingTarget does not return an assignment_target but
 *# it does return a symbol_reference and this is an assignment_type of
 *# assign_only, create an assignment_target value from the symbol_reference.
 *#
 * AssignmentTarget ::= AssignmentBindingTarget | Reference
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_AssignmentTarget(
    afw_compile_parser_t *parser,
    afw_compile_internal_assignment_type_t assignment_type)
{
    const afw_value_t *result;
    const afw_value_type_t *type;
    afw_compile_assignment_target_t *target;

    afw_compile_parse_AssignmentBindingTarget(parser,
        assignment_type, &type, &result);
    if (!afw_value_is_assignment_target(result) &&
        assignment_type != afw_compile_assignment_type_assign_only &&
        afw_value_is_symbol_reference(result))
    {
        target = afw_pool_calloc_type(parser->p,
            afw_compile_assignment_target_t, parser->xctx);
        target->assignment_type = assignment_type;
        target->target_type =
            afw_compile_assignment_target_type_symbol_reference;
        target->variable_type = type;
        target->symbol_reference =
            (const afw_value_symbol_reference_t *)result;
        result = afw_value_assignment_target_create(
            target->symbol_reference->contextual,
            target, parser->p, parser->xctx);
    }

    return result;
}


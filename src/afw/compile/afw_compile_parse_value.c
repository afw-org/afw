// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_compile_parse_value.c
 * @brief Adaptive Framework Compiler Parser.
 */

#include "afw_internal.h"



/*ebnf>>>
 *
 *# Expression must produce a value of data type list.
 * ListExpression ::= Expression
 *
 * List ::=
 *    '['
 *        ( Json (',' Json)* )?
 *    ']'
 *
 * RelaxedList ::=
 *    '['
 *        ( RelaxedJson (',' RelaxedJson)* )? ','?
 *    ']'
 *
 * ListLiteral ::=
 *    '['
 *        ( Literal (',' Literal )* )? ','?
 *    ']'
 *
 * ListValue ::=
 *    '['
 *        (
 *            ( Expression | ( '...' ListExpression ) )
 *            (',' ( Expression | ( '...' ListExpression ) ) )*
 *        )?
 *        ','?
 *    ']'
 *
 *<<<ebnf*/
/* Parse a JSON array. Input cursor must point to a [. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_List(
    afw_compile_parser_t *parser,
    afw_boolean_t allow_expression,
    afw_boolean_t allow_enhanced_literals)
{
    const afw_list_t *list;
    const afw_value_t *entry;
    const afw_value_t *value;
    const afw_value_t *result;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    afw_size_t argc;
    afw_size_t start_offset;
    const afw_iterator_t *iterator;

    /* Return NULL if next token is not '['. */
    afw_compile_save_cursor(start_offset);
    afw_compile_get_token();
    if (!afw_compile_token_is(open_bracket)) {
        afw_compile_reuse_token();
        return NULL;
    }

    /* Create result list. */
    list = NULL;
    args = NULL;

    /* Process array values until ']'. */
    afw_compile_get_token();
    if (!afw_compile_token_is(close_bracket)) {
        afw_compile_reuse_token();
        for (;;) {

            /* Spread operator. */
            afw_compile_get_token();
            if (allow_expression && afw_compile_token_is(ellipsis)) {
                if ((parser)->compile_type == afw_compile_type_json ||
                    (parser)->compile_type == afw_compile_type_relaxed_json)
                {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "'...' is not allowed as json list entry");
                }

                /* Entry is list expression. */
                entry = afw_value_create_list_expression(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    afw_compile_parse_Expression(parser),
                    parser->p, parser->xctx);
            }

            /* Not spread operator. */
            else {
                afw_compile_reuse_token();

                /* Next should be a value. */
                if (allow_expression) {
                    entry = afw_compile_parse_Expression(parser);
                }
                else if (allow_enhanced_literals) {
                    entry = afw_compile_parse_Literal(parser,
                        NULL, true, false);
                }
                else {
                    entry = afw_compile_parse_Json(parser);
                }
            }

            /*
             * If not already building list constructor and this is not an
             * evaluated value, start building its args.
             */
            if (!args && !afw_value_is_defined_and_evaluated(entry)) {
                args = afw_compile_args_create(parser);
                afw_compile_args_add_value(args,
                    (const afw_value_t *)&afw_function_definition_list);

                /* Add previous list entries as args for constructor. */
                if (list) {
                    for (iterator = NULL;;) {
                        value = afw_list_get_next_value(list, &iterator,
                            parser->p, parser->xctx);
                        if (!value) {
                            break;
                        }
                        afw_compile_args_add_value(args, value);
                    }
                }
                list = NULL;
            }

            /* If building constructor, add entry as arg. */
            if (args) {
                afw_compile_args_add_value(args, entry);               
            }

            /* If all values so far are evaluated, add entry to list. */
            else {
                if (!list) {
                    list = afw_list_create_generic(parser->p, parser->xctx);
                }

                /* Add value to list. */
                afw_list_add_value(list, entry, parser->xctx);
            }

            /*
             * If next token is ',' continue if strict mode or next tokens is
             * not ']'.
             */
            afw_compile_get_token();
            if (afw_compile_token_is(comma)) {
                if (!parser->strict) {
                    afw_compile_get_token();
                    if (afw_compile_token_is(close_bracket)) {
                        break;
                    }
                    afw_compile_reuse_token();
                }
                continue;
            }

            /* Or if it is a ']', it's end of array. */
            if (afw_compile_token_is(close_bracket)) {
                break;
            }

            /* Anything else is an error. */
            AFW_COMPILE_THROW_ERROR_Z(
                "Array values must be separated with a comma");
        }
    }

    /* If there is a spread, a result is call to add_entries(). */
    if (args) {
        afw_compile_args_finalize(args, &argc, &argv);
        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            argc - 1, argv, parser->p, parser->xctx);
    }

    /* Else if no entries yet, result is empty list. */
    else if (!list) {
        list = afw_list_create_generic(parser->p, parser->xctx);
        result = afw_value_create_list(list, parser->p, parser->xctx);
    }

    /* Else result is an evaluated list value. */
    else {
        result = afw_value_create_list(list, parser->p, parser->xctx);
    }

    /* Return list value or call to add_entries. */
    return result;
}



/*ebnf>>>
 *
 *# Expression must produce a value of data type object.
 * ObjectExpression ::= Expression
 *
 * Object ::=
 *    '{'
 *        (
 *            ( String ':' Json )
 *            (',' String ':' Json )*
 *        )?
 *    '}'
 *
 * RelaxedObject ::=
 *    '{'
 *        (
 *            ( ( String | Identifier ) ':' RelaxedJson )
 *            (',' ( String | Identifier ) ':' RelaxedJson )*
 *            ','?
 *        )?
 *        ','?
 *    '}'
 *
 * ObjectLiteral ::=
 *    '{'
 *        (
 *            ( ( String | Identifier ) ':' Literal ) 
 *            ( ',' ( String | Identifier ) ':' Literal )*
 *        )?
 *        ','?
 *    '}'
 *
 * ObjectValue ::=
 *    '{'
 *        (
 *            (
 *                ( ( String | Identifier ) ':' Expression ) |
 *                ('...' ObjectExpression )
 *            )
 *            ( ','
 *                (
 *                    ( ( String | Identifier ) ':' Expression ) |
 *                    ('...' ObjectExpression )
 *                )
 *            )*
 *        )?
 *        ','?
 *    '}'
 *
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Object(
    afw_compile_parser_t *parser,
    afw_boolean_t allow_expression,
    afw_boolean_t allow_enhanced_literals)
{
    const afw_object_t *obj;
    const afw_value_t *v;
    const afw_object_t *embedding_object;
    const afw_utf8_t *property_name;
    const afw_object_t *_meta_;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    const afw_value_t *result;
    afw_size_t argc;
    afw_size_t start_offset;
    afw_boolean_t save_doing_object_spread;
    afw_boolean_t is_object_expression;

    /* Return NULL if next token is not '{'. */
    afw_compile_save_cursor(start_offset);
    afw_compile_get_token();
    if (!afw_compile_token_is(open_brace)) {
        afw_compile_reuse_token();
        return NULL;
    }

    /* Create new memory object.*/
    AFW_OBJECT_CREATE_ENTITY_OR_EMBEDDED(
        obj,
        parser->embedding_object,
        parser->property_name,
        parser->doing_object_spread,
        parser->cede_p, parser->p, parser->xctx);
    _meta_ = NULL;
    args = NULL;
    result = NULL;
    is_object_expression = false;

    /*
     * Save parser->embedding_object and set to new object.
     *
     * Save parser->property_name and set to NULL.
     */
    embedding_object = parser->embedding_object;
    parser->embedding_object = obj;
    property_name = parser->property_name;
    parser->property_name = NULL;

    /* Process name/value pairs until '}'. */
    afw_compile_get_token();
    if (!afw_compile_token_is(close_brace)) {
        afw_compile_reuse_token();
        for (;;) {

            afw_compile_get_token();
            parser->property_name = NULL;

            if (parser->strict) {
                if (afw_compile_token_is(utf8_string) &&
                    parser->token->string_quote_character == '"')
                {
                    parser->property_name = parser->token->string;
                }
            }

            else if (afw_compile_token_is(utf8_string)) {
                parser->property_name = parser->token->string;
            }

            else if (afw_compile_token_is(identifier) &&
                !parser->token->identifier_qualifier)
            {
                parser->property_name = parser->token->identifier_name;
            }

            /* If spread operator. */
            if (allow_expression && afw_compile_token_is(ellipsis)) {

                /*
                 * If this is first spread, start making args for add_entries().
                 */
                if (!args) {
                    args = afw_compile_args_create(parser);
                    afw_compile_args_add_value(args,
                        (const afw_value_t *)&afw_function_definition_add_properties);
                    if (is_object_expression) {
                        result = afw_value_create_object_expression(
                            afw_compile_create_contextual_to_cursor(start_offset),
                            obj, parser->p, parser->xctx);
                    }
                    else {
                        result = afw_value_create_object(obj, parser->p, parser->xctx);
                    }
                    afw_compile_args_add_value(args, result);
                    is_object_expression = false;
                }

                /* Add spread object expression to args. */
                save_doing_object_spread = parser->doing_object_spread;
                parser->doing_object_spread = true;
                v = afw_compile_parse_Expression(parser);
                parser->doing_object_spread = save_doing_object_spread;
                afw_compile_args_add_value(args, v);

                /* A new object will be started if needed. */
                obj = NULL;
            }

            /* If not spread */
            else {
                if (!parser->property_name) {
                    AFW_COMPILE_THROW_ERROR_Z("Invalid property name");
                }

                /* Next should be a colon. */
                afw_compile_get_token();
                if (!afw_compile_token_is(colon)) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Name of an object value must be followed by a colon");
                }

                /* Next should be a value. */
                if (allow_expression) {
                    v = afw_compile_parse_Expression(parser);
                }
                else if (allow_enhanced_literals) {
                    v = afw_compile_parse_Literal(parser,
                        NULL, true, false);
                }
                else {
                    v = afw_compile_parse_Json(parser);
                }

                /* If this is _meta_ property, set meta in object. */
                if (afw_utf8_equal(parser->property_name, &afw_s__meta_)) {
                    if (!afw_value_is_object(v)) {
                        AFW_COMPILE_THROW_ERROR_Z(
                            "_meta_ property must be an object");
                    }
                    _meta_ = ((const afw_value_object_t *)v)->internal;
                }

                /*
                 * If not _meta_ property, set property in new object to this
                 * value if it's not an object.  Object will already have been
                 * added by afw_object_create_embedded().
                 */
                else if (!afw_value_is_object(v)) {
                    if (!obj) {
                        obj = afw_object_create(
                            parser->p, parser->xctx);
                        afw_compile_args_add_value(args,
                            afw_value_create_object(obj,
                                parser->p, parser->xctx));
                    }
                    afw_object_set_property(obj, parser->property_name, v,
                        parser->xctx);
                    if (!afw_value_is_defined_and_evaluated(v)) {
                        is_object_expression = true;
                    }
                }

            }
 
            /*
             * If next token is ',' continue if strict mode or next tokens is
             * not '}'.
             */
            afw_compile_get_token();
            if (afw_compile_token_is(comma)) {
                if (!parser->strict) {
                    afw_compile_get_token();
                    if (afw_compile_token_is(close_brace)) {
                        break;
                    }
                    afw_compile_reuse_token();
                }
                continue;
            }

            /* Or if it is '}', it's end of object. */
            if (afw_compile_token_is(close_brace)) break;

            /* Anything else is an error. */
            AFW_COMPILE_THROW_ERROR_Z(
                "Name/Value pairs must be separated with a comma");
        }
    }

    /* If there were spread operators, result is call to add_properties(). */
    if (args) {
        afw_compile_args_finalize(args, &argc, &argv);
        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            argc - 1, argv, parser->p, parser->xctx);
    }

    /* Else if object expression, result is object expression value. */
    else if (is_object_expression) {
        result = afw_value_create_object_expression(
            afw_compile_create_contextual_to_cursor(start_offset),
            obj, parser->p, parser->xctx);
    }

    /* Else result is object value. */
    else {
        afw_object_meta_set_meta_object(obj, _meta_, parser->xctx);
        result = afw_value_create_object(obj, parser->p, parser->xctx);
    }

    /* Restore saved embedding object and property name. */
    parser->embedding_object = embedding_object;
    parser->property_name = property_name;

    /* Return object value. */
    return result;
}



/*ebnf>>>
 *
 * Json ::= Double | Integer | List | Object | String | 'true' | 'false' | 'null'
 *
 * RelaxedJson ::= Double | Integer | RelaxedList | RelaxedObject | String | 'true' | 'false' | 'null'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Json(afw_compile_parser_t *parser)
{
    /* parser->strict false causes RelaxedJson to be used. */

    const afw_value_t *value = NULL;

    afw_compile_get_token();

    switch (parser->token->type) {

    case afw_compile_token_type_end:
        AFW_COMPILE_THROW_ERROR_Z("Expecting Value");

    case afw_compile_token_type_open_brace:
        afw_compile_reuse_token();
        value = afw_compile_parse_Object(parser, false, false);
        break;

    case afw_compile_token_type_open_bracket:
        afw_compile_reuse_token();
        value = afw_compile_parse_List(parser, false, false);
        break;

    case afw_compile_token_type_boolean:
        value = (parser->token->boolean)
            ? afw_value_true
            : afw_value_false;
        break;

    case afw_compile_token_type_integer:
        value = afw_value_create_integer(
            parser->token->integer,
            parser->p, parser->xctx);
        break;

    case afw_compile_token_type_number:
        value = afw_value_create_double(
            parser->token->number,
            parser->p, parser->xctx);
        break;

    case afw_compile_token_type_utf8_string:
        value = afw_value_create_string(parser->token->string,
            parser->p, parser->xctx);
        break;

    case afw_compile_token_type_null:
        value = afw_value_null;
        break;

    case afw_compile_token_type_invalid:
    default:
        AFW_COMPILE_THROW_ERROR_Z("Invalid token type");
    }

    /* If value was not assigned, reuse token. */
    if (!value) {
        afw_compile_reuse_token();
    }

    /* Return value. */
    return value;
}


/*ebnf>>>
 *
 * ScalarLiteral ::= Double | Integer | String | 'true' | 'false' | 'null'
 *
 *# This may want to include Datatype(Literal) at some point too.
 * Literal ::= ListLiteral | ObjectLiteral | ScalarLiteral | 'undefined'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Literal(
    afw_compile_parser_t *parser,
    afw_boolean_t *is_Literal,
    afw_boolean_t must_be_literal,
    afw_boolean_t scalar_only)
{
    const afw_value_t *value;
    afw_boolean_t matches_production;

    matches_production = false;
    value = NULL;

    afw_compile_get_token();

    switch (parser->token->type) {

    case afw_compile_token_type_end:
        AFW_COMPILE_THROW_ERROR_Z("Expecting Value");

    case afw_compile_token_type_open_brace:
        afw_compile_reuse_token();
        if (scalar_only) {
            break;
        }
        matches_production = true;
        value = afw_compile_parse_Object(parser, false, true);
        break;

    case afw_compile_token_type_open_bracket:
        afw_compile_reuse_token();
        if (scalar_only) {
            break;
        }
        matches_production = true;
        value = afw_compile_parse_List(parser, false, true);
        break;

    case afw_compile_token_type_boolean:
        matches_production = true;
        value = (parser->token->boolean)
            ? afw_value_true
            : afw_value_false;
        break;

    case afw_compile_token_type_integer:
        matches_production = true;
        value = afw_value_create_integer(
            parser->token->integer,
            parser->p, parser->xctx);
        break;

    case afw_compile_token_type_number:
        matches_production = true;
        value = afw_value_create_double(
            parser->token->number,
            parser->p, parser->xctx);
        break;

    case afw_compile_token_type_utf8_string:
        matches_production = true;
        value = afw_value_create_string(parser->token->string,
            parser->p, parser->xctx);
        break;

    case afw_compile_token_type_null:
        matches_production = true;
        value = afw_value_null;
        break;

    case afw_compile_token_type_undefined:
        matches_production = true;
        value = NULL;
        break;

    default:
        break;
    }

    /* If requested, return is_literal. */
    if (is_Literal) {
        *is_Literal = matches_production;
    }

    /* If must be Literal, throw error if it's not. */
    if (must_be_literal && !matches_production) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting a literal");
    }

    /* If doesn't match production, reuse token. */
    if (!matches_production) {
        afw_compile_reuse_token();
    }

    /* Return value. */
    return value;
}



/*ebnf>>>
 *
 *#
 *# Evaluation must be before ParenthesizedExpression so Lambda has first shot
 *# at parenthesis.
 *#
 *
 * Value ::= ListValue |
 *         ObjectValue |
 *         ScalarLiteral |
 *         Evaluation |
 *         ParenthesizedExpression |
 *         TemplateString
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Value(afw_compile_parser_t *parser)
{
    const afw_value_t *value = NULL;
    afw_boolean_t is_Literal;

    /* If ListValue, return its value. */
    value = afw_compile_parse_List(parser, true, true);
    if (value) {
        return value;
    }

    /* If ObjectValue, return its value. */
    value = afw_compile_parse_Object(parser, true, true);
    if (value) {
        return value;
    }

    /* If ScalarLiteral, return its value. */
    value = afw_compile_parse_Literal(parser, &is_Literal, false, true);
    if (is_Literal) {
        return value;
    }

    /* If evaluation, return its value. */
    value = afw_compile_parse_Evaluation(parser);
    if (value) {
        return value;
    }

    /* If next is '(', parse ParenthesizedExpression. */
    if (afw_compile_peek_next_token_is(open_parenthesis)) {
        value = afw_compile_parse_ParenthesizedExpression(parser);
        return value;
    }

    /* If next is '`', parse TemplateString. */
    if (afw_compile_peek_next_token_is(grave)) {
        value = afw_compile_parse_TemplateString(parser);
        return value;
    }

    /* Invalid. */
    AFW_COMPILE_THROW_ERROR_Z("Expecting Value");
}

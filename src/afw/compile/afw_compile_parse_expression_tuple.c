// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_compile_parse_expression_tuple.c
 * @brief Adaptive Framework Compiler Parser.
 */

#include "afw_internal.h"


static const afw_value_t *
afw_compile_parse_ExpressionTupleObject(afw_compile_parser_t *parser);


static const afw_value_t *
afw_compile_parse_ExpressionTupleValue(afw_compile_parser_t *parser);


static void impl_parse_ExpressionTupleValues(
    afw_compile_parser_t *parser, const afw_value_t *function_value,
    afw_size_t *count, const afw_value_t * * *values);


static const afw_value_t *
impl_parse_tuple_operation_expression_tuple_with_variables(
    afw_compile_parser_t *parser);


static const afw_value_t *
impl_parse_tuple_operation_lambda_function(
    afw_compile_parser_t *parser);


static afw_value_script_function_parameter_t *
impl_parse_tuple_lambda_type(afw_compile_parser_t *parser);


static void
impl_parse_tuple_lambda_parameters(afw_compile_parser_t *parser,
    afw_size_t *count,
    const afw_value_script_function_parameter_t ** *parameters);


static const afw_value_t *
impl_parse_tuple_operation_list(
    afw_compile_parser_t *parser);


static const afw_value_t *
impl_parse_tuple_operation_object(
    afw_compile_parser_t *parser);


static const afw_value_t *
impl_parse_tuple_operation_property_reference(
    afw_compile_parser_t *parser);


static const afw_value_t *
impl_parse_tuple_operation_template(
    afw_compile_parser_t *parser);


static const afw_value_t *
impl_parse_tuple_operation_variable_reference(
    afw_compile_parser_t *parser);


static const afw_value_t *
impl_parse_tuple_operation_expression(
    afw_compile_parser_t *parser);



/*ebnf>>>*/

//* QuotedExpression ::= '"' Expression '"' /* ws: explicit */

//* QuotedFunctionName ::= '"' FunctionName '"' /* ws: explicit */

//* QuotedMethodName ::= '"' MethodName '"' /* ws: explicit */

//* QuotedPropertyName ::= '"' PropertyName '"' /* ws: explicit */

//* QuotedScript ::= '"' Script '"' /* ws: explicit */ 

//* QuotedTemplate ::= '"' Template '"' /* ws: explicit */ 

//* QuotedVariableReference ::= '"' VariableReference '"' /* ws: explicit */ 

/*<<<ebnf*/

 
 
/*ebnf>>>
 *
 * ExpressionTuple ::= '[' ExpressionTupleAnnotation ',' ExpressionTupleOperation ']'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTuple(afw_compile_parser_t *parser)
{
    const afw_value_t *annotation;
    const afw_value_t *result;
    afw_size_t save_hybrid_start_offset;
    afw_size_t start_offset;

    afw_compile_save_cursor(start_offset);

    afw_compile_get_token();

    if (!afw_compile_token_is(open_bracket)) {
        AFW_COMPILE_THROW_ERROR_Z( "Expecting '['");
    }

    save_hybrid_start_offset = parser->hybrid_start_offset;
    parser->hybrid_start_offset = parser->cursor - 1;

    /* Annotation is parsed, but ignored. */
    annotation = afw_compile_parse_ExpressionTupleAnnotation(parser);

    /* Parse based on tuple operation to get result. */
    result = afw_compile_parse_ExpressionTupleOperation(parser);

    /* If there is an annotation, make annotated value. */
    if (annotation) {
        result = afw_value_annotated_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            result,
            ((afw_value_object_t *)annotation)->internal,
            parser->p, parser->xctx);
    }

    /* Make sure tuple ends with a ']'. */
    afw_compile_get_token();
    if (!afw_compile_token_is(close_bracket)) {
        AFW_COMPILE_THROW_ERROR_Z( "Expecting ']'");
    }

    /* Restore hybrid start offset. */
    parser->hybrid_start_offset = save_hybrid_start_offset;

    /* Return result. */
    return result;
}


/*ebnf>>>
 *
 * ExpressionTupleAnnotation ::= 'null' | Object
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleAnnotation(afw_compile_parser_t *parser)
{
    afw_compile_get_token();

    if (afw_compile_token_is(null)) {
        return NULL;
    }

    if (afw_compile_token_is(open_brace)) {
        afw_compile_reuse_token();
        return afw_compile_parse_Object(parser, true, true);
    }

    AFW_COMPILE_THROW_ERROR_Z( "Expecting null or Object");
}



/*ebnf>>>
 *
 *# Expression tuple version of an expression with variable assignments
 *
 * ExpressionTupleOperation_expressionTuple  ::= '"exprT"' ','
 *    '['
 *        ( ( QuotedVariableReference "," ExpressionTupleValue ) 
 *        ( "," QuotedVariableReference "," ExpressionTupleValue )* )? 
 *    ']' ',' ExpressionTupleValue
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_expression_tuple_with_variables(
    afw_compile_parser_t *parser)
{
    const afw_value_t *expression_tuple;
    afw_value_assignment_t *assignment;
    int start_nelts;
    const afw_value_assignment_t * *assignments;
    afw_size_t start_offset;
    
    start_offset = parser->hybrid_start_offset;
    start_nelts = parser->values->nelts;

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(open_bracket)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(close_bracket)) {
        for (;;) {

            if (!afw_compile_token_is(utf8_string)) {
                goto error;
            }

            assignment = afw_pool_calloc_type(parser->p,
                afw_value_assignment_t, parser->xctx);
            APR_ARRAY_PUSH(parser->values, afw_value_assignment_t *) =
                assignment;

            assignment->name = afw_utf8_clone(parser->token->string,
                parser->p, parser->xctx);

            afw_compile_get_token();
            if (!afw_compile_token_is(comma)) {
                goto error;
            }

            assignment->value =
                afw_compile_parse_ExpressionTupleValue(parser);

            afw_compile_get_token();
            if (afw_compile_token_is(close_bracket)) {
                break;
            }
            if (!afw_compile_token_is(comma)) {
                goto error;
            }
            afw_compile_get_token();
        }
    }

    APR_ARRAY_PUSH(parser->values, afw_value_assignment_t *) = NULL;
    assignments = afw_pool_malloc(parser->p,
        (parser->values->nelts - start_nelts) *
        sizeof(afw_value_assignment_t *),
        parser->xctx);
    memcpy((void *)assignments,
        ((afw_value_assignment_t **)parser->values->elts) + start_nelts,
        (parser->values->nelts - start_nelts) *
        sizeof(afw_value_assignment_t *));
    parser->values->nelts = start_nelts;

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    expression_tuple = afw_compile_parse_ExpressionTupleValue(parser);

    return afw_value_expression_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        assignments, expression_tuple,
        parser->p, parser->xctx);

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid expression with variables tuple");
}


static afw_value_script_function_parameter_t *
impl_parse_tuple_lambda_type(afw_compile_parser_t *parser)
{
    afw_utf8_t s1;
    afw_utf8_t s2;
    const afw_utf8_octet_t *s;
    afw_size_t len;
    afw_value_script_function_parameter_t *parameter;
    afw_value_type_t *type;

    afw_compile_get_token();
    if (!afw_compile_token_is(utf8_string)) {
        goto error;
    }

    parameter = afw_pool_calloc_type(parser->p,
        afw_value_script_function_parameter_t, parser->xctx);

    s = parser->token->string->s;
    len = parser->token->string->len;
    memset(&s2, 0, sizeof(s2));
    for (s1.s = s; len > 0 && *s != ' '; s++, len--);
    s1.len = s - s1.s;
    if (len > 0) {
        s++; len--;
        for (s2.s = s; len > 0 && *s != ' '; s++, len--);
        if (len > 0) {
            goto error;
        }
        s2.len = s - s2.s;
    }

    if (s2.len > 0) {
        if (s1.len > 0) {
            type = afw_pool_calloc_type(parser->p,
                afw_value_type_t, parser->xctx);
            parameter->type = type;
            type->data_type = afw_environment_get_data_type(
                &s2, parser->xctx);
            if (!type->data_type) {
                goto error;
            }
            /** @fixme Add parsing for dataTypeParameter. */
        }
    }

    return parameter;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid lambda type");
}


static void
impl_parse_tuple_lambda_parameters(afw_compile_parser_t *parser,
    afw_size_t *count,
    const afw_value_script_function_parameter_t ** *parameters)
{
    afw_value_script_function_parameter_t *parameter;
    afw_size_t start_offset;
    int start_nelts;
    void *mem;

    start_nelts = parser->values->nelts;
    afw_compile_save_offset(start_offset);

    afw_compile_get_token();
    if (!afw_compile_token_is(open_bracket)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(close_bracket)) {
        afw_compile_parse_link_new_value_block(parser, start_offset);
        afw_compile_reuse_token();
        for (;;) {
            parameter = impl_parse_tuple_lambda_type(parser);

            afw_compile_get_token();
            if (!afw_compile_token_is(comma)) {
                goto error;
            }

            afw_compile_get_token();
            if (!afw_compile_token_is(utf8_string)) {
                goto error;
            }

            parameter->name = afw_utf8_clone(parser->token->string,
                parser->p, parser->xctx);
            
            afw_compile_parse_add_symbol_entry(parser,
                parser->token->string);

            APR_ARRAY_PUSH(parser->values, afw_value_script_function_parameter_t *) =
                parameter;

            afw_compile_get_token();
            if (afw_compile_token_is(close_bracket)) {
                break;
            }

            if (!afw_compile_token_is(comma)) {
                goto error;
            }
        }
        afw_compile_parse_pop_value_block(parser);
    }

    *count = parser->values->nelts - start_nelts;
    *parameters = NULL;
    if (*count > 0) {
        *parameters = mem = afw_pool_malloc(parser->p,
            (*count) * sizeof(afw_value_script_function_parameter_t *),
            parser->xctx);
        memcpy(mem,
            ((afw_value_script_function_parameter_t **)parser->values->elts) + start_nelts,
            (*count) * sizeof(afw_value_script_function_parameter_t *));
    }

    parser->values->nelts = start_nelts;
    return;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid lambda parameter");

}


/*ebnf>>>
 *
 *# Lambda function (return type, parameter names, expression tuple)
 *
 * ExpressionTupleOperation_lambda ::= '"lambda"' ','
 *    ( FunctionDeclarationObject | FunctionDeclarationURI ) ','
 *    ExpressionTupleValue
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_lambda_function(
    afw_compile_parser_t *parser)
{
    const afw_value_script_function_parameter_t *returns;
    afw_size_t count;
    const afw_value_script_function_parameter_t **parameters;
    const afw_value_t *body;
    afw_size_t start_offset;
    
    start_offset = parser->hybrid_start_offset;

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    returns = impl_parse_tuple_lambda_type(parser);

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    impl_parse_tuple_lambda_parameters(parser, &count, &parameters);

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    body = afw_compile_parse_ExpressionTupleValue(parser);

    if (count > 0) {
        afw_compile_parse_pop_value_block(parser);
    }

    return afw_value_script_function_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset), NULL,
        returns->type, count, parameters, body, parser->p, parser->xctx);

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid lambda function tuple");
}


/*ebnf>>>
 *
 *# An list with ExpressionTupleValue property values.
 *
 * ExpressionTupleOperation_list ::='"list"' ( "," ExpressionTupleValue )*
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_list(
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_list_t *list;
    const afw_value_t *value;

    /* Create object to hold result. */
    list = afw_list_create_generic(parser->p, parser->xctx);

    /* List properties - ( "," ExpressionTupleValue )* */
    for (;;) {

        /* "]" is end of tuple or "," is another property,value. */
        afw_compile_get_token();
        if (afw_compile_token_is(close_bracket)) {
            afw_compile_reuse_token();
            break;
        }

        /* "," */
        if (!afw_compile_token_is(comma)) {
            goto error;
        }

        /* ExpressionTupleValue */
        value = afw_compile_parse_ExpressionTupleValue(parser);

        /* Set property in object. */
        afw_list_add_value(list, value, parser->xctx);
    }

    /* Return object value. */
    result = afw_value_create_list(list, parser->p, parser->xctx);
    return result;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid list tuple");
}


/*ebnf>>>
 *
 *# An object with ExpressionTupleValue property values.
 *
 * ExpressionTupleOperation_object ::='"obj"'
 *    ( "," QuotedPropertyName "," ExpressionTupleValue )*
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_object(
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_object_t *object;
    const afw_utf8_t *property_name;
    const afw_value_t *value;

    /* Create object to hold result. */
    object = afw_object_create_managed(parser->p, parser->xctx);

    /* Object properties - (  "," QuotedPropertyName "," ExpressionTupleValue )* */
    for (;;) {

        /* "]" is end of tuple or "," is another property,value. */
        afw_compile_get_token();
        if (afw_compile_token_is(close_bracket)) {
            afw_compile_reuse_token();
            break;
        }
        if (!afw_compile_token_is(comma)) {
            goto error;
        }

        /* QuotedPropertyName */
        afw_compile_get_token();
        if (!afw_compile_token_is(utf8_string)) {
            goto error;
        }
        property_name = afw_utf8_clone(parser->token->string,
            parser->p, parser->xctx);

        /* "," */
        afw_compile_get_token();
        if (!afw_compile_token_is(comma)) {
            goto error;
        }

        /* ExpressionTupleValue */
        value = afw_compile_parse_ExpressionTupleValue(parser);

        /* Set property in object. */
        afw_object_set_property(object, property_name, value, parser->xctx);
    }

    /* Return object value. */
    result = afw_value_create_object(object, parser->p, parser->xctx);
    return result;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid object tuple");
}


/*ebnf>>>
 *
 *# Property reference (property name)
 *#
 *# This is either QuotedPropertyName or an ExpressionTuple that evaluates to a
 *# property name.
 *
 * ExpressionTupleOperation_propertyReference ::='"prop"' ','
 *    ( QuotedPropertyName | ExpressionTupleValue ) ',' ExpressionTupleObject
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_property_reference(
    afw_compile_parser_t *parser)
{
    const afw_value_t *object_value;
    const afw_value_t *key;
    const afw_value_t *result;
    afw_size_t start_offset;
    
    start_offset = parser->hybrid_start_offset;

    /* , */
    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    /* QuotedPropertyName | ExpressionTuple */
    key = NULL;
    afw_compile_get_token();
    if (afw_compile_token_is(utf8_string)) {
        key = afw_value_create_string(parser->token->string,
            parser->p, parser->xctx);
    }
    else {
        afw_compile_reuse_token();
        key = afw_compile_parse_ExpressionTupleValue(parser);
    }

    /* , */
    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    /* ExpressionTupleObject */
    object_value = afw_compile_parse_ExpressionTupleObject(parser);

    /* Create and return result. */
    result = afw_value_reference_by_key_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        object_value, key, parser->p, parser->xctx); 
    return result;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid property reference tuple");
}


/*ebnf>>>
 *
 *# A script in a string (See Script production)
 *
 * ExpressionTupleOperation_script ::='"script"' ',' QuotedScript
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_script(
    afw_compile_parser_t *parser)
{
    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(utf8_string)) {
        goto error;
    }

    /** @fixme Add offset to source location???? */
    return afw_compile_to_value(parser->token->string,
        parser->contextual.source_location,
        afw_compile_type_script,
        parser->compiled_value, parser->shared, NULL, parser->xctx);

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid script tuple");
}


/*ebnf>>>
 *
 *# A template in a string (See Template production)
 *
 * ExpressionTupleOperation_template ::='"tmpl"' ',' QuotedTemplate
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_template(
    afw_compile_parser_t *parser)
{
    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(utf8_string)) {
        goto error;
    }

    /** @fixme Add offset to source location???? */
    return afw_compile_to_value(parser->token->string,
        parser->contextual.source_location,
        afw_compile_type_template,
        parser->compiled_value, parser->shared, NULL, parser->xctx);

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid template tuple");
}


/*ebnf>>>
 *
 *# A reference to a variable (variable name)
 *
 * ExpressionTupleOperation_variableReference ::='"var"' ','
 *    QuotedVariableReference 
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_variable_reference(
    afw_compile_parser_t *parser)
{
    afw_utf8_t qualifier;
    afw_utf8_t name;
    afw_size_t len;
    const afw_utf8_octet_t *s;
    afw_size_t start_offset;
    
    start_offset = parser->hybrid_start_offset;

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(utf8_string)) {
        goto error;
    }
    afw_memory_copy(&name, parser->token->string);
    memset(&qualifier, 0, sizeof(qualifier));
    for (s = name.s, len = name.len; len > 0; len--, s++) {
        if (*s == ':') {
            if (len < 2 || *(s + 1) != ':') {
                goto error;
            }
            qualifier.len = s - name.s;
            qualifier.s = name.s;
            name.s = s + 2;
            name.len -= qualifier.len + 2;
            break;
        }
    }

    return afw_value_qualified_variable_reference_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        &qualifier, &name, parser->p, parser->xctx);

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid variable reference tuple");
}



/*ebnf>>>
 *
 *# An expression in a string (See Expression production)
 *
 * ExpressionTupleOperation_expression ::= '"expr"' ',' QuotedExpression
 *
 *<<<ebnf*/
static const afw_value_t *
impl_parse_tuple_operation_expression(
    afw_compile_parser_t *parser)
{
    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(utf8_string)) {
        goto error;
    }

    /** @fixme Add offset to source location???? */
    return afw_compile_to_value(parser->token->string,
        parser->contextual.source_location,
        afw_compile_type_expression,
        parser->compiled_value, parser->shared, NULL, parser->xctx);

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid expression tuple");
}


/*ebnf>>>
 *
 *# ExpressionTuple must evaluate to a single object.
 *
 * ExpressionTupleObject ::= Object | ExpressionTuple
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleObject(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    afw_compile_get_token();
    if (afw_compile_token_is(open_brace)) {
        afw_compile_reuse_token();
        result = afw_compile_parse_Object(parser, true, true);
    }
    else {
        afw_compile_reuse_token();
        result = afw_compile_parse_ExpressionTuple(parser);
    }

    return result;
}


/*ebnf>>>
 *
 *# Expression operation
 *
 * ExpressionTupleOperation ::=
 *    ExpressionTupleOperation_call |
 *    ExpressionTupleOperation_callAdaptiveFunction |
 *    ExpressionTupleOperation_callLambdaFunction |
 *    ExpressionTupleOperation_callMethod |
 *    ExpressionTupleOperation_expressionTuple |
 *    ExpressionTupleOperation_lambda |
 *    ExpressionTupleOperation_list |
 *    ExpressionTupleOperation_object |
 *    ExpressionTupleOperation_propertyReference |
 *    ExpressionTupleOperation_script |
 *    ExpressionTupleOperation_template |
 *    ExpressionTupleOperation_variableReference |
 *    ExpressionTupleOperation_expression
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleOperation(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t *function_value;
    const afw_compile_value_contextual_t *contextual;
    afw_utf8_t qualifier;
    afw_utf8_t name;
    const afw_value_t * *argv;
    afw_size_t argc;

    afw_compile_get_token();
    if (!afw_compile_token_is(comma)) {
        goto error;
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(utf8_string)) {
        goto error;
    }

    if (parser->token->string->len < 1) {
        goto error;
    }

/*ebnf>>>
 *
 *# Call an built-in adaptive function (function, parameters ...)
 *
 * ExpressionTupleOperation_callAdaptiveFunction ::= ( '"callA"' ',' )?
 *    ( QuotedFunctionName | ExpressionTuple )
 *    (',' ExpressionTupleValue)*
 *
 *<<<ebnf*/
    if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_call_adaptive_function))
    {
        contextual = afw_compile_create_contextual_to_cursor(parser->hybrid_start_offset);
        afw_compile_get_token();
        if (!afw_compile_token_is(comma)) {
            goto error;
        }

        /* Get function definition for built-in function. */
        afw_compile_get_token();
        if (!afw_compile_token_is(utf8_string)) {
            goto error;
        }
        afw_compile_split_qualified_name(parser->token->string,
            &qualifier, &name, parser->xctx);
        function_value = (const afw_value_t *)
            afw_environment_get_qualified_function(
            &qualifier, &name, parser->xctx);
        if (!function_value) {
            AFW_COMPILE_THROW_ERROR_FZ(
                "Unknown built-in function " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(parser->token->string));
        }

        /* Parser args and create call value result. */
        impl_parse_ExpressionTupleValues(parser, function_value, &argc, &argv);
        result = afw_value_call_create(contextual, argc, argv, true,
            parser->p, parser->xctx);
    }

/*ebnf>>>
 *
 *# Call either a lambda or adaptive function (function, parameters ...)
 *#
 *# This is either QuotedVariableReference of a variable containing a lambda value,
 *# a QuotedFunctionName identifies an adaptive function, or an ExpressionTuple
 *# that evaluates to a lambda value.
 *
 * ExpressionTupleOperation_call  ::= '"call"' ','
 *   ( QuotedVariableReference | QuotedFunctionName | ExpressionTuple )
 *   (',' ExpressionTupleValue)*
 *
 *<<<ebnf*/
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_call))
    {
        contextual = afw_compile_create_contextual_to_cursor(parser->hybrid_start_offset);
        afw_compile_get_token();
        if (!afw_compile_token_is(comma)) {
            goto error;
        }

        /*
         * If quoted string, a function value is a reference or built-in
         * function.
         */
        afw_compile_get_token();
        if (afw_compile_token_is(utf8_string)) {
            function_value = afw_compile_parse_reference_create(parser,
                contextual, parser->token->string);
        }

        /* If not quoted string, function value is an ExpressionTuple. */
        else {
            function_value = afw_compile_parse_ExpressionTuple(parser);
        }

        /* Parser args and create call value result. */
        impl_parse_ExpressionTupleValues(parser, function_value, &argc, &argv);
        result = afw_value_call_create(contextual, argc, argv, true, 
            parser->p, parser->xctx);
    }

/*ebnf>>>
 *
 *# Call a lambda function (function, parameters ...)
 *
 * ExpressionTupleOperation_callLambdaFunction ::= '"callL"' ','
 *   ( QuotedVariableReference | ExpressionTuple )
 *   (',' ExpressionTupleValue)*
 *
 *<<<ebnf*/
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_call_lambda_function))
    {
        contextual = afw_compile_create_contextual_to_cursor(parser->hybrid_start_offset);
        afw_compile_get_token();
        if (!afw_compile_token_is(comma)) {
            goto error;
        }

        /* If quoted string, function value is a reference. */
        afw_compile_get_token();
        if (afw_compile_token_is(utf8_string)) {
            function_value = afw_compile_parse_reference_create(parser,
                contextual, parser->token->string);
            if (afw_value_is_function_definition(function_value)) {
                AFW_COMPILE_THROW_ERROR_FZ(
                    "Use call or callA to call built-in function "
                    AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(parser->token->string));
            }
        }

        /* If not quoted string, function value is an ExpressionTuple. */
        else {
            function_value = afw_compile_parse_ExpressionTuple(parser);
        }

        /* Parser args and create call value result. */
        impl_parse_ExpressionTupleValues(parser, function_value, &argc, &argv);
        result = afw_value_call_create(contextual, argc, argv, true,
            parser->p, parser->xctx);
    }

    /* Expression tuple with variables. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_expression_tuple_with_variables))
    {
        result =
            impl_parse_tuple_operation_expression_tuple_with_variables(parser);
    }

    /* Lambda function. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_lambda_function))
    {
        result = impl_parse_tuple_operation_lambda_function(parser);
    }

    /* List. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_list))
    {
        result = impl_parse_tuple_operation_list(parser);
    }

    /* Object. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_object))
    {
        result = impl_parse_tuple_operation_object(parser);
    }

    /* Property reference. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_property_reference))
    {
        result = impl_parse_tuple_operation_property_reference(parser);
    }

    /* Script. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_script))
    {
        result = impl_parse_tuple_operation_script(parser);
    }

    /* Template. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_template))
    {
        result = impl_parse_tuple_operation_template(parser);
    }

    /* Variable reference. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_variable_reference))
    {
        result = impl_parse_tuple_operation_variable_reference(parser);
    }

    /* Expression. */
    else if (afw_utf8_equal(parser->token->string,
        &afw_s_a_tuple_operation_expression))
    {
        result = impl_parse_tuple_operation_expression(parser);
    }

    /* Error. */
    else {
        goto error;
    }

    /* Return result. */
    return result;

error:
    AFW_COMPILE_THROW_ERROR_Z("Invalid operation");
}



/*ebnf>>>
 *
 * ExpressionTupleValue ::= ExpressionTuple | Double | Integer | Object |
 *    String | 'true' | 'false' | 'null'
 *
 *<<<ebnf*/
static void impl_parse_ExpressionTupleValues(afw_compile_parser_t *parser,
    const afw_value_t *function_value, afw_size_t *argc, const afw_value_t * * *argv)
{
    int start_nelts;
    void *args;

    start_nelts = parser->values->nelts;
    if (function_value) {
        APR_ARRAY_PUSH(parser->values, const afw_value_t *) = function_value;
    }
    for (;;) {
        afw_compile_get_token();
        if (afw_compile_token_is(close_bracket)) {
            afw_compile_reuse_token();
            break;
        }
        if (!afw_compile_token_is(comma)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting ','");
        }
        APR_ARRAY_PUSH(parser->values, const afw_value_t *) =
            afw_compile_parse_ExpressionTupleValue(parser);
    }
    *argc = parser->values->nelts - start_nelts;
    *argv = NULL;
    if (*argc > 0) {
        args = afw_pool_malloc(parser->p,
            sizeof(afw_value_t *) * (*argc), parser->xctx);
        memcpy(args,
            ((afw_value_t **)parser->values->elts) + start_nelts,
            sizeof(afw_value_t *) * (*argc));
        *argv = args;
    }
    parser->values->nelts = start_nelts;

    /* Function value argv[0] does not count for argc. */
    if (function_value) {
        (*argc)--;
    }
}


/* Parse ExpressionTupleValue. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_ExpressionTupleValue(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    afw_compile_get_token();

    if (afw_compile_token_is(open_bracket)) {
        afw_compile_reuse_token();
        result = afw_compile_parse_ExpressionTuple(parser);
    }

    else if (afw_compile_token_is(open_brace)) {
        afw_compile_reuse_token();
        result = afw_compile_parse_Object(parser, true, true);
    }

    else if (afw_compile_token_is(utf8_string)) {
        result = afw_value_create_string(
            parser->token->string, parser->p, parser->xctx);
    }

    else if (afw_compile_token_is(integer)) {
        result = afw_value_create_integer(
            parser->token->integer, parser->p, parser->xctx);
    }

    else if (afw_compile_token_is(number)) {
        result = afw_value_create_double(
            parser->token->number, parser->p, parser->xctx);
    }

    else if (afw_compile_token_is(boolean)) {
        result = afw_value_create_boolean(
            parser->token->boolean, parser->p, parser->xctx);
    }

    else if (afw_compile_token_is(null)) {
        result = afw_value_create_null(
            parser->token->null, parser->p, parser->xctx);
    }

    else {
        AFW_COMPILE_THROW_ERROR_Z( "Invalid parameter");
    }

    return result;
}

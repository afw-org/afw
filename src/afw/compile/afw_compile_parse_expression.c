// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_expression.c
 * @brief Adaptive Framework Compiler Parser.
 */

#include "afw_internal.h"


/*ebnf>>>
 *
 *# The objectId of any /afw/_AdaptiveDataType_/ objects.
 *
 *# This production only applies if ':' occurs before the end of current line.
 * ColonBeforeEndOfLine ::= ':'
 *
 * DataType ::=
 *    'any' |
 *    'anyURI' |
 *    'base64Binary' |
 *    'boolean' |
 *    'date' |
 *    'dataTimeDuration' |
 *    'dnsName' |
 *    'double' |
 *    'expression' |
 *    'function' |
 *    'hexBinary' |
 *    'ia5String' |
 *    'integer' |
 *    'ipAddress' |
 *    'array' |
 *    'null' |
 *    'object' |
 *    'objectId' |
 *    'objectPath' |
 *    'password' |
 *    'regexp' |
 *    'rfc822Name' |
 *    'script' |
 *    'string' |
 *    'template' |
 *    'time' |
 *    'unevaluated' |
 *    'unknown' |
 *    'void' |
 *    'x500Name' |
 *    'xpathExpression' |
 *    'yearMonthDuration'
 *
 *# ObjectType should be in quoted string since some may not conform to
 *# Identifier and consistency is desired.
 * ObjectType ::= String
 *
 * Category ::= Identifier
 *
 * FunctionName ::= ( 'fn' '::')? Identifier ( '<' DataType '>' )?
 *
 * MethodName ::= ( ('fn' | Category | Qualifier) '::')? Identifier
 *
 * ParameterName ::= Identifier - ReservedWords
 *
 * PropertyName ::= Identifier
 *
 * Qualifier ::= Identifier
 *
 * VariableName ::= Identifier - ReservedWords
 * 
 * VariableReference ::= ( Qualifier '::' Identifier) || VariableName
 *
 *<<<ebnf*/



/*ebnf>>>
 *
 *# Reference is any Evaluation that evaluates to a value inf id of
 *# reference_by_key, qualified_variable_reference, or symbol_reference.
 *
 * Reference ::= Evaluation
 *
 *<<<ebnf*/
/* Parse Reference.  Returns NULL if not an evaluation. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Reference(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    result = afw_compile_parse_Evaluation(parser);

    if (result &&
        !afw_value_is_reference_by_key(result) &&
        !afw_value_is_symbol_reference(result) &&
        !afw_value_is_qualified_variable_reference(result))
    {
        AFW_COMPILE_THROW_ERROR_Z("Expecting property name or index");
    }

    return result;
}



/*ebnf>>>
 *
 * EntryFunctionLambdaOrVariableReference ::=
 *    (
 *      FunctionName |
 *      Lambda |
 *      VariableReference
 *    )
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_EntryFunctionLambdaOrVariableReference(
    afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_size_t start_offset;
    afw_value_block_symbol_t *symbol;
    const afw_value_function_definition_t *function;
    const afw_utf8_t *type_id, *untyped_function_id;

    afw_compile_save_offset(start_offset);
    result = afw_compile_parse_Lambda(parser);
    if (!result) {
        afw_compile_get_token();
        if (afw_compile_token_is(identifier)) {

            /*
             * Note:
             *     This might should test for all reserved words, but this
             *     is mainly here because this production is called after all
             *     the reserved statement names have been checked and the only
             *     other reserved words besides 'void' can not become an
             *     identifier and will not be a built-in function name.
             *  
             *     If this becomes an issue, call afw_compile_is_reserved_word()
             *     instead of testing for void, but that will take longer.
             */
            if (afw_compile_token_is_name(afw_s_void))
            {
                afw_compile_reuse_token();
                return NULL;             
            }

            /* First check to see if identifier is a variable symbol. */
            if (!parser->token->identifier_qualifier) {
                symbol = afw_compile_parse_get_symbol_entry(parser,
                    parser->token->identifier_name);
                if (symbol) {
                    result = afw_value_symbol_reference_create(
                        afw_compile_create_contextual_to_cursor(start_offset),
                        symbol, parser->p, parser->xctx);
                }
            }

            /* Next check to see if this is a function. */
            if (!result) {
                if (!parser->token->identifier_qualifier ||
                    afw_utf8_equal(parser->token->identifier_qualifier,
                        afw_s_fn))
                {
                    untyped_function_id = parser->token->identifier_name;
                    function = afw_environment_get_qualified_function(
                            NULL,
                            untyped_function_id,
                            parser->xctx);
                    if (function &&
                        afw_value_is_boolean_true(function->polymorphic))
                    {
                        afw_compile_get_token();
                        if (afw_compile_token_is(open_angle_bracket)) {
                            afw_compile_get_token();
                            if (!afw_compile_token_is_unqualified_identifier())
                            {
                                AFW_COMPILE_THROW_ERROR_Z("Expecting dataType");
                            }
                            type_id = parser->token->identifier;
                            afw_compile_get_token();
                            if (!afw_compile_token_is(close_angle_bracket)) {
                                AFW_COMPILE_THROW_ERROR_Z("Expecting '>'");
                            }
                            function = afw_environment_get_qualified_function(
                                type_id, untyped_function_id, parser->xctx);
                            if (!function) {
                                AFW_COMPILE_THROW_ERROR_FZ(
                                    "Unknown built-in function '" AFW_UTF8_FMT
                                    "<" AFW_UTF8_FMT ">'",
                                    AFW_UTF8_FMT_ARG(untyped_function_id),
                                    AFW_UTF8_FMT_ARG(type_id));
                            }
                        }
                        else {
                            afw_compile_reuse_token();
                        }
                    }
                    if (!function) {
                        AFW_COMPILE_THROW_ERROR_FZ(
                            "Unknown built-in function " AFW_UTF8_FMT_Q,
                            AFW_UTF8_FMT_ARG(untyped_function_id));                        
                    }
                    result = &function->pub;
                }

                /* Last, this must be a qualified variable. */
                else {
                    if (!parser->token->identifier_qualifier) {
                        AFW_COMPILE_THROW_ERROR_FZ(
                            "Undeclared variable " AFW_UTF8_FMT_Q,
                            AFW_UTF8_FMT_ARG(parser->token->identifier));
                    }
                    result =
                        afw_value_qualified_variable_reference_create(
                            afw_compile_create_contextual_to_cursor(start_offset),
                            parser->token->identifier_qualifier,
                            parser->token->identifier_name,
                            parser->p, parser->xctx);
                }
            }
        }
        else {
            afw_compile_reuse_token();
            return NULL;
        }
    }

    return result;
}



/*ebnf>>>
 *#
 *# Evaluation can call a FunctionName, call a Lambda defined inline, call a
 *# function in a variable, access a local variable, or access qualified a
 *# =variable.
 *#
 *# '->' calls EntryFunctionLambdaOrVariableReference with the first parameter
 *# as the value to the left of the '->' followed by the remaining parameters
 *# in ParametersExceptFirst. This acts like a function called as a method.
 *#
 *# '?->' is the same as '->' except it will return undefined if
 *# EntryFunctionLambdaOrVariableReference does not exist or contains a nullish
 *# value.  A syntax error will still be produced for a unqualified name that
 *# is not a declared variable or the name of a built-in function.
 *#
 * Evaluation ::=
 *    EntryFunctionLambdaOrVariableReference ( '?.'? Parameters )?
 *    (
 *        ( '?.'? ParametersExceptFirst ) |
 *        (
 *            ( '?.'? '[' Expression ']' ) |
 *            ( ( '.' | '?.' ) PropertyName )
 *        ) |
 *        (
 *            ( '->' | '?->' )
 *            EntryFunctionLambdaOrVariableReference
 *            '?.'? ParametersExceptFirst
 *        )
 *    )*
 *
 *<<<ebnf*/
/* Parse Evaluation.  Returns NULL if not an evaluation. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Evaluation(afw_compile_parser_t *parser)
{
    const afw_compile_value_contextual_t *contextual;
    const afw_value_t *result;
    const afw_value_t *use_function_self;
    const afw_value_t *optional_chaining_arg0;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    const afw_value_t *key;
    afw_size_t argc;
    afw_size_t start_offset;

    afw_compile_save_offset(start_offset);
    result = afw_compile_parse_EntryFunctionLambdaOrVariableReference(parser);
    if (!result) {
        return NULL;
    }

    /* Parse rest of expression */
    for (use_function_self = NULL;;)
    {
        afw_compile_save_offset(start_offset);
        afw_compile_get_token();

        /* '?.'? */
        optional_chaining_arg0 = NULL;
        if (afw_compile_token_is(optional_chaining)) {
            optional_chaining_arg0 = result;
            afw_compile_next_identifier_is_not_special_literal();
            afw_compile_get_token();
        }

        /* '?.'? Parameters */
        if (afw_compile_token_is(open_parenthesis)) {
            afw_compile_reuse_token();
            args = afw_compile_args_create(parser);
            afw_compile_args_add_value(args, result); /* Function argv[0] */
            if (use_function_self) {
                afw_compile_args_add_value(args, use_function_self);
            }
            afw_compile_parse_Parameters(parser, args);
            afw_compile_args_finalize(args, &argc, &argv);
            contextual = afw_compile_create_contextual_to_cursor(start_offset);
            result = afw_value_call_create(contextual, argc - 1, argv, true,
                parser->p, parser->xctx);
            use_function_self = NULL;
        }

        /* If required parameters from previous loop, fuss. */
        else if (use_function_self) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting parameters");
        }

        /*  '?.'? '[' Expression ']' ) */
        else if (afw_compile_token_is(open_bracket)) {
            key = afw_compile_parse_Expression(parser);
            afw_compile_get_token();
            if (!afw_compile_token_is(close_bracket)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ']'");
            }
            result = afw_value_reference_by_key_create(
                afw_compile_create_contextual_to_cursor(start_offset),
                result, key, parser->p, parser->xctx);
        }

        /* ( ( '.' | '?.' ) PropertyName ) */
        else if (optional_chaining_arg0 || afw_compile_token_is(period))
        {
            if (!optional_chaining_arg0) {
                afw_compile_next_identifier_is_not_special_literal();
                afw_compile_get_token();
            }
            if (afw_compile_token_is(identifier)) {
                if (parser->token->identifier_qualifier) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Qualifier not allowed");
                }
                key = afw_value_create_unmanaged_string(
                    parser->token->identifier_name,
                    parser->p, parser->xctx);
                result = afw_value_reference_by_key_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    result, key, parser->p, parser->xctx);
            }
            else {
                AFW_COMPILE_THROW_ERROR_Z("Expecting PropertyName");
            }
        }

        /*
         * (
         *     ( '->' | '?->' )
         *     EntryFunctionLambdaOrVariableReference
         *     '?.'? ParametersExceptFirst
         * )
         */
        else if (afw_compile_token_is(thin_arrow) ||
            afw_compile_token_is(optional_chaining_thin_arrow))
        {
            if (optional_chaining_arg0) {
                AFW_COMPILE_THROW_ERROR_Z("Unexpected '?.'");
            }
            //if (afw_compile_token_is(optional_chaining_thin_arrow)) {
            //    optional_chaining_arg0 = result;
            //}
            use_function_self = result;
            result =
                afw_compile_parse_EntryFunctionLambdaOrVariableReference(parser);
            if (!result) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting FunctionName, Lambda, Property, or "
                    "VariableReference");
            }
        }

        else {
            afw_compile_reuse_token();
            break;
        }

        /* Do optional chaining. */
        if (optional_chaining_arg0) {
            argv = afw_pool_malloc(parser->p,
                sizeof(afw_value_t *) * 3,
                parser->xctx);
            argv[0] = &afw_function_definition_optional_chaining.pub;
            argv[1] = optional_chaining_arg0;
            argv[2] = result;
            result = afw_value_call_built_in_function_create(
                afw_compile_create_contextual_to_cursor(
                    parser->token->token_source_offset),
                2, argv, true, parser->p, parser->xctx);
        }
    }

    /* Return value. */
    return result;
}



/*ebnf>>>
 *
 * ParameterList ::=
 *    (
 *        (
 *            ( RequiredParameterList | OptionalParameterList )
 *            ( ',' OptionalParameterList )*
 *            ( ',' EllipsisParameter )?
 *        ) |
 *        EllipsisParameter
 *    )    
 *
 * RequiredParameterList ::=
 *    ParameterName OptionalType
 *    (',' ParameterName OptionalType)*
 *
 * OptionalParameterList ::=
 *    ( ParameterName '?'? OptionalType ( '=' Literal )? )
 *    (
 *        ','
 *        ( ParameterName '?'? OptionalType ( '=' Literal )? )
 *    )*
 *
 * EllipsisParameter ::=
 *    '...' ParameterName OptionalType
 *
 *#
 *# An object with object type _AdaptiveFunctionDeclaration_
 *#
 * FunctionDeclarationObject ::= object
 *
 * FunctionSignature ::= '(' ParameterList ')' OptionalReturnType
 *
 *<<<ebnf*/
/*
 * This does a little more than just parse the function signature. It will
 * also parse and return the function name and require it if requested.
 * This is so the function name symbol is placed in the same block as the
 * parameters.
 */
AFW_DEFINE_INTERNAL(const afw_value_script_function_signature_t *)
afw_compile_parse_FunctionSignature(
    afw_compile_parser_t *parser,
    const afw_value_block_t **block,
    const afw_value_string_t **function_name_value,
    const afw_value_type_t **return_type)
{
    apr_array_header_t *params;
    afw_value_script_function_parameter_t *param;
    afw_value_block_symbol_t *function_symbol;
    afw_value_block_symbol_t *symbol;
    afw_size_t start_offset;
    afw_value_script_function_signature_t *signature;
    afw_boolean_t optional_encountered;
    afw_boolean_t question_this_time;

    optional_encountered = false;
    signature = afw_pool_calloc_type(parser->p,
        afw_value_script_function_signature_t, parser->xctx);
    function_symbol = NULL;

    afw_compile_save_offset(start_offset);

    /* Parse function name. */
    afw_compile_get_token();
    if (afw_compile_token_is_unqualified_identifier()) {
        if (afw_compile_is_reserved_word(parser,
            parser->token->identifier_name))
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Function name can not be a reserved word");
        }

        if (block) {
            *block = afw_compile_parse_link_new_value_block(parser,
                start_offset);
            signature->block = *block;
            function_symbol = afw_compile_parse_add_symbol_entry(parser,
                parser->token->identifier_name);
            function_symbol->symbol_type = afw_value_block_symbol_type_function;
            signature->function_name_symbol = function_symbol;
        }
        signature->function_name_value = (const afw_value_string_t *)
            afw_value_create_unmanaged_string(parser->token->identifier_name,
                parser->p, parser->xctx);
        if (function_name_value) {
            *function_name_value = signature->function_name_value;
        }
    }
    else {
        afw_compile_reuse_token();
        if (function_name_value) {
            *function_name_value = NULL;
        }
    }

    /* Parse parameters. */
    params = apr_array_make(parser->apr_p, 5,
        sizeof(afw_value_script_function_parameter_t *));

    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        afw_compile_reuse_token();
        do {
            param = afw_pool_calloc_type(parser->p,
                afw_value_script_function_parameter_t, parser->xctx);

            /* If ellipsis, this is a rest parameter. */
            afw_compile_get_token();
            if (afw_compile_token_is(ellipsis)) {
                param->is_rest = true;
            }
            else {
                afw_compile_reuse_token();
            }

            /* Next should be name. */
            afw_compile_get_token();
            if (!afw_compile_token_is_unqualified_identifier()) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting parameter name");
            }
            param->name = parser->token->identifier_name;
            if (afw_compile_is_reserved_word(parser, param->name)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Parameter name can not be a reserved word");
            }
           
            /* '?' */
            afw_compile_get_token();
            if (afw_compile_token_is(question_mark)) {
                question_this_time = true;
                param->is_optional = true;
                optional_encountered = true;
            }
            else {
                question_this_time = false;
                afw_compile_reuse_token();
            }

            /* Next is optional type. */
            param->type = afw_compile_parse_OptionalType(parser, false);

            /* Push parm on parms stack. */
            APR_ARRAY_PUSH(params, afw_value_script_function_parameter_t *) =
                param;

            /* Create block if first parameter and add symbol. */
            if (block) {
                if (!*block) {
                    *block = afw_compile_parse_link_new_value_block(parser,
                        start_offset);
                }
                signature->block = *block;
                symbol = afw_compile_parse_add_symbol_entry(
                    parser, param->name);
                symbol->symbol_type = afw_value_block_symbol_type_parameter;
                param->symbol = symbol;
                if (param->type) {
                    afw_memory_copy(&symbol->type, param->type);
                }        
            }

            /* Get next token. */
            afw_compile_get_token();

            /* If this is rest parameter, this token must be close parenthesis. */
            if (param->is_rest) {
                if (afw_compile_token_is(close_parenthesis)) {
                    break;
                }
                AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
            }

            /*  '=' Literal */
            else if (afw_compile_token_is(equal)) {
                param->default_value = afw_compile_parse_Literal(parser,
                    NULL, true, false);
                optional_encountered = true;
                afw_compile_get_token();
            }

            else if (optional_encountered && !question_this_time) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting '?' or '='");
            }

            /* Break if ')' */
            if (afw_compile_token_is(close_parenthesis)) {
                break;
            }

            /* Should be comma. */
            if (!afw_compile_token_is(comma)) {
                AFW_COMPILE_THROW_ERROR_Z("Expecting ',' or ')'");
            }

        } while (1);
    }
 
    /* Parse optional type and return completed signature. */
    signature->returns = afw_compile_parse_OptionalType(parser, true); 
    if (function_symbol && signature->returns) {
        afw_memory_copy(&function_symbol->type, signature->returns);
    }
    signature->count = params->nelts;
    signature->parameters =
        (const afw_value_script_function_parameter_t **)params->elts;
    if (return_type) {
        *return_type = signature->returns;        
    }
    return signature;
}



/*ebnf>>>
 *
 *#
 *# If Expression is an object literal it must be enclosed with parentheses.
 *#
 * FunctionSignatureAndBody ::= FunctionSignature FunctionBody
 * 
 * FunctionBody ::= ( '{' Script '}' ) | Expression
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_FunctionSignatureAndBody(
    afw_compile_parser_t *parser,
    const afw_value_string_t **function_name_value,
    const afw_value_type_t **return_type)
{
    const afw_value_t *body;
    const afw_value_block_t *block;
    const afw_value_script_function_signature_t *signature;
    afw_size_t depth;
    afw_size_t start_offset;

    block = NULL;
    afw_compile_save_offset(start_offset);

    depth = (parser->compiled_value->current_block)
        ? parser->compiled_value->current_block->depth
        : 0;

    /* Parse signature. */
    signature = afw_compile_parse_FunctionSignature(parser, &block,
        function_name_value, return_type);

    /* Parse body. */
    afw_compile_get_token();
    if (afw_compile_token_is(open_brace)) {
        body = afw_compile_parse_StatementList(parser,
            NULL, true, false, false);
    }
    else {
        afw_compile_reuse_token();
        body = afw_compile_parse_Expression(parser);
    }

    /* If there were parameters, pop block. */
    if (block) {
        afw_compile_parse_pop_value_block(parser);
    }

    /* Return lambda function value. */
    return afw_value_script_function_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        depth, signature,
        signature->returns, signature->count, signature->parameters,
        body, parser->p, parser->xctx);
}



/*ebnf>>>
 *
 *# 'function' is a reserved variable/function name.
 *
 *# This can be used in body of the Lambda to call recursively.
 * SelfReferenceLambdaName ::= FunctionName - ReservedWords
 *
 * Lambda ::= 'function' SelfReferenceLambdaName? FunctionSignatureAndBody
 *
 *<<<ebnf*/

/* 
 * Parse Lambda. Returns NULL if not a Lambda function.
 *
 * Returns an a lambda definition value.
 *
 *FIXME Change this to recognize arrow functions as well as function keyword.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Lambda(afw_compile_parser_t *parser)
{
    afw_compile_get_token();

    /* Return NULL if this is not a lambda function. */
    if (!afw_compile_token_is(identifier)) {
        afw_compile_reuse_token();
        return NULL;
    }
    if (afw_compile_token_is_name(afw_s_function)) {
        afw_compile_get_token();
        if (!afw_compile_token_is(open_parenthesis) &&
            !afw_compile_token_is(identifier))
        {
            afw_compile_reuse_token();
            return NULL;
        }
        afw_compile_reuse_token();
    }
    else {
        afw_compile_reuse_token();
        return NULL;
    }

    /* Return lambda definition. */
    return afw_compile_parse_FunctionSignatureAndBody(
        parser, NULL, NULL);
}



/*ebnf>>>
 *
 * Parameters ::= '(' Expression (',' Expression)*')'
 *
 *#
 *# Denotes a parameter list without first parameter (method style call).
 *#
 * ParametersExceptFirst ::= Parameters
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(void)
afw_compile_parse_Parameters(
    afw_compile_parser_t *parser,
    afw_compile_args_t *args)
{
    const afw_value_t *value;
    afw_boolean_t had_value;

    /* Starts with '('. */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z( "Expecting '('");
    }

    /* Loop processing parameters. */
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

        value = afw_compile_parse_Expression(parser);
        afw_compile_args_add_value(args, value);

        had_value = true;
    }
}


/*ebnf>>>
 *
 * ParenthesizedExpression ::= '(' Expression ')' Parameters*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_ParenthesizedExpression(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_compile_value_contextual_t *contextual;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    afw_size_t argc;
    afw_size_t start_offset;


    /* Parse ( expression ) */
    afw_compile_get_token();
    if (!afw_compile_token_is(open_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '('");
    }
    result = afw_compile_parse_Expression(parser);
    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
    }

    /* Parse optional Parameters. */
    while (afw_compile_peek_next_token_is(open_parenthesis)) {
        afw_compile_save_offset(start_offset);
        args = afw_compile_args_create(parser);
        afw_compile_args_add_value(args, result); /* Function argv[0] */
        afw_compile_parse_Parameters(parser, args);
        afw_compile_args_finalize(args, &argc, &argv);
        contextual = afw_compile_create_contextual_to_cursor(start_offset);
        result = afw_value_call_create(contextual, argc - 1, argv, true,
            parser->p, parser->xctx);
    }

    return result;
}


/*
 * Parse Lambda parameter type.
 *
 *FIXME
 */
/*ebnf>>>
 * 
 *# See RFC https://tools.ietf.org/html/rfc2616#section-3.7
 *# This is media-type in a quoted string
 * MediaType ::= String
 *
 * ArrayOf ::= ( 'of' 'array' )* ( 'of' Type )
 * 
 * DataTypeWithParameter ::= '('
 *               ( ( 'base64Binary' | 'hexBinary' | 'string' ) MediaType ) |
 *               ( ( 'script' | 'template' ) ReturnType ) |
 *               ( 'function' FunctionSignature ) |
 *               ( 'array' ArrayOf ) |
 *               ( ( 'object' | 'objectId' ) ObjectType ) |
 *               ( 'unevaluated' Type ) |
 *      ')'
 * 
 * TypeName ::= DataType | TypeVariableName | InterfaceName
 * 
 * TypeObject ::=
 *      '{' 
 *          PropertyName '?'?  ':' Type
 *          ( ',' PropertyName '?'?  ':' Type )*
 *          ','?
 *      '}' ';'
 *
 *# An object type _AdaptiveValueMeta_ object.
 * ValueMeta ::= 'meta' Object
 *
 * Type ::=  DataTypeWithParameter | TypeName | TypeObject | ValueMeta
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_type_t *)
afw_compile_parse_Type(afw_compile_parser_t *parser)
{
    const afw_utf8_t* dataType;
    afw_value_type_list_t *list_type;
    afw_boolean_t enclosed;
    afw_value_type_t *type;
    afw_size_t cursor_start_of_data_type_parameters;

    /* Can optionally begin with a '('. */
    afw_compile_get_token();
    enclosed = false;
    if (afw_compile_token_is(open_parenthesis)) {
        enclosed = true;
        afw_compile_get_token();
    }

    /* 'any' or DataType and optional parameters. */
    type = afw_pool_calloc_type(parser->p, afw_value_type_t, parser->xctx);
    if (afw_compile_token_is_unqualified_identifier()) {

        /* Get data_type and optional parameters.*/
        dataType = parser->token->identifier_name;
        type->data_type = afw_environment_get_data_type(dataType,
            parser->xctx);
        if (!type->data_type) {
            AFW_COMPILE_THROW_ERROR_FZ(
                "Unknown data type " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(dataType));
        }

        /* Get optional data type parameters. */
        if (enclosed && !afw_compile_token_is(close_parenthesis)) {
            afw_compile_get_token();
            cursor_start_of_data_type_parameters =
                parser->token->token_source_offset;
            for (list_type = NULL;
                !afw_compile_token_is(close_parenthesis);
                afw_compile_get_token())
            {
                /* MediaType */
                if (afw_utf8_equal(dataType, afw_s_string) ||
                    afw_utf8_equal(dataType, afw_s_base64Binary) ||
                    afw_utf8_equal(dataType, afw_s_hexBinary))
                {
                    if (!afw_compile_token_is(utf8_string)) {
                        AFW_COMPILE_THROW_ERROR_Z("Expecting quoted string");
                    }
                    type->media_type = parser->token->string;
                }

                /* ReturnType */
                else if (
                    afw_utf8_equal(dataType, afw_s_script) ||
                    afw_utf8_equal(dataType, afw_s_template))
                {
                    if (!afw_compile_token_is_name(afw_s_void)) {
                        afw_compile_reuse_token();
                        type->return_type = afw_compile_parse_Type(parser);
                    }
                }

                /* FunctionSignature */
                else if (afw_utf8_equal(dataType, afw_s_function))
                {
                    afw_compile_reuse_token();
                    type->function_signature =
                        afw_compile_parse_FunctionSignature(parser,
                            NULL, NULL, NULL);
                }

                /* ArrayOf */
                else if (afw_utf8_equal(dataType, afw_s_array))
                {
                    if (!afw_compile_token_is_name_z("of")) {
                        AFW_COMPILE_THROW_ERROR_Z(
                            "Expecting 'of'");
                    }
                    if (!list_type) {
                        list_type = afw_pool_calloc_type(
                            parser->p, afw_value_type_list_t, parser->xctx);
                        list_type->dimension = 1;
                        type->list_type = list_type;
                    }
                    afw_compile_get_token();
                    if (!afw_compile_token_is_name(afw_s_array)) {
                        list_type->dimension++;
                    }
                    else {
                        if (list_type->cell_type) {
                            AFW_COMPILE_THROW_ERROR_Z(
                                "Only 'of array' can be specified except for "
                                "final 'of'");
                        }
                        afw_compile_reuse_token();
                        list_type->cell_type = afw_compile_parse_Type(parser);
                    }
                }

                /* ObjectType */
                else if (afw_utf8_equal(dataType, afw_s_object) ||
                    afw_utf8_equal(dataType, afw_s_objectId))
                {
                    if (!afw_compile_token_is(utf8_string)) {
                        AFW_COMPILE_THROW_ERROR_Z("Expecting quoted string");
                    }
                    type->object_type_id = parser->token->string;
                }

                /* Type */
                else if (afw_utf8_equal(dataType, afw_s_unevaluated))
                {
                    afw_compile_reuse_token();
                    type->type = afw_compile_parse_Type(parser);
                }

                /* Parameters not allowed. */
                else {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Parameters are not allowed for this data type");
                }
            }

            /* Create contextual for data type parameters */
            type->data_type_parameter_contextual =
                afw_compile_create_contextual(parser,
                    cursor_start_of_data_type_parameters,
                    parser->token->token_source_offset -
                    cursor_start_of_data_type_parameters);
        }
    }

    /* ValueMetaObject. */
    else if (afw_compile_token_is(open_brace)) {
        afw_compile_reuse_token();
        AFW_COMPILE_THROW_ERROR_Z("Not implemented");
    }

    /* Expecting Type. */
    else {
        AFW_COMPILE_THROW_ERROR_Z("Expecting Type");
    }

    /* If enclosed, next should be ')' */
    if (enclosed && !afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
    }

    return type;
}


/*ebnf>>>
 *
 *# If Type is not specified, default is any.
 * OptionalType ::= ( ':' Type )?
 *
 * OptionalReturnType ::= ( ':' ( 'void' | Type ) )?
 * 
 *  *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_type_t *)
afw_compile_parse_OptionalType(
    afw_compile_parser_t *parser,
    afw_boolean_t is_return)
{
    afw_value_type_t *type;

    /* If next is ':', parse Type. */
    afw_compile_get_token();
    if (afw_compile_token_is(colon)) {
        if (is_return) {
            afw_compile_get_token();
            if (afw_compile_token_is_name(afw_s_void)) {
                return NULL;
            }
            afw_compile_reuse_token();
        }
        return afw_compile_parse_Type(parser);
    }

    /* If next is not ':', return value type for any. */
    /** @todo use defined constant */
    afw_compile_reuse_token();
    type = afw_pool_calloc_type(parser->p, afw_value_type_t, parser->xctx);
    type->data_type = afw_data_type_any;

    return type;
}



/*ebnf>>>
 *
 * NullishCoalescing ::= LogicalExpression ( '??'  LogicalExpression )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_NullishCoalescing(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_compile_args_t *args;
    afw_size_t argc;
    const afw_value_t **argv;
    afw_size_t start_offset;

    result = afw_compile_parse_LogicalExpression(parser);

    afw_compile_next_can_be_operator();
    afw_compile_get_token_and_save_offset(start_offset);
    if (!afw_compile_token_is(nullish_coalescing)) {
        afw_compile_reuse_token();
        return result;
    }

    args = afw_compile_args_create(parser);
    afw_compile_args_add_value(args,
        &afw_function_definition_nullish_coalescing.pub);
    afw_compile_args_add_value(args, result);

    for (;;)
    {
        result = afw_compile_parse_LogicalExpression(parser);
        afw_compile_args_add_value(args, result);
        afw_compile_next_can_be_operator();
        afw_compile_get_token();
        if (!afw_compile_token_is(nullish_coalescing)) {
            afw_compile_reuse_token();
            break;
        }
    }

    afw_compile_args_finalize(args, &argc, &argv);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc - 1, argv, true, parser->p, parser->xctx);

    return result;
}



/*ebnf>>>
 *
 * LogicalExpression ::= LogicalAnd ( '||' LogicalAnd )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_LogicalExpression(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    afw_size_t argc;
    afw_size_t start_offset;

    result = afw_compile_parse_LogicalAnd(parser);

    afw_compile_next_can_be_operator();
    afw_compile_get_token_and_save_offset(start_offset);
    if (!afw_compile_token_is(or)) {
        afw_compile_reuse_token();
        return result;
    }

    args = afw_compile_args_create(parser);
    afw_compile_args_add_value(args,
        &afw_function_definition_or.pub);
    afw_compile_args_add_value(args, result);

    for (;;) {
        result = afw_compile_parse_LogicalAnd(parser);
        afw_compile_args_add_value(args, result);
        afw_compile_next_can_be_operator();
        afw_compile_get_token();
        if (!afw_compile_token_is(or)) {
            afw_compile_reuse_token();
            break;
        }
    }

    afw_compile_args_finalize(args, &argc, &argv);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc - 1, argv, true, parser->p, parser->xctx);

    return result;
}


/*ebnf>>>
 *
 * LogicalAnd ::= Equality ( '&&' Equality )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_LogicalAnd(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    afw_compile_args_t *args;
    const afw_value_t **argv;
    afw_size_t argc;
    afw_size_t start_offset;

    result = afw_compile_parse_Equality(parser);

    afw_compile_next_can_be_operator();
    afw_compile_get_token_and_save_offset(start_offset);
    if (!afw_compile_token_is(and )) {
        afw_compile_reuse_token();
        return result;
    }

    args = afw_compile_args_create(parser);
    afw_compile_args_add_value(args,
        &afw_function_definition_and.pub);
    afw_compile_args_add_value(args, result);

    for (;;) {
        result = afw_compile_parse_Equality(parser);
        afw_compile_args_add_value(args, result);
        afw_compile_next_can_be_operator();
        afw_compile_get_token();
        if (!afw_compile_token_is(and )) {
            afw_compile_reuse_token();
            break;
        }
    }

    afw_compile_args_finalize(args, &argc, &argv);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        argc - 1, argv, true, parser->p, parser->xctx);

    return result;
}


/*ebnf>>>
 *
 * Equality ::= Comparison
 *   ( ('==' | '===' | '!=' | '!==' ) Comparison )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Equality(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_t *function;
    afw_size_t start_offset;

    result = afw_compile_parse_Comparison(parser);

    afw_compile_next_can_be_operator();
    afw_compile_get_token_and_save_offset(start_offset);
    function = NULL;

    switch (parser->token->type) {

    case afw_compile_token_type_equal_to:
        function = &afw_function_definition_eq.pub;
        break;

    case afw_compile_token_type_equal_value_and_type:
        function = &afw_function_definition_eqx.pub;
        break;

    case afw_compile_token_type_not_equal_to:
        function = &afw_function_definition_ne.pub;
        break;

    case afw_compile_token_type_not_equal_value_and_type:
        function = &afw_function_definition_nex.pub;
        break;

    default:
        afw_compile_reuse_token();
        return result;
    }

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = function;
    argv[1] = result;
    argv[2] = afw_compile_parse_Comparison(parser);;

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, true, parser->p, parser->xctx);

    return result;
}


/*ebnf>>>
 *
 * Comparison ::= Factor ( 
 *      ('<' | '<=' | '>' | '>=' ) Factor )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Comparison(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_function_definition_t *function;
    afw_size_t start_offset;

    result = afw_compile_parse_Factor(parser);

    afw_compile_next_can_be_operator();
    afw_compile_get_token_and_save_offset(start_offset);

    switch (parser->token->type) {

    case afw_compile_token_type_less_than:
        function = &afw_function_definition_lt;
        break;

    case afw_compile_token_type_less_than_or_equal_to:
        function = &afw_function_definition_le;
        break;

    case afw_compile_token_type_greater_than:
        function = &afw_function_definition_gt;
        break;

    case afw_compile_token_type_greater_than_or_equal_to:
        function = &afw_function_definition_ge;
        break;

    default:
        afw_compile_reuse_token();
        return result;
    }

    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = &function->pub;
    argv[1] = result;
    argv[2] = afw_compile_parse_Factor(parser);

    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, true, parser->p, parser->xctx);

    return result;
}



static const afw_value_t *
impl_parse_subtract(
    afw_compile_parser_t *parser,
    const afw_value_t *value)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = &afw_function_definition_subtract.pub;
    argv[1] = value;
    argv[2] = afw_compile_parse_Term(parser);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, true, parser->p, parser->xctx);

    afw_compile_next_can_be_operator();
    afw_compile_get_token();
    if (afw_compile_token_is(subtract)) {
        result = impl_parse_subtract(parser, result);
    }
    else {
        afw_compile_reuse_token();
    }
    return result;
}



/*ebnf>>>
 *
 * Factor ::= Term ( ('+' | '-' ) Term )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Factor(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t *value;
    afw_compile_args_t *args;
    afw_size_t argc;
    const afw_value_t **argv;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    result = afw_compile_parse_Term(parser);

    for (args = NULL;;) {

        afw_compile_next_can_be_operator();
        afw_compile_get_token();

        switch (parser->token->type) {

        case afw_compile_token_type_add:
            if (!args) {
                args = afw_compile_args_create(parser);
                afw_compile_args_add_value(args,
                    &afw_function_definition_add.pub);
                afw_compile_args_add_value(args, result);
            }
            value = afw_compile_parse_Term(parser);
            afw_compile_args_add_value(args, value);
            break;

        case afw_compile_token_type_subtract:
            if (args) {
                afw_compile_args_finalize(args, &argc, &argv);
                args = NULL;
                result = afw_value_call_built_in_function_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    argc - 1, argv, true, parser->p, parser->xctx);
                start_offset = parser->token->token_source_offset;
            }
            result = impl_parse_subtract(parser, result);
            break;

        default:
            afw_compile_reuse_token();
            if (args) {
                afw_compile_args_finalize(args, &argc, &argv);
                result = afw_value_call_built_in_function_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    argc - 1, argv, true, parser->p, parser->xctx);
            }
            return result;
        }
    }
}



static const afw_value_t *
impl_parse_divide_or_mod(
    afw_compile_parser_t *parser,
    const afw_value_t *value)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    const afw_value_t *function;
    afw_size_t start_offset;

    function = (parser->token->type == afw_compile_token_type_divide)
        ? &afw_function_definition_divide.pub
        : &afw_function_definition_mod.pub;

    start_offset = parser->token->token_source_offset;
    argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 3, parser->xctx);
    argv[0] = function;
    argv[1] = value;
    argv[2] = afw_compile_parse_Exponentiation(parser);
    result = afw_value_call_built_in_function_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        2, argv, true, parser->p, parser->xctx);
    afw_compile_next_can_be_operator();
    afw_compile_get_token();
    if (afw_compile_token_is(divide) || afw_compile_token_is(modulus))
    {
        result = impl_parse_divide_or_mod(parser, result);
    }
    else {
        afw_compile_reuse_token();
    }
    return result;
}



/*ebnf>>>
 *
 * Term ::= Exponentiation ( ('*' | '/' | '%') Exponentiation )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Term(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t *value;
    afw_compile_args_t *args;
    afw_size_t argc;
    const afw_value_t **argv;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    result = afw_compile_parse_Exponentiation(parser);

    for (args = NULL;;) {

        afw_compile_next_can_be_operator();
        afw_compile_get_token();

        switch (parser->token->type) {

        case afw_compile_token_type_multiply:
            if (!args) {
                args = afw_compile_args_create(parser);
                afw_compile_args_add_value(args,
                    &afw_function_definition_multiply.pub);
                afw_compile_args_add_value(args, result);
            }
            value = afw_compile_parse_Exponentiation(parser);
            afw_compile_args_add_value(args, value);
            break;

        case afw_compile_token_type_divide:
        case afw_compile_token_type_modulus:
            if (args) {
                afw_compile_args_finalize(args, &argc, &argv);
                args = NULL;
                result = afw_value_call_built_in_function_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    argc - 1, argv, true, parser->p, parser->xctx);
                start_offset = parser->token->token_source_offset;
            }
            result = impl_parse_divide_or_mod(parser, result);
            break;

        default:
            afw_compile_reuse_token();
            if (args) {
                afw_compile_args_finalize(args, &argc, &argv);
                result = afw_value_call_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    argc - 1, argv, true, parser->p, parser->xctx);
            }
            return result;
        }
    }
}


/*ebnf>>>
 *
 * Exponentiation ::= Prefixed ( '**' Prefixed )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Exponentiation(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t *value;
    afw_compile_args_t *args;
    afw_size_t argc;
    const afw_value_t **argv;
    afw_size_t start_offset;

    start_offset = parser->token->token_source_offset;
    result = afw_compile_parse_Prefixed(parser);

    for (args = NULL;;) {
        afw_compile_next_can_be_operator();
        afw_compile_get_token();

        if (afw_compile_token_is(exponentiation)) {
            if (!args) {
                args = afw_compile_args_create(parser);
                afw_compile_args_add_value(args,
                    &afw_function_definition_pow.pub);
                afw_compile_args_add_value(args, result);
            }
            value = afw_compile_parse_Prefixed(parser);
            afw_compile_args_add_value(args, value);
        }
        else {
            afw_compile_reuse_token();
            if (args) {
                afw_compile_args_finalize(args, &argc, &argv);
                result = afw_value_call_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    argc - 1, argv, true, parser->p, parser->xctx);
            }
            break;        
        }
    }
    
    return result;
}



/*ebnf>>>
 *
 * Prefixed ::= ( ( '+' | '-' | '!' | 'void' ) Value ) | Value
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Prefixed(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;

    afw_compile_get_token_and_save_offset(start_offset);

    switch (parser->token->type) {

    case afw_compile_token_type_unary_plus:
        result = afw_compile_parse_Value(parser);
        break;

    case afw_compile_token_type_unary_minus:
        argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 2,
            parser->xctx);
        argv[0] = &afw_function_definition_negative.pub;
        argv[1] = afw_compile_parse_Value(parser);
        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            1, argv, true, parser->p, parser->xctx);
        break;

    case afw_compile_token_type_unary_not:
        argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 2,
            parser->xctx);
        argv[0] = &afw_function_definition_not.pub;
        argv[1] = afw_compile_parse_Value(parser);
        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            1, argv, true, parser->p, parser->xctx);
        break;

    case afw_compile_token_type_identifier:
        if (!parser->token->identifier_qualifier) {
            if (afw_utf8_equal(parser->token->identifier_name,
                afw_s_void))
            {
                argv = afw_pool_malloc(parser->p, sizeof(afw_value_t *) * 2,
                    parser->xctx);
                argv[0] = &afw_function_definition_void_operator.pub;
                argv[1] = afw_compile_parse_Value(parser);
                result = afw_value_call_built_in_function_create(
                    afw_compile_create_contextual_to_cursor(start_offset),
                    1, argv, true, parser->p, parser->xctx);
                break;
            }
        }
        /* Fall through. */

    default:
        afw_compile_reuse_token();
        result = afw_compile_parse_Value(parser);
    }

    return result;
}



/*ebnf>>>
 *
 * Expression ::= NullishCoalescing ( '?' Expression ':' Expression )?
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Expression(afw_compile_parser_t *parser)
{
    const afw_value_t *result;
    const afw_value_t **argv;
    afw_size_t start_offset;

    result = afw_compile_parse_NullishCoalescing(parser);
    afw_compile_get_token_and_save_offset(start_offset);

    if (afw_compile_token_is(question_mark)) {
        argv = afw_pool_malloc(parser->p,
            sizeof(afw_value_t *) * 4, parser->xctx);
        argv[0] = &afw_function_definition_if.pub;
        argv[1] = result;
        argv[2] = afw_compile_parse_Expression(parser);

        afw_compile_get_token();
        if (!afw_compile_token_is(colon)) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting ':'");
        }
        argv[3] = afw_compile_parse_Expression(parser);

        result = afw_value_call_built_in_function_create(
            afw_compile_create_contextual_to_cursor(start_offset),
            3, argv, true, parser->p, parser->xctx);
    }

    else {
        afw_compile_reuse_token();
    }

    return result;
}

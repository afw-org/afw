// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse_expression.c
 * @brief Compiler parser for Adaptive expressions and operators.
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
 *    'array' |
 *    'base64Binary' |
 *    'boolean' |
 *    'dataTimeDuration' |
 *    'date' |
 *    'dnsName' |
 *    'double' |
 *    'expression' |
 *    'function' |
 *    'hexBinary' |
 *    'ia5String' |
 *    'integer' |
 *    'ipAddress' |
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
            afw_boolean_t saved_suppress_wrap;
            const afw_value_function_definition_t *fn_def;

            afw_compile_reuse_token();
            args = afw_compile_args_create(parser);
            afw_compile_args_add_value(args, result); /* Function argv[0] */
            if (use_function_self) {
                afw_compile_args_add_value(args, use_function_self);
            }

            /*
             * Arguments of wrap_literal_object / wrap_literal_array must not
             * auto-wrap again (issue #17 decompile/recompile round-trip).
             */
            saved_suppress_wrap = parser->suppress_object_literal_wrap;
            {
                afw_boolean_t saved_suppress_array =
                    parser->suppress_array_literal_wrap;

                fn_def = NULL;
                if (afw_value_is_function_definition(result)) {
                    fn_def = (const afw_value_function_definition_t *)result;
                }
                if (fn_def &&
                    afw_utf8_equal_utf8_z(&fn_def->functionId->internal,
                        "wrap_literal_object"))
                {
                    parser->suppress_object_literal_wrap = true;
                }
                if (fn_def &&
                    afw_utf8_equal_utf8_z(&fn_def->functionId->internal,
                        "wrap_literal_array"))
                {
                    parser->suppress_array_literal_wrap = true;
                }

                afw_compile_parse_Parameters(parser, args);
                parser->suppress_object_literal_wrap = saved_suppress_wrap;
                parser->suppress_array_literal_wrap = saved_suppress_array;
            }

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
 *#
 *# A parameter is a ParameterName or a list/object Pattern (same Patterns as
 *# let/const destructure). Pattern parameters introduce nested parameter
 *# symbols; the whole argument is bound via the Pattern at call time.
 *#
 * ParameterBinding ::=
 *    ParameterName | AssignmentListDestructureTarget |
 *    AssignmentObjectDestructureTarget
 *
 * RequiredParameterList ::=
 *    ParameterBinding OptionalType
 *    (',' ParameterBinding OptionalType)*
 *
 * OptionalParameterList ::=
 *    ( ParameterBinding '?'? OptionalType ( '=' Expression )? )
 *    (
 *        ','
 *        ( ParameterBinding '?'? OptionalType ( '=' Expression )? )
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
const afw_value_script_function_signature_t *
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

            /* If ellipsis, this is a rest parameter (name only for now). */
            afw_compile_get_token();
            if (afw_compile_token_is(ellipsis)) {
                param->is_rest = true;
            }
            else {
                afw_compile_reuse_token();
            }

            /*
             * Ensure parameter block exists before introducing any symbols
             * (simple name or Pattern leaves).
             */
            if (block && !*block) {
                *block = afw_compile_parse_link_new_value_block(parser,
                    start_offset);
                signature->block = *block;
            }
            else if (block && *block) {
                signature->block = *block;
            }

            /* Pattern parameter: [ … ] or { … } (not valid after ...rest). */
            afw_compile_get_token();
            if (!param->is_rest &&
                (afw_compile_token_is(open_bracket) ||
                    afw_compile_token_is(open_brace)))
            {
                afw_compile_reuse_token();
                if (!block) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Pattern parameters require a function body scope");
                }
                param->assignment_target =
                    afw_compile_parse_AssignmentTarget(parser,
                        afw_compile_assignment_type_parameter);
                param->name = NULL;
                param->symbol = NULL;
                symbol = NULL;
            }
            else {
                /* Simple ParameterName. */
                if (!afw_compile_token_is_unqualified_identifier()) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Expecting parameter name or Pattern");
                }
                param->name = parser->token->identifier_name;
                if (afw_compile_is_reserved_word(parser, param->name)) {
                    AFW_COMPILE_THROW_ERROR_Z(
                        "Parameter name can not be a reserved word");
                }

                symbol = NULL;
                if (block) {
                    symbol = afw_compile_parse_add_symbol_entry(
                        parser, param->name);
                    symbol->symbol_type =
                        afw_value_block_symbol_type_parameter;
                    param->symbol = symbol;
                }
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

            /*
             * Optional type: simple names and whole Pattern formals
             * (whole-arg type is stored for future compile-time check; leaves
             * may also carry types via Pattern binding syntax).
             */
            param->type = afw_compile_parse_OptionalType(parser, false);
            if (symbol && param->type) {
                afw_memory_copy(&symbol->type, param->type);
            }

            /* Push param on stack. */
            APR_ARRAY_PUSH(params, afw_value_script_function_parameter_t *) =
                param;

            /* Get next token. */
            afw_compile_get_token();

            /* Rest parameter must be last. */
            if (param->is_rest) {
                if (afw_compile_token_is(close_parenthesis)) {
                    break;
                }
                AFW_COMPILE_THROW_ERROR_Z("Expecting ')'");
            }

            /* Default: Expression (TS-like; was Literal-only). */
            else if (afw_compile_token_is(equal)) {
                param->default_value = afw_compile_parse_Expression(parser);
                param->is_optional = true;
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
const afw_value_t *
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

    /* Parse body with return type in scope for compile checks (issue #28). */
    {
        const afw_value_type_t *saved_returns;
        afw_boolean_t saved_break_allowed;
        afw_boolean_t saved_continue_allowed;
        afw_compile_loop_label_t *saved_loop_labels;

        saved_returns = parser->current_function_returns;
        parser->current_function_returns = signature->returns;
        saved_break_allowed = parser->break_allowed;
        saved_continue_allowed = parser->continue_allowed;
        saved_loop_labels = parser->loop_labels;
        parser->break_allowed = false;
        parser->continue_allowed = false;
        parser->loop_labels = NULL;

        afw_compile_get_token();
        if (afw_compile_token_is(open_brace)) {
            body = afw_compile_parse_StatementList(parser,
                NULL, true, false, false, false);
        }
        else {
            afw_compile_reuse_token();
            body = afw_compile_parse_Expression(parser);
            /* Expression body: compile-check against return type when known. */
            if (parser->current_function_returns &&
                AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED(
                    &parser->contextual, parser->xctx))
            {
                afw_value_type_check_compile_assignable(
                    parser->current_function_returns, body,
                    "return", &parser->contextual, parser->xctx);
            }
        }

        parser->current_function_returns = saved_returns;
        parser->break_allowed = saved_break_allowed;
        parser->continue_allowed = saved_continue_allowed;
        parser->loop_labels = saved_loop_labels;
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
const afw_value_t *
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
 * Parameters ::= '('
 *     ( ( Expression | ( '...' Expression ) )
 *       ( ',' ( Expression | ( '...' Expression ) ) )* )?
 *   ')'
 *
 *#
 *# Denotes a parameter list without first parameter (method style call).
 *# Call-site spread (...expr) is wrapped as list_expression so call
 *# evaluation can expand the array into separate arguments (issue #140).
 *#
 * ParametersExceptFirst ::= Parameters
 *
 *<<<ebnf*/
void
afw_compile_parse_Parameters(
    afw_compile_parser_t *parser,
    afw_compile_args_t *args)
{
    const afw_value_t *value;
    const afw_value_t *spread_internal;
    afw_size_t spread_offset;
    afw_boolean_t had_value;
    afw_boolean_t is_spread;

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

        is_spread = false;
        if (afw_compile_token_is(ellipsis)) {
            is_spread = true;
            afw_compile_save_offset(spread_offset);
        }
        else {
            afw_compile_reuse_token();
        }

        value = afw_compile_parse_Expression(parser);
        if (is_spread) {
            /*
             * Reuse list_expression as the call-site spread marker: evaluate
             * to an array, then call machinery expands elements into argv.
             */
            spread_internal = value;
            value = afw_value_create_array_expression(
                afw_compile_create_contextual_to_cursor(spread_offset),
                spread_internal, parser->p, parser->xctx);
        }
        afw_compile_args_add_value(args, value);

        had_value = true;
    }
}


/*ebnf>>>
 *
 * ParenthesizedExpression ::= '(' Expression ')' Parameters*
 *
 *<<<ebnf*/
const afw_value_t *
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
 * Script type expressions (issue #28). Hard cut: Adaptive Type forms such as
 * (array of T) / (object "OT") / meta {…} are not accepted. Grammar fragments
 * are in ebnf comment blocks on the impl_parse_* functions below.
 */

static afw_value_type_t *
impl_type_alloc(afw_compile_parser_t *parser)
{
    return afw_pool_calloc_type(parser->p, afw_value_type_t, parser->xctx);
}



static const afw_value_type_t *
impl_type_data_type(
    afw_compile_parser_t *parser,
    const afw_data_type_t *data_type)
{
    afw_value_type_t *type;

    type = impl_type_alloc(parser);
    type->kind = afw_value_type_kind_data_type;
    type->data_type = data_type;
    return type;
}



/*
 * Resolve TypeName: script-local type/interface, Adaptive data type id, or
 * unresolved reference (non-checking mode).
 */
static const afw_value_type_t *
impl_type_lookup_name(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name)
{
    const afw_value_type_t *type;
    const afw_data_type_t *data_type;
    afw_value_type_t *ref;

    if (parser->script_type_names) {
        type = apr_hash_get(parser->script_type_names, name->s, name->len);
        if (type) {
            return type;
        }
    }

    data_type = afw_environment_get_data_type(name, parser->xctx);
    if (data_type) {
        return impl_type_data_type(parser, data_type);
    }

    /* Unresolved name: keep as reference (non-checking mode). */
    ref = impl_type_alloc(parser);
    ref->kind = afw_value_type_kind_reference;
    ref->reference.name = name;
    ref->reference.resolved = NULL;
    return ref;
}



void
afw_compile_script_type_register(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name,
    const afw_value_type_t *type)
{
    if (!parser->script_type_names) {
        parser->script_type_names = apr_hash_make(parser->apr_p);
    }
    if (apr_hash_get(parser->script_type_names, name->s, name->len)) {
        AFW_COMPILE_THROW_ERROR_FZ(
            "Type or interface " AFW_UTF8_FMT_Q " is already defined",
            AFW_UTF8_FMT_ARG(name));
    }
    apr_hash_set(parser->script_type_names, name->s, name->len, type);
}






/*ebnf>>>
 *
 *# Property name in an object type literal (identifier or string).
 * ObjectTypePropertyName ::= PropertyName | String
 *
 * ObjectTypeLiteral ::=
 *     '{'
 *         (
 *             ObjectTypePropertyName '?'? ':' Type
 *             ( ',' ObjectTypePropertyName '?'? ':' Type )*
 *             ','?
 *         )?
 *     '}'
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_ObjectTypeLiteral(afw_compile_parser_t *parser)
{
    afw_value_type_t *type;
    afw_value_type_property_t *prop;
    afw_value_type_property_t *props_head;
    afw_value_type_property_t *props_tail;
    const afw_utf8_t *prop_name;
    afw_boolean_t optional;

    /* Token '{' already consumed or current. */
    if (!afw_compile_token_is(open_brace)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting '{'");
    }

    props_head = props_tail = NULL;
    afw_compile_get_token();
    if (!afw_compile_token_is(close_brace)) {
        for (;;) {
            if (afw_compile_token_is_unqualified_identifier()) {
                prop_name = parser->token->identifier_name;
            }
            else if (afw_compile_token_is(utf8_string)) {
                prop_name = parser->token->string;
            }
            else {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting property name in object type");
            }

            optional = false;
            afw_compile_get_token();
            if (afw_compile_token_is(question_mark)) {
                optional = true;
                afw_compile_get_token();
            }
            if (!afw_compile_token_is(colon)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting ':' after property name in object type");
            }

            prop = afw_pool_calloc_type(parser->p,
                afw_value_type_property_t, parser->xctx);
            prop->name = prop_name;
            prop->optional = optional;
            prop->type = afw_compile_parse_UnionType(parser);

            if (!props_head) {
                props_head = props_tail = prop;
            }
            else {
                props_tail->next = prop;
                props_tail = prop;
            }

            afw_compile_get_token();
            if (afw_compile_token_is(close_brace)) {
                break;
            }
            if (!afw_compile_token_is(comma)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting ',' or '}' in object type");
            }
            afw_compile_get_token();
            if (afw_compile_token_is(close_brace)) {
                break;
            }
        }
    }

    type = impl_type_alloc(parser);
    type->kind = afw_value_type_kind_object;
    type->object.properties = props_head;
    return type;
}



/*ebnf>>>
 *
 * TupleType ::= '[' ( Type ( ',' Type )* ','? )? ']'
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_TupleType(afw_compile_parser_t *parser)
{
    apr_array_header_t *elems;
    const afw_value_type_t **elements;
    afw_value_type_t *type;
    afw_size_t i;

    /* Current token is '['. Type / UnionType starts with get_token. */
    elems = apr_array_make(parser->apr_p, 4, sizeof(const afw_value_type_t *));
    afw_compile_get_token();
    if (!afw_compile_token_is(close_bracket)) {
        afw_compile_reuse_token();
        for (;;) {
            APR_ARRAY_PUSH(elems, const afw_value_type_t *) =
                afw_compile_parse_UnionType(parser);
            afw_compile_get_token();
            if (afw_compile_token_is(close_bracket)) {
                break;
            }
            if (!afw_compile_token_is(comma)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting ',' or ']' in tuple type");
            }
            afw_compile_get_token();
            if (afw_compile_token_is(close_bracket)) {
                break;
            }
            afw_compile_reuse_token();
        }
    }

    type = impl_type_alloc(parser);
    type->kind = afw_value_type_kind_tuple;
    type->tuple.count = (afw_size_t)elems->nelts;
    if (type->tuple.count > 0) {
        elements = afw_pool_malloc(parser->p,
            sizeof(afw_value_type_t *) * type->tuple.count, parser->xctx);
        for (i = 0; i < type->tuple.count; i++) {
            elements[i] =
                ((const afw_value_type_t **)elems->elts)[i];
        }
        type->tuple.elements = elements;
    }
    return type;
}



/*ebnf>>>
 *
 *# Called with '(' already the current token (see ParenthesizedOrFunctionType).
 * FunctionType ::=
 *     '(' FunctionTypeParameterList? ')' '=>' Type
 *
 * FunctionTypeParameterList ::=
 *     FunctionTypeParameter ( ',' FunctionTypeParameter )*
 *
 *# Named param: name ?: Type. Bare identifier without ':' is TypeName with
 *# optional '[]' only (not '|' / '&'). Non-identifier Type forms use full Type
 *# (object/tuple/paren).
 * FunctionTypeParameter ::=
 *     '...'? Identifier '?'? ':' Type |
 *     '...'? TypeName ( '[' ']' )* |
 *     '...'? (
 *         ObjectTypeLiteral |
 *         TupleType |
 *         ParenthesizedOrFunctionType
 *     )
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_FunctionType(afw_compile_parser_t *parser)
{
    afw_value_type_function_param_t *params_head;
    afw_value_type_function_param_t *params_tail;
    afw_value_type_function_param_t *param;
    afw_value_type_t *type;
    afw_boolean_t is_rest;
    afw_boolean_t optional;
    const afw_utf8_t *name;

    /* '(' already consumed. */
    params_head = params_tail = NULL;
    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        for (;;) {
            is_rest = false;
            optional = false;
            name = NULL;
            param = afw_pool_calloc_type(parser->p,
                afw_value_type_function_param_t, parser->xctx);

            if (afw_compile_token_is(ellipsis)) {
                is_rest = true;
                afw_compile_get_token();
            }

            /*
             * Named parameter: name ?: Type
             * Or bare Type (no name).
             */
            if (afw_compile_token_is_unqualified_identifier()) {
                name = parser->token->identifier_name;
                afw_compile_get_token();
                if (afw_compile_token_is(question_mark)) {
                    optional = true;
                    afw_compile_get_token();
                }
                if (afw_compile_token_is(colon)) {
                    param->name = name;
                    param->optional = optional;
                    param->is_rest = is_rest;
                    param->type = afw_compile_parse_UnionType(parser);
                }
                else {
                    /* Identifier was start of a type name, not a param name. */
                    afw_compile_reuse_token();
                    /* Restore: re-parse from the identifier as Type. */
                    /* We already consumed identifier — rebuild as type name. */
                    param->name = NULL;
                    param->optional = false;
                    param->is_rest = is_rest;
                    param->type = impl_type_lookup_name(parser, name);
                    /* Apply postfix [] / union is wrong here — only primary.
                     * For bare type params without composition, OK for v1;
                     * full Type after bare name needs reuse. Simpler path:
                     * if no ':' treat name as type and allow [] only via
                     * full expression by rewinding — use full Type parse
                     * by putting identifier back. */
                    {
                        afw_value_type_t *wrap;

                        /* Postfix arrays: name[] */
                        afw_compile_get_token();
                        while (afw_compile_token_is(open_bracket)) {
                            afw_compile_get_token();
                            if (!afw_compile_token_is(close_bracket)) {
                                AFW_COMPILE_THROW_ERROR_Z(
                                    "Expecting ']' in array type");
                            }
                            wrap = impl_type_alloc(parser);
                            wrap->kind = afw_value_type_kind_array;
                            wrap->array.element = param->type;
                            param->type = wrap;
                            afw_compile_get_token();
                        }
                        afw_compile_reuse_token();
                    }
                }
            }
            else {
                param->is_rest = is_rest;
                param->type = afw_compile_parse_UnionType(parser);
            }

            if (!params_head) {
                params_head = params_tail = param;
            }
            else {
                params_tail->next = param;
                params_tail = param;
            }

            afw_compile_get_token();
            if (afw_compile_token_is(close_parenthesis)) {
                break;
            }
            if (!afw_compile_token_is(comma)) {
                AFW_COMPILE_THROW_ERROR_Z(
                    "Expecting ',' or ')' in function type parameters");
            }
            afw_compile_get_token();
        }
    }

    afw_compile_get_token();
    if (!afw_compile_token_is(fat_arrow)) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Expecting '=>' in function type");
    }

    type = impl_type_alloc(parser);
    type->kind = afw_value_type_kind_function;
    type->function.parameters = params_head;
    type->function.returns = afw_compile_parse_UnionType(parser);
    return type;
}



/* Rewind to the '(' of this production and parse FunctionType. */
static inline const afw_value_type_t *
impl_parse_function_type_from_open(
    afw_compile_parser_t *parser,
    afw_size_t save_open)
{
    afw_compile_restore_cursor(save_open);
    afw_compile_get_token(); /* '(' */
    return afw_compile_parse_FunctionType(parser);
}


/*
 * Parse ( Type ) once. If '=>' follows the matching ')', this was a
 * FunctionType with one bare Type parameter — reparse as FunctionType.
 * One reparse per such '=>', not a full scan at every nested '('.
 */
static inline const afw_value_type_t *
impl_parse_paren_type_or_bare_function(
    afw_compile_parser_t *parser,
    afw_size_t save_open)
{
    const afw_value_type_t *inner;

    afw_compile_restore_cursor(save_open);
    afw_compile_get_token(); /* '(' */
    inner = afw_compile_parse_UnionType(parser);
    afw_compile_get_token();
    if (!afw_compile_token_is(close_parenthesis)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting ')' after type");
    }
    afw_compile_get_token();
    if (afw_compile_token_is(fat_arrow)) {
        return impl_parse_function_type_from_open(parser, save_open);
    }
    afw_compile_reuse_token();
    return inner;
}



/*ebnf>>>
 *
 *# Current token is '('. A few tokens of lookahead choose FunctionType
 *# (named param, rest, empty '()', or '=>' after '(TypeName)'). Otherwise
 *# parse ( Type ) once; if '=>' follows, reparse as FunctionType.
 * ParenthesizedOrFunctionType ::=
 *     FunctionType |
 *     '(' Type ')'
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_ParenthesizedOrFunctionType(afw_compile_parser_t *parser)
{
    afw_size_t save_open;
    const afw_utf8_t *name;

    /* Current token is '('. */
    save_open = parser->token->token_source_offset;
    afw_compile_get_token();

    /* () => T  or  (...rest: T) => T */
    if (afw_compile_token_is(close_parenthesis) ||
        afw_compile_token_is(ellipsis))
    {
        return impl_parse_function_type_from_open(parser, save_open);
    }

    if (afw_compile_token_is_unqualified_identifier()) {
        name = parser->token->identifier_name;
        afw_compile_get_token();
        /* Named / optional param, or a multi-parameter list. */
        if (afw_compile_token_is(question_mark) ||
            afw_compile_token_is(colon) ||
            afw_compile_token_is(comma))
        {
            return impl_parse_function_type_from_open(parser, save_open);
        }
        if (afw_compile_token_is(close_parenthesis)) {
            afw_compile_get_token();
            if (afw_compile_token_is(fat_arrow)) {
                return impl_parse_function_type_from_open(parser, save_open);
            }
            /* Parenthesized TypeName: (integer) */
            afw_compile_reuse_token();
            return impl_type_lookup_name(parser, name);
        }
        /* Identifier continues as a Type: (T | U), (T[]), … */
        return impl_parse_paren_type_or_bare_function(parser, save_open);
    }

    /* '{', '[', '(' or other Type start. */
    return impl_parse_paren_type_or_bare_function(parser, save_open);
}



/*ebnf>>>
 *
 *# Single identifier: resolves to DataType, registered type/interface, or
 *# unresolved reference (non-checking). TypeVariableName / InterfaceName are
 *# both Identifier; DataType is the closed set of Adaptive data type ids.
 * TypeName ::= DataType | Identifier
 *
 *# Array element types use postfix T[] only (not TypeScript Array<T>).
 * PrimaryType ::=
 *     ObjectTypeLiteral |
 *     TupleType |
 *     ParenthesizedOrFunctionType |
 *     TypeName
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_PrimaryType(afw_compile_parser_t *parser)
{
    const afw_utf8_t *name;

    afw_compile_get_token();

    if (afw_compile_token_is(open_brace)) {
        return afw_compile_parse_ObjectTypeLiteral(parser);
    }

    if (afw_compile_token_is(open_bracket)) {
        return afw_compile_parse_TupleType(parser);
    }

    if (afw_compile_token_is(open_parenthesis)) {
        return afw_compile_parse_ParenthesizedOrFunctionType(parser);
    }

    if (!afw_compile_token_is_unqualified_identifier()) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting Type");
    }

    name = parser->token->identifier_name;

    /* TypeName (includes data type id "array"; no Array<T> generic). */
    return impl_type_lookup_name(parser, name);
}



/*ebnf>>>
 *
 * ArrayType ::= PrimaryType ( '[' ']' )*
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_ArrayType(afw_compile_parser_t *parser)
{
    const afw_value_type_t *type;
    afw_value_type_t *arr;

    type = afw_compile_parse_PrimaryType(parser);
    for (;;) {
        afw_compile_get_token();
        if (!afw_compile_token_is(open_bracket)) {
            afw_compile_reuse_token();
            break;
        }
        afw_compile_get_token();
        if (!afw_compile_token_is(close_bracket)) {
            AFW_COMPILE_THROW_ERROR_Z(
                "Expecting ']' in array type (use [T, U] for tuples)");
        }
        arr = impl_type_alloc(parser);
        arr->kind = afw_value_type_kind_array;
        arr->array.element = type;
        type = arr;
    }
    return type;
}



/*ebnf>>>
 *
 * IntersectionType ::= ArrayType ( '&' ArrayType )*
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_IntersectionType(afw_compile_parser_t *parser)
{
    apr_array_header_t *members;
    const afw_value_type_t *type;
    const afw_value_type_t **list;
    afw_value_type_t *node;
    afw_size_t i;

    type = afw_compile_parse_ArrayType(parser);
    afw_compile_get_token();
    if (!afw_compile_token_is(ampersand)) {
        afw_compile_reuse_token();
        return type;
    }

    members = apr_array_make(parser->apr_p, 4, sizeof(const afw_value_type_t *));
    APR_ARRAY_PUSH(members, const afw_value_type_t *) = type;
    while (afw_compile_token_is(ampersand)) {
        APR_ARRAY_PUSH(members, const afw_value_type_t *) =
            afw_compile_parse_ArrayType(parser);
        afw_compile_get_token();
    }
    afw_compile_reuse_token();

    node = impl_type_alloc(parser);
    node->kind = afw_value_type_kind_intersection;
    node->compound.count = (afw_size_t)members->nelts;
    list = afw_pool_malloc(parser->p,
        sizeof(afw_value_type_t *) * node->compound.count, parser->xctx);
    for (i = 0; i < node->compound.count; i++) {
        list[i] = ((const afw_value_type_t **)members->elts)[i];
    }
    node->compound.members = list;
    return node;
}



/*ebnf>>>
 *
 * UnionType ::= IntersectionType ( '|' IntersectionType )*
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_UnionType(afw_compile_parser_t *parser)
{
    apr_array_header_t *members;
    const afw_value_type_t *type;
    const afw_value_type_t **list;
    afw_value_type_t *node;
    afw_size_t i;

    afw_compile_parse_nesting_enter(parser);
    type = afw_compile_parse_IntersectionType(parser);
    afw_compile_get_token();
    if (!afw_compile_token_is(vertical_bar)) {
        afw_compile_reuse_token();
        afw_compile_parse_nesting_leave(parser);
        return type;
    }

    members = apr_array_make(parser->apr_p, 4, sizeof(const afw_value_type_t *));
    APR_ARRAY_PUSH(members, const afw_value_type_t *) = type;
    while (afw_compile_token_is(vertical_bar)) {
        APR_ARRAY_PUSH(members, const afw_value_type_t *) =
            afw_compile_parse_IntersectionType(parser);
        afw_compile_get_token();
    }
    afw_compile_reuse_token();

    node = impl_type_alloc(parser);
    node->kind = afw_value_type_kind_union;
    node->compound.count = (afw_size_t)members->nelts;
    list = afw_pool_malloc(parser->p,
        sizeof(afw_value_type_t *) * node->compound.count, parser->xctx);
    for (i = 0; i < node->compound.count; i++) {
        list[i] = ((const afw_value_type_t **)members->elts)[i];
    }
    node->compound.members = list;
    afw_compile_parse_nesting_leave(parser);
    return node;
}



/*ebnf>>>
 *
 *# Script type expression (issue #28). Primary leaves are TypeName
 *# (DataType or script type/interface name). Hard cut: (array of T) /
 *# (object "OT") / meta {…} are not accepted.
 * Type ::= UnionType
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_Type(afw_compile_parser_t *parser)
{
    return afw_compile_parse_UnionType(parser);
}


/*ebnf>>>
 *
 *# If Type is not specified, default is any (afw_data_type_any).
 * OptionalType ::= ( ':' Type )?
 *
 * OptionalReturnType ::= ( ':' Type )?
 *
 *<<<ebnf*/
const afw_value_type_t *
afw_compile_parse_OptionalType(
    afw_compile_parser_t *parser,
    afw_boolean_t is_return)
{
    /* If next is ':', parse Type (including void as data type leaf). */
    afw_compile_get_token();
    if (afw_compile_token_is(colon)) {
        (void)is_return;
        return afw_compile_parse_UnionType(parser);
    }

    /* Missing annotation → any (error if noImplicitAny + type checking). */
    afw_compile_reuse_token();
    if (AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED(
            &parser->contextual, parser->xctx) &&
        AFW_VALUE_TYPE_CHECK_NO_IMPLICIT_ANY(
            &parser->contextual, parser->xctx))
    {
        AFW_COMPILE_THROW_ERROR_Z(
            "Type annotation required (compile:noImplicitAny)");
    }
    return impl_type_data_type(parser, afw_data_type_any);
}



/*ebnf>>>
 *
 * NullishCoalescing ::= LogicalExpression ( '??'  LogicalExpression )*
 *
 *<<<ebnf*/
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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
const afw_value_t *
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

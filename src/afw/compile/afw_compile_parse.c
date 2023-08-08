// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_compile_parse.c
 * @brief Adaptive Framework Compiler Parser.
 */

#include "afw_internal.h"



/*ebnf>>>
 *
 *# This grammar uses W3C (https://www.w3.org/) EBNF notation.
 *#
 *# Note: This grammar is intended as documentation and is not
 *# precise.
 *#
 *# Directory src/afw/doc/syntax/ must be produced manually from
 *# src/afw/generated/ebnf/syntax.ebnf using the following steps:
 *#
 *# 1) Visit http://bottlecaps.de/rr/ui in a web browser.
 *# 2) Optionally, go to the "Options" tab and change Color to one
 *#    compatible with the admin app such as #DCDCDC which is shades
 *#    of gray.
 *# 3) Paste the contents of src/afw/generated/syntax/compile.ebnf in
 *#    the text area of the "Edit Grammar" tab.
 *# 4) Select the "View Diagram" tab then click the PNG radio button
 *#    in "Download diagram" box in the top right corner and press
 *#    the "Download" button.
 *# 5) Replace the contents of the syntax/ directory with the contents
 *#    of the downloaded zip file.
 *#
 *# Root productions
 *#
 *# A parameter passed to the adaptive compiler determines the first
 *# production used for parse.
 *#
 *# Expression - afw_compile_type_expression
 *#
 *# Hybrid - afw_compile_type_hybrid
 *#
 *# Json - afw_compile_type_json and afw_compile_type_relaxed_json
 *#
 *# ParenthesizedExpression - afw_compile_type_parenthesized_expression
 *#
 *# Script - afw_compile_type_script
 *#
 *# Template - afw_compile_type_template
 *#
 *
 *<<<ebnf*/



AFW_DEFINE_INTERNAL(const afw_compile_value_contextual_t*)
afw_compile_create_contextual(
    afw_compile_parser_t* parser,
    afw_size_t start_offset,
    afw_size_t size)
{
    afw_compile_value_contextual_t* result;

    result = afw_pool_malloc_type(parser->p, afw_compile_value_contextual_t,
        parser->xctx);

    result->compiled_value = parser->compiled_value;
    result->source_location = parser->contextual.source_location;
    result->value_offset = start_offset;
    result->value_size = size;
    return result;
}



AFW_DEFINE_INTERNAL(const afw_utf8_t *)
afw_compile_create_source_location_impl(
    afw_compile_parser_t *parser,
    afw_size_t start_offset)
{
    const afw_utf8_t *result;
    afw_size_t line;
    afw_size_t column;
    afw_boolean_t has_nl;

    has_nl = afw_utf8_line_column_of_offset(
        &line, &column,
        parser->full_source, start_offset,
        4, parser->xctx);

    if (has_nl) {
        result = afw_utf8_printf(parser->p, parser->xctx,
            AFW_UTF8_FMT "[" AFW_SIZE_T_FMT ":" AFW_SIZE_T_FMT "]",
            AFW_UTF8_FMT_ARG(parser->contextual.source_location),
            line, column);
    }
    else {
        result = afw_utf8_printf(parser->p, parser->xctx,
            AFW_UTF8_FMT "+" AFW_SIZE_T_FMT,
            AFW_UTF8_FMT_ARG(parser->contextual.source_location),
            start_offset);
    }

    return result;
}



/* Reference create. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_reference_create(
    afw_compile_parser_t *parser,
    const afw_compile_value_contextual_t *contextual,
    const afw_utf8_t *identifier)
{
    const afw_value_t *result;
    afw_value_block_symbol_t *symbol;
    afw_utf8_t qualifier;
    afw_utf8_t name;

    result = NULL;

    afw_compile_split_qualified_name(parser->token->string,
        &qualifier, &name, parser->xctx);
    if (qualifier.len == 0) {
        symbol = afw_compile_parse_get_symbol_entry(parser, &name);
        if (symbol) {
            result = afw_value_variable_reference_create(
                contextual, symbol, parser->p, parser->xctx);
        }
    }
    if (!result) {
        result = (const afw_value_t *)
            afw_environment_get_qualified_function(
                afw_utf8_equal(&qualifier, &afw_s_fn) ? NULL : &qualifier,
                &name, parser->xctx);
        if (!result) {
            if (qualifier.len == 0) {
                AFW_COMPILE_THROW_ERROR_FZ(
                    "Undeclared variable " AFW_UTF8_FMT_Q,
                    AFW_UTF8_FMT_ARG(&name));
            }
            if (afw_utf8_equal(&qualifier, &afw_s_fn)) {
                AFW_COMPILE_THROW_ERROR_FZ(
                    "Unknown built-in function " AFW_UTF8_FMT_Q,
                    AFW_UTF8_FMT_ARG(&name));
            }
            result =
                afw_value_qualified_variable_reference_create(
                    contextual, &qualifier, &name,
                    parser->p, parser->xctx);
        }
    }

    return result;
}


/* Embellish error. */
AFW_DEFINE_INTERNAL(void)
afw_compile_parse_embellish_error(
    afw_compile_parser_t *parser,
    afw_error_t *error)
{
    afw_size_t line;
    afw_size_t column;

    /* If compile info already part of message, return. */
    if (error->parser_source) {
        return;
    }

    afw_utf8_line_column_of_offset(
        &line, &column,
        parser->full_source, parser->token->token_source_offset,
        4, parser->xctx);

    error->message_z = afw_utf8_z_printf(parser->p, parser->xctx,
        "Error during compile at offset "
        AFW_SIZE_T_FMT
        " around line "
        AFW_SIZE_T_FMT
        " column "
        AFW_SIZE_T_FMT
        ": %s",
        parser->token->token_source_offset, line, column,
        error->message_z);

    error->contextual = &parser->contextual;
    error->parser_cursor = parser->cursor;

    error->parser_source = parser->full_source;
}



/* Parse error. */
AFW_DEFINE_INTERNAL(void)
afw_compile_parse_set_error_z(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *message_z)
{
    afw_size_t line;
    afw_size_t column;

    afw_compile_set_contextual();
    afw_utf8_line_column_of_offset(
        &line, &column,
        parser->full_source, parser->token->token_source_offset,
        4, parser->xctx);

    afw_error_set_fz(afw_error_code_syntax,
        source_z, parser->xctx,
        "Parse error at offset "
        AFW_SIZE_T_FMT
        " around line "
        AFW_SIZE_T_FMT
        " column "
        AFW_SIZE_T_FMT
        ": %s",
        parser->token->token_source_offset, line, column, message_z);

    parser->xctx->error->parser_source = parser->full_source;
}



/* Parse error. */
AFW_DEFINE_INTERNAL(void)
afw_compile_parse_set_error_fz(
    afw_compile_parser_t *parser,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z, ...)
{
    va_list ap;
    const afw_utf8_z_t *message_z;

    va_start(ap, format_z);
    message_z = afw_utf8_z_printf_v(format_z, ap,
        parser->p, parser->xctx);
    va_end(ap);

    afw_compile_parse_set_error_z(parser, source_z, message_z);
}


AFW_DEFINE_INTERNAL(const afw_value_variable_reference_t *)
afw_compile_parse_variable_reference_create(
    afw_compile_parser_t *parser,
    const afw_compile_value_contextual_t *contextual,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_utf8_t *identifier,
    const afw_value_type_t *type)
{
    afw_value_block_symbol_t *symbol;

    if (assignment_type == afw_compile_assignment_type_let) {
        symbol = afw_compile_parse_add_symbol_entry(parser, identifier);
        if (type) {
            afw_memory_copy(&symbol->type, type);
        }
    }
    else if (assignment_type == afw_compile_assignment_type_const) {
        symbol = afw_compile_parse_add_symbol_entry(parser, identifier);
        if (type) {
            afw_memory_copy(&symbol->type, type);
        }
    }
    else if (assignment_type == afw_compile_assignment_type_reference_only) {
        symbol = afw_compile_parse_get_symbol_entry(parser, identifier);
        if (!symbol) {
            AFW_COMPILE_THROW_ERROR_FZ(
                "Variable " AFW_UTF8_FMT_Q " is not declared",
                AFW_UTF8_FMT_ARG(identifier));
        }
    }
    else {
        symbol = afw_compile_parse_get_symbol_entry(parser, identifier);
        if (!symbol) {
            AFW_COMPILE_THROW_ERROR_FZ(
                "Variable " AFW_UTF8_FMT_Q " is not declared",
                AFW_UTF8_FMT_ARG(identifier));
        }
    }

    return (const afw_value_variable_reference_t *)
        afw_value_variable_reference_create(
            contextual, symbol, parser->p, parser->xctx);
}



AFW_DEFINE_INTERNAL(afw_value_block_symbol_t *)
afw_compile_parse_get_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name)
{
    afw_value_block_t *block;
    afw_value_block_symbol_t *entry;

    for (block = parser->compiled_value->current_block;
        block;
        block = block->parent_block)
    {
        for (entry = block->first_entry; entry; entry = entry->next_entry)
        {
            if (afw_utf8_equal(entry->name, name)) {
                return entry;
            }
        }
    }

    return NULL;
}



AFW_DEFINE_INTERNAL(afw_value_block_symbol_t *)
afw_compile_parse_get_local_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name)
{
    afw_value_block_symbol_t *entry;

    for (entry = parser->compiled_value->current_block->first_entry;
        entry;
        entry = entry->next_entry)
    {
        if (afw_utf8_equal(entry->name, name)) {
            return entry;
        }
    }

    return entry;
}



AFW_DEFINE_INTERNAL(afw_value_block_symbol_t *)
afw_compile_parse_add_symbol_entry(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name)
{
    afw_value_block_symbol_t *entry;

    if (afw_compile_parse_get_local_symbol_entry(parser, name)) {
        AFW_COMPILE_THROW_ERROR_FZ(AFW_UTF8_FMT_Q " already defined",
            AFW_UTF8_FMT_ARG(name));
    }

    entry = afw_pool_calloc_type(parser->p,
        afw_value_block_symbol_t, parser->xctx);
    entry->name = name;
    entry->parent_block = parser->compiled_value->current_block;
    if (!parser->compiled_value->current_block->first_entry) {
        parser->compiled_value->current_block->first_entry = entry;
        parser->compiled_value->current_block->final_entry = entry;
    }
    else {
        entry->index = parser->compiled_value->current_block->entry_count;
        parser->compiled_value->current_block->final_entry->next_entry = entry;
        parser->compiled_value->current_block->final_entry = entry;
    }
    parser->compiled_value->current_block->entry_count++;

    return entry;
}



AFW_DEFINE_INTERNAL(const afw_value_block_t *)
afw_compile_parse_link_new_value_block(
    afw_compile_parser_t *parser,
    afw_size_t start_offset)
{
    const afw_value_block_t *block;
    const afw_compile_value_contextual_t *contextual;

    contextual = afw_compile_create_contextual_to_cursor(start_offset);

    block = afw_value_block_allocated_and_link(
        contextual,
        parser->compiled_value,
        &parser->block_count,
        parser->p, parser->xctx);

    return block;
}



AFW_DEFINE_INTERNAL(void)
afw_compile_parse_check_symbol(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    afw_size_t symbol_cursor)
{

}



/*ebnf>>>
 *
 *# Hybrid is used to parse data type hybrid.
 *#
 *# A hybrid is parsed as a Script, ExpressionTuple, Template, or an evaluated
 *# string as follows:
 *#
 *#     1) If it begins with a '#!', it is parsed as a Script.
 *#     2) If it begins with '[', it is parsed as an ExpressionTuple.
 *#     3) Otherwise, it is parsed as a Template.  Note that if the
 *#        template does not contain '${', it produces an evaluated string.
 *#
 *
 * Hybrid ::= Script | ExpressionTuple | Template
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Hybrid(afw_compile_parser_t *parser)
{
    afw_code_point_t cp;
    afw_utf8_t line;

    /* Note: if full_source_type is hybrid (top call), make it more specific. */

    /* If starts with shebang, this is an adaptive script. */
    if (afw_compile_next_raw_starts_with_z("#!")) {
        afw_compile_get_raw_line(&line);
        if (!afw_utf8_contains(&line, &afw_s_afw) &&
            !afw_utf8_contains(&line, &afw_s_maluba))
        {
            AFW_COMPILE_THROW_ERROR_Z(
                "Shebang line must contain 'afw' or 'maluba' to be "
                "recognized as an adaptive script in a hybrid value");
        }

        if (parser->compiled_value->full_source_type == &afw_s_hybrid) {
            parser->compiled_value->full_source_type = &afw_s_script;
        }

        return afw_compile_parse_StatementList(parser,
            NULL, false, false, true);
    }

    /* Get first codepoint. */
    cp = afw_compile_peek_code_point(parser);

    /* If no input, return empty string. */
    if (cp < 0) {
        if (parser->compiled_value->full_source_type == &afw_s_hybrid) {
            parser->compiled_value->full_source_type = &afw_s_string;
        }
        return afw_value_empty_string;
    }

    /* If this starts with '[', parse as ExpressionTuple. */
    if (cp == '[') {
        if (parser->compiled_value->full_source_type == &afw_s_hybrid) {
            parser->compiled_value->full_source_type = &afw_s_expression_tuple;
        }
        return afw_compile_parse_ExpressionTuple(parser);
    }


    /*
     * Otherwise, return parser template.  Template is always one line, so
     * force residual check to none and callback to NULL.
     */
    parser->residual_check = afw_compile_residual_check_none;
    parser->callback = NULL;
    if (parser->compiled_value->full_source_type == &afw_s_hybrid) {
        parser->compiled_value->full_source_type = &afw_s_template;
    }
    return afw_compile_parse_Template(parser);
}

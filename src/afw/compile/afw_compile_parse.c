// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Compiler Parser
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_compile_parse.c
 * @brief Shared Adaptive Script/compiler parse entry helpers.
 */

#include "afw_internal.h"


void
afw_compile_parse_nesting_enter(afw_compile_parser_t *parser)
{
    AFW_XCTX_THROW_IF_TERMINATING(parser->xctx);
    parser->parse_nesting++;
    if (parser->parse_nesting > AFW_COMPILE_PARSE_NESTING_MAX) {
        AFW_COMPILE_THROW_ERROR_Z(
            "Type or pattern nesting is too deep");
    }
}


void
afw_compile_parse_nesting_leave(afw_compile_parser_t *parser)
{
    if (parser->parse_nesting > 0) {
        parser->parse_nesting--;
    }
}



/*ebnf>>>
 *
 *# This grammar uses W3C (https://www.w3.org/) EBNF notation.
 *#
 *# Note: This grammar is intended as documentation and is not
 *# precise. The C parser remains authoritative if docs and code
 *# disagree.
 *#
 *# Dual use of the harvested file
 *# src/afw/generated/ebnf/syntax.ebnf (do not hand-edit; produced by
 *# afwdev generate from these ebnf comments):
 *#
 *# A) Automated handbook / docs build (usual path)
 *#    afwdev build --docs / --fulldev runs Railroad Diagram Generator
 *#    (rr.war; same engine as https://bottlecaps.de/rr) with
 *#    -color:#DCDCDC, unpacks PNG + index.html under
 *#    build/docs/.../reference/language/ebnf/syntax/, and themes that
 *#    page for the Docs site. Handbook XML embeds PNGs from
 *#    the diagram directory (e.g. statements.html).
 *#
 *# B) Manual exploration (optional)
 *#    Paste syntax.ebnf into https://bottlecaps.de/rr/ui yourself if you
 *#    want alternate colors, SVG, or a one-off diagram:
 *#    1) Open https://bottlecaps.de/rr/ui
 *#    2) Options tab: set Color if desired (docs default is #DCDCDC)
 *#    3) Edit Grammar: paste src/afw/generated/ebnf/syntax.ebnf
 *#    4) View Diagram: download PNG/SVG as needed
 *#    You do not need to copy results back into the tree for the
 *#    published handbook unless you are changing the automated path.
 *#
 *# Root productions
 *#
 *# A parameter passed to the adaptive compiler determines the first
 *# production used for parse.
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



const afw_compile_value_contextual_t*
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



const afw_utf8_t *
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
const afw_value_t *
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
            result = afw_value_symbol_reference_create(
                contextual, symbol, parser->p, parser->xctx);
        }
    }
    if (!result) {
        result = (const afw_value_t *)
            afw_environment_get_qualified_function(
                afw_utf8_equal(&qualifier, afw_s_fn) ? NULL : &qualifier,
                &name, parser->xctx);
        if (!result) {
            if (qualifier.len == 0) {
                AFW_COMPILE_THROW_ERROR_FZ(
                    "Undeclared variable " AFW_UTF8_FMT_Q,
                    AFW_UTF8_FMT_ARG(&name));
            }
            if (afw_utf8_equal(&qualifier, afw_s_fn)) {
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
void
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
void
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
void
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


const afw_value_symbol_reference_t *
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
        symbol->symbol_type = afw_value_block_symbol_type_let;
    }
    else if (assignment_type == afw_compile_assignment_type_const) {
        symbol = afw_compile_parse_add_symbol_entry(parser, identifier);
        if (type) {
            afw_memory_copy(&symbol->type, type);
        }
        symbol->symbol_type = afw_value_block_symbol_type_const;
    }
    else if (assignment_type == afw_compile_assignment_type_parameter) {
        symbol = afw_compile_parse_add_symbol_entry(parser, identifier);
        if (type) {
            afw_memory_copy(&symbol->type, type);
        }
        symbol->symbol_type = afw_value_block_symbol_type_parameter;
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

    return (const afw_value_symbol_reference_t *)
        afw_value_symbol_reference_create(
            contextual, symbol, parser->p, parser->xctx);
}



afw_value_block_symbol_t *
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



afw_value_block_symbol_t *
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



afw_value_block_symbol_t *
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
        entry->index = parser->compiled_value->current_block->symbol_count;
        parser->compiled_value->current_block->final_entry->next_entry = entry;
        parser->compiled_value->current_block->final_entry = entry;
    }
    parser->compiled_value->current_block->symbol_count++;

    return entry;
}



const afw_value_block_t *
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



void
afw_compile_parse_check_symbol(
    afw_compile_parser_t *parser,
    const afw_utf8_t *name,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    afw_size_t symbol_cursor)
{

}

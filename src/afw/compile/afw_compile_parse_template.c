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
 * Substitution ::= '${' Script '}'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Substitution(afw_compile_parser_t *parser)
{
    const afw_value_t *result;

    afw_compile_get_token();

    if (afw_compile_token_is(substitute_start)) {
        result = afw_compile_parse_Script(parser, true);
    }

    else {
        AFW_COMPILE_THROW_ERROR_Z("Internal error");
    }

    /* Return result. */
    return result;
}



/*ebnf>>>
 *
 * Template ::=
 *    (
 *        (Char - '$') | ( '\$' ) | '$' (Char - '{') | Substitution
 *    )*
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_Template(afw_compile_parser_t *parser)
{
    const afw_utf8_t *string;
    apr_array_header_t *values;
    afw_size_t start_offset;
    afw_size_t string_cursor;
    afw_size_t previous_cursor;
    afw_code_point_t cp;
    afw_boolean_t substitution;
    afw_size_t len;
    afw_utf8_octet_t *s;

    values = apr_array_make(parser->apr_p, 5, sizeof(afw_value_t *));

    /* Loop parsing template. */
    afw_compile_save_cursor(start_offset);
    afw_compile_save_cursor(string_cursor);
    for (substitution = false;;) {

        /* Save cursor and get next code point. */
        afw_compile_save_cursor(previous_cursor);
        cp = afw_compile_get_code_point();

        /* If '\$' change to '$'. */
        if (cp == '\\') {
            cp = afw_compile_get_code_point();
            if (cp == '$') {
                len = afw_compile_source_buffer_length_from(string_cursor) - 1;
                s = afw_pool_malloc(parser->p, len, parser->xctx);
                s[len - 1] = '$';
                if (len > 1) {
                    memcpy(s,
                        afw_compile_source_buffer_at(string_cursor),
                        len - 1);
                }
                string = afw_utf8_create(s, len, parser->p, parser->xctx);
                APR_ARRAY_PUSH(values, const afw_value_t *) =
                    afw_value_create_string(string, parser->p, parser->xctx);
                afw_compile_save_cursor(string_cursor);
                continue;
            }
            afw_compile_restore_cursor(previous_cursor);
            cp = afw_compile_get_code_point();
        }

        /* If '${', indicate substitution and restore cursor. */
        if (cp == '$') {
            cp = afw_compile_get_code_point();
            if (cp == '{') {
                afw_compile_restore_cursor(previous_cursor);
                substitution = true;
            }
        }

        /* If substitution or end and string is not empty, push string value. */
        if ((substitution || cp < 0) &&
            !afw_compile_cursor_equal(string_cursor))
        {
            string = afw_utf8_create_copy(
                afw_compile_source_buffer_at(string_cursor),
                afw_compile_source_buffer_length_from(string_cursor),
                parser->p, parser->xctx);
            APR_ARRAY_PUSH(values, const afw_value_t *) =
                afw_value_create_string(string, parser->p, parser->xctx);
        }

        /* If end, break out of loop. */
        if (cp < 0) break;

        /* If substitution, parse substitution and push value. */
        if (substitution) {
            APR_ARRAY_PUSH(values, const afw_value_t *) =
                afw_compile_parse_Substitution(parser);
            substitution = false;
            afw_compile_save_cursor(string_cursor);
        }
    }

    /* If no elements, return empty string. */
    if (values->nelts == 0) {
        return afw_value_empty_string;
    }

    /* If only one element and it is a string, return string value. */
    if (values->nelts == 1 &&
        afw_value_is_string(*((afw_value_t * *)values->elts)))
    {
        return *((afw_value_t * *)values->elts);
    }

    /* Return template value. */
    return afw_value_template_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        values->nelts,
        (const afw_value_t * const *)values->elts,
        parser->p, parser->xctx);
}



/*ebnf>>>
 *
 * TemplateString ::= '`' + Template + '`'
 *
 *<<<ebnf*/
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_compile_parse_TemplateString(afw_compile_parser_t *parser)
{
    const afw_utf8_t *string;
    apr_array_header_t *values;
    afw_size_t start_offset;
    afw_size_t previous_cursor;
    afw_size_t previous_cursor2;
    afw_code_point_t cp, cp2;
    afw_boolean_t substitution;

    afw_compile_get_token_and_save_offset(start_offset);

    if (!afw_compile_token_is(grave)) {
        AFW_COMPILE_THROW_ERROR_Z("Expecting template string");
    }

    values = apr_array_make(parser->apr_p, 5, sizeof(afw_value_t *));

    /* Clear array used for building string. */
    apr_array_clear(parser->s);

    /* Loop parsing template. */
    afw_compile_save_cursor(start_offset);
    for (substitution = false;;) {

        /* Save cursor and get next code point. */
        afw_compile_save_cursor(previous_cursor);
        cp = afw_compile_get_unescaped_code_point();

        /* If '${', indicate substitution and restore cursor. */
        if (cp == '$') {
            afw_compile_save_cursor(previous_cursor2);
            cp2 = afw_compile_get_code_point();
            if (cp2 == '{') {
                afw_compile_restore_cursor(previous_cursor);
                substitution = true;
            }
            else {
                afw_compile_restore_cursor(previous_cursor2);
                afw_compile_internal_s_push_code_point(parser, cp);
            }
        }
        else if (cp < 0) {
            AFW_COMPILE_THROW_ERROR_Z("Expecting grave symbol ('`')");
        }
        else if (cp != '`') {
            afw_compile_internal_s_push_code_point(parser, cp);
        }

        /* If substitution or end and string is not empty, push string value. */
        if ((substitution || cp == '`') && parser->s->nelts > 0)
        {
            string = afw_utf8_create_copy(
                (afw_utf8_octet_t *)parser->s->elts, parser->s->nelts,
                parser->p, parser->xctx);
            APR_ARRAY_PUSH(values, const afw_value_t *) =
                afw_value_create_string(string, parser->p, parser->xctx);
            apr_array_clear(parser->s);
        }

        /* Finished if grave symbol. */
        if (cp == '`') {
            break;
        };

        /* If substitution, parse substitution and push value. */
        if (substitution) {
            APR_ARRAY_PUSH(values, const afw_value_t *) =
                afw_compile_parse_Substitution(parser);
            substitution = false;
            apr_array_clear(parser->s);
        }
    }

    /* If no elements, return empty string. */
    if (values->nelts == 0) {
        return afw_value_empty_string;
    }

    /* If only one element and it is a string, return string value. */
    if (values->nelts == 1 &&
        afw_value_is_string(*((afw_value_t * *)values->elts)))
    {
        return *((afw_value_t * *)values->elts);
    }

    /* Return template value. */
    return afw_value_template_definition_create(
        afw_compile_create_contextual_to_cursor(start_offset),
        values->nelts,
        (const afw_value_t * const *)values->elts,
        parser->p, parser->xctx);
}



/*ebnf>>>
 *
 * NamedTemplateString ::= '`' + ((Char - '$') | ('\$') | ('$' (Char - '{')) | '${' PropertyName '}') * '`'
 *
 *<<<ebnf*/

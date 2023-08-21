// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW parse functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_compile.c
 * @brief AFW parse functions.
 *
 */

#include "afw_internal.h"

#define XX(name, data_type, compile_function, description)                      \
                                                                                \
static const afw_utf8_t                                                         \
impl_compile_type_ ## name ## _name =                                           \
    AFW_UTF8_LITERAL(#name);                                                    \
                                                                                \
static const afw_value_string_t                                                 \
impl_compile_type_ ## name ## _name_value =                                     \
    {&afw_value_evaluated_string_inf, AFW_UTF8_LITERAL(#name)};                 \
                                                                                \
static const afw_utf8_t                                                         \
impl_compile_type_ ## name ## _description =                                    \
    AFW_UTF8_LITERAL(#description);                                             \
                                                                                \
static const afw_value_string_t                                                 \
impl_compile_type_ ## name ## _description_value =                              \
    {&afw_value_evaluated_string_inf, AFW_UTF8_LITERAL(#description)};          \

AFW_COMPILE_TYPE_MAP(XX)
    
#undef XX



static const afw_compile_type_info_t
impl_compile_type_pneumonic[] = {
#define XX(name, data_type, compile_function, description)                      \
{                                                                               \
    afw_compile_type_ ## name,                                                  \
    &impl_compile_type_ ## name ## _name,                                       \
    (const afw_value_t *)&impl_compile_type_ ## name ## _name_value,            \
    &impl_compile_type_ ## name ## _description,                                \
    (const afw_value_t *)&impl_compile_type_ ## name ## _description_value,     \
    &afw_data_type_ ## data_type ##_direct,                                     \
    compile_function                                                            \
},                                                                              \

    AFW_COMPILE_TYPE_MAP(XX)
#undef XX
{
    afw_compile_type_error,
    NULL, NULL, NULL, NULL, NULL, NULL
}
};



/* Compile and evaluate a string. */
AFW_DEFINE(const afw_value_t *)
afw_compile_and_evaluate(
    const afw_utf8_t *string,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *compiled_value;
    const afw_value_t *result;

    /** @todo more needs to be thought about for memory usage. Only
     * result needs to be saved, but anything "done" in the script
     * needs to remain "done".  For instance, if a service is started,
     * its pool should be available. This might solve itself with
     * new memory management.
     */
    compiled_value = afw_compile_to_value_with_callback(
        string, NULL, NULL,
        source_location, compile_type,
        afw_compile_residual_check_to_full, \
        NULL, NULL, p, xctx);

    result = afw_value_evaluate(compiled_value, p, xctx);

    return result;
}



/* Return afw_compile_type_t for a pneumonic. */
AFW_DEFINE(const afw_compile_type_info_t *)
afw_compile_type_get_info_by_pneumonic(
    const afw_utf8_t *pneumonic,
    const afw_xctx_t *xctx)
{
    afw_compile_type_t compile_type;

    for (compile_type = afw_compile_type_error;
        compile_type < afw_compile_type_count &&
        !afw_utf8_equal(pneumonic,
            impl_compile_type_pneumonic[compile_type].name);
        compile_type++);

    if (compile_type >= afw_compile_type_count) {
        compile_type = afw_compile_type_error;
    }

    return &impl_compile_type_pneumonic[compile_type];
}



/* Return pneumonic for a afw_compile_type_t. */
AFW_DEFINE(const afw_compile_type_info_t *)
afw_compile_type_get_info(
    afw_compile_type_t compile_type,
    const afw_xctx_t *xctx)
{
    const afw_compile_type_info_t *result;
    result = &impl_compile_type_pneumonic[afw_compile_type_error];
    if (compile_type > 0 &&
        compile_type < afw_compile_type_count)
    {
        result = &impl_compile_type_pneumonic[compile_type];
    }
    return result;    
}



/*
 * Parse string to adaptive value with callback.
 */
AFW_DEFINE(const afw_value_t *)
afw_compile_to_value_with_callback(
    const afw_utf8_t *string,
    afw_utf8_octet_get_cb_t callback,
    void *callback_data,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    afw_compile_residual_check_t residual_check,
    const afw_value_compiled_value_t *parent,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_compile_parser_t *parser;
    const afw_value_t *result = NULL;
    const afw_value_t * *interim  = NULL;
    const afw_utf8_octet_t *line_begin;
    const afw_utf8_octet_t *cursor;
    const afw_value_block_t *block;
    const afw_value_t **block_argv;
    afw_size_t count;

    if (compile_type == afw_compile_type_regexp) {
        AFW_THROW_ERROR_Z(general,
            "afw_compile_type_regexp not supported yet",
            xctx);
    }
    else  if (compile_type == afw_compile_type_xpathExpression) {
        AFW_THROW_ERROR_Z(general,
            "afw_compile_type_xpathExpression not supported yet",
            xctx);
    }

    /* Create parser. */
    parser = afw_compile_lexical_parser_create(
        string, callback, callback_data, source_location,
        compile_type, residual_check, false, parent, shared, p, xctx);

    /* Parse. */
    AFW_TRY{

        if (compile_type == afw_compile_type_json ||
            compile_type == afw_compile_type_relaxed_json)
        {
            parser->compiled_value->full_source_type = &afw_s_json;
            result = afw_compile_parse_Json(parser);
            afw_compile_check_for_residual(parser);
        }

        else {

            /* Result is a compiled value. */
            parser->compiled_value->reference_id =
                afw_uuid_create_utf8(parser->p, xctx);
            result = (const afw_value_t *)parser->compiled_value;
            interim = &parser->compiled_value->root_value;

            /*
             * There is no parent and this is not a script, surround root value
             * with a block.
             */
            block = NULL;
            if (!parent && compile_type != afw_compile_type_script)
            {
                block_argv = afw_pool_malloc_type(parser->p,
                    const afw_value_t *, xctx);
                interim = &block_argv[0];
                block = afw_compile_parse_link_new_value_block(parser, 0);
            }

            /* Process based on compile option. */
            if (compile_type == afw_compile_type_script) {
                parser->compiled_value->full_source_type = &afw_s_script;
                *interim = afw_compile_parse_Script(parser);
            }

            else if (compile_type == afw_compile_type_template) {
                parser->compiled_value->full_source_type = &afw_s_template;
                *interim = afw_compile_parse_Template(parser);
            }

            else if (compile_type == afw_compile_type_expression) {
                parser->compiled_value->full_source_type = &afw_s_expression;
                *interim = afw_compile_parse_Expression(parser);
            }

            else if (compile_type == afw_compile_type_parenthesized_expression) {
                parser->compiled_value->full_source_type = &afw_s_expression;
                afw_compile_skip_ws(parser);
                if (afw_compile_is_at_eof()) {
                    result = NULL;
                    break;
                }
                *interim = afw_compile_parse_ParenthesizedExpression(parser);
            }

            else if (compile_type == afw_compile_type_hybrid) {
                parser->compiled_value->full_source_type = &afw_s_hybrid;
                afw_compile_skip_ws(parser);
                *interim = afw_compile_parse_Hybrid(parser);
            }

            else if (compile_type == afw_compile_type_test_script) {
                parser->compiled_value->full_source_type = &afw_s_test_script;
                afw_compile_skip_ws(parser);
                *interim = afw_compile_parse_TestScript(parser);
            }

            else {
                AFW_THROW_ERROR_Z(general, "Invalid compile type.", xctx);
            }
            
            /* If surrounding with a block, finalize it. */
            if (block) {
                parser->compiled_value->root_value =
                    afw_value_block_finalize(block, 1, block_argv, xctx);
                afw_compile_parse_pop_value_block(parser);
            }

            /* Get clone of full source in correct pool. */
            if (parser->full_source)
            {
                parser->compiled_value->full_source = afw_utf8_clone(
                    parser->full_source, parser->p, xctx);
                for (
                    count = 0,
                    cursor = line_begin = parser->full_source->s;
                    count < parser->full_source->len; count++, cursor++)
                {
                    if (*cursor == '\n')
                    {
                        (parser->compiled_value->line_count)++;
                        if ((afw_size_t)(cursor - line_begin) >
                            parser->compiled_value->longest_line)
                        {
                            parser->compiled_value->longest_line =
                                cursor - line_begin;
                        }
                        line_begin = cursor + 1;
                    }
                }
            }

            afw_compile_check_for_residual(parser);
        }

    }
    AFW_CATCH_UNHANDLED {
        afw_compile_parse_embellish_error(parser, &this_THROWN_ERROR);
        AFW_ERROR_RETHROW;
    }
    AFW_FINALLY {
        afw_compile_lexical_parser_finish_and_release(parser, xctx);
    }
    AFW_ENDTRY;

    /* Return result. */
    return result;
}


/*
 * Implementation of method raw_to_object of interface afw_content_type.
 */
AFW_DEFINE(const afw_object_t *)
afw_compile_to_object(
    const afw_utf8_t *string,
    const afw_utf8_t *source_location,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_compile_parser_t *parser;
    const afw_object_t *result = NULL;
    const afw_value_t *value;
    const afw_pool_t *parser_p;

    /* Create parser. */
    parser_p = (cede_p) ? p : afw_pool_create(p, xctx);

    if ((adaptor_id || object_type_id || object_id) &&
        (!adaptor_id || !object_type_id || !object_id))
    {
        AFW_THROW_ERROR_Z(general,
            "If adaptor_id, object_type_id, or object_id is not NULL, all must "
            "not be NULL",
            xctx);
    }

    /* Default source location to path if adaptor_id supplied. */
    if (!source_location && adaptor_id) {
        source_location = afw_object_path_make(
            adaptor_id, object_type_id, object_id, parser_p, xctx);
    }

    /* Create parser. */
    parser = afw_compile_lexical_parser_create(
        string, NULL, NULL, source_location,
        afw_compile_type_json, afw_compile_residual_check_to_full,
        true, NULL, NULL, parser_p, xctx);
    parser->compiled_value->full_source_type = &afw_s_json;

    /* Parse. */
    AFW_TRY {
        value = afw_compile_parse_Object(parser, false, false);
        afw_compile_check_for_residual(parser);
        if (value) {
            result = afw_value_as_object(value, xctx);
            if (adaptor_id) {
                afw_object_meta_set_ids(result,
                    adaptor_id, object_type_id, object_id, xctx);
            }
        }
    }
    AFW_CATCH_UNHANDLED {
        afw_compile_parse_embellish_error(parser, &this_THROWN_ERROR);
        AFW_ERROR_RETHROW;
    }
    AFW_FINALLY {
        afw_compile_lexical_parser_finish_and_release(parser, xctx);
    }
    AFW_ENDTRY;

    /* Return result. */
    return result;
}


/* Compile hybrid. */
AFW_DEFINE(const afw_value_t *)
afw_compile_hybrid(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_value_compiled_value_t *parent,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *source;
    const afw_value_t *result;

    //FIXME
    if (afw_value_is_object(value)) {
        return value;
    }

    source = afw_value_as_utf8(value, p, xctx);
    result = afw_compile_hybrid_source(source,
        source_location, parent, shared, p, xctx);
    return result;
}



/* Compile object's expressions and templates properties. */
AFW_DEFINE(void)
afw_compile_expressions_templates_and_hybrids(
    const afw_object_t *object,
    const afw_utf8_t *source_location,
    afw_boolean_t recursive,
    const afw_compile_shared_t *shared,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_utf8_t *detail_source_location;
    const afw_value_t *value;

    /* Default source_location is object path. */
    if (!source_location) {
        source_location = afw_object_meta_get_path(object, xctx);
    }

    /* If shared is NULL, make one. */
    if (!shared) {
        shared = afw_compile_shared_create(object->p, xctx);
    }

    /* Compile single expressions and templates. */
    for (iterator = NULL;;) {

        /* Get the next property. */
        value = afw_object_get_next_property(object,
                &iterator,  &property_name, xctx);
        if (!value) break;

        /* If this is an expression value, compile it. */
        if (afw_value_is_expression(value)) {
            detail_source_location = afw_utf8_printf(object->p, xctx,
                AFW_UTF8_FMT "/" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(property_name));
            value = afw_compile_to_value(
                &((afw_value_expression_t *)value)->internal,
                detail_source_location,
                afw_compile_type_expression,
                NULL, shared, NULL, xctx);
            afw_object_set_property(object, property_name, value, xctx);
        }

        /* If this is a template value, compile it. */
        else if (afw_value_is_template(value)) {
            detail_source_location = afw_utf8_printf(object->p, xctx,
                AFW_UTF8_FMT "/" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(property_name));
            value = afw_compile_to_value(
                &((afw_value_template_t *)value)->internal,
                detail_source_location,
                afw_compile_type_template,
                NULL, shared, NULL, xctx);
            afw_object_set_property(object, property_name, value, xctx);
        }

        /* If this is a hybrid value, compile it. */
        else if (afw_value_is_hybrid(value)) {
            detail_source_location = afw_utf8_printf(object->p, xctx,
                AFW_UTF8_FMT "/" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(property_name));
            value = afw_compile_hybrid(
                value, detail_source_location,
                NULL, shared, NULL, xctx);
            afw_object_set_property(object, property_name, value, xctx);
        }

        /* Recursively process objects. */
        else if (recursive && afw_value_is_object(value)) {
            detail_source_location = afw_utf8_printf(object->p, xctx,
                AFW_UTF8_FMT "/" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(property_name));
            afw_compile_expressions_templates_and_hybrids(
                ((const afw_value_object_t *)value)->internal,
                detail_source_location, true, shared, xctx);
        }

        /** @fixme Support bags and lists? */
    }
}


/* Compile an object with all hybrid properties. */
AFW_DEFINE(const afw_object_t *)
afw_compile_object_all_hybrid_properties(
    const afw_object_t *object,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_utf8_t *property_name;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_value_t *compiled_value;
    const afw_utf8_t *detail_source_location;
    const afw_data_type_t *value_data_type;

    if (!source_location) {
        source_location = afw_object_meta_get_path(object, xctx);
    }
    if (!source_location) {
        source_location = &afw_s_a_empty_string;
    }
    result = afw_object_create(p, xctx);
    iterator = NULL;
    while ((value = afw_object_get_next_property(
        object, &iterator, &property_name, xctx)))
    {
        detail_source_location = afw_utf8_printf(object->p, xctx,
                AFW_UTF8_FMT " property " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(property_name));
        value_data_type = afw_value_get_data_type(value, xctx);
        if (!value_data_type ||
            !afw_utf8_equal(&value_data_type->cType, &afw_s_afw_utf8_t))
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_FMT " is not a hybrid",
                AFW_UTF8_FMT_ARG(detail_source_location));
        }
        compiled_value = afw_compile_hybrid_source(
            &((const afw_value_string_t *)value)->internal,
            detail_source_location, NULL, shared, p, xctx);
        afw_object_set_property(result, property_name, compiled_value, xctx);
    }  
    return result;
}


/* Split name with optional qualifier. */
AFW_DEFINE(void)
afw_compile_split_qualified_name(
    const afw_utf8_t *qualified_name,
    afw_utf8_t *qualifier,
    afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    afw_size_t len;

    afw_memory_clear(qualifier);
    afw_memory_copy(name, qualified_name);

    for (len = qualified_name->len, s = qualified_name->s; len > 0; len--, s++)
    {
        if (*s == ':') {
            if (len < 2 || *(s + 1) != ':') {
                AFW_THROW_ERROR_Z(general, "invalid qualified name", xctx);
            }
            qualifier->len = s - qualified_name->s;
            qualifier->s = qualified_name->s;
            name->len = len - 2;
            name->s = s + 2;
            break;
        }
    }
}


/* Return the source location of a value. */
AFW_DECLARE(const afw_utf8_t *)
afw_compile_source_location_of_value(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_info_t info;
    const afw_utf8_t *result = NULL;
    afw_size_t line_number;
    afw_size_t column_number;

    afw_value_get_info(value, &info, p, xctx);
    if (info.contextual) {
        if (info.contextual->compiled_value)
        {
            afw_utf8_line_column_of_offset(
                &line_number, &column_number,
                info.contextual->compiled_value->full_source,
                info.contextual->value_offset,
                4, xctx);
            result = afw_utf8_printf(p, xctx,
                AFW_UTF8_FMT
                "+" AFW_SIZE_T_FMT
                "(" AFW_SIZE_T_FMT
                ":" AFW_SIZE_T_FMT ")",
                AFW_UTF8_FMT_ARG(info.contextual->source_location),
                info.contextual->value_offset,
                line_number, column_number);
        }
        else if (info.contextual->source_location)
        {
            if (info.contextual->value_offset == 0) {
                result = afw_utf8_printf(p, xctx,
                    AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(info.contextual->source_location));
            }
            else {
                result = afw_utf8_printf(p, xctx,
                    AFW_UTF8_FMT
                    "%s+" AFW_SIZE_T_FMT,
                    AFW_UTF8_FMT_ARG(info.contextual->source_location),
                    info.contextual->value_offset);
            }
        }
        else {
            if (info.contextual->value_offset == 0) {
                result = &afw_s_a_empty_string;
            }
            else {
                result = afw_utf8_printf(p, xctx,
                    AFW_SIZE_T_FMT,
                    info.contextual->value_offset);
            }
        }
    }
    
    return result;
}

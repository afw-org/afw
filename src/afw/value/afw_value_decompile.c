// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value Decompile
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_decompile.c
 * @brief Decompile value graphs back to Adaptive source text.
 */

#include "afw_internal.h"



/* Decompile a value to a string. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_decompile_to_string(
    const afw_value_t *value,
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_writer_t *writer;
    const afw_utf8_t *result;
    afw_utf8_t current_string;


    if (afw_value_is_undefined(value)) {
        result = afw_s_undefined;
    }

    else {
        writer = afw_utf8_writer_create(tab, p, xctx);
        afw_value_decompile(value, writer, xctx);
        afw_utf8_writer_current_string(writer, &current_string, xctx);
        result = afw_utf8_create_copy(current_string.s, current_string.len,
            p, xctx);
        afw_writer_release(writer, xctx);
    }

    return result;
}


/* Decompile call args.
 *
 * args->argc is the number of parameters (not including argv[0]). Parameter
 * values are argv[1] .. argv[argc]. Use first_arg 1 after writing the callee
 * name separately, or first_arg 0 to include argv[0] in the parentheses.
 */
AFW_DEFINE(void)
afw_value_decompile_call_args(
    const afw_writer_t *writer,
    afw_size_t first_arg,
    const afw_value_call_args_t *args,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }

    for (i = first_arg; i <= args->argc; i++) {
        if (i != first_arg) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        /*
         * Call-site spread was compiled as list_expression / array_expression;
         * surface form is ...expr (issue #140).
         */
        if (args->argv[i] && afw_value_is_array_expression(args->argv[i])) {
            const afw_value_list_expression_t *le =
                (const afw_value_list_expression_t *)args->argv[i];
            afw_writer_write_z(writer, "...", xctx);
            afw_value_decompile_value(le->internal, writer, xctx);
        }
        else {
            afw_value_decompile_value(args->argv[i], writer, xctx);
        }
    }

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);
}


/* Write synthetic decompile name: '#' + value implementation id (pragma form). */
AFW_DEFINE(void)
afw_value_decompile_write_synthetic_function_name(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_writer_write_z(writer, "#", xctx);
    afw_writer_write_utf8(writer, &instance->inf->rti.implementation_id, xctx);
}


/* Parenthesized list of values (same whitespace rules as call args). */
AFW_DEFINE(void)
afw_value_decompile_value_list(
    const afw_writer_t *writer,
    afw_size_t argc,
    const afw_value_t * const *argv,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }

    for (i = 0; i < argc; i++) {
        if (i != 0) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_value_decompile_value(argv[i], writer, xctx);
    }

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);
}


/* Decompile Value::. */
AFW_DEFINE(void)
afw_value_decompile_value(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    if (afw_value_is_undefined(instance)) {
        afw_writer_write_utf8(writer, afw_s_undefined, xctx);
    }
    /*
     * Switch "default" clause uses a unique permanent null pointer for
     * identity compares. Decompile as #switch_default so recompile restores
     * that same pointer (plain null would not match).
     */
    else if (instance == afw_value_unique_default_case_value) {
        afw_writer_write_z(writer, "#switch_default", xctx);
    }
    else {
        afw_value_decompile(instance, writer, xctx);
    }
}


/*
 * True if type is missing or bare "any" (default when no annotation written).
 */
static afw_boolean_t
impl_type_is_default_any(const afw_value_type_t *type)
{
    if (!type || !type->data_type) {
        return true;
    }
    if (type->data_type != afw_data_type_any) {
        return false;
    }
    /* Bare any with no parameters. */
    return type->data_type_parameter_contextual == NULL &&
        type->list_type == NULL &&
        type->media_type == NULL &&
        type->return_type == NULL &&
        type->function_signature == NULL &&
        type->object_type_id == NULL &&
        type->type == NULL &&
        type->value_meta_object == NULL;
}


/*
 * Type decompile: Adaptive Type surface (see OptionalType / Type in compile).
 * Omits bare "any" (default when no annotation was written).
 */
AFW_DEFINE(afw_boolean_t)
afw_value_decompile_type(
    const afw_value_type_t *type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_type_list_t *list_type;
    const afw_compile_value_contextual_t *ctx;
    afw_size_t i;
    afw_boolean_t need_parens;
    afw_value_string_t s;

    if (impl_type_is_default_any(type)) {
        return false;
    }

    /*
     * Parameterized types use (dataType …). Prefer recorded source span when
     * present and bounded.
     */
    ctx = type->data_type_parameter_contextual;
    need_parens = (ctx != NULL) ||
        (type->list_type != NULL) ||
        (type->media_type != NULL) ||
        (type->object_type_id != NULL) ||
        (type->return_type != NULL) ||
        (type->type != NULL);

    if (need_parens) {
        afw_writer_write_z(writer, "(", xctx);
    }

    afw_writer_write_utf8(writer, &type->data_type->data_type_id, xctx);

    if (ctx &&
        ctx->compiled_value &&
        ctx->compiled_value->full_source &&
        ctx->value_size > 0 &&
        ctx->value_offset + ctx->value_size <=
            ctx->compiled_value->full_source->len)
    {
        afw_writer_write_z(writer, " ", xctx);
        afw_writer_write(writer,
            ctx->compiled_value->full_source->s + ctx->value_offset,
            ctx->value_size, xctx);
    }
    else if (type->list_type) {
        list_type = type->list_type;
        for (i = 0; i < list_type->dimension; i++) {
            afw_writer_write_z(writer, " of", xctx);
            if (i + 1 < list_type->dimension) {
                afw_writer_write_z(writer, " array", xctx);
            }
            else if (list_type->cell_type) {
                afw_writer_write_z(writer, " ", xctx);
                afw_value_decompile_type(list_type->cell_type, writer, xctx);
            }
            else {
                afw_writer_write_z(writer, " array", xctx);
            }
        }
    }
    else if (type->media_type) {
        afw_writer_write_z(writer, " ", xctx);
        s.inf = &afw_value_unmanaged_string_inf;
        s.internal = *type->media_type;
        afw_value_decompile((const afw_value_t *)&s, writer, xctx);
    }
    else if (type->object_type_id) {
        afw_writer_write_z(writer, " ", xctx);
        s.inf = &afw_value_unmanaged_string_inf;
        s.internal = *type->object_type_id;
        afw_value_decompile((const afw_value_t *)&s, writer, xctx);
    }
    else if (type->return_type) {
        afw_writer_write_z(writer, " ", xctx);
        afw_value_decompile_type(type->return_type, writer, xctx);
    }
    else if (type->type) {
        afw_writer_write_z(writer, " ", xctx);
        afw_value_decompile_type(type->type, writer, xctx);
    }

    if (need_parens) {
        afw_writer_write_z(writer, ")", xctx);
    }

    return true;
}


AFW_DEFINE(void)
afw_value_decompile_optional_type(
    const afw_value_type_t *type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    if (impl_type_is_default_any(type)) {
        return;
    }

    afw_writer_write_z(writer, ":", xctx);
    if (writer->tab) {
        afw_writer_write_z(writer, " ", xctx);
    }
    afw_value_decompile_type(type, writer, xctx);
}

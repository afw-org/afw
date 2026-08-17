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
    if (!type) {
        return true;
    }
    /* Zero-init / missing leaf: treat as default any (omit on decompile). */
    if (type->kind == afw_value_type_kind_data_type &&
        (!type->data_type || type->data_type == afw_data_type_any))
    {
        return true;
    }
    return false;
}


/*
 * Type decompile: TS-like surface (issue #28). Omits bare "any".
 */
AFW_DEFINE(afw_boolean_t)
afw_value_decompile_type(
    const afw_value_type_t *type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_type_function_param_t *param;
    afw_size_t i;
    afw_boolean_t need_comma;

    if (impl_type_is_default_any(type)) {
        return false;
    }

    switch (type->kind) {

    case afw_value_type_kind_data_type:
        afw_writer_write_utf8(writer, &type->data_type->data_type_id, xctx);
        break;

    case afw_value_type_kind_reference:
        afw_writer_write_utf8(writer, type->reference.name, xctx);
        break;

    case afw_value_type_kind_object:
        if (type->object.interface_name) {
            afw_writer_write_utf8(writer, type->object.interface_name, xctx);
            break;
        }
        afw_value_decompile_type_object_literal(type, writer, xctx);
        break;

    case afw_value_type_kind_array:
        if (type->array.element &&
            (type->array.element->kind == afw_value_type_kind_union ||
                type->array.element->kind ==
                    afw_value_type_kind_intersection ||
                type->array.element->kind ==
                    afw_value_type_kind_function))
        {
            afw_writer_write_z(writer, "(", xctx);
            if (!afw_value_decompile_type(type->array.element, writer,
                xctx))
            {
                afw_writer_write_z(writer, "any", xctx);
            }
            afw_writer_write_z(writer, ")", xctx);
        }
        else if (type->array.element) {
            if (!afw_value_decompile_type(type->array.element, writer,
                xctx))
            {
                afw_writer_write_z(writer, "any", xctx);
            }
        }
        else {
            afw_writer_write_z(writer, "array", xctx);
            break;
        }
        afw_writer_write_z(writer, "[]", xctx);
        break;

    case afw_value_type_kind_tuple:
        afw_writer_write_z(writer, "[", xctx);
        for (i = 0; i < type->tuple.count; i++) {
            if (i > 0) {
                afw_writer_write_z(writer, ",", xctx);
            }
            if (!afw_value_decompile_type(type->tuple.elements[i], writer,
                xctx))
            {
                afw_writer_write_z(writer, "any", xctx);
            }
        }
        afw_writer_write_z(writer, "]", xctx);
        break;

    case afw_value_type_kind_function:
        afw_writer_write_z(writer, "(", xctx);
        need_comma = false;
        for (param = type->function.parameters; param; param = param->next) {
            if (need_comma) {
                afw_writer_write_z(writer, ",", xctx);
            }
            need_comma = true;
            if (param->is_rest) {
                afw_writer_write_z(writer, "...", xctx);
            }
            if (param->name) {
                afw_writer_write_utf8(writer, param->name, xctx);
                if (param->optional) {
                    afw_writer_write_z(writer, "?", xctx);
                }
                afw_writer_write_z(writer, ":", xctx);
            }
            if (!afw_value_decompile_type(param->type, writer, xctx)) {
                afw_writer_write_z(writer, "any", xctx);
            }
        }
        afw_writer_write_z(writer, ")=>", xctx);
        if (!afw_value_decompile_type(type->function.returns, writer, xctx)) {
            afw_writer_write_z(writer, "any", xctx);
        }
        break;

    case afw_value_type_kind_union:
    case afw_value_type_kind_intersection:
        for (i = 0; i < type->compound.count; i++) {
            if (i > 0) {
                afw_writer_write_z(writer,
                    type->kind == afw_value_type_kind_union ? "|" : "&",
                    xctx);
            }
            if (!afw_value_decompile_type(type->compound.members[i], writer,
                xctx))
            {
                afw_writer_write_z(writer, "any", xctx);
            }
        }
        break;

    default:
        afw_writer_write_z(writer, "any", xctx);
        break;
    }

    return true;
}



AFW_DEFINE(void)
afw_value_decompile_type_object_literal(
    const afw_value_type_t *type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_type_property_t *prop;
    afw_boolean_t need_comma;

    if (!type || type->kind != afw_value_type_kind_object) {
        afw_writer_write_z(writer, "any", xctx);
        return;
    }

    afw_writer_write_z(writer, "{", xctx);
    need_comma = false;
    for (prop = type->object.properties; prop; prop = prop->next) {
        if (need_comma) {
            afw_writer_write_z(writer, ",", xctx);
        }
        need_comma = true;
        afw_writer_write_utf8(writer, prop->name, xctx);
        if (prop->optional) {
            afw_writer_write_z(writer, "?", xctx);
        }
        afw_writer_write_z(writer, ":", xctx);
        if (writer->tab) {
            afw_writer_write_z(writer, " ", xctx);
        }
        if (!afw_value_decompile_type(prop->type, writer, xctx)) {
            afw_writer_write_z(writer, "any", xctx);
        }
    }
    afw_writer_write_z(writer, "}", xctx);
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

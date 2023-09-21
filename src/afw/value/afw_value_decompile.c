// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value Decompile
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_value_decompile.c
 * @brief Adaptive Framework value decompile
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
        result = &afw_s_undefined;
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


/* Decompile call args. */
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

    for (i = first_arg; i < args->argc; i++) {
        if (i != first_arg) {      
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_value_decompile_value(args->argv[i], writer, xctx);     
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
        afw_writer_write_utf8(writer, &afw_s_undefined, xctx);
    }
    else {
        afw_value_decompile(instance, writer, xctx);
    }
}

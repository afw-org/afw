// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Lambda Definition
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_function_thunk.c
 * @brief Implementation of afw_value interface for lambda definition
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL


/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_function_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_function_direct, \
    NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "function_thunk"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_function_thunk_inf
#include "afw_value_impl_declares.h"



/* Create a function thunk value. */
AFW_DEFINE(const afw_value_t *)
afw_value_function_thunk_create_impl(
    const afw_utf8_t *name,
    const afw_value_t *like_function_value,
    afw_function_thunk_execute_t execute,
    void *ctx,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_function_thunk_t *self;

    self = afw_pool_calloc_type(p, afw_value_function_thunk_t, xctx);
    self->inf = &afw_value_function_thunk_inf;
    self->name = name;
    self->like_function_value = like_function_value;
    self->execute = execute;
    self->ctx = ctx;
    self->detail = afw_utf8_printf(p, xctx,
        "Thunk %" AFW_UTF8_FMT " created at %s",
        AFW_UTF8_FMT_ARG(name), source_z);

    return (const afw_value_t *)self;
}



/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return instance;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}


/*
 * Implementation of method compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_function_thunk_t *self =
        (const afw_value_function_thunk_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
        NULL, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);
    afw_writer_write_z(writer, "detail: ", xctx);
    afw_writer_write_utf8(writer, self->detail, xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_decompile(
    const afw_value_t * instance,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
#ifdef __FIXME_REMOVE_
    const afw_value_function_thunk_t *self =
        (const afw_value_function_thunk_t *)instance;
    afw_size_t i;

    afw_writer_write_z(writer, "function ", xctx);
    impl_decompile_type(writer, self->returns, xctx);

    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
    }

    for (i = 0; i < self->count; i++) {
        if (i != 0) {      
            afw_writer_write_z(writer, ",", xctx);
            if (writer->tab) {
                afw_writer_write_eol(writer, xctx);
            }
        }
        impl_decompile_type(writer, self->parameters[i]->type, xctx);
        afw_writer_write_utf8(writer, self->parameters[i]->name, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }

    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
    }
    afw_value_decompile(self->body, writer, xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);
#endif
}


/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    const afw_value_t *instance,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_function_thunk_t *self =
        (const afw_value_function_thunk_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->detail = self->detail;
}

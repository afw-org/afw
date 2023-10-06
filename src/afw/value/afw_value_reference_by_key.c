// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for reference_by_key
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_reference_by_key.c
 * @brief Implementation of afw_value interface for reference_by_key
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "reference_by_key"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_reference_by_key_inf
#include "afw_value_impl_declares.h"

/** @fixme Should have list entry by index too. */


/* Create function for reference_by_key value. */
AFW_DEFINE(const afw_value_t *)
afw_value_reference_by_key_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_t *aggregate_value,
    const afw_value_t *key,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_reference_by_key_t *self;

    self = afw_pool_calloc_type(p, afw_value_reference_by_key_t, xctx);
    self->inf = &afw_value_reference_by_key_inf;
    self->contextual = contextual;
    self->aggregate_value = aggregate_value;
    self->key = key;

    /* Add backtrace detail. */
    if (afw_value_is_string(key)) {
        self->backtrace_detail = afw_utf8_printf(
            p, xctx,
            "[string " AFW_UTF8_FMT "]",
            AFW_UTF8_FMT_ARG(&((const afw_value_string_t *)key)->internal));
    }
    else if (afw_value_is_integer(key)) {
        self->backtrace_detail = afw_utf8_printf(
            p, xctx,
            "[" AFW_INTEGER_FMT "]",
            ((const afw_value_integer_t *)key)->internal);
    }
    else if (!afw_value_is_defined_and_evaluated(key)) {
        self->backtrace_detail = afw_s_a_bracketed_value;
    }
    else {
        AFW_THROW_ERROR_Z(general,
            "index must be an integer, expression, or string",
            xctx);
    }
   
    /** @fixme add optimization. */
    self->optimized_value = (const afw_value_t *)self;

    /** @fixme Get right data type. */
    //self->evaluated_data_type = afw_data_type_string;

    /* Return adaptive value. */
    return (afw_value_t *)self;
}


/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    const afw_value_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_reference_by_key_t *self =
        (const afw_value_reference_by_key_t *)instance;

    const afw_value_t *v;
    const afw_value_object_t *object_value;
    const afw_value_array_t *list;
    const afw_value_t *key;
    const afw_utf8_t *name;
    afw_size_t i;
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;

    v = afw_value_evaluate(self->aggregate_value, p, xctx);

    /* If value is single object, index is property name. */
    if (afw_value_is_object(v)) {
        object_value = (const afw_value_object_t *)v;

        key = afw_value_evaluate(self->key, p, xctx);
        if (afw_value_is_string(key)) {
            name = &((const afw_value_string_t *)key)->internal;
        }
        else {
            name = afw_value_as_utf8(key, p, xctx);
        }

        v = afw_object_get_property(object_value->internal, name, xctx);
        if (!v) {
            return NULL;
        }
        result = afw_value_evaluate(v, p, xctx);
    }

    /* If value is single list, index is index is index into list. */
    else if (afw_value_is_array(v)) {
        list = (const afw_value_array_t *)v;

        key = afw_value_evaluate(self->key, p, xctx);
        if (!afw_value_is_integer(key)) {
            AFW_THROW_ERROR_Z(evaluate,
                "Index must be integer for list", xctx);
        }
        i = afw_safe_cast_integer_to_size(
            ((const afw_value_integer_t *)key)->internal,
            xctx);

        result = afw_array_get_entry_value(list->internal,
            i, p, xctx);
        if (!result) {
            AFW_THROW_ERROR_Z(evaluate,
                "Index out of range for list", xctx);
        }
    }

    else {
        AFW_THROW_ERROR_Z(evaluate,
            "Expecting object or list", xctx);
    }

    /* Pop value from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_value(xctx);
    xctx->error->contextual = saved_contextual;
    return result;
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
 * Implementation of method get_evaluated_meta for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_evaluated_meta(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_reference_by_key_t *self =
        (const afw_value_reference_by_key_t *)instance;
    const afw_value_t *result;
    const afw_value_t *key;
    const afw_value_t *aggregate_value;

    key = afw_value_evaluate(self->key, p, xctx);
    aggregate_value = afw_value_evaluate(self->aggregate_value, p, xctx);

    if (afw_value_is_object(aggregate_value)) {
        key = afw_value_convert(key, afw_data_type_string, false, p, xctx);
        if (!afw_value_is_string(key)) {
            AFW_THROW_ERROR_Z(general,
                "reference_by_key key for object must be a string",
                xctx);
        }
        result = afw_object_get_property_meta(
            ((const afw_value_object_t *)aggregate_value)->internal,
            &((const afw_value_string_t *)key)->internal,
            p, xctx);
    }
    else if (afw_value_is_array(aggregate_value)) {
        key = afw_value_convert(key, afw_data_type_integer, false, p, xctx);
        if (!afw_value_is_integer(key)) {
            AFW_THROW_ERROR_Z(general,
                "reference_by_key key for list must be an integer",
                xctx);
        }
        result = afw_array_get_entry_meta(
            ((const afw_value_array_t *)aggregate_value)->internal,
            ((const afw_value_integer_t *)key)->internal,
            p, xctx);
    }
    else {
        AFW_THROW_ERROR_Z(general,
            "reference_by_key aggregate_value must be list or object",
            xctx);
    }

    return result;
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
    const afw_value_reference_by_key_t *self =
        (const afw_value_reference_by_key_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    afw_writer_write_z(writer, "key: [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);
    afw_value_compiler_listing_value(self->key, writer, xctx);
    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);

    afw_value_produce_compiler_listing(self->aggregate_value, writer, xctx);

    if (self->evaluated_data_type) {
        afw_writer_write_z(writer, "evaluated_data_type: ", xctx);
        afw_writer_write_utf8(writer,
            &self->evaluated_data_type->data_type_id, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->optimized_value != instance) {
        afw_writer_write_z(writer, "optimized_value: ", xctx);
        afw_value_produce_compiler_listing(self->optimized_value, writer, xctx);
        afw_writer_write_eol(writer, xctx);
    }

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
    const afw_value_reference_by_key_t *self =
        (const afw_value_reference_by_key_t *)instance;

    afw_value_decompile(self->aggregate_value, writer, xctx);
    afw_writer_write_z(writer, "[", xctx);
    afw_value_decompile_value(self->key, writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
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
    const afw_value_reference_by_key_t *self =
        (const afw_value_reference_by_key_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->detail = self->backtrace_detail;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;
}

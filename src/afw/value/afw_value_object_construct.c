// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Object Construct
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_object_construct.c
 * @brief afw_value implementation for object construct (expression property names)
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_for_object

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_for_object

#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "object_construct"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_object_construct_inf
#define AFW_VALUE_SELF_T afw_value_object_construct_t
#include "afw_value_impl_declares.h"


/* Create function for object construct value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_object_construct(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_object_construct_entry_t *entries,
    const afw_object_t *meta,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_object_construct_t *self;

    self = afw_pool_calloc(p, sizeof(afw_value_object_construct_t), xctx);
    self->inf = &afw_value_object_construct_inf;
    self->contextual = contextual;
    self->entries = entries;
    self->meta = meta;
    return &self->pub;
}


/*
 * Resolve a property name value to utf8 (same idea as reference_by_key assign).
 */
static const afw_utf8_t *
impl_name_from_value(
    const afw_value_t *name_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *name;

    if (!name_value) {
        AFW_THROW_ERROR_Z(evaluation_error,
            "Object property name expression evaluated to undefined",
            xctx);
    }
    if (afw_value_is_string(name_value)) {
        return &((const afw_value_string_t *)name_value)->internal;
    }
    name = afw_value_as_utf8(name_value, p, xctx);
    if (!name) {
        AFW_THROW_ERROR_Z(evaluation_error,
            "Object property name expression must produce a string",
            xctx);
    }
    return name;
}


/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_object_construct_entry_t *e;
    const afw_object_t *to;
    const afw_object_t *from;
    const afw_value_t *v;
    const afw_value_t *name_v;
    const afw_utf8_t *property_name;
    const afw_iterator_old_t *iterator;
    const afw_compile_value_contextual_t *saved_contextual;

    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;

    to = afw_object_create_unmanaged(p, xctx);

    for (e = self->entries; e; e = e->next) {
        if (e->type == afw_value_object_construct_entry_spread) {
            v = afw_value_evaluate(e->spread_expr, p, xctx);
            if (!v || !afw_value_is_object(v)) {
                AFW_THROW_ERROR_Z(evaluation_error,
                    "Object spread must evaluate to an object",
                    xctx);
            }
            from = ((const afw_value_object_t *)v)->internal;
            for (iterator = NULL;;) {
                v = afw_object_get_next_property(from, &iterator,
                    &property_name, xctx);
                if (!v) {
                    break;
                }
                v = afw_value_evaluate(v, p, xctx);
                afw_object_set_property(to, property_name, v, xctx);
            }
        }
        else if (e->type == afw_value_object_construct_entry_name_expr) {
            name_v = afw_value_evaluate(e->name_expr, p, xctx);
            property_name = impl_name_from_value(name_v, p, xctx);
            v = afw_value_evaluate(e->value, p, xctx);
            afw_object_set_property(to, property_name, v, xctx);
        }
        else {
            /* static */
            v = afw_value_evaluate(e->value, p, xctx);
            afw_object_set_property(to, e->static_name, v, xctx);
        }
    }

    if (self->meta) {
        afw_object_meta_set_meta_object(to, self->meta, xctx);
    }

    xctx->error->contextual = saved_contextual;
    return afw_value_create_unmanaged_object(to, p, xctx);
}


/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    return afw_data_type_object;
}


/*
 * Implementation of method compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_object_construct_entry_t *e;

    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED
    for (e = self->entries; e; e = e->next) {
        if (e->type == afw_value_object_construct_entry_spread) {
            afw_writer_write_z(writer, "spread ", xctx);
            afw_value_compiler_listing_value(e->spread_expr, writer, xctx);
        }
        else if (e->type == afw_value_object_construct_entry_name_expr) {
            afw_writer_write_z(writer, "property [", xctx);
            afw_value_compiler_listing_value(e->name_expr, writer, xctx);
            afw_writer_write_z(writer, "] ", xctx);
            afw_value_compiler_listing_value(e->value, writer, xctx);
        }
        else {
            afw_writer_write_z(writer, "property ", xctx);
            afw_writer_write_utf8(writer, e->static_name, xctx);
            afw_writer_write_z(writer, " ", xctx);
            afw_value_compiler_listing_value(e->value, writer, xctx);
        }
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
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_object_construct_entry_t *e;
    afw_boolean_t first;

    afw_writer_write_z(writer, "{", xctx);
    first = true;
    for (e = self->entries; e; e = e->next) {
        if (!first) {
            afw_writer_write_z(writer, ", ", xctx);
        }
        first = false;
        if (e->type == afw_value_object_construct_entry_spread) {
            afw_writer_write_z(writer, "...", xctx);
            afw_value_decompile_value(e->spread_expr, writer, xctx);
        }
        else if (e->type == afw_value_object_construct_entry_name_expr) {
            afw_writer_write_z(writer, "[", xctx);
            afw_value_decompile_value(e->name_expr, writer, xctx);
            afw_writer_write_z(writer, "]: ", xctx);
            afw_value_decompile_value(e->value, writer, xctx);
        }
        else {
            /* Quoted string key is always valid Adaptive object syntax. */
            afw_data_type_write_as_expression(
                afw_data_type_string, writer, e->static_name, xctx);
            afw_writer_write_z(writer, ": ", xctx);
            afw_value_decompile_value(e->value, writer, xctx);
        }
    }
    afw_writer_write_z(writer, "}", xctx);
}


/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_memory_clear(info);
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = &self->pub;
}

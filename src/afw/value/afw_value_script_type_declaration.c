// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for script type / interface declaration
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_script_type_declaration.c
 * @brief Compiled `type` / `interface` statement (decompile #type / #interface).
 */

#include "afw_internal.h"

#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL
#define impl_afw_value_get_assignable_value NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

#define impl_afw_value_create_iterator NULL

#define AFW_IMPLEMENTATION_ID "type"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_script_type_declaration_inf
#define AFW_VALUE_SELF_T afw_value_script_type_declaration_t
#include "afw_value_impl_declares.h"



AFW_DEFINE(const afw_value_t *)
afw_value_script_type_declaration_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_utf8_t *name,
    const afw_value_type_t *type,
    afw_boolean_t is_interface,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_script_type_declaration_t *self;

    self = afw_pool_calloc_type(p, afw_value_script_type_declaration_t,
        xctx);
    self->inf = &afw_value_script_type_declaration_inf;
    self->contextual = contextual;
    self->name = name;
    self->type = type;
    self->is_interface = is_interface;
    return &self->pub;
}



/*
 * Implementation of method optional_evaluate for interface afw_value.
 *
 * Compile-time only. Return void so the running result is unchanged.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)p;
    (void)xctx;
    return afw_value_void;
}



const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;
    return NULL;
}



void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->contextual, xctx);
    afw_writer_write_z(writer, self->is_interface ? " interface " : " type ",
        xctx);
    if (self->name) {
        afw_writer_write_utf8(writer, self->name, xctx);
    }
    afw_writer_write_eol(writer, xctx);
}



void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_string_t name_string;
    afw_size_t i;

    if (self->is_interface) {
        afw_writer_write_z(writer, "#interface", xctx);
    }
    else {
        afw_value_decompile_write_synthetic_function_name(&self->pub,
            writer, xctx);
    }
    afw_writer_write_z(writer, "(", xctx);

    name_string.inf = &afw_value_unmanaged_string_inf;
    if (self->name) {
        name_string.internal.s = self->name->s;
        name_string.internal.len = self->name->len;
    }
    else {
        name_string.internal.s = "";
        name_string.internal.len = 0;
    }
    afw_value_decompile((const afw_value_t *)&name_string, writer, xctx);
    afw_writer_write_z(writer, ",", xctx);

    if (self->is_interface) {
        afw_value_decompile_type_object_literal(self->type, writer, xctx);
        if (self->type &&
            self->type->kind == afw_value_type_kind_object)
        {
            for (i = 0; i < self->type->object.extends_count; i++) {
                afw_writer_write_z(writer, ",", xctx);
                if (!afw_value_decompile_type(
                    self->type->object.extends[i], writer, xctx))
                {
                    afw_writer_write_z(writer, "any", xctx);
                }
            }
        }
    }
    else if (!afw_value_decompile_type(self->type, writer, xctx)) {
        afw_writer_write_z(writer, "any", xctx);
    }

    afw_writer_write_z(writer, ")", xctx);
}



void
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    (void)xctx;
    afw_memory_clear(info);
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = &self->pub;
}

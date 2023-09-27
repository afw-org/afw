// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for assignment target
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_assignment_target.c
 * @brief Implementation of afw_value interface for assignment_target
 */

#include "afw_internal.h"

#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "assignment_target"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_assignment_target_inf
#define impl_afw_value_create_iterator NULL
#include "afw_value_impl_declares.h"


typedef struct {
    const afw_utf8_t id;
    const afw_utf8_t description;
} impl_type_entry_t;


/* Target types */
static impl_type_entry_t
impl_assignment_types[] = {
#define XX(id, description) \
{   AFW_UTF8_LITERAL(AFW_STRINGIFY(id)), \
    AFW_UTF8_LITERAL(description) \
},
    AFW_COMPILE_INTERNAL_ASSIGNMENT_TYPE_MAP(XX)
#undef XX
    { { NULL } }
};


/* Create function for assignment_target value. */
AFW_DEFINE(const afw_value_t *)
afw_value_assignment_target_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_compile_assignment_target_t *assignment_target,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_assignment_target_t *self;

    self = afw_pool_calloc_type(p, afw_value_assignment_target_t, xctx);
    self->inf = &afw_value_assignment_target_inf;
    self->contextual = contextual;
    self->assignment_target = assignment_target;

    return (const afw_value_t *)self;
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
    const afw_value_assignment_target_t *self =
        (const afw_value_assignment_target_t *)instance;

    if (self->assignment_target->target_type ==
        afw_compile_assignment_target_type_symbol_reference)
    {
        return (const afw_value_t *)self->assignment_target->symbol_reference;
    }

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
 * Implementation of method get_evaluated_meta for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_evaluated_meta(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}


static void
impl_assignment_element_produce_compiler_listing(
    const afw_compile_assignment_element_t *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, NULL, NULL, xctx);
    afw_writer_write_z(writer, "assignment_element: [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    if (self->assignment_target) {
        afw_value_produce_compiler_listing(self->assignment_target, writer, xctx);
    }
    else {
        afw_writer_write_z(writer, "<skipped>", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->default_value) {
        afw_writer_write_z(writer, "default: ", xctx);
        afw_value_produce_compiler_listing(self->default_value, writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_assignment_property_produce_compiler_listing(
    const afw_compile_assignment_property_t *self ,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, NULL, NULL, xctx);
    if (self->is_rename) {
        afw_writer_write_z(writer, "assignment_property rename: [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);

        afw_writer_write_z(writer, "property_name: ", xctx);
        afw_writer_write_utf8(writer, self->property_name, xctx);
        afw_writer_write_eol(writer, xctx);

        impl_assignment_element_produce_compiler_listing(
            self->assignment_element, writer, xctx);
    }
    else {
        afw_writer_write_z(writer, "assignment_property reference: [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);

        afw_value_produce_compiler_listing(
            (const afw_value_t *)self->symbol_reference, writer, xctx);

        if (self->default_value) {
            afw_writer_write_z(writer, "default: ", xctx);
            afw_value_produce_compiler_listing(self->default_value, writer, xctx);
        }
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_list_destructure_produce_compiler_listing(
    const afw_compile_list_destructure_t *self ,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_element_t *ae;

    afw_value_compiler_listing_begin_value(writer, NULL, NULL, xctx);
    afw_writer_write_z(writer, "list_destructure: [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    for (ae = self->assignment_element; ae; ae = ae->next) {
        impl_assignment_element_produce_compiler_listing(ae, writer, xctx);
    }

    if (self->rest) {
        afw_writer_write_z(writer, "rest: ", xctx);
        afw_value_produce_compiler_listing(self->rest, writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_object_destructure_produce_compiler_listing(
    const afw_compile_object_destructure_t *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_property_t *ap;

    afw_value_compiler_listing_begin_value(writer, NULL, NULL, xctx);
    afw_writer_write_z(writer, "object_destructure: [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    for (ap = self->assignment_property; ap; ap = ap->next) {
        impl_assignment_property_produce_compiler_listing(ap, writer, xctx);
    }

    if (self->rest) {
        afw_writer_write_z(writer, "rest: ", xctx);
        afw_value_produce_compiler_listing(self->rest, writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
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
    const afw_value_assignment_target_t *self =
        (const afw_value_assignment_target_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, " ", xctx);
    afw_writer_write_utf8(writer,
        &impl_assignment_types[self->assignment_target->assignment_type].id,
        xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    switch (self->assignment_target->target_type) {
    case afw_compile_assignment_target_type_list_destructure:
        impl_list_destructure_produce_compiler_listing(
            self->assignment_target->list_destructure, writer, xctx);
        break;

    case afw_compile_assignment_target_type_object_destructure:
        impl_object_destructure_produce_compiler_listing(
            self->assignment_target->object_destructure, writer, xctx);
        break;

    case afw_compile_assignment_target_type_symbol_reference:
        afw_value_produce_compiler_listing(
            (const afw_value_t *)self->assignment_target->symbol_reference,
            writer, xctx);
        break;
    
    case afw_compile_assignment_target_type_max_type:
    default:
        AFW_THROW_ERROR_Z(code, "Invalid case", xctx);
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
    AFW_THROW_ERROR_Z(general,
        "Decompile is not supported for assignment target.",
        xctx);
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
    const afw_value_assignment_target_t *self =
        (const afw_value_assignment_target_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}

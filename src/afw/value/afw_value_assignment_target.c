// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for assignment target
 *
 * Copyright (c) 2010-2024 Clemson University
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

    return &self->pub;
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
        return &self->assignment_target->symbol_reference->pub;
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
 * Pattern decompile (second arg of #assignment_target).
 *
 * Nested targets are Patterns only (no nested #assignment_target wrapper):
 *   symbol_reference → name[: Type]
 *   list_destructure → [elem, …]
 *   object_destructure → {prop, …}
 * Elements may be holes, defaults (name = expr), rest (...name), renames
 * (prop: target), nested patterns, and type annotations.
 */

static void
impl_decompile_pattern(
    const afw_value_t *value,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);


static void
impl_decompile_binding_name_and_type(
    const afw_value_t *target_value,
    const afw_value_type_t *element_type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_assignment_target_t *at;
    const afw_value_type_t *type;
    const afw_value_symbol_reference_t *sym;

    /*
     * Note: assignment_target_t stores symbol_reference and variable_type in
     * the same union — for symbol targets the type lives on symbol->type only
     * (and on list/object element type fields).
     */
    type = element_type;
    if (afw_value_is_symbol_reference(target_value)) {
        sym = (const afw_value_symbol_reference_t *)target_value;
        afw_value_decompile(target_value, writer, xctx);
        if (!type) {
            type = &sym->symbol->type;
        }
        afw_value_decompile_optional_type(type, writer, xctx);
        return;
    }

    if (afw_value_is_assignment_target(target_value)) {
        at = (const afw_value_assignment_target_t *)target_value;
        if (at->assignment_target->target_type ==
            afw_compile_assignment_target_type_symbol_reference)
        {
            sym = at->assignment_target->symbol_reference;
            afw_value_decompile((const afw_value_t *)sym, writer, xctx);
            if (!type && sym) {
                type = &sym->symbol->type;
            }
            afw_value_decompile_optional_type(type, writer, xctx);
            return;
        }
    }

    impl_decompile_pattern(target_value, writer, xctx);
    afw_value_decompile_optional_type(element_type, writer, xctx);
}


static void
impl_decompile_list_pattern(
    const afw_compile_list_destructure_t *ld,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_element_t *ae;
    afw_boolean_t need_comma;

    afw_writer_write_z(writer, "[", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }
    need_comma = false;
    for (ae = ld->assignment_element; ae; ae = ae->next) {
        if (need_comma) {
            afw_writer_write_z(writer, ",", xctx);
        }
        need_comma = true;
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        if (ae->assignment_target) {
            impl_decompile_binding_name_and_type(ae->assignment_target,
                ae->type, writer, xctx);
            if (ae->default_value) {
                afw_writer_write_z(writer,
                    writer->tab ? " = " : "=", xctx);
                afw_value_decompile_value(ae->default_value, writer, xctx);
            }
        }
        /* else hole: nothing between commas */
    }
    if (ld->rest) {
        if (need_comma) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_writer_write_z(writer, "...", xctx);
        impl_decompile_binding_name_and_type(ld->rest, ld->rest_type,
            writer, xctx);
    }
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, "]", xctx);
}


static void
impl_decompile_object_pattern(
    const afw_compile_object_destructure_t *od,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_property_t *ap;
    const afw_compile_assignment_element_t *ae;
    afw_boolean_t need_comma;

    afw_writer_write_z(writer, "{", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }
    need_comma = false;
    for (ap = od->assignment_property; ap; ap = ap->next) {
        if (need_comma) {
            afw_writer_write_z(writer, ",", xctx);
        }
        need_comma = true;
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        if (ap->is_rename) {
            afw_writer_write_utf8(writer, ap->property_name, xctx);
            afw_writer_write_z(writer, writer->tab ? ": " : ":", xctx);
            ae = ap->assignment_element;
            if (ae && ae->assignment_target) {
                impl_decompile_binding_name_and_type(ae->assignment_target,
                    ae->type, writer, xctx);
                if (ae->default_value) {
                    afw_writer_write_z(writer,
                        writer->tab ? " = " : "=", xctx);
                    afw_value_decompile_value(ae->default_value, writer,
                        xctx);
                }
            }
        }
        else {
            afw_value_decompile(
                (const afw_value_t *)ap->symbol_reference, writer, xctx);
            if (ap->symbol_reference) {
                afw_value_decompile_optional_type(
                    &ap->symbol_reference->symbol->type, writer, xctx);
            }
            if (ap->default_value) {
                afw_writer_write_z(writer,
                    writer->tab ? " = " : "=", xctx);
                afw_value_decompile_value(ap->default_value, writer, xctx);
            }
        }
    }
    if (od->rest) {
        if (need_comma) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_writer_write_z(writer, "...", xctx);
        impl_decompile_binding_name_and_type(od->rest, od->rest_type,
            writer, xctx);
    }
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, "}", xctx);
}


static void
impl_decompile_pattern(
    const afw_value_t *value,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_assignment_target_t *at;

    if (!value) {
        return;
    }

    if (afw_value_is_symbol_reference(value)) {
        afw_value_decompile(value, writer, xctx);
        afw_value_decompile_optional_type(
            &((const afw_value_symbol_reference_t *)value)->symbol->type,
            writer, xctx);
        return;
    }

    if (afw_value_is_assignment_target(value)) {
        at = (const afw_value_assignment_target_t *)value;
        switch (at->assignment_target->target_type) {
        case afw_compile_assignment_target_type_symbol_reference:
            /* Type is on symbol (union shares storage with variable_type). */
            impl_decompile_binding_name_and_type(
                (const afw_value_t *)
                    at->assignment_target->symbol_reference,
                NULL, writer, xctx);
            break;
        case afw_compile_assignment_target_type_list_destructure:
            impl_decompile_list_pattern(
                at->assignment_target->list_destructure, writer, xctx);
            break;
        case afw_compile_assignment_target_type_object_destructure:
            impl_decompile_object_pattern(
                at->assignment_target->object_destructure, writer, xctx);
            break;
        case afw_compile_assignment_target_type_max_type:
        default:
            afw_writer_write_utf8(writer, afw_s_undefined, xctx);
            break;
        }
        return;
    }

    /* Unexpected nested value — fall back to generic decompile. */
    afw_value_decompile_value(value, writer, xctx);
}


/*
 * Implementation of method decompile for interface afw_value.
 *
 * Synthetic call #assignment_target("const"|"let"|..., Pattern).
 * Pattern is a name or surface-like list/object destructure pattern.
 */
void
impl_afw_value_decompile(
    const afw_value_t * instance,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_value_assignment_target_t *self =
        (const afw_value_assignment_target_t *)instance;
    afw_value_string_t type_string;
    const afw_utf8_t *type_id;

    afw_value_decompile_write_synthetic_function_name(instance, writer, xctx);
    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    type_id = &impl_assignment_types[
        self->assignment_target->assignment_type].id;
    type_string.inf = &afw_value_unmanaged_string_inf;
    type_string.internal.s = type_id->s;
    type_string.internal.len = type_id->len;
    afw_value_decompile((const afw_value_t *)&type_string, writer, xctx);

    afw_writer_write_z(writer, ",", xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
    }

    impl_decompile_pattern(instance, writer, xctx);

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);
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

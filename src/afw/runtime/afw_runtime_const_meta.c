// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface for const runtime meta
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */



/**
 * @file afw_runtime_const_meta.c
 * @brief Implementation of afw_object interface for runtime const meta
 */

#include "afw_internal.h"


#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/** @fixme Add include for headers and anything else you want in this part of code. */

/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "afw_runtime_const_meta"
#include "afw_object_impl_declares.h"

/*
 * inf for interface afw_object implementation.
 */
AFW_DEFINE_CONST_DATA(afw_object_inf_t)
afw_runtime_inf_const_meta_object_inf = {
    {
        AFW_UTF8_LITERAL("afw_object"),
        AFW_UTF8_LITERAL(__FILE__),
        AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID)
    },
    impl_afw_object_release,
    impl_afw_object_get_reference,
    impl_afw_object_get_count,
    impl_afw_object_get_meta,
    impl_afw_object_get_property,
    impl_afw_object_get_property_meta,
    impl_afw_object_get_next_property,
    impl_afw_object_get_next_property_meta,
    impl_afw_object_has_property,
    impl_afw_object_get_setter
};



/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}



/*
 * Implementation of method get_reference of interface afw_object.
 */
void
impl_afw_object_get_reference (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}

/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
impl_afw_object_get_count(
    const afw_object_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property (
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_runtime_const_object_meta_object_t * self = 
        (afw_runtime_const_object_meta_object_t *)instance;
    const afw_value_t *result;

    result = NULL;

    /* Parent paths. */
    if (afw_utf8_equal(property_name, afw_s_parentPaths)) {
        if (self->parent_paths) {
            result = &self->parent_paths->pub;
        }
    }

    return result;
}



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property (
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_runtime_const_object_meta_object_t * self = 
        (afw_runtime_const_object_meta_object_t *)instance;


    /* Parent paths is only property. */
    if (!*iterator)
    {
        *iterator = (afw_iterator_t *)afw_s_null;
        if (self->parent_paths) {
            if (property_name) {
                *property_name = afw_s_parentPaths;
            }
            return &self->parent_paths->pub;
        }
    }

    *iterator = NULL;
    return NULL;
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property (
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    return (afw_utf8_equal(property_name, afw_s_parentPaths)) ? true : false;
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* No setter. */
    return NULL;
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_object Implementation for afw_xctx_qualifier_object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_xctx_qualifier_object.c
 * @brief Implementation of afw_object interface for afw_xctx_qualifier_object
 */

#include "afw_internal.h"


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "afw_xctx_qualifier_object"
#include "afw_object_impl_declares.h"



typedef struct {
    const afw_utf8_t *qualifier;
    const afw_object_t *variables_object;
} impl_qualifier_entry_t;

typedef struct impl_qualifier_entry_stack_s
impl_qualifier_entry_stack_t;

AFW_STACK_STRUCT(impl_qualifier_entry_stack_s,
    impl_qualifier_entry_t);


/* Create object to access active variables for a qualifier. */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifier_object_create(
    const afw_utf8_t *qualifier,
    afw_boolean_t for_testing,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_object_self_t *self;

    /*
     * You may want to create a new pool for instance, but will just use
     * xctx's pool in this example.
     */
    self = afw_pool_calloc_type(p, afw_xctx_qualifier_object_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->qualifier = qualifier;
    self->for_testing = for_testing;
    afw_pool_get_reference(p, xctx);

    /* Return new instance. */
    return (afw_object_t *)self;
}



/* Create object to access active qualified variables. */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifiers_object_create(
    afw_boolean_t for_testing,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *qualifiers;
    const afw_object_t *qualifier;
    const afw_xctx_qualifier_stack_entry_t *c;

    qualifiers = afw_object_create_unmanaged(p, xctx);
    for (c = xctx->qualifier_stack->top;
        c >= xctx->qualifier_stack->first;
        c--)
    {
        if (!afw_object_has_property(qualifiers, &c->qualifier, xctx)) {
            qualifier = afw_xctx_qualifier_object_create(
                &c->qualifier, for_testing,
                qualifiers->p, xctx);
            afw_object_set_property_as_object(qualifiers,
                &c->qualifier, qualifier, xctx);
        }
    }

    return qualifiers;
}



/*
 * Implementation of method release for interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_object_self_t *self =
        (afw_xctx_qualifier_object_self_t *)instance;

    self->reference_count--;
    if (self->reference_count <= 0) {
        afw_pool_release(self->pub.p, xctx);
    }
}

/*
 * Implementation of method get_reference for interface afw_object.
 */
void
impl_afw_object_get_reference(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_object_self_t *self =
        (afw_xctx_qualifier_object_self_t *)instance;

    self->reference_count++;
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
 * Implementation of method get_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_object_self_t *self =
        (afw_xctx_qualifier_object_self_t *)instance;

    return afw_xctx_get_optionally_qualified_variable(self->qualifier,
        property_name, xctx);
}

/*
 * Implementation of method get_property_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_property_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method has_property for interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_object_self_t *self =
        (afw_xctx_qualifier_object_self_t *)instance;

    return afw_xctx_get_optionally_qualified_variable(self->qualifier,
        property_name, xctx) != NULL;
}

/*
 * Implementation of method get_setter for interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    return NULL;
}

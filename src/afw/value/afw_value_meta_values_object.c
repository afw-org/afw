// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework List Implementation for Meta Values of an Object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_value_meta_values_object.c
 * @brief List implementation for meta values of an object.
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "afw_value_meta_values_object"
#include "afw_array_impl_declares.h"


/*
 * Implementation of method release for interface afw_array.
 */
void
impl_afw_array_release(
    const afw_array_t *instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    const afw_array_t *instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    const afw_array_t *instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_entry_internal(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_data_type_t **data_type,
    const void **internal,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_entry_value(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_next_internal(
    const afw_array_t *instance,
    const afw_iterator_t **iterator,
    const afw_data_type_t **data_type,
    const void **internal,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_next_value(
    const afw_array_t *instance,
    const afw_iterator_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_setter for interface afw_array.
 */
const afw_array_setter_t *
impl_afw_array_get_setter(
    const afw_array_t *instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_meta_values_list_for_object_create(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_meta_values_object_list_self_t *self;

    self = afw_xctx_calloc_type(afw_value_meta_values_object_list_self_t, xctx);
    self->pub.inf = &impl_afw_array_inf;
    self->p = p;
    AFW_VALUE_ASSERT_IS_DATA_TYPE(value, object, xctx);
    self->value = (const afw_value_object_t *)value;

    return afw_value_create_unmanaged_array(
        (const afw_array_t *)self, p, xctx);
}

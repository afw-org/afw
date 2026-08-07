// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_array interface helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_array.c
 * @brief Adaptive Framework afw_array interface helpers
 */

#include "afw_internal.h"


/* Get next value from array whose data type cType is afw_utf8_t. */
AFW_DEFINE(const afw_utf8_t *)
afw_array_of_utf8_get_next(
    const afw_array_t *instance,
    const afw_iterator_old_t * *iterator,
    afw_xctx_t *xctx)
{
    const void *internal;
    const afw_data_type_t *data_type;

    afw_array_get_next_internal(instance, iterator, &data_type, &internal, xctx);
    if (!internal) {
        return NULL;
    }

    if (!afw_utf8_equal(&data_type->cType, afw_s_afw_utf8_t))
    {
        AFW_THROW_ERROR_Z(general,
            "cType of array data type is not afw_utf8_t", xctx);
    }
    return (const afw_utf8_t *)internal;
}


/* Set an array to immutable if it is not already. */
AFW_DEFINE(void)
afw_array_set_immutable(
    const afw_array_t *instance, afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    /* If already immutable, ignore, otherwise; set immutable. */
    if (setter) {
        afw_array_setter_set_immutable(setter, xctx);
    }
}


/* Set an array to immutable and determine data type. */
AFW_DEFINE(const afw_data_type_t *)
afw_array_determine_data_type_and_set_immutable(
    const afw_array_t *instance, afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    return afw_array_setter_determine_data_type_and_set_immutable(
        setter, xctx);
}


/* push_value */
AFW_DEFINE(void)
afw_array_push_value(
    const afw_array_t *instance,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_push_value(setter, value, xctx);
}


/* push_internal */
AFW_DEFINE(void)
afw_array_push_internal(
    const afw_array_t *instance,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_push_internal(setter, data_type, internal, xctx);
}


/* pop_value */
AFW_DEFINE(const afw_value_t *)
afw_array_pop_value(
    const afw_array_t *instance,
    afw_boolean_t *found,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    return afw_array_setter_pop_value(setter, found, xctx);
}


/* shift_value */
AFW_DEFINE(const afw_value_t *)
afw_array_shift_value(
    const afw_array_t *instance,
    afw_boolean_t *found,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    return afw_array_setter_shift_value(setter, found, xctx);
}


/* insert_value */
AFW_DEFINE(void)
afw_array_insert_value(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_insert_value(setter, index, value, xctx);
}


/* set_value */
AFW_DEFINE(void)
afw_array_set_value(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_set_value(setter, index, value, xctx);
}


/* remove_value_by_index */
AFW_DEFINE(void)
afw_array_remove_value_by_index(
    const afw_array_t *instance,
    afw_integer_t index,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_remove_value_by_index(setter, index, xctx);
}


/* remove_value */
AFW_DEFINE(void)
afw_array_remove_value(
    const afw_array_t *instance,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_remove_value(setter, value, xctx);
}


/* remove_internal */
AFW_DEFINE(void)
afw_array_remove_internal(
    const afw_array_t *instance,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_remove_internal(setter, data_type, internal, xctx);
}


/* remove_all_values */
AFW_DEFINE(void)
afw_array_remove_all_values(
    const afw_array_t *instance,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);

    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_remove_all_values(setter, xctx);
}

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


AFW_DEFINE(const afw_value_t *)
afw_array_as_value(
    const afw_array_t *array,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_array_t *v;

    if (array && array->value) {
        return array->value;
    }
    v = afw_pool_calloc(p, sizeof(afw_value_array_t), xctx);
    v->inf = &afw_value_unmanaged_array_inf;
    v->internal = array;
    return &v->pub;
}


/* Get next value from array whose data type cType is afw_utf8_t. */
AFW_DEFINE(const afw_utf8_t *)
afw_array_of_utf8_get_next(
    const afw_array_t *instance,
    const afw_iterator_old_t * *iterator,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_data_type_t *data_type;

    value = afw_array_get_next_value(instance, iterator, xctx);
    if (!value) {
        return NULL;
    }

    data_type = afw_value_get_data_type(value, xctx);
    if (!data_type ||
        !afw_utf8_equal(&data_type->cType, afw_s_afw_utf8_t))
    {
        AFW_THROW_ERROR_Z(general,
            "cType of array data type is not afw_utf8_t", xctx);
    }
    return (const afw_utf8_t *)AFW_VALUE_INTERNAL(value);
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

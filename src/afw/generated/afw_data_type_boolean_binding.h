// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Data Type boolean
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_DATA_TYPE_BOOLEAN_BINDING_H__
#define __AFW_DATA_TYPE_BOOLEAN_BINDING_H__


#include "afw_minimal.h"
#include "afw_data_type_typedefs.h"
#include "afw_declare_helpers.h"

/**
 * @defgroup afw_c_api_data_type_boolean boolean
 * @ingroup afw_c_api_data_types
 *
 * Adaptive Data Type boolean
 *
 * @{
 */

/**
 * @file afw_data_type_boolean_binding.h
 * @brief Adaptive data type boolean header.
 */

AFW_BEGIN_DECLARES

/** @brief Quoted data type id for boolean */
#define AFW_DATA_TYPE_ID_Q_boolean "boolean"

/** @brief Unquoted data type id for boolean */
#define AFW_DATA_TYPE_ID_U_boolean boolean

/** @brief String for data type id boolean */
#define AFW_DATA_TYPE_ID_S_boolean afw_s_boolean

/** @brief Quoted c type for data type id boolean */
#define AFW_DATA_TYPE_CTYPE_Q_boolean "afw_boolean_t"

/** @brief Adaptive string value for "boolean". */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_data_type_boolean_id_value;

/** @brief Data type struct for boolean. */
AFW_DECLARE_CONST_DATA(afw_data_type_t *)
afw_data_type_boolean;

/**
 * @brief Unmanaged evaluated value inf for data type boolean.
 *
 * The lifetime of the value is the lifetime of its containing pool.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_unmanaged_boolean_inf;

/**
 * @brief Managed evaluated value inf for data type boolean.
 *
 * The lifetime of the value is managed by reference count in xctx->p.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_managed_boolean_inf;

/**
 * @brief Permanent (life of afw environment) value inf for data type boolean.
 *
 * The lifetime of the value is the lifetime of the afw environment.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_permanent_boolean_inf;

/**
 * @brief Macro to determine if data type is boolean.
 * @param A_DATA_TYPE to test.
 * @return boolean result.
 */
#define afw_data_type_is_boolean(A_DATA_TYPE) \
( \
    (A_DATA_TYPE) && \
    (A_DATA_TYPE) == afw_data_type_boolean \
)

/**
 * @brief Macro to determine if value is evaluated boolean.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_boolean(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_boolean\
)

/**
 * @brief Macro to determine if value is evaluated array of boolean.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_array_of_boolean(A_VALUE) \
( \
    afw_value_is_array(A_VALUE) && \
    afw_array_get_data_type( \
        ((const afw_value_array_t *)(A_VALUE))->internal, \
        xctx) == afw_data_type_boolean \
)

/**
 * @brief Convert data type boolean string to afw_boolean_t *.
 * @param to_internal place to return result.  Must be sizeof(afw_boolean_t).
 * @param from_utf8 to convert.
 * @param p to use for returned internal.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_data_type_boolean_to_internal(
    afw_boolean_t *to_internal,
    const afw_utf8_t *from_utf8,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Convert data type boolean internal representation to utf-8.
 * @param internal (afw_boolean_t) to convert.
 * @param p to use for returned string.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) normalized string representation of value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_data_type_boolean_to_utf8(
    afw_boolean_t internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/** @brief struct for data type boolean values. */
struct afw_value_boolean_s {
    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    /** @brief  Internal afw_boolean_t value. */
    afw_boolean_t internal;
};

/** @brief struct for managed data type boolean values.
 *
 * This is the same as afw_value_boolean_s with the addition of a
 * reference count. This is intended for internal use only.
 */
struct afw_value_boolean_managed_s {
    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    /** @brief  Internal afw_boolean_t value. */
    afw_boolean_t internal;

    /** @brief  Reference count for value. */
    afw_size_t reference_count;
};

/**
 * @brief Typesafe cast of data type boolean.
 * @param value (const afw_value_t *).
 * @return (afw_boolean_t)
 */
AFW_DECLARE(afw_boolean_t)
afw_value_as_boolean(
    const afw_value_t *value,
    afw_xctx_t *xctx);

/**
 * @brief Allocate function for managed data type boolean value.
 * @param internal place to put pointer to internal of value.
 * @param xctx of caller.
 * @return Allocated afw_value_t with appropriate inf set.
 *
 * This allocates memory for the value in xctx->p. Set *internal to the 
 * 'afw_boolean_t' internal value before using. The corresponding create is
 * often more appropriate to use.
 *
 * The value's lifetime is managed by reference count.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_allocate_managed_boolean(
    afw_boolean_t **internal,
    afw_xctx_t *xctx);

/**
 * @brief Allocate function for data type boolean value.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Allocated afw_value_boolean_t with appropriate inf set.
 *
 * The value's lifetime is not managed so it will last for the life of the pool.
 */
AFW_DECLARE(afw_value_boolean_t *)
afw_value_allocate_unmanaged_boolean(
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Create function for managed data type boolean value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 *
 * The value's lifetime is managed by reference count.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_managed_boolean(
    afw_boolean_t internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Create function for unmanaged data type boolean value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 *
 * The value's lifetime is not managed so it will last for the life of the pool.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_unmanaged_boolean(afw_boolean_t internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for permanent data type boolean value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_permanent_boolean(afw_boolean_t internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/* Data type boolean not yet supported for some functions. */

/**
 * @brief Get property function for data type boolean value.
 * @deprecated
 * @param object of property to get.
 * @param property_name of property to get.
 * @param found is place to return whether property is found.
 * @param xctx of caller.
 * @return afw_boolean_t.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_property_as_boolean( \
    object, property_name, found, xctx) \
afw_object_get_property_as_boolean_source( \
    object, property_name, found, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get property function for data type boolean value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param found is place to return whether property is found.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return afw_boolean_t.
 */
#define afw_object_get_property_as_boolean( \
    object, property_name, found, p, xctx) \
afw_object_get_property_as_boolean_source( \
    object, property_name, found, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type boolean value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param found is place to return whether property is found.
 * @param source_z file:line.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return afw_boolean_t.
 */
AFW_DECLARE(afw_boolean_t)
afw_object_get_property_as_boolean_source(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_boolean_t *found,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Get next property function for data type boolean value.
 * @deprecated
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param found is place to return whether property is found.
 * @param xctx of caller.
 * @return afw_boolean_t.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_next_property_as_boolean( \
    object, iterator, property_name, found, xctx) \
afw_object_get_next_property_as_boolean_source( \
    object, iterator, property_name, found, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get next property function for data type boolean value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param found is place to return whether property is found.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return afw_boolean_t.
 */
#define afw_object_get_next_property_as_boolean( \
    object, iterator, property_name, found, p, xctx) \
afw_object_get_next_property_as_boolean_source( \
    object, iterator, property_name, found, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type boolean value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param found is place to return whether property is found.
 * @param source_z file:line.
 * @param p to use for result if conversion is required.
 * @param xctx of caller.
 * @return afw_boolean_t.
 */
AFW_DECLARE(afw_boolean_t)
afw_object_get_next_property_as_boolean_source(
    const afw_object_t *object,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    afw_boolean_t *found,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Set property function for data type boolean values.
 * @param object of property to set.
 * @param property_name of property to set.
 * @param value of value to set.
 * @param xctx of caller.
 *
 * The value will be allocated in the object's pool. *
 */
AFW_DECLARE(void)
afw_object_set_property_as_boolean(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_boolean_t internal,
    afw_xctx_t *xctx);

/**
 * @brief Get next value from array of boolean.
 * @param instance of array.
 * @param iterator.
 * @param found is place to return whether value is found.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (afw_boolean_t) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
#define afw_array_of_boolean_get_next( \
    array, iterator, found, xctx) \
    afw_array_of_boolean_get_next_source( \
    array, iterator, found, AFW__FILE_LINE__, xctx)

/**
 * @brief Get next value from array of boolean.
 * @param instance of array.
 * @param iterator.
 * @param found is place to return whether value is found.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (afw_boolean_t) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
AFW_DECLARE(afw_boolean_t)
afw_array_of_boolean_get_next_source(
    const afw_array_t *instance,
    const afw_iterator_t * *iterator,
    afw_boolean_t *found,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx);

/**
 * @brief Add value from array of boolean.
 * @param instance of array.
 * @param value to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_of_boolean_add(
    const afw_array_t *instance,
    const afw_boolean_t *value,
    afw_xctx_t *xctx);

/**
 * @brief Remove value from array of boolean.
 * @param instance of array.
 * @param value to remove.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_of_boolean_remove(
    const afw_array_t *instance,
    const afw_boolean_t *value,
    afw_xctx_t *xctx);

/**
 * @brief extern for data type boolean struct.
 *
 * This should only be managed in the linkage unit the extern is
 * defined in.  Use afw_data_type_boolean when not referencing in
 * a static.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_boolean_direct;

/**
 * @brief extern for data type boolean inf.
 *
 * This should only be managed in the linkage unit the extern is
 * defined in.
 *
 * The implementation of the data type must define this.  It is
 * managed by the generated data type instance.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_inf_t)
afw_data_type_boolean_inf;

AFW_END_DECLARES

/** @} */


#endif /* __AFW_DATA_TYPE_BOOLEAN_BINDING_H__ */

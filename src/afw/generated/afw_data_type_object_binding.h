// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Data Type object
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


#ifndef __AFW_DATA_TYPE_OBJECT_BINDING_H__
#define __AFW_DATA_TYPE_OBJECT_BINDING_H__


#include "afw_minimal.h"
#include "afw_data_type_typedefs.h"
#include "afw_declare_helpers.h"

/**
 * @defgroup afw_c_api_data_type_object object
 * @ingroup afw_c_api_data_types
 *
 * Adaptive Data Type object
 *
 * @{
 */

/**
 * @file afw_data_type_object_binding.h
 * @brief Adaptive data type object header.
 */

AFW_BEGIN_DECLARES

/** @brief Quoted data type id for object */
#define AFW_DATA_TYPE_ID_Q_object "object"

/** @brief Unquoted data type id for object */
#define AFW_DATA_TYPE_ID_U_object object

/** @brief String for data type id object */
#define AFW_DATA_TYPE_ID_S_object afw_s_object

/** @brief Quoted c type for data type id object */
#define AFW_DATA_TYPE_CTYPE_Q_object "const afw_object_t *"

/** @brief Adaptive string value for "object". */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_data_type_object_id_value;

/** @brief Data type struct for object. */
AFW_DECLARE_CONST_DATA(afw_data_type_t *)
afw_data_type_object;

/**
 * @brief Unmanaged evaluated value inf for data type object.
 *
 * The lifetime of the value is the lifetime of its containing pool.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_object_inf;

/**
 * @brief Managed evaluated value inf for data type object.
 *
 * The lifetime of the value is managed by reference.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_managed_object_inf;

/**
 * @brief Permanent managed (life of afw environment) value inf for data type object.
 *
 * The lifetime of the value is the lifetime of the afw environment.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_permanent_object_inf;

/**
 * @brief Macro to determine if data type is object.
 * @param A_DATA_TYPE to test.
 * @return boolean result.
 */
#define afw_data_type_is_object(A_DATA_TYPE) \
( \
    (A_DATA_TYPE) && \
    (A_DATA_TYPE) == afw_data_type_object \
)

/**
 * @brief Macro to determine if value is evaluated object.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_object(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_object\
)

/**
 * @brief Macro to determine if value is evaluated array of object.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_array_of_object(A_VALUE) \
( \
    afw_value_is_array(A_VALUE) && \
    afw_array_get_data_type( \
        ((const afw_value_array_t *)(A_VALUE))->internal, \
        xctx) == afw_data_type_object \
)

/**
 * @brief Convert data type object string to const afw_object_t * *.
 * @param to_internal place to return result.  Must be sizeof(const afw_object_t *).
 * @param from_utf8 to convert.
 * @param p to use for returned internal.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_data_type_object_to_internal(const afw_object_t * *to_internal,
    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Convert data type object internal representation to utf-8.
 * @param internal (const afw_object_t *) to convert.
 * @param p to use for returned string.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) normalized string representation of value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_data_type_object_to_utf8(const afw_object_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/** @brief struct for data type object values. */
struct afw_value_object_s {
    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    /** @brief  Internal const afw_object_t * value. */
    const afw_object_t * internal;
};

/**
 * @brief Typesafe cast of data type object.
 * @param value (const afw_value_t *).
 * @return (const afw_object_t *)
 */
AFW_DECLARE(const afw_object_t *)
afw_value_as_object(const afw_value_t *value, afw_xctx_t *xctx);

/**
 * @brief Allocate function for managed data type object value.
 * @param internal place to put pointer to internal of value.
 * @param xctx of caller.
 * @return Allocated afw_value_t with appropriate inf set.
 *
 * This value is allocated in xctx->p. Set *internal to the 
 * 'const afw_object_t *' internal value before using.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_alloc_managed_object(
    const afw_object_t * **internal,
    afw_xctx_t *xctx);

/**
 * @brief Allocate function for data type object value.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Allocated afw_value_object_t with appropriate inf set.
 */
AFW_DECLARE(afw_value_object_t *)
afw_value_allocate_object(const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for managed data type object value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_managed_object(const afw_object_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for data type object value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_object(const afw_object_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for permanent data type object value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_permanent_object(const afw_object_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Get property function for data type object value.
 * @deprecated
 * @param object of property to get.
 * @param property_name of property to get.
 * @param xctx of caller.
 * @return const afw_object_t *.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_property_as_object( \
    object, property_name, xctx) \
afw_object_get_property_as_object_source( \
    object, property_name, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get property function for data type object value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_object_t *.
 */
#define afw_object_get_property_as_object( \
    object, property_name, p, xctx) \
afw_object_get_property_as_object_source( \
    object, property_name, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type object value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param source_z file:line.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_object_t *.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_get_property_as_object_source(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Get next property function for data type object value.
 * @deprecated
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param xctx of caller.
 * @return const afw_object_t *.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_next_property_as_object( \
    object, iterator, property_name, xctx) \
afw_object_get_next_property_as_object_source( \
    object, iterator, property_name, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get next property function for data type object value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_object_t *.
 */
#define afw_object_get_next_property_as_object( \
    object, iterator, property_name, p, xctx) \
afw_object_get_next_property_as_object_source( \
    object, iterator, property_name, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type object value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param source_z file:line.
 * @param p to use for result if conversion is required.
 * @param xctx of caller.
 * @return const afw_object_t *.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_get_next_property_as_object_source(
    const afw_object_t *object,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Set property function for data type object values.
 * @param object of property to set.
 * @param property_name of property to set.
 * @param value of value to set.
 * @param xctx of caller.
 *
 * The value will be allocated in the object's pool. *
 */
AFW_DECLARE(void)
afw_object_set_property_as_object(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_object_t * internal,
    afw_xctx_t *xctx);

/**
 * @brief Get next value from array of object.
 * @param instance of array.
 * @param iterator.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (const afw_object_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
#define afw_array_of_object_get_next( \
    array, iterator, xctx) \
    afw_array_of_object_get_next_source( \
    array, iterator, AFW__FILE_LINE__, xctx)

/**
 * @brief Get next value from array of object.
 * @param instance of array.
 * @param iterator.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (const afw_object_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
AFW_DECLARE(const afw_object_t *)
afw_array_of_object_get_next_source(
    const afw_array_t *instance,
    const afw_iterator_t * *iterator,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx);

/**
 * @brief Add value from array of object.
 * @param instance of array.
 * @param value to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_of_object_add(
    const afw_array_t *instance,
    const afw_object_t *value,
    afw_xctx_t *xctx);

/**
 * @brief Remove value from array of object.
 * @param instance of array.
 * @param value to remove.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_of_object_remove(
    const afw_array_t *instance,
    const afw_object_t *value,
    afw_xctx_t *xctx);

/**
 * @brief extern for data type object struct.
 *
 * This should only be managed in the linkage unit the extern is
 * defined in.  Use afw_data_type_object when not referencing in
 * a static.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_object_direct;

/**
 * @brief extern for data type object inf.
 *
 * This should only be managed in the linkage unit the extern is
 * defined in.
 *
 * The implementation of the data type must define this.  It is
 * managed by the generated data type instance.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_inf_t)
afw_data_type_object_inf;

AFW_END_DECLARES

/** @} */


#endif /* __AFW_DATA_TYPE_OBJECT_BINDING_H__ */

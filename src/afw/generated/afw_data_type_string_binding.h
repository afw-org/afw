// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Data Type string
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


#ifndef __AFW_DATA_TYPE_STRING_BINDING_H__
#define __AFW_DATA_TYPE_STRING_BINDING_H__


#include "afw_minimal.h"
#include "afw_data_type_typedefs.h"
#include "afw_declare_helpers.h"

/**
 * @defgroup afw_c_api_data_type_string string
 * @ingroup afw_c_api_data_types
 *
 * Adaptive Data Type string
 *
 * @{
 */

/**
 * @file afw_data_type_string_binding.h
 * @brief Adaptive data type string header.
 */

AFW_BEGIN_DECLARES

/** @brief Quoted data type id for string */
#define AFW_DATA_TYPE_ID_Q_string "string"

/** @brief Unquoted data type id for string */
#define AFW_DATA_TYPE_ID_U_string string

/** @brief String for data type id string */
#define AFW_DATA_TYPE_ID_S_string afw_s_string

/** @brief Quoted c type for data type id string */
#define AFW_DATA_TYPE_CTYPE_Q_string "afw_utf8_t"

/** @brief Adaptive string value for "string". */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_data_type_string_id_value;

/** @brief Data type struct for string. */
AFW_DECLARE_CONST_DATA(afw_data_type_t *)
afw_data_type_string;

/**
 * @brief Unmanaged evaluated value inf for data type string.
 *
 * The lifetime of the value is the lifetime of its containing pool.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_string_inf;

/**
 * @brief Managed evaluated value inf for data type string.
 *
 * The lifetime of the value is managed by reference.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_referenced_string_inf;

/**
 * @brief Permanent managed (life of afw environment) value inf for data type string.
 *
 * The lifetime of the value is the lifetime of the afw environment.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_permanent_string_inf;

/**
 * @brief Macro to determine if data type is string.
 * @param A_DATA_TYPE to test.
 * @return boolean result.
 */
#define afw_data_type_is_string(A_DATA_TYPE) \
( \
    (A_DATA_TYPE) && \
    (A_DATA_TYPE) == afw_data_type_string \
)

/**
 * @brief Macro to determine if value is evaluated string.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_string(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_string\
)

/**
 * @brief Macro to determine if value is evaluated array of string.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_array_of_string(A_VALUE) \
( \
    afw_value_is_array(A_VALUE) && \
    afw_array_get_data_type( \
        ((const afw_value_array_t *)(A_VALUE))->internal, \
        xctx) == afw_data_type_string \
)

/**
 * @brief Convert data type string string to afw_utf8_t *.
 * @param to_internal place to return result.  Must be sizeof(afw_utf8_t).
 * @param from_utf8 to convert.
 * @param p to use for returned internal.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_data_type_string_to_internal(afw_utf8_t *to_internal,
    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Convert data type string internal representation to utf-8.
 * @param internal (const afw_utf8_t *) to convert.
 * @param p to use for returned string.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) normalized string representation of value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_data_type_string_to_utf8(const afw_utf8_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/** @brief struct for data type string values. */
struct afw_value_string_s {
    /** @brief  Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };

    /** @brief  Internal afw_utf8_t value. */
    afw_utf8_t internal;
};

/**
 * @brief Typesafe cast of data type string.
 * @param value (const afw_value_t *).
 * @return (const afw_utf8_t *)
 */
AFW_DECLARE(const afw_utf8_t *)
afw_value_as_string(const afw_value_t *value, afw_xctx_t *xctx);

/**
 * @brief Allocate function for referenced data type string value.
 * @param s place to put pointer to allocated memory for internal->s.
 * @param len of memory to allocate for internal->s.
 * @param xctx of caller.
 * @return Allocated afw_value_string_t with appropriate inf set.
 *
 * This value and memory for the specified len is allocated in xctx->p.
 * Set *s for the specified len to a valid utf-8 string.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_alloc_referenced_string(
    afw_utf8_octet_t **s,
    afw_size_t len,
    afw_xctx_t *xctx);

/**
 * @brief Allocate function for unmanaged data type string value.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Allocated afw_value_string_t with appropriate inf set.
 */
AFW_DECLARE(afw_value_string_t *)
afw_value_allocate_string(const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for referenced data type string value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_referenced_string(const afw_utf8_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for referenced data type string slice value.
 * @param containing_value with a cType of 'afw_utf8_t'.
 * @param offset in contain value's internal.
 * @param len of slice.
 * @param xctx of caller.
 * @return  Created const afw_value_t *.
 *
 * This value and memory for the specified len is allocated in xctx->p.
 * Set *s for the specified len to a valid utf-8 string.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_referenced_string_slice(
    const afw_value_t *containing_value,
    afw_size_t offset,
    afw_size_t len,
    afw_xctx_t *xctx);

/**
 * @brief Create function for unmanaged data type string value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_string(const afw_utf8_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for permanent data type string value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_permanent_string(const afw_utf8_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Get property function for data type string value.
 * @deprecated
 * @param object of property to get.
 * @param property_name of property to get.
 * @param xctx of caller.
 * @return const afw_utf8_t *.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_property_as_string( \
    object, property_name, xctx) \
afw_object_get_property_as_string_source( \
    object, property_name, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get property function for data type string value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_utf8_t *.
 */
#define afw_object_get_property_as_string( \
    object, property_name, p, xctx) \
afw_object_get_property_as_string_source( \
    object, property_name, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type string value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param source_z file:line.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_utf8_t *.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_get_property_as_string_source(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Get next property function for data type string value.
 * @deprecated
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param xctx of caller.
 * @return const afw_utf8_t *.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_next_property_as_string( \
    object, iterator, property_name, xctx) \
afw_object_get_next_property_as_string_source( \
    object, iterator, property_name, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get next property function for data type string value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_utf8_t *.
 */
#define afw_object_get_next_property_as_string( \
    object, iterator, property_name, p, xctx) \
afw_object_get_next_property_as_string_source( \
    object, iterator, property_name, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type string value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param source_z file:line.
 * @param p to use for result if conversion is required.
 * @param xctx of caller.
 * @return const afw_utf8_t *.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_get_next_property_as_string_source(
    const afw_object_t *object,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Set property function for data type string values.
 * @param object of property to set.
 * @param property_name of property to set.
 * @param value of value to set.
 * @param xctx of caller.
 *
 * The value will be allocated in the object's pool. *
 */
AFW_DECLARE(void)
afw_object_set_property_as_string(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_utf8_t * internal,
    afw_xctx_t *xctx);

/**
 * @brief Get next value from array of string.
 * @param instance of array.
 * @param iterator.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
#define afw_array_of_string_get_next( \
    array, iterator, xctx) \
    afw_array_of_string_get_next_source( \
    array, iterator, AFW__FILE_LINE__, xctx)

/**
 * @brief Get next value from array of string.
 * @param instance of array.
 * @param iterator.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_array_of_string_get_next_source(
    const afw_array_t *instance,
    const afw_iterator_t * *iterator,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx);

/**
 * @brief Add value from array of string.
 * @param instance of array.
 * @param value to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_of_string_add(
    const afw_array_t *instance,
    const afw_utf8_t *value,
    afw_xctx_t *xctx);

/**
 * @brief Remove value from array of string.
 * @param instance of array.
 * @param value to remove.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_of_string_remove(
    const afw_array_t *instance,
    const afw_utf8_t *value,
    afw_xctx_t *xctx);

/**
 * @brief extern for data type string struct.
 *
 * This should only be referenced in the linkage unit the extern is
 * defined in.  Use afw_data_type_string when not referencing in
 * a static.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_string_direct;

/**
 * @brief extern for data type string inf.
 *
 * This should only be referenced in the linkage unit the extern is
 * defined in.
 *
 * The implementation of the data type must define this.  It is
 * referenced by the generated data type instance.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_inf_t)
afw_data_type_string_inf;

AFW_END_DECLARES

/** @} */


#endif /* __AFW_DATA_TYPE_STRING_BINDING_H__ */

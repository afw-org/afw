// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Data Type date
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


#ifndef __AFW_DATA_TYPE_DATE_BINDING_H__
#define __AFW_DATA_TYPE_DATE_BINDING_H__


#include "afw_minimal.h"
#include "afw_data_type_typedefs.h"
#include "afw_declare_helpers.h"

/**
 * @defgroup afw_c_api_data_type_date date
 * @ingroup afw_c_api_data_types
 *
 * Adaptive Data Type date
 *
 * @{
 */

/**
 * @file afw_data_type_date_binding.h
 * @brief Adaptive data type date header.
 */

AFW_BEGIN_DECLARES

/** @brief Quoted data type id for date */
#define AFW_DATA_TYPE_ID_Q_date "date"

/** @brief Unquoted data type id for date */
#define AFW_DATA_TYPE_ID_U_date date

/** @brief String for data type id date */
#define AFW_DATA_TYPE_ID_S_date afw_s_date

/** @brief Quoted c type for data type id date */
#define AFW_DATA_TYPE_CTYPE_Q_date "afw_date_t"

/** @brief Adaptive string value for "date". */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_data_type_date_id_value;

/** @brief Data type struct for date. */
AFW_DECLARE_CONST_DATA(afw_data_type_t *)
afw_data_type_date;

/**
 * @brief Unmanaged evaluated value inf for data type date.
 *
 * The lifetime of the value is the lifetime of its containing pool.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_evaluated_date_inf;

/**
 * @brief Managed evaluated value inf for data type date.
 *
 * The lifetime of the value is managed by reference.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_managed_date_inf;

/**
 * @brief Permanent managed (life of afw environment) value inf for data type date.
 *
 * The lifetime of the value is the lifetime of the afw environment.
 */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_permanent_date_inf;

/**
 * @brief Macro to determine if data type is date.
 * @param A_DATA_TYPE to test.
 * @return boolean result.
 */
#define afw_data_type_is_date(A_DATA_TYPE) \
( \
    (A_DATA_TYPE) && \
    (A_DATA_TYPE) == afw_data_type_date \
)

/**
 * @brief Macro to determine if value is evaluated date.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_date(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_date\
)

/**
 * @brief Macro to determine if value is evaluated list of date.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_list_of_date(A_VALUE) \
( \
    afw_value_is_list(A_VALUE) && \
    afw_list_get_data_type( \
        ((const afw_value_list_t *)(A_VALUE))->internal, \
        xctx) == afw_data_type_date \
)

/**
 * @brief Convert data type date string to afw_date_t *.
 * @param to_internal place to return result.  Must be sizeof(afw_date_t).
 * @param from_utf8 to convert.
 * @param p to use for returned internal.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_data_type_date_to_internal(afw_date_t *to_internal,
    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Convert data type date internal representation to utf-8.
 * @param internal (const afw_date_t *) to convert.
 * @param p to use for returned string.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) normalized string representation of value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_data_type_date_to_utf8(const afw_date_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/** @brief struct for data type date values. */
struct afw_value_date_s {
    const afw_value_inf_t *inf;
    afw_date_t internal;
};

/**
 * @brief Typesafe cast of data type date.
 * @param value (const afw_value_t *).
 * @return (const afw_date_t *)
 */
AFW_DECLARE(const afw_date_t *)
afw_value_as_date(const afw_value_t *value, afw_xctx_t *xctx);

/**
 * @brief Allocate function for unmanaged data type date value.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Allocated afw_value_date_t with appropriate inf set.
 */
AFW_DECLARE(afw_value_date_t *)
afw_value_allocate_date(const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for unmanaged data type date value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_date(const afw_date_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Allocate function for managed data type date value.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Allocated afw_value_date_t with appropriate inf set.
 */
AFW_DECLARE(afw_value_date_t *)
afw_value_allocate_managed_date(const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for managed data type date value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_managed_date(const afw_date_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Allocate function for permanent data type date value.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Allocated afw_value_date_t with appropriate inf set.
 */
AFW_DECLARE(afw_value_date_t *)
afw_value_allocate_permanent_date(const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Create function for permanent data type date value.
 * @param internal.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return Created const afw_value_t *.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_permanent_date(const afw_date_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Get property function for data type date value.
 * @deprecated
 * @param object of property to get.
 * @param property_name of property to get.
 * @param xctx of caller.
 * @return const afw_date_t *.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_property_as_date( \
    object, property_name, xctx) \
afw_object_get_property_as_date_source( \
    object, property_name, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get property function for data type date value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_date_t *.
 */
#define afw_object_get_property_as_date( \
    object, property_name, p, xctx) \
afw_object_get_property_as_date_source( \
    object, property_name, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type date value.
 * @param object of property to get.
 * @param property_name of property to get.
 * @param source_z file:line.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_date_t *.
 */
AFW_DECLARE(const afw_date_t *)
afw_object_get_property_as_date_source(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Get next property function for data type date value.
 * @deprecated
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param xctx of caller.
 * @return const afw_date_t *.
 *
 * This is a deprecated function used to get around an exception that
 * was occurring when an object did not have a pool. Use the function
 * without an "_old" in the name for all new code and replace calls in
 * old code when possible.
 *
 */
#define afw_object_old_get_next_property_as_date( \
    object, iterator, property_name, xctx) \
afw_object_get_next_property_as_date_source( \
    object, iterator, property_name, AFW__FILE_LINE__, \
    ((object)->p ? (object)->p : (xctx)->p), (xctx))

/**
 * @brief Get next property function for data type date value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param p to use for result if evaluation or conversion is required.
 * @param xctx of caller.
 * @return const afw_date_t *.
 */
#define afw_object_get_next_property_as_date( \
    object, iterator, property_name, p, xctx) \
afw_object_get_next_property_as_date_source( \
    object, iterator, property_name, AFW__FILE_LINE__, p, xctx)

/**
 * @brief Get property function for data type date value.
 * @param object of property to get.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param source_z file:line.
 * @param p to use for result if conversion is required.
 * @param xctx of caller.
 * @return const afw_date_t *.
 */
AFW_DECLARE(const afw_date_t *)
afw_object_get_next_property_as_date_source(
    const afw_object_t *object,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Set property function for data type date values.
 * @param object of property to set.
 * @param property_name of property to set.
 * @param value of value to set.
 * @param xctx of caller.
 *
 * The value will be allocated in the object's pool. *
 */
AFW_DECLARE(void)
afw_object_set_property_as_date(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_date_t * internal,
    afw_xctx_t *xctx);

/**
 * @brief Get next value from list of date.
 * @param instance of list.
 * @param iterator.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (const afw_date_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
#define afw_list_of_date_get_next( \
    list, iterator, xctx) \
    afw_list_of_date_get_next_source( \
    list, iterator, AFW__FILE_LINE__, xctx)

/**
 * @brief Get next value from list of date.
 * @param instance of list.
 * @param iterator.
 * @param source_z file:line.
 * @param xctx of caller.
 * @return (const afw_date_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
AFW_DECLARE(const afw_date_t *)
afw_list_of_date_get_next_source(
    const afw_list_t *instance,
    const afw_iterator_t * *iterator,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx);

/**
 * @brief Add value from list of date.
 * @param instance of list.
 * @param value to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_of_date_add(
    const afw_list_t *instance,
    const afw_date_t *value,
    afw_xctx_t *xctx);

/**
 * @brief Remove value from list of date.
 * @param instance of list.
 * @param value to remove.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_of_date_remove(
    const afw_list_t *instance,
    const afw_date_t *value,
    afw_xctx_t *xctx);

/**
 * @brief extern for data type date struct.
 *
 * This should only be referenced in the linkage unit the extern is
 * defined in.  Use afw_data_type_date when not referencing in
 * a static.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_date_direct;

/**
 * @brief extern for data type date inf.
 *
 * This should only be referenced in the linkage unit the extern is
 * defined in.
 *
 * The implementation of the data type must define this.  It is
 * referenced by the generated data type instance.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_data_type_inf_t)
afw_data_type_date_inf;

AFW_END_DECLARES

/** @} */


#endif /* __AFW_DATA_TYPE_DATE_BINDING_H__ */

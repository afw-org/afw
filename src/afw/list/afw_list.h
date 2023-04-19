// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_list interface helper header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LIST_H__
#define __AFW_LIST_H__

/**
 * @defgroup afw_list List
 * @ingroup afw_c_api_public
 *
 * List API.
 * @{
 */

/**
 * @file afw_list.h
 * @brief Adaptive Framework afw_list interface helper header
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

#define AFW_LIST_ERROR_OBJECT_IMMUTABLE \
AFW_THROW_ERROR_Z(read_only, "List immutable", xctx)


AFW_DECLARE(afw_size_t)
afw_list_count(const afw_list_t *list, afw_xctx_t *xctx);



/**
 * @brief Create an list in memory with options.
 * @param options
 * @param data_type if list only holds one data type or NULL.
 * @param p to use for the list.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value list.
 * 
 * If data_type is NULL and only values of a single evaluated
 * data type are added, afw_list_get_data_type() will return
 * that data type.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_create_with_options(
    int options,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create an list of a specific data type in memory.
 * @param data_type if list only holds one data type or NULL.
 * @param p to use for the list.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value list.
 *
 * If data_type is NULL and only values of a single evaluated
 * data type are added, afw_list_get_data_type() will return
 * that data type.
 */
#define afw_list_of_create(data_type, p, xctx) \
    afw_list_create_with_options(0, data_type, p, xctx)



/**
 * @brief Create an value list in memory.
 * @param p to use for the object.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value list.
 *
 * Use this function to create a value list that is not associated with an
 * adaptor.
 * 
 * If only values of a single evaluated data type are added,
 * afw_list_get_data_type() will return that data type.
 */
#define afw_list_create_generic(p, xctx) \
    afw_list_create_with_options(0, NULL, p, xctx)



/**
 * @brief Create an immutable list from an array of objects.
 * @param values is address of first value in array.
 * @param count is number for values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return list instance.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_const_create_array_of_objects(
    const afw_object_t *const *objects,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an immutable list from an array of values.
 * @param values is address of first value in array.
 * @param count is number for values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return list instance.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_const_create_array_of_values(
    const afw_value_t *const *values,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an immutable list from NULL terminated array of objects.
 * @param values is NULL terminated array of objects.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return list instance.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_const_create_null_terminated_array_of_objects(
    const afw_object_t *const *objects,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an immutable list from NULL terminated array of values.
 * @param values is NULL terminated array of values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return list instance.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_const_create_null_terminated_array_of_values(
    const afw_value_t *const *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a clone of a list in memory.
 * @param list to clone or NULL.
 * @param data_type to use if list is NULL.
 * @param clone_values true will clone values as well.
 * @param p to use for the list.
 * @param xctx of caller.
 * @return cloned instance of list.
 *
 * If data_type is not NULL and list is typed, they must match.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_create_or_clone(
    const afw_list_t *list,
    const afw_data_type_t *data_type,
    afw_boolean_t clone_values,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a typed list from a value.
 * @param data_type of list
 * @param value.
 * @param p to use for the list.
 * @param xctx of caller.
 * @return typed list.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_of_create_from_value(
    const afw_data_type_t *data_type,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert a list to a list of strings.
 * @param list to convert.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return converted list or original if already list of strings.
 */
AFW_DECLARE(const afw_list_t *)
afw_list_convert_to_list_of_strings(
    const afw_list_t *list,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a immutable list wrapper for an array.
 * @param array of internal values.
 * @param indirect if true array is array of pointers to internal value.
 * @param data_type of array.
 * @param count of entries in array or -1 for NULL terminated.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return list instance.
 *
 * If indirect is false, array must be an array of data_type->cType.  If
 * indirect is true, array must be an array of data_type->cType *.
 *
 * If count is -1, the array must be a list of pointers.  This can either
 * be because data_type->cType ends in an asterisk or the indirect parameter
 * is true.
 */
AFW_DEFINE(const afw_list_t *)
afw_list_create_wrapper_for_array(
    const void *array,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Self for immutable list wrapper for a array
 *
 * This is self for immutable list wrapper for a array.  This can defined
 * and filled out in a local variable instead of calling
 * afw_list_create_wrapper_for_array() if the instance is only needed
 * temporarily.  Helper macro AFW_LIST_WRAPPER_FOR_ARRAY_P() can be used to do
 * this.
 */
typedef struct afw_list_wrapper_for_array_self_s {
    const afw_list_inf_t *inf;
    const afw_data_type_t *data_type;
    afw_size_t count;
    const void *internal;
    const afw_pool_t *p;
    afw_boolean_t indirect;
} afw_list_wrapper_for_array_self_t;



/**
 * @brief inf for afw_list_wrapper_for_array afw_list implementation.
 *
 * See AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY() macro.
 */
AFW_DECLARE_CONST_DATA(afw_list_inf_t)
afw_list_wrapper_for_array_inf;



/**
 * @brief Helper macro to fill out afw_list_wrapper_for_array_self_t.
 * @brief instance is pointer to afw_list_wrapper_for_array_self_t.
 * @param _internal array of internal values.
 * @param _indirect if true array is array of pointers to internal value.
 * @param _data_type of array.
 * @param _count of entries in array or -1 for NULL terminated.
 *
 * If method afw_list_get_next_value() will be called, use helper macro
 * AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY_P() instead.  Calling
 * afw_list_get_next() will work fine since it does not allocate memory.
 *
 * Example:
 *
 *   afw_list_wrapper_for_array_self_t wrapper;
 *
 *   AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY(&wrapper, array, data_type, count);
 */
#define AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY( \
instance, _internal, _indirect, _data_type, _count) \
    (instance)->inf = &afw_list_wrapper_for_array_inf; \
    (instance)->p = NULL; \
    (instance)->internal = _internal; \
    (instance)->indirect = _indirect; \
    (instance)->data_type = _data_type; \
    (instance)->count = _count


/**
 * @brief Helper macro to fill out afw_list_wrapper_for_array_self_t.
 * @brief instance is pointer to afw_list_wrapper_for_array_self_t.
 * @param _internal array of internal values.
 * @param _indirect if true array is array of pointers to internal value.
 * @param _data_type of array.
 * @param _count of entries in array or -1 for NULL terminated.
 * @param _p for use by method afw_list_get_next_value().
 *
 * If possible, use AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY() instead.
 */
#define AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY_P(instance, \
_internal, _indirect, _data_type, _count, _p) \
    (instance)->inf = &afw_list_wrapper_for_array_inf; \
    (instance)->p = _p; \
    (instance)->internal = _internal; \
    (instance)->indirect = _indirect; \
    (instance)->data_type = _data_type; \
    (instance)->count = _count



/**
 * @brief Get next value from list whose data type cType is afw_utf8_t.
 * @param instance of list.
 * @param iterator.
 * @param xctx of caller.
 * @return (const afw_utf8_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_list_of_utf8_get_next(
    const afw_list_t *instance,
    const afw_iterator_t * *iterator,
    afw_xctx_t *xctx);



/**
 * @brief Set a list to immutable if not already.
 * @param instance Pointer to this value list instance.
 * @param xctx of caller.
 *
 * Once a list is set to immutable, all other set calls will throw
 * an error.
 */
AFW_DECLARE(void)
afw_list_set_immutable(
    const afw_list_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Set list to immutable and determine data type of entries. 
 * @param instance Pointer to this value list instance.
 * @param xctx of caller.
 * @return the data type of all values in list or NULL if different
 *    or unknown.
 *
 * If data type of values in list is not already known and all
 * values is list have the same data type, make the list a list of
 * that data type.
 *
 * Once a list is set to immutable, all other set calls will throw
 * an error.
 */
AFW_DECLARE(const afw_data_type_t *)
afw_list_determine_data_type_and_set_immutable(
    const afw_list_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Call method add of interface afw_list_setter
 * @param instance Pointer to this value list instance.
 * @param data_type of internal.
 * @param internal A value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_add_internal(
    const afw_list_t *instance,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx);



/**
 * @brief Call method add_value of interface afw_list_setter
 * @param instance Pointer to this value list instance.
 * @param value A value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_add_value(
    const afw_list_t *instance,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Call method remove_all_values of interface afw_list_setter
 * @param instance Pointer to this value list instance.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_remove_all_values(
    const afw_list_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Call method remove of interface afw_list_setter
 * @param instance Pointer to this value list instance.
 * @param data_type of internal.
 * @param internal A value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_remove_internal(
    const afw_list_t *instance,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx);



/**
 * @brief Call method remove_value of interface afw_list_setter
 * @param instance Pointer to this value list instance.
 * @param value Value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_remove_value(
    const afw_list_t *instance,
    const afw_value_t *value,
    afw_xctx_t *xctx);


/**
 * @brief Call method set_value_by_index of interface afw_list_setter
 * @param instance Pointer to this value list instance.
 * @param index.
 * @param value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_list_set_value_by_index(
    const afw_list_t *instance,
    afw_size_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx);


/**
 * @brief Determine if list is immutable.
 * @param instance of list.
 * @param xctx of caller.
 * @return boolean result.
 */
#define afw_list_is_immutable(list, xctx) \
(afw_list_get_setter(list, xctx) == NULL)



AFW_END_DECLARES

/** @} */

#endif /* __AFW_LIST_H__ */

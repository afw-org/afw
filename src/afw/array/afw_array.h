// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_array interface helper header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ARRAY_H__
#define __AFW_ARRAY_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_array
 * @{
 */

/**
 * @file afw_array.h
 * @brief Adaptive Framework afw_array interface helper header
 *
 * See @ref afw_array.
 */

AFW_BEGIN_DECLARES

#define AFW_LIST_ERROR_OBJECT_IMMUTABLE \
AFW_THROW_ERROR_Z(read_only, "List immutable", xctx)


/**
 * @brief Create an array in memory with options.
 * @param options
 * @param data_type if array only holds one data type or NULL.
 * @param p to use for the array.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value array.
 * 
 * If data_type is NULL and only values of a single evaluated
 * data type are added, afw_array_get_data_type() will return
 * that data type.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_with_options(
    int options,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a memory array that wraps another array (mutable face).
 * @param options reserved (same convention as create_with_options; usually 0).
 * @param wrapped base array for isolation. Required (non-NULL). Any
 *     afw_array implementation is allowed (memory, adapter-backed, view of
 *     C array, …).
 * @param p to use for the face.
 * @param xctx of caller.
 * @return instance of new wrapper array.
 *
 * The face materializes entry value pointers into a local ring so mutators
 * only touch the face. Nested mutable objects/arrays are promoted to nested
 * faces on get. Sets never write to @p wrapped (issue #17).
 *
 * Not the same as afw_array_create_view_of_c_array (immutable C storage view).
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_wrapper_with_options(
    int options,
    const afw_array_t *wrapped,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a memory wrapper over another array (options 0).
 */
#define afw_array_create_wrapper(wrapped, p, xctx) \
    afw_array_create_wrapper_with_options(0, wrapped, p, xctx)


/**
 * @brief Create an unmanaged memory wrapper over another array.
 * Same as create_wrapper for current memory arrays (pool-owned face).
 */
#define afw_array_create_wrapper_unmanaged(wrapped, p, xctx) \
    afw_array_create_wrapper_with_options(0, wrapped, p, xctx)


/**
 * @brief True if array is a memory wrapper face (create_wrapper_*).
 * @param array to test (may be NULL).
 * @return true if create_wrapper_* produced this instance.
 *
 * Used by wrap_literal_array for idempotent wrap (issue #17).
 */
AFW_DECLARE(afw_boolean_t)
afw_array_is_memory_wrapper(const afw_array_t *array);



/**
 * @brief Create an array of a specific data type in memory.
 * @param data_type if array only holds one data type or NULL.
 * @param p to use for the array.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value array.
 *
 * If data_type is NULL and only values of a single evaluated
 * data type are added, afw_array_get_data_type() will return
 * that data type.
 */
#define afw_array_of_create(data_type, p, xctx) \
    afw_array_create_with_options(0, data_type, p, xctx)



/**
 * @brief Create an value array in memory.
 * @param p to use for the object.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value array.
 *
 * Use this function to create a value array that is not associated with an
 * adapter.
 * 
 * If only values of a single evaluated data type are added,
 * afw_array_get_data_type() will return that data type.
 */
#define afw_array_create_generic(p, xctx) \
    afw_array_create_with_options(0, NULL, p, xctx)



/**
 * @brief Create an immutable array from an array of objects.
 * @param objects is address of first object in array.
 * @param count is number of objects.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_const_create_array_of_objects(
    const afw_object_t *const *objects,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an immutable array from an array of values.
 * @param values is address of first value in array.
 * @param count is number for values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_const_create_array_of_values(
    const afw_value_t *const *values,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an immutable array from NULL terminated array of objects.
 * @param objects is NULL terminated array of objects.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_const_create_null_terminated_array_of_objects(
    const afw_object_t *const *objects,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an immutable array from NULL terminated array of values.
 * @param values is NULL terminated array of values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_const_create_null_terminated_array_of_values(
    const afw_value_t *const *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a clone of an array in memory.
 * @param array to clone or NULL.
 * @param data_type to use if array is NULL.
 * @param clone_values true will clone values as well.
 * @param p to use for the array.
 * @param xctx of caller.
 * @return cloned instance of array.
 *
 * If data_type is not NULL and array is typed, they must match.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_or_clone(
    const afw_array_t *array,
    const afw_data_type_t *data_type,
    afw_boolean_t clone_values,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a typed array from a value.
 * @param data_type of array
 * @param value
 * @param p to use for the array.
 * @param xctx of caller.
 * @return typed array.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_of_create_from_value(
    const afw_data_type_t *data_type,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert an array to an array of strings.
 * @param array to convert.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return converted array or original if already array of strings.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_convert_to_array_of_strings(
    const afw_array_t *array,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create an immutable afw_array that views a C array of internals.
 * @param array of internal values (or pointers to internals if indirect).
 * @param indirect if true, array is array of pointers to internal values.
 * @param data_type of each element.
 * @param count of entries in array or -1 for NULL-terminated pointer list.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance (immutable view; does not copy the C storage).
 *
 * This is **not** a mutable look-through face over another adaptive array
 * (reserved naming: create_wrapper_* for #17-style faces). It only exposes
 * existing C memory as an `afw_array_t`.
 *
 * If indirect is false, array must be an array of data_type->cType.  If
 * indirect is true, array must be an array of data_type->cType *.
 *
 * If count is -1, the array must be an array of pointers.  This can either
 * be because data_type->cType ends in an asterisk or the indirect parameter
 * is true.
 */
AFW_DEFINE(const afw_array_t *)
afw_array_create_view_of_c_array(
    const void *array,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Self for immutable afw_array view of a C array.
 *
 * Can be defined and filled out in a local variable instead of calling
 * afw_array_create_view_of_c_array() if the instance is only needed
 * temporarily. Helper macro AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY_P() can be
 * used to do this.
 */
typedef struct afw_array_view_of_c_array_self_s {
    afw_array_t pub;
    const afw_data_type_t *data_type;
    afw_size_t count;
    const void *internal;
    afw_boolean_t indirect;
} afw_array_view_of_c_array_self_t;



/**
 * @brief inf for afw_array_view_of_c_array afw_array implementation.
 *
 * See AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY() macro.
 */
AFW_DECLARE_CONST_DATA(afw_array_inf_t)
afw_array_view_of_c_array_inf;



/**
 * @brief Helper macro to fill out afw_array_view_of_c_array_self_t.
 * @param instance is pointer to afw_array_view_of_c_array_self_t.
 * @param _internal array of internal values.
 * @param _indirect if true array is array of pointers to internal value.
 * @param _data_type of array.
 * @param _count of entries in array or -1 for NULL terminated.
 *
 * If method afw_array_get_next_value() will be called, use helper macro
 * AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY_P() instead.  Calling
 * afw_array_get_next() will work fine since it does not allocate memory.
 *
 * Example:
 *
 *   afw_array_view_of_c_array_self_t view;
 *
 *   AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY(&view, array, data_type, count);
 */
#define AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY( \
instance, _internal, _indirect, _data_type, _count) \
    (instance)->pub.inf = &afw_array_view_of_c_array_inf; \
    (instance)->pub.p = NULL; \
    (instance)->internal = _internal; \
    (instance)->indirect = _indirect; \
    (instance)->data_type = _data_type; \
    (instance)->count = _count


/**
 * @brief Helper macro to fill out afw_array_view_of_c_array_self_t.
 * @brief instance is pointer to afw_array_view_of_c_array_self_t.
 * @param _internal array of internal values.
 * @param _indirect if true array is array of pointers to internal value.
 * @param _data_type of array.
 * @param _count of entries in array or -1 for NULL terminated.
 * @param _p for use by method afw_array_get_next_value().
 *
 * If possible, use AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY() instead.
 */
#define AFW_ARRAY_INITIALIZE_VIEW_OF_C_ARRAY_P(instance, \
_internal, _indirect, _data_type, _count, _p) \
    (instance)->pub.inf = &afw_array_view_of_c_array_inf; \
    (instance)->pub.p = _p; \
    (instance)->internal = _internal; \
    (instance)->indirect = _indirect; \
    (instance)->data_type = _data_type; \
    (instance)->count = _count



/**
 * @brief Get next value from array whose data type cType is afw_utf8_t.
 * @param instance of array.
 * @param iterator
 * @param xctx of caller.
 * @return (const afw_utf8_t *) or NULL.
 * 
 * Set the iterator to NULL before the first call and anytime
 * you want to start from the first value again.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_array_of_utf8_get_next(
    const afw_array_t *instance,
    const afw_iterator_t * *iterator,
    afw_xctx_t *xctx);



/**
 * @brief Set an array to immutable if not already.
 * @param instance Pointer to this value array instance.
 * @param xctx of caller.
 *
 * Once an array is set to immutable, all other set calls will throw
 * an error.
 */
AFW_DECLARE(void)
afw_array_set_immutable(
    const afw_array_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Set array to immutable and determine data type of entries. 
 * @param instance Pointer to this value array instance.
 * @param xctx of caller.
 * @return the data type of all values in array or NULL if different
 *    or unknown.
 *
 * If data type of values in array is not already known and all
 * values is array have the same data type, make the array an array of
 * that data type.
 *
 * Once an array is set to immutable, all other set calls will throw
 * an error.
 */
AFW_DECLARE(const afw_data_type_t *)
afw_array_determine_data_type_and_set_immutable(
    const afw_array_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Append value at end (push back / enqueue).
 * @param instance Pointer to this value array instance.
 * @param value A value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_push_value(
    const afw_array_t *instance,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Append internal value at end (typed push).
 * @param instance Pointer to this value array instance.
 * @param data_type of internal.
 * @param internal A value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_push_internal(
    const afw_array_t *instance,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx);



/**
 * @brief Remove and return last value (pop back).
 * @param instance Pointer to this value array instance.
 * @param found Optional; if non-NULL, true if an element was removed.
 * @param xctx of caller.
 * @return Removed value (not cloned), or NULL if empty (or stored NULL).
 *
 * Pass found=NULL when empty vs removed-NULL need not be distinguished.
 * Empty is found==false; a removed NULL/undefined slot is found==true.
 */
AFW_DECLARE(const afw_value_t *)
afw_array_pop_value(
    const afw_array_t *instance,
    afw_boolean_t *found,
    afw_xctx_t *xctx);



/**
 * @brief Remove and return first value (shift / dequeue).
 * @param instance Pointer to this value array instance.
 * @param found Optional; if non-NULL, true if an element was removed.
 * @param xctx of caller.
 * @return Removed value (not cloned), or NULL if empty (or stored NULL).
 *
 * Pass found=NULL when empty vs removed-NULL need not be distinguished.
 * Empty is found==false; a removed NULL/undefined slot is found==true.
 */
AFW_DECLARE(const afw_value_t *)
afw_array_shift_value(
    const afw_array_t *instance,
    afw_boolean_t *found,
    afw_xctx_t *xctx);



/**
 * @brief Insert value before index (signed index; 0 = front / unshift).
 * @param instance Pointer to this value array instance.
 * @param index Zero-based or negative from end; 0 inserts at front.
 * @param value A value.
 * @param xctx of caller.
 *
 * There is no separate afw_array_unshift_value helper. Unshift is:
 *   afw_array_insert_value(array, 0, value, xctx);
 * Index equal to the current count appends (same as push_value).
 */
AFW_DECLARE(void)
afw_array_insert_value(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Replace value at index (signed index).
 * @param instance Pointer to this value array instance.
 * @param index Zero-based or negative from end.
 * @param value New value.
 * @param xctx of caller.
 *
 * Stores as-is today (like object set_property). Issue #2: optional_release
 * previous managed value once hold-on-store is wired.
 */
AFW_DECLARE(void)
afw_array_set_value(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Remove value at index (signed index).
 * @param instance Pointer to this value array instance.
 * @param index Zero-based or negative from end.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_remove_value_by_index(
    const afw_array_t *instance,
    afw_integer_t index,
    afw_xctx_t *xctx);



/**
 * @brief Remove first equal value (content remove).
 * @param instance Pointer to this value array instance.
 * @param value Value to match.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_remove_value(
    const afw_array_t *instance,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Remove first matching internal value (typed content remove).
 * @param instance Pointer to this value array instance.
 * @param data_type of internal.
 * @param internal A value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_remove_internal(
    const afw_array_t *instance,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx);



/**
 * @brief Remove all values (clear).
 * @param instance Pointer to this value array instance.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_array_remove_all_values(
    const afw_array_t *instance,
    afw_xctx_t *xctx);


/**
 * @brief Determine if array is immutable.
 * @param array of array.
 * @param xctx of caller.
 * @return boolean result.
 */
#define afw_array_is_immutable(array, xctx) \
(afw_array_get_setter(array, xctx) == NULL)



AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ARRAY_H__ */

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
 * Memory array create options. Same bit values as
 * AFW_OBJECT_MEMORY_OPTION_*. Pool-world only. 0 = live in p.
 * new_p — new child of p->managed_p. cede_p — this p is the array's
 * pool. Instance get_reference / release pin that pool. Value
 * get_reference / release throw. new_p | cede_p is invalid.
 */
#define AFW_ARRAY_MEMORY_OPTION_new_p                (1 << 0)
#define AFW_ARRAY_MEMORY_OPTION_cede_p               (1 << 1)
#define AFW_ARRAY_MEMORY_OPTION_IS(options_mask, option) \
    ((((options_mask) & (AFW_ARRAY_MEMORY_OPTION_ ## option))) != 0)


/**
 * @brief Create an array in memory with options.
 * @param options AFW_ARRAY_MEMORY_OPTION_* (same bits as objects).
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
 * @brief Create a managed memory array in xctx->p.
 * @param data_type if array only holds one data type or NULL.
 * @param xctx of caller.
 * @return instance (reference count 1).
 *
 * Slot protocol: push/set use get_assignable_value; replace/remove
 * release the occupant. Last array release releases remaining
 * elements then free_memorys the header. No dest p.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed(
    const afw_data_type_t *data_type,
    afw_xctx_t *xctx);


/**
 * @brief Managed clone of an existing array into xctx->p.
 * @param from array to copy elements from.
 * @param xctx of caller.
 * @return managed array, or from if already this implementation.
 *
 * Deep clone: nested objects/arrays become managed clones; scalars
 * promote via get_assignable_value. Already-managed source is held.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed_clone(
    const afw_array_t *from,
    afw_xctx_t *xctx);


/**
 * @brief True if array is the new managed memory bag (xctx->p, slots).
 */
AFW_DECLARE(afw_boolean_t)
afw_array_is_memory_managed(const afw_array_t *array);



/**
 * @brief Create a memory array that wraps another array (mutable face).
 * @param options AFW_ARRAY_MEMORY_OPTION_* (unmanaged borrow vs managed pin).
 * @param wrapped base array for isolation. Required (non-NULL). Any
 *     afw_array implementation is allowed (memory, adapter-backed,
 *     from_values, …).
 * @param p to use for the face.
 * @param xctx of caller.
 * @return instance of new wrapper array.
 *
 * The face materializes entry value pointers into a local ring so mutators
 * only touch the face. Nested mutable objects/arrays are promoted to nested
 * faces on get. Sets never write to @p wrapped (issue #17).
 *
 * Not the same as afw_array_create_unmanaged_from_c_array (typed
 * from_values array copied from C internals).
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
#define afw_array_create_wrapper_unmanaged_new_p(wrapped, p, xctx) \
    afw_array_create_wrapper_with_options( \
        AFW_ARRAY_MEMORY_OPTION_new_p, wrapped, p, xctx)


/**
 * @brief Create an unmanaged memory wrapper over another array.
 */
#define afw_array_create_wrapper_unmanaged(wrapped, p, xctx) \
    afw_array_create_wrapper_with_options( \
        0, wrapped, p, xctx)


/**
 * @brief Empty script-evaluation-aware array (face over an empty base).
 * @param p pool for the face.
 * @param xctx of caller.
 * @return unmanaged memory wrapper; element store is overlay holds.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_script_wrapper(
    const afw_pool_t *p,
    afw_xctx_t *xctx);


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
 * @brief True if array is a generic memory array (face or not).
 * @param array to test (may be NULL).
 *
 * Custom infs (metas views, from_values) are false. Used so clone_or_reference
 * wraps compiled memory arrays (mutable overlay) but not immutable arrays.
 */
AFW_DECLARE(afw_boolean_t)
afw_array_is_memory(const afw_array_t *array);


/**
 * @brief Base array under a memory face, or array itself if not a face.
 * @param array may be NULL.
 * @return wrapped base if create_wrapper_* face; else array; NULL if array NULL.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_memory_wrapper_base(const afw_array_t *array);



/**
 * @brief Evaluated value for an array instance.
 * @param array instance, or NULL.
 * @param p used only if a heap wrapper is required (no dual face).
 * @param xctx of caller.
 * @return array->value when present; otherwise an unmanaged heap
 *    wrapper. NULL if array is NULL.
 *
 * Use this whenever C has an instance and needs an Adaptive value.
 * Do not call afw_value_array_create for that — it allocates a
 * second header whose optional_release does not hold the instance.
 */
AFW_DECLARE(const afw_value_t *)
afw_array_as_value(
    const afw_array_t *array,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


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
#define afw_array_create_unmanaged_of(data_type, p, xctx) \
    afw_array_create_with_options( \
        0, data_type, p, xctx)


/**
 * @brief Create a pool-world memory array (new child of p->managed_p).
 */
#define afw_array_create_unmanaged_new_p(p, xctx) \
    afw_array_create_with_options( \
        AFW_ARRAY_MEMORY_OPTION_new_p, NULL, p, xctx)


/**
 * @brief Create an array that lives in pool p.
 * @param p to use for the array.
 * @param xctx of caller.
 * @return Pointer to interface pointer of new value array.
 *
 * Start 0. Lifetime is p. Value get_reference / release throw.
 */
#define afw_array_create_unmanaged(p, xctx) \
    afw_array_create_with_options(0, NULL, p, xctx)



/**
 * @brief Create an unmanaged from_values array from object pointers.
 * @param objects is address of first object in array.
 * @param count is number of objects.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance. get_data_type() is object.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_unmanaged_from_objects(
    const afw_object_t *const *objects,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an unmanaged from_values array from value pointers.
 * @param data_type if every element is that type, or NULL if mixed.
 * @param values is address of first value in array.
 * @param count is number for values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance. Dual face is unmanaged_array.
 *
 * Elements are existing value pointers; get_entry_value() does not wrap.
 * Not a permanent array. Generate uses the public self with
 * afw_array_permanent_from_values_inf.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_unmanaged_from_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an unmanaged from_values array from NULL-terminated objects.
 * @param objects is NULL terminated array of objects.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance. get_data_type() is object.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_unmanaged_from_null_terminated_objects(
    const afw_object_t *const *objects,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an unmanaged from_values array from NULL-terminated values.
 * @param data_type if every element is that type, or NULL if mixed.
 * @param values is NULL terminated array of values.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return array instance.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_unmanaged_from_null_terminated_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Self for immutable array of value pointers.
 *
 * Shared layout for unmanaged, managed (later), and permanent infs.
 * Generate and typed empty arrays initialize this statically with
 * afw_array_permanent_from_values_inf. Runtime creates allocate it
 * plus a dual value.
 */
typedef struct afw_array_from_values_self_s {
    afw_array_t pub;
    const afw_data_type_t *data_type;
    afw_size_t count;
    const afw_value_t *const *values;
    /* Managed inf only. Permanent/unmanaged leave this 0. */
    afw_size_t reference_count;
} afw_array_from_values_self_t;


/**
 * @brief inf for unmanaged from_values arrays (runtime create).
 */
AFW_DECLARE_CONST_DATA(afw_array_inf_t)
afw_array_unmanaged_from_values_inf;


/**
 * @brief inf for permanent from_values arrays (generate / empty arrays).
 */
AFW_DECLARE_CONST_DATA(afw_array_inf_t)
afw_array_permanent_from_values_inf;


/**
 * @brief inf for managed from_values arrays (xctx->p, RC).
 */
AFW_DECLARE_CONST_DATA(afw_array_inf_t)
afw_array_managed_from_values_inf;



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
afw_array_create_unmanaged_from_value(
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
 * @brief Create an unmanaged from_values array by copying C internals.
 * @param array of internal values (or pointers to internals if indirect).
 * @param indirect if true, array is array of pointers to internal values.
 * @param data_type of each element. Required.
 * @param count of entries in array or -1 for NULL-terminated pointer list.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return typed unmanaged from_values array.
 *
 * Copies each internal into a value in @p p at create. C storage need
 * only live until this call returns. Fill C storage before calling.
 *
 * This is **not** a mutable look-through face over another adaptive array
 * (reserved naming: create_wrapper_* for #17-style faces).
 *
 * If indirect is false, array must be an array of data_type->cType.  If
 * indirect is true, array must be an array of data_type->cType *.
 *
 * If count is -1, the array must be an array of pointers.  This can either
 * be because data_type->cType ends in an asterisk or the indirect parameter
 * is true.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_unmanaged_from_c_array(
    const void *array,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a managed from_values array from value pointers.
 * @param data_type if every element is that type, or NULL if mixed.
 * @param values is address of first value in array.
 * @param count is number of values.
 * @param xctx of caller.
 * @return instance (reference count 1). Dual face is managed_array.
 *
 * Each element is stored via get_assignable_value. No dest p.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed_from_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    afw_size_t count,
    afw_xctx_t *xctx);



/**
 * @brief Create a managed from_values array from object pointers.
 * @param objects is address of first object in array.
 * @param count is number of objects.
 * @param xctx of caller.
 * @return instance (reference count 1). get_data_type() is object.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed_from_objects(
    const afw_object_t *const *objects,
    afw_size_t count,
    afw_xctx_t *xctx);



/**
 * @brief Create a managed from_values array from NULL-terminated values.
 * @param data_type if every element is that type, or NULL if mixed.
 * @param values is NULL terminated array of values.
 * @param xctx of caller.
 * @return instance (reference count 1).
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed_from_null_terminated_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    afw_xctx_t *xctx);



/**
 * @brief Create a managed from_values array from NULL-terminated objects.
 * @param objects is NULL terminated array of objects.
 * @param xctx of caller.
 * @return instance (reference count 1). get_data_type() is object.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed_from_null_terminated_objects(
    const afw_object_t *const *objects,
    afw_xctx_t *xctx);



/**
 * @brief Create a managed from_values array by copying C internals.
 * @param array of internal values (or pointers to internals if indirect).
 * @param indirect if true, array is array of pointers to internal values.
 * @param data_type of each element. Required.
 * @param count of entries in array or -1 for NULL-terminated pointer list.
 * @param xctx of caller.
 * @return instance (reference count 1).
 *
 * Copies each internal into a managed value in xctx->p at create.
 */
AFW_DECLARE(const afw_array_t *)
afw_array_create_managed_from_c_array(
    const void *array,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    afw_xctx_t *xctx);



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
    const afw_iterator_old_t * *iterator,
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
 * Overlay hold: release the stored pointer, add_reference the new
 * value, store. Same protocol as object set_property.
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

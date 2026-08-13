// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Runtime Value Accessor Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_RUNTIME_VALUE_ACCESSOR_H__
#define __AFW_RUNTIME_VALUE_ACCESSOR_H__

#include "afw_runtime.h"

AFW_BEGIN_DECLARES

/**
 * @addtogroup afw_runtime_value_accessor
 * @{
 */

/**
 * @file afw_runtime_value_accessor.h
 * @brief Runtime object map value accessor callbacks and registration info.
 *
 * Each accessor implementation should have a co-located
 * afw_runtime_value_accessor_info_t describing key, brief, description, and
 * lifetime contracts. Register that struct with
 * afw_environment_register_runtime_value_accessor().
 *
 * Adaptive catalog: `/afw/_AdaptiveRuntimeValueAccessor_/<key>`.
 * C call path: afw_environment_get_runtime_value_accessor().
 */

/**
 * @brief Registered runtime value accessor (env registry value).
 *
 * Map typedef for `_AdaptiveRuntimeValueAccessor_`. Place instances next to
 * the accessor function they describe so contracts stay with the code.
 *
 * The C function pointer is not exposed as an Adaptive property.
 */
struct afw_runtime_value_accessor_info_s {

    /** @brief Registry key / objectId (propertyTypes.runtime.valueAccessor). */
    const afw_utf8_t *key;

    /** @brief Accessor function. */
    afw_runtime_value_accessor_t function;

    /** @brief Short description for lists and query. */
    const afw_utf8_t *brief;

    /** @brief Longer description including internal shape and lifetime. */
    const afw_utf8_t *description;

    /**
     * @brief True if the accessor locks (or equivalent) and copies into p.
     *
     * Result is then stable for the caller after return.
     */
    afw_boolean_t copies_under_lock;

    /**
     * @brief True if the Adaptive value may alias live runtime/env state.
     *
     * State may change or be released while the value is still held unless
     * copies_under_lock also applies.
     */
    afw_boolean_t returns_live_reference;
};


/**
 * @brief Runtime value accessor 'default' for default internal.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to internal for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * This is the default value accessor.  It uses the information in
 * afw_runtime_object_map_property and expects internal to point
 * directly to the value used as internal for the created afw_value_t.
 */
const afw_value_t *
afw_runtime_value_accessor_default(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor 'compile_type'.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer afw_compile_type_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * This value accessor will return the string value for a afw_compile_type_t
 * internal value.
 */
const afw_value_t *
afw_runtime_value_accessor_compile_type(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor 'data_type_id' for const afw_data_type_t *.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to const afw_data_type_t * (may be NULL).
 * @param p is pool to use.
 * @param xctx of caller.
 * @return string value of data_type->data_type_id, or NULL if pointer is NULL.
 *
 * Used for Adaptive properties that store a data type pointer in C but expose
 * the type id string (e.g. iteratorReturnDataType on _AdaptiveDataType_).
 */
const afw_value_t *
afw_runtime_value_accessor_data_type_id(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor 'indirect' for indirect internal.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to pointer of internal for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * This is like afw_runtime_value_accessor_default(), but expects
 * internal to point to a pointer to the value used as internal for
 * the created afw_value_t.
 *
 * An example of when this is used, is if internal is a pointer to
 * afw_utf8_t * instead of afw_utf8_t.
 */
const afw_value_t *
afw_runtime_value_accessor_indirect(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for afw_octet_t as afw_integer_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_size_t for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be integer.
 */
const afw_value_t *
afw_runtime_value_accessor_octet(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Runtime value accessor for stopping adapter instance reference
 *    counts.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is adapter_id afw_utf8_t **.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_stopping_adapter_instances(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for stopping authorization handler
 *    reference counts.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is authorization_handler_id afw_utf8_t **.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_stopping_authorization_handler_instances(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Snapshot adapter anchor reference_count under adapter_id_anchor_lock.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_integer_t reference_count on anchor.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return integer value copied under lock.
 */
const afw_value_t *
afw_runtime_value_accessor_adapter_reference_count(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Snapshot authorization handler anchor reference_count under rw lock.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_integer_t reference_count on anchor.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return integer value copied under read lock.
 */
const afw_value_t *
afw_runtime_value_accessor_authorization_handler_reference_count(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for NULL terminated list of internal.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is const afw_object_t * const * with last entry NULL.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_internal(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for NULL terminated list of objects.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is const afw_object_t * const * with last entry NULL.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_objects(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for NULL terminated list of utf8_z key/value
 *   pair objects.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal const afw_utf8_z_t *[] (See description).
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 * 
 * The internal parameter is an array of const afw_utf8_z_t * key/value pairs.
 * Each object is terminated with a NULL and the final object is indicated
 * with an extra NULL.
 * 
 * Example:
 * 
 * static const afw_utf8_z_t *
 * impl_eq_anyURI_errorsThrown[] = {
 *     "error", "conversion",
 *     "reason", "arg2 cannot be converted to the data type of arg1.",
 *     NULL, 
 *     "error", "argument_error",
 *     "reason", "General parameter issue..",
 *     NULL,
 *     NULL
 * };
 * 
 */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_utf8_z_key_value_pair_objects(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for NULL terminated list of internal pointers.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is const afw_object_t * const * with last entry NULL.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_pointers(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for NULL terminated list of values.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is const afw_value_t * const * with last entry NULL.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_null_terminated_array_of_values(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for afw_service_startup_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_size_t for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be integer.
 */
const afw_value_t *
afw_runtime_value_accessor_service_startup(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for afw_service_status_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_size_t for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be integer.
 */
const afw_value_t *
afw_runtime_value_accessor_service_status(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for afw_size_t as afw_integer_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_size_t for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be integer.
 */
const afw_value_t *
afw_runtime_value_accessor_size(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for afw_uint32_t as afw_integer_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_size_t for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be integer.
 */
const afw_value_t *
afw_runtime_value_accessor_uint32(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor for afw_uint32_t as afw_integer_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_size_t for result afw_value_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be integer.
 */
const afw_value_t *
afw_runtime_value_accessor_uint32(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Live adapter metrics object (active adapter pointer under lock).
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to const afw_adapter_t * on adapter id anchor.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return object value wrapping metrics_object, or NULL if no active adapter.
 *
 * Loads the adapter pointer under adapter_id_anchor_lock. Result is still a
 * live reference (not a deep snapshot of counters).
 */
const afw_value_t *
afw_runtime_value_accessor_adapter_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Live adapter anchor properties object (pointer under lock).
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to const afw_object_t * properties on anchor.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return object value wrapping properties, or NULL.
 */
const afw_value_t *
afw_runtime_value_accessor_adapter_properties(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor to call afw_adapter_get_additional_metrics().
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer start of afw_adapter_impl_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be object.
 *
 * Parameter internal is mapped by afw_adapter_impl_t. The value will be the
 * result of calling afw_adapter_get_additional_metrics() with the adapter
 * pointed to by afw_adapter_impl_t.
 */
const afw_value_t *
afw_runtime_value_accessor_adapter_additional_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor to ensure afw_components extension is loaded.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to afw_components_t->object_id.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return true if was loaded.
 *
 * The data_type in prop must be object.
 *
 * Parameter internal is mapped by afw_adapter_impl_t object_id member.
 */
const afw_value_t *
afw_runtime_value_accessor_ensure_afw_components_extension_loaded(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Runtime value accessor to produce triggeredBy for a flag.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer to flag_id.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return true if was loaded.
 *
 * The data_type in prop must be list.
 *
 * Parameter internal must be afw_flag_t flag_id member.
 */
const afw_value_t *
afw_runtime_value_accessor_applicable_flags(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Runtime value accessor for an afw_value_t.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer start of afw_adapter_impl_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop can be anything.  Parameter internal must point
 * to a afw_value_t *.
 */
const afw_value_t *
afw_runtime_value_accessor_value(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_RUNTIME_VALUE_ACCESSOR_H__ */

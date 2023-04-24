// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Runtime Value Accessor Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_RUNTIME_VALUE_ACCESSOR_H__
#define __AFW_RUNTIME_VALUE_ACCESSOR_H__

#include "afw_runtime.h"

AFW_BEGIN_DECLARES

/**
 * @defgroup afw_runtime_value_accessor Runtime value accessors
 * @ingroup afw_c_api_public
 *
 * This is an Adaptive Framework core runtime value accessors.
 *
 * @{
 */

/**
 * @file afw_runtime_value_accessor.h
 * @brief Core runtime value accessors.
 *
 * These are internal functions used by AFW core.  Access to
 * accessors external to core should use the
 * afw_environment_get_runtime_value_accessor() function.
 */

/**
 * @brief Register core runtime value accessors.
 * @param xctx of caller.
 */
void afw_runtime_register_core_value_accessors(afw_xctx_t *xctx);


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
 * @brief Runtime value accessor for stopping adaptor instance reference
 *    counts.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is adaptor_id afw_utf8_t **.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 */
const afw_value_t *
afw_runtime_value_accessor_stopping_adaptor_instances(
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
 *     "error", "arg_error",
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
 * @brief Runtime value accessor for /afw/_AdaptiveAdaptorMetrics_/<adaptorId>.
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer const afw_utf8_t *adaptor_id.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be object.
 *
 * Parameter internal is mapped afw_utf8_t *adaptor_id. The value will be the
 * result of getting the runtime /afw/_AdaptiveAdaptorMetrics_/<adaptorId>.
 */
const afw_value_t *
afw_runtime_value_accessor_adaptor_metrics(
    const afw_runtime_object_map_property_t * prop,
    const void *internal, const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Runtime value accessor to call afw_adaptor_get_additional_metrics().
 * @param prop is associated afw_runtime_object_map_property_t.
 * @param internal is pointer start of afw_adaptor_impl_t.
 * @param p is pool to use.
 * @param xctx of caller.
 * @return afw_value_t representing value.
 *
 * The data_type in prop must be object.
 *
 * Parameter internal is mapped by afw_adaptor_impl_t. The value will be the
 * result of calling afw_adaptor_get_additional_metrics() with the adaptor
 * pointed to by afw_adaptor_impl_t.
 */
const afw_value_t *
afw_runtime_value_accessor_adaptor_additional_metrics(
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
 * Parameter internal is mapped by afw_adaptor_impl_t object_id member.
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
 * @param internal is pointer start of afw_adaptor_impl_t.
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

/** @} */

#endif /* __AFW_RUNTIME_VALUE_ACCESSOR_H__ */

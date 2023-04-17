// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Software Flag Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_FLAG_H__
#define __AFW_FLAG_H__


/**
 * @defgroup afw_flag Software Flag
 * @ingroup afw_c_api
 * 
 * Software flags.
 * @{
 */

/**
 * @file afw_flag.h
 * @brief Adaptive Framework Software Flag Header.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

#define AFW_FLAG_INITIAL_ALLOCATED_COUNT 64

/** @brief Struct used for a registered flag. */
struct afw_flag_s {

    /*
     * flagid_ always be first because of
     * afw_runtime_value_accessor_applicable_flags.
     */
    /** @brief objectId of this flag. */
    const afw_utf8_t *flag_id;

    /** @brief sting value of objectId of this flag. */
    const afw_value_t *flag_id_value;

    /** @brief runtime object for this flag. */
    const afw_object_t *object;

    /** @brief object value for runtime object. */
    const afw_value_t *object_value;

    /** @brief Index of this flag in flags array. */
    afw_size_t flag_index;
 
    /** @brief Count of applicable_flags. */
    afw_size_t applicable_flags_count_allocated;
 
    /** @brief Applicable flags indexed by associated flag_index. */
    const afw_boolean_t *applicable_flags;

    /** @brief Brief predicate about this flag. */
    const afw_utf8_t *brief;
 
    /** @brief Description of this flag. */
    const afw_utf8_t *description;
 
    /** @brief list value of all flags that include this flag. */
    const afw_value_list_t *included_by_value;
 
    /** @brief list value of all other flags this flag includes. */
    const afw_value_list_t *includes_value;

    /** @brief Pointer to direct parent afw_flag_t. */
    const afw_flag_t *parent;

};



/**
 * @brief Determine if flag for flag index is set in xctx.
 * @param flag_index
 * @param xctx of caller
 * @return true or false
 */
#define afw_flag_is_active(flag_index, xctx) \
((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index])



/**
 * @brief Determine if flag for flag id is set in xctx.
 * @param flag_id
 * @param xctx of caller
 * @return true or false
 */
AFW_DECLARE(afw_boolean_t)
afw_flag_by_id_is_active(
    const afw_utf8_t *flag_id,
    afw_xctx_t *xctx);



/**
 * @brief Get flag by index
 * @param flag_index
 * @param xctx of caller
 * @return flag struct or NULL if not found.
 */
AFW_DECLARE(const afw_flag_t *)
afw_flag_get_by_index(
    afw_size_t flag_index,
    afw_xctx_t *xctx);



/**
 * @brief Get the flag index for a flag id.
 * @param flag_id
 * @param xctx of caller
 * @return The flag index or throws error if not found.
 */
AFW_DECLARE(afw_size_t)
afw_flag_get_index(
    const afw_utf8_t *flag_id,
    afw_xctx_t *xctx);



/**
 * @brief Set the default value of a flag.
 * @param flag_id of flag to set as default.  The flag must already be registered.
 * @param set_to true to set or false to unset
 * @param xctx of caller.
 *
 * This function only sets the flag as a default until the environment is
 * refreshed.  Add this flag to the defaultFlags property of the application
 * conf to make it permanent.
 */
AFW_DECLARE(void)
afw_flag_set_default(
    const afw_utf8_t *flag_id,
    afw_boolean_t set_to,
    afw_xctx_t *xctx);



/**
 * @brief Set a new default flags list.
 * @param default_flag_ids list.
 * @param xctx of caller
 *
 * The is normally called when the application conf is processed based on the
 * defaultFlags parameter.  The flagIds in the list do not have to already be
 * defined.  When a flag is registered this list is checked to determine flags
 * that should be set.
 */
AFW_DECLARE(void)
afw_flag_set_default_flag_ids(
    const afw_list_t *default_flag_ids,
    afw_xctx_t *xctx);



/**
 * @brief Set a flag in xctx.
 * @param flag_index
 * @param set_to true to set or false to unset
 * @param xctx of caller
 */
AFW_DECLARE(void)
afw_flag_set(
    const afw_utf8_t *flag_id,
    afw_boolean_t set_to,
    afw_xctx_t *xctx);



/**
 * @brief Set xctx default flags plus one or more additional flags.
 * @param flag_ids is NULL terminated array of flag ids.
 * @param xctx of caller
 */
AFW_DECLARE(void)
afw_flag_set_to_defaults_plus_array(
    const afw_utf8_t * const *flag_ids,
    afw_xctx_t *xctx);



/**
 * @brief Register a flag definition.
 * @param flag_id.
 * @param brief.
 * @param description.
 * @param included_by_flag_id
 * @param log_priority to be used if this flag triggers a log message.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_flag_environment_register_flag(
    const afw_utf8_t *flag_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    const afw_utf8_t *included_by_flag_id,
    afw_xctx_t *xctx);



/**
 * @brief Add another include_by flag to a registered flag.
 * @param flag_id of a registered flag.
 * @param included_by_flag_id of a registered flag.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_flag_add_included_by(
    const afw_utf8_t *flag_id,
    const afw_utf8_t *included_by_flag_id,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Register early core flags.
 */
AFW_DECLARE_INTERNAL(void)
afw_flag_internal_early_register_core(afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_TRACE_H__ */

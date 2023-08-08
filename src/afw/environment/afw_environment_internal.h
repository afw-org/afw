// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Environment Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ENVIRONMENT_INTERNAL_H__
#define __AFW_ENVIRONMENT_INTERNAL_H__

#include "afw_interface.h"
#include "afw_environment.h"
#include <apr_dso.h>

/**
 * @defgroup afw_environment_internal Environment
 * @ingroup afw_c_api_internal
 * 
 * Internal environment API.
 * @{
 */

/**
 * @file afw_environment.h
 * @brief Internal header for AFW environment.
 */

AFW_BEGIN_DECLARES

/*
 * Environment structure.
 */
typedef struct afw_environment_internal_s {

    afw_environment_t pub;

    /* Base execution context. */
    afw_xctx_t *base_xctx;

    /* Configuration object. */
    const afw_array_t *configuration;

    /* Hash table of registry type name to registry type number. */
    apr_hash_t *registry_names_ht;

    /*
    * Array indexed by afw_environment_registry_type_enum_t used to hold
    * afw_environment_registry_entry_t * for each type.
    */
    apr_array_header_t *registry_types;

    /* Hash table of data type method numbers. */
    apr_hash_t * data_type_method_number_ht;

    /* Array indexed by data_type_number of pointers to array of methods. */
    apr_array_header_t * data_type_methods;

    /* Core data types have been registered. */
    afw_boolean_t core_data_types_registered;

    /* Core functions have been registered. */
    afw_boolean_t core_functions_registered;

    /* Core object type maps have been registered. */
    afw_boolean_t core_object_type_maps_registered;

    /*
     * NULL terminated list of default flagIds. Use
     * afw_flag_set_default_flag_ids() to set.
     */
    const afw_utf8_t * const *default_flag_ids;

    /*
     * Pool used to hold everything in default_flags_list.  This pool will be
     * destroyed when a new default_flags_list is set by a call to
     * afw_flag_set_default_flag_ids()
     */
    const afw_pool_t *default_flags_ids_p;

} afw_environment_internal_t;


/* Register anything that is part of libafw. */
void afw_environment_internal_register_core(afw_xctx_t *xctx);


/** @brief type=extension conf handler. */
void afw_environment_internal_extension_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ENVIRONMENT_INTERNAL_H__ */

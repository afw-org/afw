// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework environment internal members
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_environment_internal_members.h
 * @brief Non-public members of struct afw_environment_s.
 *
 * Included from struct afw_environment_s in afw_environment.h only when
 * AFW_ENVIRONMENT_INTERNAL_MEMBERS is defined (afw_internal.h). Do not
 * include this file directly. Public fields stay a prefix of the
 * struct so a short view is a valid prefix of the full one.
 *
 * Vector types are completed in afw_environment_internal.h.
 */

    /* Base execution context. */
    afw_xctx_t *base_xctx;

    /* Configuration object. */
    const afw_array_t *configuration;

    /* Hash table of registry type name to registry type number. */
    afw_void_hash_table_t *registry_names_ht;

    /*
     * Vector indexed by afw_environment_registry_type_enum_t of
     * afw_environment_registry_type_t * for each type.
     */
    afw_environment_registry_type_p_vector_t *registry_types;

    /* Hash table of data type method numbers. */
    afw_void_hash_table_t *data_type_method_number_ht;

    /*
     * Vector indexed by data_type_number of pointers to method
     * vectors.
     */
    afw_environment_data_type_methods_vector_t *data_type_methods;

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

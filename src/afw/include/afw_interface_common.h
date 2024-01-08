// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Interface Common Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_INTERFACE_COMMON_H__
#define __AFW_INTERFACE_COMMON_H__

#include "afw_common.h"

/**
 * @addtogroup afw_common
 * @{
 */

/**
 * @file afw_interface_common.h
 * @brief Adaptive Framework Interface Common Header.
*
 * Do not put anything here that can not included in all interfaces.
*
 */

AFW_BEGIN_DECLARES

/** @brief Interface Variable Run Time Information. */
typedef struct afw_interface_rti_variable_s {
    const afw_utf8_z_t * name;
    const afw_utf8_z_t * type;
    const afw_utf8_z_t * c_declaration;
    const afw_utf8_z_t * description;
} afw_interface_rti_variable_t;


/** @brief Interface Method Run Time Information. */
typedef struct afw_interface_rti_method_s {
    const afw_utf8_z_t * name;
    /** NULL terminated list of parameters.  The return value has a NULL name. */
    const afw_interface_rti_variable_t * const (*parameters);
    const afw_utf8_z_t * description;
} afw_interface_rti_method_t;


/** @brief Interface Run Time Information. */
typedef struct afw_interface_rti_s {
    const afw_utf8_z_t * name;
    const afw_utf8_z_t * id;
    const afw_interface_rti_variable_t * const (*variables);
    const afw_interface_rti_method_t * const (*methods);
} afw_interface_rti_t;


/** @brief Interface Implementation Run Time Information. */
typedef struct afw_interface_implementation_rti_s {
    const afw_utf8_t interface_name;
    const afw_utf8_t implementation_source;
    const afw_utf8_t implementation_id;
    const void      *implementation_specific;
} afw_interface_implementation_rti_t;


/** @brief Minimal Interface. */
typedef struct afw_interface_s {
    afw_interface_implementation_rti_t rti;
    /* Function pointer for each method. */
} afw_interface_t;


/** @brief Minimal Instance. */
typedef struct afw_instance_s {
    /** @brief Interface to instance is always first. */
    const afw_interface_t *inf;
    /* There may be additional public instance info. */
} afw_instance_t;


AFW_END_DECLARES

/** @} */

#endif /* __AFW_INTERFACE_COMMON_H__ */

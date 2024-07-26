// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework YAML
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_YAML_H__
#define __AFW_YAML_H__

#include "afw_minimal.h"
#include "generated/afw_yaml_generated.h"

/**
 * @defgroup afw_yaml YAML content-type
 * @ingroup afw_included_extensions
 *
 * Adaptive Framework YAML support
 * 
 * @{
 */

/**
 * @file afw_yaml.h
 * @brief Header file for Adaptive Framework YAML
 */

AFW_BEGIN_DECLARES

/** yaml_parser rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_yaml_parser "yaml_parser"

/** yaml_token_type rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_yaml_token_type "yaml_token_type"

#define AFW_YAML_Q_CONTENT_TYPE       "application/x-yaml"
#define AFW_YAML_S_CONTENT_TYPE       afw_s_a_application_yaml
#define AFW_YAML_Q_CONTENT_TYPE_SHORT "yaml"
#define AFW_YAML_S_CONTENT_TYPE_SHORT afw_s_yaml


/**
 * @brief Register YAML support.
 * @param xctx of caller.
*
 * This is called by afw_environment_register_core.c to register YAML
 * support.
 */
void afw_yaml_register(afw_xctx_t *xctx);

/**
 * @brief Convert error from xctx to a YAML object.
 * @param xctx of caller.
 * @return YAML string.
 */
const afw_utf8_t * afw_yaml_from_error(afw_xctx_t *xctx);

/**
 * @brief Convert an adaptive value to YAML.
 * @param value 
 * @param p to use.
 * @param xctx of caller.
 * @return yaml
*
 * This function can be used for callbacks of type afw_utf8_from_value_t.
 */
const afw_utf8_t * afw_yaml_from_value(
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Convert YAML to an adaptive value.
 * @param yaml YAML.
 * @param path Object path or NULL.
 * @param object_id Optional id of object if value is object or NULL.
 * @param pool to allocate memory on.
 * @param xctx of caller.
 * @return value
*
 * This function can be used for callbacks of type afw_utf8_to_value_t.
*
 * The path parameter is only used if the YAML being parsed is an object entity.
*
 * The path must be of the form `/<adapter id>/<object type>/<id>`.  This path
 * is parsed to provide return values for the object's
 * afw_object_meta_get_object_id(), afw_object_meta_get_path(), and
 * afw_object_get_object_type() methods.
 */
const afw_value_t * afw_yaml_to_value(
    const afw_memory_t *yaml,
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Convert from YAML to adaptive object.
 * @param raw YAML to convert.
 * @param source_location or NULL.
 * @param adapter_id to use for created object.
 * @param object_type_id to use for created object.
 * @param object_id to use for created object.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use.
 * @param xctx of caller.
 * @return object instance.
 *
 */
const afw_object_t * afw_yaml_to_object(
    const afw_memory_t  * yaml,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adapter_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t      cede_p,
    const afw_pool_t * p,
    afw_xctx_t      * xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_YAML_H__ */

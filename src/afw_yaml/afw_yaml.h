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
#include "generated/afw_yaml_generated_internal.h"

/**
 * @defgroup afw_yaml YAML content-type
 * @ingroup afw_included_extensions
 *
 * YAML content type (`src/afw_yaml/`): serialize adaptive values as YAML
 * media types. Self-contained extension srcdir over public core APIs.
 * Registers like core JSON; call content-type macros.
 *
 * @{
 */

/**
 * @file afw_yaml.h
 * @brief YAML content-type and from/to value public API.
 *
 * See @ref afw_content_type and @ref afw_included_extensions.
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
 * @param p to allocate memory on.
 * @param xctx of caller.
 * @return value
 *
 * Used by content-type raw_to_value (conf, request bodies, journal decode).
 *
 * Does **not** apply issue #17 mutable faces: conf and stored objects stay
 * plain. Script-facing adapter/journal APIs wrap on return.
 *
 * The path parameter is only used if the YAML root is an object entity.
 * Path form: `/<adapter id>/<object type>/<id>`. Applied via
 * afw_object_meta_set_ids_using_path() after parse.
 */
const afw_value_t * afw_yaml_to_value(
    const afw_memory_t *yaml,
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Convert from YAML to adaptive object.
 * @param yaml YAML bytes to convert.
 * @param source_location or NULL (interface parity with JSON; unused today).
 * @param adapter_id to use for created object.
 * @param object_type_id to use for created object.
 * @param object_id to use for created object.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use.
 * @param xctx of caller.
 * @return object instance.
 *
 * Content-type raw_to_object (file adapter when contentType is yaml).
 * Root must be a YAML mapping. Sets object meta ids when adapter_id is set.
 * Does not face-wrap; adapter script returns apply issue #17 faces.
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

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework JSON
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_JSON_H__
#define __AFW_JSON_H__


/**
 * @defgroup afw_json JSON support
 * @ingroup afw_c_api
 *
 * Adaptive Framework JSON support
 * 
 * @{
 */

/**
 * @file afw_json.h
 * @brief Header file for Adaptive Framework JSON
 */

#include "afw_minimal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AFW_JSON_Q_CONTENT_TYPE       "application/json"
#define AFW_JSON_S_CONTENT_TYPE       afw_s_a_application_json
#define AFW_JSON_Q_CONTENT_TYPE_SHORT "json"
#define AFW_JSON_S_CONTENT_TYPE_SHORT afw_s_json


#define AFW_JSON_PN_JSON_PRIMITIVE   AFW_Q_jsonPrimitive
#define AFW_JSON_PN_s_JSON_PRIMITIVE afw_s_jsonPrimitive
#define AFW_JSON_Q_PRIMITIVE_OBJECT  AFW_Q_object
#define AFW_JSON_S_PRIMITIVE_OBJECT  afw_s_object
#define AFW_JSON_Q_PRIMITIVE_ARRAY   AFW_Q_array
#define AFW_JSON_S_PRIMITIVE_ARRAY   afw_s_array
#define AFW_JSON_Q_PRIMITIVE_NUMBER  AFW_Q_number
#define AFW_JSON_S_PRIMITIVE_NUMBER  afw_s_number
#define AFW_JSON_Q_PRIMITIVE_STRING  AFW_Q_string
#define AFW_JSON_S_PRIMITIVE_STRING  afw_s_string
#define AFW_JSON_Q_PRIMITIVE_BOOLEAN AFW_Q_boolean
#define AFW_JSON_S_PRIMITIVE_BOOLEAN afw_s_boolean
#define AFW_JSON_Q_PRIMITIVE_NULL    AFW_Q_null
#define AFW_JSON_S_PRIMITIVE_NULL    afw_s_null
#define AFW_JSON_Q_PRIMITIVE_BOOLEAN_TRUE  AFW_Q_true
#define AFW_JSON_S_PRIMITIVE_BOOLEAN_TRUE  afw_s_true
#define AFW_JSON_Q_PRIMITIVE_BOOLEAN_FALSE AFW_Q_false
#define AFW_JSON_S_PRIMITIVE_BOOLEAN_FALSE afw_s_false

#define AFW_JSON_Q_INFINITY        "Infinity"
#define AFW_JSON_Q_MINUS_INFINITY  "-Infinity"
#define AFW_JSON_Q_NAN             "NaN"
#define AFW_JSON_Q_MINUS_NAN       "-NaN"

/**
 * Property name "_meta_" of a special JSON object property whose value
 * is an object.  The properties of this object correspond to certain methods
 * of the afw_object interface or carry additional information.
 *
 * | Property name |  Related method or purpose                              |
 * | ------------- | ------------------------------------------------------- |
 * | objectId      | afw_object_meta_get_object_id()                         |
 * | path          | afw_object_meta_get_path()                              |
 * | objectType    | afw_object_meta_get_object_type()                       |
 * | parentPaths   | afw_object_meta_get_parent_paths()                      |
 *
 * Properties parentPaths and expanded can be included in input objects to
 *
 */
#define AFW_JSON_Q_PN_META         AFW_OBJECT_Q_PN_META
#define AFW_JSON_Q_PN_ID           AFW_OBJECT_Q_PN_ID
#define AFW_JSON_Q_PN_PATH         AFW_OBJECT_Q_PN_PATH
#define AFW_JSON_Q_PN_OBJECT_TYPE  AFW_OBJECT_Q_PN_OBJECT_TYPE
#define AFW_JSON_Q_PN_PARENT_PATH  AFW_OBJECT_Q_PN_PARENT_PATH


/**
 * @brief Get the content type instance for json.
 */
AFW_DECLARE(const afw_content_type_t *)
afw_json_content_type_get();


/**
 * @brief Register JSON support.
 * @param xctx of caller.
 *
 * This is called by afw_environment_register_core.c to register JSON
 * support.
 */
AFW_DECLARE(void)
afw_json_register(afw_xctx_t *xctx);

/**
 * @brief Convert an adaptive value to JSON.
 * @param value
 * @param options or NULL.
 * @param p to use.
 * @param xctx of caller.
 * @return json
 *
 * This function can be used for callbacks of type afw_utf8_from_value_t.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_json_from_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Convert JSON to an adaptive value.
 * @param json JSON.
 * @param source_location or NULL.
 * @param object_id Optional id of object if value is object or NULL.
 * @param p to use.
 * @param xctx of caller.
 * @return value
 *
 * This function can be used for callbacks of type afw_utf8_to_value_t.
 */
#define afw_json_to_value(json, source_location, p, xctx) \
    afw_compile_to_value(json, source_location, \
        afw_compile_type_relaxed_json, \
        NULL, NULL, p, xctx)


/**
 * @brief Compile json string to adaptive object.
 * @param string to compile.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use.
 * @param xctx of caller.
 * @return object instance.
 */
#define afw_json_to_object(string, cede_p, p, xctx) \
    afw_compile_to_object(string, NULL, NULL, NULL, NULL, \
        cede_p, p, xctx)


/**
 * @brief Compile json string to adaptive object with meta.
 * @param string to compile.
 * @param source_location or NULL.
 * @param adaptor_id to use for created object.
 * @param object_type_id to use for created object.
 * @param object_id to use for created object.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use.
 * @param xctx of caller.
 * @return object instance.
 *
 * If source_location is NULL and path is specified, adaptor_id,
 * object_type_id, and object_id will be used to produce a path that will
 * be used as the source location.
 * 
 * If adaptor_id is NULL, object_type_id and object_id must be NULL. If they're
 * NULL, the object's meta will not be set.
 *
 */
#define afw_json_to_object_with_meta(string, source_location, \
    adaptor_id, object_type_id, object_id, \
    cede_p, p, xctx) \
    afw_compile_to_object(string, source_location, \
        adaptor_id, object_type_id, object_id, \
        cede_p, p, xctx)


/**
 * @brief Write string as json encoded string.
 * @param string to write.
 * @param writer to use.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_json_write_encoded_string(
    const afw_utf8_t *string,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);



/**
 * @brief Create a json encoded quoted string.
 * @param string to encode.
 * @param p for result.
 * @return Quoted string.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_json_utf8_string_create(
    const afw_utf8_t *string,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_JSON_H__ */

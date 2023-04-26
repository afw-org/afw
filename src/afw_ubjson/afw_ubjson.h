// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Universal Binary JSON (UBJSON)
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_UBJSON_H__
#define __AFW_UBJSON_H__

#include "afw_minimal.h"

/**
 * @defgroup afw_ubjson Universal Binary JSON content-type
 * @ingroup  afw_included_extensions
 * 
 * Adaptive Framework UBJSON support.
 * 
* @{
 */

/**
 * @file afw_ubjson.h
 * @brief Header file for Adaptive Framework UBJSON
 */

AFW_BEGIN_DECLARES

/** ubjson_marker rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_ubjson_marker "ubjson_marker"

#define AFW_UBJSON_Q_CONTENT_TYPE       "application/ubjson"
#define AFW_UBJSON_S_CONTENT_TYPE       afw_s_a_application_ubjson
#define AFW_UBJSON_Q_CONTENT_TYPE_SHORT "ubjson"
#define AFW_UBJSON_S_CONTENT_TYPE_SHORT afw_s_ubjson



/*
 * Type Reference
 *
 * http://www.ubjson.org/type-reference/
 *
 */

typedef enum {
    AFW_UBJSON_VALUE_TYPE_NULL,
    AFW_UBJSON_VALUE_TYPE_NOOP,
    AFW_UBJSON_VALUE_TYPE_TRUE,
    AFW_UBJSON_VALUE_TYPE_FALSE,
    AFW_UBJSON_VALUE_TYPE_INT8,
    AFW_UBJSON_VALUE_TYPE_UINT8,
    AFW_UBJSON_VALUE_TYPE_INT16,
    AFW_UBJSON_VALUE_TYPE_INT32,
    AFW_UBJSON_VALUE_TYPE_INT64,
    AFW_UBJSON_VALUE_TYPE_FLOAT32,
    AFW_UBJSON_VALUE_TYPE_FLOAT64,
    AFW_UBJSON_VALUE_TYPE_HIGH_PRECISION,
    AFW_UBJSON_VALUE_TYPE_CHAR,
    AFW_UBJSON_VALUE_TYPE_STRING
} AFW_UBJSON_VALUE_TYPE;

typedef enum {
    AFW_UBJSON_CONTAINER_TYPE_ARRAY,
    AFW_UBJSON_CONTAINER_TYPE_OBJECT
} AFW_UBJSON_CONTAINER_TYPE;


/* the size (in bytes) of each numeric type */
extern const int AFW_UBJSON_SIZE[];


#define AFW_UBJSON_MARKER_NULL            'Z'
#define AFW_UBJSON_MARKER_NOOP            'N'
#define AFW_UBJSON_MARKER_TRUE            'T'
#define AFW_UBJSON_MARKER_FALSE           'F'
#define AFW_UBJSON_MARKER_INT8            'i'
#define AFW_UBJSON_MARKER_UINT8           'U'
#define AFW_UBJSON_MARKER_INT16           'I'
#define AFW_UBJSON_MARKER_INT32           'l'
#define AFW_UBJSON_MARKER_INT64           'L'
#define AFW_UBJSON_MARKER_FLOAT32         'd'
#define AFW_UBJSON_MARKER_FLOAT64         'D'
#define AFW_UBJSON_MARKER_HIGH_PRECISION  'H'
#define AFW_UBJSON_MARKER_CHAR            'C'
#define AFW_UBJSON_MARKER_STRING          'S'
#define AFW_UBJSON_MARKER_ARRAY           '['
#define AFW_UBJSON_MARKER_ARRAY_          ']'
#define AFW_UBJSON_MARKER_OBJECT          '{'
#define AFW_UBJSON_MARKER_OBJECT_         '}'
#define AFW_UBJSON_MARKER_TYPE            '$'
#define AFW_UBJSON_MARKER_COUNT           '#'

#define AFW_UBJSON_BYTES_CHAR     1
#define AFW_UBJSON_BYTES_INT8     1
#define AFW_UBJSON_BYTES_UINT8    1 
#define AFW_UBJSON_BYTES_INT16    2
#define AFW_UBJSON_BYTES_INT32    4
#define AFW_UBJSON_BYTES_INT64    8 
#define AFW_UBJSON_BYTES_FLOAT32  4 
#define AFW_UBJSON_BYTES_FLOAT64  8


/**
 * @brief Convert error from xctx to a UBJSON object.
 * @param xctx of caller.
 * @return UBJSON string.
 */
const afw_utf8_t * afw_ubjson_from_error(afw_xctx_t *xctx);

/**
 * @brief Convert an adaptive value to UBJSON.
 * @param value 
 * @param xctx of caller.
 * @return ubjson
*
 * This function can be used for callbacks of type afw_utf8_from_value_t.
 */
const afw_memory_t * afw_ubjson_from_value(
    const afw_value_t *value,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);

/**
 * @brief Convert UBJSON to an adaptive value.
 * @param ubjson UBJSON.
 * @param path Object path or NULL.
 * @param object_id Optional id of object if value is object or NULL.
 * @param pool to use for memory allocation.
 * @param xctx of caller.
 * @return value
*
 * This function can be used for callbacks of type afw_utf8_to_value_t.
*
 * The path parameter is only used if the UBJSON being parsed is an object entity.
*
 * The path must be of the form `/<adaptor id>/<object type>/<id>`.  This path
 * is parsed to provide return values for the object's
 * afw_object_meta_get_object_id(), afw_object_meta_get_path(), and
 * afw_object_get_object_type() methods.
 */
const afw_value_t * afw_ubjson_to_value(
    const afw_memory_t * ubjson,
    const afw_utf8_t *path,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);

/**
 * @brief Convert from UBJSON to adaptive object.
 * @param raw UBJSON to convert.
 * @param source_location or NULL.
 * @param adaptor_id to use for created object.
 * @param object_type_id to use for created object.
 * @param object_id to use for created object.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use.
 * @param xctx of caller.
 * @return object instance.
 *
 */
const afw_object_t *
afw_ubjson_to_object(
    const afw_memory_t *ubjson,
    const afw_utf8_t *source_location,
    const afw_utf8_t * adaptor_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t *xctx);
    
AFW_END_DECLARES

/** @} */

#endif /* __AFW_UBJSON_H__ */

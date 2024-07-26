// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Type
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OBJECT_TYPE_H
#define __AFW_OBJECT_TYPE_H

#include "afw_interface.h"

/**
 * @defgroup afw_object_type Object type
 * @ingroup afw_c_api_public
 *
 * Provide afw_object interface common support.
 * @{
 */

/**
 * @file afw_object_type.h
 * @brief Object type helpers.
 */

AFW_BEGIN_DECLARES

/** @brief Struct for afw_object_type_t. */
struct afw_object_type_s {
    const afw_object_type_t *next;
    const afw_utf8_t *adapter_id;
    const afw_utf8_t *object_type_id;
    const afw_object_t *object_type_object;
    const afw_object_t *property_types_object;
    const afw_object_t *other_properties_object;
    const afw_object_type_property_type_t *first_property_type;
    const afw_object_type_property_type_t *other_properties;
};

typedef struct afw_object_type_extended_data_type_s
afw_object_type_extended_data_type_t;


struct afw_object_type_extended_data_type_s {
    /* "dataType" or "objectType". */
    const afw_utf8_t *data_type_parameter_type;
    union {
        const afw_object_type_t *object_type;
        const afw_data_type_t *data_type;
    };
};

/** @brief Struct for afw_object_type_property_type_t. */
struct afw_object_type_property_type_s {
    const afw_object_type_property_type_t *next;
    const afw_object_t *property_type_object;
    const afw_utf8_t *property_name;

    /** @fixme add constraint things. */

    const afw_utf8_t *context_type_id;
    const afw_data_type_t *data_type;
    const afw_utf8_t *data_type_parameter;

    /* If this is data type object, this is the object type struct. */
    const afw_object_type_t *object_type;

    const afw_value_t *default_value;

    afw_boolean_t allow_write;
    afw_boolean_t allow_query;
    afw_boolean_t required;
};


/**
 * @brief Create an object type.
 * @param adapter
 * @param object_type_object.
 * @param p for result.
 * @param xctx of caller.
 * @return object type.
 */
const afw_object_type_t *
afw_object_type_internal_create(
    const afw_adapter_t *adapter,
    const afw_object_t *object_type_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Get a object type property.
 * @param object_type.
 * @param property_name is the object type property name.
 * @param xctx of caller.
 * @return value of object type property.
 */
#define afw_object_type_get_property(object_type, property_name, xctx) \
    afw_object_get_property((object_type)->object_type_object, \
        property_name, xctx)



/**
 * @brief Get a meta property as utf8.
 * @param object_type.
 * @param property_name is the object type property name.
 * @param xctx of caller.
 * @return utf8 or NULL if not found.
 */
#define afw_object_type_get_property_as_utf8(object_type_object, \
    property_name, xctx) \
    afw_object_old_get_property_as_utf8((object_type)->object_type_object, \
        property_name, xctx)


/**
 * @brief Get property type object for property
 * @param object_type.
 * @param property_name to get property type for.
 * @param xctx of caller.
 * @return property type or NULL if not found.
 *
 * If there is not a specific property type object for the named
 * property, the otherProperties property type is returned if
 * available.
 */
AFW_DECLARE(const afw_object_type_property_type_t *)
afw_object_type_property_type_get(
    const afw_object_type_t *object_type,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Get property type object for an object type's own property 
 *        or embedded property.
 * @param object_type.
 * @param property_name of property type to get. Can be dotted to access
 *       embedded property type objects.
 * @param xctx of caller.
 * @return property type or NULL if not found.
 *
 * If there is not a specific property type object for the named
 * property, the otherProperties property type is returned if
 * available.
 */
AFW_DECLARE(const afw_object_type_property_type_t *)
afw_object_type_property_type_get_extended(
    const afw_object_type_t *object_type,
    const afw_utf8_t *property_name_extended,
    afw_xctx_t *xctx);


/**
 * @brief Get next property type for object type.
 * @param object_type.
 * @param iterator pointer. Set to NULL before first call.
 * @param property_name is place to return pointer to property name.
 * @param xctx of caller.
 * @return property type or NULL if not found.
 */
AFW_DECLARE(const afw_object_type_property_type_t *)
afw_object_type_property_type_get_next(
    const afw_object_type_t *object_type,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Normalize a value based on property type.
 * @param pt.
 * @param value to normalize.
 * @param p to use.
 * @param xctx of caller.
 * @return normalized value or original value if already normalized.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_type_property_type_normalize(
    const afw_object_type_property_type_t *pt,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Normalize a value based an object type and property name.
 * @param object_type.
 * @param property_name to get property type for.
 * @param value to normalize.
 * @param p to use.
 * @param xctx of caller.
 * @return normalized value or original value if already normalized.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_type_property_normalize(
    const afw_object_type_t *object_type,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_OBJECT_TYPE_H */

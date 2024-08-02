// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework interface helpers for afw_object* header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OBJECT_H
#define __AFW_OBJECT_H

#include "afw_interface.h"

/**
 * @defgroup afw_object Object
 * @ingroup afw_c_api_public
 *
 * Provide afw_object interface common support and memory implementation.
 * @{
 */

/**
 * @file afw_object.h
 * @brief Header for interface afw_object helpers.
 */

AFW_BEGIN_DECLARES

/** @brief Quoted prefix for all core adaptive object types. */
#define AFW_OBJECT_Q_CORE_ID_PREFIX AFW_Q__Adaptive

/** @brief String prefix for all core adaptive object types. */
#define AFW_OBJECT_S_CORE_ID_PREFIX afw_s__Adaptive

/** @brief Quoted object type id for untyped adaptive object. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_OBJECT AFW_Q__AdaptiveObject_

/** @brief String object type id for untyped adaptive object. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_OBJECT afw_s__AdaptiveObject_

/** @brief Quoted object type id for Object Type object. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_OBJECT_TYPE AFW_Q__AdaptiveObjectType_

/** @brief String object type id for Object Type object. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_OBJECT_TYPE afw_s__AdaptiveObjectType_

/** @brief Quoted object type id for Value Meta object. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_VALUE_META AFW_Q__AdaptiveValueMeta_

/** @brief String object type id for Property Type object. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_VALUE_META afw_s__AdaptiveValueMeta_

/** @brief Quoted object type id for Data Type object. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_DATA_TYPE AFW_Q__AdaptiveDataType_

/** @brief String object type id for Data Type object. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_DATA_TYPE afw_s__AdaptiveDataType_

/** @brief Quoted object type id for Journal Entry object. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY AFW_Q__AdaptiveJournalEntry_

/** @brief String object type id for Journal Entry object. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY afw_s__AdaptiveJournalEntry_

/** @brief Quoted object type id for Provisioning Peer object. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_PROVISIONING_PEER AFW_Q__AdaptiveProvisioningPeer_

/** @brief String object type id for Provisioning Peer object. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_PROVISIONING_PEER afw_s__AdaptiveProvisioningPeer_

/** @brief Quoted object type id for Response. */
#define AFW_OBJECT_Q_OBJECT_TYPE_ID_RESPONSE AFW_Q__AdaptiveResponse_

/** @brief String object type id for Response. */
#define AFW_OBJECT_S_OBJECT_TYPE_ID_RESPONSE afw_s__AdaptiveResponse_

/** @brief Quoted pseudo meta property name. */
#define AFW_OBJECT_Q_PN_META AFW_Q__meta_

/** @brief String pseudo meta property name. */
#define AFW_OBJECT_S_PN_META afw_s__meta_

/** @brief Quoted pseudo meta property name for objectId. */
#define AFW_OBJECT_Q_PN_OBJECT_ID AFW_Q_objectId

/** @brief String pseudo meta property name for objectId. */
#define AFW_OBJECT_S_PN_OBJECT_ID afw_s_objectId

/** @brief Quoted pseudo meta property name for path. */
#define AFW_OBJECT_Q_PN_PATH AFW_Q_path

/** @brief String pseudo meta property name for path. */
#define AFW_OBJECT_S_PN_PATH afw_s_path

/** @brief Quoted pseudo meta property name for adapterId. */
#define AFW_OBJECT_Q_PN_ADAPTER_ID AFW_Q_adapterId

/** @brief String pseudo meta property name for adapterId. */
#define AFW_OBJECT_S_PN_ADAPTER_ID afw_s_adapterId

/** @brief Quoted pseudo meta property name for objectType. */
#define AFW_OBJECT_Q_PN_OBJECT_TYPE AFW_Q_objectType

/** @brief String pseudo meta property name for objectType. */
#define AFW_OBJECT_S_PN_OBJECT_TYPE afw_s_objectType

/** @brief Quoted pseudo meta property name for parentPaths. */
#define AFW_OBJECT_Q_PN_PARENT_PATHS AFW_Q_parentPaths

/** @brief String pseudo meta property name for parentPaths. */
#define AFW_OBJECT_S_PN_PARENT_PATHS afw_s_parentPaths

/** @brief Quoted pseudo meta property name for property types. */
#define AFW_OBJECT_Q_PN_PROPERTY_TYPES AFW_Q_propertyTypes

/** @brief String pseudo meta property name for property types. */
#define AFW_OBJECT_S_PN_PROPERTY_TYPES afw_s_propertyTypes

/** @brief Quoted _meta_.objectType */
#define AFW_OBJECT_Q_PN_META_OBJECTTYPE \
    AFW_OBJECT_Q_PN_META "." AFW_OBJECT_Q_PN_OBJECT_TYPE

/** @brief String _meta_.objectType */
#define AFW_OBJECT_S_PN_META_OBJECTTYPE afw_s_a_meta_objectType

/** @brief The object type id for object type objects is _AdaptiveObjectType_. */
#define AFW_OBJECT_ID_IS_OBJECT_TYPE(OBJECT_TYPE_ID) \
    afw_utf8_equal((OBJECT_TYPE_ID), afw_s__AdaptiveObjectType_)

/** @brief Core object type objects have an object id starting with _Adaptive. */
#define AFW_OBJECT_IS_CORE_OBJECT_TYPE_ID(OBJECT_TYPE_ID, OBJECT_ID) \
    AFW_OBJECT_ID_IS_OBJECT_TYPE(OBJECT_TYPE_ID) && \
    afw_utf8_starts_with((OBJECT_ID), afw_s__Adaptive)

/** @brief Core type ids starting with _Adaptive. */
#define AFW_OBJECT_IS_CORE_TYPE_ID(ID) \
    afw_utf8_starts_with((ID), afw_s__Adaptive)


/**
 * @brief Count the number of properties in an object.
 * @param object instance.
 * @param xctx of caller.
 * @return number of properties.
 */
AFW_DECLARE(afw_size_t)
afw_object_property_count(
    const afw_object_t *object,
    afw_xctx_t *xctx);


/**
 * @brief Determine if object is immutable.
 * @param instance of object.
 * @param xctx of caller.
 * @return boolean result.
 */
#define afw_object_is_immutable(instance, xctx) \
    (afw_object_get_setter(instance, xctx) == NULL)



#define AFW_OBJECT_IMPL_ERROR_OBJECT_NOT_ENTITY \
    AFW_THROW_ERROR_Z(general, "Object is not an entity", xctx)


/**
 * @brief Asserts that an object is an entity.
 * @param instance of object to check.
 * @param xctx of caller.
 */
#define AFW_OBJECT_ASSERT_ENTITY(instance, xctx) \
do { \
    if (instance->meta.embedding_object) \
        AFW_OBJECT_IMPL_ERROR_OBJECT_NOT_ENTITY; \
    } \
while (0)


#define AFW_OBJECT_ERROR_OBJECT_IMMUTABLE \
    AFW_THROW_ERROR_Z(read_only, "Object immutable", xctx)


/**
 * @brief Asserts that an object is mutable.
 * @param instance of object to check.
 * @param xctx of caller.
 */
#define AFW_OBJECT_ASSERT_MUTABLE(instance, xctx) \
do { \
    if (afw_object_is_immutable(instance, xctx)) \
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; \
    } \
while (0)


#define AFW_OBJECT_ERROR_PROPERTY_IMMUTABLE(property_name) \
AFW_THROW_ERROR_FZ(read_only, xctx, \
    "Object property " AFW_UTF8_FMT_Q " is immutable", \
    AFW_UTF8_FMT_ARG(property_name))

/**
 * @brief Set an object to immutable if it is not already.
 * @param instance of object.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_immutable(const afw_object_t *instance, afw_xctx_t *xctx);


/**
 * @brief Remove a property from object.
 * @param instance of object.
 * @param property_name of property to remove.
 * @param xctx of caller.
 *
 * Remove a property owned by an object.  The object ancestors are not
 * changed.  If the property does not exist, this method will do nothing.
 * If it is important to know if property exists that is being removed,
 * use method has_property() first.
 */
AFW_DECLARE(void)
afw_object_remove_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Set the value of an object's property.
 * @param instance of object.
 * @param property_name of property to set.
 * @param value to set or NULL to remove property.
 * @param xctx of caller.
 *
 * Set the value of a property owned by an object.  The property will be
 * added, if necessary.  The object ancestors are not changed.  If it is
 * important to know if property exists that is being set, use method
 * has_property() first.
 *
 * The value and name must be available for the life of the object.  Use the
 * object's pool to allocate memory for the name and value, if necessary.
 *
 * If value is NULL, the property is removed.  If the property does not exist,
 * no error is thrown.
 *
 * An exception is thrown if the object is immutable.
 */
AFW_DECLARE(void)
afw_object_set_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Set a date property from parts.
 * @param instance of object.
 * @param property_name of property to set.
 * @param year including century. Negative is BC. No 0.
 * @param month (1-12).
 * @param day of month (1-31).  Month limited to days in month..
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_property_as_date_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    int year,
    int month,
    int day,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx);



/**
 * @brief Set a dateTime property from parts.
 * @param instance of object.
 * @param property_name of property to set.
 * @param year including century. Negative is BC. No 0.
 * @param month (1-12).
 * @param day of month (1-31).  Month limited to days in month..
 * @param hour (0-24).  If hour is 24 then other values must be 0.
 * @param minute (0-59).
 * @param second (0-60+). 60 or more is allowed for leap seconds.
 * @param microsecond (0-999999).
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_property_as_dateTime_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx);


/**
 * @brief Set a dayTimeDuration property from parts.
 * @param instance of object.
 * @param property_name of property to set.
 * @param is_positive true if duration is positive or false if negative.
 * @param days (must be between 0 and AFW_INT32_MAX).
 * @param hours (must be between 0 and AFW_INT32_MAX).
 * @param minutes (must be between 0 and AFW_INT32_MAX).
 * @param seconds (must be between 0 and AFW_INT32_MAX).
 * @param microseconds (must be between 0 and AFW_INT32_MAX).
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_property_as_dayTimeDuration_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_boolean_t is_positive,
    int days,
    int hours,
    int minutes,
    int seconds,
    int microseconds,
    afw_xctx_t *xctx);



/**
 * @brief Set a time property from parts.
 * @param instance of object.
 * @param property_name of property to set.
 * @param hour (0-24).  If hour is 24 then other values must be 0.
 * @param minute (0-59).
 * @param second (0-60+). 60 or more is allowed for leap seconds.
 * @param microsecond (0-999999).
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_property_as_time_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx);


/**
 * @brief Set a yearMonthDuration property from parts.
 * @param instance of object.
 * @param property_name of property to set.
 * @param is_positive true if duration is positive or false if negative.
 * @param years (must be between 0 and AFW_INT32_MAX).
 * @param months (must be between 0 and AFW_INT32_MAX).
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_property_as_yearMonthDuration_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_boolean_t is_positive,
    int years,
    int months,
    afw_xctx_t *xctx);


/**
 * @brief Set an string property from utf8_z.
 * @param instance of object.
 * @param property_name of property to set.
 * @param string_z zero terminate string.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_set_property_as_string_from_utf8_z(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_z_t *string_z,
    afw_xctx_t *xctx);


/**
 * @brief Macro to get entity for object.
 * @param entity is place to set entity.
 * @param object to find entity for.
 */
#define AFW_OBJECT_GET_ENTITY(entity,object) \
    entity = object; \
    while (entity->meta.embedding_object) \
        entity = entity->meta.embedding_object


/**
 * @brief Parse entity path from path.
 * @param entity_path is pointer to preallocated afw_utf8_t to put results.
 * @param path is path to parse.
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_object_parse_entity_path_from_path(
    afw_utf8_t *entity_path,
    const afw_utf8_t *path)
{
    const afw_utf8_octet_t *s;

    entity_path->s = path->s;
    entity_path->len = path->len;
    for (s = path->s + path->len - 1;
        s > path->s && *s != '/';
        s--)
    {
        if (*s == '.') {
            entity_path->len = s - path->s;
        }
    }
}


/**
 * @brief Get entity for object.
 * @param object to find entity for.
 * @param xctx of caller.
 * @return entity.
 */
AFW_DEFINE_STATIC_INLINE(const afw_object_t *)
afw_object_get_entity(const afw_object_t *object, afw_xctx_t *xctx)
{
    const afw_object_t *entity;
    AFW_OBJECT_GET_ENTITY(entity, object);
    return entity;
}



/**
 * @brief Resolve an object's instance pointer.
 * @param object Object pointer to return if not NULL.
 * @param path Path of object.
 * @param xctx of caller
 * @return Path.
 *
 * Return object passed.  If the object passed is NULL and the path passed is
 * not NULL, use path to attempt to locate and return object from the current
 * xctx's object registry.
 */
AFW_DEFINE_STATIC_INLINE(const afw_object_t *)
afw_object_resolve_instance(
    const afw_object_t *object,
    const afw_utf8_t *path,
    afw_xctx_t *xctx)
{
    if (object) return object;
    if (!path) return NULL;
    /* Will not happen if objects retrieved using afw_cache_* */
    AFW_THROW_ERROR_FZ(objects_needed, xctx, AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(path));
};



/**
 * @brief Compile a property value using specified compile type.
 * @param instance of object.
 * @param property_name of property.
 * @param source_location to associate with compiled value or NULL (path used).
 * @param compile_type (set afw_compile_type_t)
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return result of compiling the property value.
 * 
 * A slash ('/') followed by the property name will be appended to the source
 * location when calling compile.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_get_property_compile_as(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile and evaluate a property value using specified compile type.
 * @param instance of object.
 * @param property_name of property.
 * @param source_location to associate with compiled value or NULL (path used).
 * @param compile_type (set afw_compile_type_t)
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return result of compiling and evaluating the property value.
 * 
 * A slash ('/') followed by the property name will be appended to the source
 * location when calling compile.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_get_property_compile_and_evaluate_as(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Get the value of an object's own property or embedded property.
 * @param instance of object.
 * @param property_name of property to get. Can be dotted to access
 *       embedded object properties.
 * @param xctx of caller.
 * @return value or NULL if not found;
 *
 * This function also supports several read-only pseudo property names.
 *
 * | Property name       | Value
 * | ------------------  | --------------------------------------
 * | _meta_.objectId     | The result of calling afw_object_meta_get_object_id().
 * | _meta_.objectType   | The result of calling afw_object_meta_get_object_type_id().
 * | _meta_.parentPaths  | The result of calling afw_object_meta_get_parent_paths().
 * | _meta_.path         | The result of calling afw_object_meta_get_path().
 *
 */
AFW_DECLARE(const afw_value_t *)
afw_object_get_property_extended(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Return a NULL terminated list of values for an object property in a
 *    specified pool.
 * @param instance of object.
 * @param property_name of property.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return result of afw_value_as_array_of_values() on the property value.
 */
AFW_DECLARE(const afw_value_t * const *)
afw_object_old_get_property_as_array_of_values(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Return a NULL terminated list of strings for an object property in a
 *    specified pool.
 * @param instance of object.
 * @param property_name of property.  See afw_object_get_property().
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return NULL terminated list of strings or NULL if value is NULL.
 *
 * The value can be a list, bag, or scalar.
 */
AFW_DECLARE(const afw_utf8_t * const *)
afw_object_old_get_property_as_array_of_strings(
    const afw_object_t *instance, const afw_utf8_t *property_name,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Return a compiled script property value.
 * @param instance of object.
 * @param property_name of property.  See afw_object_get_property().
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return the value representation of a compiled script property.
 *
 * The property value must be an evaluated value.  If the value is
 * data type script or string, it is compiled as a script; otherwise,
 * it's just returned.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_old_get_property_as_compiled_script(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Return a compiled template property value.
 * @param instance of object.
 * @param property_name of property.  See afw_object_get_property().
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return the value representation of a compiled template property.
 *
 * The property value must be an evaluated value.  If the value is
 * data type template or string, it is compiled as a template; otherwise,
 * it's just returned.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_old_get_property_as_compiled_template(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Get an object's property value as a boolean.
 * @param instance of object.
 * @param property_name Name of property to get.
 * @param xctx of caller.
 * @return value or false if not found.
 *
 * This accepts a boolean value of a case insensitive string value of "true",
 * "t", "false", "f", "0", or "1".
 */
AFW_DECLARE(afw_boolean_t)
afw_object_old_get_property_as_boolean_deprecated(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Get an object's property value as an integer.
 * @param instance of object.
 * @param property_name Name of property to get.
 * @param found true if property exists.
 * @param xctx of caller.
 * @return value or false if not found.
 *
 * This is like afw_object_old_get_property_as_integer() except the value
 * will be converted to integer if needed instead of throwing a type safe
 * error.
 */
AFW_DECLARE(afw_integer_t)
afw_object_old_get_property_as_integer_deprecated(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_boolean_t *found,
    afw_xctx_t *xctx);


/**
 * @brief Get an object's property value as a string in specified pool.
 * @param instance of object.
 * @param property_name Name of property to get.
 * @param p pool to use for returned string.
 * @param xctx of caller.
 * @return value or NULL if not found;
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_old_get_property_as_utf8(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Get an object's property value including ancestors as utf8_z in
 *    specified pool.
 * @param instance of object.
 * @param property_name Name of property to get.
 * @param p pool to use for returned string.
 * @param xctx of caller.
 * @return value or NULL if not found;
 *
 * Use with caution since strings can contain x'00' characters.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_object_old_get_property_as_utf8_z(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Merge properties from one object into another.
 * @param instance of object to merge into.
 * @param from this object.
 * @param replace properties in instance.
 * @param xctx of caller.
 *
 * Embedded objects will be merged as well.
 */
AFW_DECLARE(void)
afw_object_merge(
    const afw_object_t *instance,
    const afw_object_t *from,
    afw_boolean_t replace,
    afw_xctx_t *xctx);



/**
 * @brief Create memory object associative array.
 * @param p to use for associative array.
 * @param xctx of caller.
 * @return object associative array instance.
 */
AFW_DECLARE(const afw_object_associative_array_t *)
afw_object_memory_associative_array_create(
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Default object behaviour.
 *
 * A subpool will be created of the p passed to create and be considered the
 * object's pool.  The lifetime of the object's pool will be controlled by
 * calls to the object's get_reference() and release() methods.
 */
#define AFW_OBJECT_MEMORY_OPTION_managed              0


/**
 * @brief Object should be unmanaged.
 *
 * Calls to methods release() and get_reference() will not do reference
 * counting and the associated pool and will not call release() for the pool.
 *
 * Normally, managed entity objects and embedded object reside in a pool
 * who's lifetime is controlled by the reference count of the object.
 * Specifying this option disables this normal behaviour.
 */
#define AFW_OBJECT_MEMORY_OPTION_unmanaged            (1 << 0)


/**
 * @brief Object cedes control of the specified pool.
 *
 * The p passed to create will cede control to the object and be considered
 * the object's pool.  The lifetime of the object's pool will be controlled
 * by calls to the object's get_reference() and release() methods.
 */
#define AFW_OBJECT_MEMORY_OPTION_managed_cede_p               (1 << 1)


/** @brief Test memory object option mask option. */
#define AFW_OBJECT_MEMORY_OPTION_IS(options_mask, option) \
    ((((options_mask) & (AFW_OBJECT_MEMORY_OPTION_ ## option))) != 0)


/**
 * @brief Create an empty entity object with options.
 * @param options as defined by AFW_OBJECT_MEMORY_OPTION_* defines.
 * @param p to use based on options.
 * @param xctx of caller.
 * @return instance of new object.
 *
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_with_options(
    int options, const afw_pool_t * p, afw_xctx_t *xctx);


/**
 * @brief Create a composite of immutable objects.
 * @param mutable true makes composite mutable.
 * @param p is pool for result.
 * @param xctx of caller.
 * @param ... is one or more objects terminated by a NULL.
 * @return instance of new object.
 *
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_composite(
    afw_boolean_t mutable,
    const afw_pool_t *p, afw_xctx_t *xctx,
    ...);



/**
 * @brief Typedef for an object property get callback.
 * @param data to passed to afw_object_create_properties_callback().
 * @param property_name to get.
 * @param xctx of caller.
 * @return property value or NULL if not found.
 */
typedef const afw_value_t *(*afw_object_properties_callback_entry_get_t) (
    void *data,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);



/**
 * @brief Typedef for an object property set callback.
 * @param data to passed to afw_object_create_properties_callback().
 * @param property_name to set.
 * @param value to set.
 * @param xctx of caller.
 */
typedef void (*afw_object_properties_callback_entry_set_t) (
    void *data,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Struct for afw_object_properties_callback_entry_t.
 *
 * An array of this is passed to the afw_object_create_properties_callback()
 * in as the callback parameter.  This consists of the property name, get
 * callback, and set callback or NULL if immutable.
 */
struct afw_object_properties_callback_entry_s {
    const afw_utf8_t *property_name;
    afw_object_properties_callback_entry_get_t get;
    afw_object_properties_callback_entry_set_t set;
};



/**
 * @brief Create a mutable composite of unmutable objects.
 * @param data to pass as first parameter to callbacks.
 * @param count is number of callbacks.
 * @param callbacks array.
 * @param p is pool for result.
 * @param xctx of caller.
 * @return instance of new object.
 *
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_properties_callback(
    void *data,
    afw_size_t count,
    const afw_object_properties_callback_entry_t callbacks[],
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create an empty entity object in memory in specified pool and cede
 *    control of the pool to the object.
 * @param p that will be used and controlled by object.
 * @param xctx of caller.
 * @return instance of new object.
 *
 * The pool specified is use to hold the object and it's properties. The pool
 * will be released when the object is released.
 *
 * Only a single entity object should cede control of any individual pool or
 * errors will occur.
 *
 * This function is useful when a pool's resources are needed before an object
 * is created, but those resource need to have the same lifetime as the
 * object.
 *
 * Example:
 *
 * const afw_pool_t * new_p;
 * const afw_object_t * object;
 *
 * new_p = afw_pool_calloc(xctx->p, xctx);
 *
 * ... allocate memory and register cleanup using new_p ...
 *
 * object = afw_object_create_cede_p(new_p, xctx);
 *
 * ... set properties and use object ...
 *
 * afw_object_release(object, xctx);
 */
#define afw_object_create_cede_p(p, xctx) \
    afw_object_create_with_options( \
        AFW_OBJECT_MEMORY_OPTION_managed_cede_p, p, xctx)


/**
 * @brief Create an empty entity object in its own pool.
 * @param p to use for the object.
 * @param xctx of caller.
 * @return instance of new object.
 *
 * This function creates a subpool of the pool specified to hold the object and
 * it's properties.
 *
 * A call to afw_object_release() for this object will release this subpool.
 */
#define afw_object_create(p, xctx) \
    afw_object_create_with_options( \
        AFW_OBJECT_MEMORY_OPTION_managed, p, xctx)


/**
 * @brief Create an empty unmanaged object in memory.
 * @param p to use for the object.
 * @param xctx of caller.
 * @return instance of new object.
 *
 * Reference counting is not done for an unmanaged object, so the lifetime of
 * the object will be controlled by the lifetime of the p specified.
 */
#define afw_object_create_unmanaged(p, xctx) \
    afw_object_create_with_options( \
        AFW_OBJECT_MEMORY_OPTION_unmanaged, p, xctx)


/**
 * @brief Create an empty embedded object in a memory object.
 * @param embedding_object of the new object.
 * @param property_name of embedded object within embedding object.
 * @param xctx of caller.
 * @return instance of new object.
 *
 * The embedded object and its properties will use the embedding object's
 * pool, options, and reference counting.  The property_name property of the
 * embedding object will be set to the new embedding object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_embedded(
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);



/**
 * @brief Insure embedded object exists creating if necessary.
 * @param embedding_object of the new object.
 * @param property_name of embedded object within embedding object.
 * @param xctx of caller.
 * @return instance of embedded object.
 *
 * The embedded object and its properties will use the embedding object's
 * pool, options, and reference counting.  The property_name property of the
 * embedding object will be set to the new embedding object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_insure_embedded_exists(
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);




/**
 * @brief Helper macro to create a new entity or embedded object.
 * @param result will be set to new object instance.
 * @param embedding_object for new embedded object or NULL for entity.
 * @param property_name for embedded object or NULL for entity.
 * @param always_create_unmanaged object (ignore embedding and cede)
 * @param cede_p if true, the entity object will cede control of entity_p.
 * @param entity_p to use for entity object.  Ignored for embedded object.
 * @param xctx of caller.
 *
 * This macro will call afw_object_create,
 * afw_object_create_cede_p() or
 * afw_object_create_embedded() depending on whether embedding_object
 * is NULL and cede_p is true.
 *
 * Param cede_p should only be used to cede control of a pool to a single
 * entity object.
 *
 * This macro is useful for situations such as a recursive object parse
 * function that uses an embedding_object and embedding_property_name
 * variable that will be NULL on the first call and have the embedder on
 * subsequent calls.
 */
#define AFW_OBJECT_CREATE_ENTITY_OR_EMBEDDED( \
    result,embedding_object,property_name,always_create_unmanaged, \
    cede_p,entity_p,xctx) \
    if (always_create_unmanaged || \
        (property_name && afw_utf8_equal(property_name, afw_s__meta_))) \
    { \
        result = afw_object_create_unmanaged(entity_p, xctx); \
    } \
    else if (embedding_object) { \
        result = afw_object_create_embedded( \
            embedding_object, property_name, xctx); \
    } \
    else { \
        result = afw_object_create_with_options( \
            (cede_p) \
            ? AFW_OBJECT_MEMORY_OPTION_managed_cede_p \
            : AFW_OBJECT_MEMORY_OPTION_managed, \
            entity_p, xctx); \
    } \



/**
 * @brief Clone an object to a specified pool.
 * @param object to clone.
 * @param p used for cloned object.
 * @param xctx of caller.
 * @return cloned object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_clone(
    const afw_object_t *object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a memory object with properties from two other objects.
 * @param primary object.  Properties take precedence.
 * @param secondary object. Properties that are in primary will be ignored.
 * @param p to use for the object.
 * @param xctx of caller.
 * @return Merged object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_merged(
    const afw_object_t *primary,
    const afw_object_t *secondary,
    const afw_pool_t *p,
    afw_xctx_t *xctx);




/**
 * @brief Create an aggregate object with an external object list.
 * @param object_list is a NULL terminated list of object pointers.
 * @param p to use for the delta object.
 * @param xctx of caller.
 * @return aggregate object.
 * 
 * IMPORTANT:  This implementation of object only supports one use of the
 *             "get_next" functions per instance at a time since the iterator
 *             memory is part of the instance's self.
 * 
 * The object_list parameter is stored and used each time an object method is
 * called so it must remain allocated.
 * 
 * This object_list can be modified by the caller after calling
 * afw_object_aggregate_external_create() except in the case of reusing an
 * iterator to call afw_object_get_next_property() and
 * afw_object_get_next_property_meta().
 * 
 * Each request to access the object will process the individual objects in
 * order. For instance, afw_object_get_property() will go through the list of
 * objects until it finds the property requested. Also,
 * afw_object_get_next_property() and afw_object_get_next_property_meta() will
 * make sure that they only returns the first instance of a property name.
 * 
 * The setter of the first object instance or NULL is returned by
 * afw_object_get_setter().
 * 
 * After afw_object_aggregate_external_create(), each object in the object_list
 * can still be accessed individually.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_aggregate_external_create(
    const afw_object_t * const *object_list,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a const object from key/value utf-8 null terminated strings.
 * @param path for object.
 * @param pairs key_value pairs of afw_utf8_z_t with NULL at end.
 * @param p for object.
 * @param xctx of caller.
 * @return object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_create_const_from_key_value_strings_z(
    const afw_utf8_t *path,
    const afw_utf8_z_t * const *pairs,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_OBJECT_H */

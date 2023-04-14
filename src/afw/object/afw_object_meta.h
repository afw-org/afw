// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_object Interface Meta Helpers
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_OBJECT_META_H
#define __AFW_OBJECT_META_H


/**
 * @addtogroup afw_c_api
 *
 * @{
 */

/**
 * @defgroup afw_object_meta Object meta
 *
 * Provide afw_object interface common support.
 * @{
 */

/**
 * @file afw_object_meta.h
 * @brief afw_object Interface meta helpers.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES




/* Struct for meta object self. */
struct afw_object_meta_object_s {
    afw_object_t pub;
    afw_object_setter_t setter;

    /* Object type or NULL if not available. */
    const afw_object_type_t *object_type;

    /*
     * Delta from object type of current instance.  This is often
     * otherProperties.
     */
    const afw_object_t *delta;
};


/**
 * @brief Return meta object for an object or NULL if there is not one.
 * @param instance of object whose meta object is to be returned.
 * @return meta object.
 */
#define afw_object_meta_object(instance) \
    (const afw_object_t *)((instance)->meta.meta_object)



/**
 * @brief Add an error message to instance's meta.
 * @param instance of object.
 * @param message to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_meta_add_error(
    const afw_object_t *instance,
    const afw_utf8_t *message,
    afw_xctx_t *xctx);



/**
 * @brief Add a null terminated error message to instance's meta.
 * @param instance of object.
 * @param message_z null terminated string error message.
 * @param xctx of caller.
 */
#define afw_object_meta_add_error_z(instance, \
    message_z, xctx) \
afw_object_meta_add_error(instance, \
    afw_utf8_create(message_z, AFW_UTF8_Z_LEN, (instance)->p, xctx), \
    xctx)



/**
 * @brief Add a formatted error message to instance's meta.
 * @param instance of object.
 * @param xctx of caller.
 * @param format_z format for error message.
 * @param ... for format_z.
 */
#define afw_object_meta_add_error_fz(instance, \
    xctx, format_z, ...) \
afw_object_meta_add_error(instance, \
    afw_utf8_printf((instance)->p, xctx, format_z, __VA_ARGS__), \
    xctx)



/**
 * @brief Add a needed object type object.
 * @param instance of object.
 * @param object_type object.
 * @param xctx of caller.
 *
 * An error is thrown if the object is immutable.
 *
 * This will add/replace a property with then property name of the supplied
 * object type in the meta objectTypes property of the instance's entity.
 */
AFW_DECLARE(void)
afw_object_meta_add_needed_object_type(
    const afw_object_t *instance,
    const afw_object_t *object_type,
    afw_xctx_t *xctx);


/**
 * @brief Add a thrown error to instance's meta.
 * @param instance of object.
 * @param error to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_meta_add_thrown_error(
    const afw_object_t *instance,
    const afw_error_t *error,
    afw_xctx_t *xctx);



/**
 * @brief Add an error message for a property to instance's meta.
 * @param instance of object.
 * @param message to add.
 * @param property_name of property.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_meta_add_property_error(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *message,
    afw_xctx_t *xctx);



/**
 * @brief Add a null terminate error message for a property to instance's meta.
 * @param instance of object.
 * @param property_name of property.
 * @param message_z null terminated string error message.
 * @param xctx of caller.
 */
#define afw_object_meta_add_property_error_z(instance, \
    property_name, message_z, xctx) \
afw_object_meta_add_property_error(instance, property_name, \
    afw_utf8_create(message_z, AFW_UTF8_Z_LEN, (instance)->p, xctx), \
    xctx)



/**
 * @brief Add a formatted error message for a property to instance's meta.
 * @param instance of object.
 * @param property_name of property.
 * @param xctx of caller.
 * @param format_z format for error message.
 * @param ... for format_z.
 */
#define afw_object_meta_add_property_error_fz(instance, \
    property_name, xctx, format_z, ...) \
afw_object_meta_add_property_error(instance, property_name, \
    afw_utf8_printf((instance)->p, xctx, format_z, __VA_ARGS__), \
    xctx)



/**
 * @brief Add a thrown error for a property to instance's meta.
 * @param instance of object.
 * @param property_name of property or NULL.
 * @param index if list entry, otherwise -1.
 * @param error to add.
 * @param xctx of caller.
 *
 * If property_name is NULL, afw_object_meta_add_thrown_error() is called.
 */
AFW_DECLARE(void)
afw_object_meta_add_thrown_property_error(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_integer_t index,
    const afw_error_t *error,
    afw_xctx_t *xctx);



/**
 * @brief Check if object flagged for errors.
 * @param instance of object.
 * @param xctx of caller.
 * @return true if meta property hasError is true.
 */
AFW_DECLARE(afw_boolean_t)
afw_object_meta_has_errors(
    const afw_object_t *instance,
    afw_xctx_t *xctx);


/**
 * @brief Log meta errors.
 * @param instance of object.
 * @param source_location to preface log messages.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_meta_log_errors(
    const afw_object_t *instance,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx);


/**
 * @brief Create an object instance to access an object's meta with options.
 * @param instance of object whose meta is to be accessed.
 * @param options.
 * @param p to use.
 * @param xctx of caller.
 * @return Object instance to access meta or NULL if there is none to access.
 *
 * This is a special purpose and limited implementation of the afw_object
 * inteface.  It only implements the get_next_property() and
 * afw_object_release() methods -- all other methods will throw an error.
 *
 * The original purpose of this implementation is for implementations of the
 * write_value) method of the afw_content_type interface to assist in
 * producing the requested meta information.  See afw_json_from_value.c for an
 * example of its use.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_meta_create_accessor_with_options(
    const afw_object_t *instance,
    const afw_object_options_t *options,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Return meta object for an object creating an empty one if
 *    needed.
 * @param instance of object whose meta object is to be returned.
 * @param xctx of caller.
 * @return meta object.
 */
AFW_DECLARE(const afw_object_meta_object_t *)
afw_object_meta_get_nonempty_meta_object(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Return meta delta object for an object creating an empty one if
 *    needed.
 * @param instance of object whose meta object is to be returned.
 * @param xctx of caller.
 * @return meta object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_meta_get_nonempty_delta(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Get a meta property.
 * @param instance of object.
 * @param property_name of meta property.  Can be dotted.
 * @param xctx of caller.
 * @return value of meta property.
 */
#define afw_object_meta_get_property(instance, property_name, xctx) \
(instance->meta.meta_object) \
    ? afw_object_get_property(afw_object_meta_object(instance), \
        property_name, xctx) \
    : NULL



/**
 * @brief Get a meta property as single string.
 * @param instance of object.
 * @param property_name of meta property.  Can be dotted.
 * @param xctx of caller.
 * @return value of meta property.
 */
#define afw_object_meta_get_property_as_string(instance, \
    property_name, xctx) \
(instance->meta.meta_object) \
    ? afw_object_old_get_property_as_string( \
        afw_object_meta_object(instance), \
            property_name, xctx) \
    : NULL



/**
 * @brief Get entity object's embedding object.
 * @param instance of object.
 * @param xctx of caller.
 * @return embedding object or NULL.
 */
#define afw_object_meta_get_embedding_object(instance, xctx) \
((instance)->embedding_object)



/**
 * @brief Get entity object's object id.
 * @param instance of object.
 * @param xctx of caller.
 * @return object id or NULL.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_meta_get_object_id(
    const afw_object_t *instance,
    afw_xctx_t *xctx);


/**
 * @brief Get object's object_type_id.
 * @param instance of object.
 * @param xctx of caller.
 * @return embedding object or NULL.
 */
#define afw_object_meta_get_object_type_id(instance, xctx) \
( \
    (instance)->meta.object_type_uri && \
    (instance)->meta.object_type_uri->len != 0 \
) \
? (instance)->meta.object_type_uri : NULL



/**
 * @brief Get object's property name in embedding object.
 * @param instance of object.
 * @param xctx of caller.
 * @return property name or NULL.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_meta_get_property_name(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Get entity object's object id value.
 * @param instance of object.
 * @param xctx of caller.
 * @return object id or NULL.
 */
AFW_DECLARE(const afw_value_string_t *)
afw_object_meta_get_object_id_value(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Add a parent path to instance's meta.
 * @param instance of object.
 * @param parent_path.
 * @param xctx of caller.
 *
 * If the object already has a parent path, this one will be added to the end.
 */
AFW_DECLARE(void)
afw_object_meta_add_parent_path(
    const afw_object_t *instance,
    const afw_utf8_t *parent_path,
    afw_xctx_t *xctx);



/**
 * @brief Get meta parentPaths property value.
 * @param instance of object.
 * @param xctx of caller.
 * @return parent paths value or NULL.
 */
AFW_DECLARE(const afw_value_list_t *)
afw_object_meta_get_parent_paths_value(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Get an object's path.
 * @param instance of object.
 * @param xctx of caller.
 * @return path or NULL.
 *
 * If the object does not have a path, is an embedded object, and is not a
 * const object (has a p), afw_object_path_make_for_embedded) will be used
 * to make the path and that path will be stored as the object's path.  This
 * works this way so that paths do not have to be created for embedded objects
 * unless the are accessed, so as the use of the inheritedFrom object option.
 *
 * If a object is a const object and does not have a path, NULL is returned.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_meta_get_path(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Get the property type object for an object's property from the meta
 *    of an object, creating it if needed.
 * @param instance of object.
 * @param property_name of property in instance.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_meta_get_property_type(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);



/**
 * @brief Set an object's meta from a meta object.
 * @param instance of object whose meta is to be set.
 * @param meta object.
 * @param xctx of caller.
 *
 * This will set an instances parentPaths and path meta from a meta object.
 * The meta must have a lifetime as long as the instance.
 *
 * @fixme Decide if meta object is validated at this point or later.  Probably
 *    now or with a different call.
 */
AFW_DEFINE(void)
afw_object_meta_set_meta_object(
    const afw_object_t *instance,
    const afw_object_t *meta,
    afw_xctx_t *xctx);



/**
 * @brief Create and set object's meta to an empty object and return delta.
 * @param instance of object whose meta is to be set to empty object.
 * @param xctx of caller.
 * @return meta object added.
 *
 * Create an new unmanaged object to hold the object meta in the pool of the
 * instance and set instance->meta to that object.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_meta_set_empty(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Set object's ids.
 * @param instance of object.
 * @param adaptor_id
 * @param object_type_id
 * @param object_id
 * @param xctx of caller.
 *
 * This produces the same results as afw_object_meta_set_ids_using_path().
 *
 * An error is thrown if the object is immutable.
 */
AFW_DECLARE(void)
afw_object_meta_set_ids(
    const afw_object_t *instance,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx);



/**
 * @brief Set object's ids using path.
 * @param instance of object.
 * @param path
 * @param xctx of caller.
 *
 * This produces the same results as afw_object_meta_set_ids().
 *
 * An error is thrown if the object is immutable.
 */
AFW_DECLARE(void)
afw_object_meta_set_ids_using_path(
    const afw_object_t *instance,
    const afw_utf8_t *path,
    afw_xctx_t *xctx);



/**
 * @brief Set object's meta to indicate object is read-only.
 * @param instance of object.
 * @param xctx of caller.
 *
 * This sets allowChange and allowDelete to false in meta.
 */
AFW_DECLARE(void)
afw_object_meta_set_read_only(
    const afw_object_t *instance,
    afw_xctx_t *xctx);



/**
 * @brief Set object's object type id.
 * @param instance of object.
 * @param object_type of object.
 * @param xctx of caller.
 *
 * An error is thrown if the object is immutable.
 */
AFW_DECLARE(void)
afw_object_meta_set_object_type(
    const afw_object_t *instance,
    const afw_object_type_t *object_type,
    afw_xctx_t *xctx);



/**
 * @brief Set object's object type id.
 * @param instance of object.
 * @param object_type_id of object.
 * @param xctx of caller.
 *
 * An error is thrown if the object is immutable.
 */
AFW_DECLARE(void)
afw_object_meta_set_object_type_id(
    const afw_object_t *instance,
    const afw_utf8_t *object_type_id,
    afw_xctx_t *xctx);



/**
 * @brief Set meta parentPaths property.
 * @param instance of object.
 * @param parent_paths value.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_object_meta_set_parent_paths(
    const afw_object_t *instance,
    const afw_value_list_t *parent_paths,
    afw_xctx_t *xctx);



/**
 * @brief Set a property in the meta of an object.
 * @param instance of object.
 * @param property_name of meta property to set.
 * @param value to set.
 * @param xctx of caller.
 */
#define afw_object_meta_set_property(instance, property_name, value, xctx) \
afw_object_set_property( \
    afw_object_meta_get_nonempty_delta(instance, xctx), property_name, value, xctx)



/**
 * @brief Set a string property in the meta of an object from utf8_z.
 * @param instance of object.
 * @param property_name of meta property to set.
 * @param string_z zero terminate string.
 * @param xctx of caller.
 */
#define afw_object_meta_set_property_from_utf8_z(instance, \
    property_name, string_z, xctx) \
afw_object_set_property_as_string_from_utf8_z( \
    afw_object_meta_get_nonempty_delta(instance, xctx), \
        property_name, string_z, xctx)



/**
 * @brief Set a property in the meta of an object as a value and data type.
 * @param instance of object.
 * @param property_name of meta property to set.
 * @param data_type unquoted id
 * @param value to set.
 * @param xctx of caller.
 */
#define afw_object_meta_set_property_as(instance, property_name, \
    data_type, value, xctx) \
afw_object_set_property_as_ ## data_type( \
    afw_object_meta_get_nonempty_delta(instance, xctx), \
    property_name, value, xctx)



/**
 * @brief Set a property type property for a property in the meta of an object.
 * @param instance of object.
 * @param property_name of property in instance.
 * @param property_type_property_name is name of property in property type.
 * @param value to set.
 * @param xctx of caller.
 */
#define afw_object_meta_set_property_type_property(instance, \
    property_name, property_type_property_name, value, xctx) \
afw_object_set_property( \
    afw_object_meta_get_property_type(instance, property_name, xctx), \
    property_type_property_name, value, xctx)



/**
 * @brief Set a property type string property for a property in the meta of
 *    an object from utf8_z.
 * @param instance of object.
 * @param property_name of property in instance.
 * @param property_type_property_name is name of property in property type.
 * @param string_z zero terminate string.
 * @param xctx of caller.
 */
#define afw_object_meta_set_property_type_property_from_utf8_z(instance, \
    property_name, property_type_property_name, string_z, xctx) \
afw_object_set_property_as_string_from_utf8_z( \
    afw_object_meta_get_property_type(instance, property_name, xctx), \
    property_type_property_name, string_z, xctx)



/**
 * @brief Set a property type property for a property in the meta of an object
 *    as a value and data type.
 * @param instance of object.
 * @param property_name of meta property to set.
 * @param property_type_property_name is name of property in property type.
 * @param data_type unquoted id
 * @param value to set.
 * @param xctx of caller.
 */
#define afw_object_meta_set_property_type_property_as(instance, \
    property_name, property_type_property_name, \
    data_type, value, xctx) \
afw_object_set_property_as_ ## data_type( \
    afw_object_meta_get_property_type(instance, property_name, xctx), \
    property_type_property_name, value, xctx)



/**
 * @brief Set object's meta using a clone of the meta of another object.
 * @param instance of object whose meta is to be set.
 * @param from is object whose meta is to be cloned.
 * @param xctx of caller.
 *
 * If the from object has no meta, this function just returns.
 *
 * If the meta is already set in the instance, an error is thrown.
 */
AFW_DECLARE(void)
afw_object_meta_clone_and_set(
    const afw_object_t *instance,
    const afw_object_t *from,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_OBJECT_META_H */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptive Object Path
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OBJECT_PATH_H
#define __AFW_OBJECT_PATH_H

#include "afw_interface.h"

/**
 * @defgroup afw_object_path Adaptive object path
 * @ingroup afw_c_api_public
 *
 * Provide adaptive object path support.
 * @{
 */

/**
 * @file afw_object_path.h
 * @brief Header for Adaptive Object Path
 */

AFW_BEGIN_DECLARES

/** @brief Property name path struct. */
struct afw_object_path_property_name_entry_s {
    const afw_object_path_property_name_entry_t *next;
    afw_utf8_t property_name;
};


/**
 * @brief Typedef for parsed object path.
 * 
 * `/<adaptor_id>/<object_type_id>[;own]/[<object_id>][.<property_name>]*`
 *
 */
struct afw_object_path_parsed_s {

    /* @brief Copy of path passed to afw_object_path_parse(). */
    afw_utf8_t original_path;

    /* @brief Normalized path. */
    afw_utf8_t normalized_path;

    /* @brief Entity part of normalized path. */
    afw_utf8_t entity_path;

    /* @brief `<adaptor_id>` */
    afw_utf8_t adaptor_id;

    /* @brief `<object_type_id>` */
    afw_utf8_t object_type_id;

    /* @brief `<object_id>` */
    afw_utf8_t entity_object_id;

    /* @brief `<object_id>` plus any dotted property names. NOT URI DECODED. */
    afw_utf8_t undecoded_object_id;

    /* @brief List of `<property_name>` if uri has dotted property names. */
    const afw_object_path_property_name_entry_t *first_property_name;

    /* @brief Options object or NULL. See afw_object_options.h. */
    const afw_object_t *options_object;

    /* @brief Options based on options_object or NULL. */
    const afw_object_options_t *options;

    /* @brief Current path was used for substitutions. */
    afw_boolean_t substitution_occurred;

    /* @brief Adaptor id was substituted from current path. */
    afw_boolean_t substituted_adaptor_id;

    /* @brief Object type id was substituted from current path. */
    afw_boolean_t substituted_object_type_id;

    /* @brief Entire object id was substituted from current path. */
    afw_boolean_t substituted_entire_object_id;

    /* @brief Entity object id was substituted from current path. */
    afw_boolean_t substituted_entity_object_id;

    /* @brief At least one property name was substituted from current path. */
    afw_boolean_t substituted_property_name;

    /* @brief At least one property name was substituted from current path. */
    afw_boolean_t contains_unresolved_substitutions;
};


/**
 * @brief Get object property value using property names.
 * @param object containing first property.
 * @param first_property_name is first of list of names.
 * @param xctx of caller
 * @return value or NULL if not found.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_path_property_name_list_get_property(
    const afw_object_t *object,
    const afw_object_path_property_name_entry_t *first_property_name,
    afw_xctx_t *xctx);


/**
 * @brief Construct an object path in a specified pool.
 * @param adaptor_id Adaptor id.
 * @param object_type_id Object type id.
 * @param object_id Object id.
 * @param p Pool return value will be placed in.
 * @param xctx of caller
 * @return pointer to object path.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_path_make(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Construct the path for an embedded object.
 * @param embedded_object object.
 * @param p Pool return value will be placed in.
 * @param xctx of caller
 * @return path or NULL.
 *
 * If entity path is NULL or contains asterisks, NULL is returned.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_path_make_for_embedded(
    const afw_object_t *embedded_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Construct a property name expression from property name list.
 * @param first property name entry.
 * @param p Pool return value will be placed in.
 * @param xctx of caller
 * @return pointer to object path.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_object_path_make_property_name_expression(
    const afw_object_path_property_name_entry_t *first,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Parse an object value path in specific pool.
 * @param path to parse.
 * @param current_path used for resolving asterisks in path or NULL.
 * @param default_options or NULL.
 * @param p to use for result.
 * @param xctx of caller
 * @return Parsed path or NULL if path is invalid.
 *
 * Parse an local path.
 *
 * Generally, a path is of the form:
 *
 * `/<adaptorId>/<objectType>/<objectId>` 
 *
 * where `<adaptorId>` is an adaptor id of the running instance, `<objectType>`
 * if the object type at path `/<adaptorId>/_AdaptiveObjectType_/<objectType>`,
 * and `<objectId>` is the entity object id `<entityObjectId>` followed by zero
 * or more period "." `<propertyName>`.
 *
 * `<objectType>` can also have an options part where the object type id is
 * followed by a semicolon ";" and one or more `<name>` with optional
 * equal "=" `<value>` separated with an ampersand "&".  See
 * afw_object_options.h for more information on options.
 *
 * Local paths can be data type anyURI values.  See data type anyURI for more
 * information.
 *
 * If current_path is specified, the path parameter can include asterisks
 * that are replaced with corresponding parts of current_path. In this case,
 * `<adaptorId>`, `<objectType>`, `<entityObjectId>`, and `<propertyName>` can
 * be an asterisk "*" to use the corresponding part from the current_path, plus
 * double asterisk "**" can be specified for the entire `<objectId>` to use the
 * entire `<objectId>` from the current path.  Also, `<objectId>` can be
 * specified alone without a preceding slash "/" when the `<adaptorId>` and
 * `<objectType>`are the same as in the current path.  No substitution occurs in
 * the options part of `<objectType>`.
 */
AFW_DECLARE(const afw_object_path_parsed_t *)
afw_object_path_parse(
    const afw_utf8_t *path,
    const afw_utf8_t *current_path,
    const afw_object_options_t *default_options,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Parse simple path into ids.
 * @param path to parse.
 * @param adaptor_id return location.
 * @param object_type_id return location.
 * @param object_id return location.
 * @param p to use for result.
 * @param xctx of caller.
 *
 * This is a lightweight path parse that breaks the path into up into 3 parts
 * that are separated by '/'.  Each part will be uri decoded.  This is not
 * appropriate if parts contain multiple segments, so this should only be
 * used for simple paths.  Use afw_object_path_parse() for relative paths,
 * object options, normalization, and to break apart dotted property names.
 *
 * The returned values may point to locations in the path passed.
 */
AFW_DECLARE(void)
afw_object_path_parse_simple(
    const afw_utf8_t *path,
    const afw_utf8_t * *adaptor_id,
    const afw_utf8_t * *object_type_id,
    const afw_utf8_t * *object_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Turn a parsed path into an object representation.
 * @param parsed path.
 * @param p to use for result.
 * @param xctx of caller
 * @return _AdaptiveParsedURI_ representation of parsed path.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_path_parsed_to_object(
    const afw_object_path_parsed_t *parsed,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Determine if two parsed paths are equivalent.
 * @param parsed1 parsed path.
 * @param parsed2 parsed path.
 * @param xctx of caller
 * @return boolean result of equivalent test.
 */
AFW_DECLARE(afw_boolean_t)
afw_object_path_parsed_are_equivalent(
    const afw_object_path_parsed_t *parsed1,
    const afw_object_path_parsed_t *parsed2,
    afw_xctx_t *xctx);


/**
 * @brief Determine if two paths are equivalent.
 * @param path1 parsed path.
 * @param path2 parsed path.
 * @param current_path used for resolving asterisks in path1 and path2 or NULL.
 * @param p to use while determining equivalence.
 * @param xctx of caller
 * @return boolean result of equivalent test.
 */
AFW_DECLARE(afw_boolean_t)
afw_object_path_are_equivalent(
    const afw_utf8_t *path1,
    const afw_utf8_t *path2,
    const afw_utf8_t *current_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_OBJECT_PATH_H */

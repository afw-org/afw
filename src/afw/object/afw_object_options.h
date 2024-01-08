// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Processing Options
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OBJECT_OPTIONS_H__
#define __AFW_OBJECT_OPTIONS_H__

#include "afw_interface.h"

/**
 * @defgroup afw_object_options Object processing options
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework object processing options.
 *
 * @{
 */

/**
 * @file afw_object_options.h
 * @brief Header file for Adaptive Framework object processing options
 */

AFW_BEGIN_DECLARES

/**
 * @brief Struct for object processing options.
 *
 * If afw_object_options_t is NULL, the option is to return all properties
 * that are in the object's meta.
 */
struct afw_object_options_s {

    /*
     * @brief Object processing options mask.
     * 
     * See AFW_OBJECT_OPTION_* for values.
     */
    afw_uint32_t mask;
};


/**
 * @brief Indicates that object should checked for missing required properties.
 *
 * This option implies the normalize option.
 */
#define AFW_OBJECT_OPTION_checkRequired          (1 << 0)



/**
 * @brief Include all inherited properties.
 *
 * This indicates that both direct and embedded inheritance should be performed.
 */
#define AFW_OBJECT_OPTION_composite              (1 << 1)



/**
 * @brief Indicates that object should include default property values.
 *
 * This indicates that default property values should be included.  This
 * option implies the normalize option.
 */
#define AFW_OBJECT_OPTION_includeDefaultValues   (1 << 3)



/**
 * @brief Include objects with descendent object types on retrieve.
 *
 * This only applies to retrieve objects requests and indicates that objects
 * with the supplied object type along with objects with any of its
 * descendent object types should be included in the search.
 */
#define AFW_OBJECT_OPTION_includeDescendentObjectTypes (1 << 4)



/**
 * @brief Include inheritedFrom property for all inherited properties.
 *
 * This option is ignored unless composite option is also set.
 *
 * The inheritedFrom property in meta `properties.<propertyName>` where
 * `<propertyName>` is the associated property name will be set.
 */
#define AFW_OBJECT_OPTION_inheritedFrom          (1 << 5)



/**
 * @brief Cause all data type integer values to be converted to string.
 *
 * This is a mask for afw_object_options_t common option.
 *
 * This overrides the large_integers_as_string option.  If neither
 * the integers_as_string or large_integer_as_string is specified,
 * integers will remain integers.  If this option is specified, all
 * data type integer values will be converted to the string data type.
 *
 * This options is useful if a content type is used that represents
 * integers as double (floating point) values.
 */
#define AFW_OBJECT_OPTION_integersAsString       (1 << 6)



/**
 * @brief Cause all large data type integer values to be converted to string.
 *
 * This is a mask for afw_object_options_t common option.
 * 
 * This option is overridden by the integers_as_string option.
 *
 * If this option is specified, all integers that have a magnitude that
 * exceeds plus or minus 9007199254740991 (2^53-1) will be converted
 * to the string data type.
 *
 * This options is useful if a content type is used that represents
 * integers as double (floating point) values, but integers are normally
 * small enough to be represented properly.
 */
#define AFW_OBJECT_OPTION_largeIntegersAsString  (1 << 7)



/**
 * @brief Include full meta.
 *
 * This indicates that all meta, including properties that are not part of
 * delta from object type, are to be included.  This overrides
 * AFW_OBJECT_OPTION_metaLimited.
 */
#define AFW_OBJECT_OPTION_metaFull              (1 << 8)



/**
 * @brief Include only requested meta.
 *
 * This indicates that only the requested meta is returned plus essential
 * meta (parentPaths when composite is not specified).
 */
#define AFW_OBJECT_OPTION_metaLimited            (1 << 9)



/**
 * @brief Indicates that object should be normalized based on object type.
 *
 * This options is assumed by several options as indicated in their
 * description.
 *
 * Indicates that each object's object type should be processed, assigning
 * each embedded object's object type, converting any properties to their
 * correct data type, plus computing values when needed.
 * 
 * Use the includeDefaultValues option to include default values.
 */
#define AFW_OBJECT_OPTION_normalize              (1 << 10)



/**
 * @brief Include meta objectId for entity objects.
 *
 * Indicates that meta objectId should be included for the entity object.
 */
#define AFW_OBJECT_OPTION_objectId               (1 << 11)



/**
 * @brief Include meta objectType (object id of adaptive object type).
 *
 * Indicates that meta objectType should be included for entity objects.
 */
#define AFW_OBJECT_OPTION_objectType             (1 << 12)



/**
 * @brief Include meta objectTypes
 *
 * Indicates that the objectTypes property should be added to the meta of the
 * entity containing a property of each object type used by the entity and all
 * of its embedded objects.
 */
#define AFW_OBJECT_OPTION_objectTypes            (1 << 13)



/**
 * @brief Include meta path for entity objects.
 *
 * Indicates that meta path should be included for entity objects.
 */
#define AFW_OBJECT_OPTION_path                   (1 << 14)



/**
 * @brief Include meta path for embedded objects.
 *
 * Indicates that meta path should be included for embedded objects.
 */
#define AFW_OBJECT_OPTION_pathEmbedded           (1 << 15)


/**
 * @brief Include meta reconcilable to entity's meta.
 *
 * Indicates that meta reconcilable should be included in entity's meta.
 * If this is specified on a get/retrieve object request and the retrieved
 * object is changed, calling update_object() with the changed object will
 * use the information in meta reconcilable to produce a modify request
 * for only the changes.
 */
#define AFW_OBJECT_OPTION_reconcilable           (1 << 16)



/**
 * @brief Include meta resolvedParentPaths for all objects.
 *
 * Indicates that meta resolvedParentPaths should be included with a bag of
 * parentPaths that have already been resolved by the composite option.
 */
#define AFW_OBJECT_OPTION_resolvedParentPaths    (1 << 17)



/**
 * @brief Indicates that each value should include type information.
 *
 * The content type determines how this is represented.
 */
#define AFW_OBJECT_OPTION_typedValues            (1 << 18)



/**
 * @brief Indicates content type should include whitespace characters.
 *
 * This option can be optionally supported by a content type implementation.
 * In the case of JSON, this indicates that newline, space, and tab characters
 * should be included to make the output more readable.
 */
#define AFW_OBJECT_OPTION_whitespace             (1 << 19)



/**
 * @brief Indicates content type should use non-standard tokens.
 *
 * This option can be optionally supported by a content type implementation.
 * In the case of JSON, this indicates that non-standard tokens will be used
 * for undefined, NaN, and Infinity values.  This means that there will be a
 * 'undefined' token for undefined values instead of the standard JSON null
 * token and that NaN and Infinity will also be special tokens instead of
 * strings.
 */
#define AFW_OBJECT_OPTION_useNonStandardTokens (1 << 20)



/**
 * @brief Object processing options - no options set.
 */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_none;

/**
 * @brief Object processing options - objectType.
 */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_objectType;


/** @brief Object processing options - composite only. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_composite;

/*
 * @brief Object processing options -
 *    checkRequired + composite + includeDefaultValues + normalize.
 */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_composite_normalize_defaults_required;

/** @brief Object processing options - composite only and defaults. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_composite_and_defaults;


/** @brief Object processing options - metaLimited. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_essential;


/** @brief Object processing options - metaLimited + whitespace. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_essential_with_whitespace;


/** @brief Object processing options to produce reconcilable meta property. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_reconcilable_meta_property;

/** @brief Object processing options - composite + inheritance related. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_composite_inheritedFrom_resolvedParentPaths;


/** @brief Whitespace only. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_whitespace;

/** @brief Use non-standard tokens. */
AFW_DECLARE_CONST_DATA(afw_object_options_t)
afw_object_options_useNonStandardTokens;


/** @brief Test mask. */
#define AFW_OBJECT_OPTION_IS(_options, _option) \
((_options) && (((((_options)->mask) & (AFW_OBJECT_OPTION_ ## _option))) != 0))


/** @brief Set option on. */
#define AFW_OBJECT_OPTION_SET_ON(_options, _option) \
    (_options)->mask |= AFW_OBJECT_OPTION_ ## _option


/** @brief Set option off. */
#define AFW_OBJECT_OPTION_SET_OFF(_options, _option) \
    (_options)->mask &= ~(AFW_OBJECT_OPTION_ ## _option)


/** @brief Set option. */
#define AFW_OBJECT_OPTION_SET(_options, _option, _bool_value) \
do { \
    if (_bool_value) { \
        AFW_OBJECT_OPTION_SET_ON(_options, _option); \
    } \
    else { \
        AFW_OBJECT_OPTION_SET_OFF(_options, _option); \
    } \
} while (0)


/**
 * @brief Set object processing options from options object.
 * @param initial_options or NULL.
 * @param options_object that contains options.
 * @param p to use for result.
 * @param xctx of caller.
 * @return options after applying options_obj property to initial_options.
 */
AFW_DECLARE(const afw_object_options_t *)
afw_object_options_set_from_object(
    const afw_object_options_t *initial_options,
    const afw_object_t *options_object,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Create a options.
 * @param initial_options or NULL.
 * @param options.
 * @param p to use for result.
 * @param xctx of caller.
 * @return Normalized copy of options.
 *
 * A copy is made of options and initial_options are merged, if specified.
 * The copy will have implied options set such as "normalize" if option
 * "includeDefaultValues" is set.
 */
AFW_DECLARE(const afw_object_options_t *)
afw_object_options_create(
    const afw_object_options_t *initial_options,
    const afw_object_options_t *options,
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_OBJECT_OPTIONS_H__ */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LDAP Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LDAP_METADATA_H__
#define __AFW_LDAP_METADATA_H__


/**
 * @file afw_ldap_metadata.h
 * @brief Adaptive Framework LDAP Metadata Header
 *
 * Internal.
 */


#include "afw_ldap.h"
#include "afw_ldap_internal.h"


#ifdef __cplusplus
extern "C" {
#endif



struct afw_ldap_metadata_syntax_handler_s {
    void *data;
    afw_ldap_ber_to_value_p_cb_t to_value;
    afw_ldap_value_to_ber_p_cb_t to_ber;
};


struct afw_ldap_object_type_attribute_s {

    /* attribute_type or NULL. */
    afw_ldap_metadata_attribute_type_t *attribute_type;

    /* Associated property type object. */
    const afw_object_t *property_type_object;

    /* Pointer to next attribute for object type or NULL if end. */
    afw_ldap_object_type_attribute_t *next;

    /* Indicates attribute is required. */
    afw_boolean_t is_required;

};


/** @fixme metadata_adaptor needs usecount are something for release. */
struct afw_ldap_metadata_s {

    /* Use count.  FIXME Start using. */
    unsigned int use_count;

    /* Subpool for metadata. */
    const afw_pool_t *p;

    /* Value of subschemaSubentry from rootDSE. */
    const afw_utf8_t * subschema_subentry;

    /* RootDSE. */
    const afw_object_t *rootdse_object;

    /* Schema, */
    const afw_object_t *schema_object;

    /* An object for each attribute type. */
    apr_hash_t *attribute_type_objects;

    /* An object for each syntax. */
    apr_hash_t *ldap_syntax_objects;

    /* An object for each matching rule. */
    apr_hash_t *matching_rule_objects;

    /* An object for each matching rule use. */
    apr_hash_t *matching_rule_use_objects;

    /* An object for each object class. */
    apr_hash_t *object_class_objects;

    /* An object for each value meta object. */
    apr_hash_t *value_meta_objects;

    /* An object for each object type. */
    apr_hash_t *object_type_objects;

    /* First afw_ldap_object_type_attribute_t * for each object type. */
    apr_hash_t *object_type_attributes;

    /* An afw_ldap_metadata_attribute_type_t for each attributeType. */
    apr_hash_t *attribute_types;

};


/* @brief Runtime attribute type info. */
struct afw_ldap_metadata_attribute_type_s {

    /* oid without {} part. */
    const afw_utf8_t *syntax_oid;

    /* Associated attribute type object. */
    const afw_object_t *attribute_type_object;

    /* Attribute type/property name. */
    const afw_utf8_t *property_name;

    /* Associated property type object. */
    const afw_object_t *property_type_object;

    /* Syntax Handler. */
    const afw_ldap_metadata_syntax_handler_t *syntax_handler;

    /* Data type id value. */
    const afw_value_t *data_type_id_value;

    /* Lower Bound. */
    afw_integer_t lower_bound;

    /* Upper Bound. */
    afw_integer_t upper_bound;

    /* Lower Bound present. */
    afw_boolean_t lower_bound_present;

    /* Upper Bound present. */
    afw_boolean_t upper_bound_present;

    /* Is single value. */
    afw_boolean_t is_single;

    /* Allow Read */
    afw_boolean_t never_allow_read;

    /* Allow Write */
    afw_boolean_t never_allow_write;

    /* Operational */
    afw_boolean_t operational;
};


/**
 * @brief Load LDAP adaptors Metadata .
 * @param adaptor  Internal ldap adaptor.
 * @param xctx of caller.
*
 * This function loads ldap metadata and sets the metadata variable of the
 * afw_ldap_internal_adaptor_t struct passed.  This should only be called
 * once per adaptor create.  Restart the adaptor to reload meta data.
*
 * The information in the metadata variable is rootdse and schema
 * related.
*
 * This function:
*
 *  1) Gets a fresh copy of the rootdse and schema.
 *  2) Parses these into memory objects.
 *  3) Creates a memory adaptor.
 *  4) Adds ObjectType, DataType and other type of objects to this memory
 *     adaptor using information this metadata and the LDAP adaptor
 *     configuration.
*
 * A very simple LDAP schema entry parser is used based on information from
 * RFC 4512.  This parser assumes that entries are valid.
*
 * The parser parses ldapSyntaxes, objectClasses, attributeTypes,
 * matchingRuleUse, matchingRules and similar values.
*
 * These values must always start with an '(' and end with a ')'.
*
 * The property name for the first value is passed to the parse function.
 * This name should normally be 'numericoid' or 'ruleid'.
*
 * The name of the other properties are the tokens themselves.
*
 * The tokens OBSOLETE, ABSTRACT, STRUCTURAL, AUXILIARY, COLLECTIVE,
 * and NO-USER-MODIFICATION are considered booleans with a value of TRUE.
*
 * The other property name tokens are followed by a single string value or '('
 * followed by a list of '$' separated string values ending with a ')'.  This
 * includes X-' extensions.  String that start with a single quote are converted
 * to unquoted string values.  All other values are kept asis.
 */
void
afw_ldap_metadata_load(
    afw_ldap_internal_adaptor_t *adaptor,
    afw_xctx_t *xctx);

/**
 * @brief Called by afw_ldap_adaptor_session() to retrieve metadata objects.
 */
void
afw_ldap_metadata_retrieve_objects(
    afw_ldap_internal_adaptor_session_t * self,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx);

/**
 * @brief Called by afw_ldap_adaptor_session() to get a metadata object.
 */
void
afw_ldap_metadata_get_object(
    afw_ldap_internal_adaptor_session_t * self,
    const afw_utf8_t * object_type,
    const afw_utf8_t * object_id,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx);

/**
 * @brief Return afw_ldap_object_type_attribute_t * for name.
 * @param first_attribute entry.
 * @param name to find.
 * @return entry found or NULL;
 */
afw_ldap_object_type_attribute_t *
afw_ldap_metadata_get_object_type_attribute(
    afw_ldap_object_type_attribute_t *first_attribute,
    const afw_utf8_t *name);


/* Tests whether an object type is handled by afw_ldap_metadata.c. */
afw_boolean_t
afw_ldap_metadata_handles(
    const afw_utf8_t *object_type);

const afw_value_t *
afw_ldap_metadata_bv_to_value(
    afw_ldap_internal_adaptor_session_t *session,
    afw_ldap_object_type_attribute_t *attribute,
    const afw_utf8_t *attribute_name, struct berval * *bv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

struct berval **
afw_ldap_metadata_value_to_bv(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_utf8_t *attribute_name, const afw_value_t *value,
    afw_xctx_t *xctx);

#ifdef __cplusplus
}
#endif

#endif /* __AFW_LDAP_METADATA_H__ */

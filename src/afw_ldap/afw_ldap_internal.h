// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LDAP Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_LDAP_INTERNAL_H__
#define __AFW_LDAP_INTERNAL_H__


/**
 * @file afw_ldap_internal.h
 * @brief Adaptive Framework LDAP Internal Header  
 */


#include "afw_minimal.h"
#include "generated/afw_ldap_generated.h"
#include <apr_ldap.h>
#if APR_HAS_MICROSOFT_LDAPSDK == 1
#include <winber.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* @brief afw_ldap_metadata_attribute_type_t opaque typedef. */
typedef struct afw_ldap_metadata_attribute_type_s
afw_ldap_metadata_attribute_type_t;

/* @brief afw_ldap_metadata_syntax_handler_t opaque typedef. */
typedef struct afw_ldap_metadata_syntax_handler_s
afw_ldap_metadata_syntax_handler_t;

/* @brief afw_ldap_metadata_t opaque typedef. */
typedef struct afw_ldap_metadata_s
afw_ldap_metadata_t;

/* @brief afw_ldap_object_type_attribute_t opaque typedef. */
typedef struct afw_ldap_object_type_attribute_s
afw_ldap_object_type_attribute_t;

/* @brief typedef for ber to value callback. */
typedef const afw_value_t *
(*afw_ldap_ber_to_value_p_cb_t)(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data,
    struct berval * *bv,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/* @brief typedef for value to ber callback. */
typedef struct berval **
(*afw_ldap_value_to_ber_p_cb_t)(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/* Note: A register function might be added to register these callbacks */

AFW_LDAP_DECLARE_INTERNAL_CONST_DATA(afw_adaptor_factory_t) afw_ldap_adaptor_factory;

#ifdef WIN32
#define AFW_LDAP_TIMEVAL LDAP_TIMEVAL
#else
#define AFW_LDAP_TIMEVAL struct timeval
#endif


typedef struct afw_ldap_internal_adaptor_s {
    afw_adaptor_t pub;
    const afw_utf8_z_t *url_z;
    const afw_value_t *bind_parameters;
    apr_ldap_url_desc_t *lud;
    afw_ldap_metadata_t *metadata;
    AFW_LDAP_TIMEVAL timeout;
    afw_boolean_t prevent_verify_cert;
} afw_ldap_internal_adaptor_t;

typedef struct afw_ldap_internal_adaptor_session_s {
    afw_adaptor_session_t pub;
    afw_ldap_internal_adaptor_t *adaptor;
    afw_adaptor_session_t *metadata_session;
    const afw_utf8_z_t * bind_dn_z;
    const afw_utf8_z_t * bind_password_z;
    afw_adaptor_object_type_cache_t object_type_cache;
    afw_boolean_t release_called_from_cleanup;
    LDAP *ld;
} afw_ldap_internal_adaptor_session_t;

extern char * afw_ldap_internal_allattrs[];


/**
 * @brief Internal create an LDAP adaptor.
 * @param properties Parameters.
 * @param p to use for adaptor resources.
 * @param xctx of caller.
 * @return New instance of LDAP adaptor.
 */
const afw_adaptor_t *
afw_ldap_internal_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Internal create an LDAP adaptor session.
 * @param adaptor Internal instance of adaptor.
 * @param xctx of caller.
 * @return New instance of LDAP adaptor session.
 */
afw_ldap_internal_adaptor_session_t *
afw_ldap_internal_adaptor_session_create(
    afw_ldap_internal_adaptor_t * adaptor,
    afw_xctx_t *xctx);

void
afw_ldap_internal_cleanup_ldap_msgfree(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);

void
afw_ldap_internal_cleanup_ldap_value_free_len(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);

void
afw_ldap_internal_cleanup_ldap_memfree(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);

LDAPMessage *
afw_ldap_internal_search_s(
    afw_ldap_internal_adaptor_session_t *session, const afw_utf8_z_t *dn,
    int ldap_scope, const afw_utf8_z_t *filter, afw_xctx_t *xctx);

const afw_utf8_t *
afw_ldap_internal_get_object_id(
    afw_ldap_internal_adaptor_session_t *self,
    LDAPMessage *e, afw_boolean_t check_base, afw_xctx_t *xctx);

void
afw_ldap_internal_session_begin(
    afw_ldap_internal_adaptor_session_t *self,
    afw_xctx_t *xctx);

void
afw_ldap_internal_session_end(afw_ldap_internal_adaptor_session_t *self);

/* Internal create an adaptive object from an LDAPMessage. */
const afw_object_t *
afw_ldap_internal_create_object_from_entry(
    afw_ldap_internal_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    LDAPMessage *e,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

const afw_utf8_t *
afw_ldap_internal_expression_from_query_criteria(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_query_criteria_filter_entry_t * entry,
    afw_xctx_t *xctx);

/* Implemented in afw_ldap_syntax_handler.c. */
void
afw_ldap_internal_syntax_handler_assign(
    afw_ldap_metadata_t *metadata,
    afw_ldap_metadata_attribute_type_t *attribute_type,
    afw_xctx_t *xctx);


const afw_value_t *
afw_ldap_internal_syntax_handler_unknown_to_value(
    struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

#ifdef __cplusplus
}
#endif

#endif /* __AFW_LDAP_INTERNAL_H__ */

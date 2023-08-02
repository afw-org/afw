// See the 'COPYING' file in the project root for licensing information.
/*
 * Internal LDAP Adaptive Framework Adaptor functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_ldap_internal.c
 * @brief  Internal LDAP Adaptive Framework Adaptor functions
 */


#include "afw.h"
#include "afw_ldap_internal.h"
#include "afw_ldap_metadata.h"

char * afw_ldap_internal_allattrs[] = { "+", "*", NULL };

static const int impl_zero = 0;


LDAPMessage *
afw_ldap_internal_search_s(
    afw_ldap_internal_adaptor_session_t *session, const afw_utf8_z_t *dn,
    int ldap_scope, const afw_utf8_z_t *filter, afw_xctx_t *xctx)
{
    int rv;
    LDAPMessage *result;

    rv = ldap_search_s(session->ld, (char *)dn, ldap_scope, (char *)filter,
        afw_ldap_internal_allattrs, 0, &result);
    if (result) {
        afw_pool_register_cleanup_before(session->pub.p, result, NULL,
            afw_ldap_internal_cleanup_ldap_msgfree, xctx);
    }
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, "ldap_search_s() failed.",
            xctx);
    }

    return result;
}


void
afw_ldap_internal_cleanup_ldap_msgfree(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    ldap_msgfree((LDAPMessage *)data);
}

void
afw_ldap_internal_cleanup_ldap_value_free_len(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    ldap_value_free_len((struct berval **)data);
}

void
afw_ldap_internal_cleanup_ldap_memfree(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    ldap_memfree(data);
}


const afw_utf8_t *
afw_ldap_internal_get_object_id(
    afw_ldap_internal_adaptor_session_t *self,
    LDAPMessage *e, afw_boolean_t check_base, afw_xctx_t *xctx)
{
    afw_utf8_z_t *dn_z;
    int errno;

    dn_z = ldap_get_dn(self->ld, e);
    if (dn_z) {
        afw_pool_register_cleanup_before(self->pub.p, dn_z, NULL,
            afw_ldap_internal_cleanup_ldap_memfree,
            xctx);
    }
    else {
        ldap_get_option(self->ld, LDAP_OPT_ERROR_NUMBER, &errno);
        AFW_THROW_ERROR_RV_Z(general, ldap, errno, "ldap_get_dn() error.",
            xctx);
    }

    return afw_utf8_create(dn_z, AFW_UTF8_Z_LEN, xctx->p, xctx);
}


/* Used on windows to support preventVerifyCert. */
#if defined(_WIN32) || defined(WIN32)
BOOLEAN impl_prevent_verify_server_cert(
    PLDAP Connection,
    PCCERT_CONTEXT *pServerCert
)
{
    return TRUE;
}
#endif

void
afw_ldap_internal_session_begin(
    afw_ldap_internal_adaptor_session_t *self,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_t *adaptor =
        (afw_ldap_internal_adaptor_t *)self->pub.adaptor;
    const afw_pool_t *p = self->pub.p;
    const afw_value_t *bind_parameters_value;
    const afw_object_t *bind_parameters;
    int rv = 0;
    apr_ldap_err_t *err = NULL;
    int version = LDAP_VERSION3;
    int secure;

    /* check to see if we're already initialized
    if (obj->ldap_initialized) {
    return rc;
    }*/

    /* Create an LDAP handle */
    if (self->adaptor->lud->lud_scheme && 
        strcmp(self->adaptor->lud->lud_scheme, "ldaps") == 0) 
    {
        secure = APR_LDAP_SSL;
    } else
        secure = APR_LDAP_NONE;

    rv = apr_ldap_init(afw_pool_get_apr_pool(p), &(self->ld),
        self->adaptor->lud->lud_host, self->adaptor->lud->lud_port,
        secure, &err);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv,
            (err && err->msg)?err->msg:"Unknown error.", xctx);
    }

    /* Set version to 3 as required by OpenLDAP and supported by others. */
    rv = apr_ldap_set_option(afw_pool_get_apr_pool(p), self->ld,
        LDAP_OPT_PROTOCOL_VERSION,
        (void*)&version,
        &err);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, err->msg, xctx);
    }

    /* Set prevent verify cert if requested.*/
    if (adaptor->prevent_verify_cert) {
        /*
         * apr_ldap_set_option() doesn't support APR_LDAP_OPT_VERIFY_CERT for
         * windows.
         */
#if defined(_WIN32) || defined(WIN32)
        rv = ldap_set_option(self->ld, LDAP_OPT_SERVER_CERTIFICATE,
            impl_prevent_verify_server_cert);
#else
        rv = apr_ldap_set_option(afw_pool_get_apr_pool(p), self->ld,
            APR_LDAP_OPT_VERIFY_CERT,
            &impl_zero,
            &err);
#endif
    }

    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, err->msg, xctx);
    }

    /* Set to use referral.
    rv = apr_ldap_set_option(xctx->p, self->ld,
        APR_LDAP_OPT_REFERRALS,
        (void*)&one,
        &err);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, err->msg, xctx);
    }
     */

    /* Get bind dn and password */
    bind_parameters_value = afw_value_evaluate(
        self->adaptor->bind_parameters, p, xctx);
    if (!afw_value_is_object(bind_parameters_value)) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Invalid bindParameters for adaptorId " AFW_UTF8_FMT_Q,
            &adaptor->pub.adaptor_id);
    }
    bind_parameters = ((const afw_value_object_t *)bind_parameters_value)
        ->internal;
    self->bind_dn_z = afw_object_old_get_property_as_utf8_z(bind_parameters,
        &afw_ldap_s_dn, p, xctx);
    self->bind_password_z = afw_object_old_get_property_as_utf8_z(bind_parameters,
        &afw_ldap_s_password, p, xctx);
    if (!self->bind_dn_z || !self->bind_password_z) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Invalid bindParameters for adaptorId " AFW_UTF8_FMT_Q,
            &adaptor->pub.adaptor_id);
    }


    /*
     * Bind
     *
     * Cast const away on bind_dn and bind password since ldap doesn't specify
     * const on them but also doesn't modify them.
     */
    rv = ldap_simple_bind_s(self->ld,
        (char *)self->bind_dn_z,
        (char *)self->bind_password_z);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv,
            "Error binding with user and password.", xctx);
    }

}

void
afw_ldap_internal_session_end(afw_ldap_internal_adaptor_session_t *self)
{
    /* Free stuff .... */

    /* unbind */
    if (self->ld) {
        ldap_unbind(self->ld);
    }
}


/* Internal create an adaptive object from an LDAPMessage. */
const afw_object_t *
afw_ldap_internal_create_object_from_entry(
    afw_ldap_internal_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    LDAPMessage *e,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *o;
    char *a;
    const afw_utf8_t *property_name;
    BerElement *be;
    struct berval **bv;
    const afw_value_t *value;
    afw_ldap_object_type_attribute_t *first_attribute;
    afw_ldap_object_type_attribute_t *attribute;

    if (!object_id || !object_id->s) {
        object_id = afw_ldap_internal_get_object_id(self, e, FALSE, xctx);
    }

    /*
     * Get first attribute for this object type.  There might not be one
     * early on.
     */
    first_attribute = NULL;
    if (self->adaptor->metadata &&
        self->adaptor->metadata->object_type_attributes)
    {
        first_attribute = apr_hash_get
            (self->adaptor->metadata->object_type_attributes,
                object_type_id->s, object_type_id->len);
    }

    o = afw_object_create_managed(p, xctx);
    bv = NULL;
    a = NULL;
    be = NULL;
    AFW_TRY {
        for (a = ldap_first_attribute(self->ld, e, &be); a;
            a = ldap_next_attribute(self->ld, e, be))
        {
            bv = ldap_get_values_len(self->ld, e, a);
            property_name = afw_utf8_create_copy(a, AFW_UTF8_Z_LEN,
                o->p, xctx);

            /* Get attribute for this property name if there is one. */
            attribute = afw_ldap_metadata_get_object_type_attribute(
                first_attribute, property_name);

            /* Change bv to value. */
            value = afw_ldap_metadata_bv_to_value(self, attribute,
                property_name, bv, o->p, xctx);

            /* If there is a value, set its property. */
            if (value) {
                afw_object_set_property(o, property_name, value, xctx);
            }

            /* Free bv and a. */
            ldap_value_free_len(bv);
            bv = NULL;
            ldap_memfree(a);
            a = NULL;
        }
    }
    AFW_FINALLY{

        /* Free bv if needed. */
        if (bv) {
            ldap_value_free_len(bv);
        }

        /* Free a if needed. */
        if (a) {
            ldap_memfree(a);
        }

        /* Free be if needed. */
        if (be) {
            ber_free(be, 0);
        }
    }
    AFW_ENDTRY;

    afw_object_meta_set_ids(o, &self->adaptor->pub.adaptor_id,
        object_type_id, object_id, xctx);

    return o;
}

/* useful macro for determining if node, x, is a non-leaf node */
#define AFW_QUERY_CRITERIA_CONTINUE(x) \
    (x != AFW_QUERY_CRITERIA_FALSE && x != AFW_QUERY_CRITERIA_TRUE)

/*
 * afw_ldap_internal_expression_from_filter_entry()
 *
 * This routine takes an individual query criteria filter entry
 * and generates the appropriate LDAP search filter.
 *
 */
const afw_utf8_t *
afw_ldap_internal_expression_from_filter_entry(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_query_criteria_filter_entry_t * entry,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *filter_expression = NULL;
    const afw_utf8_t *property_name, *property_value;
    struct berval **bv;
    

    if (entry == AFW_QUERY_CRITERIA_FALSE ||
        entry == AFW_QUERY_CRITERIA_TRUE) 
    {
        return NULL;
    }

    property_name = entry->property_name;

    /* convert the filter value to a string for LDAP comparison to use */
    bv = afw_ldap_metadata_value_to_bv(session, property_name, entry->value, xctx);
    if (bv == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Query criteria could not be converted into an LDAP filter string.", xctx);
    }

    /* turn it into an afw string for easy use by the format routines */
    property_value = afw_utf8_create((*bv)->bv_val, (*bv)->bv_len, xctx->p, xctx);

    switch (entry->op_id) {
        case afw_query_criteria_filter_op_id_eq:
            filter_expression = afw_utf8_printf(xctx->p, xctx,
                AFW_UTF8_FMT "=" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(property_name), 
                AFW_UTF8_FMT_ARG(property_value));
            break;
        case afw_query_criteria_filter_op_id_ne:
            filter_expression = afw_utf8_printf(xctx->p, xctx,
                "!(" AFW_UTF8_FMT "=" AFW_UTF8_FMT ")",
                AFW_UTF8_FMT_ARG(property_name), 
                AFW_UTF8_FMT_ARG(property_value));
            break;
        case afw_query_criteria_filter_op_id_lt:
            filter_expression = afw_utf8_printf(xctx->p, xctx,
                AFW_UTF8_FMT "<" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(property_name), 
                AFW_UTF8_FMT_ARG(property_value));
            break;
        case afw_query_criteria_filter_op_id_le:
            filter_expression = afw_utf8_printf(xctx->p, xctx,
                AFW_UTF8_FMT "<=" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(property_name), 
                AFW_UTF8_FMT_ARG(property_value));
            break;
        case afw_query_criteria_filter_op_id_gt:
            filter_expression = afw_utf8_printf(xctx->p, xctx,
                AFW_UTF8_FMT ">" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(property_name), 
                AFW_UTF8_FMT_ARG(property_value));
            break;
        case afw_query_criteria_filter_op_id_ge:
            filter_expression = afw_utf8_printf(xctx->p, xctx,
                AFW_UTF8_FMT ">=" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(property_name), 
                AFW_UTF8_FMT_ARG(property_value));
            break;
        default:
            AFW_THROW_ERROR_Z(general,
                "query operator not supported by LDAP", xctx);
            break;
    }

    return filter_expression;
}

/*
 *
 */
const afw_utf8_t *
afw_ldap_internal_expression_from_query_criteria(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_query_criteria_filter_entry_t * entry,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *filter_expression = NULL;
    const afw_utf8_t *on_true;
    const afw_utf8_t *on_false;

    filter_expression = afw_ldap_internal_expression_from_filter_entry(
        session, entry, xctx);

    /* A bottom leaf of our decision tree, so return our expression */
    if (entry->on_true == AFW_QUERY_CRITERIA_TRUE &&
        entry->on_false == AFW_QUERY_CRITERIA_FALSE)
    {
        return filter_expression;
    }

    /* (entry AND on_true) OR (on_false) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_true) &&
            AFW_QUERY_CRITERIA_CONTINUE(entry->on_false))
    {
        on_true = afw_ldap_internal_expression_from_query_criteria(
            session, entry->on_true, xctx);
        on_false = afw_ldap_internal_expression_from_query_criteria(
            session, entry->on_false, xctx);

        /* (|(&(filter_expression)(on_true))(on_false)) */
        filter_expression = afw_utf8_printf(xctx->p, xctx,
            "|(&(" AFW_UTF8_FMT ")(" AFW_UTF8_FMT "))(" AFW_UTF8_FMT ")",
            AFW_UTF8_FMT_ARG(filter_expression), 
            AFW_UTF8_FMT_ARG(on_true), AFW_UTF8_FMT_ARG(on_false));
    }

    /* (entry AND on_true) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_true)) 
    {
        on_true = afw_ldap_internal_expression_from_query_criteria(
            session, entry->on_true, xctx);

        /* (&(filter_expression)(on_true)) */
        filter_expression = afw_utf8_printf(xctx->p, xctx, "&(" AFW_UTF8_FMT ")(" AFW_UTF8_FMT ")",
            filter_expression->len, filter_expression->s,
            on_true->len, on_true->s);
    }

    /* (entry OR on_false) */
    else if (AFW_QUERY_CRITERIA_CONTINUE(entry->on_false))
    {
        on_false = afw_ldap_internal_expression_from_query_criteria(
            session, entry->on_false, xctx);

        /* (|(filter_expression)(on_false)) */
        filter_expression = afw_utf8_printf(xctx->p, xctx,
            "|(" AFW_UTF8_FMT ")(" AFW_UTF8_FMT ")",
            AFW_UTF8_FMT_ARG(filter_expression),
            AFW_UTF8_FMT_ARG(on_false));
    }

    return filter_expression;

}

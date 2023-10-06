// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adaptor interface for LDAP
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_ldap_adaptor.c
 * @brief Implementation of afw_adaptor interface for LDAP
 */

#include "afw.h"
#include "afw_ldap_internal.h"
#include "afw_adaptor_impl.h"
#include "afw_ldap_metadata.h"
#include "generated/afw_ldap_generated.h"

/* Declares and rti/inf defines for interface afw_adaptor */
#define AFW_IMPLEMENTATION_ID "ldap"
#include "afw_adaptor_impl_declares.h"


const afw_adaptor_t *
afw_ldap_internal_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_t *self;
    afw_adaptor_t *adaptor;
    apr_ldap_err_t *err;
    apr_status_t rv;
    afw_boolean_t found;


    /* Create adaptor, process common properties, check required.  */
    adaptor = afw_adaptor_impl_create_cede_p(
        &impl_afw_adaptor_inf,
        sizeof(afw_ldap_internal_adaptor_t),
        properties, p, xctx);
    self = (afw_ldap_internal_adaptor_t *)adaptor;

    /* Process url property. */
    self->url_z = afw_object_old_get_property_as_utf8_z(properties,
        &afw_ldap_self_s_url, p, xctx);
    rv = apr_ldap_url_parse(
        afw_pool_get_apr_pool(self->pub.p),
        self->url_z,
        &(self->lud), &err);
    if (rv != APR_SUCCESS) {
        afw_adaptor_impl_throw_property_invalid(adaptor,
            &afw_ldap_self_s_url, xctx);
    }

    /* Get compiled bindParameter. */
    self->bind_parameters = afw_object_old_get_property_as_compiled_template(
        properties, &afw_ldap_self_s_bindParameters,
        adaptor->impl->source_location, NULL, adaptor->p, xctx);

    /* Get preventVerifyCert. */
    self->prevent_verify_cert = afw_object_old_get_property_as_boolean(
        properties, &afw_ldap_self_s_preventVerifyCert, &found, xctx);

    /** @fixme add parameter. */
    self->timeout.tv_sec = 30;

    /* Load metadata. */
    afw_ldap_metadata_load(self, xctx);

    /* Return adaptor. */
    return (const afw_adaptor_t *)self;
}



/*
 * Implementation of method destroy of interface afw_adaptor.
 */
void
impl_afw_adaptor_destroy(
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    /* Release pool. */
    afw_pool_release(instance->p, xctx);
}



/*
 * Implementation of method create_adaptor_session of interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session (
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_t *self = (afw_ldap_internal_adaptor_t *)instance;

    return (const afw_adaptor_session_t *)
        afw_ldap_internal_adaptor_session_create(self, xctx);
}


/*
 * Implementation of method get_additional_metrics of interface afw_adaptor.
 */
const afw_object_t *
impl_afw_adaptor_get_additional_metrics (
    const afw_adaptor_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* There are no adaptor specific metrics. */
    return NULL;
}

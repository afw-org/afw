// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adapter interface for LDAP
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_ldap_adapter.c
 * @brief Implementation of afw_adapter interface for LDAP
 */

#include "afw.h"
#include "afw_ldap_internal.h"
#include "afw_adapter_impl.h"
#include "afw_ldap_metadata.h"
#include "generated/afw_ldap_generated_internal.h"

/* Declares and rti/inf defines for interface afw_adapter */
#define AFW_IMPLEMENTATION_ID "ldap"
#define AFW_ADAPTER_SELF_T afw_ldap_internal_adapter_t
#include "afw_adapter_impl_declares.h"


const afw_adapter_t *
afw_ldap_internal_adapter_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_ldap_internal_adapter_t *self;
    afw_adapter_t *adapter;
    apr_ldap_err_t *err;
    apr_status_t rv;
    afw_boolean_t found;


    /* Create adapter, process common properties, check required.  */
    adapter = afw_adapter_impl_create_cede_p(
        &impl_afw_adapter_inf,
        sizeof(afw_ldap_internal_adapter_t),
        properties, p, xctx);
    self = (afw_ldap_internal_adapter_t *)adapter;

    /* Process url property (template evaluated at adapter start, issue #15). */
    {
        const afw_value_t *url_value;

        url_value = afw_object_get_property_compile_and_evaluate_using(
            properties, afw_ldap_v_url, adapter->source_location,
            afw_compile_type_template, p, xctx);
        if (!afw_value_is_string(url_value)) {
            afw_adapter_impl_throw_property_invalid(adapter,
                afw_ldap_v_url, xctx);
        }
        self->url_z = afw_utf8_to_utf8_z(
            &((const afw_value_string_t *)url_value)->internal, p, xctx);
    }
    rv = apr_ldap_url_parse(
        afw_pool_get_apr_pool(self->pub.p),
        self->url_z,
        &(self->lud), &err);
    if (rv != APR_SUCCESS) {
        afw_adapter_impl_throw_property_invalid(adapter,
            afw_ldap_v_url, xctx);
    }

    /* Get compiled bindParameter. */
    self->bind_parameters = afw_object_get_property_compile_template(
        properties, afw_ldap_v_bindParameters,
        adapter->impl->source_location, NULL, adapter->p, xctx);

    /* Get preventVerifyCert. */
    self->prevent_verify_cert = afw_object_get_property_as_boolean_internal(
        properties, afw_ldap_v_preventVerifyCert, &found, xctx);

    /** @fixme add parameter. */
    self->timeout.tv_sec = 30;

    /* Load metadata. */
    afw_ldap_metadata_load(self, xctx);

    /* Return adapter. */
    return (const afw_adapter_t *)self;
}



/*
 * Implementation of method destroy of interface afw_adapter.
 */
void
impl_afw_adapter_destroy(
    AFW_ADAPTER_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Release pool. */
    afw_pool_release(self->pub.p, xctx);
}



/*
 * Implementation of method create_adapter_session of interface afw_adapter.
 */
const afw_adapter_session_t *
impl_afw_adapter_create_adapter_session (
    AFW_ADAPTER_SELF_T *self,
    afw_xctx_t *xctx)
{

    return (const afw_adapter_session_t *)
        afw_ldap_internal_adapter_session_create(self, xctx);
}


/*
 * Implementation of method get_additional_metrics of interface afw_adapter.
 */
const afw_object_t *
impl_afw_adapter_get_additional_metrics (
    AFW_ADAPTER_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* There are no adapter specific metrics. */
    return NULL;
}

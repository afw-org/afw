// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adaptor_session interface LDAP
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_ldap_adaptor_session.c
 * @brief Implementation of afw_adaptor_session interface for LDAP
 */

#include "afw.h"
#include "afw_ldap_internal.h"
#include "afw_ldap_metadata.h"
#include "afw_adaptor_impl.h"

/* Declares and rti/inf defines for interface afw_adaptor_session */
#define AFW_IMPLEMENTATION_ID "ldap"
#include "afw_adaptor_session_impl_declares.h"


afw_ldap_internal_adaptor_session_t *
afw_ldap_internal_adaptor_session_create(
    afw_ldap_internal_adaptor_t * adaptor,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *self;

    self = afw_xctx_calloc_type(afw_ldap_internal_adaptor_session_t, xctx);
    self->pub.inf = &impl_afw_adaptor_session_inf;
    self->pub.adaptor = (afw_adaptor_t *)adaptor;
    self->pub.p = xctx->p;
    self->adaptor = adaptor;

    /* Get active connection to LDAP. */
    afw_ldap_internal_session_begin(self, xctx);

    /* Return session. */
    return self;
}



/*
 * Implementation of method destroy of interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *self = (afw_ldap_internal_adaptor_session_t *)instance;

    /* Close ldap connection. */
    afw_ldap_internal_session_end(self);
}

/*
 * Implementation of method retrieve_objects for interface
 * afw_adaptor_session.
 */
void
impl_afw_adaptor_session_retrieve_objects(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t *criteria,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t * self =
        (afw_ldap_internal_adaptor_session_t *)instance;
    afw_ldap_internal_adaptor_t * adaptor =
        (afw_ldap_internal_adaptor_t *)instance->adaptor;
    LDAPMessage *res;
    LDAPMessage *e;
    const afw_utf8_t *filter;
    const afw_utf8_t *filter_class;
    const char *filter_z;
    const afw_object_t *o;
    const afw_utf8_t *object_id;
    const afw_utf8_t *s;
    char *base_z;
    int msgid;
    int rv;
    int err;
    afw_boolean_t abandon;

    /* If this is one of the synthetic object types, call metadata function. */
    if (afw_ldap_metadata_handles(object_type_id))
    {
        afw_ldap_metadata_retrieve_objects(
            self, object_type_id, criteria, context, callback, xctx);
        return;
    }

    /* Determine base_z using optional type specific parameter. */
    base_z = "";
    if (adaptor_type_specific) {
        s = afw_object_old_get_property_as_string(adaptor_type_specific,
            &afw_ldap_self_s_base, xctx);
        if (s) {
            base_z = (char *)afw_utf8_to_utf8_z(s, p, xctx);
        }
    }

    /* Determine filter_z. */
    if (object_type_id) {
        /* Check to see if the user wants inherited object types to be returned */
        if (AFW_OBJECT_OPTION_IS(impl_request->options, includeDescendentObjectTypes)) {
            filter_class = afw_utf8_printf(p, xctx,
                "objectclass=" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(object_type_id));
        } else {
            filter_class = afw_utf8_printf(p, xctx,
                "structuralobjectclass=" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(object_type_id));
        }
    } else {
        filter_class = afw_utf8_printf(p, xctx, "objectclass=*");
    }
    if (criteria && criteria->filter) {
        filter = afw_ldap_internal_expression_from_query_criteria(
            self, criteria->filter, xctx);
        filter = afw_utf8_printf(p, xctx,
            "(&(" AFW_UTF8_FMT ")(" AFW_UTF8_FMT "))",
            AFW_UTF8_FMT_ARG(filter_class), AFW_UTF8_FMT_ARG(filter));
    } else {
        filter = filter_class;
    }
    filter_z = afw_utf8_z_create(filter->s, filter->len, p, xctx);

    /* Start search. */
    msgid = ldap_search(self->ld, base_z, LDAP_SCOPE_SUBTREE,
        (char *)filter_z, afw_ldap_internal_allattrs, 0);
    if (msgid < 0) {
        ldap_get_option(self->ld, LDAP_OPT_ERROR_NUMBER, &err);
        AFW_THROW_ERROR_RV_Z(general, ldap, err,
            "ldap_search() failed.", xctx);
    }

    /* Process each result. */
    res = NULL;
    abandon = false;
    AFW_TRY {
        for (;;) {
            rv = ldap_result(self->ld, msgid, 0, &adaptor->timeout, &res);
            if (rv < 0) {
                ldap_get_option(self->ld, LDAP_OPT_ERROR_NUMBER, &err);
                AFW_THROW_ERROR_RV_Z(general, ldap, err,
                    "ldap_result() failed", xctx);
            }
            if (rv == 0) {
                AFW_THROW_ERROR_Z(general,
                    "ldap_result() timeout.", xctx);
            }
            if (rv == LDAP_RES_SEARCH_RESULT) {
                break;
            }
            if (rv != LDAP_RES_SEARCH_ENTRY) {
                AFW_THROW_ERROR_RV_Z(general, ldap, rv,
                    "ldap_result() unexpected return value", xctx);
            }

            /* Get each entry and callback with its adaptive object representation. */
            e = ldap_first_entry(self->ld, res);
            while (e) {
                object_id = afw_ldap_internal_get_object_id(self, e, true, xctx);
                o = afw_ldap_internal_create_object_from_entry(self, object_type_id,
                    object_id, e, xctx->p, xctx);
                /* Call callback and abandon request if requested. */
                abandon = callback(o, context, xctx);
                if (abandon) {
                    ldap_abandon(self->ld, msgid);
                    break;
                }
                e = ldap_next_entry(self->ld, e);
            }

            /* Free res. */
            ldap_msgfree(res);
            res = NULL;

            /* Break if asked to abandon. */
            if (abandon) {
                break;
            }
        }
    }
    AFW_FINALLY{
        /* Make sure last message is freed. */
        if (res) {
            ldap_msgfree(res);
            res = NULL;
        }
    }
    AFW_ENDTRY;

    /* Callback one last time with NULL object pointer. */
    callback(NULL, context, xctx);
}



/*
 * Implementation of method get_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_get_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t * self = (afw_ldap_internal_adaptor_session_t *)instance;
    int rv;
    LDAPMessage *res;
    LDAPMessage *e;
    const afw_utf8_z_t *dn_z;
    const afw_utf8_z_t *filter_z;
    const afw_object_t *obj;
    int ldap_scope;
    int count;
       
    /** @fixme Add timeout config parameter. */

    /* If this is one of the synthetic object types, call metadata function. */
    if (afw_ldap_metadata_handles(object_type_id))
    {
        afw_ldap_metadata_get_object(
            self, object_type_id, object_id, context, callback, xctx);
        return;
    }

    /* Make null terminated string from object_id. */
    dn_z = (char *)afw_utf8_to_utf8_z(object_id, p, xctx);

    /* Read entry. */
    res = NULL;
    ldap_scope = LDAP_SCOPE_BASE;
    filter_z = apr_psprintf(afw_pool_get_apr_pool(xctx->p),
        "(structuralobjectclass=" AFW_UTF8_FMT ")",
        AFW_UTF8_FMT_ARG(object_type_id));
    rv = ldap_search_s(self->ld, (char *)dn_z, ldap_scope, (char *)filter_z,
        afw_ldap_internal_allattrs, 0, &res);
    if (res) {
        afw_pool_register_cleanup_before(xctx->p, res, NULL,
            afw_ldap_internal_cleanup_ldap_msgfree, xctx);
    }
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, "ldap_search_s() failed.",
            xctx);
    }

    /* Make sure exactly one entry is returned. */
    count = ldap_count_entries(self->ld, res);
    if (count == 0) {
        AFW_THROW_ERROR_Z(not_found, "Not found", xctx);
    }
    if (count != 1) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Expecting ldap_search_s() to return exactly 1 entry but "
            "%d returned",
            count);
    }

    /* Get entry and return it as an adaptive object. */
    e = ldap_first_entry(self->ld, res);
    if (!e) {
        AFW_THROW_ERROR_Z(general,
            "ldap_first_entry() failed.", xctx);
    }

    /* Callback with object. */
    obj = afw_ldap_internal_create_object_from_entry(self, object_type_id,
        object_id, e, xctx->p, xctx);
    callback(obj, context, xctx);
}



/*
 * Implementation of method add_object for interface afw_adaptor_session.
 */
const afw_utf8_t *
impl_afw_adaptor_session_add_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *self =
        (afw_ldap_internal_adaptor_session_t *)instance;
    afw_ldap_metadata_t *metadata = self->adaptor->metadata;
    const afw_pool_t *p;
    afw_ldap_object_type_attribute_t *first_attribute;
    afw_ldap_object_type_attribute_t *attribute;
    apr_array_header_t *mods;
    const afw_value_t *value;
    const afw_utf8_t *property_name;
    const afw_iterator_t *iterator;
    struct berval **bvals;
    LDAPMod *mod;
    char *dn_z;
    int rv;

    /* This method is not allowed for synthetic object types. */
    if (afw_ldap_metadata_handles(object_type_id))
    {
        AFW_THROW_ERROR_Z(read_only, "Not allowed", xctx);
    }

    /* This adaptor required object_id to be specified */
    if (suggested_object_id == NULL) {
        AFW_THROW_ERROR_Z(bad_request,
            "An objectId is required for LDAP operations.",
            xctx);
    }

    /* Get first attribute for object type. */
    first_attribute = apr_hash_get(metadata->object_type_attributes,
        object_type_id->s, object_type_id->len);
    if (!first_attribute) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Object type " AFW_UTF8_FMT_Q " not found",
            AFW_UTF8_FMT_ARG(object_type_id));
    }

    /* Create mods array. */
    p = afw_pool_create(instance->p, xctx);
    mods = apr_array_make(afw_pool_get_apr_pool(p), 10, sizeof(LDAPMod *));

    /* Add objectClass. */
    mod = afw_pool_calloc_type(p, LDAPMod, xctx);
    mod->mod_op = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
    mod->mod_type = "objectClass";
    bvals = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
    *bvals = afw_pool_calloc_type(p, struct berval, xctx);
    (*bvals)->bv_len = (ber_len_t)object_type_id->len;
    (*bvals)->bv_val = (char *)object_type_id->s;
    mod->mod_vals.modv_bvals = bvals;
    APR_ARRAY_PUSH(mods, LDAPMod *) = mod;

    /* Loop adding properties. */
    iterator = NULL;
    while ((value = afw_object_get_next_property(object,
        &iterator, &property_name, xctx)))
    {
        attribute = afw_ldap_metadata_get_object_type_attribute(
            first_attribute, property_name);
        if (!attribute || attribute->attribute_type->never_allow_write) {
            continue;  /** @fixme Should this be an error??? */
        }
        
        bvals = afw_ldap_metadata_value_to_bv(self, property_name, value,
            xctx);
        if (bvals) {
            mod = afw_pool_calloc_type(p, LDAPMod, xctx);
            mod->mod_op = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
            mod->mod_type = apr_pstrndup(afw_pool_get_apr_pool(p),
                property_name->s, property_name->len);
            mod->mod_vals.modv_bvals = bvals;
            APR_ARRAY_PUSH(mods, LDAPMod *) = mod;
        }
    }

    /* Mark end of mods with NULL. */
    APR_ARRAY_PUSH(mods, LDAPMod *) = NULL;

    /* Make null terminated dn. */
    dn_z = (char *)afw_utf8_to_utf8_z(suggested_object_id, p, xctx);

    /* Add object. */
    rv = ldap_add_s(self->ld, dn_z, (LDAPMod * *)mods->elts);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, "ldap_add_s() failed.",
            xctx);
    }

    /* Release temporary pool. */
    afw_pool_release(p, xctx);

    /* Return object id. */
    return suggested_object_id;
}



/*
 * Implementation of method modify_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_modify_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_adaptor_modify_entry_t *const *entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *self =
        (afw_ldap_internal_adaptor_session_t *)instance;
    afw_ldap_metadata_t *metadata = self->adaptor->metadata;
    const afw_pool_t *p;
    afw_ldap_object_type_attribute_t *first_attribute;
    afw_ldap_object_type_attribute_t *attribute;
    apr_array_header_t *mods;
    const afw_utf8_t *property_name;
    const afw_adaptor_modify_entry_t * const * e;
    LDAPMod *mod;
    char *dn_z;
    int rv;

    /* This method is not allowed for synthetic object types. */
    if (afw_ldap_metadata_handles(object_type_id))
    {
        AFW_THROW_ERROR_Z(read_only, "Not allowed", xctx);
    }

    /* Get first attribute for object type. */
    first_attribute = apr_hash_get(metadata->object_type_attributes,
        object_type_id->s, object_type_id->len);
    if (!first_attribute) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Object type " AFW_UTF8_FMT_Q " not found",
            AFW_UTF8_FMT_ARG(object_type_id));
    }

    /* Create mods. */
    p = afw_pool_create(instance->p, xctx);
    mods = apr_array_make(afw_pool_get_apr_pool(p), 10, sizeof(LDAPMod *));
    afw_memory_clear(&mod);
    for (e = entry; *e; e++) {

        /* Only entity properties allowed. */
        if (!(*e)->first_property_name_entry ||
            (*e)->first_property_name_entry->next)
        {
            AFW_THROW_ERROR_Z(general,
                "LDAP adaptor only allows entity properties to be modified",
                xctx);
        }
        property_name = &(*e)->first_property_name_entry->property_name;

        /* Get property type for property and make sure it can be written. */
        attribute = afw_ldap_metadata_get_object_type_attribute(
            first_attribute, property_name);
        if (!attribute || attribute->attribute_type->never_allow_write)
        {
            AFW_THROW_ERROR_FZ(read_only, xctx,
                "Property " AFW_UTF8_FMT_Q " can not be modified",
                AFW_UTF8_FMT_ARG(property_name)
            );
        }

        /* Create and initialize mod. */
        mod = afw_pool_calloc_type(p, LDAPMod, xctx);
        mod->mod_type = apr_pstrndup(afw_pool_get_apr_pool(p),
            property_name->s, property_name->len);
        mod->mod_type = apr_pstrndup(afw_pool_get_apr_pool(p),
            (*e)->first_property_name_entry->property_name.s,
            (*e)->first_property_name_entry->property_name.len);

        switch ((*e)->type) {

        case afw_adaptor_modify_entry_type_add_value:
            mod->mod_op = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
            mod->mod_vals.modv_bvals = afw_ldap_metadata_value_to_bv(self,
                property_name, (*e)->value, xctx);
            break;

        case afw_adaptor_modify_entry_type_remove_property:
            mod->mod_op = LDAP_MOD_DELETE;
            break;

        case afw_adaptor_modify_entry_type_remove_value:
            mod->mod_op = LDAP_MOD_DELETE | LDAP_MOD_BVALUES;
            mod->mod_vals.modv_bvals = afw_ldap_metadata_value_to_bv(self,
                property_name, (*e)->value, xctx);
            break;

        case afw_adaptor_modify_entry_type_set_property:
            mod->mod_op = LDAP_MOD_REPLACE | LDAP_MOD_BVALUES;
            mod->mod_vals.modv_bvals = afw_ldap_metadata_value_to_bv(self,
                property_name, (*e)->value, xctx);
            break;

        default:
            AFW_THROW_ERROR_FZ(general, xctx,
                "Unsupported entry type %d", (*e)->type);
        }

        APR_ARRAY_PUSH(mods, LDAPMod *) = mod;
    }

    /* Mark end of mods with NULL. */
    APR_ARRAY_PUSH(mods, LDAPMod *) = NULL;

    /* Make null terminated dn. */
    dn_z = (char *)afw_utf8_to_utf8_z(object_id, p, xctx);

    /* Modify object. */
    rv = ldap_modify_s(self->ld, dn_z, (LDAPMod * *)mods->elts);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, "ldap_modify_s() failed.",
            xctx);
    }

    /* Release temporary pool. */
    afw_pool_release(p, xctx);
}



/*
 * Implementation of method replace_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_replace_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    /*
     * Replace doesn't make sense for LDAP because of internally maintained
     * attributes.
     */
    AFW_THROW_ERROR_Z(general,
        "LDAP adaptor does not support replace_object()",
        xctx);
}



/*
 * Implementation of method delete_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_delete_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *self =
        (afw_ldap_internal_adaptor_session_t *)instance;
    char *dn_z;
    int rv;

    /* This method is not allowed for synthetic object types. */
    if (afw_ldap_metadata_handles(object_type_id))
    {
        AFW_THROW_ERROR_Z(read_only, "Not allowed.", xctx);
    }

    /* Make null terminated dn. */
    dn_z = (char *)afw_utf8_to_utf8_z(object_id, xctx->p, xctx);

    /* Delete object. */
    rv = ldap_delete_s(self->ld, dn_z);
    if (rv != LDAP_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, ldap, rv, "ldap_delete_s() failed.",
            xctx);
    }
}



/*
 * Implementation of method begin_transaction of interface afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* LDAP does not support transactions. */
    return NULL;
}



/*
 * Implementation of method get_journal of interface afw_adaptor_session.
 */
const afw_adaptor_journal_t *
impl_afw_adaptor_session_get_journal_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* Event journal is not supported by this adaptor. */
    return NULL;
}



/*
 * Implementation of method get_key_value_interface of interface
 * afw_adaptor_session.
 */
const afw_adaptor_key_value_t *
impl_afw_adaptor_session_get_key_value_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* Key value interface is not supported by this adaptor. */
    return NULL;
}

/*
 * Implementation of method get_index_interface of interface afw_adaptor_session.
 */
const afw_adaptor_impl_index_t *
impl_afw_adaptor_session_get_index_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* Index interface is not supported by this adaptor. */
    return NULL;
}


/*
 * Implementation of method get_object_type_cache_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_object_type_cache_t *
impl_afw_adaptor_session_get_object_type_cache_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *self =
        (afw_ldap_internal_adaptor_session_t *)instance;

    afw_adaptor_impl_object_type_cache_initialize(
        &self->object_type_cache,
        &afw_adaptor_impl_object_type_cache_inf,
        instance, true, xctx);

    return &self->object_type_cache;
}

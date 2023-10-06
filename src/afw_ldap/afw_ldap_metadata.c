// See the 'COPYING' file in the project root for licensing information.
/*
 * Internal LDAP Adaptive Framework Adaptor functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_ldap_metadata.c
 * @brief  Internal LDAP Schema parser
 */

#include "afw.h"
#include "afw_ldap_metadata.h"

typedef struct impl_lexical_s {
    const afw_pool_t *p;
    afw_xctx_t *xctx;
    const afw_utf8_octet_t *start;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end;
    const afw_utf8_z_t *tkn;
    afw_utf8_t token;
} impl_lexical_t;

static const afw_object_t *
impl_get_rootdse(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_object_t *
impl_get_schema(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_utf8_t *subschema_subentry,
    const afw_pool_t *p, afw_xctx_t *xctx);


static const afw_object_t *
impl_parse_schema_entry(
    const afw_utf8_t *name,
    const afw_utf8_t *string,
    const afw_pool_t *p, afw_xctx_t *xctx);


static apr_hash_t *
impl_parse_definition(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *name_in_schema,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx);


static void
impl_retrieve_objects(
    afw_ldap_internal_adaptor_session_t * self,
    apr_hash_t *ht,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx);


static void
impl_make_property_type_and_handler_hash_tables(
    const afw_utf8_t *adaptor_id,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx);


static void
impl_make_object_types(
    const afw_utf8_t *adaptor_id,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx);



const afw_object_t *
impl_get(
    apr_hash_t *ht, const afw_utf8_t *id)
{
    return apr_hash_get(ht, id->s, id->len);
}



const afw_object_t *
impl_get_rootdse(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    LDAPMessage *res;
    LDAPMessage *e;

    res = afw_ldap_internal_search_s(session, "", LDAP_SCOPE_BASE,
        "(objectclass=*)", xctx);

    /* Get entry and return it as an adaptive object. */
    e = ldap_first_entry(session->ld, res);
    if (!e) {
        AFW_THROW_ERROR_Z(general,
            "ldap_first_entry() failed.", xctx);
    }

    return afw_ldap_internal_create_object_from_entry(session,
        afw_ldap_s__AdaptiveLdapRootDse_, afw_s_current,
        e, p, xctx);

}

const afw_object_t *
impl_get_schema(
    afw_ldap_internal_adaptor_session_t *session,
    const afw_utf8_t *subschema_subentry,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    LDAPMessage *res;
    LDAPMessage *e;
    const afw_utf8_z_t *s;

    /** @fixme Should probably keep this one around.*/
    s = afw_utf8_z_create(subschema_subentry->s, subschema_subentry->len, p, xctx);


    res = afw_ldap_internal_search_s(session, s,
        LDAP_SCOPE_BASE, "(objectclass=*)", xctx);

    /* Get entry and return it as an adaptive object. */
    e = ldap_first_entry(session->ld, res);
    if (!e) {
        AFW_THROW_ERROR_Z(general,
            "ldap_first_entry() failed.", xctx);
    }

    return afw_ldap_internal_create_object_from_entry(session,
        afw_ldap_s__AdaptiveLdapSchema_, afw_s_current,
        e, p, xctx);
}


static impl_lexical_t *
impl_create_lexical_analyzer(const afw_utf8_t *s,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_lexical_t *self;

    self = afw_xctx_calloc_type(impl_lexical_t, xctx);

    self->start = s->s;
    self->c = s->s;
    self->end = s->s + s->len;
    self->p = p;
    self->xctx = xctx;

    return self;
}


static const afw_utf8_z_t *
impl_get_token(impl_lexical_t *self)
{
    const afw_utf8_octet_t *c;

    /* Skip whitespace. */
    for (c = self->c; c < self->end; ++c) {
        if (*c != ' ' /*&& *c != '\t' && *c != '\n' && *c != '\r'*/) break;
    }
    self->c = c;

    /* Return NULL if at end of input. */
    if (self->c >= self->end) {
        return NULL;
    }

    /* If it starts with an apostrophe, token is chars up to next apostrophe. */
    if (*c == '\'') {
        for (c = ++(self->c); c < self->end && *c != '\''; ++c);
        if (c >= self->end) {
            AFW_THROW_ERROR_Z(general, "Error parsing schema.", self->xctx);
        }
        self->token.s = self->c;
        self->token.len = c - self->c;
        self->tkn = afw_utf8_z_create(self->c, c - self->c,
            self->p, self->xctx);
        c++; /* Skip apostrophe */
    }

    /* Else token is special characters or characters up to space or end. */
    else {
        if (*c == '(' || *c == ')' || *c == '$') {
            c++;
        } else {
            for (c = self->c; c < self->end && *c != ' '; ++c);
        }
        self->token.s = self->c;
        self->token.len = c - self->c;
        self->tkn = afw_utf8_z_create(self->c, c - self->c,
            self->p, self->xctx);
    }

    self->c = c;
    return self->tkn;
}


static void
impl_set_string(impl_lexical_t *self, afw_utf8_t *string)
{
    /* If starts with single quote, parse as quoted string. */
    if (*(self->tkn) == '\'') {
        /** @fixme Do escaping. */
        string->len = strlen(self->tkn);
        if (string->len < 2) {
            AFW_THROW_ERROR_Z(general, "Error parsing schema.", self->xctx);
        }
        string->len -= 2;
        string->s = self->tkn + 1;
    }

    /* If does not start with single quote, use value asis. */
    else {
        string->s = self->tkn;
        string->len = strlen(self->tkn);
    }
}


static const afw_value_t *
impl_get_value(impl_lexical_t *self)
{
    const afw_utf8_z_t *tkn;
    const afw_value_t *val;
    afw_size_t count;
    const afw_utf8_octet_t *save_c;
    const afw_array_t *list;
    afw_utf8_t *s;
    afw_utf8_t string;

    tkn = impl_get_token(self);
    if (!tkn) {
        AFW_THROW_ERROR_Z(general, "Error parsing schema.", self->xctx);
    }

    /* If '(', build list of single values. */
    else if (afw_utf8_z_equal(tkn, "(")) {

        /* Determine number of strings. */
        save_c = self->c;
        for (count = 0; ;) {
            tkn = impl_get_token(self);

            /* Expect token. */
            if (!tkn) {
                AFW_THROW_ERROR_Z(general,
                    "Error parsing schema",
                    self->xctx);
            }

            /* If ')' it's end of list. */
            if (afw_utf8_z_equal(tkn, ")")) break;

            /* If '$', skip. */
            if (afw_utf8_z_equal(tkn, "$")) continue;

            /* Increment string count. */
            count++;
        }
        self->c = save_c;

        /* Allocate list to hold strings and populate it. */
        s = afw_pool_malloc(self->p, count * sizeof(afw_utf8_t), self->xctx);
        list = afw_array_create_wrapper_for_array(s, false,
            afw_data_type_string, count, self->p, self->xctx);
        val = afw_value_create_array(list, self->p, self->xctx);
        for (;;) {
            tkn = impl_get_token(self);
            if (afw_utf8_z_equal(tkn, ")")) break;
            if (afw_utf8_z_equal(tkn, "$")) continue;
            impl_set_string(self, s++);
        }
    }

    /* If not list, just return single value. */
    else {
        impl_set_string(self, &string);
        val = afw_value_create_string(&string, self->p, self->xctx);
    }

    /* Return single value or list. */
    return val;

}



/* Convert LDAP schema value to an object. */
const afw_object_t *
impl_parse_schema_entry(
    const afw_utf8_t *name,
    const afw_utf8_t *string,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_z_t *tkn;
    impl_lexical_t *self;
    const afw_object_t *obj;
    const afw_utf8_z_t *kwd;
    const afw_value_t *val;

    self = impl_create_lexical_analyzer(string, p, xctx);
    obj = afw_object_create(p, xctx);

    /* Starts with '(' */
    tkn = impl_get_token(self);
    if (!tkn || !afw_utf8_z_equal(tkn, "(")) {
        AFW_THROW_ERROR_Z(general, "Error parsing schema.", self->xctx);
    }

    /* Get value and set property with this value and passed name. */
    val = impl_get_value(self);
    afw_object_set_property(obj, name, val, xctx);

    /* Loop processing keywords. */
    while ((kwd = impl_get_token(self))) {

        /* If to closing ')', make an object value and return. */
        if (afw_utf8_z_equal(kwd, ")")) {
            tkn = impl_get_token(self);
            if (tkn) {
                AFW_THROW_ERROR_Z(general, "Error parsing schema.",
                    self->xctx);
            }
            return obj;
        }

        /* If one of the reserved boolean keywords, set it. */
        if (
            afw_utf8_z_equal(kwd, "SINGLE-VALUE") ||
            afw_utf8_z_equal(kwd, "COLLECTIVE") ||
            afw_utf8_z_equal(kwd, "OBSOLETE") ||
            afw_utf8_z_equal(kwd, "ABSTRACT") ||
            afw_utf8_z_equal(kwd, "STRUCTURAL") ||
            afw_utf8_z_equal(kwd, "AUXILIARY") ||
            afw_utf8_z_equal(kwd, "COLLECTIVE") ||
            afw_utf8_z_equal(kwd, "NO-USER-MODIFICATION")
            )
        {
            val = afw_value_true;
        }

        /* If any other keyword, get following value and set. */
        else {
            val = impl_get_value(self);
        }

        /* Set property. */
        afw_object_set_property(obj,
            afw_utf8_create(kwd, AFW_UTF8_Z_LEN, p, xctx),
            val, xctx);
    }

    /* Error if it did not end with a ')'. */
    AFW_THROW_ERROR_Z(general, "Error parsing schema.",
        self->xctx);
}
 
static void
impl_parse_definition_add(
    apr_hash_t *ht,
    const afw_object_t *obj,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type,
    const afw_utf8_t *id,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *object_id;

    /* Set ids in object. */
    object_id = afw_utf8_clone(id, metadata->p, xctx);
    afw_object_meta_set_ids(obj, adaptor_id, object_type, object_id, xctx);

    /* Add object to hash table. */
    apr_hash_set(ht, object_id->s, object_id->len, obj);    
};

apr_hash_t *
impl_parse_definition(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *name_in_schema,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    const afw_object_t *obj;
    const afw_value_t *value;
    const afw_value_t *name_value;
    apr_hash_t *ht;
    const afw_utf8_t *id;
    const afw_utf8_t *e;
    const afw_utf8_t *name;
    const afw_array_t *list;
    const afw_iterator_t *iterator;
    const afw_iterator_t *iterator2;

    /* Use metadata p. */
    p = metadata->p;

    value = afw_object_get_property(metadata->schema_object, name_in_schema, xctx);
    if (!value) {
        return NULL;
    }
    if (!afw_value_is_array_of_string(value)) {
        AFW_THROW_ERROR_Z(general, "Expecting list of strings", xctx);
    }
    list = ((const afw_value_array_t *)value)->internal;

    /* Create hash table. */
    ht = apr_hash_make(afw_pool_get_apr_pool(p));

    /* Process each definition in list. */
    for (iterator = NULL;;) {
        afw_array_get_next_internal(list, &iterator, NULL, (const void **)&e, xctx);
        if (!e) {
            break;
        }
        obj = impl_parse_schema_entry(afw_ldap_s_numericoid, e,
            metadata->p, xctx);

        name_value = afw_object_get_property(obj,
            afw_ldap_s_NAME, xctx);

        if (!name_value) {
            name_value = afw_object_get_property(obj,
                afw_ldap_s_numericoid, xctx);
            if (!name_value) {
                AFW_THROW_ERROR_Z(general, "Error parsing schema", xctx);
            }
        }
        if (afw_value_is_array_of_string(name_value)) {
            for (iterator2 = NULL;;)
            {
                afw_array_get_next_internal(
                    ((const afw_value_array_t *)name_value)->internal,
                    &iterator2, NULL, (const void **)&name, xctx);
                if (!name) {
                    break;
                }
                impl_parse_definition_add(ht, obj, adaptor_id, object_type_id,
                    name, metadata, xctx);
            }
        }
        else {
            id = afw_value_as_utf8(name_value, p, xctx);
            if (!id) {
                AFW_THROW_ERROR_Z(general, "Error parsing schema", xctx);
            }
            impl_parse_definition_add(ht, obj, adaptor_id, object_type_id, id,
                metadata, xctx);
        }
    }

    return ht;
}


void
impl_make_property_type_and_handler_hash_tables(
    const afw_utf8_t *adaptor_id,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx)
{
    apr_hash_t *ht;

    const afw_value_t *value;
    apr_hash_index_t *hi;
    const afw_object_t *attribute_type_object;
    const afw_utf8_t *id;
    afw_utf8_t syntax_oid;
    apr_hash_t *ht_attribute_types;
    const afw_utf8_t *syntax;
    const afw_utf8_t *error_id;
    const afw_utf8_t *string;
    afw_size_t len;
    const afw_utf8_octet_t *s;
    afw_utf8_t temp;
    afw_ldap_metadata_attribute_type_t *attribute_type;
    const void *key;
    apr_ssize_t klen;
    afw_boolean_t found;
    const afw_pool_t *p;

    /* Use metadata p. */
    p = metadata->p;

    /* Make value meta hash table. */
    ht = apr_hash_make(afw_pool_get_apr_pool(p));
    metadata->value_meta_objects = ht;

    /* Make attribute types hash table. */
    ht_attribute_types = apr_hash_make(afw_pool_get_apr_pool(p));
    metadata->attribute_types = ht_attribute_types;

    /* Create property type and object for each attribute type. */
    for (hi = apr_hash_first(afw_pool_get_apr_pool(p),
        metadata->attribute_type_objects);
        hi; hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, &key, &klen, (void **)&attribute_type_object);
        attribute_type = afw_pool_calloc_type(p,
            afw_ldap_metadata_attribute_type_t, xctx);
        attribute_type->attribute_type_object = attribute_type_object;

        /*
         * Use syntax to determine datatype and handler.  Add handler to
         * ht_attribute_types.
         */
        syntax = afw_object_old_get_property_as_string(attribute_type_object,
            afw_ldap_s_SYNTAX, xctx);
        if (syntax) {

            /* Get oid part of syntax. */
            error_id = afw_ldap_s_SYNTAX;
            for (len = syntax->len, s = syntax->s;
                len > 0 && *s != '{';
                len--, s++);
            syntax_oid.s = syntax->s;
            syntax_oid.len = s - syntax->s;
            attribute_type->syntax_oid = afw_utf8_clone(
                &syntax_oid, p, xctx);

            /* If there is a "{length}", set constraint. */
            if (len > 0) {
                temp.s = syntax->s + syntax_oid.len + 1;
                if (len < 3)  goto error;
                temp.len = len - 2;
                if (temp.s[temp.len] != '}') goto error;
                attribute_type->upper_bound =
                    afw_number_utf8_to_integer(&temp,
                    metadata->p, xctx);
                attribute_type->upper_bound_present = true;
            }

            /* Determine if single. */
            attribute_type->is_single =
                afw_object_old_get_property_as_boolean(
                    attribute_type_object, afw_ldap_s_a_single_dash_value,
                    &found, xctx);

            /* X-NDS_LOWER_BOUND. */
            attribute_type->lower_bound =
                afw_object_old_get_property_as_integer_deprecated(
                    attribute_type_object, afw_ldap_s_a_X_NDS_LOWER_BOUND,
                    &attribute_type->lower_bound_present, xctx);

            /* X-NDS_UPPER_BOUND if already be obtained by {} in syntax. */
            if (!attribute_type->upper_bound_present) {
                attribute_type->upper_bound =
                    afw_object_old_get_property_as_integer_deprecated(
                        attribute_type_object, afw_ldap_s_a_X_NDS_UPPER_BOUND,
                        &attribute_type->upper_bound_present, xctx);
            }

            /* NO-USER-MODIFICATION - Never allow write. */
            if (afw_object_old_get_property_as_boolean_deprecated(attribute_type_object,
                afw_ldap_s_a_NO_USER_MODIFICATION, xctx))
            {
                attribute_type->never_allow_write = true;
            }

            /* X-NDS_HIDDEN - Never allow read or write. */
            if (afw_object_old_get_property_as_boolean_deprecated(attribute_type_object,
                afw_ldap_s_a_X_NDS_HIDDEN, xctx))
            {
                attribute_type->never_allow_read = true;
                attribute_type->never_allow_write = true;
            }

            /* X-NDS_NONREMOVABLE - Never remove. */
            if (afw_object_old_get_property_as_boolean_deprecated(attribute_type_object,
                afw_ldap_s_a_X_NDS_NONREMOVABLE, xctx))
            {
                attribute_type->never_allow_write = true;
            }

            /* X-NDS_READ_FILTERED - Operational. */
            if (afw_object_old_get_property_as_boolean_deprecated(attribute_type_object,
                afw_ldap_s_a_X_NDS_READ_FILTERED, xctx))
            {
                attribute_type->operational = true;
                attribute_type->never_allow_write = true;
            }

            /*
             * Assign syntax handler related values in attribute type.  This
             * must happen after other values in attribute type have been
             * set.
             */
            afw_ldap_internal_syntax_handler_assign(
                metadata, attribute_type, xctx);

            /* If attribute can be read, make a property type object. */
            if (!attribute_type->never_allow_read) {

                /* Create property type object and set ids. */
                attribute_type->property_type_object =
                    afw_object_create(metadata->p, xctx);
                id = afw_object_meta_get_object_id(attribute_type_object, xctx);
                afw_object_meta_set_ids(
                    attribute_type->property_type_object,
                    adaptor_id, afw_s__AdaptiveValueMeta_, id, xctx);
                attribute_type->property_name = id;

                /* Set data type and data type parameter. */
                if (attribute_type->is_single) {
                    if (attribute_type->data_type_id_value) {
                        afw_object_set_property(
                            attribute_type->property_type_object,
                            afw_s_dataType, attribute_type->data_type_id_value,
                            xctx);
                    }
                }
                else {
                    afw_object_set_property(
                        attribute_type->property_type_object,
                        afw_s_dataType, afw_data_type_array_id_value, 
                        xctx);
                    if (attribute_type->data_type_id_value) {
                        afw_object_set_property(
                            attribute_type->property_type_object,
                            afw_s_dataTypeParameter,
                            attribute_type->data_type_id_value,
                            xctx);
                    }
                }

                /* If lower bound present, set in property type object. */
                if (attribute_type->lower_bound_present) {
                    string = afw_number_integer_to_utf8(
                        attribute_type->lower_bound, p, xctx);
                    afw_object_set_property_as_string(
                        attribute_type->property_type_object,
                        afw_s_minValue, string, xctx);

                }

                /* If upper bound present, set in property type object. */
                if (attribute_type->upper_bound_present) {
                    string = afw_number_integer_to_utf8(
                        attribute_type->upper_bound, p, xctx);
                    afw_object_set_property_as_string(
                        attribute_type->property_type_object,
                        afw_s_maxValue, string, xctx);
                }

                /* allowWrite=false if applicable. */
                if (attribute_type->never_allow_write) {
                    afw_object_set_property(
                        attribute_type->property_type_object,
                        afw_s_allowWrite,
                        afw_value_false,
                        xctx);
                }

                /* If you can read it, you can query it. */
                afw_object_set_property_as_boolean(
                    attribute_type->property_type_object,
                    afw_s_allowQuery, afw_value_true, xctx);

                /* Description. */
                value = afw_object_get_property(attribute_type_object, afw_ldap_s_DESC,
                    xctx);
                if (value) {
                    afw_object_set_property(
                        attribute_type->property_type_object,
                        afw_s_description, value, xctx);
                }

                /* Add property type object to propertyType hash table. */
                apr_hash_set(ht, id->s, id->len,
                    attribute_type->property_type_object);

            }


            /* Add attribute type to ht. */
            apr_hash_set(ht_attribute_types, key, klen, attribute_type);
        }

    }

    return;
    
error:
    AFW_THROW_ERROR_FZ(general, xctx, "Internal " AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(error_id));
}

static void
impl_a_property_to_object_type(
    afw_boolean_t required,
    const afw_utf8_t *name,
    const afw_object_t *properties,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx)
{
    const afw_object_t *prop;
    const afw_object_t *parent;
    afw_value_array_t *parent_paths;
    const afw_utf8_t *s;
    afw_ldap_metadata_attribute_type_t *attribute_type;

    /* If property is a never read, don't add to object type. */
    attribute_type = apr_hash_get(metadata->attribute_types,
        name->s, name->len);
    if (!attribute_type || attribute_type->never_allow_read) {
        return;
    }

    /* Create object for property. */
    prop = afw_object_create_embedded(properties,
        name, xctx);

    /* Set parent to corresponding value meta. */
    parent = apr_hash_get(metadata->value_meta_objects,
        name->s, name->len);
    if (parent) {
        s = afw_object_meta_get_path(parent, xctx);
        parent_paths = afw_value_allocate_array(prop->p, xctx);
        parent_paths->internal = afw_array_create_wrapper_for_array(
            (const void *)s, false, afw_data_type_anyURI, 1, prop->p, xctx);
        afw_object_meta_set_parent_paths(prop, parent_paths, xctx);
    }

    /* If required, add required property. */
    if (required) {
        afw_object_set_property(prop, afw_s_required,
            afw_value_true, xctx);
    }
}

static void
impl_properties_to_object_type(
    afw_boolean_t required,
    const afw_object_t *object_class_object,
    const afw_object_t *properties,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_array_t *list;
    const afw_utf8_t *s;
    const afw_iterator_t *iterator;
    afw_utf8_t req_pn;

    req_pn.s = (required) ? "MUST" : "MAY";
    req_pn.len = strlen(req_pn.s);
    value = afw_object_get_property(object_class_object, &req_pn, xctx);
    if (!value) {
        return;
    }
    if (afw_value_is_array_of_string(value)) {
        list = ((const afw_value_array_t *)value)->internal;
        for (iterator = NULL;;)
        {
            afw_array_get_next_internal(list,
                &iterator, NULL, (const void **)&s, xctx);
            if (!s) {
                break;
            }
            impl_a_property_to_object_type(required, s, properties, metadata,
                xctx);
        }
    }
    else if (afw_value_is_defined_and_evaluated(value)) {
        impl_a_property_to_object_type(required,
            afw_value_one_and_only_as_utf8(value, metadata->p, xctx),
            properties,
            metadata, xctx);
    }
    else {
        AFW_THROW_ERROR_Z(general, "Valuetype error.", xctx);
    }   
}

static void
impl_set_synthetic_object_type(apr_hash_t *ht,
    const afw_utf8_t *object_type_id,
    afw_xctx_t *xctx)
{
    const afw_object_t *object_type;

    object_type = afw_runtime_get_object(
        afw_s__AdaptiveObjectType_, object_type_id, xctx);
    if (!object_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Missing object '/afw/_AdaptiveObjectType_/" AFW_UTF8_FMT "'",
            AFW_UTF8_FMT_ARG(object_type_id));
    }
    apr_hash_set(ht, object_type_id->s, object_type_id->len,
        object_type);
}


/* Return afw_ldap_object_type_attribute_t * for name. */
afw_ldap_object_type_attribute_t *
afw_ldap_metadata_get_object_type_attribute(
    afw_ldap_object_type_attribute_t *first,
    const afw_utf8_t *name)
{
    afw_ldap_object_type_attribute_t *result;

    for (result = first;
        result &&
        (!result->attribute_type ||
            !afw_utf8_equal(name, result->attribute_type->property_name)
            );
        result = result->next);

    return result;
}



/*
 * Set parents and property types.  Return first
 * afw_ldap_object_type_attribute_t or NULL.
 */
static afw_ldap_object_type_attribute_t *
impl_add_parents_and_property_types(
    afw_ldap_metadata_t *metadata,
    const afw_object_t *object_class_object,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    const afw_value_t *value;
    const afw_object_t *object_type_object;
    const afw_object_t *property_types_object;
    const afw_object_t *property_type_object;
    const afw_object_t *parent;
    const afw_object_t *parent_object_class_object;
    afw_ldap_object_type_attribute_t *result;
    afw_ldap_object_type_attribute_t *object_type_attribute;
    afw_ldap_object_type_attribute_t *parent_object_type_attribute;
    afw_ldap_metadata_attribute_type_t *attribute_type;
    afw_value_array_t *parent_paths;
    const afw_utf8_t *property_name;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *s;
    afw_utf8_t *ids;
    const afw_utf8_t *parent_id;
    const afw_utf8_t *parent_id2;
    const afw_iterator_t *iterator;
    const afw_array_t *list;
    afw_size_t count;
    afw_size_t count2;
    afw_size_t i;

    /* Use metadata p. */
    p = metadata->p;

    /* Get object type object.  Return if there is not one. */
    object_type_id = afw_object_meta_get_object_id(object_class_object, xctx);
    object_type_object = impl_get(metadata->object_type_objects,
        object_type_id);
    if (!object_type_object) {
        return NULL;
    }

    /* If object_type_attribute already set for this object type id, return. */
    result = apr_hash_get(metadata->object_type_attributes,
        object_type_id->s, object_type_id->len);
    if (result) {
        return result;
    }

    /* Make and set initial object_type_attribute. */
    object_type_attribute = afw_pool_calloc_type(p,
        afw_ldap_object_type_attribute_t, xctx);
    apr_hash_set(metadata->object_type_attributes,
        object_type_id->s, object_type_id->len,
        object_type_attribute);
    result = object_type_attribute;

    /* Make list of property types for this object type. */
    property_types_object = afw_object_old_get_property_as_object(
        object_type_object, afw_s_propertyTypes, xctx);
    if (property_types_object) {
        iterator = NULL;
        while ((property_type_object =
            afw_object_old_get_next_property_as_object(
                property_types_object, &iterator, &property_name, xctx))
            )
        {
            attribute_type = apr_hash_get(metadata->attribute_types,
                property_name->s, property_name->len);
            if (object_type_attribute->attribute_type) {
                object_type_attribute->next =
                    afw_pool_calloc_type(p,
                        afw_ldap_object_type_attribute_t,
                        xctx);
                object_type_attribute = object_type_attribute->next;
            }
            object_type_attribute->attribute_type = attribute_type;
            object_type_attribute->property_type_object =
                property_types_object;
            object_type_attribute->is_required =
                afw_object_old_get_property_as_boolean_deprecated(property_type_object,
                    afw_s_required, xctx);
        }
    }

    /*  Determine max parent count and pointer to first parent id. */
    count = 0;
    parent_id = NULL;
    value = afw_object_get_property(object_class_object,
        afw_ldap_s_SUP, xctx);
    if (value) {
        if (afw_value_is_string(value)) {
            count = 1;
            parent_id = &((const afw_value_string_t *)value)->
                internal;
        }

        /* Note: All of this can be improve.  Was retrofitting a change. */
        else if (afw_value_is_array_of_string(value)) {
            list = ((afw_value_array_t *)value)->internal;
            count = afw_array_get_count(list, xctx);
            ids = afw_pool_calloc(p, count * sizeof(afw_utf8_t), xctx);
            parent_id = ids;
            for (iterator = NULL;;) {
                afw_array_get_next_internal(list, &iterator, NULL,
                    (const void **)&s, xctx);
                if (!s) {
                    break;
                }
                afw_memory_copy(ids, s);
                ids++;
            }
        }
    }

    /* Some parents don't exist such as internal ones. */
    for (parent_id2 = parent_id, count2 = count, i = 0;
        i < count;
        i++, parent_id2++)
    {
        parent = impl_get(metadata->object_type_objects, parent_id2);
        if (!parent) {
            count2--;
        }
    }

    /* If there are parents, add parent paths and add attributes to list. */
    if (count2 > 0) {
        parent_paths = afw_value_allocate_array(p, xctx);
        parent_paths->internal = afw_array_of_create(
            afw_data_type_anyURI, p, xctx);
        for (i = 0; i < count; i++, parent_id++) {
            parent = impl_get(metadata->object_type_objects,
                parent_id);

            /* If this doesn't exist, skip. */
            if (!parent) {
                continue;
            }

            /* Set parent path. */
            s = afw_object_meta_get_path(parent, xctx);
            afw_array_of_anyURI_add(parent_paths->internal, s, xctx);

            /* Make sure parent is processed first. */
            parent_object_class_object =
                impl_get(metadata->object_class_objects, parent_id);
            if (!parent_object_class_object) {
                continue;
            }
            parent_object_type_attribute =
                impl_add_parents_and_property_types(metadata,
                    parent_object_class_object, xctx);

            /* Add any unique parent property types. */
            for (;
                parent_object_type_attribute;
                parent_object_type_attribute =
                parent_object_type_attribute->next
                )
            {
                if (parent_object_type_attribute->attribute_type &&
                    !afw_ldap_metadata_get_object_type_attribute(result,
                        parent_object_type_attribute->attribute_type->
                        property_name))
                {
                    if (object_type_attribute->attribute_type) {
                        object_type_attribute->next =
                            afw_pool_calloc_type(p,
                                afw_ldap_object_type_attribute_t,
                                xctx);
                        object_type_attribute = object_type_attribute->next;
                    }
                    object_type_attribute->attribute_type =
                        parent_object_type_attribute->attribute_type;
                    object_type_attribute->property_type_object =
                        parent_object_type_attribute->property_type_object;
                    object_type_attribute->is_required =
                        parent_object_type_attribute->is_required;
                }
            }

            object_type_attribute->next =
                afw_pool_calloc_type(p,
                    afw_ldap_object_type_attribute_t,
                    xctx);
            object_type_attribute = object_type_attribute->next;
            object_type_attribute->next = apr_hash_get(
                metadata->object_type_attributes,
                parent_id->s, parent_id->len);

        }
        afw_object_meta_set_parent_paths(object_type_object,
            parent_paths, xctx);
    }

    return result;
}


/* impl_make_property_type_and_handler_hash_tables() must be called first. */
static void
impl_make_object_types(
    const afw_utf8_t *adaptor_id,
    afw_ldap_metadata_t *metadata,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    apr_hash_index_t *hi;
    const afw_object_t *object_class_object;
    const afw_object_t *object_type_object;
    const afw_utf8_t *id;
    const afw_object_t *property_types_object;
    const afw_utf8_t *default_description;
    const afw_object_t *other_properties;
    const afw_pool_t *p;

    /* Use metadata p. */
    p = metadata->p;

    /* Make hash tables. */
    metadata->object_type_objects = apr_hash_make(afw_pool_get_apr_pool(p));
    metadata->object_type_attributes = apr_hash_make(afw_pool_get_apr_pool(p));

    /* Loop though all ldap object classes. */
    for (hi = apr_hash_first(afw_pool_get_apr_pool(p),
        metadata->object_class_objects);
        hi; hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, NULL, NULL, (void **)&object_class_object);

        /* Create _AdaptiveObjectType_ and initialize. */
        object_type_object = afw_object_create(p, xctx);
        id = afw_object_meta_get_object_id(object_class_object, xctx);
        afw_object_meta_set_ids(object_type_object, adaptor_id,
            afw_ldap_s__AdaptiveObjectType_, id, xctx);
        afw_object_set_property_as_string(object_type_object, afw_s_objectType,
            id, xctx);
        afw_object_set_property(object_type_object, afw_s_allowEntity,
            (afw_object_old_get_property_as_boolean_deprecated(object_class_object,
                afw_ldap_s_STRUCTURAL, xctx))
            ? afw_value_true
            : afw_value_false,
            xctx);

        afw_object_set_property(object_type_object, afw_s_allowAdd,
            afw_value_true, xctx);
        afw_object_set_property(object_type_object, afw_s_allowChange,
            afw_value_true, xctx);
        afw_object_set_property(object_type_object, afw_s_allowDelete,
            afw_value_true, xctx);

        /*
         * Set otherProperties to cover all operational properties.  These
         * are all allowWrite=false.
         */
        other_properties = afw_object_create_embedded(
            object_type_object, afw_s_otherProperties, xctx);
        afw_object_set_property(other_properties,
            afw_s_allowWrite, afw_value_false, xctx);

        /* Add object type to object_type_objects ht. */
        apr_hash_set(metadata->object_type_objects, id->s, id->len,
            object_type_object);

        /* If object class has a description, use it otherwise, make up one. */
        value = afw_object_get_property(object_class_object, afw_ldap_s_DESC,
            xctx);
        if (!value) {
            default_description = afw_utf8_printf(metadata->p, xctx,
                "_AdaptiveObjectType representation of LDAP object"
                "class " AFW_UTF8_FMT_Q ".",
                AFW_UTF8_FMT_ARG(id));
            value = afw_value_make_single_string(
                default_description->s, default_description->len,
                p, xctx);
        }
        afw_object_set_property(object_type_object, afw_s_description, value,
            xctx);

        /* Create properties object. */
        property_types_object = afw_object_create_embedded(
            object_type_object,
            afw_s_propertyTypes, xctx);

        /* Add MUST and MAY properties. */
        impl_properties_to_object_type(AFW_TRUE,
            object_class_object, property_types_object, metadata, xctx);
        impl_properties_to_object_type(AFW_FALSE,
            object_class_object, property_types_object, metadata, xctx);

        /** @fixme Add code. */

    }

    /* Run back though setting parents and making composite property types. */
    for (hi = apr_hash_first(afw_pool_get_apr_pool(p),
        metadata->object_class_objects);
        hi; hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, NULL, NULL, (void **)&object_class_object);
        impl_add_parents_and_property_types(
            metadata, object_class_object, xctx);
    }

    /* Add synthetic object types. */
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_s__AdaptiveObjectType_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_s__AdaptiveValueMeta_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapAttributeType_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapMatchingRuleUse_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapMatchingRuleUse_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapMatchingRule_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapObjectClass_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapRootDse_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapSchema_, xctx);
    impl_set_synthetic_object_type(metadata->object_type_objects,
        afw_ldap_s__AdaptiveLdapSyntax_, xctx);
}


void
afw_ldap_metadata_load(
    afw_ldap_internal_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_ldap_internal_adaptor_session_t *session;
    afw_ldap_metadata_t *new_metadata;
    afw_ldap_metadata_t *old_metadata;
    const afw_value_t *value;
    const afw_pool_t *p;

    /* Use adaptor's pool. */
    p = adaptor->pub.p;

    /* Allocate new afw_ldap_metadata_t in subpool. */
    new_metadata = afw_pool_calloc_type(p, afw_ldap_metadata_t, xctx);
    new_metadata->p = p;

    /* Begin a temporary session with ldap adaptor. */
    session = afw_ldap_internal_adaptor_session_create(adaptor, xctx);

    /* Get rootDSE. */
    new_metadata->rootdse_object =
        impl_get_rootdse(session, p, xctx);

    /* Get subschemaSubentry property of rootDSE. */
    value = afw_object_get_property(new_metadata->rootdse_object,
        afw_ldap_s_subschemaSubentry,
        xctx);
    if (!value) {
        AFW_THROW_ERROR_Z(general, "subschemaSubentry not found.", xctx);
    }
    new_metadata->subschema_subentry =
        afw_value_one_and_only_as_utf8(value, p, xctx);

    /* Get schema and parse definitions. */
    new_metadata->schema_object =
        impl_get_schema(session,
            new_metadata->subschema_subentry, p, xctx);

    new_metadata->attribute_type_objects =
        impl_parse_definition(
            &adaptor->pub.adaptor_id,
            afw_ldap_s__AdaptiveLdapAttributeType_,
            afw_ldap_s_attributeTypes,
            new_metadata, xctx);

    new_metadata->ldap_syntax_objects =
        impl_parse_definition(
            &adaptor->pub.adaptor_id,
            afw_ldap_s__AdaptiveLdapSyntax_,
            afw_ldap_s_ldapSyntaxes,
            new_metadata, xctx);

    new_metadata->matching_rule_objects =
        impl_parse_definition(
            &adaptor->pub.adaptor_id,
            afw_ldap_s__AdaptiveLdapMatchingRule_,
            afw_ldap_s_matchingRules,
            new_metadata, xctx);

    new_metadata->matching_rule_use_objects =
        impl_parse_definition(
            &adaptor->pub.adaptor_id,
            afw_ldap_s__AdaptiveLdapMatchingRuleUse_,
            afw_ldap_s_matchingRuleUse,
            new_metadata, xctx);

    new_metadata->object_class_objects =
        impl_parse_definition(
            &adaptor->pub.adaptor_id,
            afw_ldap_s__AdaptiveLdapObjectClass_,
            afw_ldap_s_objectClasses,
            new_metadata, xctx);

    impl_make_property_type_and_handler_hash_tables(
        &adaptor->pub.adaptor_id,
        new_metadata, xctx);

    impl_make_object_types(
            &adaptor->pub.adaptor_id,
            new_metadata, xctx);

    /** @fixme add code*/

    /* Start using new metadata. */
    /** @fixme use use count based release. */
    old_metadata = adaptor->metadata;
    adaptor->metadata = new_metadata;
    if (old_metadata) {
        afw_pool_release(old_metadata->p, xctx);
    }

    /* Release temporary session. */
    afw_adaptor_session_release((const afw_adaptor_session_t *)session, xctx);

    return;
}

void
impl_retrieve_objects(
    afw_ldap_internal_adaptor_session_t * self,
    apr_hash_t *ht,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx)
{
    apr_hash_index_t *hi;
    afw_object_t *obj;

    for (hi = apr_hash_first(afw_pool_get_apr_pool(xctx->p), ht);
        hi;
        hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, NULL, NULL, (void **)&obj);
        if (afw_query_criteria_test_object(obj, criteria,
            xctx->p, xctx))
        {
            callback(obj, context, xctx);
        }
    }
}


/* Used by afw_ldap_adaptor_session() to retrieve metadata objects. */
void
afw_ldap_metadata_retrieve_objects(
    afw_ldap_internal_adaptor_session_t * self,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx)
{
    apr_hash_t *ht;
    const afw_object_t *obj;

    ht = NULL;

    /* ObjectType. */
    if (afw_utf8_equal(object_type_id,
        AFW_OBJECT_S_OBJECT_TYPE_ID_OBJECT_TYPE))
    {
        ht = self->adaptor->metadata->object_type_objects;
    }

    /* ValueMeta. */
    else if (afw_utf8_equal(object_type_id,
        AFW_OBJECT_S_OBJECT_TYPE_ID_VALUE_META))
    {
        ht = self->adaptor->metadata->value_meta_objects;
    }

    /* Object Class. */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapObjectClass_))
    {
        ht = self->adaptor->metadata->object_class_objects;
    }

    /* Attribute Type */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapAttributeType_))
    {
        ht = self->adaptor->metadata->attribute_type_objects;
    }

    /* Syntax */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapSyntax_))
    {
        ht = self->adaptor->metadata->ldap_syntax_objects;
    }

    /* Matching Rule */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapMatchingRule_))
    {
        ht = self->adaptor->metadata->matching_rule_objects;
    }

    /* Matching Rule Use */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapMatchingRuleUse_))
    {
        ht = self->adaptor->metadata->matching_rule_use_objects;
    }

    /* If one of the multi entry pseudo object types, call retrieve them. */
    if (ht) {
        impl_retrieve_objects(self, ht, criteria, context, callback, xctx);
    }

    /* If one of the single entry object types, return it. */
    else {

        /* LDAP Root DSE */
        if (afw_utf8_equal(object_type_id,
            afw_ldap_s__AdaptiveLdapRootDse_))
        {
            obj = self->adaptor->metadata->rootdse_object;
            if (afw_query_criteria_test_object(obj, criteria,
                xctx->p, xctx))
            {
                callback(obj, context, xctx);
            }
        }

        /* LDAP Schema */
        if (afw_utf8_equal(object_type_id,
            afw_ldap_s__AdaptiveLdapSchema_))
        {
            obj = self->adaptor->metadata->schema_object;
            if (afw_query_criteria_test_object(obj, criteria,
                xctx->p, xctx))
            {
                callback(obj, context, xctx);
            }
        }

    }

    /* Callback one last time with NULL object pointer. */
    callback(NULL, context, xctx);
}


AFW_DEFINE_STATIC_INLINE(const afw_object_t *)
impl_get_object(
    afw_ldap_internal_adaptor_session_t * self,
    apr_hash_t *ht,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx)
{
    return apr_hash_get(ht, object_id->s, object_id->len);
}


/* Called by afw_ldap_adaptor_session() to get a metadata object. */
void
afw_ldap_metadata_get_object(
    afw_ldap_internal_adaptor_session_t * self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;

    /* Assume no object will be found. */
    obj = NULL;

    /* RootDSE */
    if (afw_utf8_equal(object_type_id, afw_ldap_s__AdaptiveLdapRootDse_))
    {
        if (afw_utf8_equal(object_id, afw_s_current)) {
            obj = self->adaptor->metadata->rootdse_object;
        }
    }

    /* Schema */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapSchema_))
    {
        if (afw_utf8_equal(object_id, afw_s_current)) {
            obj = self->adaptor->metadata->schema_object;
        }
    }

    /* ObjectType. */
    else if (afw_utf8_equal(object_type_id, afw_s__AdaptiveObjectType_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->object_type_objects,
            object_id, xctx);
    }

    /* ValueMeta. */
    else if (afw_utf8_equal(object_type_id, afw_s__AdaptiveValueMeta_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->value_meta_objects,
            object_id, xctx);
    }

    /* Object Class. */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapObjectClass_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->object_class_objects,
            object_id, xctx);
    }

    /* Attribute Type */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapAttributeType_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->attribute_type_objects,
            object_id, xctx);
    }

    /* Syntax */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapSyntax_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->ldap_syntax_objects,
            object_id, xctx);
    }

    /* Matching Rule */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapMatchingRule_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->matching_rule_objects,
            object_id, xctx);
    }

    /* Matching Rule Use */
    else if (afw_utf8_equal(object_type_id,
        afw_ldap_s__AdaptiveLdapMatchingRuleUse_))
    {
        obj = impl_get_object(self,
            self->adaptor->metadata->matching_rule_use_objects,
            object_id, xctx);
    }

    /* Callback with object or NULL. */
    callback(obj, context, xctx);
}

/* Determine if afw_ldap_metadata.c handles an ObjectType. */
afw_boolean_t
afw_ldap_metadata_handles(const afw_utf8_t *object_type)
{
    if (object_type  && afw_utf8_starts_with(object_type, afw_s__Adaptive))
    {
        return AFW_TRUE;
    }

    return AFW_FALSE;
}


const afw_value_t *
afw_ldap_metadata_bv_to_value(
    afw_ldap_internal_adaptor_session_t *session,
    afw_ldap_object_type_attribute_t *attribute,
    const afw_utf8_t *attribute_name,
    struct berval * *bv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    if (attribute && attribute->attribute_type)
    {
        result = attribute->attribute_type->syntax_handler->to_value(
            attribute->attribute_type,
            attribute->attribute_type->syntax_handler->data,
            bv,
            p, xctx);
    }
    else {
        result = afw_ldap_internal_syntax_handler_unknown_to_value(
            bv, p, xctx);
    }
    return result;
}

struct berval **
afw_ldap_metadata_value_to_bv(afw_ldap_internal_adaptor_session_t *session,
    const afw_utf8_t *attribute_name, const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_ldap_metadata_attribute_type_t *attribute_type;

    attribute_type = apr_hash_get(
        session->adaptor->metadata->attribute_types,
        attribute_name->s, attribute_name->len);
    if (!attribute_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Unknown attribute type " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(attribute_name));
    }
    if (!value) {
        AFW_THROW_ERROR_Z(general,
            "Missing value calling afw_ldap_metadata_value_to_bv()",
            xctx);
    }
    return attribute_type->syntax_handler->to_ber(
        attribute_type, attribute_type->syntax_handler->data, value,
        session->pub.p, xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Modify Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adaptor_modify.c
 * @brief Adaptive Framework adaptor modify object
 */

#include "afw_internal.h"



static const afw_value_string_t
impl_value_set_property = {
    {&afw_value_unmanaged_string_inf},
    AFW_UTF8_LITERAL("set_property")
};


/* Work area used for reconcile. */
typedef struct impl_reconcile_wa_s {
    const afw_array_t *entries;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_reconcile_wa_t;



/* Array of entry type ids. */
static const afw_utf8_t * entry_type[] = {
#define XX(id, _) &afw_self_s_ ## id,
    AFW_ADAPTOR_MODIFY_ENTRY_TYPE_MAP(XX)
#undef XX
    NULL
};


/* Array to determine if type has a value. */
static const afw_boolean_t entry_type_has_value[] = {
#define XX(id, has) has,
    AFW_ADAPTOR_MODIFY_ENTRY_TYPE_MAP(XX)
#undef XX
    false
};


/* Array of entry type ids as adaptive string values. */
static const afw_value_string_t entry_type_value[] = {
#define XX(id, _) { {&afw_value_unmanaged_string_inf}, \
    { #id, sizeof(#id) - 1 } },
    AFW_ADAPTOR_MODIFY_ENTRY_TYPE_MAP(XX)
#undef XX
    {{NULL}}
};



/*
 * Find the innermost object/property name using name entry list.  Create
 * objects as needed if requested.
 */
static const afw_object_t *
impl_find_object(
    const afw_utf8_t * *property_name,
    afw_boolean_t create_if_necessary,
    const afw_object_t *entity,
    const afw_object_path_property_name_entry_t *first_property_name_entry,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_object_t *object;
    const afw_object_path_property_name_entry_t *entry;

    result = entity;
    *property_name = &first_property_name_entry->property_name;
    for (entry = first_property_name_entry; entry->next; entry = entry->next)
    {
        *property_name = &entry->next->property_name;
        object = afw_object_old_get_property_as_object(result,
            &entry->property_name, xctx);
        if (!object) {
            if (create_if_necessary) {
                object = afw_object_create_embedded(result,
                    &entry->property_name, xctx);
            }
            else {
                result = NULL;
                break;
            }
        }
        result = object;
    }

    return result;
}


/* Set a property using name entry list. */
static void
impl_set_property(
    const afw_object_t *object,
    const afw_object_path_property_name_entry_t *first_property_name_entry,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_utf8_t *property_name;

    obj = impl_find_object(&property_name, true, object,
        first_property_name_entry, xctx);
    afw_object_set_property(obj, property_name, value, xctx);
}



/* Convert entry type mnemonic to enum. */
AFW_DEFINE(afw_adaptor_modify_entry_type_t)
afw_adaptor_modify_entry_type(
    const afw_utf8_t *mnemonic)
{
    afw_adaptor_modify_entry_type_t i;

    for (i = 0;
        entry_type[i] && !afw_utf8_equal(entry_type[i], mnemonic);
        i++);

    if (i > afw_adaptor_modify_entry_type_invalid) {
        i = afw_adaptor_modify_entry_type_invalid;
    }

    return i;
}



/* Convert entry type enum to mnemonic. */
AFW_DEFINE(const afw_utf8_t *)
afw_adaptor_modify_entry_type_mnemonic(
    afw_adaptor_modify_entry_type_t type)
{
    return (type < 0 || type >= afw_adaptor_modify_entry_type_invalid)
        ? NULL
        : entry_type[type];
}



/* Convert entry type enum to mnemonic. */
AFW_DEFINE(const afw_value_t *)
afw_adaptor_modify_entry_type_value(
    afw_adaptor_modify_entry_type_t type)
{
    return (type < 0 || type >= afw_adaptor_modify_entry_type_invalid)
        ? NULL
        : &entry_type_value[type].pub;
}



/* Create modify entries from list of tuple lists in specified pool. */
AFW_DEFINE(const afw_adaptor_modify_entry_t * const *)
afw_adaptor_modify_entries_from_list(
    const afw_array_t *list, const afw_pool_t *p, afw_xctx_t *xctx)
{
    apr_array_header_t *ary;
    const afw_iterator_t *entry_i;
    const afw_iterator_t *tuple_i;
    const afw_iterator_t *names_i;
    const afw_array_t *tuple;
    afw_adaptor_modify_entry_t *entry;
    const afw_utf8_t *s;
    const afw_value_t *value;
    afw_object_path_property_name_entry_t *property_name_entry;
    afw_object_path_property_name_entry_t *prev_property_name_list;

    ary = apr_array_make(afw_pool_get_apr_pool(p),
        5, sizeof(afw_adaptor_modify_entry_t *));
    entry_i = NULL;
    for (;;) {

        /* Get next tuple.  Break out of loop if there are no more. */
        tuple_i = NULL;
        tuple = afw_array_of_array_get_next(list, &entry_i, xctx);
        if (!tuple) {
            break;
        }

        /* Allocate entry. */
        entry = afw_pool_calloc_type(p, afw_adaptor_modify_entry_t, xctx);

        /* Entry type. */
        s = afw_array_of_string_get_next(tuple, &tuple_i, xctx);
        if (!s) {
            goto error;
        }
        entry->type = afw_adaptor_modify_entry_type(s);
        if (entry->type == afw_adaptor_modify_entry_type_invalid) {
            goto error;
        }

        /* Entry name/names. */
        value = afw_array_get_next_value(tuple, &tuple_i, p, xctx);
        if (!value) {
            goto error;
        }
        if (afw_value_is_string(value)) {
            entry->first_property_name_entry = property_name_entry =
                afw_pool_calloc_type(p, afw_object_path_property_name_entry_t, xctx);
            s = &((const afw_value_string_t *)value)->internal;
            property_name_entry->property_name.s = s->s;
            property_name_entry->property_name.len = s->len;
        }
        else if (afw_value_is_array(value)) {
            for (names_i = NULL, prev_property_name_list = NULL;;) {
                s = afw_array_of_string_get_next(
                    ((const afw_value_array_t *)value)->internal,
                    &names_i, xctx);
                if (!s) {
                    if (!prev_property_name_list) {
                        goto error;
                    }
                    break;
                }
                property_name_entry = afw_pool_calloc_type(p,
                    afw_object_path_property_name_entry_t, xctx);
                property_name_entry->property_name.s = s->s;
                property_name_entry->property_name.len = s->len;
                if (prev_property_name_list) {
                    prev_property_name_list->next = property_name_entry;
                }
                else {
                    entry->first_property_name_entry = property_name_entry;
                }
                prev_property_name_list = property_name_entry;
            }
        }

        /* Get value. */
        entry->value = afw_array_get_next_value(tuple, &tuple_i, p, xctx);
        if ((entry->value && !entry_type_has_value[entry->type]) ||
            (!entry->value && entry_type_has_value[entry->type]))
        {
            goto error;
        }

        /* It's an error if there is a 4th value in tuple. */
        if (entry->value) {
            if (afw_array_get_next_value(tuple, &tuple_i, p, xctx)) {
                goto error;
            }
        }

        /* Push entry on entry list. */
        APR_ARRAY_PUSH(ary, afw_adaptor_modify_entry_t *) = entry;
    }

    /* NULL terminate entries list and return it. */
    APR_ARRAY_PUSH(ary, afw_adaptor_modify_entry_t *) = NULL;
    return (const afw_adaptor_modify_entry_t **)ary->elts;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Modify entry tuple number %d is invalid",
        ary->nelts + 1);

}


/* Create modify entries from list of tuple lists in specified pool. */
AFW_DEFINE(const afw_array_t *)
afw_adaptor_modify_entries_to_list(
    const afw_adaptor_modify_entry_t * const *entries,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_array_t *result;
    const afw_array_t *tuple;
    const afw_array_t *name_list;
    const afw_object_path_property_name_entry_t *property_name_entry;
    const afw_adaptor_modify_entry_t * const *e;
    const afw_value_t *value;

    /* Create list for result. */
    result = afw_array_of_create(afw_data_type_array, p, xctx);

    /* Process all modify entries. */
    for (e = entries; *e; e++) {

        /* Create a new list for tuple. */
        tuple = afw_array_create_generic(p, xctx);

        /* Entry type. */
        value = afw_adaptor_modify_entry_type_value((*e)->type);
        if (!value) {
            AFW_THROW_ERROR_Z(general, "Invalid type", xctx);
        }
        afw_array_add_value(tuple, value, xctx);

        /* Property name or list of names. */
        if ((*e)->first_property_name_entry->next) {
            name_list = afw_array_create_generic(p, xctx);
            for (property_name_entry = (*e)->first_property_name_entry;
                property_name_entry;
                property_name_entry = property_name_entry->next)
            {
                value = afw_value_create_unmanaged_string(
                    &property_name_entry->property_name,
                    p, xctx);
                afw_array_add_value(name_list, value, xctx);
            }
            value = afw_value_create_unmanaged_array(name_list, p, xctx);
        }
        else {
            value = afw_value_create_unmanaged_string(
                &(*e)->first_property_name_entry->property_name,
                p, xctx);
        }
        afw_array_add_value(tuple, value, xctx);

        /* Value. */
        if ((*e)->value) {
            afw_array_add_value(tuple, (*e)->value, xctx);
        }

        /* Add tuple to result list. */
        value = afw_value_create_unmanaged_array(tuple, p, xctx);
        afw_array_add_value(result, value, xctx);

    }

    /* Return result list. */
    return result;
}



/* Apply modify entries to unnormalize object. */
AFW_DEFINE(void)
afw_adaptor_modify_entries_apply_to_unnormalized_object(
    const afw_adaptor_modify_entry_t * const *entries,
    const afw_object_t *object,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = object->p;
    const afw_adaptor_modify_entry_t * const * entry;
    const afw_object_path_property_name_entry_t *first_property_name_entry;
    const afw_value_t *value, *old_value = NULL;
    const afw_value_t *new_value;
    const afw_array_t *list;
    const afw_object_t *obj;
    const afw_utf8_t *property_name = NULL;
    const afw_utf8_t *s;

    /** @fixme is this supposed to be use **/
    // const afw_iterator_t *iterator = NULL;

    /* Process all entries. */
    for (entry = entries; *entry; entry++)
    {
        first_property_name_entry = ((*entry)->first_property_name_entry);
        value = (*entry)->value;

        /* Process based on entry type. */
        switch ((*entry)->type) {


        /* Add value. */
        case afw_adaptor_modify_entry_type_add_value:

            if (!afw_value_is_defined_and_evaluated(value)) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "add_value " AFW_UTF8_FMT_Q
                    " must be a defined and evaluated value",
                    AFW_UTF8_FMT_ARG(
                        &first_property_name_entry->property_name));
            }

            /* If object already has this property, add value to it. */
            old_value = afw_object_path_property_name_list_get_property(object,
                first_property_name_entry, xctx);

            if (old_value) {

                /* If old value is a list, just add new value to it. */
                if (afw_value_is_array(old_value)) {
                    list = afw_value_as_array(old_value, xctx);
                    afw_array_add_value(list, value, xctx);
                }

                /*
                 * If value is a single value, add it and new value to a list and use
                 * that.  This may result in a list with value of different data type,
                 * as well as a property being a list of values that later will be
                 * one value.  When the result object is actually used, it will probably
                 * be normalize using an object type, which will resolve this either
                 * by converting values or indication properties in error.
                 */
                else {
                    list = afw_array_create_generic(object->p, xctx);
                    afw_array_add_value(list, old_value, xctx);
                    afw_array_add_value(list, value, xctx);
                    value = afw_value_create_unmanaged_array(list,
                        p, xctx);
                    impl_set_property(object, first_property_name_entry, value,
                        xctx);
                }

            }

            else {
                impl_set_property(object, first_property_name_entry, value,
                    xctx);
            }

            break;


        /* Remove property.  Fail is silent. */
        case afw_adaptor_modify_entry_type_remove_property:

            obj = impl_find_object(&property_name, false, object,
                first_property_name_entry, xctx);
            if (obj) {
                impl_set_property(object, first_property_name_entry, NULL,
                    xctx);
            }

            break;


        /* Remove value. */
        case afw_adaptor_modify_entry_type_remove_value:
            old_value = afw_object_path_property_name_list_get_property(object,
                first_property_name_entry, xctx);

            if (old_value) {

                if (afw_value_is_array(old_value)) {
                    list = ((const afw_value_array_t *)old_value)
                        ->internal;
                    afw_array_remove_value(list, value, xctx);
                    new_value = afw_value_create_unmanaged_array(list, p, xctx);
                    impl_set_property(object, first_property_name_entry,
                            new_value, xctx);
                }

                else {
                    if (!afw_value_equal(value, old_value, xctx)) {
                        s = afw_object_path_make_property_name_expression(
                            first_property_name_entry, p, xctx);
                        AFW_THROW_ERROR_FZ(general, xctx,
                            "Error: Value does not exist for property "
                            AFW_UTF8_FMT_Q,
                            AFW_UTF8_FMT_ARG(s));
                    }
                    else {
                        impl_set_property(object, first_property_name_entry,
                            NULL, xctx);
                    }
                }
            }

            else {
                /* There are no values that exist for this property */
                s = afw_object_path_make_property_name_expression(
                    first_property_name_entry, p, xctx);
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Error: Value does not exist for property " AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(s));
            }

            break;


        /* Set property. */
        case afw_adaptor_modify_entry_type_set_property:

            obj = impl_find_object(&property_name, true, object,
                first_property_name_entry, xctx);

            afw_object_set_property(obj, property_name,
                value, xctx);

            break;


        /* Invalid modify type. */
        default:
            AFW_THROW_ERROR_FZ(general, xctx, "Invalid modify type %d",
                (*entry)->type);
        }
    }

}


static void
impl_add_reconcile_property(
    impl_reconcile_wa_t *wa,
    const afw_object_type_property_type_t *embedding_pt,
    const afw_utf8_t *embedding_property_name,
    const afw_object_type_property_type_t *pt,
    const afw_utf8_t *property_name,
    const afw_array_t *property_names,
    const afw_value_t *value)
{
    const afw_array_t *new_property_names;
    const afw_array_t *tuple;
    const afw_value_t *v;

    if (embedding_pt && !embedding_pt->allow_write) {
        //return; /** @fixme Ignore these on reconcile???
        AFW_THROW_ERROR_FZ(general, wa->xctx,
            "Property " AFW_UTF8_FMT " can not be modified",
            AFW_UTF8_FMT_ARG(embedding_property_name));
        //*/
    }

    if (pt && !pt->allow_write) {
        // return; /** @fixme Ignore these on reconcile???
        AFW_THROW_ERROR_FZ(general, wa->xctx,
            "Property " AFW_UTF8_FMT " can not be modified",
            AFW_UTF8_FMT_ARG(property_name));
        //*/
    }

    tuple = afw_array_create_generic(wa->p, wa->xctx);

    /* Entry type. */
    v = (const afw_value_t *)((value)
        ? &entry_type_value[afw_adaptor_modify_entry_type_set_property]
        : &entry_type_value[afw_adaptor_modify_entry_type_remove_property]);
    afw_array_add_value(tuple, v, wa->xctx);

    /* Property name. */
    if (property_names) {
        new_property_names = afw_array_create_or_clone(
            property_names, afw_data_type_string, false,
            wa->p, wa->xctx);
        v = afw_value_create_unmanaged_string(property_name,
            wa->p, wa->xctx);
        afw_array_add_value(new_property_names, v, wa->xctx);
        v = afw_value_create_unmanaged_array(new_property_names,
            wa->p, wa->xctx);
    }
    else {
        v = afw_value_create_unmanaged_string(property_name,
            wa->p, wa->xctx);
    }
    afw_array_add_value(tuple, v, wa->xctx);

    /* Value */
    if (value) {
        afw_array_add_value(tuple, value, wa->xctx);
    }

    /* Add tuple to entries. */
    v = afw_value_create_unmanaged_array(tuple, wa->p, wa->xctx);
    afw_array_add_value(wa->entries, v, wa->xctx);
}


static void
impl_reconcile_object(
    impl_reconcile_wa_t *wa,
    const afw_object_type_property_type_t *embedding_pt,
    const afw_utf8_t *embedding_property_name,
    const afw_object_type_t *object_type,
    const afw_array_t *property_names,
    const afw_object_t *original,
    const afw_object_t *modified,
    const afw_object_t *journal_entry)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *original_value;
    const afw_value_t *modified_value;
    const afw_value_t *value;
    const afw_array_t *new_property_names;
    const afw_object_type_property_type_t *pt;
    const afw_object_type_t *property_object_type;
    const afw_utf8_t *object_type_id;
    const afw_object_t *object = NULL;

    /* Run though modified producing set_property if different from original. */
    for (iterator = NULL;;) {

        /* Get next property and stop looping if there are no more. */
        modified_value = afw_object_get_next_property(modified,
            &iterator, &property_name, wa->xctx);
        if (!modified_value) break;

        /* Get property type. */
        pt = afw_object_type_property_type_get(object_type,
            property_name, wa->xctx);
        if (!pt) {
            AFW_THROW_ERROR_FZ(general, wa->xctx,
                "Missing property type for " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(property_name));
        }

        /* Normalize modified value. */
        modified_value = afw_object_type_property_type_normalize(pt,
            modified_value, wa->p, wa->xctx);

        /*
         * Try to get original value with same property name.  If there is not
         * one or modified value is an object but original is not, add value.
         */
        original_value = afw_object_get_property(original,
            property_name, wa->xctx);
        if (!original_value ||
            (afw_value_is_object(modified_value) &&
            !afw_value_is_object(original_value)))
        {
            impl_add_reconcile_property(wa,
                embedding_pt, embedding_property_name,
                pt, property_name,
                property_names, modified_value);
        }

        /* If there is a corresponding original value, set if different. */
        else  {

            /* Handled case where original and modified are both objects */
            if (afw_value_is_object(modified_value)) {
                new_property_names =
                    afw_array_create_or_clone(
                        property_names, afw_data_type_string, false,
                        wa->p, wa->xctx);
                value = afw_value_create_unmanaged_string(property_name,
                    wa->p, wa->xctx);
                afw_array_add_value(new_property_names, value, wa->xctx);

                /* Try getting object type id from pt. */
                object_type_id = pt->data_type_parameter;

                /*
                 * If object id is not in pt, try to get object type id from
                 * modified object.
                 */
                if (!object_type_id) {
                    object = ((const afw_value_object_t *)
                        modified_value)->internal;
                    object_type_id = afw_object_meta_get_object_type_id(object, xctx);
                }
                if (!object_type_id && object->meta.meta_object) {
                    object_type_id = afw_object_meta_get_object_type_id(
                        object, xctx);
                }

                /*
                 * If object type id still not determined, try getting it from
                 * original object.
                 */
                if (!object_type_id) {
                    object = ((const afw_value_object_t *)
                        original_value)->internal;
                    object_type_id = afw_object_meta_get_object_type_id(object, xctx);
                }
                if (!object_type_id && object->meta.meta_object) {
                    object_type_id = afw_object_meta_get_object_type_id(
                        object, xctx);
                }

                /* Get the property's object type.  Error if not available. */
                property_object_type = NULL;
                if (object_type_id) {
                    property_object_type = afw_adaptor_get_object_type(
                        object_type->adaptor_id, object_type_id,
                        journal_entry,
                        wa->xctx);
                }
                if (!property_object_type) {
                    AFW_THROW_ERROR_Z(general,
                        "object type needed to reconcile embedded object",
                        wa->xctx);
                }

                /* Reconcile the two objects. */
                impl_reconcile_object(wa,
                    pt, property_name,
                    property_object_type,
                    new_property_names,
                    ((const afw_value_object_t *)original_value)
                    ->internal,
                    ((const afw_value_object_t *)modified_value)
                    ->internal,
                    journal_entry);
            }

            else {
                original_value = afw_object_type_property_type_normalize(pt,
                    original_value, wa->p, wa->xctx);
                if (!afw_value_equal(modified_value, original_value, wa->xctx))
                {
                    impl_add_reconcile_property(wa,
                        embedding_pt, embedding_property_name,
                        pt, property_name,
                        property_names, modified_value);
                }
            }
        }
    }

    /* Run though original producing remove_property if not in modified. */
    for (iterator = NULL;;) {
        original_value = afw_object_get_next_property(original,
            &iterator, &property_name, wa->xctx);
        if (!original_value) break;

        if (!afw_object_has_property(modified, property_name, wa->xctx))
        {
            pt = afw_object_type_property_type_get(object_type,
                property_name, wa->xctx);
            impl_add_reconcile_property(wa,
                embedding_pt, embedding_property_name,
                pt, property_name,
                property_names, NULL);
        }
    }
}


/* Produce modify entries needed to reconcile two objects. */
AFW_DEFINE(void)
afw_adaptor_modify_needed_to_reconcile(
    const afw_utf8_t * *adaptor_id,
    const afw_utf8_t * *object_type_id,
    const afw_utf8_t * *object_id,
    const afw_array_t * *entries,
    const afw_object_t *original,
    const afw_object_t *modified,
    const afw_object_t *journal_entry,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_reconcile_wa_t wa;
    const afw_utf8_t *original_path;
    const afw_utf8_t *path;
    const afw_object_type_t *object_type;
    const afw_object_path_parsed_t *parsed_path;

    original_path = afw_object_meta_get_path(original, xctx);
    path = afw_object_meta_get_path(modified, xctx);

    if (!original_path || !path || !afw_utf8_equal(original_path, path)) 
    {
        AFW_THROW_ERROR_Z(general,
            "reconcile expects original and modified "
            "paths to match",
            xctx);
    }

    /* Parse path and make sure it is an entity. */
    parsed_path = afw_object_path_parse(path, NULL, NULL, p, xctx);
    if (!parsed_path || parsed_path->first_property_name) {
        AFW_THROW_ERROR_Z(general,
            "reconcile_object() expects path to be an entity",
            xctx);
    }
    *adaptor_id = &parsed_path->adaptor_id;
    *object_type_id = &parsed_path->object_type_id;
    *object_id = &parsed_path->entity_object_id;

    object_type = afw_adaptor_get_object_type(*adaptor_id, *object_type_id,
        journal_entry, xctx);
    if (!object_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Object type " AFW_UTF8_FMT_Q
            " does not exist in Adaptor " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(*object_type_id),
            AFW_UTF8_FMT_ARG(*adaptor_id));
    }

    wa.entries = afw_array_of_create(afw_data_type_array, p, xctx);
    wa.p = p;
    wa.xctx = xctx;

    impl_reconcile_object(&wa, NULL, NULL, object_type, NULL,
        original, modified, journal_entry);

    *entries = wa.entries;
}



/* Patch object and remove from cache. */
AFW_DEFINE(void)
afw_adaptor_modify_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_array_t *entries,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adaptor_session_t *session;
    const afw_adaptor_modify_entry_t * const *entry;
    const afw_object_t *request;
    afw_adaptor_impl_request_t impl_request;

    /* Set request in journal entry. */
    afw_memory_clear(&impl_request);
    impl_request.request = request = afw_object_create_embedded(
        journal_entry, afw_s_request, xctx);
    impl_request.p = request->p;
    impl_request.journal_entry = journal_entry;
    impl_request.resource_id = afw_utf8_printf(impl_request.p, xctx,
        "/"
        AFW_UTF8_FMT "/"
        AFW_UTF8_FMT "/"
        AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(adaptor_id),
        AFW_UTF8_FMT_ARG(object_type_id),
        AFW_UTF8_FMT_ARG(object_id));
    afw_object_set_property_as_string(request,
        afw_s_resourceId, impl_request.resource_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_function, afw_s_modify_object, xctx);
    afw_object_set_property_as_string(request,
        afw_s_adaptorId, adaptor_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectType, object_type_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectId, object_id, xctx);
    afw_object_set_property_as_array(request,
        afw_s_entries, entries, xctx);

    /* Parse entries. */
    entry = afw_adaptor_modify_entries_from_list(entries, xctx->p, xctx);

    /* Get an active session with adaptor. */
    session = afw_adaptor_session_get_cached(adaptor_id, true, xctx);

    /* Action prologue. */
    afw_adaptor_internal_journal_prologue(session, journal_entry, xctx);

    /* If one of the core object types, handle special */
    if (afw_utf8_starts_with(object_type_id, AFW_OBJECT_S_CORE_ID_PREFIX))
    {

        /* Journal entry. */
        if (afw_utf8_equal(object_type_id,
            AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
        {
            AFW_THROW_ERROR_Z(general,
                "modify_object() is not supported for "
                AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY,
                xctx);
        }
    }

    /* Modify object. */
    afw_adaptor_session_modify_object(session, &impl_request,
        object_type_id, object_id, entry,
        adaptor_type_specific, xctx);

    /* Action epilogue. */
    afw_adaptor_internal_journal_epilogue(session, journal_entry, true, xctx);
}



/* Modify using update object and remove from cache. */
AFW_DEFINE(void)
afw_adaptor_modify_using_update_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *update_object,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *value;
    const afw_array_t *entries;
    const afw_array_t *entry;
    const afw_value_t *entry_value;
    const afw_value_t *property_name_value;


    /** @fixme move this out of update
       
        Journal entry.

    if (afw_utf8_equal(object_type_id,
        AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
    {
        afw_adaptor_journal_entry_consume(session, object_id, update_object,
            xctx);
        return;
    }
     */

    /* Convert update_object to list of modify entries. */
    entries = afw_array_of_create(afw_data_type_array, xctx->p, xctx);
    iterator = NULL;
    while ((value = afw_object_get_next_property(update_object,
        &iterator, &property_name, xctx)))
    {
        /* Add ["set_property", <property name>, value]. */
        entry = afw_array_create_generic(xctx->p, xctx);
        afw_array_add_value(entry,
            &impl_value_set_property.pub,
            xctx);
        property_name_value = afw_value_create_unmanaged_string(
            property_name, xctx->p, xctx);
        afw_array_add_value(entry, property_name_value, xctx);
        afw_array_add_value(entry, value, xctx);
        entry_value = afw_value_create_unmanaged_array(entry, xctx->p, xctx);
        afw_array_add_value(entries, entry_value, xctx);
    }

    /* Modify object. */
    afw_adaptor_modify_object(
        adaptor_id, object_type_id, object_id,
        entries, journal_entry, adaptor_type_specific, xctx);
}

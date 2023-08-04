// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Register Environment Configuration
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_environment_configuration.c
 * @brief AFW environment configuration.
 */

#include "afw_internal.h"



/* Configure environment with a configuration entry. */
AFW_DEFINE(void)
afw_environment_configure_with_object(
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *type;
    const afw_environment_conf_type_t *conf_type;
    const afw_pool_t *p;

    /* Create new multithreaded pool */
    p = afw_pool_create_multithreaded(xctx->env->p, xctx);

    /* Clone conf and source_location to new pool. */
    conf = afw_object_create_clone(conf, p, xctx);
    source_location = afw_utf8_clone(source_location, p, xctx);

    /* Added sourceLocation property to conf. */
    afw_object_set_property_as_string(conf,
        &afw_s_sourceLocation, source_location, xctx);

    /* Get type property. */
    type = afw_object_old_get_property_as_utf8(conf, &afw_s_type, p, xctx);
    if (!type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "missing type property",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* Get conf_type and call create function. */
    conf_type = afw_environment_get_conf_type(type, xctx);
    if (!conf_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "invalid type " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(type));
    }
    conf_type->create(type, conf, source_location, p, xctx);
}


/* Configure environment with list of configuration entries. */
AFW_DEFINE(void)
afw_environment_configure_with_object_list(
    const afw_list_t *entry_list,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_object_t *entry;
    const afw_utf8_t *detail_source_location;
    int count;

    /* Default source_location to "Configuration". */
    if (!source_location) {
        source_location = &afw_s_Configuration;
    }

    /* Process each configuration entry. */   
    for (iterator = NULL, count = 1;; count++) {

        /* Get next configuration entry. */
        value = afw_list_get_next_value(entry_list, &iterator,
            xctx->p, xctx);
        if (!value) break;

        /* Source location will include entry number. */
        detail_source_location = afw_utf8_printf(xctx->env->p, xctx,
            AFW_UTF8_FMT " entry %d",
            AFW_UTF8_FMT_ARG(source_location), count);

        if (!afw_value_is_object(value)) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "is not an object",
                AFW_UTF8_FMT_ARG(detail_source_location));
        }
        entry = ((const afw_value_object_t *)value)->internal;

        /* Configure using entry. */
        afw_environment_configure_with_object(entry, detail_source_location,
            xctx);
    };
}


void afw_environment_internal_extension_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *extension_id;
    const afw_utf8_t *module_path;
    const afw_object_t* manifest;

    extension_id = afw_object_old_get_property_as_utf8(entry,
        &afw_s_extensionId, p, xctx);

    if (!extension_id) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "\"extension_id\" required.",
            AFW_UTF8_FMT_ARG(source_location));
    }

    module_path = afw_object_old_get_property_as_utf8(entry,
        &afw_s_modulePath, p, xctx);

    /* If module_path is not supplied, see if it is registered. */
    if (!module_path) {
        manifest = afw_runtime_get_object(&afw_s__AdaptiveManifest_,
            extension_id, xctx);
        if (manifest) {
            module_path = afw_object_old_get_property_as_string(manifest,
                &afw_s_modulePath, xctx);
        }
    }
    if (!module_path) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "\"modulePath\" needed for \"extension\" " AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(source_location), 
            AFW_UTF8_FMT_ARG(extension_id));
    }

    afw_environment_load_extension(extension_id, module_path,
        entry, xctx);
}


/* Prepare properties for a conf type. */
AFW_DEFINE(const afw_object_t *)
afw_environment_prepare_conf_type_properties(
    const afw_object_t *properties,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    const afw_object_t *result;
    const afw_utf8_t *type;
    const afw_utf8_t *subtype;
    const afw_utf8_t *id;
    const afw_utf8_t *source_location;
    const afw_environment_conf_type_t *conf_type;
    const afw_utf8_t *path;

    /* Use properties p. */
    p = properties->p;

    /* Get sourceLocation.  Default for now to empty string. */
    source_location = afw_object_old_get_property_as_string(
        properties, &afw_s_sourceLocation, xctx);
    if (!source_location) {
        source_location = &afw_s_a_empty_string;
    }

    /* Get type. */
    type = afw_object_old_get_property_as_string(properties,
        &afw_s_type, xctx);
    if (!type || type->len == 0) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            " missing type",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* Get conf_type for type. */
    conf_type = afw_environment_get_conf_type(type, xctx);
    if (!conf_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "type " AFW_UTF8_FMT_Q
            " is not valid",
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(type));
    }

    /* If appropriate, get subtype. */
    subtype = NULL;
    if (conf_type->subtype_property_name) {
        subtype = afw_object_old_get_property_as_string(
            properties, conf_type->subtype_property_name, xctx);
        if (!subtype || subtype->len == 0) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                AFW_UTF8_FMT_Q
                " property required for conf type " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(conf_type->subtype_property_name),
                AFW_UTF8_FMT_ARG(type));
        }
    }

    /* If appropriate, get id.  If not present, default to subtype. */
    id = &afw_s_current;
    if (conf_type->id_property_name) {
        id = afw_object_old_get_property_as_string(
            properties, conf_type->id_property_name, xctx);
        if (!id || id->len == 0) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                " " AFW_UTF8_FMT_Q
                " property required for conf type " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(conf_type->id_property_name),
                AFW_UTF8_FMT_ARG(type));
        }
    }

    /* Construct path. */
    path = afw_utf8_printf(p, xctx,
        "/afw/_AdaptiveConf_" AFW_UTF8_FMT
        "%s" AFW_UTF8_FMT
        "/" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(type),
        (const char *)((subtype) ? "_" : ""),
        (int)((subtype) ? (int)subtype->len : 0),
        (const char *)((subtype) ? (const char *)subtype->s : ""),
        AFW_UTF8_FMT_ARG(id));


    /* If defaulting source location, make it path. */
    if (source_location == &afw_s_a_empty_string) {
        source_location = path;
        afw_object_set_property_as_string(properties,
            &afw_s_sourceLocation, source_location, xctx);
    }

    /* If not adaptor for afw (which will cause loop), normalize result. */
    result = properties;
    if (!afw_utf8_equal(type, &afw_s_adaptor) ||
        !afw_utf8_equal(id, &afw_s_afw))
    {
        result = afw_object_view_create(properties,
            path, &afw_object_options_composite_normalize_defaults_required,
            p, xctx);
        result = afw_object_create_clone(result, p, xctx);
        if (afw_object_meta_has_errors(result, xctx)) {
            afw_object_meta_log_errors(result, source_location, xctx);
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                " configuration error(s) logged",
                AFW_UTF8_FMT_ARG(source_location));
        }
    }
    

    /* Return result. */
    return result;
}

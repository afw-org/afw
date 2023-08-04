// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW YAML Miscellaneous Functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_yaml.c
 * @brief YAML Miscellaneous Functions.
 */

#include "afw.h"
#include "afw_yaml.h"
#include "afw_content_type_impl.h"
#include "generated/afw_yaml_version_info.h"


/* Declares and rti/inf defines for interface afw_extension */
#define AFW_IMPLEMENTATION_ID "afw_yaml"
#define AFW_IMPLEMENTATION_VERSION AFW_YAML_VERSION_STRING
#include "afw_extension_impl_declares.h"
#include "afw_content_type_impl_declares.h"


/* Media types supported by this afw_content_type implementation. */
static const afw_utf8_t impl_media_types[] = {
    AFW_UTF8_LITERAL(AFW_YAML_Q_CONTENT_TYPE)
};

/* Raw begin object list. */
static const afw_memory_t impl_raw_begin_object_list = {
    (const afw_byte_t *)"[\n",
    sizeof("[\n") - 1
};

/* Raw object separator. */
static const afw_memory_t impl_raw_object_separator = {
    (const afw_byte_t *)",\n",
    sizeof(",\n") - 1
};

/* Raw last object separator. */
static const afw_memory_t impl_raw_last_object_separator = {
    (const afw_byte_t *)"\n",
    sizeof("\n") - 1
};

/* Raw end object list. */
static const afw_memory_t impl_raw_end_object_list = {
    (const afw_byte_t *)"]\n",
    sizeof("[\n") - 1
};

/* Content type singleton instance for this implementation. */
static const afw_content_type_t impl_afw_content_type =
{
    &impl_afw_content_type_inf,
    AFW_UTF8_LITERAL("yaml"),
    sizeof(impl_media_types) / sizeof(afw_utf8_t),
    &impl_media_types[0]
};


/* Convert a value to yaml and write it. Defined in afw_yaml_from_value. */
extern void afw_yaml_internal_write_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Get the content type instance for FIXME.
 *
 * Call this from extension impl_initialize() function
 * as parameter to afw_content_type_register() function.
 *
 * afw_content_type_register(afw_yaml_content_type_get(), xctx);
 */
const afw_content_type_t *
afw_yaml_content_type_get()
{
    return &impl_afw_content_type;
}



/* Register xml support. */
void afw_yaml_register(afw_xctx_t *xctx)
{
    afw_content_type_register(afw_yaml_content_type_get(), xctx);
}


/* Define extension implementation. */
AFW_ENVIRONMENT_DEFINE_EXTENSION_IMPL();


/*
 * Implementation of method initialize for interface afw_extension.
 */
const afw_extension_t *
impl_afw_extension_initialize(
    const afw_extension_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{   
    /*
     *  IMPORTANT:
     *
     *  Make sure associated _AdaptiveManifest_ is up to date,
     *  especially the "registers" property.
     *
     */
    
    /* Call the register function for this extension. */
    afw_yaml_register(xctx);

    /* Return extension instance. */
    return &impl_extension;
}


/*
 * Implementation of method release of interface afw_extension.
 */
void
impl_afw_extension_release(
    const afw_extension_t * instance,
    afw_xctx_t *xctx)
{
}

static const afw_utf8_z_t * impl_u8z_to_yaml(
    const afw_utf8_z_t *s, afw_xctx_t *xctx)
{
    const afw_utf8_z_t *c;
    apr_array_header_t *a;
    char *u;

    if (!s) return "null";

    a = apr_array_make(afw_pool_get_apr_pool(xctx->p), 128, sizeof(char));

    /* Add characters to array with proper escaping. */
    for (c = s; *c; c++) {

        /* If control character, use \u00xx escape. */
        if (*c < 32) {
            u = apr_psprintf(afw_pool_get_apr_pool(xctx->p), "\\u%02x", *c);
            while (*u) {
                APR_ARRAY_PUSH(a, char) = *u;
                u++;
            }
        }

        /*
         * If not control char, add char to array.
         */
        else {
            APR_ARRAY_PUSH(a, char) = *c;
        }

    }

    /* Add starting quote and zero terminate. */
    APR_ARRAY_PUSH(a, char) = 0;

    return (const afw_utf8_z_t *)a->elts;
}

const afw_utf8_t * afw_yaml_from_error(afw_xctx_t *xctx)
{
    afw_error_t *error = xctx->error;

    return afw_utf8_printf(xctx->p, xctx,
        "\"status\": \"error\",\n"
        "\"errorCode\": %d,\n"
        "\"id\": %s,\n"
        "\"sourceFile\": %s,\n"
        "\"function\": %s,\n"
        "\"lineNumber\": %d,\n"
        "\"rvSourceId\": %s,\n"
        "\"rv\": %d,\n"
        "\"rvDecoded\": %s,\n"
        "\"message\": %s\n",
        error->code,
        afw_error_code_id_z(error),
        impl_u8z_to_yaml(afw_error_source_file(error), xctx),
        error->source_z,
        impl_u8z_to_yaml(error->rv_source_id_z, xctx),
        error->rv,
        impl_u8z_to_yaml(error->rv_decoded_z, xctx),
        impl_u8z_to_yaml(error->message_z, xctx)
        );
}



/*
 * Implementation of method to_value_p of interface afw_content_type.
 */
const afw_value_t *
impl_afw_content_type_raw_to_value(
    const afw_content_type_t * instance,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_pool_t * p,
    afw_xctx_t *xctx) 
{
    return afw_yaml_to_value(raw, NULL, p, xctx);
}



/*
 * Implementation of method raw_to_object of interface afw_content_type.
 */
const afw_object_t *
impl_afw_content_type_raw_to_object (
    const afw_content_type_t * instance,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adaptor_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_yaml_to_object(raw, source_location,
        adaptor_id, object_type_id, object_id, cede_p, p, xctx);
}



/*
 * Implementation of method write_value of interface afw_content_type.
 */
void
impl_afw_content_type_write_value(
    const afw_content_type_t * instance,
    const afw_value_t * value,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_yaml_internal_write_value(value, options, context, callback,
        p, xctx);
}



/*
 * Implementation of method create_object_list_writer of interface afw_content_type.
 */
const afw_content_type_object_list_writer_t *
impl_afw_content_type_create_object_list_writer(
    const afw_content_type_t * instance,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_content_type_impl_create_object_list_writer(
        instance, options, context, callback,
        &impl_raw_begin_object_list,
        &impl_raw_object_separator,
        &impl_raw_last_object_separator,
        &impl_raw_end_object_list,
        p, xctx);
}

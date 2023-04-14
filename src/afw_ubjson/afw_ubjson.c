// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_content_type interface for Universal Binary JSON
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_ubjson.c
 * @brief Implementation of afw_content_type interface for Universal Binary JSON
 */

#include "afw.h"
#include "afw_content_type_impl.h"
#include "generated/afw_ubjson_generated.h"
#include "generated/afw_ubjson_version_info.h"


#include "afw_ubjson.h"

/* Declares and rti/inf defines for interface afw_content_type */
#define AFW_IMPLEMENTATION_ID "afw_ubjson"
#define AFW_IMPLEMENTATION_VERSION AFW_UBJSON_VERSION_STRING
#include "afw_extension_impl_declares.h"
#include "afw_content_type_impl_declares.h"

/* Media types supported by this afw_content_type implementation. */
static const afw_utf8_t impl_media_types[] = {
    AFW_UTF8_LITERAL(AFW_UBJSON_Q_CONTENT_TYPE)
};


/** @fixme Make these correct. */

/* Raw begin object list. */
static const afw_memory_t impl_raw_begin_object_list = {
    (const afw_byte_t *)"[",
    sizeof("[") - 1
};

/* Raw end object list. */
static const afw_memory_t impl_raw_end_object_list = {
    (const afw_byte_t *)"]",
    sizeof("[") - 1
};

/* This is referenced by generate/registers/content_type/ubjson.c. */
const afw_content_type_t
afw_ubjson_internal_content_type =
{
    &impl_afw_content_type_inf,
    AFW_UTF8_LITERAL("ubjson"),
    sizeof(impl_media_types) / sizeof(afw_utf8_t),
    &impl_media_types[0]
};


/* Convert a value to json and write it defined in afw_ubjson_from_value.c */
extern void afw_ubjson_internal_write_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p, afw_xctx_t *xctx);


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
   
    /* See function in generated/afw_ubjson_generated.c. */
    afw_ubjson_generated_register(xctx);

    /* Return extension info. */
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
    /* Extension release() is not currently called. */
}


/*
 * Implementation of method to_value of interface afw_content_type.
 */
const afw_value_t *
impl_afw_content_type_raw_to_value(
    const afw_content_type_t * instance,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_pool_t * p,
    afw_xctx_t *xctx) 
{
    return afw_ubjson_to_value(raw, NULL, p, xctx);
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
    return afw_ubjson_to_object(raw, source_location,
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
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_ubjson_internal_write_value(value, options, context, callback,
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
        NULL,
        NULL,
        &impl_raw_end_object_list,
        p, xctx);
}

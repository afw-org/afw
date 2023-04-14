// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW JSON Miscellaneous Functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_json.c
 * @brief JSON Miscellaneous Functions.
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_content_type */
#define AFW_IMPLEMENTATION_ID AFW_JSON_Q_CONTENT_TYPE_SHORT
#include "afw_content_type_impl_declares.h"


/* Media types supported by this afw_content_type implementation. */
static const afw_utf8_t impl_media_types[] = {
    AFW_UTF8_LITERAL(AFW_JSON_Q_CONTENT_TYPE),
    AFW_UTF8_LITERAL("*") /* Let JSON handle *. */
};

/* Raw begin object list. */
static const afw_memory_t impl_raw_begin_object_list = {
    (const afw_octet_t *)"[\n",
    sizeof("[\n")-1
};

/* Raw object separator. */
static const afw_memory_t impl_raw_object_separator = {
    (const afw_octet_t *)",\n",
    sizeof(",\n") - 1
};

/* Raw last object separator. */
static const afw_memory_t impl_raw_last_object_separator = {
    (const afw_octet_t *)"\n",
    sizeof("\n") - 1
};

/* Raw end object list. */
static const afw_memory_t impl_raw_end_object_list = {
    (const afw_octet_t *)"]\n",
    sizeof("]\n") - 1
};

/* Content type singleton instance for this implementation. */
static const afw_content_type_t impl_afw_content_type =
{
    &impl_afw_content_type_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    sizeof(impl_media_types) / sizeof(afw_utf8_t),
    &impl_media_types[0]
};


/* Function to return the content type singleton. */
AFW_DEFINE(const afw_content_type_t *)
afw_json_content_type_get()
{
    return &impl_afw_content_type;
}


/* Register json support. */
AFW_DEFINE(void) afw_json_register(afw_xctx_t *xctx)
{
    afw_content_type_register(afw_json_content_type_get(), xctx);
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
    return afw_json_to_value(afw_utf8_from_raw(raw, p, xctx),
        source_location, p, xctx);
}



/*
 * Implementation of method raw_to_object of interface afw_content_type.
 */
const afw_object_t *
impl_afw_content_type_raw_to_object(
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
    const afw_pool_t *use_p;
    const afw_utf8_t *string;

    /* If not cede_p, get subpool now to use for afw_utf8_from_raw(). */
    use_p = p;
    if (!cede_p) {
        use_p = afw_pool_create(p, xctx);
    }

    /* Convert raw to utf8. An error may be thrown if not valid utf-8. */
    string = afw_utf8_from_raw(raw, use_p, xctx);

    /*
     * Call afw_compile_to_object() to convert json to object.  Always cede_p
     * since subpool has already been created.  Return result.
     */
    return afw_json_to_object_with_meta(string, source_location,
        adaptor_id, object_type_id, object_id, true, p, xctx);
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
    afw_json_internal_write_value(value, options, context, callback,
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

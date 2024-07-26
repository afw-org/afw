<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_content_type for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_content_type */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_CONTENT_TYPE_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_content_type_impl_declares.h"

      
/* Media types supported by this afw_content_type implementation. */
/** @todo Put one or more media type AFW_UTF8_LITERALS here. */
static const afw_utf8_t
impl_media_types[] = {
    AFW_UTF8_LITERAL("application/type1"),
    AFW_UTF8_LITERAL("application/type2")
};

/* Content type singleton instance for this implementation. */
static const afw_content_type_t
impl_afw_content_type =
{
    &impl_afw_content_type_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    sizeof(impl_media_types) / sizeof(afw_utf8_t),
    &impl_media_types[0]
};


/*!
 *  \brief Get the content type instance for <afwdev {implementation_id}>.
 *
 *  Call this from extension initialize() method as parameter to
 *  afw_content_type_register() function.
 * 
 *  afw_content_type_register(..._content_type_get(), xctx);
 */
<afwdev {srcdir_prefix.upper()}>DEFINE(const afw_content_type_t *)
<afwdev {prefixed_interface_name}>_get()
{
    return &impl_afw_content_type;
}

    
/*
 * Implementation of method raw_to_value for interface afw_content_type.
 */
const afw_value_t *
impl_afw_content_type_raw_to_value(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method raw_to_object for interface afw_content_type.
 */
const afw_object_t *
impl_afw_content_type_raw_to_object(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adapter_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write_value for interface afw_content_type.
 */
void
impl_afw_content_type_write_value(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_value_t * value,
    const afw_object_options_t * options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method create_object_list_writer for interface
 * afw_content_type.
 */
const afw_content_type_object_list_writer_t *
impl_afw_content_type_create_object_list_writer(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_object_options_t * options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{

        
    /* Raw begin object list used by object_list_writer. */
    /** @todo Replace "[\n" with appropriate value or remove here and use NULL in call. */
    static const afw_memory_t
    impl_raw_begin_object_list = {
        (const afw_octet_t *)"[\n",
        sizeof("[\n") - 1 /* "- 1" for string. */
    };

    /* Raw object separator used by object_list_writer. */
    /** @todo Replace ",\n" with appropriate value or remove here and use NULL in call. */
    static const afw_memory_t
    impl_raw_object_separator = {
        (const afw_octet_t *)",\n",
        sizeof(",\n") - 1 /* "- 1" for string. */
    };

    /* Raw last object separator used by object_list_writer. */
    /** @todo Replace "\n" with appropriate value or remove here and use NULL in call. */
    static const afw_memory_t
    impl_raw_last_object_separator = {
        (const afw_octet_t *)"\n",
        sizeof("\n") - 1  /* "- 1" for string. */
    };

    /* Raw end object list used by object_list_writer. */
    /** @todo Replace "]\n" with appropriate value or remove here and use NULL in call. */
    static const afw_memory_t
    impl_raw_end_object_list = {
        (const afw_octet_t *)"]\n",
        sizeof("]\n") - 1 /* "- 1" for string. */
    };

    /* Create object_list_writer instance. */
    return afw_content_type_impl_create_object_list_writer(
        &self->pub, options, context, callback,
        &impl_raw_begin_object_list,
        &impl_raw_object_separator,
        &impl_raw_last_object_separator,
        &impl_raw_end_object_list,
        p, xctx);
      
}

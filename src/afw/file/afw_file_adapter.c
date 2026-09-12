// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework file support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_file_adapter.c
 * @brief File adapter (objects stored as files).
 */

#include "afw_internal.h"
#include <string.h>


/* Declares and rti/inf defines for interface afw_adapter_factory */
#define AFW_IMPLEMENTATION_ID "file"
#include "afw_adapter_factory_impl_declares.h"
#define AFW_ADAPTER_SELF_T afw_file_internal_adapter_t
#include "afw_adapter_impl_declares.h"
#define AFW_ADAPTER_SESSION_SELF_T afw_file_internal_adapter_session_t
#include "afw_adapter_session_impl_declares.h"


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Adapter type for accessing objects contained in files.");

static const afw_utf8_t impl_s_journal_dir = AFW_UTF8_LITERAL(
    AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY "/");
static const afw_utf8_t impl_s_journal_lock = AFW_UTF8_LITERAL(
    AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY "/journal_lock");

/* File adapter factory instance. */
static const afw_adapter_factory_t impl_adapter_factory =
{
    &impl_afw_adapter_factory_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    &impl_factory_description
};


/* Get the factory for file adapter. */
AFW_DEFINE(const afw_adapter_factory_t *)
afw_file_adapter_factory_get()
{
    return &impl_adapter_factory;
}


/*
 * Implementation of method create_adapter_cede_p of interface afw_adapter_factory.
 */
const afw_adapter_t *
impl_afw_adapter_factory_create_adapter_cede_p (
    const afw_adapter_factory_t * self,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Create file adapter. */
    return afw_file_adapter_create_cede_p(properties, p, xctx);
}



/* Create function for file adapter. */
AFW_DEFINE(const afw_adapter_t *)
afw_file_adapter_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *self;
    afw_adapter_t *adapter;
    const afw_utf8_t *content_type;
    const afw_value_t *value;
    afw_boolean_t b;
    afw_boolean_t found;

    /* Create adapter and process common properties.  */
    adapter = afw_adapter_impl_create_cede_p(
        &impl_afw_adapter_inf,
        sizeof(afw_file_internal_adapter_t),
        properties, p, xctx);
    self = (afw_file_internal_adapter_t *)adapter;
    p = self->pub.p;

    /* Get content_type parameters. */
    content_type = afw_object_get_property_convert_to_utf8(properties,
        afw_v_contentType, p, xctx);
    self->content_type = afw_environment_get_content_type(content_type,
        xctx);
    if (!self->content_type)
    {
        afw_adapter_impl_throw_property_invalid(adapter,
            afw_v_contentType, xctx);
    }

    /* Get optional filename extension */
    self->filename_suffix = afw_object_get_property_convert_to_utf8(
        properties, afw_v_filenameSuffix, p, xctx);
    if (!self->filename_suffix) {
        self->filename_suffix = afw_s_a_empty_string;
    }

    /* Get root from parameters and make it full path. */
    value = afw_object_get_property_compile_and_evaluate_using(
        properties,  afw_v_root, adapter->source_location,
        afw_compile_type_template, p, xctx);
    if (!afw_value_is_string(value)) {
        afw_adapter_impl_throw_property_invalid(adapter,
            afw_v_root, xctx);
    }
    self->root = afw_file_insure_full_path(
        &((afw_value_string_t *)value)->internal,
        p, xctx);

    /* Make path for journal directory. */
    self->journal_dir_path_z = afw_utf8_to_utf8_z(
        afw_utf8_concat(p, xctx, self->root, &impl_s_journal_dir, NULL),
        p, xctx);

    /* Make path to journal lock file. */
    self->journal_lock_file_path_z = afw_utf8_to_utf8_z(
        afw_utf8_concat(p, xctx, self->root, &impl_s_journal_lock, NULL),
        p, xctx);

    self->journal_rw_lock = afw_lock_create_rw_and_register(
        afw_s_a_lock_file_journal_anchor,
        afw_s_a_lock_file_journal_anchor_brief,
        afw_s_a_lock_file_journal_anchor_description,
        xctx
    );

    /* If isDevelopmentInput is true, provide appropriate object types. */
    b = afw_object_get_property_as_boolean_internal(properties,
        afw_v_isDevelopmentInput, &found, xctx);
    if (b) {
        afw_adapter_impl_set_supported_core_object_type(adapter,
            afw_s__AdaptiveCollection_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(adapter,
            afw_s__AdaptiveDataTypeGenerate_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(adapter,
            afw_s__AdaptiveManifest_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(adapter,
            afw_s__AdaptiveFunctionGenerate_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(adapter,
            afw_s__AdaptiveObjectType_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(adapter,
            afw_s__AdaptiveValueMeta_, true, true, xctx);
    }

    /* Return adapter. */
    return adapter;
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
    afw_file_internal_adapter_session_t *session;

    session = afw_xctx_calloc_type(afw_file_internal_adapter_session_t, xctx);
    session->pub.inf = &impl_afw_adapter_session_inf;
    session->pub.adapter = (afw_adapter_t *)self;
    session->pub.p = xctx->p;
    session->adapter = self;

    /* Adapter session &self->pub holds event journal &self->pub. */
    session->journal.inf = afw_file_internal_get_journal_inf();
    session->journal.session = (afw_adapter_session_t *)session;

    /* Return session. */
    return (const afw_adapter_session_t *)session;
}



/*
 * Implementation of method get_additional_metrics of interface afw_adapter.
 */
const afw_object_t *
impl_afw_adapter_get_additional_metrics(
    AFW_ADAPTER_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* There are no adapter specific metrics. */
    return NULL;
}


/* Helper to get full path. */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_t *)
impl_get_full_path(
    afw_file_internal_adapter_t *adapter,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_utf8_concat(p, xctx,
        adapter->root,
        object_type_id,
        afw_s_a_slash,
        object_id,
        adapter->filename_suffix
            ? adapter->filename_suffix : afw_s_a_empty_string,
        NULL);
}


/*
 * Implementation of method destroy of interface afw_adapter_session.
 */
void
impl_afw_adapter_session_destroy(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}


/*
 * Implementation of method retrieve_objects for interface
 * afw_adapter_session.
 */
void
impl_afw_adapter_session_retrieve_objects(
    AFW_ADAPTER_SESSION_SELF_T *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t *criteria,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adapter_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *adapter = (afw_file_internal_adapter_t *)self->adapter;
    const char *dirname_z;
    const afw_utf8_t *full_path;
    afw_file_dir_t *dir;
    afw_file_dirent_t ent;
    const afw_pool_t *obj_p;
    const afw_object_t *obj;
    const afw_memory_t *raw;
    const afw_utf8_t *object_id;
    afw_size_t len;
    afw_boolean_t stop;

    /* Open ObjectType's directory. Concat .len, then C-string door. */
    dirname_z = afw_utf8_to_utf8_z(
        afw_utf8_concat(p, xctx,
            adapter->root, object_type_id, afw_s_a_slash, NULL),
        p, xctx);
    dir = afw_file_dir_open(dirname_z, p, xctx);

    /* If not found, return no objects. */
    if (!dir) {
        callback(NULL, context, xctx);
        return;
    }

    stop = false;
    AFW_TRY {

        /* Process each JSON object in directory. */
        while (!stop && afw_file_dir_read(dir, &ent, xctx)) {

            AFW_XCTX_THROW_IF_TERMINATING(xctx);

            /* Skip ., .., and hidden files. */
            if (ent.name_z[0] == '.') {
                continue;
            }
            if (ent.type != afw_file_type_file) {
                continue;
            }

            len = strlen(ent.name_z);
            if (adapter->filename_suffix) {
                if (len <= adapter->filename_suffix->len ||
                    memcmp(ent.name_z +
                        (len - adapter->filename_suffix->len),
                        adapter->filename_suffix->s,
                        adapter->filename_suffix->len) != 0)
                {
                    continue;
                }
                len -= adapter->filename_suffix->len;
            }

            obj_p = afw_pool_create(p, xctx);
            object_id = afw_utf8_create(ent.name_z, len, obj_p, xctx);
            full_path = afw_utf8_concat(obj_p, xctx,
                adapter->root,
                object_type_id,
                afw_s_a_slash,
                object_id,
                adapter->filename_suffix
                    ? adapter->filename_suffix : afw_s_a_empty_string,
                NULL);

            raw = afw_file_to_memory(full_path, 0, obj_p, xctx);
            obj = afw_content_type_raw_to_object(
                adapter->content_type, raw, full_path,
                &adapter->pub.adapter_id,
                object_type_id, object_id, true, obj_p, xctx);

            if (afw_query_criteria_test_object(obj, criteria, p, xctx)) {
                if (callback(obj, context, xctx)) {
                    stop = true;
                }
            }
            else {
                afw_object_release(obj, xctx);
            }
        }
    }
    AFW_FINALLY {
        afw_file_dir_close(dir, xctx);
    }
    AFW_ENDTRY;

    callback(NULL, context, xctx);
}


/*
 * Implementation of method get_object for interface afw_adapter_session.
 */
void
impl_afw_adapter_session_get_object(
    AFW_ADAPTER_SESSION_SELF_T *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adapter_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *adapter = (afw_file_internal_adapter_t *)self->adapter;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;
    const afw_object_t *object;
    const afw_pool_t *obj_p;

    /* Create pool for object and related memory. */
    obj_p = afw_pool_create(p, xctx);

    /* Determine full path. */
    full_path = impl_get_full_path(adapter, object_type_id, object_id,
        obj_p, xctx);

    /*
     * Load file to memory and convert to object &self->pub.  Ceed control
     * of obj_p to object.
     */
    raw = afw_file_to_memory(full_path, 0, obj_p, xctx);
    object = afw_content_type_raw_to_object(
        adapter->content_type, raw, full_path,
        &adapter->pub.adapter_id, object_type_id, object_id,
        true, obj_p, xctx);

    /* Pass object to callback.  Callback will release object. */
    callback(object, context, xctx);
}


/*
 * Implementation of method add_object for interface afw_adapter_session.
 */
const afw_utf8_t *
impl_afw_adapter_session_add_object(
    AFW_ADAPTER_SESSION_SELF_T *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *adapter = (afw_file_internal_adapter_t *)self->adapter;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;
    const afw_utf8_t *object_id;

    object_id = (suggested_object_id)
        ? suggested_object_id
        : afw_uuid_create_utf8(xctx->p, xctx);

    full_path = impl_get_full_path(adapter, object_type_id, object_id,
        xctx->p, xctx);
    raw = afw_content_type_object_to_raw(adapter->content_type,
        object, &afw_object_options_essential_with_whitespace,
        xctx->p, xctx);
    afw_file_from_memory(full_path, raw, afw_file_mode_write_new, xctx);

    return object_id;
}


/*
 * Implementation of method modify_object for interface afw_adapter_session.
 */
void
impl_afw_adapter_session_modify_object(
    AFW_ADAPTER_SESSION_SELF_T *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_adapter_modify_entry_t *const *entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *adapter =
        (afw_file_internal_adapter_t *)self->adapter;
    const afw_object_t *object;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;

    if (!object_id || !object_type_id) {
        AFW_THROW_ERROR_Z(general,
            "Missing object id or object_type.", xctx);
    }

    /* _AdaptiveJournalEntry_ objects are read-only. */
    if (afw_utf8_equal(object_type_id,
        AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
    {
        AFW_THROW_ERROR_Z(read_only,
            AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            " objects are read-only", xctx);
    }

    full_path = impl_get_full_path(adapter, object_type_id, object_id,
        xctx->p, xctx);

    /* Get object to modify. */
    raw = afw_file_to_memory(full_path, 0, xctx->p, xctx);
    object = afw_content_type_raw_to_object(
        adapter->content_type, raw, full_path,
        &adapter->pub.adapter_id,
        object_type_id, object_id, false, xctx->p, xctx);

    /* Apply modifications. */
    afw_adapter_modify_entries_apply_to_unnormalized_object(
        entry, object, xctx);

    /* Write modified object. */
    raw = afw_content_type_object_to_raw(adapter->content_type,
        object, &afw_object_options_essential_with_whitespace,
        xctx->p, xctx);
    afw_file_from_memory(full_path, raw, afw_file_mode_write_existing,
        xctx);
}


/*
 * Implementation of method replace_object for interface afw_adapter_session.
 */
void
impl_afw_adapter_session_replace_object(
    AFW_ADAPTER_SESSION_SELF_T *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *adapter = (afw_file_internal_adapter_t *)self->adapter;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;

    if (!object_id || !object_type_id) {
        AFW_THROW_ERROR_Z(general,
            "Updated object missing id or object_type.", xctx);
    }

    full_path = impl_get_full_path(adapter, object_type_id,
        object_id, xctx->p, xctx);

    /* Write updated object. */
    raw = afw_content_type_object_to_raw(adapter->content_type,
        replacement_object, &afw_object_options_essential_with_whitespace,
        xctx->p, xctx);
    afw_file_from_memory(full_path, raw, afw_file_mode_write_existing,
        xctx);
}


/*
 * Implementation of method delete_object for interface afw_adapter_session.
 */
void
impl_afw_adapter_session_delete_object(
    AFW_ADAPTER_SESSION_SELF_T *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx)
{
    afw_file_internal_adapter_t *adapter = (afw_file_internal_adapter_t *)self->adapter;
    const afw_utf8_t *full_path;

    full_path = impl_get_full_path(adapter, object_type_id, object_id,
        xctx->p, xctx);
    afw_file_delete(full_path, xctx);
}


/*
 * Implementation of method begin_transaction of interface afw_adapter_session.
 */
const afw_adapter_transaction_t *
impl_afw_adapter_session_begin_transaction(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* This adapter does not support transactions. */
    return NULL;
}


/*
 * Implementation of method get_journal of interface afw_adapter_session.
 */
const afw_adapter_journal_t *
impl_afw_adapter_session_get_journal_interface(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{

    /* Return event journal &self->pub. */
    return (afw_adapter_journal_t *)&self->journal;
}



/*
 * Implementation of method get_key_value_interface of interface
 * afw_adapter_session.
 */
const afw_adapter_key_value_t *
impl_afw_adapter_session_get_key_value_interface (
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Key value interface is not supported by this adapter. */
    return NULL;
}

/*
 * Implementation of method get_index_interface of interface afw_adapter_session.
 */
const afw_adapter_impl_index_t *
impl_afw_adapter_session_get_index_interface (
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Key value interface is not supported by this adapter. */
    return NULL;
}


/*
 * Implementation of method get_object_type_cache_interface for interface
 * afw_adapter_session.
 */
const afw_adapter_object_type_cache_t *
impl_afw_adapter_session_get_object_type_cache_interface(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* There is on adapter cache. */
    return NULL;
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework file support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_file.c
 * @brief Adaptive framework file support
 */

#include "afw_internal.h"
#include <apr_buckets.h>
#include <apr_strings.h>


/* Declares and rti/inf defines for interface afw_adaptor_factory */
#define AFW_IMPLEMENTATION_ID "file"
#include "afw_adaptor_factory_impl_declares.h"
#include "afw_adaptor_impl_declares.h"
#include "afw_adaptor_session_impl_declares.h"


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Adaptor type for accessing objects contained in files.");

/* File adaptor factory instance. */
static const afw_adaptor_factory_t impl_adaptor_factory =
{
    &impl_afw_adaptor_factory_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    &impl_factory_description
};


/* Return full file path. */
AFW_DEFINE(const afw_utf8_t *)
afw_file_insure_full_path(const afw_utf8_t *path,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *full_path;
    const char *path_z;
    char *full_path_z;
    size_t len;
    apr_status_t rv;
    
    path_z = afw_utf8_to_utf8_z(path, p, xctx);
    rv = apr_filepath_merge(
        &full_path_z, NULL, path_z,
        APR_FILEPATH_TRUENAME,
        afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            "Unresolvable path %s", path_z);
    }
    len = strlen(full_path_z);
    if (len > 0 && full_path_z[len - 1] != '/') {
        full_path = afw_utf8_printf(p, xctx, "%s/", full_path_z);
    }
    else {
        full_path = afw_utf8_create_copy(full_path_z, AFW_UTF8_Z_LEN, p, xctx);
    }
    return full_path;  
}


/* Read a file into memory in a specifed pool. */
AFW_DEFINE(const afw_memory_t *)
afw_file_to_memory(
    const afw_utf8_t * file_path,
    apr_size_t file_size,
    const afw_pool_t *p,
    afw_xctx_t *xctx) 
{
    afw_memory_t *to_memory;
    afw_byte_t *buff;
    FILE *in;
    apr_status_t rv;
    apr_finfo_t finfo;
    const afw_utf8_z_t *file_path_z;

    /* Allocate to_memory struct. */
    to_memory = afw_pool_calloc_type(p, afw_memory_t, xctx);

    /* Open file. */
    file_path_z = afw_utf8_to_utf8_z(file_path, p, xctx);
    in = fopen(file_path_z, "r");
    if (!in) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Error opening %s errno %d", file_path_z, errno);
    }

    AFW_TRY {

        /* Get size of file if not supplied. */
        if (file_size == 0) {
            rv = apr_stat(&finfo, file_path_z, APR_FINFO_SIZE,
                afw_pool_get_apr_pool(p));
            if (rv != APR_SUCCESS) {
                AFW_THROW_ERROR_RV_Z(general, apr, rv,
                    "apr_stat() failed.", xctx);
            }
            file_size = (size_t)finfo.size;
        }
        /* Create a buffer big enough for contents of file. */
        buff = afw_pool_malloc(p, file_size, xctx);

        /* Read file into buffer. */
        to_memory->size = fread(buff, 1, file_size, in);
        to_memory->ptr = buff;
        if (to_memory->size < 1) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Error reading %" AFW_UTF8_FMT ".", 
                AFW_UTF8_FMT_ARG(file_path));
        }

    }

    AFW_FINALLY{
        /* Close input file. */
        fclose(in);
    }

    AFW_ENDTRY;

    /* Return result. */
    return to_memory;

}


/* Write a file from memory. */
AFW_DEFINE(void)
afw_file_from_memory(
    const afw_utf8_t * file_path,
    const afw_memory_t * from_memory,
    afw_file_mode_t mode,
    afw_xctx_t *xctx) 
{
    apr_pool_t *apr_p = afw_pool_get_apr_pool(xctx->p);
    apr_file_t *f;
    afw_utf8_z_t *file_path_z;
    apr_status_t rv;
    const afw_utf8_z_t *step_z;
    const afw_utf8_octet_t *i;
    afw_utf8_octet_t *o;
    afw_utf8_octet_t c;
    afw_utf8_octet_t *last_slash;
    afw_size_t count;

    /*
     * Make afw_u8_z copy of name with '\' changed to '/'.  Remember location
     * of last slash.
     */
    file_path_z = afw_xctx_calloc(file_path->len + 1, xctx);
    last_slash = NULL;
    for (i = file_path->s, o = file_path_z, count = file_path->len;
        count > 0;
        i++, o++, count--)
    {
        *o = *i;
        if (*o == '\\') *o = '/';
        if (*o == '/') last_slash = o;
    }
    *o = 0;


    /* Process based on file mode. */
    switch (mode) {

    /* Mode write ok. */
    case afw_file_mode_write:
        break;

    /*
     * For mode write_new, check that file does not already exist and that
     * the directory stucture exists.
     */
    case afw_file_mode_write_new:
        rv = apr_file_open(&f, file_path_z, APR_FOPEN_READ,
            APR_FPROT_OS_DEFAULT, apr_p);
        if (rv == APR_SUCCESS) {
            apr_file_close(f);
            AFW_THROW_ERROR_FZ(general, xctx,
                "File %s already exists.", file_path_z);
        }
        /* If there is a '/' in path, make sure all of the directories exist */
        if (last_slash) {
            o = last_slash + 1;
            c = *o;
            *o = 0;
            step_z = "apr_dir_make_recursive()";
            rv = apr_dir_make_recursive(file_path_z,
                APR_FPROT_OS_DEFAULT, apr_p);
            if (rv != APR_SUCCESS) goto error_apr;
            *o = c;
        }
        break;

    /* Mode write_existing ok. */
    case afw_file_mode_write_existing:
        break;

    /* Other modes are invalid. */
    default:
        AFW_THROW_ERROR_FZ(general, xctx, "Invalid mode %d.", mode);
    };

    /* Open file for write. */
    step_z = "apr_file_open()";
    rv = apr_file_open(&f, file_path_z, APR_FOPEN_WRITE + APR_FOPEN_CREATE,
        APR_FPROT_OS_DEFAULT, apr_p);
    if (rv != APR_SUCCESS) goto error_apr;

    /* Write memory to file. */
    step_z = "apr_file_write()";
    count = from_memory->size;
    rv = apr_file_write(f, from_memory->ptr, &count);
    if (rv != APR_SUCCESS) goto error_apr;

    /* Truncate file to length written. */
    step_z = "apr_file_trunc()";
    rv = apr_file_trunc(f, from_memory->size);
    if (rv != APR_SUCCESS) goto error_apr;

    /* Close file. */
    step_z = "apr_file_close()";
    rv = apr_file_close(f);
    if (rv != APR_SUCCESS) goto error_apr;

    /* Return. */
    return;

error_apr:
    AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
        "Error writing file %s - %s",
        file_path_z, step_z);
}


/* Delete file. */
AFW_DEFINE(void) afw_file_delete(
    const afw_utf8_t * file_path,
    afw_xctx_t *xctx)
{
    const afw_utf8_z_t *file_path_z;

    file_path_z = afw_utf8_to_utf8_z(file_path, xctx->p, xctx);
    if (apr_file_remove(file_path_z, afw_pool_get_apr_pool(xctx->p)) < 0) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Error deleting %s.", file_path_z);
    }

}


/* Get the factory for file adaptor .*/
AFW_DEFINE(const afw_adaptor_factory_t *)
afw_file_adaptor_factory_get()
{
    return &impl_adaptor_factory;
}


/*
 * Implementation of method create_adaptor_cede_p of interface afw_adaptor_factory.
 */
const afw_adaptor_t *
impl_afw_adaptor_factory_create_adaptor_cede_p (
    const afw_adaptor_factory_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Create file adaptor. */
    return afw_file_adaptor_create_cede_p(properties, p, xctx);
}



/* Create function for file adaptor. */
AFW_DEFINE(const afw_adaptor_t *)
afw_file_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_file_internal_adaptor_t *self;
    afw_adaptor_t *adaptor;
    const afw_utf8_t *content_type;
    const afw_value_t *value;
    afw_boolean_t b;

    /* Create adaptor and process common properties.  */
    adaptor = afw_adaptor_impl_create_cede_p(
        &impl_afw_adaptor_inf,
        sizeof(afw_file_internal_adaptor_t),
        properties, p, xctx);
    self = (afw_file_internal_adaptor_t *)adaptor;
    p = self->pub.p;

    /* Get content_type parameters. */
    content_type = afw_object_old_get_property_as_utf8(properties,
        &afw_s_contentType, p, xctx);
    self->content_type = afw_environment_get_content_type(content_type,
        xctx);
    if (!self->content_type)
    {
        afw_adaptor_impl_throw_property_invalid(adaptor,
            &afw_s_contentType, xctx);
    }

    /* Get optional filename extension */
    self->filename_suffix = afw_object_old_get_property_as_utf8(
        properties, &afw_s_filenameSuffix, p, xctx);
    if (!self->filename_suffix) {
        self->filename_suffix = &afw_s_a_empty_string;
    }

    /* Get root from parameters and make it full path. */
    value = afw_object_get_property_compile_and_evaluate_as(
        properties,  &afw_s_root, adaptor->source_location,
        afw_compile_type_hybrid, p, xctx);
    if (!afw_value_is_string(value)) {
        afw_adaptor_impl_throw_property_invalid(adaptor,
            &afw_s_root, xctx);
    }
    self->root = afw_file_insure_full_path(
        &((afw_value_string_t *)value)->internal,
        p, xctx);

    /* Make path for journal directory. */
    self->journal_dir_path_z = afw_utf8_z_printf(p, xctx,
        "%" AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY "/",
        AFW_UTF8_FMT_ARG(self->root));

    /* Make path to journal lock file. */
    self->journal_lock_file_path_z = afw_utf8_z_printf(p, xctx,
        "%" AFW_UTF8_FMT AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY "/journal_lock",
        AFW_UTF8_FMT_ARG(self->root));

    /* If isDevelopmentInput is true, provide appropriate object types. */
    b = afw_object_old_get_property_as_boolean_deprecated(properties,
        &afw_s_isDevelopmentInput, xctx);
    if (b) {
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            &afw_s__AdaptiveCollection_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            &afw_s__AdaptiveDataTypeGenerate_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            &afw_s__AdaptiveManifest_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            &afw_s__AdaptiveFunctionGenerate_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            &afw_s__AdaptiveObjectType_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            &afw_s__AdaptiveValueMeta_, true, true, xctx);
    }

    /* Return adaptor. */
    return adaptor;
}


/*
 * Implementation of method destroy of interface afw_adaptor.
 */
void
impl_afw_adaptor_destroy(
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    /* Release pool. */
    afw_pool_release(instance->p, xctx);
}


/*
 * Implementation of method create_adaptor_session of interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session (
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    afw_file_internal_adaptor_t *self = (afw_file_internal_adaptor_t *)instance;
    afw_file_internal_adaptor_session_t *session;

    session = afw_xctx_calloc_type(afw_file_internal_adaptor_session_t, xctx);
    session->pub.inf = &impl_afw_adaptor_session_inf;
    session->pub.adaptor = (afw_adaptor_t *)self;
    session->pub.p = xctx->p;
    session->adaptor = self;

    /* Adaptor session instance holds event journal instance. */
    session->journal.inf = afw_file_internal_get_journal_inf();
    session->journal.session = (afw_adaptor_session_t *)session;

    /* Return session. */
    return (const afw_adaptor_session_t *)session;
}



/*
 * Implementation of method get_additional_metrics of interface afw_adaptor.
 */
const afw_object_t *
impl_afw_adaptor_get_additional_metrics(
    const afw_adaptor_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* There are no adaptor specific metrics. */
    return NULL;
}


/* Helper to get full path. */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_t *)
impl_get_full_path(
    afw_file_internal_adaptor_t *adaptor,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_utf8_printf(p, xctx,
        "%" AFW_UTF8_FMT "%" AFW_UTF8_FMT "/%" AFW_UTF8_FMT "%" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(adaptor->root),
        AFW_UTF8_FMT_ARG(object_type_id),
        AFW_UTF8_FMT_ARG(object_id),
        AFW_UTF8_FMT_ARG(adaptor->filename_suffix));
}


/*
 * Implementation of method destroy of interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
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
    afw_file_internal_adaptor_session_t * self = (afw_file_internal_adaptor_session_t *)instance;
    afw_file_internal_adaptor_t *adaptor = (afw_file_internal_adaptor_t *)self->adaptor;
    const char *dirname_z;
    const afw_utf8_t *full_path;
    apr_dir_t *dir;
    apr_status_t rv;
    apr_finfo_t finfo;
    char err[100];
    const afw_pool_t *obj_p;
    const afw_object_t *obj;
    const afw_memory_t *raw;
    const afw_utf8_t *object_id;
    afw_size_t len;

    /* Open ObjectType's directory. */
    dirname_z = apr_psprintf(afw_pool_get_apr_pool(p),
        "%" AFW_UTF8_FMT "%" AFW_UTF8_FMT "/",
        AFW_UTF8_FMT_ARG(adaptor->root),
        AFW_UTF8_FMT_ARG(object_type_id));
    rv = apr_dir_open(&dir, dirname_z, afw_pool_get_apr_pool(p));

    /* If not found, return no objects. */
    if (APR_STATUS_IS_ENOENT(rv)) {
        callback(NULL, context, xctx);
        return;
    }

    /* If there is another problem, throw error. */
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_open() failed.",
            xctx);
    }

    /* Process each JSON object in directory. */
    while (1) {

        /* Read next directory entry until there are no more.*/
        rv = apr_dir_read(&finfo, APR_FINFO_SIZE + APR_FINFO_NAME, dir);
        if (rv == APR_ENOENT ||
            /** @fixme seems to be this on windows */ rv == 720018) break;
        if (rv != APR_SUCCESS) {
            apr_strerror(rv, &err[0], 100);
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_open() failed.",
                xctx);
        }

        /* Skip ., .., and hidden files. */
        if (*(finfo.name) == '.') {
            continue;
        }

        /* Create object from corresponding file. */
        len = strlen(finfo.name);
        if (adaptor->filename_suffix) {
            if (len <= adaptor->filename_suffix->len ||
                memcmp(finfo.name + (len - adaptor->filename_suffix->len),
                    adaptor->filename_suffix->s,
                    adaptor->filename_suffix->len) != 0)
            {
                continue;
            }
            len -= adaptor->filename_suffix->len;
        }


        /* Create pool for object and related memory. */
        obj_p = afw_pool_create(p, xctx);

        /* Determine object_id and full_path. */
        object_id = afw_utf8_create(finfo.name, len, obj_p, xctx);
        
        /** @fixme: Mike filename_suffix could be NULL here */
        full_path = afw_utf8_printf(obj_p, xctx,
            "%" AFW_UTF8_FMT "%" AFW_UTF8_FMT "/%" AFW_UTF8_FMT "%" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(adaptor->root),
            AFW_UTF8_FMT_ARG(object_type_id),
            AFW_UTF8_FMT_ARG(object_id),
            AFW_UTF8_FMT_OPTIONAL_ARG(adaptor->filename_suffix));

        /*
         * Load file to memory and convert to object instance.  Ceed control
         * of obj_p to object.
         */
        raw = afw_file_to_memory(full_path, (apr_size_t)finfo.size,
            obj_p, xctx);
        obj = afw_content_type_raw_to_object(
            adaptor->content_type, raw, full_path, &adaptor->pub.adaptor_id,
            object_type_id, object_id, true, obj_p, xctx);

        /*
         * If query criteria met, callback with object.  Callback will release
         * object. If callback returns true, prematurely stop retrieving.
         */
        if (afw_query_criteria_test_object(obj, criteria, p, xctx)) {
            if (callback(obj, context, xctx)) {
                break;
            }
        }

        /* If query criteria not met, release object. */
        else {
            afw_object_release(obj, xctx);
        }
    }

    /* Close ObjectType's directory. */
    rv = apr_dir_close(dir);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_close() failed.",
            xctx);
    }

    /* Call callback one more time with NULL object pointer. */
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
    afw_file_internal_adaptor_session_t * self = (afw_file_internal_adaptor_session_t *)instance;
    afw_file_internal_adaptor_t *adaptor = (afw_file_internal_adaptor_t *)self->adaptor;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;
    const afw_object_t *object;
    const afw_pool_t *obj_p;

    /* Create pool for object and related memory. */
    obj_p = afw_pool_create(p, xctx);

    /* Determine full path. */
    full_path = impl_get_full_path(adaptor, object_type_id, object_id,
        obj_p, xctx);

    /*
     * Load file to memory and convert to object instance.  Ceed control
     * of obj_p to object.
     */
    raw = afw_file_to_memory(full_path, 0, obj_p, xctx);
    object = afw_content_type_raw_to_object(
        adaptor->content_type, raw, full_path,
        &adaptor->pub.adaptor_id, object_type_id, object_id,
        true, obj_p, xctx);

    /* Pass object to callback.  Callback will release object. */
    callback(object, context, xctx);
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
    afw_file_internal_adaptor_session_t * self = (afw_file_internal_adaptor_session_t *)instance;
    afw_file_internal_adaptor_t *adaptor = (afw_file_internal_adaptor_t *)self->adaptor;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;
    const afw_utf8_t *object_id;

    object_id = (suggested_object_id)
        ? suggested_object_id
        : afw_uuid_create_utf8(xctx->p, xctx);

    full_path = impl_get_full_path(adaptor, object_type_id, object_id,
        xctx->p, xctx);
    raw = afw_content_type_object_to_raw(adaptor->content_type,
        object, &afw_object_options_essential_with_whitespace,
        xctx->p, xctx);
    afw_file_from_memory(full_path, raw, afw_file_mode_write_new, xctx);

    return object_id;
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
    afw_file_internal_adaptor_session_t * self =
        (afw_file_internal_adaptor_session_t *)instance;
    afw_file_internal_adaptor_t *adaptor =
        (afw_file_internal_adaptor_t *)self->adaptor;
    const afw_object_t *object;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;

    if (!object_id || !object_type_id) {
        AFW_THROW_ERROR_Z(general,
            "Missing object id or object_type.", xctx);
    }

    /* _AdaptiveJournalEntry_ objects are read-only. */
    if (afw_utf8_equal(object_type_id,
        &AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
    {
        AFW_THROW_ERROR_Z(read_only,
            AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            " objects are read-only", xctx);
    }

    full_path = impl_get_full_path(adaptor, object_type_id, object_id,
        xctx->p, xctx);

    /* Get object to modify. */
    raw = afw_file_to_memory(full_path, 0, xctx->p, xctx);
    object = afw_content_type_raw_to_object(
        adaptor->content_type, raw, full_path,
        &adaptor->pub.adaptor_id,
        object_type_id, object_id, false, xctx->p, xctx);

    /* Apply modifications. */
    afw_adaptor_modify_entries_apply_to_unnormalized_object(
        entry, object, xctx);

    /* Write modified object. */
    raw = afw_content_type_object_to_raw(adaptor->content_type,
        object, &afw_object_options_essential_with_whitespace,
        xctx->p, xctx);
    afw_file_from_memory(full_path, raw, afw_file_mode_write_existing,
        xctx);
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
    afw_file_internal_adaptor_session_t * self = (afw_file_internal_adaptor_session_t *)instance;
    afw_file_internal_adaptor_t *adaptor = (afw_file_internal_adaptor_t *)self->adaptor;
    const afw_utf8_t *full_path;
    const afw_memory_t *raw;

    if (!object_id || !object_type_id) {
        AFW_THROW_ERROR_Z(general,
            "Updated object missing id or object_type.", xctx);
    }

    full_path = impl_get_full_path(adaptor, object_type_id,
        object_id, xctx->p, xctx);

    /* Write updated object. */
    raw = afw_content_type_object_to_raw(adaptor->content_type,
        replacement_object, &afw_object_options_essential_with_whitespace,
        xctx->p, xctx);
    afw_file_from_memory(full_path, raw, afw_file_mode_write_existing,
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
    afw_file_internal_adaptor_session_t * self = (afw_file_internal_adaptor_session_t *)instance;
    afw_file_internal_adaptor_t *adaptor = (afw_file_internal_adaptor_t *)self->adaptor;
    const afw_utf8_t *full_path;

    full_path = impl_get_full_path(adaptor, object_type_id, object_id,
        xctx->p, xctx);
    afw_file_delete(full_path, xctx);
}


/*
 * Implementation of method begin_transaction of interface afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* This adaptor does not support transactions. */
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
    afw_file_internal_adaptor_session_t * self = (afw_file_internal_adaptor_session_t *)instance;

    /* Return event journal instance. */
    return (afw_adaptor_journal_t *)&self->journal;
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
    /* Key value interface is not supported by this adaptor. */
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
    /* There is on adaptor cache. */
    return NULL;
}

/* Interface afw_adaptor_session Implementation for afw_vfs        */
// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_vfs_adaptor_session.c
 * @brief Implementation of afw_adaptor_session interface for afw_vfs
 */

#include "afw.h"
#include "afw_adaptor_impl.h"
#include "afw_vfs_adaptor_internal.h"
#include <apr_fnmatch.h>


/* Declares and rti/inf defines for interface afw_adaptor_session */
#define AFW_IMPLEMENTATION_ID "vfs"
#include "afw_adaptor_session_impl_declares.h"


static const afw_key_z_string_z_t *
impl_get_vfs_entry(
    const afw_vfs_adaptor_internal_session_t *self,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    const afw_vfs_adaptor_internal_t *adaptor;
    const afw_key_z_string_z_t *e;

    adaptor = (const afw_vfs_adaptor_internal_t * )self->pub.adaptor;
    for (e = adaptor->vfs_map; e->key_z; e++) {
        if (afw_utf8_starts_with(object_id, &e->key)) {
            break;
        }
    }
    if (e && !e->key.s) {
        e = NULL;
    }

    return e;
}


static const afw_object_t *
impl_read_file_object(
    const afw_vfs_adaptor_internal_session_t *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_key_z_string_z_t *vfs_entry,
    const afw_utf8_t *object_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_utf8_z_t *file_path_z;
    const afw_utf8_t *data_string;
    const afw_memory_t *data_binary;
    const afw_utf8_t *vfs_path;
    const afw_array_t *filenames;
    afw_value_dateTime_t *dateTime;
    afw_utf8_t adjusted;
    FILE *fd;
    afw_byte_t *buff;
    apr_dir_t *dir;
    afw_size_t size;
    afw_size_t size_read;
    apr_finfo_t finfo;
    apr_status_t rv;

    object = NULL;
    adjusted.s = object_id->s + vfs_entry->key.len;
    adjusted.len = object_id->len - vfs_entry->key.len;
    file_path_z = afw_utf8_z_printf(p, xctx,
        AFW_UTF8_FMT AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&vfs_entry->string),
        AFW_UTF8_FMT_ARG(&adjusted));
    rv = apr_stat(&finfo, file_path_z, APR_FINFO_MIN, afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        /* If not found, return with NULL. */
        if (APR_STATUS_IS_ENOENT(rv)) {
            return NULL;
        }
        /*
            If APR_INCOMPLETE is returned all the fields in finfo may not be
            filled in, and you need to check the finfo->valid bitmask to verify
            that what you're looking for is there.
         */
        AFW_THROW_ERROR_RV_Z(general,
            apr, rv, "apr_stat() error", xctx);
    }

    /* File type directory. */
    if (finfo.filetype == APR_DIR) {

        /* Open directory. */
        rv = apr_dir_open(&dir, file_path_z, afw_pool_get_apr_pool(p));

        /* If not found, return no objects. */
        if (APR_STATUS_IS_ENOENT(rv)) {
            return NULL;
        }

        /* If there is another problem, throw error. */
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "apr_dir_open() %s failed.",
                vfs_entry->string_z);
        }

        /* Process each file in directory. */
        object = afw_object_create_managed_cede_p(p, xctx);
        afw_object_meta_set_ids(object,
            &self->pub.adaptor->adaptor_id,
            afw_vfs_s__AdaptiveFile_vfs,
            object_id,
            xctx);
        afw_object_set_property(object,
            afw_vfs_s_isDirectory, afw_value_true, xctx);
        vfs_path = afw_utf8_printf(p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&self->pub.adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_id));
        afw_object_set_property_as_anyURI(object,
            afw_vfs_s_vfsPath, vfs_path, xctx);
        filenames = afw_array_of_create(afw_data_type_string, p, xctx);
        afw_object_set_property_as_array(object,
            afw_vfs_s_data, filenames, xctx);
        for (;;) {

            /* Read next directory entry until there are no more.*/
            rv = apr_dir_read(&finfo, APR_FINFO_NAME | APR_FINFO_TYPE, dir);
            if (rv == APR_ENOENT ||
                /** @fixme seems to be this on windows */ rv == 720018) break;
            if (rv != APR_SUCCESS) {
                apr_dir_close(dir);
                AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_read() failed.",
                    xctx);
            }

            /* Directory. */
            if (finfo.filetype == APR_DIR) {
                /* Skip ./ and ../ since those are never allowed in objectIds.*/
                if (strcmp(finfo.name, ".") == 0 ||
                    strcmp(finfo.name, "..") == 0)
                {
                    continue;
                }
                size = strlen(finfo.name) + 1;
                buff = afw_pool_malloc(p, size, xctx);
                buff[size - 1] = '/';
                memcpy(buff, finfo.name, size - 1);
                data_string = afw_utf8_create((const afw_utf8_octet_t *)buff, size, p, xctx);
                afw_array_add_value(filenames,
                    afw_value_create_unmanaged_string(data_string, p, xctx),
                    xctx);
            }

            /* Regular file. */
            else if (finfo.filetype == APR_REG) {
                size = strlen(finfo.name);
                buff = afw_pool_malloc(p, size, xctx);
                memcpy(buff, finfo.name, size);
                data_string = afw_utf8_create((const afw_utf8_octet_t *)buff, size, p, xctx);
                afw_array_add_value(filenames,
                    afw_value_create_unmanaged_string(data_string, p, xctx),
                    xctx);
            }

            /* Ignore other filetypes. */
        }

        /* Close ObjectType's directory. */
        rv = apr_dir_close(dir);
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_close() failed.",
                xctx);
        }
    }

    /* File type regular file. */
    else if (finfo.filetype == APR_REG) {
        fd = fopen(file_path_z, "r");
        if (!fd) {
            AFW_THROW_ERROR_Z(general, "fopen() failed.", xctx);
        }
        size = (size_t)finfo.size;
        buff = afw_pool_malloc(p, size, xctx);
        size_read = fread(buff, 1, size, fd);
        fclose(fd);
        if (size_read == -1) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Error reading %s.", file_path_z);
        }
        object = afw_object_create_managed_cede_p(p, xctx);
        afw_object_meta_set_ids(object,
            &self->pub.adaptor->adaptor_id,
            afw_vfs_s__AdaptiveFile_vfs,
            object_id,
            xctx);
        vfs_path = afw_utf8_printf(p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&self->pub.adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_id));
        afw_object_set_property_as_anyURI(object,
            afw_vfs_s_vfsPath, vfs_path, xctx);
        if (afw_utf8_is_valid((const afw_utf8_octet_t *)buff, size_read, xctx)) {
            data_string = afw_utf8_create((const afw_utf8_octet_t *)buff, size_read,
                p, xctx);
            afw_object_set_property_as_string(object,
                afw_vfs_s_data, data_string, xctx);
        }
        else {
            data_binary = afw_memory_create(buff, size_read,
                p, xctx);
            afw_object_set_property_as_hexBinary(object,
                afw_vfs_s_data, data_binary, xctx);
        }
    }

    /* If file type wasn't ignore, add time properties to object. */
    if (finfo.filetype == APR_DIR || finfo.filetype == APR_REG) {

        /** The time the file was last accessed. */
        if (finfo.atime != 0) {
            dateTime = afw_value_allocate_unmanaged_dateTime(object->p, xctx);
            afw_dateTime_set_from_apr_time(&dateTime->internal,
                finfo.atime, xctx);
            afw_object_set_property(
                object, afw_vfs_s_timeAccessed, &dateTime->pub, xctx);
        }

        /** The time the file was created. */
        if (finfo.ctime != 0) {
            dateTime = afw_value_allocate_unmanaged_dateTime(object->p, xctx);
            afw_dateTime_set_from_apr_time(&dateTime->internal,
                finfo.ctime, xctx);
            afw_object_set_property(
                object, afw_vfs_s_timeCreated, &dateTime->pub, xctx);
        }

        /** The time the file was last modified. */
        if (finfo.mtime != 0) {
            dateTime = afw_value_allocate_unmanaged_dateTime(object->p, xctx);
            afw_dateTime_set_from_apr_time(&dateTime->internal,
                finfo.mtime, xctx);
            afw_object_set_property(
                object, afw_vfs_s_timeModified, &dateTime->pub, xctx);
        }

    }

    /* Return object. */
    return object;
}



static afw_boolean_t
impl_process_directory(
    const afw_vfs_adaptor_internal_session_t *self,
    afw_vfs_adaptor_internal_session_context_t *ctx,
    const afw_key_z_string_z_t *vfs_entry,
    afw_xctx_t *xctx)
{
    const afw_pool_t *object_p;
    const afw_object_t *object;
    const afw_utf8_t *object_id;
    afw_key_z_string_z_t subdirectory_vfs_entry;
    apr_finfo_t finfo;
    apr_dir_t *dir;
    apr_status_t rv;
    afw_boolean_t shortcut;

    shortcut = false;

    /* Open directory. */
    rv = apr_dir_open(&dir, vfs_entry->string_z, afw_pool_get_apr_pool(ctx->p));

    /* If not found, return no objects. */
    if (APR_STATUS_IS_ENOENT(rv)) {
        return shortcut;
    }

    /* If there is another problem, throw error. */
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            "apr_dir_open() %s failed.",
            vfs_entry->string_z);
    }

    /* Process each file in directory. */
    for (;;) {
        object = NULL;
        object_p = NULL;

        /* Read next directory entry until there are no more.*/
        afw_memory_clear(&finfo);
        rv = apr_dir_read(&finfo, APR_FINFO_NAME | APR_FINFO_TYPE, dir);
        if (rv == APR_ENOENT ||
            /** @fixme seems to be this on windows */ rv == 720018) break;
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_read() failed.",
                xctx);
        }

        /* Always skip ./ and ../ plus hidden files unless requested. */
        if (*(finfo.name) == '.') {
            if (!ctx->includeHidden ||
                (finfo.filetype == APR_DIR &&
                    (strcmp(finfo.name, "./") == 0 ||
                        strcmp(finfo.name, "../") == 0)))
            {
                continue;
            }
        }

        /* If this is a directory and recursive is specified, traverse. */
        if (ctx->recursive && finfo.filetype == APR_DIR) {
            afw_memory_clear(&subdirectory_vfs_entry);
            subdirectory_vfs_entry.key_z = afw_utf8_z_printf(
                ctx->p, xctx, "%s%s/", vfs_entry->key_z, finfo.name);
            subdirectory_vfs_entry.key.len =
                strlen(subdirectory_vfs_entry.key_z);
            subdirectory_vfs_entry.string_z = afw_utf8_z_printf(
                ctx->p, xctx, "%s%s/", vfs_entry->string_z, finfo.name);
            subdirectory_vfs_entry.string.len =
                strlen(subdirectory_vfs_entry.string_z);
            shortcut = impl_process_directory(self,
                ctx, &subdirectory_vfs_entry, xctx);
            if (shortcut) {
                return shortcut;
            }
        }

        /* Otherwise ... */
        else {

            /* If regular file that doesn't have correct suffix, skip it. */
            if (finfo.filetype == APR_REG && ctx->suffix) {
                if (strlen(finfo.name) < ctx->suffix->len ||
                    memcmp(
                        finfo.name +
                        strlen(finfo.name) -
                        ctx->suffix->len,
                        ctx->suffix->s,
                        ctx->suffix->len
                    ) != 0)
                {
                    continue;
                }
            }

            /* Read file object.  Object has its own pool. */
            object_p = afw_pool_create(ctx->p, xctx);
            object_id = afw_utf8_printf(object_p, xctx,
                "%s%s%s", vfs_entry->key_z, finfo.name,
                finfo.filetype == APR_DIR ? "/" : "");
            object = impl_read_file_object(self, ctx->impl_request, vfs_entry,
                object_id, object_p, xctx);
        }

        /*
         * If query criteria met, callback with object.  Callback will
         * release object. If callback returns true, prematurely stop
         * retrieving.
         */
        if (object &&
            afw_query_criteria_test_object(object, ctx->criteria,
                ctx->p, xctx))
        {
            shortcut = ctx->original_callback(object,
                ctx->original_context, xctx);
            if (shortcut) {
                break;
            }
        }

        /* If query criteria not met, release pool. */
        else if (object_p) {
            afw_pool_release(object_p, xctx);
        }
    }

    /* Close ObjectType's directory. */
    rv = apr_dir_close(dir);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_close() failed.",
            xctx);
    }

    return shortcut;
}



/* Internal session create */
const afw_adaptor_session_t *
afw_vfs_adaptor_internal_session_create(
    afw_vfs_adaptor_internal_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_vfs_adaptor_internal_session_t *self;

    /*
     * You may want to create a new pool for instance, but will just use
     * xctx's pool in this example.
     */
    self = afw_xctx_calloc_type(
        afw_vfs_adaptor_internal_session_t, xctx);
    self->pub.inf = &impl_afw_adaptor_session_inf;
    self->pub.adaptor = (const afw_adaptor_t *)adaptor;
    self->pub.p = xctx->p;

    /* Return new instance. */
    return (afw_adaptor_session_t *)self;
}



/*
 * Implementation of method destroy for interface afw_adaptor_session.
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
    afw_vfs_adaptor_internal_session_t *self =
        (afw_vfs_adaptor_internal_session_t *)instance;
    const afw_vfs_adaptor_internal_t *adaptor =
        (const afw_vfs_adaptor_internal_t *)self->pub.adaptor;
    afw_vfs_adaptor_internal_session_context_t ctx;
    const afw_key_z_string_z_t *vfs_entry;
    const afw_utf8_t *subdirectory;
    const afw_object_t *object;
    afw_key_z_string_z_t subdirectory_vfs_entry;
    afw_boolean_t found;
    afw_boolean_t short_circuit;

    subdirectory = NULL;
    afw_memory_clear(&ctx);
    ctx.p = p;
    ctx.impl_request = impl_request;
    ctx.criteria = criteria;
    ctx.original_context = context;
    ctx.original_callback = callback;
    ctx.suffix = afw_s_a_empty_string;

    /* Object type _AdaptiveObjectType_ */
    if (afw_utf8_equal(object_type_id, afw_s__AdaptiveObjectType_)) {
        short_circuit = false;
        object = afw_runtime_get_object(
            afw_s__AdaptiveObjectType_,
            afw_vfs_s__AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects,
            xctx);
        if (object &&
            afw_query_criteria_test_object(object, criteria, p, xctx))
        {
            short_circuit = callback(object, context, xctx);
        }

        if (!short_circuit) {
            object = afw_runtime_get_object(
                afw_s__AdaptiveObjectType_,
                afw_vfs_s__AdaptiveFile_vfs,
                xctx);
            if (object &&
                afw_query_criteria_test_object(object, criteria, p, xctx))
            {
                callback(object, context, xctx);
            }
        }

        callback(NULL, context, xctx);
        return;
    }

    /* There are no other object types other than _AdaptiveFile_vfs. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        callback(NULL, context, xctx);
        return;
    }

    /* Adaptor vfs specific properties. */
    if (adaptor_type_specific) {

        /* includeHidden */
        ctx.includeHidden = afw_object_old_get_property_as_boolean(
            adaptor_type_specific, afw_vfs_s_includeHidden, &found, xctx);

        /* subdirectory */
        subdirectory = afw_object_old_get_property_as_string(
            adaptor_type_specific, afw_vfs_s_subdirectory, xctx);
        if (subdirectory &&
            (
                afw_utf8_starts_with(subdirectory, afw_s_a_slash) ||
                !afw_utf8_ends_with(subdirectory, afw_s_a_slash) ||
                afw_utf8_contains(subdirectory, afw_s_a_backslash) ||
                afw_utf8_contains(subdirectory, afw_s_a_dot_slash)
            ))
        {
            AFW_THROW_ERROR_Z(general,
                "The subdirectory property of the adaptorTypeSpecific "
                "parameter of retrieve_objects() can not start with '/', "
                "must end with '/', and can not contain '\\', './' or '../'",
                xctx);
        }

        /* suffix */
        ctx.suffix = afw_object_old_get_property_as_string(
            adaptor_type_specific, afw_vfs_s_suffix, xctx);

        /* recursive */
        ctx.recursive = afw_object_old_get_property_as_boolean(
            adaptor_type_specific, afw_vfs_s_recursive, &found, xctx);
    }

    /* Process each map entry applying subdirectory if specified. */
    for (vfs_entry = adaptor->vfs_map; vfs_entry->key_z; vfs_entry++)
    {
        if (subdirectory) {
            subdirectory_vfs_entry.key_z = afw_utf8_z_printf(p, xctx,
                AFW_UTF8_FMT AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&vfs_entry->key),
                AFW_UTF8_FMT_ARG(subdirectory));
            subdirectory_vfs_entry.key.len =
                strlen(subdirectory_vfs_entry.key_z);
            subdirectory_vfs_entry.string_z = afw_utf8_z_printf(p, xctx,
                AFW_UTF8_FMT AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&vfs_entry->string),
                AFW_UTF8_FMT_ARG(subdirectory));
            subdirectory_vfs_entry.string.len =
                strlen(subdirectory_vfs_entry.string_z);
        }
        else {
            afw_memory_copy(&subdirectory_vfs_entry, vfs_entry);
        }
        impl_process_directory(self, &ctx, &subdirectory_vfs_entry, xctx);
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
    afw_vfs_adaptor_internal_session_t *self =
        (afw_vfs_adaptor_internal_session_t *)instance;
    const afw_object_t *object;
    const afw_pool_t *object_p;
    const afw_key_z_string_z_t *vfs_entry;

    /* Only object type _AdaptiveFile_vfs is supported. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        callback(NULL, context, xctx);
        return;
    }

    /* Don't allow ./, ../, or \ in object id. */
    if (afw_utf8_contains(object_id, afw_s_a_dot_slash) ||
        afw_utf8_contains(object_id, afw_s_a_backslash))
    {
        callback(NULL, context, xctx);
        return;
    }

    /* Get object or NULL and pass it to callback. */
    object = NULL;
    vfs_entry = impl_get_vfs_entry(self, object_id, xctx);
    if (vfs_entry) {
        object_p = afw_pool_create(p, xctx);
        object = impl_read_file_object(self, impl_request,
            vfs_entry, object_id, object_p, xctx);
        if (!object) {
            afw_pool_release(object_p, xctx);
        }
    }
    callback(object, context, xctx);
}



static void
impl_determine_path_for_object_id(
    afw_vfs_adaptor_internal_session_t *self,
    const afw_utf8_t *object_id,
    afw_boolean_t expect_exists,
    afw_utf8_utf8_z_t *path,
    afw_boolean_t *is_directory,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_key_z_string_z_t *vfs_entry;
    afw_utf8_utf8_z_t adjusted;
    apr_finfo_t finfo;
    afw_boolean_t exists;
    apr_status_t rv;

    /* Initialize return parameters. */
    path->s.s = NULL;
    path->s.len = 0;
    *is_directory = false;

    /* Get vfs entry. */
    vfs_entry = impl_get_vfs_entry(self, object_id, xctx);
    if (!vfs_entry) {
        AFW_THROW_ERROR_Z(not_found,
            "object_id does not match anything in vfs map",
            xctx);
    }

    /* If object_id ended with '/', consider it a directory. */
    if (afw_utf8_ends_with(object_id, afw_s_a_slash)) {
        *is_directory = true;
    }

    /* Construct path. */
    adjusted.s.s = object_id->s + vfs_entry->key.len;
    adjusted.s.len = object_id->len - vfs_entry->key.len;
    path->s_z = afw_utf8_z_printf(p, xctx,
        AFW_UTF8_FMT AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&vfs_entry->string),
        AFW_UTF8_FMT_ARG(&adjusted.s));
    path->s.len = strlen(path->s_z);

    /* Get if path exists and make sure is_directory is correct. */
    rv = apr_stat(&finfo, path->s_z, APR_FINFO_TYPE, afw_pool_get_apr_pool(p));
    exists = true;
    if (APR_STATUS_IS_ENOENT(rv)) {
        exists = false;
    }
    else if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general,
            apr, rv, "apr_stat() error", xctx);
    }

    /* Check expect_exists */
    if (expect_exists) {
        if (!exists) {
            AFW_THROW_ERROR_FZ(not_found, xctx,
                "object_id " AFW_UTF8_FMT_Q " doesn't exist",
                AFW_UTF8_FMT_OPTIONAL_ARG(object_id));
        }
    }
    else {
        if (exists) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "object_id " AFW_UTF8_FMT_Q " already exists",
                AFW_UTF8_FMT_OPTIONAL_ARG(object_id));
        }
    }

    /* If exists, make sure filetype acceptable. */
    if (exists) {
        if (finfo.filetype == APR_DIR) {
            if (!*is_directory) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "object_id " AFW_UTF8_FMT_Q
                    " is directory so must end with '/'",
                    AFW_UTF8_FMT_OPTIONAL_ARG(object_id));
            }
        }
        else if (finfo.filetype == APR_REG) {
            if (*is_directory) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "object_id " AFW_UTF8_FMT_Q
                    " is a regular file so must not end with '/'",
                    AFW_UTF8_FMT_OPTIONAL_ARG(object_id));
            }
        }
        else {
            AFW_THROW_ERROR_FZ(general, xctx,
                "object_id " AFW_UTF8_FMT_Q
                " filetype is not allowed",
                AFW_UTF8_FMT_OPTIONAL_ARG(object_id));
        }
    }
}


static void
impl_write_data_to_file(
    afw_vfs_adaptor_internal_session_t *self,
    const afw_utf8_t *object_id,
    const afw_utf8_z_t *path_z,
    const afw_value_t *data,
    afw_boolean_t is_create,
    afw_xctx_t *xctx)
{
    const afw_vfs_adaptor_internal_t *adaptor =
        (const afw_vfs_adaptor_internal_t *)self->pub.adaptor;
    const afw_utf8_utf8_z_t *pattern;
    const afw_utf8_z_t *vfs_path_z;
    const void *buf;
    apr_size_t nbytes;
    apr_size_t bytes_written;
    apr_file_t *fd;
    apr_int32_t flag;
    apr_status_t rv;

    if (!data ||
        (!afw_value_is_string(data) && !afw_value_is_hexBinary(data)))
    {
        AFW_THROW_ERROR_Z(general,
            "object must have a \"data\" property that is data type "
            "string or hexBinary for vfs adaptor",
            xctx);
    }

    /* Flag is write/create plus binary if not string data. */
    flag = APR_FOPEN_WRITE | APR_FOPEN_CREATE;
    if (!afw_value_is_string(data)) {
        flag |= APR_FOPEN_BINARY;
    }

    /* If is create, set that in flag. */
    if (is_create) {
        flag |= APR_FOPEN_CREATE;
    }

    /* Open file. */
    rv = apr_file_open(&fd, path_z, flag, APR_FPROT_OS_DEFAULT,
        afw_pool_get_apr_pool(xctx->p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "apr_file_open() error",
            xctx);
    }

    /* Write full file. */
    if (afw_value_is_string(data)) {
        buf = (const void *)((const afw_value_string_t *)data)->internal.s;
        nbytes = (apr_size_t)((const afw_value_string_t *)data)->internal.len;
    }
    else {
        buf = (const void *)((const afw_value_hexBinary_t *)data)->internal.ptr;
        nbytes = (apr_size_t)((const afw_value_hexBinary_t *)data)->internal.size;
    }
    rv = apr_file_write_full(fd, buf, nbytes, &bytes_written);
    apr_file_close(fd);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "apr_file_write_full() error",
            xctx);
    }

    /* Make file executable if match in adaptor->mark_executable. */
    if (adaptor->mark_executable) {
        vfs_path_z = afw_utf8_z_printf(self->pub.p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->pub.adaptor_id),
            AFW_UTF8_FMT_ARG(object_id));
        for (pattern = adaptor->mark_executable; pattern->s_z; pattern++) {
            if (apr_fnmatch(pattern->s_z, vfs_path_z, 0) ==
                APR_SUCCESS)
            {
                rv = apr_file_attrs_set(path_z,
                    APR_FILE_ATTR_EXECUTABLE, APR_FILE_ATTR_EXECUTABLE,
                    afw_pool_get_apr_pool(xctx->p));
                if (rv != APR_SUCCESS && rv != APR_ENOTIMPL) {
                    AFW_THROW_ERROR_RV_Z(general, apr, rv,
                        "apr_file_attrs_set() error",
                        xctx);
                }
                break;
            }
        }
    }
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
    afw_vfs_adaptor_internal_session_t *self =
        (afw_vfs_adaptor_internal_session_t *)instance;
    afw_utf8_utf8_z_t path;
    const afw_value_t *data;
    apr_status_t rv;
    afw_boolean_t is_directory;

    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed by vfs adaptor",
            xctx);
    }

    /* Determine path and check existence. */
    impl_determine_path_for_object_id(
        self, suggested_object_id, false,
        &path, &is_directory,
        xctx->p, xctx);


    /* If directory, make it recursively. Property data is ignored. */
    if (is_directory) {

        /* Make directory.*/
        rv = apr_dir_make_recursive(path.s_z, APR_FPROT_OS_DEFAULT,
            afw_pool_get_apr_pool(xctx->p));
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv,
                "apr_dir_make_recursive() error",
                xctx);
        }
    }

    /* If regular file, get data property and write to file. */
    else {
        data = afw_object_get_property(object, afw_vfs_s_data, xctx);
        impl_write_data_to_file(self,
            suggested_object_id, path.s_z, data, true, xctx);
    }

    /* The suggested_object_id is the actual one. */
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
    afw_vfs_adaptor_internal_session_t *self =
        (afw_vfs_adaptor_internal_session_t *)instance;
    afw_utf8_utf8_z_t path;
    afw_boolean_t is_directory;
    afw_boolean_t valid;


    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed by vfs adaptor",
            xctx);
    }

    /* Determine path and check existence. */
    impl_determine_path_for_object_id(
        self, object_id, true,
        &path, &is_directory,
        xctx->p, xctx);

    /* Can't modify a directory. */
    if (is_directory) {
        AFW_THROW_ERROR_Z(general,
            "modify_object() is not allowed for a directory by vfs adaptor",
            xctx);
    }

    /* Only [["set_property", "data", "<file context>"]] is supported. */
    if (!*entry) {
        return;
    }
    valid = true;
    if ((*entry)->type != afw_adaptor_modify_entry_type_set_property) {
        valid = false;
    }
    else if (!(*entry)->first_property_name_entry ||
        (*entry)->first_property_name_entry->next)
    {
        valid = false;
    }
    else if (
        !afw_utf8_equal(
            &(*entry)->first_property_name_entry->property_name,
            afw_vfs_s_data))
    {
        valid = false;
    }
    else if (*(entry + 1)) {
        valid = false;
    }
    if (!valid) {
        AFW_THROW_ERROR_Z(general,
            "modify_object() only supports "
            "[[\"set_property\", \"data\", value]] for vfs adaptor",
            xctx);
    }

    /* Write data to file. */
    impl_write_data_to_file(self,
        object_id, path.s_z, (*entry)->value, false, xctx);
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
    afw_vfs_adaptor_internal_session_t *self =
        (afw_vfs_adaptor_internal_session_t *)instance;
    afw_utf8_utf8_z_t path;
    const afw_value_t *data;
    afw_boolean_t is_directory;

    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed by vfs adaptor",
            xctx);
    }

    /* Determine path and check existence. */
    impl_determine_path_for_object_id(
        self, object_id, true,
        &path, &is_directory,
        xctx->p, xctx);

    /* Can't replace a directory. */
    if (is_directory) {
        AFW_THROW_ERROR_Z(general,
            "replace_object() is not allowed for a directory by vfs adaptor",
            xctx);
    }

    /* Write data property and write to file. */
    data = afw_object_get_property(replacement_object, afw_vfs_s_data, xctx);
    impl_write_data_to_file(self,
        object_id, path.s_z, data, false, xctx);
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
    afw_vfs_adaptor_internal_session_t *self =
        (afw_vfs_adaptor_internal_session_t *)instance;
    afw_utf8_utf8_z_t path;
    apr_status_t rv;
    afw_boolean_t is_directory;

    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed",
            xctx);
    }

    /* Determine path and check existence. */
    impl_determine_path_for_object_id(
        self, object_id, true,
        &path, &is_directory,
        xctx->p, xctx);

    /* If directory, remove it. */
    if (is_directory) {
        rv = apr_dir_remove(path.s_z, afw_pool_get_apr_pool(xctx->p));
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv,
                "apr_dir_remove() error",
                xctx);
        }
    }

    /* If regular file, remove it. */
    else {
        rv = apr_file_remove(path.s_z, afw_pool_get_apr_pool(xctx->p));
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv,
                "apr_file_remove() error",
                xctx);
        }
    }
}



/*
 * Implementation of method begin_transaction for interface
 * afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_journal_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_journal_t *
impl_afw_adaptor_session_get_journal_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_key_value_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_key_value_t *
impl_afw_adaptor_session_get_key_value_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_index_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_impl_index_t *
impl_afw_adaptor_session_get_index_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
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
    return NULL;
}

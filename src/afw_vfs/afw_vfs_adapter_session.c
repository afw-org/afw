/* Interface afw_adapter_session Implementation for afw_vfs        */
// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_vfs_adapter_session.c
 * @brief Implementation of afw_adapter_session interface for afw_vfs
 */

#include "afw.h"
#include "afw_adapter_impl.h"
#include "afw_vfs_adapter_internal.h"
#include <apr_fnmatch.h>
#include <apr_file_info.h>


/* Declares and rti/inf defines for interface afw_adapter_session */
#define AFW_IMPLEMENTATION_ID "vfs"
#define AFW_ADAPTER_SESSION_SELF_T afw_vfs_adapter_internal_session_t
#include "afw_adapter_session_impl_declares.h"


/*
 * APR end-of-directory: ENOENT. Some Windows APR builds surface
 * ERROR_NO_MORE_FILES as a packed status (historically seen as 720018).
 */
static afw_boolean_t
impl_dir_read_done(apr_status_t rv)
{
    if (rv == APR_SUCCESS) {
        return false;
    }
    if (APR_STATUS_IS_ENOENT(rv)) {
        return true;
    }
#if defined(_WIN32) || defined(WIN32)
    if (rv == 720018 || rv == APR_FROM_OS_ERROR(18)) {
        return true;
    }
#endif
    return false;
}



/*
 * Reject objectId path segments '.' / '..', empty segments (except trailing
 * slash for directories), and backslash. VFS is not a full security boundary
 * against hostile host FS content (e.g. symlinks under a map root).
 */
static void
impl_validate_object_id(
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;
    const afw_utf8_octet_t *seg;
    afw_size_t seglen;

    if (afw_utf8_contains(object_id, afw_s_a_backslash)) {
        AFW_THROW_ERROR_Z(general,
            "object_id cannot contain backslash",
            xctx);
    }

    s = object_id->s;
    end = s + object_id->len;
    while (s < end) {
        seg = s;
        while (s < end && *s != '/') {
            s++;
        }
        seglen = (afw_size_t)(s - seg);
        if (seglen == 0) {
            /* Trailing slash (directory) is OK; empty middle segment is not. */
            if (s < end) {
                AFW_THROW_ERROR_Z(general,
                    "object_id cannot contain empty path segments",
                    xctx);
            }
            break;
        }
        if (seglen == 1 && seg[0] == '.') {
            AFW_THROW_ERROR_Z(general,
                "object_id cannot contain '.' path segment",
                xctx);
        }
        if (seglen == 2 && seg[0] == '.' && seg[1] == '.') {
            AFW_THROW_ERROR_Z(general,
                "object_id cannot contain '..' path segment",
                xctx);
        }
        if (s < end) {
            s++; /* skip '/' */
        }
    }
}



/*
 * True if object_id matches vfsMap prefix with a path boundary (same idea as
 * rootFilePaths impl_prefix_matches in afw_file_path.c / issue #103).
 * Empty prefix is the vfs root and matches every objectId.
 */
static afw_boolean_t
impl_prefix_matches(
    const afw_utf8_t *object_id,
    const afw_utf8_t *prefix)
{
    if (prefix->len == 0) {
        return true;
    }
    if (!afw_utf8_starts_with(object_id, prefix)) {
        return false;
    }
    if (object_id->len == prefix->len) {
        return true;
    }
    /*
     * Conf requires non-empty prefixes to end with '/'. starts_with then
     * already implies a path boundary. Keep an explicit check for safety if
     * a prefix without trailing slash ever appears.
     */
    if (prefix->s[prefix->len - 1] == '/') {
        return true;
    }
    return object_id->s[prefix->len] == '/';
}



/* True if candidate is equal to root or strictly under root + '/'. */
static afw_boolean_t
impl_path_is_under_root(
    const char *candidate_z,
    const char *root_z)
{
    size_t root_len;
    size_t cand_len;

    if (!candidate_z || !root_z) {
        return false;
    }
    root_len = strlen(root_z);
    cand_len = strlen(candidate_z);
    /*
     * Compare without a trailing slash on root so root "/tmp/afw/" and
     * candidate "/tmp/afw/temp/x" share prefix "/tmp/afw" with boundary '/'.
     */
    while (root_len > 1 &&
        (root_z[root_len - 1] == '/'
#if defined(_WIN32) || defined(WIN32)
            || root_z[root_len - 1] == '\\'
#endif
            ))
    {
        root_len--;
    }
    if (cand_len < root_len) {
        return false;
    }
    if (strncmp(candidate_z, root_z, root_len) != 0) {
        return false;
    }
    if (cand_len == root_len) {
        return true;
    }
    return candidate_z[root_len] == '/'
#if defined(_WIN32) || defined(WIN32)
        || candidate_z[root_len] == '\\'
#endif
        ;
}



/*
 * Build host path under vfs_entry root using APR SECUREROOT (aligned with
 * rootFilePaths resolution). Host roots are canonicalized at adapter create.
 */
static const afw_utf8_z_t *
impl_resolve_host_path(
    const afw_key_z_string_z_t *vfs_entry,
    const afw_utf8_t *object_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t adjusted;
    const afw_utf8_z_t *addpath_z;
    const afw_utf8_z_t *root_z;
    char *merged_z;
    apr_pool_t *apr_p;
    apr_status_t rv;
    afw_size_t add_len;
    afw_boolean_t want_trailing_slash;

    impl_validate_object_id(object_id, xctx);

    root_z = vfs_entry->string_z;
    want_trailing_slash = afw_utf8_ends_with(object_id, afw_s_a_slash);

    adjusted.s = object_id->s + vfs_entry->key.len;
    adjusted.len = object_id->len - vfs_entry->key.len;
    if (adjusted.len == 0) {
        return root_z;
    }

    add_len = adjusted.len;
    /* apr_filepath_merge prefers relative addpath without leading slash. */
    if (add_len > 0 && adjusted.s[0] == '/') {
        adjusted.s++;
        add_len--;
    }
    /* Drop trailing slash for merge; restore for directories after. */
    if (add_len > 0 && adjusted.s[add_len - 1] == '/') {
        add_len--;
    }

    apr_p = afw_pool_get_apr_pool(p);
    if (add_len == 0) {
        return root_z;
    }

    addpath_z = afw_utf8_z_create(adjusted.s, add_len, p, xctx);

    /*
     * Merge under root with SECUREROOT so ".." and absolute addpath cannot
     * escape. TRUENAME when the leaf exists; without TRUENAME for create.
     */
    rv = apr_filepath_merge(&merged_z, root_z, addpath_z,
        APR_FILEPATH_SECUREROOT | APR_FILEPATH_TRUENAME,
        apr_p);
    if (rv != APR_SUCCESS) {
        rv = apr_filepath_merge(&merged_z, root_z, addpath_z,
            APR_FILEPATH_SECUREROOT,
            apr_p);
    }
    if (rv != APR_SUCCESS) {
        /*
         * Last resort for unusual roots: concat after segment validation.
         * Prefer failing closed when containment cannot be proven later.
         */
        adjusted.s = object_id->s + vfs_entry->key.len;
        adjusted.len = object_id->len - vfs_entry->key.len;
        merged_z = (char *)afw_utf8_z_printf(p, xctx,
            AFW_UTF8_FMT AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&vfs_entry->string),
            AFW_UTF8_FMT_ARG(&adjusted));
    }

    if (!impl_path_is_under_root(merged_z, root_z)) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "object_id " AFW_UTF8_FMT_Q
            " resolves outside vfsMap host root %s",
            AFW_UTF8_FMT_OPTIONAL_ARG(object_id),
            root_z);
    }

    /* If object_id was a directory, ensure trailing slash on host path. */
    if (want_trailing_slash) {
        afw_size_t mlen = strlen(merged_z);
        if (mlen == 0 || merged_z[mlen - 1] != '/') {
            return afw_utf8_z_printf(p, xctx, "%s/", merged_z);
        }
    }

    return merged_z;
}



/*
 * Longest matching vfsMap prefix (explicit best-of scan, not first hit).
 * Mirrors rootFilePaths longest-prefix selection.
 */
static const afw_key_z_string_z_t *
impl_get_vfs_entry(
    const afw_vfs_adapter_internal_session_t *self,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    const afw_vfs_adapter_internal_t *adapter;
    const afw_key_z_string_z_t *e;
    const afw_key_z_string_z_t *best;

    (void)xctx;

    adapter = (const afw_vfs_adapter_internal_t *)self->pub.adapter;
    best = NULL;
    for (e = adapter->vfs_map; e->key_z; e++) {
        if (impl_prefix_matches(object_id, &e->key)) {
            if (!best || e->key.len > best->key.len) {
                best = e;
            }
        }
    }

    return best;
}



static const afw_object_t *
impl_read_file_object(
    const afw_vfs_adapter_internal_session_t *self,
    const afw_adapter_impl_request_t *impl_request,
    const afw_key_z_string_z_t *vfs_entry,
    const afw_utf8_t *object_id,
    afw_boolean_t include_hidden,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_vfs_adapter_internal_t *adapter;
    const afw_object_t *object;
    const afw_utf8_z_t *file_path_z;
    const afw_utf8_t *data_string;
    const afw_memory_t *data_binary;
    const afw_utf8_t *vfs_path;
    const afw_array_t *filenames;
    afw_value_dateTime_t *dateTime;
    apr_file_t *fd;
    afw_byte_t *buff;
    apr_dir_t *dir;
    afw_size_t size;
    apr_size_t size_read;
    apr_finfo_t finfo;
    apr_status_t rv;

    (void)impl_request;

    adapter = (const afw_vfs_adapter_internal_t *)self->pub.adapter;
    object = NULL;
    file_path_z = impl_resolve_host_path(vfs_entry, object_id, p, xctx);
    rv = apr_stat(&finfo, file_path_z, APR_FINFO_MIN, afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        /* If not found, return with NULL. */
        if (APR_STATUS_IS_ENOENT(rv)) {
            return NULL;
        }
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            "apr_stat() error for %s", file_path_z);
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
                file_path_z);
        }

        /* Process each file in directory. */
        object = afw_object_create_cede_p(p, xctx);
        afw_object_meta_set_ids(object,
            &self->pub.adapter->adapter_id,
            afw_vfs_s__AdaptiveFile_vfs,
            object_id,
            xctx);
        afw_object_set_property(object,
            afw_vfs_s_isDirectory, afw_boolean_v_true, xctx);
        vfs_path = afw_utf8_printf(p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&self->pub.adapter->adapter_id),
            AFW_UTF8_FMT_ARG(object_id));
        afw_object_set_property_as_anyURI(object,
            afw_vfs_s_vfsPath, vfs_path, xctx);
        filenames = afw_array_of_create(afw_data_type_string, p, xctx);
        afw_object_set_property_as_array(object,
            afw_vfs_s_data, filenames, xctx);
        for (;;) {

            /* Read next directory entry until there are no more.*/
            rv = apr_dir_read(&finfo, APR_FINFO_NAME | APR_FINFO_TYPE, dir);
            if (impl_dir_read_done(rv)) {
                break;
            }
            if (rv != APR_SUCCESS) {
                apr_dir_close(dir);
                AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                    "apr_dir_read() failed for %s", file_path_z);
            }

            /* Skip ./ and ../ always; skip other dot-names unless requested. */
            if (finfo.name[0] == '.') {
                if (strcmp(finfo.name, ".") == 0 ||
                    strcmp(finfo.name, "..") == 0)
                {
                    continue;
                }
                if (!include_hidden) {
                    continue;
                }
            }

            /* Directory. */
            if (finfo.filetype == APR_DIR) {
                size = strlen(finfo.name) + 1;
                buff = afw_pool_malloc(p, size, xctx);
                buff[size - 1] = '/';
                memcpy(buff, finfo.name, size - 1);
                data_string = afw_utf8_create(
                    (const afw_utf8_octet_t *)buff, size, p, xctx);
                afw_array_push_value(filenames,
                    afw_value_create_unmanaged_string(data_string, p, xctx),
                    xctx);
            }

            /* Regular file. */
            else if (finfo.filetype == APR_REG) {
                size = strlen(finfo.name);
                if (size == 0) {
                    continue;
                }
                buff = afw_pool_malloc(p, size, xctx);
                memcpy(buff, finfo.name, size);
                data_string = afw_utf8_create(
                    (const afw_utf8_octet_t *)buff, size, p, xctx);
                afw_array_push_value(filenames,
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
        size = (afw_size_t)finfo.size;

        /* Enforce maxReadBytes (0 = unlimited). */
        if (adapter->max_read_bytes != 0 &&
            size > adapter->max_read_bytes)
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                "File %s size " AFW_SIZE_T_FMT
                " exceeds maxReadBytes " AFW_SIZE_T_FMT,
                file_path_z, size, adapter->max_read_bytes);
        }

        /*
         * Empty files have size 0. afw_pool_malloc() rejects size 0, so skip
         * allocate/read and treat the content as empty. (issue #79)
         */
        buff = NULL;
        size_read = 0;
        if (size != 0) {
            rv = apr_file_open(&fd, file_path_z,
                APR_FOPEN_READ | APR_FOPEN_BINARY,
                APR_FPROT_OS_DEFAULT, afw_pool_get_apr_pool(p));
            if (rv != APR_SUCCESS) {
                AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                    "apr_file_open() failed for %s", file_path_z);
            }
            buff = afw_pool_malloc(p, size, xctx);
            size_read = (apr_size_t)size;
            rv = apr_file_read_full(fd, buff, size_read, &size_read);
            apr_file_close(fd);
            if (rv != APR_SUCCESS && rv != APR_EOF) {
                AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                    "Error reading %s", file_path_z);
            }
            if ((afw_size_t)size_read != size) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Short read of %s: expected " AFW_SIZE_T_FMT
                    " bytes, got " AFW_SIZE_T_FMT,
                    file_path_z, size, (afw_size_t)size_read);
            }
        }

        object = afw_object_create_cede_p(p, xctx);
        afw_object_meta_set_ids(object,
            &self->pub.adapter->adapter_id,
            afw_vfs_s__AdaptiveFile_vfs,
            object_id,
            xctx);
        afw_object_set_property(object,
            afw_vfs_s_isDirectory, afw_boolean_v_false, xctx);
        vfs_path = afw_utf8_printf(p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&self->pub.adapter->adapter_id),
            AFW_UTF8_FMT_ARG(object_id));
        afw_object_set_property_as_anyURI(object,
            afw_vfs_s_vfsPath, vfs_path, xctx);
        if (afw_utf8_is_valid((const afw_utf8_octet_t *)buff, size_read, xctx)) {
            data_string = afw_utf8_create((const afw_utf8_octet_t *)buff,
                size_read, p, xctx);
            afw_object_set_property_as_string(object,
                afw_vfs_s_data, data_string, xctx);
        }
        else {
            data_binary = afw_memory_create(buff, size_read, p, xctx);
            afw_object_set_property_as_hexBinary(object,
                afw_vfs_s_data, data_binary, xctx);
        }
    }

    /* If file type wasn't ignore, add time properties to object. */
    if (object &&
        (finfo.filetype == APR_DIR || finfo.filetype == APR_REG))
    {
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
    const afw_vfs_adapter_internal_session_t *self,
    afw_vfs_adapter_internal_session_context_t *ctx,
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
        if (impl_dir_read_done(rv)) {
            break;
        }
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_dir_read() failed.",
                xctx);
        }

        /* Always skip ./ and ../ plus hidden files unless requested. */
        if (*(finfo.name) == '.') {
            if (strcmp(finfo.name, ".") == 0 ||
                strcmp(finfo.name, "..") == 0)
            {
                continue;
            }
            if (!ctx->includeHidden) {
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
                object_id, ctx->includeHidden, object_p, xctx);
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
const afw_adapter_session_t *
afw_vfs_adapter_internal_session_create(
    afw_vfs_adapter_internal_t *adapter,
    afw_xctx_t *xctx)
{
    afw_vfs_adapter_internal_session_t *self;

    self = afw_xctx_calloc_type(
        afw_vfs_adapter_internal_session_t, xctx);
    self->pub.inf = &impl_afw_adapter_session_inf;
    self->pub.adapter = (const afw_adapter_t *)adapter;
    self->pub.p = xctx->p;

    /* Return new instance. */
    return (afw_adapter_session_t *)self;
}



/*
 * Implementation of method destroy for interface afw_adapter_session.
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
    const afw_vfs_adapter_internal_t *adapter =
        (const afw_vfs_adapter_internal_t *)self->pub.adapter;
    afw_vfs_adapter_internal_session_context_t ctx;
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
            afw_vfs_s__AdaptiveAdapterTypeSpecific_vfs_retrieve_objects,
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

    /* Adapter vfs specific properties. */
    if (adapter_type_specific) {

        /* includeHidden */
        ctx.includeHidden = afw_object_old_get_property_as_boolean(
            adapter_type_specific, afw_vfs_s_includeHidden, &found, xctx);

        /* subdirectory */
        subdirectory = afw_object_old_get_property_as_string(
            adapter_type_specific, afw_vfs_s_subdirectory, xctx);
        if (subdirectory &&
            (
                afw_utf8_starts_with(subdirectory, afw_s_a_slash) ||
                !afw_utf8_ends_with(subdirectory, afw_s_a_slash) ||
                afw_utf8_contains(subdirectory, afw_s_a_backslash) ||
                afw_utf8_contains(subdirectory, afw_s_a_dot_slash)
            ))
        {
            AFW_THROW_ERROR_Z(general,
                "The subdirectory property of the adapterTypeSpecific "
                "parameter of retrieve_objects() can not start with '/', "
                "must end with '/', and can not contain '\\', './' or '../'",
                xctx);
        }

        /* suffix */
        ctx.suffix = afw_object_old_get_property_as_string(
            adapter_type_specific, afw_vfs_s_suffix, xctx);

        /* recursive */
        ctx.recursive = afw_object_old_get_property_as_boolean(
            adapter_type_specific, afw_vfs_s_recursive, &found, xctx);
    }

    /*
     * Process map entries. When subdirectory is set, only entries whose
     * objectId prefix matches that path are used, and the remainder after
     * the prefix is appended to the host root (do not double-apply the
     * map key as a host subpath). Aligns multi-map with rootFilePaths-style
     * longest-prefix usage.
     */
    for (vfs_entry = adapter->vfs_map; vfs_entry->key_z; vfs_entry++)
    {
        if (subdirectory) {
            afw_utf8_t remainder;

            if (!impl_prefix_matches(subdirectory, &vfs_entry->key)) {
                continue;
            }
            remainder.s = subdirectory->s + vfs_entry->key.len;
            remainder.len = subdirectory->len - vfs_entry->key.len;

            subdirectory_vfs_entry.key_z = afw_utf8_z_create(
                subdirectory->s, subdirectory->len, p, xctx);
            subdirectory_vfs_entry.key.len = subdirectory->len;
            if (remainder.len == 0) {
                subdirectory_vfs_entry.string_z = vfs_entry->string_z;
                subdirectory_vfs_entry.string.len = vfs_entry->string.len;
            }
            else {
                subdirectory_vfs_entry.string_z = afw_utf8_z_printf(p, xctx,
                    AFW_UTF8_FMT AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(&vfs_entry->string),
                    AFW_UTF8_FMT_ARG(&remainder));
                subdirectory_vfs_entry.string.len =
                    strlen(subdirectory_vfs_entry.string_z);
            }
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
    const afw_object_t *object;
    const afw_pool_t *object_p;
    const afw_key_z_string_z_t *vfs_entry;
    afw_boolean_t include_hidden;
    afw_boolean_t found;

    /* Only object type _AdaptiveFile_vfs is supported. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        callback(NULL, context, xctx);
        return;
    }

    /*
     * Validate object_id path segments. On invalid path, callback NULL
     * (same as not found) rather than throwing — matches prior behavior for
     * get_object. Do not return from inside AFW_CATCH (corrupts try stack).
     */
    {
        afw_boolean_t invalid_object_id = false;

        AFW_TRY {
            impl_validate_object_id(object_id, xctx);
        }
        AFW_CATCH_UNHANDLED {
            invalid_object_id = true;
        }
        AFW_ENDTRY;

        if (invalid_object_id) {
            callback(NULL, context, xctx);
            return;
        }
    }

    include_hidden = false;
    if (adapter_type_specific) {
        include_hidden = afw_object_old_get_property_as_boolean(
            adapter_type_specific, afw_vfs_s_includeHidden, &found, xctx);
    }

    /* Get object or NULL and pass it to callback. */
    object = NULL;
    vfs_entry = impl_get_vfs_entry(self, object_id, xctx);
    if (vfs_entry) {
        object_p = afw_pool_create(p, xctx);
        object = impl_read_file_object(self, impl_request,
            vfs_entry, object_id, include_hidden, object_p, xctx);
        if (!object) {
            afw_pool_release(object_p, xctx);
        }
    }
    callback(object, context, xctx);
}



static void
impl_determine_path_for_object_id(
    afw_vfs_adapter_internal_session_t *self,
    const afw_utf8_t *object_id,
    afw_boolean_t expect_exists,
    afw_utf8_utf8_z_t *path,
    afw_boolean_t *is_directory,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_key_z_string_z_t *vfs_entry;
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

    /* Construct path under map root with segment checks. */
    path->s_z = impl_resolve_host_path(vfs_entry, object_id, p, xctx);
    path->s.len = strlen(path->s_z);
    path->s.s = path->s_z;

    /* Get if path exists and make sure is_directory is correct. */
    rv = apr_stat(&finfo, path->s_z, APR_FINFO_TYPE, afw_pool_get_apr_pool(p));
    exists = true;
    if (APR_STATUS_IS_ENOENT(rv)) {
        exists = false;
    }
    else if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            "apr_stat() error for %s", path->s_z);
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
            AFW_THROW_ERROR_FZ(conflict, xctx,
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
    afw_vfs_adapter_internal_session_t *self,
    const afw_utf8_t *object_id,
    const afw_utf8_z_t *path_z,
    const afw_value_t *data,
    afw_boolean_t is_create,
    afw_xctx_t *xctx)
{
    const afw_vfs_adapter_internal_t *adapter =
        (const afw_vfs_adapter_internal_t *)self->pub.adapter;
    const afw_utf8_utf8_z_t *pattern;
    const afw_utf8_z_t *vfs_path_z;
    const afw_utf8_z_t *tmp_path_z;
    const void *buf;
    apr_size_t nbytes;
    apr_size_t bytes_written;
    apr_file_t *fd;
    apr_int32_t flag;
    apr_status_t rv;
    apr_pool_t *apr_p;

    /* Omitted data defaults to empty string (matches object type default). */
    if (!data) {
        data = afw_v_a_empty_string;
    }

    if (!afw_value_is_string(data) && !afw_value_is_hexBinary(data)) {
        AFW_THROW_ERROR_Z(general,
            "object must have a \"data\" property that is data type "
            "string or hexBinary for vfs adapter",
            xctx);
    }

    if (afw_value_is_string(data)) {
        buf = (const void *)((const afw_value_string_t *)data)->internal.s;
        nbytes = (apr_size_t)((const afw_value_string_t *)data)->internal.len;
    }
    else {
        buf = (const void *)((const afw_value_hexBinary_t *)data)->internal.ptr;
        nbytes = (apr_size_t)((const afw_value_hexBinary_t *)data)->internal.size;
        if (nbytes > 0 && !buf) {
            AFW_THROW_ERROR_Z(general,
                "hexBinary data pointer is NULL",
                xctx);
        }
    }

    apr_p = afw_pool_get_apr_pool(xctx->p);

    /*
     * Atomic full-file write: write to a temp sibling, then rename into place
     * so a failed write does not leave a truncated destination.
     */
    tmp_path_z = afw_utf8_z_printf(xctx->p, xctx, "%s.afw-tmp", path_z);
    flag = APR_FOPEN_WRITE | APR_FOPEN_CREATE | APR_FOPEN_TRUNCATE |
        APR_FOPEN_BINARY | APR_FOPEN_EXCL;
    rv = apr_file_open(&fd, tmp_path_z, flag, APR_FPROT_OS_DEFAULT, apr_p);
    if (rv != APR_SUCCESS) {
        /* Retry without EXCL if a prior temp was left behind. */
        apr_file_remove(tmp_path_z, apr_p);
        flag = APR_FOPEN_WRITE | APR_FOPEN_CREATE | APR_FOPEN_TRUNCATE |
            APR_FOPEN_BINARY;
        rv = apr_file_open(&fd, tmp_path_z, flag, APR_FPROT_OS_DEFAULT, apr_p);
    }
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            "apr_file_open() error for temp %s", tmp_path_z);
    }

    rv = apr_file_write_full(fd, buf ? buf : "", nbytes, &bytes_written);
    apr_file_close(fd);
    if (rv != APR_SUCCESS) {
        apr_file_remove(tmp_path_z, apr_p);
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            "apr_file_write_full() error for %s", tmp_path_z);
    }

    /*
     * On Windows, rename fails if the destination exists; remove first when
     * replacing. On POSIX rename replaces atomically.
     */
    if (!is_create) {
#if defined(_WIN32) || defined(WIN32)
        apr_file_remove(path_z, apr_p);
#endif
    }
    rv = apr_file_rename(tmp_path_z, path_z, apr_p);
    if (rv != APR_SUCCESS) {
#if !defined(_WIN32) && !defined(WIN32)
        /* If create raced, fail without clobbering. */
        if (is_create) {
            apr_file_remove(tmp_path_z, apr_p);
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "apr_file_rename() error creating %s", path_z);
        }
        /* Replace: try remove+rename if needed. */
        apr_file_remove(path_z, apr_p);
        rv = apr_file_rename(tmp_path_z, path_z, apr_p);
#endif
        if (rv != APR_SUCCESS) {
            apr_file_remove(tmp_path_z, apr_p);
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "apr_file_rename() error for %s", path_z);
        }
    }

    /* Make file executable if match in adapter->mark_executable. */
    if (adapter->mark_executable) {
        vfs_path_z = afw_utf8_z_printf(self->pub.p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adapter->pub.adapter_id),
            AFW_UTF8_FMT_ARG(object_id));
        for (pattern = adapter->mark_executable; pattern->s_z; pattern++) {
            if (apr_fnmatch(pattern->s_z, vfs_path_z, 0) ==
                APR_SUCCESS)
            {
                rv = apr_file_attrs_set(path_z,
                    APR_FILE_ATTR_EXECUTABLE, APR_FILE_ATTR_EXECUTABLE,
                    apr_p);
                if (rv != APR_SUCCESS && rv != APR_ENOTIMPL) {
                    AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                        "apr_file_attrs_set() error for %s", path_z);
                }
                break;
            }
        }
    }
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
    afw_utf8_utf8_z_t path;
    const afw_value_t *data;
    apr_status_t rv;
    afw_boolean_t is_directory;

    (void)impl_request;
    (void)adapter_type_specific;

    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed by vfs adapter",
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
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "apr_dir_make_recursive() error for %s", path.s_z);
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
    afw_utf8_utf8_z_t path;
    afw_boolean_t is_directory;
    afw_boolean_t valid;

    (void)impl_request;
    (void)adapter_type_specific;

    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed by vfs adapter",
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
            "modify_object() is not allowed for a directory by vfs adapter",
            xctx);
    }

    /* Only [["set_property", "data", "<file context>"]] is supported. */
    if (!*entry) {
        return;
    }
    valid = true;
    if ((*entry)->type != afw_adapter_modify_entry_type_set_property) {
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
            "[[\"set_property\", \"data\", value]] for vfs adapter",
            xctx);
    }

    /* Write data to file. */
    impl_write_data_to_file(self,
        object_id, path.s_z, (*entry)->value, false, xctx);
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
    afw_utf8_utf8_z_t path;
    const afw_value_t *data;
    afw_boolean_t is_directory;

    (void)impl_request;
    (void)adapter_type_specific;

    /* Only object type _AdaptiveFile_vfs is allowed. */
    if (!afw_utf8_equal(object_type_id, afw_vfs_s__AdaptiveFile_vfs)) {
        AFW_THROW_ERROR_Z(general,
            "Only object type _AdaptiveFile_vfs is allowed by vfs adapter",
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
            "replace_object() is not allowed for a directory by vfs adapter",
            xctx);
    }

    /* Write data property and write to file (omitted data => empty). */
    data = afw_object_get_property(replacement_object, afw_vfs_s_data, xctx);
    impl_write_data_to_file(self,
        object_id, path.s_z, data, false, xctx);
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
    afw_utf8_utf8_z_t path;
    apr_status_t rv;
    afw_boolean_t is_directory;

    (void)impl_request;
    (void)adapter_type_specific;

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

    /* If directory, remove it (non-recursive; must be empty). */
    if (is_directory) {
        rv = apr_dir_remove(path.s_z, afw_pool_get_apr_pool(xctx->p));
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "apr_dir_remove() error for %s (directory must be empty)",
                path.s_z);
        }
    }

    /* If regular file, remove it. */
    else {
        rv = apr_file_remove(path.s_z, afw_pool_get_apr_pool(xctx->p));
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "apr_file_remove() error for %s", path.s_z);
        }
    }
}



/*
 * Implementation of method begin_transaction for interface
 * afw_adapter_session.
 */
const afw_adapter_transaction_t *
impl_afw_adapter_session_begin_transaction(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_journal_interface for interface
 * afw_adapter_session.
 */
const afw_adapter_journal_t *
impl_afw_adapter_session_get_journal_interface(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_key_value_interface for interface
 * afw_adapter_session.
 */
const afw_adapter_key_value_t *
impl_afw_adapter_session_get_key_value_interface(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_index_interface for interface
 * afw_adapter_session.
 */
const afw_adapter_impl_index_t *
impl_afw_adapter_session_get_index_interface(
    AFW_ADAPTER_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
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
    return NULL;
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework rootFilePaths path resolution
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_file_path.c
 * @brief Resolve logical paths using application rootFilePaths.
 */

#include "afw_internal.h"
#include <apr_file_info.h>
#include <apr_strings.h>


/*
 * True if logical_path matches prefix with a path boundary:
 * path equals prefix, or continues with '/'.
 * Prevents property "include" from matching path "includes/...".
 */
static afw_boolean_t
impl_prefix_matches(
    const afw_utf8_t *logical_path,
    const afw_utf8_t *prefix)
{
    if (!afw_utf8_starts_with(logical_path, prefix)) {
        return false;
    }
    if (logical_path->len == prefix->len) {
        return true;
    }
    return logical_path->s[prefix->len] == '/';
}



/*
 * Reject remainder segments that are ".." (before filesystem merge).
 * remainder may start with '/'.
 */
static afw_boolean_t
impl_remainder_has_dotdot(const afw_utf8_t *remainder)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;
    const afw_utf8_octet_t *seg;
    afw_size_t seglen;

    s = remainder->s;
    end = remainder->s + remainder->len;
    while (s < end) {
        while (s < end && *s == '/') {
            s++;
        }
        if (s >= end) {
            break;
        }
        seg = s;
        while (s < end && *s != '/') {
            s++;
        }
        seglen = (afw_size_t)(s - seg);
        if (seglen == 2 && seg[0] == '.' && seg[1] == '.') {
            return true;
        }
    }
    return false;
}



/* Resolve host root directory to an absolute real path (must exist). */
static const char *
impl_canonicalize_root_dir_z(
    const afw_utf8_t *host_root,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const char *host_root_z;
    char *merged_z;
    char *real_z;
    apr_status_t rv;
    apr_pool_t *apr_p;

    apr_p = afw_pool_get_apr_pool(p);
    host_root_z = afw_utf8_to_utf8_z(host_root, p, xctx);

    /* Absolutize relative roots against CWD; TRUENAME requires existence. */
    rv = apr_filepath_merge(&merged_z, NULL, host_root_z,
        APR_FILEPATH_TRUENAME | APR_FILEPATH_NOTRELATIVE,
        apr_p);
    if (rv != APR_SUCCESS) {
        /* Retry without NOTRELATIVE for relative roots that need CWD. */
        rv = apr_filepath_merge(&merged_z, NULL, host_root_z,
            APR_FILEPATH_TRUENAME,
            apr_p);
    }
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(not_found, apr, rv, xctx,
            "rootFilePaths host directory " AFW_UTF8_FMT_Q
            " could not be resolved (must exist as a directory)",
            AFW_UTF8_FMT_ARG(host_root));
    }

    real_z = merged_z;
    return real_z;
}



/* True if candidate is equal to root or strictly under root + '/'. */
static afw_boolean_t
impl_path_is_under_root(
    const char *candidate_z,
    const char *root_z)
{
    size_t root_len;
    size_t cand_len;

    root_len = strlen(root_z);
    cand_len = strlen(candidate_z);

    /*
     * Directory roots may include a trailing slash (e.g. from
     * afw_file_insure_full_path). Strip trailing slashes for the
     * prefix/boundary check so '/tmp/data/' still matches
     * '/tmp/data/file.txt'.
     */
    while (root_len > 1 && root_z[root_len - 1] == '/') {
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
    /* Boundary: next char must be separator (avoid /tmp/afw vs /tmp/afw-evil). */
    return candidate_z[root_len] == '/';
}



/**
 * Resolve logical path using env->root_file_paths.
 *
 * Longest prefix match, absolute/relative host roots, traversal hardening.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_file_path_resolve_rootFilePaths(
    const afw_utf8_t *logical_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *roots;
    const afw_iterator_old_t *iterator = NULL;
    const afw_value_t *property_name;
    const afw_utf8_t *property_value;
    const afw_utf8_t *best_prefix;
    const afw_utf8_t *best_host;
    afw_utf8_t remainder;
    const char *root_z;
    char *merged_z;
    char *addpath_z;
    apr_status_t rv;
    apr_pool_t *apr_p;
    const afw_utf8_octet_t *rem_s;
    afw_size_t rem_len;

    if (!logical_path || logical_path->len == 0) {
        AFW_THROW_ERROR_Z(not_found,
            "Failed to resolve file location: empty path.", xctx);
    }

    /* Defensive: embedded NUL not allowed in logical path. */
    if (memchr(logical_path->s, '\0', logical_path->len)) {
        AFW_THROW_ERROR_Z(bad_request,
            "Failed to resolve file location: invalid path.", xctx);
    }

    roots = xctx->env->root_file_paths;
    if (!roots) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Failed to resolve file location '%.*s' "
            "(application rootFilePaths not configured).",
            (int)logical_path->len, logical_path->s);
    }

    /* Longest matching prefix among rootFilePaths properties. */
    best_prefix = NULL;
    best_host = NULL;
    iterator = NULL;
    property_value = afw_object_get_next_property_as_string_internal(roots,
        &iterator, &property_name, p, xctx);
    while (property_value) {
        const afw_utf8_t *name_utf8 =
            afw_object_string_property_name_as_utf8(property_name, xctx);
        if (impl_prefix_matches(logical_path, name_utf8)) {
            if (!best_prefix || name_utf8->len > best_prefix->len) {
                best_prefix = name_utf8;
                best_host = property_value;
            }
        }
        property_value = afw_object_get_next_property_as_string_internal(roots,
            &iterator, &property_name, p, xctx);
    }

    if (!best_prefix) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Failed to resolve file location '%.*s'.",
            (int)logical_path->len, logical_path->s);
    }

    remainder.s = logical_path->s + best_prefix->len;
    remainder.len = logical_path->len - best_prefix->len;

    if (impl_remainder_has_dotdot(&remainder)) {
        AFW_THROW_ERROR_FZ(bad_request, xctx,
            "Failed to resolve file location '%.*s': path traversal rejected.",
            (int)logical_path->len, logical_path->s);
    }

    /*
     * Remainder after a proper prefix match is either empty or starts with '/'.
     * Strip leading '/' for apr_filepath_merge relative addpath.
     */
    rem_s = remainder.s;
    rem_len = remainder.len;
    while (rem_len > 0 && *rem_s == '/') {
        rem_s++;
        rem_len--;
    }

    apr_p = afw_pool_get_apr_pool(p);
    root_z = impl_canonicalize_root_dir_z(best_host, p, xctx);

    if (rem_len == 0) {
        /* Logical path was exactly the root key — host root directory. */
        if (!impl_path_is_under_root(root_z, root_z)) {
            AFW_THROW_ERROR_Z(general,
                "Internal error resolving rootFilePaths.", xctx);
        }
        return afw_utf8_create(root_z, AFW_UTF8_Z_LEN, p, xctx);
    }

    addpath_z = apr_pstrndup(apr_p, (const char *)rem_s, rem_len);

    /*
     * Merge under root with SECUREROOT so ".." and absolute addpath cannot
     * escape. TRUENAME when possible; if the leaf does not exist yet (create
     * modes), merge without TRUENAME then re-check containment of parent.
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
        AFW_THROW_ERROR_RV_FZ(bad_request, apr, rv, xctx,
            "Failed to resolve file location '%.*s' under rootFilePaths.",
            (int)logical_path->len, logical_path->s);
    }

    if (!impl_path_is_under_root(merged_z, root_z)) {
        AFW_THROW_ERROR_FZ(bad_request, xctx,
            "Failed to resolve file location '%.*s': "
            "path escapes rootFilePaths root.",
            (int)logical_path->len, logical_path->s);
    }

    return afw_utf8_create(merged_z, AFW_UTF8_Z_LEN, p, xctx);
}

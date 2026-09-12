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
#include <errno.h>
#include <string.h>


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
     * Directory roots may include a trailing slash (e.g. from
     * afw_file_insure_full_path). Strip trailing slashes for the
     * prefix/boundary check so '/tmp/data/' still matches
     * '/tmp/data/file.txt'.
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
    /* Boundary: next char must be separator (avoid /tmp/afw vs /tmp/afw-evil). */
    return candidate_z[root_len] == '/'
#if defined(_WIN32) || defined(WIN32)
        || candidate_z[root_len] == '\\'
#endif
        ;
}



/*
 * Join addpath under root lexically. Collapse extra slashes and '.' .
 * '..' pops a segment but will not leave root. Returns pool-allocated
 * 0-terminated path, or NULL on escape.
 */
static char *
impl_join_lexical(
    const char *root_z,
    const char *addpath_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    char *dst;
    afw_size_t cap;
    afw_size_t root_len;
    afw_size_t add_len;
    afw_size_t jail;
    afw_size_t n;
    const char *s;
    const char *seg;
    afw_size_t seglen;

    root_len = strlen(root_z);
    add_len = addpath_z ? strlen(addpath_z) : 0;
    cap = root_len + add_len + 2;
    dst = afw_pool_malloc(p, cap, xctx);

    memcpy(dst, root_z, root_len);
    n = root_len;
    while (n > 1 && dst[n - 1] == '/') {
        n--;
    }
    jail = n;
    dst[n] = 0;

    if (!addpath_z || add_len == 0) {
        return dst;
    }

    s = addpath_z;
    while (*s) {
        while (*s == '/') {
            s++;
        }
        if (!*s) {
            break;
        }
        seg = s;
        while (*s && *s != '/') {
            s++;
        }
        seglen = (afw_size_t)(s - seg);

        if (seglen == 1 && seg[0] == '.') {
            continue;
        }
        if (seglen == 2 && seg[0] == '.' && seg[1] == '.') {
            if (n <= jail) {
                return NULL;
            }
            while (n > jail && dst[n - 1] != '/') {
                n--;
            }
            if (n > jail) {
                n--; /* drop slash */
            }
            dst[n] = 0;
            continue;
        }

        if (n == 0 || dst[n - 1] != '/') {
            dst[n++] = '/';
        }
        memcpy(dst + n, seg, seglen);
        n += seglen;
        dst[n] = 0;
    }

    return dst;
}



/* Canonicalize path to an absolute real path (must exist). */
AFW_DEFINE(const afw_utf8_t *)
afw_file_path_canonicalize(
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_z_t *path_z;
    const afw_utf8_t *real;
    int err;

    path_z = afw_utf8_to_utf8_z(path, p, xctx);
    real = afw_os_realpath(path_z, p, xctx);
    if (!real) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "Unresolvable path %s", path_z);
    }
    return real;
}



/* Join addpath under root; result stays under root. */
AFW_DEFINE(const afw_utf8_t *)
afw_file_path_join_under_root(
    const afw_utf8_t *root,
    const afw_utf8_t *addpath,
    afw_boolean_t trailing_slash,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_z_t *root_z;
    const afw_utf8_z_t *add_z;
    char *joined_z;
    const afw_utf8_t *real;
    const afw_utf8_t *result;
    const afw_utf8_z_t *check_z;

    if (!root || root->len == 0) {
        AFW_THROW_ERROR_Z(bad_request,
            "join under root: empty root", xctx);
    }

    root_z = afw_utf8_to_utf8_z(root, p, xctx);
    if (!addpath || addpath->len == 0) {
        add_z = "";
    }
    else {
        add_z = afw_utf8_to_utf8_z(addpath, p, xctx);
    }

    joined_z = impl_join_lexical(root_z, add_z, p, xctx);
    if (!joined_z) {
        AFW_THROW_ERROR_FZ(bad_request, xctx,
            "path escapes root '%s'", root_z);
    }

    real = afw_os_realpath(joined_z, p, xctx);
    if (real) {
        check_z = afw_utf8_to_utf8_z(real, p, xctx);
        if (!impl_path_is_under_root(check_z, root_z)) {
            AFW_THROW_ERROR_FZ(bad_request, xctx,
                "path escapes root '%s'", root_z);
        }
        result = real;
    }
    else {
        if (!impl_path_is_under_root(joined_z, root_z)) {
            AFW_THROW_ERROR_FZ(bad_request, xctx,
                "path escapes root '%s'", root_z);
        }
        result = afw_utf8_create(joined_z, AFW_UTF8_Z_LEN, p, xctx);
    }

    if (trailing_slash &&
        (result->len == 0 || result->s[result->len - 1] != '/'))
    {
        result = afw_utf8_concat(p, xctx, result, afw_s_a_slash, NULL);
    }
    return result;
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
    const afw_utf8_t *root;
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
        &iterator, &property_name, xctx);
    while (property_value) {
        const afw_utf8_t *name_utf8 =
            afw_object_string_property_name_internal(property_name, xctx);
        if (impl_prefix_matches(logical_path, name_utf8)) {
            if (!best_prefix || name_utf8->len > best_prefix->len) {
                best_prefix = name_utf8;
                best_host = property_value;
            }
        }
        property_value = afw_object_get_next_property_as_string_internal(roots,
            &iterator, &property_name, xctx);
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

    rem_s = remainder.s;
    rem_len = remainder.len;
    while (rem_len > 0 && *rem_s == '/') {
        rem_s++;
        rem_len--;
    }

    root = afw_file_path_canonicalize(best_host, p, xctx);

    if (rem_len == 0) {
        return root;
    }

    remainder.s = rem_s;
    remainder.len = rem_len;
    return afw_file_path_join_under_root(root, &remainder,
        false, p, xctx);
}

/* Interface afw_adapter Implementation for vfs */
// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */



/**
 * @file afw_vfs_adapter.c
 * @brief Implementation of afw_adapter interface for afw_vfs
 */

#include "afw.h"
#include "afw_adapter_impl.h"
#include "afw_vfs_adapter_internal.h"


/* Declares and rti/inf defines for interface afw_adapter */
#define AFW_IMPLEMENTATION_ID "vfs"
#include "afw_adapter_impl_declares.h"


/* Create an instance of the adapter. */
const afw_adapter_t *
afw_vfs_adapter_internal_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_vfs_adapter_internal_t *self;
    afw_adapter_t *adapter;
    const afw_array_t *vfs_map;
    const afw_array_t *x_list;
    afw_utf8_utf8_z_t *mark_executable;
    const afw_iterator_t *iterator;
    afw_key_z_string_z_t *entries;
    afw_key_z_string_z_t *e;
    afw_key_z_string_z_t temp_entry;
    const afw_value_t *value;
    const afw_utf8_t *entry;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end_c;
    afw_size_t count;
    apr_finfo_t finfo;
    apr_status_t rv;

    /* Create adapter and process common properties.  */
    adapter = afw_adapter_impl_create_cede_p(
            &impl_afw_adapter_inf,
            sizeof(afw_vfs_adapter_internal_t),
            properties, p, xctx);
    p = adapter->p;
    self = (afw_vfs_adapter_internal_t *)adapter;

    /* Get vfsMap property. */
    vfs_map = afw_object_old_get_property_as_array(
        adapter->properties, afw_vfs_s_vfsMap, xctx);
    count = 0;
    if (vfs_map) {
        count = afw_array_get_count(vfs_map, xctx);
    }
    if (count == 0) {
        AFW_THROW_ERROR_Z(general,
            "At least one \"vfsMap\" entry must be specified",
            xctx);
    }

    /* Allocate memory for entries. */
    entries = afw_pool_calloc(p,
        (count + 1) * sizeof(afw_key_z_string_z_t), xctx);
    self->vfs_map = entries;

    /* Add entries to allocated memory. */
    for (iterator = NULL;;entries++) {

        /* Get next entry and parse until there are no more. */
        value = afw_array_get_next_value(vfs_map, &iterator, p, xctx);
        if (!value) {
            break;
        }
        if (!afw_value_is_string(value)) {
            AFW_THROW_ERROR_Z(general,
                "\"vfsMap\" entries must be strings",
                xctx);
        }
        entry = &((const afw_value_string_t *)value)->internal;
        for (c = entry->s, end_c = c + entry->len;
            c < end_c && *c != '=';
            c++);
        if (entry->s - c + 2 == entry->len) {
            AFW_THROW_ERROR_Z(general,
                "\"vfsMap\" entries must contain an equal ('=') "
                "followed by a path",
                xctx);
        }
        entries->key.len = c - entry->s;
        entries->key_z =
            afw_utf8_z_create(entry->s, entries->key.len, p, xctx);
        entries->string.len = end_c - c - 1;
        entries->string_z =
            afw_utf8_z_create(c + 1, entries->string.len, p, xctx);

        /* Make sure <objectId_prefix> is empty or ends with '/'. */
        if (entries->key.len > 0 &&
            !afw_utf8_ends_with(&entries->key, afw_s_a_slash))
        {
            AFW_THROW_ERROR_Z(general,
                "The <objectId_prefix> in a vfsMap entry must be empty or end "
                "with a slash ('/').",
                xctx);
        }

        /* Make sure <host file system directory path> ends with a '/' */
        if (entries->string.len == 0 ||
            !afw_utf8_ends_with(&entries->string, afw_s_a_slash))
        {
#if defined(_WIN32) || defined(WIN32)
            if (!afw_utf8_ends_with(&entries->string, afw_s_a_backslash)) {
                AFW_THROW_ERROR_Z(general,
                    "The <host file system directory path> in a vfsMap entry "
                    "must end with a slash ('/') or a backslash ('\\')",
                    xctx);
            }
#else
            AFW_THROW_ERROR_Z(general,
                "The <host file system directory path> in a vfsMap entry must "
                "end with a slash ('/')",
                xctx);
#endif
        }

        /* Make sure <host file system directory path> is existing directory. */
        rv = apr_stat(&finfo,
            entries->string_z, APR_FINFO_TYPE, afw_pool_get_apr_pool(p));
        if (rv != APR_SUCCESS || finfo.filetype != APR_DIR) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "%s in vfsMap entry <host file system directory path> is not "
                "a directory",
                entries->string_z);
        }

        /* Move new entry to its ordered place. */
        for (e = entries; e > self->vfs_map; e--) {
            if ((e - 1)->key.len > e->key.len ||
                ((e - 1)->key.len == e->key.len &&
                strcmp((e - 1)->key_z, e->key_z) < 0))
            {
                break;
            }
            if (strcmp((e - 1)->key_z, e->key_z) == 0) {
                AFW_THROW_ERROR_Z(general,
                    "vfsMap has duplicate <objectId_prefix> values",
                    xctx);
            }
            afw_memory_copy(&temp_entry, e - 1);
            afw_memory_copy(e - 1, e);
            afw_memory_copy(e, &temp_entry);
        }

    }

    /* Process markExecutable */
    x_list = afw_object_old_get_property_as_array(adapter->properties,
        afw_vfs_s_markExecutable, xctx);
    count = 0;
    if (x_list) {
        count = afw_array_get_count(x_list, xctx);
    }
    if (count != 0) {
        mark_executable = afw_pool_calloc(p,
            (count + 1) * sizeof(afw_utf8_utf8_z_t), xctx);
        self->mark_executable = mark_executable;

        for (iterator = NULL;; mark_executable++) {
            value = afw_array_get_next_value(x_list, &iterator, p, xctx);
            if (!value) {
                break;
            }
            if (!afw_value_is_string(value)) {
                AFW_THROW_ERROR_Z(general,
                    "\"executableSuffixes\" entries must be strings",
                    xctx);
            }
            mark_executable->s_z = afw_utf8_z_create(
                (((const afw_value_string_t *)value)->internal).s,
                (((const afw_value_string_t *)value)->internal).len,
                p, xctx);
            mark_executable->s.len = sizeof(mark_executable->s_z);
        }
    }

    /* Return adapter. */
    return adapter;
}



/*
 * Implementation of method destroy for interface afw_adapter.
 */
void
impl_afw_adapter_destroy(
    const afw_adapter_t * instance,
    afw_xctx_t *xctx)
{
    /*FIXME Add destroy code if needed. */
}



/*
 * Implementation of method create_adapter_session for interface
 * afw_adapter.
 */
const afw_adapter_session_t *
impl_afw_adapter_create_adapter_session(
    const afw_adapter_t *instance,
    afw_xctx_t *xctx)
{
    afw_vfs_adapter_internal_t *self =
        (afw_vfs_adapter_internal_t *)instance;
    const afw_adapter_session_t *session;

    /* Create session and return. */
    session = afw_vfs_adapter_internal_session_create(
        self, xctx);
    return session;
}



/*
 * Implementation of method get_additional_metrics for interface
 * afw_adapter.
 */
const afw_object_t *
impl_afw_adapter_get_additional_metrics(
    const afw_adapter_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Add code if this implementation has additional metrics. */
    return NULL;
}

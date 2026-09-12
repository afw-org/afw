// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework file support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_file.c
 * @brief Host file read/write helpers (adapter lives in afw_file_adapter.c).
 */

#include "afw_internal.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


/* Return full file path. */
AFW_DEFINE(const afw_utf8_t *)
afw_file_insure_full_path(const afw_utf8_t *path,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *full_path;
    const afw_utf8_z_t *path_z;

    path_z = afw_utf8_to_utf8_z(path, p, xctx);
    full_path = afw_os_realpath(path_z, p, xctx);
    if (!full_path) {
        full_path = afw_file_path_absolutize(path, p, xctx);
    }
    if (full_path->len == 0 ||
        full_path->s[full_path->len - 1] != '/')
    {
        full_path = afw_utf8_concat(p, xctx,
            full_path, afw_s_a_slash, NULL);
    }
    return full_path;
}


/* Read a file into memory in a specifed pool. */
AFW_DEFINE(const afw_memory_t *)
afw_file_to_memory(
    const afw_utf8_t * file_path,
    afw_size_t file_size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_t *to_memory;
    afw_byte_t *buff;
    FILE *in;
    afw_file_info_t info;
    const afw_utf8_z_t *file_path_z;

    to_memory = afw_pool_calloc_type(p, afw_memory_t, xctx);
    file_path_z = afw_utf8_to_utf8_z(file_path, p, xctx);
    in = fopen(file_path_z, "r");
    if (!in) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Error opening %s errno %d", file_path_z, errno);
    }

    AFW_TRY {

        if (file_size == 0) {
            afw_file_stat(file_path_z, &info, xctx);
            if (info.type == afw_file_type_missing) {
                AFW_THROW_ERROR_FZ(not_found, xctx,
                    "Error opening %s errno %d", file_path_z, ENOENT);
            }
            file_size = (afw_size_t)info.size;
        }
        if (file_size == 0) {
            to_memory->size = 0;
            to_memory->ptr = (const afw_byte_t *)"";
        }
        else {
            buff = afw_pool_malloc(p, file_size, xctx);
            to_memory->size = fread(buff, 1, file_size, in);
            to_memory->ptr = buff;
            if (to_memory->size < 1) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Error reading '%ku'.",
                    file_path);
            }
        }
    }

    AFW_FINALLY{
        fclose(in);
    }

    AFW_ENDTRY;

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
    afw_utf8_z_t *file_path_z;
    const afw_utf8_octet_t *i;
    afw_utf8_octet_t *o;
    afw_utf8_octet_t *last_slash;
    afw_size_t count;
    afw_file_info_t info;
    int fd;
    int flags;

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

    switch (mode) {

    case afw_file_mode_write:
        break;

    case afw_file_mode_write_new:
        afw_file_stat(file_path_z, &info, xctx);
        if (info.type != afw_file_type_missing) {
            AFW_THROW_ERROR_FZ(conflict, xctx,
                "File %s already exists.", file_path_z);
        }
        if (last_slash) {
            *last_slash = 0;
            afw_file_mkdir_p(file_path_z, xctx);
            *last_slash = '/';
        }
        break;

    case afw_file_mode_write_existing:
        break;

    default:
        AFW_THROW_ERROR_FZ(general, xctx, "Invalid mode %d.", mode);
    };

    flags = O_WRONLY | O_CREAT | O_TRUNC;
    if (mode == afw_file_mode_write_new) {
        flags |= O_EXCL;
    }
    fd = -1;
    AFW_TRY {
        fd = afw_file_open(file_path_z, flags, xctx);
        afw_file_write_full(fd,
            from_memory->ptr ? from_memory->ptr : (const afw_byte_t *)"",
            from_memory->size, xctx);
    }
    AFW_FINALLY {
        afw_file_close(fd, xctx);
    }
    AFW_ENDTRY;
}


/* Delete file. */
AFW_DEFINE(void) afw_file_delete(
    const afw_utf8_t * file_path,
    afw_xctx_t *xctx)
{
    const afw_utf8_z_t *file_path_z;

    file_path_z = afw_utf8_to_utf8_z(file_path, xctx->p, xctx);
    afw_file_unlink(file_path_z, xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework POSIX file/dir I/O
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_file_io.c
 * @brief Host filesystem I/O used by file adapter, VFS, and compile.
 */

#include "afw_internal.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#ifndef _WIN32
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#include <direct.h>
#define open _open
#define close _close
#define read _read
#define write _write
#define unlink _unlink
#define rmdir _rmdir
#define lseek _lseeki64
#define ftruncate _chsize_s
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

#ifdef _WIN32
typedef int ssize_t;
#endif


struct afw_file_dir_s {
#ifndef _WIN32
    DIR *d;
#else
    intptr_t handle;
    struct _finddata_t first;
    afw_boolean_t used_first;
    afw_boolean_t exhausted;
#endif
    const afw_utf8_z_t *path_z;
    const afw_pool_t *p;
};


static afw_os_time_t
impl_timespec_to_os(time_t sec, long nsec)
{
    return (afw_os_time_t)sec * 1000000 + (afw_os_time_t)(nsec / 1000);
}


static void
impl_info_from_stat(afw_file_info_t *info, const struct stat *st)
{
    if (S_ISDIR(st->st_mode)) {
        info->type = afw_file_type_directory;
    }
    else if (S_ISREG(st->st_mode)) {
        info->type = afw_file_type_file;
    }
    else {
        info->type = afw_file_type_other;
    }
    info->size = (afw_off_t)st->st_size;
#if defined(__APPLE__)
    info->atime = impl_timespec_to_os(
        st->st_atimespec.tv_sec, st->st_atimespec.tv_nsec);
    info->mtime = impl_timespec_to_os(
        st->st_mtimespec.tv_sec, st->st_mtimespec.tv_nsec);
    info->ctime = impl_timespec_to_os(
        st->st_ctimespec.tv_sec, st->st_ctimespec.tv_nsec);
#elif defined(_WIN32)
    info->atime = (afw_os_time_t)st->st_atime * 1000000;
    info->mtime = (afw_os_time_t)st->st_mtime * 1000000;
    info->ctime = (afw_os_time_t)st->st_ctime * 1000000;
#else
    info->atime = impl_timespec_to_os(
        st->st_atim.tv_sec, st->st_atim.tv_nsec);
    info->mtime = impl_timespec_to_os(
        st->st_mtim.tv_sec, st->st_mtim.tv_nsec);
    info->ctime = impl_timespec_to_os(
        st->st_ctim.tv_sec, st->st_ctim.tv_nsec);
#endif
}


static afw_file_type_t
impl_type_from_stat_path(
    const afw_utf8_z_t *dir_z,
    const afw_utf8_z_t *name_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t dir;
    afw_utf8_t name;
    const afw_utf8_t *joined;
    afw_file_info_t info;

    dir.s = (const afw_utf8_octet_t *)dir_z;
    dir.len = strlen(dir_z);
    name.s = (const afw_utf8_octet_t *)name_z;
    name.len = strlen(name_z);
    if (dir.len > 0 && dir.s[dir.len - 1] == '/') {
        joined = afw_utf8_concat(p, xctx, &dir, &name, NULL);
    }
    else {
        joined = afw_utf8_concat(p, xctx, &dir, afw_s_a_slash, &name, NULL);
    }
    afw_file_stat(afw_utf8_to_utf8_z(joined, p, xctx), &info, xctx);
    return info.type;
}


AFW_DEFINE(void)
afw_file_stat(
    const afw_utf8_z_t *path_z,
    afw_file_info_t *info,
    afw_xctx_t *xctx)
{
    struct stat st;
    int err;

    if (!path_z || !*path_z) {
        errno = EINVAL;
        err = errno;
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "stat: empty path", xctx);
    }
    if (stat(path_z, &st) != 0) {
        err = errno;
        if (err == ENOENT) {
            info->type = afw_file_type_missing;
            info->size = 0;
            info->atime = 0;
            info->mtime = 0;
            info->ctime = 0;
            return;
        }
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "stat %s failed", path_z);
    }
    impl_info_from_stat(info, &st);
}


AFW_DEFINE(void)
afw_file_mkdir_p(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx)
{
    char *buf;
    afw_size_t len;
    afw_size_t i;
    int err;
    afw_file_info_t info;

    if (!path_z || !*path_z) {
        AFW_THROW_ERROR_Z(general, "mkdir_p: empty path", xctx);
    }
    len = strlen(path_z);
    buf = afw_pool_malloc(xctx->p, len + 1, xctx);
    memcpy(buf, path_z, len + 1);
    while (len > 1 && buf[len - 1] == '/') {
        buf[--len] = 0;
    }
    for (i = 1; i < len; i++) {
        if (buf[i] != '/') {
            continue;
        }
        buf[i] = 0;
#ifdef _WIN32
        if (_mkdir(buf) != 0 && errno != EEXIST) {
#else
        if (mkdir(buf, 0777) != 0 && errno != EEXIST) {
#endif
            err = errno;
            AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
                "mkdir %s failed", buf);
        }
        buf[i] = '/';
    }
#ifdef _WIN32
    if (_mkdir(buf) != 0) {
#else
    if (mkdir(buf, 0777) != 0) {
#endif
        err = errno;
        if (err != EEXIST) {
            AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
                "mkdir %s failed", buf);
        }
        afw_file_stat(buf, &info, xctx);
        if (info.type != afw_file_type_directory) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "mkdir_p: %s exists and is not a directory",
                buf);
        }
    }
}


AFW_DEFINE(void)
afw_file_unlink(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx)
{
    int err;

    if (unlink(path_z) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(not_found, errno, err, xctx,
            "unlink %s failed", path_z);
    }
}


AFW_DEFINE(void)
afw_file_unlink_if_exists(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx)
{
    int err;

    if (unlink(path_z) != 0) {
        err = errno;
        if (err != ENOENT) {
            AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
                "unlink %s failed", path_z);
        }
    }
}


AFW_DEFINE(void)
afw_file_rmdir(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx)
{
    int err;

    if (rmdir(path_z) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "rmdir %s failed (directory must be empty)", path_z);
    }
}


AFW_DEFINE(void)
afw_file_rename(
    const afw_utf8_z_t *from_z,
    const afw_utf8_z_t *to_z,
    afw_xctx_t *xctx)
{
    int err;

    if (rename(from_z, to_z) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "rename %s to %s failed", from_z, to_z);
    }
}


AFW_DEFINE(void)
afw_file_add_execute_bit(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx)
{
    struct stat st;
    int err;

    if (stat(path_z, &st) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "stat %s failed", path_z);
    }
#ifndef _WIN32
    if (chmod(path_z, st.st_mode | S_IXUSR | S_IXGRP | S_IXOTH) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "chmod %s failed", path_z);
    }
#else
    (void)st;
#endif
}


AFW_DEFINE(int)
afw_file_open(
    const afw_utf8_z_t *path_z,
    int flags,
    afw_xctx_t *xctx)
{
    int fd;
    int err;

    fd = open(path_z, flags | O_BINARY | O_CLOEXEC, 0666);
    if (fd < 0) {
        err = errno;
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "open %s failed", path_z);
    }
    return fd;
}


AFW_DEFINE(void)
afw_file_close(int fd, afw_xctx_t *xctx)
{
    int err;

    if (fd < 0) {
        return;
    }
    if (close(fd) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "close failed", xctx);
    }
}


AFW_DEFINE(afw_size_t)
afw_file_read(
    int fd,
    void *buf,
    afw_size_t n,
    afw_xctx_t *xctx)
{
    afw_byte_t *out;
    afw_size_t remain;
    afw_size_t total;
    ssize_t got;
    int err;

    out = (afw_byte_t *)buf;
    remain = n;
    total = 0;
    while (remain > 0) {
        got = read(fd, out, remain);
        if (got < 0) {
            err = errno;
            if (err == EINTR) {
                continue;
            }
            AFW_THROW_ERROR_RV_Z(general, errno, err,
                "read failed", xctx);
        }
        if (got == 0) {
            break;
        }
        out += (afw_size_t)got;
        remain -= (afw_size_t)got;
        total += (afw_size_t)got;
    }
    return total;
}


AFW_DEFINE(void)
afw_file_write_full(
    int fd,
    const void *buf,
    afw_size_t n,
    afw_xctx_t *xctx)
{
    const afw_byte_t *in;
    afw_size_t remain;
    ssize_t put;
    int err;

    in = (const afw_byte_t *)buf;
    remain = n;
    if (remain == 0) {
        return;
    }
    if (!in) {
        AFW_THROW_ERROR_Z(general, "write: NULL buffer", xctx);
    }
    while (remain > 0) {
        put = write(fd, in, remain);
        if (put < 0) {
            err = errno;
            if (err == EINTR) {
                continue;
            }
            AFW_THROW_ERROR_RV_Z(general, errno, err,
                "write failed", xctx);
        }
        if (put == 0) {
            AFW_THROW_ERROR_Z(general, "write: short write", xctx);
        }
        in += (afw_size_t)put;
        remain -= (afw_size_t)put;
    }
}


AFW_DEFINE(afw_off_t)
afw_file_seek(
    int fd,
    afw_off_t offset,
    int whence,
    afw_xctx_t *xctx)
{
    afw_off_t pos;
    int err;

    pos = (afw_off_t)lseek(fd, (off_t)offset, whence);
    if (pos < 0) {
        err = errno;
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "seek failed", xctx);
    }
    return pos;
}


AFW_DEFINE(void)
afw_file_trunc(
    int fd,
    afw_off_t length,
    afw_xctx_t *xctx)
{
    int err;

    if (ftruncate(fd, (off_t)length) != 0) {
        err = errno;
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "ftruncate failed", xctx);
    }
}


#ifndef _WIN32

AFW_DEFINE(afw_file_dir_t *)
afw_file_dir_open(
    const afw_utf8_z_t *path_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    DIR *d;
    afw_file_dir_t *dir;
    int err;

    d = opendir(path_z);
    if (!d) {
        err = errno;
        if (err == ENOENT) {
            return NULL;
        }
        AFW_THROW_ERROR_RV_FZ(general, errno, err, xctx,
            "opendir %s failed", path_z);
    }
    dir = afw_pool_calloc_type(p, afw_file_dir_t, xctx);
    dir->d = d;
    dir->path_z = path_z;
    dir->p = p;
    return dir;
}


AFW_DEFINE(afw_boolean_t)
afw_file_dir_read(
    afw_file_dir_t *dir,
    afw_file_dirent_t *ent,
    afw_xctx_t *xctx)
{
    struct dirent *e;
    int err;

    errno = 0;
    e = readdir(dir->d);
    if (!e) {
        err = errno;
        if (err != 0) {
            AFW_THROW_ERROR_RV_Z(general, errno, err,
                "readdir failed", xctx);
        }
        return false;
    }
    ent->name_z = e->d_name;
#ifdef DT_DIR
    if (e->d_type == DT_DIR) {
        ent->type = afw_file_type_directory;
    }
    else if (e->d_type == DT_REG) {
        ent->type = afw_file_type_file;
    }
    else {
        /* Follow (symlink / unknown) so listing matches APR TYPE. */
        ent->type = impl_type_from_stat_path(
            dir->path_z, e->d_name, dir->p, xctx);
    }
#else
    ent->type = impl_type_from_stat_path(
        dir->path_z, e->d_name, dir->p, xctx);
#endif
    return true;
}


AFW_DEFINE(void)
afw_file_dir_close(
    afw_file_dir_t *dir,
    afw_xctx_t *xctx)
{
    int err;

    if (!dir || !dir->d) {
        return;
    }
    if (closedir(dir->d) != 0) {
        err = errno;
        dir->d = NULL;
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "closedir failed", xctx);
    }
    dir->d = NULL;
}

#else /* _WIN32 */

AFW_DEFINE(afw_file_dir_t *)
afw_file_dir_open(
    const afw_utf8_z_t *path_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_file_dir_t *dir;
    char pattern[4096];
    intptr_t handle;
    int n;

    n = snprintf(pattern, sizeof(pattern), "%s\\*", path_z);
    if (n < 0 || (size_t)n >= sizeof(pattern)) {
        AFW_THROW_ERROR_Z(general, "opendir path too long", xctx);
    }
    dir = afw_pool_calloc_type(p, afw_file_dir_t, xctx);
    handle = _findfirst(pattern, &dir->first);
    if (handle < 0) {
        if (errno == ENOENT) {
            return NULL;
        }
        AFW_THROW_ERROR_RV_FZ(general, errno, errno, xctx,
            "opendir %s failed", path_z);
    }
    dir->handle = handle;
    dir->used_first = false;
    dir->exhausted = false;
    dir->path_z = path_z;
    dir->p = p;
    return dir;
}


AFW_DEFINE(afw_boolean_t)
afw_file_dir_read(
    afw_file_dir_t *dir,
    afw_file_dirent_t *ent,
    afw_xctx_t *xctx)
{
    struct _finddata_t data;
    int rc;

    if (dir->exhausted) {
        return false;
    }
    if (!dir->used_first) {
        data = dir->first;
        dir->used_first = true;
    }
    else {
        rc = _findnext(dir->handle, &data);
        if (rc != 0) {
            dir->exhausted = true;
            return false;
        }
    }
    ent->name_z = afw_utf8_z_create(
        data.name, AFW_UTF8_Z_LEN, dir->p, xctx);
    if (data.attrib & _A_SUBDIR) {
        ent->type = afw_file_type_directory;
    }
    else {
        ent->type = afw_file_type_file;
    }
    return true;
}


AFW_DEFINE(void)
afw_file_dir_close(
    afw_file_dir_t *dir,
    afw_xctx_t *xctx)
{
    (void)xctx;
    if (!dir || dir->handle < 0) {
        return;
    }
    _findclose(dir->handle);
    dir->handle = -1;
}

#endif /* _WIN32 */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework file support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_FILE_H__
#define __AFW_FILE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_file
 * @{
 */

/**
 * @file afw_file.h
 * @brief Header file for Adaptive Framework file support
 */

AFW_BEGIN_DECLARES

/** File mode. */
typedef enum afw_file_mode_e {
    afw_file_mode_read,
    afw_file_mode_write,
    afw_file_mode_write_new,
    afw_file_mode_write_existing
} afw_file_mode_t;


/**
 * @brief Return full file path.
 * @param path is a full or relative path.
 * @param p to use for adapter resources.
 * @param xctx of caller.
 * @return full path.
 * 
 * The current working directory will be prepended if path is a relative path.
 * 
 */

AFW_DECLARE(const afw_utf8_t *)
afw_file_insure_full_path(const afw_utf8_t *path,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Canonicalize path to an absolute real path.
 * @param path full or relative host path.
 * @param p pool for the returned path.
 * @param xctx of caller.
 * @return absolute real path (pool-allocated).
 *
 * The path must exist. Relative paths are resolved against CWD.
 * Follows symlinks. Does not add a trailing slash.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_file_path_canonicalize(
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Make path absolute without requiring it to exist.
 * @param path full or relative host path.
 * @param p pool for the returned path.
 * @param xctx of caller.
 * @return absolute path (pool-allocated), `.` / `..` collapsed.
 *
 * Relative paths are resolved against CWD. Does not follow symlinks.
 * Does not add a trailing slash.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_file_path_absolutize(
    const afw_utf8_t *path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Join addpath under root with containment.
 * @param root canonical host directory (absolute real path).
 * @param addpath remainder relative to root; leading slashes ignored.
 * @param trailing_slash if true, insure the result ends with '/'.
 * @param p pool for the returned path.
 * @param xctx of caller.
 * @return absolute host path under root (pool-allocated).
 *
 * Extra slashes and '.' segments are collapsed. '..' that would leave
 * root is rejected. If the joined path exists, it is realpath'd so a
 * symlink whose target is outside root fails. If it does not exist
 * (create of a missing leaf), the lexical join is used and must still
 * stay under root by separator-boundary.
 *
 * Used by rootFilePaths resolve and vfsMap host-path resolve.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_file_path_join_under_root(
    const afw_utf8_t *root,
    const afw_utf8_t *addpath,
    afw_boolean_t trailing_slash,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Resolve a logical path using application rootFilePaths.
 * @param logical_path path as used by open_file / compile_from_file /
 *     eval_from_file (must start with a configured rootFilePaths property
 *     name, followed by '/' or end of string).
 * @param p pool for the returned path.
 * @param xctx of caller.
 * @return absolute host filesystem path (pool-allocated).
 *
 * Uses longest matching prefix among rootFilePaths keys. Host directory
 * values may be absolute or relative (relative is absolutized at resolve
 * time). Rejects path traversal (`..`) and paths that would escape the
 * matched root after canonicalization. Throws if rootFilePaths is missing,
 * no prefix matches, the root directory is missing, or containment fails.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_file_path_resolve_rootFilePaths(
    const afw_utf8_t *logical_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Get the factory for file adapter.
 * @return factory singleton instance.
 */
AFW_DECLARE(const afw_adapter_factory_t *)
afw_file_adapter_factory_get();

/**
 * @brief Create a file adapter.
 * @param properties configuration.
 * @param p to use for adapter resources.
 * @param xctx of caller.
 * @return New instance of file adapter.
 */
AFW_DECLARE(const afw_adapter_t *)
afw_file_adapter_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Read a file into a memory in a specifed pool.
 * @param file_path file path
 * @param file_size size of file if already known or 0.
 * @param p Pool for memory.
 * @param xctx of caller.
 * @return value
 */
AFW_DECLARE(const afw_memory_t *)
afw_file_to_memory(
    const afw_utf8_t * file_path,
    afw_size_t file_size,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Write a file from a memory.
 * @param file_path file path
 * @param from_memory memory to write.
 * @param mode file open mode.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_file_from_memory(
    const afw_utf8_t * file_path,
    const afw_memory_t * from_memory,
    afw_file_mode_t mode,
    afw_xctx_t *xctx);


/**
 * @brief Delete file.
 * @param file_path file path
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_file_delete(
    const afw_utf8_t * file_path,
    afw_xctx_t *xctx);


/**
 * Host file type from stat / directory listing.
 */
typedef enum afw_file_type_e {
    afw_file_type_missing,
    afw_file_type_file,
    afw_file_type_directory,
    afw_file_type_other
} afw_file_type_t;


/**
 * Result of afw_file_stat(). Times are microseconds since epoch.
 */
typedef struct afw_file_info_s {
    afw_file_type_t type;
    afw_off_t size;
    afw_os_time_t atime;
    afw_os_time_t mtime;
    afw_os_time_t ctime;
} afw_file_info_t;


/**
 * @brief Stat path. ENOENT sets type missing; other errors throw.
 */
AFW_DECLARE(void)
afw_file_stat(
    const afw_utf8_z_t *path_z,
    afw_file_info_t *info,
    afw_xctx_t *xctx);


/**
 * @brief Create directories in path (mkdir -p). Existing dir is OK.
 */
AFW_DECLARE(void)
afw_file_mkdir_p(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx);


/**
 * @brief Unlink a file. Throws on error (including missing).
 */
AFW_DECLARE(void)
afw_file_unlink(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx);


/**
 * @brief Unlink if present. Missing is OK; other errors throw.
 */
AFW_DECLARE(void)
afw_file_unlink_if_exists(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx);


/**
 * @brief Remove an empty directory.
 */
AFW_DECLARE(void)
afw_file_rmdir(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx);


/**
 * @brief Rename from_z to to_z.
 */
AFW_DECLARE(void)
afw_file_rename(
    const afw_utf8_z_t *from_z,
    const afw_utf8_z_t *to_z,
    afw_xctx_t *xctx);


/**
 * @brief Add user/group/other execute bits (chmod a+x).
 */
AFW_DECLARE(void)
afw_file_add_execute_bit(
    const afw_utf8_z_t *path_z,
    afw_xctx_t *xctx);


/**
 * @brief Open path with POSIX flags (O_RDONLY, O_CREAT, ...). Throws.
 * @return file descriptor.
 */
AFW_DECLARE(int)
afw_file_open(
    const afw_utf8_z_t *path_z,
    int flags,
    afw_xctx_t *xctx);


/**
 * @brief Close fd. fd < 0 is ignored.
 */
AFW_DECLARE(void)
afw_file_close(int fd, afw_xctx_t *xctx);


/**
 * @brief Read up to n bytes. 0 is EOF. Throws on error.
 */
AFW_DECLARE(afw_size_t)
afw_file_read(
    int fd,
    void *buf,
    afw_size_t n,
    afw_xctx_t *xctx);


/**
 * @brief Write all n bytes. Throws on error or short write.
 */
AFW_DECLARE(void)
afw_file_write_full(
    int fd,
    const void *buf,
    afw_size_t n,
    afw_xctx_t *xctx);


/**
 * @brief lseek. whence is SEEK_SET / SEEK_CUR / SEEK_END.
 * @return resulting offset.
 */
AFW_DECLARE(afw_off_t)
afw_file_seek(
    int fd,
    afw_off_t offset,
    int whence,
    afw_xctx_t *xctx);


/**
 * @brief ftruncate fd to length.
 */
AFW_DECLARE(void)
afw_file_trunc(
    int fd,
    afw_off_t length,
    afw_xctx_t *xctx);


/** Directory listing handle (opendir). */
typedef struct afw_file_dir_s afw_file_dir_t;


typedef struct afw_file_dirent_s {
    const afw_utf8_z_t *name_z;
    afw_file_type_t type;
} afw_file_dirent_t;


/**
 * @brief Open a directory. NULL if missing (ENOENT); throws otherwise.
 */
AFW_DECLARE(afw_file_dir_t *)
afw_file_dir_open(
    const afw_utf8_z_t *path_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Next entry. false at end. name_z is valid until the next read.
 */
AFW_DECLARE(afw_boolean_t)
afw_file_dir_read(
    afw_file_dir_t *dir,
    afw_file_dirent_t *ent,
    afw_xctx_t *xctx);


/**
 * @brief Close directory. NULL is ignored.
 */
AFW_DECLARE(void)
afw_file_dir_close(
    afw_file_dir_t *dir,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_FILE_H__ */

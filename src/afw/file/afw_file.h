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
 * @defgroup afw_file File Adaptor
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework adaptor for storing objects in file system.
 * 
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
 * @param p to use for adaptor resources.
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
 * @brief Get the factory for file adaptor.
 * @return factory singleton instance.
 */
AFW_DECLARE(const afw_adaptor_factory_t *)
afw_file_adaptor_factory_get();

/**
 * @brief Create a file adaptor.
 * @param properties configuration.
 * @param p to use for adaptor resources.
 * @param xctx of caller.
 * @return New instance of file adaptor.
 */
AFW_DECLARE(const afw_adaptor_t *)
afw_file_adaptor_create_cede_p(
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
    apr_size_t file_size,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Write a file from a memory.
 * @param file_path file path
 * @param xctx of caller.
 * @return value
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


AFW_END_DECLARES

/** @} */

#endif /* __AFW_FILE_H__ */

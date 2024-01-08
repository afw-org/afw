// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework File Support Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_FILE_INTERNAL_H__
#define __AFW_FILE_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_file_internal File
 * @ingroup afw_c_api_internal
 * 
 * Internal file API.
 * 
 * @{
 */

/**
 * @file afw_file_internal.h
 * @brief Internal header file for Adaptive Framework file support
 */

AFW_BEGIN_DECLARES

typedef struct afw_file_internal_adaptor_s
afw_file_internal_adaptor_t;

typedef struct afw_file_internal_adaptor_session_s
afw_file_internal_adaptor_session_t;

/* afw_adaptor self. */
struct afw_file_internal_adaptor_s {
    afw_adaptor_t pub;
    const afw_content_type_t *content_type;
    const afw_utf8_t *root;
    const afw_utf8_t *filename_suffix;
    const afw_utf8_z_t *journal_dir_path_z;
    const afw_utf8_z_t *journal_lock_file_path_z;
};


/* afw_adaptor_session self. */
struct afw_file_internal_adaptor_session_s {
    afw_adaptor_session_t pub;
    afw_file_internal_adaptor_t *adaptor;
    afw_adaptor_journal_t journal;
};

const afw_adaptor_journal_inf_t * afw_file_internal_get_journal_inf();

AFW_END_DECLARES

/** @} */

#endif /* __AFW_FILE_INTERNAL_H__ */

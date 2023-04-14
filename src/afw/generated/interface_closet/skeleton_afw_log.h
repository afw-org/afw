// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_log implementation for <afwdev {implementation_id}>
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__
#define __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__

/**
 * @file <afwdev {prefixed_interface_name}>.h
 * @brief <afwdev {brief}>
 *
 * This is the implementation of afw_log for <afwdev {implementation_id}>.
 */

#include "afw_interface.h"
#include "generated/<afwdev {srcdir_prefix}>declare_helpers.h"

AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for <afwdev {implementation_id}> implementation of afw_log.
 */
typedef struct
<afwdev {prefixed_interface_name}>_self_s {
    afw_log_t pub;

    /** @todo Add private implementation variables */

} <afwdev {prefixed_interface_name}>_self_t;

/**
 * @brief Create for <afwdev {implementation_id}> implementation of afw_log
 * @param xctx
 * @param p used to create afw_log.
 * @return new instance of afw_log
 */
<afwdev {srcdir_prefix.upper()}>DECLARE(const afw_log_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *parameters,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__ */

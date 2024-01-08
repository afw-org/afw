// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_request_handler implementation for <afwdev {implementation_id}>
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__
#define __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__

/**
 * @file <afwdev {prefixed_interface_name}>.h
 * @brief <afwdev {brief}>
 *
 * This is the implementation of afw_request_handler for <afwdev {implementation_id}>.
 */

#include "afw_interface.h"
#include "generated/<afwdev {srcdir_prefix}>declare_helpers.h"

AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for <afwdev {implementation_id}> implementation of afw_request_handler.
 */
typedef struct
<afwdev {prefixed_interface_name}>_self_s {
    afw_request_handler_t pub;

    /** @todo Add private implementation variables */

} <afwdev {prefixed_interface_name}>_self_t;

/**
 * @brief Create for <afwdev {implementation_id}> implementation of afw_request_handler
 * @param xctx
 * @param p used to create afw_request_handler.
 * @return new instance of afw_request_handler
 */
<afwdev {srcdir_prefix.upper()}>DECLARE(const afw_request_handler_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *parameters,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __<afwdev {prefixed_interface_name.upper().replace('.', '_')}>_H__ */

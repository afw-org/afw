// See the 'COPYING' file in the project root for licensing information.
/*
 * Interfaces afw_request* implementation development support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_REQUEST_IMPL_H__
#define __AFW_REQUEST_IMPL_H__


/**
 * @addtogroup afw_c_api_public
 *
 * @{
 */

/**
 * @addtogroup afw_request Implementation support for afw_request*
 * @{
 */

/**
 * @file afw_request_impl.h
 * @brief Header for interface afw_request* implementation development.
 */

#include "afw_interface.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Trace request begin if requested.
 * @param request instance
 * @param xctx of caller.
 *
 * trace:request and trace:request:detail.
 */
AFW_DECLARE(void)
afw_request_impl_trace_begin(
    const afw_request_t * instance, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_REQUEST_IMPL_H__ */

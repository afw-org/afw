// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Stream Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_STREAM_INTERNAL_H__
#define __AFW_STREAM_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_stream
 * @{
 */

/**
 * @file afw_stream_internal.h
 * @brief Internal stream/xctx anchor APIs for libafw (stream/ module).
 *
 * Declarations for symbols implemented in afw_stream.c that are not part of
 * the public stream API. Include via afw_internal.h only.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Create xctx->stream_anchor (libafw only).
 * @param xctx being created.
 */
extern const afw_stream_anchor_t *
afw_stream_internal_stream_anchor_create(afw_xctx_t *xctx);

/**
 * @brief Release all streams on an xctx (libafw only).
 * @param xctx of caller.
 */
extern void
afw_stream_internal_release_all_streams(afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_STREAM_INTERNAL_H__ */

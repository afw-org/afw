// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Flag Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_FLAG_INTERNAL_H__
#define __AFW_FLAG_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_flag
 * @{
 */

/**
 * @file afw_flag_internal.h
 * @brief Internal flag bootstrap for libafw (flag/ module).
 *
 * Declarations for symbols implemented in afw_flag.c that are not part of
 * the public flag API. Include via afw_internal.h only.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Register early core flags (libafw bootstrap only).
 * @param xctx of caller.
 *
 * Called from afw_environment_register_core.c.
 */
extern void
afw_flag_internal_early_register_core(afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_FLAG_INTERNAL_H__ */

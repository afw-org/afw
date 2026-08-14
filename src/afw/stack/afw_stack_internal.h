// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Stack Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_STACK_INTERNAL_H__
#define __AFW_STACK_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_stack
 * @{
 */

/**
 * @file afw_stack_internal.h
 * @brief Internal stack wiring for libafw (stack/ module).
 *
 * Declarations for symbols implemented in afw_stack.c that are not part of
 * the public stack API. Include via afw_internal.h only.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Wire qualifier stack during xctx creation (libafw only).
 * @param xctx being created.
 */
extern void
afw_stack_internal_set_qualifier_stack(
    afw_xctx_t *xctx);

/**
 * @brief Wire evaluation stack during xctx creation (libafw only).
 * @param xctx being created.
 */
extern void
afw_stack_internal_set_evaluation_stack(
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_STACK_INTERNAL_H__ */

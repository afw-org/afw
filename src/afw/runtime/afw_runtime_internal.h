// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Runtime Internal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_RUNTIME_INTERNAL_H__
#define __AFW_RUNTIME_INTERNAL_H__

#include "afw_interface.h"

/**
 * @file afw_runtime_internal.h
 * @brief Core-only runtime registration helpers (libafw).
 * @ingroup afw_c_api_internal
 *
 * Not installed; included via afw_internal.h or this header from libafw .c.
 */

/**
 * @brief Register core runtime value accessors (environment create).
 * @param xctx of caller.
 */
void afw_runtime_register_core_value_accessors(afw_xctx_t *xctx);

#endif /* __AFW_RUNTIME_INTERNAL_H__ */

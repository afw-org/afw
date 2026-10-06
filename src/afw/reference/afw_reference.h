// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_reference helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_REFERENCE_H__
#define __AFW_REFERENCE_H__

#include "afw_interface.h"

/**
 * @defgroup afw_reference afw_reference
 * @ingroup afw_c_api_public
 *
 * Helpers for the afw_reference interface (get_reference, release,
 * get_reference_count, for_each_reference). See
 * designs/lifetime-principles.md.
 *
 * @{
 */

/**
 * @file afw_reference.h
 * @brief Helpers for the afw_reference interface.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Debug check that for_each_reference lists only real references.
 * @param root counted instance to start from (NULL is ignored).
 * @param xctx of caller.
 *
 * Walks everything reachable from root through for_each_reference. For
 * each instance reached, subtracts one from a trial copy of its
 * get_reference_count for every listed reference to it. Throws if any
 * trial count goes below 0: something listed a reference it does not
 * hold. (A reference that is held but not listed cannot be seen here;
 * it can only make cycle collection miss a cycle.)
 */
AFW_DECLARE(void)
afw_reference_check(
    const afw_reference_t *root,
    afw_xctx_t *xctx);


/**
 * @brief True when the debug reference check is on.
 *
 * Set environment variable AFW_REFERENCE_CHECK to turn it on.
 */
AFW_DECLARE(afw_boolean_t)
afw_reference_check_is_enabled(void);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_REFERENCE_H__ */

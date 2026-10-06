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


/**
 * @brief Record a counted instance whose release left its count above 0.
 * @param instance counted instance that can be part of a cycle.
 * @param owner pool that owns it (its p->managed_p).
 * @param xctx of caller.
 *
 * Only an instance owned by this xctx (owner == xctx->p) is recorded:
 * cycle collection runs per owner. Called by implementations, not by
 * callers.
 */
AFW_DECLARE(void)
afw_reference_possible_root(
    const afw_reference_t *instance,
    const afw_pool_t *owner,
    afw_xctx_t *xctx);


/**
 * @brief Forget a counted instance that is being freed.
 * @param instance being freed.
 * @param xctx of caller.
 *
 * Called by implementations at last release, so no freed instance stays
 * a possible root.
 */
AFW_DECLARE(void)
afw_reference_forget(
    const afw_reference_t *instance,
    afw_xctx_t *xctx);


/**
 * @brief Safe point: collect cycles if enough possible roots.
 * @param xctx of caller.
 *
 * Called at scope exit. Collects when the possible roots reach the
 * threshold: env AFW_REFERENCE_COLLECT (default 50; 0 turns collection
 * off), raised after each collection to the number of instances it
 * walked that are still alive (amortized constant cost per root).
 */
AFW_DECLARE(void)
afw_reference_safe_point(
    afw_xctx_t *xctx);


/**
 * @brief Collect cycles among this xctx's possible roots now.
 * @param xctx of caller.
 *
 * Trial deletion (Bacon & Rajan 2001): subtract each listed reference
 * among the instances reachable from the possible roots from a trial
 * copy of their counts; anything still above 0 is referenced from
 * outside and kept with all it reaches; the rest only reference each
 * other and are freed (each is emptied with release_references, then
 * released normally).
 */
AFW_DECLARE(void)
afw_reference_collect(
    afw_xctx_t *xctx);


/**
 * @brief Nesting depth of element releases done directly before they
 *    are deferred. See afw_reference_release_held().
 */
#define AFW_REFERENCE_RELEASE_DEPTH_MAX 256


/**
 * @brief Release a reference a container held, without recursing on
 *    the data's depth.
 * @param instance held reference (NULL is ignored).
 * @param xctx of caller.
 *
 * Containers call this for each element they release (an array's
 * values, an object's property values and names). Releasing a nested
 * container releases its elements the same way, so releasing deeply
 * nested data would recurse once per level and could overflow the C
 * stack. Below AFW_REFERENCE_RELEASE_DEPTH_MAX nested levels this
 * releases directly. At that depth it records the release on the xctx
 * instead; when the outermost element release returns, it releases the
 * recorded ones in a loop. Stack use is bounded however deep the data
 * is (#482).
 */
AFW_DECLARE(void)
afw_reference_release_held(
    const afw_reference_t *instance,
    afw_xctx_t *xctx);


/**
 * @brief afw_reference_release_held() for a value.
 * @param _value held value (NULL is ignored).
 * @param _xctx of caller.
 */
#define afw_value_release_held(_value, _xctx) \
    ((_value) \
        ? afw_reference_release_held(&(_value)->ref, (_xctx)) \
        : (void)0)


/**
 * @brief Drop this xctx's cycle collection state (xctx release).
 * @param xctx being released.
 */
AFW_DECLARE(void)
afw_reference_collector_release(
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_REFERENCE_H__ */

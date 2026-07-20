// See the 'COPYING' file in the project root for licensing information.
/*
 * Qualifier / qualifiers snapshot objects (issue #9)
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_xctx_qualifier_object.c
 * @brief Build fresh memory-object snapshots of active qualified variables.
 *
 * These replace the incomplete live-view afw_object implementation. Each call
 * allocates a new object and fills it via stack entry contribute_cb. Suitable
 * for qualifier() / qualifiers() adaptive functions (debug / tooling).
 */

#include "afw_internal.h"


/*
 * True if this stack entry is visible for a snapshot.
 * for_testing: include insecure frames (untrusted view) even when xctx is
 * secure, so trusted tests can inspect what untrusted eval would see.
 */
static afw_boolean_t
impl_entry_visible_for_snapshot(
    const afw_xctx_qualifier_stack_entry_t *e,
    afw_boolean_t for_testing,
    afw_xctx_t *xctx)
{
    if (!e->contribute_cb) {
        return false;
    }
    if (for_testing) {
        return true;
    }
    if (!e->secure && xctx->secure) {
        return false;
    }
    return true;
}


/* Create a fresh memory object of active variables for one qualifier. */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifier_object_create(
    const afw_utf8_t *qualifier,
    afw_boolean_t for_testing,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_xctx_qualifier_stack_entry_t *e_cur;

    object = afw_object_create_unmanaged(p, xctx);

    if (!qualifier || qualifier->len == 0) {
        return object;
    }

    /* First matching visible frame only (same ownership as get). */
    for (
        e_cur = xctx->qualifier_stack->top;
        e_cur >= xctx->qualifier_stack->first;
        e_cur--)
    {
        if (!afw_utf8_equal(qualifier, &e_cur->qualifier)) {
            continue;
        }
        if (!impl_entry_visible_for_snapshot(e_cur, for_testing, xctx)) {
            continue;
        }
        e_cur->contribute_cb(e_cur, object, for_testing, xctx);
        break;
    }

    return object;
}



/* Create a fresh object of all active qualifiers → variable snapshots. */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifiers_object_create(
    afw_boolean_t for_testing,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *qualifiers;
    const afw_object_t *qualifier_object;
    const afw_xctx_qualifier_stack_entry_t *c;

    qualifiers = afw_object_create_unmanaged(p, xctx);
    for (c = xctx->qualifier_stack->top;
        c >= xctx->qualifier_stack->first;
        c--)
    {
        if (c->qualifier.len == 0) {
            continue;
        }
        if (!impl_entry_visible_for_snapshot(c, for_testing, xctx)) {
            continue;
        }
        /* First occurrence of each qualifier name wins (top → bottom). */
        if (afw_object_has_property(qualifiers, &c->qualifier, xctx)) {
            continue;
        }
        qualifier_object = afw_xctx_qualifier_object_create(
            &c->qualifier, for_testing, p, xctx);
        afw_object_set_property_as_object(qualifiers,
            &c->qualifier, qualifier_object, xctx);
    }

    return qualifiers;
}

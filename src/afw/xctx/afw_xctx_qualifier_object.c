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
 * Each call allocates a new object and fills it via stack entry contribute_cb.
 * For one qualifier, every matching visible stack entry may contribute
 * (most recent first; later entries only fill property names not already set).
 * Suitable for qualifier() / qualifiers() adaptive functions.
 */

#include "afw_internal.h"


/*
 * True if this stack entry is visible for a snapshot.
 *
 * Default (include_untrusted false): same as qualifier::name get — when
 * xctx is secure, skip entries pushed with secure=false. When
 * include_untrusted is true and xctx is secure, use less-secure visibility
 * (trusted and untrusted frames). When xctx is not secure, true and false
 * are the same (untrusted frames are already visible to get).
 */
static afw_boolean_t
impl_entry_visible_for_snapshot(
    const afw_xctx_qualifier_stack_entry_t *e,
    afw_boolean_t include_untrusted,
    afw_xctx_t *xctx)
{
    if (!e->contribute_cb) {
        return false;
    }
    if (xctx->secure && !e->secure && !include_untrusted) {
        return false;
    }
    return true;
}


/* Create a fresh memory object of active variables for one qualifier.
 *
 * Walk most recent → older. Every matching visible entry contributes into one
 * accumulator (contribute_cb should leave existing property names alone so the
 * most recent definition wins). Returns NULL if no matching visible entry
 * (nullish to scripts). Empty bag after contribute is still an object, not NULL.
 */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifier_object_create(
    const afw_utf8_t *qualifier,
    afw_boolean_t include_untrusted,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_xctx_qualifier_stack_entry_t *e_cur;

    object = NULL;

    if (!qualifier || qualifier->len == 0) {
        return NULL;
    }

    for (
        e_cur = xctx->qualifier_stack->top;
        e_cur >= xctx->qualifier_stack->first;
        e_cur--)
    {
        if (!afw_utf8_equal(qualifier, &e_cur->qualifier)) {
            continue;
        }
        if (!impl_entry_visible_for_snapshot(e_cur, include_untrusted, xctx)) {
            continue;
        }
        if (!object) {
            object = afw_object_create_unmanaged(p, xctx);
        }
        e_cur->contribute_cb(e_cur, object, include_untrusted, xctx);
    }

    return object;
}



/* Create a fresh object of all active qualifiers → variable snapshots. */
AFW_DEFINE(const afw_object_t *)
afw_xctx_qualifiers_object_create(
    afw_boolean_t include_untrusted,
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
        if (!impl_entry_visible_for_snapshot(c, include_untrusted, xctx)) {
            continue;
        }
        /* One nested snapshot per qualifier name; create merges all entries. */
        if (afw_object_has_property(qualifiers, &c->qualifier, xctx)) {
            continue;
        }
        qualifier_object = afw_xctx_qualifier_object_create(
            &c->qualifier, include_untrusted, p, xctx);
        /* NULL means not active — omit property; do not invent {}. */
        if (!qualifier_object) {
            continue;
        }
        afw_object_set_property_as_object(qualifiers,
            &c->qualifier, qualifier_object, xctx);
    }

    return qualifiers;
}

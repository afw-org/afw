// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_reference helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_reference.c
 * @brief Helpers for the afw_reference interface.
 */

#include "afw_internal.h"
#include <stdint.h>
#include <stdlib.h>


/*
 * Walk state. Debug only, so plain malloc: the walk must not change the
 * pools whose counts it reads.
 */
typedef struct {
    const afw_reference_t *node;
    afw_size_t count;
    afw_integer_t trial;
    int color;      /* collection only */
} impl_node_t;

typedef struct {
    impl_node_t *nodes;
    afw_size_t cap;
    afw_size_t used;
    const afw_reference_t **stack;
    afw_size_t stack_count;
    afw_size_t stack_cap;
} impl_walk_t;


static afw_size_t
impl_hash(const void *p, afw_size_t cap)
{
    uint64_t x = (uint64_t)(uintptr_t)p;

    x ^= x >> 29;
    x *= 0x9E3779B97F4A7C15ull;
    return (afw_size_t)(x >> 17) & (cap - 1);
}


static impl_node_t *
impl_find(impl_walk_t *walk, const afw_reference_t *node, afw_boolean_t *added);


static void
impl_grow(impl_walk_t *walk)
{
    impl_node_t *old = walk->nodes;
    afw_size_t old_cap = walk->cap;
    afw_size_t i;
    afw_boolean_t added;

    walk->cap = (old_cap) ? old_cap * 2 : 256;
    walk->nodes = calloc(walk->cap, sizeof(impl_node_t));
    walk->used = 0;
    for (i = 0; i < old_cap; i++) {
        if (old[i].node) {
            *impl_find(walk, old[i].node, &added) = old[i];
        }
    }
    free(old);
}


static impl_node_t *
impl_find(impl_walk_t *walk, const afw_reference_t *node, afw_boolean_t *added)
{
    afw_size_t i;

    if ((walk->used + 1) * 2 > walk->cap) {
        impl_grow(walk);
    }
    for (i = impl_hash(node, walk->cap);; i = (i + 1) & (walk->cap - 1)) {
        if (walk->nodes[i].node == node) {
            *added = false;
            return &walk->nodes[i];
        }
        if (!walk->nodes[i].node) {
            walk->nodes[i].node = node;
            walk->used++;
            *added = true;
            return &walk->nodes[i];
        }
    }
}


static void
impl_push(impl_walk_t *walk, const afw_reference_t *node)
{
    if (walk->stack_count == walk->stack_cap) {
        walk->stack_cap = (walk->stack_cap) ? walk->stack_cap * 2 : 64;
        walk->stack = realloc(walk->stack,
            walk->stack_cap * sizeof(const afw_reference_t *));
    }
    walk->stack[walk->stack_count++] = node;
}


static void
impl_visit(impl_walk_t *walk, const afw_reference_t *node, afw_xctx_t *xctx)
{
    impl_node_t *n;
    afw_boolean_t added;

    n = impl_find(walk, node, &added);
    if (added) {
        n->count = afw_reference_get_reference_count(node, xctx);
        n->trial = (afw_integer_t)n->count;
        impl_push(walk, node);
    }
}


static void
impl_edge(const afw_reference_t *counted, void *context, afw_xctx_t *xctx)
{
    impl_walk_t *walk = context;
    impl_node_t *n;

    impl_visit(walk, counted, xctx);
    n = impl_find(walk, counted, &(afw_boolean_t){false});
    n->trial--;
}


AFW_DEFINE(void)
afw_reference_check(
    const afw_reference_t *root,
    afw_xctx_t *xctx)
{
    impl_walk_t walk;
    const afw_reference_t *node;
    const impl_node_t *bad;
    afw_size_t bad_count;
    afw_size_t i;

    if (!root) {
        return;
    }
    memset(&walk, 0, sizeof(walk));
    impl_visit(&walk, root, xctx);
    while (walk.stack_count > 0) {
        node = walk.stack[--walk.stack_count];
        afw_reference_for_each_reference(node, impl_edge, &walk, xctx);
    }

    bad = NULL;
    for (i = 0; i < walk.cap; i++) {
        if (walk.nodes[i].node && walk.nodes[i].trial < 0) {
            bad = &walk.nodes[i];
            break;
        }
    }
    if (bad) {
        node = bad->node;
        bad_count = bad->count;
        free(walk.nodes);
        free(walk.stack);
        AFW_THROW_ERROR_FZ(general, xctx,
            "afw_reference_check: %ku implementation '%ku' is listed "
            "more times than its count " AFW_SIZE_T_FMT,
            &node->inf->rti.interface_name,
            &node->inf->rti.implementation_id,
            bad_count);
    }
    free(walk.nodes);
    free(walk.stack);
}


AFW_DEFINE(afw_boolean_t)
afw_reference_check_is_enabled(void)
{
    static int enabled = -1;

    if (enabled < 0) {
        enabled = getenv("AFW_REFERENCE_CHECK") ? 1 : 0;
    }
    return enabled == 1;
}


/* ---------------------------------------------------------------------
 * Cycle collection (#458, #476 step 5)
 * ------------------------------------------------------------------ */

#define IMPL_DEFAULT_COLLECT_THRESHOLD 50

#define IMPL_UNSEEN 0
#define IMPL_GRAY   1
#define IMPL_BLACK  2
#define IMPL_WHITE  3


typedef struct {
    const afw_reference_t **v;
    afw_size_t count;
    afw_size_t cap;
} impl_cc_list_t;

typedef struct {
    /* Possible roots: open addressing set; NULL empty, tombstone 1. */
    const afw_reference_t **roots;
    afw_size_t roots_cap;
    afw_size_t roots_used;      /* live + tombstones */
    afw_size_t roots_live;
    afw_boolean_t collecting;
    /* Possible roots needed before the next collection. */
    afw_size_t threshold;
} impl_collector_t;

#define IMPL_TOMB ((const afw_reference_t *)1)

static int impl_threshold = -1;


static afw_size_t
impl_collect_threshold(void)
{
    const char *v;

    if (impl_threshold < 0) {
        v = getenv("AFW_REFERENCE_COLLECT");
        impl_threshold = (v) ? atoi(v) : IMPL_DEFAULT_COLLECT_THRESHOLD;
        if (impl_threshold < 0) {
            impl_threshold = 0;
        }
    }
    return (afw_size_t)impl_threshold;
}


static void
impl_list_add(impl_cc_list_t *list, const afw_reference_t *node)
{
    if (list->count == list->cap) {
        list->cap = (list->cap) ? list->cap * 2 : 64;
        list->v = realloc(list->v, list->cap * sizeof(*list->v));
    }
    list->v[list->count++] = node;
}


static void impl_roots_grow(impl_collector_t *c);


static const afw_reference_t **
impl_roots_slot(impl_collector_t *c, const afw_reference_t *node,
    afw_boolean_t create)
{
    afw_size_t i;
    const afw_reference_t **tomb = NULL;

    if (create && (c->roots_used + 1) * 2 > c->roots_cap) {
        impl_roots_grow(c);
    }
    if (c->roots_cap == 0) {
        return NULL;
    }
    for (i = impl_hash(node, c->roots_cap);;
        i = (i + 1) & (c->roots_cap - 1))
    {
        if (!c->roots[i]) {
            if (!create) {
                return NULL;
            }
            if (tomb) {
                return tomb;
            }
            c->roots_used++;
            return &c->roots[i];
        }
        if (c->roots[i] == IMPL_TOMB) {
            if (!tomb) {
                tomb = &c->roots[i];
            }
        }
        else if (c->roots[i] == node) {
            return &c->roots[i];
        }
    }
}


static void
impl_roots_grow(impl_collector_t *c)
{
    const afw_reference_t **old = c->roots;
    afw_size_t old_cap = c->roots_cap;
    afw_size_t i;

    c->roots_cap = (old_cap) ? old_cap * 2 : 256;
    c->roots = calloc(c->roots_cap, sizeof(*c->roots));
    c->roots_used = 0;
    c->roots_live = 0;
    for (i = 0; i < old_cap; i++) {
        if (old[i] && old[i] != IMPL_TOMB) {
            *impl_roots_slot(c, old[i], true) = old[i];
            c->roots_live++;
        }
    }
    free(old);
}


AFW_DEFINE(void)
afw_reference_possible_root(
    const afw_reference_t *instance,
    const afw_pool_t *owner,
    afw_xctx_t *xctx)
{
    impl_collector_t *c;
    const afw_reference_t **slot;

    /*
     * Only an owner that is this xctx's single-threaded job heap. Its
     * values are touched only on this thread, so their last release
     * (and forget) always reaches this collector. Values owned by a
     * multithreaded pool (env->p, conf, adapter) can be last-released
     * on another thread and are not collected until worker threads
     * give every owner one thread (#343). An owner that is being
     * destroyed takes every value it holds, cycles included, and
     * afw_xctx_release() has already released this collector: a new
     * one here would leak.
     */
    if (!instance || !xctx || owner != xctx->p ||
        ((const afw_pool_internal_self_t *)owner)->destroying ||
        afw_pool_internal_is_multithreaded(owner) ||
        impl_collect_threshold() == 0)
    {
        return;
    }
    c = xctx->reference_collector;
    if (!c) {
        c = calloc(1, sizeof(impl_collector_t));
        xctx->reference_collector = c;
    }
    if (c->collecting) {
        return;
    }
    slot = impl_roots_slot(c, instance, true);
    if (*slot != instance) {
        *slot = instance;
        c->roots_live++;
    }
}


AFW_DEFINE(void)
afw_reference_forget(
    const afw_reference_t *instance,
    afw_xctx_t *xctx)
{
    impl_collector_t *c;
    const afw_reference_t **slot;

    c = (xctx) ? xctx->reference_collector : NULL;
    if (!c || c->roots_live == 0) {
        return;
    }
    slot = impl_roots_slot(c, instance, false);
    if (slot) {
        *slot = IMPL_TOMB;
        c->roots_live--;
    }
}


AFW_DEFINE(void)
afw_reference_collector_release(
    afw_xctx_t *xctx)
{
    impl_collector_t *c;

    c = xctx->reference_collector;
    if (c) {
        xctx->reference_collector = NULL;
        free(c->roots);
        free(c);
    }
}


AFW_DEFINE(void)
afw_reference_safe_point(
    afw_xctx_t *xctx)
{
    impl_collector_t *c;
    afw_size_t threshold;

    c = xctx->reference_collector;
    threshold = impl_collect_threshold();
    if (c && threshold < c->threshold) {
        threshold = c->threshold;
    }
    if (c && threshold > 0 && !c->collecting &&
        c->roots_live >= threshold)
    {
        afw_reference_collect(xctx);
    }
}


/* Mark gray: subtract every listed reference from a trial count. */
static void
impl_cc_gray_edge(const afw_reference_t *counted, void *context,
    afw_xctx_t *xctx)
{
    impl_walk_t *walk = context;
    impl_node_t *n;
    afw_boolean_t added;

    n = impl_find(walk, counted, &added);
    if (added) {
        n->count = afw_reference_get_reference_count(counted, xctx);
        n->trial = (afw_integer_t)n->count;
        n->color = IMPL_GRAY;
        impl_push(walk, counted);
    }
    n->trial--;
}


/* Scan black: something outside references this; restore what it reaches. */
static void
impl_cc_black_edge(const afw_reference_t *counted, void *context,
    afw_xctx_t *xctx)
{
    impl_walk_t *walk = context;
    impl_node_t *n;
    afw_boolean_t added;

    (void)xctx;
    n = impl_find(walk, counted, &added);
    n->trial++;
    if (n->color != IMPL_BLACK) {
        n->color = IMPL_BLACK;
        impl_push(walk, counted);
    }
}


AFW_DEFINE(void)
afw_reference_collect(
    afw_xctx_t *xctx)
{
    impl_collector_t *c;
    impl_walk_t walk;
    impl_cc_list_t roots, whites;
    impl_node_t *n;
    const afw_reference_t *node;
    afw_boolean_t added;
    afw_boolean_t negative;
    afw_size_t i;

    c = xctx->reference_collector;
    if (!c || c->collecting || c->roots_live == 0) {
        return;
    }
    c->collecting = true;
    memset(&walk, 0, sizeof(walk));
    memset(&roots, 0, sizeof(roots));
    memset(&whites, 0, sizeof(whites));

    /* Take the roots; the set starts over. */
    for (i = 0; i < c->roots_cap; i++) {
        if (c->roots[i] && c->roots[i] != IMPL_TOMB) {
            impl_list_add(&roots, c->roots[i]);
        }
    }
    memset(c->roots, 0, c->roots_cap * sizeof(*c->roots));
    c->roots_used = 0;
    c->roots_live = 0;

    /* 1. Gray: walk from every root, subtracting listed references. */
    for (i = 0; i < roots.count; i++) {
        n = impl_find(&walk, roots.v[i], &added);
        if (!added) {
            continue;
        }
        n->count = afw_reference_get_reference_count(roots.v[i], xctx);
        n->trial = (afw_integer_t)n->count;
        n->color = IMPL_GRAY;
        impl_push(&walk, roots.v[i]);
        while (walk.stack_count > 0) {
            node = walk.stack[--walk.stack_count];
            afw_reference_for_each_reference(node, impl_cc_gray_edge,
                &walk, xctx);
        }
    }

    /* A listed reference that is not held: do not free anything. */
    negative = false;
    for (i = 0; i < walk.cap; i++) {
        if (walk.nodes[i].node && walk.nodes[i].trial < 0) {
            negative = true;
            break;
        }
    }

    if (!negative) {

        /* 2. Black: anything still referenced from outside, and all it
         *    reaches, is kept. */
        for (i = 0; i < walk.cap; i++) {
            n = &walk.nodes[i];
            if (n->node && n->color == IMPL_GRAY && n->trial > 0) {
                n->color = IMPL_BLACK;
                impl_push(&walk, n->node);
                while (walk.stack_count > 0) {
                    node = walk.stack[--walk.stack_count];
                    afw_reference_for_each_reference(node,
                        impl_cc_black_edge, &walk, xctx);
                }
            }
        }

        /* 3. White: the rest only reference each other. */
        for (i = 0; i < walk.cap; i++) {
            n = &walk.nodes[i];
            if (n->node && n->color == IMPL_GRAY) {
                n->color = IMPL_WHITE;
                impl_list_add(&whites, n->node);
            }
        }

        /* Free: hold each, empty each, then release each normally. */
        for (i = 0; i < whites.count; i++) {
            afw_reference_get_reference(whites.v[i], xctx);
        }
        for (i = 0; i < whites.count; i++) {
            afw_reference_release_references(whites.v[i], xctx);
        }
        for (i = 0; i < whites.count; i++) {
            afw_reference_release(whites.v[i], xctx);
        }
    }

    /*
     * Wait for at least as many new possible roots as instances walked
     * that are still alive, so a large live structure that keeps
     * becoming a possible root is walked at amortized constant cost.
     */
    c->threshold = walk.used - whites.count;

    free(walk.nodes);
    free(walk.stack);
    free(roots.v);
    free(whites.v);
    c->collecting = false;
}


/* Record a deferred element release. Falls back to releasing now. */
static void
impl_release_pending_push(
    const afw_reference_t *instance,
    afw_xctx_t *xctx)
{
    const afw_reference_t **grown;
    afw_size_t cap;

    if (xctx->release_pending_count == xctx->release_pending_cap) {
        cap = (xctx->release_pending_cap)
            ? xctx->release_pending_cap * 2 : 64;
        grown = realloc((void *)xctx->release_pending,
            cap * sizeof(*grown));
        if (!grown) {
            /* No memory to defer: recurse, as before #482. */
            afw_reference_release(instance, xctx);
            return;
        }
        xctx->release_pending = grown;
        xctx->release_pending_cap = cap;
    }
    xctx->release_pending[xctx->release_pending_count++] = instance;
}


/*
 * Release what was deferred, one level at a time. Each release can
 * defer more (its own deep elements); keep going until none are left,
 * then free the list so nothing is left for xctx release to free.
 */
static void
impl_release_pending_drain(afw_xctx_t *xctx)
{
    const afw_reference_t *instance;

    while (xctx->release_pending_count > 0) {
        instance = xctx->release_pending[--xctx->release_pending_count];
        xctx->release_depth = 1;
        afw_reference_release(instance, xctx);
        xctx->release_depth = 0;
    }
    free((void *)xctx->release_pending);
    xctx->release_pending = NULL;
    xctx->release_pending_cap = 0;
}


AFW_DEFINE(void)
afw_reference_release_held(
    const afw_reference_t *instance,
    afw_xctx_t *xctx)
{
    if (!instance) {
        return;
    }
    if (xctx->release_depth >= AFW_REFERENCE_RELEASE_DEPTH_MAX) {
        impl_release_pending_push(instance, xctx);
        return;
    }
    xctx->release_depth++;
    afw_reference_release(instance, xctx);
    xctx->release_depth--;
    if (xctx->release_depth == 0 && xctx->release_pending_count > 0) {
        impl_release_pending_drain(xctx);
    }
}

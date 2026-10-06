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
        free(walk.nodes);
        free(walk.stack);
        AFW_THROW_ERROR_FZ(general, xctx,
            "afw_reference_check: %ku implementation '%ku' is listed "
            "more times than its count " AFW_SIZE_T_FMT,
            &node->inf->rti.interface_name,
            &node->inf->rti.implementation_id,
            bad->count);
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

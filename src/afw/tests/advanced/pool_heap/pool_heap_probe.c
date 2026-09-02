// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework heap / heap-tracker pool probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"
#include "afw_pool_internal.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

/**
 * @file pool_heap_probe.c
 * @brief C probe for heap and heap-tracker as pool implementations.
 *
 * Script cannot see the allocated list or free-list reuse. This
 * boots a core environment and calls the C pool API.
 *
 * Optional free is afw_pool_free_memory(p, address, size, xctx) on the
 * pool that allocated. General APR pools no-op.
 *
 * Same shape as tests/advanced/pool_alloc/pool_alloc_probe.c.
 *
 * Compiles with extra -I to src/afw/pool for afw_pool_internal.h.
 */

#define IMPL_SIZE_SMALL  ((afw_size_t)32)
#define IMPL_SIZE_MEDIUM ((afw_size_t)64)
#define IMPL_SIZE_SLIGHT ((afw_size_t)56)
#define IMPL_SIZE_LARGE  ((afw_size_t)200)
#define IMPL_SIZE_SCOPE  ((afw_size_t)56)
#define IMPL_CHURN_ITERS 2000
#define IMPL_FREE_WALK_CAP 100000

static afw_size_t
impl_in_use(afw_xctx_t *xctx)
{
    return (afw_size_t)xctx->env->pool_bytes_in_use;
}

static afw_pool_internal_self_t *
impl_self(const afw_pool_t *p)
{
    return (afw_pool_internal_self_t *)p;
}

static int
impl_fail(const char *label, const char *detail)
{
    fprintf(stderr, "%s: %s\n", label, detail);
    return 1;
}

static int
impl_expect_same_ptr(
    const void *got,
    const void *want,
    const char *label)
{
    if (got != want) {
        fprintf(stderr, "%s: pointer %p != %p (no reuse)\n",
            label, got, want);
        return 1;
    }
    return 0;
}

static int
impl_expect_in_use(
    afw_xctx_t *xctx,
    afw_size_t want,
    const char *label)
{
    afw_size_t got;

    got = impl_in_use(xctx);
    if (got != want) {
        fprintf(stderr, "%s: in_use " AFW_SIZE_T_FMT
            " != " AFW_SIZE_T_FMT "\n",
            label, got, want);
        return 1;
    }
    return 0;
}

static int
impl_check_fill(
    const void *p,
    afw_size_t n,
    unsigned char b,
    const char *label)
{
    const unsigned char *s = p;
    afw_size_t i;

    for (i = 0; i < n; i++) {
        if (s[i] != b) {
            fprintf(stderr, "%s: byte " AFW_SIZE_T_FMT
                " is 0x%02x, want 0x%02x\n",
                label, i, s[i], b);
            return 1;
        }
    }
    return 0;
}

/*
 * Heap malloc/free: whole chunk (header + payload) returns to the free
 * list; just-freed same-size reuse; in_use up then back.
 */
static int
impl_heap_malloc_free(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *a;
    void *b;
    afw_size_t before;
    afw_size_t after_alloc;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    before = impl_in_use(xctx);

    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    after_alloc = impl_in_use(xctx);
    if (after_alloc <= before) {
        return impl_fail("heap_malloc_free",
            "in_use did not rise on malloc");
    }
    memset(a, 0xa1, IMPL_SIZE_MEDIUM);

    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_in_use(xctx, before, "heap_malloc_free after free")) {
        return 1;
    }

    b = afw_pool_calloc(heap, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(b, a, "heap_malloc_free reuse")) {
        return 1;
    }
    if (impl_check_fill(b, IMPL_SIZE_MEDIUM, 0,
            "heap_malloc_free calloc reuse zeros"))
    {
        return 1;
    }
    memset(b, 0xa2, IMPL_SIZE_MEDIUM);

    afw_pool_free_memory(heap, b, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_in_use(xctx, before, "heap_malloc_free after second free"))
    {
        return 1;
    }

    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Tracker malloc: block on that tracker's allocated list,
 * in_use up. Allocation comes from the heap free list or heap APR.
 */
static int
impl_tracker_malloc(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *self;
    afw_pool_tracker_node_t *node;
    void *a;
    afw_size_t before;
    afw_size_t after_alloc;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    tracker = afw_pool_tracker_create(heap, xctx);
    self = impl_self(tracker);
    before = impl_in_use(xctx);

    if (self->first_allocated_memory != NULL) {
        return impl_fail("tracker_malloc",
            "allocated list not empty before first malloc");
    }

    a = afw_pool_malloc(tracker, IMPL_SIZE_MEDIUM, xctx);
    after_alloc = impl_in_use(xctx);
    if (after_alloc <= before) {
        return impl_fail("tracker_malloc",
            "in_use did not rise on malloc");
    }

    node = AFW_POOL_TRACKER_NODE(a);
    if (self->first_allocated_memory != node) {
        return impl_fail("tracker_malloc",
            "block is not first on this tracker's allocated list");
    }
    if (node->prev != NULL) {
        return impl_fail("tracker_malloc",
            "first allocated block prev is not NULL");
    }
    if (node->next != NULL) {
        return impl_fail("tracker_malloc",
            "single allocated block next is not NULL");
    }
    if (AFW_POOL_TRACKER_USER_SIZE(node) != IMPL_SIZE_MEDIUM) {
        return impl_fail("tracker_malloc",
            "node size is not the USER size");
    }

    memset(a, 0xb1, IMPL_SIZE_MEDIUM);

    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Tracker optional free: unlink from the tracker list, return the
 * whole WITH_LINKS chunk to the heap. Destroy must not return it
 * a second time. Same-size reuse on that tracker works.
 */
static int
impl_tracker_optional_free(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *self;
    void *a;
    void *b;
    afw_size_t before;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    tracker = afw_pool_tracker_create(heap, xctx);
    self = impl_self(tracker);
    before = impl_in_use(xctx);

    a = afw_pool_malloc(tracker, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0xc1, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(tracker, a, IMPL_SIZE_MEDIUM, xctx);

    if (self->first_allocated_memory != NULL) {
        return impl_fail("tracker_optional_free",
            "block still on allocated list after free");
    }
    if (impl_expect_in_use(xctx, before, "tracker_optional_free after free")) {
        return 1;
    }

    b = afw_pool_malloc(tracker, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(b, a, "tracker_optional_free reuse")) {
        return 1;
    }
    memset(b, 0xc2, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(tracker, b, IMPL_SIZE_MEDIUM, xctx);

    /* Last-release must not double-free the already-returned block. */
    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Tracker last-release: leftovers still on the list go back to the
 * heap. Nested/sibling trackers share that heap; the child returns
 * to the heap, not "lost." Both tracker creates take the heap as
 * parent (scope wiring).
 */
static int
impl_tracker_last_release(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *parent_tracker;
    const afw_pool_t *child_tracker;
    void *a;
    void *b;
    afw_size_t before;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    parent_tracker = afw_pool_tracker_create(heap, xctx);
    before = impl_in_use(xctx);
    child_tracker = afw_pool_tracker_create(heap, xctx);

    a = afw_pool_malloc(child_tracker, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0xd1, IMPL_SIZE_MEDIUM);

    /* Leftover malloc and the child header both return to the heap. */
    afw_pool_release(child_tracker, xctx);
    if (impl_expect_in_use(xctx, before,
            "tracker_last_release after child release"))
    {
        return 1;
    }

    /* First-fit may reuse the freed child header, not leftover `a`. */
    b = afw_pool_malloc(parent_tracker, IMPL_SIZE_MEDIUM, xctx);
    if (!b) {
        return impl_fail("tracker_last_release", "parent malloc after child");
    }
    memset(b, 0xd2, IMPL_SIZE_MEDIUM);

    afw_pool_release(parent_tracker, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Tracker header is a heap user block (in_use), not apr_pcalloc.
 * first_allocated_memory on the tracker must not be that header.
 * Empty destroy free_memory's the header onto the heap free list.
 */
static int
impl_tracker_header(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *heap_self;
    afw_pool_internal_self_t *tracker_self;
    afw_size_t before;
    void *a;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    heap_self = impl_self(heap);
    before = impl_in_use(xctx);

    tracker = afw_pool_tracker_create(heap, xctx);
    tracker_self = impl_self(tracker);

    if (impl_in_use(xctx) <= before) {
        return impl_fail("tracker_header",
            "creating a tracker did not raise in_use (header is a heap block)");
    }
    if (tracker_self->first_allocated_memory != NULL) {
        return impl_fail("tracker_header",
            "first_allocated_memory points at the tracker header");
    }

    afw_pool_release(tracker, xctx);

    if (heap_self->free_memory_head == NULL ||
        heap_self->free_memory_head->first == NULL)
    {
        return impl_fail("tracker_header",
            "empty tracker destroy did not free_memory the header");
    }
    if (impl_expect_in_use(xctx, before,
            "tracker_header after empty tracker release"))
    {
        return 1;
    }

    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0xe1, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);

    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Just-freed same-size reuse at two different sizes. Does not require
 * non-adjacent fragments to land on the free list (P3).
 */
static int
impl_mixed_sizes(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *small_a;
    void *small_b;
    void *large_a;
    void *large_b;

    /* Separate heaps so leftover 32 does not combine with 200. */
    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    small_a = afw_pool_malloc(heap, IMPL_SIZE_SMALL, xctx);
    memset(small_a, 0x11, IMPL_SIZE_SMALL);
    afw_pool_free_memory(heap, small_a, IMPL_SIZE_SMALL, xctx);
    small_b = afw_pool_malloc(heap, IMPL_SIZE_SMALL, xctx);
    if (impl_expect_same_ptr(small_b, small_a, "mixed_sizes small reuse")) {
        return 1;
    }
    afw_pool_free_memory(heap, small_b, IMPL_SIZE_SMALL, xctx);
    afw_pool_release(heap, xctx);

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    large_a = afw_pool_malloc(heap, IMPL_SIZE_LARGE, xctx);
    memset(large_a, 0x22, IMPL_SIZE_LARGE);
    afw_pool_free_memory(heap, large_a, IMPL_SIZE_LARGE, xctx);
    large_b = afw_pool_malloc(heap, IMPL_SIZE_LARGE, xctx);
    if (impl_expect_same_ptr(large_b, large_a, "mixed_sizes large reuse")) {
        return 1;
    }
    memset(large_b, 0x33, IMPL_SIZE_LARGE);
    afw_pool_free_memory(heap, large_b, IMPL_SIZE_LARGE, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Remainder too small for a free node stays on the list, so the
 * original USER size can reuse.
 */
static int
impl_heap_whole_block(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *a;
    void *b;
    void *c;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0x41, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);

    b = afw_pool_malloc(heap, IMPL_SIZE_SLIGHT, xctx);
    memset(b, 0x42, IMPL_SIZE_SLIGHT);
    afw_pool_free_memory(heap, b, IMPL_SIZE_SLIGHT, xctx);

    c = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(c, a, "heap_whole_block original reuse")) {
        return 1;
    }
    memset(c, 0x43, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, c, IMPL_SIZE_MEDIUM, xctx);

    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * env->p is a multithreaded heap; optional free recycles.
 */
static int
impl_general_free_noop(afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    void *a;
    void *b;
    afw_size_t before;

    p = xctx->env->p;
    if (!afw_pool_internal_is_heap(p) ||
        !afw_pool_internal_is_heap_multithreaded(p))
    {
        return impl_fail("general_free_noop",
            "env->p is not a multithreaded heap");
    }
    before = impl_in_use(xctx);
    a = afw_pool_malloc(p, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_free_memory(p, a, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_in_use(xctx, before, "general_free_noop after free")) {
        return 1;
    }
    b = afw_pool_malloc(p, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(b, a, "general_free_noop reuse")) {
        return 1;
    }
    afw_pool_free_memory(p, b, IMPL_SIZE_MEDIUM, xctx);
    return 0;
}

static int
impl_tracker_parent(afw_xctx_t *xctx)
{
    const afw_pool_t *tracker;

    tracker = afw_pool_tracker_create(xctx->env->p, xctx);
    if (!afw_pool_internal_is_tracker(tracker)) {
        return impl_fail("tracker_parent",
            "tracker under env->p heap failed");
    }
    afw_pool_release(tracker, xctx);
    return 0;
}

static int
impl_create_child_of_heap(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *child;
    const afw_pool_t *tracker;
    void *a;
    void *b;
    afw_size_t before;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    child = afw_pool_create(heap, xctx);
    if (!afw_pool_internal_is_heap(child)) {
        return impl_fail("create_child_of_heap",
            "afw_pool_create of a heap parent is not a heap");
    }
    before = impl_in_use(xctx);
    a = afw_pool_malloc(child, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_free_memory(child, a, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_in_use(xctx, before, "create_child_of_heap after free")) {
        return 1;
    }
    b = afw_pool_malloc(child, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(b, a, "create_child_of_heap reuse")) {
        return 1;
    }
    afw_pool_free_memory(child, b, IMPL_SIZE_MEDIUM, xctx);

    tracker = afw_pool_tracker_create(child, xctx);
    if (!afw_pool_internal_is_tracker(tracker)) {
        return impl_fail("create_child_of_heap",
            "tracker under create() heap failed");
    }
    afw_pool_release(tracker, xctx);
    afw_pool_release(child, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

#ifdef AFW_DEBUG_POOL
static int
impl_debug_free_wrong_size(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *a;
    int threw;
    int unexpected;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    threw = 0;
    unexpected = 0;
    AFW_TRY {
        afw_pool_free_memory(heap, a, IMPL_SIZE_SMALL, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == afw_error_code_general &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "size does not match"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "debug_free_wrong_size: threw %s\n",
                AFW_ERROR_THROWN->message_z
                    ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;
    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_release(heap, xctx);
    if (unexpected) {
        return 1;
    }
    if (!threw) {
        return impl_fail("debug_free_wrong_size", "did not throw");
    }
    return 0;
}

static int
impl_debug_free_wrong_pool(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    void *a;
    int threw;
    int unexpected;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    tracker = afw_pool_tracker_create(heap, xctx);
    a = afw_pool_malloc(tracker, IMPL_SIZE_MEDIUM, xctx);
    threw = 0;
    unexpected = 0;
    AFW_TRY {
        afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == afw_error_code_general &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "pool does not match"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "debug_free_wrong_pool: threw %s\n",
                AFW_ERROR_THROWN->message_z
                    ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;
    afw_pool_free_memory(tracker, a, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
    if (unexpected) {
        return 1;
    }
    if (!threw) {
        return impl_fail("debug_free_wrong_pool", "did not throw");
    }
    return 0;
}

static int
impl_debug_free_poisons_user(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *a;
    afw_size_t i;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0x11, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);
    for (i = 0; i < IMPL_SIZE_MEDIUM / sizeof(afw_size_t); i++) {
        if (((afw_size_t *)a)[i] != AFW_POOL_DEBUG_POISON) {
            afw_pool_release(heap, xctx);
            return impl_fail("debug_free_poisons_user",
                "USER not filled with poison");
        }
    }
    afw_pool_release(heap, xctx);
    return 0;
}
#endif


static int
impl_double_free_throws(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *a;
    int threw;
    int unexpected;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0x77, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);

    threw = 0;
    unexpected = 0;
    AFW_TRY {
        afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == afw_error_code_general &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "already freed"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "double_free_throws: threw %s\n",
                AFW_ERROR_THROWN->message_z
                    ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;

    afw_pool_release(heap, xctx);

    if (unexpected) {
        return 1;
    }
    if (!threw) {
        return impl_fail("double_free_throws", "did not throw");
    }
    return 0;
}

/*
 * get_apr_pool() is a door for leftover APR calls, not the heap store.
 * Heap returns its reservoir for now. Tracker creates a child of that
 * reservoir on first call, without opening the heap door.
 */
static int
impl_get_apr_pool(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *heap_self;
    afw_pool_internal_self_t *tracker_self;
    apr_pool_t *a;
    apr_pool_t *b;
    apr_pool_t *heap_door;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    tracker = afw_pool_tracker_create(heap, xctx);
    heap_self = impl_self(heap);
    tracker_self = impl_self(tracker);

    if (tracker_self->public_apr_p != NULL) {
        return impl_fail("get_apr_pool",
            "tracker public APR exists before first call");
    }
    if (heap_self->public_apr_p != NULL) {
        return impl_fail("get_apr_pool",
            "heap public door was already open");
    }

    a = afw_pool_get_apr_pool(tracker);
    if (!a) {
        return impl_fail("get_apr_pool", "tracker door returned NULL");
    }
    if (tracker_self->public_apr_p != a) {
        return impl_fail("get_apr_pool",
            "tracker public_apr_p is not the returned pool");
    }
    if (a == heap_self->apr_p) {
        return impl_fail("get_apr_pool",
            "tracker door returned the heap reservoir");
    }
    if (apr_pool_parent_get(a) != heap_self->apr_p) {
        return impl_fail("get_apr_pool",
            "tracker APR parent is not the heap reservoir");
    }
    if (heap_self->public_apr_p != NULL) {
        return impl_fail("get_apr_pool",
            "tracker door opened the heap public door");
    }

    b = afw_pool_get_apr_pool(tracker);
    if (impl_expect_same_ptr(b, a, "get_apr_pool tracker second call")) {
        return 1;
    }

    heap_door = afw_pool_get_apr_pool(heap);
    if (heap_door != heap_self->apr_p) {
        return impl_fail("get_apr_pool",
            "heap door is not the reservoir (for now)");
    }

    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

static void
impl_cleanup_nop(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data;
    (void)data2;
    (void)p;
    (void)xctx;
}

static int
impl_deregister_cleanup(afw_xctx_t *xctx)
{
    static int marker;
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *self;
    void *entry;
    void *again;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    tracker = afw_pool_tracker_create(heap, xctx);
    self = impl_self(tracker);

    if (self->first_allocated_memory != NULL) {
        return impl_fail("deregister_cleanup", "list not empty at start");
    }

    afw_pool_register_cleanup_before(tracker, &marker, NULL,
        impl_cleanup_nop, xctx);
    if (self->first_allocated_memory == NULL) {
        return impl_fail("deregister_cleanup",
            "cleanup entry not on allocated list");
    }
    entry = AFW_POOL_TRACKER_TO_USER(self->first_allocated_memory);

    afw_pool_deregister_cleanup(tracker, &marker, NULL,
        impl_cleanup_nop, xctx);
    if (self->first_allocated_memory != NULL) {
        return impl_fail("deregister_cleanup",
            "cleanup entry still on allocated list");
    }

    again = afw_pool_malloc(tracker, sizeof(afw_pool_cleanup_t), xctx);
    if (impl_expect_same_ptr(again, entry, "deregister_cleanup reuse")) {
        return 1;
    }
    afw_pool_free_memory(tracker, again, sizeof(afw_pool_cleanup_t), xctx);

    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Free A and C with B still live between them. Both must reuse.
 */
static int
impl_nonadjacent_reuse(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    void *a;
    void *b;
    void *c;
    void *d;
    void *e;

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    b = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    c = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0x51, IMPL_SIZE_MEDIUM);
    memset(b, 0x52, IMPL_SIZE_MEDIUM);
    memset(c, 0x53, IMPL_SIZE_MEDIUM);

    afw_pool_free_memory(heap, a, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_free_memory(heap, c, IMPL_SIZE_MEDIUM, xctx);

    d = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    e = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    if (!((d == a && e == c) || (d == c && e == a))) {
        fprintf(stderr,
            "nonadjacent_reuse: %p %p not reuse of %p and %p\n",
            d, e, a, c);
        return 1;
    }
    memset(d, 0x54, IMPL_SIZE_MEDIUM);
    memset(e, 0x55, IMPL_SIZE_MEDIUM);
    memset(b, 0x56, IMPL_SIZE_MEDIUM);

    afw_pool_free_memory(heap, b, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_free_memory(heap, d, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_free_memory(heap, e, IMPL_SIZE_MEDIUM, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Walk the heap free list. A cycle or a walk that never ends is the
 * first-fit livelock (tracker calloc of ~56 while many smaller
 * fragments are on the list).
 */
static int
impl_free_list_walk_ok(
    afw_pool_internal_self_t *heap_self,
    const char *label)
{
    afw_pool_free_node_t *slow;
    afw_pool_free_node_t *fast;
    afw_size_t steps;

    if (!heap_self->free_memory_head) {
        return 0;
    }
    slow = heap_self->free_memory_head->first;
    fast = slow;
    steps = 0;
    while (slow) {
        steps++;
        if (steps > IMPL_FREE_WALK_CAP) {
            fprintf(stderr,
                "%s: free-list walk exceeded " AFW_SIZE_T_FMT
                " nodes (cycle or unbounded)\n",
                label, (afw_size_t)IMPL_FREE_WALK_CAP);
            return 1;
        }
        if (fast) {
            fast = fast->next;
        }
        if (fast) {
            fast = fast->next;
        }
        if (fast && fast == slow) {
            fprintf(stderr, "%s: free-list next cycle at %p\n",
                label, (void *)slow);
            return 1;
        }
        slow = slow->next;
    }
    return 0;
}

/*
 * C-style for clone: new tracker, calloc ~56 (scope), mixed-size
 * optional frees on the shared heap, last-release. Then another
 * tracker calloc of 56 — the gdb hang from boxing.
 *
 * The hang is a cycle: a tracker-allocated chunk is returned to the
 * heap free list (optional free with the heap as p) without unlinking
 * from the tracker. Last-release then walks memory->next as if it
 * were still the allocated list.
 */
static void
impl_churn_alarm(int sig)
{
    (void)sig;
    fprintf(stderr,
        "for_clone_churn: timed out (free-list walk likely hung)\n");
    _exit(1);
}

static int
impl_for_clone_churn(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *heap_self;
    void *scope;
    void *small;
    afw_size_t i;
    afw_size_t small_size;
    static const afw_size_t small_sizes[] = { 16, 24, 32, 40, 48 };

    heap = afw_pool_create_xctx_p(xctx->p, xctx);
    heap_self = impl_self(heap);
    signal(SIGALRM, impl_churn_alarm);
    alarm(15);

    for (i = 0; i < IMPL_CHURN_ITERS; i++) {
        if (impl_free_list_walk_ok(heap_self, "for_clone_churn before calloc"))
        {
            alarm(0);
            return 1;
        }
        tracker = afw_pool_tracker_create(heap, xctx);
        scope = afw_pool_calloc(tracker, IMPL_SIZE_SCOPE, xctx);
        memset(scope, 0xa5, IMPL_SIZE_SCOPE);

        small_size = small_sizes[i % 5];
        /* Allocate on the tracker and free on the tracker (correct p).
         * Last-release must not livelock. */
        small = afw_pool_malloc(tracker, small_size, xctx);
        memset(small, 0x5a, small_size);
        afw_pool_free_memory(tracker, small, small_size, xctx);

        afw_pool_release(tracker, xctx);

        if (impl_free_list_walk_ok(heap_self, "for_clone_churn")) {
            alarm(0);
            return 1;
        }
    }

    tracker = afw_pool_tracker_create(heap, xctx);
    scope = afw_pool_calloc(tracker, IMPL_SIZE_SCOPE, xctx);
    memset(scope, 0xa5, IMPL_SIZE_SCOPE);
    if (impl_free_list_walk_ok(heap_self, "for_clone_churn after calloc")) {
        alarm(0);
        return 1;
    }
    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
    alarm(0);
    return 0;
}

int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    rc = 0;

    if (strcmp(case_name, "heap_malloc_free") == 0) {
        rc = impl_heap_malloc_free(xctx);
    }
    else if (strcmp(case_name, "tracker_malloc") == 0) {
        rc = impl_tracker_malloc(xctx);
    }
    else if (strcmp(case_name, "tracker_optional_free") == 0) {
        rc = impl_tracker_optional_free(xctx);
    }
    else if (strcmp(case_name, "tracker_last_release") == 0) {
        rc = impl_tracker_last_release(xctx);
    }
    else if (strcmp(case_name, "tracker_header") == 0) {
        rc = impl_tracker_header(xctx);
    }
    else if (strcmp(case_name, "mixed_sizes") == 0) {
        rc = impl_mixed_sizes(xctx);
    }
    else if (strcmp(case_name, "heap_whole_block") == 0) {
        rc = impl_heap_whole_block(xctx);
    }
    else if (strcmp(case_name, "general_free_noop") == 0) {
        rc = impl_general_free_noop(xctx);
    }
    else if (strcmp(case_name, "tracker_parent") == 0) {
        rc = impl_tracker_parent(xctx);
    }
    else if (strcmp(case_name, "create_child_of_heap") == 0) {
        rc = impl_create_child_of_heap(xctx);
    }
    else if (strcmp(case_name, "get_apr_pool") == 0) {
        rc = impl_get_apr_pool(xctx);
    }
    else if (strcmp(case_name, "deregister_cleanup") == 0) {
        rc = impl_deregister_cleanup(xctx);
    }
    else if (strcmp(case_name, "nonadjacent_reuse") == 0) {
        rc = impl_nonadjacent_reuse(xctx);
    }
    else if (strcmp(case_name, "for_clone_churn") == 0) {
        rc = impl_for_clone_churn(xctx);
    }
    else if (strcmp(case_name, "double_free_throws") == 0) {
        rc = impl_double_free_throws(xctx);
    }
#ifdef AFW_DEBUG_POOL
    else if (strcmp(case_name, "debug_free_wrong_size") == 0) {
        rc = impl_debug_free_wrong_size(xctx);
    }
    else if (strcmp(case_name, "debug_free_wrong_pool") == 0) {
        rc = impl_debug_free_wrong_pool(xctx);
    }
    else if (strcmp(case_name, "debug_free_poisons_user") == 0) {
        rc = impl_debug_free_poisons_user(xctx);
    }
#endif
    else {
        fprintf(stderr, "usage: pool_heap_probe "
            "heap_malloc_free|tracker_malloc|tracker_optional_free|"
            "tracker_last_release|tracker_header|mixed_sizes|"
            "heap_whole_block|general_free_noop|tracker_parent|"
            "get_apr_pool|deregister_cleanup|nonadjacent_reuse|"
            "for_clone_churn|create_child_of_heap|double_free_throws"
#ifdef AFW_DEBUG_POOL
            "|debug_free_wrong_size|debug_free_wrong_pool"
            "|debug_free_poisons_user"
#endif
            "\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}

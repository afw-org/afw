// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework heap / heap-tracker pool probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"
#include "afw_pool_heap_internal.h"

#include <stdio.h>
#include <string.h>

/**
 * @file pool_heap_probe.c
 * @brief C probe for heap and heap-tracker as pool implementations.
 *
 * Script cannot see prefixes, the tracker allocated list, or free-list
 * reuse. This boots a core environment and calls the C pool API.
 *
 * Optional free is afw_pool_free_memory(p, address, xctx) on the pool
 * that allocated. General APR pools no-op.
 *
 * Same shape as tests/advanced/pool_alloc/pool_alloc_probe.c.
 *
 * Compiles with extra -I to src/afw/pool for afw_pool_heap_internal.h.
 */

#define IMPL_SIZE_SMALL  ((afw_size_t)32)
#define IMPL_SIZE_MEDIUM ((afw_size_t)64)
#define IMPL_SIZE_SLIGHT ((afw_size_t)56)
#define IMPL_SIZE_LARGE  ((afw_size_t)200)

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
 * Heap malloc/free: whole chunk (prefix + payload) returns to the free
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

    heap = afw_pool_heap_create(xctx->p, xctx);
    before = impl_in_use(xctx);

    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    after_alloc = impl_in_use(xctx);
    if (after_alloc <= before) {
        return impl_fail("heap_malloc_free",
            "in_use did not rise on malloc");
    }
    memset(a, 0xa1, IMPL_SIZE_MEDIUM);

    afw_pool_free_memory(heap, a, xctx);
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

    afw_pool_free_memory(heap, b, xctx);
    if (impl_expect_in_use(xctx, before, "heap_malloc_free after second free"))
    {
        return 1;
    }

    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Tracker malloc: prefix-with-links, block on that tracker's list,
 * in_use up. Allocation comes from the heap free list or heap APR.
 */
static int
impl_tracker_malloc(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    const afw_pool_t *tracker;
    afw_pool_internal_self_t *self;
    afw_pool_internal_memory_prefix_with_links_t *block;
    void *a;
    afw_size_t before;
    afw_size_t after_alloc;

    heap = afw_pool_heap_create(xctx->p, xctx);
    tracker = afw_pool_heap_tracker_create(heap, xctx);
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

    block = AFW_POOL_INTERNAL_MEMORY_PREFIX_WITH_LINKS(a);
    if (self->first_allocated_memory != block) {
        return impl_fail("tracker_malloc",
            "block is not first on this tracker's allocated list");
    }
    if (block->common.p != tracker) {
        return impl_fail("tracker_malloc",
            "prefix p is not the tracker");
    }
    if (block->prev != NULL) {
        return impl_fail("tracker_malloc",
            "first allocated block prev is not NULL");
    }
    if (block->next != NULL) {
        return impl_fail("tracker_malloc",
            "single allocated block next is not NULL");
    }
    if (block->common.size <
        IMPL_SIZE_MEDIUM +
        sizeof(afw_pool_internal_memory_prefix_with_links_t))
    {
        return impl_fail("tracker_malloc",
            "prefix size is smaller than user + with-links prefix");
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

    heap = afw_pool_heap_create(xctx->p, xctx);
    tracker = afw_pool_heap_tracker_create(heap, xctx);
    self = impl_self(tracker);
    before = impl_in_use(xctx);

    a = afw_pool_malloc(tracker, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0xc1, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(tracker, a, xctx);

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
    afw_pool_free_memory(tracker, b, xctx);

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

    heap = afw_pool_heap_create(xctx->p, xctx);
    parent_tracker = afw_pool_heap_tracker_create(heap, xctx);
    child_tracker = afw_pool_heap_tracker_create(heap, xctx);
    before = impl_in_use(xctx);

    a = afw_pool_malloc(child_tracker, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0xd1, IMPL_SIZE_MEDIUM);

    /* No optional free: leftover must return on last release. */
    afw_pool_release(child_tracker, xctx);
    if (impl_expect_in_use(xctx, before,
            "tracker_last_release after child release"))
    {
        return 1;
    }

    b = afw_pool_malloc(parent_tracker, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(b, a, "tracker_last_release sibling reuse")) {
        return 1;
    }
    memset(b, 0xd2, IMPL_SIZE_MEDIUM);

    afw_pool_release(parent_tracker, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * Tracker header is apr_pcalloc on the heap APR pool (RSS, not in_use).
 * first_allocated_memory must not be that header. Empty last-release
 * must not put the header on the heap free list.
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

    heap = afw_pool_heap_create(xctx->p, xctx);
    heap_self = impl_self(heap);
    before = impl_in_use(xctx);

    tracker = afw_pool_heap_tracker_create(heap, xctx);
    tracker_self = impl_self(tracker);

    if (impl_in_use(xctx) != before) {
        return impl_fail("tracker_header",
            "creating a tracker changed in_use (header is RSS)");
    }
    if (tracker_self->first_allocated_memory != NULL) {
        return impl_fail("tracker_header",
            "first_allocated_memory points at the tracker header");
    }
    if (heap_self->free_memory_head == NULL ||
        heap_self->free_memory_head->first != NULL)
    {
        return impl_fail("tracker_header",
            "heap free list not empty after tracker create");
    }

    afw_pool_release(tracker, xctx);

    if (heap_self->free_memory_head->first != NULL) {
        return impl_fail("tracker_header",
            "empty tracker destroy put the header on the heap free list");
    }
    if (impl_expect_in_use(xctx, before,
            "tracker_header after empty tracker release"))
    {
        return 1;
    }

    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    memset(a, 0xe1, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, a, xctx);

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

    /* Separate heaps so a leftover 32-byte free does not combine with
     * the 200-byte chunk (adjacent-only first-fit). Each size is
     * just-freed same-size reuse, which must work. Non-adjacent
     * fragments are not required to land on the list (P3).
     */
    heap = afw_pool_heap_create(xctx->p, xctx);
    small_a = afw_pool_malloc(heap, IMPL_SIZE_SMALL, xctx);
    memset(small_a, 0x11, IMPL_SIZE_SMALL);
    afw_pool_free_memory(heap, small_a, xctx);
    small_b = afw_pool_malloc(heap, IMPL_SIZE_SMALL, xctx);
    if (impl_expect_same_ptr(small_b, small_a, "mixed_sizes small reuse")) {
        return 1;
    }
    afw_pool_free_memory(heap, small_b, xctx);
    afw_pool_release(heap, xctx);

    heap = afw_pool_heap_create(xctx->p, xctx);
    large_a = afw_pool_malloc(heap, IMPL_SIZE_LARGE, xctx);
    memset(large_a, 0x22, IMPL_SIZE_LARGE);
    afw_pool_free_memory(heap, large_a, xctx);
    large_b = afw_pool_malloc(heap, IMPL_SIZE_LARGE, xctx);
    if (impl_expect_same_ptr(large_b, large_a, "mixed_sizes large reuse")) {
        return 1;
    }
    memset(large_b, 0x33, IMPL_SIZE_LARGE);
    afw_pool_free_memory(heap, large_b, xctx);
    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * First-fit takes the whole free block when the remainder is too
 * small to keep. The recorded size must be that whole block, not
 * the requested size, or a later same-as-original alloc cannot reuse.
 *
 * Heap prefix 16: user 64 → 80, user 56 → 72, remainder 8 < 16.
 */
static int
impl_heap_whole_block(afw_xctx_t *xctx)
{
    const afw_pool_t *heap;
    afw_pool_internal_memory_prefix_t *block;
    void *a;
    void *b;
    void *c;
    afw_size_t original_chunk;

    heap = afw_pool_heap_create(xctx->p, xctx);
    a = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    block = AFW_POOL_INTERNAL_MEMORY_PREFIX(a);
    original_chunk = block->size;
    memset(a, 0x41, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, a, xctx);

    b = afw_pool_malloc(heap, IMPL_SIZE_SLIGHT, xctx);
    if (impl_expect_same_ptr(b, a, "heap_whole_block slight reuse")) {
        return 1;
    }
    block = AFW_POOL_INTERNAL_MEMORY_PREFIX(b);
    if (block->size != original_chunk) {
        fprintf(stderr,
            "heap_whole_block: took whole block of " AFW_SIZE_T_FMT
            " but recorded " AFW_SIZE_T_FMT "\n",
            original_chunk, block->size);
        return 1;
    }
    memset(b, 0x42, IMPL_SIZE_SLIGHT);
    afw_pool_free_memory(heap, b, xctx);

    c = afw_pool_malloc(heap, IMPL_SIZE_MEDIUM, xctx);
    if (impl_expect_same_ptr(c, a, "heap_whole_block original reuse")) {
        return 1;
    }
    memset(c, 0x43, IMPL_SIZE_MEDIUM);
    afw_pool_free_memory(heap, c, xctx);

    afw_pool_release(heap, xctx);
    return 0;
}

/*
 * General APR pools stay destroy-is-lifetime; optional free is a no-op.
 */
static int
impl_general_free_noop(afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    void *a;
    afw_size_t after_alloc;

    p = afw_pool_create(xctx->p, xctx);
    a = afw_pool_malloc(p, IMPL_SIZE_MEDIUM, xctx);
    after_alloc = impl_in_use(xctx);
    memset(a, 0xf1, IMPL_SIZE_MEDIUM);

    afw_pool_free_memory(p, a, xctx);
    if (impl_expect_in_use(xctx, after_alloc, "general_free_noop")) {
        return 1;
    }

    afw_pool_release(p, xctx);
    return 0;
}

static int
impl_tracker_parent(afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    int threw;
    int unexpected;

    p = afw_pool_create(xctx->p, xctx);
    threw = 0;
    unexpected = 0;
    AFW_TRY {
        (void)afw_pool_heap_tracker_create(p, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == afw_error_code_general &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "parent must be a heap"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "tracker_parent: threw %s\n",
                AFW_ERROR_THROWN->message_z
                    ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;

    afw_pool_release(p, xctx);

    if (unexpected) {
        return 1;
    }
    if (!threw) {
        return impl_fail("tracker_parent", "did not throw");
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

    heap = afw_pool_heap_create(xctx->p, xctx);
    tracker = afw_pool_heap_tracker_create(heap, xctx);
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

    heap = afw_pool_heap_create(xctx->p, xctx);
    tracker = afw_pool_heap_tracker_create(heap, xctx);
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
    entry = ((char *)self->first_allocated_memory) +
        sizeof(afw_pool_internal_memory_prefix_with_links_t);

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
    afw_pool_free_memory(tracker, again, xctx);

    afw_pool_release(tracker, xctx);
    afw_pool_release(heap, xctx);
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
    else if (strcmp(case_name, "get_apr_pool") == 0) {
        rc = impl_get_apr_pool(xctx);
    }
    else if (strcmp(case_name, "deregister_cleanup") == 0) {
        rc = impl_deregister_cleanup(xctx);
    }
    else {
        fprintf(stderr, "usage: pool_heap_probe "
            "heap_malloc_free|tracker_malloc|tracker_optional_free|"
            "tracker_last_release|tracker_header|mixed_sizes|"
            "heap_whole_block|general_free_noop|tracker_parent|"
            "get_apr_pool|deregister_cleanup\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}

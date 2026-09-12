// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap and tracker pool implementation.
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_pool.c
 * @brief Heap and tracker pool implementation.
 *
 * A pool is a heap unless it is a tracker. A tracker gets memory from
 * a heap, tracks live USER blocks, and returns them to the heap on
 * free or tracker destroy. The heap owns the free list (overlay on
 * freed blocks; else a bump from a 4k-aligned chunk). Destroy walks
 * the chunk list and free()s each. Single-thread heaps: create/use/
 * release on one thread. Multithreaded heap is lock wrappers.
 * Heap live: [USER] or [size][pool][USER] if AFW_DEBUG_POOL.
 * Tracker live: [prev][next][size][USER], plus [pool] if debug.
 * Debug free fills USER with AFW_POOL_DEBUG_POISON (bad inf).
 */

#include "afw_internal.h"
#include <stdio.h>
#include <stdlib.h>

/* multithreaded pool lock begin */
#define IMPL_MULTITHREADED_LOCK_BEGIN(xctx) \
AFW_LOCK_BEGIN((xctx)->env->multithreaded_pool_lock)

/* multithreaded pool lock end */
#define IMPL_MULTITHREADED_LOCK_END \
AFW_LOCK_END;

/*
 * The pool methods begin with 'impl_afw_pool_' only.
 */
#define AFW_POOL_SELF_T afw_pool_internal_self_t

#define AFW_IMPLEMENTATION_ID "heap"

static const afw_pool_internal_inf_implementation_specific_t
impl_pool_implementation_specific =
    {
        /* multithreaded */ false,
        /* tracker */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_implementation_specific

static void
impl_heap_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_destroy impl_heap_afw_pool_destroy

static void *
impl_heap_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_calloc impl_heap_afw_pool_calloc

static void *
impl_heap_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_malloc impl_heap_afw_pool_malloc

static void
impl_heap_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_free_memory impl_heap_afw_pool_free_memory

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory

#define AFW_POOL_INF_ONLY 1

/*
 * Some tracker methods begin with 'impl_tracker_afw_pool_'; the rest
 * match the heap methods.
 */
#define AFW_IMPLEMENTATION_ID "tracker"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_tracker_inf

static void
impl_tracker_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_destroy \
    impl_tracker_afw_pool_destroy

static void *
impl_tracker_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc \
    impl_tracker_afw_pool_calloc

static void *
impl_tracker_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_malloc \
    impl_tracker_afw_pool_malloc

static void
impl_tracker_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free_memory \
    impl_tracker_afw_pool_free_memory

static const afw_pool_internal_inf_implementation_specific_t
impl_tracker_implementation_specific =
    {
        /* multithreaded */ false,
        /* tracker */ true
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_tracker_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory


/* --------------------------- internal macros ------------------------------ */

#ifdef AFW_DEBUG_POOL

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool %s " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            info_z, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool " format_z " " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            __VA_ARGS__, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#else

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z)
#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...)

#endif


static void
impl_account_alloc(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated += consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use += consumed;
    }
}


static void
impl_account_free(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated -= consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -= consumed;
    }
}


static void
impl_account_destroy(afw_pool_internal_self_t *self, afw_xctx_t *xctx)
{
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -=
            self->bytes_allocated;
    }
    self->bytes_allocated = 0;
}


static afw_pool_internal_self_t *
impl_reservoir_heap(afw_pool_internal_self_t *self)
{
    while (self->parent) {
        self = self->parent;
    }
    return self;
}


static char *
impl_chunk_usable(afw_pool_chunk_t *chunk)
{
    return (char *)chunk + AFW_POOL_ALIGN_UP(sizeof(afw_pool_chunk_t));
}


static char *
impl_chunk_end(afw_pool_chunk_t *chunk)
{
    return (char *)chunk + chunk->size;
}


static afw_pool_chunk_t *
impl_chunk_containing(afw_pool_internal_self_t *heap, void *addr)
{
    afw_pool_chunk_t *chunk;
    char *p;

    p = (char *)addr;
    for (chunk = heap->first_chunk; chunk; chunk = chunk->next) {
        if (p >= (char *)chunk && p < impl_chunk_end(chunk)) {
            return chunk;
        }
    }
    return NULL;
}


static afw_boolean_t
impl_same_chunk(afw_pool_internal_self_t *heap, void *a, void *b)
{
    afw_pool_chunk_t *ca;
    afw_pool_chunk_t *cb;

    ca = impl_chunk_containing(heap, a);
    cb = impl_chunk_containing(heap, b);
    return (ca && ca == cb);
}


static afw_size_t
impl_chunk_need(afw_size_t min_payload)
{
    afw_size_t header;
    afw_size_t need;
    afw_size_t rem;

    header = AFW_POOL_ALIGN_UP(sizeof(afw_pool_chunk_t));
    if (min_payload > AFW_SIZE_T_MAX - header) {
        return 0;
    }
    need = header + min_payload;
    if (need < AFW_POOL_CHUNK_MIN) {
        need = AFW_POOL_CHUNK_MIN;
    }
    /* Whole pages so posix_memalign/aligned_alloc 4k is legal. */
    rem = need & (AFW_POOL_CHUNK_MIN - 1);
    if (rem) {
        if (need > AFW_SIZE_T_MAX - (AFW_POOL_CHUNK_MIN - rem)) {
            return 0;
        }
        need += AFW_POOL_CHUNK_MIN - rem;
    }
    return need;
}


static afw_pool_chunk_t *
impl_chunk_malloc(afw_size_t min_payload)
{
    afw_size_t need;
    void *mem;
    afw_pool_chunk_t *chunk;
    int rv;

    need = impl_chunk_need(min_payload);
    if (need == 0) {
        return NULL;
    }
    mem = NULL;
    rv = posix_memalign(&mem, AFW_POOL_CHUNK_MIN, need);
    if (rv != 0 || !mem) {
        return NULL;
    }
    chunk = (afw_pool_chunk_t *)mem;
    chunk->next = NULL;
    chunk->size = need;
    return chunk;
}


static afw_pool_internal_self_t *
impl_heap_allocate_self(const afw_pool_inf_t *inf)
{
    afw_size_t self_bytes;
    afw_pool_chunk_t *chunk;
    afw_pool_internal_self_with_free_memory_head_t *mem;
    afw_pool_internal_self_t *self;
    char *usable;
    char *after_self;

    self_bytes = AFW_POOL_ALIGN_UP(
        sizeof(afw_pool_internal_self_with_free_memory_head_t));
    chunk = impl_chunk_malloc(self_bytes);
    if (!chunk) {
        return NULL;
    }
    usable = impl_chunk_usable(chunk);
    mem = (afw_pool_internal_self_with_free_memory_head_t *)(void *)usable;
    memset(mem, 0, sizeof(*mem));
    self = &mem->common;
    self->pub.inf = inf;
    self->pub.managed_p = &self->pub;
    self->first_chunk = chunk;
    self->current_chunk = chunk;
    after_self = usable + self_bytes;
    self->bump = after_self;
    self->remaining = (afw_size_t)(impl_chunk_end(chunk) - after_self);
    self->free_memory_head = &mem->memory_for_free_memory_head;
    self->reference_count = 1;
    return self;
}


/* Process base pool. Keeps the 4k chunks reachable until exit. */
static afw_pool_internal_self_t *impl_base_pool_self;


/* --------------------------- internal functions --------------------------- */

static void
impl_add_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_get_reference(&parent->pub, xctx);

    child->next_sibling = parent->first_child;
    parent->first_child = child;
}


static void
impl_remove_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *prev;
    afw_pool_internal_self_t *sibling;

    for (prev = NULL, sibling = parent->first_child;
        sibling;
        prev = sibling, sibling = sibling->next_sibling)
    {
        if (sibling == child) {
            if (!prev) {
                parent->first_child = sibling->next_sibling;
            }
            else {
                prev->next_sibling = sibling->next_sibling;
            }
            break;
        }
    }

    if (!sibling) {
        AFW_THROW_ERROR_Z(general, "Not a child of parent", xctx);
    }

    afw_pool_release(&parent->pub, xctx);
}


/* Create skeleton heap struct. Parent is any AFW pool. */
static afw_pool_internal_self_t *
impl_heap_create(
    const afw_pool_t *afw_parent,
    const afw_pool_inf_t *inf,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_internal_self_t *parent_self;

    self = impl_heap_allocate_self(inf);
    if (!self) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self->parent = NULL;
    self->external_parent = afw_parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);
    self->thread = xctx->thread;

    /*
     * Link onto the parent so destroy (and valgrind) can chase child
     * heaps. parent stays NULL so this heap keeps its own chunks.
     * add_child holds the parent.
     */
    if (afw_parent) {
        parent_self = (afw_pool_internal_self_t *)afw_parent;
        if (parent_self->thread) {
            impl_add_child(parent_self, self, xctx);
        }
        else {
            IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
                impl_add_child(parent_self, self, xctx);
            }
            IMPL_MULTITHREADED_LOCK_END;
        }
    }

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

/* Create skeleton pool struct for tracker. */
static afw_pool_internal_self_t *
impl_create_for_tracker(
    afw_pool_internal_self_t *parent,
    const afw_pool_inf_t *inf,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required for tracker", xctx);
    }

    /*
     * Header is a heap user block so destroy can free_memory it.
     * Not on this tracker’s allocated list.
     */
    self = afw_pool_calloc(&parent->pub,
        sizeof(afw_pool_internal_self_t), xctx);
    self->pub.inf = inf;
    self->pub.managed_p = parent->pub.managed_p
        ? parent->pub.managed_p
        : &parent->pub;
    self->parent = parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);
    self->reference_count = 1;

    /* Trackers allocate from the parent heap free list / chunks. */
    self->free_memory_head = parent->free_memory_head;

    /* If parent, add this new child. */
    if (parent) {
        self->thread = parent->thread;
        if (self->thread) {
            impl_add_child(parent, self, xctx);
        }
        else {
            IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
                impl_add_child(parent, self, xctx);
            }
            IMPL_MULTITHREADED_LOCK_END;
        }
    }

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    /* Return new tracker. */
    return self;
}

/*
 * First-fit on an address-ordered free list. Overlay lives only on
 * freed blocks. Remainder too small to hold a free node is left on
 * the list so total is always recoverable as prefix + USER size.
 */
static void
impl_heap_free_unlink(
    afw_pool_free_node_t **head,
    afw_pool_free_node_t *node)
{
    if (node->prev) {
        node->prev->next = node->next;
    }
    else {
        *head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
}


static void
impl_tracker_unlink(
    afw_pool_tracker_node_t **head,
    afw_pool_tracker_node_t *node)
{
    if (node->prev) {
        node->prev->next = node->next;
    }
    else {
        *head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
}


static afw_size_t
impl_block_bytes(
    afw_size_t prefix_bytes,
    afw_size_t user_size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    afw_size_t need;
    afw_size_t aligned;

    if (prefix_bytes > AFW_SIZE_T_MAX - user_size) {
        if (unhandled) {
            return 0;
        }
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    need = prefix_bytes + user_size;
    if (need < sizeof(afw_pool_free_node_t)) {
        need = sizeof(afw_pool_free_node_t);
    }
    aligned = AFW_POOL_ALIGN_UP(need);
    if (aligned < need) {
        if (unhandled) {
            return 0;
        }
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    return aligned;
}


static void
impl_heap_add_to_free_list(
    AFW_POOL_SELF_T *self,
    void *start,
    afw_size_t total,
    afw_xctx_t *xctx);

static void *
impl_heap_take_from_free_list_or_chunk(
    AFW_POOL_SELF_T *self,
    afw_size_t total,
    afw_boolean_t *reused,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    afw_pool_free_node_t *curr;
    afw_pool_free_node_t *prev;
    afw_pool_free_node_t *next;
    afw_pool_free_node_t *rest;
    afw_pool_free_node_t *slow;
    afw_pool_free_node_t *fast;

    curr = NULL;
    if (self->free_memory_head) {
        slow = self->free_memory_head->first;
        fast = slow;
        for (curr = slow; curr; curr = curr->next) {
            if (curr->total >= total &&
                (curr->total == total ||
                    curr->total - total >= sizeof(afw_pool_free_node_t)))
            {
                break;
            }
            if (fast) {
                fast = fast->next;
            }
            if (fast) {
                fast = fast->next;
            }
            if (fast && fast == curr) {
                if (unhandled) {
                    return NULL;
                }
                AFW_THROW_ERROR_Z(general,
                    "heap free-list cycle",
                    xctx);
            }
        }
    }

    if (curr) {
        prev = curr->prev;
        next = curr->next;
        impl_heap_free_unlink(&self->free_memory_head->first, curr);
        if (curr->total - total >= sizeof(afw_pool_free_node_t)) {
            rest = (afw_pool_free_node_t *)(((char *)curr) + total);
            rest->total = curr->total - total;
            rest->prev = prev;
            if (prev) {
                prev->next = rest;
            }
            else {
                self->free_memory_head->first = rest;
            }
            if (next &&
                ((char *)rest) + rest->total == (char *)next)
            {
                rest->total += next->total;
                rest->next = next->next;
                if (next->next) {
                    next->next->prev = rest;
                }
            }
            else {
                rest->next = next;
                if (next) {
                    next->prev = rest;
                }
            }
        }
        *reused = true;
        return curr;
    }

    *reused = false;
    {
        afw_pool_internal_self_t *heap;
        afw_pool_chunk_t *chunk;
        char *end;
        void *start;

        heap = impl_reservoir_heap(self);
        if (heap->remaining >= total) {
            start = heap->bump;
            heap->bump += total;
            heap->remaining -= total;
            return start;
        }

        if (heap->current_chunk &&
            heap->remaining >= sizeof(afw_pool_free_node_t))
        {
            impl_heap_add_to_free_list(self, heap->bump,
                heap->remaining, xctx);
        }
        heap->bump = NULL;
        heap->remaining = 0;

        chunk = impl_chunk_malloc(total);
        if (!chunk) {
            if (unhandled) {
                return NULL;
            }
            AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
        }
        chunk->next = heap->first_chunk;
        heap->first_chunk = chunk;
        heap->current_chunk = chunk;
        heap->bump = impl_chunk_usable(chunk);
        end = impl_chunk_end(chunk);
        heap->remaining = (afw_size_t)(end - heap->bump);
        if (heap->remaining < total) {
            if (unhandled) {
                return NULL;
            }
            AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
        }
        start = heap->bump;
        heap->bump += total;
        heap->remaining -= total;
        return start;
    }
}


static void
impl_heap_add_to_free_list(
    AFW_POOL_SELF_T *self,
    void *start,
    afw_size_t total,
    afw_xctx_t *xctx)
{
    afw_pool_free_node_t *freeing;
    afw_pool_free_node_t *prev;
    afw_pool_free_node_t *curr;
    afw_pool_internal_self_t *heap;
    afw_pool_internal_free_memory_head_t *head;

    (void)xctx;
    heap = impl_reservoir_heap(self);
    head = heap->free_memory_head;
    if (!head) {
        return;
    }

    freeing = (afw_pool_free_node_t *)start;
    freeing->total = total;
    freeing->prev = NULL;
    freeing->next = NULL;

    prev = NULL;
    curr = head->first;
    while (curr && curr < freeing) {
        prev = curr;
        curr = curr->next;
    }

    freeing->prev = prev;
    freeing->next = curr;
    if (prev) {
        prev->next = freeing;
    }
    else {
        head->first = freeing;
    }
    if (curr) {
        curr->prev = freeing;
    }

    if (curr &&
        ((char *)freeing) + freeing->total == (char *)curr &&
        impl_same_chunk(heap, freeing, curr))
    {
        freeing->total += curr->total;
        freeing->next = curr->next;
        if (curr->next) {
            curr->next->prev = freeing;
        }
    }

    if (prev &&
        ((char *)prev) + prev->total == (char *)freeing &&
        impl_same_chunk(heap, prev, freeing))
    {
        prev->total += freeing->total;
        prev->next = freeing->next;
        if (freeing->next) {
            freeing->next->prev = prev;
        }
    }
}


#ifdef AFW_DEBUG_POOL
static void
impl_debug_prefix_set(
    AFW_POOL_SELF_T *self,
    void *user,
    afw_size_t size)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)user -
        sizeof(afw_pool_debug_prefix_t));
    pre->size = size;
    pre->pool = &self->pub;
}

static void
impl_debug_check_prefix(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)address -
        sizeof(afw_pool_debug_prefix_t));
    if (pre->pool != &self->pub) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: pool does not match allocation",
            xctx);
    }
    if (pre->size != size) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: size does not match allocation",
            xctx);
    }
}

static void
impl_debug_poison_user(void *user, afw_size_t size)
{
    afw_size_t i;
    afw_size_t n;
    afw_size_t *w;
    unsigned char *b;
    unsigned char *end;
    afw_size_t poison;

    if (!user || size == 0) {
        return;
    }
    poison = AFW_POOL_DEBUG_POISON;
    w = (afw_size_t *)user;
    n = size / sizeof(afw_size_t);
    for (i = 0; i < n; i++) {
        w[i] = poison;
    }
    b = (unsigned char *)user + n * sizeof(afw_size_t);
    end = (unsigned char *)user + size;
    i = 0;
    while (b < end) {
        *b++ = (unsigned char)(poison >>
            (8 * (i % sizeof(afw_size_t))));
        i++;
    }
}
#else
#define impl_debug_prefix_set(self, user, size) ((void)0)
#define impl_debug_check_prefix(self, address, size, xctx) ((void)0)
#define impl_debug_poison_user(user, size) ((void)0)
#endif


/* --------------------------- pool implementations ------------------------- */

/*
 * While error_processing_count > 0, last release/destroy is recorded
 * and skipped. Catching ENDTRY runs them when the count is 0 again.
 * Returns true if the caller should return without doing the work.
 */
static afw_boolean_t
impl_error_delaying_release(
    AFW_POOL_SELF_T *self,
    afw_boolean_t is_destroy,
    afw_xctx_t *xctx)
{
    if (!xctx || xctx->error_processing_count == 0) {
        return false;
    }
    /* Scope trackers (child of a heap). Not compile heaps, not
     * create() of a tracker. */
    if (!afw_pool_internal_is_tracker(&self->pub)) {
        return false;
    }
    if (self->parent &&
        afw_pool_internal_is_tracker(&self->parent->pub))
    {
        return false;
    }
    if (is_destroy) {
        self->error_processing_destroy = true;
    }
    else if (self->error_delaying_release) {
        return true;
    }
    else if (self->reference_count != 1) {
        return false;
    }
    if (!self->error_delaying_release) {
        self->error_delaying_release = true;
        self->error_delaying_release_next =
            (afw_pool_internal_self_t *)(void *)
                xctx->error_delaying_release_first;
        xctx->error_delaying_release_first = &self->pub;
    }
    return true;
}


static int
impl_pool_parent_depth(afw_pool_internal_self_t *p)
{
    int depth;

    for (depth = 0; p; p = p->parent) {
        depth++;
    }
    return depth;
}


AFW_DEFINE(void)
afw_pool_error_processing_finish(afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *head;
    afw_pool_internal_self_t *curr;
    afw_pool_internal_self_t *pick;
    afw_pool_internal_self_t **pos;
    afw_pool_internal_self_t **pick_pos;
    int pick_depth;
    int depth;
    afw_boolean_t do_destroy;

    head = (afw_pool_internal_self_t *)(void *)
        xctx->error_delaying_release_first;
    xctx->error_delaying_release_first = NULL;

    while (head) {
        pick = NULL;
        pick_pos = &head;
        pick_depth = -1;
        pos = &head;
        curr = head;
        while (curr) {
            if (curr->error_delaying_release) {
                depth = impl_pool_parent_depth(curr);
                if (depth >= pick_depth) {
                    pick = curr;
                    pick_pos = pos;
                    pick_depth = depth;
                }
            }
            pos = &curr->error_delaying_release_next;
            curr = curr->error_delaying_release_next;
        }
        if (!pick) {
            break;
        }
        *pick_pos = pick->error_delaying_release_next;
        pick->error_delaying_release_next = NULL;
        pick->error_delaying_release = false;
        do_destroy = pick->error_processing_destroy;
        pick->error_processing_destroy = false;
        if (do_destroy) {
            afw_pool_destroy(&pick->pub, xctx);
        }
        else {
            afw_pool_release(&pick->pub, xctx);
        }
    }
}


/*
 * Implementation of method release for interface afw_pool.
 *
 * Returns the pool if it still exists, or NULL if this call destroyed it.
 */
const afw_pool_t *
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");

    if (impl_error_delaying_release(self, false, xctx)) {
        return &self->pub;
    }

    /* Decrement reference count and release pools resources if zero. */
    if (--(self->reference_count) == 0) {
        afw_pool_destroy(&self->pub, xctx);
        return NULL;
    }
    return &self->pub;
}


/*
 * Implementation of method get_reference for interface afw_pool.
 */
void
impl_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "get_reference");

    /* Increment reference count. */
    self->reference_count++;
}

/*
 * Implementation of method destroy for interface afw_pool.
 */
void
impl_heap_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *child;
    afw_pool_internal_self_t *parent_self;
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");

    if (impl_error_delaying_release(self, true, xctx)) {
        return;
    }
    self->error_delaying_release = false;

    /*
     * Call all of the cleanup routines for this pool before releasing children.
     */
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }

    /*
     * Release children.
     *
     * Release of child sets self->first_child to its next sibling.
     */
    for (child = self->first_child;
        child;
        child = self->first_child)
    {
        afw_pool_release(&child->pub, xctx);
        if (self->first_child == child) {
            /* Delayed (throw path) or extra-held. Do not spin. */
            break;
        }
    }

    /* If tracker, parent is the heap. If heap, unlink from AFW parent. */
    if (self->parent) {
        impl_remove_as_child(self->parent, self, xctx);
    }
    else if (self->external_parent) {
        parent_self = (afw_pool_internal_self_t *)self->external_parent;
        if (parent_self->thread) {
            impl_remove_as_child(parent_self, self, xctx);
        }
        else {
            IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
                impl_remove_as_child(parent_self, self, xctx);
            }
            IMPL_MULTITHREADED_LOCK_END;
        }
    }

    impl_account_destroy(self, xctx);

    /* Walk chunks last: the heap self lives in the first chunk. */
    {
        afw_pool_chunk_t *chunk;
        afw_pool_chunk_t *next;

        chunk = self->first_chunk;
        self->first_chunk = NULL;
        self->current_chunk = NULL;
        self->bump = NULL;
        self->remaining = 0;
        while (chunk) {
            next = chunk->next;
            free(chunk);
            chunk = next;
        }
    }
}

static void *
impl_heap_malloc_internal(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    void *start;
    void *user;
    afw_size_t total;
    afw_boolean_t reused;

    if (size == 0) {
        if (unhandled) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    total = impl_block_bytes(AFW_POOL_HEAP_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = impl_heap_take_from_free_list_or_chunk(self, total, &reused,
        xctx, unhandled);
    if (!start) {
        return NULL;
    }
    IMPL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    if (xctx) {
        impl_account_alloc(self, total, xctx);
    }
    user = AFW_POOL_HEAP_USER_FROM_START(start);
    impl_debug_prefix_set(self, user, size);
    return user;
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
impl_heap_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_heap_malloc_internal(self, size, xctx, false);
    memset(result, 0, size);
    return result;
}

/*
 * Implementation of method malloc for interface afw_pool.
 */
void *
impl_heap_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, false);
}

/*
 * Implementation of method free_memory for interface afw_pool.
 */
void
impl_heap_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *start;
    afw_size_t total;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
    }
    impl_debug_check_prefix(self, address, size, xctx);
    impl_debug_poison_user(address, size);
    total = impl_block_bytes(AFW_POOL_HEAP_PREFIX_BYTES, size,
        xctx, false);
    start = AFW_POOL_HEAP_ALLOC_START(address);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    impl_account_free(self, total, xctx);
    impl_heap_add_to_free_list(self, start, total, xctx);
}

/*
 * Implementation of method register_cleanup_before for interface afw_pool.
 */
void
impl_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "register_cleanup_before %p %p",
        data, cleanup);

    /* Allocate entry which will also make sure its ok to use pool. */
    e = afw_pool_calloc_type(&self->pub, afw_pool_cleanup_t, xctx);

    /* Add entry to front of list of cleanup functions. */
    e->data = data;
    e->data2 = data2;
    e->cleanup = cleanup;
    e->next_cleanup = self->first_cleanup;
    self->first_cleanup = e;
}

/*
 * Implementation of method deregister_cleanup for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e, *prev;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "deregister_cleanup %p %p",
        data, cleanup);

    /* Search for entry and remove. */
    for (prev = (afw_pool_cleanup_t *)& self->first_cleanup,
        e = self->first_cleanup;
        e; prev = e, e = e->next_cleanup)
    {
        if (e->data == data && e->data2 == data2 && e->cleanup == cleanup) {
            prev->next_cleanup = e->next_cleanup;
            afw_pool_free_memory(&self->pub, e,
                sizeof(afw_pool_cleanup_t), xctx);
            break;
        }
    }
}


/* --- heap multithreaded wrappers (lock, then ST heap methods) --------- */

static const afw_pool_t *
impl_mt_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_heap_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_heap_afw_pool_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        result = impl_heap_afw_pool_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_heap_afw_pool_free_memory(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_register_cleanup_before(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(xctx) {
        impl_afw_pool_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

#define impl_afw_pool_release impl_mt_afw_pool_release
#define impl_afw_pool_get_reference impl_mt_afw_pool_get_reference
#define impl_afw_pool_destroy impl_mt_afw_pool_destroy
#define impl_afw_pool_calloc impl_mt_afw_pool_calloc
#define impl_afw_pool_malloc impl_mt_afw_pool_malloc
#define impl_afw_pool_free_memory impl_mt_afw_pool_free_memory
#define impl_afw_pool_register_cleanup_before \
    impl_mt_afw_pool_register_cleanup_before
#define impl_afw_pool_deregister_cleanup impl_mt_afw_pool_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "heap_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_heap_multithreaded_inf
#define AFW_POOL_INF_ONLY 1

static const afw_pool_internal_inf_implementation_specific_t
impl_pool_mt_implementation_specific =
    {
        /* multithreaded */ true,
        /* tracker */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_mt_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef AFW_POOL_INF_ONLY
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_register_cleanup_before
#undef impl_afw_pool_deregister_cleanup


/* --------------------------- tracker implementations ---------------------- */

void
impl_tracker_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_tracker_node_t *memory;
    afw_pool_internal_self_t *child;
    afw_pool_internal_self_t *parent;
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");

    if (impl_error_delaying_release(self, true, xctx)) {
        return;
    }
    self->error_delaying_release = false;

    /* Tracker always has a parent. (needed to suppress valgrind error) */
    if (!self->parent) {
        AFW_THROW_ERROR_Z(general, "Tracker has no parent", xctx);
    }
    parent = self->parent;

    /*
     * Call all of the cleanup routines for this pool before releasing children.
     */
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }

    /* Release all of the children of this tracker. */
    for (child = self->first_child;
        child;
        child = self->first_child)
    {
        afw_pool_release(&child->pub, xctx);
    }

    /* Leftover children honor destroy; do not reparent. */
    while (self->first_child) {
        afw_pool_destroy(&self->first_child->pub, xctx);
    }

    /* Return leftovers. Unlink first so next is still the allocated
     * list, not a free-list overlay. */
    while (self->first_allocated_memory) {
        memory = self->first_allocated_memory;
        impl_tracker_unlink(&self->first_allocated_memory, memory);
        impl_debug_poison_user(AFW_POOL_TRACKER_TO_USER(memory),
            AFW_POOL_TRACKER_USER_SIZE(memory));
        impl_heap_add_to_free_list(self, memory,
            impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES,
                AFW_POOL_TRACKER_USER_SIZE(memory), xctx, false),
            xctx);
    }

    impl_account_destroy(self, xctx);

    /* Removed self as child of parent. Header was calloc’d from the heap. */
    impl_remove_as_child(parent, self, xctx);
    afw_pool_free_memory(&parent->pub, self,
        sizeof(afw_pool_internal_self_t), xctx);
}


static void *
impl_tracker_malloc_internal(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    void *start;
    void *user;
    afw_pool_tracker_node_t *node;
    afw_size_t total;
    afw_boolean_t reused;

    if (size == 0) {
        if (unhandled) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    total = impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = impl_heap_take_from_free_list_or_chunk(self, total, &reused,
        xctx, unhandled);
    if (!start) {
        return NULL;
    }
    IMPL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    node = (afw_pool_tracker_node_t *)start;
    node->prev = NULL;
    node->next = self->first_allocated_memory;
    if (self->first_allocated_memory) {
        self->first_allocated_memory->prev = node;
    }
    self->first_allocated_memory = node;
    user = AFW_POOL_TRACKER_TO_USER(node);
#ifdef AFW_DEBUG_POOL
    node->debug.size = size;
    node->debug.pool = &self->pub;
#else
    node->size = size;
#endif
    if (xctx) {
        impl_account_alloc(self, total, xctx);
    }
    return user;
}


void *
impl_tracker_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_tracker_malloc_internal(self, size, xctx, false);
    memset(result, 0, size);
    return result;
}


static void *
impl_tracker_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_tracker_malloc_internal(self, size, xctx, false);
}


static void
impl_tracker_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_tracker_node_t *node;
    afw_size_t total;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
    }
    impl_debug_check_prefix(self, address, size, xctx);
    impl_debug_poison_user(address, size);
    node = AFW_POOL_TRACKER_NODE(address);
    total = impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES, size,
        xctx, false);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    impl_account_free(self, total, xctx);
    impl_tracker_unlink(&self->first_allocated_memory, node);
    impl_heap_add_to_free_list(self, node, total, xctx);
}


/* ---------------------------- extern functions ---------------------------- */


AFW_DEFINE(afw_boolean_t)
afw_pool_internal_is_heap(const afw_pool_t *p)
{
    return p && (p->inf == &impl_afw_pool_inf ||
        p->inf == &impl_afw_pool_heap_multithreaded_inf);
}


AFW_DEFINE(afw_boolean_t)
afw_pool_internal_is_heap_multithreaded(const afw_pool_t *p)
{
    return p && p->inf == &impl_afw_pool_heap_multithreaded_inf;
}


AFW_DEFINE(afw_boolean_t)
afw_pool_internal_is_tracker(const afw_pool_t *p)
{
    return p && p->inf == &impl_afw_pool_tracker_inf;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_internal_heap_create(
    const afw_pool_t *parent,
    afw_boolean_t multithreaded,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    const afw_pool_inf_t *inf;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    inf = multithreaded
        ? &impl_afw_pool_heap_multithreaded_inf
        : &impl_afw_pool_inf;
    self = impl_heap_create(parent, inf, xctx);
    if (multithreaded) {
        self->thread = NULL;
    }
    return &self->pub;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create_xctx_p(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    return afw_pool_internal_heap_create(parent, false, xctx);
}


AFW_DEFINE(void *)
afw_pool_malloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    if (!instance) {
        return NULL;
    }
    self = (AFW_POOL_SELF_T *)instance;
    if (afw_pool_internal_is_tracker(instance)) {
        return impl_tracker_malloc_internal(self, size, xctx, true);
    }
    return impl_heap_malloc_internal(self, size, xctx, true);
}


AFW_DEFINE(void *)
afw_pool_calloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = afw_pool_malloc_unhandled(instance, size, xctx);
    if (result) {
        memset(result, 0, size);
    }
    return result;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_internal_create_base_pool()
{
    afw_pool_internal_self_t *self;

    self = impl_heap_allocate_self(&impl_afw_pool_heap_multithreaded_inf);
    if (!self) {
        return NULL;
    }
    self->name = afw_s_base;
    self->pool_number = 1;
    self->thread = NULL;
    impl_base_pool_self = self;
    return &self->pub;
}


AFW_DEFINE(void)
afw_pool_internal_destroy_base_pool(afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;

    if (!xctx || !xctx->env || !xctx->env->p) {
        return;
    }
    self = (afw_pool_internal_self_t *)xctx->env->p;
    /* Skip the MT wrapper: that lock is allocated from this pool. */
    impl_heap_afw_pool_destroy(self, xctx);
}


AFW_DEFINE(afw_thread_t *)
afw_pool_thread_create(
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    AFW_POOL_SELF_T *self;
    afw_thread_t *thread;

    if (size == (afw_size_t)-1 || size < sizeof(afw_thread_t)) {
        size = sizeof(afw_thread_t);
    }

    p = afw_pool_create_xctx_p(xctx->p, xctx);
    self = (AFW_POOL_SELF_T *)p;
    thread = afw_pool_calloc(p, size, xctx);
    self->thread = thread;
    thread->p = p;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "thread_create " AFW_SIZE_T_FMT,
        size);

    return thread;
}


void
afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const AFW_POOL_SELF_T *self = (const AFW_POOL_SELF_T *)pool;
    const afw_pool_internal_self_t *child;
    int i;

    (void)xctx;
    for (i = 0; i < indent; i++) {
        printf("  ");
    }
    printf(
        "pool " AFW_INTEGER_FMT " " AFW_SIZE_T_FMT " refs " AFW_INTEGER_FMT
        " parent " AFW_INTEGER_FMT "\n",
        self->pool_number,
        self->bytes_allocated,
        self->reference_count,
        self->parent ? self->parent->pool_number : (afw_integer_t)0);

    for (child = self->first_child; child; child = child->next_sibling) {
        afw_pool_print_debug_info(indent + 2, &child->pub, xctx);
    }
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_tracker_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    AFW_POOL_SELF_T *heap;
    const afw_pool_inf_t *inf;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_internal_is_heap(parent) &&
        !afw_pool_internal_is_tracker(parent))
    {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_tracker_create() parent must be a heap or tracker",
            xctx);
    }

    heap = (AFW_POOL_SELF_T *)parent;
    /* Same thread as the heap. */
    inf = &impl_afw_pool_tracker_inf;

    self = impl_create_for_tracker(heap, inf, xctx);
    return &self->pub;
}

/* ---------------------------- create() -------------------------------- */

AFW_DEFINE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    /*
     * Extra rule (ok for now): create() of a tracker is a tracker, so
     * we do not make a heap under a tracker. May revisit.
     */
    if (afw_pool_internal_is_tracker(parent)) {
        return afw_pool_tracker_create(parent, xctx);
    }
    return afw_pool_internal_heap_create(parent,
        afw_pool_internal_is_heap_multithreaded(parent), xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create_as_managed_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;

    p = afw_pool_create(parent, xctx);
    ((afw_pool_t *)p)->managed_p = p;
    return p;
}


/* Release the value registered with afw_pool_release_value_at_cleanup(). */
static void
impl_release_value_at_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    afw_value_release((const afw_value_t *)data, xctx);
}


/* Release a value when a pool is destroyed. */
AFW_DEFINE(void)
afw_pool_release_value_at_cleanup(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value) {
        return;
    }
    afw_pool_register_cleanup_before(p, (void *)value, NULL,
        impl_release_value_at_cleanup, xctx);
}

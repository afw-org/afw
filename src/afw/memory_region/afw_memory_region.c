// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory_region implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_memory_region.c
 * @brief Default afw_memory_region: capped free lists of mapped
 *    pages for heaps.
 *
 * The instance is C calloc; release() frees it. Chunks come from
 * afw_os_map_pages() / afw_os_unmap_pages() so RSS drops when a
 * chunk does not stay on a free list. get and free do not lock.
 * The caller holds the right lock: a multithreaded pool holds the
 * multithreaded region's mutex, and a single-threaded pool is the
 * only caller of its thread's region. Cap 0 is map/unmap on every
 * get/free (metrics still update).
 *
 * Three lists: small (small_size), large (large_size), and other.
 * Small and large get are a pop from the head; other is an
 * exact-size scan. Each list is newest at the head, oldest at the
 * tail. A free over the cap unmaps from the tails: other, then
 * small, then large.
 *
 * trim() keeps the newest keep_small_count / keep_large_count
 * resident. The rest keep their first page (it holds the list node)
 * and afw_os_discard_pages() gives back the others; a one-page
 * region is unmapped instead. Discarded regions sit behind the
 * resident ones, so a get takes a resident one first.
 */

#include "afw_internal.h"
#include <stdlib.h>


typedef struct impl_free_node_s impl_free_node_t;
struct impl_free_node_s {
    impl_free_node_t *next;
    impl_free_node_t *prev;
    afw_size_t size;
    /* Pages after the first were given back by trim(). */
    afw_boolean_t discarded;
};


typedef struct impl_free_list_s {
    impl_free_node_t *head;
    impl_free_node_t *tail;
} impl_free_list_t;


#define IMPL_LIST_SMALL 0
#define IMPL_LIST_LARGE 1
#define IMPL_LIST_OTHER 2
#define IMPL_LIST_COUNT 3


typedef struct impl_afw_memory_region_self_s
    impl_afw_memory_region_self_t;
struct impl_afw_memory_region_self_s {
    afw_memory_region_t pub;
    impl_free_list_t lists[IMPL_LIST_COUNT];
    afw_os_mutex_t *mutex;
};


#define AFW_IMPLEMENTATION_ID "default"
#define AFW_MEMORY_REGION_SELF_T impl_afw_memory_region_self_t
#include "afw_memory_region_impl_declares.h"


static afw_size_t
impl_round_up(afw_size_t size)
{
    return afw_pool_round_up_chunk_size(size);
}


/* Bytes trim() gives back from a region of this size. */
static afw_size_t
impl_discard_bytes(afw_size_t size)
{
    return size - AFW_MEMORY_REGION_ALIGN;
}


/* Clear checker marks first so a later map of these pages is clean. */
static void
impl_unmap(void *mem, afw_size_t size)
{
    AFW_MEMORY_ANNOTATE_ACCESS(mem, size);
    afw_os_unmap_pages(mem, size);
}


/* Large first: if the sizes are the same, keep_large_count applies. */
static impl_free_list_t *
impl_list_for(impl_afw_memory_region_self_t *self, afw_size_t size)
{
    if (self->pub.large_size != 0 && size == self->pub.large_size) {
        return &self->lists[IMPL_LIST_LARGE];
    }
    if (self->pub.small_size != 0 && size == self->pub.small_size) {
        return &self->lists[IMPL_LIST_SMALL];
    }
    return &self->lists[IMPL_LIST_OTHER];
}


static void
impl_push_head(impl_free_list_t *list, impl_free_node_t *node)
{
    node->prev = NULL;
    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    }
    else {
        list->tail = node;
    }
    list->head = node;
}


static void
impl_unlink(impl_free_list_t *list, impl_free_node_t *node)
{
    if (node->prev) {
        node->prev->next = node->next;
    }
    else {
        list->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    else {
        list->tail = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
}


static void
impl_account_in_use(afw_memory_region_t *pub, afw_size_t size)
{
    pub->bytes_in_use += size;
    pub->regions_in_use += 1;
    if (pub->bytes_in_use > pub->peak_bytes_in_use) {
        pub->peak_bytes_in_use = pub->bytes_in_use;
    }
}


static void
impl_account_returned(afw_memory_region_t *pub, afw_size_t size)
{
    if (pub->bytes_in_use >= size) {
        pub->bytes_in_use -= size;
    }
    else {
        pub->bytes_in_use = 0;
    }
    if (pub->regions_in_use > 0) {
        pub->regions_in_use -= 1;
    }
}


static afw_environment_t *
impl_env(afw_xctx_t *xctx)
{
    if (!xctx || !xctx->env) {
        return NULL;
    }
    return (afw_environment_t *)xctx->env;
}


static void
impl_env_peak(AFW_ATOMIC afw_size_t *peak, afw_size_t n)
{
    if (n > *peak) {
        *peak = n;
    }
}


static void
impl_env_sub(AFW_ATOMIC afw_size_t *slot, afw_size_t n)
{
    if (*slot >= n) {
        *slot -= n;
    }
    else {
        *slot = 0;
    }
}


static void
impl_env_hit(afw_xctx_t *xctx, afw_size_t size, afw_size_t discarded)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    env->memory_region_get_hits += 1;
    impl_env_sub(&env->memory_region_free_list_count, 1);
    impl_env_sub(&env->memory_region_free_list_bytes, size);
    impl_env_sub(&env->memory_region_free_list_discarded_bytes, discarded);
    env->memory_region_bytes_in_use += size;
    env->memory_region_regions_in_use += 1;
    impl_env_peak(&env->memory_region_peak_bytes_in_use,
        env->memory_region_bytes_in_use);
}


static void
impl_env_miss(afw_xctx_t *xctx, afw_size_t size)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    env->memory_region_get_misses += 1;
    env->memory_region_bytes_in_use += size;
    env->memory_region_regions_in_use += 1;
    impl_env_peak(&env->memory_region_peak_bytes_in_use,
        env->memory_region_bytes_in_use);
}


static void
impl_env_to_list(afw_xctx_t *xctx, afw_size_t size)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    impl_env_sub(&env->memory_region_bytes_in_use, size);
    impl_env_sub(&env->memory_region_regions_in_use, 1);
    env->memory_region_free_list_count += 1;
    env->memory_region_free_list_bytes += size;
    impl_env_peak(&env->memory_region_peak_free_list_bytes,
        env->memory_region_free_list_bytes);
}


static void
impl_env_over_cap(afw_xctx_t *xctx, afw_size_t size)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    impl_env_sub(&env->memory_region_bytes_in_use, size);
    impl_env_sub(&env->memory_region_regions_in_use, 1);
    env->memory_region_free_over_cap += 1;
}


static void
impl_env_drain_list(afw_xctx_t *xctx, afw_size_t bytes, afw_size_t count,
    afw_size_t discarded)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    impl_env_sub(&env->memory_region_free_list_bytes, bytes);
    impl_env_sub(&env->memory_region_free_list_count, count);
    impl_env_sub(&env->memory_region_free_list_discarded_bytes, discarded);
}


static void
impl_lock(impl_afw_memory_region_self_t *self, afw_xctx_t *xctx)
{
    if (self->mutex) {
        afw_os_mutex_lock(self->mutex, xctx);
    }
}


static void
impl_unlock(impl_afw_memory_region_self_t *self, afw_xctx_t *xctx)
{
    if (self->mutex) {
        afw_os_mutex_unlock(self->mutex, xctx);
    }
}


/* Take node off its list and out of the free-list counts. */
static void
impl_remove(impl_afw_memory_region_self_t *self, impl_free_list_t *list,
    impl_free_node_t *node, afw_xctx_t *xctx)
{
    afw_memory_region_t *pub;
    afw_size_t discarded;

    pub = &self->pub;
    impl_unlink(list, node);
    discarded = node->discarded ? impl_discard_bytes(node->size) : 0;
    pub->free_list_count -= 1;
    pub->free_list_bytes -= node->size;
    pub->free_list_discarded_bytes -= discarded;
    impl_env_drain_list(xctx, node->size, 1, discarded);
}


/* Unmap the oldest region: other, then small, then large. */
static afw_boolean_t
impl_evict_oldest(impl_afw_memory_region_self_t *self, afw_xctx_t *xctx)
{
    static const int order[IMPL_LIST_COUNT] = {
        IMPL_LIST_OTHER, IMPL_LIST_SMALL, IMPL_LIST_LARGE
    };
    impl_free_list_t *list;
    impl_free_node_t *node;
    afw_environment_t *env;
    int i;

    for (i = 0; i < IMPL_LIST_COUNT; i++) {
        list = &self->lists[order[i]];
        node = list->tail;
        if (node) {
            impl_remove(self, list, node, xctx);
            self->pub.free_over_cap += 1;
            env = impl_env(xctx);
            if (env) {
                env->memory_region_free_over_cap += 1;
            }
            impl_unmap(node, node->size);
            return true;
        }
    }
    return false;
}


/*
 * Implementation of method get for interface afw_memory_region.
 */
void
impl_afw_memory_region_get(
    AFW_MEMORY_REGION_SELF_T *self,
    void **region,
    afw_size_t *size,
    afw_xctx_t *xctx)
{
    afw_memory_region_t *pub;
    impl_free_list_t *list;
    impl_free_node_t *node;
    void *mem;
    afw_size_t need;
    afw_size_t discarded;

    if (region) {
        *region = NULL;
    }
    if (!region || !size) {
        if (size) {
            *size = 0;
        }
        return;
    }
    pub = &self->pub;
    need = impl_round_up(*size);
    if (need == 0) {
        *size = 0;
        return;
    }

    if (pub->free_list_max_bytes != 0) {
        list = impl_list_for(self, need);
        for (node = list->head; node; node = node->next) {
            if (node->size == need) {
                break;
            }
        }
        if (node) {
            impl_unlink(list, node);
            discarded = node->discarded ? impl_discard_bytes(need) : 0;
            pub->free_list_count -= 1;
            pub->free_list_bytes -= need;
            pub->free_list_discarded_bytes -= discarded;
            pub->get_hits += 1;
            impl_account_in_use(pub, need);
            impl_env_hit(xctx, need, discarded);
            AFW_MEMORY_ANNOTATE_ACCESS(node, need);
            *region = node;
            *size = need;
            return;
        }
    }

    mem = afw_os_map_pages(need);
    if (!mem) {
        *size = 0;
        return;
    }
    pub->get_misses += 1;
    impl_account_in_use(pub, need);
    impl_env_miss(xctx, need);
    *region = mem;
    *size = need;
}


/*
 * Implementation of method free for interface afw_memory_region.
 */
void
impl_afw_memory_region_free(
    AFW_MEMORY_REGION_SELF_T *self,
    void *region,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_memory_region_t *pub;
    impl_free_node_t *node;

    if (!region) {
        return;
    }
    pub = &self->pub;
    size = impl_round_up(size);
    impl_account_returned(pub, size);
    /* The heap left its own marks. Start this chunk over. */
    AFW_MEMORY_ANNOTATE_ACCESS(region, size);

    /*
     * Keep this chunk when it fits the cap. If the lists are full,
     * drop the oldest until it fits. Otherwise a later get of this
     * size misses and a new map grows RSS for one swap.
     */
    if (pub->free_list_max_bytes != 0 &&
        size <= pub->free_list_max_bytes)
    {
        while (pub->free_list_bytes + size > pub->free_list_max_bytes) {
            if (!impl_evict_oldest(self, xctx)) {
                break;
            }
        }
        if (pub->free_list_bytes + size <= pub->free_list_max_bytes)
        {
            node = (impl_free_node_t *)region;
            node->size = size;
            node->discarded = false;
            impl_push_head(impl_list_for(self, size), node);
            pub->free_list_count += 1;
            pub->free_list_bytes += size;
            if (pub->free_list_bytes > pub->peak_free_list_bytes) {
                pub->peak_free_list_bytes = pub->free_list_bytes;
            }
            impl_env_to_list(xctx, size);
            /* Cached pages are no-access past the node until get. */
            AFW_MEMORY_ANNOTATE_NOACCESS((char *)node + sizeof(*node),
                size - sizeof(*node));
            return;
        }
    }

    pub->free_over_cap += 1;
    impl_env_over_cap(xctx, size);
    impl_unmap(region, size);
}


/*
 * Keep the first keep resident regions of list. Discard the pages of
 * the rest past the first, or unmap a one-page region.
 */
static void
impl_trim_list(
    impl_afw_memory_region_self_t *self,
    impl_free_list_t *list,
    afw_size_t keep,
    afw_xctx_t *xctx)
{
    afw_memory_region_t *pub;
    impl_free_node_t *node;
    impl_free_node_t *next;
    afw_environment_t *env;
    afw_size_t kept;
    afw_size_t bytes;

    pub = &self->pub;
    env = impl_env(xctx);
    kept = 0;
    for (node = list->head; node; node = next) {
        next = node->next;
        if (node->discarded) {
            continue;
        }
        if (kept < keep) {
            kept++;
            continue;
        }
        if (node->size > AFW_MEMORY_REGION_ALIGN) {
            bytes = impl_discard_bytes(node->size);
            afw_os_discard_pages((char *)node + AFW_MEMORY_REGION_ALIGN,
                bytes);
            node->discarded = true;
            pub->free_list_discarded_bytes += bytes;
            pub->discards += 1;
            if (env) {
                env->memory_region_free_list_discarded_bytes += bytes;
                env->memory_region_discards += 1;
            }
        }
        else {
            impl_remove(self, list, node, xctx);
            pub->trim_unmaps += 1;
            if (env) {
                env->memory_region_trim_unmaps += 1;
            }
            impl_unmap(node, node->size);
        }
    }
}


/*
 * Implementation of method trim for interface afw_memory_region.
 */
void
impl_afw_memory_region_trim(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_trim_list(self, &self->lists[IMPL_LIST_SMALL],
        self->pub.keep_small_count, xctx);
    impl_trim_list(self, &self->lists[IMPL_LIST_LARGE],
        self->pub.keep_large_count, xctx);
    impl_trim_list(self, &self->lists[IMPL_LIST_OTHER], 0, xctx);
}


/*
 * Implementation of method cleanup for interface afw_memory_region.
 */
void
impl_afw_memory_region_cleanup(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_free_node_t *node;
    impl_free_node_t *next;
    afw_size_t bytes;
    afw_size_t count;
    afw_size_t discarded;
    int i;

    bytes = self->pub.free_list_bytes;
    count = self->pub.free_list_count;
    discarded = self->pub.free_list_discarded_bytes;
    self->pub.free_list_count = 0;
    self->pub.free_list_bytes = 0;
    self->pub.free_list_discarded_bytes = 0;
    impl_env_drain_list(xctx, bytes, count, discarded);
    for (i = 0; i < IMPL_LIST_COUNT; i++) {
        node = self->lists[i].head;
        self->lists[i].head = NULL;
        self->lists[i].tail = NULL;
        while (node) {
            next = node->next;
            impl_unmap(node, node->size);
            node = next;
        }
    }
}


void
impl_afw_memory_region_lock(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_lock(self, xctx);
}


void
impl_afw_memory_region_unlock(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_unlock(self, xctx);
}


/*
 * Implementation of method release for interface afw_memory_region.
 */
void
impl_afw_memory_region_release(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_afw_memory_region_cleanup(self, xctx);
    afw_os_mutex_free_unhandled(self->mutex);
    self->mutex = NULL;
    free(self);
}


AFW_DEFINE(const afw_memory_region_t *)
afw_memory_region_create(
    afw_size_t free_list_max_bytes,
    afw_xctx_t *xctx)
{
    impl_afw_memory_region_self_t *self;

    (void)xctx;
    self = (impl_afw_memory_region_self_t *)
        calloc(1, sizeof(impl_afw_memory_region_self_t));
    if (!self) {
        return NULL;
    }
    self->mutex = afw_os_mutex_create_unhandled(
        AFW_OS_MUTEX_NESTED);
    if (!self->mutex) {
        free(self);
        return NULL;
    }
    self->pub.inf = &impl_afw_memory_region_inf;
    self->pub.free_list_max_bytes = free_list_max_bytes;
    self->pub.keep_small_count = AFW_MEMORY_REGION_KEEP_SMALL_COUNT;
    self->pub.keep_large_count = AFW_MEMORY_REGION_KEEP_LARGE_COUNT;
    return &self->pub;
}


AFW_DEFINE(void)
afw_memory_region_configure(
    const afw_memory_region_t *instance,
    const afw_environment_t *env,
    afw_xctx_t *xctx)
{
    impl_afw_memory_region_self_t *self;

    if (!instance || !env) {
        return;
    }
    self = (impl_afw_memory_region_self_t *)instance;
    if (self->pub.free_list_count != 0 &&
        (self->pub.small_size != env->small_chunk_min ||
            self->pub.large_size != env->xctx_chunk_min))
    {
        impl_afw_memory_region_cleanup(self, xctx);
    }
    self->pub.small_size = env->small_chunk_min;
    self->pub.large_size = env->xctx_chunk_min;
    self->pub.free_list_max_bytes = env->memory_region_free_list_max_bytes;
    self->pub.keep_small_count = env->memory_region_keep_small_count;
    self->pub.keep_large_count = env->memory_region_keep_large_count;
}

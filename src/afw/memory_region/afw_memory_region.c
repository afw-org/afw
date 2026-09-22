// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory_region implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_memory_region.c
 * @brief Default afw_memory_region: capped free list of mmap pages
 *    for heaps.
 *
 * The instance is C calloc; release() frees it. Chunks are mmap'd
 * and munmap'd so RSS drops when a chunk does not stay on the free
 * list. get/free/cleanup do not lock. MT heap/tracker wrappers call
 * lock/unlock. Cap 0 is mmap/munmap on every get/free (metrics
 * still update).
 */

#include "afw_internal.h"
#include <stdlib.h>
#include <sys/mman.h>


typedef struct impl_free_node_s impl_free_node_t;
struct impl_free_node_s {
    impl_free_node_t *next;
    afw_size_t size;
};


typedef struct impl_afw_memory_region_self_s
    impl_afw_memory_region_self_t;
struct impl_afw_memory_region_self_s {
    afw_memory_region_t pub;
    impl_free_node_t *free_list;
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
impl_env_hit(afw_xctx_t *xctx, afw_size_t size)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    env->memory_region_get_hits += 1;
    impl_env_sub(&env->memory_region_free_list_count, 1);
    impl_env_sub(&env->memory_region_free_list_bytes, size);
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
impl_env_drain_list(afw_xctx_t *xctx, afw_size_t bytes, afw_size_t count)
{
    afw_environment_t *env;

    env = impl_env(xctx);
    if (!env) {
        return;
    }
    impl_env_sub(&env->memory_region_free_list_bytes, bytes);
    impl_env_sub(&env->memory_region_free_list_count, count);
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


/*
 * mmap, not posix_memalign. free() of a 64 KiB aligned block stays
 * in the glibc heap, so RSS climbed on every miss/over-cap pair.
 * munmap gives the pages back. The pointer is still page aligned.
 */
static void *
impl_map_chunk(afw_size_t size)
{
    void *mem;

    mem = mmap(NULL, size, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        return NULL;
    }
    return mem;
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
    impl_free_node_t **prev;
    impl_free_node_t *node;
    void *mem;
    afw_size_t need;

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
        prev = &self->free_list;
        for (node = self->free_list; node; node = node->next) {
            if (node->size == need) {
                *prev = node->next;
                pub->free_list_count -= 1;
                pub->free_list_bytes -= need;
                pub->get_hits += 1;
                impl_account_in_use(pub, need);
                impl_env_hit(xctx, need);
                *region = node;
                *size = need;
                return;
            }
            prev = &node->next;
        }
    }

    mem = impl_map_chunk(need);
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

    /*
     * Keep this chunk when it fits the cap. If the list is full of
     * other sizes, drop the oldest until it fits. Otherwise a later
     * get of this size misses and mmap grows RSS for one swap.
     */
    if (pub->free_list_max_bytes != 0 &&
        size <= pub->free_list_max_bytes)
    {
        while (self->free_list &&
            pub->free_list_bytes + size > pub->free_list_max_bytes)
        {
            node = self->free_list;
            self->free_list = node->next;
            pub->free_list_count -= 1;
            pub->free_list_bytes -= node->size;
            impl_env_drain_list(xctx, node->size, 1);
            pub->free_over_cap += 1;
            {
                afw_environment_t *env = impl_env(xctx);
                if (env) {
                    env->memory_region_free_over_cap += 1;
                }
            }
            munmap(node, node->size);
        }
        if (pub->free_list_bytes + size <= pub->free_list_max_bytes)
        {
            node = (impl_free_node_t *)region;
            node->next = self->free_list;
            node->size = size;
            self->free_list = node;
            pub->free_list_count += 1;
            pub->free_list_bytes += size;
            if (pub->free_list_bytes > pub->peak_free_list_bytes) {
                pub->peak_free_list_bytes = pub->free_list_bytes;
            }
            impl_env_to_list(xctx, size);
            return;
        }
    }

    pub->free_over_cap += 1;
    impl_env_over_cap(xctx, size);
    munmap(region, size);
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

    bytes = self->pub.free_list_bytes;
    count = self->pub.free_list_count;
    node = self->free_list;
    self->free_list = NULL;
    self->pub.free_list_count = 0;
    self->pub.free_list_bytes = 0;
    impl_env_drain_list(xctx, bytes, count);
    while (node) {
        next = node->next;
        munmap(node, node->size);
        node = next;
    }
}


/*
 * Implementation of method release for interface afw_memory_region.
 */
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
    return &self->pub;
}


AFW_DEFINE(void)
afw_memory_region_set_free_list_max_bytes(
    const afw_memory_region_t *instance,
    afw_size_t free_list_max_bytes,
    afw_xctx_t *xctx)
{
    impl_afw_memory_region_self_t *self;

    if (!instance) {
        return;
    }
    self = (impl_afw_memory_region_self_t *)instance;
    impl_lock(self, xctx);
    self->pub.free_list_max_bytes = free_list_max_bytes;
    impl_unlock(self, xctx);
}

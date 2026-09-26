// See the 'COPYING' file in the project root for licensing information.
/*
 * Scope pool: one evaluation frame.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_scope.c
 * @brief Scope pool and xctx frame. The scope object is the pool.
 *
 * Old link rule: create holds the parent until teardown. The scope
 * reference count is separate and stays at 1 for the creator, so
 * throw-path delay still sees a pool count of 1.
 */

#include "afw_internal.h"

#define AFW_POOL_SELF_T afw_pool_internal_self_t

static const afw_pool_internal_inf_implementation_specific_t
impl_scope_specific =
    {
        /* multithreaded */ false,
        /* tracker */ false
    };

static const afw_pool_internal_inf_implementation_specific_t
impl_scope_mt_specific =
    {
        /* multithreaded */ true,
        /* tracker */ false
    };


static void
impl_clear_delay(
    afw_pool_internal_scope_self_t *me, afw_xctx_t *xctx)
{
    const afw_pool_t **pos;
    afw_pool_internal_scope_self_t *curr;

    if (!me->error_delaying_release) {
        return;
    }
    me->error_delaying_release = false;
    if (!xctx) {
        me->error_delaying_release_next = NULL;
        return;
    }
    pos = &xctx->error_delaying_release_first;
    while (*pos) {
        curr = afw_pool_heap_internal_as_scope(
            (afw_pool_internal_self_t *)(void *)*pos);
        if (curr == me) {
            *pos = curr->error_delaying_release_next;
            curr->error_delaying_release_next = NULL;
            return;
        }
        pos = &curr->error_delaying_release_next;
    }
    me->error_delaying_release_next = NULL;
}


/*
 * While error_processing_count > 0, last release of a scope pool
 * is recorded and skipped. Catching ENDTRY runs
 * afw_pool_heap_internal_release_delayed() when the count is 0 again.
 */
static afw_boolean_t
impl_error_delaying_release(
    afw_pool_internal_scope_self_t *me,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;

    self = &me->heap.common;
    if (!xctx || xctx->error_processing_count == 0) {
        return false;
    }
    if (me->error_delaying_release) {
        return true;
    }
    if (self->reference_count != 1) {
        return false;
    }
    me->error_delaying_release = true;
    me->error_delaying_release_next =
        xctx->error_delaying_release_first;
    xctx->error_delaying_release_first = &self->pub;
    return true;
}


static void
impl_scope_teardown(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    /*
     * One parent hold from create. It is not kept in parent_pins, so
     * extra releases leave it. Teardown releases that one hold.
     */
    self->parent_pins = 1;
    afw_pool_heap_teardown_store(self, xctx);
}


static void
impl_scope_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "get_reference");
    self->reference_count++;
}


static const afw_pool_t *
impl_scope_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");
    if (impl_error_delaying_release(
            afw_pool_heap_internal_as_scope(self), xctx))
    {
        return &self->pub;
    }
    return afw_pool_internal_release_common(
        self, xctx, impl_scope_teardown);
}


static void
impl_scope_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
    if (!self->destroying) {
        impl_clear_delay(afw_pool_heap_internal_as_scope(self), xctx);
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_run_child_cleanups(self, xctx);
    afw_pool_internal_run_cleanups(self, xctx);
}


static void
impl_scope_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        impl_clear_delay(afw_pool_heap_internal_as_scope(self), xctx);
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_destroy_children(self, xctx);
    impl_scope_teardown(self, xctx);
}


static void
impl_scope_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    (void)self;
    (void)xctx;
}


#define AFW_IMPLEMENTATION_ID "scope"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_scope_inf
#define AFW_IMPLEMENTATION_SPECIFIC &impl_scope_specific
#define impl_afw_pool_release impl_scope_afw_pool_release
#define impl_afw_pool_get_reference impl_scope_afw_pool_get_reference
#define impl_afw_pool_run_cleanups impl_scope_afw_pool_run_cleanups
#define impl_afw_pool_destroy impl_scope_afw_pool_destroy
#define impl_afw_pool_calloc afw_pool_heap_calloc
#define impl_afw_pool_malloc afw_pool_heap_malloc
#define impl_afw_pool_free_memory afw_pool_heap_free_memory
#define impl_afw_pool_free_memory_no_throw \
    afw_pool_heap_free_memory_no_throw
#define impl_afw_pool_calloc_no_throw afw_pool_heap_calloc_no_throw
#define impl_afw_pool_malloc_no_throw afw_pool_heap_malloc_no_throw
#define impl_afw_pool_garbage_collect impl_scope_garbage_collect
#define impl_afw_pool_register_cleanup afw_pool_internal_register_cleanup
#define impl_afw_pool_deregister_cleanup afw_pool_internal_deregister_cleanup

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup


static const afw_pool_t *
impl_mt_scope_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_scope_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_scope_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_scope_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_scope_afw_pool_run_cleanups(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_scope_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_scope_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_scope_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_scope_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_calloc_no_throw(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_scope_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_malloc_no_throw(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_scope_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_heap_free_memory(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_heap_free_memory_no_throw(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_scope_garbage_collect(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_register_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_register_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}


#define AFW_POOL_INF_ONLY 1
#define AFW_IMPLEMENTATION_ID "scope_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_scope_multithreaded_inf
#define AFW_IMPLEMENTATION_SPECIFIC &impl_scope_mt_specific
#define impl_afw_pool_release impl_mt_scope_release
#define impl_afw_pool_get_reference impl_mt_scope_get_reference
#define impl_afw_pool_run_cleanups impl_mt_scope_run_cleanups
#define impl_afw_pool_destroy impl_mt_scope_destroy
#define impl_afw_pool_calloc impl_mt_scope_calloc
#define impl_afw_pool_malloc impl_mt_scope_malloc
#define impl_afw_pool_calloc_no_throw impl_mt_scope_calloc_no_throw
#define impl_afw_pool_malloc_no_throw impl_mt_scope_malloc_no_throw
#define impl_afw_pool_free_memory impl_mt_scope_free_memory
#define impl_afw_pool_free_memory_no_throw impl_mt_scope_free_memory_no_throw
#define impl_afw_pool_garbage_collect impl_mt_scope_garbage_collect
#define impl_afw_pool_register_cleanup impl_mt_scope_register_cleanup
#define impl_afw_pool_deregister_cleanup impl_mt_scope_deregister_cleanup

#include "afw_pool_impl_declares.h"
#undef AFW_POOL_INF_ONLY
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup


static afw_pool_scope_t *
impl_scope_object_create(
    const afw_pool_t *parent,
    afw_size_t self_bytes,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_scope_t *scope;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_internal_is_heap(parent) &&
        !afw_pool_internal_is_tracker(parent))
    {
        AFW_THROW_ERROR_Z(general,
            "Scope pool parent must be a heap or tracker",
            xctx);
    }
    if (self_bytes < offsetof(afw_pool_scope_t, frame_slots)) {
        self_bytes = offsetof(afw_pool_scope_t, frame_slots);
    }
    if (afw_pool_internal_is_multithreaded(parent)) {
        self = afw_pool_heap_multithreaded_create_self(parent,
            &impl_afw_pool_scope_multithreaded_inf,
            false,
            (xctx->env && xctx->env->small_chunk_min)
                ? xctx->env->small_chunk_min
                : AFW_ENVIRONMENT_SMALL_CHUNK_MIN,
            self_bytes, NULL, xctx);
    }
    else {
        self = afw_pool_heap_create_self(parent,
            &impl_afw_pool_scope_inf,
            false,
            (xctx->env && xctx->env->small_chunk_min)
                ? xctx->env->small_chunk_min
                : AFW_ENVIRONMENT_SMALL_CHUNK_MIN,
            self_bytes, NULL, xctx);
    }
    scope = (afw_pool_scope_t *)self;
    scope->p = &scope->pub;
    /*
     * Old link rule: hold the parent for the life of this scope pool.
     * Pool count stays 1, so throw-path delay still sees a last release.
     */
    if (self->parent && !self->parent->destroying) {
        afw_pool_get_reference(&self->parent->pub, xctx);
    }
    return scope;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_scope_allocate(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    afw_pool_scope_t *scope;

    scope = impl_scope_object_create(
        parent, sizeof(afw_pool_scope_t), xctx);
    return scope->p;
}


void
afw_pool_heap_internal_release_delayed(
    const afw_pool_t *instance,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    afw_pool_internal_scope_self_t *delay;

    (void)instance;
    if (!xctx) {
        return;
    }
    while (xctx->error_delaying_release_first) {
        p = xctx->error_delaying_release_first;
        delay = afw_pool_heap_internal_as_scope(
            (afw_pool_internal_self_t *)p);
        impl_clear_delay(delay, xctx);
        afw_pool_release(p, xctx);
    }
}


//#define AFW_POOL_SCOPE_DEBUG

#ifdef AFW_POOL_SCOPE_DEBUG
#define afw_pool_scope_debug(_place, _block, _scope, _parent_scope, _note, _xctx) \
    impl_scope_debug(_place, _block, _scope, _parent_scope, _note, _xctx)
#else
#define afw_pool_scope_debug(_place, _block, _scope, _parent_scope, _note, _xctx)
#endif

#ifdef AFW_POOL_SCOPE_DEBUG
static void impl_scope_debug(
    const afw_utf8_z_t *place,
    const afw_value_block_t *block,
    const afw_pool_scope_t *scope,
    const afw_pool_scope_t *parent_scope,
    const afw_utf8_z_t *note,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *current_scope;

    current_scope = afw_pool_scope_current(xctx);
    printf("\n");
    printf("--- debug: " AFW_SIZE_T_FMT " %s",
        (scope)
            ? scope->scope_number
            : xctx->scope_count + 1,
        place);

    if (block) {
        printf(
            ", block number: " AFW_SIZE_T_FMT
            ", depth: " AFW_SIZE_T_FMT,
            block->number, block->depth);
    }
    else {
        printf(", block: NULL");
    }

    if (scope) {
        printf(
            ", pool: " AFW_INTEGER_FMT
            ", scope number: " AFW_SIZE_T_FMT
            ", refs: " AFW_SIZE_T_FMT,
            (afw_integer_t)(afw_size_t)scope->p,
            scope->scope_number,
            scope->reference_count);
    }
    else {
        printf(" scope: NULL");
    }

    if (parent_scope) {
        printf(
            ", parent scope number: " AFW_SIZE_T_FMT,
            parent_scope->scope_number);
    }
    else {
        printf(" parent_scope: NULL");
    }

    if (current_scope) {
        printf(
            ", current scope number: " AFW_SIZE_T_FMT,
            current_scope->scope_number);
    }
    else {
        printf(", current scope: NULL");
    }

    printf(
        ", total scope count: " AFW_SIZE_T_FMT
        ", active scope count: " AFW_SIZE_T_FMT,
        xctx->scope_count, xctx->scope_stack->count);

    if (note) {
        printf(" %s", note);
    }
    printf("\n");
}
#endif


const afw_value_t **
afw_pool_scope_symbol_get_value_address(
    const afw_value_block_symbol_t *symbol,
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    for (;
         scope &&
         scope->block->scope_depth > symbol->parent_block->scope_depth;
         scope = scope->parent_lexical_scope);

    if (!scope ||
        scope->block->scope_depth != symbol->parent_block->scope_depth)
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol '%ku' not found in current scope chain",
            &symbol->name->internal);
    }

    if (symbol->index >= scope->symbol_count) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol '%ku' index " AFW_SIZE_T_FMT
            " is out of range for scope",
            &symbol->name->internal, symbol->index);
    }

    return (const afw_value_t **)&scope->frame_slots[symbol->index];
}


const afw_value_t **
afw_pool_scope_symbol_get_value_address_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *scope;
    const afw_value_block_t *block;
    const afw_value_block_symbol_t *symbol;

    for (scope = afw_pool_scope_current(xctx);
         scope;
         scope = scope->parent_lexical_scope)
    {
        for (block = scope->block,
             symbol = block->first_entry;
             symbol;
             symbol = symbol->next_entry)
        {
            if (afw_utf8_equal(symbol_name, &symbol->name->internal)) {
                return (const afw_value_t **)
                    &scope->frame_slots[symbol->index];
            }
        }
    }

    return NULL;
}


const afw_value_t *
afw_pool_scope_symbol_get_value(
    const afw_value_block_symbol_t *symbol,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;

    value_address = afw_pool_scope_symbol_get_value_address(
        symbol, afw_pool_scope_current(xctx), xctx);

    return *value_address;
}


const afw_value_t *
afw_pool_scope_symbol_get_value_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;

    value_address = afw_pool_scope_symbol_get_value_address_by_name(
        symbol_name, xctx);

    if (!value_address) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol name '%ku' not found in current scope chain",
            symbol_name);
    }

    return *value_address;
}


afw_boolean_t
afw_pool_scope_symbol_exists_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx)
{
    return afw_pool_scope_symbol_get_value_address_by_name(
        symbol_name, xctx) != NULL;
}


void
afw_pool_scope_symbol_set_value(
    const afw_value_block_symbol_t *symbol,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;
    const afw_pool_scope_t *scope;

    scope = afw_pool_scope_current(xctx);
    value_address = afw_pool_scope_symbol_get_value_address(
        symbol, scope, xctx);

    afw_value_slot_store(value_address, value,
        scope ? scope->p : xctx->p, xctx);
}


void
afw_pool_scope_symbol_set_value_by_name(
    const afw_utf8_t *symbol_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;
    const afw_pool_scope_t *scope;

    value_address = afw_pool_scope_symbol_get_value_address_by_name(
        symbol_name, xctx);

    if (!value_address) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol name '%ku' not found in current scope chain",
            symbol_name);
    }

    scope = afw_pool_scope_current(xctx);
    afw_value_slot_store(value_address, value,
        scope ? scope->p : xctx->p, xctx);
}


const afw_pool_scope_t *
afw_pool_scope_create(
    const afw_value_block_t *block,
    const afw_pool_scope_t *parent_lexical_scope,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_pool_scope_t *scope;
    afw_size_t self_bytes;
    afw_size_t i;

    if (!block) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_scope_create(): block required",
            xctx);
    }

    if (parent_lexical_scope) {
        if (parent_lexical_scope->block != block->parent_scope_block) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "afw_pool_scope_create(): parent_lexical_scope block is "
                "not parent_scope_block "
                "(scope count: " AFW_SIZE_T_FMT
                ", active scopes: " AFW_SIZE_T_FMT
                ", parent scope number: " AFW_SIZE_T_FMT
                ", parent scope_depth: " AFW_SIZE_T_FMT
                ", block scope_depth: " AFW_SIZE_T_FMT ")",
                xctx->scope_count, xctx->scope_stack->count,
                parent_lexical_scope->scope_number,
                parent_lexical_scope->block->scope_depth,
                block->scope_depth);
        }
    }
    else if (block->parent_scope_block || block->scope_depth != 0) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_scope_create(): parent_lexical_scope required "
            "unless this is the top frame",
            xctx);
    }

    if (!p) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_scope_create(): p required", xctx);
    }

    self_bytes = offsetof(afw_pool_scope_t, frame_slots)
        + (block->symbol_count * sizeof(const afw_value_t *));
    scope = impl_scope_object_create(p, self_bytes, xctx);
    scope->block = block;
    scope->symbol_count = block->symbol_count;
    scope->reference_count = 1;
    scope->last_result = afw_value_void;
    xctx->scope_count++;
    scope->scope_number = xctx->scope_count;

    for (i = 0; i < scope->symbol_count; i++) {
        scope->frame_slots[i] = afw_value_undefined;
    }

    if (parent_lexical_scope) {
        scope->parent_lexical_scope = parent_lexical_scope;
        ((afw_pool_scope_t *)parent_lexical_scope)->reference_count++;
    }

    afw_pool_scope_debug(
        "*  afw_pool_scope_create()",
        block, scope, parent_lexical_scope, NULL, xctx);

    return scope;
}


const afw_pool_scope_t *
afw_pool_scope_find_for_block(
    const afw_value_block_t *block,
    const afw_pool_scope_t *from,
    afw_xctx_t *xctx)
{
    const afw_value_block_t *frame;
    const afw_pool_scope_t *scope;

    (void)xctx;

    if (!from) {
        return NULL;
    }

    if (!block) {
        for (scope = from; scope; scope = scope->parent_lexical_scope) {
            if (!scope->parent_lexical_scope) {
                return scope;
            }
        }
        return NULL;
    }

    frame = afw_value_block_scope_block(block);
    if (!frame) {
        return NULL;
    }
    for (scope = from; scope; scope = scope->parent_lexical_scope) {
        if (scope->block == frame) {
            return scope;
        }
    }
    return NULL;
}


const afw_pool_scope_t *
afw_pool_scope_clone(
    const afw_pool_scope_t *original_scope,
    afw_xctx_t *xctx)
{
    afw_pool_scope_t *scope;
    afw_size_t i;

    {
        const afw_pool_internal_self_t *parent_self;

        parent_self = ((const afw_pool_internal_self_t *)
            original_scope->p)->parent;
        if (!parent_self) {
            AFW_THROW_ERROR_Z(general,
                "afw_pool_scope_clone(): original scope pool has no parent",
                xctx);
        }
        scope = (afw_pool_scope_t *)afw_pool_scope_create(
            original_scope->block, original_scope->parent_lexical_scope,
            &parent_self->pub, xctx);
    }

    for (i = 0; i < scope->symbol_count; i++) {
        afw_value_slot_store(&scope->frame_slots[i],
            original_scope->frame_slots[i], scope->p, xctx);
    }

    afw_xctx_script_result_set_value(original_scope->last_result, xctx);
    ((afw_pool_scope_t *)original_scope)->cloned = true;

    afw_pool_scope_debug(
        "*c afw_pool_scope_clone()",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    return scope;
}


void
afw_pool_scope_activate(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    ((afw_pool_scope_t *)scope)->reference_count++;
    afw_vector_push(xctx->scope_stack, xctx) = scope;

    afw_pool_scope_debug(
        "-> afw_pool_scope_activate()",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);
}


const afw_pool_scope_t *
afw_pool_scope_get_reference(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    ((afw_pool_scope_t *)scope)->reference_count++;

    afw_pool_scope_debug(
        "+1 afw_pool_scope_get_reference()",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    return scope;
}


void
afw_pool_scope_deactivate(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    afw_pool_scope_debug(
        "<- afw_pool_scope_deactivate() begin",
        scope->block, scope, scope->parent_lexical_scope,
        (afw_pool_scope_current(xctx) == scope)
            ? NULL
            : "- current scope is not scope passed",
        xctx);

    if (scope != afw_pool_scope_current(xctx)) {
        AFW_THROW_ERROR_Z(general,
            "Request to deactivate scope that is not current",
            xctx);
    }

    if (!scope->cloned) {
        afw_xctx_script_result_set(scope->last_result, xctx);
    }
    afw_vector_pop(xctx->scope_stack, xctx);
    afw_pool_scope_release(scope, xctx);
}


void
afw_pool_scope_unwind(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *current_scope;

    afw_pool_scope_debug(
        "-n afw_pool_scope_unwind() begin",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    for (;;) {
        current_scope = afw_pool_scope_current(xctx);
        if (!current_scope) {
            AFW_THROW_ERROR_Z(general,
                "afw_pool_scope_unwind() did not find specified scope",
                xctx);
        }
        if (scope == current_scope) {
            break;
        }
        afw_pool_scope_deactivate(current_scope, xctx);
    }
}


void
afw_pool_scope_release(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    afw_pool_scope_debug(
        "-1 afw_pool_scope_release() begin",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    if (scope->reference_count == 0) {
        return;
    }

    ((afw_pool_scope_t *)scope)->reference_count--;
    if (scope->reference_count > 0) {
        return;
    }

    if (scope->block) {
        for (i = 0; i < scope->symbol_count; i++) {
            afw_value_release(scope->frame_slots[i], xctx);
            ((afw_pool_scope_t *)scope)->frame_slots[i] =
                afw_value_undefined;
        }
    }
    if (scope->parent_lexical_scope) {
        afw_pool_scope_release(scope->parent_lexical_scope, xctx);
    }
    afw_pool_release(scope->p, xctx);
}


void
afw_pool_scope_set_last_result(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *scope;

    if (!value || afw_value_is_void(value)) {
        return;
    }
    scope = afw_pool_scope_current(xctx);
    if (scope) {
        ((afw_pool_scope_t *)scope)->last_result = value;
    }
}


void
afw_pool_scope_clear_last_result(
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *scope;

    scope = afw_pool_scope_current(xctx);
    if (scope) {
        ((afw_pool_scope_t *)scope)->last_result = afw_value_void;
    }
}


const afw_value_t *
afw_pool_scope_get_assignable_for_p_lifetime(
    const afw_value_t *value,
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx)
{
    if (!value || afw_value_is_void(value)) {
        return value ? value : afw_value_void;
    }
    if (!value->inf || !value->inf->optional_release) {
        return value;
    }
    if (scope &&
        afw_pool_is_value_release_registered(value, scope->p, xctx))
    {
        return value;
    }
    value = afw_value_get_assignable(value,
        scope ? scope->p : xctx->p, xctx);
    if (scope) {
        afw_pool_release_value_at_cleanup(value, scope->p, xctx);
    }
    return value;
}


const afw_value_t *
afw_pool_scope_get_assignable_for_scope_lifetime(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    return afw_pool_scope_get_assignable_for_p_lifetime(
        value, afw_pool_scope_current(xctx), xctx);
}


const afw_value_t *
afw_pool_scope_set_last_result_for_lifetime(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    value = afw_pool_scope_get_assignable_for_scope_lifetime(value, xctx);
    afw_pool_scope_set_last_result(value, xctx);
    return value;
}

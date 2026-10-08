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
 * The scope's count is the pool's count. Last release releases the
 * frame slots and the lexical parent, then the pool. While a throw is
 * processed, that last release is delayed until the ENDTRY that
 * handles it, so a catch can still read what the error points to.
 * The multithreaded inf is `afw_pool_scope_multithreaded.c`.
 */

#include "afw_internal.h"

#define AFW_POOL_SELF_T afw_pool_internal_self_t

static const afw_pool_internal_inf_implementation_specific_t
impl_scope_specific =
    {
        /* multithreaded */ false,
        /* tracker */ false
    };

static void
impl_clear_delay(
    afw_pool_heap_internal_scope_self_t *me, afw_xctx_t *xctx)
{
    const afw_pool_t **pos;
    const afw_pool_t *prev;
    afw_pool_heap_internal_scope_self_t *curr;

    if (!me->error_delaying_release) {
        return;
    }
    me->error_delaying_release = false;
    if (!xctx) {
        me->error_delaying_release_next = NULL;
        return;
    }
    prev = NULL;
    pos = &xctx->error_delaying_release_first;
    while (*pos) {
        curr = afw_pool_heap_internal_as_scope(
            (afw_pool_internal_self_t *)(void *)*pos);
        if (curr == me) {
            *pos = curr->error_delaying_release_next;
            if (xctx->error_delaying_release_last ==
                &me->heap.common.pub)
            {
                xctx->error_delaying_release_last = prev;
            }
            curr->error_delaying_release_next = NULL;
            return;
        }
        prev = *pos;
        pos = &curr->error_delaying_release_next;
    }
    me->error_delaying_release_next = NULL;
}


/*
 * While error_processing_count > 0, last release of a scope pool
 * is recorded at the end of the list and skipped. Catching ENDTRY
 * runs afw_pool_heap_internal_release_delayed() when the count is 0
 * again, oldest first.
 */
static afw_boolean_t
impl_error_delaying_release(
    afw_pool_heap_internal_scope_self_t *me,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_heap_internal_scope_self_t *last;

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
    me->error_delaying_release_next = NULL;
    if (xctx->error_delaying_release_last) {
        last = afw_pool_heap_internal_as_scope(
            (afw_pool_internal_self_t *)(void *)
            xctx->error_delaying_release_last);
        last->error_delaying_release_next = &self->pub;
    }
    else {
        xctx->error_delaying_release_first = &self->pub;
    }
    xctx->error_delaying_release_last = &self->pub;
    return true;
}


static void
impl_scope_teardown(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_reference_forget(&self->pub.ref, xctx);
    afw_pool_heap_internal_teardown_store(self, xctx);
}


/*
 * for_each_reference of a scope: its frame slots and its lexical
 * parent (the references its last release releases).
 */
void
afw_pool_internal_scope_for_each_reference(
    AFW_POOL_SELF_T *self,
    afw_reference_cb_t callback,
    void *context,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *scope = (const afw_pool_scope_t *)self;
    afw_size_t i;

    if (scope->block) {
        for (i = 0; i < scope->symbol_count; i++) {
            afw_value_list_reference(scope->frame_slots[i],
                callback, context, xctx);
        }
    }
    if (scope->parent_lexical_scope) {
        callback(&scope->parent_lexical_scope->pub.ref, context, xctx);
    }
}


/* release_references of a scope: frame slots and lexical parent. */
void
afw_pool_internal_scope_release_references(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_scope_t *scope = (afw_pool_scope_t *)self;
    const afw_pool_scope_t *parent;
    const afw_value_t *value;
    afw_size_t i;

    if (scope->block) {
        for (i = 0; i < scope->symbol_count; i++) {
            value = scope->frame_slots[i];
            scope->frame_slots[i] = afw_value_undefined;
            afw_value_release(value, xctx);
        }
    }
    parent = scope->parent_lexical_scope;
    scope->parent_lexical_scope = NULL;
    if (parent) {
        afw_pool_scope_release(parent, xctx);
    }
}


/* Same rule as every pool (see holds_parent). */
void
afw_pool_internal_scope_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_internal_get_reference(self, xctx);
}


/*
 * A scope's count is its pool's count. The last release first releases
 * the frame slots and the lexical parent, then the pool.
 */
const afw_pool_t *
afw_pool_internal_scope_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_scope_t *scope = (afw_pool_scope_t *)self;
    const afw_pool_scope_t *parent;
    afw_size_t i;

    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "release");
    if (scope->releasing_frame) {
        return &self->pub;
    }
    if (impl_error_delaying_release(
            afw_pool_heap_internal_as_scope(self), xctx))
    {
        return &self->pub;
    }
    if (self->reference_count > 1 && self->parent) {
        afw_reference_possible_root(&self->pub.ref, &self->parent->pub,
            xctx);
    }
    if (self->reference_count == 1 && !self->destroying) {
        scope->releasing_frame = true;
        if (scope->block) {
            for (i = 0; i < scope->symbol_count; i++) {
                afw_value_release(scope->frame_slots[i], xctx);
                scope->frame_slots[i] = afw_value_undefined;
            }
        }
        parent = scope->parent_lexical_scope;
        scope->parent_lexical_scope = NULL;
        if (parent) {
            afw_pool_scope_release(parent, xctx);
        }
        scope->releasing_frame = false;
    }
    return afw_pool_internal_release_common(
        self, xctx, impl_scope_teardown);
}


void
afw_pool_internal_scope_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
    if (!self->destroying) {
        impl_clear_delay(afw_pool_heap_internal_as_scope(self), xctx);
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_run_child_cleanups(self, xctx);
    afw_pool_internal_run_cleanups(self, xctx);
}


void
afw_pool_internal_scope_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        impl_clear_delay(afw_pool_heap_internal_as_scope(self), xctx);
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_destroy_children(self, xctx);
    impl_scope_teardown(self, xctx);
}


void
afw_pool_internal_scope_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    (void)self;
    (void)xctx;
}


#define AFW_IMPLEMENTATION_ID "scope"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_scope_inf
#define AFW_IMPLEMENTATION_SPECIFIC &impl_scope_specific
#define impl_afw_pool_release afw_pool_internal_scope_release
#define impl_afw_pool_get_reference afw_pool_internal_scope_get_reference
#define impl_afw_pool_run_cleanups afw_pool_internal_scope_run_cleanups
#define impl_afw_pool_destroy afw_pool_internal_scope_destroy
#define impl_afw_pool_calloc afw_pool_heap_internal_calloc
#define impl_afw_pool_malloc afw_pool_heap_internal_malloc
#define impl_afw_pool_free_memory afw_pool_heap_internal_free_memory
#define impl_afw_pool_free_memory_no_throw \
    afw_pool_heap_internal_free_memory_no_throw
#define impl_afw_pool_calloc_no_throw afw_pool_heap_internal_calloc_no_throw
#define impl_afw_pool_malloc_no_throw afw_pool_heap_internal_malloc_no_throw
#define impl_afw_pool_garbage_collect afw_pool_internal_scope_garbage_collect
#define impl_afw_pool_register_cleanup afw_pool_internal_register_cleanup
#define impl_afw_pool_deregister_cleanup afw_pool_internal_deregister_cleanup

AFW_POOL_INTERNAL_REFERENCE_WRAPPERS(impl_pool_ref_4, afw_pool_internal_scope_release, afw_pool_internal_scope_get_reference)
#undef impl_afw_pool_release
#define impl_afw_pool_release impl_pool_ref_4_release
#undef impl_afw_pool_get_reference
#define impl_afw_pool_get_reference impl_pool_ref_4_get_reference
#undef impl_afw_pool_get_reference_count
#define impl_afw_pool_get_reference_count afw_pool_internal_get_reference_count
#undef impl_afw_pool_for_each_reference
#define impl_afw_pool_for_each_reference afw_pool_internal_scope_for_each_reference
#undef impl_afw_pool_release_references
#define impl_afw_pool_release_references afw_pool_internal_scope_release_references
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
        self = afw_pool_heap_internal_multithreaded_create_self(parent,
            &afw_pool_internal_scope_multithreaded_inf,
            false,
            (xctx->env && xctx->env->small_chunk_min)
                ? xctx->env->small_chunk_min
                : AFW_ENVIRONMENT_SMALL_CHUNK_MIN,
            self_bytes, NULL, xctx);
    }
    else {
        self = afw_pool_heap_internal_create_self(parent,
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
     * No parent reference from create (see holds_parent). A frame's
     * parent is the job heap (see afw_pool_scope_create), which
     * outlives it.
     */
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
    afw_pool_heap_internal_scope_self_t *delay;

    (void)instance;
    if (!xctx) {
        return;
    }
    /* Oldest first: the order the releases would have happened. */
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

    current_scope = afw_pool_scope_internal_current(xctx);
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
            afw_pool_get_reference_count(scope->p, NULL));
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

    for (scope = afw_pool_scope_internal_current(xctx);
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
        symbol, afw_pool_scope_internal_current(xctx), xctx);

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

    scope = afw_pool_scope_internal_current(xctx);
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

    scope = afw_pool_scope_internal_current(xctx);
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

    /*
     * Scope pool parent is the job heap (p->managed_p), not dest p. A
     * scope lives by its scope RC. Its create reference on the parent
     * must not keep dest p alive: a closure whose last-release is
     * registered on dest p references this frame, so a frame that pins
     * dest p is a cycle. Lexical parents live by scope RC, not by the
     * pool tree.
     */
    self_bytes = offsetof(afw_pool_scope_t, frame_slots)
        + (block->symbol_count * sizeof(const afw_value_t *));
    scope = impl_scope_object_create(p->managed_p, self_bytes, xctx);
    scope->block = block;
    scope->symbol_count = block->symbol_count;
    scope->last_statement_non_void_value = afw_value_void;
    xctx->scope_count++;
    scope->scope_number = xctx->scope_count;

    for (i = 0; i < scope->symbol_count; i++) {
        scope->frame_slots[i] = afw_value_undefined;
    }

    if (parent_lexical_scope) {
        scope->parent_lexical_scope = afw_pool_scope_get_reference(
            parent_lexical_scope, xctx);
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

    scope = (afw_pool_scope_t *)afw_pool_scope_create(
        original_scope->block, original_scope->parent_lexical_scope,
        original_scope->p, xctx);

    for (i = 0; i < scope->symbol_count; i++) {
        afw_value_slot_store(&scope->frame_slots[i],
            original_scope->frame_slots[i], scope->p, xctx);
    }

    afw_xctx_script_result_set_value(original_scope->last_statement_non_void_value,
        original_scope->p, xctx);
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
    afw_pool_get_reference(scope->p, xctx);
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
    afw_pool_get_reference(scope->p, xctx);

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
        (afw_pool_scope_internal_current(xctx) == scope)
            ? NULL
            : "- current scope is not scope passed",
        xctx);

    if (scope != afw_pool_scope_internal_current(xctx)) {
        AFW_THROW_ERROR_Z(general,
            "Request to deactivate scope that is not current",
            xctx);
    }

    if (!scope->cloned) {
        afw_xctx_script_result_set(scope->last_statement_non_void_value, scope->p, xctx);
    }
    afw_vector_pop(xctx->scope_stack, xctx);
    if (afw_reference_check_is_enabled()) {
        afw_reference_check(&scope->pub.ref, xctx);
    }
    afw_pool_scope_release(scope, xctx);
    afw_reference_safe_point(xctx);
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
        current_scope = afw_pool_scope_internal_current(xctx);
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
    afw_pool_scope_debug(
        "-1 afw_pool_scope_release() begin",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    afw_pool_release(scope->p, xctx);
}


void
afw_pool_scope_set_last_statement_non_void_value(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *scope;

    if (!value || afw_value_is_void(value)) {
        return;
    }
    scope = afw_pool_scope_internal_current(xctx);
    if (scope) {
        ((afw_pool_scope_t *)scope)->last_statement_non_void_value = value;
    }
}


void
afw_pool_scope_clear_last_statement_non_void_value(
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *scope;

    scope = afw_pool_scope_internal_current(xctx);
    if (scope) {
        ((afw_pool_scope_t *)scope)->last_statement_non_void_value = afw_value_void;
    }
}


/*
 * Nearest scope in dest p's pool-parent chain. Throw if dest p is a
 * job heap (p == p->managed_p) or the chain ends with no scope.
 * Probe: do not last-release on the job heap from these helpers.
 */
static const afw_pool_t *
impl_scope_dest_p(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_pool_internal_self_t *self;

    if (!p) {
        AFW_THROW_ERROR_Z(general,
            "for_scope_lifetime with no dest p",
            xctx);
    }
    for (;;) {
        if (afw_pool_internal_is_scope(p)) {
            return p;
        }
        if (p->managed_p == p) {
            AFW_THROW_ERROR_Z(general,
                "for_scope_lifetime dest p reached job heap "
                "(p == p->managed_p)",
                xctx);
        }
        self = (const afw_pool_internal_self_t *)p;
        if (!self->parent) {
            AFW_THROW_ERROR_Z(general,
                "for_scope_lifetime dest p has no scope in parent chain",
                xctx);
        }
        p = &self->parent->pub;
    }
}


const afw_value_t *
afw_pool_scope_get_assignable_for_p_lifetime(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value) {
        return afw_value_void;
    }
    if (!p) {
        AFW_THROW_ERROR_Z(general,
            "get_assignable_for_p_lifetime with no dest p", xctx);
    }
    return afw_value_get_for_p_lifetime(value, p, xctx);
}


const afw_value_t *
afw_pool_scope_get_assignable_for_scope_lifetime(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_pool_scope_get_assignable_for_p_lifetime(
        value, impl_scope_dest_p(p, xctx), xctx);
}


const afw_value_t *
afw_pool_scope_release_value_at_cleanup(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value) {
        return value;
    }
    p = impl_scope_dest_p(p, xctx);
    afw_pool_release_value_at_cleanup(value, p, xctx);
    return value;
}


const afw_value_t *
afw_pool_scope_set_last_statement_non_void_value_for_lifetime(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    value = afw_pool_scope_get_assignable_for_scope_lifetime(value, p, xctx);
    afw_pool_scope_set_last_statement_non_void_value(value, xctx);
    return value;
}

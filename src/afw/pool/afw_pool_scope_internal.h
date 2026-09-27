// See the 'COPYING' file in the project root for licensing information.
/*
 * Scope pool is the evaluation frame.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_SCOPE_INTERNAL_H__
#define __AFW_POOL_SCOPE_INTERNAL_H__

#include "afw_pool_heap_internal.h"
#include "afw_vector.h"

/**
 * @file afw_pool_scope_internal.h
 * @brief Evaluation `{ }` frame. The scope object is its pool.
 *
 * Core only. `afw_pool_scope_t` in public headers is an incomplete type.
 * `p` points at `pub`, which is the same address as this object.
 * `frame_slots` is last. Create sizes the pool self to `symbol_count`.
 */

AFW_BEGIN_DECLARES

struct afw_pool_scope_s {

    union {
        afw_pool_t pub;
        afw_pool_heap_internal_scope_self_t internal;
    };

    /** Same address as pub. Callers keep using scope->p. */
    const afw_pool_t *p;

    const afw_value_block_t *block;

    const afw_pool_scope_t *parent_lexical_scope;

    /** Scope holds. The pool count is separate (create ref stays 1). */
    afw_size_t reference_count;

    afw_size_t scope_number;

    /** Last non-void statement. Starts void. */
    const afw_value_t *last_result;

    /**
     * Length of frame_slots. Copied from the block at create.
     * Release uses this, not block->symbol_count: the block lives
     * in the compile pool and must not be able to change the walk.
     */
    afw_size_t symbol_count;

    /**
     * Set when a sibling clone is taken. This frame is no longer
     * the running iteration; deactivate does not script_result_set.
     */
    afw_boolean_t cloned;

    /**
     * Flexible array, length block->symbol_count. Indexed by
     * afw_value_block_symbol_t.index.
     */
    const afw_value_t *frame_slots[1];
};


/* ----------------------------------------------------------------------------

    Execution Context (xctx) Scope
    
---------------------------------------------------------------------------- */


AFW_VECTOR_STRUCT(afw_pool_scope_p_vector_s, const afw_pool_scope_t *);

/**
 * @brief Get current scope.
 * @param xctx of caller.
 * @return Current scope.
 */
#define afw_pool_scope_internal_current(_xctx) \
    ((_xctx->scope_stack->count > 0) \
    ? _xctx->scope_stack->entries[_xctx->scope_stack->count - 1] \
    : NULL)

/**
 * @brief Scope that invoked the current frame, or NULL.
 *
 * The entry under current. Compiled-value sentinel is NULL, so a
 * top-level call has no Adaptive caller. After a script function
 * body, nested `{ }` have unwound and this is the caller `{ }`.
 */
#define afw_pool_scope_internal_of_caller(_xctx) \
    ((_xctx->scope_stack->count >= 2) \
    ? _xctx->scope_stack->entries[_xctx->scope_stack->count - 2] \
    : NULL)

/**
 * @brief Store a non-void statement result on the current scope.
 * @param value statement result.
 * @param xctx of caller.
 *
 * Pointer only. Void, NULL, and no current scope are ignored. Isolate
 * out of this frame is script_result_set_value at deactivate, or at
 * clone while this p is still alive.
 */
void
afw_pool_scope_set_last_result(
    const afw_value_t *value,
    afw_xctx_t *xctx);


/**
 * @brief Set current last_result to void.
 * @param xctx of caller.
 *
 * set_last_result() ignores void so a prior last is sticky. Nested
 * `{ }` that already isolated into script_result uses this so parent
 * deactivate does not stomp. No-op if there is no current scope.
 */
void
afw_pool_scope_clear_last_result(
    afw_xctx_t *xctx);


/**
 * @brief Get an assignable and keep it until the current scope ends.
 * @param value to keep. Void and NULL are returned unchanged.
 * @param xctx of caller.
 * @return assignable value, or void/NULL unchanged.
 *
 * get_assignable plus pool-cleanup on current scope->p. Does not
 * store last_result. Built-ins that return an input or a managed
 * result use this.
 */
const afw_value_t *
afw_pool_scope_get_assignable_for_scope_lifetime(
    const afw_value_t *value,
    afw_xctx_t *xctx);


/**
 * @brief Get an assignable and keep it until this scope ends.
 * @param value to keep. Void and NULL are returned unchanged.
 * @param scope whose p last-release drops the hold, or NULL for
 *    get_assignable only.
 * @param xctx of caller.
 * @return assignable value, or void/NULL unchanged.
 *
 * get_assignable (self-reference if managed, often clone_managed if
 * unmanaged) then cleanup release on scope->p. Managed values
 * (including closures) may use any scope: RC keeps them alive; the
 * callback drops the extra hold when that p ends.
 */
const afw_value_t *
afw_pool_scope_get_assignable_for_p_lifetime(
    const afw_value_t *value,
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Set last_result to an assignable held until this scope ends.
 * @param value to keep. Void and NULL are returned unchanged.
 * @param xctx of caller.
 * @return held value, or void/NULL unchanged.
 *
 * afw_pool_scope_get_assignable_for_scope_lifetime() then
 * afw_pool_scope_set_last_result().
 */
const afw_value_t *
afw_pool_scope_set_last_result_for_lifetime(
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Create a new scope.
 * @param block associated with this scope.
 * @param parent_lexical_scope of this scope or NULL for first one.
 * @param p dest pool of this evaluate (top: evaluate dest; nested:
 *    parent scope->p).
 * @param xctx of caller.
 * @return New xctx scope.
 *
 * Function afw_pool_scope_create() is used to create a new scope for the
 * supplied block. last_result starts as the void singleton. Each
 * frame_slots[] entry starts as the permanent
 * **afw_value_undefined** singleton (not C NULL) so a bound name always has a
 * value pointer; see afw_pool_scope_symbol_exists_by_name and issue #131.
 * afw_pool_scope_symbol_set_value() also stores that singleton when given
 * C NULL.
 *
 * If a parent_lexical_scope is specified, it's reference count will be
 * incremented. That scope's block must be this block's parent_scope_block
 * (the enclosing `{ }`). Every `{ }` is a frame.
 *
 * If parent_lexical_scope is NULL, this must be the top frame
 * (parent_scope_block NULL, scope_depth 0).
 *
 * Create starts at reference count 1 (the creator owns it). The creator
 * must afw_pool_scope_release() when done. activate / get_reference and
 * create-with-this-as-parent increment. deactivate, release, and unwind
 * decrement. When the count reaches 0, walk frame_slots[], release the
 * parent lexical scope if any, and release this scope's pool. A closure
 * can keep the scope alive after the creator's release.
 *
 * More detail on how scopes work:
 *
 * An empty scope stack is created when the xctx is created and destroyed when
 * the xctx is destroyed. This scope stack is a stack of pointers to scope
 * structs of the currently active scopes in order of their activation.
 *
 * The current scope, which can be retrieved by calling
 * afw_pool_scope_internal_current(), is at the top of the scope stack.
 *
 * The scope stack is maintained by afw_pool_scope_activate(), which
 * pushes a scope and takes a stack reference, paired with
 * afw_pool_scope_deactivate() which pops and releases that stack
 * reference. The creator's reference is separate.
 *
 * afw_pool_scope_unwind(), used in catch, deactivates every current
 * scope down to (not including) the scope at try entry.
 *
 * The evaluate for a compiled value always pushes a NULL on the scope
 * stack before evaluating its root value, then makes sure the NULL is
 * still there and removes it when evaluation is complete. The root
 * value thus begins with current NULL, so its first frame is the top
 * block (scope_depth 0).
 *
 * Symbols (variables, parameters, etc.) go in and out of scope. The scope
 * struct has frame_slots[], indexed by afw_value_block_symbol_t.index (compile
 * time). A symbol's block has a scope_depth. The current scope's scope_depth
 * minus that is how many times parent_lexical_scope is followed to find
 * the scope with that symbol's frame_slots[] entry.
 *
 * When a closure binding is created, afw_pool_scope_get_reference() is called
 * on its enclosing scope. When the closure binding's last release runs, a
 * corresponding afw_pool_scope_release() is called.
 *
 * The afw_pool_scope_symbol_*() functions get and set frame_slots[] entries.
 */
const afw_pool_scope_t *
afw_pool_scope_create(
    const afw_value_block_t *block,
    const afw_pool_scope_t *parent_lexical_scope,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Find the live scope for a compile-time block.
 * @param block whose scope to find, or NULL for the compiled-value root.
 * @param from start of the lexical parent chain (usually current).
 * @param xctx of caller.
 * @return matching scope, or NULL if it is not on the chain.
 *
 * Match the live chain for this block's frame: the block itself if it
 * has a scope, else parent_scope_block. Missing is the same "not on the
 * stack" hole as a non-closure call after the defining function returned.
 */
const afw_pool_scope_t *
afw_pool_scope_find_for_block(
    const afw_value_block_t *block,
    const afw_pool_scope_t *from,
    afw_xctx_t *xctx);


/**
 * @brief Clone a scope.
 * @param original_scope to clone.
 * @param xctx of caller.
 *
 * This function calls afw_pool_scope_create() and stores a reference to
 * each original frame_slots[] occupant into the new scope (same protocol
 * as assign). script_result_set(original last_result) then clone
 * last_result stays void from create. Marks original cloned so its
 * deactivate does not script_result_set (it is not the running
 * iteration). The clone is a sibling (same parent_lexical_scope).
 *
 * for (let) clones so a closure from the body can hold that trip's
 * names. Next trip copies slots then releases the previous clone.
 */
const afw_pool_scope_t *
afw_pool_scope_clone(
    const afw_pool_scope_t *original_scope,
    afw_xctx_t *xctx);



/**
 * @brief Activate scope.
 * @param scope to activate as the current scope.
 * @param xctx of caller.
 * 
 * Push this scope as current and take a stack reference. Pair with
 * deactivate. Same scope may be activated more than once.
 */
void
afw_pool_scope_activate(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx);



/**
 * @brief Add a reference to a scope.
 * @param scope to be referenced. 
 * @param xctx of caller.
 * @return scope.
 */
const afw_pool_scope_t *
afw_pool_scope_get_reference(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Deactivate scope.
 * @param scope to deactivate that must be the current scope.
 * @param xctx of caller.
 *
 * If this scope was not cloned, script_result_set(last_result).
 * Then pop and release the stack's reference. Pair with activate.
 * Does not drop the creator's reference. Return/break/continue only
 * set statement_flow.
 */
void
afw_pool_scope_deactivate(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx);



/**
 * @brief Release a reference to a scope.
 * @param scope to release.
 * @param xctx of caller.
 * 
 * Decrement the reference count. On last release (count goes 0), walk
 * frame_slots[] (`release` each), then the parent lexical scope, then
 * this scope's pool. Re-entry while the count is already 0 is a no-op.
 */
void
afw_pool_scope_release(
    const afw_pool_scope_t *scope, afw_xctx_t *xctx);



/**
 * @brief Unwind the scope stack down to but not including the specified scope.
 * @param scope to unwind down to
 * @param xctx of caller.
 * 
 * This pops and releases all of the scopes in the scope stack down to but not
 * including the specified scope.  This is used to unwind the scope stack when
 * an error occurs.
 */
void
afw_pool_scope_unwind(
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Get the address where the value of a symbol is stored within the
 *     current scope chain.
 * @param symbol whose value address is to be returned.
 * @param scope to start search from.
 * @param xctx of caller.
 * @return value address (never NULL on success).
 *
 * An error is thrown if the symbol's value location is not found. This most
 * likely is caused by a compile error.
 *
 * Non-NULL address means the symbol is bound. The frame_slots[] entry is
 * normally the permanent undefined singleton until assigned (or an explicit
 * value); C NULL in a slot is legacy — treat with afw_value_is_undefined().
 */
const afw_value_t **
afw_pool_scope_symbol_get_value_address(
    const afw_value_block_symbol_t *symbol,
    const afw_pool_scope_t *scope,
    afw_xctx_t *xctx);


/**
 * @brief Get the address where the value of a named symbol is stored within the
 *     current scope chain.
 * @param symbol_name of symbol whose value address is to be returned.
 * @param xctx of caller.
 * @return value address, or NULL if no symbol with that name is bound.
 *
 * NULL return means the name is **not bound**. Non-NULL means bound; *address
 * is the current value (undefined singleton until assigned). Script
 * variable_exists uses this distinction (issue #131). Prefer this or
 * afw_pool_scope_symbol_exists_by_name() over truthiness-testing a get
 * result from the unqualified get path alone.
 */
const afw_value_t **
afw_pool_scope_symbol_get_value_address_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx);



/**
 * @brief Get the value of a symbol in the current scope chain.
 * @param symbol to get value of.
 * @param xctx of caller.
 * @return value pointer (undefined singleton until assigned; use
 *     afw_value_is_undefined).
 *
 * An error is thrown if the symbol's value location is not found. Does not mean
 * “missing symbol” when the value is undefined.
 */
const afw_value_t *
afw_pool_scope_symbol_get_value(
    const afw_value_block_symbol_t *symbol,
    afw_xctx_t *xctx);



/**
 * @brief Get the value of a named symbol in the current scope chain.
 * @param symbol_name of value to get.
 * @param xctx of caller.
 * @return value pointer (undefined singleton until assigned).
 *
 * An error is thrown if the name is not bound. Undefined value is not “missing.”
 */
const afw_value_t *
afw_pool_scope_symbol_get_value_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx);



/**
 * @brief True if the named lexical symbol is bound in the current scope chain.
 * @param symbol_name of symbol to check.
 * @param xctx of caller.
 * @return true if a symbol with that name exists (any value, including
 *     the undefined singleton).
 *
 * This is the C-side “variable_exists” for unqualified names: **bound**, not
 * “value is non-nullish.” Does not consult the qualifier stack (use
 * afw_xctx_get_optionally_qualified_variable for qualifier::name presence via
 * get_cb contract).
 */
afw_boolean_t
afw_pool_scope_symbol_exists_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx);



/**
 * @brief Set the value of a symbol in the current scope chain.
 * @param symbol to set value of.
 * @param value to set.
 * @param xctx of caller.
 */
void
afw_pool_scope_symbol_set_value(
    const afw_value_block_symbol_t *symbol,
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Set the value of a named symbol in the current scope chain.
 * @param symbol_name of value to set.
 * @param value to set.
 * @param xctx of caller.
 */
void
afw_pool_scope_symbol_set_value_by_name(
    const afw_utf8_t *symbol_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);



AFW_END_DECLARES

#endif /* __AFW_POOL_SCOPE_INTERNAL_H__ */

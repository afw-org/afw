// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_xctx interface
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_xctx.c
 * @brief Execution context (xctx) create, scope, and error state.
 */

#include "afw_internal.h"
#include <libxml/xmlregexp.h>


static void XMLCDECL
impl_suppress_libxml2_message(
    void *ctx,
    const char *msg,
    ...)
{
    /* Do nothing just suppress print. */
}



AFW_DEFINE(afw_xctx_t *)
afw_xctx_internal_create_initialize(
    afw_try_t *unhandled_error,
    afw_error_t *error,
    afw_environment_internal_t *env,
    const afw_pool_t *p)
{
    afw_xctx_t *self;

    if (!error) {
        /* Allocate cleared afw_error_t. */
        error = apr_pcalloc(afw_pool_get_apr_pool(p), sizeof(afw_error_t));
    }

    /* Initialize self. */
    self = apr_pcalloc(afw_pool_get_apr_pool(p), sizeof(afw_xctx_t));
    if (!self) {
        AFW_THROW_UNHANDLED_ERROR(unhandled_error, error, general,
            na, 0, "apr_pcalloc() failed");
    }
    self->p = p;
    self->script_result = afw_value_undefined;
    self->mode = afw_authorization_mode_id_user_value;
    self->current_try = unhandled_error;
    self->error = error;
    self->env = (const afw_environment_t *)env;
    error->xctx = self;
    self->flags_count = env->pub.flags_count_allocated;
    self->flags = (afw_boolean_t *)env->pub.default_flags;
    /*! \fixme stream_anchor may be too early??? */
    self->stream_anchor = afw_stream_internal_stream_anchor_create(self);

    self->scope_stack = apr_array_make(afw_pool_get_apr_pool(p),
        10, sizeof(afw_xctx_scope_t *));
    if (!self->scope_stack) {
        AFW_THROW_UNHANDLED_ERROR(unhandled_error, error, general, na, 0,
            "apr_array_make() failed");
    }

    /*
     * Set libxml2 error func to suppress error print. Use
     * xmlGetLastError() instead.
     */
    self->libxml2_error_func = (void *)impl_suppress_libxml2_message;
    initGenericErrorDefaultFunc((xmlGenericErrorFunc *)&self->libxml2_error_func);

    /* Make qualifier and evaluation stack. */
    afw_stack_internal_set_qualifier_stack(self);
    afw_stack_internal_set_evaluation_stack(self);

    /* Return new xctx. */
    return self;
}


AFW_DEFINE(void)
afw_xctx_internal_create_finishup(afw_xctx_t *xctx)
{
    /*
     * Finish setting up new xctx.
     *
     * Warning:  Make sure functions called can deal with partially set up
     *           xctx.  We need to call these with new xctx so the
     *           resources will be associated with the new xctx.
     *
     */

    /* Get xctx's UUID. */
    xctx->uuid = afw_uuid_create_utf8(xctx->p, xctx);

    /* Create a properties object. */
    xctx->properties = afw_object_create_unmanaged(
        xctx->p, xctx);

    /* Set times. */
    afw_dateTime_set_now(&xctx->local_dateTime_when_created,
        &xctx->utc_dateTime_when_created, xctx);

    /* Push application qualifiers if appropriate. */
    afw_application_internal_push_qualifiers(xctx);
}


AFW_DEFINE(afw_xctx_t *)
afw_xctx_internal_create_thread_xctx(
    const afw_thread_t *thread, afw_xctx_t *xctx) 
{
    afw_xctx_t *self;

    self = afw_xctx_internal_create_initialize(xctx->current_try,
        NULL, (afw_environment_internal_t *)xctx->env, thread->p);
    if (!self) {
        AFW_THROW_ERROR_Z(general, "Error creating xctx", xctx);
    }

    /* Set thread, name and number in xctx. */
    self->thread = thread;
    self->name = thread->name;
    self->parent = xctx;

    xctx = (afw_xctx_t *)self;

    /* Finish xctx creation. */
    afw_xctx_internal_create_finishup(xctx);

    /* Return self. */
    return self;
}


AFW_DEFINE(afw_xctx_t *)
afw_xctx_create(
    const afw_utf8_t *name, afw_integer_t number, afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    afw_xctx_t *self;

    /* Create a new pool for xctx and initialize. */
    p = afw_pool_create_xctx_p(xctx->p, xctx);
    self = afw_xctx_internal_create_initialize(xctx->current_try,
        NULL, (afw_environment_internal_t *)xctx->env, p);
    if (!self) {
        AFW_THROW_ERROR_Z(general, "Error creating xctx", xctx);
    }

    /* Set thread, name and number in xctx. */
    self->thread = xctx->thread;
    self->name = name;
    self->parent = xctx;

    xctx = (afw_xctx_t *)self;

    /* Finish xctx creation. */
    afw_xctx_internal_create_finishup(xctx);

    /* Return self. */
    return self;
}



AFW_DEFINE(const afw_value_t **)
afw_xctx_scope_symbol_get_value_address(
    const afw_value_block_symbol_t *symbol,
    const afw_xctx_scope_t *scope,
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
            "symbol " AFW_UTF8_FMT_Q
            " not found in current scope chain",
            AFW_UTF8_FMT_ARG(&symbol->name->internal));
    }

    if (symbol->index >= scope->block->symbol_count) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol " AFW_UTF8_FMT_Q
            " index " AFW_SIZE_T_FMT
            " is out of range for scope",
            AFW_UTF8_FMT_ARG(&symbol->name->internal), symbol->index);
    }

    return (const afw_value_t **)&scope->frame_slots[symbol->index];
}


AFW_DEFINE(const afw_value_t **)
afw_xctx_scope_symbol_get_value_address_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx)
{
    const afw_xctx_scope_t *scope;
    const afw_value_block_t *block;
    const afw_value_block_symbol_t *symbol;

    for (scope = afw_xctx_scope_current(xctx);
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




/* Get the value of a symbol in the current scope chain. */
AFW_DEFINE(const afw_value_t *)
afw_xctx_scope_symbol_get_value(
    const afw_value_block_symbol_t *symbol,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;

    value_address = afw_xctx_scope_symbol_get_value_address(
        symbol, afw_xctx_scope_current(xctx), xctx);

    return *value_address;
}



/* Get the value of a named symbol in the current scope chain. */
AFW_DEFINE(const afw_value_t *)
afw_xctx_scope_symbol_get_value_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;

    value_address = afw_xctx_scope_symbol_get_value_address_by_name(
        symbol_name, xctx);

    if (!value_address) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol name " AFW_UTF8_FMT_Q
            " not found in current scope chain",
            AFW_UTF8_FMT_ARG(symbol_name));
    }

    return *value_address;
}



/* True if lexical symbol is bound (any slot value, including C NULL). */
AFW_DECLARE(afw_boolean_t)
afw_xctx_scope_symbol_exists_by_name(
    const afw_utf8_t *symbol_name,
    afw_xctx_t *xctx)
{
    return afw_xctx_scope_symbol_get_value_address_by_name(
        symbol_name, xctx) != NULL;
}


/*  Set the value of a symbol in the current scope chain. */
AFW_DEFINE(void)
afw_xctx_scope_symbol_set_value(
    const afw_value_block_symbol_t *symbol,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;

    value_address = afw_xctx_scope_symbol_get_value_address(
        symbol, afw_xctx_scope_current(xctx), xctx);

    /*
     * Prefer permanent undefined singleton over C NULL in slots so "bound
     * with undefined" is never confused with "not applicable" at the pointer
     * level (issue #131). let without initializer and nullish assigns land here.
     */
    afw_value_slot_store(value_address, value, xctx);
}



/* Set the value of a named symbol in the current scope chain. */
AFW_DEFINE(void)
afw_xctx_scope_symbol_set_value_by_name(
    const afw_utf8_t *symbol_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_t **value_address;

    value_address = afw_xctx_scope_symbol_get_value_address_by_name(
        symbol_name, xctx);

    if (!value_address) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol name " AFW_UTF8_FMT_Q
            " not found in current scope chain",
            AFW_UTF8_FMT_ARG(symbol_name));
    }

    afw_value_slot_store(value_address, value, xctx);
}



/*
 * Get optionally qualified variable value.
 *
 * Unqualified: return slot contents (*address). Bound empty values are the
 * undefined singleton after scope create / set_value; use address/exists
 * helpers for “is bound?” (issue #131).
 *
 * Qualified: get_cb contract — non-NULL including undefined/null singletons
 * means defined on that frame; C NULL means not on this frame.
 */
AFW_DEFINE(const afw_value_t *)
afw_xctx_get_optionally_qualified_variable(
    const afw_utf8_t *qualifier,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_t * result;
    const afw_xctx_qualifier_stack_entry_t * e_cur;
    const afw_value_t **value_address;

    if (!qualifier || (qualifier->len == 0)) {
        /* Lexical: NULL only if name not bound (slots start as undefined). */
        value_address = afw_xctx_scope_symbol_get_value_address_by_name(
            name, xctx);
        if (value_address) {
            return *value_address;
        }
        else {
            return NULL;
        }
    }

    /*
     * Walk matching frames newest → oldest. First non-NULL get_cb result wins
     * (including permanent singletons afw_value_undefined / afw_value_null).
     * C NULL from get_cb means "not defined on this frame" — keep looking.
     * See afw_xctx_get_variable_cb_t contract.
     */
    for (
        result = NULL,
        e_cur = xctx->qualifier_stack->top;
        e_cur >= xctx->qualifier_stack->first;
        e_cur--)
    {
        if (!e_cur->get_cb) {
            continue;
        }

        if (!afw_utf8_equal(qualifier, &e_cur->qualifier)) {
            continue;
        }

        if (!e_cur->secure && xctx->secure) {
            continue;
        }

        result = e_cur->get_cb(e_cur, name, xctx);
        if (result) {
            break;
        }
    }

    /* Return result (NULL if no frame defined the name). */
    return result;
}



/* Get stack top. */
AFW_DEFINE(int)
afw_xctx_qualifier_stack_top_get(
    afw_xctx_t *xctx)
{
    return (int)(xctx->qualifier_stack->top - xctx->qualifier_stack->first);
}



/* Set qualifier stack top. */
AFW_DEFINE(void)
afw_xctx_qualifier_stack_top_set(
    int top, afw_xctx_t *xctx)
{
    ((afw_xctx_qualifier_stack_t *)xctx->qualifier_stack)->top =
        xctx->qualifier_stack->first + top;
}



/* Push qualifiers object on to stack. */
AFW_DEFINE(void)
afw_xctx_qualifier_stack_qualifiers_object_push(
    const afw_object_t *context_object,
    afw_boolean_t secure,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_value_t *qualifier_name;
    const afw_object_t *qualifier_object;

    for (iterator = NULL;
        (qualifier_object = afw_object_get_next_property_as_object_internal(
            context_object, &iterator, &qualifier_name, xctx)); )
    {
        afw_xctx_qualifier_stack_qualifier_object_push(
            afw_object_string_property_name_internal(qualifier_name, xctx),
            qualifier_object,
            secure, p, xctx);
    }
}



/* Push qualifier on to stack. */
AFW_DEFINE(afw_xctx_qualifier_stack_entry_t *)
afw_xctx_qualifier_stack_qualifier_push(
    const afw_utf8_t *qualifier,
    const afw_object_t *qualifier_object,
    afw_boolean_t secure,
    afw_xctx_get_variable_cb_t get_cb,
    afw_xctx_contribute_variables_cb_t contribute_cb,
    void * data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_stack_entry_t *entry;

    if (!qualifier || qualifier->len == 0) {
        AFW_THROW_ERROR_Z(general, "Qualifier required", xctx);
    }
    if (!get_cb) {
        AFW_THROW_ERROR_Z(general, "get_cb required", xctx);
    }
    if (!contribute_cb) {
        AFW_THROW_ERROR_Z(general, "contribute_cb required", xctx);
    }

    afw_stack_push_and_get_entry(
        (afw_xctx_qualifier_stack_t *)xctx->qualifier_stack, entry, xctx);

    memset(entry, 0, sizeof(afw_xctx_qualifier_stack_entry_t));
    entry->p = p;
    memcpy(&entry->qualifier, qualifier, sizeof(afw_utf8_t));
    entry->qualifier_object = qualifier_object;
    entry->get_cb = get_cb;
    entry->contribute_cb = contribute_cb;
    entry->data = data;
    entry->secure = secure;

    return entry;
}



static const afw_value_t *
impl_get_object_variable_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    /*
     * Object-backed frame: missing property → C NULL (not this frame).
     * Present null/undefined properties should be stored as afw_value_null /
     * afw_value_undefined (or other values), not omitted.
     */
    const afw_value_string_t name_value = AFW_VALUE_STRING_UNMANAGED(name);

    return afw_object_get_property(entry->qualifier_object,
        &name_value.pub, xctx);
}


/*
 * Fixed contribute for object-push frames: walk qualifier_object and set
 * missing names on the accumulator using the iterated name+value (same values
 * as get_property / get_cb for that object; avoids a redundant re-get).
 */
static void
impl_contribute_object_variables_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_object_t *object,
    afw_boolean_t include_untrusted,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_value_t *property_name;
    const afw_value_t *value;

    (void)include_untrusted;

    if (!entry->qualifier_object) {
        AFW_THROW_ERROR_Z(general,
            "object-push contribute_cb requires qualifier_object", xctx);
    }

    iterator = NULL;
    while ((value = afw_object_get_next_property(
        entry->qualifier_object, &iterator, &property_name, xctx)))
    {
        if (!property_name ||
            afw_object_has_property(object, property_name, xctx))
        {
            continue;
        }
        afw_object_set_property(object, property_name, value, xctx);
    }
}


/* Push qualifier object on to stack. */
AFW_DEFINE(void)
afw_xctx_qualifier_stack_qualifier_object_push(
    const afw_utf8_t *qualifier_name,
    const afw_object_t *qualifier_object,
    afw_boolean_t secure,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_xctx_qualifier_stack_entry_t *entry;

    afw_stack_push_and_get_entry(
        (afw_xctx_qualifier_stack_t *)xctx->qualifier_stack, entry, xctx);
    afw_memory_clear(entry);
    entry->p = p;
    if (qualifier_name) {
        memcpy(&entry->qualifier, qualifier_name, sizeof(afw_utf8_t));
    }
    entry->qualifier_object = qualifier_object;
    entry->get_cb = impl_get_object_variable_cb;
    entry->contribute_cb = impl_contribute_object_variables_cb;
    entry->secure = secure;
}



/* Release an Adaptive Framework xctx. */
AFW_DEFINE(void)
afw_xctx_release(
    const afw_xctx_t *instance,
    afw_xctx_t *xctx)
{  
    /* Release streams. */
    afw_stream_internal_release_all_streams(xctx);

    /* Release xctx's pool. */
    if (instance->p) {
        afw_pool_destroy(instance->p, xctx);
    }
}



//#define AFW_XCTX_SCOPE_DEBUG

#ifdef AFW_XCTX_SCOPE_DEBUG
#define afw_xctx_scope_debug(place, block, scope, parent_scope, note, xctx) \
    impl_scope_debug(place, block, scope, parent_scope, note, xctx)
#else
#define afw_xctx_scope_debug(place, block, scope, parent_scope, note, xctx)
#endif

#ifdef AFW_XCTX_SCOPE_DEBUG
static void impl_scope_debug(
    const afw_utf8_z_t *place,   
    const afw_value_block_t *block,
    const afw_xctx_scope_t *scope,
    const afw_xctx_scope_t *parent_scope,
    const afw_utf8_z_t *note,
    afw_xctx_t *xctx)
{
    const afw_xctx_scope_t *current_scope;

    current_scope = afw_xctx_scope_current(xctx);
    printf("\n");
    printf("--- debug: " AFW_SIZE_T_FMT " %s",
        (scope)
            ? scope->scope_number
            : xctx->scope_count + 1 /* Will be number for new scope. */,
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
        ", active scope count: %d",
        xctx->scope_count, xctx->scope_stack->nelts);

    if (note) {
        printf(" %s", note);
    }

    printf("\n");

    // afw_pool_print_debug_info(8, xctx->p, xctx);
    // printf("\n");

}

#endif


/* Begin begin a scope */
AFW_DEFINE(const afw_xctx_scope_t *)
afw_xctx_scope_create(
    const afw_value_block_t *block,
    const afw_xctx_scope_t *parent_lexical_scope,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    afw_xctx_scope_t *scope;

    if (!block) {
        AFW_THROW_ERROR_Z(general,
            "afw_xctx_scope_create(): block required",
            xctx);
    }

    if (parent_lexical_scope) {
        /*
         * Live parent frame is parent_scope_block (0-symbol `{ }` skipped
         * at compile). Clones share that block pointer.
         */
        if (parent_lexical_scope->block != block->parent_scope_block) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "afw_xctx_scope_create(): parent_lexical_scope block is "
                "not parent_scope_block "
                "(scope count: " AFW_SIZE_T_FMT
                ", active scopes: %d"
                ", parent scope number: " AFW_SIZE_T_FMT
                ", parent scope_depth: " AFW_SIZE_T_FMT
                ", block scope_depth: " AFW_SIZE_T_FMT ")",
                xctx->scope_count, xctx->scope_stack->nelts,
                parent_lexical_scope->scope_number,
                parent_lexical_scope->block->scope_depth,
                block->scope_depth);
        }
    }
    else if (block->parent_scope_block || block->scope_depth != 0) {
        AFW_THROW_ERROR_Z(general,
            "afw_xctx_scope_create(): parent_lexical_scope required "
            "unless this is the top frame",
            xctx);
    }
    
    if (!xctx->evaluation_heap) {
        xctx->evaluation_heap = afw_pool_create_xctx_p(xctx->p, xctx);
    }
    p = afw_pool_tracker_create(xctx->evaluation_heap, xctx);
    scope = afw_pool_calloc(p,
        (
            sizeof(afw_xctx_scope_t) + // Size of struct.
            + (sizeof(afw_value_t *) * block->symbol_count ) // frame_slots[]
            - sizeof(afw_value_t *) // To account for the one in the struct.
        ),
        xctx);
    scope->p = p;
    scope->block = block;
    scope->reference_count = 1;
    xctx->scope_count++;
    scope->scope_number = xctx->scope_count;

    /*
     * Bound symbols start as Adaptive undefined (singleton), not C NULL, so
     * a frame_slots[] entry is never "not a value pointer" while the name is
     * bound (issue #131). set_value also coerces NULL → undefined.
     */
    {
        afw_size_t i;

        for (i = 0; i < block->symbol_count; i++) {
            scope->frame_slots[i] = afw_value_undefined;
        }
    }

    /* If there is a parent_lexical_scope, update its reference count. */
    if (parent_lexical_scope) {
        scope->parent_lexical_scope = parent_lexical_scope;
        ((afw_xctx_scope_t *)parent_lexical_scope)->reference_count++;
    }

    afw_xctx_scope_debug(
        "*  afw_xctx_scope_create()",
        block, scope, parent_lexical_scope, NULL, xctx);

    return scope;
}



/* Live scope for this block's frame (parent_scope_block if none). */
AFW_DEFINE(const afw_xctx_scope_t *)
afw_xctx_scope_find_for_block(
    const afw_value_block_t *block,
    const afw_xctx_scope_t *from,
    afw_xctx_t *xctx)
{
    const afw_value_block_t *frame;
    const afw_xctx_scope_t *scope;

    (void)xctx;

    if (!from) {
        return NULL;
    }

    /* NULL block: compiled-value root. */
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



/* Begin clone a scope */
AFW_DEFINE(const afw_xctx_scope_t *)
afw_xctx_scope_clone(
    const afw_xctx_scope_t *original_scope,
    afw_xctx_t *xctx)
{
    afw_xctx_scope_t *scope;

    scope = (afw_xctx_scope_t *)afw_xctx_scope_create(
        original_scope->block, original_scope->parent_lexical_scope, xctx);

    /* Reference each original frame_slots[]; hidden result is not copied. */
    for (afw_size_t i = 0; i < scope->block->symbol_count; i++) {
        afw_value_slot_store(&scope->frame_slots[i],
            original_scope->frame_slots[i], xctx);
    }

    afw_xctx_scope_debug(
        "*c afw_xctx_scope_clone()",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    return scope;        
}



/* Activate a scope. */
AFW_DEFINE(void)
afw_xctx_scope_activate(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{
    ((afw_xctx_scope_t *)scope)->reference_count++;
    APR_ARRAY_PUSH(xctx->scope_stack, const afw_xctx_scope_t *) = scope;

    afw_xctx_scope_debug(
        "-> afw_xctx_scope_activate()",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);
}



/* Add a reference to a scope. */
AFW_DEFINE(const afw_xctx_scope_t *)
afw_xctx_scope_get_reference(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{
    ((afw_xctx_scope_t *)scope)->reference_count++;

    afw_xctx_scope_debug(
        "+1 afw_xctx_scope_get_reference()",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    return scope;
}



/* Deactivate a scope. */
AFW_DEFINE(void)
afw_xctx_scope_deactivate(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{ 
    afw_xctx_scope_debug(
        "<- afw_xctx_scope_deactivate() begin",
        scope->block, scope, scope->parent_lexical_scope,
        (afw_xctx_scope_current(xctx) == scope)
            ? NULL 
            : "- current scope is not scope passed",
        xctx);

    if ((scope) != afw_xctx_scope_current(xctx)) { 
        AFW_THROW_ERROR_Z(general, 
            "Request to deactivate scope that is not current",
            xctx);
    }

    apr_array_pop(xctx->scope_stack);
    afw_xctx_scope_release(scope, xctx);
}



/* Unwind the scope stack down to but not including the specified scope. */
AFW_DEFINE(void)
afw_xctx_scope_unwind(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{
    const afw_xctx_scope_t *current_scope;

    afw_xctx_scope_debug(
        "-n afw_xctx_scope_unwind() begin",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);
 
    for (;;) {
        current_scope = afw_xctx_scope_current(xctx);
        if (!current_scope) {
            AFW_THROW_ERROR_Z(general,
                "afw_xctx_scope_unwind() did not find specified scope",
                xctx);
        }
        if (scope == current_scope) {
            break;
        }
        afw_xctx_scope_deactivate(current_scope, xctx);
    }
}



/* Set the current scope. */
AFW_DEFINE(void)
afw_xctx_scope_release(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{    
    afw_xctx_scope_debug(
        "-1 afw_xctx_scope_release() begin",
        scope->block, scope, scope->parent_lexical_scope, NULL, xctx);

    /* Already gone (re-enter from a closure in frame_slots[]). */
    if (scope->reference_count == 0) {
        return;
    }

    ((afw_xctx_scope_t *)scope)->reference_count--;
    if (scope->reference_count > 0) {
        return;
    }

    /* Last release: walk frame_slots[], parent lexical, then this pool. */
    if (scope->block) {
        for (afw_size_t i = 0; i < scope->block->symbol_count; i++) {
            afw_value_release(scope->frame_slots[i], xctx);
            ((afw_xctx_scope_t *)scope)->frame_slots[i] =
                afw_value_undefined;
        }
    }
    if (scope->parent_lexical_scope) {
        afw_xctx_scope_release(scope->parent_lexical_scope, xctx);
    }
    afw_pool_release(scope->p, xctx);
}


/*
 * Parked parameter occupants are defined_and_evaluated (or a leftover
 * return-temp wrapper). Call frames are graph infs and are not.
 */
AFW_DEFINE(afw_boolean_t)
afw_xctx_evaluation_stack_is_parked_occupant(const afw_value_t *v)
{
    if (!v ||
        ((afw_size_t)v <= 4096) ||
        (((afw_size_t)v) & (sizeof(void *) - 1)) != 0)
    {
        return false;
    }
    if (afw_value_is_function_return_value(v)) {
        return true;
    }
    /* Extra holds have optional_release. Graph calls on the stack do not. */
    if (!v->inf || !v->inf->optional_release) {
        return false;
    }
    return afw_value_is_defined_and_evaluated(v);
}


/*
 * Pop a VALUE, releasing parked occupant holds. Skip leftover
 * parameter-number pairs so a number slot is never used as a value
 * pointer.
 */
AFW_DEFINE(void)
afw_xctx_evaluation_stack_pop_value_impl(afw_xctx_t *xctx)
{
    afw_xctx_evaluation_stack_t *stack;
    const afw_value_t *v;

    stack = xctx->evaluation_stack;
    while (!afw_stack_is_empty(stack)) {
        if (stack->top->entry_id == afw_s_parameter_number) {
            afw_stack_pop(stack, xctx);
            if (!afw_stack_is_empty(stack)) {
                afw_stack_pop(stack, xctx);
            }
            continue;
        }
        v = stack->top->value;
        if (afw_xctx_evaluation_stack_is_parked_occupant(v)) {
            afw_value_release(v, xctx);
            afw_stack_pop(stack, xctx);
            continue;
        }
        break;
    }
    if (!afw_stack_is_empty(stack)) {
        afw_stack_pop(stack, xctx);
    }
}


/*
 * Rewind evaluation stack to saved_top. Release parked occupant holds.
 * Skip parameter-number pairs without treating the number as a value
 * pointer.
 */
AFW_DEFINE(void)
afw_xctx_evaluation_stack_rewind(
    afw_xctx_evaluation_stack_entry_t *saved_top,
    afw_xctx_t *xctx)
{
    afw_xctx_evaluation_stack_t *stack;
    const afw_value_t *v;

    stack = xctx->evaluation_stack;
    while (stack->top > saved_top && stack->top >= stack->first) {
        if (stack->top->entry_id == afw_s_parameter_number) {
            stack->top--;
            if (stack->top > saved_top && stack->top >= stack->first) {
                stack->top--;
            }
            continue;
        }
        v = stack->top->value;
        if (afw_xctx_evaluation_stack_is_parked_occupant(v)) {
            afw_value_release(v, xctx);
        }
        stack->top--;
    }
    stack->top = saved_top;
}


/* Assign into the current hidden result slot. */
AFW_DEFINE(void)
afw_xctx_script_result_set_value(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (!value || afw_value_is_void(value)) {
        return;
    }
    afw_value_slot_store(&xctx->script_result, value, xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_xctx interface
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_xctx.c
 * @brief Implementation of afw_xctx interface.
 */

#include "afw_internal.h"


/* Declares and rti/inf defines for interface afw_xctx */
#define AFW_IMPLEMENTATION_ID "core"
#include "afw_xctx_impl_declares.h"
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
    self->inf = &impl_afw_xctx_inf;
    self->p = p;
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
    xctx->properties = afw_object_create(
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
    p = afw_pool_create(xctx->p, xctx);
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
         scope && scope->block->depth > symbol->parent_block->depth;
         scope = scope->parent_lexical_scope);

    if (!scope ||
        scope->block->depth != symbol->parent_block->depth)
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol " AFW_UTF8_FMT_Q
            " not found in current scope chain",
            AFW_UTF8_FMT_ARG(symbol->name));
    }

    if (symbol->index >= scope->block->symbol_count) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "symbol " AFW_UTF8_FMT_Q
            " index " AFW_SIZE_T_FMT
            " is out of range for scope",
            AFW_UTF8_FMT_ARG(symbol->name), symbol->index);
    }

    return (const afw_value_t **)&scope->symbol_values[symbol->index];   
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
            if (afw_utf8_equal(symbol_name, symbol->name)) {
                return (const afw_value_t **)
                &scope->symbol_values[symbol->index];
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



/* Determine if the named symbol exists in the current scope chain. */
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

    *value_address = value;
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

    *value_address = value;
}



/* Get a variable from xctx stack. */
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
        value_address = afw_xctx_scope_symbol_get_value_address_by_name(
            name, xctx);
        if (value_address) {
            return *value_address;
        }
        else {
            return NULL;
        }
    }

    for (
        result = NULL,
        e_cur = xctx->qualifier_stack->top;
        e_cur >= xctx->qualifier_stack->first;
        e_cur--)
    {
        if (!e_cur->get) {
            continue;
        }

        if (!afw_utf8_equal(qualifier, &e_cur->qualifier)) {
            continue;
        }

        if (!e_cur->secure && xctx->secure) {
            continue;
        }

        result = e_cur->get(e_cur, name, xctx);
        break;
    }

    /* Return result. */
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
    const afw_iterator_t *iterator;
    const afw_utf8_t *qualifier_name;
    const afw_object_t *qualifier_object;

    for (iterator = NULL;
        (qualifier_object = afw_object_old_get_next_property_as_object(
            context_object, &iterator, &qualifier_name, xctx)); )
    {
        afw_xctx_qualifier_stack_qualifier_object_push(qualifier_name, qualifier_object,
            secure, p, xctx);
    }
}



/* Push qualifier on to stack. */
AFW_DEFINE(afw_xctx_qualifier_stack_entry_t *)
afw_xctx_qualifier_stack_qualifier_push(
    const afw_utf8_t *qualifier,
    const afw_object_t *qualifier_object,
    afw_boolean_t secure,
    afw_xctx_get_variable_t get,
    void * data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /*! \fixme add support for qualifier object. */
    afw_xctx_qualifier_stack_entry_t *entry;

    if (!qualifier || qualifier->len == 0) {
        AFW_THROW_ERROR_Z(general, "Qualifier required", xctx);
    }

    afw_stack_push_and_get_entry(
        (afw_xctx_qualifier_stack_t *)xctx->qualifier_stack, entry, xctx);

    memset(entry, 0, sizeof(afw_xctx_qualifier_stack_entry_t));
    entry->p = p;
    if (qualifier) {
        memcpy(&entry->qualifier, qualifier,
            sizeof(afw_xctx_qualifier_stack_entry_t));
    }
    entry->qualifier_object = qualifier_object;
    entry->get = get;
    entry->data = data;
    entry->secure = secure;
    
    return entry;
}



static const afw_value_t *
impl_get_object_variable(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    /* Return property from qualifier_object as variable. */
    return afw_object_get_property(entry->qualifier_object, name, xctx);
}


/* Push qualifier on to stack. */
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
    entry->get = impl_get_object_variable;
    entry->secure = secure;
}



/*
 * Implementation of method release of interface afw_xctx.
 */
void
impl_afw_xctx_release(
    const afw_xctx_t * instance,
    afw_xctx_t *xctx)
{  
    /* Release streams. */
    afw_stream_internal_release_all_streams(xctx);

    /* Release xctx's pool. */
    if (instance->p) {
        // printf("\nxctx release\n");
        // afw_pool_print_debug_info(0, instance->p, xctx);
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
            ", pool number: " AFW_INTEGER_FMT
            ", scope number: " AFW_SIZE_T_FMT
            ", refs: " AFW_SIZE_T_FMT,
            ((afw_pool_internal_singlethreaded_self_t *)scope->p)->pool_number,
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
        if (parent_lexical_scope->block->depth != block->depth - 1) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "afw_xctx_scope_create(): parent_lexical_scope block depth must "
                "be one less than block depth "
                "(scope count: " AFW_SIZE_T_FMT
                ", active scopes: %d"
                ", parent scope number: " AFW_SIZE_T_FMT 
                ", parent block depth: " AFW_SIZE_T_FMT
                ", block depth: " AFW_SIZE_T_FMT ")",
                xctx->scope_count, xctx->scope_stack->nelts,
                parent_lexical_scope->scope_number,
                parent_lexical_scope->block->depth,
                block->depth);
        }
    }
    else if (block->depth != 0) {
        AFW_THROW_ERROR_Z(general,
            "afw_xctx_scope_create(): block depth must be zero if "
            "parent_lexical_scope is NULL",
            xctx);
    }
    
    p = afw_pool_create(xctx->p, xctx);
    scope = afw_pool_calloc(p,
        (
            sizeof(afw_xctx_scope_t) + // Size of struct.
            + (sizeof(afw_value_t *) * block->symbol_count ) // symbol_values[]
            - sizeof(afw_value_t *) // To account for the one in the struct.
        ),
        xctx);
    scope->p = p;
    scope->block = block;
    xctx->scope_count++;
    scope->scope_number = xctx->scope_count;

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



/* Begin clone a scope */
AFW_DEFINE(const afw_xctx_scope_t *)
afw_xctx_scope_clone(
    const afw_xctx_scope_t *original_scope,
    afw_xctx_t *xctx)
{
    afw_xctx_scope_t *scope;

    scope = (afw_xctx_scope_t *)afw_xctx_scope_create(
        original_scope->block, original_scope->parent_lexical_scope, xctx);

    /* Copy entries. */
    for (afw_size_t i = 0; i < scope->block->symbol_count; i++) {
        /** @fixme change these to value references when that's done. */
        scope->symbol_values[i] = original_scope->symbol_values[i];
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
afw_xctx_scope_add_reference(
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{
    ((afw_xctx_scope_t *)scope)->reference_count++;

    afw_xctx_scope_debug(
        "+1 afw_xctx_scope_add_reference()",
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

    /*
     * If never activated or reference count is 1, release reference to parent
     * lexical scope and free this scope's pool.
     */
    if (scope->reference_count <= 1) {
        if (scope->parent_lexical_scope) {
            afw_xctx_scope_release(scope->parent_lexical_scope, xctx);
        }
        afw_pool_release(scope->p, xctx);
        return;
    }

    /* Otherwise, just decrement reference count. */
    ((afw_xctx_scope_t *)scope)->reference_count--;
}


/* Set the xctx evaluation result. */
AFW_DEFINE(void)
afw_xctx_evaluation_result_set(
    afw_xctx_t *xctx,
    const afw_value_t *value)
{
    /** @fixme Change to use afw_value_get_reference() when that is in place. */
    ((afw_xctx_t *)xctx)->evaluation_result = value;
}

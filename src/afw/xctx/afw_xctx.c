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



static void
impl_set_qualifier_stack(afw_xctx_t *xctx)
{
    /*
     * Fixed size: entry pointers stay valid. Early xctx create cannot
     * use AFW_TRY / throwing afw_pool_calloc().
     */
    xctx->qualifier_stack = afw_vector_create_fixed_unhandled(
        afw_xctx_qualifier_stack_t, 100, xctx->p, xctx);
}


/* Extra eval-stack slots so Adaptive catch can run after the tripwire. */
#define IMPL_EVAL_STACK_ERROR_HEADROOM ((afw_size_t)64)

static void
impl_set_evaluation_stack(afw_xctx_t *xctx)
{
    afw_size_t n;

    /*
     * Allocate the cap up front so the vector never grows (entry
     * pointers stay valid). Early xctx create cannot use AFW_TRY.
     * Extra slots are for the error path; the tripwire uses the
     * published limit.
     */
    n = xctx->env->limit_evaluation_stack_count;
    if (n == 0) {
        xctx->evaluation_stack = afw_vector_create(
            afw_xctx_evaluation_stack_t, 64, xctx->p, xctx);
        return;
    }
    if (n > AFW_SIZE_T_MAX - IMPL_EVAL_STACK_ERROR_HEADROOM) {
        n = AFW_SIZE_T_MAX;
    }
    else {
        n += IMPL_EVAL_STACK_ERROR_HEADROOM;
    }
    xctx->evaluation_stack = afw_vector_create_fixed_unhandled(
        afw_xctx_evaluation_stack_t, n, xctx->p, xctx);
}


afw_xctx_t *
afw_xctx_internal_create_initialize(
    afw_try_t *unhandled_error,
    afw_error_t *error,
    afw_environment_internal_t *env,
    const afw_pool_t *p)
{
    afw_xctx_t *self;

    if (!error) {
        /* No xctx yet; cannot AFW_THROW. */
        error = afw_pool_internal_calloc_unhandled(p, sizeof(afw_error_t), NULL);
        if (!error) {
            return NULL;
        }
    }

    /* Initialize self. evaluation_stack is not ready; no AFW_TRY. */
    self = afw_pool_internal_calloc_unhandled(p, sizeof(afw_xctx_t), NULL);
    if (!self) {
        AFW_THROW_UNHANDLED_ERROR(unhandled_error, error, general,
            na, 0, "afw_pool_internal_calloc_unhandled() failed");
    }
    self->p = p;
    self->script_result = afw_value_undefined;
    self->mode = afw_authorization_mode_id_user_value;
    self->current_try = unhandled_error;
    self->error = error;
    self->env = (const afw_environment_t *)env;
    error->xctx = self;
    self->flags_count = env->flags_count_allocated;
    self->flags = (afw_boolean_t *)env->default_flags;
    /*! \fixme stream_anchor may be too early??? */
    self->stream_anchor = afw_stream_internal_stream_anchor_create(self);
    if (!self->stream_anchor) {
        AFW_THROW_UNHANDLED_ERROR(unhandled_error, error, memory,
            na, 0, "allocation failed");
    }

    /*
     * Fixed vector: xctx init cannot use AFW_TRY / throwing calloc.
     * Cap matches evaluation stack so nested scopes cannot outrun eval.
     */
    {
        afw_size_t n;

        n = env->limit_evaluation_stack_count;
        self->scope_stack = afw_vector_create_fixed_unhandled(
            afw_pool_scope_p_vector_t, n, p, self);
    }

    /*
     * Set libxml2 error func to suppress error print. Use
     * xmlGetLastError() instead.
     */
    self->libxml2_error_func = (void *)impl_suppress_libxml2_message;
    initGenericErrorDefaultFunc((xmlGenericErrorFunc *)&self->libxml2_error_func);

    /* Make qualifier and evaluation stacks (fixed vectors). */
    impl_set_qualifier_stack(self);
    impl_set_evaluation_stack(self);

    /* Return new xctx. */
    return self;
}


void
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

    /* Snapshot thread currents after this xctx's thread is set. */
    if (xctx->thread) {
        xctx->snap_pool_bytes_in_use =
            xctx->thread->pool_bytes_in_use;
        xctx->snap_pool_chunk_bytes =
            xctx->thread->pool_chunk_bytes;
    }
}


static afw_size_t
impl_c_stack_remaining(const afw_thread_t *thread)
{
    const char *sp;
    const char *low;
    const char *high;
    char probe;

    if (!thread || !thread->c_stack_base || thread->c_stack_size == 0) {
        return AFW_SIZE_T_MAX;
    }
    sp = &probe;
    low = (const char *)thread->c_stack_base;
    high = low + thread->c_stack_size;
    if (sp < low || sp > high) {
        return AFW_SIZE_T_MAX;
    }
    return (afw_size_t)(sp - low);
}


AFW_DEFINE(void)
afw_xctx_check_resource_limits(
    afw_xctx_t *xctx, afw_size_t extra_eval_slots)
{
    const afw_environment_t *env;
    const afw_thread_t *thread;
    afw_size_t limit;
    afw_size_t remaining;

    if (!xctx || !xctx->env) {
        return;
    }
    if (xctx->error_processing_count > 0) {
        return;
    }
    env = xctx->env;

    limit = env->limit_evaluation_stack_count;
    if (extra_eval_slots != 0 && limit != 0 &&
        xctx->evaluation_stack &&
        xctx->evaluation_stack->count + extra_eval_slots > limit)
    {
        AFW_THROW_ERROR_Z(payload_too_large,
            "Evaluation stack limit exceeded.", xctx);
    }

    thread = xctx->thread;
    if (!thread) {
        return;
    }

    limit = env->limit_request_pool_bytes;
    if (limit != 0 &&
        thread->pool_bytes_in_use >= limit &&
        (thread->type == afw_thread_type_request ||
            (env->limit_request_pool_apply_to_base &&
                xctx != ((const afw_environment_internal_t *)env)->
                    base_xctx)))
    {
        AFW_THROW_ERROR_Z(payload_too_large,
            "Request pool limit exceeded.", xctx);
    }

    limit = env->limit_c_stack_headroom_bytes;
    if (limit != 0) {
        remaining = impl_c_stack_remaining(thread);
        if (remaining < limit) {
            AFW_THROW_ERROR_Z(payload_too_large,
                "C stack headroom exhausted.", xctx);
        }
    }
}


afw_xctx_t *
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
    p = afw_pool_heap_create_as_managed_p(xctx->p,
        xctx->env->xctx_chunk_min, xctx);
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
        value_address = afw_pool_scope_symbol_get_value_address_by_name(
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
        e_cur = xctx->qualifier_stack->count
            ? &xctx->qualifier_stack->entries[
                xctx->qualifier_stack->count - 1]
            : NULL;
        e_cur && e_cur >= xctx->qualifier_stack->entries;
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
    return (int)xctx->qualifier_stack->count - 1;
}



/* Set qualifier stack top. */
AFW_DEFINE(void)
afw_xctx_qualifier_stack_top_set(
    int top, afw_xctx_t *xctx)
{
    ((afw_xctx_qualifier_stack_t *)xctx->qualifier_stack)->count =
        (afw_size_t)(top + 1);
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

    afw_vector_push_index_impl(
        &((afw_xctx_qualifier_stack_t *)xctx->qualifier_stack)->internal,
        xctx);
    entry = &((afw_xctx_qualifier_stack_t *)xctx->qualifier_stack)->entries[
        xctx->qualifier_stack->count - 1];

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
    const afw_value_t *result;

    result = afw_object_get_property(entry->qualifier_object,
        &name_value.pub, xctx);
    /*
     * This get_cb is shared (app:: templates, current:: runtime objects,
     * adapter:: conf, …). Only compiled units should run here. Evaluate of
     * a runtime current:: property is not always identity.
     */
    if (afw_value_is_compiled_value(result)) {
        result = afw_value_evaluate(result, xctx->p, xctx);
    }
    return result;
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
        if (afw_value_is_compiled_value(value)) {
            value = afw_value_evaluate(value, xctx->p, xctx);
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

    afw_vector_push_index_impl(
        &((afw_xctx_qualifier_stack_t *)xctx->qualifier_stack)->internal,
        xctx);
    entry = &((afw_xctx_qualifier_stack_t *)xctx->qualifier_stack)->entries[
        xctx->qualifier_stack->count - 1];
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
    /*
     * Streams and callbacks may throw (fclose, cleanup). destroy always
     * frees storage. xctx lives in instance->p; return before AFW_ENDTRY.
     */
    if (instance->p) {
        AFW_TRY {
            afw_error_release_backtrace(xctx->error, xctx);
            afw_stream_internal_release_all_streams(xctx);
            afw_pool_run_cleanups(instance->p, xctx);
        }
        AFW_FINALLY {
            afw_os_backtrace_cleanup((afw_xctx_t *)instance);
            afw_pool_destroy(instance->p, xctx);
            return;
        }
        AFW_ENDTRY;
    }
    else {
        afw_os_backtrace_cleanup((afw_xctx_t *)instance);
        afw_stream_internal_release_all_streams(xctx);
    }
}



AFW_DEFINE(void)
afw_xctx_evaluation_stack_pop_value_impl(afw_xctx_t *xctx)
{
    if (xctx->evaluation_stack && xctx->evaluation_stack->count > 0) {
        afw_vector_pop(xctx->evaluation_stack, xctx);
    }
}


AFW_DEFINE(void)
afw_xctx_evaluation_stack_rewind(
    afw_size_t save_count,
    afw_xctx_t *xctx)
{
    if (xctx->evaluation_stack &&
        xctx->evaluation_stack->count > save_count)
    {
        xctx->evaluation_stack->count = save_count;
    }
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
    afw_value_slot_store(&xctx->script_result, value, xctx->p, xctx);
}



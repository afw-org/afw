// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for compiled_value
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_compiled_value.c
 * @brief Implementation of afw_value interface for compiled_value
 */

#include "afw_internal.h"


#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

#define impl_afw_value_create_iterator NULL

/*
 * Two infs. A compiled value is counted: compile returns it at RC 1 and
 * registers that release on dest p. Last release releases the unit
 * pool it owns, or frees the header when the pool is shared. A unit in
 * a multithreaded pool uses afw_value_compiled_value_multithreaded_inf
 * (end of this file): the same methods with an atomic count.
 */
#define impl_afw_value_get_for_p_lifetime afw_value_counted_get_for_p_lifetime
#define impl_afw_value_for_each_reference afw_value_no_references_for_each
#define impl_afw_value_release_references afw_value_no_references_release_references
#define impl_afw_value_get_counted afw_value_self_get_counted

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "compiled_value"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_compiled_value_inf
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    NULL, \
    NULL, \
    true
#define AFW_VALUE_SELF_T afw_value_compiled_value_t
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_INF_VARIABLES
#undef impl_afw_value_create_iterator
#undef impl_afw_value_get_evaluated_meta
#undef impl_afw_value_get_evaluated_metas
#undef impl_afw_value_get_for_p_lifetime
#undef impl_afw_value_for_each_reference
#undef impl_afw_value_get_counted


void
impl_afw_value_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (self->reference_count <= 0) {
        return;
    }
    self->reference_count--;
    if (self->reference_count != 0) {
        return;
    }
    if (self->unit_owns_p) {
        afw_pool_release(self->p, xctx);
    }
    else {
        afw_pool_free_memory(self->p, self,
            sizeof(afw_value_compiled_value_t), xctx);
    }
}


const afw_value_t *
impl_afw_value_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    self->reference_count++;
    return &self->pub;
}


const afw_value_t *
impl_afw_value_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    return impl_afw_value_get_reference(self, xctx);
}


/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_value_t *saved_script_result;
    afw_size_t count;

    result = NULL;
    count = xctx->scope_stack->count;

    saved_script_result = xctx->script_result;
    xctx->script_result = afw_value_undefined;
    /*
     * Evaluate is caller does not release. Dest p is the p passed
     * to this evaluate. Nested evaluate(compile()) parks
     * script_result. Isolate dest is dest p of the caller that
     * writes the slot (scope->p at deactivate).
     */

    AFW_TRY {

        /* Push a NULL onto the scope stack to indicate new compiled value. */
        afw_vector_push(xctx->scope_stack, xctx) = NULL;

        /*
         * Block root (script, and template/test_script wrapped in a
         * block): as_value false so deactivate isolates last into
         * script_result using dest scope->p. Caller last is
         * already parked.
         */
        if (self->root_value &&
            afw_value_is_block(self->root_value))
        {
            afw_function_execute_t exec;

            exec.p = p;
            exec.xctx = xctx;
            result = afw_value_block_evaluate_block(&exec,
                (const afw_value_block_t *)self->root_value, p, xctx,
                false);
        }
        else {
            result = afw_value_evaluate(self->root_value, p, xctx);
        }

    }
    AFW_FINALLY {
        const afw_value_t *slot;

        /* Pop off the NULL compiled value indicator on scope stack. */
        if (xctx->scope_stack->count != count + 1) {
            xctx->script_result = saved_script_result;
            AFW_THROW_ERROR_Z(general,
                "Scope stack still has active scopes at end after computed "
                "value is evaluated",
                xctx);
        }
        afw_vector_pop(xctx->scope_stack, xctx);

        slot = xctx->script_result;
        if (slot &&
            !afw_value_is_undefined(slot) &&
            !afw_value_is_void(slot))
        {
            result = slot;
        }
        if (!result || afw_value_is_void(result)) {
            result = afw_value_undefined;
        }

        /*
         * Restore before clone/register so a throw in FINALLY cannot
         * leave nested last on the xctx.
         */
        xctx->script_result = saved_script_result;

        /*
         * Caller does not release: get_assignable_value of the result
         * (unmanaged, including unit literals, is copied) with its last
         * release registered on dest p.
         */
        if (result &&
            !afw_value_is_undefined(result) &&
            !afw_value_is_void(result))
        {
            result = afw_pool_scope_get_assignable_for_p_lifetime(
                result, p, xctx);
        }

        /* Drop the script_result slot's own reference. */
        if (slot &&
            slot != saved_script_result &&
            !afw_value_is_undefined(slot) &&
            !afw_value_is_void(slot))
        {
            afw_value_release(slot, xctx);
        }

    }
    AFW_ENDTRY;

    /* Always set execution flow back to sequential after compiled unit. */
    afw_xctx_statement_flow_set_type(sequential, xctx);

    return result;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Compiled values are always data type unevaluated. */
    return &afw_data_type_unevaluated_direct;
}

/*
 * Implementation of method compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *reference_id;

    reference_id = afw_value_compiler_listing_for_child(
        &self->pub, writer, xctx);

    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->contextual, xctx);
    afw_writer_write_z(writer,
        " // See below beginning with: ---CompiledValue ",
        xctx);
    afw_writer_write_utf8(writer, reference_id, xctx);
    afw_writer_write_eol(writer, xctx);
}

/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{

    afw_value_decompile(self->root_value, writer, xctx);
    /*FIXME Improve */
}


/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_clear(info);
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->optimized_value = &self->pub;
}


/*
 * Implementation of method get_reference_count for interface afw_value.
 */
afw_size_t
impl_afw_value_get_reference_count(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    return (afw_size_t)self->reference_count;
}


/*
 * Multithreaded compiled value: a unit compiled into a multithreaded
 * pool (a model's on* script, conf scripts) is evaluated by many request
 * threads at once. Evaluation changes nothing in the unit except its
 * count (a closure references the unit its definition lives in), so
 * only get_reference and release differ: the count is atomic.
 */
static const afw_value_t *
impl_mt_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    afw_atomic_integer_increment(&self->atomic_reference_count);
    return &self->pub;
}


static void
impl_mt_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (afw_atomic_integer_decrement(&self->atomic_reference_count) != 0) {
        return;
    }
    if (self->unit_owns_p) {
        afw_pool_release(self->p, xctx);
    }
    else {
        afw_pool_free_memory(self->p, self,
            sizeof(afw_value_compiled_value_t), xctx);
    }
}


static const afw_value_t *
impl_mt_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    return impl_mt_get_reference(self, xctx);
}


#undef impl_afw_value_release_references
#define impl_afw_value_get_reference impl_mt_get_reference
#define impl_afw_value_release impl_mt_release
#define impl_afw_value_get_assignable_value impl_mt_get_assignable_value
#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default
#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
#define impl_afw_value_create_iterator NULL
#define impl_afw_value_get_for_p_lifetime afw_value_counted_get_for_p_lifetime
#define impl_afw_value_for_each_reference afw_value_no_references_for_each
#define impl_afw_value_release_references afw_value_no_references_release_references
#define impl_afw_value_get_counted afw_value_self_get_counted

#define AFW_VALUE_INF_ONLY 1
#define AFW_IMPLEMENTATION_ID "compiled_value_multithreaded"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_compiled_value_multithreaded_inf
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    NULL, \
    NULL, \
    true
#include "afw_value_impl_declares.h"

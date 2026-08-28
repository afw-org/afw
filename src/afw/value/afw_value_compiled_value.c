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


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL
#define impl_afw_value_get_assignable_value NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "compiled_value"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_compiled_value_inf
#define AFW_VALUE_SELF_T afw_value_compiled_value_t
#include "afw_value_impl_declares.h"


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
    const afw_value_t *occupant;
    const afw_value_t *saved_script_result;
    const afw_pool_t *heap;
    const afw_pool_t *saved_evaluation_heap;
    const afw_pool_t *dest_p;
    afw_boolean_t saved_script_result_active;
    afw_boolean_t saved_script_result_written;
    int nelts;

    result = NULL;
    nelts = xctx->scope_stack->nelts;
    saved_script_result = xctx->script_result;
    saved_script_result_active = xctx->script_result_active;
    saved_script_result_written = xctx->script_result_written;
    saved_evaluation_heap = xctx->evaluation_heap;
    heap = afw_pool_create_xctx_p(p, xctx);
    xctx->evaluation_heap = heap;
    if (self->full_source_type &&
        afw_utf8_equal(self->full_source_type, afw_s_script))
    {
        xctx->script_result = afw_value_undefined;
        xctx->script_result_active = true;
        xctx->script_result_written = false;
    }
    AFW_TRY {

        /* Push a NULL onto the scope stack to indicate new compiled value. */
        APR_ARRAY_PUSH(xctx->scope_stack, const afw_xctx_scope_t *) = NULL;

        /* Evaluate compiled value root value. */
        if (xctx->script_result_active &&
            self->root_value &&
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

        /* Make sure all scopes were released during evaluate. */
        if (xctx->scope_stack->nelts != nelts + 1) {
            AFW_THROW_ERROR_Z(general,
                "Scope stack still has active scopes at end after computed "
                "value is evaluated",
                xctx);
        }

        /* Pop off the NULL compiled value indicator on scope stack. */
        apr_array_pop(xctx->scope_stack);

        /*
         * Escape the callee heap onto the caller's evaluation heap so
         * overwrite can recycle (general xctx->p free is a no-op).
         */
        dest_p = saved_evaluation_heap;
        if (!dest_p || dest_p == heap) {
            dest_p = xctx->p;
        }

        if (xctx->script_result_active &&
            xctx->script_result &&
            !afw_value_is_undefined(xctx->script_result) &&
            !afw_value_is_void(xctx->script_result))
        {
            result = xctx->script_result;
        }
        if (result &&
            !afw_value_is_undefined(result) &&
            !afw_value_is_void(result) &&
            !afw_value_is_function_return_value(result))
        {
            occupant = result;
            result = afw_value_function_return_value_create(
                occupant, dest_p, xctx);
            /*
             * FRV holds the occupant. Drop the running-result slot hold
             * so restore does not need donate.
             */
            if (xctx->script_result == occupant) {
                afw_value_release(occupant, xctx);
                xctx->script_result = result;
            }
        }
        afw_xctx_script_result_restore(
            saved_script_result,
            saved_script_result_active,
            saved_script_result_written,
            xctx);

        xctx->evaluation_heap = saved_evaluation_heap;
        if (heap) {
            afw_pool_release(heap, xctx);
        }
        
    }
    AFW_ENDTRY;

    /* Always set execution flow back to sequential after compiled unit. */
    afw_xctx_statement_flow_set_type(sequential, xctx);
    
    /* Return the result of calling root value's evaluate(). */
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

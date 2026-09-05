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

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "compiled_value"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_compiled_value_inf
#define AFW_VALUE_SELF_T afw_value_compiled_value_t
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_create_iterator
#undef impl_afw_value_get_evaluated_meta
#undef impl_afw_value_get_evaluated_metas


static void
impl_assignable_optional_release(
    afw_value_compiled_value_t *self, afw_xctx_t *xctx);
static const afw_value_t *
impl_assignable_get_reference(
    afw_value_compiled_value_t *self, afw_xctx_t *xctx);
static const afw_value_t *
impl_assignable_get_assignable_value(
    afw_value_compiled_value_t *self, afw_xctx_t *xctx);

#define AFW_IMPLEMENTATION_ID "compiled_value_assignable"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_compiled_value_assignable_inf
#define AFW_VALUE_INF_ONLY
#define impl_afw_value_optional_release impl_assignable_optional_release
#define impl_afw_value_get_reference impl_assignable_get_reference
#define impl_afw_value_get_assignable_value \
    impl_assignable_get_assignable_value
#define impl_afw_value_create_iterator NULL
#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default
#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
#include "afw_value_impl_declares.h"
#undef AFW_VALUE_INF_ONLY
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_optional_release
#undef impl_afw_value_get_reference
#undef impl_afw_value_get_assignable_value
#undef impl_afw_value_create_iterator
#undef impl_afw_value_get_evaluated_meta
#undef impl_afw_value_get_evaluated_metas


void
impl_afw_value_optional_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (self->p) {
        afw_pool_release(self->p, xctx);
    }
}


const afw_value_t *
impl_afw_value_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    AFW_THROW_ERROR_Z(general,
        "get_reference of unmanaged compiled_value", xctx);
}


const afw_value_t *
impl_afw_value_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (!self->p || afw_pool_internal_is_tracker(self->p)) {
        AFW_THROW_ERROR_Z(general,
            "get_assignable_value of compiled_value requires "
            "compile p to be a heap (Adaptive compile uses xctx->p)",
            xctx);
    }
    afw_pool_get_reference(self->p, xctx);
    self->inf = &afw_value_compiled_value_assignable_inf;
    return &self->pub;
}


static void
impl_assignable_optional_release(
    afw_value_compiled_value_t *self,
    afw_xctx_t *xctx)
{
    if (self->p) {
        afw_pool_release(self->p, xctx);
    }
}


static const afw_value_t *
impl_assignable_get_reference(
    afw_value_compiled_value_t *self,
    afw_xctx_t *xctx)
{
    if (self->p) {
        afw_pool_get_reference(self->p, xctx);
    }
    return &self->pub;
}


static const afw_value_t *
impl_assignable_get_assignable_value(
    afw_value_compiled_value_t *self,
    afw_xctx_t *xctx)
{
    return impl_assignable_get_reference(self, xctx);
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
    int nelts;

    result = NULL;
    nelts = xctx->scope_stack->nelts;

    saved_script_result = xctx->script_result;
    xctx->script_result = afw_value_undefined;

    AFW_TRY {

        /* Push a NULL onto the scope stack to indicate new compiled value. */
        APR_ARRAY_PUSH(xctx->scope_stack, const afw_xctx_scope_t *) = NULL;

        /* Evaluate compiled value root value. */
        if (self->full_source_type &&
            afw_utf8_equal(self->full_source_type, afw_s_script) &&
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

        if (xctx->script_result &&
            !afw_value_is_undefined(xctx->script_result) &&
            !afw_value_is_void(xctx->script_result))
        {
            result = xctx->script_result;
        }
        if (!result || afw_value_is_void(result)) {
            result = afw_value_undefined;
        }
        if (result &&
            !afw_value_is_undefined(result) &&
            !afw_value_is_void(result))
        {
            const afw_data_type_t *dt;

            dt = result->inf
                ? result->inf->is_evaluated_of_data_type
                : NULL;
            if (dt && dt->clone_value_unmanaged) {
                result = afw_value_clone_unmanaged(
                    result, p, xctx);
            }
        }

        if (xctx->script_result &&
            xctx->script_result != saved_script_result &&
            xctx->script_result != result &&
            !afw_value_is_undefined(xctx->script_result) &&
            !afw_value_is_void(xctx->script_result))
        {
            afw_value_release(xctx->script_result, xctx);
        }
        xctx->script_result = saved_script_result;

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

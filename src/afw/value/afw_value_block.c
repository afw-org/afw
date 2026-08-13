// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for block
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_block.c
 * @brief Implementation of afw_value interface for block
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "block"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_block_inf
#define AFW_VALUE_SELF_T afw_value_block_t
#include "afw_value_impl_declares.h"

#define IMPL_TEMP_FIX_ASSIGNS(XX) \
    modified_x.p = p; \
    modified_x.xctx = xctx; \
    modified_x.function = &afw_function_definition_ ## XX; \
    modified_x.argv = argv; \
    modified_x.argc = argc; \

#define IMPL_TEMP_FIX(XX) \
    afw_function_execute_t modified_x; \
    afw_memory_clear(&modified_x); \
    IMPL_TEMP_FIX_ASSIGNS(XX) \
    x = &modified_x


/*
 * let/const/control do not yield a script result by themselves. A script
 * that is only a call or #block still yields that value (decompile of
 * `1+2` is #block(add(1,2))).
 */
static afw_boolean_t
impl_statement_is_void_for_script_result(const afw_value_t *statement)
{
    const afw_value_call_built_in_function_t *call;
    const afw_utf8_t *id;

    if (!statement) {
        return true;
    }
    if (!afw_value_is_call_built_in_function(statement)) {
        return false;
    }
    call = (const afw_value_call_built_in_function_t *)statement;
    if (!call->function || !call->function->functionId) {
        return false;
    }
    id = &call->function->functionId->internal;
    return
        afw_utf8_equal(id, afw_s_let) ||
        afw_utf8_equal(id, afw_s_const) ||
        afw_utf8_equal(id, afw_s_if) ||
        afw_utf8_equal(id, afw_s_for) ||
        afw_utf8_equal(id, afw_s_for_of) ||
        afw_utf8_equal(id, afw_s_while) ||
        afw_utf8_equal(id, afw_s_do_while) ||
        afw_utf8_equal(id, afw_s_switch) ||
        afw_utf8_equal(id, afw_s_try) ||
        afw_utf8_equal(id, afw_s_throw) ||
        afw_utf8_equal(id, afw_s_break) ||
        afw_utf8_equal(id, afw_s_continue);
}


const afw_value_t *
afw_value_block_evaluate_block(
    afw_function_execute_t *x,
    const afw_value_block_t *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx,
    afw_boolean_t as_value)
{
    const afw_value_t *result;
    const afw_value_t *last;
    const afw_compile_value_contextual_t *saved_contextual;
    afw_size_t i;
    const afw_xctx_scope_t *scope;
    afw_boolean_t use_running;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(
        (const afw_value_t *)self, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;
    use_running = xctx->script_result_active && !as_value;
    result = use_running
        ? afw_xctx_script_result_get(xctx)
        : afw_value_undefined;
    last = result;

    scope = afw_xctx_scope_create(self, afw_xctx_scope_current(xctx), xctx);
    afw_xctx_scope_activate(scope, xctx);
    AFW_TRY{
        for (i = 0; i < self->statement_count; i++) {
            last = afw_value_block_evaluate_statement(
                x, self->statements[i], p, xctx);
            if (use_running) {
                if (afw_xctx_statement_flow_is_type(return, xctx)) {
                    afw_xctx_script_result_set(last, xctx);
                    result = last;
                    break;
                }
                if (!afw_xctx_statement_flow_is_type(sequential, xctx)) {
                    /* break / continue / rethrow: keep the running result. */
                    result = afw_xctx_script_result_get(xctx);
                    break;
                }
                result = afw_xctx_script_result_get(xctx);
            }
            else {
                result = last;
                if (!afw_xctx_statement_flow_is_type(sequential, xctx)) {
                    break;
                }
            }
        }
        if (use_running &&
            !xctx->script_result_written &&
            self->statement_count == 1 &&
            !impl_statement_is_void_for_script_result(self->statements[0]))
        {
            result = last;
        }
    }
    AFW_FINALLY{
        afw_xctx_scope_deactivate(scope, xctx);
    }
    AFW_ENDTRY;

    /* Pop value from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_value(xctx);
    xctx->error->contextual = saved_contextual;

    return result;
}



const afw_value_t *
afw_value_block_evaluate_statement(
    afw_function_execute_t *x,
    const afw_value_t *statement,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    afw_xctx_statement_flow_set_type(sequential, xctx);
    xctx->statement_flow_label = NULL;

    /* If statement is block, handle special. */
    if (afw_value_is_block(statement)) {
        result = afw_value_block_evaluate_block(
            x, (const afw_value_block_t *)statement, p, xctx,
            false);
    }

    /* If not block, just evaluate. */
    else {
        result = afw_value_evaluate(statement, p, xctx);
    }

    return result;
}



AFW_DEFINE(const afw_value_block_t *)
afw_value_block_allocated_and_link(
    const afw_compile_value_contextual_t *contextual,
    afw_value_compiled_value_t *compiled_value,
    afw_size_t *block_count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_block_t *block;

    /* Error if top block has already ended. */
    if (compiled_value->top_block &&
        !compiled_value->current_block)
    {
        /*
         * This is probably caused by parent being passed with top_block
         * but already backed out of current_block.
         */
        AFW_THROW_ERROR_Z(general, "Block block underflow", xctx);

        /** @fixme Remove this code. Saving for now in case error is wrong.
        block = compiled_value->top_block;
        compiled_value->current_block = block;
        return (const afw_value_block_t *)block;
         */
    }
    else {
        block = afw_pool_calloc_type(p, afw_value_block_t, xctx);
        block->inf = &afw_value_block_inf;
        block->contextual = contextual;
    }
    if (!compiled_value->top_block) {
        compiled_value->top_block = block;
    }
    else {
        block->parent_block = compiled_value->current_block;
        block->depth = block->parent_block->depth + 1;
        (*block_count)++;
        block->number = *block_count;
        if (!block->parent_block->first_child_block) {
            block->parent_block->first_child_block = block;
        }
        else {
            block->parent_block->final_child_block->next_sibling_block = block;
        }
        block->parent_block->final_child_block = block;
    }
    compiled_value->current_block = block;

    return (const afw_value_block_t*)block;
}



AFW_DEFINE(const afw_value_t *)
afw_value_block_finalize(
    const afw_value_block_t *block,
    afw_size_t statement_count,
    const afw_value_t * const *statements,
    afw_xctx_t *xctx)
{
    afw_value_block_t *self = (afw_value_block_t *)block;

    self->statement_count = statement_count;
    self->statements = statements;

    return &self->pub;
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
    afw_function_execute_t x;

    /** @todo temporary */
    x.p = p;
    x.xctx = xctx;

    /* Evaluate block. */
    result = afw_value_block_evaluate_block(
        &x, (const afw_value_block_t *)&self->pub, p, xctx,
        true);

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
    return NULL;
}

/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_block_symbol_t *symbol;
    afw_size_t i;

    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->contextual, xctx);

    afw_writer_write_z(writer, " number=", xctx);
    afw_writer_write_size(writer, self->number, xctx);
    afw_writer_write_z(writer, " depth=", xctx);
    afw_writer_write_size(writer, self->depth, xctx);
    afw_writer_write_z(writer, " : [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED

    if (self->symbol_count > 0) {
        afw_writer_write_z(writer, "symbols: [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
        for (symbol = self->first_entry; symbol; symbol = symbol->next_entry) {
            afw_writer_write_z(writer, "[", xctx);
            afw_writer_write_size(writer, symbol->index, xctx);
            afw_writer_write_z(writer, "] ", xctx);
            afw_writer_write_utf8(writer,
                afw_value_compiler_listing_symbol_type_name(
                    symbol->symbol_type),
                xctx);
            afw_writer_write_z(writer, " ", xctx);
            afw_value_compiler_listing_name_and_type(writer,
                symbol->name, &symbol->type, xctx);
            afw_writer_write_eol(writer, xctx);
        }
        afw_writer_decrement_indent(writer, xctx);
        afw_writer_write_z(writer, "]", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->statement_count > 0) {
        afw_writer_write_z(writer, "statements: [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
        for (i = 0; i < self->statement_count; i++) {
            afw_value_produce_compiler_listing(self->statements[i], writer, xctx);
        }
        afw_writer_decrement_indent(writer, xctx);
        afw_writer_write_z(writer, "]", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);

    afw_writer_write_eol(writer, xctx);
}



/*
 * Implementation of method decompile for interface afw_value.
 *
 * Synthetic call #block(statement, ...) matching listing's statement list.
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{

    afw_value_decompile_write_synthetic_function_name(&self->pub, writer, xctx);
    afw_value_decompile_value_list(writer, self->statement_count,
        self->statements, xctx);
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
    info->contextual = self->contextual;
    info->optimized_value = &self->pub;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}

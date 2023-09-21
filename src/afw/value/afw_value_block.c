// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for block
 *
 * Copyright (c) 2010-2023 Clemson University
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

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "block"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_block_inf
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


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_block(
    afw_function_execute_t *x,
    const afw_value_block_t *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;
    afw_size_t i;
    const afw_xctx_scope_t *scope;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(
        (const afw_value_t *)self, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;
    result = afw_value_undefined;

    scope = afw_xctx_scope_create(self, afw_xctx_scope_current(xctx), xctx);
    afw_xctx_scope_activate(scope, xctx);
    AFW_TRY{
        for (i = 0; i < self->statement_count; i++) {
            result = afw_value_block_evaluate_statement(
                x, self->statements[i], p, xctx);
            if (!afw_xctx_statement_flow_is_type(sequential, xctx)) {
                break;
            }
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



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_statement(
    afw_function_execute_t *x,
    const afw_value_t *statement,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    afw_xctx_statement_flow_set_type(sequential, xctx);

    /* If statement is block, handle special. */
    if (afw_value_is_block(statement)) {
        result = afw_value_block_evaluate_block(
            x, (const afw_value_block_t *)statement, p, xctx);
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

    return (const afw_value_t *)self;
}



/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    const afw_value_t * instance,
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
        &x, (const afw_value_block_t *)instance, p, xctx);

    return result;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}

/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_block_t *self =
        (const afw_value_block_t *)instance;
    afw_value_block_symbol_t *symbol;
    afw_size_t i;

    afw_value_compiler_listing_begin_value(writer, instance,
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
 */
void
impl_afw_value_decompile(
    const afw_value_t * instance,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    /*

    if (self->qualifier.len > 0) {
        afw_writer_write_utf8(writer, &self->qualifier, xctx);
        afw_writer_write_z(writer, "::", xctx);
    }
    afw_writer_write_utf8(writer, &self->name, xctx);
    afw_value_decompile_call_args(writer, 0, &self->args, xctx);

     */
}


/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    const afw_value_t *instance,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_block_t *self =
        (const afw_value_block_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}

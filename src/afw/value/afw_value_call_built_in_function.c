// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for call_built_in_function
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_call_built_in_function.c
 * @brief Implementation of afw_value interface for call_built_in_function
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
#define AFW_IMPLEMENTATION_ID "call_built_in_function"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_call_built_in_function_inf
#define AFW_VALUE_SELF_T afw_value_call_built_in_function_t
#include "afw_value_impl_declares.h"



/* Create function for call value. */
AFW_DEFINE(const afw_value_t *)
afw_value_call_built_in_function_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_call_built_in_function_t *self;
    const afw_value_function_definition_t *function;
    afw_value_info_t info;

    if (!afw_value_is_function_definition(argv[0])) {
        AFW_THROW_ERROR_Z(general,
            "afw_value_call_built_in_function_create() argv[0] must be "
            "function definition", xctx);
    }

    self = afw_pool_calloc_type(p, afw_value_call_built_in_function_t, xctx);
    self->inf = &afw_value_call_built_in_function_inf;
    function = (const afw_value_function_definition_t *)argv[0];
    self->function = function;
    self->args.contextual = contextual;
    self->args.argc = argc;
    self->args.argv = argv;
    self->optimized_value = (const afw_value_t *)self;

    if (allow_optimize && afw_flag_is_active(
        xctx->env->flag_index_compile_noOptimize_active, xctx))
    {
        if (afw_value_is_boolean_true(
                function->polymorphic) &&
            !afw_value_is_boolean_true(
                function->polymorphicExecuteFunctionEvaluatesFirstParameter)
            )
        {
            if (argc < 1 || !argv[1]) {
                AFW_THROW_ERROR_Z(general,
                    "afw_value_call_built_in_function_create() argv[1] must be "
                    "defined for polymorphic functions", xctx);
            }
            afw_value_get_info(argv[1], &info, p, xctx);
            if (info.evaluated_data_type) {
                function = afw_environment_get_qualified_function(
                    &info.evaluated_data_type->data_type_id,
                    &function->functionId->internal, xctx);
                if (!function) {
                    function = self->function;
                }
            }
            self->function = function;
        }
    }

    /* If function returns a value, set optimized_value_data_type. */
    if (function->returns && function->returns->data_type) {
        self->evaluated_data_type = function->returns->data_type;
    }
    /*
     * Polymorphic return ≈ param1 type only on compile create
     * (allow_optimize). Runtime call_create sites (e.g. reduce/map) often
     * pass argc with argv slots not yet filled — get_info would SEGV.
     */
    else if (allow_optimize &&
        function->returns &&
        afw_value_is_boolean_true(function->returns->polymorphicDataType) &&
        argc >= 1 && argv[1])
    {
        afw_value_get_info(argv[1], &info, p, xctx);
        if (info.evaluated_data_type) {
            self->evaluated_data_type = info.evaluated_data_type;
        }
    }

    /*
     * Formal/arity typeCheck is compile-only (same allow_optimize gate).
     * Runtime one-shot and higher-order create paths must not walk args.
     */
    if (allow_optimize) {
        afw_value_type_check_adaptive_function_call(self->function, argc, argv,
            xctx);
    }

    return &self->pub;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_call_built_in_function(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_function_definition_t *function,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    /* Optimize is set to false since this is one time call. */
    value = afw_value_call_built_in_function_create(
        contextual, argc, argv, false, p, xctx);
    return impl_afw_value_optional_evaluate((AFW_VALUE_SELF_T *)value, p, xctx);
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
    const afw_compile_value_contextual_t *saved_contextual;
    const afw_value_t *result;
    afw_function_execute_t x;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(&self->pub, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->args.contextual;

    /* Make function execute struct to pass to function. */
    x.self = self;
    x.p = p;
    x.xctx = xctx;
    x.function = self->function;
    x.data_type = self->function->data_type;
    x.first_arg = NULL;
    /* Expand call-site ...spreads before required/max checks and execute. */
    afw_value_call_args_expand_spreads(
        self->args.argc, self->args.argv,
        &x.argc, &x.argv, p, xctx);

    /* Make there are at least the required number of parameters. */
    if (x.argc < x.function->numberOfRequiredParameters->internal) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q
            " expects " AFW_SIZE_T_FMT " required parameters",
            AFW_UTF8_FMT_ARG(&x.function->functionId->internal),
            x.function->numberOfRequiredParameters->internal);
    }

    /* Make there are at least the required number of parameters. */
    if (x.function->maximumNumberOfParameters->internal != -1 &&
        x.argc > x.function->maximumNumberOfParameters->internal)
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q
            " expects no more than " AFW_SIZE_T_FMT " parameters",
            AFW_UTF8_FMT_ARG(&x.function->functionId->internal),
            x.function->maximumNumberOfParameters->internal);
    }

    /*
     * If function AFW_FUNCTION_EXECUTE_STANDARD_POLYMORPHIC_FUNCTION_HANDLING
     * (NULL), process it here and call correct <dataType> function. This is the
     * only place a NULL execute function is handled.
     * 
     * This will determine the correct function->execute to use based on the
     * <dataType> of the first parameter.
     */
    if (self->function->execute == 
        AFW_FUNCTION_EXECUTE_STANDARD_POLYMORPHIC_FUNCTION_HANDLING)
    {
        x.first_arg = afw_function_evaluate_parameter(&x, 1, NULL);
        if (!x.first_arg) {
            AFW_THROW_ERROR_FZ(arg_error, xctx,
                "Polymorphic function " AFW_UTF8_FMT_Q
                " requires first parameter not be undefined",
                AFW_UTF8_FMT_ARG(&self->function->functionId->internal));
        }
        x.data_type = afw_value_get_data_type(x.first_arg, xctx);
        x.function = afw_environment_registry_get_data_type_method(
            x.data_type, x.function->dataTypeMethodNumber, xctx);
        if (!x.function) {
            AFW_THROW_ERROR_FZ(arg_error, xctx,
                AFW_UTF8_FMT_Q " is not a method of data type " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(&self->function->functionId->internal),
                AFW_UTF8_FMT_ARG(&x.data_type->data_type_id));
        }
    }

    /*
     * If <dataType> function, evaluate it's first parameter if there is one.
     * afw_function_evaluate_parameter() will throw an error if
     * one is actually required. This insures that the first parameter has
     * been evaluated for <dataType> functions called directly or
     * polymorphically.
     */
    else if (x.data_type) {
        x.first_arg = afw_function_evaluate_parameter(&x, 1, NULL);
    }

    /* Call function implementation. */
    result = x.function->execute(&x);

    /* Pop value from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_value(xctx);
    xctx->error->contextual = saved_contextual;
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
    return self->evaluated_data_type;
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

    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->args.contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    if (self->evaluated_data_type) {
        afw_writer_write_z(writer, "evaluated_data_type: ", xctx);
        afw_writer_write_utf8(writer,
            &self->evaluated_data_type->data_type_id, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->optimized_value != &self->pub) {
        afw_writer_write_z(writer, "optimized_value: ", xctx);
        afw_value_produce_compiler_listing(self->optimized_value, writer, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    afw_value_compiler_listing_value(self->args.argv[0], writer, xctx);

    afw_writer_write_z(writer, "arguments : [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);
    for (afw_size_t i = 1; i <= self->args.argc; i++) {
        afw_value_compiler_listing_value(self->args.argv[i], writer, xctx);
    }
    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}



/*
 * Implementation of method decompile for interface afw_value.
 *
 * For const/let, a bare symbol_reference target (e.g. function statement)
 * is written as #assignment_target so recompile introduces a binding.
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *fid;
    const afw_value_t *arg;
    const afw_value_symbol_reference_t *sym;
    afw_size_t i;
    afw_boolean_t is_const_or_let;
    afw_boolean_t is_try;
    afw_value_string_t kind;

    fid = &self->function->functionId->internal;
    afw_writer_write_utf8(writer, fid, xctx);

    is_const_or_let =
        afw_utf8_equal_utf8_z(fid, "const") ||
        afw_utf8_equal_utf8_z(fid, "let");
    is_try = afw_utf8_equal_utf8_z(fid, "try");

    if (!is_const_or_let && !is_try) {
        afw_value_decompile_call_args(writer, 1, &self->args, xctx);
        return;
    }

    /* Custom arg list for const/let/try binding targets. */
    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }
    for (i = 1; i <= self->args.argc; i++) {
        /*
         * Pattern catch: 4th try arg is embedded in the catch #block;
         * omit it as a separate try() argument (see Pattern branch below).
         */
        if (is_try && i == 4 &&
            afw_value_is_assignment_target(self->args.argv[i]))
        {
            continue;
        }
        if (i != 1) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        arg = self->args.argv[i];
        /*
         * const/let first param: bare symbol_reference must reparse as
         * #assignment_target so a binding is introduced.
         */
        if (is_const_or_let && i == 1 &&
            afw_value_is_symbol_reference(arg))
        {
            sym = (const afw_value_symbol_reference_t *)arg;
            afw_writer_write_z(writer, "#assignment_target(", xctx);
            kind.inf = &afw_value_unmanaged_string_inf;
            kind.internal = *fid;
            afw_value_decompile((const afw_value_t *)&kind, writer, xctx);
            afw_writer_write_z(writer, ",", xctx);
            afw_writer_write_utf8(writer, sym->symbol->name, xctx);
            afw_value_decompile_optional_type(&sym->symbol->type, writer,
                xctx);
            afw_writer_write_z(writer, ")", xctx);
        }
        /*
         * try catch block (3rd) + error binding (4th):
         *
         * Identifier catch: emit
         *   #block(#assignment_target("let", e), stmts...), "e"
         * so reparse introduces e in the block; execute_try resolves the
         * string name (or assigns via symbol_reference).
         *
         * Pattern catch (issue #140): emit
         *   #block(#assignment_target("let", Pattern), stmts...)
         * only (no separate 4th arg). execute_try treats the first
         * statement of the catch block as the bind target when argv[4]
         * is absent, then runs the remaining statements.
         */
        else if (is_try && i == 3 && afw_value_is_block(arg) &&
            self->args.argc >= 4)
        {
            const afw_value_block_t *catch_block =
                (const afw_value_block_t *)arg;
            const afw_value_t *err_bind = self->args.argv[4];
            afw_size_t j;
            afw_boolean_t pattern_bind;

            pattern_bind = afw_value_is_assignment_target(err_bind);

            afw_writer_write_z(writer, "#block(", xctx);
            if (writer->tab) {
                afw_writer_increment_indent(writer, xctx);
                afw_writer_write_eol(writer, xctx);
            }
            if (pattern_bind) {
                /* Full Pattern as #assignment_target("let", Pattern). */
                afw_value_decompile_value(err_bind, writer, xctx);
            }
            else if (afw_value_is_symbol_reference(err_bind)) {
                sym = (const afw_value_symbol_reference_t *)err_bind;
                afw_writer_write_z(writer, "#assignment_target(", xctx);
                kind.inf = &afw_value_unmanaged_string_inf;
                kind.internal.s = "let";
                kind.internal.len = 3;
                afw_value_decompile((const afw_value_t *)&kind, writer, xctx);
                afw_writer_write_z(writer, ",", xctx);
                afw_writer_write_utf8(writer, sym->symbol->name, xctx);
                afw_writer_write_z(writer, ")", xctx);
            }
            else if (afw_value_is_string(err_bind)) {
                /* Reparse of identifier catch: 4th arg is the name string. */
                afw_writer_write_z(writer, "#assignment_target(", xctx);
                kind.inf = &afw_value_unmanaged_string_inf;
                kind.internal.s = "let";
                kind.internal.len = 3;
                afw_value_decompile((const afw_value_t *)&kind, writer, xctx);
                afw_writer_write_z(writer, ",", xctx);
                afw_writer_write_utf8(writer,
                    &((const afw_value_string_t *)err_bind)->internal, xctx);
                afw_writer_write_z(writer, ")", xctx);
            }
            else {
                afw_value_decompile_value(err_bind, writer, xctx);
            }
            /*
             * Skip leading assignment_target statement if reparse already
             * embedded it as the first catch statement (avoid doubling).
             */
            j = 0;
            if (catch_block->statement_count > 0 &&
                afw_value_is_assignment_target(catch_block->statements[0]))
            {
                j = 1;
            }
            for (; j < catch_block->statement_count; j++) {
                afw_writer_write_z(writer, ",", xctx);
                if (writer->tab) {
                    afw_writer_write_eol(writer, xctx);
                }
                afw_value_decompile_value(catch_block->statements[j],
                    writer, xctx);
            }
            if (writer->tab) {
                afw_writer_write_eol(writer, xctx);
                afw_writer_decrement_indent(writer, xctx);
            }
            afw_writer_write_z(writer, ")", xctx);
        }
        else if (is_try && i == 4 &&
            afw_value_is_symbol_reference(arg))
        {
            sym = (const afw_value_symbol_reference_t *)arg;
            kind.inf = &afw_value_unmanaged_string_inf;
            kind.internal = *sym->symbol->name;
            afw_value_decompile((const afw_value_t *)&kind, writer, xctx);
        }
        else {
            afw_value_decompile_value(arg, writer, xctx);
        }
    }
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);
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
    info->detail = &self->function->functionId->internal;
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->contextual = self->args.contextual;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}

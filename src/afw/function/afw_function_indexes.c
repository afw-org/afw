// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for indexes
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_indexes.c
 * @brief afw_function_execute_* functions for indexes.
 */

#include "afw_internal.h"
#include "afw_adaptor_impl_index.h"


/*
 * Adaptive function: index_create
 *
 * afw_function_execute_index_create
 *
 * See afw_function_bindings.h for more information.
 *
 * Create an index definition.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function index_create(
 *       adaptorId: string,
 *       key: string,
 *       value?: string,
 *       objectType?: string,
 *       filter?: string,
 *       options?: string,
 *       retroactive?: boolean,
 *       test?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor.
 *
 *   key - (string) Name of the property index to be created.
 *
 *   value - (optional string) Expression to calculate the index value(s).
 *
 *   objectType - (optional string) Object Type(s) this index may apply to.
 *
 *   filter - (optional string) Expression to determine if this index applies
 *       to a particular object.
 *
 *   options - (optional string) Indexing options.
 *
 *   retroactive - (optional boolean) Retroactively generate indexes for
 *       existing objects.
 *
 *   test - (optional boolean) Test create (don't actually perform).
 *
 * Returns:
 *
 *   (object) Object response from the indexing process.
 */
const afw_value_t *
afw_function_execute_index_create(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *key;
    const afw_value_string_t *value = NULL;
    const afw_value_array_t *objectType = NULL;
    const afw_value_string_t *filter = NULL;
    const afw_value_array_t *options = NULL;
    const afw_value_boolean_t *retroactive = NULL;
    const afw_value_boolean_t *test = NULL;
    const afw_value_t *parsedFilter;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(key, 2, string);

    if (value) {
        AFW_TRY {
            /* go ahead and parse the value to make sure it's valid */
            afw_compile_to_value(
                &value->internal, AFW_FUNCTION_SOURCE_LOCATION,
                afw_compile_type_expression,
                NULL, NULL, x->p, xctx);
        } AFW_CATCH_UNHANDLED {
            result = afw_object_create_managed(x->p, xctx);
            afw_object_set_property_as_object(result, &afw_s_error,
                afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
            return afw_value_create_object(result, x->p, xctx);
        }

        AFW_ENDTRY;
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 3, string);
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType, 4, array);
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(filter, 5, string);

        /* go ahead and parse the filter to make sure it's valid */
        parsedFilter = afw_compile_to_value(
            &filter->internal, AFW_FUNCTION_SOURCE_LOCATION, 
            afw_compile_type_expression,
            NULL, NULL, x->p, xctx);
        if (parsedFilter == NULL) {
            AFW_THROW_ERROR_Z(general, "Error parsing filter expression.", xctx);
        }
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(6)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(options, 6, array);
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(7)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(retroactive, 7, boolean);
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(8)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(test, 8, boolean);
    }

    result = afw_adaptor_impl_index_create(
        &adaptorId->internal, &key->internal, 
        (value ? &value->internal : NULL),
        (objectType ? objectType->internal : NULL),
        (filter ? &filter->internal : NULL), 
        (options ? options->internal : NULL), 
        (retroactive ? retroactive->internal : true), 
        (test ? test->internal : false), x->p, xctx);

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: index_list
 *
 * afw_function_execute_index_list
 *
 * See afw_function_bindings.h for more information.
 *
 * List property indexes
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function index_list(
 *       adaptorId: string,
 *       objectType?: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor.
 *
 *   objectType - (optional string) Id of adaptive object type indexes.
 *
 * Returns:
 *
 *   (object) Object response from the index repair process.
 */
const afw_value_t *
afw_function_execute_index_list(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType = NULL;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 1, string);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
            2, string);
    }

    result = afw_adaptor_impl_index_list(
        &adaptorId->internal, (objectType) ? &objectType->internal : NULL,
            x->p, x->xctx);

    return afw_value_create_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: index_remove
 *
 * afw_function_execute_index_remove
 *
 * See afw_function_bindings.h for more information.
 *
 * Remove an index definition.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function index_remove(
 *       adaptorId: string,
 *       key: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor.
 *
 *   key - (string) The index key to be removed.
 *
 * Returns:
 *
 *   (object) Object response from the indexing process.
 */
const afw_value_t *
afw_function_execute_index_remove(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *key;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(key, 2, string);

    result = afw_adaptor_impl_index_remove(
        &adaptorId->internal, &key->internal, x->p, x->xctx);

    return afw_value_create_object(result, x->p, x->xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for journal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_journal.c
 * @brief afw_function_execute_* functions for journal.
 */

#include "afw_internal.h"



/*
 * Adaptive function: journal_advance_cursor_for_consumer
 *
 * afw_function_execute_journal_advance_cursor_for_consumer
 *
 * See afw_function_bindings.h for more information.
 *
 * Update the advance cursor for a consumer referenced by the consumerId
 * parameter. The limit parameter specifies the maximum number of entries to
 * scan for an applicable entry for consumer before returning. NULL is always
 * returned.
 * 
 * There are no response properties set by this function.
 * 
 * The properties of the _AdaptiveProvisioningPeer_ object associated with the
 * consumer_id are used in the following way:
 * 
 * The consumerFilter expression is used to determine if an entry is applicable.
 * 
 * Journal entries are scanned beginning at the entry at the cursor in the
 * advanceCursor property. If the advanceCursor property is not present, the
 * scan begins after the cursor in currentCursor. If neither are present, the
 * scan begins at the start of the journal.
 * 
 * If an new applicable entry is found or if the limit is met, the advanceCursor
 * property is set to the currently scanned entry's cursor.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function journal_advance_cursor_for_consumer(
 *       adapterId: string,
 *       consumerId: string,
 *       limit?: integer
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   consumerId - (string) The consumerId property value of the associated
 *       _AdaptiveProvisioningPeer_ object.
 *
 *   limit - (optional integer) The maximum number of entries that will be
 *       scanned for an entry where the consumerFilter expression in the
 *       associated _AdaptiveProvisioningPeer_ object evaluates to true.
 *
 * Returns:
 *
 *   (object) Response object.
 */
const afw_value_t *
afw_function_execute_journal_advance_cursor_for_consumer(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_value_string_t *consumerId;
    const afw_value_integer_t *limit_arg;
    afw_size_t limit;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(consumerId, 2, string);
    limit = 0;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(limit_arg, 3, integer);
        limit = (afw_size_t)limit_arg->internal;
    }
    
    result = afw_adapter_journal_advance_cursor_for_consumer(
        &adapterId->internal, &consumerId->internal, limit, x->p, x->xctx);
    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: journal_get_by_cursor
 *
 * afw_function_execute_journal_get_by_cursor
 *
 * See afw_function_bindings.h for more information.
 *
 * Get journal entry specified by entry_cursor parameter.
 * 
 * This option will set response properties 'entry' and 'cursor' if there is an
 * entry to retrieve. If an entry with the supplied cursor does not exist, a
 * not_found error is thrown.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function journal_get_by_cursor(
 *       adapterId: string,
 *       cursor: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   cursor - (string) Journal entry cursor.
 *
 * Returns:
 *
 *   (object) Response object.
 */
const afw_value_t *
afw_function_execute_journal_get_by_cursor(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_value_string_t *cursor;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(cursor, 2, string);
    
    result = afw_adapter_journal_get_by_cursor(
        &adapterId->internal, &cursor->internal, x->p, x->xctx);
    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: journal_get_first
 *
 * afw_function_execute_journal_get_first
 *
 * See afw_function_bindings.h for more information.
 *
 * Get first journal entry.
 * 
 * This option will set response properties 'entry' and 'cursor' if there is a
 * first entry to return.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function journal_get_first(
 *       adapterId: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 * Returns:
 *
 *   (object) Response object.
 */
const afw_value_t *
afw_function_execute_journal_get_first(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
   
    result = afw_adapter_journal_get_first(
        &adapterId->internal, x->p, x->xctx);
    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: journal_get_next_after_cursor
 *
 * afw_function_execute_journal_get_next_after_cursor
 *
 * See afw_function_bindings.h for more information.
 *
 * Get the next journal entry after the one specified by the entry_cursor
 * parameter.
 * 
 * This option will set response properties 'entry' and 'cursor' if there is a
 * next entry to retrieve.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function journal_get_next_after_cursor(
 *       adapterId: string,
 *       cursor: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   cursor - (string) Journal entry cursor.
 *
 * Returns:
 *
 *   (object) Response object.
 */
const afw_value_t *
afw_function_execute_journal_get_next_after_cursor(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_value_string_t *cursor;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(cursor, 2, string);
    
    result = afw_adapter_journal_get_next_after_cursor(
        &adapterId->internal, &cursor->internal, x->p, x->xctx);
    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: journal_get_next_for_consumer
 *
 * afw_function_execute_journal_get_next_for_consumer
 *
 * See afw_function_bindings.h for more information.
 *
 * Get the next journal entry for a consumer referenced by the consumer_id
 * parameter. The entry_cursor parameter is ignored. The limit parameter
 * specifies the maximum number of entries to scan for an applicable entry for
 * consumer before returning.
 * 
 *  This option will set response properties 'entry' and 'entryCursor' if an
 * applicable entry is retrieved. Property 'reissue' will be set as described
 * below.
 * 
 * The properties of the _AdaptiveProvisioningPeer_ object associated with the
 * consumer_id are used in the following way:
 * 
 * The consumerFilter expression is used to determine if an entry is applicable.
 * 
 * If consumeCursor property exists, return that entry at that cursor again
 * immediately with a 'reissue' property added and set to true.
 * 
 * Journal entries are scanned beginning at the entry at the cursor in the
 * advanceCursor property. If the advanceCursor property is not present, the
 * scan begins after the cursor in currentCursor. If neither are present, the
 * scan begins at the start of the journal.
 * 
 * If an applicable entry is found, properties consumeStart and consumeCursor
 * are set, advanceCursor is removed, and the entry is returned. Method
 * mark_entry_consumed() will remove these properties.
 * 
 * If no applicable entry is found, advanceCursor is set to the last entry
 * scanned.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function journal_get_next_for_consumer(
 *       adapterId: string,
 *       consumerId: string,
 *       limit?: integer
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   consumerId - (string) The consumerId property value of the associated
 *       _AdaptiveProvisioningPeer_ object.
 *
 *   limit - (optional integer) The maximum number of entries that will be
 *       scanned for an entry where the consumerFilter expression in the
 *       associated _AdaptiveProvisioningPeer_ object evaluates to true.
 *
 * Returns:
 *
 *   (object) Response object.
 */
const afw_value_t *
afw_function_execute_journal_get_next_for_consumer(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_value_string_t *consumerId;
    const afw_value_integer_t *limit_arg;
    afw_size_t limit;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(consumerId, 2, string);
    limit = 0;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(limit_arg, 3, integer);
        limit = (afw_size_t)limit_arg->internal;
    }
    
    result = afw_adapter_journal_get_next_for_consumer(
        &adapterId->internal, &consumerId->internal, limit, x->p, x->xctx);
    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: journal_get_next_for_consumer_after_cursor
 *
 * afw_function_execute_journal_get_next_for_consumer_after_cursor
 *
 * See afw_function_bindings.h for more information.
 *
 * Get the next journal entry for a consumer referenced by the consumer_id after
 * the one specified by the entry_cursor parameter. The limit parameter
 * specifies the maximum number of entries to scan for an applicable entry for
 * consumer before returning.
 * 
 * This option will set response properties 'entry' and 'cursor' if an
 * applicable entry is retrieved.
 * 
 * The properties of the _AdaptiveProvisioningPeer_ object associated with the
 * consumer_id are used in the following way:
 * 
 * The consumerFilter expression is used to determine if an entry is applicable.
 * 
 * Unlike option get_next_for_consumer, no other properties are referenced or
 * modified.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function journal_get_next_for_consumer_after_cursor(
 *       adapterId: string,
 *       consumerId: string,
 *       cursor: string,
 *       limit?: integer
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   consumerId - (string) The consumerId property value of the associated
 *       _AdaptiveProvisioningPeer_ object.
 *
 *   cursor - (string) Journal entry cursor.
 *
 *   limit - (optional integer) The maximum number of entries that will be
 *       scanned for an entry where the consumerFilter expression in the
 *       associated _AdaptiveProvisioningPeer_ object evaluates to true.
 *
 * Returns:
 *
 *   (object) Response object.
 */
const afw_value_t *
afw_function_execute_journal_get_next_for_consumer_after_cursor(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_value_string_t *consumerId;
    const afw_value_string_t *cursor;
    const afw_value_integer_t *limit_arg;
    afw_size_t limit;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(consumerId, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(cursor, 3, string);
    limit = 0;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(limit_arg, 4, integer);
        limit = (afw_size_t)limit_arg->internal;
    }
    
    result = afw_adapter_journal_get_next_for_consumer_after_cursor(
        &adapterId->internal, &consumerId->internal, &cursor->internal,
        limit, x->p, x->xctx);
    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: journal_mark_consumed
 *
 * afw_function_execute_journal_mark_consumed
 *
 * See afw_function_bindings.h for more information.
 *
 * Mark a journal entry returned by get_next_for_consumer() as consumed.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function journal_mark_consumed(
 *       adapterId: string,
 *       consumerId: string,
 *       cursor: string
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   consumerId - (string) The consumerId property value of the associated
 *       _AdaptiveProvisioningPeer_ object.
 *
 *   cursor - (string) Journal entry cursor.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_journal_mark_consumed(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_value_string_t *consumerId;
    const afw_value_string_t *cursor;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(consumerId, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(cursor, 3, string);
    
    afw_adapter_journal_mark_consumed(
        &adapterId->internal, &consumerId->internal, &cursor->internal,
        x->p, x->xctx);

    /* Return undefined for void. */
    return afw_value_undefined;
}

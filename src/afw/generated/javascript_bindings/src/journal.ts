// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for journal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file journal.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the journal category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
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
 * The consumerFilter expression is used to determine if an entry is
 * applicable.
 * 
 * Journal entries are scanned beginning at the entry at the cursor in the
 * advanceCursor property. If the advanceCursor property is not present, the
 * scan begins after the cursor in currentCursor. If neither are present, the
 * scan begins at the start of the journal.
 * 
 * If an new applicable entry is found or if the limit is met, the
 * advanceCursor property is set to the currently scanned entry's cursor.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} consumerId - The consumerId property value of the
 *     associated _AdaptiveProvisioningPeer_ object.
 * 
 * @param {integer} limit - The maximum number of entries that will be scanned
 *     for an entry where the consumerFilter expression in the associated
 *     _AdaptiveProvisioningPeer_ object evaluates to true.
 * 
 * @returns {object} Response object.
 */
export function afwJournalAdvanceCursorForConsumer(client : any, consumerId : string, adapterId? : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_advance_cursor_for_consumer";
    _action["consumerId"] = consumerId;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Get journal entry specified by entry_cursor parameter.
 * 
 * This option will set response properties 'entry' and 'cursor' if there is
 * an entry to retrieve. If an entry with the supplied cursor does not exist,
 * a not_found error is thrown.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} cursor - Journal entry cursor.
 * 
 * @returns {object} Response object.
 */
export function afwJournalGetByCursor(client : any, cursor : string, adapterId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_get_by_cursor";
    _action["cursor"] = cursor;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    return client.perform(_action);
}

/**
 * Get first journal entry.
 * 
 * This option will set response properties 'entry' and 'cursor' if there is a
 * first entry to return.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @returns {object} Response object.
 */
export function afwJournalGetFirst(client : any, adapterId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_get_first";

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    return client.perform(_action);
}

/**
 * Get the next journal entry after the one specified by the entry_cursor
 * parameter.
 * 
 * This option will set response properties 'entry' and 'cursor' if there is a
 * next entry to retrieve.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} cursor - Journal entry cursor.
 * 
 * @returns {object} Response object.
 */
export function afwJournalGetNextAfterCursor(client : any, cursor : string, adapterId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_get_next_after_cursor";
    _action["cursor"] = cursor;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    return client.perform(_action);
}

/**
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
 * The consumerFilter expression is used to determine if an entry is
 * applicable.
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
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} consumerId - The consumerId property value of the
 *     associated _AdaptiveProvisioningPeer_ object.
 * 
 * @param {integer} limit - The maximum number of entries that will be scanned
 *     for an entry where the consumerFilter expression in the associated
 *     _AdaptiveProvisioningPeer_ object evaluates to true.
 * 
 * @returns {object} Response object.
 */
export function afwJournalGetNextForConsumer(client : any, consumerId : string, adapterId? : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_get_next_for_consumer";
    _action["consumerId"] = consumerId;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Get the next journal entry for a consumer referenced by the consumer_id
 * after the one specified by the entry_cursor parameter. The limit parameter
 * specifies the maximum number of entries to scan for an applicable entry for
 * consumer before returning.
 * 
 * This option will set response properties 'entry' and 'cursor' if an
 * applicable entry is retrieved.
 * 
 * The properties of the _AdaptiveProvisioningPeer_ object associated with the
 * consumer_id are used in the following way:
 * 
 * The consumerFilter expression is used to determine if an entry is
 * applicable.
 * 
 * Unlike option get_next_for_consumer, no other properties are referenced or
 * modified.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} consumerId - The consumerId property value of the
 *     associated _AdaptiveProvisioningPeer_ object.
 * 
 * @param {string} cursor - Journal entry cursor.
 * 
 * @param {integer} limit - The maximum number of entries that will be scanned
 *     for an entry where the consumerFilter expression in the associated
 *     _AdaptiveProvisioningPeer_ object evaluates to true.
 * 
 * @returns {object} Response object.
 */
export function afwJournalGetNextForConsumerAfterCursor(client : any, consumerId : string, cursor : string, adapterId? : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_get_next_for_consumer_after_cursor";
    _action["consumerId"] = consumerId;
    _action["cursor"] = cursor;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Mark a journal entry returned by get_next_for_consumer() as consumed.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} consumerId - The consumerId property value of the
 *     associated _AdaptiveProvisioningPeer_ object.
 * 
 * @param {string} cursor - Journal entry cursor.
 * 
 * @returns {void}
 */
export function afwJournalMarkConsumed(client : any, consumerId : string, cursor : string, adapterId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "journal_mark_consumed";
    _action["consumerId"] = consumerId;
    _action["cursor"] = cursor;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    return client.perform(_action);
}


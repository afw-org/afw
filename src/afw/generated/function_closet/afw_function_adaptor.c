// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for adaptor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_adaptor.c
 * @brief afw_function_execute_* functions for adaptor.
 */

#include "afw.h"



/*
 * Adaptive function: adaptor_objectCallback_signature
 *
 * afw_function_execute_adaptor_objectCallback_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for the objectCallback parameter in adaptor
 * functions. Calling this directly will throw a 'Do not call directly' error.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function adaptor_objectCallback_signature(
 *       object: object,
 *       userData?: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   object - (object) This is the object from adaptor or undefined if there
 *       are no more objects to return.
 *
 *   userData - (optional any) This is the value of the userData parameter when
 *       objectCallback was called.
 *
 * Returns:
 *
 *   (boolean) Return true if this callback should not be called again for any
 *       reason such a limit exceeded or the desired object being found.
 */
const afw_value_t *
afw_function_execute_adaptor_objectCallback_signature(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: add_object
 *
 * afw_function_execute_add_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Add an adaptive object to an adaptor, specified by the adaptorId, with a
 * specified objectType. You may supply an optional objectId, if the underlying
 * adaptorId allows it.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_object(
 *       adaptorId: string,
 *       objectType: string,
 *       object: object,
 *       objectId?: string,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor.
 *
 *   objectType - (string) Id of adaptive object type of object being added.
 *
 *   object - (object) Object to add.
 *
 *   objectId - (optional string) Optional preferred objectId of object to add.
 *       The adaptor may ignore this.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_add_object
 *       
 *       where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry. Property
 *       'objectId' is the objectId assigned by the adaptor.
 */
const afw_value_t *
afw_function_execute_add_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: add_object_with_uri
 *
 * afw_function_execute_add_object_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Add an adaptive object with a given URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_object_with_uri(
 *       uri: anyURI,
 *       object: object,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of object to add. If a URI begins with a single slash
 *       ('/'), it is the local object path. In the case of a local path, an
 *       optional preferred objectId of object can be including in URI which
 *       the adaptor may ignore.
 *
 *   object - (object) Object to add.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_add_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry. Property
 *       'objectId' is the objectId assigned by the adaptor.
 */
const afw_value_t *
afw_function_execute_add_object_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: convert_AdaptiveQueryCriteria_to_query_string
 *
 * afw_function_execute_convert_AdaptiveQueryCriteria_to_query_string
 *
 * See afw_function_bindings.h for more information.
 *
 * This function converts an _AdaptiveQueryCriteria_ object to a query string.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function convert_AdaptiveQueryCriteria_to_query_string(
 *       queryCriteria: (object _AdaptiveQueryCriteria_),
 *       adaptorId?: string,
 *       objectType?: string,
 *       style?: integer
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   queryCriteria - (object _AdaptiveQueryCriteria_) An object type
 *       _AdaptiveQueryCriteria_ object.
 *
 *   adaptorId - (optional string) This is the adaptorId of adaptor containing
 *       the objectType for validating and mapping this query. If not
 *       specified, no validation or mapping will occur.
 *
 *   objectType - (optional string) This is the objectType used in conjunction
 *       with the adaptorId parameter. This parameter is only allowed and is
 *       required if the adaptorId parameter is specified.
 *
 *   style - (optional integer) The style of the query string produced. All of
 *       the styles are appropriately url encoded.
 *       
 *       0 - 'name=op=value' where '=op=' will be '==', '!=', '=ge=', etc. with
 *       conjunctions '&' (and) and '|' (or). Parentheses are placed around
 *       each group of like conjunctions except for outer '&'. (default)
 *       
 *       1 - the same as 1 with '=gte=' instead of '=ge=', '=lte=' instead of
 *       '=le=', '=eq=' instead of '=', and '=ne=' instead of '!='.
 *       
 *       2 - similar to 1 with ';' for '&' and ',' for '|'.
 *       
 *       3 - comparisons 'op(name,value)' where 'op' will be 'eq', 'ne', 'ge',
 *       ..., plus conjunctions 'and(list)' and 'or(list)' where 'array' is a
 *       comma separated list of any comparison or conjunction.
 *
 * Returns:
 *
 *   (string) Query string representation of the query criteria.
 */
const afw_value_t *
afw_function_execute_convert_AdaptiveQueryCriteria_to_query_string(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: convert_query_string_to_AdaptiveQueryCriteria
 *
 * afw_function_execute_convert_query_string_to_AdaptiveQueryCriteria
 *
 * See afw_function_bindings.h for more information.
 *
 * This function converts a query string to an _AdaptiveQueryCriteria_ object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function convert_query_string_to_AdaptiveQueryCriteria(
 *       queryString: string,
 *       adaptorId?: string,
 *       objectType?: string
 *   ): (object _AdaptiveQueryCriteria_);
 * ```
 *
 * Parameters:
 *
 *   queryString - (string) The query string to convert.
 *
 *   adaptorId - (optional string) This is the adaptorId of adaptor containing
 *       the objectType for validating and mapping this query. If not
 *       specified, no validation or mapping will occur.
 *
 *   objectType - (optional string) This is the objectType used in conjunction
 *       with the adaptorId parameter. This parameter is only allowed and is
 *       required if the adaptorId parameter is specified.
 *
 * Returns:
 *
 *   (object _AdaptiveQueryCriteria_) An object type _AdaptiveQueryCriteria_
 *       object representing query string.
 */
const afw_value_t *
afw_function_execute_convert_query_string_to_AdaptiveQueryCriteria(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: delete_object
 *
 * afw_function_execute_delete_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Delete an adaptive object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function delete_object(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing object to delete.
 *
 *   objectType - (string) Id of adaptive object type of object to delete.
 *
 *   objectId - (string) Id of object to delete.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_delete_object
 *       
 *       where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_delete_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: delete_object_with_uri
 *
 * afw_function_execute_delete_object_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Delete an adaptive object with a given URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function delete_object_with_uri(
 *       uri: anyURI,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of object to delete. If a URI begins with a single
 *       slash ('/'), it is the local object path.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_delete_object
 *       
 *       where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_delete_object_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: get_object
 *
 * afw_function_execute_get_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Get an adaptive object from the adaptor, specified by adaptorId, objectType
 * and objectId. Optional view options and adaptor-specific options may be
 * supplied to influence how the object is returned.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function get_object(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing object to retrieve.
 *
 *   objectType - (string) Id of adaptive object type of object to retrieve.
 *
 *   objectId - (string) Id of object to retrieve.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_get_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object) Object retrieved or NULL if not found.
 */
const afw_value_t *
afw_function_execute_get_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: get_object_with_uri
 *
 * afw_function_execute_get_object_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Get an object with a given URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function get_object_with_uri(
 *       uri: anyURI,
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of object to retrieve. If a URI begins with a single
 *       slash ('/'), it is the local object path.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_get_object
 *       
 *       where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object) Object retrieved or NULL if not found.
 */
const afw_value_t *
afw_function_execute_get_object_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: modify_object
 *
 * afw_function_execute_modify_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Modify an adaptive object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function modify_object(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       entries: array,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing object to modify.
 *
 *   objectType - (string) Id of adaptive object type of object to modify.
 *
 *   objectId - (string) Id of object to modify.
 *
 *   entries - (array) List of modifications. Entries are of the form:
 *       
 *           [
 *               'add_value',
 *               'property name' or ['property name', ... ],
 *               value
 *           ]
 *       
 *           [
 *               'remove_property',
 *               'property name' or ['property name', ... ]
 *           ]
 *       
 *           [
 *               'remove_value',
 *               'property name' or ['property name', ... ],
 *               value
 *           ]
 *       
 *           [
 *               'set_property',
 *               'property name' or ['property name', ... ],
 *               value
 *           ].
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_modify_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: modify_object_with_uri
 *
 * afw_function_execute_modify_object_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Modify an adaptive object with a given URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function modify_object_with_uri(
 *       uri: anyURI,
 *       entries: array,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of object to modify. If a URI begins with a single
 *       slash ('/'), it is the local object path.
 *
 *   entries - (array) List of asserts and modifications. Entries are of the
 *       form:
 *       
 *           [
 *               'add_value',
 *               'property name' or ['property name', ... ],
 *               value
 *           ]
 *       
 *           [
 *               'remove_property',
 *               'property name' or ['property name', ... ]
 *           ]
 *       
 *           [
 *               'remove_value',
 *               'property name' or ['property name', ... ],
 *               value
 *           ]
 *       
 *           [
 *               'set_property',
 *               'property name' or ['property name', ... ],
 *               value
 *           ].
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_modify_object_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: reconcile_object
 *
 * afw_function_execute_reconcile_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Reconcile an adaptive object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function reconcile_object(
 *       object: object,
 *       checkOnly?: boolean
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   object - (object) An object that has been retrieved with object options
 *       reconcilable, and whose properties have been modified. A modify_object
 *       request will be produced reflecting the difference between the
 *       original object and this unmodified object.
 *
 *   checkOnly - (optional boolean) Produce the modify_object request but don't
 *       execute it. The returned journal enter will contain the modify_object
 *       request.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_reconcile_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: replace_object
 *
 * afw_function_execute_replace_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Replace an adaptive object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function replace_object(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       object: object,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing object to replace.
 *
 *   objectType - (string) Id of adaptive object type of object to replace.
 *
 *   objectId - (string) Id of object to replace.
 *
 *   object - (object) Replacement object.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_replace_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_replace_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: replace_object_with_uri
 *
 * afw_function_execute_replace_object_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Replace an adaptive object with a given URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function replace_object_with_uri(
 *       uri: anyURI,
 *       object: object,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of object to replace. If a URI begins with a single
 *       slash ('/'), it is the local object path.
 *
 *   object - (object) Replacement object.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_replace_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_replace_object_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects
 *
 * afw_function_execute_retrieve_objects
 *
 * See afw_function_bindings.h for more information.
 *
 * This function retrieves adaptive objects from an adaptor, specified by
 * adaptorId, which match the type specified by objectType.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects(
 *       adaptorId: string,
 *       objectType: string,
 *       queryCriteria?: (object _AdaptiveQueryCriteria_),
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing objects to retrieve.
 *
 *   objectType - (string) Id of adaptive object type of objects to retrieve.
 *
 *   queryCriteria - (optional object _AdaptiveQueryCriteria_) This is the
 *       query criteria for objects to be retrieved. If not specified, all
 *       objects will be retrieved.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (array) This is the array of objects retrieved.
 */
const afw_value_t *
afw_function_execute_retrieve_objects(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_to_callback
 *
 * afw_function_execute_retrieve_objects_to_callback
 *
 * See afw_function_bindings.h for more information.
 *
 * This function retrieves adaptive objects from an adaptor, specified by
 * adaptorId, which match the type specified by objectType.
 * 
 * Each object is passed to the callback as it's retrieved.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_to_callback(
 *       objectCallback: (function (object: object, userData: any): boolean),
 *       userData: any,
 *       adaptorId: string,
 *       objectType: string,
 *       queryCriteria?: (object _AdaptiveQueryCriteria_),
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   objectCallback - (function (object: object, userData: any): boolean) If
 *       this is specified, this function is called once for each object
 *       retrieved instead of adding the object to the return array. Parameter
 *       object will be an object retrieved or undefined if there are no more
 *       objects. This function should return true if it wants to abort the
 *       retrieve request.
 *
 *   userData - (any dataType) This value is passed to the objectCallback
 *       function in the userData parameter.
 *
 *   adaptorId - (string) Id of adaptor containing objects to retrieve.
 *
 *   objectType - (string) Id of adaptive object type of objects to retrieve.
 *
 *   queryCriteria - (optional object _AdaptiveQueryCriteria_) This is the
 *       query criteria for objects to be retrieved. If not specified, all
 *       objects will be retrieved.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects_to_callbac
 *       k
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_retrieve_objects_to_callback(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_to_response
 *
 * afw_function_execute_retrieve_objects_to_response
 *
 * See afw_function_bindings.h for more information.
 *
 * This function retrieves adaptive objects from an adaptor, specified by
 * adaptorId, which match the type specified by objectType.
 * 
 * This function is only allowed when called during a request with content type
 * 'application/x-afw'. An '_AdaptiveResponse_' object is written to the
 * request's response as each object is retrieved. This '_AdaptiveResponse_'
 * object has a 'result' property containing the retrieved object and a
 * 'intermediate' property set to true.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_to_response(
 *       adaptorId: string,
 *       objectType: string,
 *       queryCriteria?: (object _AdaptiveQueryCriteria_),
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing objects to retrieve.
 *
 *   objectType - (string) Id of adaptive object type of objects to retrieve.
 *
 *   queryCriteria - (optional object _AdaptiveQueryCriteria_) This is the
 *       query criteria for objects to be retrieved. If not specified, all
 *       objects will be retrieved.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_retrieve_objects_to_response(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_to_stream
 *
 * afw_function_execute_retrieve_objects_to_stream
 *
 * See afw_function_bindings.h for more information.
 *
 * This function retrieves adaptive objects from an adaptor, specified by
 * adaptorId, which match the type specified by objectType.
 * 
 * Each object is written to the 'streamId' stream as it's retrieved.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_to_stream(
 *       streamNumber: integer,
 *       adaptorId: string,
 *       objectType: string,
 *       queryCriteria?: (object _AdaptiveQueryCriteria_),
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) Stream number objects will be written to as they
 *       are retrieved.
 *
 *   adaptorId - (string) Id of adaptor containing objects to retrieve.
 *
 *   objectType - (string) Id of adaptive object type of objects to retrieve.
 *
 *   queryCriteria - (optional object _AdaptiveQueryCriteria_) This is the
 *       query criteria for objects to be retrieved. If not specified, all
 *       objects will be retrieved.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_retrieve_objects_to_stream(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_with_uri
 *
 * afw_function_execute_retrieve_objects_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Retrieve adaptive objects with a given URI.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_with_uri(
 *       uri: anyURI,
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of objects to retrieve. If a URI begins with a single
 *       slash ('/'), it is the local object path. A query string can be
 *       specified.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (array) This is the array of objects retrieved.
 */
const afw_value_t *
afw_function_execute_retrieve_objects_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_with_uri_to_callback
 *
 * afw_function_execute_retrieve_objects_with_uri_to_callback
 *
 * See afw_function_bindings.h for more information.
 *
 * Retrieve adaptive objects with a given URI.
 * 
 * Each object is passed to the callback as it's retrieved.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_with_uri_to_callback(
 *       objectCallback: (function (object: object, userData: any): boolean),
 *       userData: any,
 *       uri: anyURI,
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   objectCallback - (function (object: object, userData: any): boolean) If
 *       this is specified, this function is called once for each object
 *       retrieved instead of adding the object to the return array. Parameter
 *       object will be an object retrieved or undefined if there are no more
 *       objects. This function should return true if it wants to abort the
 *       retrieve request.
 *
 *   userData - (any) This is the value passed to the objectCallback function
 *       in the userData parameter.
 *
 *   uri - (anyURI) URI of objects to retrieve. If a URI begins with a single
 *       slash ('/'), it is the local object path. A query string can be
 *       specified.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_retrieve_objects_with_uri_to_callback(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_with_uri_to_response
 *
 * afw_function_execute_retrieve_objects_with_uri_to_response
 *
 * See afw_function_bindings.h for more information.
 *
 * This function retrieves adaptive objects with a given URI.
 * 
 * This function is only allowed when called during a request with content type
 * 'application/x-afw'. An '_AdaptiveResponse_' object is written to the
 * request's response as each object is retrieved. This '_AdaptiveResponse_'
 * object has a 'result' property containing the retrieved object and a
 * 'partial' property set to true.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_with_uri_to_response(
 *       uri: anyURI,
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of objects to retrieve. If a URI begins with a single
 *       slash ('/'), it is the local object path. A query string can be
 *       specified.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_retrieve_objects_with_uri_to_response(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: retrieve_objects_with_uri_to_stream
 *
 * afw_function_execute_retrieve_objects_with_uri_to_stream
 *
 * See afw_function_bindings.h for more information.
 *
 * This function retrieves adaptive objects with a given URI.
 * 
 * Each object is written to the 'streamId' stream as it's retrieved.
 * 
 * The optional queryCriteria is used to filter the adaptive objects returned.
 * Use the objectOptions parameter to influence how the objects are viewed.
 * 
 * Options, specific to the adaptorId, can be optionally supplied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function retrieve_objects_with_uri_to_stream(
 *       streamNumber: integer,
 *       uri: anyURI,
 *       options?: (object _AdaptiveObjectOptions_),
 *       adaptorTypeSpecific?: object
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) Stream number objects will be written to as they
 *       are retrieved.
 *
 *   uri - (anyURI) URI of objects to retrieve. If a URI begins with a single
 *       slash ('/'), it is the local object path. A query string can be
 *       specified.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object view options.
 *       See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_retrieve_objects_with_uri_to_stream(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: update_object
 *
 * afw_function_execute_update_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Update an adaptive object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function update_object(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       object: object,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Id of adaptor containing object to update.
 *
 *   objectType - (string) Id of adaptive object type of object to update.
 *
 *   objectId - (string) Id of object to update.
 *
 *   object - (object) Object containing properties to replace in existing
 *       object. You can use object_modify instead to update properties of
 *       embedded objects and arrays, as well as to modify individual values of
 *       the object.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_update_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: update_object_with_uri
 *
 * afw_function_execute_update_object_with_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * Update an adaptive object with a given URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function update_object_with_uri(
 *       uri: anyURI,
 *       object: object,
 *       journal?: object,
 *       adaptorTypeSpecific?: object
 *   ): (object _AdaptiveJournalEntry_);
 * ```
 *
 * Parameters:
 *
 *   uri - (anyURI) URI of object to update. If a URI begins with a single
 *       slash ('/'), it is the local object path.
 *
 *   object - (object) Object containing properties to replace in existing
 *       object. You can use object_modify instead to update properties of
 *       embedded objects and arrays, as well as to modify individual values of
 *       the object.
 *
 *   journal - (optional object) The properties of this object will be added to
 *       the associated journal entry. Refer to
 *       /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names to
 *       avoid and for ones that have specific semantics.
 *
 *   adaptorTypeSpecific - (optional object) This is an optional object
 *       parameter with an objectType determined by the adaptorType associated
 *       with the adaptorId parameter. If the adaptorType supports this
 *       parameter, adaptor afw will have an adaptive object type with an id
 *       of:
 *       
 *       _AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object
 *       
 *       Where ${adaptorType} is the adaptor type id.
 *
 * Returns:
 *
 *   (object _AdaptiveJournalEntry_) Resulting journal entry.
 */
const afw_value_t *
afw_function_execute_update_object_with_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}

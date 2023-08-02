// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for Adaptor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_Adaptor.c
 * @brief afw_function_execute_* functions for Adaptor.
 */

#include "afw_internal.h"



static const afw_object_t *
impl_create_journal_entry(const afw_value_object_t *journal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *journal_entry;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_utf8_t *property_name;

    journal_entry = afw_object_create_managed(p, xctx);
    if (journal) {
        for (iterator = NULL;;) {
            value = afw_object_get_next_property(journal->internal,
                &iterator, &property_name, xctx);
            if (!value) break;
            afw_object_set_property(journal_entry, property_name, value, xctx);
        }
    }

    return journal_entry;
}
/* Used by retrieve_objects*() */
typedef struct impl_retrieve_cb_ctx_s {
    const afw_pool_t *p;
    const afw_list_t *list;
    const afw_value_t *objectCallback;
    const afw_value_t *userData;
    const afw_object_options_t *object_options;
    const afw_value_t *call;
    const afw_value_t *argv[3];
} impl_retrieve_cb_ctx_t;

static afw_boolean_t
impl_retrieve_cb(const afw_object_t *object, void *context,
    afw_xctx_t *xctx)
{
    impl_retrieve_cb_ctx_t *ctx = (impl_retrieve_cb_ctx_t *)context;
    const afw_value_t *abort_value;
    const afw_pool_t *p;
    afw_boolean_t abort;

    abort = false;
    if (object) {
        p = (object->p) ? object->p : ctx->p;
        /** @fixme Need corresponding releases. */
        if (ctx->list) {
            afw_object_add_reference(object, xctx);
            afw_list_add_value(ctx->list,
                afw_value_create_object(object, ctx->p, xctx), xctx);
        }
        else {
            ctx->argv[0] = ctx->objectCallback;
            ctx->argv[1] = afw_value_create_object(object, p, xctx);
            ctx->argv[2] = ctx->userData;
            if (!ctx->call) {
                ctx->call = afw_value_call_create(NULL,
                    2, &ctx->argv[0], false, ctx->p, xctx);
            }
            abort_value = afw_value_evaluate(ctx->call, p, xctx);
            if (!afw_value_is_boolean(abort_value)) {
                AFW_THROW_ERROR_Z(general,
                    "objectCallback did not return boolean", xctx);
            }
            abort = ((const afw_value_boolean_t *)abort_value)->internal;
        }
    }

    return abort;
}



/* Used by retrieve_objects*_to_response() */
typedef struct impl_retrieve_to_response_cb_ctx_s {
    const afw_pool_t *p;
    const afw_content_type_t *response_content_type;
    const afw_object_options_t *object_options;
} impl_retrieve_to_response_cb_ctx_t;

static afw_boolean_t
impl_retrieve_to_response_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_retrieve_to_response_cb_ctx_t *ctx =
        (impl_retrieve_to_response_cb_ctx_t *)context;
    const afw_pool_t *p;
    const afw_value_t *object_value;
    afw_boolean_t abort;
    const afw_stream_t *response_stream;
    const afw_object_t *response_object;

    abort = false;

    if (object) {
        p = (object->p) ? object->p : ctx->p;
        response_object = afw_object_create(p, xctx);
        afw_object_set_property_as_boolean(response_object,
            &afw_s_intermediate, true, xctx);
        afw_object_set_property_as_object(response_object,
            &afw_s_result, object, xctx);
        object_value = afw_value_create_object(response_object, p, xctx);
        response_stream = afw_stream_standard(response_body, xctx);
        afw_content_type_write_value(ctx->response_content_type,
            object_value, ctx->object_options,
            (void *)response_stream, response_stream->write_cb,
            p, xctx);
        afw_stream_flush(response_stream, xctx);
        //afw_object_release(object, xctx);
    }

    return abort;
}



/* Used by retrieve_objects*_to_stream() */
typedef struct impl_retrieve_to_stream_cb_ctx_s {
    const afw_pool_t *p;
    const afw_stream_t *stream;
    const afw_content_type_t *response_content_type;
    const afw_object_options_t *object_options;
} impl_retrieve_to_stream_cb_ctx_t;

static afw_boolean_t
impl_retrieve_to_stream_cb(const afw_object_t *object, void *context,
    afw_xctx_t *xctx)
{
    impl_retrieve_to_stream_cb_ctx_t *ctx =
        (impl_retrieve_to_stream_cb_ctx_t *)context;
    const afw_pool_t *p;
    const afw_value_t *object_value;
    afw_boolean_t abort;

    abort = false;
    if (object) {
        p = (object->p) ? object->p : ctx->p;
        object_value = afw_value_create_object(object, p, xctx);
        afw_content_type_write_value(ctx->response_content_type,
            object_value, ctx->object_options,
            (void *)ctx->stream, ctx->stream->write_cb,
            p, xctx);
        afw_stream_flush(ctx->stream, xctx);
        afw_object_release(object, xctx);
    }

    return abort;
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
 *       "objectId" is the objectId assigned by the adaptor.
 */
const afw_value_t *
afw_function_execute_add_object(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId; /* Can be null. */
    const afw_value_object_t *object;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_utf8_t *object_id;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 3, object);

    object_id = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId, 4, string);
        object_id = &objectId->internal;
    }

    /* Optional journal. */
    journal = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(journal, 5, object);
    }

    /* Optional adaptorTypeSpecific. */
    adaptorTypeSpecific = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(6)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
            6, object);
    }

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Add object. */
    afw_adaptor_add_object(&adaptorId->internal,
        &objectType->internal,
        object_id, object->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal : NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
 *       "objectId" is the objectId assigned by the adaptor.
 */
const afw_value_t *
afw_function_execute_add_object_with_uri(
    afw_function_execute_t *x)
{
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *object;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri, 1, anyURI);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 2, object);

    /* Optional journal. */
    journal = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(journal, 3, object);
    }

    /* Optional adaptorTypeSpecific. */
    adaptorTypeSpecific = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
            4, object);
    }

    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }

    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Add object. */
    afw_adaptor_add_object(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        &parsed_uri->path_parsed->entity_object_id,
        object->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
 *       0 - "name=op=value" where "=op=" will be "==", "!=", "=ge=", etc. with
 *       conjunctions "&" (and) and "|" (or). Parentheses are placed around
 *       each group of like conjunctions except for outer "&". (default)
 *       
 *       1 - the same as 1 with "=gte=" instead of "=ge=", "=lte=" instead of
 *       "=le=", "=eq=" instead of "=", and "=ne=" instead of '!='.
 *       
 *       2 - similar to 1 with ';' for '&' and ',' for '|'.
 *       
 *       3 - comparisons "op(name,value)" where "op" will be "eq", "ne", "ge",
 *       ..., plus conjunctions "and(list)" and "or(list)" where "list" is a
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
    const afw_value_object_t *queryCriteria;
    const afw_value_string_t *adaptorId_value;
    const afw_value_string_t *objectType_value;
    const afw_value_integer_t *style_value;
    afw_query_criteria_style_t style;
    const afw_query_criteria_t *criteria;
    const afw_utf8_t *s;
    const afw_object_type_t *object_type;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(queryCriteria, 1, object);

    /* Get object type, if adaptorId and objectType is specified. */
    object_type = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            adaptorId_value, 2, string);
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            objectType_value, 3, string);
        journal_entry = afw_object_create_managed(x->p, x->xctx);
        object_type = afw_adaptor_get_object_type(
            &adaptorId_value->internal,
            &objectType_value->internal,
            journal_entry, x->xctx);
        if (!object_type) {
            AFW_THROW_ERROR_FZ(general, x->xctx,
                "adaptorId " AFW_UTF8_FMT
                " objectType " AFW_UTF8_FMT
                " is not found or invalid",
                AFW_UTF8_FMT_ARG(&adaptorId_value->internal),
                AFW_UTF8_FMT_ARG(&objectType_value->internal));
        }
    }
    else if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_THROW_ERROR_Z(general,
            "objectType parameter is required if adaptorId is specified",
            x->xctx);
    }

    style = afw_query_criteria_style_operator;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(
            style_value, 4, integer);
        if (style_value->internal < 0 ||
            style_value->internal > afw_query_criteria_style_max)
        {
            AFW_THROW_ERROR_Z(general, "invalid value for style parameter",
                x->xctx);
        }
        style = (afw_query_criteria_style_t)style_value->internal;
    }

    criteria = afw_query_criteria_parse_AdaptiveQueryCriteria_object(
        queryCriteria->internal, object_type, x->p, x->xctx);
    s = afw_query_criteria_to_query_string(criteria, style, x->p, x->xctx);

    return afw_value_create_string(s, x->p, x->xctx);
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
    const afw_value_string_t *queryString;
    const afw_value_string_t *adaptorId_value;
    const afw_value_string_t *objectType_value;
    const afw_query_criteria_t *criteria;
    const afw_object_t *object;
    const afw_object_type_t *object_type;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(queryString, 1, string);

    /* Get object type, if adaptorId and objectType is specified. */
    object_type = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            adaptorId_value, 2, string);
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            objectType_value, 3, string);
        journal_entry = afw_object_create_managed(x->p, x->xctx);
        object_type = afw_adaptor_get_object_type(
            &adaptorId_value->internal,
            &objectType_value->internal,
            journal_entry, x->xctx);
        if (!object_type) {
            AFW_THROW_ERROR_FZ(general, x->xctx,
                "adaptorId " AFW_UTF8_FMT
                " objectType " AFW_UTF8_FMT
                " is not found or invalid",
                AFW_UTF8_FMT_ARG(&adaptorId_value->internal),
                AFW_UTF8_FMT_ARG(&objectType_value->internal));
        }
    }
    else if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_THROW_ERROR_Z(general,
            "objectType parameter is required if adaptorId is specified",
            x->xctx);
    }

    /* Parse and return _AdaptiveQueryCriteria_ object. */
    criteria = afw_query_criteria_parse_url_encoded_rql_string(
        &queryString->internal, object_type, x->p, x->xctx);
    object = afw_query_criteria_to_AdaptiveQueryCriteria_object(
        criteria, x->p, x->xctx);
    return afw_value_create_object(object, x->p, x->xctx);
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId, 3, string);

    /* Optional journal. */
    journal = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(journal, 4, object);
    }

    /* Optional adaptorTypeSpecific. */
    adaptorTypeSpecific = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
            5, object);
    }

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Delete object. */
    afw_adaptor_delete_object(&adaptorId->internal, &objectType->internal,
        &objectId->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri, 1, anyURI);

    /* Optional journal. */
    journal = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(journal,
            2, object);
    }

    /* Optional adaptorTypeSpecific. */
    adaptorTypeSpecific = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
            3, object);
    }

    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }

    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Delete object. */
    afw_adaptor_delete_object(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        &parsed_uri->path_parsed->entity_object_id,
        journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_options_t *object_options;
    const afw_object_t *obj;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId,
        3, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        5, object);

    object_options = &afw_object_options_none;
    if (options) {
        object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    journal_entry = afw_object_create_managed(x->p, x->xctx);
    obj = afw_adaptor_get_object(
        &adaptorId->internal, &objectType->internal, &objectId->internal,
        object_options, NULL, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    if (!obj) {
        AFW_THROW_ERROR_Z(not_found, "Not found", x->xctx);
    }

    return afw_value_create_object(obj, x->p, x->xctx);
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
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *obj;
    const afw_object_t *journal_entry;
    const afw_object_options_t *object_options;
    const afw_uri_parsed_t *parsed_uri;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        1, anyURI);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        2, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        3, object);

    object_options = &afw_object_options_none;
    if (options) {
        object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }

    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }
    
    journal_entry = afw_object_create_managed(x->p, x->xctx);
    obj = afw_adaptor_get_object(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        &parsed_uri->path_parsed->entity_object_id,
        object_options, NULL, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    if (!obj) {
        AFW_THROW_ERROR_Z(not_found, "Not found", x->xctx);
    }

    return afw_value_create_object(obj, x->p, x->xctx);
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
 *       entries: list,
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
 *   entries - (list) List of modifications. Entries are of the form:
 *       
 *           [
 *               "add_value",
 *               "property name" or ["property name", ... ],
 *               value
 *           ]
 *       
 *           [
 *               "remove_property",
 *               "property name" or ["property name", ... ]
 *           ]
 *       
 *           [
 *               "remove_value",
 *               "property name" or ["property name", ... ],
 *               value
 *           ]
 *       
 *           [
 *               "set_property",
 *               "property name" or ["property name", ... ],
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;
    const afw_value_list_t *entries;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId,
        3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(entries,
        4, list);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(journal,
        5, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        6, object);

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Modify object. */
    afw_adaptor_modify_object(&adaptorId->internal, &objectType->internal,
        &objectId->internal, entries->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
 *       entries: list,
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
 *   entries - (list) List of asserts and modifications. Entries are of the
 *       form:
 *       
 *           [
 *               "add_value",
 *               "property name" or ["property name", ... ],
 *               value
 *           ]
 *       
 *           [
 *               "remove_property",
 *               "property name" or ["property name", ... ]
 *           ]
 *       
 *           [
 *               "remove_value",
 *               "property name" or ["property name", ... ],
 *               value
 *           ]
 *       
 *           [
 *               "set_property",
 *               "property name" or ["property name", ... ],
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
    const afw_value_anyURI_t *uri;
    const afw_value_list_t *entries;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        1, anyURI);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(entries,
        2, list);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(journal,
        3, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        4, object);

    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }

    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Modify object. */
    afw_adaptor_modify_object(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        &parsed_uri->path_parsed->entity_object_id,
        entries->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
    const afw_value_object_t *object;
    const afw_value_boolean_t *checkOnly;
    const afw_utf8_t *reconcilable;
    const afw_value_t *reconcilable_value;
    const afw_object_t *original;
    const afw_object_t *journal_entry;
    const afw_utf8_t *adaptor_id;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *object_id;
    const afw_list_t *entries;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    checkOnly = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(checkOnly,
            2, boolean);
    }

    reconcilable = afw_object_meta_get_property_as_string(
        object->internal, &afw_s_reconcilable, x->xctx);
    if (!reconcilable) {
        AFW_THROW_ERROR_Z(general,
            "object is not reconcilable",
            x->xctx);
    }
    reconcilable_value = afw_json_to_value(reconcilable, &afw_s_reconcilable,
        x->p, x->xctx);
    AFW_VALUE_ASSERT_IS_DATA_TYPE(reconcilable_value, object, x->xctx);
    original = ((const afw_value_object_t *)reconcilable_value)->internal;

    /* Create journal entry including optional properties. */
    journal_entry = afw_object_create_managed(x->p, x->xctx);

    /* Make modify needed to reconcile. */
    afw_adaptor_modify_needed_to_reconcile(
        &adaptor_id, &object_type_id, &object_id, &entries,
        original, object->internal,
        journal_entry, x->p, x->xctx);

    /* If checkOnly, just add return journal entry with modify entries. */
    if (checkOnly && checkOnly->internal) {
        /** @fixme Make part of journal object. */
        return afw_value_create_list(entries, x->p, x->xctx);
    }


    /* If not checkOnly, call modify object using entries. */
    else {
        afw_adaptor_modify_object(
            adaptor_id, object_type_id, object_id, entries,
            journal_entry, NULL, x->xctx);
    }

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;
    const afw_value_object_t *object;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_utf8_t *id;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId,
        3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(journal,
        5, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        6, object);

    id = (objectId && objectId->internal.len > 0)
        ? &objectId->internal
        : NULL;

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Replace object. */
    afw_adaptor_replace_object(&adaptorId->internal, &objectType->internal,
        id, object->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *object;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;
    const afw_utf8_t *id;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        1, anyURI);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object,
        2, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(journal,
        3, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        4, object);

    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }

    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    id = (parsed_uri->path_parsed->entity_object_id.len > 0)
        ? &parsed_uri->path_parsed->entity_object_id
        : NULL;

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Replace object. */
    afw_adaptor_replace_object(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        id, object->internal,
        journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
 *   ): list;
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
 *   (list) This is the list of objects retrieved.
 */
const afw_value_t *
afw_function_execute_retrieve_objects(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_object_t *queryCriteria;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_value_object_t *options;
    const afw_query_criteria_t * criteria;
    const afw_object_t *journal_entry;
    impl_retrieve_cb_ctx_t ctx;

    criteria = NULL;
    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.list = afw_list_of_create(afw_data_type_object, x->p, x->xctx);
    criteria = NULL;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        2, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(queryCriteria,
        3, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        5, object);

    journal_entry = afw_object_create_managed(x->p, x->xctx);

    /* Optional query criteria. */
    if (queryCriteria) {
        criteria = afw_adaptor_query_criteria_parse_object(
            queryCriteria->internal,
            &adaptorId->internal, &objectType->internal,
            journal_entry, x->p, x->xctx);
    }

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Retrieve objects. */
    afw_adaptor_retrieve_objects(
        &adaptorId->internal, &objectType->internal,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    return afw_value_create_list(ctx.list, x->p, x->xctx);
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
 *       retrieved instead of adding the object to the return list. Parameter
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_object_t *queryCriteria;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_value_object_t *options;
    const afw_query_criteria_t *criteria;
    const afw_object_t *journal_entry;
    impl_retrieve_cb_ctx_t ctx;

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    criteria = NULL;

    AFW_FUNCTION_EVALUATE_PARAMETER(ctx.objectCallback,
        1);
    AFW_FUNCTION_EVALUATE_PARAMETER(ctx.userData,
        2);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        4, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(queryCriteria,
        5, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        6, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        7, object);

    journal_entry = afw_object_create_managed(x->p, x->xctx);

    /* Optional query criteria. */
    if (queryCriteria) {
        criteria = afw_adaptor_query_criteria_parse_object(
            queryCriteria->internal,
            &adaptorId->internal, &objectType->internal,
            journal_entry, x->p, x->xctx);
    }

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Retrieve objects. */
    afw_adaptor_retrieve_objects(
        &adaptorId->internal, &objectType->internal,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal : NULL,
        x->p, x->xctx);

    /* Return undefined. */
    return NULL;
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
 * "application/x-afw". An "_AdaptiveResponse_" object is written to the
 * request's response as each object is retrieved. This "_AdaptiveResponse_"
 * object has a "result" property containing the retrieved object and a
 * "intermediate" property set to true.
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_object_t *queryCriteria;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_value_object_t *options;
    const afw_query_criteria_t *criteria;
    const afw_object_t *journal_entry;
    impl_retrieve_to_response_cb_ctx_t ctx;

    if (!x->xctx->request ||
        !x->xctx->request->response_content_type ||
        !afw_content_type_is_application_afw(
            x->xctx->request->response_content_type))
    {
        AFW_THROW_ERROR_Z(general,
            "retrieve_objects_to_response() "
            "is only allowed to be called "
            "during a request with response content type "
            "\"application/x-afw\".",
            x->xctx);
    }

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.response_content_type = x->xctx->request->response_content_type;
    criteria = NULL;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType, 2, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(queryCriteria,
        3, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        5, object);

    journal_entry = afw_object_create_managed(x->p, x->xctx);

    /* Optional query criteria. */
    if (queryCriteria) {
        criteria = afw_adaptor_query_criteria_parse_object(
            queryCriteria->internal,
            &adaptorId->internal, &objectType->internal,
            journal_entry, x->p, x->xctx);
    }

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Retrieve objects. */
    afw_adaptor_retrieve_objects(
        &adaptorId->internal, &objectType->internal,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_to_response_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    return NULL;
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
 * Each object is written to the "streamId" stream as it's retrieved.
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
    const afw_value_integer_t *streamNumber;
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_object_t *queryCriteria;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_value_string_t *contextType;
    const afw_query_criteria_t *criteria;
    const afw_object_t *adaptor_type_specific;
    const afw_object_t *journal_entry;
    impl_retrieve_to_stream_cb_ctx_t ctx;

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    criteria = NULL;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        3, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(queryCriteria,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        5, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        6, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(contextType, 7, string);

    journal_entry = afw_object_create_managed(x->p, x->xctx);

    /* Stream. */
    ctx.stream = afw_stream_get_by_streamNumber(
        streamNumber->internal, x->xctx);
    if (!ctx.stream || !ctx.stream->write_cb) {
        AFW_THROW_ERROR_FZ(general, x->xctx,
            "streamNumber %" AFW_INTEGER_FMT " is not an open output stream",
            streamNumber->internal);
    }

    /* Optional query criteria. */
    if (queryCriteria) {
        criteria = afw_adaptor_query_criteria_parse_object(
            queryCriteria->internal,
            &adaptorId->internal, &objectType->internal,
            journal_entry, x->p, x->xctx);
    }

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Optional adaptorTypeSpecific. */
    adaptor_type_specific = NULL;
    if (adaptorTypeSpecific) {
        adaptor_type_specific = adaptorTypeSpecific->internal;
    }

    /* Optional contentType. */
    if (contextType) {
        ctx.response_content_type = afw_environment_get_content_type(
            &contextType->internal, x->xctx);
        if (!ctx.response_content_type) {
            AFW_THROW_ERROR_FZ(general, x->xctx,
                "Unknown contentType " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&contextType->internal));
        }
    }
    else if (x->xctx->request && x->xctx->request->response_content_type)
    {
        ctx.response_content_type = x->xctx->request->response_content_type;
    }
    else {
        ctx.response_content_type = afw_json_content_type_get();
    }

    /* Retrieve objects. */
    afw_adaptor_retrieve_objects(
        &adaptorId->internal, &objectType->internal,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_to_stream_cb,
        adaptor_type_specific,
        x->p, x->xctx);

    /* Return undefined. */
    return NULL;
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
 *   ): list;
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
 *   (list) This is the list of objects retrieved.
 */
const afw_value_t *
afw_function_execute_retrieve_objects_with_uri(
    afw_function_execute_t *x)
{
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;
    const afw_query_criteria_t *criteria;
    impl_retrieve_cb_ctx_t ctx;

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.list = afw_list_of_create(afw_data_type_object, x->p, x->xctx);
    criteria = NULL;
    journal_entry = afw_object_create_managed(x->p, x->xctx);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        1, anyURI);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        2, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        3, object);

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Parse URI to get adaptorId and objectType. */
    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }
    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Parse optional query string. */
    if (parsed_uri->original_query.len > 0) {
        criteria = afw_adaptor_query_criteria_parse_url_encoded_rql_string(
            &parsed_uri->original_query, 
            &parsed_uri->path_parsed->adaptor_id,
            &parsed_uri->path_parsed->object_type_id,
            journal_entry, x->p, x->xctx);
    }

    /* Retrieve objects. */
    afw_adaptor_retrieve_objects(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    /* Return retrieved objects as a list. */
    return afw_value_create_list(ctx.list, x->p, x->xctx);
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
 *       retrieved instead of adding the object to the return list. Parameter
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
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;
    const afw_query_criteria_t *criteria;
    impl_retrieve_cb_ctx_t ctx;

    afw_memory_clear(&ctx);
    journal_entry = afw_object_create_managed(x->p, x->xctx);
    criteria = NULL;
    afw_memory_clear(&ctx);
    ctx.p = x->p;

    AFW_FUNCTION_EVALUATE_PARAMETER(ctx.objectCallback,
        1);
    AFW_FUNCTION_EVALUATE_PARAMETER(ctx.userData,
        2);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        3, anyURI);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        5, object);

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Parse URI to get adaptorId and objectType. */
    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }
    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Parse optional query string. */
    if (parsed_uri->original_query.len > 0) {
        criteria = afw_adaptor_query_criteria_parse_url_encoded_rql_string(
            &parsed_uri->original_query,
            &parsed_uri->path_parsed->adaptor_id,
            &parsed_uri->path_parsed->object_type_id,
            journal_entry, x->p, x->xctx);
    }

    /* Retrieve objects. */
    afw_adaptor_retrieve_objects(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    /* Return retrieved objects as a list. */
    return afw_value_create_list(ctx.list, x->p, x->xctx);
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
 * "application/x-afw". An "_AdaptiveResponse_" object is written to the
 * request's response as each object is retrieved. This "_AdaptiveResponse_"
 * object has a "result" property containing the retrieved object and a
 * "partial" property set to true.
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
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;
    const afw_query_criteria_t *criteria;
    impl_retrieve_to_response_cb_ctx_t ctx;


    if (!x->xctx->request ||
        !x->xctx->request->response_content_type ||
        !afw_content_type_is_application_afw(
            x->xctx->request->response_content_type))
    {
        AFW_THROW_ERROR_Z(general,
            "retrieve_objects_with_uri_to_response() "
            "is only allowed to be called "
            "during a request with response content type "
            "\"application/x-afw\".",
            x->xctx);
    }

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.response_content_type = x->xctx->request->response_content_type;
    criteria = NULL;
    journal_entry = afw_object_create_managed(x->p, x->xctx);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        1, anyURI);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        2, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        3, object);

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Parse URI to get adaptorId and objectType. */
    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }
    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Parse optional query string. */
    if (parsed_uri->original_query.len > 0) {
        criteria = afw_adaptor_query_criteria_parse_url_encoded_rql_string(
            &parsed_uri->original_query,
            &parsed_uri->path_parsed->adaptor_id,
            &parsed_uri->path_parsed->object_type_id,
            journal_entry, x->p, x->xctx);
    }

    /* Retrieve objects to response. */
    afw_adaptor_retrieve_objects(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_to_response_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    /* Return undefined. */
    return NULL;
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
 * Each object is written to the "streamId" stream as it's retrieved.
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
    const afw_value_integer_t *streamNumber;
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *options;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;
    const afw_query_criteria_t *criteria;
    impl_retrieve_to_stream_cb_ctx_t ctx;

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    criteria = NULL;
    journal_entry = afw_object_create_managed(x->p, x->xctx);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        2, anyURI);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(options,
        3, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        4, object);

    /* Stream. */
    ctx.stream = afw_stream_get_by_streamNumber(
        streamNumber->internal, x->xctx);
    if (!ctx.stream || !ctx.stream->write_cb) {
        AFW_THROW_ERROR_FZ(general, x->xctx,
            "streamNumber %" AFW_INTEGER_FMT " is not an open output stream",
            streamNumber->internal);
    }

    /* Optional options. */
    if (options) {
        ctx.object_options = afw_object_options_set_from_object(NULL,
            options->internal, x->p, x->xctx);
    }

    /* Parse URI to get adaptorId and objectType. */
    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }
    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    /* Parse optional query string. */
    if (parsed_uri->original_query.len > 0) {
        criteria = afw_adaptor_query_criteria_parse_url_encoded_rql_string(
            &parsed_uri->original_query,
            &parsed_uri->path_parsed->adaptor_id,
            &parsed_uri->path_parsed->object_type_id,
            journal_entry, x->p, x->xctx);
    }

    /* Retrieve objects to response. */
    afw_adaptor_retrieve_objects(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        ctx.object_options, criteria, journal_entry, (void *)&ctx,
        impl_retrieve_to_stream_cb,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->p, x->xctx);

    /* Return undefined. */
    return NULL;
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
 *       embedded objects and lists, as well as to modify individual values of
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
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;
    const afw_value_object_t *object;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_utf8_t *id;
    const afw_object_t *journal_entry;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType,
        2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId,
        3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object,
        4, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(journal,
        5, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        6, object);

    id = (objectId && objectId->internal.len > 0)
        ? &objectId->internal
        : NULL;

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Update object. */
    afw_adaptor_modify_using_update_object(
        &adaptorId->internal, &objectType->internal,
        id, object->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
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
 *       embedded objects and lists, as well as to modify individual values of
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
    const afw_value_anyURI_t *uri;
    const afw_value_object_t *object;
    const afw_value_object_t *journal;
    const afw_value_object_t *adaptorTypeSpecific;
    const afw_object_t *journal_entry;
    const afw_uri_parsed_t *parsed_uri;
    const afw_utf8_t *id;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri,
        1, anyURI);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object,
        2, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(journal,
        3, object);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(adaptorTypeSpecific,
        4, object);

    parsed_uri = afw_uri_parse(&uri->internal, true, NULL, x->p, x->xctx);
    if (!parsed_uri) {
        AFW_THROW_ERROR_Z(general, "Parameter uri is not valid", x->xctx);
    }

    if (!parsed_uri->path_parsed ||
        parsed_uri->path_parsed->first_property_name)
    {
        AFW_THROW_ERROR_Z(general,
            "Only local entity object paths are currently supported", x->xctx);
    }

    id = (parsed_uri->path_parsed->entity_object_id.len > 0)
        ? &parsed_uri->path_parsed->entity_object_id
        : NULL;

    /* Create journal entry including optional properties. */
    journal_entry = impl_create_journal_entry(journal, x->p, x->xctx);

    /* Update object. */
    afw_adaptor_modify_using_update_object(
        &parsed_uri->path_parsed->adaptor_id,
        &parsed_uri->path_parsed->object_type_id,
        id, object->internal, journal_entry,
        (adaptorTypeSpecific) ? adaptorTypeSpecific->internal: NULL,
        x->xctx);

    /* Return journal entry as response. */
    return afw_value_create_object(journal_entry, x->p, x->xctx);
}



/*
 * Adaptive function: adaptor_objectCallback_signature
 *
 * afw_function_execute_adaptor_objectCallback_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for the objectCallback parameter in adaptor
 * functions. Calling this directly will throw a "Do not call directly" error.
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
    AFW_THROW_ERROR_Z(general, "Do not call directly", x->xctx);
}

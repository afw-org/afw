// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_request_handler interface for adapter
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_request_handler_adapter.c
 * @brief Implementation of afw_request_handler interface for adapter
 *
 * This request handler is used to interface with an adapter.  If
 * configured to do so, it supports pre and post validation and
 * authorization.
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_request_handler */
#define AFW_IMPLEMENTATION_ID "adapter"
#include "afw_request_handler_impl_declares.h"


typedef struct {
    const afw_request_t *request;
    const afw_object_t *journal_entry;
    const afw_content_type_t *content_type;
    const afw_content_type_object_list_writer_t *writer;
    const afw_object_options_t *options;
    afw_boolean_t called_once;
} impl_retrieve_cb_context_t;


AFW_DEFINE(const afw_request_handler_t *)
afw_request_handler_adapter_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_request_handler_adapter_internal_self_t *self;
    const afw_object_t *options_object;

    self = afw_pool_calloc_type(p, afw_request_handler_adapter_internal_self_t, xctx);

    self->pub.inf = &impl_afw_request_handler_inf;
    self->xctx = xctx;
    self->p = xctx->p;
    self->properties = properties;
    
    options_object = afw_object_old_get_property_as_object(self->properties,
        afw_s_defaultOptions, xctx);
    if (options_object) {
        self->default_options = afw_object_options_set_from_object(
            NULL, options_object, p, xctx);
    }

    /* Return self */
    return (const afw_request_handler_t *)self;
}

/* Callback function for retrieve. */
static afw_boolean_t impl_retrieve_cb(const afw_object_t *object,
    void *context, afw_xctx_t *xctx)
{
    impl_retrieve_cb_context_t *ctx = context;

    /* If first call, write begin object list. */
    if (!ctx->content_type) {
        ctx->content_type =
            afw_request_prepare_response_content_type(ctx->request, xctx);
        ctx->writer = afw_content_type_create_object_list_writer(
            ctx->content_type, ctx->options,
            (void *)ctx->request, ctx->request->write_content_cb,
            xctx->p, xctx);
    }

    /* If there is an object, write it. */
    if (object) {
        afw_content_type_object_list_writer_write_object(ctx->writer, object,
            xctx->p, xctx);
        /** @fixme Flush is not really necessary, but might make object available to to client sooner. */
        /*afw_request_flush_response(ctx->request, xctx);*/
    }

    /* If there are not more objects, release writer. */
    else {
        afw_content_type_object_list_writer_release(ctx->writer, xctx);
    }

    /* Return indicating not to short circuit */
    return false;
}

/*
 * Implementation of method release of interface afw_request_handler.
 */
void
impl_afw_request_handler_release(
    const afw_request_handler_t * instance,
    afw_xctx_t *xctx)
{
    /* Resources will be released when execution context (xctx) is released. */
}


/*
 * Implementation of method process of interface afw_request_handler.
 */
void
impl_afw_request_handler_process(
    const afw_request_handler_t * instance,
    const afw_request_t * request,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_query_criteria_t * query_criteria;
    const afw_object_t *obj;
    const afw_object_path_parsed_t *parsed_path;
    const afw_object_t *journal_entry;
    const afw_content_type_t *response_content_type;
    const afw_stream_t *stream;
    impl_retrieve_cb_context_t retrieve_cb_context;

    afw_request_handler_adapter_internal_self_t * self = (afw_request_handler_adapter_internal_self_t *)instance;

    /* Parse the path. */
    parsed_path = afw_object_path_parse(request->uri, NULL, self->default_options,
        xctx->p, xctx);
    if (!parsed_path) {
        AFW_THROW_ERROR_Z(not_found, "Path format error.", xctx);
    }

    /* Create journal entry memory object. */
    journal_entry = afw_object_create(xctx->p, xctx);

    /* Allow journal entry to be used as additional error info. */
    afw_request_set_error_info(request, journal_entry, xctx);


    /*
     * Process based on HTTP method.
     *
     * Notes for using dojo/store/JsonRest:
     * 
     *  The Dojo documentation recommends using /{Table}/{id} as the URL
     *  structure for resources.  This whole string is considered URI of
     *  the adaptive object. {Table} is the adapter id. {id} is the object id.
     *
     *  FIXME fix comment
     * 
     *  /{Table} is the request->uri up to, but not including, the second
     *  slash.  The /{id} is the second slash and remainder of request->uri.
     *
     */

    AFW_TRY {

        /* If retrieve_objects or get objects: */
        if (afw_utf8_equal_utf8_z(request->method, AFW_REQUEST_Q_METHOD_GET))
        {
            /* Object type required. */
            if (parsed_path->object_type_id.len == 0) {
                /** @fixme Make user error. */
                AFW_THROW_ERROR_Z(general, "URI missing Object type", xctx);
            }

            /* If query_objects(), which is a GET without an object_id. */
            else if (parsed_path->entity_object_id.len == 0)
            {

                /* If query string specified, create query criteria. */
                query_criteria = NULL;
                if (request->query_string && request->query_string->len > 0) {
                    query_criteria =
                        afw_adapter_query_criteria_parse_url_encoded_rql_string(
                            request->query_string,
                            &parsed_path->adapter_id,
                            &parsed_path->object_type_id,
                            journal_entry,
                            xctx->p, xctx);
                }

                /* Prepare retrieve callback context. */
                memset(&retrieve_cb_context, 0, sizeof(retrieve_cb_context));
                retrieve_cb_context.request = request;
                retrieve_cb_context.journal_entry = journal_entry;
                retrieve_cb_context.options = parsed_path->options;

                /* Call adapter to query for objects. */
                afw_adapter_retrieve_objects(
                    &parsed_path->adapter_id, &parsed_path->object_type_id,
                    parsed_path->options, query_criteria, journal_entry,
                    &retrieve_cb_context, impl_retrieve_cb, 
                    NULL, xctx->p, xctx);
            }

            /* If get_object(), which is a GET where path does not end with '/'. */
            else {

                /* If query string specified, create query criteria. */
                query_criteria = NULL;
                if (request->query_string && request->query_string->len > 0) {
                    query_criteria =
                        afw_adapter_query_criteria_parse_url_encoded_rql_string(
                            request->query_string,
                            &parsed_path->adapter_id,
                            &parsed_path->object_type_id,
                            journal_entry, xctx->p, xctx);
                }

                /* Get object cache. */
                obj = afw_adapter_get_object(
                    &parsed_path->adapter_id, &parsed_path->object_type_id,
                    &parsed_path->entity_object_id,
                    parsed_path->options, query_criteria,
                    journal_entry, NULL, xctx->p, xctx);

                /* If object not found, indicate. */
                if (!obj) {
                    value = NULL;
                }

                /* If there is a property name in path, get it. */
                else if (parsed_path->first_property_name) {
                    value = afw_object_path_property_name_list_get_property(obj,
                        parsed_path->first_property_name, xctx);
                }
                
                /* If no property name in path, obj is value. */
                else {
                    value = afw_value_create_unmanaged_object(
                        obj, xctx->p, xctx);
                }

                /* If value not found, throw error. */
                if (!value) {
                    AFW_THROW_ERROR_FZ(not_found, xctx,
                        AFW_UTF8_FMT_Q " Not found.",
                        AFW_UTF8_FMT_ARG(request->uri));
                }

                /* Write value to response body. */
                afw_request_write_value_to_response_body(request, value,
                    parsed_path->options, xctx);
            }
        }

        else if (afw_utf8_equal_utf8_z(request->method,
            AFW_REQUEST_Q_METHOD_PUT))
        {
            /*
             * Adaptive method Replace.
             *
             * The PUT method is used to completely replace an existing object
             * whose object id is know.
             *
             * The body of the request must contain a JSON object containing the
             * complete object.
             *
             * For now, If_None_Match and If_Match are ignore.  If_None_Match:*
             * is checked in the future, it will produce an error.  If _Match:*
             * is checked, it will basically be ignore since in the adaptive
             * rest api, PUT is always a complete replacement or an existing
             * object.
             *
             * In DOJO, specify an id, incremental false, and overwrite true
             * to produce a Replace request.
             */
            value = afw_request_body_to_value(request, xctx->p, xctx);
            value = afw_value_undecorate(value);
            if (!value || !afw_value_is_object(value)) {
                AFW_THROW_ERROR_Z(general, "Invalid post data", xctx);
            }
            obj = ((const afw_value_object_t *)value)->internal;

            if (parsed_path->entity_object_id.len == 0) {
                AFW_THROW_ERROR_Z(general, "Object id missing in URI for replace.", xctx);
            }
            if (parsed_path->first_property_name) {
                AFW_THROW_ERROR_Z(general,
                    "Object id can not have dotted property name for replace.", xctx);
            }
            /** @fixme Remove */ afw_object_meta_set_ids_using_path(obj,
                &parsed_path->normalized_path, xctx);
            afw_adapter_replace_object(&parsed_path->adapter_id,
                &parsed_path->object_type_id, &parsed_path->entity_object_id,
                obj, journal_entry, NULL, xctx);
            afw_request_write_success_response(request, journal_entry, xctx);
        }

        else if (afw_utf8_equal_utf8_z(request->method,
            AFW_REQUEST_Q_METHOD_POST))
        {
            /*
             * Adaptive methods Add, Update, Modify and Publish.
             *
             * The POST method is used to add a new object or update/modify an
             * existing object.
             *
             * Perform actions request:
             *
             * POST /afw
             *
             * A request to perform actions.
             * See /afw/_AdaptiveObjectType_/_AdaptiveActions_ for more
             * information.
             *
             *
             * Just adapter_id specified.
             *
             * POST /<adapter_id>
             *
             * Reserved.  An error for now.
             *
             *
             * Method Modify:
             *
             * Modify an object using action tuples.
             *
             * POST /<adapter_id>/<object_type>/<object_id>
             *
             * Request data is a list of action tuples.
             *
             *
             * Method Add:
             *
             * Add adds a new object.
             *
             * POST /<adapter_id>/<object_type>
             *
             * An Add request does not specify an object id.  The body of the
             * request must be an object containing the complete object.
             *
             * In DOJO, do not specify id, specify incremental false and overwrite
             * false.
             *
             *
             * Method Update:
             *
             * POST /<adapter_id>/<object_type>/<object_id>
             *
             * Request data is an object.
             *
             * Update replaces properties of an existing object with new values.
             * The body of the request must be a JSON object containing properties
             * to replace in the existing object.  Any properties in the existing
             * object that are not specified in the Update remain unchanged.  If
             * a property is specified with no value, it is removed from the
             * existing object.
             *
             * In DOJO, specify id, incremental true and overwrite true.
             *
             * For now, If_None_Match and If_Match are ignore.
             *
             * If If_None_Match: * header is present, the object will not be
             * overwritten.
             *
             * If If_Match: * header is present, the object must already exist
             * and will be overwritten.
             *
             */
            value = afw_request_body_to_value(request, xctx->p, xctx);
            if (!value) {
                AFW_THROW_ERROR_Z(general, "Post data missing", xctx);
            }
            value = afw_value_undecorate(value);

            /* If Modify ... */
            if (afw_value_is_array(value)) {
                if (parsed_path->object_type_id.len == 0 ||
                    parsed_path->entity_object_id.len == 0)
                {
                    AFW_THROW_ERROR_Z(general,
                        "Modify requires object type and object id", xctx);
                }
                if (parsed_path->first_property_name) {
                    AFW_THROW_ERROR_Z(general,
                        "Object id can not have dotted property name for modify.", xctx);
                }
                afw_adapter_modify_object(&parsed_path->adapter_id,
                    &parsed_path->object_type_id, &parsed_path->entity_object_id,
                    ((const afw_value_array_t *)value)->internal, journal_entry,
                    NULL, xctx);
            }

            else if (afw_value_is_object(value)) {
                obj = ((const afw_value_object_t *)value)->internal;

                /* Perform actions if just /afw and no object type.  */
                if (parsed_path->object_type_id.len == 0 &&
                    afw_utf8_equal(&parsed_path->adapter_id, afw_s_afw))
                {
                    response_content_type = afw_request_prepare_response_content_type(
                        request, xctx);
                    journal_entry = afw_action_perform(
                        obj, response_content_type, journal_entry, xctx->p, xctx);
                }

                /* An error if just adapter_id other than afw. */
                else if (parsed_path->object_type_id.len == 0) {
                    AFW_THROW_ERROR_Z(general, "Invalid post path", xctx);
                }

                /* If Add ... */
                else if (parsed_path->entity_object_id.len == 0) {
                    afw_adapter_add_object(&parsed_path->adapter_id,
                        &parsed_path->object_type_id, NULL, obj, journal_entry,
                        /*FIXME */ NULL, xctx);
                }

                /* If Update ... */
                else {
                    if (parsed_path->first_property_name) {
                        AFW_THROW_ERROR_Z(general,
                            "Object id can not have dotted property name for update.",
                            xctx);
                    }
                    /** @fixme Remove. */afw_object_meta_set_ids_using_path(obj,
                        &parsed_path->normalized_path, xctx);
                    afw_adapter_modify_using_update_object(
                        &parsed_path->adapter_id,
                        &parsed_path->object_type_id,
                        &parsed_path->entity_object_id,
                        ((const afw_value_object_t *)value)->internal,
                        journal_entry, NULL, xctx);
                }
            }

            /* Not single_list or single_object, error. */
            else {
                AFW_THROW_ERROR_Z(general, "Invalid post data", xctx);
            }

            /* Write success response. */
            afw_request_write_success_response(request, journal_entry, xctx);
        }

        /* If delete_object() */
        else if (afw_utf8_equal_utf8_z(request->method,
            AFW_REQUEST_Q_METHOD_DELETE))
        {
            /* Call adapter to delete object. */
            if (parsed_path->first_property_name) {
                AFW_THROW_ERROR_Z(general,
                    "Object id can not have dotted property name for delete.",
                    xctx);
            }
            afw_adapter_delete_object(
                &parsed_path->adapter_id, &parsed_path->object_type_id,
                &parsed_path->entity_object_id, journal_entry, /*FIXME */ NULL,
                xctx);
            afw_request_write_success_response(request, journal_entry, xctx);
        }

        /* Else not supported method. */
        else {
            AFW_THROW_ERROR_Z(general, "Method not supported.", xctx);
        }

    }

    AFW_CATCH_UNHANDLED {

        /* Abort changes and release cached sessions and objects. */
        afw_adapter_session_commit_and_release_cache(true, xctx);

        AFW_ERROR_RETHROW;
    }

    AFW_ENDTRY;

    /* Commit changes and release cached sessions and objects. */
    afw_adapter_session_commit_and_release_cache(false, xctx);

    /* If response_content_type application/x-afw set, write end frame. */
    if (afw_content_type_is_application_afw(request->response_content_type)) {
        stream = afw_stream_standard(raw_response_body, xctx);
        afw_stream_write_integer(stream, ++xctx->write_sequence, xctx);
        afw_stream_write_z(stream, " 0 end\n", xctx);
    }

    /* Finish response. */
    afw_request_finish_response(request, xctx);
}

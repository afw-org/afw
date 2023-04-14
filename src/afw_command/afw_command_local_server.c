// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#include "afw.h"
#include "afw_command_internal.h"
#include "afw_command_local_server.h"
#include "afw_command_local_request.h"
#include "afw_command_local_parse.h"

/* Declares and rti/inf defines for interface afw_server */
#define AFW_IMPLEMENTATION_ID "afw_command_local"
#include "afw_server_impl_declares.h"

/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command_local_server.c
 * @brief Implementation for interface afw_command_local
 *
 * This is the implementation of afw_server for afw_command_local.
 */

/* Compiled afw version. */
static const afw_utf8_t
impl_compiled_afw_version =
AFW_UTF8_LITERAL(AFW_VERSION_STRING);

static const afw_utf8_t
impl_default_afw_conf = AFW_UTF8_LITERAL(
"[\n"
"    {\n"
"        \"type\"                : \"requestHandler\",\n"
"        \"uriPrefix\"           : \"/\",\n"
"        \"requestHandlerType\"  : \"adaptor\"\n"
"    }\n"
"]\n"
);


/* Get input in local mode. */
static const afw_memory_t *
impl_local_get_input(
    afw_command_local_server_self_t *self,
    afw_xctx_t *xctx)
{
    afw_memory_t *result;
    long long len;
    int rv;
    int c;
    
    /* If eof, just return NULL. */
    if (self->eof) {
        return NULL;
    }

    /* Read chunks into input buffer. */
    apr_array_clear(self->input_buffer);
    for (;;) {

        /*
         * Get and unget first char to make sure it is a digit.  Function
         * fscanf() can skip characters, so this is here to make sure
         * a stealth chunk error doesn't occur.
         */
        c = fgetc(self->fd_input);
        if (c == EOF) {
            if (self->input_buffer->nelts != 0) {
                goto error;
            }
            return NULL;
        }
        if (c < '0' || c > '9') {
            goto error;
        }
        rv = ungetc(c, self->fd_input);
        if (rv != c) {
            goto error;
        }

        /* Get len followed by \n. If 0\n, break. */
        rv = fscanf(self->fd_input, "%lld", &len);
        if (rv < 0 || rv != 1) {
            goto error;
        }
        c = fgetc(self->fd_input);
        if (c != '\n') {
            goto error;
        }
        if (len == 0) {
            break;
        }

        /* Push the characters from chunk on input buffer. */
        for (; len > 0; len--) {
            c = fgetc(self->fd_input);
            if (c < 0 || c > 255) {
                goto error;
            }
            APR_ARRAY_PUSH(self->input_buffer, unsigned char) = c;
        }
    }

    /* Return result. */
    result = afw_xctx_malloc_type(afw_memory_t, xctx);
    result->ptr = (const afw_byte_t *)self->input_buffer->elts;
    result->size = self->input_buffer->nelts;
    return result;

error:
    self->fatal_error = true;
    AFW_THROW_ERROR_Z(general, "Invalid chunk", xctx);
}


static const afw_object_t *
impl_get_directive_input(
    afw_command_local_server_self_t *self,
    const afw_utf8_t *string,
    afw_size_t directive_len,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    afw_utf8_t s;

    if (string->len == directive_len) {
        return afw_object_create_managed(self->pub.xctx->p, self->pub.xctx);
    }

    if (string->s[directive_len] != ':') {
        AFW_THROW_ERROR_Z(general, "Missing ':'", xctx);
    }

    s.s = string->s + directive_len + 1;
    s.len = string->len - directive_len - 1;
    afw_data_type_convert_internal(afw_data_type_string,
        &result, &s, afw_data_type_object, self->pub.xctx->p, self->pub.xctx);

    return result;
}



static void
impl_process_directive(
    afw_command_local_server_self_t *self,
    const afw_memory_t *input,
    afw_xctx_t *xctx)
{
    afw_utf8_t partial;
    const afw_utf8_t *string;
    afw_boolean_t is_invalid;

    is_invalid = true;

    /* Create string from input in current's requests memory so it will free. */
    string = afw_utf8_create(
        (const afw_utf8_octet_t *)input->ptr, input->size,
        xctx->p, xctx);

    /* ++afw-local-mode-evaluate-direct */
    if (afw_utf8_starts_with(string,
        &afw_command_s_a_local_mode_evaluate_direct))
    {
        self->mode = afw_command_local_server_mode_evaluate_direct;
        if (self->multi_request_mode_properties) {
            afw_object_release(self->multi_request_mode_properties,
                self->pub.xctx);
        }
        self->multi_request_mode_properties = impl_get_directive_input(self,
            string, afw_command_s_a_local_mode_evaluate_direct.len, xctx);
        is_invalid = false;       
    }

    /* ++afw-local-mode-evaluate */
    else if (afw_utf8_starts_with(string,
        &afw_command_s_a_local_mode_evaluate))
    {
        self->mode = afw_command_local_server_mode_evaluate;
        if (self->multi_request_mode_properties) {
            afw_object_release(self->multi_request_mode_properties,
                self->pub.xctx);
        }
        self->multi_request_mode_properties = impl_get_directive_input(self,
            string, afw_command_s_a_local_mode_evaluate.len, xctx);
        is_invalid = false;       
    }

    /* ++afw-local-mode-action-direct */
    else if (afw_utf8_starts_with(string,
        &afw_command_s_a_local_mode_action_direct))
    {
        self->mode = afw_command_local_server_mode_action_direct;
        if (self->multi_request_mode_properties) {
            afw_object_release(self->multi_request_mode_properties,
                self->pub.xctx);
        }
        self->multi_request_mode_properties = impl_get_directive_input(self,
            string, afw_command_s_a_local_mode_action_direct.len, xctx);            
        is_invalid = false;       
    }

    /* ++afw-local-mode-action */
    else if (afw_utf8_starts_with(string,
        &afw_command_s_a_local_mode_action))
    {
        self->mode = afw_command_local_server_mode_action;
        if (self->multi_request_mode_properties) {
            afw_object_release(self->multi_request_mode_properties,
                self->pub.xctx);
        }
        self->multi_request_mode_properties = impl_get_directive_input(self,
            string, afw_command_s_a_local_mode_action.len, xctx);            
        is_invalid = false;       
    }

    /* ++afw-local-mode-http-like */
    else if (afw_utf8_starts_with(string,
        &afw_command_s_a_local_mode_http_like))
    {
        self->mode = afw_command_local_server_mode_http_like;
        if (self->multi_request_mode_properties) {
            afw_object_release(self->multi_request_mode_properties,
                self->pub.xctx);
        }
        self->multi_request_mode_properties = impl_get_directive_input(self,
            string, afw_command_s_a_local_mode_http_like.len, xctx);            
        is_invalid = false;       
    }

    /* ++afw-local-multi-request-properties */
    else if (afw_utf8_starts_with(string,
        &afw_command_s_a_local_request_properties))
    {
        if (self->multi_request_properties) {
            afw_object_release(self->multi_request_properties, self->pub.xctx);
        }
        self->multi_request_properties = impl_get_directive_input(self,
            string, afw_command_s_a_local_request_properties.len, xctx); 
        is_invalid = false;       
    }

    /* Invalid directive. */
    if (is_invalid) {
        if (string->len <= 30) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid directive: %" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(string));
        }
        else {
            partial.s = string->s;
            partial.len = 30;
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid directive beginning: %" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&partial));
        }
    }
}



static afw_boolean_t
impl_read_and_process_request(
    afw_command_local_server_self_t *self)
{
    const afw_pool_t *p;
    afw_xctx_t *xctx;
    const afw_memory_t *input;
    const afw_memory_t *body;
    const afw_object_t *action_object;
    const afw_object_t *response_object;
    const afw_utf8_t *string;
    afw_boolean_t error_occurred;
    afw_boolean_t keep_going;

    xctx = afw_xctx_create(&afw_command_s_afw_command_local_mode,
        0, self->command_self->xctx);
    p = xctx->p;
    error_occurred = false;
    keep_going = true;

    AFW_TRY {

        /*
         * Loop until an error, exit, or request processed. This is to allow
         * directives to be processed and then a request.
         */
        for (;;) {
            
            /* Get next input segment. */
            input = impl_local_get_input(self, xctx);

            /* If fatal error, set keep_going to false and break out of loop. */
            if (self->fatal_error) {
                keep_going = false;
                break;
            }

            /* If no input, break out of loop. */
            if (!input) {
                break;
            }

            /** @todo These 3 casted inputs need to use different function */
            /* If "exit", set keep_going to false and break out of loop. */
            if (afw_utf8_equal_utf8_z((const afw_utf8_t *)input, "exit\n") ||
                afw_utf8_equal_utf8_z((const afw_utf8_t *)input, "exit"))
            {
                keep_going = false;
                break;
            }

            /* If this is a directive, process it and continue looping. */
            if (afw_utf8_starts_with((const afw_utf8_t *)input,
                &afw_command_s_a_local_directive_starts_with))
            {
                impl_process_directive(self, input, xctx);
                continue;
            }

            /*
             * At this point there is a request to process. Process it then
             * break out of loop.
             */
            self->pub.request_count++;
            self->this_request_properties = afw_object_create(p, xctx);
            switch (self->mode) {

            case afw_command_local_server_mode_action:
                xctx->request = afw_command_local_request_create(self,
                    input, self->request_properties, xctx);
                afw_request_handler_process(self->director,
                    xctx->request, xctx);
                break;

            case afw_command_local_server_mode_action_direct:
                string = afw_utf8_create(
                    (const afw_utf8_octet_t *)input->ptr, input->size,
                    p, xctx);
                action_object = afw_json_to_object(string, false, p, xctx);
                response_object = afw_action_perform(
                    action_object, self->content_type, NULL, p, xctx);
                string = afw_data_type_object_to_utf8(response_object,
                    p, xctx);
                afw_command_local_server_write_result(self,
                    "%" AFW_UTF8_FMT, AFW_UTF8_FMT_ARG(string));
                break;

            case afw_command_local_server_mode_evaluate:
                string = afw_utf8_create(
                    (const afw_utf8_octet_t *)input->ptr, input->size,
                    p, xctx);
                string = afw_json_utf8_string_create(string, p, xctx);
                input = (const afw_memory_t *)afw_utf8_printf(p, xctx,
                    "{\n"
                    "    \"function\": \"%" AFW_UTF8_FMT "\",\n"
                    "    \"source\": %" AFW_UTF8_FMT "\n"
                    "}\n",
                AFW_UTF8_FMT_ARG(self->evaluate_function_id),
                AFW_UTF8_FMT_ARG(string)
                );
                xctx->request = afw_command_local_request_create(self,
                    (const afw_memory_t *)input, self->request_properties,
                    xctx);
                afw_request_handler_process(self->director,
                    xctx->request, xctx);
                break;

            case afw_command_local_server_mode_evaluate_direct:
                string = afw_utf8_create(
                    (const afw_utf8_octet_t *)input->ptr, input->size,
                    p, xctx);
                action_object = afw_object_create(p, xctx);
                afw_object_set_property_as_string(action_object,
                    &afw_s_function, &afw_s_evaluate_expression, xctx);
                afw_object_set_property_as_string(action_object,
                    &afw_s_source, string, xctx);
                response_object = afw_action_perform(
                    action_object, self->content_type, NULL, p, xctx);
                string = afw_data_type_object_to_utf8(response_object,
                    p, xctx);
                afw_command_local_server_write_result(self,
                    "%" AFW_UTF8_FMT, AFW_UTF8_FMT_ARG(string));
                break;

            case afw_command_local_server_mode_http_like:
                body = afw_command_local_parse_request(self, &input, xctx);
                xctx->request = afw_command_local_request_create(
                    self, body, self->request_properties, xctx);
                afw_request_handler_process(self->director,
                    xctx->request, xctx);
                break;

            default:
                AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
            }
            break; // Break out of loop
        }
    }

    AFW_CATCH_UNHANDLED{
        afw_command_local_server_write_error(self, AFW_ERROR_THROWN, xctx);
        error_occurred = true;
    }

    AFW_FINALLY{
        xctx->request = NULL;
        afw_adaptor_session_commit_and_release_cache(error_occurred, xctx);
        /* Special case: xctx is gone, so return before AFW_ENDTRY. */
        if (keep_going) {
            afw_command_local_server_write_end(self);
        }
        afw_xctx_release(xctx, self->command_self->xctx);
        return keep_going && !self->fatal_error;
    }

    AFW_ENDTRY;

    /* Fix warning. Should not get to here. See Special case comment above. */
    return false;
}



/*
 * Implementation of method release for interface afw_server.
 */
void
impl_afw_server_release(
    const afw_server_t * instance,
    afw_xctx_t *xctx)
{
    /* Everything will be released by afw_command.c */
}



/*
 * Implementation of method run for interface afw_server.
 */
void
impl_afw_server_run(
    const afw_server_t * instance,
    const afw_request_handler_t * handler,
    afw_xctx_t *xctx)
{
    afw_command_local_server_self_t *self =
        (afw_command_local_server_self_t *)instance;

    
    afw_command_local_server_write_result(self,
        "afw " AFW_VERSION_STRING "\n\n" "Local mode.\n");
    afw_command_local_server_write_end(self);

    while (impl_read_and_process_request(self));
}



AFW_COMMAND_DEFINE_INTERNAL(const afw_server_t *)
afw_command_local_server_create(
    afw_command_self_t *command_self)
{
    afw_command_local_server_self_t *self;
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const afw_value_t *conf;

    xctx = command_self->xctx;
    p = xctx->p;

    self = afw_pool_calloc_type(p, afw_command_local_server_self_t, xctx);
    self->pub.inf = &impl_afw_server_inf;
    self->pub.xctx = xctx;
    self->pub.properties = afw_object_create_managed(xctx->p, xctx);
    self->pub.afw_compiled_version = &impl_compiled_afw_version;
    self->pub.afw_version = afw_version_string();
    self->pub.concurrent = 1;
    self->pub.max_concurrent = 1;
    self->pub.server_type = &afw_command_s_afw_command_local_mode;
    self->pub.server_version = &impl_compiled_afw_version; /* Use afw version. */
    self->pub.start_time = afw_dateTime_now_local(xctx->p, xctx);
    self->pub.thread_count = 1;
    self->pub.server_version = &impl_compiled_afw_version;

    self->command_self = command_self;
    self->input_buffer = apr_array_make(afw_pool_get_apr_pool(p), 2000, 1);
    self->fd_input = command_self->fd_input;
    self->fd_output = command_self->fd_output;

    self->mode = afw_command_local_server_mode_evaluate_direct;

    /** @todo these should be able to be changed. */
    self->evaluate_function_id = &afw_s_evaluate_expression;
    self->content_type = command_self->content_type_in;
   
    /* Create and set runtime object for server. */
    afw_runtime_env_create_and_set_indirect_object(
        &afw_s__AdaptiveServer_,
        &afw_s_current, self, true, xctx);

    /* If there's not a conf file already processed, process a default one. */
    if (!self->command_self->conf_z) {
        self->command_self->conf_z = "internal";
        self->command_self->conf.len = strlen("internal");
        conf = afw_content_type_raw_to_value(
            afw_json_content_type_get(),
            (const afw_memory_t *)&impl_default_afw_conf,
            &self->command_self->conf,
            p, xctx);
        if (!afw_value_is_list(conf)) {
            AFW_THROW_ERROR_Z(general, "Invalid internal configuration", xctx);
        }
        afw_environment_configure_with_object_list(
            ((const afw_value_list_t *)conf)->internal,
            &self->command_self->conf, xctx);
    }

    /* Create request directory. */
    self->director = afw_request_handler_director_create(
        (const afw_server_t *)self,
        self->command_self->conf_z,
        p, xctx);

    /* Make aggregate request properties object. */
    self->multi_request_mode_properties = afw_object_create_managed(p, xctx);
    self->mode = afw_command_local_server_mode_evaluate_direct;
    if (self->multi_request_mode_properties) {
        afw_object_release(self->multi_request_mode_properties,
            self->pub.xctx);
    }
    self->multi_request_mode_properties =
        afw_object_create_managed(self->pub.xctx->p, self->pub.xctx);
    self->request_properties = afw_object_aggregate_external_create(
        &self->properties_array[0], p, xctx);
    afw_object_meta_set_ids(self->request_properties,
        &afw_s_afw, &afw_s__AdaptiveRequestProperties_, &afw_s_current, xctx);
    afw_runtime_env_set_object(self->request_properties, false, xctx);

    /* Return self. */
    return (const afw_server_t *)self;
}



AFW_COMMAND_DEFINE_INTERNAL(void)
afw_command_local_server_write_result(
    afw_command_local_server_self_t *self,
    const char *format, ...)
{
    va_list ap;
    int len;
    int rv;

    /*
     * If length of result is not 0, print length followed\n by a \n and then
     * result.  Do nothing if length is 0.
     */
    va_start (ap, format);
    len = vsnprintf(NULL, 0, format, ap);
    va_end(ap);
    if (len < 0) exit(EXIT_FAILURE);
    if (len > 0) {
        rv = fprintf(self->fd_output, "%d\n", len);
        if (rv < 0) exit(EXIT_FAILURE);
        rv = fflush(self->fd_output);
        if (rv < 0) exit(EXIT_FAILURE);

        va_start (ap, format);
        rv = vfprintf(self->fd_output, format, ap);
        va_end(ap);
        if (rv < 0) exit(EXIT_FAILURE);

        rv = fflush(self->fd_output);
        if (rv < 0) exit(EXIT_FAILURE);
    }
}



AFW_COMMAND_DEFINE_INTERNAL(void)
afw_command_local_server_write_error(
    afw_command_local_server_self_t *self, 
    const afw_error_t *error,
    afw_xctx_t *xctx)
{
    const afw_object_t *error_object;
    const afw_object_t *response_object;
    const afw_utf8_t *status;
    const afw_utf8_t *string;
    int rv;

    response_object = afw_object_create(xctx->p, xctx);

    status = (self->fatal_error) ? &afw_s_fatal : &afw_s_error;
    afw_object_set_property_as_string(response_object,
        &afw_s_status, status, xctx);

    error_object = afw_error_to_object(error, xctx->p, xctx);
    afw_object_set_property_as_object(response_object,
        &afw_s_error, error_object, xctx);

    string = afw_data_type_object_to_utf8(response_object, xctx->p, xctx);
    rv = fprintf(self->fd_output, "%" AFW_SIZE_T_FMT "\n", string->len);
    if (rv < 0) exit(EXIT_FAILURE);
    rv = fprintf(self->fd_output, "%" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(string));
    if (rv < 0) exit(EXIT_FAILURE);
    rv = fflush(self->fd_output);
    if (rv < 0) exit(EXIT_FAILURE);
}



AFW_COMMAND_DEFINE_INTERNAL(void)
afw_command_local_server_write_end(
    afw_command_local_server_self_t *self)
{
    int rv;

    if (self) {
        rv = fprintf(self->fd_output, "0\n");
        if (rv < 0) exit(EXIT_FAILURE);
        rv = fflush(self->fd_output);
        if (rv < 0) exit(EXIT_FAILURE);
    }
}

/** @} */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Frame FCGI Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_server_fcgi.c
 * @brief Adaptive Frame FastCGI Support
 *
 */

/* This header has to be first because it includes fcgi_stdio.h. */
#include "afw_server_fcgi_internal.h" 

#include "afw.h"
#include "afw_request_impl.h"
#include <fcgiapp.h>
#include <apr_signal.h>

/* Declares and rti/inf defines for interface afw_server */
#define AFW_IMPLEMENTATION_ID "fcgi"
#include "afw_server_impl_declares.h"
#include "afw_server_fcgi_strings.h"
#include "generated/afw_server_fcgi_version_info.h"

/* Global to hold base xctx pointer. */
static afw_xctx_t *impl_server_xctx;

/* Compiled afw version. */
static const afw_utf8_t impl_compiled_afw_version =
AFW_UTF8_LITERAL(AFW_VERSION_STRING);

#ifdef __FIXME__
/** @fixme Replace this when afw_signal is implemented. */
static void impl_handle_sigterm(int signum)
{
    afw_environment_t *env;

    /* Set flag in environment. */
    env = (afw_environment_t *)impl_server_xctx->env;
    env->terminating = true;

    /* Tell FCGI shutdown is pending. */
    FCGX_ShutdownPending();
}
#endif


/* Internal server create. */
const afw_server_t *
afw_server_fcgi_internal_create(const char *path, 
    afw_integer_t thread_count, afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_t *self = NULL;
    int rc;

    /* Initialize FCGX. */
    rc = FCGX_Init();
    if (rc != 0) {
        AFW_THROW_ERROR_Z(general,
            "FCGX_Init() error", xctx);
    }

    /* Call FCGI_Finish on exit. */
    atexit(&FCGX_Finish);


    /* Set signal handler function for sigterm. */
    impl_server_xctx = xctx;

    /** @fixme Removed for now because not working fully
    apr_signal(SIGTERM, impl_handle_sigterm);
     */


    /* Allocate and initialize self. */
    self = afw_xctx_calloc_type(afw_server_fcgi_internal_t, xctx);
    self->pub.inf = &impl_afw_server_inf;
    self->pub.xctx = xctx;
    self->pub.properties = afw_object_create_managed(xctx->p, xctx);
    self->pub.thread_count = thread_count;
    self->pub.afw_version = afw_version_string();
    self->pub.afw_compiled_version = &impl_compiled_afw_version;
    self->pub.server_version = &impl_compiled_afw_version; /* Use afw version. */
    self->pub.server_type = &afw_server_fcgi_s_afw_server_fcgi;
    self->pub.start_time = afw_dateTime_now_local(xctx->p, xctx);

    /* Use afw version. */
    self->pub.server_version = &impl_compiled_afw_version;

    /* Set object used to access environment variables. */
    self->environment_variables_object =
        afw_environment_create_environment_variables_object(true, xctx);
    afw_runtime_xctx_set_object(self->environment_variables_object,
        true, xctx);
    afw_xctx_qualifier_stack_qualifier_object_push(&afw_s_environment,
        self->environment_variables_object, true, xctx->p, xctx);        

    /* Open socket. */
    self->sock = FCGX_OpenSocket(path, 500);
    if (self->sock < 0) {
        AFW_THROW_ERROR_Z(general,
            "FCGX_OpenSocket() error", xctx);
    }

    /* Make sure this is not a CGI.
    if (FCGX_IsCGI()) {
        AFW_THROW_ERROR_Z(general,
            "This is not Fast CGI", xctx);
    } */

    /* Create and set runtime object for server. */
    afw_runtime_env_create_and_set_indirect_object(
        &afw_s__AdaptiveServer_,
        &afw_s_current, self, true, xctx);

    /* Register flag trace:server */
    afw_flag_environment_register_flag(
        &afw_server_fcgi_s_a_flag_trace_server,
        &afw_server_fcgi_s_a_flag_trace_server_brief,
        &afw_server_fcgi_s_a_flag_trace_server_description,
        &afw_s_a_flag_trace,
        xctx);

    /* Register flag trace:server:request */
    afw_flag_environment_register_flag(
        &afw_server_fcgi_s_a_flag_trace_server_request,
        &afw_server_fcgi_s_a_flag_trace_server_request_brief,
        &afw_server_fcgi_s_a_flag_trace_server_request_description,
        &afw_server_fcgi_s_a_flag_trace_server,
        xctx);

    /* Register flag trace:server:request:process */
    afw_flag_environment_register_flag(
        &afw_server_fcgi_s_a_flag_trace_server_request_process,
        &afw_server_fcgi_s_a_flag_trace_server_request_process_brief,
        &afw_server_fcgi_s_a_flag_trace_server_request_process_description,
        &afw_server_fcgi_s_a_flag_trace_server_request,
        xctx);
    self->flag_index_trace_process = afw_flag_get_index(
        &afw_server_fcgi_s_a_flag_trace_server_request_process,
        xctx);

    /* Get flag_index for request trace. */

    /* Return new adaptor. */
    return (afw_server_t *)self;
}


/*
 * Implementation of method release of interface afw_server.
 */
void
impl_afw_server_release(
    const afw_server_t * instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */

    /*
     * Continue release, even if there is already an error.  Don't overwrite
     * existing error.
     */
}


/* Process a request. */
static void impl_process_request(afw_server_fcgi_internal_t *self,
    const afw_request_handler_t *director,
    afw_server_fcgi_internal_server_thread_t *server_thread,
    afw_xctx_t *xctx)
{
    const afw_request_t * request;

    /* Create request. */
    request = (const afw_request_t *)afw_server_fcgi_internal_create_request(
        self, server_thread->fcgx_request, xctx);
    xctx->request = request;

    afw_trace_fz(1, self->flag_index_trace_process,
        &self->pub, xctx,
        "Request begin thread %d concurrent %d",
        server_thread->thread->thread_number,
        server_thread->server->pub.concurrent);

    /* Handle request errors in request session xctx. */
    AFW_TRY{

        afw_request_impl_trace_begin(request, xctx);

        /* Process request. */
        afw_request_handler_process(self->director, request, xctx);
    }

    /* If client_closed, don't try to write response, just log it. */
    AFW_CATCH(client_closed) {
        afw_error_write_log(afw_log_priority_info, AFW_ERROR_THROWN, xctx);
    }

    /* If memory error, try not to use memory. */
    AFW_CATCH(memory) {
        afw_error_write_log(afw_log_priority_info, AFW_ERROR_THROWN, xctx);
    }

    /* For all other errors, log and write to response body. */
    AFW_CATCH_UNHANDLED{
        afw_error_write_log(afw_log_priority_info, AFW_ERROR_THROWN, xctx);
        afw_request_write_error_to_response_body(request,
            afw_error_http_status(AFW_ERROR_THROWN), AFW_ERROR_THROWN, xctx);
    }

    /* Always cleanup. */
    AFW_FINALLY{
        afw_request_release(request, xctx);
    }

    AFW_ENDTRY;

    afw_trace_fz(1, self->flag_index_trace_process,
        &self->pub, xctx,
        "Request end   thread %d concurrent %d",
        server_thread->thread->thread_number,
        server_thread->server->pub.concurrent);
}


/*  Request thread run loop. */
static void* AFW_THREAD_FUNCTION
impl_afw_server_request_thread_start(const afw_thread_t *thread,
    void *arg)
{
    afw_server_fcgi_internal_server_thread_t *server_thread = arg;
    afw_server_fcgi_internal_t * server = server_thread->server;
    afw_xctx_t *xctx = thread->xctx;
    afw_xctx_t *request_session_xctx = NULL;
    afw_integer_t request_count;
    afw_integer_t concurrent;
    afw_integer_t max_concurrent;    

    /* Initialize a FCGX_Request for this thread. */
    server_thread->fcgx_request = afw_xctx_calloc_type(FCGX_Request, xctx);
    FCGX_InitRequest(server_thread->fcgx_request, server->sock,
        FCGI_FAIL_ACCEPT_ON_INTR);

    /* Request loop. */
    while (!afw_xctx_environment_is_terminating(xctx) &&
        FCGX_Accept_r(server_thread->fcgx_request) >= 0)
    {
        /* Increment count. */
        request_count = afw_atomic_integer_increment(&server->pub.request_count);

        /* Process request. */
        AFW_TRY {
 
            /* Increment concurrent count. */
            max_concurrent = server->pub.max_concurrent;
            concurrent = afw_atomic_integer_increment(&server->pub.concurrent);
            if (concurrent > max_concurrent) {
                afw_atomic_integer_cas(&server->pub.max_concurrent,
                    max_concurrent, concurrent);
            }

            /* Create Adaptive Framework xctx for request session. */
            request_session_xctx = afw_xctx_create(
                &AFW_XCTX_s_NAME_REQUEST_SESSION, request_count, xctx);

            /* Process request. */
            impl_process_request(server, server->director,
                server_thread, request_session_xctx);
        }

        AFW_CATCH_UNHANDLED{
            /* Increment unhandled error count. */
            afw_atomic_integer_increment(&server->pub.unhandled_errors);
        }

        AFW_FINALLY {

            /* Decrement concurrent count. */
            afw_atomic_integer_decrement(&server->pub.concurrent);

            /* Release xctx resources. */
            if (request_session_xctx) {
                afw_xctx_release(request_session_xctx, xctx);
            }
 
        }

        AFW_ENDTRY;
    }

    /* Normal return. */
    return NULL;
}


/*
 * Implementation of method run of interface afw_server.
 */
void
impl_afw_server_run(
    const afw_server_t * instance,
    const afw_request_handler_t * handler,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_t * server =
        (afw_server_fcgi_internal_t *)instance;

    afw_integer_t count;
    afw_server_fcgi_internal_server_thread_t *server_thread;
    afw_thread_attr_t *thread_attr;

    server->director = handler;
    server->xctx = xctx;
    server->threads = afw_xctx_calloc(
        sizeof(afw_server_fcgi_internal_server_thread_t) *
        (afw_size_t)server->pub.thread_count,
        xctx);

    /** @fixme Put in signal handling */

    /* The default thread attribute: detachable */
    thread_attr = afw_thread_attr_create(xctx->p, xctx);


    /* Create the request threads. */
    for (count = 1, server_thread = server->threads;
        count <= server->pub.thread_count;
        count++, server_thread++)
    {
        server_thread->server = server;
        server_thread->thread = afw_thread_create(
            thread_attr,
            impl_afw_server_request_thread_start, server_thread,
            &AFW_XCTX_s_NAME_REQUEST_THREAD,
            count, xctx);
    }

    /* Wait for request threads to die. */
    for (count = 1, server_thread = server->threads;
        count <= server->pub.thread_count;
        count++, server_thread++)
    {
        afw_thread_join(server_thread->thread, xctx);
    }


    /* Run is finished. */
}

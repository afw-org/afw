// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Internal FCGI Server Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_SERVER_FCGI_INTERNAL_H__
#define __AFW_SERVER_FCGI_INTERNAL_H__



/**
 * @file afw_server_fcgi_internal.h
 * @brief Internal header file for AFW FCGI Server
 */

#include "fcgiapp.h"
#include "afw_minimal.h"
#include "afw_request.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct afw_server_fcgi_internal_s 
afw_server_fcgi_internal_t; 

typedef struct afw_server_fcgi_internal_request_s 
afw_server_fcgi_internal_request_t;

typedef struct afw_server_fcgi_internal_server_thread_s
afw_server_fcgi_internal_server_thread_t;

struct afw_server_fcgi_internal_s {
    afw_server_t pub;

    /* Server pool. */
    const afw_pool_t *p;

    /* Server xctx. */
    afw_xctx_t *xctx;

    /* Server request director. */
    const afw_request_handler_t * director;

    /* Object used to access server environment variables. */
    const afw_object_t *environment_variables_object;

    /* Trace flag index for request process. */
    afw_size_t flag_index_trace_process;

    /* Socket */
    int sock;
 
    /* threads. */
    afw_server_fcgi_internal_server_thread_t *threads;
};


struct afw_server_fcgi_internal_server_thread_s {

    afw_server_fcgi_internal_t * server;

    const afw_thread_t *thread;

    FCGX_Request *fcgx_request;
};


struct afw_server_fcgi_internal_request_s {
    afw_request_t pub;

    /* Server */
    afw_server_fcgi_internal_t *server;

    /* Request state. */
    afw_request_state_t state;

    /* FCGX Request used in run loop. */
    FCGX_Request *fcgx_request;
};


/** fcgi rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_fcgi "fcgi"

/**
 * @brief Create an FCGI afw_server.
 * @param path to FCGI server. Default ":9345".
 * @param thread_count number of threads.
 * @param xctx execution context.
 * @return Pointer to interface pointer of new server.
 */
const afw_server_t *
afw_server_fcgi_internal_create(
    const char *path, afw_integer_t thread_count, afw_xctx_t *xctx);

/**
 * Create an internal fcgi request.
 */
afw_server_fcgi_internal_request_t *
afw_server_fcgi_internal_create_request(
    afw_server_fcgi_internal_t *server,
    FCGX_Request *fcgx_request,
    afw_xctx_t *xctx);

/**
 * Create an internal fcgi request.
 */
const afw_object_t *
afw_server_fcgi_internal_create_properties_object(
    afw_server_fcgi_internal_request_t *request,
    afw_xctx_t *xctx);


#ifdef __cplusplus
}
#endif

#endif /* __AFW_SERVER_FCGI_INTERNAL_H__ */

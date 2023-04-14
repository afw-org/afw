// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Director Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_REQUEST_HANDLER_INTERNAL_H__
#define __AFW_REQUEST_HANDLER_INTERNAL_H__



/**
 * @file afw_request_handler_internal.h
 * @brief Header file for Adaptive Framework Request Handler Internals
 */

#include "afw_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct afw_request_handler_adaptor_internal_self_s {
    afw_request_handler_t pub;
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const afw_object_t *properties;
    const afw_object_options_t *default_options;
} afw_request_handler_adaptor_internal_self_t;

typedef struct afw_request_handler_entry_s
    afw_request_handler_entry_t;

struct afw_request_handler_entry_s {
    afw_request_handler_entry_t *next;
    const afw_utf8_t *object_id;
    const afw_utf8_t *uri_prefix;
    const afw_utf8_t *handler_type;
    const afw_object_t *properties;
    const afw_request_handler_t *rh;
    const afw_request_handler_factory_t *factory;
};


#define AFW_REQUEST_HANDLER_S_SINGLETON_KEY_HEAD afw_s_afw_request_handler_head

typedef struct afw_request_handler_head_s {
    afw_request_handler_entry_t *first_handler;
    afw_request_handler_entry_t *last_handler;
} afw_request_handler_head_t;


/** @brief Configuration handler for entry type "requestHandler". */
void afw_request_handler_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


#ifdef __cplusplus
}
#endif

#endif /* __AFW_REQUEST_HANDLER_INTERNAL_H__ */

// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_interface Implementation Declares
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#include "afw_interface_opaques.h"

AFW_BEGIN_DECLARES

/**
 * @addtogroup afw_interface Interfaces
 *
 * @{
 */

/**
 * @addtogroup afw_request_interface afw_request
 * @{
 */

/**
 * @addtogroup afw_request_impl_declares afw_request
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_request.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_request.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_request_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_request_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_request_rti.
 * 3. inf will be defined with a name of impl_afw_request_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_request_inf'.
 *
 *   AFW_REQUEST_SELF_T - (optional) defaults to 'const afw_request_t'.
 *       The const is not required and normally should not be specified. It is
 *       the default for historical reasons.
 *
 * Example:
 *~~~~~~~~~~~~~~~{.c}
 *
 *   static const sometype_t impl_myimpl_stuff =  {
 *       ...
 *   };
 *
 *   #define AFW_IMPLEMENTATION_ID "myimpl"
 *   #define AFW_IMPLEMENTATION_SPECIFIC &impl_myimpl_stuff
 *   #include "afw_request_impl_declares.h"
 *
 * A second #include "afw_request_impl_declares.h" can be specified if
" *   #define "AFW_REQUEST_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_request_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_REQUEST_IMPL_DECLARES_H__
#ifndef AFW_REQUEST_INF_ONLY
#error "afw_request_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_REQUEST_IMPL_DECLARES_H__
#ifdef AFW_REQUEST_INF_ONLY
#error "afw_request_impl_declares.h must be included before AFW_REQUEST_INF_ONLY."
#endif
#define __AFW_REQUEST_IMPL_DECLARES_H__ 1
#endif

#ifdef AFW_IMPLEMENTATION_ID
#define _AFW_IMPLEMENTATION_ID_ AFW_IMPLEMENTATION_ID
#else
#define _AFW_IMPLEMENTATION_ID_ "unspecified"
#endif
#ifdef AFW_IMPLEMENTATION_SPECIFIC
#define _AFW_IMPLEMENTATION_SPECIFIC_ AFW_IMPLEMENTATION_SPECIFIC
#else
#define _AFW_IMPLEMENTATION_SPECIFIC_ NULL
#endif

#ifndef AFW_REQUEST_SELF_T
#define AFW_REQUEST_SELF_T const afw_request_t
#endif

#ifndef AFW_REQUEST_INF_ONLY

#ifndef impl_afw_request_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_request_release(
    AFW_REQUEST_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_set_error_info
/* Declare method set_error_info */
AFW_DECLARE_STATIC(void)
impl_afw_request_set_error_info(
    AFW_REQUEST_SELF_T *self,
    const afw_object_t * error_info,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_read_raw_request_body
/* Declare method read_raw_request_body */
AFW_DECLARE_STATIC(void)
impl_afw_request_read_raw_request_body(
    AFW_REQUEST_SELF_T *self,
    afw_size_t buffer_size,
    void * buffer,
    afw_size_t * size,
    afw_boolean_t * more_to_read,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_set_response_status_code
/* Declare method set_response_status_code */
AFW_DECLARE_STATIC(void)
impl_afw_request_set_response_status_code(
    AFW_REQUEST_SELF_T *self,
    const afw_utf8_t * code,
    const afw_utf8_t * reason,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_write_response_header
/* Declare method write_response_header */
AFW_DECLARE_STATIC(void)
impl_afw_request_write_response_header(
    AFW_REQUEST_SELF_T *self,
    const afw_utf8_t * name,
    const afw_utf8_t * value,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_write_raw_response_body
/* Declare method write_raw_response_body */
AFW_DECLARE_STATIC(void)
impl_afw_request_write_raw_response_body(
    AFW_REQUEST_SELF_T *self,
    afw_size_t size,
    const void * buffer,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_flush_response
/* Declare method flush_response */
AFW_DECLARE_STATIC(void)
impl_afw_request_flush_response(
    AFW_REQUEST_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_request_finish_response
/* Declare method finish_response */
AFW_DECLARE_STATIC(void)
impl_afw_request_finish_response(
    AFW_REQUEST_SELF_T *self,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_request */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_request_inf_t)
#else
static const afw_request_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_request_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_request"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_request_release_t)
    impl_afw_request_release,
    (afw_request_set_error_info_t)
    impl_afw_request_set_error_info,
    (afw_request_read_raw_request_body_t)
    impl_afw_request_read_raw_request_body,
    (afw_request_set_response_status_code_t)
    impl_afw_request_set_response_status_code,
    (afw_request_write_response_header_t)
    impl_afw_request_write_response_header,
    (afw_request_write_raw_response_body_t)
    impl_afw_request_write_raw_response_body,
    (afw_request_flush_response_t)
    impl_afw_request_flush_response,
    (afw_request_finish_response_t)
    impl_afw_request_finish_response
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

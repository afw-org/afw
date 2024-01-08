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
 * @addtogroup afw_adaptor_session_interface afw_adaptor_session
 * @{
 */

/**
 * @addtogroup afw_adaptor_session_impl_declares afw_adaptor_session
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_adaptor_session.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_adaptor_session.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_adaptor_session_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_adaptor_session_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_adaptor_session_rti.
 * 3. inf will be defined with a name of impl_afw_adaptor_session_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_adaptor_session_inf'.
 *
 *   AFW_ADAPTOR_SESSION_SELF_T - (optional) defaults to 'const afw_adaptor_session_t'.
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
 *   #include "afw_adaptor_session_impl_declares.h"
 *
 * A second #include "afw_adaptor_session_impl_declares.h" can be specified if
" *   #define "AFW_ADAPTOR_SESSION_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_adaptor_session_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_ADAPTOR_SESSION_IMPL_DECLARES_H__
#ifndef AFW_ADAPTOR_SESSION_INF_ONLY
#error "afw_adaptor_session_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_ADAPTOR_SESSION_IMPL_DECLARES_H__
#ifdef AFW_ADAPTOR_SESSION_INF_ONLY
#error "afw_adaptor_session_impl_declares.h must be included before AFW_ADAPTOR_SESSION_INF_ONLY."
#endif
#define __AFW_ADAPTOR_SESSION_IMPL_DECLARES_H__ 1
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

#ifndef AFW_ADAPTOR_SESSION_SELF_T
#define AFW_ADAPTOR_SESSION_SELF_T const afw_adaptor_session_t
#endif

#ifndef AFW_ADAPTOR_SESSION_INF_ONLY

#ifndef impl_afw_adaptor_session_destroy
/* Declare method destroy */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_session_destroy(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_retrieve_objects
/* Declare method retrieve_objects */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_session_retrieve_objects(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t * adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_get_object
/* Declare method get_object */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_session_get_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t * adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_add_object
/* Declare method add_object */
AFW_DECLARE_STATIC(const afw_utf8_t *)
impl_afw_adaptor_session_add_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * suggested_object_id,
    const afw_object_t * object,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_modify_object
/* Declare method modify_object */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_session_modify_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_adaptor_modify_entry_t * const * entry,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_replace_object
/* Declare method replace_object */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_session_replace_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_object_t * replacement_object,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_delete_object
/* Declare method delete_object */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_session_delete_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_begin_transaction
/* Declare method begin_transaction */
AFW_DECLARE_STATIC(const afw_adaptor_transaction_t *)
impl_afw_adaptor_session_begin_transaction(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_get_journal_interface
/* Declare method get_journal_interface */
AFW_DECLARE_STATIC(const afw_adaptor_journal_t *)
impl_afw_adaptor_session_get_journal_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_get_key_value_interface
/* Declare method get_key_value_interface */
AFW_DECLARE_STATIC(const afw_adaptor_key_value_t *)
impl_afw_adaptor_session_get_key_value_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_get_index_interface
/* Declare method get_index_interface */
AFW_DECLARE_STATIC(const afw_adaptor_impl_index_t *)
impl_afw_adaptor_session_get_index_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_session_get_object_type_cache_interface
/* Declare method get_object_type_cache_interface */
AFW_DECLARE_STATIC(const afw_adaptor_object_type_cache_t *)
impl_afw_adaptor_session_get_object_type_cache_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_adaptor_session */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_adaptor_session_inf_t)
#else
static const afw_adaptor_session_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_adaptor_session_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_adaptor_session"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_adaptor_session_destroy_t)
    impl_afw_adaptor_session_destroy,
    (afw_adaptor_session_retrieve_objects_t)
    impl_afw_adaptor_session_retrieve_objects,
    (afw_adaptor_session_get_object_t)
    impl_afw_adaptor_session_get_object,
    (afw_adaptor_session_add_object_t)
    impl_afw_adaptor_session_add_object,
    (afw_adaptor_session_modify_object_t)
    impl_afw_adaptor_session_modify_object,
    (afw_adaptor_session_replace_object_t)
    impl_afw_adaptor_session_replace_object,
    (afw_adaptor_session_delete_object_t)
    impl_afw_adaptor_session_delete_object,
    (afw_adaptor_session_begin_transaction_t)
    impl_afw_adaptor_session_begin_transaction,
    (afw_adaptor_session_get_journal_interface_t)
    impl_afw_adaptor_session_get_journal_interface,
    (afw_adaptor_session_get_key_value_interface_t)
    impl_afw_adaptor_session_get_key_value_interface,
    (afw_adaptor_session_get_index_interface_t)
    impl_afw_adaptor_session_get_index_interface,
    (afw_adaptor_session_get_object_type_cache_interface_t)
    impl_afw_adaptor_session_get_object_type_cache_interface
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

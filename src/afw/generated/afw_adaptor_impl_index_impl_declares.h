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
 * @addtogroup afw_adaptor_impl_index_interface afw_adaptor_impl_index
 * @{
 */

/**
 * @addtogroup afw_adaptor_impl_index_impl_declares afw_adaptor_impl_index
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_adaptor_impl_index.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_adaptor_impl_index.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_adaptor_impl_index_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_adaptor_impl_index_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_adaptor_impl_index_rti.
 * 3. inf will be defined with a name of impl_afw_adaptor_impl_index_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_adaptor_impl_index_inf'.
 *
 *   AFW_ADAPTOR_IMPL_INDEX_SELF_T - (optional) defaults to 'const afw_adaptor_impl_index_t'.
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
 *   #include "afw_adaptor_impl_index_impl_declares.h"
 *
 * A second #include "afw_adaptor_impl_index_impl_declares.h" can be specified if
" *   #define "AFW_ADAPTOR_IMPL_INDEX_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_adaptor_impl_index_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_ADAPTOR_IMPL_INDEX_IMPL_DECLARES_H__
#ifndef AFW_ADAPTOR_IMPL_INDEX_INF_ONLY
#error "afw_adaptor_impl_index_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_ADAPTOR_IMPL_INDEX_IMPL_DECLARES_H__
#ifdef AFW_ADAPTOR_IMPL_INDEX_INF_ONLY
#error "afw_adaptor_impl_index_impl_declares.h must be included before AFW_ADAPTOR_IMPL_INDEX_INF_ONLY."
#endif
#define __AFW_ADAPTOR_IMPL_INDEX_IMPL_DECLARES_H__ 1
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

#ifndef AFW_ADAPTOR_IMPL_INDEX_SELF_T
#define AFW_ADAPTOR_IMPL_INDEX_SELF_T const afw_adaptor_impl_index_t
#endif

#ifndef AFW_ADAPTOR_IMPL_INDEX_INF_ONLY

#ifndef impl_afw_adaptor_impl_index_open
/* Declare method open */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_impl_index_open(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    afw_boolean_t integer,
    afw_boolean_t unique,
    afw_boolean_t reverse,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_impl_index_release(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_get_index_definitions
/* Declare method get_index_definitions */
AFW_DECLARE_STATIC(const afw_object_t *)
impl_afw_adaptor_impl_index_get_index_definitions(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_update_index_definitions
/* Declare method update_index_definitions */
AFW_DECLARE_STATIC(void)
impl_afw_adaptor_impl_index_update_index_definitions(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_object_t * indexDefinitions,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_add
/* Declare method add */
AFW_DECLARE_STATIC(afw_rc_t)
impl_afw_adaptor_impl_index_add(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * key,
    const afw_utf8_t* value,
    afw_boolean_t unique,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_delete
/* Declare method delete */
AFW_DECLARE_STATIC(afw_rc_t)
impl_afw_adaptor_impl_index_delete(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * key,
    const afw_utf8_t* value,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_replace
/* Declare method replace */
AFW_DECLARE_STATIC(afw_rc_t)
impl_afw_adaptor_impl_index_replace(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * key,
    const afw_utf8_t* old_value,
    const afw_utf8_t* new_value,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_drop
/* Declare method drop */
AFW_DECLARE_STATIC(afw_rc_t)
impl_afw_adaptor_impl_index_drop(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_open_cursor
/* Declare method open_cursor */
AFW_DECLARE_STATIC(afw_adaptor_impl_index_cursor_t *)
impl_afw_adaptor_impl_index_open_cursor(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * index_key,
    int operator,
    const afw_utf8_t * value,
    afw_boolean_t unique,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adaptor_impl_index_get_session
/* Declare method get_session */
AFW_DECLARE_STATIC(const afw_adaptor_session_t *)
impl_afw_adaptor_impl_index_get_session(
    AFW_ADAPTOR_IMPL_INDEX_SELF_T *self,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_adaptor_impl_index */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_adaptor_impl_index_inf_t)
#else
static const afw_adaptor_impl_index_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_adaptor_impl_index_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_adaptor_impl_index"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_adaptor_impl_index_open_t)
    impl_afw_adaptor_impl_index_open,
    (afw_adaptor_impl_index_release_t)
    impl_afw_adaptor_impl_index_release,
    (afw_adaptor_impl_index_get_index_definitions_t)
    impl_afw_adaptor_impl_index_get_index_definitions,
    (afw_adaptor_impl_index_update_index_definitions_t)
    impl_afw_adaptor_impl_index_update_index_definitions,
    (afw_adaptor_impl_index_add_t)
    impl_afw_adaptor_impl_index_add,
    (afw_adaptor_impl_index_delete_t)
    impl_afw_adaptor_impl_index_delete,
    (afw_adaptor_impl_index_replace_t)
    impl_afw_adaptor_impl_index_replace,
    (afw_adaptor_impl_index_drop_t)
    impl_afw_adaptor_impl_index_drop,
    (afw_adaptor_impl_index_open_cursor_t)
    impl_afw_adaptor_impl_index_open_cursor,
    (afw_adaptor_impl_index_get_session_t)
    impl_afw_adaptor_impl_index_get_session
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

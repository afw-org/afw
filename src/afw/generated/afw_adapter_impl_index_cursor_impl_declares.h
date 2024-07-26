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
 * @addtogroup afw_adapter_impl_index_cursor_interface afw_adapter_impl_index_cursor
 * @{
 */

/**
 * @addtogroup afw_adapter_impl_index_cursor_impl_declares afw_adapter_impl_index_cursor
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_adapter_impl_index_cursor.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_adapter_impl_index_cursor.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_adapter_impl_index_cursor_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_adapter_impl_index_cursor_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_adapter_impl_index_cursor_rti.
 * 3. inf will be defined with a name of impl_afw_adapter_impl_index_cursor_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_adapter_impl_index_cursor_inf'.
 *
 *   AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T - (optional) defaults to 'const afw_adapter_impl_index_cursor_t'.
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
 *   #include "afw_adapter_impl_index_cursor_impl_declares.h"
 *
 * A second #include "afw_adapter_impl_index_cursor_impl_declares.h" can be specified if
" *   #define "AFW_ADAPTER_IMPL_INDEX_CURSOR_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_adapter_impl_index_cursor_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_ADAPTER_IMPL_INDEX_CURSOR_IMPL_DECLARES_H__
#ifndef AFW_ADAPTER_IMPL_INDEX_CURSOR_INF_ONLY
#error "afw_adapter_impl_index_cursor_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_ADAPTER_IMPL_INDEX_CURSOR_IMPL_DECLARES_H__
#ifdef AFW_ADAPTER_IMPL_INDEX_CURSOR_INF_ONLY
#error "afw_adapter_impl_index_cursor_impl_declares.h must be included before AFW_ADAPTER_IMPL_INDEX_CURSOR_INF_ONLY."
#endif
#define __AFW_ADAPTER_IMPL_INDEX_CURSOR_IMPL_DECLARES_H__ 1
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

#ifndef AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T
#define AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T const afw_adapter_impl_index_cursor_t
#endif

#ifndef AFW_ADAPTER_IMPL_INDEX_CURSOR_INF_ONLY

#ifndef impl_afw_adapter_impl_index_cursor_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_adapter_impl_index_cursor_release(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adapter_impl_index_cursor_get_next_object
/* Declare method get_next_object */
AFW_DECLARE_STATIC(const afw_object_t *)
impl_afw_adapter_impl_index_cursor_get_next_object(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    const afw_pool_t * pool,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adapter_impl_index_cursor_contains_object
/* Declare method contains_object */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_adapter_impl_index_cursor_contains_object(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    const afw_object_t * object,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adapter_impl_index_cursor_inner_join
/* Declare method inner_join */
AFW_DECLARE_STATIC(const afw_adapter_impl_index_cursor_t *)
impl_afw_adapter_impl_index_cursor_inner_join(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    const afw_adapter_impl_index_cursor_t * cursor,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_adapter_impl_index_cursor_get_count
/* Declare method get_count */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_adapter_impl_index_cursor_get_count(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    size_t * count,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_adapter_impl_index_cursor */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_adapter_impl_index_cursor_inf_t)
#else
static const afw_adapter_impl_index_cursor_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_adapter_impl_index_cursor_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_adapter_impl_index_cursor"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_adapter_impl_index_cursor_release_t)
    impl_afw_adapter_impl_index_cursor_release,
    (afw_adapter_impl_index_cursor_get_next_object_t)
    impl_afw_adapter_impl_index_cursor_get_next_object,
    (afw_adapter_impl_index_cursor_contains_object_t)
    impl_afw_adapter_impl_index_cursor_contains_object,
    (afw_adapter_impl_index_cursor_inner_join_t)
    impl_afw_adapter_impl_index_cursor_inner_join,
    (afw_adapter_impl_index_cursor_get_count_t)
    impl_afw_adapter_impl_index_cursor_get_count
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

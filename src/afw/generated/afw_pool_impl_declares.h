// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_interface Implementation Declares
 *
 * Copyright (c) 2010-2023 Clemson University
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
 * @addtogroup afw_pool_interface afw_pool
 * @{
 */

/**
 * @addtogroup afw_pool_impl_declares afw_pool
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_pool.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_pool.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_pool_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_pool_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_pool_rti.
 * 3. inf will be defined with a name of impl_afw_pool_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_pool_inf'.
 *
 *   AFW_POOL_SELF_T - (optional) defaults to 'const afw_pool_t'.
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
 *   #include "afw_pool_impl_declares.h"
 *
 * A second #include "afw_pool_impl_declares.h" can be specified if
" *   #define "AFW_POOL_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_pool_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_POOL_IMPL_DECLARES_H__
#ifndef AFW_POOL_INF_ONLY
#error "afw_pool_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_POOL_IMPL_DECLARES_H__
#ifdef AFW_POOL_INF_ONLY
#error "afw_pool_impl_declares.h must be included before AFW_POOL_INF_ONLY."
#endif
#define __AFW_POOL_IMPL_DECLARES_H__ 1
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

#ifndef AFW_POOL_SELF_T
#define AFW_POOL_SELF_T const afw_pool_t
#endif

#ifndef AFW_POOL_INF_ONLY

#ifndef impl_afw_pool_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_get_reference
/* Declare method get_reference */
AFW_DECLARE_STATIC(void)
impl_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_destroy
/* Declare method destroy */
AFW_DECLARE_STATIC(void)
impl_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_get_apr_pool
/* Declare method get_apr_pool */
AFW_DECLARE_STATIC(apr_pool_t *)
impl_afw_pool_get_apr_pool(
    AFW_POOL_SELF_T *self);
#endif

#ifndef impl_afw_pool_calloc
/* Declare method calloc */
AFW_DECLARE_STATIC(void *)
impl_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_malloc
/* Declare method malloc */
AFW_DECLARE_STATIC(void *)
impl_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_free_memory_internal
/* Declare method free_memory_internal */
AFW_DECLARE_STATIC(void)
impl_afw_pool_free_memory_internal(
    AFW_POOL_SELF_T *self,
    void * address,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_register_cleanup_before
/* Declare method register_cleanup_before */
AFW_DECLARE_STATIC(void)
impl_afw_pool_register_cleanup_before(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_deregister_cleanup
/* Declare method deregister_cleanup */
AFW_DECLARE_STATIC(void)
impl_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_pool */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_pool_inf_t)
#else
static const afw_pool_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_pool_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_pool"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_pool_release_t)
    impl_afw_pool_release,
    (afw_pool_get_reference_t)
    impl_afw_pool_get_reference,
    (afw_pool_destroy_t)
    impl_afw_pool_destroy,
    (afw_pool_get_apr_pool_t)
    impl_afw_pool_get_apr_pool,
    (afw_pool_calloc_t)
    impl_afw_pool_calloc,
    (afw_pool_malloc_t)
    impl_afw_pool_malloc,
    (afw_pool_free_memory_internal_t)
    impl_afw_pool_free_memory_internal,
    (afw_pool_register_cleanup_before_t)
    impl_afw_pool_register_cleanup_before,
    (afw_pool_deregister_cleanup_t)
    impl_afw_pool_deregister_cleanup
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

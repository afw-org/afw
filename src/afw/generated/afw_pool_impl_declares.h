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
 * - AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 * - AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 * - AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to impl_afw_pool_inf.
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
#ifndef AFW_POOL_INF_ONLY

#ifndef impl_afw_pool_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_pool_release(
    const afw_pool_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_add_reference
/* Declare method add_reference */
AFW_DECLARE_STATIC(void)
impl_afw_pool_add_reference(
    const afw_pool_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_destroy
/* Declare method destroy */
AFW_DECLARE_STATIC(void)
impl_afw_pool_destroy(
    const afw_pool_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_get_apr_pool
/* Declare method get_apr_pool */
AFW_DECLARE_STATIC(apr_pool_t *)
impl_afw_pool_get_apr_pool(
    const afw_pool_t * instance);
#endif

#ifndef impl_afw_pool_calloc
/* Declare method calloc */
AFW_DECLARE_STATIC(void *)
impl_afw_pool_calloc(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_malloc
/* Declare method malloc */
AFW_DECLARE_STATIC(void *)
impl_afw_pool_malloc(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_free
/* Declare method free */
AFW_DECLARE_STATIC(void)
impl_afw_pool_free(
    const afw_pool_t * instance,
    void * address,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_register_cleanup_before
/* Declare method register_cleanup_before */
AFW_DECLARE_STATIC(void)
impl_afw_pool_register_cleanup_before(
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_deregister_cleanup
/* Declare method deregister_cleanup */
AFW_DECLARE_STATIC(void)
impl_afw_pool_deregister_cleanup(
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_pool_release_debug
/* Declare method release_debug */
AFW_DECLARE_STATIC(void)
impl_afw_pool_release_debug(
    const afw_pool_t * instance,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_add_reference_debug
/* Declare method add_reference_debug */
AFW_DECLARE_STATIC(void)
impl_afw_pool_add_reference_debug(
    const afw_pool_t * instance,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_destroy_debug
/* Declare method destroy_debug */
AFW_DECLARE_STATIC(void)
impl_afw_pool_destroy_debug(
    const afw_pool_t * instance,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_calloc_debug
/* Declare method calloc_debug */
AFW_DECLARE_STATIC(void *)
impl_afw_pool_calloc_debug(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_malloc_debug
/* Declare method malloc_debug */
AFW_DECLARE_STATIC(void *)
impl_afw_pool_malloc_debug(
    const afw_pool_t * instance,
    afw_size_t size,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_free_debug
/* Declare method free_debug */
AFW_DECLARE_STATIC(void)
impl_afw_pool_free_debug(
    const afw_pool_t * instance,
    void * address,
    afw_size_t size,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_register_cleanup_before_debug
/* Declare method register_cleanup_before_debug */
AFW_DECLARE_STATIC(void)
impl_afw_pool_register_cleanup_before_debug(
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
#endif

#ifndef impl_afw_pool_deregister_cleanup_debug
/* Declare method deregister_cleanup_debug */
AFW_DECLARE_STATIC(void)
impl_afw_pool_deregister_cleanup_debug(
    const afw_pool_t * instance,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t * xctx,
    const afw_utf8_z_t * source_z);
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
    impl_afw_pool_release,
    impl_afw_pool_add_reference,
    impl_afw_pool_destroy,
    impl_afw_pool_get_apr_pool,
    impl_afw_pool_calloc,
    impl_afw_pool_malloc,
    impl_afw_pool_free,
    impl_afw_pool_register_cleanup_before,
    impl_afw_pool_deregister_cleanup,
    impl_afw_pool_release_debug,
    impl_afw_pool_add_reference_debug,
    impl_afw_pool_destroy_debug,
    impl_afw_pool_calloc_debug,
    impl_afw_pool_malloc_debug,
    impl_afw_pool_free_debug,
    impl_afw_pool_register_cleanup_before_debug,
    impl_afw_pool_deregister_cleanup_debug
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

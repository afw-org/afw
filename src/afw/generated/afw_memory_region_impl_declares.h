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
 * @addtogroup afw_memory_region_interface afw_memory_region
 * @{
 */

/**
 * @addtogroup afw_memory_region_impl_declares afw_memory_region
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_memory_region.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_memory_region.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_memory_region_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_memory_region_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_memory_region_rti.
 * 3. inf will be defined with a name of impl_afw_memory_region_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_memory_region_inf'.
 *
 *   AFW_MEMORY_REGION_SELF_T - (optional) defaults to 'const afw_memory_region_t'.
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
 *   #include "afw_memory_region_impl_declares.h"
 *
 * A second #include "afw_memory_region_impl_declares.h" can be specified if
" *   #define "AFW_MEMORY_REGION_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_memory_region_impl_declares.h
 * @brief Generated implementation declares for interface afw_interface.
 */

#ifdef __AFW_MEMORY_REGION_IMPL_DECLARES_H__
#ifndef AFW_MEMORY_REGION_INF_ONLY
#error "afw_memory_region_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_MEMORY_REGION_IMPL_DECLARES_H__
#ifdef AFW_MEMORY_REGION_INF_ONLY
#error "afw_memory_region_impl_declares.h must be included before AFW_MEMORY_REGION_INF_ONLY."
#endif
#define __AFW_MEMORY_REGION_IMPL_DECLARES_H__ 1
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

#ifndef AFW_MEMORY_REGION_SELF_T
#define AFW_MEMORY_REGION_SELF_T const afw_memory_region_t
#endif

#ifndef AFW_MEMORY_REGION_INF_ONLY

#ifndef impl_afw_memory_region_get
/* Declare method get */
AFW_DECLARE_STATIC(void)
impl_afw_memory_region_get(
    AFW_MEMORY_REGION_SELF_T *self,
    void ** region,
    afw_size_t * size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_memory_region_free
/* Declare method free */
AFW_DECLARE_STATIC(void)
impl_afw_memory_region_free(
    AFW_MEMORY_REGION_SELF_T *self,
    void * region,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_memory_region_cleanup
/* Declare method cleanup */
AFW_DECLARE_STATIC(void)
impl_afw_memory_region_cleanup(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_memory_region_lock
/* Declare method lock */
AFW_DECLARE_STATIC(void)
impl_afw_memory_region_lock(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_memory_region_unlock
/* Declare method unlock */
AFW_DECLARE_STATIC(void)
impl_afw_memory_region_unlock(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_memory_region_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_memory_region_release(
    AFW_MEMORY_REGION_SELF_T *self,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_memory_region */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_memory_region_inf_t)
#else
static const afw_memory_region_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_memory_region_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_memory_region"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_memory_region_get_t)
    impl_afw_memory_region_get,
    (afw_memory_region_free_t)
    impl_afw_memory_region_free,
    (afw_memory_region_cleanup_t)
    impl_afw_memory_region_cleanup,
    (afw_memory_region_lock_t)
    impl_afw_memory_region_lock,
    (afw_memory_region_unlock_t)
    impl_afw_memory_region_unlock,
    (afw_memory_region_release_t)
    impl_afw_memory_region_release
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

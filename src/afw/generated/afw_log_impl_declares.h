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
 * @addtogroup afw_log_interface afw_log
 * @{
 */

/**
 * @addtogroup afw_log_impl_declares afw_log
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_log.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_log.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_log_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_log_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_log_rti.
 * 3. inf will be defined with a name of impl_afw_log_inf.
 *
 * Before including, define the following symbols:
 *
 * - AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 * - AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 * - AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to impl_afw_log_inf.
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
 *   #include "afw_log_impl_declares.h"
 *
 * A second #include "afw_log_impl_declares.h" can be specified if
" *   #define "AFW_LOG_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_log_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_LOG_IMPL_DECLARES_H__
#ifndef AFW_LOG_INF_ONLY
#error "afw_log_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_LOG_IMPL_DECLARES_H__
#ifdef AFW_LOG_INF_ONLY
#error "afw_log_impl_declares.h must be included before AFW_LOG_INF_ONLY."
#endif
#define __AFW_LOG_IMPL_DECLARES_H__ 1
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
#ifndef AFW_LOG_INF_ONLY

#ifndef impl_afw_log_destroy
/* Declare method destroy */
AFW_DECLARE_STATIC(void)
impl_afw_log_destroy(
    const afw_log_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_log_set_own_mask
/* Declare method set_own_mask */
AFW_DECLARE_STATIC(void)
impl_afw_log_set_own_mask(
    const afw_log_t * instance,
    afw_log_priority_mask_t mask,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_log_write
/* Declare method write */
AFW_DECLARE_STATIC(void)
impl_afw_log_write(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_t * message,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_log */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_log_inf_t)
#else
static const afw_log_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_log_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_log"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    impl_afw_log_destroy,
    impl_afw_log_set_own_mask,
    impl_afw_log_write
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

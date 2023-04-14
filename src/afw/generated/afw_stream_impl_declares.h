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
 * @addtogroup afw_stream_interface afw_stream
 * @{
 */

/**
 * @addtogroup afw_stream_impl_declares afw_stream
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_stream.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_stream.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_stream_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_stream_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_stream_rti.
 * 3. inf will be defined with a name of impl_afw_stream_inf.
 *
 * Before including, #define the following symbols:
 *
 * - AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 * - AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 * - AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to impl_afw_stream_inf.
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
 *   #include "afw_stream_impl_declares.h"
 *
 * A second #include "afw_stream_impl_declares.h" can be specified if
" *   #define "AFW_STREAM_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_stream_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_STREAM_IMPL_DECLARES_H__
#ifndef AFW_STREAM_INF_ONLY
#error "afw_stream_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_STREAM_IMPL_DECLARES_H__
#ifdef AFW_STREAM_INF_ONLY
#error "afw_stream_impl_declares.h must be included before AFW_STREAM_INF_ONLY."
#endif
#define __AFW_STREAM_IMPL_DECLARES_H__ 1
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
#ifndef AFW_STREAM_INF_ONLY

#ifndef impl_afw_stream_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_stream_release(
    const afw_stream_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_stream_flush
/* Declare method flush */
AFW_DECLARE_STATIC(void)
impl_afw_stream_flush(
    const afw_stream_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_stream_read
/* Declare method read */
AFW_DECLARE_STATIC(void)
impl_afw_stream_read(
    const afw_stream_t * instance,
    const void * buffer,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_stream_write
/* Declare method write */
AFW_DECLARE_STATIC(void)
impl_afw_stream_write(
    const afw_stream_t * instance,
    const void * buffer,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_stream */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_stream_inf_t)
#else
static const afw_stream_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_stream_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_stream"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    impl_afw_stream_release,
    impl_afw_stream_flush,
    impl_afw_stream_read,
    impl_afw_stream_write
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

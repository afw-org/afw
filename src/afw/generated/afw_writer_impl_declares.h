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
 * @addtogroup afw_writer_interface afw_writer
 * @{
 */

/**
 * @addtogroup afw_writer_impl_declares afw_writer
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_writer.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_writer.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_writer_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_writer_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_writer_rti.
 * 3. inf will be defined with a name of impl_afw_writer_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_writer_inf'.
 *
 *   AFW_WRITER_SELF_T - (optional) defaults to 'const afw_writer_t'.
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
 *   #include "afw_writer_impl_declares.h"
 *
 * A second #include "afw_writer_impl_declares.h" can be specified if
" *   #define "AFW_WRITER_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_writer_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_WRITER_IMPL_DECLARES_H__
#ifndef AFW_WRITER_INF_ONLY
#error "afw_writer_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_WRITER_IMPL_DECLARES_H__
#ifdef AFW_WRITER_INF_ONLY
#error "afw_writer_impl_declares.h must be included before AFW_WRITER_INF_ONLY."
#endif
#define __AFW_WRITER_IMPL_DECLARES_H__ 1
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

#ifndef AFW_WRITER_SELF_T
#define AFW_WRITER_SELF_T const afw_writer_t
#endif

#ifndef AFW_WRITER_INF_ONLY

#ifndef impl_afw_writer_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_writer_release(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_writer_flush
/* Declare method flush */
AFW_DECLARE_STATIC(void)
impl_afw_writer_flush(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_writer_write
/* Declare method write */
AFW_DECLARE_STATIC(void)
impl_afw_writer_write(
    AFW_WRITER_SELF_T *self,
    const void * buffer,
    afw_size_t size,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_writer_write_eol
/* Declare method write_eol */
AFW_DECLARE_STATIC(void)
impl_afw_writer_write_eol(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_writer_increment_indent
/* Declare method increment_indent */
AFW_DECLARE_STATIC(void)
impl_afw_writer_increment_indent(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_writer_decrement_indent
/* Declare method decrement_indent */
AFW_DECLARE_STATIC(void)
impl_afw_writer_decrement_indent(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_writer */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_writer_inf_t)
#else
static const afw_writer_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_writer_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_writer"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_writer_release_t)
    impl_afw_writer_release,
    (afw_writer_flush_t)
    impl_afw_writer_flush,
    (afw_writer_write_t)
    impl_afw_writer_write,
    (afw_writer_write_eol_t)
    impl_afw_writer_write_eol,
    (afw_writer_increment_indent_t)
    impl_afw_writer_increment_indent,
    (afw_writer_decrement_indent_t)
    impl_afw_writer_decrement_indent
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

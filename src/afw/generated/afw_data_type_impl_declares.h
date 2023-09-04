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
 * @addtogroup afw_data_type_interface afw_data_type
 * @{
 */

/**
 * @addtogroup afw_data_type_impl_declares afw_data_type
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_data_type.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_data_type.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_data_type_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_data_type_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_data_type_rti.
 * 3. inf will be defined with a name of impl_afw_data_type_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_data_type_inf'.
 *
 *   AFW_DATA_TYPE_SELF_T - (optional) defaults to 'const afw_data_type_t'.
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
 *   #include "afw_data_type_impl_declares.h"
 *
 * A second #include "afw_data_type_impl_declares.h" can be specified if
" *   #define "AFW_DATA_TYPE_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_data_type_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_DATA_TYPE_IMPL_DECLARES_H__
#ifndef AFW_DATA_TYPE_INF_ONLY
#error "afw_data_type_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_DATA_TYPE_IMPL_DECLARES_H__
#ifdef AFW_DATA_TYPE_INF_ONLY
#error "afw_data_type_impl_declares.h must be included before AFW_DATA_TYPE_INF_ONLY."
#endif
#define __AFW_DATA_TYPE_IMPL_DECLARES_H__ 1
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

#ifndef AFW_DATA_TYPE_SELF_T
#define AFW_DATA_TYPE_SELF_T const afw_data_type_t
#endif

#ifndef AFW_DATA_TYPE_INF_ONLY

#ifndef impl_afw_data_type_internal_to_utf8
/* Declare method internal_to_utf8 */
AFW_DECLARE_STATIC(const afw_utf8_t *)
impl_afw_data_type_internal_to_utf8(
    AFW_DATA_TYPE_SELF_T *self,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_data_type_utf8_to_internal
/* Declare method utf8_to_internal */
AFW_DECLARE_STATIC(void)
impl_afw_data_type_utf8_to_internal(
    AFW_DATA_TYPE_SELF_T *self,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_data_type_compare_internal
/* Declare method compare_internal */
AFW_DECLARE_STATIC(int)
impl_afw_data_type_compare_internal(
    AFW_DATA_TYPE_SELF_T *self,
    const void * internal1,
    const void * internal2,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_data_type_convert_internal
/* Declare method convert_internal */
AFW_DECLARE_STATIC(void)
impl_afw_data_type_convert_internal(
    AFW_DATA_TYPE_SELF_T *self,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t * to_data_type,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_data_type_clone_internal
/* Declare method clone_internal */
AFW_DECLARE_STATIC(void)
impl_afw_data_type_clone_internal(
    AFW_DATA_TYPE_SELF_T *self,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_data_type_value_compiler_listing
/* Declare method value_compiler_listing */
AFW_DECLARE_STATIC(void)
impl_afw_data_type_value_compiler_listing(
    AFW_DATA_TYPE_SELF_T *self,
    const afw_writer_t * writer,
    const afw_value_t * value,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_data_type_write_as_expression
/* Declare method write_as_expression */
AFW_DECLARE_STATIC(void)
impl_afw_data_type_write_as_expression(
    AFW_DATA_TYPE_SELF_T *self,
    const afw_writer_t * writer,
    const void * from_internal,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_data_type */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_data_type_inf_t)
#else
static const afw_data_type_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_data_type_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_data_type"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_data_type_internal_to_utf8_t)
    impl_afw_data_type_internal_to_utf8,
    (afw_data_type_utf8_to_internal_t)
    impl_afw_data_type_utf8_to_internal,
    (afw_data_type_compare_internal_t)
    impl_afw_data_type_compare_internal,
    (afw_data_type_convert_internal_t)
    impl_afw_data_type_convert_internal,
    (afw_data_type_clone_internal_t)
    impl_afw_data_type_clone_internal,
    (afw_data_type_value_compiler_listing_t)
    impl_afw_data_type_value_compiler_listing,
    (afw_data_type_write_as_expression_t)
    impl_afw_data_type_write_as_expression
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

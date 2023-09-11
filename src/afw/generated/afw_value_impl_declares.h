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
 * @addtogroup afw_value_interface afw_value
 * @{
 */

/**
 * @addtogroup afw_value_impl_declares afw_value
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_value.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_value.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_value_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_value_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_value_rti.
 * 3. inf will be defined with a name of impl_afw_value_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_value_inf'.
 *
 *   AFW_VALUE_SELF_T - (optional) defaults to 'const afw_value_t'.
 *       The const is not required and normally should not be specified. It is
 *       the default for historical reasons.
 *
 * - AFW_IMPLEMENTATION_INF_VARIABLES - (optional) this will be added at the end of
 *       the inf initializer and should represent the inf_variable
 *       defined in interface.
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
 *   #include "afw_value_impl_declares.h"
 *
 * A second #include "afw_value_impl_declares.h" can be specified if
" *   #define "AFW_VALUE_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_value_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_VALUE_IMPL_DECLARES_H__
#ifndef AFW_VALUE_INF_ONLY
#error "afw_value_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_VALUE_IMPL_DECLARES_H__
#ifdef AFW_VALUE_INF_ONLY
#error "afw_value_impl_declares.h must be included before AFW_VALUE_INF_ONLY."
#endif
#define __AFW_VALUE_IMPL_DECLARES_H__ 1
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

#ifndef AFW_VALUE_SELF_T
#define AFW_VALUE_SELF_T const afw_value_t
#endif

#ifndef AFW_VALUE_INF_ONLY

#ifndef impl_afw_value_optional_release
/* Declare method optional_release */
AFW_DECLARE_STATIC(void)
impl_afw_value_optional_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_clone_or_reference
/* Declare method clone_or_reference */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_clone_or_reference(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_optional_evaluate
/* Declare method optional_evaluate */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_get_data_type
/* Declare method get_data_type */
AFW_DECLARE_STATIC(const afw_data_type_t *)
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_get_evaluated_meta
/* Declare method get_evaluated_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_get_evaluated_meta(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_get_evaluated_metas
/* Declare method get_evaluated_metas */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_get_evaluated_metas(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_produce_compiler_listing
/* Declare method produce_compiler_listing */
AFW_DECLARE_STATIC(void)
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_decompile
/* Declare method decompile */
AFW_DECLARE_STATIC(void)
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_value_get_info
/* Declare method get_info */
AFW_DECLARE_STATIC(void)
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t * info,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_value */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_value_inf_t)
#else
static const afw_value_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_value_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_value"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_value_optional_release_t)
    impl_afw_value_optional_release,
    (afw_value_clone_or_reference_t)
    impl_afw_value_clone_or_reference,
    (afw_value_optional_evaluate_t)
    impl_afw_value_optional_evaluate,
    (afw_value_get_data_type_t)
    impl_afw_value_get_data_type,
    (afw_value_get_evaluated_meta_t)
    impl_afw_value_get_evaluated_meta,
    (afw_value_get_evaluated_metas_t)
    impl_afw_value_get_evaluated_metas,
    (afw_value_produce_compiler_listing_t)
    impl_afw_value_produce_compiler_listing,
    (afw_value_decompile_t)
    impl_afw_value_decompile,
    (afw_value_get_info_t)
    impl_afw_value_get_info
#ifdef AFW_IMPLEMENTATION_INF_VARIABLES
    ,AFW_IMPLEMENTATION_INF_VARIABLES
#endif

};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

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
 * @addtogroup afw_array_setter_interface afw_array_setter
 * @{
 */

/**
 * @addtogroup afw_array_setter_impl_declares afw_array_setter
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_array_setter.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_array_setter.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_array_setter_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_array_setter_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_array_setter_rti.
 * 3. inf will be defined with a name of impl_afw_array_setter_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_array_setter_inf'.
 *
 *   AFW_ARRAY_SETTER_SELF_T - (optional) defaults to 'const afw_array_setter_t'.
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
 *   #include "afw_array_setter_impl_declares.h"
 *
 * A second #include "afw_array_setter_impl_declares.h" can be specified if
" *   #define "AFW_ARRAY_SETTER_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_array_setter_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_ARRAY_SETTER_IMPL_DECLARES_H__
#ifndef AFW_ARRAY_SETTER_INF_ONLY
#error "afw_array_setter_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_ARRAY_SETTER_IMPL_DECLARES_H__
#ifdef AFW_ARRAY_SETTER_INF_ONLY
#error "afw_array_setter_impl_declares.h must be included before AFW_ARRAY_SETTER_INF_ONLY."
#endif
#define __AFW_ARRAY_SETTER_IMPL_DECLARES_H__ 1
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

#ifndef AFW_ARRAY_SETTER_SELF_T
#define AFW_ARRAY_SETTER_SELF_T const afw_array_setter_t
#endif

#ifndef AFW_ARRAY_SETTER_INF_ONLY

#ifndef impl_afw_array_setter_set_immutable
/* Declare method set_immutable */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_set_immutable(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_determine_data_type_and_set_immutable
/* Declare method determine_data_type_and_set_immutable */
AFW_DECLARE_STATIC(const afw_data_type_t *)
impl_afw_array_setter_determine_data_type_and_set_immutable(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_add_internal
/* Declare method add_internal */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_add_internal(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_add_value
/* Declare method add_value */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_add_value(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_value_t * value,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_insert_internal
/* Declare method insert_internal */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_insert_internal(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_size_t index,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_insert_value
/* Declare method insert_value */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_insert_value(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_value_t * value,
    afw_size_t index,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_remove_all_values
/* Declare method remove_all_values */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_remove_all_values(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_remove_internal
/* Declare method remove_internal */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_remove_internal(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_remove_value
/* Declare method remove_value */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_remove_value(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_value_t * value,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_setter_set_value_by_index
/* Declare method set_value_by_index */
AFW_DECLARE_STATIC(void)
impl_afw_array_setter_set_value_by_index(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_size_t index,
    const afw_value_t * value,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_array_setter */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_array_setter_inf_t)
#else
static const afw_array_setter_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_array_setter_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_array_setter"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_array_setter_set_immutable_t)
    impl_afw_array_setter_set_immutable,
    (afw_array_setter_determine_data_type_and_set_immutable_t)
    impl_afw_array_setter_determine_data_type_and_set_immutable,
    (afw_array_setter_add_internal_t)
    impl_afw_array_setter_add_internal,
    (afw_array_setter_add_value_t)
    impl_afw_array_setter_add_value,
    (afw_array_setter_insert_internal_t)
    impl_afw_array_setter_insert_internal,
    (afw_array_setter_insert_value_t)
    impl_afw_array_setter_insert_value,
    (afw_array_setter_remove_all_values_t)
    impl_afw_array_setter_remove_all_values,
    (afw_array_setter_remove_internal_t)
    impl_afw_array_setter_remove_internal,
    (afw_array_setter_remove_value_t)
    impl_afw_array_setter_remove_value,
    (afw_array_setter_set_value_by_index_t)
    impl_afw_array_setter_set_value_by_index
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

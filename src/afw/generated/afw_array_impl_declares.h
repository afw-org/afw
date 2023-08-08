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
 * @addtogroup afw_array_interface afw_array
 * @{
 */

/**
 * @addtogroup afw_array_impl_declares afw_array
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_array.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_array.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_array_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_array_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_array_rti.
 * 3. inf will be defined with a name of impl_afw_array_inf.
 *
 * Before including, define the following symbols:
 *
 * - AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 * - AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 * - AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to impl_afw_array_inf.
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
 *   #include "afw_array_impl_declares.h"
 *
 * A second #include "afw_array_impl_declares.h" can be specified if
" *   #define "AFW_ARRAY_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_array_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_ARRAY_IMPL_DECLARES_H__
#ifndef AFW_ARRAY_INF_ONLY
#error "afw_array_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_ARRAY_IMPL_DECLARES_H__
#ifdef AFW_ARRAY_INF_ONLY
#error "afw_array_impl_declares.h must be included before AFW_ARRAY_INF_ONLY."
#endif
#define __AFW_ARRAY_IMPL_DECLARES_H__ 1
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
#ifndef AFW_ARRAY_INF_ONLY

#ifndef impl_afw_array_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_array_release(
    const afw_array_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_count
/* Declare method get_count */
AFW_DECLARE_STATIC(afw_size_t)
impl_afw_array_get_count(
    const afw_array_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_data_type
/* Declare method get_data_type */
AFW_DECLARE_STATIC(const afw_data_type_t *)
impl_afw_array_get_data_type(
    const afw_array_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_entry_meta
/* Declare method get_entry_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_array_get_entry_meta(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_entry_internal
/* Declare method get_entry_internal */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_array_get_entry_internal(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_entry_value
/* Declare method get_entry_value */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_array_get_entry_value(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_next_entry_meta
/* Declare method get_next_entry_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_array_get_next_entry_meta(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_next_internal
/* Declare method get_next_internal */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_array_get_next_internal(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_next_value
/* Declare method get_next_value */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_array_get_next_value(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_array_get_setter
/* Declare method get_setter */
AFW_DECLARE_STATIC(const afw_array_setter_t *)
impl_afw_array_get_setter(
    const afw_array_t * instance,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_array */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_array_inf_t)
#else
static const afw_array_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_array_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_array"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    impl_afw_array_release,
    impl_afw_array_get_count,
    impl_afw_array_get_data_type,
    impl_afw_array_get_entry_meta,
    impl_afw_array_get_entry_internal,
    impl_afw_array_get_entry_value,
    impl_afw_array_get_next_entry_meta,
    impl_afw_array_get_next_internal,
    impl_afw_array_get_next_value,
    impl_afw_array_get_setter
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

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
 * @addtogroup afw_object_interface afw_object
 * @{
 */

/**
 * @addtogroup afw_object_impl_declares afw_object
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_object.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file generated/interface_closet/skeleton_afw_object.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_object_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_object_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_object_rti.
 * 3. inf will be defined with a name of impl_afw_object_inf.
 *
 * Before including, define the following symbols:
 *
 * - AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 * - AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 * - AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to impl_afw_object_inf.
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
 *   #include "afw_object_impl_declares.h"
 *
 * A second #include "afw_object_impl_declares.h" can be specified if
" *   #define "AFW_OBJECT_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_object_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_OBJECT_IMPL_DECLARES_H__
#ifndef AFW_OBJECT_INF_ONLY
#error "afw_object_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_OBJECT_IMPL_DECLARES_H__
#ifdef AFW_OBJECT_INF_ONLY
#error "afw_object_impl_declares.h must be included before AFW_OBJECT_INF_ONLY."
#endif
#define __AFW_OBJECT_IMPL_DECLARES_H__ 1
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
#ifndef AFW_OBJECT_INF_ONLY

#ifndef impl_afw_object_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_object_release(
    const afw_object_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_add_reference
/* Declare method add_reference */
AFW_DECLARE_STATIC(void)
impl_afw_object_add_reference(
    const afw_object_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_count
/* Declare method get_count */
AFW_DECLARE_STATIC(afw_size_t)
impl_afw_object_get_count(
    const afw_object_t * instance,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_entry
/* Declare method get_entry */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_entry(
    const afw_object_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_entry_meta
/* Declare method get_entry_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_entry_meta(
    const afw_object_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_meta
/* Declare method get_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_meta(
    const afw_object_t * instance,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_property
/* Declare method get_property */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_property_meta
/* Declare method get_property_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_property_meta(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_next_property
/* Declare method get_next_property */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_next_property(
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_next_property_meta
/* Declare method get_next_property_meta */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_object_get_next_property_meta(
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_has_property
/* Declare method has_property */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_object_has_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_get_setter
/* Declare method get_setter */
AFW_DECLARE_STATIC(const afw_object_setter_t *)
impl_afw_object_get_setter(
    const afw_object_t * instance,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_object */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_object_inf_t)
#else
static const afw_object_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_object_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_object"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    impl_afw_object_release,
    impl_afw_object_add_reference,
    impl_afw_object_get_count,
    impl_afw_object_get_entry,
    impl_afw_object_get_entry_meta,
    impl_afw_object_get_meta,
    impl_afw_object_get_property,
    impl_afw_object_get_property_meta,
    impl_afw_object_get_next_property,
    impl_afw_object_get_next_property_meta,
    impl_afw_object_has_property,
    impl_afw_object_get_setter
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

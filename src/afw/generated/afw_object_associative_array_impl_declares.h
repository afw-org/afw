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
 * @addtogroup afw_object_associative_array_interface afw_object_associative_array
 * @{
 */

/**
 * @addtogroup afw_object_associative_array_impl_declares afw_object_associative_array
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_object_associative_array.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_object_associative_array.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_object_associative_array_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_object_associative_array_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_object_associative_array_rti.
 * 3. inf will be defined with a name of impl_afw_object_associative_array_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_object_associative_array_inf'.
 *
 *   AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T - (optional) defaults to 'const afw_object_associative_array_t'.
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
 *   #include "afw_object_associative_array_impl_declares.h"
 *
 * A second #include "afw_object_associative_array_impl_declares.h" can be specified if
" *   #define "AFW_OBJECT_ASSOCIATIVE_ARRAY_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_object_associative_array_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_OBJECT_ASSOCIATIVE_ARRAY_IMPL_DECLARES_H__
#ifndef AFW_OBJECT_ASSOCIATIVE_ARRAY_INF_ONLY
#error "afw_object_associative_array_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_OBJECT_ASSOCIATIVE_ARRAY_IMPL_DECLARES_H__
#ifdef AFW_OBJECT_ASSOCIATIVE_ARRAY_INF_ONLY
#error "afw_object_associative_array_impl_declares.h must be included before AFW_OBJECT_ASSOCIATIVE_ARRAY_INF_ONLY."
#endif
#define __AFW_OBJECT_ASSOCIATIVE_ARRAY_IMPL_DECLARES_H__ 1
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

#ifndef AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T
#define AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T const afw_object_associative_array_t
#endif

#ifndef AFW_OBJECT_ASSOCIATIVE_ARRAY_INF_ONLY

#ifndef impl_afw_object_associative_array_release
/* Declare method release */
AFW_DECLARE_STATIC(void)
impl_afw_object_associative_array_release(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_associative_array_add_reference
/* Declare method add_reference */
AFW_DECLARE_STATIC(void)
impl_afw_object_associative_array_add_reference(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_associative_array_get
/* Declare method get */
AFW_DECLARE_STATIC(const afw_object_t *)
impl_afw_object_associative_array_get(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    const afw_utf8_t * key,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_associative_array_get_reference
/* Declare method get_reference */
AFW_DECLARE_STATIC(const afw_object_t *)
impl_afw_object_associative_array_get_reference(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    const afw_utf8_t * key,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_associative_array_for_each
/* Declare method for_each */
AFW_DECLARE_STATIC(void)
impl_afw_object_associative_array_for_each(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    void * context,
    afw_object_cb_t cb,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_object_associative_array_set
/* Declare method set */
AFW_DECLARE_STATIC(void)
impl_afw_object_associative_array_set(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    const afw_utf8_t * key,
    const afw_object_t * object,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_object_associative_array */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_object_associative_array_inf_t)
#else
static const afw_object_associative_array_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_object_associative_array_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_object_associative_array"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_object_associative_array_release_t)
    impl_afw_object_associative_array_release,
    (afw_object_associative_array_add_reference_t)
    impl_afw_object_associative_array_add_reference,
    (afw_object_associative_array_get_t)
    impl_afw_object_associative_array_get,
    (afw_object_associative_array_get_reference_t)
    impl_afw_object_associative_array_get_reference,
    (afw_object_associative_array_for_each_t)
    impl_afw_object_associative_array_for_each,
    (afw_object_associative_array_set_t)
    impl_afw_object_associative_array_set
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

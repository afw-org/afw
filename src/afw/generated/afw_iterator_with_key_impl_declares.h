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
 * @addtogroup afw_iterator_with_key_interface afw_iterator_with_key
 * @{
 */

/**
 * @addtogroup afw_iterator_with_key_impl_declares afw_iterator_with_key
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_iterator_with_key.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_iterator_with_key.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_iterator_with_key_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_iterator_with_key_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_iterator_with_key_rti.
 * 3. inf will be defined with a name of impl_afw_iterator_with_key_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_iterator_with_key_inf'.
 *
 *   AFW_ITERATOR_WITH_KEY_SELF_T - (optional) defaults to 'const afw_iterator_with_key_t'.
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
 *   #include "afw_iterator_with_key_impl_declares.h"
 *
 * A second #include "afw_iterator_with_key_impl_declares.h" can be specified if
" *   #define "AFW_ITERATOR_WITH_KEY_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_iterator_with_key_impl_declares.h
 * @brief Generated implementation declares for interface afw_interface.
 */

#ifdef __AFW_ITERATOR_WITH_KEY_IMPL_DECLARES_H__
#ifndef AFW_ITERATOR_WITH_KEY_INF_ONLY
#error "afw_iterator_with_key_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_ITERATOR_WITH_KEY_IMPL_DECLARES_H__
#ifdef AFW_ITERATOR_WITH_KEY_INF_ONLY
#error "afw_iterator_with_key_impl_declares.h must be included before AFW_ITERATOR_WITH_KEY_INF_ONLY."
#endif
#define __AFW_ITERATOR_WITH_KEY_IMPL_DECLARES_H__ 1
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

#ifndef AFW_ITERATOR_WITH_KEY_SELF_T
#define AFW_ITERATOR_WITH_KEY_SELF_T const afw_iterator_with_key_t
#endif

#ifndef AFW_ITERATOR_WITH_KEY_INF_ONLY

#ifndef impl_afw_iterator_with_key_get_next
/* Declare method get_next */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_iterator_with_key_get_next(
    AFW_ITERATOR_WITH_KEY_SELF_T *self,
    const afw_value_t ** key,
    const afw_value_t ** value,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_iterator_with_key_get_by_index
/* Declare method get_by_index */
AFW_DECLARE_STATIC(afw_boolean_t)
impl_afw_iterator_with_key_get_by_index(
    AFW_ITERATOR_WITH_KEY_SELF_T *self,
    afw_integer_t index,
    const afw_value_t ** key,
    const afw_value_t ** value,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_iterator_with_key_get_count
/* Declare method get_count */
AFW_DECLARE_STATIC(afw_size_t)
impl_afw_iterator_with_key_get_count(
    AFW_ITERATOR_WITH_KEY_SELF_T *self,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_iterator_with_key */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_iterator_with_key_inf_t)
#else
static const afw_iterator_with_key_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_iterator_with_key_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_iterator_with_key"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_iterator_with_key_get_next_t)
    impl_afw_iterator_with_key_get_next,
    (afw_iterator_with_key_get_by_index_t)
    impl_afw_iterator_with_key_get_by_index,
    (afw_iterator_with_key_get_count_t)
    impl_afw_iterator_with_key_get_count
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

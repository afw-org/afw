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
 * @addtogroup afw_content_type_interface afw_content_type
 * @{
 */

/**
 * @addtogroup afw_content_type_impl_declares afw_content_type
 * @{
 *
 * This file should only be included in c source files that implement interface
 * afw_content_type.
 *
 * You can find a skeleton for using this #include file in the corresponding
 * source directory in file
 * generated/interface_closet/skeleton_afw_content_type.c.
 *
 * Mutiple *_impl.declares.h can be included in each c source file, but only
 * one for each interface name.
 *
 * After the #include for  afw_content_type_impl_declares.h:
 *
 * 1. declarations will exist for all of the interfaces methods with names of
 *    impl_afw_content_type_ followed by the method name.
 * 2. rti will be defined with a name of impl_afw_content_type_rti.
 * 3. inf will be defined with a name of impl_afw_content_type_inf.
 *
 * Before including, define the following symbols:
 *
 *   AFW_IMPLEMENTATION_ID - Implementation id string for this implementation.
 *
 *   AFW_IMPLEMENTATION_INF_SPECIFIER - (optional) defaults to static.
 *
 *   AFW_IMPLEMENTATION_INF_LABEL - (optional) defaults to 'impl_afw_content_type_inf'.
 *
 *   AFW_CONTENT_TYPE_SELF_T - (optional) defaults to 'const afw_content_type_t'.
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
 *   #include "afw_content_type_impl_declares.h"
 *
 * A second #include "afw_content_type_impl_declares.h" can be specified if
" *   #define "AFW_CONTENT_TYPE_INF_ONLY" is specified to generate inf only."
 *~~~~~~~~~~~~~~~
 */


/**
 * @file afw_content_type_impl_declares.h
 * @brief Interface afw_interface implementation declares.
 */

#ifdef __AFW_CONTENT_TYPE_IMPL_DECLARES_H__
#ifndef AFW_CONTENT_TYPE_INF_ONLY
#error "afw_content_type_impl_declares.h already included."
#endif
#endif
#ifndef __AFW_CONTENT_TYPE_IMPL_DECLARES_H__
#ifdef AFW_CONTENT_TYPE_INF_ONLY
#error "afw_content_type_impl_declares.h must be included before AFW_CONTENT_TYPE_INF_ONLY."
#endif
#define __AFW_CONTENT_TYPE_IMPL_DECLARES_H__ 1
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

#ifndef AFW_CONTENT_TYPE_SELF_T
#define AFW_CONTENT_TYPE_SELF_T const afw_content_type_t
#endif

#ifndef AFW_CONTENT_TYPE_INF_ONLY

#ifndef impl_afw_content_type_raw_to_value
/* Declare method raw_to_value */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_content_type_raw_to_value(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_content_type_raw_to_object
/* Declare method raw_to_object */
AFW_DECLARE_STATIC(const afw_object_t *)
impl_afw_content_type_raw_to_object(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adapter_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_content_type_write_value
/* Declare method write_value */
AFW_DECLARE_STATIC(void)
impl_afw_content_type_write_value(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_value_t * value,
    const afw_object_options_t * options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif

#ifndef impl_afw_content_type_create_object_list_writer
/* Declare method create_object_list_writer */
AFW_DECLARE_STATIC(const afw_content_type_object_list_writer_t *)
impl_afw_content_type_create_object_list_writer(
    AFW_CONTENT_TYPE_SELF_T *self,
    const afw_object_options_t * options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t * p,
    afw_xctx_t * xctx);
#endif
#endif

/* inf for interface afw_content_type */
#ifdef __BASE_FILE__
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __BASE_FILE__
#else
#define _AFW_IMPLEMENTATION_SOURCE_FILE_ __FILE__
#endif
#ifdef AFW_IMPLEMENTATION_INF_SPECIFIER
AFW_IMPLEMENTATION_INF_SPECIFIER(afw_content_type_inf_t)
#else
static const afw_content_type_inf_t
#endif
#ifdef AFW_IMPLEMENTATION_INF_LABEL
AFW_IMPLEMENTATION_INF_LABEL = {
#else
impl_afw_content_type_inf = {
#endif
    {
        AFW_UTF8_LITERAL("afw_content_type"),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_SOURCE_FILE_),
        AFW_UTF8_LITERAL(_AFW_IMPLEMENTATION_ID_),
        _AFW_IMPLEMENTATION_SPECIFIC_
    },
    (afw_content_type_raw_to_value_t)
    impl_afw_content_type_raw_to_value,
    (afw_content_type_raw_to_object_t)
    impl_afw_content_type_raw_to_object,
    (afw_content_type_write_value_t)
    impl_afw_content_type_write_value,
    (afw_content_type_create_object_list_writer_t)
    impl_afw_content_type_create_object_list_writer
};

#undef _AFW_IMPLEMENTATION_ID_
#undef _AFW_IMPLEMENTATION_SPECIFIC_
#undef _AFW_IMPLEMENTATION_SOURCE_FILE_

/** @} */

/** @} */

/** @} */

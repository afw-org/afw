// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Data Type Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_DATA_TYPE_H__
#define __AFW_DATA_TYPE_H__

#include "afw_interface.h"

/**
 * @defgroup afw_data_type Data type
 * @ingroup afw_c_api_public
 * 
 * Common to all Adaptive Framework applications
 * @{
 */

/**
 * @file afw_data_type.h
 * @brief Header for Adaptive Framework data type.
 *
 * Also see afw_core_data_types.h.
 */

AFW_BEGIN_DECLARES

/** @brief afw_boolean_t true value. */
AFW_DECLARE_CONST_DATA(afw_boolean_t)
afw_data_type_boolean_true;

/** @brief afw_boolean_t false value. */
AFW_DECLARE_CONST_DATA(afw_boolean_t)
afw_data_type_boolean_false;

#define AFW_DATA_TYPE_EQUAL(A_DATATYPE_1_, A_DATATYPE_2_) \
((A_DATATYPE_1_) == (A_DATATYPE_2_))

#define AFW_DATA_TYPE_ASSERT_EQUAL(A_DATATYPE_1_, A_DATATYPE_2_, A_SCOPE) \
if (!AFW_DATA_TYPE_EQUAL(A_DATATYPE_1_,A_DATATYPE_2_)) { \
    AFW_THROW_ERROR_Z(cast_error, "Type safe exception.", A_SCOPE); \
}


/**
 * @brief Clone an object to a managed object.
 * @param object to clone.
 * @param p used for cloned object.
 * @param xctx of caller.
 * @return cloned object.
 *
 * This will use afw_object_create_managed() to create the
 * cloned object.
 */
AFW_DECLARE(const afw_object_t *)
afw_data_type_object_create_clone_to_managed_object(
    const afw_object_t *object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Directly call value_compiler_listing() for data type 'object'.
 * @param writer instance.
 * @param object_value of object.
 * @param scalar_only true if only scalar properties should be listed.
 * @param xctx of caller.
 *
 * This will use afw_object_create_managed() to create the
 * cloned object.
 */
AFW_DECLARE(void)
afw_data_type_object_value_compiler_listing(
    const afw_writer_t *writer,
    const afw_value_t *object_value,
    afw_boolean_t scalar_only,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_DATA_TYPE_H__ */

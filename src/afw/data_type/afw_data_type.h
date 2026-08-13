// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Data Type Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_DATA_TYPE_H__
#define __AFW_DATA_TYPE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_data_type
 * @{
 */

/**
 * @file afw_data_type.h
 * @brief Hand helpers around adaptive data types.
 *
 * See @ref afw_data_type and @ref afw_c_api_data_types. Prefer generated
 * per-type bindings (`afw_data_type_*_binding.h`) and create APIs from
 * extensions. Also see `afw_core_data_types.h`.
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
    AFW_THROW_ERROR_Z(conversion_error, "Type safe exception.", A_SCOPE); \
}


/**
 * @brief True if this data type supports keyless afw_iterator (#153).
 * @param data_type const afw_data_type_t *.
 *
 * Runtime capability is optional_initialize_iterator non-NULL on the inf
 * (wired in afw_data_type.c). Not derived from generate docs or from
 * iterator_return_data_type (array iterates but has no fixed step type).
 */
#define afw_data_type_has_iterator(data_type) \
    ((data_type) && (data_type)->inf && \
     (data_type)->inf->optional_initialize_iterator)


/**
 * @brief Fixed data type of each keyless iterator step, or NULL.
 * @param data_type const afw_data_type_t *.
 * @return const afw_data_type_t * or NULL.
 *
 * Reads data_type->iterator_return_data_type. NULL if data_type is NULL,
 * the type has no keyless iterator, or step type is not fixed at type level
 * (e.g. array). See issue #153.
 */
#define afw_data_type_iterator_return_data_type(data_type) \
    ((data_type) ? (data_type)->iterator_return_data_type : NULL)


/**
 * @brief Initialize a keyless afw_iterator for a value of this data type.
 * @param data_type const afw_data_type_t *.
 * @param internal pointer to the value's cType internal.
 * @param iterator caller-defined afw_iterator_t storage (opaque; host fills).
 * @param xctx of caller.
 *
 * Calls optional_initialize_iterator if present; otherwise throws.
 * Soft probe: use afw_data_type_has_iterator() first, or test the optional
 * method pointer.
 */
#define afw_data_type_initialize_iterator(data_type, internal, iterator, xctx) \
do { \
    if (!(data_type) || !(data_type)->inf || \
        !(data_type)->inf->optional_initialize_iterator) \
    { \
        AFW_THROW_ERROR_Z(general, \
            "Data type does not support iterator", (xctx)); \
    } \
    (data_type)->inf->optional_initialize_iterator( \
        (data_type), (internal), (iterator), (xctx)); \
} while (0)


/**
 * @brief Clone an object to a managed object.
 * @param object to clone.
 * @param p used for cloned object.
 * @param xctx of caller.
 * @return cloned object.
 *
 * This will use afw_object_create() to create the
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
 * This will use afw_object_create() to create the
 * cloned object.
 */
AFW_DECLARE(void)
afw_data_type_object_value_compiler_listing(
    const afw_writer_t *writer,
    const afw_value_t *object_value,
    afw_boolean_t scalar_only,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_DATA_TYPE_H__ */

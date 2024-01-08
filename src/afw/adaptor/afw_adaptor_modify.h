// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Modify Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_MODIFY_H__
#define __AFW_ADAPTOR_MODIFY_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adaptor
 * @{
 */

/**
 * @file afw_adaptor_modify.h
 * @brief Adaptive Framework adaptor modify object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Adaptor session modify type map.
 *
 * XX(name, true/false if tuple has value)
 */
#define AFW_ADAPTOR_MODIFY_ENTRY_TYPE_MAP(XX)                                  \
    XX(add_value,           true)                                              \
    XX(remove_property,     false)                                             \
    XX(remove_value,        true)                                              \
    XX(set_property,        true)                                              \


/**  @brief Adaptor session modify type enum. */
typedef enum afw_adaptor_modify_entry_type_e {
#define XX(id, _) afw_adaptor_modify_entry_type_ ## id,
    AFW_ADAPTOR_MODIFY_ENTRY_TYPE_MAP(XX)
#undef XX
    afw_adaptor_modify_entry_type_invalid
} afw_adaptor_modify_entry_type_t;



/**  @brief Adaptor modify entry. */
struct afw_adaptor_modify_entry_s {

    /** @brief Modify type. */
    afw_adaptor_modify_entry_type_t type;

    /** @brief First entry in property name list. */
    const afw_object_path_property_name_entry_t *first_property_name_entry;

    /** @brief Associated value. */
    const afw_value_t *value;
};



/**
 * @brief Modify object and remove from cache.
 * @param adaptor_id of adaptor used to patch object.
 * @param object_type_id of object to modify.
 * @param object_id of object to modify.
 * @param entries for parameters to modify.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adaptor_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return
 *
 * Patch an existing adaptive object via adaptor.
 */
AFW_DECLARE(void)
afw_adaptor_modify_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_array_t *entries,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx);


/**
 * @brief Modify using update object and remove from cache.
 * @param adaptor_id Id of adaptor used to update object.
 * @param object_type_id of object to update.
 * @param object_id of object to update.
 * @param update_object containing properties to replace.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adaptor_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return
 *
 * Update an existing adaptive object via adaptor.  Existing properties will be
 * removed if they are not present if the updated object.
 *
 * The values of properties in the update object will completely replace the
 * values of properties in the existing object. The
 * afw_object_meta_get_object_id() and afw_object_type_get_object_type()
 * methods of the update object are used to identify the existing object to
 * update.
 *
 * This function calls an adaptor modify method after converting the update object
 * to a modify request.
 */
AFW_DECLARE(void)
afw_adaptor_modify_using_update_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *update_object,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx);


/**
 * @brief Convert entry type mnemonic to enum.
 * @param mnemonic of entry type
 * @return enum of entry type
 */
AFW_DECLARE(afw_adaptor_modify_entry_type_t)
afw_adaptor_modify_entry_type(const afw_utf8_t *mnemonic);


/**
 * @brief Convert entry type enum to mnemonic.
 * @param type enum to convert.
 * @return mnemonic or NULL if invalid.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_adaptor_modify_entry_type_mnemonic(afw_adaptor_modify_entry_type_t type);


/**
 * @brief Convert entry type enum to value.
 * @param type enum to convert.
 * @return (const afw_value_t *) or NULL if invalid.
 */
AFW_DECLARE(const afw_value_t *)
afw_adaptor_modify_entry_type_value(afw_adaptor_modify_entry_type_t type);


/**
 * @brief Create modify entries from list of tuple lists in specified pool.
 * @param list of tuple lists.
 * @param p Pool for result.
 * @param xctx of caller.
 * @return pointer to list of modify entry pointers.
 *
 * The following tuples define property and property value operations
 * that apply to an Object. The "propertyname" can also be a list of names
 * when referring to an embedded object.  To reference a.b['c d'] as expressed
 * in adaptive expression syntax, use ["a", "b", "c d"] in a modify entry.
 *
 *
 * The "add_value" tuple will add "value" to "property3".  If the DataType
 * is list, then the new "value" will be appended.  If DataType is not
 * list, it will fail if "property3" already has a value; otherwise, it's
 * equivalent to "set_property":
 *
 * [
 *     "add_value",
 *     "propertyname", or ["propertyname", "propertyname", ...],
 *     "value"
 * ],
 *
 *
 * The "remove_property" tuple will remove "property2" and all of it's
 * value(s):
 *
 * [
 *     "remove_property",
 *     "propertyname" or ["propertyname", "propertyname", ...]
 * ],
 *
 *
 * The "remove_value" tuple will remove "value" from "property4".  If
 * DataType is list, then "value" will be removed from the list.  If
 * the DataType is not list, then it's equivalent to "remove_property".
 * In either case, an exception is thrown if "value" does not exist in
 * "property4":
 *
 * [
 *     "remove_value",
 *     "propertyname", or ["propertyname", "propertyname", ...],
 *     "value"
 * ],
 *
 *
 * The "set_property" tuple will set "property1" to "value", regardless of
 * what is in the store.
 *
 * [
 *     "set_property",
 *     "propertyname", or ["propertyname", "propertyname", ...],
 *     "value"
 * ],
 *
 */
AFW_DECLARE(const afw_adaptor_modify_entry_t * const *)
afw_adaptor_modify_entries_from_list(
    const afw_array_t *list, const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Create a list from modify entries in specified pool.
 * @param entries
 * @param p for result.
 * @param xctx of caller.
 * @return list.
 */
AFW_DECLARE(const afw_array_t *)
afw_adaptor_modify_entries_to_list(
    const afw_adaptor_modify_entry_t * const *entries,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Apply modify entries to an unnormalized object.
 * @param entries pointer to NULL terminated list of entry pointers.
 * @param object to modify.
 * @param xctx of caller.
 *
 * This function applies changes, in place, to an unnormalized object.
 * The resulting object will also be unnormalized.
 *
 * The original intent of this function is for use by adaptor implementations
 * that store objects without their data type. When objects are retrieved from
 * this type of adaptor, they are usually normalized which will convert values
 * to their correct data type.
 *
 * An add_value for a property that has a single value will result in a
 * multi-value property.  This may be a consideration when the value is
 * later normalized.
 */
AFW_DECLARE(void)
afw_adaptor_modify_entries_apply_to_unnormalized_object(
    const afw_adaptor_modify_entry_t * const *entries,
    const afw_object_t *object,
    afw_xctx_t *xctx);


/**
 * @brief Produce modify needed to reconcile two objects.
 * @param adaptor_id return pointer.
 * @param object_type_id return pointer.
 * @param object_id return pointer.
 * @param entries return pointer.
 * @param original object.
 * @param modified object.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param p for result.
 * @param xctx of caller.
 *
 * Parameters adaptor_id, object_type_id, object_id, and entries are pointers
 * to variables that are suitable to pass to the corresponding parameters of
 * afw_adaptor_modify_object().
 */
AFW_DECLARE(void)
afw_adaptor_modify_needed_to_reconcile(
    const afw_utf8_t * *adaptor_id,
    const afw_utf8_t * *object_type_id,
    const afw_utf8_t * *object_id,
    const afw_array_t * *entries,
    const afw_object_t *original,
    const afw_object_t *modified,
    const afw_object_t *journal_entry,
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_MODIFY_H__ */

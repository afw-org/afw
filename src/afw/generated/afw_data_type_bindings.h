// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Data Type Bindings
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_DATA_TYPE_BINDINGS_H__
#define __AFW_DATA_TYPE_BINDINGS_H__


/**
 * @file afw_data_type_bindings.h
 * @brief Adaptive Framework core data type bindings header.
 */

#include "afw_data_type_any_binding.h"
#include "afw_data_type_anyURI_binding.h"
#include "afw_data_type_array_binding.h"
#include "afw_data_type_base64Binary_binding.h"
#include "afw_data_type_boolean_binding.h"
#include "afw_data_type_date_binding.h"
#include "afw_data_type_dateTime_binding.h"
#include "afw_data_type_dayTimeDuration_binding.h"
#include "afw_data_type_dnsName_binding.h"
#include "afw_data_type_double_binding.h"
#include "afw_data_type_function_binding.h"
#include "afw_data_type_hexBinary_binding.h"
#include "afw_data_type_ia5String_binding.h"
#include "afw_data_type_integer_binding.h"
#include "afw_data_type_ipAddress_binding.h"
#include "afw_data_type_json_binding.h"
#include "afw_data_type_null_binding.h"
#include "afw_data_type_object_binding.h"
#include "afw_data_type_objectId_binding.h"
#include "afw_data_type_objectPath_binding.h"
#include "afw_data_type_password_binding.h"
#include "afw_data_type_regexp_binding.h"
#include "afw_data_type_relaxed_json_binding.h"
#include "afw_data_type_rfc822Name_binding.h"
#include "afw_data_type_script_binding.h"
#include "afw_data_type_string_binding.h"
#include "afw_data_type_template_binding.h"
#include "afw_data_type_time_binding.h"
#include "afw_data_type_undefined_binding.h"
#include "afw_data_type_unevaluated_binding.h"
#include "afw_data_type_unknown_binding.h"
#include "afw_data_type_void_binding.h"
#include "afw_data_type_x500Name_binding.h"
#include "afw_data_type_xpathExpression_binding.h"
#include "afw_data_type_yearMonthDuration_binding.h"

/**
 * @brief Data type map.
 */
#define AFW_DATA_TYPES_MAP(XX) \
    XX(any) \
    XX(anyURI) \
    XX(array) \
    XX(base64Binary) \
    XX(boolean) \
    XX(date) \
    XX(dateTime) \
    XX(dayTimeDuration) \
    XX(dnsName) \
    XX(double) \
    XX(function) \
    XX(hexBinary) \
    XX(ia5String) \
    XX(integer) \
    XX(ipAddress) \
    XX(json) \
    XX(null) \
    XX(object) \
    XX(objectId) \
    XX(objectPath) \
    XX(password) \
    XX(regexp) \
    XX(relaxed_json) \
    XX(rfc822Name) \
    XX(script) \
    XX(string) \
    XX(template) \
    XX(time) \
    XX(undefined) \
    XX(unevaluated) \
    XX(unknown) \
    XX(void) \
    XX(x500Name) \
    XX(xpathExpression) \
    XX(yearMonthDuration) \


/**
 * @brief Core data types map.
 */
typedef enum afw_data_type_number_e {
    afw_data_type_number_special,
#define XX(id) afw_data_type_number_ ## id,
    AFW_DATA_TYPES_MAP(XX)
    #undef XX
    afw_data_type_number_max
} afw_data_type_number_t;

/**
 * @brief Register each afw_data_type_<dataType>.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_register_data_types(
    afw_xctx_t *xctx);

/**
 * @brief Register each afw__value_unmanaged_<dataType>_inf.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_register_data_type_infs(
    afw_xctx_t *xctx);


#endif /* __AFW_DATA_TYPE_BINDINGS_H__ */

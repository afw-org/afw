// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Data Type Typedefs yearMonthDuration
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


#ifndef __AFW_DATA_TYPE_TYPEDEFS_H__
#define __AFW_DATA_TYPE_TYPEDEFS_H__


/**
 * @file afw_data_type_typedefs.h
 * @brief Adaptive data type typedefs yearMonthDuration header.
 */

#include "afw_minimal.h"
#include "afw_declare_helpers.h"

AFW_BEGIN_DECLARES

/**
 * @addtogroup afw_c_api_data_type_any
 * @{
 */

/** @brief Typedef for data type any internal. */
typedef const afw_value_t * afw_data_type_any_internal_t;

/** @brief Typedef for data type any parameter. */
typedef const afw_value_t * afw_data_type_any_parameter_t;

/** @brief Typedef for data type any return. */
typedef const afw_value_t * afw_data_type_any_return_t;

/** @brief Typedef for data type any evaluated value. */
typedef struct afw_value_any_s
    afw_value_any_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_anyURI
 * @{
 */

/** @brief Typedef for data type anyURI internal. */
typedef afw_utf8_t afw_data_type_anyURI_internal_t;

/** @brief Typedef for data type anyURI parameter. */
typedef const afw_utf8_t * afw_data_type_anyURI_parameter_t;

/** @brief Typedef for data type anyURI return. */
typedef const afw_utf8_t * afw_data_type_anyURI_return_t;

/** @brief Typedef for data type anyURI evaluated value. */
typedef struct afw_value_anyURI_s
    afw_value_anyURI_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_array
 * @{
 */

/** @brief Typedef for data type array internal. */
typedef const afw_array_t * afw_data_type_array_internal_t;

/** @brief Typedef for data type array parameter. */
typedef const afw_array_t * afw_data_type_array_parameter_t;

/** @brief Typedef for data type array return. */
typedef const afw_array_t * afw_data_type_array_return_t;

/** @brief Typedef for data type array evaluated value. */
typedef struct afw_value_array_s
    afw_value_array_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_base64Binary
 * @{
 */

/** @brief Typedef for data type base64Binary internal. */
typedef afw_memory_t afw_data_type_base64Binary_internal_t;

/** @brief Typedef for data type base64Binary parameter. */
typedef const afw_memory_t * afw_data_type_base64Binary_parameter_t;

/** @brief Typedef for data type base64Binary return. */
typedef const afw_memory_t * afw_data_type_base64Binary_return_t;

/** @brief Typedef for data type base64Binary evaluated value. */
typedef struct afw_value_base64Binary_s
    afw_value_base64Binary_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_boolean
 * @{
 */

/** @brief Typedef for data type boolean internal. */
typedef afw_boolean_t afw_data_type_boolean_internal_t;

/** @brief Typedef for data type boolean parameter. */
typedef const afw_boolean_t * afw_data_type_boolean_parameter_t;

/** @brief Typedef for data type boolean return. */
typedef afw_boolean_t afw_data_type_boolean_return_t;

/** @brief Typedef for data type boolean evaluated value. */
typedef struct afw_value_boolean_s
    afw_value_boolean_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_date
 * @{
 */

/** @brief Typedef for data type date internal. */
typedef afw_date_t afw_data_type_date_internal_t;

/** @brief Typedef for data type date parameter. */
typedef const afw_date_t * afw_data_type_date_parameter_t;

/** @brief Typedef for data type date return. */
typedef const afw_date_t * afw_data_type_date_return_t;

/** @brief Typedef for data type date evaluated value. */
typedef struct afw_value_date_s
    afw_value_date_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_dateTime
 * @{
 */

/** @brief Typedef for data type dateTime internal. */
typedef afw_dateTime_t afw_data_type_dateTime_internal_t;

/** @brief Typedef for data type dateTime parameter. */
typedef const afw_dateTime_t * afw_data_type_dateTime_parameter_t;

/** @brief Typedef for data type dateTime return. */
typedef const afw_dateTime_t * afw_data_type_dateTime_return_t;

/** @brief Typedef for data type dateTime evaluated value. */
typedef struct afw_value_dateTime_s
    afw_value_dateTime_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_dayTimeDuration
 * @{
 */

/** @brief Typedef for data type dayTimeDuration internal. */
typedef afw_dayTimeDuration_t afw_data_type_dayTimeDuration_internal_t;

/** @brief Typedef for data type dayTimeDuration parameter. */
typedef const afw_dayTimeDuration_t * afw_data_type_dayTimeDuration_parameter_t;

/** @brief Typedef for data type dayTimeDuration return. */
typedef const afw_dayTimeDuration_t * afw_data_type_dayTimeDuration_return_t;

/** @brief Typedef for data type dayTimeDuration evaluated value. */
typedef struct afw_value_dayTimeDuration_s
    afw_value_dayTimeDuration_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_dnsName
 * @{
 */

/** @brief Typedef for data type dnsName internal. */
typedef afw_utf8_t afw_data_type_dnsName_internal_t;

/** @brief Typedef for data type dnsName parameter. */
typedef const afw_utf8_t * afw_data_type_dnsName_parameter_t;

/** @brief Typedef for data type dnsName return. */
typedef const afw_utf8_t * afw_data_type_dnsName_return_t;

/** @brief Typedef for data type dnsName evaluated value. */
typedef struct afw_value_dnsName_s
    afw_value_dnsName_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_double
 * @{
 */

/** @brief Typedef for data type double internal. */
typedef double afw_data_type_double_internal_t;

/** @brief Typedef for data type double parameter. */
typedef const double * afw_data_type_double_parameter_t;

/** @brief Typedef for data type double return. */
typedef double afw_data_type_double_return_t;

/** @brief Typedef for data type double evaluated value. */
typedef struct afw_value_double_s
    afw_value_double_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_expression
 * @{
 */

/** @brief Typedef for data type expression internal. */
typedef afw_utf8_t afw_data_type_expression_internal_t;

/** @brief Typedef for data type expression parameter. */
typedef const afw_utf8_t * afw_data_type_expression_parameter_t;

/** @brief Typedef for data type expression return. */
typedef const afw_utf8_t * afw_data_type_expression_return_t;

/** @brief Typedef for data type expression evaluated value. */
typedef struct afw_value_expression_s
    afw_value_expression_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_function
 * @{
 */

/** @brief Typedef for data type function internal. */
typedef const afw_value_t * afw_data_type_function_internal_t;

/** @brief Typedef for data type function parameter. */
typedef const afw_value_t * afw_data_type_function_parameter_t;

/** @brief Typedef for data type function return. */
typedef const afw_value_t * afw_data_type_function_return_t;

/** @brief Typedef for data type function evaluated value. */
typedef struct afw_value_function_s
    afw_value_function_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_hexBinary
 * @{
 */

/** @brief Typedef for data type hexBinary internal. */
typedef afw_memory_t afw_data_type_hexBinary_internal_t;

/** @brief Typedef for data type hexBinary parameter. */
typedef const afw_memory_t * afw_data_type_hexBinary_parameter_t;

/** @brief Typedef for data type hexBinary return. */
typedef const afw_memory_t * afw_data_type_hexBinary_return_t;

/** @brief Typedef for data type hexBinary evaluated value. */
typedef struct afw_value_hexBinary_s
    afw_value_hexBinary_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_hybrid
 * @{
 */

/** @brief Typedef for data type hybrid internal. */
typedef afw_utf8_t afw_data_type_hybrid_internal_t;

/** @brief Typedef for data type hybrid parameter. */
typedef const afw_utf8_t * afw_data_type_hybrid_parameter_t;

/** @brief Typedef for data type hybrid return. */
typedef const afw_utf8_t * afw_data_type_hybrid_return_t;

/** @brief Typedef for data type hybrid evaluated value. */
typedef struct afw_value_hybrid_s
    afw_value_hybrid_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_ia5String
 * @{
 */

/** @brief Typedef for data type ia5String internal. */
typedef afw_utf8_t afw_data_type_ia5String_internal_t;

/** @brief Typedef for data type ia5String parameter. */
typedef const afw_utf8_t * afw_data_type_ia5String_parameter_t;

/** @brief Typedef for data type ia5String return. */
typedef const afw_utf8_t * afw_data_type_ia5String_return_t;

/** @brief Typedef for data type ia5String evaluated value. */
typedef struct afw_value_ia5String_s
    afw_value_ia5String_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_integer
 * @{
 */

/** @brief Typedef for data type integer internal. */
typedef afw_integer_t afw_data_type_integer_internal_t;

/** @brief Typedef for data type integer parameter. */
typedef const afw_integer_t * afw_data_type_integer_parameter_t;

/** @brief Typedef for data type integer return. */
typedef afw_integer_t afw_data_type_integer_return_t;

/** @brief Typedef for data type integer evaluated value. */
typedef struct afw_value_integer_s
    afw_value_integer_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_ipAddress
 * @{
 */

/** @brief Typedef for data type ipAddress internal. */
typedef afw_utf8_t afw_data_type_ipAddress_internal_t;

/** @brief Typedef for data type ipAddress parameter. */
typedef const afw_utf8_t * afw_data_type_ipAddress_parameter_t;

/** @brief Typedef for data type ipAddress return. */
typedef const afw_utf8_t * afw_data_type_ipAddress_return_t;

/** @brief Typedef for data type ipAddress evaluated value. */
typedef struct afw_value_ipAddress_s
    afw_value_ipAddress_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_null
 * @{
 */

/** @brief Typedef for data type null internal. */
typedef void * afw_data_type_null_internal_t;

/** @brief Typedef for data type null parameter. */
typedef const void * afw_data_type_null_parameter_t;

/** @brief Typedef for data type null return. */
typedef void * afw_data_type_null_return_t;

/** @brief Typedef for data type null evaluated value. */
typedef struct afw_value_null_s
    afw_value_null_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_object
 * @{
 */

/** @brief Typedef for data type object internal. */
typedef const afw_object_t * afw_data_type_object_internal_t;

/** @brief Typedef for data type object parameter. */
typedef const afw_object_t * afw_data_type_object_parameter_t;

/** @brief Typedef for data type object return. */
typedef const afw_object_t * afw_data_type_object_return_t;

/** @brief Typedef for data type object evaluated value. */
typedef struct afw_value_object_s
    afw_value_object_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_objectId
 * @{
 */

/** @brief Typedef for data type objectId internal. */
typedef afw_utf8_t afw_data_type_objectId_internal_t;

/** @brief Typedef for data type objectId parameter. */
typedef const afw_utf8_t * afw_data_type_objectId_parameter_t;

/** @brief Typedef for data type objectId return. */
typedef const afw_utf8_t * afw_data_type_objectId_return_t;

/** @brief Typedef for data type objectId evaluated value. */
typedef struct afw_value_objectId_s
    afw_value_objectId_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_objectPath
 * @{
 */

/** @brief Typedef for data type objectPath internal. */
typedef afw_utf8_t afw_data_type_objectPath_internal_t;

/** @brief Typedef for data type objectPath parameter. */
typedef const afw_utf8_t * afw_data_type_objectPath_parameter_t;

/** @brief Typedef for data type objectPath return. */
typedef const afw_utf8_t * afw_data_type_objectPath_return_t;

/** @brief Typedef for data type objectPath evaluated value. */
typedef struct afw_value_objectPath_s
    afw_value_objectPath_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_password
 * @{
 */

/** @brief Typedef for data type password internal. */
typedef afw_utf8_t afw_data_type_password_internal_t;

/** @brief Typedef for data type password parameter. */
typedef const afw_utf8_t * afw_data_type_password_parameter_t;

/** @brief Typedef for data type password return. */
typedef const afw_utf8_t * afw_data_type_password_return_t;

/** @brief Typedef for data type password evaluated value. */
typedef struct afw_value_password_s
    afw_value_password_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_regexp
 * @{
 */

/** @brief Typedef for data type regexp internal. */
typedef afw_utf8_t afw_data_type_regexp_internal_t;

/** @brief Typedef for data type regexp parameter. */
typedef const afw_utf8_t * afw_data_type_regexp_parameter_t;

/** @brief Typedef for data type regexp return. */
typedef const afw_utf8_t * afw_data_type_regexp_return_t;

/** @brief Typedef for data type regexp evaluated value. */
typedef struct afw_value_regexp_s
    afw_value_regexp_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_rfc822Name
 * @{
 */

/** @brief Typedef for data type rfc822Name internal. */
typedef afw_utf8_t afw_data_type_rfc822Name_internal_t;

/** @brief Typedef for data type rfc822Name parameter. */
typedef const afw_utf8_t * afw_data_type_rfc822Name_parameter_t;

/** @brief Typedef for data type rfc822Name return. */
typedef const afw_utf8_t * afw_data_type_rfc822Name_return_t;

/** @brief Typedef for data type rfc822Name evaluated value. */
typedef struct afw_value_rfc822Name_s
    afw_value_rfc822Name_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_script
 * @{
 */

/** @brief Typedef for data type script internal. */
typedef afw_utf8_t afw_data_type_script_internal_t;

/** @brief Typedef for data type script parameter. */
typedef const afw_utf8_t * afw_data_type_script_parameter_t;

/** @brief Typedef for data type script return. */
typedef const afw_utf8_t * afw_data_type_script_return_t;

/** @brief Typedef for data type script evaluated value. */
typedef struct afw_value_script_s
    afw_value_script_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_string
 * @{
 */

/** @brief Typedef for data type string internal. */
typedef afw_utf8_t afw_data_type_string_internal_t;

/** @brief Typedef for data type string parameter. */
typedef const afw_utf8_t * afw_data_type_string_parameter_t;

/** @brief Typedef for data type string return. */
typedef const afw_utf8_t * afw_data_type_string_return_t;

/** @brief Typedef for data type string evaluated value. */
typedef struct afw_value_string_s
    afw_value_string_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_template
 * @{
 */

/** @brief Typedef for data type template internal. */
typedef afw_utf8_t afw_data_type_template_internal_t;

/** @brief Typedef for data type template parameter. */
typedef const afw_utf8_t * afw_data_type_template_parameter_t;

/** @brief Typedef for data type template return. */
typedef const afw_utf8_t * afw_data_type_template_return_t;

/** @brief Typedef for data type template evaluated value. */
typedef struct afw_value_template_s
    afw_value_template_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_time
 * @{
 */

/** @brief Typedef for data type time internal. */
typedef afw_time_t afw_data_type_time_internal_t;

/** @brief Typedef for data type time parameter. */
typedef const afw_time_t * afw_data_type_time_parameter_t;

/** @brief Typedef for data type time return. */
typedef const afw_time_t * afw_data_type_time_return_t;

/** @brief Typedef for data type time evaluated value. */
typedef struct afw_value_time_s
    afw_value_time_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_unevaluated
 * @{
 */

/** @brief Typedef for data type unevaluated internal. */
typedef const afw_value_t * afw_data_type_unevaluated_internal_t;

/** @brief Typedef for data type unevaluated parameter. */
typedef const afw_value_t * afw_data_type_unevaluated_parameter_t;

/** @brief Typedef for data type unevaluated return. */
typedef const afw_value_t * afw_data_type_unevaluated_return_t;

/** @brief Typedef for data type unevaluated evaluated value. */
typedef struct afw_value_unevaluated_s
    afw_value_unevaluated_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_unknown
 * @{
 */

/** @brief Typedef for data type unknown internal. */
typedef const afw_value_t * afw_data_type_unknown_internal_t;

/** @brief Typedef for data type unknown parameter. */
typedef const afw_value_t * afw_data_type_unknown_parameter_t;

/** @brief Typedef for data type unknown return. */
typedef const afw_value_t * afw_data_type_unknown_return_t;

/** @brief Typedef for data type unknown evaluated value. */
typedef struct afw_value_unknown_s
    afw_value_unknown_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_void
 * @{
 */

/** @brief Typedef for data type void internal. */
typedef void * afw_data_type_void_internal_t;

/** @brief Typedef for data type void parameter. */
typedef const void * afw_data_type_void_parameter_t;

/** @brief Typedef for data type void return. */
typedef void * afw_data_type_void_return_t;

/** @brief Typedef for data type void evaluated value. */
typedef struct afw_value_void_s
    afw_value_void_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_x500Name
 * @{
 */

/** @brief Typedef for data type x500Name internal. */
typedef afw_utf8_t afw_data_type_x500Name_internal_t;

/** @brief Typedef for data type x500Name parameter. */
typedef const afw_utf8_t * afw_data_type_x500Name_parameter_t;

/** @brief Typedef for data type x500Name return. */
typedef const afw_utf8_t * afw_data_type_x500Name_return_t;

/** @brief Typedef for data type x500Name evaluated value. */
typedef struct afw_value_x500Name_s
    afw_value_x500Name_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_xpathExpression
 * @{
 */

/** @brief Typedef for data type xpathExpression internal. */
typedef afw_utf8_t afw_data_type_xpathExpression_internal_t;

/** @brief Typedef for data type xpathExpression parameter. */
typedef const afw_utf8_t * afw_data_type_xpathExpression_parameter_t;

/** @brief Typedef for data type xpathExpression return. */
typedef const afw_utf8_t * afw_data_type_xpathExpression_return_t;

/** @brief Typedef for data type xpathExpression evaluated value. */
typedef struct afw_value_xpathExpression_s
    afw_value_xpathExpression_t;

/** @} */

/**
 * @addtogroup afw_c_api_data_type_yearMonthDuration
 * @{
 */

/** @brief Typedef for data type yearMonthDuration internal. */
typedef afw_yearMonthDuration_t afw_data_type_yearMonthDuration_internal_t;

/** @brief Typedef for data type yearMonthDuration parameter. */
typedef const afw_yearMonthDuration_t * afw_data_type_yearMonthDuration_parameter_t;

/** @brief Typedef for data type yearMonthDuration return. */
typedef const afw_yearMonthDuration_t * afw_data_type_yearMonthDuration_return_t;

/** @brief Typedef for data type yearMonthDuration evaluated value. */
typedef struct afw_value_yearMonthDuration_s
    afw_value_yearMonthDuration_t;

/** @} */

AFW_END_DECLARES


#endif /* __AFW_DATA_TYPE_YEARMONTHDURATION_BINDING_H__ */

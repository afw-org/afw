// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework core data type functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_data_type.c
 * @brief Adaptive Framework data type functions.
 */

#include "afw_internal.h"
#include "apr_base64.h"



static void
impl_clone_and_set_property(
    const afw_object_t *obj,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);


/* object clone. */
static void
impl_afw_data_type_object_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/* afw_boolean_t true value. */
AFW_DEFINE_CONST_DATA(afw_boolean_t)
afw_data_type_boolean_true = AFW_TRUE;

/* afw_boolean_t false value. */
AFW_DEFINE_CONST_DATA(afw_boolean_t)
afw_data_type_boolean_false = AFW_FALSE;


/* ---- Common utf8 to internal ----------------------------------------------*/

static void
impl_afw_data_type_utf8_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* ---- Common internal to utf8 ----------------------------------------------*/

/* Can use for any afw_utf8_t internal. */
static const afw_utf8_t *
impl_afw_data_type_utf8_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return (const afw_utf8_t *)from_internal;
}


/* ---- Common compare -------------------------------------------------------*/

/* Can use for any afw_memory_t internal. */
static int
impl_afw_data_type_raw_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_memory_t *v1 = value1;
    const afw_memory_t *v2 = value2;
    int result;

    if (v1->size == v2->size) {
        result = memcmp(v1->ptr, v2->ptr, v1->size);
    }

    else if (v1->size < v2->size) {
        result = memcmp(v1->ptr, v2->ptr, v1->size);
        if (result == 0) result = -1;
    }

    else {
        result = memcmp(v1->ptr, v2->ptr, v2->size);
        if (result == 0) result = 1;
    }

    return result;
}


/* Can use for any afw_utf8_t internal. */
static int
impl_afw_data_type_utf8_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *v1 = value1;
    const afw_utf8_t *v2 = value2;
    int result;

    if (v1->len == v2->len) {
        result = memcmp(v1->s, v2->s, v1->len);
    }

    else if (v1->len < v2->len) {
        result = memcmp(v1->s, v2->s, v1->len);
        if (result == 0) result = -1;
    }

    else {
        result = memcmp(v1->s, v2->s, v2->len);
        if (result == 0) result = 1;
    }

    return result;
}


/* ---- Convert  ------------------------------------------------------*/

/* Can use for any data type tha does not have a convert function. */
static void
impl_afw_data_type_standard_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *from_utf8;

    /* Try converting to utf8 and then to to_data_type. */
    from_utf8 = afw_data_type_internal_to_utf8(instance, from_internal,
        p, xctx);
    afw_data_type_utf8_to_internal(to_data_type, to_internal, from_utf8,
        p, xctx);
}


/* Can use for any data type with cType afw_utf8_t. */
static void
impl_afw_data_type_from_utf8_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_boolean(to_data_type)) {
        *((afw_boolean_t *)to_internal) =
            (!from_internal ||
            (((const afw_utf8_t *)from_internal)->len == 0))
            ? false
            : true;
    }
    else {
        afw_data_type_utf8_to_internal(to_data_type,
            to_internal, (const afw_utf8_t *)from_internal,
            p, xctx);
    }
}


/* Can use for any data type with cType afw_memory_t. */
static void
impl_afw_data_type_from_raw_convert_internal(
    const afw_data_type_t *instance,
    void *to_internal,
    const void *from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_boolean(to_data_type)) {
        *((afw_boolean_t *)to_internal) =
            (((const afw_memory_t *)from_internal)->size == 0) ? false : true;
    }
    else if (afw_data_type_is_base64Binary(to_data_type) ||
        afw_data_type_is_hexBinary(to_data_type))
    {
        memcpy(to_internal, (afw_octet_t *)from_internal, sizeof(afw_memory_t));
    }
    else {
        impl_afw_data_type_standard_convert_internal(
            instance, to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* Can use for any data type with cType afw_memory_t. */
static void
impl_afw_data_type_from_pointer_convert_internal(
    const afw_data_type_t *instance,
    void *to_internal,
    const void *from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_boolean(to_data_type)) {
        *((afw_boolean_t *)to_internal) =
            (*(char *)from_internal) ? false : true;
    }
    else {
        impl_afw_data_type_standard_convert_internal(
            instance, to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* Used for any data type dateTime.  Will convert to date and time.  */
static void
impl_afw_data_type_from_dateTime_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_dateTime(to_data_type)) {
        memcpy(to_internal, from_internal, sizeof(afw_dateTime_t));
    }
    else if (afw_data_type_is_date(to_data_type)) {
        memcpy(&((afw_date_t *)to_internal)->date,
            &((const afw_dateTime_t *)from_internal)->date,
            sizeof(afw_date_no_time_zone_t));
        memcpy(&((afw_date_t *)to_internal)->time_zone,
            &((const afw_dateTime_t *)from_internal)->time_zone,
            sizeof(afw_time_zone_t));
    }
    else if (afw_data_type_is_time(to_data_type)) {
        memcpy(&((afw_time_t *)to_internal)->time,
            &((const afw_dateTime_t *)from_internal)->time,
            sizeof(afw_time_no_time_zone_t));
        memcpy(&((afw_time_t *)to_internal)->time_zone,
            &((const afw_dateTime_t *)from_internal)->time_zone,
            sizeof(afw_time_zone_t));
    }
    else {
        impl_afw_data_type_standard_convert_internal(instance,
            to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* Used for data type double.  Will convert double to integer.   */
static void
impl_afw_data_type_from_double_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_integer_t i;

    if (afw_data_type_is_double(to_data_type)) {
        memcpy(to_internal, from_internal, sizeof(afw_double_t));
    }
    else if (afw_data_type_is_integer(to_data_type)) {
        /*
        AFW_THROW_ERROR_FZ(general, xctx,
            "Cannot convert data type " AFW_UTF8_FMT_Q
            " to " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(&instance->data_type_id),
            AFW_UTF8_FMT_ARG(&to_data_type->data_type_id));
         */
        i = (afw_integer_t)(*(const afw_double_t *)from_internal);
        //if (i != *(const afw_double_t *)from_internal) {
        //    AFW_THROW_ERROR_Z(general,
        //        "This double value can not be converted to integer",
        //        xctx);
        //}
        *((afw_integer_t *)to_internal) = i;
    }
    else if (afw_data_type_is_boolean(to_data_type)) {
        *(afw_boolean_t *)to_internal =
            afw_number_is_NaN(*(const afw_double_t *)from_internal)
            ? false
            : *(const afw_double_t *)from_internal != 0.0;
    }
    else {
        impl_afw_data_type_standard_convert_internal(instance,
            to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* Used for data type integer.  Will convert integer to double.   */
static void
impl_afw_data_type_from_integer_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_integer(to_data_type)) {
        memcpy(to_internal, from_internal, sizeof(afw_integer_t));
    }
    else if (afw_data_type_is_double(to_data_type)) {
        *((afw_double_t *)to_internal) =
            (afw_double_t)(*(const afw_integer_t *)from_internal);
    }
    else if (afw_data_type_is_boolean(to_data_type)) {
        *(afw_boolean_t *)to_internal =
            *(const afw_integer_t *)from_internal != 0;
    }
    else {
        impl_afw_data_type_standard_convert_internal(instance,
            to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* Used for data type date.  Will convert date to dateTime with just date.   */
static void
impl_afw_data_type_from_date_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_date(to_data_type)) {
        memcpy(to_internal, from_internal, sizeof(afw_date_t));
    }
    else if (afw_data_type_is_dateTime(to_data_type)) {
        afw_memory_clear(&((afw_dateTime_t *)to_internal)->time);
        memcpy(&((afw_dateTime_t *)to_internal)->date,
            &((const afw_date_t *)from_internal)->date,
            sizeof(afw_date_no_time_zone_t));
        memcpy(&((afw_dateTime_t *)to_internal)->time_zone,
            &((const afw_date_t *)from_internal)->time_zone,
            sizeof(afw_time_zone_t));
    }
    else {
        impl_afw_data_type_standard_convert_internal(instance,
            to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* Used for data type time.  Will convert time to dateTime with just time.  */
static void
impl_afw_data_type_from_time_convert_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t *to_data_type,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    if (afw_data_type_is_time(to_data_type)) {
        memcpy(to_internal, from_internal, sizeof(afw_time_t));
    }
    else if (afw_data_type_is_dateTime(to_data_type)) {
        afw_memory_clear(&((afw_dateTime_t *)to_internal)->date);
        memcpy(&((afw_dateTime_t *)to_internal)->time,
            &((const afw_time_t *)from_internal)->time,
            sizeof(afw_time_no_time_zone_t));
        memcpy(&((afw_dateTime_t *)to_internal)->time_zone,
            &((const afw_time_t *)from_internal)->time_zone,
            sizeof(afw_time_zone_t));
    }
    else {
        impl_afw_data_type_standard_convert_internal(instance,
            to_internal, from_internal, to_data_type, p, xctx);
    }
}


/* ---- Data type specific ---------------------------------------------------*/

/* Data type anyURI to internal. */
static void
impl_afw_data_type_anyURI_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type base64Binary to utf8. */
static const afw_utf8_t *
impl_afw_data_type_base64Binary_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_utf8_t *encoded;

    encoded = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    afw_memory_encode_base64(encoded, from_internal, p, xctx);
    return encoded;
}



/* Data type base64Binary to internal. */
static void
impl_afw_data_type_base64Binary_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_memory_decode_base64(to_internal, from_utf8, p, xctx);
}


/* Data type boolean to utf8. */
static const afw_utf8_t *
impl_afw_data_type_boolean_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return (*(afw_boolean_t *)from_internal) ? afw_s_true : afw_s_false;
}


/* Data type boolean to internal. */
static void
impl_afw_data_type_boolean_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    afw_size_t len;

    s = from_utf8->s;
    len = from_utf8->len;

    if (len == 1) {
        if (*s == '1') {
            *(afw_boolean_t *)to_internal = AFW_TRUE;
        } else if (*s == 't') {
            *(afw_boolean_t *)to_internal = AFW_TRUE;
        } else if (*s == 'T') {
            *(afw_boolean_t *)to_internal = AFW_TRUE;
        } else if (*s == '0') {
            *(afw_boolean_t *)to_internal = AFW_FALSE;
        } else if (*s == 'f') {
            *(afw_boolean_t *)to_internal = AFW_FALSE;
        } else if (*s == 'F') {
            *(afw_boolean_t *)to_internal = AFW_FALSE;
        }
    } else if (len == 4) {
        if (*s != 't' && *s != 'T') goto error;
        s++;
        if (*s != 'r' && *s != 'R') goto error;
        s++;
        if (*s != 'u' && *s != 'U') goto error;
        s++;
        if (*s != 'e' && *s != 'E') goto error;
        *(afw_boolean_t *)to_internal = true;
    } else if (len == 5) {
        if (*s != 'f' && *s != 'F') goto error;
        s++;
        if (*s != 'a' && *s != 'A') goto error;
        s++;
        if (*s != 'l' && *s != 'L') goto error;
        s++;
        if (*s != 's' && *s != 'S') goto error;
        s++;
        if (*s != 'e' && *s != 'E') goto error;
        *(afw_boolean_t *)to_internal = false;
    }
    else {
        goto error;
    }
    
    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Not a valid boolean value " AFW_UTF8_FMT_Q ".",
        (int)len, s);
}


/* Data type boolean compare. */
static int
impl_afw_data_type_boolean_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_boolean_t *v1 = value1;
    const afw_boolean_t *v2 = value2;

    /* Consider true > false. */
    if (*v1 == false) {
        if (*v2 == false) return 0;
        return -1;
    }
    if (*v2 == false) return 1;
    return 0;
}


/* Data type date to utf8. */
static const afw_utf8_t *
impl_afw_data_type_date_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_date_internal_to_utf8(
        (const afw_date_t *)from_internal, p, xctx);
}


/* Data type date to internal. */
static void
impl_afw_data_type_date_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_date_utf8_set_internal(from_utf8, (afw_date_t *)to_internal, xctx);
}


/* Data type date compare internal. */
static int
impl_afw_data_type_date_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    return afw_date_compare(value1, value2, xctx);
}


/* Data type dateTime to utf8. */
static const afw_utf8_t *
impl_afw_data_type_dateTime_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_dateTime_internal_to_utf8(
        (const afw_dateTime_t *)from_internal, p, xctx);
}


/* Data type dateTime to internal. */
static void
impl_afw_data_type_dateTime_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_dateTime_utf8_set_internal(from_utf8,
        (afw_dateTime_t *)to_internal, xctx);
}


/* Data type dateTime compare internal. */
static int
impl_afw_data_type_dateTime_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    return afw_dateTime_compare(value1, value2, xctx);
}


/* Data type dayTimeDuration to utf8. */
static const afw_utf8_t *
impl_afw_data_type_dayTimeDuration_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_dayTimeDuration_internal_to_utf8(
        (const afw_dayTimeDuration_t *)from_internal, p, xctx);
}


/* Data type dayTimeDuration to internal. */
static void
impl_afw_data_type_dayTimeDuration_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_dayTimeDuration_utf8_set_internal(from_utf8,
        (afw_dayTimeDuration_t *)to_internal, xctx);
}


/* Data type dayTimeDuration compare internal. */
static int
impl_afw_data_type_dayTimeDuration_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    return afw_dayTimeDuration_compare(value1, value2, xctx);
}


/* Data type dnsName to utf8. */
static void
impl_afw_data_type_dnsName_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type double to utf8. */
static const afw_utf8_t *
impl_afw_data_type_double_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_number_double_to_utf8(*(afw_double_t *)from_internal,
        p, xctx);
}


/* Data type double to internal. */
static void
impl_afw_data_type_double_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_double_t d;

    d = afw_number_utf8_to_double(from_utf8, p, xctx);
    memcpy(to_internal, &d, sizeof(d));
}


/* Data type double compare. */
/*                                                                           */
/* http://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html                                                                          */
/* In comparison operations, positive infinity is larger than all values     */
/* except itself and NaN, and negative infinity is smaller than all values   */
/* except itself and NaN.   NaN is unordered: it is not equal to,            */
/* greater than, or less than anything, including itself.                    */
static int
impl_afw_data_type_double_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_double_t *v1 = value1;
    const afw_double_t *v2 = value2;

    if (afw_number_is_NaN(*v1)) {
        if (afw_number_is_positive_infinity(*v2))
            return 1;
        else if (afw_number_is_NaN(*v2))
            return 0;
        else
            return -1;
    };
    if (afw_number_is_positive_infinity(*v1)) {
        if (afw_number_is_positive_infinity(*v2))
            return 0;
        else
            return 1;
    };
    if (afw_number_is_negative_infinity(*v1)) {
        if (afw_number_is_negative_infinity(*v2))
            return 0;
        else
            return -1;
    };

    if (*v1 == *v2) return 0;
    if (*v1 > *v2) return 1;
    return -1;
}


/* Data type function to utf8. */
/* Note: some other impl_* functions call with instance and p set to NULL. */
static const afw_utf8_t *
impl_afw_data_type_function_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value = from_internal;
    const afw_utf8_t *result;

    if (afw_value_is_string(value)) {
        result = AFW_VALUE_INTERNAL(value);
    }
    else if (afw_value_is_function_definition(value)) {
        result = &((const afw_value_function_definition_t *)value)->
            functionId->internal;
    }
    else if (afw_value_is_script_function_definition(value)) {
        result = ((const afw_value_script_function_definition_t *)value)->
            contextual->compiled_value->full_source;
    }
    else {
        result = afw_s_a_empty_string;
        /** @fixme
        AFW_THROW_ERROR_Z(general,
            "Invalid function internal",
            xctx);
         */
    }

    return result;
}


/* Data type function to internal. */
static void
impl_afw_data_type_function_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_value_create_unmanaged_string(from_utf8, p, xctx);
    memcpy(to_internal, (const void *)&value, sizeof(afw_value_t *));
}


/* Data type function compare. */
static int
impl_afw_data_type_function_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    /* The address of function is used for comparison purposes. */
    return (value1 == value2) ? 0 : (value1 > value2) ? 1 : -1;
}


/* Data type function clone. */
static void
impl_afw_data_type_function_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *s;
    const afw_value_t *value;

    s = impl_afw_data_type_function_internal_to_utf8(
        NULL, from_internal, NULL, xctx);
    value = afw_value_create_unmanaged_string(s, p, xctx);
    memcpy(to_internal, (const void *)&value, sizeof(afw_value_t *));
}


/* Data type hexBinary to utf8. */
static const afw_utf8_t *
impl_afw_data_type_hexBinary_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_utf8_t *encoded;

    encoded = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    afw_memory_encode_printable_hex(
        encoded,
        (const afw_memory_t *)from_internal,
        p, xctx);

    return encoded;
}


/* Data type hexBinary to internal. */
static void
impl_afw_data_type_hexBinary_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_memory_decode_printable_hex((afw_memory_t *)to_internal, from_utf8, p, xctx);
}


/* Data type ia5String ... */

/* Data type ia5String to utf8. */
static void
impl_afw_data_type_ia5String_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type integer to utf8. */
static const afw_utf8_t *
impl_afw_data_type_integer_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_number_integer_to_utf8(
        *(afw_integer_t *)from_internal, p, xctx);
}


/* Data type integer to internal. */
static void
impl_afw_data_type_integer_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_integer_t i;

    i = afw_number_utf8_to_integer(from_utf8, p, xctx);
    memcpy(to_internal, &i, sizeof(i));
}


/* Data type integer compare. */
static int
impl_afw_data_type_integer_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_integer_t *v1 = value1;
    const afw_integer_t *v2 = value2;

    /* Done this way because afw_integer_t may be larger than int. */
    if (*v1 == *v2) return 0;
    if (*v1 > *v2) return 1;
    return -1;
}


/* Data type ipAddress to internal. */
static void
impl_afw_data_type_ipAddress_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type array to utf8. */
static const afw_utf8_t *
impl_afw_data_type_array_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_value_array_t array;

    array.inf = &afw_value_unmanaged_array_inf;
    array.internal = *(const afw_array_t **)from_internal;
    return afw_json_from_value(
        &array.pub, &afw_object_options_useNonStandardTokens,
        p, xctx);
}


/* Data type array to internal. */
static void
impl_afw_data_type_array_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_json_to_value(from_utf8, NULL, p, xctx);

    AFW_VALUE_ASSERT_IS_DATA_TYPE(value, array, xctx);

    *((const afw_array_t **)to_internal) =
        ((afw_value_array_t *)value)->internal;
}


/* Data type array compare. */
static int
impl_afw_data_type_array_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_array_t *array1 = *(const afw_array_t * const *)value1;
    const afw_array_t *array2 = *(const afw_array_t * const *)value2;
    const afw_iterator_t *iterator1;
    const afw_iterator_t *iterator2;
    const void *internal1;
    const void *internal2;
    const afw_data_type_t *data_type1;
    const afw_data_type_t *data_type2;
    int result;

    for (iterator1 = NULL, iterator2 = NULL, result = 0; result == 0;) {
        afw_array_get_next_internal(array1, &iterator1,
            &data_type1, &internal1, xctx);
        afw_array_get_next_internal(array2, &iterator2,
            &data_type2, &internal2, xctx);

        if (!internal1) {
            if (internal2) {
                result = -1;
            }
            break;
        }

        if (!internal2) {
            result = 1;
            break;
        }

        if (!data_type1) {
            AFW_THROW_ERROR_Z(general,
                "array needs data type for compare",
                xctx);
        }

        if (data_type1 != data_type2) {
            result = 1;
            break;
        }

        result = afw_data_type_compare_internal(
            data_type1, internal1, internal2, xctx);
    }

    return result;
}


/* Data type null to utf8. */
static const afw_utf8_t *
impl_afw_data_type_null_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_s_null;
}


/* Data type null to internal. */
static void
impl_afw_data_type_null_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    memset(to_internal, 0, sizeof(void *));
}


/* Data type null compare. */
static int
impl_afw_data_type_null_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    /* Null has no value so comparing two nulls is always true. */
    return 0;
}


/* Data type undefined to utf8. */
static const afw_utf8_t *
impl_afw_data_type_undefined_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_s_undefined;
}


/* Data type undefined to internal. */
static void
impl_afw_data_type_undefined_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    memset(to_internal, 0, sizeof(void *));
}


/* Data type undefined compare. */
static int
impl_afw_data_type_undefined_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    /* Undefined has no value so comparing two undefined values is true. */
    return 0;
}


/* Data type object to utf8. */
static const afw_utf8_t *
impl_afw_data_type_object_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_value_object_t obj;

    obj.inf = &afw_value_unmanaged_object_inf;
    obj.internal = *(const afw_object_t **)from_internal;

    return afw_json_from_value(
        &obj.pub, &afw_object_options_useNonStandardTokens,
        p, xctx);
}


/* Data type object to internal. */
static void
impl_afw_data_type_object_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_json_to_value(from_utf8, NULL, p, xctx);

    AFW_VALUE_ASSERT_IS_DATA_TYPE(value, object, xctx);

    *((const afw_object_t **)to_internal) =
        ((afw_value_object_t *)value)->internal;
}


/* Data type object compare. */
static int
impl_afw_data_type_object_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    const afw_object_t *o1;
    const afw_object_t *o2;
    const afw_value_t *v1;
    const afw_value_t *v2;
    const afw_utf8_t *property_name;
    afw_size_t count1, count2;
    const afw_iterator_t *iterator;

    /* Get object 1 & 2 pointer. */
    o1 = *(const afw_object_t **)value1;
    o2 = *(const afw_object_t **)value2;

    /* Compare matching properties. Return -1 if one doesn't match. */
    iterator = NULL;
    count1 = 0;
    while ((v1 = afw_object_get_next_property(o1, &iterator, &property_name,
        xctx)))
    {
        count1++;
        v2 = afw_object_get_property(o2, property_name, xctx);
        if (!afw_value_equal(v1, v2, xctx)) {
            return -1;
        }
    }

    /* Count properties in second object. */
    iterator = NULL;
    count2 = 0;
    while (afw_object_get_next_property(o2, &iterator, &property_name, xctx))
    {
        count2++;
    }

    /* Return 0 if same number of entries else -1.  */
    return (count1 == count2) ? 0 : -1;
}


/* Data type objectId to utf8. */
static void
impl_afw_data_type_objectId_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type objectPath to utf8. */
static void
impl_afw_data_type_objectPath_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type rfc822Name to utf8. */
static void
impl_afw_data_type_rfc822Name_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type time to utf8. */
static const afw_utf8_t *
impl_afw_data_type_time_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_time_internal_to_utf8(
        (const afw_time_t *)from_internal, p, xctx);
}


/* Data type time to internal. */
static void
impl_afw_data_type_time_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_time_utf8_set_internal(from_utf8, (afw_time_t *)to_internal, xctx);
}


/* Data type time compare internal. */
static int
impl_afw_data_type_time_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    return afw_time_compare(value1, value2, xctx);
}


/* Data type x500Name to internal. */
static void
impl_afw_data_type_x500Name_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Input must already be NFC. */
    memcpy(to_internal, from_utf8, sizeof(afw_utf8_t));
}


/* Data type yearMonthDuration to utf8. */
static const afw_utf8_t *
impl_afw_data_type_yearMonthDuration_internal_to_utf8(
    const afw_data_type_t * instance,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_yearMonthDuration_internal_to_utf8(
        (const afw_yearMonthDuration_t *)from_internal, p, xctx);
}


/* Data type yearMonthDuration to internal. */
static void
impl_afw_data_type_yearMonthDuration_utf8_to_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_yearMonthDuration_utf8_set_internal(from_utf8,
        (afw_yearMonthDuration_t *)to_internal, xctx);
}


/* Data type yearMonthDuration compare internal. */
static int
impl_afw_data_type_yearMonthDuration_compare_internal(
    const afw_data_type_t * instance,
    const void * value1,
    const void * value2,
    afw_xctx_t *xctx)
{
    return afw_yearMonthDuration_compare(value1, value2, xctx);
}


/* ---- Common clone  ------------------------------------------------------*/

/* utf8 clone. */
static void
impl_afw_data_type_utf8_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *from;
    afw_utf8_t *to;
    afw_utf8_octet_t *s;

    memset(to_internal, 0, sizeof(afw_utf8_t));
    from = from_internal;
    if (from->len > 0) {
        s = afw_pool_malloc(p, from->len, xctx);
        to = to_internal;
        to->s = s;
        to->len = from->len;
        memcpy(s, from->s, from->len);
    }
}


/* raw clone. */
static void
impl_afw_data_type_raw_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_t *from;
    afw_memory_t *to;
    afw_byte_t *ptr;

    memset(to_internal, 0, sizeof(afw_utf8_t));
    from = from_internal;
    if (from->size > 0) {
        ptr = afw_pool_malloc(p, from->size, xctx);
        to = to_internal;
        to->ptr = ptr;
        to->size = from->size;
        memcpy(ptr, from->ptr, from->size);
    }
}


/* direct clone. */
static void
impl_afw_data_type_direct_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    memcpy(to_internal, from_internal, instance->c_type_size);
}


/* array clone. */
static void
impl_afw_data_type_array_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *from;
    const afw_array_t *to;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_value_t *cloned_value;
    const afw_data_type_t *data_type;

    from = *(const afw_array_t * *)from_internal;
    data_type = afw_array_get_data_type(from, xctx);
    to = afw_array_of_create(data_type, p, xctx);
    memcpy(to_internal, &to, sizeof(const afw_array_t *));

    for (iterator = NULL;;) {
        value = afw_array_get_next_value(from, &iterator, p, xctx);
        if (!value) {
            break;
        }
        cloned_value = afw_value_clone(value, p, xctx);
        afw_array_add_value(to, cloned_value, xctx);
    }
}



/* Clone object properties and meta. */
static void
impl_object_clone_properties_and_meta(
    const afw_object_t *to,
    const afw_object_t *from,
    const afw_object_t *embedding_object,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = to->p;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_utf8_t *property_name;
    afw_object_meta_t *to_meta;
    const afw_object_t *delta;

    /* Set to meta directly. */
    to_meta = (afw_object_meta_t *)&to->meta;

    /* If const object, no clone is needed of ids. */
    if (!from->p) {
        to_meta->object_type_uri = 
            afw_object_meta_get_object_type_id(from, xctx);
        to_meta->id = from->meta.id;
        to_meta->object_uri = from->meta.object_uri;
    }

    /* If not const object, clone ids. */
    else {
        if (from->meta.object_type_uri) {
            to_meta->object_type_uri = afw_utf8_clone(
                from->meta.object_type_uri,
                p, xctx);
        }
        if (!to->meta.embedding_object && from->meta.id)
        {
            to_meta->id = afw_utf8_clone(from->meta.id, p, xctx);
        }
        if (!to->meta.embedding_object && from->meta.object_uri)
        {
            to_meta->object_uri =
                afw_utf8_clone(from->meta.object_uri, p, xctx);
        }
    }

    /* Get meta info. */
    if (from->meta.meta_object) {
        delta = afw_object_meta_get_nonempty_delta(to, xctx);
        impl_object_clone_properties_and_meta(
            delta, from->meta.meta_object, NULL, xctx);
    }

    /* Clone properties. */
    for (iterator = NULL;;) {
        value = afw_object_get_next_property(from,
            &iterator, &property_name, xctx);
        if (!value) {
            break;
        }
        impl_clone_and_set_property(to, property_name, value,
            xctx);
    }
}



AFW_DEFINE_STATIC_INLINE(void)
impl_clone_embedded_object(
    const afw_object_t * *to,
    const afw_object_t *from,
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    *to = afw_object_create_embedded(
        embedding_object, property_name,
        xctx);

    impl_object_clone_properties_and_meta(*to, from,
        embedding_object, xctx);
}



static void
impl_clone_and_set_property(
    const afw_object_t *obj,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_t *cloned_value;
    const afw_pool_t *p = obj->p;

    /* Clone property name.*/
    property_name = afw_utf8_clone(property_name, p, xctx);

    /* If object, handle special to support embedding object. */
    if (afw_value_is_object(value)) {
        cloned_value = (const afw_value_t *)
            afw_value_allocate_unmanaged_object(p, xctx);
        impl_clone_embedded_object(
            &((afw_value_object_t *)cloned_value)->internal,
            ((const afw_value_object_t *)value)->internal,
            obj, property_name, xctx);
    }

    /* Other cases, just use normal value clone. */
    else {
        cloned_value = afw_value_clone(value, p, xctx);
    }

    /* Set property to cloned value. */
    afw_object_set_property(obj,
        afw_utf8_clone(property_name, p, xctx),
        cloned_value, xctx);
}



/* object clone. */
static void
impl_afw_data_type_object_clone_internal(
    const afw_data_type_t * instance,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *from;
    const afw_object_t *to;

    from = *(const afw_object_t * *)from_internal;
    to = afw_object_create_unmanaged(p, xctx);
    memcpy(to_internal, &to, sizeof(const afw_object_t *));
    impl_object_clone_properties_and_meta(to, from, NULL, xctx);
}




/* Clone an object to a managed object. */
AFW_DEFINE(const afw_object_t *)
afw_data_type_object_create_clone_to_managed_object(
    const afw_object_t * object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_managed(p, xctx);
    impl_object_clone_properties_and_meta(result, object, NULL, xctx);

    return result;
}



/* ---- Value compiler listing -----------------------------------*/

static void
impl_afw_data_type_double_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    string = afw_data_type_internal_to_utf8(
        instance,
        AFW_VALUE_INTERNAL(value),
        writer->p, xctx);
    afw_writer_write_utf8(writer, string, xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_afw_data_type_boolean_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);

    if (((const afw_value_boolean_t *)value)->internal) {
        afw_writer_write_z(writer, "true", xctx);
    }
    else {
        afw_writer_write_z(writer, "false", xctx);
    }
    afw_writer_write_eol(writer, xctx);
}



static void
impl_afw_data_type_function_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    /** @fixme improve. */
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    string = afw_data_type_internal_to_utf8(
        instance,
        AFW_VALUE_INTERNAL(value),
        writer->p, xctx);
    afw_writer_write_utf8(writer, string, xctx);
    afw_writer_write_eol(writer, xctx);
}



static void
impl_afw_data_type_integer_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    string = afw_data_type_internal_to_utf8(
        instance,
        AFW_VALUE_INTERNAL(value),
        writer->p, xctx);
    afw_writer_write_utf8(writer, string, xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_afw_data_type_null_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " null", xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_afw_data_type_undefined_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " undefined", xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_afw_data_type_utf8_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    afw_json_write_encoded_string(
        AFW_VALUE_INTERNAL(value),
        writer, xctx);
    afw_writer_write_eol(writer, xctx);
}



static void
impl_afw_data_type_typed_to_string_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    string = afw_data_type_internal_to_utf8(instance,
        AFW_VALUE_INTERNAL(value), writer->p, xctx);
    afw_json_write_encoded_string(
        string, writer, xctx);
    afw_writer_write_eol(writer, xctx);
}


static void
impl_afw_data_type_array_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_t *array;
    const afw_iterator_t *iterator;
    const afw_value_t *entry;

    array = ((const afw_value_array_t *)value)->internal;
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    for (iterator = NULL;;)
    {
        entry = afw_array_get_next_value(array, &iterator, writer->p, xctx);
        if (!entry) break;
        afw_value_compiler_listing_value(entry, writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}


AFW_DEFINE(void)
afw_data_type_object_value_compiler_listing(
    const afw_writer_t *writer,
    const afw_value_t *object_value,
    afw_boolean_t scalar_only,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_iterator_t *iterator;
    const afw_value_t *pv;
    const afw_utf8_t *property_name;
 
    object = afw_value_as_object(object_value, xctx);
    afw_value_compiler_listing_begin_value(writer, object_value, NULL, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    for (iterator = NULL;;)
    {
        pv = afw_object_get_next_property(object,
            &iterator, &property_name, xctx);
        if (!pv) {
            break;
        }
        if (scalar_only && !afw_value_is_scalar(pv, xctx)) {
            continue;
        }
        afw_writer_write_z(writer, "property ", xctx);
        afw_writer_write_utf8(writer, property_name, xctx);
        afw_writer_write_z(writer, " ", xctx);
        afw_value_compiler_listing_value(pv, writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}



static void
impl_afw_data_type_object_value_compiler_listing(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_data_type_object_value_compiler_listing(writer, value, false, xctx);
}



/* ---- Write as expression  -------------------------------------------------*/

static void
impl_afw_data_type_double_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    string = afw_data_type_internal_to_utf8(
        instance,
        from_internal,
        writer->p, xctx);
    afw_writer_write_utf8(writer, string, xctx);
}


static void
impl_afw_data_type_boolean_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    if (*(const afw_boolean_t *)from_internal) {
        afw_writer_write_z(writer, "true", xctx);
    }
    else {
        afw_writer_write_z(writer, "false", xctx);
    }
}


static void
impl_afw_data_type_integer_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    string = afw_data_type_internal_to_utf8(
        instance,
        from_internal,
        writer->p, xctx);
    afw_writer_write_utf8(writer, string, xctx);
}


static void
impl_afw_data_type_null_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    afw_writer_write_z(writer, "null", xctx);
}


static void
impl_afw_data_type_undefined_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    afw_writer_write_z(writer, "undefined", xctx);
}


static void
impl_afw_data_type_string_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    afw_json_write_encoded_string(
        (const afw_utf8_t *)from_internal,
        writer, xctx);
}


static void
impl_afw_data_type_typed_string_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    afw_writer_write_utf8(writer, &instance->data_type_id, xctx);
    afw_writer_write_z(writer, "(", xctx);
    afw_json_write_encoded_string(
        (const afw_utf8_t *)from_internal,
        writer, xctx);
    afw_writer_write_z(writer, ")", xctx);
}


static void
impl_afw_data_type_typed_to_string_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    string = afw_data_type_internal_to_utf8(instance, from_internal,
        writer->p, xctx);
    afw_writer_write_utf8(writer, &instance->data_type_id, xctx);
    afw_writer_write_z(writer, "(", xctx);
    afw_json_write_encoded_string(
        string, writer, xctx);
    afw_writer_write_z(writer, ")", xctx);
}


static void
impl_afw_data_type_array_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    const afw_array_t *array;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    afw_size_t i;

    array = *(const afw_array_t * const *)from_internal;

    afw_writer_write_z(writer, "[", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }

    for (i = 0, iterator = NULL;
        (value = afw_array_get_next_value(array, &iterator, writer->p, xctx));
        i++)
    {
        if (i != 0) {      
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_value_decompile_value(value, writer, xctx);     
    }

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, "]", xctx);

}


static void
impl_afw_data_type_object_write_as_expression(
    const afw_data_type_t *instance,
    const afw_writer_t *writer,
    const void *from_internal,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_utf8_t *property_name;
    afw_size_t i;

    object = *(const afw_object_t * const *)from_internal;

    afw_writer_write_z(writer, "{", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }

    for (i = 0, iterator = NULL;
        (value = afw_object_get_next_property(object,
            &iterator, &property_name, xctx));
        i++)
    {
        if (i != 0) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_json_write_encoded_string(property_name, writer, xctx);
        if (writer->tab) {
            /* if there is any tab argument, use one space to separate property keys from values */
            afw_writer_write_z(writer, ": ", xctx);
        } else {
            afw_writer_write_z(writer, ":", xctx);
        }
        afw_value_decompile_value(value, writer, xctx);
    }

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, "}", xctx);
}


/* ---- Not implemented methods -------------------------------------------- */

static void
impl_afw_data_type_evaluate_utf8_to_internal(
    const afw_data_type_t* instance,
    void* to_internal,
    const afw_utf8_t* from_utf8,
    const afw_pool_t* p,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}


static const afw_utf8_t*
impl_afw_data_type_evaluate_internal_to_utf8(
    const afw_data_type_t* instance,
    const void* from_internal,
    const afw_pool_t* p,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}


static int
impl_afw_data_type_evaluate_compare_internal(
    const afw_data_type_t* instance,
    const void* value1,
    const void* value2,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}


static void
impl_afw_data_type_evaluate_convert_internal(
    const afw_data_type_t* instance,
    void* to_internal,
    const void* from_internal,
    const afw_data_type_t* to_data_type,
    const afw_pool_t* p,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}


static void
impl_afw_data_type_evaluate_clone_internal(
    const afw_data_type_t* instance,
    void* to_internal,
    const void* from_internal,
    const afw_pool_t* p,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}


static void
impl_afw_data_type_evaluate_value_compiler_listing(
    const afw_data_type_t* instance,
    const afw_writer_t* writer,
    const afw_value_t* value,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}


static void
impl_afw_data_type_evaluate_write_as_expression(
    const afw_data_type_t* instance,
    const afw_writer_t* writer,
    const void* from_internal,
    afw_xctx_t* xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented for this data type", xctx);
}



/* ---- Interface definitions ------------------------------------------------*/

#define IMPL_DATA_TYPE_INF(                                                 \
    _id, _to_utf8, _to_internal, _compare, _conv, _clone,                   \
    _compiler_listing, _expression)                                         \
                                                                            \
AFW_DEFINE_INTERNAL_CONST_DATA(afw_data_type_inf_t)                         \
afw_data_type_ ## _id ## _inf = {                                           \
    {                                                                       \
        AFW_UTF8_LITERAL("afw_data_type"),                                  \
        AFW_UTF8_LITERAL(__FILE__),                                         \
        AFW_UTF8_LITERAL("afw_data_type_" #_id )                            \
    },                                                                      \
    impl_afw_data_type_ ## _to_utf8 ## _internal_to_utf8,                   \
    impl_afw_data_type_ ## _to_internal ## _utf8_to_internal,               \
    impl_afw_data_type_ ## _compare ## _compare_internal,                   \
    impl_afw_data_type_ ## _conv ## _convert_internal,                      \
    impl_afw_data_type_ ## _clone ## _clone_internal,                       \
    impl_afw_data_type_ ## _compiler_listing ## _value_compiler_listing,    \
    impl_afw_data_type_ ## _expression ## _write_as_expression              \
};                                                                          \


IMPL_DATA_TYPE_INF(
    any,                  /* data type id      */
    evaluate,             /* to utf8           */
    evaluate,             /* to internal       */
    evaluate,             /* compare           */
    evaluate,             /* conversion        */
    evaluate,             /* clone             */
    evaluate,             /* compiler listing  */
    evaluate)             /* as expression     */

IMPL_DATA_TYPE_INF(
    anyURI,               /* data type id      */
    utf8,                 /* to utf8           */
    anyURI,               /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    array,                /* data type id      */
    array,                /* to utf8           */
    array,                /* to internal       */
    array,                /* compare           */
    from_pointer,         /* conversion        */
    array,                /* clone             */
    array,                /* compiler listing  */
    array)                /* as expression     */

IMPL_DATA_TYPE_INF(
    base64Binary,         /* data type id      */
    base64Binary,         /* to utf8           */
    base64Binary,         /* to internal       */
    raw,                  /* compare           */
    from_raw,             /* conversion        */
    raw,                  /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

IMPL_DATA_TYPE_INF(
    boolean,              /* data type id      */
    boolean,              /* to utf8           */
    boolean,              /* to internal       */
    boolean,              /* compare           */
    standard,             /* conversion        */
    direct,               /* clone             */
    boolean,              /* compiler listing  */
    boolean)              /* as expression     */

IMPL_DATA_TYPE_INF(
    date,                 /* data type id      */
    date,                 /* to utf8           */
    date,                 /* to internal       */
    date,                 /* compare           */
    from_date,            /* conversion        */
    direct,               /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

IMPL_DATA_TYPE_INF(
    dateTime,             /* data type id      */
    dateTime,             /* to utf8           */
    dateTime,             /* to internal       */
    dateTime,             /* compare           */
    from_dateTime,        /* conversion        */
    direct,               /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

IMPL_DATA_TYPE_INF(
    dayTimeDuration,      /* data type id      */
    dayTimeDuration,      /* to utf8           */
    dayTimeDuration,      /* to internal       */
    dayTimeDuration,      /* compare           */
    standard,             /* conversion        */
    direct,               /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

IMPL_DATA_TYPE_INF(
    dnsName,              /* data type id      */
    utf8,                 /* to utf8           */
    dnsName,              /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    double,               /* data type id      */
    double,               /* to_utf8           */
    double,               /* to internal       */
    double,               /* compare           */
    from_double,          /* conversion        */
    direct,               /* clone             */
    double,               /* compiler listing  */
    double)               /* as expression     */

IMPL_DATA_TYPE_INF(
    function,             /* data type id      */
    function,             /* to utf8           */
    function,             /* to internal       */
    function,             /* compare           */
    standard,             /* conversion        */
    function,             /* clone             */
    function,             /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    hexBinary,            /* data type id      */
    hexBinary,            /* to utf8           */
    hexBinary,            /* to internal       */
    raw,                  /* compare           */
    from_raw,             /* conversion        */
    raw,                  /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

IMPL_DATA_TYPE_INF(
    ia5String,            /* data type id      */
    utf8,                 /* to utf8           */
    ia5String,            /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    integer,              /* data type id      */
    integer,              /* to utf8           */
    integer,              /* to internal       */
    integer,              /* compare           */
    from_integer,         /* conversion        */
    direct,               /* clone             */
    integer,              /* compiler listing  */
    integer)              /* as expression     */

IMPL_DATA_TYPE_INF(
    ipAddress,            /* data type id      */
    utf8,                 /* to utf8           */
    ipAddress,            /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    json,                 /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    null,                 /* data type id      */
    null,                 /* to utf8           */
    null,                 /* to internal       */
    null,                 /* compare           */
    standard,             /* conversion        */
    direct,               /* clone             */
    null,                 /* compiler listing  */
    null)                 /* as expression     */

IMPL_DATA_TYPE_INF(
    object,               /* data type id      */
    object,               /* to utf8           */
    object,               /* to internal       */
    object,               /* compare           */
    from_pointer,         /* conversion        */
    object,               /* clone             */
    object,               /* compiler listing  */
    object)               /* as expression     */

IMPL_DATA_TYPE_INF(
    objectId,             /* data type id      */
    utf8,                 /* to utf8           */
    objectId,             /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    objectPath,           /* data type id      */
    utf8,                 /* to utf8           */
    objectPath,           /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

/** @fixme Should password be raw or encoded?  This will not do. */
IMPL_DATA_TYPE_INF(
    password,             /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    regexp,               /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    relaxed_json,         /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    rfc822Name,           /* data type id      */
    utf8,                 /* to utf8           */
    rfc822Name,           /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    script,               /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    string,               /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    string)               /* as expression     */

IMPL_DATA_TYPE_INF(
    template,             /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    time,                 /* data type id      */
    time,                 /* to utf8           */
    time,                 /* to internal       */
    time,                 /* compare           */
    from_time,            /* conversion        */
    direct,               /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

IMPL_DATA_TYPE_INF(
    undefined,            /* data type id      */
    undefined,            /* to utf8           */
    undefined,            /* to internal       */
    undefined,            /* compare           */
    standard,             /* conversion        */
    direct,               /* clone             */
    undefined,            /* compiler listing  */
    undefined)            /* as expression     */

IMPL_DATA_TYPE_INF(
    unevaluated,          /* data type id      */
    evaluate,             /* to utf8           */
    evaluate,             /* to internal       */
    evaluate,             /* compare           */
    evaluate,             /* conversion        */
    evaluate,             /* clone             */
    evaluate,             /* compiler listing  */
    evaluate)             /* as expression     */

/** @fixme make appropriate changes. */
IMPL_DATA_TYPE_INF(
    unknown,              /* data type id      */
    evaluate,             /* to utf8           */
    evaluate,             /* to internal       */
    evaluate,             /* compare           */
    evaluate,             /* conversion        */
    evaluate,             /* clone             */
    evaluate,             /* compiler listing  */
    evaluate)             /* as expression     */

/** @fixme make appropriate changes. */
IMPL_DATA_TYPE_INF(
    void,                 /* data type id      */
    evaluate,             /* to utf8           */
    evaluate,             /* to internal       */
    evaluate,             /* compare           */
    evaluate,             /* conversion        */
    evaluate,             /* clone             */
    evaluate,             /* compiler listing  */
    evaluate)             /* as expression     */

IMPL_DATA_TYPE_INF(
    x500Name,             /* data type id      */
    utf8,                 /* to utf8           */
    x500Name,             /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    xpathExpression,      /* data type id      */
    utf8,                 /* to utf8           */
    utf8,                 /* to internal       */
    utf8,                 /* compare           */
    from_utf8,            /* conversion        */
    utf8,                 /* clone             */
    utf8,                 /* compiler listing  */
    typed_string)         /* as expression     */

IMPL_DATA_TYPE_INF(
    yearMonthDuration,    /* data type id      */
    yearMonthDuration,    /* to utf8           */
    yearMonthDuration,    /* to internal       */
    yearMonthDuration,    /* compare           */
    standard,             /* conversion        */
    direct,               /* clone             */
    typed_to_string,      /* compiler listing  */
    typed_to_string)      /* as expression     */

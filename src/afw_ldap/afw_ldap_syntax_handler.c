// See the 'COPYING' file in the project root for licensing information.
/*
 * Internal LDAP Adaptive Framework Syntax Handlers
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_ldap_syntax_handler.c
 * @brief  Internal LDAP Syntax Handlers
 */

#include "afw.h"
#include "afw_ldap_internal.h"
#include "afw_ldap_metadata.h"


static const struct berval
impl_ber_true = { 4, "TRUE" };

static const struct berval
impl_ber_false = { 5, "FALSE" };

static void
impl_x_nds_syntax_handler(
    const afw_ldap_metadata_syntax_handler_t * *syntax_handler,
    const afw_value_t * *data_type_id_value,
    const afw_object_t *attribute_type_object,
    const afw_utf8_t *id, afw_boolean_t is_single, afw_xctx_t *xctx);


static const afw_value_t *
impl_syntax_handler_single_binary_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_single_binary_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_single_binary =
{
    NULL,
    impl_syntax_handler_single_binary_to_value,
    impl_syntax_handler_single_binary_to_ber
};


static const afw_value_t *
impl_syntax_handler_single_boolean_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_single_boolean_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_single_boolean =
{
    NULL,
    impl_syntax_handler_single_boolean_to_value,
    impl_syntax_handler_single_boolean_to_ber
};


static const afw_value_t *
impl_syntax_handler_single_generalized_time_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data,
    struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_single_generalized_time_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_single_generalized_time =
{
    NULL,
    impl_syntax_handler_single_generalized_time_to_value,
    impl_syntax_handler_single_generalized_time_to_ber
};


static const afw_value_t *
impl_syntax_handler_single_integer_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_single_integer_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_single_integer =
{
    NULL,
    impl_syntax_handler_single_integer_to_value,
    impl_syntax_handler_single_integer_to_ber
};


static const afw_value_t *
impl_syntax_handler_single_string_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_single_string_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_single_string =
{
    NULL,
    impl_syntax_handler_single_string_to_value,
    impl_syntax_handler_single_string_to_ber
};


static const afw_value_t *
impl_syntax_handler_single_unknown_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_single_unknown_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_single_unknown =
{
    NULL,
    impl_syntax_handler_single_unknown_to_value,
    impl_syntax_handler_single_unknown_to_ber
};


static const afw_value_t *
impl_syntax_handler_list_binary_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_list_binary_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_list_binary =
{
    NULL,
    impl_syntax_handler_list_binary_to_value,
    impl_syntax_handler_list_binary_to_ber
};


static const afw_value_t *
impl_syntax_handler_list_boolean_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_list_boolean_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_list_boolean =
{
    NULL,
    impl_syntax_handler_list_boolean_to_value,
    impl_syntax_handler_list_boolean_to_ber
};


static const afw_value_t *
impl_syntax_handler_list_generalized_time_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data,
    struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_list_generalized_time_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_list_generalized_time =
{
    NULL,
    impl_syntax_handler_list_generalized_time_to_value,
    impl_syntax_handler_list_generalized_time_to_ber
};


static const afw_value_t *
impl_syntax_handler_list_integer_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_list_integer_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_list_integer =
{
    NULL,
    impl_syntax_handler_list_integer_to_value,
    impl_syntax_handler_list_integer_to_ber
};


static const afw_value_t *
impl_syntax_handler_list_string_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_list_string_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_list_string =
{
    NULL,
    impl_syntax_handler_list_string_to_value,
    impl_syntax_handler_list_string_to_ber
};



static const afw_value_t *
impl_syntax_handler_list_unknown_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx);

static struct berval **
impl_syntax_handler_list_unknown_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);

static const afw_ldap_metadata_syntax_handler_t
impl_syntax_handler_list_unknown =
{
    NULL,
    impl_syntax_handler_list_unknown_to_value,
    impl_syntax_handler_list_unknown_to_ber
};




static const afw_value_t *
impl_syntax_handler_single_binary_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_memory_t raw;

    if (ldap_count_values_len(bv) != 1) {
        AFW_THROW_ERROR_Z(general, "Expecting single value", xctx);
    }

    raw.ptr = afw_memory_dup((*bv)->bv_val, (*bv)->bv_len, p, xctx);
    raw.size = (*bv)->bv_len;

    return afw_value_create_base64Binary(&raw, p, xctx);
}


static struct berval **
impl_syntax_handler_single_binary_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t size;
    const afw_byte_t *ptr;
    struct berval **result;
    const afw_data_type_t *value_data_type;

    value_data_type = afw_value_get_data_type(value, xctx);

    if (!afw_value_is_defined_and_evaluated(value)) {
        AFW_THROW_ERROR_Z(general, "Expecting evaluated value", xctx);
    }

    if (afw_utf8_equal(&value_data_type->cType, &afw_s_afw_memory_t)) {
        size = ((afw_value_hexBinary_t *)value)->internal.size;
        ptr = ((afw_value_hexBinary_t *)value)->internal.ptr;
    }
    else if (afw_utf8_equal(&value_data_type->cType, &afw_s_afw_utf8_t)) {
        size = ((afw_value_string_t *)value)->internal.len;
        ptr = (afw_byte_t *)((afw_value_string_t *)value)->internal.s;
    }
    else {
        AFW_THROW_ERROR_Z(general,
            "Expecting a data type with cType afw_memory_t or afw_utf8_t",
            xctx);
    }

    result = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
    result[0] = afw_pool_calloc_type(p, struct berval, xctx);
    result[0]->bv_len = (ber_len_t)size;
    result[0]->bv_val = (char *)ptr;

    return result;
}


static const afw_value_t *
impl_syntax_handler_single_boolean_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;

    if (ldap_count_values_len(bv) != 1) {
        AFW_THROW_ERROR_Z(general, "Expecting single value", xctx);
    }

    if ((*bv)->bv_len == 4 && memcmp((*bv)->bv_val, "TRUE", 4) == 0) {
        result = afw_value_true;
    }
    else if ((*bv)->bv_len == 5 && memcmp((*bv)->bv_val, "FALSE", 5) == 0) {
        result = afw_value_false;
    }
    else {
        AFW_THROW_ERROR_Z(general, "Invalid ldap boolean value", xctx);
    }

    return result;
}


static struct berval **
impl_syntax_handler_single_boolean_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    const struct berval *ber;
    afw_boolean_t b;

    b = afw_value_as_boolean(value, xctx);
    ber = (b)
        ? &impl_ber_true
        : &impl_ber_false;

    result = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
    result[0] = (struct berval *)ber;

    return result;
}


static const afw_value_t *
impl_syntax_handler_single_generalized_time_to_value(    
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t generalized_time;
    int count;
    afw_dateTime_t internal;

    count = ldap_count_values_len(bv);
    if (count != 1) {
        AFW_THROW_ERROR_Z(general, "Expecting single value", xctx);
    }

    generalized_time.s = (*bv)->bv_val;
    generalized_time.len = (*bv)->bv_len;

    afw_dataType_generalized_time_set_internal(&generalized_time, &internal,
        xctx);

    return afw_value_create_dateTime(&internal, p, xctx);
}


static struct berval **
impl_syntax_handler_single_generalized_time_to_ber(    
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    const afw_utf8_t *s;

    /* If value is dateTime, convert it to generalized and create result. */
    if (afw_value_is_dateTime(value)) {
        s = afw_dateType_internal_to_generalized_time(
            &((const afw_value_dateTime_t *)value)->internal,
            p, xctx);
        result = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
        result[0] = afw_pool_malloc_type(p, struct berval, xctx);
        result[0]->bv_len = (ber_len_t)s->len;
        result[0]->bv_val = (char *)s->s;
    }

    /* If value is not dateTime, treat it like string. Let ldap deal with it. */
    else {
        result = impl_syntax_handler_single_string_to_ber(
            attribute_type, data, value, p, xctx);
    }

    return result;
}


static const afw_value_t *
impl_syntax_handler_single_integer_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t integer;
    afw_utf8_t s;

    if (ldap_count_values_len(bv) != 1) {
        AFW_THROW_ERROR_Z(general, "Expecting single value", xctx);
    }

    s.s = (*bv)->bv_val;
    s.len = (*bv)->bv_len;

    integer = afw_number_utf8_to_integer(&s, p, xctx);

    return afw_value_create_integer(integer, p, xctx);
}


static struct berval **
impl_syntax_handler_single_integer_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* string_to_ber will handle this correctly. */
    return impl_syntax_handler_single_string_to_ber(
        attribute_type, data, value, p, xctx);
}


static const afw_value_t *
impl_syntax_handler_single_string_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *s;
    int count;

    count = ldap_count_values_len(bv);
    if (count != 1) {
        AFW_THROW_ERROR_Z(general, "Expecting single value", xctx);
    }

    s = afw_utf8_create_copy((*bv)->bv_val, (*bv)->bv_len, p, xctx);
    return afw_value_create_string(s, p, xctx);
}


static struct berval **
impl_syntax_handler_single_string_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    const afw_value_string_t *string;

    if (afw_value_is_string(value)) {
        string = (const afw_value_string_t *)value;
    }
    else {
        string = (const afw_value_string_t *)
            afw_value_convert(value,
                afw_data_type_string, true, p, xctx);
    }

    result = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
    result[0] = afw_pool_malloc_type(p, struct berval, xctx);
    result[0]->bv_len = (ber_len_t)string->internal.len;
    result[0]->bv_val = (char *)string->internal.s;

    return result;
}


static const afw_value_t *
impl_syntax_handler_single_unknown_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    int count;

    count = ldap_count_values_len(bv);
    if (count != 1) {
        AFW_THROW_ERROR_Z(general, "Expecting single value", xctx);
    }

    if (afw_utf8_is_valid((*bv)->bv_val, (*bv)->bv_len, xctx)) {
        return impl_syntax_handler_single_string_to_value(
            attribute_type, data, bv, p, xctx);
    }
    else {
        return impl_syntax_handler_single_binary_to_value(
            attribute_type, data, bv, p, xctx);
    }
}


static struct berval **
impl_syntax_handler_single_unknown_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* Single binary will handle afw_utf8_t and afw_memory_t. */
    return impl_syntax_handler_single_binary_to_ber(
        attribute_type, data, value, p, xctx);
}


static const afw_value_t *
impl_syntax_handler_list_binary_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    int count;
    const afw_value_t *result;
    const afw_list_t *list;
    afw_memory_t *e;

    count = ldap_count_values_len(bv);
    e = afw_pool_malloc(p, count * sizeof(afw_memory_t), xctx);
    list = afw_list_create_wrapper_for_array(
        e, false, afw_data_type_base64Binary, count, p, xctx);
    result = afw_value_create_list(list, p, xctx);

    for (; count > 0; count--, bv++, e++) {
        e->ptr = afw_memory_dup((*bv)->bv_val, (*bv)->bv_len, p, xctx);
        e->size = (*bv)->bv_len;
    }

    return result;
}


static struct berval **
impl_syntax_handler_list_binary_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    struct berval **bv;
    afw_size_t count;
    const afw_list_t *list;
    const afw_memory_t *raw;
    const afw_utf8_t *s;
    const afw_iterator_t *iterator;
    const afw_data_type_t *data_type;

    if (afw_value_is_list(value)) {
        list = ((afw_value_list_t *)value)->internal;
        count = afw_list_get_count(list, xctx);
        result = afw_pool_calloc(p, sizeof(struct berval *) * (count + 1),
            xctx);
        data_type = afw_list_get_data_type(value, xctx);
        if (afw_utf8_equal(&data_type->cType, &afw_s_afw_memory_t)) {
            for (iterator = NULL, bv = result; ;bv++) {
                afw_list_get_next_internal(
                    list, &iterator, NULL, (const void **)&raw, xctx);
                if (!raw) {
                    break;
                }
                *bv = afw_pool_malloc_type(p, struct berval, xctx);
                (*bv)->bv_len = (ber_len_t)raw->size;
                (*bv)->bv_val = (char *)raw->ptr;
            }
        }
        else if (afw_utf8_equal(&data_type->cType, &afw_s_afw_utf8_t)) {
            for (iterator = NULL, bv = result; ;bv++) {
                afw_list_get_next_internal(
                    list, &iterator, NULL, (const void **)&s, xctx);
                if (!s) {
                    break;
                }
                *bv = afw_pool_malloc_type(p, struct berval, xctx);
                (*bv)->bv_len = (ber_len_t)s->len;
                (*bv)->bv_val = (char *)s->s;
            }
        }
        else {
            AFW_THROW_ERROR_Z(general,
                "Expecting a data type with cType afw_memory_t or afw_utf8_t",
                xctx);
        }
    }

    else {
        result = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
        data_type = afw_value_get_data_type(value, xctx);
        if (afw_utf8_equal(&data_type->cType, &afw_s_afw_memory_t)) {
            raw = (const afw_memory_t *)AFW_VALUE_INTERNAL(value);
            *result = afw_pool_malloc_type(p, struct berval, xctx);
            (*result)->bv_len = (ber_len_t)raw->size;
            (*result)->bv_val = (char *)raw->ptr;
        }
        else if (afw_utf8_equal(&data_type->cType, &afw_s_afw_utf8_t)) {
            s = (const afw_utf8_t *)AFW_VALUE_INTERNAL(value);
            *result = afw_pool_malloc_type(p, struct berval, xctx);
            (*result)->bv_len = (ber_len_t)s->len;
            (*result)->bv_val = (char *)s->s;
        }
        else {
            AFW_THROW_ERROR_Z(general,
                "Expecting a data type with cType afw_memory_t or afw_utf8_t",
                xctx);
        }
    }

    return result;
}


static const afw_value_t *
impl_syntax_handler_list_boolean_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    int count;
    const afw_value_t *result;
    const afw_list_t *list;
    afw_boolean_t *e;

    count = ldap_count_values_len(bv);
    if (count == 0) {
        return afw_data_type_boolean->empty_list_value;
    }

    e = afw_pool_malloc(p, sizeof(afw_boolean_t) * count, xctx);
    list = afw_list_create_wrapper_for_array(
        e, false, afw_data_type_boolean, count, p, xctx);
    result = afw_value_create_list(list, p, xctx);

    for (; count > 0; count--, bv++, e++) {
        if ((*bv)->bv_len == 4 && memcmp((*bv)->bv_val, "TRUE", 4) == 0) {
            *e = true;
        } else if ((*bv)->bv_len == 5 && memcmp((*bv)->bv_val, "FALSE", 5) == 0) {
            *e = false;
        } else {
            AFW_THROW_ERROR_Z(general, "Invalid ldap boolean value", xctx);
        }
    }

    return result;
}


static struct berval **
impl_syntax_handler_list_boolean_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    struct berval **bv;
    afw_boolean_t b;
    afw_size_t count;
    const afw_data_type_t *data_type;
    afw_list_wrapper_for_array_self_t wrapper;
    const afw_list_t *list;
    const afw_iterator_t *iterator;
    const void *internal;

    if (afw_value_is_list(value)) {
        list = (const afw_list_t *)AFW_VALUE_INTERNAL(value);
        count = afw_list_get_count(list, xctx);
    }
    else {
        count = 1;
        AFW_LIST_INITIALIZE_WRAPPER_FOR_ARRAY_P(
            &wrapper, AFW_VALUE_INTERNAL(value), false,
            afw_value_get_data_type(value, xctx),
            count, p);
        list = (const afw_list_t *)&wrapper;
    }

    result = afw_pool_malloc(p, sizeof(struct berval *) *
        (count + 1), xctx);
    result[count] = NULL;

    for (iterator = NULL, bv = result; ;bv++) {
        afw_list_get_next_internal(list,
            &iterator, &data_type, &internal, xctx);
        if (!internal) {
            break;
        }
        if (data_type == afw_data_type_boolean) {
            b = *(afw_boolean_t *)internal;
        }
        else {
            afw_data_type_convert_internal(data_type,
                &b, internal, afw_data_type_boolean, p, xctx);
        }
        *bv = (struct berval *)
            ((b)
            ? &impl_ber_true
            : &impl_ber_false);
    }

    return result;
}


static const afw_value_t *
impl_syntax_handler_list_generalized_time_to_value(    
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t generalized_time;
    afw_size_t count;
    const afw_value_t *result;
    const afw_list_t *list;
    afw_dateTime_t *e;

    count = ldap_count_values_len(bv);
    if (count == 0) {
        return afw_data_type_dateTime->empty_list_value;
    }
    e = afw_pool_malloc(p, sizeof(afw_dateTime_t) * count, xctx);
    list = afw_list_create_wrapper_for_array(
        (const void *)e, false, afw_data_type_dateTime, count, p, xctx);
    result = afw_value_create_list(list, p, xctx);
    for (; count > 0; count--, bv++, e++) {
        generalized_time.s = (*bv)->bv_val;
        generalized_time.len = (*bv)->bv_len;
        afw_dataType_generalized_time_set_internal(
            &generalized_time, e, xctx);
    }

    return result;
}


static struct berval **
impl_syntax_handler_list_generalized_time_to_ber(    
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    struct berval **bv;
    const afw_utf8_t *s;
    const afw_dateTime_t *val;
    const afw_list_t *list;
    const afw_iterator_t *iterator;
    afw_size_t count;

    /* Process single dataTime. */
    if (afw_value_is_dateTime(value)) {
        result = afw_pool_calloc(p, sizeof(struct berval *) * 2, xctx);
        s = afw_dateType_internal_to_generalized_time(
            AFW_VALUE_INTERNAL(value), p, xctx);
        *result = afw_pool_malloc_type(p, struct berval, xctx);
        (*result)->bv_len = (ber_len_t)s->len;
        (*result)->bv_val = (char *)s->s;
    }

    /* Process list of dataTime. */
    else if (afw_value_is_list_of_dateTime(value)) {
        list = ((afw_value_list_t *)value)->internal;
        count = afw_list_get_count(list, xctx);
        result = afw_pool_calloc(p, sizeof(struct berval *) * (count + 1),
            xctx);
        for (iterator = NULL, bv = result; ;bv++) {
            afw_list_get_next_internal(
                list, &iterator, NULL, (const void **)&val, xctx);
            if (!val) {
                break;
            }
            s = afw_dateType_internal_to_generalized_time(val, p, xctx);
            *bv = afw_pool_malloc_type(p, struct berval, xctx);
            (*bv)->bv_len = (ber_len_t)s->len;
            (*bv)->bv_val = (char *)s->s;
        }
    }

    /* If value is not dateTime, treat it like string. Let ldap deal with it. */
    else {
        result = impl_syntax_handler_list_string_to_ber(
            attribute_type, data, value, p, xctx);
    }

    /* Return result. */
    return result;
}


static const afw_value_t *
impl_syntax_handler_list_integer_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_list_t *list;
    afw_integer_t *e;
    afw_utf8_t s;
    int count;

    count = ldap_count_values_len(bv);
    if (count == 0) {
        return afw_data_type_integer->empty_list_value;
    }
    e = afw_pool_malloc(p, sizeof(afw_integer_t) * count, xctx);
    list = afw_list_create_wrapper_for_array(
        e, false, afw_data_type_integer, count, p, xctx);
    result = afw_value_create_list(list, p, xctx);

    for (; count > 0; count--, bv++, e++) {
        s.s = (*bv)->bv_val;
        s.len = (*bv)->bv_len;
        *e = afw_number_utf8_to_integer(&s, p, xctx);
    }

    return result;
}


static struct berval **
impl_syntax_handler_list_integer_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* String will do this correctly. */
    return impl_syntax_handler_list_string_to_ber(
        attribute_type, data, value, p, xctx);
}


static const afw_value_t *
impl_syntax_handler_list_string_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *s;
    const afw_value_t *result;
    const afw_list_t *list;
    afw_utf8_t *e;
    int count;

    count = ldap_count_values_len(bv);
    if (count == 0) {
        return afw_data_type_string->empty_list_value;
    }

    e = afw_pool_malloc(p, sizeof(afw_utf8_t) * count, xctx);
    list = afw_list_create_wrapper_for_array(
        e, false, afw_data_type_string, count, p, xctx);
    result = afw_value_create_list(list, p, xctx);

    for (; count > 0; count--, bv++, e++) {
        s = afw_utf8_create_copy((*bv)->bv_val, (*bv)->bv_len, p, xctx);
        e->s = s->s;
        e->len = s->len;
    }

    return (const afw_value_t *)result;
}


static struct berval **
impl_syntax_handler_list_string_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    struct berval **result;
    struct berval **bv;
    const afw_utf8_t *s;
    const afw_iterator_t *iterator;
    const afw_list_t *list;
    const afw_data_type_t *data_type;
    const void *internal;
    afw_size_t count;

    /* Get the list of values. */
    if (afw_value_is_list(value)) {
        list = ((const afw_value_list_t *)value)->internal;
        data_type = afw_list_get_data_type(list, xctx);
    }
    else {
        data_type = afw_value_get_data_type(value, xctx);
        list = afw_list_create_wrapper_for_array(
            AFW_VALUE_INTERNAL(value), false, data_type, 1, p, xctx);
    }

    /* Get memory for result. */
    count = afw_list_get_count(list, xctx);
    result = afw_pool_malloc(p, sizeof(struct berval *) * (count + 1),
        xctx);
    result[count] = NULL;

    /* Iterator setting berval. */
    for (iterator = NULL, bv = result; ;bv++) {
        afw_list_get_next_internal(
            list, &iterator, NULL, &internal, xctx);
        if (!internal) {
            break;
        }
        if (data_type == afw_data_type_string) {
            s = (const afw_utf8_t *)internal;
        }
        else {
            s = afw_data_type_internal_to_utf8(data_type, internal, p, xctx);
        }
        (*bv) = afw_pool_malloc_type(p, struct berval, xctx);
        (*bv)->bv_len = (ber_len_t)s->len;
        (*bv)->bv_val = (char *)s->s;
    }

    /* Return result. */
    return result;
}


static const afw_value_t *
impl_syntax_handler_list_unknown_to_value(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    int count;
    struct berval * *save_bv;
    afw_boolean_t valid_utf8;

    count = ldap_count_values_len(bv);
    save_bv = bv;
    valid_utf8 = true;
    bv = save_bv;

    for (; count > 0; count--, bv++) {
        if (!afw_utf8_is_valid((*bv)->bv_val, (*bv)->bv_len, xctx)) {
            valid_utf8 = false;
            break;
        }
    }

    return (valid_utf8)
        ? impl_syntax_handler_list_string_to_value(
            attribute_type, data, save_bv, p, xctx)
        : impl_syntax_handler_list_binary_to_value(
            attribute_type, data, save_bv, p, xctx);
}


const afw_value_t *
afw_ldap_internal_syntax_handler_unknown_to_value(
    struct berval * *bv,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    int count;
    struct berval * *save_bv;
    afw_boolean_t valid_utf8;
    const afw_value_t *result;
    afw_boolean_t single;

    count = ldap_count_values_len(bv);
    single = count == 1;
    save_bv = bv;
    valid_utf8 = true;
    bv = save_bv;

    for (; count > 0; count--, bv++) {
        if (!afw_utf8_is_valid((*bv)->bv_val, (*bv)->bv_len, xctx)) {
            valid_utf8 = false;
            break;
        }
    }

    if (single) {
        result = (valid_utf8)
            ? impl_syntax_handler_single_string_to_value(
                NULL, NULL, save_bv, p, xctx)
            : impl_syntax_handler_single_binary_to_value(
                NULL, NULL, save_bv, p, xctx);
    }
    else {
        result = (valid_utf8)
            ? impl_syntax_handler_list_string_to_value(
                NULL, NULL, save_bv, p, xctx)
            : impl_syntax_handler_list_binary_to_value(
                NULL, NULL, save_bv, p, xctx);
    }
    return result;
}


static struct berval **
impl_syntax_handler_list_unknown_to_ber(
    const afw_ldap_metadata_attribute_type_t *attribute_type,
    void *data, const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* Bag binary will handle afw_utf8_t and afw_memory_t. */
    return impl_syntax_handler_list_binary_to_ber(
        attribute_type, data, value, p, xctx);
}


static void
impl_x_nds_syntax_handler(
    const afw_ldap_metadata_syntax_handler_t * *handler,
    const afw_value_t * *data_type_id_value,
    const afw_object_t *attribute_type_object,
    const afw_utf8_t *syntax_id, afw_boolean_t is_single, 
    afw_xctx_t *xctx)
{
    afw_integer_t id;

    /* Reference NDK: Novell eDirectory Core Services > Values > Syntax IDs */
    id = afw_number_utf8_to_integer(syntax_id, xctx->p, xctx);

    *data_type_id_value = NULL;

    switch (id) {

    /* Used: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 21 22 24 25 29 */

    case 0: /* Binary string. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 1: /* names of objects in the eDirectory tree */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 2: /* Case sensitive utf-8 */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 3: /* Case insensitive utf-8 */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 4: /* Printable strings as defined in CCITT X.208. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 5: /* Printable numeric strings as defined in CCITT X.208 */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 6: /* List of case insensitive utf-8 strings. */
        *handler = &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 7: /* Boolean. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_boolean
            : &impl_syntax_handler_list_boolean;
        *data_type_id_value = afw_data_type_boolean_id_value;
        break;

    case 8: /* Signed integer. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_integer
            : &impl_syntax_handler_list_integer;
        *data_type_id_value = afw_data_type_integer_id_value;
        break;

    case 9: /* Binary string. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 10: /* Telephone number. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 11: /* Fax Number. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 12: /* Network-layer addresses in binary format */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 13: /* Ordered list of octet strings. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 14: /* Email address binary. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 15: /* File system path. (might be utf8 so use unknown) */
        *handler = (is_single)
            ? &impl_syntax_handler_single_unknown
            : &impl_syntax_handler_list_unknown;
        break;

    case 16: /* Used by Replica attributes. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;
     
    case 17: /* ACL Attribute. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 18: /* Unicode strings of postal addresses. */
        *handler = &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 19: /* Mark time when a particular event occurs. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 20: /* eDirectory object class name. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_string
            : &impl_syntax_handler_list_string;
        *data_type_id_value = afw_data_type_string_id_value;
        break;

    case 21: /* Binary data. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 22: /* Counter. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_integer
            : &impl_syntax_handler_list_integer;
        *data_type_id_value = afw_data_type_integer_id_value;
        break;

    case 23: /* Back link. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 24: /* Unsigned integer that is time in seconds. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_generalized_time
            : &impl_syntax_handler_list_generalized_time;
        *data_type_id_value = afw_data_type_dateTime_id_value;
        break;

    case 25: /* Level and an interval associated with an object name. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        *data_type_id_value = afw_data_type_base64Binary_id_value;
        break;

    case 26: /* Hold which is a signed integer. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_integer
            : &impl_syntax_handler_list_integer;
        *data_type_id_value = afw_data_type_integer_id_value;
        break;

    case 27: /* Interval of time. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_binary
            : &impl_syntax_handler_list_binary;
        break;

    case 29: /* Large integer. */
        *handler = (is_single)
            ? &impl_syntax_handler_single_integer
            : &impl_syntax_handler_list_integer;
        *data_type_id_value = afw_data_type_integer_id_value;
        break;

    default:
        *handler = (is_single)
            ? &impl_syntax_handler_single_unknown
            : &impl_syntax_handler_list_unknown;
    }

    return;
}


void
afw_ldap_internal_syntax_handler_assign(
    afw_ldap_metadata_t *metadata,
    afw_ldap_metadata_attribute_type_t *attribute_type,
    afw_xctx_t *xctx)
{
    const afw_object_t *ldap_syntax_object;
    const afw_utf8_t *x_nds_syntax;

    /* Determine syntax_handler based on syntax. */
    ldap_syntax_object = apr_hash_get(metadata->ldap_syntax_objects,
        attribute_type->syntax_oid->s,
        attribute_type->syntax_oid->len);
    if (ldap_syntax_object) {

        /* If there is an X-NDS_SYNTAX, assign syntax_handler based on it. */
        x_nds_syntax = afw_object_old_get_property_as_string(
            ldap_syntax_object,
            &afw_ldap_s_a_X_NDS_SYNTAX, xctx);
        if (x_nds_syntax) {
            impl_x_nds_syntax_handler(
                &attribute_type->syntax_handler,
                &attribute_type->data_type_id_value,
                attribute_type->attribute_type_object,
                x_nds_syntax,
                attribute_type->is_single, xctx);
        }

        /** @fixme Add support for normal LDAP, probably using oid lookup. */
        else {
            attribute_type->syntax_handler = (attribute_type->is_single)
                ? &impl_syntax_handler_single_unknown
                : &impl_syntax_handler_list_unknown;
        }
    }

    /*
     * There are a number of syntaxes like 2.16.840.1.113719.1.1.5.1.0 that
     * are not defined and considered unknown.
     */
    else {
        attribute_type->syntax_handler = (attribute_type->is_single)
            ? &impl_syntax_handler_single_unknown
            : &impl_syntax_handler_list_unknown;
        attribute_type->never_allow_read = true;
        attribute_type->never_allow_write = true;
    }
}

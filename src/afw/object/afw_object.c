// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework interface helpers for afw_object*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object.c
 * @brief Interface helpers for afw_object.
 */

#include "afw_internal.h"


/* Set an object to immutable if it is not already. */
AFW_DEFINE(void)
afw_object_set_immutable(
    const afw_object_t *instance, afw_xctx_t *xctx)
{
    const afw_object_setter_t *setter;

    setter = afw_object_get_setter(instance, xctx);

    /* If already immutable, ignore, otherwise; set immutable. */
    if (setter) {
        afw_object_setter_set_immutable(setter, xctx);
    }
}


/* Remove a property from object. */
AFW_DEFINE(void)
afw_object_remove_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_object_set_property(instance, property_name, NULL, xctx);
}


/* Set the value of an object's property. */
AFW_DEFINE(void)
afw_object_set_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_object_setter_t *setter;

    setter = afw_object_get_setter(instance, xctx);

    if (!setter) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    afw_object_setter_set_property(setter, property_name, value, xctx);
}


/* Set a date property from parts. */
AFW_DECLARE(void)
afw_object_set_property_as_date_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    int year,
    int month,
    int day,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx)
{
    afw_value_date_t *value;

    value = afw_value_allocate_date(instance->p, xctx);
    afw_date_set_from_parts(
        &value->internal,
        year, month, day,
        tz_hours_offset, tz_minutes_offset,
        xctx);
    afw_object_set_property(instance, property_name,
        (const afw_value_t *)value, xctx);
}


/* Set a dateTime property from parts. */
AFW_DECLARE(void)
afw_object_set_property_as_dateTime_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx)
{
    afw_value_dateTime_t *value;

    value = afw_value_allocate_dateTime(instance->p, xctx);
    afw_dateTime_set_from_parts(
        &value->internal,
        year, month, day,
        hour, minute, second, microsecond,
        tz_hours_offset, tz_minutes_offset,
        xctx);
    afw_object_set_property(instance, property_name,
        (const afw_value_t *)value, xctx);
}


/*  Set a dayTimeDuration property from parts. */
AFW_DEFINE(void)
afw_object_set_property_as_dayTimeDuration_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_boolean_t is_positive,
    int days,
    int hours,
    int minutes,
    int seconds,
    int microseconds,
    afw_xctx_t *xctx)
{
    afw_value_dayTimeDuration_t *value;

    value = afw_value_allocate_dayTimeDuration(instance->p, xctx);
    afw_dayTimeDuration_set_from_parts(
        &value->internal,
        is_positive,
        days, hours, minutes, seconds, microseconds, xctx);
    afw_object_set_property(instance, property_name,
        (const afw_value_t *)value, xctx);
}


/* Set a time property from parts. */
AFW_DEFINE(void)
afw_object_set_property_as_time_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx)
{
    afw_value_time_t *value;

    value = afw_value_allocate_time(instance->p, xctx);
    afw_time_set_from_parts(
        &value->internal,
        hour, minute, second, microsecond,
        tz_hours_offset, tz_minutes_offset,
        xctx);
    afw_object_set_property(instance, property_name,
        (const afw_value_t *)value, xctx);
}


/* Set a yearMonthDuration property from parts. */
AFW_DEFINE(void)
afw_object_set_property_as_yearMonthDuration_from_parts(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_boolean_t is_positive,
    int years,
    int months,
    afw_xctx_t *xctx)
{
    afw_value_yearMonthDuration_t *value;

    value = afw_value_allocate_yearMonthDuration(instance->p, xctx);
    afw_yearMonthDuration_set_from_parts(
        &value->internal, is_positive,
        years, months, 
        xctx);
    afw_object_set_property(instance, property_name,
        (const afw_value_t *)value, xctx);
}


/* Set an afw_value_string_t property from utf8_z. */
AFW_DEFINE(void)
afw_object_set_property_as_string_from_utf8_z(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_z_t *string_z,
    afw_xctx_t *xctx)
{
    afw_value_string_t *string;

    string = afw_value_allocate_string(instance->p, xctx);
    string->internal.s = string_z;
    string->internal.len = strlen(string_z);

    afw_object_set_property(instance,
        property_name, (const afw_value_t *)string,
        xctx);
}


/* Get a property in an object's lineage. */
AFW_DEFINE(const afw_value_t *)
afw_object_get_property_extended(
    const afw_object_t *instance,
    const afw_utf8_t *property_name_extended,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_utf8_t *meta_value;
    afw_utf8_t after_dot;
    const afw_value_t *result;
    afw_utf8_t meta_pn;
    afw_utf8_t pn;

    /** @fixme Correct? Return NULL if instance or property_name is missing. */
    if (!instance ||
        !property_name_extended ||
        property_name_extended->len == 0)
    {
        return NULL;
    }

    /* Return property from object passed or an ancestor. */
    obj = instance;
    result = NULL;

    /* If the property name starts with _meta_, process accordingly. */
    if (afw_utf8_starts_with_z(property_name_extended,
        AFW_OBJECT_Q_PN_META  "."))
    {
        meta_value = NULL;
        afw_utf8_substring_byte(&meta_pn, property_name_extended,
            strlen(AFW_OBJECT_Q_PN_META  "."), property_name_extended->len);
        if (afw_utf8_equal_utf8_z(&meta_pn, "objectId")) {
            meta_value = afw_object_meta_get_object_id(instance, xctx);
        }
        else if (afw_utf8_equal_utf8_z(&meta_pn, "objectType")) {
            meta_value = afw_object_meta_get_object_type_id(instance, xctx);
        }
        else if (afw_utf8_equal_utf8_z(&meta_pn, "path")) {
            meta_value = afw_object_meta_get_path(instance, xctx);
        }
        /** @fixme make a list.
        else if (afw_utf8_equal_utf8_z(&meta_pn, "parentPaths")) {
            meta_value = 
                afw_object_get_parent_paths(instance, xctx);
        };
         */
        if (meta_value) {
            result = afw_value_create_string(meta_value, xctx->p, xctx);
        }
        return result;
    }

    /* Check for dotted property name. */
    pn.s = property_name_extended->s;
    pn.len = property_name_extended->len;

    for (after_dot.s = property_name_extended->s,
        after_dot.len = property_name_extended->len;
        after_dot.len > 0;
        (after_dot.s)++, (after_dot.len)--)
    {
        /** @fixme Property names like "xyz." and "xyz..x get through. */
        if (*after_dot.s == '.') {
            pn.len -= after_dot.len;
            (after_dot.s)++;
            (after_dot.len)--;
            break;
        }
    }


    /* Attempt to get property and return if error or found. */
    result = afw_object_get_property(obj, &pn, xctx);
    if (result) {

        /* If dotted name, process rest of name if it's object. */
        if (after_dot.len > 0) {
            if (afw_value_is_object(result)) {
                result = afw_object_get_property_extended(
                    ((afw_value_object_t *)result)->internal,
                    &after_dot, xctx);
            }
        }
    }

    /* Return result. */
    return result;
}


/*
 * Return a NULL terminated list of values for an object property in a
 * specified pool.
 */
AFW_DEFINE(const afw_value_t * const *)
afw_object_old_get_property_as_array_of_values(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_object_get_property(instance, property_name, xctx);

    return afw_value_as_array_of_values(value, p, xctx);
}


/*
 * Return a NULL terminated list of strings for an object property in a
 * specified pool.
 */
AFW_DEFINE(const afw_utf8_t * const *)
afw_object_old_get_property_as_array_of_strings(
    const afw_object_t *instance, const afw_utf8_t *property_name,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_object_get_property(instance, property_name, xctx);

    return afw_value_as_array_of_utf8(value, p, xctx);;
}



/* Return a compiled hybrid property value. */
AFW_DEFINE(const afw_value_t *)
afw_object_old_get_property_as_compiled_hybrid(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;

    /* Get value. Return NULL if it isn't present. */
    result = afw_object_get_property(instance, property_name, xctx);
    if (!result) {
        return NULL;
    }

    /* Only evaluated values are supported. */
    if (!afw_value_is_defined_and_evaluated(result)) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "%" AFW_UTF8_FMT " %" AFW_UTF8_FMT
            " is not an evaluated value",
            AFW_UTF8_FMT_OPTIONAL_ARG(source_location),
            AFW_UTF8_FMT_ARG(property_name));
    }

    result = afw_compile_hybrid(result,
        source_location, NULL, shared, p, xctx);

    return result;
}



/* Get an object's property value including ancestors as a boolean. */
AFW_DEFINE(afw_boolean_t)
afw_object_old_get_property_as_boolean_deprecated(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;
    const afw_value_t *value;
    afw_boolean_t result;

    result = AFW_FALSE;
    value = afw_object_get_property(instance, property_name, xctx);
    if (value) {
        value = afw_value_evaluate(value, xctx->p, xctx);
        if (afw_value_is_boolean(value)) {
            result = ((const afw_value_boolean_t *)value)->internal;
        }
        else {
            string = afw_value_as_utf8(value, xctx->p, xctx);

            if (
                afw_utf8_compare_ignore_case(string, &afw_s_true, xctx) == 0 ||
                (string->len == 1 &&
                    (
                    *string->s == 't' ||
                    *string->s == 'T' ||
                    *string->s == '1')
                    )
                )
            {
                result = AFW_TRUE;
            }

            else if (
                afw_utf8_compare_ignore_case(string, &afw_s_false, xctx)== 0 ||
                (string->len == 1 &&
                    (
                    *string->s == 'f' ||
                    *string->s == 'F' ||
                    *string->s == '0')
                    )
                )
            {
                result = AFW_FALSE;
            }

            else
            {
                AFW_THROW_ERROR_Z(general, "Boolean must be true or false",
                    xctx);
            }
        }
    }

    return result;
}


/* Get an object's property value as an integer. */
AFW_DEFINE(afw_integer_t)
afw_object_old_get_property_as_integer_deprecated(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_boolean_t *found,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    afw_integer_t result = 0;

    *found = false;
    value = afw_object_get_property(instance, property_name, xctx);
    if (value) {
        if (afw_value_is_integer(value)) {
            *found = true;
            result = ((afw_value_integer_t *)value)->internal;
        }
        else {
            value = afw_value_convert(value,
                afw_data_type_integer, false, xctx->p, xctx);
            if (value) {
                result = ((afw_value_integer_t *)value)->internal;
                *found = true;
            }
        }
    }

    return result;
}



/*
 * Get an object's property value including ancestors as a string in specified
 * pool.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_object_old_get_property_as_utf8(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_t *string;

    string = NULL;
    value = afw_object_get_property(instance, property_name, xctx);
    if (value) {
        value = afw_value_evaluate(value, p, xctx);
        string = afw_value_as_utf8(value, p, xctx);
    }
    return string;
}


/*
 * Get an object's property value including ancestors as a utf8_z in specified
 * pool.
 */
AFW_DEFINE(const afw_utf8_z_t *)
afw_object_old_get_property_as_utf8_z(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_z_t *utf8_z;

    utf8_z = NULL;
    value = afw_object_get_property(instance, property_name, xctx);
    if (value) {
        utf8_z = afw_value_as_utf8_z(value, p, xctx);
    }
    return utf8_z;
}



/* Merge properties from one object into another. */
AFW_DEFINE(void)
afw_object_merge(
    const afw_object_t *instance,
    const afw_object_t *from,
    afw_boolean_t replace,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_value_t *from_value;
    const afw_utf8_t *property_name;
    const afw_object_t *embedded_object;

    iterator = NULL;
    while ( (from_value = afw_object_get_next_property(
        from, &iterator, &property_name, xctx)) )
    {
        value = afw_object_get_property(instance, property_name, xctx);
        if (!value)
        {
            if (afw_value_is_object(from_value)) {
                embedded_object = afw_object_create_embedded(instance,
                    property_name, xctx);
                afw_object_merge(embedded_object,
                    ((const afw_value_object_t *)from_value)->internal,
                    replace, xctx);
            } else {
                afw_object_set_property(instance, property_name, from_value, xctx);
            }
        }

        else if (afw_value_is_object(from_value) &&
            afw_value_is_object(value))
        {
            afw_object_merge(
                ((const afw_value_object_t *)value)->internal,
                ((const afw_value_object_t *)from_value)->internal,
                replace, xctx);
        }
        else if (replace)
        {
            afw_object_set_property(instance, property_name, from_value, xctx);
        }
    }
}


/* Count the number of properties in an object. */
AFW_DEFINE(afw_size_t)
afw_object_property_count(
    const afw_object_t *object,
    afw_xctx_t *xctx)
{
    afw_size_t result;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;

    iterator = NULL;
    result = 0;
    while (afw_object_get_next_property(object,
        &iterator, &property_name, xctx))
    {
        result++;
    }

    return result;
}

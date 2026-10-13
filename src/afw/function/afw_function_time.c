// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for time
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_time.c
 * @brief Adaptive function execute implementations for category `time`.
 */

#include "afw_internal.h"



/* Microseconds into one day (0 to one day less 1 microsecond). */
static afw_integer_t
impl_time_of_day(afw_integer_t usec)
{
    afw_integer_t day;

    day = AFW_TIME_DAYS_TO_MICROSECONDS(1);
    usec %= day;
    return (usec < 0) ? usec + day : usec;
}


/*
 * Adaptive function: in_range<time>
 *
 * afw_function_execute_in_range_time
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks if time is between startTime and endTime, inclusive. Regardless of
 * endTime value, it is always considered to be equal to, but less than 24 hours
 * greater than startTime. If no time zone is specified for time, the default
 * time zone is used. If no time zone is specified for startTime or endTime, the
 * time zone of time is used.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function in_range<time>(
 *       time: time,
 *       startTime: time,
 *       endTime: time
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   time - (time)
 *
 *   startTime - (time)
 *
 *   endTime - (time)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_in_range_time(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_time_t *arg1;
    const afw_value_time_t *arg2;
    const afw_value_time_t *arg3;
    afw_time_t time1, time2, time3;
    afw_integer_t usec1, usec2, usec3;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, time);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, time);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg3, 3, time);

    /* Make working copy of times. */
    memcpy(&time1, &arg1->internal, sizeof(time1));
    memcpy(&time2, &arg2->internal, sizeof(time2));
    memcpy(&time3, &arg3->internal, sizeof(time3));

    /* If there is no arg1 time zone, use scopes local time zone. */
    if (AFW_TIME_ZONE_IS_NONE(&time1.time_zone)) {
        time1.time_zone.hours =
            xctx->local_dateTime_when_created.time_zone.hours;
        time1.time_zone.minutes =
            xctx->local_dateTime_when_created.time_zone.minutes;
    }

    /* If there is no arg2 time zone, use arg1's time zone. */
    if (AFW_TIME_ZONE_IS_NONE(&time2.time_zone)) {
        memcpy(&time2.time_zone, &time1.time_zone, sizeof(time2.time_zone));
    }

    /* If there is no arg3 time zone, use arg1's time zone. */
    if (AFW_TIME_ZONE_IS_NONE(&time3.time_zone)) {
        memcpy(&time3.time_zone, &time1.time_zone, sizeof(time3.time_zone));
    }

    /* Convert times to microseconds for compare. */
    usec1 = afw_time_to_microseconds_utc(&time1, xctx);
    usec2 = afw_time_to_microseconds_utc(&time2, xctx);
    usec3 = afw_time_to_microseconds_utc(&time3, xctx);

    /*
     * Times in UTC can be before 00:00 or after 24:00 (04:00:00+05:00 is
     * 23:00 the day before), so bring each into one day first.
     */
    usec1 = impl_time_of_day(usec1);
    usec2 = impl_time_of_day(usec2);
    usec3 = impl_time_of_day(usec3);

    /* If endTime is before startTime, the range wraps past midnight. */
    if (usec2 <= usec3) {
        return (usec1 >= usec2 && usec1 <= usec3)
            ? afw_boolean_v_true : afw_boolean_v_false;
    }
    return (usec1 >= usec2 || usec1 <= usec3)
        ? afw_boolean_v_true : afw_boolean_v_false;
}

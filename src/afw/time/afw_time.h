// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Time Support Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_TIME_H__
#define __AFW_TIME_H__

#include "afw_interface.h"

/**
 * @defgroup afw_time Time
 * @ingroup afw_c_api_public
 *
 * Time support.
 * 
 * @{
 */

/**
 * @file afw_time.h
 * @brief Header for Adaptive Framework Time Support. 
 */

AFW_BEGIN_DECLARES

#define AFW_TIME_SECONDS_TO_MICROSECONDS(SECONDS) \
    (afw_integer_t)(SECONDS) * (afw_integer_t)1000000

#define AFW_TIME_MINUTES_TO_MICROSECONDS(MINUTES) \
    AFW_TIME_SECONDS_TO_MICROSECONDS( \
        (afw_integer_t)(MINUTES) * (afw_integer_t)60)

#define AFW_TIME_HOURS_TO_MICROSECONDS(HOURS) \
    AFW_TIME_MINUTES_TO_MICROSECONDS( \
        (afw_integer_t)(HOURS) * (afw_integer_t)60)

#define AFW_TIME_DAYS_TO_MICROSECONDS(DAYS) \
    AFW_TIME_HOURS_TO_MICROSECONDS( \
        (afw_integer_t)(DAYS) * (afw_integer_t)24)


/**
 * @brief Set afw_date_t from parts.
 * @param date is pointer to preallocated afw_date_t.
 * @param year including century. Negative is BC. No 0.
 * @param month (1-12).
 * @param day of month (1-31).  Month limited to days in month..
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_date_set_from_parts(
    afw_date_t *date,
    int year,
    int month,
    int day,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx);


/**
 * @brief Create afw_date_t from parts in specific pool.
 * @param date is pointer to preallocated afw_date_t.
 * @param year including century. Negative is BC. No 0.
 * @param month (1-12).
 * @param day of month (1-31).  Month limited to days in month..
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param p to use for result.
 * @param xctx of caller.
 * @return afw_date_t struct.
 */
AFW_DECLARE(const afw_date_t *)
afw_date_create_from_parts(
    int year,
    int month,
    int day,
    int tz_hours_offset,
    int tz_minutes_offset,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set afw_dateTime_t from apr_time.
 * @param dateTime is pointer to preallocated afw_dateTime_t.
 * @param apr_time
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_set_from_apr_time(
    afw_dateTime_t *dateTime,
    apr_time_t apr_time,
    afw_xctx_t *xctx);



/**
 * @brief Create afw_dateTime_t from apr time.
 * @param apr_time
 * @param p to use for result.
 * @param xctx of caller.
 * @return afw_dateTime_t struct.
 */
AFW_DECLARE(const afw_dateTime_t *)
afw_dateTime_create_from_apr_time(
    apr_time_t apr_time,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set afw_dateTime_t from parts.
 * @param dateTime is pointer to preallocated afw_dateTime_t.
 * @param year including century. Negative is BC. No 0.
 * @param month (1-12).
 * @param day of month (1-31).  Month limited to days in month..
 * @param hour (0-24).  If hour is 24 then other values must be 0.
 * @param minute (0-59).
 * @param second (0-60+). 60 or more is allowed for leap seconds.
 * @param microsecond (0-999999).
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_set_from_parts(
    afw_dateTime_t *dateTime,
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx);


/**
 * @brief Create afw_dateTime_t from parts in specific pool.
 * @param dateTime is pointer to preallocated afw_dateTime_t.
 * @param year including century. Negative is BC. No 0.
 * @param month (1-12).
 * @param day of month (1-31).  Month limited to days in month..
 * @param hour (0-24).  If hour is 24 then other values must be 0.
 * @param minute (0-59).
 * @param second (0-60+). 60 or more is allowed for leap seconds.
 * @param microsecond (0-999999).
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param p to use for result.
 * @param xctx of caller.
 * @return afw_dateTime_t struct.
 */
AFW_DECLARE(const afw_dateTime_t *)
afw_dateTime_create_from_parts(
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set afw_dayTimeDuration_t from parts.
 * @param dayTimeDuration is pointer to preallocated afw_dayTimeDuration_t.
 * @param is_positive true if duration is positive or false if negative.
 * @param days (must be between 0 and AFW_INT32_MAX).
 * @param hours (must be between 0 and AFW_INT32_MAX).
 * @param minutes (must be between 0 and AFW_INT32_MAX).
 * @param seconds (must be between 0 and AFW_INT32_MAX).
 * @param microseconds (must be between 0 and AFW_INT32_MAX).
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dayTimeDuration_set_from_parts(
    afw_dayTimeDuration_t *dayTimeDuration,
    afw_boolean_t is_positive,
    int days,
    int hours,
    int minutes,
    int seconds,
    int microseconds,
    afw_xctx_t *xctx)
;


/**
 * @brief Create afw_dayTimeDuration_t from parts in specific pool.
 * @param is_positive true if duration is positive or false if negative.
 * @param days (must be positive).
 * @param hours (must be positive).
 * @param minutes (must be positive).
 * @param seconds (must be positive).
 * @param microseconds (must be positive).
 * @param p to use for result.
 * @param xctx of caller.
 * @return afw_dayTimeDuration_t struct.
 */
AFW_DECLARE(const afw_dayTimeDuration_t *)
afw_dayTimeDuration_create_from_parts(
    afw_boolean_t is_positive,
    afw_int32_t days,
    afw_int32_t hours,
    afw_int32_t minutes,
    afw_int32_t seconds,
    afw_int32_t microseconds,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set afw_time_t from parts.
 * @param time is pointer to preallocated afw_time_t.
 * @param hour (0-24).  If hour is 24 then other values must be 0.
 * @param minute (0-59).
 * @param second (0-60+). 60 or more is allowed for leap seconds.
 * @param microsecond (0-999999).
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_time_set_from_parts(
    afw_time_t *time,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx);


/**
 * @brief Create afw_time_t from parts in specific pool.
 * @param dateTime is pointer to preallocated afw_time_t.
 * @param hour (0-24).  If hour is 24 then other values must be 0.
 * @param minute (0-59).
 * @param second (0-60+). 60 or more is allowed for leap seconds.
 * @param microsecond (0-999999).
 * @param tz_hours_offset Hours (-14 - +14).  If +14 or -14, minutes must be 0.
 * @param tz_minutes_offset Minutes (0 - 59 or -1).  -1 indicates no time zone,
 *    in which case, hours must be 0.
 * @param p to use for result.
 * @param xctx of caller.
 * @return afw_time_t struct.
 */
AFW_DECLARE(const afw_time_t *)
afw_time_create_from_parts(
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set afw_yearMonthDuration_t from parts.
 * @param yearMonthDuration is pointer to preallocated afw_yearMonthDuration_t.
 * @param is_positive true if duration is positive or false if negative.
 * @param years (must be between 0 and AFW_INT32_MAX).
 * @param months (must be between 0 and AFW_INT32_MAX).
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_yearMonthDuration_set_from_parts(
    afw_yearMonthDuration_t *yearMonthDuration,
    afw_boolean_t is_positive,
    int years,
    int months,
    afw_xctx_t *xctx);


/**
 * @brief Create afw_yearMonthDuration_t from parts in specific pool.
 * @param is_positive true if duration is positive or false if negative.
 * @param years (must be between 0 and AFW_INT32_MAX).
 * @param months (must be between 0 and AFW_INT32_MAX).
 * @param p to use for result.
 * @param xctx of caller.
 * @return afw_yearMonthDuration_t struct.
 */
AFW_DECLARE(const afw_yearMonthDuration_t *)
afw_yearMonthDuration_create_from_parts(
    afw_boolean_t is_positive,
    afw_int32_t years,
    afw_int32_t months,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Set local and/or utc dateTime to now.
 * @param dateTime local pointer to place to set or NULL.
 * @param dateTime utc pointer to place to set or NULL.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_set_now(
    afw_dateTime_t *dateTime_local, afw_dateTime_t *dateTime_utc,
    afw_xctx_t *xctx);


/**
 * @brief Set local and/or utc time to now.
 * @param time local pointer to place to set or NULL.
 * @param time utc pointer to place to set or NULL.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_time_set_now(
    afw_time_t *time_local, afw_time_t *time_utc,
    afw_xctx_t *xctx);


/**
 * @brief Get now time as dateTime in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return date time
 */
AFW_DECLARE(const afw_dateTime_t *)
afw_dateTime_now_utc(
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Get now time as time only in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return time
 */
AFW_DECLARE(const afw_time_t *)
afw_time_now_utc(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Get now local time as dateTime in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return date time
 */
AFW_DECLARE(const afw_dateTime_t *)
afw_dateTime_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Get now local time as time only in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return time
 */
AFW_DECLARE(const afw_time_t *)
afw_time_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert time normalize to utc to microseconds.
 * @param time to convert.
 * @param xctx of caller.
 * @return time in microseconds
 */
AFW_DECLARE(afw_integer_t)
afw_time_to_microseconds_utc(
    const afw_time_t *time, afw_xctx_t *xctx);


/**
 * @brief Determine maximum days in a month.
 * @param year
 * @param month which can exceed 12.
 * @return maximum days in month.
 */
AFW_DECLARE(int)
afw_date_maximum_days_in_month(
    afw_integer_t year, afw_integer_t month);


/**
 * @brief Convert internal date to utf8 xml schema date in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return utf8 with converted value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_date_internal_to_utf8(
    const afw_date_t *date, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert utf8 xml schema date to internal and set.
 * @param utf8 is xml schema compliant date.
 * @param internal is place to return internal representation.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_date_utf8_set_internal(
    const afw_utf8_t *utf8, afw_date_t *internal, afw_xctx_t *xctx);


/**
 * @brief Convert internal dateTime to utf8 xml schema dateTime in specified
 *    pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return utf8 with converted value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_dateTime_internal_to_utf8(
    const afw_dateTime_t *dateTime, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert utf8 xml schema dateTime to internal and set.
 * @param utf8 is xml schema compliant date.
 * @param internal is place to return internal representation.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_utf8_set_internal(
    const afw_utf8_t *utf8, afw_dateTime_t *internal, afw_xctx_t *xctx);


/**
 * @brief Convert internal dayTimeDuration to utf8 xml schema dayTimeDuration
 *    in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return utf8 with converted value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_dayTimeDuration_internal_to_utf8(
    const afw_dayTimeDuration_t *dayTimeDuration,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert utf8 xml schema dayTimeDuration to internal and set.
 * @param utf8 is xml schema compliant dayTimeDuration.
 * @param internal is place to return internal representation.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dayTimeDuration_utf8_set_internal(
    const afw_utf8_t *utf8, afw_dayTimeDuration_t *internal, afw_xctx_t *xctx);


/**
 * @brief Convert internal time to utf8 xml schema time in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return utf8 with converted value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_time_internal_to_utf8(
    const afw_time_t *time, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert utf8 xml schema dateTime to internal and set.
 * @param utf8 is xml schema compliant date.
 * @param internal is place to return internal representation.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_time_utf8_set_internal(
    const afw_utf8_t *utf8, afw_time_t *internal, afw_xctx_t *xctx);


/**
 * @brief Convert internal yearMonthDuration to utf8 xml schema
 *     yearMonthDuration in specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return utf8 with converted value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_yearMonthDuration_internal_to_utf8(
    const afw_yearMonthDuration_t *yearMonthDuration,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert utf8 xml schema yearMonthDuration to internal and set.
 * @param utf8 is xml schema compliant yearMonthDuration.
 * @param internal is place to return internal representation.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_yearMonthDuration_utf8_set_internal(
    const afw_utf8_t *utf8, afw_yearMonthDuration_t *internal,
    afw_xctx_t *xctx);


/**
 * @brief compare two date values.
 * @param v1 is first value
 * @param v2 is second value
 * @return int >0 if v1 larger, <0 if v2 larger, 0 if they are equal.
 *
 * The time zones of both v1 and v2 must match or an error is thrown.
 */
AFW_DECLARE(int)
afw_date_compare(const afw_date_t *v1, const afw_date_t *v2,
    afw_xctx_t *xctx);


/**
 * @brief compare two dateTime values.
 * @param v1 is first value
 * @param v2 is second value
 * @return int >0 if v1 larger, <0 if v2 larger, 0 if they are equal.
 *
 * The time zones of both v1 and v2 must match or an error is thrown.
 */
AFW_DECLARE(int)
afw_dateTime_compare(const afw_dateTime_t *v1, const afw_dateTime_t *v2,
    afw_xctx_t *xctx);


/**
 * @brief compare two time values.
 * @param v1 is first value
 * @param v2 is second value
 * @return int >0 if v1 larger, <0 if v2 larger, 0 if they are equal.
 *
 * The time zones of both v1 and v2 must match or an error is thrown.
 */
AFW_DECLARE(int)
afw_time_compare(const afw_time_t *v1, const afw_time_t *v2,
    afw_xctx_t *xctx);


/**
 * @brief compare two dateTime values.
 * @param v1 is first value
 * @param v2 is second value
 * @return int >0 if v1 larger, <0 if v2 larger, 0 if they are equal.
 *
 * The time zones of both v1 and v2 must match or an error is thrown.
 */
AFW_DECLARE(int)
afw_dayTimeDuration_compare(const afw_dayTimeDuration_t *v1,
    const afw_dayTimeDuration_t *v2, afw_xctx_t *xctx);


/**
 * @brief compare two yearMonthDuration values.
 * @param v1 is first value
 * @param v2 is second value
 * @return int >0 if v1 larger, <0 if v2 larger, 0 if they are equal.
 */
AFW_DECLARE(int)
afw_yearMonthDuration_compare(const afw_yearMonthDuration_t *v1,
    const afw_yearMonthDuration_t *v2, afw_xctx_t *xctx);


/**
 * @brief Add yearMonthDuration to date.
 * @param date pointer.
 * @param yearMonthDuration to add to date.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_date_add_yearMonthDuration(
    afw_date_t *date,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx);


/**
 * @brief Subtract yearMonthDuration from date.
 * @param date pointer.
 * @param yearMonthDuration to subtract from date.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_date_subtract_yearMonthDuration(
    afw_date_t *date,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx);


/**
 * @brief Add dayTimeDuration to dateTime.
 * @param dateTime pointer.
 * @param dayTimeDuration to add to dateTime.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_add_dayTimeDuration(
    afw_dateTime_t *dateTime,
    const afw_dayTimeDuration_t *dayTimeDuration,
    afw_xctx_t *xctx);


/**
 * @brief Subtract dayTimeDuration from dateTime.
 * @param dateTime pointer.
 * @param dayTimeDuration to subtract from dateTime.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_subtract_dayTimeDuration(
    afw_dateTime_t *dateTime,
    const afw_dayTimeDuration_t *dayTimeDuration,
    afw_xctx_t *xctx);


/**
 * @brief Add yearMonthDuration to dateTime.
 * @param dateTime pointer.
 * @param yearMonthDuration to add to dateTime.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_add_yearMonthDuration(
    afw_dateTime_t *dateTime,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx);


/**
 * @brief Subtract yearMonthDuration from dateTime.
 * @param dateTime pointer.
 * @param yearMonthDuration to subtract from dateTime.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dateTime_subtract_yearMonthDuration(
    afw_dateTime_t *dateTime,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx);


/**
 * @brief Convert internal dateTime to generalized time (see X.208) in
 *    specified pool.
 * @param p pool to use for result.
 * @param xctx of caller.
 * @return utf8 with converted value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_dateType_internal_to_generalized_time(
    const afw_dateTime_t *dateTime,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Convert generalized time (see X.208) to internal and set.
 * @param generalized_time (see X.208).
 * @param internal is place to return internal representation.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_dataType_generalized_time_set_internal(
    const afw_utf8_t *generalized_time, afw_dateTime_t *internal,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_TIME_H__ */

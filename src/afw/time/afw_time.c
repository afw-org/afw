// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Time Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_time.c
 * @brief Adaptive Framework Time Support. 
 */

#include "afw_internal.h"

#define IMPL_IS_DIGIT(n) ((n) >= '0' && (n) <= '9')

#define IMPL_SAFE_DIGIT(c, to, error) \
do { \
    to = c - '0'; \
    if (to < 0 || to > 9) goto error; \
} while (0)

#define IMPL_MAX_TIME_ZONE_LEN sizeof("-01:00") - 1

#define IMPL_MAX_DATE_SEPARATE_LEN sizeof("-2147483648-01-01") - 1

#define IMPL_MAX_DATE_LEN IMPL_MAX_DATE_SEPARATE_LEN + IMPL_MAX_TIME_ZONE_LEN

#define IMPL_MAX_TIME_SEPARATE_LEN sizeof("12:34:56.000000") - 1

#define IMPL_MAX_TIME_LEN IMPL_MAX_TIME_SEPARATE_LEN + IMPL_MAX_TIME_ZONE_LEN

#define IMPL_MAX_DATE_TIME_LEN \
    IMPL_MAX_DATE_SEPARATE_LEN + 1 + \
    IMPL_MAX_TIME_SEPARATE_LEN + \
    IMPL_MAX_TIME_ZONE_LEN


/* Get now time as dateTime in specified pool. */
AFW_DEFINE_STATIC_INLINE(void)
impl_dateTime_tm_set(
    afw_dateTime_t *dateTime, apr_time_exp_t *tm, afw_xctx_t *xctx)
{
    /* Make sure any sluff is cleared. */
    memset(dateTime, 0, sizeof(*dateTime));

    /* Fill in dateTime. */
    dateTime->date.year = 1900 + tm->tm_year;
    dateTime->date.month = tm->tm_mon + 1;
    dateTime->date.day = tm->tm_mday;
    dateTime->time.hour = tm->tm_hour;
    dateTime->time.minute = tm->tm_min;
    dateTime->time.second = tm->tm_sec;
    dateTime->time.microsecond = tm->tm_usec;
    dateTime->time_zone.hours = tm->tm_gmtoff / 3600;
    dateTime->time_zone.minutes = abs(tm->tm_gmtoff / 60 % 60);
}


/* Get now time as time only in specified pool. */
AFW_DEFINE_STATIC_INLINE(void)
impl_time_tm_set(
    afw_time_t *time, apr_time_exp_t *tm, afw_xctx_t *xctx)
{
    /* Make sure any sluff is cleared. */
    memset(time, 0, sizeof(*time));

    /* Fill in time. */
    time->time.hour = tm->tm_hour;
    time->time.minute = tm->tm_min;
    time->time.second = tm->tm_sec;
    time->time.microsecond = tm->tm_usec;
    time->time_zone.hours = tm->tm_gmtoff / 3600;
    time->time_zone.minutes = abs(tm->tm_gmtoff / 60 % 60);
}


AFW_DEFINE_STATIC_INLINE(void)
impl_set_date_no_time_zone(
    afw_date_no_time_zone_t *date,
    int year,
    int month,
    int day,
    afw_xctx_t *xctx)
{
    if (year < 0 AFW_INT32_MIN || year > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "year must be between "
            AFW_STRINGIFY(AFW_INT32_MIN)
            " and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (month < 1 || month > 12) {
        AFW_THROW_ERROR_Z(general,
            "month must be between 1 and 12",
            xctx);
    }

    /** @fixme check for leapyear. */
    if (day < 1 || day > 31) {
        AFW_THROW_ERROR_Z(general,
            "day must be between 1 and 31",
            xctx);
    }

    date->year = (afw_int32_t)year;
    date->month = (afw_uint8_t)month;
    date->day = (afw_uint8_t)day;
}


AFW_DEFINE_STATIC_INLINE(void)
impl_set_time_no_time_zone(
    afw_time_no_time_zone_t *time,
    int hour,
    int minute,
    int second,
    int microsecond,
    afw_xctx_t *xctx)

{
    if (hour < 0 || hour > 24) {
        AFW_THROW_ERROR_Z(general,
            "hour must be between 0 and 24",
            xctx);
    }

    if (hour == 24 && minute != 0 && second != 0 && microsecond != 0) {
        AFW_THROW_ERROR_Z(general,
            "if hour is 24, minute, second, and microsecond must be 0",
            xctx);
    }

    if (minute < 0 || minute > 59) {
        AFW_THROW_ERROR_Z(general,
            "minute must be between 0 and 59",
            xctx);
    }

    if (second < 0 || second > 255) {
        AFW_THROW_ERROR_Z(general,
            "second must be between 0 and 255",
            xctx);
    }

    if (microsecond < 0 || microsecond > 999999) {
        AFW_THROW_ERROR_Z(general,
            "microsecond must be between 0 and 999999",
            xctx);
    }

    time->hour = (afw_uint8_t)hour;
    time->minute = (afw_uint8_t)minute;
    time->second = (afw_uint8_t)second;
    time->microsecond = (afw_uint32_t)microsecond;
}


AFW_DEFINE_STATIC_INLINE(void)
impl_set_time_zone(
    afw_time_zone_t *time_zone,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx)
{
    if (tz_hours_offset < -14 || tz_hours_offset > 14) {
        AFW_THROW_ERROR_Z(general,
            "tz_hours_offset must be between -14 and 14",
            xctx);
    }

    if (tz_hours_offset == -14 || tz_hours_offset == 14) {
        if (tz_minutes_offset != 0) {
            AFW_THROW_ERROR_Z(general,
                "if tz_hours_offset is -14 or 14, "
                "tz_minutes_offset must be 0",
                xctx);
        }
    }

    if (tz_minutes_offset < -1 || tz_minutes_offset > 59) {
        AFW_THROW_ERROR_Z(general,
            "tz_minutes_offset must be between -1 and 59",
            xctx);
    }

    time_zone->hours = (afw_int8_t)tz_hours_offset;
    time_zone->minutes = (afw_int8_t)tz_minutes_offset;
}


/* Set afw_date_t from parts. */
AFW_DEFINE(void)
afw_date_set_from_parts(
    afw_date_t *date,
    int year,
    int month,
    int day,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx)
{
    impl_set_date_no_time_zone(
        &date->date,
        year, month, day,
        xctx);
    impl_set_time_zone(
        &date->time_zone,
        tz_hours_offset, tz_minutes_offset,
        xctx);
}


/* Create afw_date_t from parts in specific pool. */
AFW_DEFINE(const afw_date_t *)
afw_date_create_from_parts(
    int year,
    int month,
    int day,
    int tz_hours_offset,
    int tz_minutes_offset,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_date_t *result;

    result = afw_pool_calloc_type(p, afw_date_t, xctx);
    afw_date_set_from_parts(result,
        year, month, day,
        tz_hours_offset, tz_minutes_offset,
        xctx);
    return result;
}


/*
 * @brief Set afw_dateTime_t from parts.
 * @param xctx of caller.
 */
AFW_DEFINE(void)
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
    afw_xctx_t *xctx)
{
    impl_set_date_no_time_zone(
        &dateTime->date,
        year, month, day,
        xctx);
    impl_set_time_no_time_zone(
        &dateTime->time,
        hour, minute, second, microsecond,
        xctx);
    impl_set_time_zone(
        &dateTime->time_zone,
        tz_hours_offset, tz_minutes_offset,
        xctx);
}


/*  Create afw_dateTime_t from parts in specific pool. */
AFW_DEFINE(const afw_dateTime_t *)
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
    afw_xctx_t *xctx)
{
    afw_dateTime_t *result;

    result = afw_pool_calloc_type(p, afw_dateTime_t, xctx);
    afw_dateTime_set_from_parts(result,
        year, month, day,
        hour, minute, second, microsecond,
        tz_hours_offset, tz_minutes_offset,
        xctx);
    return result;
}





/* Set afw_dateTime_t from apr_time. */
AFW_DECLARE(void)
afw_dateTime_set_from_apr_time(
    afw_dateTime_t *dateTime,
    apr_time_t apr_time,
    afw_xctx_t *xctx)
{
    apr_status_t rv;
    apr_time_exp_t tm;

    rv = apr_time_exp_lt(&tm, apr_time);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_lt() failed",
            xctx);
    }
    impl_dateTime_tm_set(dateTime, &tm, xctx);
}



/* Create afw_dateTime_t from apr time.*/
AFW_DECLARE(const afw_dateTime_t *)
afw_dateTime_create_from_apr_time(
    apr_time_t apr_time,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_dateTime_t *result;

    result = afw_pool_calloc_type(p, afw_dateTime_t, xctx);
    afw_dateTime_set_from_apr_time(result, apr_time, xctx);
    return result;
}



/* Set afw_dayTimeDuration_t from parts. */
AFW_DEFINE(void)
afw_dayTimeDuration_set_from_parts(
    afw_dayTimeDuration_t *dayTimeDuration,
    afw_boolean_t is_positive,
    int days,
    int hours,
    int minutes,
    int seconds,
    int microseconds,
    afw_xctx_t *xctx)
{
    if (days < 0 || days > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "days parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (hours < 0 || hours > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "hours parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (minutes < 0 || minutes > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "minutes parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (seconds < 0 || seconds > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "seconds parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (microseconds < 0 || microseconds > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "microseconds parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (is_positive) {
        dayTimeDuration->days = (afw_int32_t)days;
        dayTimeDuration->hours = (afw_int32_t)hours;
        dayTimeDuration->minutes = (afw_int32_t)minutes;
        dayTimeDuration->seconds = (afw_int32_t)seconds;
        dayTimeDuration->microseconds = (afw_int32_t)microseconds;
    }
    else {
        dayTimeDuration->days = -(afw_int32_t)days;
        dayTimeDuration->hours = -(afw_int32_t)hours;
        dayTimeDuration->minutes = -(afw_int32_t)minutes;
        dayTimeDuration->seconds = -(afw_int32_t)seconds;
        dayTimeDuration->microseconds = -(afw_int32_t)microseconds;
    }
}


/* Create afw_dayTimeDuration_t from parts in specific pool. */
AFW_DEFINE(const afw_dayTimeDuration_t *)
afw_dayTimeDuration_create_from_parts(
    afw_boolean_t is_positive,
    afw_int32_t days,
    afw_int32_t hours,
    afw_int32_t minutes,
    afw_int32_t seconds,
    afw_int32_t microseconds,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_dayTimeDuration_t *result;

    result = afw_pool_calloc_type(p, afw_dayTimeDuration_t, xctx);
    afw_dayTimeDuration_set_from_parts(result, is_positive,
        days, hours, minutes, seconds, microseconds, xctx);
    return result;
}


/* Set afw_time_t from parts. */
AFW_DEFINE(void)
afw_time_set_from_parts(
    afw_time_t *time,
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    afw_xctx_t *xctx)
{
    impl_set_time_no_time_zone(
        &time->time,
        hour, minute, second, microsecond,
        xctx);
    impl_set_time_zone(
        &time->time_zone,
        tz_hours_offset, tz_minutes_offset,
        xctx);
}


/* Create afw_time_t from parts in specific pool. */
AFW_DEFINE(const afw_time_t *)
afw_time_create_from_parts(
    int hour,
    int minute,
    int second,
    int microsecond,
    int tz_hours_offset,
    int tz_minutes_offset,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_time_t *result;

    result = afw_pool_calloc_type(p, afw_time_t, xctx);
    afw_time_set_from_parts(result,
        hour, minute, second, microsecond,
        tz_hours_offset, tz_minutes_offset,
        xctx);
    return result;
}


/* Set afw_yearMonthDuration_t from parts. */
AFW_DEFINE(void)
afw_yearMonthDuration_set_from_parts(
    afw_yearMonthDuration_t *yearMonthDuration,
    afw_boolean_t is_positive,
    int years,
    int months,
    afw_xctx_t *xctx)
{
    if (years < 0 || years > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "years parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (months < 0 || months > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "months parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (is_positive) {
        yearMonthDuration->years = (afw_int32_t)years;
        yearMonthDuration->months = (afw_int32_t)months;
    }
    else {
        yearMonthDuration->years = -(afw_int32_t)years;
        yearMonthDuration->months = -(afw_int32_t)months;
    }
}


/* Create afw_yearMonthDuration_t from parts in specific pool. */
AFW_DEFINE(const afw_yearMonthDuration_t *)
afw_yearMonthDuration_create_from_parts(
    afw_boolean_t is_positive,
    afw_int32_t years,
    afw_int32_t months,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_yearMonthDuration_t *result;

    result = afw_pool_calloc_type(p, afw_yearMonthDuration_t, xctx);
    afw_yearMonthDuration_set_from_parts(result, is_positive,
        years, months, xctx);
    return result;
}


/* Get now time as dateTime in specified pool. */
AFW_DEFINE(void)
afw_dateTime_set_now(
    afw_dateTime_t *dateTime_local, afw_dateTime_t *dateTime_utc,
    afw_xctx_t *xctx)
{
    apr_status_t rv;
    apr_time_exp_t tm;
    apr_time_t now;

    /* Make sure both will get same time. */
    now = apr_time_now();

    /* If requested, set local dateTime to now. */
    if (dateTime_local) {
        rv = apr_time_exp_lt(&tm, now);
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_lt() failed",
                xctx);
        }
        impl_dateTime_tm_set(dateTime_local, &tm, xctx);
    }

    /* If requested, set utc dateTime to now. */
    if (dateTime_utc) {
        rv = apr_time_exp_gmt(&tm, now);
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_gmt() failed",
                xctx);
        }
        impl_dateTime_tm_set(dateTime_utc, &tm, xctx);
    }
}


/* Get now time as time only in specified pool. */
AFW_DEFINE(void)
afw_time_set_now(
    afw_time_t *time_local, afw_time_t *time_utc,
    afw_xctx_t *xctx)
{
    apr_status_t rv;
    apr_time_exp_t tm;
    apr_time_t now;

    /* Make sure both will get same time. */
    now = apr_time_now();

    /* If requested, set local time. */
    if (time_local) {
        rv = apr_time_exp_lt(&tm, now);
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_lt() failed",
                xctx);
        }
        impl_time_tm_set(time_local, &tm, xctx);
    }

    /* If requested, set utc time. */
    if (time_utc) {
        rv = apr_time_exp_gmt(&tm, now);
        if (rv != APR_SUCCESS) {
            AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_gmt() failed",
                xctx);
        }
        impl_time_tm_set(time_utc, &tm, xctx);
    }
}


/* Get now time as dateTime in specified pool. */
AFW_DEFINE(const afw_dateTime_t *)
afw_dateTime_now_utc(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_dateTime_t *dateTime;

    dateTime = afw_pool_calloc(p, sizeof(afw_dateTime_t), xctx);
    afw_dateTime_set_now(NULL, dateTime, xctx);
    return dateTime;
}


/* Get now time as time only in specified pool. */
AFW_DEFINE(const afw_time_t *)
afw_time_now_utc(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_time_t *time;

    time = afw_pool_calloc(p, sizeof(afw_time_t), xctx);
    afw_time_set_now(NULL, time, xctx);
    return time;
}


/* Get now local time as time only in specified pool. */
AFW_DEFINE(void)
afw_time_set_now_local(
    afw_time_t *time, afw_xctx_t *xctx)
{
    apr_status_t rv;
    apr_time_exp_t tm;

    /* Explode now time. */
    rv = apr_time_exp_lt(&tm, apr_time_now());
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv, "apr_time_exp_gmt() failed",
            xctx);
    }

    /* Set time. */
    impl_time_tm_set(time, &tm, xctx);
}


/* Get now local time as dateTime in specified pool. */
AFW_DEFINE(const afw_dateTime_t *)
afw_dateTime_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_dateTime_t *dateTime;

    dateTime = afw_pool_calloc(p, sizeof(afw_dateTime_t), xctx);
    afw_dateTime_set_now(dateTime, NULL, xctx);
    return dateTime;
}


/* Get now local time as time only in specified pool. */
AFW_DEFINE(const afw_time_t *)
afw_time_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_time_t *time;

    time = afw_pool_calloc(p, sizeof(afw_time_t), xctx);
    afw_time_set_now_local(time, xctx);
    return time;
}


/* Convert time normalize to utc to microseconds. */
AFW_DEFINE(afw_integer_t)
afw_time_to_microseconds_utc(
    const afw_time_t *time, afw_xctx_t *xctx)
{
    afw_integer_t result;

    result = AFW_TIME_HOURS_TO_MICROSECONDS(time->time.hour) +
        AFW_TIME_MINUTES_TO_MICROSECONDS(time->time.minute) +
        AFW_TIME_SECONDS_TO_MICROSECONDS(time->time.second) +
        time->time.microsecond;

    if (time->time_zone.minutes != -1) {
        result -= AFW_TIME_HOURS_TO_MICROSECONDS(time->time_zone.hours) +
            AFW_TIME_MINUTES_TO_MICROSECONDS(time->time_zone.minutes);
    }

    return result;
}


static afw_size_t
impl_set_internal_to_utf8_date(const afw_date_no_time_zone_t *date,
    afw_utf8_octet_t *to, afw_xctx_t *xctx)
{
    afw_utf8_octet_t *c;
    int len;
    afw_int32_t year;

    /* [-]yyyyyyyyyy-mm-dd.  Year must be at least 4 digits. */
    if (date->year <= 9999 && date->year >= -9999) {
        c = to;
        year = date->year;
        if (date->year < 0) {
            year = -year;
            *c++ = '-';
        }
        *c++ = (char)(year / 1000) + '0';
        *c++ = (char)((year % 1000) / 100) + '0';
        *c++ = year % 100 / 10 + '0';
        *c++ = year % 10 + '0';
    }
    else {
        len = snprintf(to, IMPL_MAX_DATE_SEPARATE_LEN, "%d", date->year);
        if (len < 0 || len >= IMPL_MAX_DATE_SEPARATE_LEN) {
            AFW_THROW_ERROR_Z(general, "Internal error", xctx);
        }
        c = to + len;
    }
    *c++ = '-';
    *c++ = date->month % 100 / 10 + '0';
    *c++ = date->month % 10 + '0';
    *c++ = '-';
    *c++ = date->day % 100 / 10 + '0';
    *c++ = date->day % 10 + '0';

    /* Return len of date. */
    return (afw_size_t)(c - to);
}


static afw_size_t
impl_set_internal_to_utf8_time(
    const afw_time_no_time_zone_t *time,
    afw_utf8_octet_t *to, afw_xctx_t *xctx)
{
    afw_utf8_octet_t *c;
    apr_int32_t x;

    c = to;
    *c++ = time->hour % 100 / 10 + '0';
    *c++ = time->hour % 10 + '0';
    *c++ = ':';
    *c++ = time->minute % 100 / 10 + '0';
    *c++ = time->minute % 10 + '0';
    *c++ = ':';
    *c++ = time->second % 100 / 10 + '0';
    *c++ = time->second % 10 + '0';
    do {
        if (time->microsecond == 0) break;
        *c++ = '.';
        *c++ = time->microsecond % 1000000 / 100000 + '0';
        x = time->microsecond % 100000;
        if (x == 0) break;
        *c++ = x / 10000 + '0';
        x = time->microsecond % 10000;
        if (x == 0) break;
        *c++ = x / 1000 + '0';
        x = time->microsecond % 1000;
        if (x == 0) break;
        *c++ = x / 100 + '0';
        x = time->microsecond % 100;
        if (x == 0) break;
        *c++ = x / 10 + '0';
        x = time->microsecond % 10;
        if (x == 0) break;
        *c++ = x + '0';
    } while (0);

    /* Return len of time. */
    return (afw_size_t)(c - to);
}


static afw_size_t
impl_set_internal_to_utf8_time_zone(
    const afw_time_zone_t *tz,
    afw_utf8_octet_t *to, afw_xctx_t *xctx)
{
    afw_utf8_octet_t *c;
    afw_int8_t hours;

    c = to;

    /* If minutes is negative, there is no time zone. */
    if (tz->minutes < 0) return 0;

    /* If hours and minutes is 0, time zone is 'Z'. */
    if (tz->hours == 0 && tz->minutes == 0) {
        *to = 'Z';
        return 1;
    }

    /* Sign of time zone is sign of hours. */
    if (tz->hours < 0) {
        *c++ = '-';
        hours = -tz->hours;
    }
    else {
        *c++ = '+';
        hours = tz->hours;
    }

    /* hh:mm */
    *c++ = hours % 100 / 10 + '0';
    *c++ = hours % 10 + '0';
    *c++ = ':';
    *c++ = tz->minutes % 100 / 10 + '0';
    *c++ = tz->minutes % 10 + '0';

    /* Return len of time zone. */
    return (afw_size_t)(c - to);
}


static afw_size_t
impl_utf8_date_set_internal(
    const afw_utf8_octet_t *from, afw_size_t len,
    afw_date_no_time_zone_t *to, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    afw_boolean_t is_negative;

    /* Clear any sluff in result and initialize locals. */
    memset(to, 0, sizeof(*to));
    s = from;
    is_negative = false;

    /* Error if not minimal size. */
    if (len < sizeof("1-23-45") - 1) goto error;

    /* Can begin with a '-' (not '+' by specification) */
    if (*s == '-') {
        is_negative = true;
        s++;
    }

    /*
     * Convert year.
     *
     * Note: This only throws an error if there is not at least one digit.
     *      This is intentional. The specification requires at least 4.
     *      to_utf8() will produce at least 4, but it seems ok to allow
     *      less than 4 on input.
     */
    if (*s < '0' || *s > '9') goto error;
    for (; (afw_size_t)(s - from) < len; s++) {
        if (*s < '0' || *s > '9') break;
        to->year = to->year * 10 + (*s - '0');
        if (to->year < 0) goto error;     
    }
    if (is_negative) to->year = -to->year;

    /*
     * A 0 year is an error.
     *
     * This error is commented out.  3.2.7 of XML Schema Part 2 says that
     * 0000 is not allowed, but they intend to allow it.  0000 will represent
     * year 1 BC.
     *
     * if (to->year == 0) goto error;
     */
    
    /* If not enough left for month and day, it's an error. */
    if (len - (s - from) < sizeof("-01-10") - 1) goto error;

    /* Next should be '-'. */
    if (*s++ != '-') goto error;

    /* Next should be MM (01-12). */
    if (*s != '0' && *s != '1') goto error;
    to->month = (*s++ - '0') * 10;
    if (*s < '0' || *s > '9') goto error;
    to->month += (*s++ - '0');
    if (to->month < 1 || to->month > 12) goto error;

    /* Next should be '-'. */
    if (*s++ != '-') goto error;

    /* Next should be DD (01-12). */
    if (*s < '0' || *s > '3') goto error;
    to->day = (*s++ - '0') * 10;
    if (*s < '0' || *s > '9') goto error;
    to->day += (*s++ - '0');
    if (to->day < 1) goto error;

    /* Check days in month. */
    switch (to->month) {

    case 2: /* February */
        if (to->day <= 28) break;
        if (to->day > 29) goto error;
        if (to->year % 4 != 0 || (to->year % 100 == 0 && to->year % 400 != 0))
            goto error;
        break;

    case 4:  /* April     */
    case 6:  /* June      */
    case 9:  /* September */
    case 11: /* November  */
        if (to->day > 30) goto error;
        break;

    default: /* Other months. */
        if (to->day > 31) goto error;
    }

    /* Return length remaining. */
    return len - (afw_size_t)(s - from);

error:
    AFW_THROW_ERROR_Z(general, "date invalid", xctx);
}

static afw_size_t
impl_set_utf8_time_to_internal(
    const afw_utf8_octet_t *from, afw_size_t len,
    afw_time_no_time_zone_t *to, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    char d1, d2;

    memset(to, 0, sizeof(*to));
    s = from;

    /* hh[:mm[:ss[.nnnnnn]]][Z | (+|-)hh:mm] */

    /* hh[[:mm]:ss] */
    if (len < 2) goto error;
    len -= 2;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    to->hour = d1 * 10 + d2;
    if (len < 3 || *s == '+' || *s == '-' || *s == 'Z') goto finish_up;
    if (*s++ != ':') goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    len -= 3;
    to->minute = d1 * 10 + d2;
    if (len < 3 || *s == '+' || *s == '-' || *s == 'Z') goto finish_up;
    if (*s++ != ':') goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    len -= 3;
    to->second = d1 * 10 + d2;
    if (len <= 0) goto finish_up;

    /* Optional [.nnnnnn] */
    if (*s == '.') do {
        s++; len--;
        if (len <= 0 || !IMPL_IS_DIGIT(*s)) goto error;
        len--;
        to->microsecond =  (*s++ - '0') * 100000;
        if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
        len--;
        to->microsecond += (*s++ - '0') * 10000;
        if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
        len--;
        to->microsecond += (*s++ - '0') * 1000;
        if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
        len--;
        to->microsecond += (*s++ - '0') * 100;
        if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
        len--;
        to->microsecond += (*s++ - '0') * 10;
        if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
        len--;
        to->microsecond += (*s++ - '0');
    } while (0);

    /* Return length used. */
finish_up:

    /* Handle special case of 24:00:00. */
    if (to->hour == 24) {
        if (to->minute != 0 || to->second != 0 || to->microsecond != 0) {
            goto error;
        }
    }

    /* If not hour 24, make sure rest of time is within bounds. */
    else if (to->hour > 24 || to->minute >= 60 || to->second >= 60) {
        goto error;
    }

    /* Return length remaining. */
    return len;

error:
    AFW_THROW_ERROR_Z(general, "time invalid", xctx);
}


static afw_size_t
impl_set_utf8_time_zone_to_internal(
    const afw_utf8_octet_t *from, afw_size_t len,
    afw_time_zone_t *to, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    char d1, d2;
    afw_boolean_t negative;

    negative = false;
    to->hours = 0;
    to->minutes = -1; /* Assume on time zone */
    s = from;

    /* If len is 0, just return. */
    if (len <= 0) return len;

    /* Optional [Z | (+|-)hh:mm] */
    if (*s == 'Z') {
        if (len != 1) goto error;
        to->minutes = 0;
        return len - 1;
    }
    if (len < 3) goto finish_up;
    if (*s == '+') negative = false;
    else if (*s == '-') negative = true;
    else goto finish_up;
    s++;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    len -= 3;
    to->hours = d1 * 10 + d2;
    if (*s != ':') goto finish_up;
    s++;
    if (len < 3) goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    len -= 3;
    to->minutes = d1 * 10 + d2;

    /* Check result and return length remaining. */
finish_up:
    if (to->hours > 14 || to->minutes > 59 ||
        (to->hours == 14 && to->minutes != 0))
    {
        goto error;
    }
    if (negative) to->hours = -to->hours;
    return len;

error:
    AFW_THROW_ERROR_Z(general, "time zone invalid.  "
        "(('+' | '-') hh ':' mm) | 'Z'.  "
        "hh 00-14, mm 00-59 or 00 if hh is 14", xctx);
}


/* Convert internal date to utf8 xml schema date in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_date_internal_to_utf8(
    const afw_date_t *date, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_octet_t to[IMPL_MAX_DATE_LEN];
    afw_size_t len;

    len = impl_set_internal_to_utf8_date(&date->date, &to[0], xctx);
    len += impl_set_internal_to_utf8_time_zone(&date->time_zone, &to[len],
        xctx);

    return afw_utf8_create_copy(&to[0], len, p, xctx);
}


/* Convert utf8 xml schema date to internal and set. */
AFW_DEFINE(void)
afw_date_utf8_set_internal(
    const afw_utf8_t *utf8, afw_date_t *internal, afw_xctx_t *xctx)
{
    afw_size_t len;
    const afw_utf8_octet_t *s;

    s = utf8->s;
    len = utf8->len;
    len = impl_utf8_date_set_internal(s, len, &internal->date, xctx);
    s += utf8->len - len;
    len = impl_set_utf8_time_zone_to_internal(s, len, &internal->time_zone,
        xctx);
    if (len != 0) {
        AFW_THROW_ERROR_Z(general, "date invalid", xctx);
    }
}


/* Convert internal dateTime to utf8 xml schema dateTime in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_dateTime_internal_to_utf8(
    const afw_dateTime_t *dateTime, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_octet_t to[IMPL_MAX_DATE_TIME_LEN];
    afw_size_t len;

    len = impl_set_internal_to_utf8_date(&dateTime->date, &to[0], xctx);
    to[len++] = 'T';
    len += impl_set_internal_to_utf8_time(&dateTime->time, &to[len], xctx);
    len += impl_set_internal_to_utf8_time_zone(&dateTime->time_zone, &to[len],
        xctx);

    return afw_utf8_create_copy(&to[0], len, p, xctx);
}


/* Convert utf8 xml schema dateTime to internal and set. */
AFW_DEFINE(void)
afw_dateTime_utf8_set_internal(
    const afw_utf8_t *utf8, afw_dateTime_t *internal, afw_xctx_t *xctx)
{
    afw_size_t len, prev;
    const afw_utf8_octet_t *s;

    s = utf8->s;
    len = utf8->len;
    len = impl_utf8_date_set_internal(s, len, &internal->date, xctx);
    s += utf8->len - len;
    if (len-- < 1 || *s++ != 'T')  goto error;
    prev = len;
    len = impl_set_utf8_time_to_internal(s, len, &internal->time, xctx);
    s += prev - len;
    len = impl_set_utf8_time_zone_to_internal(s, len, &internal->time_zone,
        xctx);
    if (len != 0) goto error;

    return;

error:
    AFW_THROW_ERROR_Z(general, "dateTime invalid", xctx);
}


/* Convert internal dayTimeDuration to utf8 xml schema dayTimeDuration. */
AFW_DEFINE(const afw_utf8_t *)
afw_dayTimeDuration_internal_to_utf8(
    const afw_dayTimeDuration_t *dayTimeDuration,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_octet_t wa[
        sizeof("-P2147483648DT2147483648H2147483648M2147483648.999999S") - 1];
    afw_utf8_octet_t *s, *c, *end;
    afw_int32_t x;
    afw_integer_t days, hours, minutes, seconds, microseconds;

    /* Initialize locals. */
    c = s = &wa[0];
    end = &wa[sizeof(wa)];

    /* If negative duration, start with '-' and use negative of all members. */
    if (dayTimeDuration->days < 0 ||
        dayTimeDuration->hours < 0 ||
        dayTimeDuration->minutes < 0 ||
        dayTimeDuration->seconds < 0 ||
        dayTimeDuration->microseconds < 0)
    {
        days = -dayTimeDuration->days;
        hours = -dayTimeDuration->hours;
        minutes = -dayTimeDuration->minutes;
        seconds = -dayTimeDuration->seconds;
        microseconds = -dayTimeDuration->microseconds;
        *c++ = '-';
    }

    /* If positive duration, use members asis. */
    else {
        days = dayTimeDuration->days;
        hours = dayTimeDuration->hours;
        minutes = dayTimeDuration->minutes;
        seconds = dayTimeDuration->seconds;
        microseconds = dayTimeDuration->microseconds;
    }

    /* Next is always 'P'. */
    *c++ = 'P';

    /* If hours, put number of hours followed by 'H'. */
    if (days != 0) {
        c += afw_number_integer_set_u8(days, c, end - c,
            xctx);
        *c++ = 'D';
    }

    /* Time portion required a 'T' */
    if (hours != 0 || minutes != 0 || seconds != 0 || microseconds != 0) {
        *c++ = 'T';
    }

    /* If hours, put number of hours followed by 'H'. */
    if (hours != 0) {
        c += afw_number_integer_set_u8(hours, c, end - c,
            xctx);
        *c++ = 'H';
    }

    /* If minutes, put number of minutes followed by 'H'. */
    if (minutes != 0) {
        c += afw_number_integer_set_u8(minutes, c, end - c,
            xctx);
        *c++ = 'M';
    }

    /*
     * If seconds or microseconds, put number of seconds, followed by
     * fractional seconds if there are microseconds, ending by 'S'.
     */
    if (seconds != 0 || microseconds != 0) {
        if (seconds == 0) {
            *c++ = '0';
        }
        else {
            c += afw_number_integer_set_u8(seconds,
                c, end - c, xctx);
        }
        if (microseconds != 0) do {
            *c++ = '.';
            *c++ = (afw_utf8_octet_t)(microseconds % 1000000 / 100000 + '0');
            x = microseconds % 100000;
            if (x == 0) break;
            *c++ = (afw_utf8_octet_t)(x / 10000 + '0');
            x = microseconds % 10000;
            if (x == 0) break;
            *c++ = (afw_utf8_octet_t)(x / 1000 + '0');
            x = microseconds % 1000;
            if (x == 0) break;
            *c++ = (afw_utf8_octet_t)(x / 100 + '0');
            x = microseconds % 100;
            if (x == 0) break;
            *c++ = (afw_utf8_octet_t)(x / 10 + '0');
            x = microseconds % 10;
            if (x == 0) break;
            *c++ = x + '0';
        } while (0);
        *c++ = 'S';
    }

    /* Return result. */
    return afw_utf8_create_copy(s, c - s, p, xctx);
}


/* Convert utf8 xml schema dayTimeDuration to internal and set. */
AFW_DEFINE(void)
afw_dayTimeDuration_utf8_set_internal(
    const afw_utf8_t *utf8, afw_dayTimeDuration_t *internal,
    afw_xctx_t *xctx)
{
    afw_size_t len, x;
    const afw_utf8_octet_t *s;
    afw_integer_t i;
    afw_boolean_t is_negative;
    afw_boolean_t t_encountered;

    s = utf8->s;
    len = utf8->len;
    memset(internal, 0, sizeof(*internal));

    /* Can start with '-'. */
    if (len < 1) goto error;
    is_negative = false;
    if (*s == '-') {
        is_negative = true;
        s++;
        len--;
    }

    /* Next must be 'P'. */
    if (*s++ != 'P') goto error;
    len--;
    
    t_encountered = false;
    /* Next can be 'T'. */
    if (*s == 'T') {
        t_encountered = true;
        len--;
        s++;
    };

    /* Next must be a non-negative number. */
    x = afw_number_parse_integer(s, len, &i, xctx->p, xctx);
    if (i < 0) goto error;
    s += x;
    len -= x;
    if (len < 1) goto error;

    /* Next can be 'D'. */
    if (*s == 'D') {
        if (t_encountered) goto error;
        if (i > AFW_INT32_MAX) goto error;
        internal->days = (afw_int32_t)((is_negative) ? -i  : i);
        s++;
        if (--len <= 0) goto finished;
    }

    /* 'T' required before time part. This is the time separator */
    if (!t_encountered) {
        if (*s++ != 'T') goto error;
        len--;
        if (len < 1) goto error;
        x = afw_number_parse_integer(s, len, &i, xctx->p, xctx);
        if (i < 0) goto error;
        s += x;
        len -= x;
        if (len < 1) goto error;
    };

    /* Next can be 'H'. */
    if (*s == 'H') {
        if (i > AFW_INT32_MAX) goto error;
        internal->hours = (afw_int32_t)((is_negative) ? -i  : i);
        s++;
        if (--len <= 0) goto finished;
        x = afw_number_parse_integer(s, len, &i, xctx->p, xctx);
        if (i < 0) goto error;
        s += x;
        len -= x;
        if (len < 1) goto error;
    }

    /* Next can be 'M'. */
    if (*s == 'M') {
        if (i > AFW_UINT32_MAX) goto error;
        internal->minutes = (afw_int32_t)((is_negative) ? -i  : i);
        s++;
        if (--len <= 0) goto finished;
        x = afw_number_parse_integer(s, len, &i, xctx->p, xctx);
        if (i < 0) goto error;
        s += x;
        len -= x;
        if (len < 1) goto error;
    }

    /* Must be seconds at this point. Will check for 'S' later. */
    if (i > AFW_UINT32_MAX) goto error;
    internal->seconds = (afw_int32_t)((is_negative) ? -i  : i);

    /* Next can be '.' followed by microseconds. */
    if (*s == '.') {
        do {
            s++; len--;
            if (len <= 0 || !IMPL_IS_DIGIT(*s)) goto error;
            len--;
            internal->microseconds = (*s++ - '0') * 100000;
            if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
            len--;
            internal->microseconds += (*s++ - '0') * 10000;
            if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
            len--;
            internal->microseconds += (*s++ - '0') * 1000;
            if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
            len--;
            internal->microseconds += (*s++ - '0') * 100;
            if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
            len--;
            internal->microseconds += (*s++ - '0') * 10;
            if (len <= 0 || !IMPL_IS_DIGIT(*s)) break;
            len--;
            internal->microseconds += (*s++ - '0');
        } while (0);
        if (is_negative) {
            internal->microseconds = -internal->microseconds;
        }
    }

    /* At this point, last char must be 'S'. */
    if (len != 1 || *s != 'S') goto error;

finished:
    /* Return */
    return;

error:
    AFW_THROW_ERROR_Z(general, "dayTimeDuration invalid", xctx);
}


/* Convert internal time to utf8 xml schema time in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_time_internal_to_utf8(
    const afw_time_t *time, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_octet_t to[IMPL_MAX_TIME_LEN];
    afw_size_t len;

    len = impl_set_internal_to_utf8_time(&time->time, &to[0], xctx);
    len += impl_set_internal_to_utf8_time_zone(&time->time_zone, &to[len],
        xctx);

    return afw_utf8_create_copy(&to[0], len, p, xctx);
}


/* Convert utf8 xml schema dateTime to internal and set. */
AFW_DEFINE(void)
afw_time_utf8_set_internal(
    const afw_utf8_t *utf8, afw_time_t *internal, afw_xctx_t *xctx)
{
    afw_size_t len;
    const afw_utf8_octet_t *s;

    s = utf8->s;
    len = utf8->len;
    len = impl_set_utf8_time_to_internal(s, len, &internal->time, xctx);
    s += utf8->len - len;
    len = impl_set_utf8_time_zone_to_internal(s, len, &internal->time_zone,
        xctx);
    if (len != 0) {
        AFW_THROW_ERROR_Z(general, "Time invalid", xctx);
    }
}

/*
 * Convert internal yearMonthDuration to utf8 xml schema yearMonthDuration in
 * specified pool.
 */
AFW_DEFINE(const afw_utf8_t *)
afw_yearMonthDuration_internal_to_utf8(
    const afw_yearMonthDuration_t *yearMonthDuration,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_octet_t wa[sizeof("-P4294967295Y65535M") - 1];
    afw_utf8_octet_t *s, *c, *end;
    afw_integer_t years, months;

    /* Initialize locals. */
    c = s = &wa[0];
    end = &wa[sizeof(wa)];

    /* If negative duration, start with '-' and use negative of all members. */
    if (yearMonthDuration->years < 0 ||
        yearMonthDuration->months < 0)
    {
        years = -yearMonthDuration->years;
        months = -yearMonthDuration->months;
        *c++ = '-';
    }

    /* If positive duration, use members asis. */
    else {
        years = yearMonthDuration->years;
        months = yearMonthDuration->months;
    }

    /* Next is always 'P'. */
    *c++ = 'P';

    /* If years, put number of years followed by 'Y'. */
    if (years != 0) {
        c += afw_number_integer_set_u8(years, c, end - c,
            xctx);
        *c++ = 'Y';
    }

    /* If months, put number of months followed by 'M'. */
    if (months != 0) {
        c += afw_number_integer_set_u8(months, c, end - c,
            xctx);
        *c++ = 'M';
    }

    /* Return result. */
    return afw_utf8_create_copy(s, c - s, p, xctx);
}


/* Convert utf8 xml schema yearMonthDuration to internal and set. */
AFW_DEFINE(void)
afw_yearMonthDuration_utf8_set_internal(
    const afw_utf8_t *utf8, afw_yearMonthDuration_t *internal,
    afw_xctx_t *xctx)
{
    afw_size_t len, x;
    const afw_utf8_octet_t *s;
    afw_integer_t i;
    afw_boolean_t is_negative;

    s = utf8->s;
    len = utf8->len;
    memset(internal, 0, sizeof(*internal));

    /* Can start with '-'. */
    if (len < 1) goto error;
    is_negative = false;
    if (*s == '-') {
        is_negative = true;
        s++;
        len--;
    }

    /* Next must be 'P'. */
    if (*s++ != 'P') goto error;
    len--;

    /* Next must be a non-negative number. */
    x = afw_number_parse_integer(s, len, &i, xctx->p, xctx);
    if (i < 0) goto error;
    s += x;
    len -= x;
    if (len < 1) goto error;

    /* Next can be 'Y'. */
    if (*s == 'Y') {
        if (i > AFW_INT32_MAX) goto error;
        internal->years = (afw_int32_t)((is_negative) ? -i  : i);
        s++;
        if (--len <= 0) goto finished;
        x = afw_number_parse_integer(s, len, &i, xctx->p, xctx);
        if (i < 0) goto error;
        s += x;
        len -= x;
    }

    /* At this point, last char must be 'M'. */
    if (len != 1 || *s != 'M' || i > AFW_INT32_MAX) goto error;
    internal->months = (afw_int32_t)((is_negative) ? -i  : i);

finished:
    /* Return */
    return;

error:
    AFW_THROW_ERROR_Z(general, "yearMonthDuration invalid", xctx);
}


AFW_DEFINE(int)
afw_date_compare(const afw_date_t *v1, const afw_date_t *v2,
    afw_xctx_t *xctx)
{
    afw_integer_t result;

    if (v1->time_zone.hours != v2->time_zone.hours ||
        v1->time_zone.minutes != v2->time_zone.minutes) {
        AFW_THROW_ERROR_Z(general,
            "Can't compare dates with different time zone values",
            xctx);
    }
    result = v1->date.year - v2->date.year;
    if (result != 0) goto finished;
    result = v1->date.month - v2->date.month;
    if (result != 0) goto finished;
    result = v1->date.day - v2->date.day;

finished:
    return (result == 0) ? 0 : (result > 0) ? 1 : -1;
}


AFW_DEFINE(int)
afw_dateTime_compare(const afw_dateTime_t *v1, const afw_dateTime_t *v2,
    afw_xctx_t *xctx)
{
    afw_integer_t result;
    afw_integer_t v1hour, v2hour, v1minute, v2minute;

    /* if only one value has a time zone then cannot compare */
    if ((v1->time_zone.minutes == -1 && v2->time_zone.minutes != -1) ||
        (v1->time_zone.minutes != -1 && v2->time_zone.minutes == -1))
    {
        AFW_THROW_ERROR_Z(general,
            "Can't compare dateTimes with only one value time zone",
            xctx);
    }


    result = v1->date.year - v2->date.year;
    if (result != 0) goto finished;
    result = v1->date.month - v2->date.month;
    if (result != 0) goto finished;
    result = v1->date.day - v2->date.day;
    if (result != 0) goto finished;
    /* if different time zones, normalize the two times */
    if (v1->time_zone.hours != v2->time_zone.hours ||
        v1->time_zone.minutes != v2->time_zone.minutes) {
        v1hour = v1->time.hour - v1->time_zone.hours;
        v2hour = v2->time.hour - v2->time_zone.hours;
        v1minute = v1->time.minute - v1->time_zone.minutes;
        v2minute = v2->time.minute - v2->time_zone.minutes;
        result = v1hour - v2hour;
        if (result != 0) goto finished;
        result = v1minute - v2minute;
        if (result != 0) goto finished;
    }
    else {
        result = (afw_integer_t)v1->time.hour - (afw_integer_t)v2->time.hour;
        if (result != 0) goto finished;
        result = (afw_integer_t)v1->time.minute - (afw_integer_t)v2->time.minute;
        if (result != 0) goto finished;
    }
    result = (afw_integer_t)v1->time.second - (afw_integer_t)v2->time.second;
    if (result != 0) goto finished;
    result = (afw_integer_t)v1->time.microsecond - (afw_integer_t)v2->time.microsecond;

finished:
    return (result == 0) ? 0 : (result > 0) ? 1 : -1;
}


AFW_DEFINE(int)
afw_time_compare(const afw_time_t *v1, const afw_time_t *v2,
    afw_xctx_t *xctx)
{
    afw_integer_t result;
    afw_integer_t v1hour, v2hour, v1minute, v2minute;

    /* if only one value has a time zone then cannot compare */
    if ((v1->time_zone.minutes == -1 && v2->time_zone.minutes != -1)||
        (v1->time_zone.minutes != -1 && v2->time_zone.minutes == -1))
    {
        AFW_THROW_ERROR_Z(general,
            "Can't compare times with only one value time zone",
            xctx);
    }

    /* if different time zones, normalize the two times */
    if (v1->time_zone.hours != v2->time_zone.hours ||
        v1->time_zone.minutes != v2->time_zone.minutes) {
        v1hour = v1->time.hour + v1->time_zone.hours;
        v2hour = v2->time.hour + v2->time_zone.hours;
        v1minute = v1->time.minute + v1->time_zone.minutes;
        v2minute = v2->time.minute + v2->time_zone.minutes;
        result = v1hour - v2hour;
        if (result != 0) goto finished;
        result = v1minute - v2minute;
        if (result != 0) goto finished;
    }
    else {
        result = (afw_integer_t)v1->time.hour - (afw_integer_t)v2->time.hour;
        if (result != 0) goto finished;
        result = (afw_integer_t)v1->time.minute - (afw_integer_t)v2->time.minute;
        if (result != 0) goto finished;
    }

    result = (afw_integer_t)v1->time.second - (afw_integer_t)v2->time.second;
    if (result != 0) goto finished;
    result = (afw_integer_t)v1->time.microsecond - (afw_integer_t)v2->time.microsecond;
 
finished:
    return (result == 0) ? 0 : (result > 0) ? 1 : -1;
}


AFW_DEFINE_STATIC_INLINE(afw_integer_t)
impl_fQuotient2(afw_integer_t a, unsigned int b)
{
    afw_integer_t result;

    if (a < 0) {
        result = (a - (afw_integer_t)b) / b;
    }
    else {
        result = a / b;
    }

    return result;
}


AFW_DEFINE_STATIC_INLINE(afw_integer_t)
impl_modulo2(afw_integer_t a, unsigned int b)
{
    return a - impl_fQuotient2(a, b) * b;
}


AFW_DEFINE_STATIC_INLINE(afw_integer_t)
impl_fQuotient3(afw_integer_t a, int low, int high)
{
    return impl_fQuotient2((a - low), (high - low));
}


AFW_DEFINE_STATIC_INLINE(afw_integer_t)
impl_modulo3(afw_integer_t a, int low, int high)
{
    return impl_modulo2((a - low), (high - low)) + low;
}


AFW_DEFINE(int)
afw_date_maximum_days_in_month(afw_integer_t year, afw_integer_t month)
{
    afw_integer_t m;

    m = impl_modulo3(month, 1, 13);

    switch (m) {

    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return 29;
        }
        return 28;

    case 4:
    case 6:
    case 9:
    case 11:
        return 30;

    default:
        return 31;
    }
}


static void
impl_normalize_dayTimeDuration(
    const afw_dayTimeDuration_t *from,
    afw_dayTimeDuration_t *to,
    afw_xctx_t *xctx)
{
    memset(to, 0, sizeof(*to));
    afw_integer_t days, minutes, hours;

    to->seconds = from->seconds % 60;

    minutes = from->minutes + (from->seconds / 60);
    to->minutes = minutes % 60;

    hours = from->hours + (minutes / 60);
    if (hours < AFW_INT32_MIN || hours > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "dayTimeDuration limit exceeded", xctx);
    }
    to->hours = (afw_int32_t)hours;

    days = from->days + (hours / 24);
    if (days < AFW_INT32_MIN || days > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "dayTimeDuration limit exceeded", xctx);
    }
    to->days = (afw_int32_t)days;

}


static void
impl_normalize_yearMonthDuration(
    const afw_yearMonthDuration_t *from,
    afw_yearMonthDuration_t *to,
    afw_xctx_t *xctx)
{
    afw_integer_t years;

    memset(to, 0, sizeof(*to));

    to->months = (afw_int32_t)impl_modulo3(from->months, 1, 13);

    years = from->years + impl_fQuotient3(from->months, 1, 13);
    if (years < AFW_INT32_MIN || years > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "yearMonthDuration limit exceeded", xctx);
    }
    to->years = (afw_int32_t)years;
}


/* Compare two dateTime values. */
AFW_DEFINE(int)
afw_dayTimeDuration_compare(
    const afw_dayTimeDuration_t *v1,
    const afw_dayTimeDuration_t *v2, afw_xctx_t *xctx)
{
    afw_dayTimeDuration_t n1, n2;

    impl_normalize_dayTimeDuration(v1, &n1, xctx);
    impl_normalize_dayTimeDuration(v2, &n2, xctx);

    /* Return results. */
    if (n1.days    != n2.days)    return (n1.days    > n2.days)    ? 1 : -1;
    if (n1.hours   != n2.hours)   return (n1.hours   > n2.hours)   ? 1 : -1;
    if (n1.minutes != n2.minutes) return (n1.minutes > n2.minutes) ? 1 : -1;
    if (n1.seconds != n2.seconds) return (n1.seconds > n2.seconds) ? 1 : -1;
    if (n1.microseconds != n2.microseconds)
        return (n1.microseconds > n2.microseconds) ? 1 : -1;
    return 0;
}


/* Compare two yearMonthDuration values. */
AFW_DEFINE(int)
afw_yearMonthDuration_compare(
    const afw_yearMonthDuration_t *v1,
    const afw_yearMonthDuration_t *v2, afw_xctx_t *xctx)
{
    afw_yearMonthDuration_t n1, n2;

    impl_normalize_yearMonthDuration(v1, &n1, xctx);
    impl_normalize_yearMonthDuration(v2, &n2, xctx);

    /* Return results. */
    if (n1.years  != n2.years)  return (n1.years  > n2.years)  ? 1 : -1;
    if (n1.months != n2.months) return (n1.months > n2.months) ? 1 : -1;
    return 0;
}


/* Add dayTimeDuration to date. */
AFW_DEFINE(void)
afw_date_add_yearMonthDuration(
    afw_date_t *date,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx)
{
    afw_integer_t year, month;
    int days_in_month;

    year = date->date.year + yearMonthDuration->years;
    month = date->date.month + yearMonthDuration->months;

    year += impl_fQuotient3(month, 1, 13);
    month = impl_modulo3(month, 1, 13);
    
    if (year < AFW_INT32_MIN || year > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "year limit exceeded", xctx);
    }

    date->date.year = (afw_int32_t)year;
    date->date.month = (afw_int8_t)month;
    days_in_month = afw_date_maximum_days_in_month(year, month);
    if (date->date.day > days_in_month) {
        date->date.day = (afw_int8_t)days_in_month;
    }
}


/* Subtract yearMonthDuration from date. */
AFW_DEFINE(void)
afw_date_subtract_yearMonthDuration(
    afw_date_t *date,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx)
{
    afw_integer_t year, month;
    int days_in_month;

    year = date->date.year - yearMonthDuration->years;
    month = date->date.month - yearMonthDuration->months;

    year += impl_fQuotient3(month, 1, 13);
    month = impl_modulo3(month, 1, 13);
    
    if (year < AFW_INT32_MIN || year > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "year limit exceeded", xctx);
    }

    date->date.year = (afw_int32_t)year;
    date->date.month = (afw_int8_t)month;
    days_in_month = afw_date_maximum_days_in_month(year, month);
    if (date->date.day > days_in_month) {
        date->date.day = (afw_int8_t)days_in_month;
    }
}


/* Add dayTimeDuration to dateTime. */
AFW_DEFINE(void)
afw_dateTime_add_dayTimeDuration(
    afw_dateTime_t *dateTime,
    const afw_dayTimeDuration_t *dayTimeDuration,
    afw_xctx_t *xctx)
{
    afw_integer_t year, month, day, temp, carry;
    int dim;

    year = dateTime->date.year;
    month = dateTime->date.month;

    temp = dateTime->time.microsecond + dayTimeDuration->microseconds;
    dateTime->time.microsecond = (afw_uint32_t)impl_modulo2(temp, 1000000);
    carry = impl_fQuotient2(temp, 1000000);

    temp = dateTime->time.second + dayTimeDuration->seconds + carry;
    dateTime->time.second = (afw_uint8_t)impl_modulo2(temp, 60);
    carry = impl_fQuotient2(temp, 60);

    temp = dateTime->time.minute + dayTimeDuration->minutes + carry;
    dateTime->time.minute = (afw_uint8_t)impl_modulo2(temp, 60);
    carry = impl_fQuotient2(temp, 60);

    temp = dateTime->time.hour + dayTimeDuration->hours + carry;
    dateTime->time.hour = (afw_uint8_t)impl_modulo2(temp, 24);
    carry = impl_fQuotient2(temp, 24);

    dim = afw_date_maximum_days_in_month(year, month);
    day = dateTime->date.day;
    if (day > dim) {
        day = dim;
    }
    else if (dateTime->date.day < 1) {
        day = 1;
    }
    day += dayTimeDuration->days + carry;

    for (;;) {
        if (day < 1) {
            day += afw_date_maximum_days_in_month(year, month - 1);
            carry = -1;
        }
        else {
            dim = afw_date_maximum_days_in_month(year, month);
            if (day <= dim) break;
            day -= dim;
            carry = 1;
        }
        temp = month + carry;
        month = impl_modulo3(temp, 1, 13);
        year += impl_fQuotient3(temp, 1, 13);
    }

    dateTime->date.day = (afw_uint8_t)day;
    dateTime->date.month = (afw_uint8_t)month;
    if (year > AFW_INT32_MAX || year < AFW_INT32_MIN) {
        AFW_THROW_ERROR_Z(general, "Year limit exceeded", xctx);
    }
    dateTime->date.year = (afw_int32_t)year;
}


/* Subtract dayTimeDuration from dateTime. */
AFW_DEFINE(void)
afw_dateTime_subtract_dayTimeDuration(
    afw_dateTime_t *dateTime,
    const afw_dayTimeDuration_t *dayTimeDuration,
    afw_xctx_t *xctx)
{
    afw_integer_t year, month, day, temp, carry;
    int dim;

    year = dateTime->date.year;
    month = dateTime->date.month;

    temp = dateTime->time.microsecond - dayTimeDuration->microseconds;
    dateTime->time.microsecond = (afw_uint32_t)impl_modulo2(temp, 1000000);
    carry = impl_fQuotient2(temp, 1000000);

    temp = dateTime->time.second - dayTimeDuration->seconds + carry;
    dateTime->time.second = (afw_uint8_t)impl_modulo2(temp, 60);
    carry = impl_fQuotient2(temp, 60);

    temp = dateTime->time.minute - dayTimeDuration->minutes + carry;
    dateTime->time.minute = (afw_uint8_t)impl_modulo2(temp, 60);
    carry = impl_fQuotient2(temp, 60);

    temp = dateTime->time.hour - dayTimeDuration->hours + carry;
    dateTime->time.hour = (afw_uint8_t)impl_modulo2(temp, 24);
    carry = impl_fQuotient2(temp, 24);

    dim = afw_date_maximum_days_in_month(year, month);
    day = dateTime->date.day;
    if (day > dim) {
        day = dim;
    }
    else if (dateTime->date.day < 1) {
        day = 1;
    }
    day = day - dayTimeDuration->days + carry;

    for (;;) {
        if (day < 1) {
            day += afw_date_maximum_days_in_month(year, month - 1);
            carry = -1;
        }
        else {
            dim = afw_date_maximum_days_in_month(year, month);
            if (day <= dim) break;
            day -= dim;
            carry = 1;
        }
        temp = month + carry;
        month = impl_modulo3(temp, 1, 13);
        year += impl_fQuotient3(temp, 1, 13);
    }

    dateTime->date.day = (afw_uint8_t)day;
    dateTime->date.month = (afw_uint8_t)month;
    if (year > AFW_INT32_MAX || year < AFW_INT32_MIN) {
        AFW_THROW_ERROR_Z(general, "Year limit exceeded", xctx);
    }
    dateTime->date.year = (afw_int32_t)year;
}


/* Add yearMonthDuration to dateTime. */
AFW_DEFINE(void)
afw_dateTime_add_yearMonthDuration(
    afw_dateTime_t *dateTime,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx)
{
    afw_integer_t year, month;
    int days_in_month;

    year = dateTime->date.year + yearMonthDuration->years;
    month = dateTime->date.month + yearMonthDuration->months;

    year += impl_fQuotient3(month, 1, 13);
    month = impl_modulo3(month, 1, 13);
    
    if (year < AFW_INT32_MIN || year > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "year limit exceeded", xctx);
    }

    dateTime->date.year = (afw_int32_t)year;
    dateTime->date.month = (afw_int8_t)month;
    days_in_month = afw_date_maximum_days_in_month(year, month);
    if (dateTime->date.day > days_in_month) {
        dateTime->date.day = (afw_int8_t)days_in_month;
    }
}


/* Subtract yearMonthDuration from dateTime. */
AFW_DEFINE(void)
afw_dateTime_subtract_yearMonthDuration(
    afw_dateTime_t *dateTime,
    const afw_yearMonthDuration_t *yearMonthDuration,
    afw_xctx_t *xctx)
{
    afw_integer_t year, month;
    int days_in_month;

    year = dateTime->date.year - yearMonthDuration->years;
    month = dateTime->date.month - yearMonthDuration->months;

    year += impl_fQuotient3(month, 1, 13);
    month = impl_modulo3(month, 1, 13);
    
    if (year < AFW_INT32_MIN || year > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "year limit exceeded", xctx);
    }

    dateTime->date.year = (afw_int32_t)year;
    dateTime->date.month = (afw_int8_t)month;
    days_in_month = afw_date_maximum_days_in_month(year, month);
    if (dateTime->date.day > days_in_month) {
        dateTime->date.day = (afw_int8_t)days_in_month;
    }
}


/* Convert internal dateTime to generalized time (see X.208) */
AFW_DEFINE(const afw_utf8_t *)
afw_dateType_internal_to_generalized_time(
    const afw_dateTime_t *dateTime,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_octet_t to[IMPL_MAX_DATE_TIME_LEN];
    afw_size_t len;

    len = impl_set_internal_to_utf8_date(&dateTime->date, &to[0], xctx);
    if (len != 4) {
        AFW_THROW_ERROR_Z(general,
            "afw_dateType_internal_to_generalized_time() "
            "can not convert to > 4 digit years",
            xctx);
    }
    len += impl_set_internal_to_utf8_time(&dateTime->time, &to[len], xctx);
    len += impl_set_internal_to_utf8_time_zone(&dateTime->time_zone, &to[len],
        xctx);

    return afw_utf8_create_copy(&to[0], len, p, xctx);
}


/* Convert generalized time (see X.208) to internal and set. */
AFW_DEFINE(void)
afw_dataType_generalized_time_set_internal(
    const afw_utf8_t *generalized_time, afw_dateTime_t *internal,
    afw_xctx_t *xctx)
{
    afw_utf8_octet_t wa[IMPL_MAX_DATE_TIME_LEN];
    const afw_utf8_octet_t *i;
    afw_utf8_t utf8;
    afw_utf8_octet_t *o;
    afw_size_t len;

    len = generalized_time->len;
    i = generalized_time->s;
    o = &wa[0];
    if (len < sizeof("yyyymmddhhmmss") - 1) goto error;
    utf8.s = o;

    *o++ = *i++;
    *o++ = *i++;
    *o++ = *i++;
    *o++ = *i++;
    *o++ = '-';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = '-';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = 'T';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = ':';
    *o++ = *i++;
    *o++ = *i++;
    *o++ = ':';
    *o++ = *i++;
    *o++ = *i++;

    utf8.len = o - &wa[0];
    len -= sizeof("yyyymmddhhmmss") - 1;
    if (len > 0) {
        if (utf8.len + len > IMPL_MAX_DATE_TIME_LEN) goto error;
        memcpy(o, i, len);
        utf8.len += len;
    }

    afw_dateTime_utf8_set_internal(&utf8, internal, xctx);

    return;

error:
    AFW_THROW_ERROR_Z(general, "Invalid generalized time", xctx);
}

// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Time Support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_time.c
 * @brief Date/time parse, format, and arithmetic helpers.
 */

#include "afw_internal.h"

#define IMPL_IS_DIGIT(_n) ((_n) >= '0' && (_n) <= '9')

#define IMPL_SAFE_DIGIT(_c, _to, _error) \
do { \
    _to = _c - '0'; \
    if (_to < 0 || _to > 9) goto _error; \
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


static void
impl_normalize_dayTimeDuration(
    const afw_dayTimeDuration_t *from,
    afw_dayTimeDuration_t *to,
    afw_xctx_t *xctx);


/* Get now time as dateTime in specified pool. */
AFW_DEFINE_STATIC_INLINE(void)
impl_dateTime_exploded_set(
    afw_dateTime_t *dateTime, const afw_os_time_exploded_t *tm,
    afw_xctx_t *xctx)
{
    (void)xctx;

    /* Make sure any sluff is cleared. */
    memset(dateTime, 0, sizeof(*dateTime));

    /* Fill in dateTime. */
    dateTime->date.year = tm->year;
    dateTime->date.month = tm->month;
    dateTime->date.day = tm->day;
    dateTime->time.hour = tm->hour;
    dateTime->time.minute = tm->minute;
    dateTime->time.second = tm->second;
    dateTime->time.microsecond = tm->microsecond;
    /* Both parts have the sign of the offset (-00:30 is 0 and -30). */
    dateTime->time_zone.hours = tm->gmtoff / 3600;
    dateTime->time_zone.minutes = tm->gmtoff / 60 % 60;
}


/* Get now time as time only in specified pool. */
AFW_DEFINE_STATIC_INLINE(void)
impl_time_exploded_set(
    afw_time_t *time, const afw_os_time_exploded_t *tm,
    afw_xctx_t *xctx)
{
    (void)xctx;

    /* Make sure any sluff is cleared. */
    memset(time, 0, sizeof(*time));

    /* Fill in time. */
    time->time.hour = tm->hour;
    time->time.minute = tm->minute;
    time->time.second = tm->second;
    time->time.microsecond = tm->microsecond;
    time->time_zone.hours = tm->gmtoff / 3600;
    time->time_zone.minutes = tm->gmtoff / 60 % 60;
}


AFW_DEFINE_STATIC_INLINE(void)
impl_set_date_no_time_zone(
    afw_date_no_time_zone_t *date,
    int year,
    int month,
    int day,
    afw_xctx_t *xctx)
{
    /* An int is at most 32 bits, so every year fits. 0 is 1 BCE. */

    if (month < 1 || month > 12) {
        AFW_THROW_ERROR_Z(argument_error,
            "month must be between 1 and 12",
            xctx);
    }

    if (day < 1 || day > afw_date_maximum_days_in_month(year, month)) {
        AFW_THROW_ERROR_Z(argument_error,
            "day must be between 1 and the number of days in the month",
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
        AFW_THROW_ERROR_Z(argument_error,
            "hour must be between 0 and 24",
            xctx);
    }

    if (hour == 24 && (minute != 0 || second != 0 || microsecond != 0)) {
        AFW_THROW_ERROR_Z(argument_error,
            "if hour is 24, minute, second, and microsecond must be 0",
            xctx);
    }

    if (minute < 0 || minute > 59) {
        AFW_THROW_ERROR_Z(argument_error,
            "minute must be between 0 and 59",
            xctx);
    }

    if (second < 0 || second > 59) {
        AFW_THROW_ERROR_Z(argument_error,
            "second must be between 0 and 59",
            xctx);
    }

    if (microsecond < 0 || microsecond > 999999) {
        AFW_THROW_ERROR_Z(argument_error,
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
        AFW_THROW_ERROR_Z(argument_error,
            "tz_hours_offset must be between -14 and 14",
            xctx);
    }

    if (tz_hours_offset == -14 || tz_hours_offset == 14) {
        if (tz_minutes_offset != 0) {
            AFW_THROW_ERROR_Z(argument_error,
                "if tz_hours_offset is -14 or 14, "
                "tz_minutes_offset must be 0",
                xctx);
        }
    }

    if (tz_minutes_offset == AFW_TIME_ZONE_NONE) {
        if (tz_hours_offset != 0) {
            AFW_THROW_ERROR_Z(argument_error,
                "if tz_minutes_offset is AFW_TIME_ZONE_NONE, "
                "tz_hours_offset must be 0",
                xctx);
        }
    }

    else if (tz_minutes_offset < -59 || tz_minutes_offset > 59 ||
        (tz_hours_offset < 0 && tz_minutes_offset > 0) ||
        (tz_hours_offset > 0 && tz_minutes_offset < 0))
    {
        AFW_THROW_ERROR_Z(argument_error,
            "tz_minutes_offset must be between -59 and 59, with the "
            "sign of tz_hours_offset",
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





/* Set afw_dateTime_t from os time (microseconds since epoch). */
AFW_DEFINE(void)
afw_dateTime_set_from_os_time(
    afw_dateTime_t *dateTime,
    afw_os_time_t t,
    afw_xctx_t *xctx)
{
    afw_os_time_exploded_t tm;

    afw_os_time_explode_local(&tm, t, xctx);
    impl_dateTime_exploded_set(dateTime, &tm, xctx);
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
        AFW_THROW_ERROR_Z(argument_error,
            "days parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (hours < 0 || hours > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(argument_error,
            "hours parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (minutes < 0 || minutes > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(argument_error,
            "minutes parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (seconds < 0 || seconds > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(argument_error,
            "seconds parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (microseconds < 0 || microseconds > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(argument_error,
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
        AFW_THROW_ERROR_Z(argument_error,
            "years parameter must be between 0 and "
            AFW_STRINGIFY(AFW_INT32_MAX),
            xctx);
    }

    if (months < 0 || months > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(argument_error,
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
    afw_os_time_exploded_t tm;
    afw_os_time_t now;

    /* Make sure both will get same time. */
    now = afw_os_time_now();

    /* If requested, set local dateTime to now. */
    if (dateTime_local) {
        afw_os_time_explode_local(&tm, now, xctx);
        impl_dateTime_exploded_set(dateTime_local, &tm, xctx);
    }

    /* If requested, set utc dateTime to now. */
    if (dateTime_utc) {
        afw_os_time_explode_utc(&tm, now, xctx);
        impl_dateTime_exploded_set(dateTime_utc, &tm, xctx);
    }
}


/* Get now time as time only in specified pool. */
AFW_DEFINE(void)
afw_time_set_now(
    afw_time_t *time_local, afw_time_t *time_utc,
    afw_xctx_t *xctx)
{
    afw_os_time_exploded_t tm;
    afw_os_time_t now;

    /* Make sure both will get same time. */
    now = afw_os_time_now();

    /* If requested, set local time. */
    if (time_local) {
        afw_os_time_explode_local(&tm, now, xctx);
        impl_time_exploded_set(time_local, &tm, xctx);
    }

    /* If requested, set utc time. */
    if (time_utc) {
        afw_os_time_explode_utc(&tm, now, xctx);
        impl_time_exploded_set(time_utc, &tm, xctx);
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
    afw_os_time_exploded_t tm;

    /* Explode now time. */
    afw_os_time_explode_local(&tm, afw_os_time_now(), xctx);

    /* Set time. */
    impl_time_exploded_set(time, &tm, xctx);
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


/* Time zone offset east of UTC in minutes (0 when there is no time zone). */
AFW_DEFINE(afw_integer_t)
afw_time_zone_offset_minutes(const afw_time_zone_t *time_zone)
{
    if (AFW_TIME_ZONE_IS_NONE(time_zone)) {
        return 0;
    }
    /* Hours and minutes have the same sign. */
    return (afw_integer_t)time_zone->hours * 60 + time_zone->minutes;
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

    result -= AFW_TIME_MINUTES_TO_MICROSECONDS(
        afw_time_zone_offset_minutes(&time->time_zone));

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
    afw_int32_t x;

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
    int hours, minutes;

    c = to;

    if (AFW_TIME_ZONE_IS_NONE(tz)) return 0;

    /* If hours and minutes is 0, time zone is 'Z'. */
    if (tz->hours == 0 && tz->minutes == 0) {
        *to = 'Z';
        return 1;
    }

    /* Hours and minutes have the sign of the time zone. */
    if (tz->hours < 0 || tz->minutes < 0) {
        *c++ = '-';
        hours = -tz->hours;
        minutes = -tz->minutes;
    }
    else {
        *c++ = '+';
        hours = tz->hours;
        minutes = tz->minutes;
    }

    /* hh:mm */
    *c++ = hours % 100 / 10 + '0';
    *c++ = hours % 10 + '0';
    *c++ = ':';
    *c++ = minutes % 100 / 10 + '0';
    *c++ = minutes % 10 + '0';

    /* Return len of time zone. */
    return (afw_size_t)(c - to);
}


static afw_size_t
impl_utf8_date_set_internal(
    const afw_utf8_octet_t *from, afw_size_t len,
    afw_date_no_time_zone_t *to, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *digits;
    afw_boolean_t is_negative;

    /* Clear any sluff in result and initialize locals. */
    memset(to, 0, sizeof(*to));
    s = from;
    is_negative = false;

    /* Error if not minimal size. */
    if (len < sizeof("yyyy-mm-dd") - 1) goto error;

    /* Can begin with a '-' (not '+' by specification) */
    if (*s == '-') {
        is_negative = true;
        s++;
    }

    /*
     * Convert year: at least 4 digits, and no leading 0 when there are
     * more than 4. 0000 is the year 1 BCE (XML Schema 1.1), and -0000 is
     * not a year.
     */
    digits = s;
    for (; (afw_size_t)(s - from) < len; s++) {
        if (*s < '0' || *s > '9') break;
        /* Check first: signed overflow is undefined. */
        if (to->year > (AFW_INT32_MAX - (*s - '0')) / 10) goto error;
        to->year = to->year * 10 + (*s - '0');
    }
    if (s - digits < 4 || (s - digits > 4 && *digits == '0')) goto error;
    if (is_negative) {
        if (to->year == 0) goto error;
        to->year = -to->year;
    }

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
    AFW_THROW_ERROR_Z(conversion_error, "date invalid", xctx);
}

static afw_size_t
impl_set_utf8_time_to_internal(
    const afw_utf8_octet_t *from, afw_size_t len,
    afw_time_no_time_zone_t *to, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;
    char d1, d2;
    afw_uint32_t scale;

    memset(to, 0, sizeof(*to));
    s = from;
    end = from + len;

    /* hh ':' mm ':' ss ('.' s+)? */
    if (len < sizeof("hh:mm:ss") - 1) goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    to->hour = d1 * 10 + d2;
    if (*s++ != ':') goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    to->minute = d1 * 10 + d2;
    if (*s++ != ':') goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    to->second = d1 * 10 + d2;

    /*
     * Fractional seconds are kept to the microsecond. More digits are
     * read only if they are 0, so no value is rounded.
     */
    if (s < end && *s == '.') {
        s++;
        if (s >= end || !IMPL_IS_DIGIT(*s)) goto error;
        for (scale = 100000; s < end && IMPL_IS_DIGIT(*s); s++) {
            if (scale > 0) {
                to->microsecond += (*s - '0') * scale;
                scale /= 10;
            }
            else if (*s != '0') {
                goto error;
            }
        }
    }

    /* 24:00:00 is allowed; it is the start of the next day. */
    if (to->hour == 24) {
        if (to->minute != 0 || to->second != 0 || to->microsecond != 0) {
            goto error;
        }
    }
    else if (to->hour > 24 || to->minute >= 60 || to->second >= 60) {
        goto error;
    }

    /* Return length remaining. */
    return (afw_size_t)(end - s);

error:
    AFW_THROW_ERROR_Z(conversion_error, "time invalid", xctx);
}


static afw_size_t
impl_set_utf8_time_zone_to_internal(
    const afw_utf8_octet_t *from, afw_size_t len,
    afw_time_zone_t *to, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    char d1, d2;
    int hours, minutes;
    afw_boolean_t negative;

    to->hours = 0;
    to->minutes = AFW_TIME_ZONE_NONE;
    s = from;

    /* No time zone. Anything other than one is left for the caller. */
    if (len <= 0 || (*s != 'Z' && *s != '+' && *s != '-')) return len;

    /* 'Z' | ('+' | '-') hh ':' mm */
    if (*s == 'Z') {
        to->minutes = 0;
        return len - 1;
    }
    if (len < sizeof("+hh:mm") - 1) goto error;
    negative = (*s++ == '-');
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    hours = d1 * 10 + d2;
    if (*s++ != ':') goto error;
    IMPL_SAFE_DIGIT(*s++, d1, error);
    IMPL_SAFE_DIGIT(*s++, d2, error);
    minutes = d1 * 10 + d2;
    if (hours > 14 || minutes > 59 || (hours == 14 && minutes != 0)) {
        goto error;
    }

    /* Both parts have the sign of the time zone; -00:00 is 'Z'. */
    to->hours = (afw_int8_t)(negative ? -hours : hours);
    to->minutes = (afw_int8_t)(negative ? -minutes : minutes);
    return len - (sizeof("+hh:mm") - 1);

error:
    AFW_THROW_ERROR_Z(conversion_error, "time zone invalid.  "
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

    return afw_utf8_create(&to[0], len, p, xctx);
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
        AFW_THROW_ERROR_Z(conversion_error, "date invalid", xctx);
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

    return afw_utf8_create(&to[0], len, p, xctx);
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

    /* 24:00:00 is 00:00:00 of the next day (XML Schema 1.1). */
    if (internal->time.hour == 24) {
        afw_dayTimeDuration_t one_day;

        afw_memory_clear(&one_day);
        one_day.days = 1;
        internal->time.hour = 0;
        afw_dateTime_add_dayTimeDuration(internal, &one_day, xctx);
    }

    return;

error:
    AFW_THROW_ERROR_Z(conversion_error, "dateTime invalid", xctx);
}


/*
 * Read the digits of a duration part (leading zeros allowed) into *value
 * and return how many there were. A value past AFW_INT32_MAX reads as no
 * digits, with *s unchanged, so the caller rejects the text.
 */
static afw_size_t
impl_parse_duration_digits(
    const afw_utf8_octet_t **s,
    const afw_utf8_octet_t *end,
    afw_integer_t *value)
{
    const afw_utf8_octet_t *c;
    afw_size_t count;

    *value = 0;
    for (c = *s; c < end && IMPL_IS_DIGIT(*c); c++) {
        *value = *value * 10 + (*c - '0');
        if (*value > AFW_INT32_MAX) {
            *value = 0;
            return 0;
        }
    }
    count = (afw_size_t)(c - *s);
    *s = c;
    return count;
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
    afw_dayTimeDuration_t normalized;

    /* Initialize locals. */
    c = s = &wa[0];
    end = &wa[sizeof(wa)];

    /* The canonical form carries each part into the next (XML Schema). */
    impl_normalize_dayTimeDuration(dayTimeDuration, &normalized, xctx);
    dayTimeDuration = &normalized;

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

    /* Zero is "PT0S" ("P" alone is not a dayTimeDuration). */
    if (days == 0 && hours == 0 && minutes == 0 && seconds == 0 &&
        microseconds == 0)
    {
        *c++ = 'T';
        *c++ = '0';
        *c++ = 'S';
    }

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
    return afw_utf8_create(s, c - s, p, xctx);
}


/* Convert utf8 xml schema dayTimeDuration to internal and set. */
AFW_DEFINE(void)
afw_dayTimeDuration_utf8_set_internal(
    const afw_utf8_t *utf8, afw_dayTimeDuration_t *internal,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;
    afw_integer_t i;
    afw_int32_t sign;
    afw_size_t digits;
    afw_boolean_t any;
    afw_uint32_t scale;

    s = utf8->s;
    end = s + utf8->len;
    memset(internal, 0, sizeof(*internal));

    /*
     * '-'? 'P' (duDayFrag duTimeFrag? | duTimeFrag), where duTimeFrag is
     * 'T' followed by hours, minutes, and seconds in order, at least one
     * of them (XML Schema 1.1).
     */
    sign = 1;
    if (s < end && *s == '-') {
        sign = -1;
        s++;
    }
    if (s >= end || *s++ != 'P') goto error;
    any = false;

    digits = impl_parse_duration_digits(&s, end, &i);
    if (digits > 0) {
        if (s >= end || *s != 'D') goto error;
        internal->days = (afw_int32_t)(sign * i);
        s++;
        any = true;
    }

    if (s < end) {
        if (*s++ != 'T') goto error;
        any = false;

        digits = impl_parse_duration_digits(&s, end, &i);
        if (digits > 0 && s < end && *s == 'H') {
            internal->hours = (afw_int32_t)(sign * i);
            s++;
            any = true;
            digits = impl_parse_duration_digits(&s, end, &i);
        }
        if (digits > 0 && s < end && *s == 'M') {
            internal->minutes = (afw_int32_t)(sign * i);
            s++;
            any = true;
            digits = impl_parse_duration_digits(&s, end, &i);
        }

        /* Seconds: digits, digits '.' digits?, or '.' digits, then 'S'. */
        if (digits > 0 || (s < end && *s == '.')) {
            internal->seconds = (afw_int32_t)(sign * i);
            if (s < end && *s == '.') {
                s++;
                if (digits == 0 && (s >= end || !IMPL_IS_DIGIT(*s))) {
                    goto error;
                }
                /* Kept to the microsecond; more digits only if 0. */
                for (scale = 100000; s < end && IMPL_IS_DIGIT(*s); s++) {
                    if (scale > 0) {
                        internal->microseconds += sign * (*s - '0') *
                            (afw_int32_t)scale;
                        scale /= 10;
                    }
                    else if (*s != '0') {
                        goto error;
                    }
                }
            }
            if (s >= end || *s++ != 'S') goto error;
            any = true;
        }
    }

    if (!any || s != end) goto error;
    return;

error:
    AFW_THROW_ERROR_Z(conversion_error, "dayTimeDuration invalid", xctx);
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

    return afw_utf8_create(&to[0], len, p, xctx);
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
        AFW_THROW_ERROR_Z(conversion_error, "time invalid", xctx);
    }

    /* 24:00:00 is 00:00:00 (XML Schema 1.1). */
    if (internal->time.hour == 24) {
        internal->time.hour = 0;
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
    afw_integer_t years, months, total;

    /* Initialize locals. */
    c = s = &wa[0];
    end = &wa[sizeof(wa)];

    /*
     * The canonical form carries months into years (P1Y13M is P2Y1M).
     * Both members have the same sign.
     */
    total = (afw_integer_t)yearMonthDuration->years * 12 +
        yearMonthDuration->months;
    if (total < 0) {
        total = -total;
        *c++ = '-';
    }
    years = total / 12;
    months = total % 12;

    /* Next is always 'P'. */
    *c++ = 'P';

    /* Zero is "P0M" ("P" alone is not a yearMonthDuration). */
    if (years == 0 && months == 0) {
        *c++ = '0';
        *c++ = 'M';
    }

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
    return afw_utf8_create(s, c - s, p, xctx);
}


/* Convert utf8 xml schema yearMonthDuration to internal and set. */
AFW_DEFINE(void)
afw_yearMonthDuration_utf8_set_internal(
    const afw_utf8_t *utf8, afw_yearMonthDuration_t *internal,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;
    afw_integer_t i;
    afw_int32_t sign;
    afw_size_t digits;

    s = utf8->s;
    end = s + utf8->len;
    memset(internal, 0, sizeof(*internal));

    /* '-'? 'P' ((digits 'Y' (digits 'M')?) | digits 'M') */
    sign = 1;
    if (s < end && *s == '-') {
        sign = -1;
        s++;
    }
    if (s >= end || *s++ != 'P') goto error;

    digits = impl_parse_duration_digits(&s, end, &i);
    if (digits == 0 || s >= end) goto error;
    if (*s == 'Y') {
        internal->years = (afw_int32_t)(sign * i);
        s++;
        if (s == end) return;
        digits = impl_parse_duration_digits(&s, end, &i);
        if (digits == 0 || s >= end) goto error;
    }
    if (*s++ != 'M' || s != end) goto error;
    internal->months = (afw_int32_t)(sign * i);
    return;

error:
    AFW_THROW_ERROR_Z(conversion_error, "yearMonthDuration invalid", xctx);
}


/*
 * Days from 1970-01-01 to a civil date (proleptic Gregorian, negative
 * years allowed; H. Hinnant's days_from_civil). Used to order instants.
 */
static afw_integer_t
impl_days_from_civil(afw_integer_t y, afw_integer_t m, afw_integer_t d)
{
    afw_integer_t era, yoe, doy, doe;

    y -= (m <= 2) ? 1 : 0;
    era = (y >= 0 ? y : y - 399) / 400;
    yoe = y - era * 400;
    doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}


/* The civil date of a day from 1970-01-01 (inverse of days_from_civil). */
static void
impl_civil_from_days(afw_integer_t z,
    afw_integer_t *year, afw_integer_t *month, afw_integer_t *day)
{
    afw_integer_t era, doe, yoe, doy, mp;

    z += 719468;
    era = (z >= 0 ? z : z - 146096) / 146097;
    doe = z - era * 146097;
    yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    mp = (5 * doy + 2) / 153;
    *day = doy - (153 * mp + 2) / 5 + 1;
    *month = mp + (mp < 10 ? 3 : -9);
    *year = yoe + era * 400 + (*month <= 2 ? 1 : 0);
}


/*
 * Offset in minutes of a time zone, or of the implicit time zone (the local
 * time zone when xctx was created) if there is none, as XACML and XPath
 * compare a value without a time zone. Two values without a time zone get
 * the same offset, so they compare as written.
 */
static afw_integer_t
impl_offset_minutes_or_implicit(
    const afw_time_zone_t *time_zone, afw_xctx_t *xctx)
{
    if (AFW_TIME_ZONE_IS_NONE(time_zone)) {
        time_zone = &xctx->local_dateTime_when_created.time_zone;
    }
    return afw_time_zone_offset_minutes(time_zone);
}


/* Compare two dates by the instant each starts. */
AFW_DEFINE(int)
afw_date_compare(const afw_date_t *v1, const afw_date_t *v2,
    afw_xctx_t *xctx)
{
    afw_integer_t m1, m2;

    m1 = impl_days_from_civil(v1->date.year, v1->date.month,
        v1->date.day) * 24 * 60 -
        impl_offset_minutes_or_implicit(&v1->time_zone, xctx);
    m2 = impl_days_from_civil(v2->date.year, v2->date.month,
        v2->date.day) * 24 * 60 -
        impl_offset_minutes_or_implicit(&v2->time_zone, xctx);

    return (m1 == m2) ? 0 : (m1 > m2) ? 1 : -1;
}


AFW_DEFINE(int)
afw_dateTime_compare(const afw_dateTime_t *v1, const afw_dateTime_t *v2,
    afw_xctx_t *xctx)
{
    afw_integer_t s1, s2, result;

    /*
     * Compare the instants: seconds since the epoch in UTC, then
     * microseconds. A value without a time zone has the implicit one.
     */
    s1 = (((impl_days_from_civil(v1->date.year, v1->date.month,
        v1->date.day) * 24 + v1->time.hour) * 60 + v1->time.minute -
        impl_offset_minutes_or_implicit(&v1->time_zone, xctx)) * 60) +
        v1->time.second;
    s2 = (((impl_days_from_civil(v2->date.year, v2->date.month,
        v2->date.day) * 24 + v2->time.hour) * 60 + v2->time.minute -
        impl_offset_minutes_or_implicit(&v2->time_zone, xctx)) * 60) +
        v2->time.second;
    result = s1 - s2;
    if (result == 0) {
        result = (afw_integer_t)v1->time.microsecond -
            (afw_integer_t)v2->time.microsecond;
    }

    return (result == 0) ? 0 : (result > 0) ? 1 : -1;
}


AFW_DEFINE(int)
afw_time_compare(const afw_time_t *v1, const afw_time_t *v2,
    afw_xctx_t *xctx)
{
    afw_integer_t s1, s2, result;

    /*
     * Compare as UTC on one reference day (the offset is subtracted, and
     * not wrapped, as XPath compares times on 1972-12-31). A value without
     * a time zone has the implicit one.
     */
    s1 = ((afw_integer_t)v1->time.hour * 60 + v1->time.minute -
        impl_offset_minutes_or_implicit(&v1->time_zone, xctx)) * 60 +
        v1->time.second;
    s2 = ((afw_integer_t)v2->time.hour * 60 + v2->time.minute -
        impl_offset_minutes_or_implicit(&v2->time_zone, xctx)) * 60 +
        v2->time.second;
    result = s1 - s2;
    if (result == 0) {
        result = (afw_integer_t)v1->time.microsecond -
            (afw_integer_t)v2->time.microsecond;
    }

    return (result == 0) ? 0 : (result > 0) ? 1 : -1;
}


AFW_DEFINE_STATIC_INLINE(afw_integer_t)
impl_fQuotient2(afw_integer_t a, unsigned int b)
{
    afw_integer_t result;

    /*
     * Floor of a / b. C division truncates toward zero, so a negative a
     * needs b - 1 more (not b: -60 / 60 is -1, not -2, which made
     * modulo give 60 seconds, hour 24, or month 13).
     */
    if (a < 0) {
        result = (a - ((afw_integer_t)b - 1)) / (afw_integer_t)b;
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
    afw_integer_t seconds, minutes, hours, days;

    /*
     * Carry each part into the next (PT36H is P1DT12H, PT90S is PT1M30S).
     * All parts have the same sign, so truncating division carries
     * correctly for a negative duration.
     */
    seconds = (afw_integer_t)from->seconds + from->microseconds / 1000000;
    minutes = (afw_integer_t)from->minutes + seconds / 60;
    hours = (afw_integer_t)from->hours + minutes / 60;
    days = (afw_integer_t)from->days + hours / 24;
    if (days < AFW_INT32_MIN || days > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "dayTimeDuration limit exceeded", xctx);
    }

    to->microseconds = from->microseconds % 1000000;
    to->seconds = (afw_int32_t)(seconds % 60);
    to->minutes = (afw_int32_t)(minutes % 60);
    to->hours = (afw_int32_t)(hours % 24);
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

    year = (afw_integer_t)date->date.year +
        yearMonthDuration->years;
    month = (afw_integer_t)date->date.month +
        yearMonthDuration->months;

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

    year = (afw_integer_t)date->date.year -
        yearMonthDuration->years;
    month = (afw_integer_t)date->date.month -
        yearMonthDuration->months;

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

    /* time.microsecond is unsigned; widen it before adding a negative. */
    temp = (afw_integer_t)dateTime->time.microsecond +
        dayTimeDuration->microseconds;
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
    impl_civil_from_days(
        impl_days_from_civil(year, month, day) + dayTimeDuration->days + carry,
        &year, &month, &day);

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

    temp = (afw_integer_t)dateTime->time.microsecond -
        dayTimeDuration->microseconds;
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
    impl_civil_from_days(
        impl_days_from_civil(year, month, day) - dayTimeDuration->days + carry,
        &year, &month, &day);

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

    year = (afw_integer_t)dateTime->date.year +
        yearMonthDuration->years;
    month = (afw_integer_t)dateTime->date.month +
        yearMonthDuration->months;

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

    year = (afw_integer_t)dateTime->date.year -
        yearMonthDuration->years;
    month = (afw_integer_t)dateTime->date.month -
        yearMonthDuration->months;

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

    return afw_utf8_create(&to[0], len, p, xctx);
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
    AFW_THROW_ERROR_Z(conversion_error, "Invalid generalized time", xctx);
}

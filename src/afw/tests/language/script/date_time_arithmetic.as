#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: date_time_arithmetic.as
//? customPurpose: Part of language/script tests
//? description: ...
dateTime and date arithmetic with durations carries and borrows exactly.
Floor division of a negative value that is an exact multiple (-60 / 60)
was one too low, so a borrow gave second 60, minute 60, hour 24, or
month 13 instead of carrying into the next unit.
//? sourceType: script
//?
//? test: subtract-day-time-duration-borrow-exact
//? description: borrowing exactly a whole unit carries instead of leaving 60 or 24
//? expect: 0
//? source: ...

function sub(t: string, d: string): string {
    return string(subtract_dayTimeDuration<dateTime>(to_dateTime(t), to_dayTimeDuration(d)));
}
function add(t: string, d: string): string {
    return string(add_dayTimeDuration<dateTime>(to_dateTime(t), to_dayTimeDuration(d)));
}
assert(sub("4813-01-26T14:59:32Z", "P189DT15H81M92S") === "4812-07-20T22:37:00Z",
    sub("4813-01-26T14:59:32Z", "P189DT15H81M92S"));
assert(sub("3933-12-15T01:34:32Z", "P318DT1H33M92S") === "3933-01-31T00:00:00Z",
    sub("3933-12-15T01:34:32Z", "P318DT1H33M92S"));
assert(add("2613-09-25T06:19:19Z", "-P165DT7H58M79S") === "2613-04-12T22:20:00Z",
    add("2613-09-25T06:19:19Z", "-P165DT7H58M79S"));
assert(add("9431-05-26T07:44:36Z", "-P30DT30H64M6S") === "9431-04-25T00:40:30Z",
    add("9431-05-26T07:44:36Z", "-P30DT30H64M6S"));
assert(sub("2020-01-01T00:00:00Z", "PT1S") === "2019-12-31T23:59:59Z");
assert(sub("2020-01-01T00:01:00Z", "PT60S") === "2020-01-01T00:00:00Z");
assert(sub("2020-03-01T00:00:00Z", "PT24H") === "2020-02-29T00:00:00Z");
return 0;

//?
//? test: subtract-year-month-duration-borrow-exact
//? description: borrowing a whole year of months gives month 12 of the year before, not month 13
//? expect: 0
//? source: ...

function ym(t: string, d: string): string {
    return string(add_yearMonthDuration<dateTime>(to_dateTime(t), to_yearMonthDuration(d)));
}
assert(ym("9036-10-24T01:57:16Z", "-P2Y21M") === "9033-01-24T01:57:16Z",
    ym("9036-10-24T01:57:16Z", "-P2Y21M"));
assert(ym("2020-12-15T00:00:00Z", "-P12M") === "2019-12-15T00:00:00Z",
    ym("2020-12-15T00:00:00Z", "-P12M"));
assert(string(add_yearMonthDuration<date>("2020-12-15"date, "-P1Y"yearMonthDuration)) ===
    "2019-12-15");
assert(string(subtract_yearMonthDuration<date>("2020-12-15"date, "P12M"yearMonthDuration)) ===
    "2019-12-15");
return 0;

//?
//? test: compare-date-time-across-time-zones
//? description: equal instants in different time zones are equal, and order is by instant
//? expect: 0
//? source: ...

assert("1932-04-02T02:54:04+05:30"dateTime == "1932-04-01T16:24:04-05:00"dateTime, "+05:30 vs -05:00");
assert("2046-01-02T18:08:37+14:00"dateTime == "2046-01-01T23:08:37-05:00"dateTime, "+14:00 vs -05:00");
assert("1921-03-25T14:54:59Z"dateTime == "1921-03-25T20:24:59+05:30"dateTime, "Z vs +05:30");
assert("2020-01-01T00:30:00+01:00"dateTime < "2019-12-31T23:45:00Z"dateTime, "earlier instant, later date");
assert("2019-12-31T23:45:00Z"dateTime > "2020-01-01T00:30:00+01:00"dateTime);
assert("2020-01-01T00:00:00.5Z"dateTime > "2020-01-01T01:00:00+01:00"dateTime, "microseconds");
return 0;

//?
//? test: compare-time-across-time-zones
//? description: times with time zones compare as UTC
//? expect: 0
//? source: ...

assert("12:00:00+05:30"time == "06:30:00Z"time, "+05:30");
assert("12:00:00-05:00"time == "17:00:00Z"time, "-05:00");
assert("12:00:00-05:00"time > "12:00:00Z"time, "later in UTC");
assert("10:00:00+01:00"time < "09:30:00Z"time, "09:00Z < 09:30Z");
return 0;

//?
//? test: in-range-time-negative-half-hour-offset
//? description: in_range<time> converts a -05:30 time to UTC like compare does (it added the 30 minutes instead of subtracting them)
//? expect: 0
//? source: ...

assert("12:00:00-05:30"time == "17:30:00Z"time, "compare");
assert(in_range<time>("05:00:00-05:30"time, "10:20:00Z"time, "10:40:00Z"time), "-05:30 in range");
assert(!in_range<time>("05:00:00-05:30"time, "09:20:00Z"time, "09:40:00Z"time), "-05:30 not at 09:30Z");
assert(in_range<time>("05:00:00+05:30"time, "23:20:00Z"time, "23:40:00Z"time), "+05:30 in range");
return 0;

//?
//? test: compare-day-time-duration-normalized
//? description: durations compare by length, however they are spelled, including fractional seconds
//? expect: 0
//? source: ...

assert("P1DT32H78M108S"dayTimeDuration > "P2DT1H"dayTimeDuration, "hours past a day");
assert("P1DT32H"dayTimeDuration == "P2DT8H"dayTimeDuration, "PT32H in P1D");
assert("P3DT17H58M26.945215S"dayTimeDuration == "PT323906.945215S"dayTimeDuration, "all seconds");
assert("-P1DT45H"dayTimeDuration < "-P2DT20H"dayTimeDuration, "negative");
assert("PT1.5S"dayTimeDuration < "PT1.6S"dayTimeDuration, "microseconds");
assert(!("PT1.5S"dayTimeDuration == "PT1.6S"dayTimeDuration), "microseconds differ");
return 0;

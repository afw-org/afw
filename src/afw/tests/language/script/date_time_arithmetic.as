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
    return string(subtract_dayTimeDuration<dateTime>(dateTime(t), dayTimeDuration(d)));
}
function add(t: string, d: string): string {
    return string(add_dayTimeDuration<dateTime>(dateTime(t), dayTimeDuration(d)));
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
    return string(add_yearMonthDuration<dateTime>(dateTime(t), yearMonthDuration(d)));
}
assert(ym("9036-10-24T01:57:16Z", "-P2Y21M") === "9033-01-24T01:57:16Z",
    ym("9036-10-24T01:57:16Z", "-P2Y21M"));
assert(ym("2020-12-15T00:00:00Z", "-P12M") === "2019-12-15T00:00:00Z",
    ym("2020-12-15T00:00:00Z", "-P12M"));
assert(string(add_yearMonthDuration<date>(date("2020-12-15"), yearMonthDuration("-P1Y"))) ===
    "2019-12-15");
assert(string(subtract_yearMonthDuration<date>(date("2020-12-15"), yearMonthDuration("P12M"))) ===
    "2019-12-15");
return 0;

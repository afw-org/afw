#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: date_time_xml_schema.as
//? customPurpose: Part of language/script tests
//? description: date, dateTime, time, and duration text, comparison, and arithmetic follow XML Schema 1.1
//? sourceType: script
//?
//? test: read-dateTime
//? description: what dateTime text reads as, and text that is not a dateTime
//? expect: 0
//? source: ...

function text(f: function): string {
    try {
        return to_string(f());
    }
    catch (e) {
        return "error";
    }
}

const rows = [
    // 24:00:00 is the start of the next day.
    ["2026-12-31T24:00:00Z", "2027-01-01T00:00:00Z"],
    ["2026-10-12T24:00:01Z", "error"],
    // Years: at least 4 digits, no extra leading 0, 0000 is 1 BCE, no -0000.
    ["0000-01-01T00:00:00Z", "0000-01-01T00:00:00Z"],
    ["-0001-01-01T00:00:00Z", "-0001-01-01T00:00:00Z"],
    ["10000-01-01T00:00:00Z", "10000-01-01T00:00:00Z"],
    ["012-01-01T00:00:00Z", "error"],
    ["01234-01-01T00:00:00Z", "error"],
    ["-0000-01-01T00:00:00Z", "error"],
    // hh:mm:ss is required.
    ["2026-10-12T01:02Z", "error"],
    // A time zone is 'Z' or +hh:mm / -hh:mm; -00:30 keeps its sign.
    ["2026-10-12T01:02:03+05", "error"],
    ["2026-10-12T01:02:03-00:30", "2026-10-12T01:02:03-00:30"],
    ["2026-10-12T01:02:03-00:00", "2026-10-12T01:02:03Z"],
    // Fractional seconds to the microsecond; more digits only if 0.
    ["2026-10-12T01:02:03.1234560Z", "2026-10-12T01:02:03.123456Z"],
    ["2026-10-12T01:02:03.1234567Z", "error"]
];
for (const [t, expected] of rows) {
    const actual = text(function () { return to_dateTime(t); });
    assert(actual === expected, t + " is " + actual);
}
return 0;

//?
//? test: read-time-and-date
//? description: time needs hh:mm:ss and 24:00:00 is 00:00:00; date years follow dateTime
//? expect: 0
//? source: ...

function text(f: function): string {
    try {
        return to_string(f());
    }
    catch (e) {
        return "error";
    }
}

assert(text(function () { return to_time("24:00:00"); }) === "00:00:00");
assert(text(function () { return to_time("01"); }) === "error");
assert(text(function () { return to_time("01:02"); }) === "error");
assert(text(function () { return to_time("01:02:03-00:45"); }) === "01:02:03-00:45");
assert(text(function () { return to_date("012-01-01"); }) === "error");
assert(text(function () { return to_date("2026-10-12+05"); }) === "error");
return 0;

//?
//? test: read-durations
//? description: duration parts may have leading zeros; seconds may be "1." or ".5"; canonical text carries parts
//? expect: 0
//? source: ...

function text(f: function): string {
    try {
        return to_string(f());
    }
    catch (e) {
        return "error";
    }
}
function dtd(t: string): string {
    return text(function () { return to_dayTimeDuration(t); });
}
function ymd(t: string): string {
    return text(function () { return to_yearMonthDuration(t); });
}

assert(dtd("PT01H") === "PT1H");
assert(dtd("PT.5S") === "PT0.5S");
assert(dtd("PT1.S") === "PT1S");
assert(dtd("PT36H") === "P1DT12H");
assert(dtd("PT90M") === "PT1H30M");
assert(dtd("PT1.1234560S") === "PT1.123456S");
assert(dtd("P0D") === "PT0S");
assert(dtd("-PT0.5S") === "-PT0.5S");
assert(dtd("P2147483648D") === "error");
assert(dtd("PT") === "error");
assert(dtd("P1DT") === "error");
assert(dtd("PT1M1H") === "error");
assert(ymd("P01Y") === "P1Y");
assert(ymd("P1Y13M") === "P2Y1M");
assert(ymd("P0Y") === "P0M");
assert(ymd("P1M1Y") === "error");
return 0;

//?
//? test: compare-time-zones
//? description: values compare as instants; a value without a time zone has the implicit (local) one
//? expect: 0
//? source: ...

// Dates and dateTimes in different zones, and one with a zone against one
// without: the results below hold for every implicit zone (-14:00 to +14:00).
assert(("2026-10-12+05:00"date == "2026-10-12Z"date) === false);
assert("2026-10-12+05:00"date < "2026-10-12Z"date);
assert("2026-10-12T00:00:00-00:30"dateTime == "2026-10-12T00:30:00Z"dateTime);
assert("2026-10-12T00:00:00"dateTime < "2026-10-14T00:00:00Z"dateTime);
assert("2026-10-12"date < "2026-10-14Z"date);
assert("01:00:00+05:00"time < "23:00:00"time);
assert("24:00:00"time == "00:00:00"time);
return 0;

//?
//? test: arithmetic
//? description: fractional seconds borrow correctly; limits are errors, not a wrapped year
//? expect: 0
//? source: ...

function text(f: function): string {
    try {
        return to_string(f());
    }
    catch (e) {
        return "error";
    }
}

assert(to_string(add_dayTimeDuration("2026-10-12T00:00:00Z"dateTime,
    "-PT0.5S"dayTimeDuration)) === "2026-10-11T23:59:59.5Z");
assert(to_string(subtract_dayTimeDuration("2026-10-12T00:00:00Z"dateTime,
    "PT0.5S"dayTimeDuration)) === "2026-10-11T23:59:59.5Z");
assert(to_string(add_dayTimeDuration("2024-02-28T23:59:59.75Z"dateTime,
    "P1DT0.5S"dayTimeDuration)) === "2024-03-01T00:00:00.25Z");
assert(text(function () {
    return add_yearMonthDuration("2026-10-12T00:00:00Z"dateTime,
        "P2147483647Y"yearMonthDuration);
}) === "error");
return 0;

//?
//? test: in-range-wraps-in-utc
//? description: in_range<time> brings each time into one UTC day before checking the range
//? expect: [true, false, true]
//? source: ...

[
    in_range<time>("23:30:00Z"time, "04:00:00+05:00"time, "01:00:00Z"time),
    in_range<time>("12:00:00Z"time, "04:00:00+05:00"time, "01:00:00Z"time),
    in_range<time>("03:00:00Z"time, "22:00:00Z"time, "05:00:00Z"time)
]

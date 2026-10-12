#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: to_t_from_string.as
//? customPurpose: Conversion functions
//? description: to_T<string> reads text with the data type's parser and throws on bad text (#588)
//? sourceType: script
//?
//? test: each-data-type
//? description: to_T<string> of good text is a T whose to_string is the canonical text; bad text is an error
//? expect: 0
//? source: ...

// [to_T, is_T, text, canonical text, bad text]. The parsers of anyURI,
// dnsName, ia5String, ipAddress, objectId, objectPath, password,
// rfc822Name, and x500Name accept any text today (#593), so they have no
// bad text.
const rows = [
    [to_anyURI, is_anyURI, "https://example.com/a?b=c", "https://example.com/a?b=c", []],
    [to_base64Binary, is_base64Binary, "YWJj", "YWJj", ["@@@", "YWJ"]],
    [to_date, is_date, "2026-10-12", "2026-10-12", ["nope", "2026-13-01", "2026-10-12T00:00:00"]],
    [to_dateTime, is_dateTime, "2026-10-12T01:02:03Z", "2026-10-12T01:02:03Z", ["nope", "2026-10-12"]],
    [to_dayTimeDuration, is_dayTimeDuration, "P1DT2H", "P1DT2H", ["nope", "P1Y"]],
    [to_dnsName, is_dnsName, "example.com", "example.com", []],
    [to_double, is_double, "1.5", "1.5E0", ["nope", "5", "1.5x", " 1.5"]],
    [to_hexBinary, is_hexBinary, "0a1B", "0A1B", ["zz", "0a1"]],
    [to_ia5String, is_ia5String, "abc", "abc", []],
    [to_integer, is_integer, "-12", "-12", ["nope", "1.5", " 12", "12x"]],
    [to_ipAddress, is_ipAddress, "10.0.0.1", "10.0.0.1", []],
    [to_objectId, is_objectId, "abc", "abc", []],
    [to_objectPath, is_objectPath, "/afw/_AdaptiveFunction_/add", "/afw/_AdaptiveFunction_/add", []],
    [to_password, is_password, "secret", "secret", []],
    [to_rfc822Name, is_rfc822Name, "a@example.com", "a@example.com", []],
    [to_time, is_time, "01:02:03", "01:02:03", ["nope", "25:00:00"]],
    [to_x500Name, is_x500Name, "cn=a,o=b", "cn=a,o=b", []],
    [to_yearMonthDuration, is_yearMonthDuration, "P1Y2M", "P1Y2M", ["nope", "P1D"]]
];

for (const [to_t, is_t, text, canonical, bad] of rows) {
    const v = to_t(text);
    assert(is_t(v), text);
    assert(to_string(v) === canonical, text + " " + to_string(v));
    for (const b of bad) {
        let result = "returned";
        try {
            to_t(b);
        }
        catch (e) {
            result = "threw";
        }
        assert(result === "threw", text + " bad text " + b);
    }
}
return 0;

//?
//? test: same-as-typed-literal
//? description: to_T<string> of text is the value the typed literal of that text is
//? expect: 0
//? source: ...

assert(to_anyURI("https://example.com/") === "https://example.com/"anyURI);
assert(to_base64Binary("YWJj") === "YWJj"base64Binary);
assert(to_date("2026-10-12") === "2026-10-12"date);
assert(to_dateTime("2026-10-12T01:02:03Z") === "2026-10-12T01:02:03Z"dateTime);
assert(to_dayTimeDuration("P1D") === "P1D"dayTimeDuration);
assert(to_hexBinary("0a") === "0A"hexBinary);
assert(to_ia5String("abc") === "abc"ia5String);
assert(to_time("01:02:03") === "01:02:03"time);
assert(to_yearMonthDuration("P1Y") === "P1Y"yearMonthDuration);
return 0;

//?
//? test: binary-reads-encoded-text
//? description: to_base64Binary and to_hexBinary read encoded text, unlike encode_as_*, which encode the string's bytes
//? expect: [true, false]
//? source: ...

[to_string(to_base64Binary("YWJj")) === "YWJj",
    to_string(encode_as_base64Binary("YWJj")) === "YWJj"]

//?
//? test: to_integer-double-truncates
//? description: to_integer<double> truncates toward zero
//? expect: [1, -1, 0, 0, 2]
//? source: ...

[to_integer(1.9), to_integer(-1.9), to_integer(0.5), to_integer(-0.5),
    to_integer(2.0)]

//?
//? test: to_double-integer-text
//? description: integer text is not double text; convert through to_integer
//? expect: 5.0
//? source: ...

to_double(to_integer("5"))

#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: typed_literals.as
//? customPurpose: Typed literals
//? description: "text"dataType and its short form parse text with the data type (#581)
//? sourceType: script
//?
//? test: each-data-type
//? description: every data type with a typed literal, by data type id
//? expect: 0
//? source: ...

assert(is<anyURI>("https://example.com/"anyURI));
assert("https://example.com/"anyURI == anyURI("https://example.com/"));
assert(is<base64Binary>("AQID"base64Binary));
assert("AQID"base64Binary == base64Binary("AQID"));
assert(is<date>("2026-10-11"date));
assert("2026-10-11"date == date("2026-10-11"));
assert(is<dateTime>("2026-10-11T12:00:00Z"dateTime));
assert("2026-10-11T12:00:00Z"dateTime == dateTime("2026-10-11T12:00:00Z"));
assert(is<dayTimeDuration>("P1DT2H"dayTimeDuration));
assert("P1DT2H"dayTimeDuration == dayTimeDuration("P1DT2H"));
assert(is<dnsName>("example.com"dnsName));
assert("example.com"dnsName == dnsName("example.com"));
assert(is<hexBinary>("0a1b"hexBinary));
assert("0a1b"hexBinary == hexBinary("0a1b"));
assert(is<ia5String>("abc"ia5String));
assert("abc"ia5String == ia5String("abc"));
assert(is<ipAddress>("10.0.0.1"ipAddress));
assert("10.0.0.1"ipAddress == ipAddress("10.0.0.1"));
assert(is<password>("secret"password));
assert("secret"password == password("secret"));
assert(is<rfc822Name>("a@example.com"rfc822Name));
assert("a@example.com"rfc822Name == rfc822Name("a@example.com"));
assert(is<time>("12:00:00"time));
assert("12:00:00"time == time("12:00:00"));
assert(is<x500Name>("cn=a,o=b"x500Name));
assert("cn=a,o=b"x500Name == x500Name("cn=a,o=b"));
assert(is<xpathExpression>("/a/b"xpathExpression));
assert("/a/b"xpathExpression == xpathExpression("/a/b"));
assert(is<yearMonthDuration>("P1Y2M"yearMonthDuration));
assert("P1Y2M"yearMonthDuration == yearMonthDuration("P1Y2M"));
assert(stringify("a+b"regexp) == stringify(regexp("a+b")));
return 0;

//?
//? test: short-forms
//? description: each short form gives the same value as the data type id
//? expect: 0
//? source: ...

assert("https://example.com/"uri == "https://example.com/"anyURI);
assert("AQID"b64 == "AQID"base64Binary);
assert("2026-10-11T12:00:00Z"dt == "2026-10-11T12:00:00Z"dateTime);
assert("example.com"dns == "example.com"dnsName);
assert("0a1b"hex == "0a1b"hexBinary);
assert("10.0.0.1"ip == "10.0.0.1"ipAddress);
assert("a@example.com"rfc822 == "a@example.com"rfc822Name);
assert("cn=a,o=b"dn == "cn=a,o=b"x500Name);
assert("/a/b"xpath == "/a/b"xpathExpression);
assert(stringify("a+b"re) == stringify("a+b"regexp));
return 0;

//?
//? test: suffix-registry
//? description: each suffix has an _AdaptiveTypedLiteralSuffix_ object
//? expect: 0
//? source: ...

const dt = get_object("afw", "_AdaptiveTypedLiteralSuffix_", "dt");
assert(dt.dataType === "dateTime");
assert(dt.typedLiteralShortForm === "dt");
const d = get_object("afw", "_AdaptiveTypedLiteralSuffix_", "date");
assert(d.dataType === "date");
assert(d.typedLiteralShortForm === undefined);
return 0;

//?
//? test: retrieve-suffixes
//? description: retrieve /afw/_AdaptiveTypedLiteralSuffix_ matches the data types with typedLiteral
//? expect: 0
//? source: ...

let expected = [];
for (const t of retrieve_objects("afw", "_AdaptiveDataType_")) {
    if (t.typedLiteral) {
        expected = [...expected, t.dataType];
        if (t.typedLiteralShortForm !== undefined) {
            expected = [...expected, t.typedLiteralShortForm];
        }
    }
}

let ids = [];
for (const o of retrieve_objects("afw", "_AdaptiveTypedLiteralSuffix_",
    undefined, {"objectId": true}))
{
    const id = meta(o).objectId;
    assert(id === o.dataType || id === o.typedLiteralShortForm, id);
    assert(get_object("afw", "_AdaptiveDataType_", o.dataType).typedLiteral,
        id);
    ids = [...ids, id];
}
assert(length(ids) === length(expected), "count");
for (const id of expected) {
    assert(includes(ids, id), id);
}
assert(!includes(ids, "integer"), "integer");
assert(!includes(ids, "string"), "string");

let dateTimeIds = [];
for (const o of retrieve_objects("afw", "_AdaptiveTypedLiteralSuffix_",
    {"urlEncodedRQLString": "dataType=dateTime"}, {"objectId": true}))
{
    dateTimeIds = [...dateTimeIds, meta(o).objectId];
}
assert(length(dateTimeIds) === 2, "dateTime count");
assert(includes(dateTimeIds, "dateTime") && includes(dateTimeIds, "dt"),
    "dateTime ids");
return 0;

//?
//? test: contexts
//? description: single quotes, escapes, arrays, objects, switch, templates
//? expect: 0
//? source: ...

assert('2026-10-11'date == "2026-10-11"date);
assert("\x32026-10-11"date == "2026-10-11"date);
const a = ["2026-10-11"date, "P1D"dayTimeDuration];
assert(is<date>(a[0]) && is<dayTimeDuration>(a[1]));
const o = {when: "2026-10-11T12:00:00Z"dt};
assert(is<dateTime>(o.when));
let r = 0;
switch ("2026-10-11"date) {
    case "2026-10-12"date: r = 1; break;
    case "2026-10-11"date: r = 2; break;
}
assert(r === 2);
assert(`day ${"2026-10-11"date}` === "day 2026-10-11");
assert(add_dayTimeDuration<dateTime>("2026-10-11T12:00:00Z"dt, "PT1H"dayTimeDuration) ==
    "2026-10-11T13:00:00Z"dt);
return 0;

//?
//? test: compile-errors
//? description: bad text, unknown suffix, data type without typed literal, whitespace, property name, template string
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        return e.message;
    }
    return "compiled";
}

assert(includes(compileError('return "nope"dt;'),
    "Invalid dateTime typed literal"), "bad text");
assert(includes(compileError('return "x"foo;'),
    "'foo' is not a typed literal suffix"), "unknown suffix");
assert(includes(compileError('return "1"integer;'),
    "Data type 'integer' does not have a typed literal"), "integer");
assert(includes(compileError('return "x"string;'),
    "Data type 'string' does not have a typed literal"), "string");
assert(includes(compileError('return "x"objectId;'),
    "Data type 'objectId' does not have a typed literal"), "objectId");
assert(compileError('return "2026-10-11" date;') !== "compiled",
    "whitespace before suffix");
assert(compileError('return { "2026-10-11"date: 1 };') !== "compiled",
    "property name");
assert(compileError('return `2026-10-11`date;') !== "compiled",
    "template string");
return 0;

//?
//? test: json-rejects-suffix
//? description: JSON and relaxed JSON do not have typed literals
//? expect: 0
//? source: ...

let e1 = "compiled";
try {
    compile<json>(json('["2026-10-11"date]'));
}
catch (e) {
    e1 = e.message;
}
assert(includes(e1, "Array values must be separated with a comma"), "json");
let e2 = "compiled";
try {
    compile<relaxedJson>(relaxedJson('["2026-10-11"date]'));
}
catch (e) {
    e2 = e.message;
}
assert(includes(e2, "Array values must be separated with a comma"),
    "relaxedJson");
return 0;

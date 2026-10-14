#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/test.as
//? customPurpose: Part of core function tests
//? description: Tests for test: whether a regexp matches anywhere in a value.
//? sourceType: script
//?
//? test: test-found
//? description: True when the regexp matches anywhere in the value.
//? expect: [true, false]
//? source: ...

[test(/a+b/, "xyzaab"), test(/a+b/, "xyz")]

//?
//? test: test-method
//? description: The regexp is the receiver, as in ECMAScript's re.test(s).
//? expect: [true, true]
//? source: ...

let re = /^adm/;
return [re->test("admin"), /^adm/->test("admin")];

//?
//? test: test-anchored
//? description: ^ and \z match the whole value.
//? expect: [true, false]
//? source: ...

[test(/^ab\z/, "ab"), test(/^ab\z/, "abc")]

//?
//? test: test-text-types
//? description: Any value whose data type holds text can be tested.
//? expect: [true, true, true, true, true]
//? source: ...

[
    test(/oasis/, "http://www.oasis.org"anyURI),
    test(/\.org$/, "www.oasis.org"dnsName),
    test(/^10\./, "10.0.0.1"ipAddress),
    test(/@example/, "a@example.com"rfc822Name),
    test(/cn=/i, "CN=Bob, O=Example"x500Name)
]

//?
//? test: test-not-text
//? description: A value that does not hold text is an error.
//? expect: error:Value's data type not supported for this function
//? source: ...

test(/1/, 1)

//?
//? test: test-string-pattern
//? description: A string is not a regexp.
//? expect: error:Parameter 1 of function 'test' must evaluate to data type 'regexp' but evaluated to be 'string'; write "a"regexp or to_regexp(v)
//? source: ...

test("a", "abc")

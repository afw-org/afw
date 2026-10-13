#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: rfc822Name_x500Name.as
//? customPurpose: Data types
//? description: rfc822Name, x500Name, dnsName, and ipAddress equality; match; set functions; no ordering (#593)
//? sourceType: script
//?
//? test: rfc822Name-equal
//? description: the local part compares exactly and the domain part ignoring case
//? expect: [true, false, true, false]
//? source: ...

[
    "a@Example.COM"rfc822Name === "a@example.com"rfc822Name,
    "A@example.com"rfc822Name === "a@example.com"rfc822Name,
    "\"a@b\"@Example.com"rfc822Name === "\"a@b\"@example.COM"rfc822Name,
    "a@example.com"rfc822Name !== "a@EXAMPLE.com"rfc822Name
]

//?
//? test: rfc822Name-kept-as-written
//? description: the text is kept as written; only comparing ignores case
//? expect: "a@Example.COM"
//? source: ...

to_string("a@Example.COM"rfc822Name)

//?
//? test: rfc822Name-sets
//? description: set functions use rfc822Name equality
//? expect: [true, ["a@EXAMPLE.com"rfc822Name], true]
//? source: ...

[
    is_in("a@EXAMPLE.com"rfc822Name, ["b@example.com"rfc822Name, "a@example.com"rfc822Name]),
    intersection<rfc822Name>(
        array<rfc822Name>("a@EXAMPLE.com"rfc822Name, "B@example.com"rfc822Name),
        array<rfc822Name>("a@example.com"rfc822Name, "b@example.com"rfc822Name)),
    set_equals<rfc822Name>(
        array<rfc822Name>("a@x.org"rfc822Name, "b@Y.org"rfc822Name),
        array<rfc822Name>("b@y.ORG"rfc822Name, "a@X.org"rfc822Name))
]

//?
//? test: rfc822Name-match
//? description: a mailbox, a domain, and a subdomain pattern
//? expect: [true, false, true, false, true, false, false]
//? source: ...

const value = "Anderson@East.Sun.COM"rfc822Name;
return [
    rfc822Name_match("Anderson@east.sun.com", value),
    rfc822Name_match("anderson@east.sun.com", value),
    rfc822Name_match("east.sun.com", value),
    rfc822Name_match("sun.com", value),
    rfc822Name_match(".sun.com", value),
    rfc822Name_match(".east.sun.com", value),
    rfc822Name_match(".ab", "x@éé"rfc822Name)
];

//?
//? test: x500Name-equal
//? description: types ignore case and match their OIDs; values ignore case and insignificant spaces
//? expect: [true, true, true, true, true, false, false]
//? source: ...

[
    "cn=A,o=B"x500Name === "CN=a, O=b"x500Name,
    "cn=John  Smith ,o=B"x500Name === "cn= john smith,o=b"x500Name,
    "2.5.4.3=A;OID.2.5.4.10=B"x500Name === "cn=a,o=b"x500Name,
    "dc=Example,dc=COM"x500Name === "0.9.2342.19200300.100.1.25=example,DC=com"x500Name,
    "cn=a\\,b,o=c"x500Name === "cn=\"A,B\",o=C"x500Name,
    "cn=a,o=b"x500Name === "o=b,cn=a"x500Name,
    "cn=a"x500Name === "cn=a,o=b"x500Name
]

//?
//? test: x500Name-multi-valued
//? description: the parts of a multi-valued RDN may be in any order
//? expect: [true, false]
//? source: ...

[
    "OU=Sales+CN=J. Smith,O=Widget Inc.,C=US"x500Name === "cn=j. smith + ou=sales,o=widget inc.,c=us"x500Name,
    "OU=Sales+CN=J. Smith,O=Widget Inc.,C=US"x500Name === "cn=j. smith,ou=sales,o=widget inc.,c=us"x500Name
]

//?
//? test: x500Name-escapes-and-hex
//? description: \XX escapes are UTF-8 bytes; # values are octets
//? expect: [true, true, false]
//? source: ...

[
    "SN=Lu\\C4\\8Di\\C4\\87"x500Name === "sn=LUČIĆ"x500Name,
    "1.3.6.1.4.1.1466.0=#04024869,O=Test"x500Name === "1.3.6.1.4.1.1466.0=#04024869,o=test"x500Name,
    "cn=#6869"x500Name === "cn=hi"x500Name
]

//?
//? test: x500Name-sets
//? description: set functions use x500Name equality
//? expect: [true, 1]
//? source: ...

[
    is_in("CN=A, O=B"x500Name, ["cn=c,o=b"x500Name, "cn=a,o=b"x500Name]),
    length(union<x500Name>(
        array<x500Name>("cn=a,o=b"x500Name),
        array<x500Name>("CN=A, O=B"x500Name)))
]

//?
//? test: x500Name-match
//? description: the pattern's RDNs are the last RDNs of the value
//? expect: [true, true, false, false]
//? source: ...

const value = "cn=John Smith,o=Medico Corp, c=US"x500Name;
return [
    x500Name_match("O=Medico Corp,C=US"x500Name, value),
    x500Name_match("C=us"x500Name, value),
    x500Name_match("cn=John Smith"x500Name, value),
    x500Name_match("cn=x,cn=John Smith,o=Medico Corp,c=US"x500Name, value)
];

//?
//? test: dnsName-ipAddress-equal
//? description: DNS names and IPv6 hex digits ignore case; other forms of an address are not equal
//? expect: [true, true, false, false]
//? source: ...

[
    "Example.COM"dnsName === "example.com"dnsName,
    "FE80::A"ipAddress === "fe80::a"ipAddress,
    "::1"ipAddress === "0:0:0:0:0:0:0:1"ipAddress,
    "a.example.com"dnsName === "b.example.com"dnsName
]

//?
//? test: no-ordering
//? description: dnsName, ipAddress, rfc822Name, and x500Name have no ordering; anyURI does
//? expect: ["threw", "threw", "threw", "threw", "threw", true]
//? source: ...

function ordered(f) {
    try {
        return f();
    }
    catch (e) {
        return "threw";
    }
}
return [
    ordered(function () { return "a"dnsName < "b"dnsName; }),
    ordered(function () { return "10.0.0.1"ipAddress <= "10.0.0.2"ipAddress; }),
    ordered(function () { return "a@b"rfc822Name > "c@d"rfc822Name; }),
    ordered(function () { return "cn=a"x500Name >= "cn=b"x500Name; }),
    ordered(function () { return compile<script>(script('return lt<x500Name>("cn=a"x500Name, "cn=b"x500Name);')); }),
    "a"anyURI < "b"anyURI
];

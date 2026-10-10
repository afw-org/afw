#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: parse_uri_authority.as
//? customPurpose: Part of miscellaneous tests
//? description: ...
parse_uri splits the authority (RFC 3986): a userinfo may hold ':'
(user:password), and the host or port ends at '?' or '#' as well as at
'/' (http://h?x=1 has host h and query x=1).
//? sourceType: script
//?
//? test: parse-uri-userinfo-with-password
//? description: user:password@host is a userinfo, host, and port (it was a parse error)
//? expect: 0
//? source: ...

const u = parse_uri("http://user:pw@host.example:8080/a/b?x=1&y=2#frag");
assert(u.userinfo === "user:pw", stringify(u));
assert(u.host === "host.example");
assert(u.port === "8080");
assert(u.path === "/a/b");
assert(u.query === "x=1&y=2");
assert(u.fragment === "frag");
return 0;

//?
//? test: parse-uri-query-or-fragment-after-host
//? description: a query or fragment right after the host or port (it was part of the host, or an error)
//? expect: 0
//? source: ...

let u = parse_uri("http://h?x=1");
assert(u.host === "h" && u.query === "x=1", stringify(u));
u = parse_uri("http://h#f");
assert(u.host === "h" && u.fragment === "f", stringify(u));
u = parse_uri("http://h:80?x");
assert(u.host === "h" && u.port === "80" && u.query === "x", stringify(u));
u = parse_uri("http://1.2.3.4?x");
assert(u.host === "1.2.3.4" && u.query === "x", stringify(u));
u = parse_uri("http://u@h?x");
assert(u.userinfo === "u" && u.host === "h" && u.query === "x", stringify(u));
u = parse_uri("http://h/a@b");
assert(u.host === "h" && u.path === "/a@b", "an @ in the path is not a userinfo");
return 0;

//?
//? test: parse-uri-normalize-keeps-reserved-escapes
//? description: normalizing decodes only unreserved characters (a%2Fb is not a/b)
//? expect: 0
//? source: ...

assert(parse_uri("http://h/a%2fb").normalizedURI === "http://h/a%2Fb",
    parse_uri("http://h/a%2fb").normalizedURI);
assert(parse_uri("http://h/a%3Fb").normalizedURI === "http://h/a%3Fb");
assert(parse_uri("http://h/?q=%41%26b").normalizedURI === "http://h/?q=A%26b");
assert(parse_uri("http://h/%7euser").normalizedURI === "http://h/~user", "unreserved decoded");
return 0;

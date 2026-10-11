#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: uri_error_codes.as
//? customPurpose: Part of core tests
//? description: A URI the caller got wrong is a syntax error (HTTP 400), not general (#537)
//? sourceType: script
//?
//? test: decode-not-utf8
//? description: A %XX that is not UTF-8 is syntax
//? expect: "syntax"
//? source: ...

try {
    parse_uri("/a/b/%FF", true);
}
catch (e) {
    return e.id;
}
return "no error";

//? test: decode-not-nfc
//? description: A decoded path that is not NFC is syntax
//? expect: "syntax"
//? source: ...

try {
    parse_uri("/a/b/c%CC%81", true);
}
catch (e) {
    return e.id;
}
return "no error";

//? test: short-percent
//? description: A truncated %X is syntax
//? expect: "syntax"
//? source: ...

try {
    parse_uri("/a/b/%4", true);
}
catch (e) {
    return e.id;
}
return "no error";

//? test: invalid-character
//? description: A character a URI cannot hold is syntax
//? expect: "syntax"
//? source: ...

try {
    parse_uri("http://a b/", false);
}
catch (e) {
    return e.id;
}
return "no error";

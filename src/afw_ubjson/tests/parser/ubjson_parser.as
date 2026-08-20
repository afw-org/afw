#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: ubjson_parser.as
//? customPurpose: Part of afw_ubjson tests
//? description: Regression for U1/U2 -- UBJSON parser off-by-one read and signed-length overflow bypassing bounds checks. Fixtures are crafted raw UBJSON bytes (see config.py), read through a file adapter with contentType ubjson -- the same afw_content_type_raw_to_object path a POST with Content-Type: application/ubjson reaches via afw_request_body_to_value. U3 (unbounded recursion) is in its own file, ubjson_parser_depth.as -- pre-fix it parses to completion instead of throwing, and the resulting deeply-nested value breaks the test runner's own JSON response decoding, which would otherwise hide these cases' results.
//? sourceType: script
//?
//? test: ubjson_valid_object_parses
//? description: Baseline sanity -- a well-formed object parses correctly.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const obj = get_object("ubjson", "Test", "valid");
assert(obj.a === "hi", "expected well-formed UBJSON object to parse correctly");

return 0;


//? test: ubjson_u1_truncated_throws
//? description: U1 -- truncated input (missing closing brace) must throw the specific end-of-document error, not silently read past the buffer.
//? expect: error:Error: expected end of document.
//? source: ...
#!/usr/bin/env afw

get_object("ubjson", "Test", "u1_truncated");


//? test: ubjson_u2_negative_length_throws
//? description: U2 -- a negative length marker must throw the specific rejection error, not sign-extend into a massive out-of-bounds read.
//? expect: error:Error: negative length not allowed.
//? source: ...
#!/usr/bin/env afw

get_object("ubjson", "Test", "u2_negative_length");

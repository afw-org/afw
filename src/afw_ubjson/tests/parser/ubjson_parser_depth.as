#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: ubjson_parser_depth.as
//? customPurpose: Part of afw_ubjson tests
//? description: Regression for U3 -- UBJSON parser unbounded recursion. Kept separate from ubjson_parser.as because pre-fix this case doesn't throw at all -- it parses a deeply-nested value to completion, and returning that from the test blows past the test runner's own JSON decoder recursion limit, which would otherwise mask the other cases' results in the same run.
//? sourceType: script
//?
//? test: ubjson_u3_deep_nesting_throws
//? description: U3 -- deeply nested arrays must throw the specific max-depth error instead of exhausting the C stack (or, pre-fix, parsing to completion with no limit at all).
//? expect: error:Error: maximum nesting depth exceeded.
//? source: ...
#!/usr/bin/env afw

get_object("ubjson", "Test", "u3_deep_nesting");

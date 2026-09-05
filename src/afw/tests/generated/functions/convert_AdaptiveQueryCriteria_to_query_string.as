#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/convert_AdaptiveQueryCriteria_to_query_string.as
//? customPurpose: Part of core function tests
//? description: Tests for the convert_AdaptiveQueryCriteria_to_query_string function.
//? sourceType: script
//?
//? test: convert_AdaptiveQueryCriteria_to_query_string-error-unknown-objectType
//? description: throws when adapterId/objectType is specified but not found
//? expect: error:adapterId 'afw' objectType 'does-not-exist-objtype' is not found or invalid
//? source: ...

convert_AdaptiveQueryCriteria_to_query_string(
    {},
    "afw",
    "does-not-exist-objtype"
)

//?
//? test: convert_AdaptiveQueryCriteria_to_query_string-error-invalid-style
//? description: throws when the style parameter is out of range
//? expect: error:invalid value for style parameter
//? source: ...

convert_AdaptiveQueryCriteria_to_query_string(
    {},
    undefined,
    undefined,
    integer(999)
)

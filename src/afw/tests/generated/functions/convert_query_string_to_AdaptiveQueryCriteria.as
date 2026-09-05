#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/convert_query_string_to_AdaptiveQueryCriteria.as
//? customPurpose: Part of core function tests
//? description: Tests for the convert_query_string_to_AdaptiveQueryCriteria function.
//? sourceType: script
//?
//? test: convert_query_string_to_AdaptiveQueryCriteria-error-unknown-objectType
//? description: throws when adapterId/objectType is specified but not found
//? expect: error:adapterId 'afw' objectType 'does-not-exist-objtype' is not found or invalid
//? source: ...

convert_query_string_to_AdaptiveQueryCriteria(
    "foo=1",
    "afw",
    "does-not-exist-objtype"
)

//?
//? test: convert_query_string_to_AdaptiveQueryCriteria-error-objectType-required
//? description: throws when objectType is specified without adapterId
//? expect: error:objectType parameter is required if adapterId is specified
//? source: ...

convert_query_string_to_AdaptiveQueryCriteria(
    "foo=1",
    undefined,
    "SomeType"
)

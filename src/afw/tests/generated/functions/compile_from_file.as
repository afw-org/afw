#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/compile_from_file.as
//? customPurpose: Part of core function tests
//? description: Tests for the compile_from_file function.
//? sourceType: script
//?
//? test: compile_from_file-error-file-not-found
//? description: compile_from_file throws when the file's location cannot be resolved (no rootFilePaths configured in this test environment)
//? expect: error:Failed to resolve file location '/tmp/does-not-exist-xyz.as' (application rootFilePaths not configured).
//? source: ...

compile_from_file("/tmp/does-not-exist-xyz.as", "script")

//?
//? test: compile_from_file-error-invalid-compile-type
//? description: compile_from_file throws a clean error when compileType is not one of json/relaxed_json/script/template
//? expect: error:Invalid compile type 'bogus_type'.
//? source: ...

compile_from_file("/tmp/does-not-exist-xyz.as", "bogus_type")

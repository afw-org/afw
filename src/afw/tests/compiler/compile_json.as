#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compile_json.as
//? customPurpose: Part of compiler category tests
//? description: Test compile_json function
//? sourceType: script
//?
//? test: compile_json_comments
//? description: Test compiling json with comments
//? expect: error
//? source: ...

compile_json([/* comment * 1, 2]);

//? test: compile_json
//? description: Test compiling json with comments
//? expect: error
//? source: ...

compile_json([1, 2]);
#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compile_relaxed_json.as
//? customPurpose: Part of compiler category tests
//? description: Test compile_relaxed_json function
//? sourceType: script
//?
//? test: compile_relaxed_json_bad_comments
//? description: Test compiling json with comments
//? expect: error
//? source: ...

compile(relaxed_json("[/* comment * 1, 2]"));

//?
//? test: compile_relaxed_json_comments
//? description: Test compiling json with comments
//? expect: 1
//? source: ...

return compile(relaxed_json("[/* comment */ 1, 2]"))[0];

//? test: compile_relaxed_json
//? description: Test compiling json with trailing comma
//? expect: 2
//? source: ...

//FIXME mgg Should not require return

return compile<relaxed_json>(relaxed_json('[1, 2,]'))[1];
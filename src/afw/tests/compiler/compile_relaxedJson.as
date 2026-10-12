#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compile_relaxedJson.as
//? customPurpose: Part of compiler category tests
//? description: Test compile_relaxedJson function
//? sourceType: script
//?
//? test: compile_relaxedJson_bad_comments
//? description: Test compiling json with comments
//? expect: error
//? source: ...

compile(relaxedJson("[/* comment * 1, 2]"));

//?
//? test: compile_relaxedJson_comments
//? description: Test compiling json with comments
//? expect: 1
//? source: ...

return compile(relaxedJson("[/* comment */ 1, 2]"))[0];

//?
//? test: compile_relaxedJson
//? description: Test compiling json with trailing comma
//? expect: 2
//? source: ...

return compile<relaxedJson>(relaxedJson('[1, 2,]'))[1];
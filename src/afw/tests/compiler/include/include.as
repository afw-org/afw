#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: include.as
//? customPurpose: Part of compiler category tests
//? description: Test include function
//? sourceType: script
//?
//? test: include-no-params
//? description: Test include without any parameters
//? expect: error:'include' expects 1 required parameters
//? source: ...

include();


//? test: include-no-such-file
//? description: Test include with bad filename
//? expect: error:Failed to resolve include file location 'no-such-file.as'.
//? source: ...

include("no-such-file.as");


//? test: include-script
//? description: Test include with valid script file
//? expect: 0
//? source: ...

const {func1, func2} = include("includes/good.as");

assert(func1());
assert(!func2());

return 0;


//? test: include-json
//? description: Test include with valid json file
//? expect: 0
//? source: ...

const obj = include("includes/data.json", "json");

assert(obj.obj.some === "data");
assert(obj.array[0] === "of");

return 0;
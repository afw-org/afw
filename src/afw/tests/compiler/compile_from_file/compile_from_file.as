#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: compile_from_file.as
//? customPurpose: Part of compiler category tests
//? description: Test compile_from_file function
//? sourceType: script
//?
//? test: compile_from_file-no-params
//? description: Test compile_from_file without any parameters
//? expect: error:'compile_from_file' expects 1 required parameters
//? source: ...

compile_from_file();


//? test: compile_from_file-no-such-file
//? description: Test compile_from_file with bad filename
//? expect: error:Failed to resolve file location 'no-such-file.as'.
//? source: ...

compile_from_file("no-such-file.as");


//? test: compile_from_file-script
//? description: Test compile_from_file with valid script file
//? expect: 0
//? source: ...

const {func1, func2} = compile_from_file("includes/good.as");

assert(func1());
assert(!func2());

return 0;


//? test: compile_from_file-json
//? description: Test compile_from_file with valid json file
//? expect: 0
//? source: ...

const obj = compile_from_file("includes/data.json", "json");

assert(obj.obj.some === "data");
assert(obj.array[0] === "of");

return 0;
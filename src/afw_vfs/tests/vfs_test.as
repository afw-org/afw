#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: vfs_test.as
//? customPurpose: Part of core function tests
//? description: Test vfs adaptor.
//? sourceType: script
//?
//? test: vfs_test-1
//? description: Script to test vfs adaptor.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const testfile_objectId: string = string(temp_directory, 'test.txt');
let object: object;
let result: object;

// Create the temp directory.
result = add_object('vfs','_AdaptiveFile_vfs', {}, temp_directory);
assert((result.objectId == temp_directory), "objectId of temp_directory doesn't match");

// Add a test file to temp directory.
result = add_object('vfs','_AdaptiveFile_vfs',
    {data:"Hello world!"},
    testfile_objectId);
assert(result.objectId == testfile_objectId , "objectId of test file doesn't match");

// Make sure test file has 'Hello World!'.
object = get_object('vfs', '_AdaptiveFile_vfs', testfile_objectId);
assert(object.data == "Hello world!", "Data doesn't match 'Hello world!'");

// Replace test file with 'Hello World Yet Again!' with replace_object().
replace_object('vfs','_AdaptiveFile_vfs', testfile_objectId,
     {data:"Hello World Yet Again!"});

// Make sure test file now has 'Hello World Yet Again!'
object = get_object('vfs', '_AdaptiveFile_vfs', testfile_objectId);
assert(object.data == "Hello World Yet Again!",
    "Replacement data doesn't match 'Hello World Yet Again!'"); 

// Replace test file with 'The World is changing!' using modify_object().
modify_object('vfs','_AdaptiveFile_vfs', testfile_objectId,
    [["set_property", "data", "The World is changing!"]]);

// Make sure test file now has 'The World is changing!'.
object = get_object('vfs', '_AdaptiveFile_vfs', testfile_objectId);
assert(object.data == "The World is changing!",
    "Data doesn't match 'The World is changing!'"); 

// Delete test file.
delete_object('vfs','_AdaptiveFile_vfs', testfile_objectId);

// Delete temp/uuid/ directory.
delete_object('vfs','_AdaptiveFile_vfs', temp_directory);

// Delete temp/ directory.
delete_object('vfs','_AdaptiveFile_vfs', './temp/');

return 0;

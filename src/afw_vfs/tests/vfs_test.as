#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: vfs_test.as
//? customPurpose: Part of vfs adapter tests
//? description: Test vfs adapter CRUD, retrieve options, and edge cases.
//? sourceType: script
//?
//? test: vfs_test-1
//? description: Basic add, get, replace, modify, and delete of a text file.
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

//?
//? test: empty_file
//? description: Read 0-byte VFS file without error (issue #79).
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const empty_file_objectId: string = string(temp_directory, 'empty.txt');
let object: object;
let result: object;

// Create the temp directory.
result = add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
assert(result.objectId == temp_directory,
    "objectId of temp_directory doesn't match");

// Add a 0-byte file.
result = add_object('vfs', '_AdaptiveFile_vfs',
    { data: "" },
    empty_file_objectId);
assert(result.objectId == empty_file_objectId,
    "objectId of empty file doesn't match");

// Reading an empty file must succeed (not throw on malloc size 0).
object = get_object('vfs', '_AdaptiveFile_vfs', empty_file_objectId);
assert(object.data == "", "Empty file data should be empty string");

// Cleanup.
delete_object('vfs', '_AdaptiveFile_vfs', empty_file_objectId);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

//?
//? test: replace_shorten_and_empty
//? description: replace/modify with shorter or empty data truncates prior content (issue #79).
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const file_id: string = string(temp_directory, 'shrink.txt');
let object: object;

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "abcdefghij" }, file_id);

// Shorter replace must not leave trailing prior bytes.
replace_object('vfs', '_AdaptiveFile_vfs', file_id, { data: "xy" });
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "xy", "Shorter replace left prior content");

// Empty replace must yield a true 0-byte file.
replace_object('vfs', '_AdaptiveFile_vfs', file_id, { data: "" });
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "", "Empty replace did not clear file");

// Shorter modify must also truncate.
replace_object('vfs', '_AdaptiveFile_vfs', file_id, { data: "1234567890" });
modify_object('vfs', '_AdaptiveFile_vfs', file_id,
    [["set_property", "data", "ab"]]);
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "ab", "Shorter modify left prior content");

// Empty modify must clear the file.
modify_object('vfs', '_AdaptiveFile_vfs', file_id,
    [["set_property", "data", ""]]);
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "", "Empty modify did not clear file");

delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

//?
//? test: directory_listing
//? description: get_object on a directory returns isDirectory, names, and vfsPath.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const sub_directory: string = string(temp_directory, 'sub/');
const file_a: string = string(temp_directory, 'a.txt');
const file_b: string = string(temp_directory, 'b.dat');
let dir: object;
let found_a: boolean = false;
let found_b: boolean = false;
let found_sub: boolean = false;

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', {}, sub_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "A" }, file_a);
add_object('vfs', '_AdaptiveFile_vfs', { data: "B" }, file_b);

dir = get_object('vfs', '_AdaptiveFile_vfs', temp_directory);
assert(dir.isDirectory == true, "Directory should have isDirectory true");
assert(!is_nullish(dir.vfsPath), "Directory should have vfsPath");
assert(dir.vfsPath == anyURI(string('/vfs/', temp_directory)),
    "Directory vfsPath mismatch");
assert(!is_nullish(dir.timeModified), "Directory should have timeModified");
assert(!is_nullish(dir.data), "Directory should list entries in data");

for (const name of dir.data) {
    if (name == "a.txt") {
        found_a = true;
    }
    if (name == "b.dat") {
        found_b = true;
    }
    if (name == "sub/") {
        found_sub = true;
    }
}
assert(found_a, "Directory listing missing a.txt");
assert(found_b, "Directory listing missing b.dat");
assert(found_sub, "Directory listing missing sub/");

// Regular files do not set isDirectory.
const file_obj: object = get_object('vfs', '_AdaptiveFile_vfs', file_a);
assert(is_nullish(file_obj.isDirectory),
    "Regular file should not set isDirectory");
assert(file_obj.vfsPath == anyURI(string('/vfs/', file_a)),
    "File vfsPath mismatch");
assert(!is_nullish(file_obj.timeModified), "File should have timeModified");

delete_object('vfs', '_AdaptiveFile_vfs', file_a);
delete_object('vfs', '_AdaptiveFile_vfs', file_b);
delete_object('vfs', '_AdaptiveFile_vfs', sub_directory);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

//?
//? test: content_variants
//? description: Unicode, whitespace, multiline, and hexBinary content round-trip.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const uni_id: string = string(temp_directory, 'uni.txt');
const ws_id: string = string(temp_directory, 'ws.txt');
const ml_id: string = string(temp_directory, 'ml.txt');
const bin_id: string = string(temp_directory, 'bin.bin');
let object: object;

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);

add_object('vfs', '_AdaptiveFile_vfs', { data: "café 日本語" }, uni_id);
object = get_object('vfs', '_AdaptiveFile_vfs', uni_id);
assert(object.data == "café 日本語", "Unicode content mismatch");

add_object('vfs', '_AdaptiveFile_vfs', { data: " \n\t" }, ws_id);
object = get_object('vfs', '_AdaptiveFile_vfs', ws_id);
assert(object.data == " \n\t", "Whitespace content mismatch");

add_object('vfs', '_AdaptiveFile_vfs', { data: "line1\nline2\n" }, ml_id);
object = get_object('vfs', '_AdaptiveFile_vfs', ml_id);
assert(object.data == "line1\nline2\n", "Multiline content mismatch");

// Non-UTF-8 bytes are stored/returned as hexBinary.
add_object('vfs', '_AdaptiveFile_vfs', { data: hexBinary("00ff") }, bin_id);
object = get_object('vfs', '_AdaptiveFile_vfs', bin_id);
assert(object.data == hexBinary("00ff"), "hexBinary content mismatch");

delete_object('vfs', '_AdaptiveFile_vfs', uni_id);
delete_object('vfs', '_AdaptiveFile_vfs', ws_id);
delete_object('vfs', '_AdaptiveFile_vfs', ml_id);
delete_object('vfs', '_AdaptiveFile_vfs', bin_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

//?
//? test: retrieve_options
//? description: retrieve_objects subdirectory, recursive, suffix, and includeHidden.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const sub_directory: string = string(temp_directory, 'sub/');
const file_a: string = string(temp_directory, 'a.txt');
const file_b: string = string(temp_directory, 'b.dat');
const nested: string = string(sub_directory, 'nested.txt');
const hidden: string = string(temp_directory, '.hidden.txt');
let objects: array;
let found: boolean;

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', {}, sub_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "A" }, file_a);
add_object('vfs', '_AdaptiveFile_vfs', { data: "B" }, file_b);
add_object('vfs', '_AdaptiveFile_vfs', { data: "N" }, nested);
add_object('vfs', '_AdaptiveFile_vfs', { data: "secret" }, hidden);

// Non-recursive: top-level files and the sub/ directory entry; no nested, no hidden.
objects = retrieve_objects(
    'vfs', '_AdaptiveFile_vfs', undefined, { objectId: true },
    { subdirectory: temp_directory });
assert(length(objects) == 3,
    "Non-recursive retrieve count should be 3 (a.txt, b.dat, sub/)");

found = false;
for (const o of objects) {
    if (meta(o).objectId == nested) {
        found = true;
    }
}
assert(!found, "Non-recursive retrieve should not include nested file");

found = false;
for (const o of objects) {
    if (meta(o).objectId == hidden) {
        found = true;
    }
}
assert(!found, "Default retrieve should not include hidden files");

// Recursive: directories are traversed instead of emitted; nested file appears.
objects = retrieve_objects(
    'vfs', '_AdaptiveFile_vfs', undefined, { objectId: true },
    { subdirectory: temp_directory, recursive: true });
found = false;
for (const o of objects) {
    if (meta(o).objectId == nested) {
        found = true;
    }
}
assert(found, "Recursive retrieve should include nested file");
assert(length(objects) == 3,
    "Recursive retrieve count should be 3 (a.txt, b.dat, nested.txt)");

// Suffix filter applies to regular files.
objects = retrieve_objects(
    'vfs', '_AdaptiveFile_vfs', undefined, { objectId: true },
    { subdirectory: temp_directory, recursive: true, suffix: ".txt" });
assert(length(objects) == 2,
    "Suffix .txt retrieve should return a.txt and nested.txt");
for (const o of objects) {
    assert(
        meta(o).objectId == file_a || meta(o).objectId == nested,
        "Unexpected objectId in suffix filter results");
}

// includeHidden adds dotfiles.
objects = retrieve_objects(
    'vfs', '_AdaptiveFile_vfs', undefined, { objectId: true },
    { subdirectory: temp_directory, includeHidden: true });
found = false;
for (const o of objects) {
    if (meta(o).objectId == hidden) {
        found = true;
    }
}
assert(found, "includeHidden retrieve should include .hidden.txt");

// Cleanup bottom-up.
delete_object('vfs', '_AdaptiveFile_vfs', nested);
delete_object('vfs', '_AdaptiveFile_vfs', hidden);
delete_object('vfs', '_AdaptiveFile_vfs', file_a);
delete_object('vfs', '_AdaptiveFile_vfs', file_b);
delete_object('vfs', '_AdaptiveFile_vfs', sub_directory);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

//?
//? test: error_cases
//? description: Missing objects, invalid paths, and disallowed operations error.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const file_id: string = string(temp_directory, 'exists.txt');
const missing_id: string = string(temp_directory, 'missing.txt');

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "present" }, file_id);

// Missing object operations.
assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', missing_id),
        "error") == "error",
    "get_object on missing file should error");
assert(
    safe_evaluate(
        delete_object('vfs', '_AdaptiveFile_vfs', missing_id),
        "error") == "error",
    "delete_object on missing file should error");
assert(
    safe_evaluate(
        modify_object('vfs', '_AdaptiveFile_vfs', missing_id,
            [["set_property", "data", "x"]]),
        "error") == "error",
    "modify_object on missing file should error");
assert(
    safe_evaluate(
        replace_object('vfs', '_AdaptiveFile_vfs', missing_id, { data: "x" }),
        "error") == "error",
    "replace_object on missing file should error");

// Duplicate add.
assert(
    safe_evaluate(
        add_object('vfs', '_AdaptiveFile_vfs', { data: "x" }, file_id),
        "error") == "error",
    "add_object of existing file should error");

// File requires data property.
assert(
    safe_evaluate(
        add_object('vfs', '_AdaptiveFile_vfs', {},
            string(temp_directory, 'nodata.txt')),
        "error") == "error",
    "add_object of file without data should error");

// Directory cannot be modified or replaced as a file.
assert(
    safe_evaluate(
        modify_object('vfs', '_AdaptiveFile_vfs', temp_directory,
            [["set_property", "data", "x"]]),
        "error") == "error",
    "modify_object on directory should error");
assert(
    safe_evaluate(
        replace_object('vfs', '_AdaptiveFile_vfs', temp_directory,
            { data: "x" }),
        "error") == "error",
    "replace_object on directory should error");

// Path traversal and backslash rejected by get_object.
assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', '../etc/passwd'),
        "error") == "error",
    "get_object with ../ should error");
assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', 'foo\\bar'),
        "error") == "error",
    "get_object with backslash should error");

// File objectId must not end with '/'; directory must end with '/'.
assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', string(file_id, '/')),
        "error") == "error",
    "get_object of file with trailing slash should error");

// Non-empty directory delete fails.
assert(
    safe_evaluate(
        delete_object('vfs', '_AdaptiveFile_vfs', temp_directory),
        "error") == "error",
    "delete_object of non-empty directory should error");

// Invalid retrieve subdirectory values.
assert(
    safe_evaluate(
        retrieve_objects('vfs', '_AdaptiveFile_vfs', undefined, undefined,
            { subdirectory: "/abs/" }),
        "error") == "error",
    "subdirectory starting with / should error");
assert(
    safe_evaluate(
        retrieve_objects('vfs', '_AdaptiveFile_vfs', undefined, undefined,
            { subdirectory: "noslash" }),
        "error") == "error",
    "subdirectory without trailing / should error");
assert(
    safe_evaluate(
        retrieve_objects('vfs', '_AdaptiveFile_vfs', undefined, undefined,
            { subdirectory: "temp/./" }),
        "error") == "error",
    "subdirectory with ./ should error");

// Wrong object type is not found for get (callback with NULL -> error).
assert(
    safe_evaluate(
        get_object('vfs', 'NotAType', file_id),
        "error") == "error",
    "get_object with wrong object type should error");

// Cleanup.
delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

//?
//? test: nested_delete
//? description: Nested directories require bottom-up delete.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const temp_directory: string = string('temp/', generate_uuid(), '/');
const mid: string = string(temp_directory, 'mid/');
const leaf: string = string(mid, 'leaf/');
const file_id: string = string(leaf, 'deep.txt');

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', {}, mid);
add_object('vfs', '_AdaptiveFile_vfs', {}, leaf);
add_object('vfs', '_AdaptiveFile_vfs', { data: "deep" }, file_id);

assert(
    get_object('vfs', '_AdaptiveFile_vfs', file_id).data == "deep",
    "Nested file content mismatch");

// Bottom-up delete.
delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', leaf);
delete_object('vfs', '_AdaptiveFile_vfs', mid);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);

assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', file_id),
        "error") == "error",
    "Nested file should be gone after delete");

delete_object('vfs', '_AdaptiveFile_vfs', './temp/');

return 0;

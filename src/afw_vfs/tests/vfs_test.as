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

const temp_directory: string = string('temp/', generate_uuid(), '/');
const testfile_objectId: string = string(temp_directory, 'test.txt');
let object: object;
let result: object;

result = add_object('vfs','_AdaptiveFile_vfs', {}, temp_directory);
assert((result.objectId == temp_directory), "objectId of temp_directory doesn't match");

result = add_object('vfs','_AdaptiveFile_vfs',
    {data:"Hello world!"},
    testfile_objectId);
assert(result.objectId == testfile_objectId , "objectId of test file doesn't match");

object = get_object('vfs', '_AdaptiveFile_vfs', testfile_objectId);
assert(object.data == "Hello world!", "Data doesn't match 'Hello world!'");

replace_object('vfs','_AdaptiveFile_vfs', testfile_objectId,
     {data:"Hello World Yet Again!"});

object = get_object('vfs', '_AdaptiveFile_vfs', testfile_objectId);
assert(object.data == "Hello World Yet Again!",
    "Replacement data doesn't match 'Hello World Yet Again!'");

modify_object('vfs','_AdaptiveFile_vfs', testfile_objectId,
    [["set_property", "data", "The World is changing!"]]);

object = get_object('vfs', '_AdaptiveFile_vfs', testfile_objectId);
assert(object.data == "The World is changing!",
    "Data doesn't match 'The World is changing!'");

delete_object('vfs','_AdaptiveFile_vfs', testfile_objectId);
delete_object('vfs','_AdaptiveFile_vfs', temp_directory);
delete_object('vfs','_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: empty_file
//? description: Read 0-byte VFS file without error (issue #79).
//? skip: false
//? expect: 0
//? source: ...

const temp_directory: string = string('temp/', generate_uuid(), '/');
const empty_file_objectId: string = string(temp_directory, 'empty.txt');
let object: object;
let result: object;

result = add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
assert(result.objectId == temp_directory,
    "objectId of temp_directory doesn't match");

result = add_object('vfs', '_AdaptiveFile_vfs',
    { data: "" },
    empty_file_objectId);
assert(result.objectId == empty_file_objectId,
    "objectId of empty file doesn't match");

object = get_object('vfs', '_AdaptiveFile_vfs', empty_file_objectId);
assert(object.data == "", "Empty file data should be empty string");

delete_object('vfs', '_AdaptiveFile_vfs', empty_file_objectId);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: replace_shorten_and_empty
//? description: replace/modify with shorter or empty data truncates prior content (issue #79).
//? skip: false
//? expect: 0
//? source: ...

const temp_directory: string = string('temp/', generate_uuid(), '/');
const file_id: string = string(temp_directory, 'shrink.txt');
let object: object;

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "abcdefghij" }, file_id);

replace_object('vfs', '_AdaptiveFile_vfs', file_id, { data: "xy" });
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "xy", "Shorter replace left prior content");

replace_object('vfs', '_AdaptiveFile_vfs', file_id, { data: "" });
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "", "Empty replace did not clear file");

replace_object('vfs', '_AdaptiveFile_vfs', file_id, { data: "1234567890" });
modify_object('vfs', '_AdaptiveFile_vfs', file_id,
    [["set_property", "data", "ab"]]);
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "ab", "Shorter modify left prior content");

modify_object('vfs', '_AdaptiveFile_vfs', file_id,
    [["set_property", "data", ""]]);
object = get_object('vfs', '_AdaptiveFile_vfs', file_id);
assert(object.data == "", "Empty modify did not clear file");

delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: directory_listing
//? description: get_object on a directory returns isDirectory, names, vfsPath; hides dotfiles by default.
//? skip: false
//? expect: 0
//? source: ...

const temp_directory: string = string('temp/', generate_uuid(), '/');
const sub_directory: string = string(temp_directory, 'sub/');
const file_a: string = string(temp_directory, 'a.txt');
const file_b: string = string(temp_directory, 'b.dat');
const hidden_id: string = string(temp_directory, '.hidden.txt');
let dir: object;
let found_a: boolean = false;
let found_b: boolean = false;
let found_sub: boolean = false;
let found_hidden: boolean = false;

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', {}, sub_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "A" }, file_a);
add_object('vfs', '_AdaptiveFile_vfs', { data: "B" }, file_b);
add_object('vfs', '_AdaptiveFile_vfs', { data: "secret" }, hidden_id);

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
    if (name == ".hidden.txt") {
        found_hidden = true;
    }
}
assert(found_a, "Directory listing missing a.txt");
assert(found_b, "Directory listing missing b.dat");
assert(found_sub, "Directory listing missing sub/");
assert(!found_hidden,
    "Default directory listing should hide dotfiles");

dir = get_object('vfs', '_AdaptiveFile_vfs', temp_directory, undefined,
    { includeHidden: true });
found_hidden = false;
for (const name of dir.data) {
    if (name == ".hidden.txt") {
        found_hidden = true;
    }
}
assert(found_hidden,
    "includeHidden directory listing should include .hidden.txt");

const file_obj: object = get_object('vfs', '_AdaptiveFile_vfs', file_a);
assert(file_obj.isDirectory == false,
    "Regular file should have isDirectory false");
assert(file_obj.vfsPath == anyURI(string('/vfs/', file_a)),
    "File vfsPath mismatch");
assert(!is_nullish(file_obj.timeModified), "File should have timeModified");

delete_object('vfs', '_AdaptiveFile_vfs', file_a);
delete_object('vfs', '_AdaptiveFile_vfs', file_b);
delete_object('vfs', '_AdaptiveFile_vfs', hidden_id);
delete_object('vfs', '_AdaptiveFile_vfs', sub_directory);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: content_variants
//? description: Unicode, whitespace, multiline, and hexBinary content round-trip.
//? skip: false
//? expect: 0
//? source: ...

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

add_object('vfs', '_AdaptiveFile_vfs', { data: hexBinary("00ff") }, bin_id);
object = get_object('vfs', '_AdaptiveFile_vfs', bin_id);
assert(object.data == hexBinary("00ff"), "hexBinary content mismatch");

delete_object('vfs', '_AdaptiveFile_vfs', uni_id);
delete_object('vfs', '_AdaptiveFile_vfs', ws_id);
delete_object('vfs', '_AdaptiveFile_vfs', ml_id);
delete_object('vfs', '_AdaptiveFile_vfs', bin_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: retrieve_options
//? description: retrieve_objects subdirectory, recursive, suffix, and includeHidden.
//? skip: false
//? expect: 0
//? source: ...

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

delete_object('vfs', '_AdaptiveFile_vfs', nested);
delete_object('vfs', '_AdaptiveFile_vfs', hidden);
delete_object('vfs', '_AdaptiveFile_vfs', file_a);
delete_object('vfs', '_AdaptiveFile_vfs', file_b);
delete_object('vfs', '_AdaptiveFile_vfs', sub_directory);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: error_cases
//? description: Missing objects, invalid paths, and disallowed operations error.
//? skip: false
//? expect: 0
//? source: ...

const temp_directory: string = string('temp/', generate_uuid(), '/');
const file_id: string = string(temp_directory, 'exists.txt');
const missing_id: string = string(temp_directory, 'missing.txt');

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs', { data: "present" }, file_id);

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

assert(
    safe_evaluate(
        add_object('vfs', '_AdaptiveFile_vfs', { data: "x" }, file_id),
        "error") == "error",
    "add_object of existing file should error");

const nodata_id: string = string(temp_directory, 'nodata.txt');
add_object('vfs', '_AdaptiveFile_vfs', {}, nodata_id);
assert(
    get_object('vfs', '_AdaptiveFile_vfs', nodata_id).data == "",
    "add_object without data should create empty file");
delete_object('vfs', '_AdaptiveFile_vfs', nodata_id);

assert(
    safe_evaluate(
        add_object('vfs', '_AdaptiveFile_vfs', { data: "x" },
            string(temp_directory, 'foo/..')),
        "error") == "error",
    "object_id ending with .. segment should error");

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

assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', string(file_id, '/')),
        "error") == "error",
    "get_object of file with trailing slash should error");

assert(
    safe_evaluate(
        delete_object('vfs', '_AdaptiveFile_vfs', temp_directory),
        "error") == "error",
    "delete_object of non-empty directory should error");

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

assert(
    safe_evaluate(
        get_object('vfs', 'NotAType', file_id),
        "error") == "error",
    "get_object with wrong object type should error");

delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: nested_delete
//? description: Nested directories require bottom-up delete.
//? skip: false
//? expect: 0
//? source: ...

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

delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', leaf);
delete_object('vfs', '_AdaptiveFile_vfs', mid);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);

assert(
    safe_evaluate(
        get_object('vfs', '_AdaptiveFile_vfs', file_id),
        "error") == "error",
    "Nested file should be gone after delete");

delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: max_read_bytes
//? description: maxReadBytes rejects oversized file reads.
//? skip: false
//? expect: 0
//? source: ...

const temp_directory: string = string('temp/', generate_uuid(), '/');
const file_id: string = string(temp_directory, 'big.txt');

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs',
    { data: "0123456789abcdefMORE" }, file_id);

assert(
    get_object('vfs', '_AdaptiveFile_vfs', file_id).data ==
        "0123456789abcdefMORE",
    "Unlimited adapter should read full content");

assert(
    safe_evaluate(
        get_object('vfs_small', '_AdaptiveFile_vfs', file_id),
        "error") == "error",
    "vfs_small should reject read exceeding maxReadBytes");

const small_id: string = string(temp_directory, 'ok.txt');
add_object('vfs', '_AdaptiveFile_vfs', { data: "0123456789abcdef" }, small_id);
assert(
    get_object('vfs_small', '_AdaptiveFile_vfs', small_id).data ==
        "0123456789abcdef",
    "vfs_small should read file at maxReadBytes boundary");

delete_object('vfs', '_AdaptiveFile_vfs', file_id);
delete_object('vfs', '_AdaptiveFile_vfs', small_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;

//?
//? test: mark_executable
//? description: markExecutable patterns apply without error on write.
//? skip: false
//? expect: 0
//? source: ...

const temp_directory: string = string('temp/', generate_uuid(), '/');
const as_id: string = string(temp_directory, 'script.as');
const txt_id: string = string(temp_directory, 'notes.txt');

add_object('vfs', '_AdaptiveFile_vfs', {}, temp_directory);
add_object('vfs', '_AdaptiveFile_vfs',
    { data: "#!/usr/bin/env afw\nreturn 0;\n" }, as_id);
add_object('vfs', '_AdaptiveFile_vfs', { data: "notes" }, txt_id);

assert(
    get_object('vfs', '_AdaptiveFile_vfs', as_id).data ==
        "#!/usr/bin/env afw\nreturn 0;\n",
    "markExecutable target should read back");
assert(
    get_object('vfs', '_AdaptiveFile_vfs', txt_id).data == "notes",
    "non-matching file should read back");

delete_object('vfs', '_AdaptiveFile_vfs', as_id);
delete_object('vfs', '_AdaptiveFile_vfs', txt_id);
delete_object('vfs', '_AdaptiveFile_vfs', temp_directory);
delete_object('vfs', '_AdaptiveFile_vfs', 'temp/');

return 0;


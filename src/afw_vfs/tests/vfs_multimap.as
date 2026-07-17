#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: vfs_multimap.as
//? customPurpose: Part of vfs adapter tests
//? description: Multi-entry vfsMap longest-prefix and path-boundary (issue #103 parity).
//? sourceType: script
//?
//? test: longest_prefix_includes
//? description: includes/ beats include/ for includes/x.txt (like rootFilePaths).
//? skip: false
//? expect: 0
//? source: ...

const obj: object = get_object('vfs_multi', '_AdaptiveFile_vfs',
    'includes/x.txt');
assert(obj.data == "from-includes\n" || obj.data == "from-includes",
    "includes/x.txt should resolve under includes_root");
assert(obj.isDirectory == false, "file isDirectory false");
return 0;

//?
//? test: include_prefix
//? description: include/ maps to include_root, not includes_root.
//? skip: false
//? expect: 0
//? source: ...

const obj: object = get_object('vfs_multi', '_AdaptiveFile_vfs',
    'include/x.txt');
assert(obj.data == "from-include\n" || obj.data == "from-include",
    "include/x.txt should resolve under include_root");
return 0;

//?
//? test: data_prefix_seed
//? description: data/ prefix reads seed file under data_root.
//? skip: false
//? expect: 0
//? source: ...

const obj: object = get_object('vfs_multi', '_AdaptiveFile_vfs',
    'data/seed.txt');
assert(obj.data == "from-data\n" || obj.data == "from-data",
    "data/seed.txt content mismatch");
return 0;

//?
//? test: multimap_write_roundtrip
//? description: add/get/delete under a multi-map prefix.
//? skip: false
//? expect: 0
//? source: ...

const id: string = string('data/rt-', generate_uuid(), '.txt');
add_object('vfs_multi', '_AdaptiveFile_vfs', { data: "roundtrip" }, id);
assert(get_object('vfs_multi', '_AdaptiveFile_vfs', id).data == "roundtrip",
    "multi-map roundtrip content");
delete_object('vfs_multi', '_AdaptiveFile_vfs', id);
assert(
    safe_evaluate(
        get_object('vfs_multi', '_AdaptiveFile_vfs', id),
        "error") == "error",
    "deleted multi-map object should be gone");
return 0;

//?
//? test: multimap_no_matching_prefix
//? description: objectId outside all prefixes is not found.
//? skip: false
//? expect: 0
//? source: ...

let caught: boolean = false;
let id: string = "";
try {
    get_object('vfs_multi', '_AdaptiveFile_vfs', 'other/nope.txt');
} catch (e) {
    caught = true;
    id = e.id;
}
assert(caught, "get outside multi-map prefixes should error");
assert(id == "not_found", "expected not_found for unmatched prefix");
return 0;

//?
//? test: multimap_retrieve_data
//? description: retrieve_objects subdirectory data/ lists seeded file.
//? skip: false
//? expect: 0
//? source: ...

const objects: array = retrieve_objects(
    'vfs_multi', '_AdaptiveFile_vfs', undefined, { objectId: true },
    { subdirectory: "data/" });
let found: boolean = false;
for (const o of objects) {
    if (meta(o).objectId == "data/seed.txt") {
        found = true;
    }
}
assert(found, "retrieve data/ should include data/seed.txt");
return 0;

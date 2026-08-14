#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: conf_path_templates.as
//? customPurpose: Issue #15 conf path templates at create
//? description: rootFilePaths, file adapter root, extensionModulePaths, modulePath as templates
//? sourceType: script
//?
//? test: rootFilePaths-environment-template
//? description: issue #15 rootFilePaths host dir from environment:: works with open_file
//? expect: 0
//? source: ...

const sn = open_file("r1", "data/sample.txt", "r");
const s = read(sn, 100);
close(sn);
assert(s === "hello-issue15\n" || s === "hello-issue15",
    "expected sample content, got " + string(s));
return 0;


//?
//? test: rootFilePaths-write-under-template-root
//? description: issue #15 write/read under template-resolved rootFilePaths
//? expect: 0
//? source: ...

const sn = open_file("w1", "data/written.txt", "w");
write(sn, "from-template-root");
flush(sn);
close(sn);

const sn2 = open_file("w2", "data/written.txt", "r");
const s = read(sn2, 100);
close(sn2);
assert(s === "from-template-root");
return 0;


//?
//? test: file-adapter-root-environment-template
//? description: issue #15 file adapter root from environment:: can get_object
//? expect: 0
//? source: ...

const o = get_object("file", "Thing", "one");
assert(o !== null && o !== undefined, "get_object Thing/one required");
assert(o.name === "one", "name mismatch");
assert(o.value === 15, "value mismatch");
return 0;


//?
//? test: file-adapter-root-template-retrieve
//? description: issue #15 retrieve_objects under template file adapter root
//? expect: 0
//? source: ...

const objs = retrieve_objects("file", "Thing");
assert(length(objs) >= 1, "expected at least one Thing");
let found = false;
for (const o of objs) {
    if (o.name === "one" && o.value === 15) {
        found = true;
    }
}
assert(found, "Thing/one not found in retrieve");
return 0;


//?
//? test: extensionModulePaths-template-loads-vfs
//? description: issue #15 extensionModulePaths array template loads libafwvfs
//? expect: 0
//? source: ...

/* vfs adapter type is registered only after afw_vfs extension loads */
const at = get_object("afw", "_AdaptiveAdapterType_", "vfs");
assert(at !== null && at !== undefined,
    "vfs adapter type missing — extensionModulePaths template load failed");
return 0;


//?
//? test: extension-conf-modulePath-template-loads-yaml
//? description: issue #15 type=extension modulePath template loads libafwyaml
//? expect: 0
//? source: ...

const ct = get_object("afw", "_AdaptiveContentType_", "yaml");
assert(ct !== null && ct !== undefined,
    "yaml content type missing — extension modulePath template load failed");
return 0;


//?
//? test: environment-qualifier-available-for-conf
//? description: issue #15 environment::AFW_ISSUE15_ROOT is set for conf templates
//? expect: 0
//? source: ...

assert(environment::AFW_ISSUE15_ROOT !== undefined,
    "AFW_ISSUE15_ROOT should be in process environment");
assert(length(string(environment::AFW_ISSUE15_ROOT)) > 0,
    "AFW_ISSUE15_ROOT should be non-empty");
return 0;

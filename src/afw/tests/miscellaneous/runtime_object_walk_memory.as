#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: runtime_object_walk_memory.as
//? customPurpose: Part of miscellaneous tests
//? description: ...
Walking a runtime object's properties frees the walk's cursor at the end
(#648). _AdaptiveObjectType_ properties are stored, not computed, so a
walk allocates nothing else. Computed properties (_AdaptiveProcess_) are
#645.
//? sourceType: script
//?
//? test: keys-of-runtime-object-is-flat
//? description: keys() of a runtime object in a loop stays flat
//? expect: true
//? source: ...

const o = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveFunction_");
function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        const k = keys(o);
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(100);
const many = run(2100);
assert(many - few < 20000, "100: " + string(few) + ", 2100: " + string(many));
return true;

//?
//? test: stringify-runtime-object-is-flat
//? description: stringify() of a runtime object in a loop stays flat
//? expect: true
//? source: ...

const o = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveFunction_");
function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        const t = stringify(o);
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(100);
const many = run(2100);
assert(many - few < 20000, "100: " + string(few) + ", 2100: " + string(many));
return true;

#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: walk_memory.as
//? customPurpose: Part of file adapter tests
//? description: Walking an adapter object's properties frees the walk's cursor at the end (#648)
//? sourceType: script
//?
//? test: keys-of-adapter-object-is-flat
//? description: keys() of a file adapter object in a loop stays flat
//? expect: true
//? source: ...

const o = get_object("file", "TestObjectType1", "Test1");
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
//? test: stringify-adapter-object-is-flat
//? description: stringify() of a file adapter object in a loop stays flat
//? expect: true
//? source: ...

const o = get_object("file", "TestObjectType1", "Test1");
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

//?
//? test: keys-of-meta-with-options-is-flat
//? description: keys(meta()) of an adapter object read with object options stays flat
//? expect: true
//? source: ...

const o = get_object("file", "TestObjectType1", "Test1",
    {objectType: true, objectId: true});
function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        const k = keys(meta(o));
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(100);
const many = run(2100);
assert(many - few < 20000, "100: " + string(few) + ", 2100: " + string(many));
return true;

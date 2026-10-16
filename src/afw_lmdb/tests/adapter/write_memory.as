#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: write_memory.as
//? customPurpose: Part of LMDB adapter tests
//? description: LMDB writes keep their scratch in the call's pool, not the job heap (#539)
//? sourceType: script
//?
//? test: write-calls-do-not-grow
//? description: add, modify, update, replace, and delete in a loop stay flat
//? expect: true
//? source: ...

function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        add_object("lmdb", "WriteMemory", {s: "x"}, "WriteMemory539");
        modify_object("lmdb", "WriteMemory", "WriteMemory539",
            [["set_property", "s", "y"]]);
        update_object("lmdb", "WriteMemory", "WriteMemory539", {s: "z"});
        replace_object("lmdb", "WriteMemory", "WriteMemory539", {s: "w"});
        delete_object("lmdb", "WriteMemory", "WriteMemory539");
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(20);
const many = run(120);
assert(many - few < 4096, "20: " + string(few) + ", 120: " + string(many));
return true;


//? test: indexed-write-calls-do-not-grow
//? description: the same writes on object types with a property index and a computed (script) index stay flat; add and replace index the caller's object, whose pool lasts the whole script (#663)
//? expect: true
//? source: ...

index_create("lmdb", "s", undefined, ["WriteMemoryIndexed"], undefined, undefined, false);
index_create("lmdb", "st663", "current::object.s + '-' + current::object.t",
    ["WriteMemoryComputed"], undefined, undefined, false);

function run(n: integer, ot: string): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        add_object("lmdb", ot, {s: "x", t: "a"}, "WriteMemory663");
        modify_object("lmdb", ot, "WriteMemory663",
            [["set_property", "s", "y"]]);
        update_object("lmdb", ot, "WriteMemory663", {s: "z"});
        replace_object("lmdb", ot, "WriteMemory663", {s: "w", t: "b"});
        delete_object("lmdb", ot, "WriteMemory663");
    }
    return pool_bytes_in_use() - s;
}

for (const ot of ["WriteMemoryIndexed", "WriteMemoryComputed"]) {
    run(5, ot);
    const few = run(20, ot);
    const many = run(120, ot);
    assert(many - few < 4096,
        ot + " 20: " + string(few) + ", 120: " + string(many));
}

/*
 * The index keys built in the write's scratch pool are what the index
 * holds. st663 is not a property, so only the index can find these.
 */
function found(value: string): integer {
    return length(retrieve_objects("lmdb", "WriteMemoryComputed",
        { filter: { op: "eq", property: "st663", value: value } }));
}

add_object("lmdb", "WriteMemoryComputed", {s: "q", t: "r"}, "WriteMemory663");
assert(found("q-r") == 1, "after add, q-r: " + string(found("q-r")));
replace_object("lmdb", "WriteMemoryComputed", "WriteMemory663", {s: "w", t: "b"});
assert(found("w-b") == 1 && found("q-r") == 0,
    "after replace, w-b: " + string(found("w-b")) + ", q-r: " + string(found("q-r")));
modify_object("lmdb", "WriteMemoryComputed", "WriteMemory663",
    [["set_property", "s", "v"]]);
assert(found("v-b") == 1 && found("w-b") == 0,
    "after modify, v-b: " + string(found("v-b")) + ", w-b: " + string(found("w-b")));
delete_object("lmdb", "WriteMemoryComputed", "WriteMemory663");
assert(found("v-b") == 0, "after delete, v-b: " + string(found("v-b")));
return true;


//? test: index-calls-do-not-grow
//? description: index_create with test true, index_create of a key that exists, and index_list stay flat; their results and scratch objects are in the caller's pool or released, not each in a pool of its own under the job heap (#663)
//? expect: true
//? source: ...

for (const i of create_array(5)) {
    add_object("lmdb", "IndexMemory", {s: "x"});
}
index_create("lmdb", "exists663", undefined, ["IndexMemory"], undefined, undefined, false);

function run(n: integer, which: string): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        if (which == "test") {
            index_create("lmdb", "s", undefined, ["IndexMemory"], undefined, undefined, true);
        } else if (which == "exists") {
            index_create("lmdb", "exists663", undefined, ["IndexMemory"], undefined, undefined, false);
        } else {
            index_list("lmdb");
            index_list("lmdb", "IndexMemory");
        }
    }
    return pool_bytes_in_use() - s;
}

for (const which of ["test", "exists", "list"]) {
    run(5, which);
    const few = run(20, which);
    const many = run(120, which);
    assert(many - few < 4096,
        which + " 20: " + string(few) + ", 120: " + string(many));
}
return true;

#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: write_memory.as
//? customPurpose: Part of adapter write tests
//? description: Adapter writes keep their scratch in the call's pool, not the job heap (#539)
//? sourceType: script
//?
//? test: write-calls-do-not-grow
//? description: add, modify, update, replace, and delete in a loop stay flat
//? expect: true
//? source: ...

function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        add_object("file", "TestObjectType1", {TestString1: "x"}, "WriteMemory539");
        modify_object("file", "TestObjectType1", "WriteMemory539",
            [["set_property", "TestString1", "y"]]);
        update_object("file", "TestObjectType1", "WriteMemory539", {TestString1: "z"});
        replace_object("file", "TestObjectType1", "WriteMemory539", {TestString1: "w"});
        delete_object("file", "TestObjectType1", "WriteMemory539");
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(20);
const many = run(120);
assert(many - few < 4096, "20: " + string(few) + ", 120: " + string(many));
return true;

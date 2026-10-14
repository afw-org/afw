#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: write_memory.as
//? customPurpose: Part of LMDB journal tests
//? description: Journaled LMDB writes keep their scratch in the call's pool, not the job heap (#539)
//? sourceType: script
//?
//? test: write-calls-do-not-grow
//? description: add, modify, update, replace, and delete in a loop stay flat, journal entries included
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

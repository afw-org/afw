#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: model_memory.as
//? customPurpose: Part of model adapter tests
//? description: ...
Model adapter reads and writes keep their scratch in the call's pool, not
the job heap: the journal entry for the mapped adapter call and the
"to adapter" context live with the request (#539 rule).
//? sourceType: script
//?
//? test: model-reads-do-not-grow
//? description: get_object and retrieve_objects through a model in a loop stay flat
//? expect: true
//? source: ...

function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        get_object("model", "MyObjectType1", "Test1");
        retrieve_objects("model", "MyObjectType1");
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(20);
const many = run(220);
assert(many - few < 8192, "20: " + string(few) + ", 220: " + string(many));
return true;

//?
//? test: model-writes-do-not-grow
//? description: add, modify, update, replace, and delete through a model in a loop stay flat
//? expect: true
//? source: ...

function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        add_object("model", "MyObjectType1", {MyTestString1: "x"}, "ModelMemory");
        modify_object("model", "MyObjectType1", "ModelMemory",
            [["set_property", "MyTestString1", "y"]]);
        update_object("model", "MyObjectType1", "ModelMemory", {MyTestString1: "z"});
        replace_object("model", "MyObjectType1", "ModelMemory", {MyTestString1: "w"});
        delete_object("model", "MyObjectType1", "ModelMemory");
    }
    return pool_bytes_in_use() - s;
}

run(5);
const few = run(20);
const many = run(220);
assert(many - few < 8192, "20: " + string(few) + ", 220: " + string(many));
return true;

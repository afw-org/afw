#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: current_variables_memory.as
//? customPurpose: Part of miscellaneous tests
//? description: Values that never change are made once, with what they describe (#546)
//? sourceType: script
//?
//? test: current-variables-read-is-flat
//? description: Reading current::pid, xctxUUID, and programName does not grow with the count
//? expect: true
//? source: ...

function run(n: integer): integer {
    const s = pool_bytes_in_use();
    for (const z of create_array(n)) {
        if (current::pid === 0 || current::xctxUUID === "" ||
            current::programName === "")
        {
            throw "empty";
        }
    }
    return pool_bytes_in_use() - s;
}

run(50);
const few = run(100);
const many = run(2000);
assert(many - few < 1024, "100: " + string(few) + ", 2000: " + string(many));
return true;

//? test: current-variables-same-each-read
//? description: Each read gives the same value
//? expect: true
//? source: ...

assert(current::pid === current::pid);
assert(current::xctxUUID === current::xctxUUID);
assert(current::programName === current::programName);
assert(qualifier("current").pid === current::pid);
assert(qualifier("current").xctxUUID === current::xctxUUID);
return true;

//? test: process-pid-matches-current
//? description: process::pid and current::pid are the env's pid
//? expect: true
//? source: ...

return process::pid === current::pid;

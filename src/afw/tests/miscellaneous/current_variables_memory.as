#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: current_variables_memory.as
//? customPurpose: Part of miscellaneous tests
//? description: ...
Reading current:: variables many times in one evaluation stays flat:
pid, xctxUUID, and programName are made once per xctx, not per read.
//? sourceType: script
//?
//? test: current-variables-read-is-flat
//? description: read current::pid, xctxUUID, programName in a loop
//? expect: 0
//? source: ...

let start = 0;
let n = 0;
for (let i = 0; i < 3000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    n = n + length(string(current::pid)) + length(current::xctxUUID) +
        length(current::programName);
}
assert(n > 0);
assert(pool_bytes_in_use() - start < 100000, "current:: reads are flat");
return 0;

//?
//? test: current-variables-same-each-read
//? description: values are the same each read
//? expect: 0
//? source: ...

assert(current::pid === current::pid);
assert(current::xctxUUID === current::xctxUUID);
assert(current::programName === current::programName);
assert(qualifier("current").pid === current::pid);
return 0;

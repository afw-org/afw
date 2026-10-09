#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: alias_write_write.as
//? customPurpose: Part of lmdb tests
//? description: ...
Write through lmdbA, then lmdbA2: two adapter ids on one MDB_env, each
with its own session, in one process (one "request"). lmdbA's write
transaction holds the MDB_env's writer mutex until the process ends, so
a second one from lmdbA2 would wait on this thread forever. The LMDB
adapter refuses it instead (EDEADLK). Before, the process hung.
//? sourceType: script
//?
//? test: alias_write_then_write_refused
//? description: The write through lmdbA2 throws "would deadlock"; lmdbA's write stays.
//? skip: false
//? expect: 0
//? source: ...

add_object('lmdbA', 'OrderRow', {item: 'x'}, 'x1');

let refused: boolean = false;
try {
    add_object('lmdbA2', 'OrderRow', {item: 'y'}, 'y1');
}
catch (e) {
    assert(includes<string>(e.message, "would deadlock"), e.message);
    assert(includes<string>(e.message, "through adapter 'lmdbA'"), e.message);
    refused = true;
}
assert(refused, "write through lmdbA2 was not refused");

assert(get_object('lmdbA', 'OrderRow', 'x1').item === 'x', "x1 lost");
assert(length(retrieve_objects('lmdbA', 'OrderRow',
    {filter: {op: 'eq', property: 'item', value: 'y'}})) === 0, "y1 written");

return 0;

//?
//? test: first_id_still_writes
//? description: The refusal leaves lmdbA's transaction usable.
//? skip: false
//? expect: 0
//? source: ...

add_object('lmdbA', 'OrderRow', {item: 'z'}, 'z1');
assert(length(retrieve_objects('lmdbA', 'OrderRow')) === 4,
    "expected s1, s2, x1 and z1");

return 0;

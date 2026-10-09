#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: alias_write_write_reverse.as
//? customPurpose: Part of lmdb tests
//? description: alias_write_write.as in the other order, lmdbA2 first; its own file so lmdbA has no transaction yet.
//? sourceType: script
//?
//? test: alias_write_then_write_reverse_refused
//? description: The write through lmdbA throws "would deadlock" naming lmdbA2; lmdbA2's write stays.
//? skip: false
//? expect: 0
//? source: ...

add_object('lmdbA2', 'OrderRow', {item: 'y'}, 'y1');

let refused: boolean = false;
try {
    add_object('lmdbA', 'OrderRow', {item: 'x'}, 'x1');
}
catch (e) {
    assert(includes<string>(e.message, "would deadlock"), e.message);
    assert(includes<string>(e.message, "through adapter 'lmdbA2'"), e.message);
    refused = true;
}
assert(refused, "write through lmdbA was not refused");

assert(get_object('lmdbA2', 'OrderRow', 'y1').item === 'y', "y1 lost");

return 0;

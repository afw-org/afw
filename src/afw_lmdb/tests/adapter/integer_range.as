#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: integer_range.as
//? customPurpose: Part of lmdb tests
//? description: Integers outside the 32-bit range are stored and read back (UBJSON int64).
//? sourceType: script
//?
//? test: integer_range_round_trip
//? description: add_object then get_object keeps integers outside int32 (they threw "Unexpected error in UBJSON converting 64-bit integer")
//? skip: false
//? expect: 0
//? source: ...

const values = [
    0, 127, 128, 255, 256, -129, 32767, 32768, -32769,
    2147483647, 2147483648, -2147483647, -2147483648, -2147483649,
    97469257417208997, 9223372036854775807, -9223372036854775807 - 1
];
let i = 0;
for (const v of values) {
    const id = "int-range-" + string(i);
    add_object('lmdb', '_AdaptiveObject_', {n: v, a: [v, {m: v}]}, id);
    const o = get_object('lmdb', '_AdaptiveObject_', id);
    assert(o.n === v, "n " + string(v) + " came back " + string(o.n));
    assert(o.a[0] === v && o.a[1].m === v, "nested " + string(v));
    delete_object('lmdb', '_AdaptiveObject_', id);
    i = i + 1;
}
return 0;

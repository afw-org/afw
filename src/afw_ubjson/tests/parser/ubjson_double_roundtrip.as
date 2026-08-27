#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: ubjson_double_roundtrip.as
//? customPurpose: Part of afw_ubjson tests
//? description: Regression for issue #263 -- a `double` property must round-trip through UBJSON encode/decode unchanged. Pre-fix, convert_double_to_ubjson() wrote raw native-endian bytes while afw_ubjson_parse_number() always byte-swapped float64 on read, so any non-zero double came back corrupted (a different, usually subnormal, value) on a little-endian host.
//? sourceType: script
//?
//? test: ubjson_double_roundtrip_negative_fractional
//? description: A negative fractional double round-trips through add_object/get_object unchanged (the exact repro from issue #263).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const uuid: string = generate_uuid();

add_object("ubjson", "TestDoubleType", { score: -3.5 }, uuid);
const g: object = get_object("ubjson", "TestDoubleType", uuid);
assert(g.score === -3.5, "expected -3.5, got " + string(g.score));

delete_object("ubjson", "TestDoubleType", uuid);

return 0;


//? test: ubjson_double_roundtrip_values
//? description: Positive, negative, zero, and fractional doubles all round-trip unchanged. Zero round-tripped correctly even pre-fix (its all-zero byte pattern is its own byte-reversal), so it is included as a non-regressing baseline alongside the values that were corrupted.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const uuidA: string = generate_uuid();
const uuidB: string = generate_uuid();
const uuidC: string = generate_uuid();
const uuidD: string = generate_uuid();

add_object("ubjson", "TestDoubleType", { score: 3.5 }, uuidA);
add_object("ubjson", "TestDoubleType", { score: 0.0 }, uuidB);
add_object("ubjson", "TestDoubleType", { score: 42.0 }, uuidC);
add_object("ubjson", "TestDoubleType", { score: 100.25 }, uuidD);

assert(get_object("ubjson", "TestDoubleType", uuidA).score === 3.5, "expected 3.5 to round-trip");
assert(get_object("ubjson", "TestDoubleType", uuidB).score === 0.0, "expected 0.0 to round-trip");
assert(get_object("ubjson", "TestDoubleType", uuidC).score === 42.0, "expected 42.0 to round-trip");
assert(get_object("ubjson", "TestDoubleType", uuidD).score === 100.25, "expected 100.25 to round-trip");

delete_object("ubjson", "TestDoubleType", uuidA);
delete_object("ubjson", "TestDoubleType", uuidB);
delete_object("ubjson", "TestDoubleType", uuidC);
delete_object("ubjson", "TestDoubleType", uuidD);

return 0;

#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_prefix_retrieve.as
//? customPurpose: Part of lmdb tests
//? description: Retrieve by object type when one type name is a prefix of another.
//? sourceType: script
//?
//? test: type_prefix_retrieve
//? description: A Primary key is {objectType}{uuid}, so keys of PrefixScanWork sort in among those of PrefixScan. A retrieve of PrefixScan must return all of its objects and none of the other type's.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw


function idsOf(objectType: string, criteria?: object): array {
    let ids: array = [];
    for (const obj: object of retrieve_objects('lmdb', objectType, criteria,
        { objectId: true }))
    {
        ids = add_entries(ids, [meta(obj).objectId]);
    }
    return sort(ids);
}

/*
 * The uuid of the first PrefixScan object sorts below "W" (0x57) and the
 * second above it, so the PrefixScanWork key sorts between them. The
 * PrefixScanWork uuid sorts low too.
 */
const low: string = "10000000-0000-4000-8000-000000000001";
const high: string = "80000000-0000-4000-8000-000000000002";
const work: string = "00000000-0000-4000-8000-000000000003";
const shorter: string = "f0000000-0000-4000-8000-000000000004";
const after: string = "00000000-0000-4000-8000-000000000005";

add_object('lmdb', 'PrefixScan', { n: 1 }, low);
add_object('lmdb', 'PrefixScan', { n: 2 }, high);
add_object('lmdb', 'PrefixScanWork', { n: 3 }, work);
add_object('lmdb', 'PrefixSca', { n: 4 }, shorter);
add_object('lmdb', 'PrefixScanX', { n: 5 }, after);

assert(idsOf('PrefixScan') == [low, high],
    "retrieve of PrefixScan: " + stringify(idsOf('PrefixScan')));
assert(idsOf('PrefixScanWork') == [work],
    "retrieve of PrefixScanWork: " + stringify(idsOf('PrefixScanWork')));
assert(idsOf('PrefixSca') == [shorter],
    "retrieve of PrefixSca: " + stringify(idsOf('PrefixSca')));
assert(idsOf('PrefixScanX') == [after],
    "retrieve of PrefixScanX: " + stringify(idsOf('PrefixScanX')));

// with a filter, the scan reaches the object past the other type's key
assert(idsOf('PrefixScan', { urlEncodedRQLString: "n=gt=1" }) == [high],
    "filtered retrieve of PrefixScan: " +
    stringify(idsOf('PrefixScan', { urlEncodedRQLString: "n=gt=1" })));

// a type with no objects, whose name is a prefix of types that have some
assert(idsOf('Prefix') == [], "retrieve of Prefix is not empty");

// a get by id still works for every one of them
assert(get_object('lmdb', 'PrefixScan', high).n == 2, "get of high");
assert(get_object('lmdb', 'PrefixScanWork', work).n == 3, "get of work");

return 0;

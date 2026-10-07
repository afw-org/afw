#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_type_cache_key.as
//? customPurpose: Part of file adapter tests
//? description: Object type lookups with an object type id built in a block that has ended.
//? sourceType: script
//?
//? test: object_type_id_from_ended_block
//? description: The session object type cache kept the caller's object type id as its key; an id made in a loop body was freed when the body ended, and the next lookup compared against freed memory (ASan use-after-poison).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n: integer = 0;
for (const i of [1, 2, 3]) {
    const ot: string = "TestObjectType" + string(i);
    const r: array = retrieve_objects("file", ot, undefined, { "objectTypes": true });
    n = n + 1;
}
for (const i of [3, 2, 1]) {
    const ot: string = "TestObjectType" + string(i);
    retrieve_objects("file", ot, undefined, { "objectTypes": true });
}
return n - 3;

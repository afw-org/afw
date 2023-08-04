#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_2.as
//? customPurpose: Part of lmdb tests
//? description: test journal_get_first with empty journal
//? sourceType: script
//?
//? test: journal_get_first
//? description: journal_get_first, then add object
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let uuid = generate_uuid();

let j1 = journal_get_first("journal");
assert(j1 == {}, string(j1));

let add_res = add_object("lmdb", "_AdaptiveObject_", { 
    firstName: "bob"
}, uuid);

let j2 = journal_get_first("journal");
assert(j2.entry.request.suggestedObjectId === uuid);

return 0;
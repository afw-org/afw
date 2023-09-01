#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_1.as
//? customPurpose: Part of lmdb tests
//? description: Test lmdb journal interface methods.
//? sourceType: script
//?
//? test: journal_get_first
//? description: test lmdb journal_get_first with no entries
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let journal = journal_get_first("journal");
let journa2 = journal_get_first("journal");

return 0;


//? test: journal_get_first
//? description: add object, then journal_get_first
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let uuid = generate_uuid();

add_object("lmdb", "_AdaptiveObject_", { 
    firstName: "bob"
}, uuid);

let journal = journal_get_first("journal");
assert(journal.entry.request.suggestedObjectId === uuid);

return 0;


//? test: journal_get_by_cursor
//? description: journal_get_by_cursor
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let entry = journal_get_first("journal");
let cursor = entry.entryCursor;
assert(cursor != null);

let e2 = journal_get_by_cursor("journal", cursor);
assert(e2 === entry);

return 0;


//? test: journal_get_next_after_cursor
//? description: journal_get_next_after_cursor
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let entry = journal_get_first("journal");
let cursor = entry.entryCursor;
assert(cursor != null);

let j1 = journal_get_next_after_cursor("journal", cursor);
assert(j1 == {});

return 0;
#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_1.as
//? customPurpose: Part of file journal tests
//? description: Test file journal interface methods.
//? sourceType: script
//?
//? test: journal_get_first
//? description: test file journal_get_first with no entries
//? skip: true
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let journal = journal_get_first("journal");
let journal2 = journal_get_first("journal");

return 0;


//? test: journal_get_first
//? description: add object, then journal_get_first
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const result = add_object("file", "_AdaptiveObject_", { 
    firstName: "bob"
});

let journal = journal_get_first("journal");
assert(journal.entry.request.objectId === result.request.objectId);

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
assert(entry.entry != null);

let j1 = journal_get_next_after_cursor("journal", cursor);
assert(j1.entry == null);


return 0;
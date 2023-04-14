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

loc journal = journal_get_first("journal");
loc journa2 = journal_get_first("journal");

return 0;


//? test: journal_get_first
//? description: add object, then journal_get_first
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc uuid = generate_uuid();

add_object("lmdb", "_AdaptiveObject_", { 
    firstName: "bob"
}, uuid);

loc journal = journal_get_first("journal");
assert(journal.entry.request.suggestedObjectId === uuid);

return 0;


//? test: journal_get_by_cursor
//? description: journal_get_by_cursor
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc entry = journal_get_first("journal");
loc cursor = entry.entryCursor;
assert(cursor != null);

loc e2 = journal_get_by_cursor("journal", cursor);
assert(e2 === entry);

return 0;


//? test: journal_get_next_after_cursor
//? description: journal_get_next_after_cursor
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc entry = journal_get_first("journal");
loc cursor = entry.entryCursor;
assert(cursor != null);

loc j1 = journal_get_next_after_cursor("journal", cursor);
assert(j1 == {});

return 0;
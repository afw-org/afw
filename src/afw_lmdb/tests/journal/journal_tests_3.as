#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_3.as
//? customPurpose: Part of lmdb tests
//? description: test a series of journal entries
//? sourceType: script
//?
//? test: journal_get_first
//? description: add 100 objects
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc j1 = journal_get_first("journal");
assert(j1 == {}, string(j1));

for (loc i = 0; i < 100; i += 1) {
    add_object("lmdb", "_AdaptiveObject_", { 
        firstName: "bob"
    });    
}

/* now read each one */
loc j2 = journal_get_first("journal");
loc entryCursor = j2.entryCursor;
assert(entryCursor != null);

loc j3;
for (loc i = 0; i < 100; i += 1) {
    j3 = journal_get_next_after_cursor("journal", entryCursor);
    assert(j3.entryCursor !== entryCursor);
    entryCursor = j3.entryCursor;
}

assert(j3 == {}, string(j3));

return 0;
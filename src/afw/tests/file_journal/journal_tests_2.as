#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_3.as
//? customPurpose: Part of file_journal tests
//? description: test a series of journal entries
//? sourceType: script
//?
//? test: journal_get_first
//? description: add 100 objects
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let err = false;
try {
    let j1 = journal_get_first("journal");
} catch (e) {
    err = true;
}
assert(err);

for (let i = 0; i < 100; i += 1) {
    add_object("file", "_AdaptiveObject_", { 
        firstName: "bob"
    });    
}

/* now read each one */
let j2 = journal_get_first("journal");
let entryCursor = j2.entryCursor;
assert(entryCursor != null);

const firstCursor = entryCursor;
const firstAgain = journal_get_by_cursor("journal", firstCursor);
assert(firstAgain.entryCursor === firstCursor);

let j3;
let midCursor = null;
for (let i = 0; i < 100; i += 1) {
    j3 = journal_get_next_after_cursor("journal", entryCursor);
    assert(j3.entryCursor !== entryCursor);
    entryCursor = j3.entryCursor;
    if (i === 49) {
        midCursor = entryCursor;
    }
}

assert(j3.entry == null, string(j3));

const mid = journal_get_by_cursor("journal", midCursor);
assert(mid.entry !== undefined);
assert(mid.entryCursor === midCursor);

return 0;
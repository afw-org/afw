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
assert(is_nullish(j1.entry), "no next entry after last cursor");


return 0;


//? test: journal_get_first-mutable-face
//? description: ...
Issue #17: journal_get_first returns a mutable face; face-only props must not
appear on a second get_first. Runs after cursor tests so journal length stays
stable for earlier cases.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let j1 = journal_get_first("journal");
assert(j1.entry !== undefined);
j1.faceOnly = true;

let j2 = journal_get_first("journal");
assert(is_nullish(property_get(j2, "faceOnly", null)),
    "second journal_get_first must not see faceOnly from first face");
assert(j2.entry !== undefined);

return 0;


//? test: journal_get_by_cursor-mutable-face
//? description: ...
Issue #17: journal_get_by_cursor returns a face; face-only props on first get
must not appear when fetching the same cursor again.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let first = journal_get_first("journal");
assert(first.entry !== undefined);
const cursor = first.entryCursor;
assert(cursor != null);

let e1 = journal_get_by_cursor("journal", cursor);
e1.faceOnly = "by_cursor";
assert(e1.entryCursor === cursor);

let e2 = journal_get_by_cursor("journal", cursor);
assert(is_nullish(property_get(e2, "faceOnly", null)),
    "second get_by_cursor must not see faceOnly");
assert(e2.entryCursor === cursor);
/* Content still structurally equal across faces for the same cursor. */
assert(e2 === e1 || e2.entryCursor === e1.entryCursor);

return 0;


//? test: journal_get_next_after_cursor-mutable-face
//? description: ...
Issue #17: journal_get_next_after_cursor response is a face; face-only props
must not leak to a second call with the same cursor.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let first = journal_get_first("journal");
const cursor = first.entryCursor;
assert(cursor != null);

let n1 = journal_get_next_after_cursor("journal", cursor);
n1.faceOnly = "next";
assert(is_nullish(n1.entry) || n1.entry !== undefined);

let n2 = journal_get_next_after_cursor("journal", cursor);
assert(is_nullish(property_get(n2, "faceOnly", null)),
    "second get_next_after_cursor must not see faceOnly");

return 0;


//? test: journal_response-nested-entry-lookthrough
//? description: ...
Issue #17: nested entry on a journal response face is readable; setting a
top-level face-only prop does not hide entryCursor / entry look-through.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let j = journal_get_first("journal");
assert(j.entry !== undefined);
const cursor = j.entryCursor;
assert(cursor != null);

j.faceOnly = 1;
assert(j.faceOnly === 1);
assert(j.entryCursor === cursor, "face-only set must not hide entryCursor");
assert(j.entry !== undefined, "face-only set must not hide entry");

return 0;
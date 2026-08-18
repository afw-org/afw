#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_cursor.as
//? customPurpose: Part of file journal tests
//? description: ...
File journal cursor parse and get_by_cursor. Token is ccyymmddhh_offset;
offset must be decimal digits that fit in signed off_t. A bad cursor is
a syntax error (not a seek to a wrapped offset). Assigned id is
entry.objectId (not request.objectId). before_each runs once per file.
//? sourceType: script
//?
//? test: empty-get-first
//? description: journal_get_first with no entries is an error
//? expect: error
//? source: ...

journal_get_first("journal")

//?
//? test: cursor-too-short
//? description: cursor shorter than ccyymmddhh_0 is an error
//? expect: error
//? source: ...

journal_get_by_cursor("journal", "2016070423")

//?
//? test: cursor-missing-underscore
//? description: 12 characters with no underscore is an error
//? expect: error
//? source: ...

journal_get_by_cursor("journal", "201607042300")

//?
//? test: cursor-non-digit-date
//? description: date prefix must be ten digits
//? expect: error
//? source: ...

journal_get_by_cursor("journal", "abcdefghij_0")

//?
//? test: cursor-non-digit-offset
//? description: cursor offset that is not all digits is an error
//? expect: error
//? source: ...

journal_get_by_cursor("journal", "2016070423_abc")

//?
//? test: cursor-mixed-offset
//? description: digits then a letter in the offset is an error
//? expect: error
//? source: ...

journal_get_by_cursor("journal", "2016070423_12a")

//?
//? test: cursor-offset-overflow
//? description: cursor offset that does not fit in signed off_t is an error
//? expect: error
//? source: ...

journal_get_by_cursor("journal",
    "2016070423_999999999999999999999999999999")

//?
//? test: next-after-bad-cursor
//? description: journal_get_next_after_cursor uses the same cursor parse
//? expect: error
//? source: ...

journal_get_next_after_cursor("journal", "2016070423_abc")

//?
//? test: well-formed-missing-file
//? description: valid cursor whose journal hour file does not exist is an error
//? expect: error
//? source: ...

journal_get_by_cursor("journal", "2099123123_0")

//?
//? test: get-by-cursor-roundtrip
//? description: get_by_cursor of a real cursor returns that entry
//? expect: 0
//? source: ...

const added = add_object("file", "_AdaptiveObject_", { firstName: "cursor1" });
const first = journal_get_first("journal");
assert(first.entry !== undefined);
assert(first.entryCursor != null);
assert(first.entry.objectId === added.objectId);

const again = journal_get_by_cursor("journal", first.entryCursor);
assert(again.entryCursor === first.entryCursor);
assert(again.entry.objectId === added.objectId);
return 0;

//?
//? test: padded-offset-same-entry
//? description: leading zeros on the offset still seek to the same entry
//? expect: 0
//? source: ...

add_object("file", "_AdaptiveObject_", { firstName: "pad" });
const first = journal_get_first("journal");
const cursor = first.entryCursor;
const padded = replace(cursor, "_", "_000");
const again = journal_get_by_cursor("journal", padded);
assert(again.entry.objectId === first.entry.objectId);
return 0;

//?
//? test: three-entries-walk-and-get
//? description: next after cursor walks three entries; get_by_cursor hits each
//? expect: 0
//? source: ...

add_object("file", "_AdaptiveObject_", { firstName: "a" }, "idA");
add_object("file", "_AdaptiveObject_", { firstName: "b" }, "idB");
add_object("file", "_AdaptiveObject_", { firstName: "c" }, "idC");

/* before_each is per file, not per case. Earlier cases in this file may
 * have appended entries; skip to the three we just added. */
let e1 = journal_get_first("journal");
let n = 0;
while (!is_nullish(e1.entry) && e1.entry.objectId !== "idA" && n < 50) {
    e1 = journal_get_next_after_cursor("journal", e1.entryCursor);
    n = n + 1;
}
assert(e1.entry.objectId === "idA");
const g1 = journal_get_by_cursor("journal", e1.entryCursor);
assert(g1.entry.objectId === "idA");

const e2 = journal_get_next_after_cursor("journal", e1.entryCursor);
assert(e2.entry !== undefined);
assert(e2.entry.objectId === "idB");
const g2 = journal_get_by_cursor("journal", e2.entryCursor);
assert(g2.entry.objectId === "idB");

const e3 = journal_get_next_after_cursor("journal", e2.entryCursor);
assert(e3.entry !== undefined);
assert(e3.entry.objectId === "idC");
const g3 = journal_get_by_cursor("journal", e3.entryCursor);
assert(g3.entry.objectId === "idC");

const end = journal_get_next_after_cursor("journal", e3.entryCursor);
assert(is_nullish(end.entry), "no entry after the last of the three");
return 0;

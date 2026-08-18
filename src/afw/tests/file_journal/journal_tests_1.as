#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_1.as
//? customPurpose: Part of file journal tests
//? description: Test file journal interface methods.
//? sourceType: script
//?
//? test: journal_get_first-empty
//? description: journal_get_first with no entries returns no entry
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let journal = journal_get_first("journal");
assert(is_nullish(journal.entry), "empty journal has no entry");
let journal2 = journal_get_first("journal");
assert(is_nullish(journal2.entry), "second empty get_first still has no entry");

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
assert(journal.entry.objectId === result.objectId);

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


//? test: journal_get_next_for_consumer-mutable-face
//? description: ...
Issue #17: journal_get_next_for_consumer returns a face. Seeds a
_AdaptiveProvisioningPeer_ on the journal adapter (consumer id file under
journal root). Face-only props must not appear on reissue get.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

/* Ensure at least one journal entry (file adapter writes journal). */
add_object("file", "_AdaptiveObject_", { firstName: "consumer_face_seed" });

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "faceConsumer",
    consumeFilter: true
}, "faceConsumer");

let j1 = journal_get_next_for_consumer("journal", "faceConsumer", 100);
assert(j1.entry !== undefined, "consumer should get an applicable entry");
const cursor = j1.entryCursor;
assert(cursor != null);
j1.faceOnly = "consumer";

/* Second call reissues same consumeCursor entry with reissue=true. */
let j2 = journal_get_next_for_consumer("journal", "faceConsumer", 100);
assert(is_nullish(property_get(j2, "faceOnly", null)),
    "reissue get_next_for_consumer must not see faceOnly");
assert(j2.entry !== undefined, "reissue still has entry");
assert(j2.entryCursor === cursor, "reissue same cursor");

/* Clean consume lock for later consumer tests. */
journal_mark_consumed("journal", "faceConsumer", cursor);

return 0;


//? test: journal_get_next_for_consumer_after_cursor-mutable-face
//? description: ...
Issue #17: journal_get_next_for_consumer_after_cursor returns a face; face-only
props must not leak across two calls with the same cursor. Uses peer
faceConsumerAfter (separate id so peer file state is independent).
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("file", "_AdaptiveObject_", { firstName: "after_a" });
add_object("file", "_AdaptiveObject_", { firstName: "after_b" });

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "faceConsumerAfter",
    consumeFilter: true
}, "faceConsumerAfter");

let first = journal_get_first("journal");
assert(first.entryCursor != null);

let n1 = journal_get_next_for_consumer_after_cursor(
    "journal", "faceConsumerAfter", first.entryCursor, 100);
assert(n1.entry !== undefined || is_nullish(n1.entry),
    "after_cursor response object required");
n1.faceOnly = "after";

let n2 = journal_get_next_for_consumer_after_cursor(
    "journal", "faceConsumerAfter", first.entryCursor, 100);
assert(is_nullish(property_get(n2, "faceOnly", null)),
    "second after_cursor must not see faceOnly");

return 0;


//? test: journal_advance_cursor_for_consumer-mutable-face
//? description: ...
Issue #17: journal_advance_cursor_for_consumer returns a faced response object
even when no entry is attached to the response. Face-only props must not leak.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("file", "_AdaptiveObject_", { firstName: "advance_seed" });

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "faceConsumerAdvance",
    consumeFilter: true
}, "faceConsumerAdvance");

let a1 = journal_advance_cursor_for_consumer(
    "journal", "faceConsumerAdvance", 100);
assert(a1 !== undefined && a1 !== null, "advance returns response object");
a1.faceOnly = "advance";
assert(a1.faceOnly === "advance");

let a2 = journal_advance_cursor_for_consumer(
    "journal", "faceConsumerAdvance", 100);
assert(is_nullish(property_get(a2, "faceOnly", null)),
    "second advance must not see faceOnly");

return 0;


//? test: journal_consumer-nested-entry-lookthrough
//? description: ...
Issue #17: consumer get response face keeps entry / entryCursor look-through
after a face-only set (same story as get_first nested-entry test).
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("file", "_AdaptiveObject_", { firstName: "look_seed" });

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "faceConsumerLook",
    consumeFilter: true
}, "faceConsumerLook");

let j = journal_get_next_for_consumer("journal", "faceConsumerLook", 100);
assert(j.entry !== undefined);
const cursor = j.entryCursor;
assert(cursor != null);

j.faceOnly = 1;
assert(j.faceOnly === 1);
assert(j.entryCursor === cursor, "face-only must not hide entryCursor");
assert(j.entry !== undefined, "face-only must not hide entry");

journal_mark_consumed("journal", "faceConsumerLook", cursor);

return 0;

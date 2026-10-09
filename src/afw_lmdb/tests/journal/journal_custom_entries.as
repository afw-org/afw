#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_custom_entries.as
//? customPurpose: Part of lmdb tests
//? description: Writing your own journal entries and consuming them (#520). Same tests as afw tests/file_journal/journal_custom_entries.as except the data adapter id.
//? sourceType: script
//?
//? test: write-and-read-back
//? description: an _AdaptiveJournalEntry_ added to the journal adapter is stored unchanged; objectId is its cursor
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const entry: object = {
    eventType: "orderPlaced",
    source: "orders-service",
    payload: { orderId: "A-100", total: 42.5 }
};
const cursor: string = add_object("journal", "_AdaptiveJournalEntry_", entry).objectId;

const got: object = journal_get_by_cursor("journal", cursor);
assert(got.entryCursor === cursor, "entryCursor is the cursor add_object returned");
assert(got.entry.eventType === "orderPlaced", "entry stored unchanged");
assert(got.entry.payload.orderId === "A-100", "payload stored unchanged");
assert(is_nullish(got.entry.request), "nothing is added to your own entry");

// Each write returns its own cursor (the file journal returned the
// offset 0 for every entry).
const cursor2: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "orderShipped" }).objectId;
assert(cursor2 !== cursor, "second write has its own cursor, got " + cursor2);
assert(journal_get_by_cursor("journal", cursor2).entry.eventType === "orderShipped");
assert(journal_get_by_cursor("journal", cursor).entry.eventType === "orderPlaced");

return 0;


//? test: interleaved-with-adapter-entries
//? description: your entries and the data adapter's entries share one journal, in order
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const data: string = "lmdb";

const first: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "interleave-1" }).objectId;
add_object(data, "_AdaptiveObject_", { firstName: "interleave" });
add_object("journal", "_AdaptiveJournalEntry_", { eventType: "interleave-2" });

const second: object = journal_get_next_after_cursor("journal", first);
assert(second.entry.request.function === "add_object",
    "the adapter's add comes next, got " + string(second.entry));
assert(second.entry.request.adapterId === data);

const third: object = journal_get_next_after_cursor("journal", second.entryCursor);
assert(third.entry.eventType === "interleave-2", "then the second own entry");

return 0;


//? test: consume-with-filter
//? description: a consumeFilter on eventType sees only matching entries; mark consumed; the loop ends when there is no entry
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const data: string = "lmdb";
const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "orders-consumer",
    consumeFilter: "(current::entry.eventType === 'filter-match')"
}, peer);

add_object("journal", "_AdaptiveJournalEntry_", { eventType: "filter-match", n: 1 });
add_object("journal", "_AdaptiveJournalEntry_", { eventType: "filter-other", n: 2 });
add_object(data, "_AdaptiveObject_", { firstName: "filter" });
add_object("journal", "_AdaptiveJournalEntry_", { eventType: "filter-match", n: 3 });

let seen: array = [];
let guard: integer = 0;
let r: object = journal_get_next_for_consumer("journal", peer, 100);
while (r.entry !== undefined) {
    assert(guard < 10, "consume loop did not end");
    push(seen, r.entry.n);
    journal_mark_consumed("journal", peer, r.entryCursor);
    r = journal_get_next_for_consumer("journal", peer, 100);
    guard = guard + 1;
}
assert(seen == [1, 3], "only matching entries, in order, got " + string(seen));

const state: object = get_object("journal", "_AdaptiveProvisioningPeer_", peer);
assert(is_nullish(state.consumeCursor), "nothing left being consumed");
assert(is_defined(state.currentCursor), "peer remembers where it is");

return 0;


//? test: reissue
//? description: an entry that is not marked consumed is returned again with reissue true
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "reissue-consumer",
    consumeFilter: "(current::entry.eventType === 'reissue-me')"
}, peer);
const cursor: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "reissue-me" }).objectId;

const r1: object = journal_get_next_for_consumer("journal", peer, 100);
assert(r1.entryCursor === cursor);
assert(is_nullish(r1.reissue), "first time is not a reissue");

const r2: object = journal_get_next_for_consumer("journal", peer, 100);
assert(r2.entryCursor === cursor, "same entry again");
assert(r2.reissue === true, "second time is a reissue");

journal_mark_consumed("journal", peer, cursor);
const r3: object = journal_get_next_for_consumer("journal", peer, 100);
assert(is_nullish(r3.entry), "nothing after it is marked consumed");

return 0;


//? test: rest-special-object-ids
//? description: the REST forms (GET /journal/_AdaptiveJournalEntry_/<id>) through get_object
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "rest-consumer",
    consumeFilter: "(current::entry.eventType === 'rest-match')"
}, peer);
const c1: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "rest-match", n: 1 }).objectId;
const c2: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "rest-match", n: 2 }).objectId;

const t: string = "_AdaptiveJournalEntry_";

let r: object = get_object("journal", t, c1);
assert(r.entryCursor === c1 && r.entry.n === 1, "bare cursor");

r = get_object("journal", t, "get_by_cursor:" + c1);
assert(r.entryCursor === c1 && r.entry.n === 1, "get_by_cursor:");

r = get_object("journal", t, "get_next_after_cursor:" + c1);
assert(r.entryCursor === c2 && r.entry.n === 2, "get_next_after_cursor:");

r = get_object("journal", t, "get_next_for_consumer:" + peer);
assert(r.entryCursor === c1 && r.entry.n === 1, "get_next_for_consumer:");

// Starts after the given cursor (it used to ignore the cursor).
r = get_object("journal", t,
    "get_next_for_consumer_after_cursor:" + peer + ":" + c1);
assert(r.entryCursor === c2 && r.entry.n === 2,
    "get_next_for_consumer_after_cursor:, got " + string(r.entryCursor));

return 0;


//? test: unknown-consumer-not-found
//? description: a consumerId with no peer object is a not_found error, whatever its form
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

for (const id of [generate_uuid(), "not-a-uuid"]) {
    let caught: any = undefined;
    try {
        journal_get_next_for_consumer("journal", id, 10);
    }
    catch (e) {
        caught = e;
    }
    assert(caught.id === "not_found",
        "get_next_for_consumer with '" + id + "', got " + string(caught.id));

    caught = undefined;
    try {
        journal_mark_consumed("journal", id, "1");
    }
    catch (e) {
        caught = e;
    }
    assert(caught.id === "not_found",
        "mark_consumed with '" + id + "', got " + string(caught.id));
}

return 0;


//? test: rest-mark-consumed
//? description: the REST form of mark consumed, an update of the entry at its cursor (POST /journal/_AdaptiveJournalEntry_/<cursor>)
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "rest-mark-consumer",
    consumeFilter: "(current::entry.eventType === 'rest-mark')"
}, peer);
const c1: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "rest-mark", n: 1 }).objectId;
const c2: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "rest-mark", n: 2 }).objectId;
const t: string = "_AdaptiveJournalEntry_";

let r: object = get_object("journal", t, "get_next_for_consumer:" + peer);
assert(r.entryCursor === c1);

update_object("journal", t, c1, { consumed: true, consumerId: peer });

r = get_object("journal", t, "get_next_for_consumer:" + peer);
assert(r.entryCursor === c2 && is_nullish(r.reissue),
    "moves on after mark consumed, got " + string(r.entryCursor));

// Only the entry being consumed can be marked, and consumed must be true.
assert(safe_evaluate(update_object("journal", t, c1,
    { consumed: true, consumerId: peer }), "error") == "error");
assert(safe_evaluate(update_object("journal", t, c2,
    { consumed: false, consumerId: peer }), "error") == "error");
assert(safe_evaluate(update_object("journal", t, c2,
    { consumed: true }), "error") == "error");

update_object("journal", t, c2, { consumed: true, consumerId: peer });
r = get_object("journal", t, "get_next_for_consumer:" + peer);
assert(is_nullish(r.entry), "nothing left");

return 0;


//? test: end-of-journal
//? description: with no entry to return, neither entry nor entryCursor is set
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const last: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "end-of-journal" }).objectId;

let r: object = journal_get_next_after_cursor("journal", last);
assert(is_nullish(r.entry) && is_nullish(r.entryCursor),
    "after the last entry, got " + string(r));

const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "end-consumer",
    consumeFilter: "(current::entry.eventType === 'never-written')"
}, peer);
r = journal_get_next_for_consumer("journal", peer, 1000);
assert(is_nullish(r.entry) && is_nullish(r.entryCursor),
    "nothing applicable for the consumer, got " + string(r));
r = journal_advance_cursor_for_consumer("journal", peer, 1000);
assert(is_nullish(r.entryCursor), "advance found nothing, got " + string(r));

return 0;


//? test: consumer-scan-limit
//? description: limit stops a consumer scan early; the next call resumes where it stopped (built-in and REST forms)
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const t: string = "_AdaptiveJournalEntry_";

for (const form of ["built-in", "rest"]) {
    const peer: string = generate_uuid();
    add_object("journal", "_AdaptiveProvisioningPeer_", {
        peerId: "limit-consumer",
        consumeFilter: "(current::entry.eventType === 'limit-match-" + form + "')"
    }, peer);
    for (const i of [1, 2, 3, 4, 5]) {
        add_object("journal", t, { eventType: "limit-skip" });
    }
    const match: string = add_object("journal", t,
        { eventType: "limit-match-" + form }).objectId;

    let calls: integer = 0;
    let r: object = {};
    while (r.entry === undefined) {
        assert(calls < 20, form + ": scan never reached the match");
        r = (form === "built-in")
            ? journal_get_next_for_consumer("journal", peer, 2)
            : get_object("journal", t, "get_next_for_consumer:" + peer + ":2");
        calls = calls + 1;
    }
    assert(r.entryCursor === match, form + ": found the match");
    assert(calls > 1, form + ": limit 2 needed more than one call, took " +
        string(calls));
    journal_mark_consumed("journal", peer, match);
}

// Bad REST limits.
const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "bad-limit" }, peer);
for (const bad of [":0", ":x", ":", ":2:3"]) {
    assert(safe_evaluate(get_object("journal", t,
        "get_next_for_consumer:" + peer + bad), "error") == "error",
        "limit '" + bad + "' is an error");
}

return 0;


//? test: advance-cursor-for-consumer
//? description: advancing moves advanceCursor to the next applicable entry without starting to consume it; get_next_for_consumer then returns it
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peer: string = generate_uuid();
add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "advance-consumer",
    consumeFilter: "(current::entry.eventType === 'advance-match')"
}, peer);
add_object("journal", "_AdaptiveJournalEntry_", { eventType: "advance-skip" });
const match: string = add_object("journal", "_AdaptiveJournalEntry_",
    { eventType: "advance-match" }).objectId;

const a: object = journal_advance_cursor_for_consumer("journal", peer, 1000);
assert(a.entryCursor === match, "advance found the match, got " + string(a));
assert(is_nullish(a.entry), "advance does not return the entry");

const state: object = get_object("journal", "_AdaptiveProvisioningPeer_", peer);
assert(state.advanceCursor === match, "advanceCursor is the match");
assert(is_nullish(state.consumeCursor), "advancing does not start consuming");

const r: object = journal_get_next_for_consumer("journal", peer, 1000);
assert(r.entryCursor === match && is_nullish(r.reissue),
    "get_next_for_consumer returns it, got " + string(r));

return 0;


//? test: journal-add-entry
//? description: journal_add_entry stores the entry as given and returns its cursor
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const cursor: string = journal_add_entry("journal",
    { eventType: "added-by-function", n: 7 });
const got: object = journal_get_by_cursor("journal", cursor);
assert(got.entryCursor === cursor);
assert(got.entry == { eventType: "added-by-function", n: 7 },
    "stored as given, got " + string(got.entry));

const cursor2: string = journal_add_entry("journal", { eventType: "second" });
assert(journal_get_next_after_cursor("journal", cursor).entryCursor === cursor2,
    "next entry after it");

return 0;

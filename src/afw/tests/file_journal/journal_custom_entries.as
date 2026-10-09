#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_custom_entries.as
//? customPurpose: Part of file journal tests
//? description: Writing your own journal entries and consuming them (#520). Same tests as afw_lmdb tests/journal/journal_custom_entries.as except the data adapter id.
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

const data: string = "file";

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

const data: string = "file";
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

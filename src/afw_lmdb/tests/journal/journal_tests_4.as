#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_tests_4.as
//? customPurpose: Part of lmdb tests
//? description: Regression for afw#231 (peer objects created via add_object() were stored under a different LMDB key than the journal consumer functions looked them up with) plus a related get_next_for_consumer_after_cursor skip-cursor fix.
//? sourceType: script
//?
//? test: journal_get_next_for_consumer
//? description: a peer created via add_object() must be discoverable by journal_get_next_for_consumer()
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peerUuid: string = generate_uuid();

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "consumer1"
}, peerUuid);

// sanity: the peer is readable the normal way
const peer: object = get_object("journal", "_AdaptiveProvisioningPeer_", peerUuid);
assert(peer.peerId == "consumer1");

// generate a journal entry for the consumer to find
add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });

const result: object = journal_get_next_for_consumer("journal", peerUuid, 10);
assert(is_defined(result.entryCursor), "expected an entryCursor, peer lookup likely failed");

return 0;


//? test: journal_get_next_for_consumer_after_cursor
//? description: peer must be discoverable, and the passed-in cursor's own entry must be skipped rather than re-returned
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peerUuid: string = generate_uuid();

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "consumer2"
}, peerUuid);

add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });
add_object("lmdb", "_AdaptiveObject_", { firstName: "sue" });

// entry_cursor "1" is the entry we've already seen; the next one is "2"
const result: object = journal_get_next_for_consumer_after_cursor(
    "journal", peerUuid, "1", 10);
assert(is_defined(result.entryCursor), "expected an entryCursor, peer lookup likely failed");
assert(result.entryCursor == "2",
    "journal_get_next_for_consumer_after_cursor should skip the passed-in cursor and return the next entry, got " + string(result.entryCursor));

return 0;


//? test: journal_advance_cursor_for_consumer
//? description: peer created via add_object() must be discoverable by journal_advance_cursor_for_consumer()
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peerUuid: string = generate_uuid();

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "consumer3"
}, peerUuid);

add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });

// must not throw "provisioning peer not found"
journal_advance_cursor_for_consumer("journal", peerUuid, 10);

return 0;


//? test: journal_mark_consumed
//? description: peer created via add_object() must be discoverable by journal_mark_consumed(), and an unknown consumer must throw rather than dereference a null peer
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peerUuid: string = generate_uuid();

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "consumer4"
}, peerUuid);

add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });

const result: object = journal_get_next_for_consumer("journal", peerUuid, 10);
assert(is_defined(result.entryCursor));

// must not throw "provisioning peer not found"
journal_mark_consumed("journal", peerUuid, result.entryCursor);

// an unknown consumerId must throw, not crash on a null peer
assert(
    safe_evaluate(
        journal_mark_consumed("journal", generate_uuid(), result.entryCursor),
        "error"
    ) == "error",
    "journal_mark_consumed should throw for an unknown consumer"
);

return 0;


//? test: journal_get_next_for_consumer-consumeFilter
//? description: consumeFilter must actually be evaluated (issue #240) - a filter matching only one of two new entries must skip the non-matching one and land on the match
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peerUuid: string = generate_uuid();

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "consumer5",
    consumeFilter: "eq(property_get(property_get(property_get(current::entry, 'request', {}), 'object', {}), 'firstName', null), 'filterMatchMe')"
}, peerUuid);

add_object("lmdb", "_AdaptiveObject_", { firstName: "filterSkipMe" });
add_object("lmdb", "_AdaptiveObject_", { firstName: "filterMatchMe" });

const result: object = journal_get_next_for_consumer("journal", peerUuid, 20);
assert(is_defined(result.entryCursor), "expected an entryCursor, consumeFilter should have found the matching entry");
assert(result.entry.request.object.firstName == "filterMatchMe",
    "consumeFilter must skip non-matching entries and land on the match, got " + string(result.entry.request.object.firstName));

return 0;


//? test: journal_get_next_for_consumer-consumeFilter-no-match
//? description: a consumeFilter matching nothing must not fall back to returning an arbitrary entry
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const peerUuid: string = generate_uuid();

add_object("journal", "_AdaptiveProvisioningPeer_", {
    peerId: "consumer6",
    consumeFilter: "eq(property_get(property_get(property_get(current::entry, 'request', {}), 'object', {}), 'firstName', null), 'thisValueNeverExists')"
}, peerUuid);

add_object("lmdb", "_AdaptiveObject_", { firstName: "irrelevant" });

const result: object = journal_get_next_for_consumer("journal", peerUuid, 20);
assert(is_nullish(result.entry), "consumeFilter matching nothing must not return an entry");

return 0;

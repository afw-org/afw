#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_alias_consumer.as
//? customPurpose: Part of lmdb tests
//? description: A journal consumerId is the peer's objectId, which may be a non-UUID alias (IdIndex) like any other LMDB object id.
//? sourceType: script
//?
//? test: alias-consumer-get-next-and-mark-consumed
//? description: a peer added with a non-UUID objectId is found by get_next_for_consumer and mark_consumed, and the cursor is kept for the alias
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "p1" }, "aliasPeer1");

add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });
add_object("lmdb", "_AdaptiveObject_", { firstName: "sue" });

const first: object = journal_get_next_for_consumer("journal", "aliasPeer1", 10);
assert(first.entryCursor == "1", "first entry, got " + string(first.entryCursor));

journal_mark_consumed("journal", "aliasPeer1", first.entryCursor);

const second: object = journal_get_next_for_consumer("journal", "aliasPeer1", 10);
assert(second.entryCursor == "2",
    "after mark_consumed the next entry, got " + string(second.entryCursor));

return 0;


//? test: alias-consumer-after-cursor-and-advance
//? description: get_next_for_consumer_after_cursor and advance_cursor_for_consumer find a peer by its alias
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "p2" }, "aliasPeer2");

add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });
add_object("lmdb", "_AdaptiveObject_", { firstName: "sue" });

const result: object = journal_get_next_for_consumer_after_cursor(
    "journal", "aliasPeer2", "1", 10);
assert(result.entryCursor == "2", "entry after cursor 1, got " + string(result.entryCursor));

journal_advance_cursor_for_consumer("journal", "aliasPeer2", 10);

return 0;


//? test: alias-consumer-not-found
//? description: an unknown alias, a 36 character non-UUID, an alias of another object type, and a deleted alias peer are all not_found
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function errorId(consumerId: string): any {
    try {
        journal_get_next_for_consumer("journal", consumerId, 10);
    }
    catch (e) {
        return e.id;
    }
    return undefined;
}

add_object("lmdb", "_AdaptiveObject_", { firstName: "bob" });

assert(errorId("noSuchPeer") === "not_found",
    "unknown alias, got " + string(errorId("noSuchPeer")));

const len36: string = "abcdefghijklmnopqrstuvwxyz0123456789";
assert(errorId(len36) === "not_found",
    "36 character non-UUID, got " + string(errorId(len36)));

// An alias belongs to its object type; the same id on another type is not a peer.
add_object("journal", "_AdaptiveObject_", { x: 1 }, "notAPeer");
assert(errorId("notAPeer") === "not_found",
    "alias of another object type, got " + string(errorId("notAPeer")));

add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "p3" }, "aliasPeer3");
assert(is_defined(journal_get_next_for_consumer("journal", "aliasPeer3", 10).entryCursor));
delete_object("journal", "_AdaptiveProvisioningPeer_", "aliasPeer3");
assert(errorId("aliasPeer3") === "not_found",
    "deleted alias peer, got " + string(errorId("aliasPeer3")));

return 0;

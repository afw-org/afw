#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_consumer_id.as
//? customPurpose: Part of file journal tests
//? description: A file journal consumer id is the peer's file name, so it must be one file name under the journal root.
//? sourceType: script
//?
//? test: consumer-id-must-be-a-file-name
//? description: a consumer id containing '/' or that is '..' is an argument error and changes nothing
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// A real peer, so the peer directory exists.
add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "real" }, "real");
add_object("journal", "_AdaptiveJournalEntry_", { eventType: "x" });

const before: object = get_object("file", "TestObjectType1", "Test1");

for (const id of ["../../objects/TestObjectType1/Test1", "..", "a/b"]) {
    let caught: any = undefined;
    try {
        journal_get_next_for_consumer("journal", id, 10);
    }
    catch (e) {
        caught = e;
    }
    assert(caught.id === "argument_error",
        "get_next_for_consumer with '" + id + "', got " + string(caught.id));

    caught = undefined;
    try {
        journal_mark_consumed("journal", id, "1");
    }
    catch (e) {
        caught = e;
    }
    assert(caught.id === "argument_error",
        "mark_consumed with '" + id + "', got " + string(caught.id));
}

const after: object = get_object("file", "TestObjectType1", "Test1");
assert(after == before, "object unchanged, got " + string(after));

return 0;

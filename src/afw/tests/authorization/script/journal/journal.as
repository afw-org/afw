#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: journal.as
//? customPurpose: Part of authorization tests
//? description: Journal operations are authorized like object operations (#520). The handler in afw.conf denies journal entries on "journal".
//? sourceType: script
//?
//? test: object-changes-still-journaled
//? description: an allowed object change is journaled even though the user may not touch journal entries
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const id: string = add_object("file", "_AdaptiveObject_", { firstName: "auth" }).objectId;
assert(get_object("file", "_AdaptiveObject_", id).firstName === "auth");

return 0;


//? test: journal-entry-operations-denied
//? description: every way to add, read, consume, or mark a journal entry is denied
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "p" }, "p");
const t: string = "_AdaptiveJournalEntry_";

let notDenied: array = [];
function check(name: string, f: function): void {
    let id: any = "no error";
    try { f(); } catch (e) { id = e.id; }
    if (id !== "denied") { push(notDenied, name + ": " + string(id)); }
}
check("add_object", function () { add_object("journal", t, { e: 1 }); });
check("journal_add_entry", function () { journal_add_entry("journal", { e: 1 }); });
check("journal_get_first", function () { journal_get_first("journal"); });
check("journal_get_by_cursor", function () { journal_get_by_cursor("journal", "x"); });
check("journal_get_next_after_cursor", function () { journal_get_next_after_cursor("journal", "x"); });
check("journal_get_next_for_consumer", function () { journal_get_next_for_consumer("journal", "p", 10); });
check("journal_get_next_for_consumer_after_cursor", function () { journal_get_next_for_consumer_after_cursor("journal", "p", "x", 10); });
check("journal_advance_cursor_for_consumer", function () { journal_advance_cursor_for_consumer("journal", "p", 10); });
check("journal_mark_consumed", function () { journal_mark_consumed("journal", "p", "x"); });
check("get_object special id", function () { get_object("journal", t, "get_first"); });
check("get_object mark_consumed", function () { get_object("journal", t, "mark_consumed:p:x"); });

assert(length(notDenied) === 0, "not denied: " + string(notDenied));

return 0;

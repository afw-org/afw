#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: resource_ids.as
//? customPurpose: Part of authorization tests
//? description: The built-in and the REST form of each journal operation are authorized on the same path (#520). The handler in afw.conf permits only the listed journal paths.
//? sourceType: script
//?
//? test: same-path-for-built-in-and-rest
//? description: each built-in and its REST form (with and without a limit) is permitted by the one path listed for it
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "p" }, "p");
const t: string = "_AdaptiveJournalEntry_";

let denied: array = [];
function check(name: string, f: function): void {
    let id: any = "no error";
    try { f(); } catch (e) { id = e.id; }
    if (id === "denied") { push(denied, name); }
}
check("add_object", function () { add_object("journal", t, { e: 1 }); });
check("journal_add_entry", function () { journal_add_entry("journal", { e: 1 }); });
check("journal_get_first", function () { journal_get_first("journal"); });
check("get_first", function () { get_object("journal", t, "get_first"); });
check("journal_get_by_cursor", function () { journal_get_by_cursor("journal", "1"); });
check("1", function () { get_object("journal", t, "1"); });
check("get_by_cursor:1", function () { get_object("journal", t, "get_by_cursor:1"); });
check("journal_get_next_after_cursor", function () { journal_get_next_after_cursor("journal", "1"); });
check("get_next_after_cursor:1", function () { get_object("journal", t, "get_next_after_cursor:1"); });
check("journal_get_next_for_consumer", function () { journal_get_next_for_consumer("journal", "p", 10); });
check("get_next_for_consumer:p", function () { get_object("journal", t, "get_next_for_consumer:p"); });
check("get_next_for_consumer:p:10", function () { get_object("journal", t, "get_next_for_consumer:p:10"); });
check("journal_get_next_for_consumer_after_cursor", function () { journal_get_next_for_consumer_after_cursor("journal", "p", "1", 10); });
check("get_next_for_consumer_after_cursor:p:1:10", function () { get_object("journal", t, "get_next_for_consumer_after_cursor:p:1:10"); });
check("journal_advance_cursor_for_consumer", function () { journal_advance_cursor_for_consumer("journal", "p", 10); });
check("advance_cursor_for_consumer:p:10", function () { get_object("journal", t, "advance_cursor_for_consumer:p:10"); });
check("journal_mark_consumed", function () { journal_mark_consumed("journal", "p", "1"); });
check("mark_consumed:p:1", function () { get_object("journal", t, "mark_consumed:p:1"); });

assert(length(denied) === 0, "denied: " + string(denied));

return 0;


//? test: other-consumer-denied
//? description: a path names its consumer, so a handler can permit one consumer and not another
//? expect: 0
//? source: ...
#!/usr/bin/env afw

add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "q" }, "q");
const t: string = "_AdaptiveJournalEntry_";

let notDenied: array = [];
function check(name: string, f: function): void {
    let id: any = "no error";
    try { f(); } catch (e) { id = e.id; }
    if (id !== "denied") { push(notDenied, name + ": " + string(id)); }
}
check("journal_get_next_for_consumer", function () { journal_get_next_for_consumer("journal", "q"); });
check("get_next_for_consumer:q", function () { get_object("journal", t, "get_next_for_consumer:q"); });
check("journal_mark_consumed", function () { journal_mark_consumed("journal", "q", "1"); });
check("mark_consumed:q:1", function () { get_object("journal", t, "mark_consumed:q:1"); });

assert(length(notDenied) === 0, "not denied: " + string(notDenied));

return 0;

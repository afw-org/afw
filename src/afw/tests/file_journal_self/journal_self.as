#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_self.as
//? customPurpose: Part of file journal tests
//? description: An adapter whose journalAdapterId is its own id starts and journals its own changes (#520).
//? sourceType: script
//?
//? test: object-changes-journaled-to-self
//? description: adds and modifies on the adapter are journaled in its own journal
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const id: string = add_object("file", "_AdaptiveObject_", { firstName: "self" }).objectId;
modify_object("file", "_AdaptiveObject_", id, [["set_property", "firstName", "self2"]]);

const first: object = journal_get_first("file");
assert(first.entry.request.function === "add_object", "add journaled, got " + string(first.entry));
assert(first.entry.objectId === id);

const second: object = journal_get_next_after_cursor("file", first.entryCursor);
assert(second.entry.request.function === "modify_object", "modify journaled");

return 0;


//? test: own-entry-on-self-journal
//? description: an entry you write lands in the journal once as written, followed by the journaled add of it
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const cursor: string = add_object("file", "_AdaptiveJournalEntry_",
    { eventType: "selfEvent" }).objectId;

const own: object = journal_get_by_cursor("file", cursor);
assert(own.entry.eventType === "selfEvent", "own entry as written");

const next: object = journal_get_next_after_cursor("file", cursor);
assert(next.entry.request.objectType === "_AdaptiveJournalEntry_",
    "then the add of it, as an ordinary journaled change, got " + string(next.entry));
assert(next.entry.objectId === cursor);

return 0;

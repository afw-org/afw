#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: journal_hours.as
//? customPurpose: Part of file journal tests
//? description: The file journal keeps one file per hour. Reads and consumers cross from one hourly file to the next (#520). config.py starts each run with a journal last written in hour 2020-01-01 00.
//? sourceType: script
//?
//? test: cross-to-next-hour
//? description: an entry written in a later hour goes in a new file; reads and a consumer go from the old file to the new one
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// The two entries already in the 2020-01-01 00 file.
const first: object = journal_get_first("journal");
assert(first.entryCursor === "2020010100_0" && first.entry.n === 1,
    "first, got " + string(first));
const second: object = journal_get_next_after_cursor("journal", first.entryCursor);
assert(starts_with(second.entryCursor, "2020010100_") && second.entry.n === 2,
    "second, got " + string(second));
assert(is_nullish(journal_get_next_after_cursor("journal", second.entryCursor).entry),
    "nothing after the old hour yet");

// Written now, so in a new hourly file.
const third: string = journal_add_entry("journal", { eventType: "hour-new", n: 3 });
assert(!starts_with(third, "2020010100_"), "new hourly file, got " + third);

// Reading after the old file's last entry crosses to the new file.
const next: object = journal_get_next_after_cursor("journal", second.entryCursor);
assert(next.entryCursor === third && next.entry.n === 3,
    "crossed to the new hour, got " + string(next));
assert(journal_get_by_cursor("journal", first.entryCursor).entry.n === 1,
    "old entries still readable by cursor");

// A consumer from the start sees all of them, in order.
const fourth: string = journal_add_entry("journal", { eventType: "hour-new", n: 4 });
const peer: string = "hours-consumer";
add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "hours" }, peer);
let got: array = [];
let r: object = journal_get_next_for_consumer("journal", peer, 100);
while (r.entry !== undefined) {
    push(got, r.entry.n);
    journal_mark_consumed("journal", peer, r.entryCursor);
    r = journal_get_next_for_consumer("journal", peer, 100);
}
assert(got == [1, 2, 3, 4], "consumer got " + string(got));

// With a limit of 1, a consumer also crosses the hour.
const limited: string = "hours-limited";
add_object("journal", "_AdaptiveProvisioningPeer_", { peerId: "limited" }, limited);
got = [];
let calls: integer = 0;
while (length(got) < 4) {
    calls = calls + 1;
    assert(calls < 50, "limited consumer stuck after " + string(got));
    r = journal_get_next_for_consumer("journal", limited, 1);
    if (r.entry !== undefined) {
        push(got, r.entry.n);
        journal_mark_consumed("journal", limited, r.entryCursor);
    }
}
assert(got == [1, 2, 3, 4], "limited consumer got " + string(got));

return 0;

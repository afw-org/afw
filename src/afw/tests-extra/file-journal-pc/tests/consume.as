/* Consume inbox journal onto outbox. Bump N with produce.as / compare.as. */
const N = 100;
const consumerId = "pc-consumer";

add_object("journal", "_AdaptiveProvisioningPeer_", {
    "peerId": consumerId
}, consumerId);

let n = 0;
while (n < N) {
    const result = journal_get_next_for_consumer("journal", consumerId, 200);
    if (!is_defined(result.entryCursor) || is_nullish(result.entry)) {
        break;
    }
    const req = result.entry.request;
    const id = req.suggestedObjectId;
    const obj = req.object;
    add_object("outbox", "Journal", {
        "n": obj.n,
        "msg": obj.msg
    }, id);
    journal_mark_consumed("journal", consumerId, result.entryCursor);
    n = n + 1;
}
assert(n === N, "consumed " + string(n) + " of " + string(N));
return n;

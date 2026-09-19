# file-journal-pc

Clean producer → consumer → compare on **two** adapters (`inbox`,
`outbox`). First shot **N = 100** (`produce.as` / `consume.as` /
`compare.as` — bump all three).

Produce writes Journal objects on `inbox` (`journalAdapterId`:
`journal`). Consume uses `journal_get_next_for_consumer` /
`journal_mark_consumed` with an `_AdaptiveProvisioningPeer_` on
`journal` and writes the same ids onto `outbox`. Compare still
get_object both sides. All three adapter types are only in
`afw.conf`.

Adapter type lives only in `afw.conf` (file for now). Swap later
without changing the scripts. Sequential produce → consume → compare
for this first shot; more journals can be extra adapters later.

```bash
afwdev test -T src/afw/tests-extra/file-journal-pc --show-all
```

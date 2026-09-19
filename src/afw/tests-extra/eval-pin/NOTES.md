# eval-pin (opt-in afwfcgi)

`compiled_value` evaluate no longer `clone_unmanaged`s onto dest `p`.
It takes `script_result` (already assignable) and
`afw_pool_release_value_at_cleanup` on dest `p`. Caller treats the
result as a temp of dest `p`, or `get_reference` to keep it.

These scripts try to break that: mutate after inner evaluate, two
evaluates of one unit as distinct values, closures, throw after eval,
`s = s + s`, firehose of the cheap cases.

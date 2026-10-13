#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: regexp_request_budget.as
//? description: A regular expression match gets at most half of what the request has left under limitRequestPoolBytes for its backtrack stack (ICU allocates it outside the pools)
//? sourceType: script
//?
//? test: backtrack-stack-follows-request-budget
//? description: With limitRequestPoolBytes 4 MiB, ^(a|b)*$ on 131,072 a (which fits the 8 MB maximum) stops with payload_too_large; on 20,000 a it matches.
//? expect: [4194304, "payload_too_large", true]
//? source: ...

let t = "a";
for (let i: integer = 0; i < 17; i = i + 1) {
    t = t + t;
}
let id = "no error";
try {
    regexp_match(t, /^(a|b)*$/);
}
catch (e) {
    id = e.id;
}
return [
    process::limitRequestPoolBytes,
    id,
    regexp_match(substring(t, 0, 20000), /^(a|b)*$/)
];

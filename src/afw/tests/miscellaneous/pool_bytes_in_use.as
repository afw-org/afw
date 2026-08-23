#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: pool_bytes_in_use.as
//? customPurpose: Part of miscellaneous category tests
//? description: pool_bytes_in_use and process_rss (#242 probes)
//? sourceType: script
//?
//? test: pool_bytes_in_use-type
//? description: pool_bytes_in_use is a non-negative integer
//? expect: 0
//? source: ...

let n = pool_bytes_in_use();
assert(meta(n).dataType === "integer");
assert(n >= 0);
return 0;

//?
//? test: process_rss-type
//? description: process_rss is a non-negative integer (KB)
//? expect: 0
//? source: ...

let r = process_rss();
assert(meta(r).dataType === "integer");
assert(r >= 0);
return 0;

//?
//? test: in-use-vs-rss
//? description: RSS is at least a hint; in_use is finite
//? expect: 0
//? source: ...

let n = pool_bytes_in_use();
let r = process_rss();
assert(n >= 0);
assert(r >= 0);
return 0;

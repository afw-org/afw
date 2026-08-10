#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: mini_suite.as
//? description: Tiny suite fed as one orchestrated work item
//? sourceType: script
//?
//? test: one
//? description: first case
//? expect: 1
//? source: ...

return 1;

//?
//? test: two
//? description: second case
//? expect: "ok"
//? source: ...

return "ok";

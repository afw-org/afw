#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/xpath_node_count.as
//? customPurpose: Part of core function tests
//? description: xpath_node_count is currently an unimplemented stub. This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: xpath_node_count-error-not-implemented
//? description: xpath_node_count always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

xpath_node_count(xpathExpression("//a"))

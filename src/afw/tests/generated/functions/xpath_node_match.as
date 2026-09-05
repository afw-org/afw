#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/xpath_node_match.as
//? customPurpose: Part of core function tests
//? description: xpath_node_match is currently an unimplemented stub. This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: xpath_node_match-error-not-implemented
//? description: xpath_node_match always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

xpath_node_match(xpathExpression("//a"), xpathExpression("//b"))

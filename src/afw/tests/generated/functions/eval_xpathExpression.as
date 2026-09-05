#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/eval_xpathExpression.as
//? customPurpose: Part of core function tests
//? description: eval_xpathExpression is currently an unimplemented stub. This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: eval_xpathExpression-error-not-implemented
//? description: eval_xpathExpression always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

eval_xpathExpression(xpathExpression("//a"))

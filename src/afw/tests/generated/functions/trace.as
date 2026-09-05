#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/trace.as
//? customPurpose: Part of core function tests
//? description: Tests for the trace function.
//? sourceType: script
//?
//? test: trace-error-priority-out-of-range
//? description: trace throws when the priority parameter is not between 1 and 8
//? expect: error:Parameter 3 must be an integer between 1 and 8.
//? source: ...

trace("hi", true, integer(9))

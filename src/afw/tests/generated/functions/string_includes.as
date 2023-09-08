#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_includes.as
//? customPurpose: Part of core function tests
//? description: Tests for includes<string> function.
//? sourceType: script
//?
//? test: includes<string>-1
//? description: true
//? expect: boolean(true)
//? source: ...

includes<string>(
    string("alice is in the woods"),
    string("alice")
)

//?
//? test: includes<string>-2
//? description: false
//? expect: boolean(false)
//? source: ...

includes<string>(
    string("alice"),
    string("alice is in the woods")
)

//?
//? test: includes<string>-3
//? description: middle
//? expect: boolean(true)
//? source: ...

includes<string>(
    string("alice is in the woods"),
    string("in the")
)

//?
//? test: includes<string>-4
//? description: end
//? expect: boolean(true)
//? source: ...

includes<string>(
    string("alice is in the woods"),
    string("oods")
)

//?
//? test: includes<string>-5
//? description: same
//? expect: boolean(true)
//? source: ...

includes<string>(
    string("alice"),
    string("alice")
)


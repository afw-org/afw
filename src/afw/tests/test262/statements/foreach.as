#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: foreach.as
//? customPurpose: Part of test262
//? description: Tests the foreach statement
//? sourceType: script
//?
//? test: foreach-1
//? description: lists should be able to be traversed using 'foreach' loop
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc array: list = [0, 'a', true, false, null];
loc i: integer = 0;

foreach loc value: any of array {
    assert(value === array[i], string("element at index ", i, " is not equal"));
    i = i + 1;
}

return 0;

//? test: foreach-2
//? description: flow during body evaluation should honor 'break' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc array: list = [0, 1, 2];
loc i: integer = 0;

foreach loc x: integer of array {
    i = i + 1;
    break;

    assert(false, "should not get here");
}

return 0;

//? test: foreach-3
//? description: flow during body evaluation should honor 'continue' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc array: list = [0, 1, 2];
loc i: integer = 0;

foreach loc x: integer of array {
    i = i + 1;
    continue;

    assert(false, "should not get here");
}

return 0;

//? test: foreach-3
//? description: flow during body evaluation should honor 'return' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc array: list = [0, 1, 2];
loc i: integer = 0;

foreach loc x: integer of array {
    i = i + 1;
    return 0;

    assert(false, "should not get here");
}
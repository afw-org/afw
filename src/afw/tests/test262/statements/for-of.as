#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: for-of.as
//? customPurpose: Part of test262
//? description: Tests the for-of statement
//? sourceType: script
//?
//? test: for-of-1
//? description: lists should be able to be traversed using 'for-of' loop
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: list = [0, 'a', true, false, null];
let i: integer = 0;

for (let value of array) {
    assert(value === array[i], string("element at index ", i, " is not equal"));
    i = i + 1;
}

return 0;

//? test: for-of-2
//? description: flow during body evaluation should honor 'break' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: list = [0, 1, 2];
let i: integer = 0;

for (let x of array) {
    i = i + 1;
    break;

    throw "should not get here";
}

return 0;

//? test: for-of-3
//? description: flow during body evaluation should honor 'continue' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: list = [0, 1, 2];
let i: integer = 0;

for (let x of array) {
    i = i + 1;
    continue;

    throw "should not get here";
}

return 0;

//? test: for-of-3
//? description: flow during body evaluation should honor 'return' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: list = [0, 1, 2];
let i: integer = 0;

for (let x of array) {
    i = i + 1;
    return 0;

    throw "should not get here";
}
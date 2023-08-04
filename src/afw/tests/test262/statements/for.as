#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: for.as
//? customPurpose: Part of test262
//? description: Tests for statement
//? sourceType: script
//?
//? test: for-1
//? description: condition is false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let count: integer = 0;
for (let i: integer = 0; false;) {
    count = count + 1;
}
assert(count === 0);

return 0;

//? test: for-2
//? description: condition is null
//? expect: error
//? source: ...
#!/usr/bin/env afw

let count: integer = 0;
for (let i: integer = 0; null;) {
    count = count + 1;
}
assert(count === 0);

return 0;

//? test: for-3
//? description: Simple test of continue loop with using "continue"
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let str: string;
str = "";

for (let index: integer = 0; index < 10; index = index + 1) {
    if (index < 5) continue;
    str = str + string(index);
}
assert(str === "56789", "invalid str = " + str);

return 0;

//? test: for-4
//? description: Breaking a loop with "break"
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let str: string;
str = "";

for (let index: integer = 0; index < 10; index = index + 1) {
    if (index > 5) break;
    str += string(index);
}
assert(str === "012345", "invalid str = " + str);

return 0;

//? test: for-5
//? description: Declaring variable in "for" expression
//? expect: 0
//? source: ...
#!/usr/bin/env afw

for (let index: integer = 0; index < 6; index = index + 1) {
    ;
}

return 0;

//? test: for-6
//? description: Using +,*,/, as the second expression
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let i: integer;
let j: integer;

for (i = 0; i < 10; i = i + 1) {}
assert(i === 10, "Expected i = 10, got " + string(i));

j = 0;
for (i = 1; i < 10; i = i * 2) {
    j = j + 1;
}
assert(i === 16, "Expected i = 16, got " + string(i));
assert(j === 4, "Expected j = 5, got " + string(j));

j = 0;
for (i = 16; i > 1; i = i / 2) {
    j = j + 1;
}
assert(i === 1, "Expected i = 1, got " + string(i));
assert(j === 4, "Expected j = 4, got " + string(j));

j = 0;
for (i = 10; i > 1; i = i - 1) {
    j = j + 1;
}
assert(i === 1, "Expected i = 1, got " + string(i));
assert(j === 9, "Expected j = 9, got " + string(j));

j = 0;
for (i = 2; i < 10; i = i * i) {
    j = j + 1;
}
assert(i === 16, "Expected i = 16, got " + string(i));
assert(j === 2, "Expected j = 2, got " + string(j));

return 0;


//? test: for-7
//? description: Statements must be evaluated before second expression is evaluated
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let i: integer;
for (i = 0; i < 10; i = i + 1) {
    i = i * 2;
    break;
}
assert(i === 0, "Expected i = 0, got " + string(i));

for (i = 0; i < 10; i = i + 1) {
    i = i * 2;
    assert(i !== 3, "Expected i != 3");
}

return 0;

//? test: for-8
//? description: Only 3 expressions allowed
//? expect: error:Parse error at offset 78 around line 3 column 59: Expecting ',' or ')'
//? source: ...
#!/usr/bin/env afw

for (let index: integer = 0; index < 10; index = index + 1; index = index - 1);

//? test: for-9
//? description: Blocks not allowed in third expression
//? expect: error:Parse error at offset 80 around line 3 column 61: Expecting ',' or '}'
//? source: ...
#!/usr/bin/env afw

for (let index: integer = 0; index < 100; {index = index + 1; index = index * 2;}) {
    ;
}

return 0;

//? test: for-10
//? description: Blocks not allowed in second expression
//? expect: error:Parse error at offset 56 around line 3 column 37: Name of an object value must be followed by a colon
//? source: ...
#!/usr/bin/env afw

for (let index: integer = 0; {index = index + 1; index < 100;}; index = index * 2;) {
    ;
}

return 0;

//? test: for-11
//? description: Blocks not allowed in first expression
//? expect: error:Parse error at offset 55 around line 4 column 16: Expecting ',' or '}'
//? source: ...
#!/usr/bin/env afw

let index: integer;
for ({index = 0; index = index + 1;}; index < 100; index = index * 2;) {
    ;
}

return 0;

//? test: for-12
//? description: Allow destructuring list in for expression
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let value: integer;

for (let [x: integer] = [23]; ; ) {
    value = x;
    break;
}

assert(value === 23, "Expected value to be 23, got " + string(value));

return 0;

//? test: for-13
//? description: Allow newline in for expression
//? expect: 0
//? source: ...
#!/usr/bin/env afw

for (; false; ) let
x : integer = 1;

return 0;
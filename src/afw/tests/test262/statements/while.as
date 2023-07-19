#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: while.as
//? customPurpose: Part of test262
//? description: Tests the while statement
//? sourceType: script
//?
//? test: while-1
//? description: evaluating various expressions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let in_while: integer;

while (false) in_while = 1;
assert(in_while !== 1, "while (false) in_while = 1");

return 0;


//? test: while-3
//? description: "break" within a "while" statement
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let in_do_before_break: integer;
let in_do_after_break: integer;

while (1 === 1) {
    in_do_before_break = 1;
    break;
    in_do_after_break = 1;    
}

assert(in_do_before_break === 1, "in_do_before_break === 1");
assert(in_do_after_break !== 1, "in_do_after_break !== 1");

return 0;
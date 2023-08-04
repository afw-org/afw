#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: do-while.as
//? customPurpose: Part of test262
//? description: Tests the do-while statement
//? sourceType: script
//?
//? test: do-while-1
//? description: when the production "do Statement while ( Expression )" is evaluated, Statement is evaluated first
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let in_do: integer;

do in_do=1; while (false);

assert(in_do === 1, "the inner statement of a do-loop should be evaluated before the expression: false");

return 0;

//? test: do-while-2
//? description: checking if execution of "do let x: integer = 1; let y: integer = 2; while (false)" fails
//? expect: error:Parse error at offset 43 around line 3 column 24: Expecting 'while'
//? source: ...
#!/usr/bin/env afw

do let x: integer = 1; let y: integer = 2; while (false);

return 0;

//? test: do-while-3
//? description: "break" within "do-while" statement is allowed
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let in_do_before_break: string;
let in_do_after_break: string;

do {
    in_do_before_break = "reached";
    break;
    in_do_after_break = "where am i";
} while (2 === 1);

assert(in_do_before_break === "reached", "in_do_before_break was not reached");
assert(in_do_after_break === undefined, "in_do_after_break was reached");

return 0;

//? test: do-while-4
//? description: block within "do-while" expression is not allowed
//? expect: error:Parse error at offset 40 around line 5 column 10: Invalid property name
//? source: ...
#!/usr/bin/env afw

do {
    ;
} while({true});

return 0;
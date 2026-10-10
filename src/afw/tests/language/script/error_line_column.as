#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: error_line_column.as
//? customPurpose: Part of language/script tests
//? description: ...
A caught error's line and column are those of its offset in the whole
source (line and column start at 1), not counted inside the text of the
value that threw.
//? sourceType: script
//?
//? test: error-line-column-in-script
//? description: line and column of an assertion on line 5 (source starts with a blank line)
//? expect: "[5,5]"
//? source: ...

let a = 1;
let b = 2;
try {
    assert(false, "x");
} catch (e) {
    return stringify([e.line, e.column]);
}

//?
//? test: error-line-column-first-line
//? description: an error on the first line of code (line 2) points at the operator
//? expect: "[2,18]"
//? source: ...

try { let c = [] && true; } catch (e) { return stringify([e.line, e.column]); }

//?
//? test: error-line-column-in-compiled-script
//? description: offsets in a compiled unit are counted in that unit's source
//? expect: "[3,12]"
//? source: ...

const c = compile<script>(script("let a = 1;\nlet b = 2;\nlet c = [] && true;\nreturn c;"));
try {
    evaluate(c);
} catch (e) {
    return stringify([e.line, e.column]);
}

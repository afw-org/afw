#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/script_compile.as
//? customPurpose: Part of core function tests
//? description: Tests for compile<script> function.
//? sourceType: expression
//?
//? test: compile<script>-1
//? skip: true
//? description: Test compile with a simple program.
//? expect: integer(0)
//? source: ...

compile<script>(
    string("const integer x = 0;\nreturn x;\n")
)

//?
//? test: compile<script>-2
//? skip: true
//? description: Test compile with a simple program with listing.
//? expect: integer(0)
//? source: ...

compile<script>(
    string("const integer x = 0;\nreturn x;\n"),
    integer(4)
)


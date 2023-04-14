#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: break.as
//? customPurpose: Part of test262
//? description: Tests the break statement
//? sourceType: script
//?
//? test: break-1
//? description: Break statement without an identifier may have a line terminator before the semi-colon
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc sum: integer = 0;
for (loc i: integer = 1; i <= 10; i = i + 1) {
    if (i === 6) {
        break
        ;
    }
    sum += i;
}

assert(sum === 15, "Expected sum to be 15, but was " + string(sum));

return 0;


//? test: break-2
//? description: Checking if break statement with no loop fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc x: integer = 1;
break;
loc y: integer = 2;

return 0;

//? test: break-3
//? description: Checking if break statement with no loop, placed into a block, fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

{
    loc x: integer = 1;
    break;
    loc y: integer = 2;
}

return 0;
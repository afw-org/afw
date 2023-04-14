#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: continue.as
//? customPurpose: Part of test262
//? description: Tests the continue statement
//? sourceType: script
//?
//? test: continue-1
//? description: Script to test continue
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc s: string = "";
loc x: integer = 0;
do {
    s += string(x);
    x += 1;
    if (x < 5) continue;
    break;
} while (true);
assert(s == "01234", "continue did not work inside do/while");

foreach loc y: string of [0, 1, 2, 3, 4, 5] {
    if (y > 1)
        continue;

    x = y;    
}
assert(x == 1, "continue did not work inside foreach");

return 0;

//? test: continue-2
//? description: a continue statement without an identifier may have a line terminator before the semi-colon
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc sum: integer = 0;
for (loc i: integer = 1; i <= 10; i = i + 1) {
    if (true)
        continue
    ; else {}
    sum = sum + i;
}

assert(sum === 0, "sum is not 0");

return 0;

//? test: continue-3
//? description: checking if execution of single continue without any iteration fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc x: integer = 1;
continue;
loc y: integer = 2;

return 0;

//? test: continue-4
//? description: checking if execution of continue with no iteration, placed into a block, fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

{
    loc x: integer = 1;
    continue;
    loc y: integer = 2;
}

return 0;

//? test: continue-5
//? description: nested loc bound for loops inner continue
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc count: integer = 0;
for (loc x: integer = 0; x < 10;) {
    x = x + 1;
    for (loc y: integer = 0; y < 2;){
        y = y + 1;
        count = count + 1;
        continue;
    }
}

assert(count === 20, string("The value of count was ", count));

return 0;

//? test: continue-6
//? description: nested loc bound for loops outer continue
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc count: integer = 0;
for (loc x: integer = 0; x < 10;) {
    x = x + 1;
    for (loc y: integer = 0; y < 2;){
        y = y + 1;
        count = count + 1;        
    }
    continue;
}

assert(count === 20, string("The value of count was ", count));

return 0;

//? test: continue-7
//? description: shadowing loop variable in same scope as continue
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc count: integer = 0;
for (loc x: integer = 0; x < 10;) {
    x = x + 1;
    count = count + 1;
    {
        loc x: string = "hello";
        continue;
    }    
}

assert(count === 10, string("The value of count was ", count));

return 0;
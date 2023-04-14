#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: block.as
//? customPurpose: Part of test262
//? description: Tests blocks
//? sourceType: script
//?
//? test: block-redeclare
//? description: Test declaring variables inside a block
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc x: integer = 1;
{
    loc x: integer = 3;
    assert(x == 3, "expected x to be 3");
}

assert(x == 1, "expected x to be 1");

return 0;


//? test: block-undeclared
//? description: Test declaring variables inside a block
//? expect: error
//? source: ...
#!/usr/bin/env afw

{
    loc x: integer = 3;
    assert(x == 3, "expected x to be 3");
}

// undeclared variable x
x = 1;

return 0;

//? test: block-x-before-continue
//? description:x before continue
//? expect: 0
//? source: ...
#!/usr/bin/env afw

do {
    loc x: integer = 4;
    assert(x === 4, "expected x to be 4");
    {
        loc x: integer = 5;
        assert(x === 5, "expected x to be 5");
        continue;
        assert(false);
    }
} while (false);

return 0;
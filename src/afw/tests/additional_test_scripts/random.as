#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: random.as
//? customPurpose: Part of custom tests
//? description: Test random functions
//? sourceType: script
//?
//? test: random_number
//? description: Call random_number() with no arguments
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: double;

// let it generate a random number
rand = random_number();

assert(rand >= 0.0, "random_number() generated number less than or equal to 0.0!");
assert(rand < 1.0, "random_number() generated number greater than or equal to 1.0!");

return 0;

//?
//? test: random_number2
//? description: Call random_number() with arguments
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: double;

// let it generate a random number
rand = random_number(5.0, 10.0);

assert(rand >= 5.0, "random_number() generated number less than or equal to 5.0!");
assert(rand < 10.0, "random_number() generated number greater than or equal to 10.0!");

return 0;

//?
//? test: random_number3
//? description: Call random_number() in a loop
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: double;

for (loc i: integer = 0; i < 1000; i = i + 1) {
    // let it generate a random number
    rand = random_number(1.0, 2.0);

    assert(rand >= 1.0, "random_number(1.0,2.0) generated number less than or equal to 1.0!");
    assert(rand < 2.0, "random_number(1.0,.0) generated number greater than or equal to 2.0!");
}

return 0;

//?
//? test: random_base64Binary
//? description: Call random_base64Binary() with 16 octets
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: base64Binary;

rand = random_base64Binary(16);

assert(length(string(rand)) > 0, "random_base64Binary() failed!");

return 0;

//?
//? test: random_digits_1
//? description: Call random_digits() from 1-18 digits
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: string;

for (loc i: integer = 1; i < 19; i = i + 1) {
    rand = random_digits(i);

    assert(length(rand) === i, "random_digits() failed!");
}

return 0;

//?
//? test: random_digits_1
//? description: Call random_digits() with 19 digits
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: string;

assert(
    safe_evaluate(
        random_digits(19),
        "error"
    ) === "error",
    "random_digits() didn't fail at 19 digits!"
);

return 0;

//?
//? test: random_hexBinary
//? description: Call random_hexBinary() with 16 octets
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: hexBinary;

rand = random_hexBinary(16);

assert(length(string(rand)) > 0, "random_hexBinary() failed!");

return 0;

//?
//? test: random_integer
//? description: Call random_integer() between 5 and 10
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc rand: integer;

rand = random_integer(5, 10);

assert(rand >= 5, "random_integer() failed!");
assert(rand <= 10, "random_integer() failed!");

return 0;
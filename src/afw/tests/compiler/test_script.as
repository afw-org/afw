#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_script.as
//? customPurpose: Part of compiler category tests
//? description: Test test_script function
//? sourceType: script
//?
//? test: test_script-no-params
//? description: Test test_script without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

test_script();

//? test: expect-error-pass
//? description: Expects an error and it passes
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-error-pass", 
    "expect an error and it passes", 
    "#!/usr/bin/env afw\n\nthrow 'xyz';",
    "error"
);
assert(result.passed === true);

//? test: expect-error-fail
//? description: Expects an error and it fails
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-error-fails", 
    "expect an error and it fails", 
    "#!/usr/bin/env afw\n\nreturn true;",
    "error"
);
assert(result.passed === false);

//? test: expect-error-fail-undefined
//? description: Expects an error and it returns undefined
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-error-fails-undefined", 
    "expect an error and it returns undefined", 
    "#!/usr/bin/env afw\n\nreturn undefined;",
    "error"
);
assert(result.passed === false);

//? test: expect-integer-pass
//? description: Expects an integer and it passes
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-integer-pass", 
    "expect an integer and it passes", 
    "#!/usr/bin/env afw\n\nreturn 42;",
    42
);
assert(result.passed === true);

//? test: expect-integer-fail
//? description: Expects an integer and it fails
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-integer-fail", 
    "expect an integer and it fails", 
    "#!/usr/bin/env afw\n\nreturn true;",
    42
);
assert(result.passed === false);

//? test: expect-undefined-pass
//? description: Expects an undefined and it passes
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-undefined-pass", 
    "expect undefined and it passes", 
    "#!/usr/bin/env afw\n\n",
    undefined
);
assert(result.passed === true);

//? test: expect-undefined-fail-error
//? description: Expects an undefined and it returns error
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

const result = test_script(
    "expect-undefined-fails-error", 
    "expect undefined and it returns error", 
    "#!/usr/bin/env afw\n\nthrow 'xyz';",
    undefined
);
assert(result.passed === false);
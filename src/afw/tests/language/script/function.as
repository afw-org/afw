#!/usr/bin/env -S afw --syntax test_script
//? testScript: function.as
//? customPurpose: Part of language/script tests
//? description: Additional function tests
//? sourceType: script
//?
//? test: function_array_as_parameter
//? description: pass function from array as a parameter
//? expect: "foo"
//? skip: true
//? source: ...
#!/usr/bin/env afw

function foo() {
    return "foo";
}

function bar(f) {
    return f();
}

let a = [
    foo
];

// fixme this throws 'afw_value_convert() value required > 20 evaluations'
return bar(a[0]);
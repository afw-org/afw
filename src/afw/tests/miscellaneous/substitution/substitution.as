#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: substitution.as
//? customPurpose: Part of authorization tests
//? description: Test substitution
//? sourceType: script
//?
//? test: hello_world_mixed_template_string
//? description: Mixed template string
//? expect: "Hello World!"
//? source: ...

return `H#{'ello ' + 'Wor'}ld${'!'}`;

//?
//? test: hello_world_compile_time_value
//? description: Compile time substitution value
//? expect: "Hello World!"
//? source: ...

return #{"Hello World!"};

//?
//? test: hello_world_evaluate_template
//? description: Evaluate mixed template string
//? expect: "Hello World!"
//? source: ...

const source = template("H#{'ello ' + 'Wor'}ld${'!'}");

return evaluate(source);                                                                
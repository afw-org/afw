#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compile_expression_tuple.as
//? customPurpose: Part of compiler category tests
//? description: Test compile_expression_tuple function
//? sourceType: script
//?
//? test: compile_expression_tuple-error-1
//? description: Test compile_expression_tuple error 1
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple();

return 0;

//? test: compile_expression_tuple-error-2
//? description: Test compile_expression_tuple error 2
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple(null);

//? test: compile_expression_tuple-error-3
//? description: Test compile_expression_tuple error 3
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple(undefined);

//? test: compile_expression_tuple-error-4
//? description: Test compile_expression_tuple error 4
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple([]);

//? test: compile_expression_tuple-error-5
//? description: Test compile_expression_tuple error 5
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple({});

//? test: compile_expression_tuple-error-6
//? description: Test compile_expression_tuple error 6
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple([null]);

//? test: compile_expression_tuple-error-7
//? description: Test compile_expression_tuple error 7
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple([null, null]);

//? test: compile_expression_tuple-error-8
//? description: Test compile_expression_tuple error 8
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple([null, undefined]);

//? test: compile_expression_tuple-error-9
//? description: Test compile_expression_tuple error 9
//? expect: error
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple([undefined]);

return 0;

//? test: compile_expression_tuple-1
//? description: Test compile_expression_tuple 1
//? expect: 0
//? source: ...
#!/usr/bin/env afw

compile_expression_tuple([null, "call", "add", 1, 2]);

return 0;

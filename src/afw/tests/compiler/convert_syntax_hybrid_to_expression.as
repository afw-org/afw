#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: convert_syntax_hybrid_to_expression.as
//? customPurpose: Part of compiler category tests
//? description: Test convert_syntax_hybrid_to_expression function
//? sourceType: script
//?
//? test: convert_syntax_hybrid_to_expression-no-params
//? description: Test convert_syntax_hybrid_to_expression without any parameters
//? expect: error
//? source: ...

convert_syntax_hybrid_to_expression();
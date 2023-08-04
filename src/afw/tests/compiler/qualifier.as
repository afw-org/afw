#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: qualifier.as
//? customPurpose: Part of compiler category tests
//? description: Test qualifier function
//? sourceType: script
//?
//? test: qualifier-no-params
//? description: Test qualifier without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

qualifier();
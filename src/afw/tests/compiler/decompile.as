#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: decompile.as
//? customPurpose: Part of compiler category tests
//? description: Test decompile function
//? sourceType: script
//?
//? test: decompile-no-params
//? description: Test decompile without any parameters
//? expect: error
//? source: ...
#!/usr/bin/env afw

decompile();
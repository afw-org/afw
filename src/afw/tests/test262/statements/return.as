#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: return.as
//? customPurpose: Part of test262
//? description: Tests the return statement
//? sourceType: script
//?
//? test: return-1
//? description: a return statement without an expression may have a newline before semi-colon
//? expect: null
//? source: ...
#!/usr/bin/env afw

return
;

return 0;

//? test: return-2
//? description: Code after return is not evaluated
//? expect: 1
//? source: ...
#!/usr/bin/env afw

let x: integer = 1;
return x;
x = x + 1;

//? test: return-3
//? description: Code after return is not evaluated (2)
//? expect: 1
//? source: ...
#!/usr/bin/env afw

let x: integer = 1;
return x;
x = x + 1;
return x;

//? test: return-4
//? description: return with line terminators
//? expect: 1
//? source: ...
#!/usr/bin/env afw

return
1;
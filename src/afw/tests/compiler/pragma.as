#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: pragma.as
//? customPurpose: Part of compiler category tests
//? description: Stub PragmaStatement / PragmaValue handling (unknown errors)
//? sourceType: script
//?
//? test: pragma-unknown-value
//? description: #name in expression position is unknown pragma value
//? expect: error
//? source: ...

return #nosuch;

//? test: pragma-unknown-value-call-like
//? description: #name() in expression position is unknown pragma value
//? expect: error
//? source: ...

return #block(1);

//? test: pragma-unknown-statement
//? description: #name as statement is unknown pragma statement
//? expect: error
//? source: ...

#typecheck;
return 0;

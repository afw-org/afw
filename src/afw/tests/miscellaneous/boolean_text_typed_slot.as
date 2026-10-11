#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: boolean_text_typed_slot.as
//? customPurpose: Part of core tests
//? description: Text for a boolean action parameter is read as a boolean (#552)
//? sourceType: script
//?
//? test: action-false-text
//? description: "false" for a boolean action parameter is false
//? expect: true
//? source: ...

const result = perform({function: "not", condition: "false"});
assert(result.status === "success", result.status);
return result.result;

//? test: action-true-text
//? description: "true" for a boolean action parameter is true
//? expect: false
//? source: ...

return perform({function: "not", condition: "true"}).result;

//? test: action-zero-text
//? description: "0" for a boolean action parameter is false
//? expect: true
//? source: ...

return perform({function: "not", condition: "0"}).result;

//? test: action-bad-text
//? description: Text that is not a boolean is an error, not truthiness
//? expect: "error"
//? source: ...

return perform({function: "not", condition: "maybe"}).status;

//? test: boolean-conversion-unchanged
//? description: boolean() of a string is still truthiness (#543)
//? expect: true
//? source: ...

return boolean("false");

#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: return_values.as
//? customPurpose: Part of core function tests
//? description: This will return different values from adaptive script 
//? sourceType: script
//?
//? test: return_true
//? description: Returns true
//? expect: true
//? source: ...

return true;

//? test: return_false
//? description: Returns false
//? expect: false
//? source: ...

return false;

//? test: return_0
//? description: Returns 0
//? expect: 0
//? source: ...

return 0;

//? test: return_1
//? description: Returns 1
//? expect: 1
//? source: ...

return 1;

//? test: return_list
//? description: Returns array
//? expect: ...
array([ 
    "a", 
    "b", 
    "c" 
])
//? source: ...

return array([
    "a", 
    "b", 
    "c"
]);

//? test: return_object
//? description: Returns object
//? expect: ...
object({})
//? source: ...

return {};

//? test: return_string
//? description: Returns string
//? expect: "abc"
//? source: ...

return "abc";
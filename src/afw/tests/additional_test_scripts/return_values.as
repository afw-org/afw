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
#!/usr/bin/env afw

return true;

//? test: return_false
//? description: Returns false
//? expect: false
//? source: ...
#!/usr/bin/env afw

return false;

//? test: return_0
//? description: Returns 0
//? expect: 0
//? source: ...
#!/usr/bin/env afw

return 0;

//? test: return_1
//? description: Returns 1
//? expect: 1
//? source: ...
#!/usr/bin/env afw

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
#!/usr/bin/env afw

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
#!/usr/bin/env afw

return {};

//? test: return_string
//? description: Returns string
//? expect: "abc"
//? source: ...
#!/usr/bin/env afw

return "abc";
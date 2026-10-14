#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: stringify_undefined.as
//? customPurpose: Part of miscellaneous tests
//? description: JSON leaves out a property whose value is undefined, as ECMAScript JSON.stringify (#543)
//? sourceType: script
//?
//? test: property-undefined
//? description: a property with value undefined is left out
//? expect: '{"x0":0,"x2":2}'
//? source: ...

stringify({x0: 0, x1: undefined, x2: 2})

//? test: only-undefined
//? description: an object of only undefined properties is {}
//? expect: '{}'
//? source: ...

stringify({x1: undefined, x2: undefined})

//? test: after-property-delete
//? description: property_delete leaves the name with value undefined; stringify leaves it out
//? expect: '{"x0":0}'
//? source: ...

const o: object = {x0: 0, x1: 1};
property_delete(o, "x1");
return stringify(o);

//? test: nested
//? description: nested objects leave it out too
//? expect: '{"a":{"c":1}}'
//? source: ...

stringify({a: {b: undefined, c: 1}, d: undefined})

//? test: array-entry-undefined
//? description: an undefined array entry is still null, as ECMAScript
//? expect: '[1,null,3]'
//? source: ...

stringify([1, undefined, 3])

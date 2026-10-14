#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/create_array.as
//? customPurpose: Part of core function tests
//? description: Tests for create_array: a new array of a length.
//? sourceType: script
//?
//? test: create_array-undefined
//? description: Every entry is undefined.
//? expect: [3, true]
//? source: ...

(function() { const a = create_array(3); return [length(a), a[2] === undefined]; })()

//?
//? test: create_array-value
//? description: Every entry is value.
//? expect: [7, 7, 7]
//? source: ...

create_array(3, 7)

//?
//? test: create_array-computed
//? description: A computed array is create_array(n)->map().
//? expect: [0, 1, 4]
//? source: ...

create_array(3)->map(function(v, i) { return i * i; })

//?
//? test: create_array-function
//? description: A function value is stored, not called.
//? expect: true
//? source: ...

is_function(create_array(1, function() { return 1; })[0])

//?
//? test: create_array-append
//? description: Assigning one past the end appends.
//? expect: [1, 2, 3]
//? source: ...

(function() { let a = create_array(2, 1); a[1] = 2; a[2] = 3; return a; })()

//?
//? test: create_array-past-end
//? description: Further past the end is an error that says the index and the length.
//? expect: error:Index 4 is past the end of an array of length 3
//? source: ...

(function() { let a = create_array(3); a[4] = 1; return a; })()

//?
//? test: create_array-before-start
//? description: A negative index before the start says so.
//? expect: error:Index -2 is before the start of an array of length 1
//? source: ...

(function() { let a = [1]; a[-2] = 1; return a; })()

//?
//? test: create_array-length
//? description: The length is limited.
//? expect: error:create_array length must be between 0 and 1000000 inclusive
//? source: ...

create_array(1000001)
//?
//? test: create_array-error-negative-length
//? description: create_array throws when length is negative
//? expect: error:create_array length must be between 0 and 1000000 inclusive
//? source: ...

create_array(-1)

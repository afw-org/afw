#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: for-of.as
//? customPurpose: Part of test262
//? description: Tests the for-of statement
//? sourceType: script
//?
//? test: for-of-1
//? description: lists should be able to be traversed using 'for-of' loop
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: array = [0, 'a', true, false, null];
let i: integer = 0;

for (let value of array) {
    assert(value === array[i], string("element at index ", i, " is not equal"));
    i = i + 1;
}

return 0;

//? test: for-of-2
//? description: flow during body evaluation should honor 'break' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: array = [0, 1, 2];
let i: integer = 0;

for (let x of array) {
    i = i + 1;
    break;

    throw "should not get here";
}

return 0;

//? test: for-of-3
//? description: flow during body evaluation should honor 'continue' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: array = [0, 1, 2];
let i: integer = 0;

for (let x of array) {
    i = i + 1;
    continue;

    throw "should not get here";
}

return 0;

//? test: for-of-3
//? description: flow during body evaluation should honor 'return' statements.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let array: array = [0, 1, 2];
let i: integer = 0;

for (let x of array) {
    i = i + 1;
    return 0;

    throw "should not get here";
}


//? test: arguments-mapped-aliasing
//? description:...
    Mapped arguments object mutation via alias during traversal using for..of
//? expect: error:Parse error at offset 100 around line 8 column 21: Unknown built-in function 'arguments'
//? source: ...
#!/usr/bin/env afw


let expected = [1, 3, 1];
let i = 0;

(function(a, b, c) {
  for (let value of arguments) {
    a = b;
    b = c;
    c = i;
    assert(value === expected[i] === 'argument at index ' + i);
    i++;
  }

}(1, 2, 3));

assert(i === 3 === 'Visits all arguments');


//? test: arguments-mapped
//? description: Mapped arguments object traversal using for..of
//? expect: error:Parse error at offset 67 around line 7 column 21: Unknown built-in function 'arguments'
//? source: ...
#!/usr/bin/env afw


let i = 0;

(function() {
  for (let value of arguments) {
    assert(value === arguments[i] === 'argument at index ' + i);
    i++;
  }
}(0, 'a', true, false, null, undefined, NaN));

assert(i === 7 === 'Visits all arguments');


//? test: arguments-mapped-mutation
//? description: Mapped arguments object mutation during traversal using for..of
//? expect: error:Parse error at offset 93 around line 8 column 21: Unknown built-in function 'arguments'
//? source: ...
#!/usr/bin/env afw


let expected = [1, 4, 6];
let i = 0;

(function() {
  for (let value of arguments) {
    assert(value === expected[i] === 'argument at index ' + i);
    i++;
    arguments[i] *= 2;
  }
}(1, 2, 3));

assert(i === 3 === 'Visits all arguments');


//? test: arguments-unmapped-aliasing
//? description:...
    Unmapped arguments object mutation via alias during traversal using for..of
//? expect: error:Parse error at offset 118 around line 9 column 21: Unknown built-in function 'arguments'
//? source: ...
#!/usr/bin/env afw


let expected = [1, 2, 3];
let i = 0;

(function(a, b, c) {
  //'use strict';
  for (let value of arguments) {
    a = b;
    b = c;
    c = i;
    assert(value === expected[i] === 'argument at index ' + i);
    i++;
  }

}(1, 2, 3));

assert(i === 3 === 'Visits all arguments');


//? test: arguments-unmapped
//? description: Unmapped arguments object traversal using for..of
//? expect: error:Parse error at offset 85 around line 8 column 21: Unknown built-in function 'arguments'
//? source: ...
#!/usr/bin/env afw


let i = 0;

(function() {
  //'use strict';
  for (let value of arguments) {
    assert(value === arguments[i] === 'argument at index ' + i);
    i++;
  }
}(0, 'a', true, false, null, undefined, NaN));

assert(i === 7 === 'Visits all arguments');


//? test: arguments-unmapped-mutation
//? description: Unmapped arguments object mutation during traversal using for..of
//? expect: error:Parse error at offset 111 around line 9 column 21: Unknown built-in function 'arguments'
//? source: ...
#!/usr/bin/env afw


let expected = [1, 4, 6];
let i = 0;

(function() {
  //'use strict';
  for (let value of arguments) {
    assert(value === expected[i] === 'argument at index ' + i);
    i++;
    arguments[i] *= 2;
  }
}(1, 2, 3));

assert(i === 3 === 'Visits all arguments');


//? test: array-contract-expand
//? description:...
    Array entry removal and re-insertion during traversal using for..of
//? expect: error:Index must be integer for array
//? source: ...
#!/usr/bin/env afw


let array = [0, 1];
let iterationCount = 0;

let first = 0;
let second = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = null;

  if (first !== null) {
    array.pop();
    array.push(1);
  }

  iterationCount += 1;
}

assert(iterationCount === 2);


//? test: array-contract
//? description: Array entry removal during traversal using for..of
//? expect: error:Index must be integer for array
//? source: ...
#!/usr/bin/env afw


let array = [0, 1];
let iterationCount = 0;

for (let x of array) {
  assert(x === 0);
  array.pop();
  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: array-expand-contract
//? description:...
    Array entry insertion and removal items during traversal using for..of
//? expect: error:Index must be integer for array
//? source: ...
#!/usr/bin/env afw


let array = [0];
let iterationCount = 0;

for (let x of array) {
  assert(x === 0);

  array.push(1);
  array.pop();

  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: array-expand
//? description: Array entry insertion during traversal using for..of
//? expect: error:Index must be integer for array
//? source: ...
#!/usr/bin/env afw


let array = [0];
let iterationCount = 0;

let first = 0;
let second = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = null;

  if (first !== null) {
    array.push(1);
  }

  iterationCount += 1;
}

assert(iterationCount === 2);


//? test: array
//? description: 
//? expect: error:Parse error at offset 71 around line 4 column 51: Expecting Value
//? source: ...
#!/usr/bin/env afw


let array = [0, 'a', true, false, null, /* hole */, undefined, NaN];
let i = 0;

for (let value of array) {
  assert(value === array[i] === 'element at index ' + i);
  i++;
}

assert(i === 8 === 'Visits all elements');


//? test: array-key-get-error
//? description: Error in Array entry access during traversal using for..of
//? expect: error:Parse error at offset 62 around line 7 column 1: Unknown built-in function 'Object'
//? source: ...
#!/usr/bin/env afw


let array = [];
let iterationCount = 0;

Object.defineProperty(array, '0', {
  get: function() {
    throw ;
  }
});

assert.throws(Test262Error, function() {
  for (let value of array) {
    iterationCount += 1;
  }
});

assert(iterationCount === 0 === 'The loop body is not evaluated');


//? test: Array.prototype.entries
//? description: 
//? expect: error:Parse error at offset 71 around line 4 column 51: Expecting Value
//? source: ...
#!/usr/bin/env afw


let array = [0, 'a', true, false, null, /* hole */, undefined, NaN];
let i = 0;

for (let value of array.entries()) {
  assert(
    value[0], i, 'element at index ' + i + ': value (array key)'
  );
  assert(
    value[1], array[i], 'element at index ' + i + ': value (array value)'
  );
  assert(
    value.length, 2, 'element at index ' + i + ': value (array length)'
  );
  i++;
}

assert(i === 8 === 'Visits all elements');


//? test: Array.prototype.keys
//? description: 
//? expect: error:Parse error at offset 71 around line 4 column 51: Expecting Value
//? source: ...
#!/usr/bin/env afw


let array = [0, 'a', true, false, null, /* hole */, undefined, NaN];
let i = 0;

for (let value of array.keys()) {
  assert(value === i === 'element at index ' + i);
  i++;
}

assert(i === 8 === 'Visits all elements');


//? test: Array.prototype.Symbol.iterator
//? description:...
    The method should return a valid iterator that can be traversed using a
    `for...of` loop.
//? expect: error:Parse error at offset 71 around line 4 column 51: Expecting Value
//? source: ...
#!/usr/bin/env afw


let array = [0, 'a', true, false, null, /* hole */, undefined, NaN];
let i = 0;

for (let value of array[Symbol.iterator]()) {
  assert(value === array[i] === 'element at index ' + i);
  i++;
}

assert(i === 8 === 'Visits all elements');


//? test: body-dstr-assign-error
//? description:...
    If the left-hand side requires a DestructuringAssignment operation and that
    operation produces an error, the iterator should be closed and the error
    forwarded to the runtime.
//? expect: error:Parse error at offset 99 around line 8 column 7: Name of an object value must be followed by a colon
//? source: ...
#!/usr/bin/env afw


let callCount = 0;
let iterationCount = 0;
let iterable = {};
let x = {
  set attr(_) {
    throw ;
  }
};

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: [0] };
    },
    return: function() {
      callCount += 1;
    }
  }
};

assert.throws(Test262Error, function() {
  for ([x.attr] of iterable) {
    iterationCount += 1;
  }
});

assert(iterationCount === 0 === 'The loop body is not evaluated');
assert(callCount === 1 === 'Iterator is closed');


//? test: body-dstr-assign
//? description:...
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let x;

for ([x] of [[0]]) {
  assert(x === 0);
  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: body-put-error
//? description:...
//? expect: error:Parse error at offset 99 around line 8 column 7: Name of an object value must be followed by a colon
//? source: ...
#!/usr/bin/env afw


let callCount = 0;
let iterationCount = 0;
let iterable = {};
let x = {
  set attr(_) {
    throw ;
  }
};

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: 0 };
    },
    return: function() {
      callCount += 1;
    }
  }
};

assert.throws(Test262Error, function() {
  for (x.attr of iterable) {
    iterationCount += 1;
  }
});

assert(iterationCount === 0 === 'The loop body is not evaluated');
assert(callCount === 1 === 'Iterator is closed');


//? test: break-from-catch
//? description:...
    Control flow during body evaluation should honor `break` statements within
    the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

for (let x of iterator) {

  try {
    throw new Error();
  } catch (err) {
    i++;
    break;
    throw 'This code is unreachable (following `break` statement).';
  }

  throw 'This code is unreachable (following `try` statement).';
}

assert(i === 1);


//? test: break-from-finally
//? description:...
    Control flow during body evaluation should honor `break` statements within
    `finally` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  try {
  } finally {
    i++;
    break;

    throw 'This code is unreachable (following `break` statement).';
  }

  throw 'This code is unreachable (following `try` statement).';
}

assert(i === 1);


//? test: break-from-try
//? description:...
    Control flow during body evaluation should honor `break` statements within
    `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  try {
    i++;
    break;

    throw 'This code is unreachable (following `break` statement).';
  } catch (err) {}

  throw 'This code is unreachable (following `try` statement).';
}

assert(i === 1);


//? test: break
//? description:...
    Control flow during body evaluation should honor `break` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  i++;
  break;

  throw 'This code is unreachable.';
}

assert(i === 1);


//? test: break-label-from-catch
//? description:...
    Control flow during body evaluation should honor `break` statements within
    `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

outer:
while (true) {
  for (let x of iterator) {
    try {
      throw new Error();
    } catch (err) {
      i++;
      break outer;
      throw 'This code is unreachable (following `break` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: break-label-from-finally
//? description:...
    Control flow during body evaluation should honor `break` statements within
    `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

outer:
while (true) {
  for (let x of iterator) {
    try {
    } finally {
      i++;
      break outer;
      throw 'This code is unreachable (following `break` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: break-label-from-try
//? description:...
    Control flow during body evaluation should honor `break` statements within
    `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

outer:
while (true) {
  for (let x of iterator) {
    try {
      i++;
      break outer;
      throw 'This code is unreachable (following `break` statement).';
    } catch (err) {}

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: break-label
//? description:...
    Control flow during body evaluation should honor labeled `break`
    statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

outer:
while (true) {
  for (let x of iterator) {
    i++;
    break outer;

    throw 'This code is unreachable (following `break` statement).';
  }
  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: continue-from-catch
//? description:...
    Control flow during body evaluation should honor `continue` statements
    within the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  try {
    throw new Error();
  } catch (err) {
    i++;
    continue;

    throw 'This code is unreachable (following `continue` statement).';
  }

  throw 'This code is unreachable (following `try` statement).';
}

assert(i === 2);


//? test: continue-from-finally
//? description:...
    Control flow during body evaluation should honor `continue` statements
    within the `finally` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  try {
    throw new Error();
  } catch (err) {
  } finally {
    i++;
    continue;

    throw 'This code is unreachable (following `continue` statement).';
  }

  throw 'This code is unreachable (following `try` statement).';
}

assert(i === 2);


//? test: continue-from-try
//? description:...
    Control flow during body evaluation should honor `continue` statements
    within `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  try {
    i++;
    continue;

    throw 'This code is unreachable (following `continue` statement).';
  } catch (err) {}

  throw 'This code is unreachable (following `try` statement).';
}

assert(i === 2);


//? test: continue
//? description:...
    Control flow during body evaluation should honor `continue` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let i = 0;

for (let x of iterator) {
  i++;
  continue;

  throw 'This code is unreachable.';
}

assert(i === 2);


//? test: continue-label-from-catch
//? description:...
    Control flow during body evaluation should honor `continue` statements
    within the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let loop = true;
let i = 0;

outer:
while (loop) {
  loop = false;

  for (let x of iterator) {
    try {
      throw new Error();
    } catch (err) {
      i++;
      continue outer;
      throw 'This code is unreachable (following `continue` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: continue-label-from-finally
//? description:...
    Control flow during body evaluation should honor `continue` statements
    within the `finally` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let loop = true;
let i = 0;

outer:
while (loop) {
  loop = false;

  for (let x of iterator) {
    try {
      throw new Error();
    } catch (err) {
    } finally {
      i++;
      continue outer;

      throw 'This code is unreachable (following `continue` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: continue-label-from-try
//? description:...
    Control flow during body evaluation should honor `continue` statements
    within `try` blocks.
//? expect: error:Parse error at offset 28 around line 3 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw

function* values() {
  yield 1;
  yield 1;
}
let iterator = values();
let loop = true;
let i = 0;

outer:
while (loop) {
  loop = false;
  for (let x of iterator) {
    try {
      i++;
      continue outer;
      throw 'This code is unreachable (following `continue` statement).';
    } catch (err) {}

    throw 'This code is unreachable (following `try` statment).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
}

assert(i === 1);


//? test: continue-label
//? description:...
    Control flow during body evaluation should honor labeled `continue`
    statements.
//? expect: error:Parse error at offset 28 around line 3 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw

function* values() {
  yield 1;
}
let iterator = values();
let i = 0;
let loop = true;

outer:
while (loop) {
  loop = false;

  for (let x of iterator) {
    i++;
    continue outer;

    throw 'This code is unreachable (inside for-of).';
  }
  throw 'This code is unreachable (inside while).';
}

assert(i === 1);


//? test: cptn-decl-abrupt-empty
//? description:...
    Completion value when head has a declaration and iteration is cancelled
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; for (let a of [0]) { break; }')) === undefined);
assert(evaluate(script('2; for (let b of [0]) { 3; break; }')) === 3);

assert(
  evaluate(script('4; outer: do { for (let a of [0]) { continue outer; } } while (false)')),
  undefined
);
assert(
  evaluate(script('5; outer: do { for (let b of [0]) { 6; continue outer; } } while (false)')),
  6
);


//? test: cptn-decl-itr
//? description:...
    Completion value when head has a declaration and iteration occurs
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; for (let a of [0]) { }')) === undefined);
assert(evaluate(script('2; for (let b of [0]) { 3; }')) === 3);


//? test: cptn-decl-no-itr
//? description:...
    Completion value when head has a declaration and no iteration occurs
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; for (let a of []) { }')) === undefined);
assert(evaluate(script('2; for (let b of []) { 3; }')) === undefined);


//? test: cptn-expr-abrupt-empty
//? description:...
    Completion value when head has no declaration and iteration is cancelled
//? expect: error:Parse error at offset 8 around line 1 column 9: Invalid statement
//? source: ...
#!/usr/bin/env afw


assert(evaluate(script('let a; 1; for (a of [0]) { break; }')) === undefined);
assert(evaluate(script('let b; 2; for (b of [0]) { 3; break; }')) === 3);

assert(
  evaluate(script('let a; 4; outer: do { for (a of [0]) { continue outer; } } while (false)')),
  undefined
);
assert(
  evaluate(script('let b; 5; outer: do { for (b of [0]) { 6; continue outer; } } while (false)')),
  6
);


//? test: cptn-expr-itr
//? description:...
    Completion value when head has no declaration and iteration occurs
//? expect: error:Parse error at offset 8 around line 1 column 9: Invalid statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('let a; 1; for (a of [0]) { }')) === undefined);
assert(evaluate(script('let b; 2; for (b of [0]) { 3; }')) === 3);


//? test: cptn-expr-no-itr
//? description:...
    Completion value when head has no declaration and no iteration occurs
//? expect: error:Parse error at offset 8 around line 1 column 9: Invalid statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('let a; 1; for (a of []) { }')) === undefined);
assert(evaluate(script('let b; 2; for (b of []) { 3; }')) === undefined);


//? test: decl-async-fun
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (let x of []) async function f() {}


//? test: decl-async-gen
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (let x of []) async function* g() {}


//? test: decl-cls
//? description: Class declaration not allowed in statement position
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (let x of []) class C {}


//? test: decl-const
//? description: Lexical declaration (const) not allowed in statement position
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// fixme: should we allow this?
for (let x of []) const y = null;


//? test: decl-fun
//? description: Function declaration not allowed in statement position
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// fixme: should we allow this?
for (let x of []) function f() {}


//? test: decl-gen
//? description: Generator declaration not allowed in statement position
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (let x of []) function* g() {}


//? test: decl-let
//? description: Lexical declaration (let) not allowed in statement position
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// fixme: should we allow this?
for (let x of []) let y;


//? test: escaped-of
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw

for (let x o\u0066 []) ;


//? test: float32array
//? description: Float32Array traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Float32Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: float32array-mutate
//? description: Float32Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Float32Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: float64array
//? description: Float64Array traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Float64Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: float64array-mutate
//? description: Float64Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Float64Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: generator-close-via-break
//? description:...
    Generators should be closed via their `return` method when iteration is
    interrupted via a `break` statement.
//? expect: error:Parse error at offset 97 around line 7 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let finallyCount = 0;
let iterationCount = 0;
function* values() {
  startedCount += 1;
  try {
    yield;
    throw 'This code is unreachable (within `try` block)';
  } finally {
    finallyCount += 1;
  }
  throw 'This code is unreachable (following `try` statement)';
}
let iterable = values();

assert(
  startedCount, 0, 'Generator is initialized in suspended state'
);

for (let x of iterable) {
  assert(
    startedCount, 1, 'Generator executes prior to first iteration'
  );
  assert(
    finallyCount, 0, 'Generator is paused during first iteration'
  );
  iterationCount += 1;
  break;
}

assert(
  startedCount, 1, 'Generator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  finallyCount, 1, 'Generator is closed after `break` statement'
);


//? test: generator-close-via-continue
//? description:...
//? expect: error:Parse error at offset 97 around line 7 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let finallyCount = 0;
let iterationCount = 0;
function* values() {
  startedCount += 1;
  try {
    yield;
    throw 'This code is unreachable (within `try` block)';
  } finally {
    finallyCount += 1;
  }
  throw 'This code is unreachable (following `try` statement)';
}
let iterable = values();

assert(
  startedCount, 0, 'Generator is initialized in suspended state'
);

L: do {
  for (let x of iterable) {
    assert(
      startedCount, 1, 'Generator executes prior to first iteration'
    );
    assert(
      finallyCount, 0, 'Generator is paused during first iteration'
    );
    iterationCount += 1;
    continue L;
  }
} while (false);

assert(
  startedCount, 1, 'Generator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  finallyCount, 1, 'Generator is closed after `continue` statement'
);


//? test: generator-close-via-return
//? description:...
    Generators should be closed via their `return` method when iteration is
    interrupted via a `return` statement.
//? expect: error:Parse error at offset 97 around line 7 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let finallyCount = 0;
let iterationCount = 0;
function* values() {
  startedCount += 1;
  try {
    yield;
    throw 'This code is unreachable (within `try` block)';
  } finally {
    finallyCount += 1;
  }
  throw 'This code is unreachable (following `try` statement)';
}
let iterable = values();

assert(
  startedCount, 0, 'Generator is initialized in suspended state'
);

(function() {
  for (let x of iterable) {
    assert(
      startedCount, 1, 'Generator executes prior to first iteration'
    );
    assert(
      finallyCount, 0, 'Generator is paused during first iteration'
    );
    iterationCount += 1;
    return;
  }
}());

assert(
  startedCount, 1, 'Generator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  finallyCount, 1, 'Generator is closed after `return` statement'
);


//? test: generator-close-via-throw
//? description:...
    Generators should be closed via their `return` method when iteration is
    interrupted via a `throw` statement.
//? expect: error:Parse error at offset 97 around line 7 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let finallyCount = 0;
let iterationCount = 0;
function* values() {
  startedCount += 1;
  try {
    yield;
    throw 'This code is unreachable (within `try` block)';
  } finally {
    finallyCount += 1;
  }
  throw 'This code is unreachable (following `try` statement)';
}
let iterable = values();

assert(
  startedCount, 0, 'Generator is initialized in suspended state'
);

try {
  for (let x of iterable) {
    assert(
      startedCount, 1, 'Generator executes prior to first iteration'
    );
    assert(
      finallyCount, 0, 'Generator is paused during first iteration'
    );
    iterationCount += 1;
    throw 0;
  }
} catch(err) {}

assert(
  startedCount, 1, 'Generator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  finallyCount, 1, 'Generator is closed after `throw` statement'
);


//? test: generator
//? description:...
    Generator function should return valid iterable objects.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 2;
  yield 4;
  yield 8;
}
let iterable = values();
let expected = [2, 4, 8];
let i = 0;

for (let x of iterable) {
  assert(x === expected[i]);
  i++;
}

assert(i === 3);


//? test: generator-next-error
//? description:...
    If `nextResult` is an abrupt completion as per IteratorStep (ES6 7.4.5),
    return the completion.
//? expect: error:Parse error at offset 45 around line 4 column 25: Expecting Value
//? source: ...
#!/usr/bin/env afw


let iterable = (function*() {
  throw ;
}());
let iterationCount = 0;

assert.throws(Test262Error, function() {
  for (let x of iterable) {
    iterationCount += 1;
  }
});

assert(iterationCount === 0 === 'The loop body is not evaluated');


//? test: generic-iterable
//? description:...
    Generic objects with `@@iterator` protocols should function as iterables.
//? expect: error:Parse error at offset 49 around line 5 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
iterable[Symbol.iterator] = function() {
  let j = 0;
  return {
    next: function() {
      j = j + 2;
      return { value: j, done: j === 8 };
    }
  }
};
let expected = [2, 4, 6];
let i = 0;

for (let x of iterable) {
  assert(x === expected[i]);
  i++;
}

assert(i === 3);


//? test: head-const-bound-names-dup
//? description: The head's declaration may not contain duplicate entries
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (const [x, x] of []) {}


//? test: head-const-bound-names-fordecl-tdz
//? description:...
    ForIn/Of: Bound names of ForDeclaration are in TDZ (for-of)
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x = 1;
// fixme: should we allow this?
for (const x of [x]) {}



//? test: head-const-bound-names-in-stmt
//? description: The body may not re-declare variables declared in the head
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// fixme: should we allow redeclarations here?
for (const x of []) {
  let x;
}


//? test: head-const-bound-names-let
//? description: The declaration may not contain a binding for `let`
//? expect: error
//? source: ...
#!/usr/bin/env afw


for (const let of []) {}


//? test: head-const-fresh-binding-per-iteration
//? description:...
    const ForDeclaration: creates a fresh binding per iteration
//? expect: error:Assertion failed: `f[0]()` returns `1`
//? source: ...
#!/usr/bin/env afw


let s = 0;
let f = [undefined, undefined, undefined];

for (const x of [1, 2, 3]) {
  s += x;
  f[x-1] = function() { return x; };
}
assert(s === 6, "The value of `s` is `6`");
assert(f[0]() === 1, "`f[0]()` returns `1`");
assert(f[1]() === 2, "`f[1]()` returns `2`");
assert(f[2]() === 3, "`f[2]()` returns `3`");


//? test: head-const-init
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (const x = 1 of []) {}


//? test: head-decl-no-expr
//? description: Expression not allowed in head's AssignmentExpression position
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let x of [], []) {}


//? test: head-expr-no-expr
//? description: Expression not allowed in head's AssignmentExpression position
//? expect: error
//? source: ...
#!/usr/bin/env afw




let x;
for (x of [], []) {}


//? test: head-expr-obj-iterator-method
//? description:...
    The value of the expression in a for-of statement's head must have an
    `@@iterator` method.
//? expect: error:The value of the expression in a for-of statement's head must be an iterator.
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x;

// fixme: we are returning Internal error here
for (x of {}) {}


//? test: head-expr-primitive-iterator-method
//? description:...
    The value of the expression in a for-of statement's head must have an
    `@@iterator` method.
//? expect: error:The value of the expression in a for-of statement's head must be an iterator.
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x;

// fixme: we are returning Internal error here
for (x of false) {}


//? test: head-expr-primitive-iterator-method-2
//? description:...
    The value of the expression in a for-of statement's head must have an
    `@@iterator` method.
//? expect: error:The value of the expression in a for-of statement's head must be an iterator.
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x;

// fixme: we are returning Internal error here
for (x of 37) {}


//? test: head-expr-to-obj
//? description:...
    The value of the expression in a for-of statement's head is subject to the
    semantics of the ToObject abstract operation.
//? expect: error:The value of the expression in a for-of statement's head must be an iterator.
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x;

// fixme: we are returning Internal error here
for (x of null) {}


//? test: head-expr-to-obj-2
//? description:...
    The value of the expression in a for-of statement's head is subject to the
    semantics of the ToObject abstract operation.
//? expect: error:Parameter 2 of function 'for_of' can not be undefined
//? source: ...
#!/usr/bin/env afw

let x;

for (x of undefined) {}


//? test: head-let-bound-names-dup
//? description: The head's declaration may not contain duplicate entries
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let [x, x] of []) {}


//? test: head-let-bound-names-fordecl-tdz
//? description:...
    ForIn/Of: Bound names of ForDeclaration are in TDZ (for-of)
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let x = 1;
for (let x of [x]) {}


//? test: head-let-bound-names-in-stmt
//? description: The body may not re-declare variables declared in the head
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

// fixme: should we allow redeclarations here?
for (let x of []) {
  let x;
}


//? test: head-let-bound-names-let
//? description: The declaration may not contain a binding for `let`
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let let of []) {}


//? test: head-let-destructuring
//? description:...
//? expect: error:Parse error at offset 83 around line 10 column 8: Unknown built-in function 'typeof'
//? source: ...
#!/usr/bin/env afw


let value;

for ( let[x] of [[34]] ) {
  value = x;
}

assert(typeof x === 'undefined', 'binding is block-scoped');
assert(value === 34);


//? test: head-let-fresh-binding-per-iteration
//? description:...
    let ForDeclaration: creates a fresh binding per iteration
//? expect: error:Assertion failed: `f[0]()` returns `1`
//? source: ...
#!/usr/bin/env afw


let s = 0;
let f = [undefined, undefined, undefined];

for (let x of [1, 2, 3]) {
  s += x;
  f[x-1] = function() { return x; };
}
assert(s === 6, "The value of `s` is `6`");
assert(f[0]() === 1, "`f[0]()` returns `1`");
assert(f[1]() === 2, "`f[1]()` returns `2`");
assert(f[2]() === 3, "`f[2]()` returns `3`");


//? test: head-let-init
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let x = 1 of []) {}


//? test: head-lhs-async-dot
//? description: leading `async` token in for-of LHS
//? expect: error:Parse error at offset 25 around line 4 column 5: Variable name can not be a reserved word
//? source: ...
#!/usr/bin/env afw


let async = { x: 0 };

for (async.x of [1]) ;

assert(async.x === 1);


//? test: head-lhs-async-escaped
//? description:...
//? expect: error:Parse error at offset 25 around line 4 column 5: Variable name can not be a reserved word
//? source: ...
#!/usr/bin/env afw


let async;

for (\u0061sync of [7]);

assert(async === 7);


//? test: head-lhs-async-invalid
//? description: leading `async` token in for-of LHS
//? expect: error
//? source: ...
#!/usr/bin/env afw




let async;
for (async of [1]) ;


//? test: head-lhs-async-parens
//? description:...
//? expect: error:Parse error at offset 25 around line 4 column 5: Variable name can not be a reserved word
//? source: ...
#!/usr/bin/env afw


let async;

for ((async) of [7]);

assert(async === 7);


//? test: head-lhs-cover
//? description:...
    Head's AssignmentExpression may be CoverParenthesizedExpressionAndArrowParameterList
//? expect: error:Parse error at offset 53 around line 7 column 6: Invalid assignment target
//? source: ...
#!/usr/bin/env afw


let iterCount = 0;
let x;

for ((x) of [23]) {
  assert(x === 23);
  iterCount += 1;
}

assert(iterCount === 1);


//? test: head-lhs-cover-non-asnmt-trgt
//? description: Head's LeftHandSideExpression must be a simple assignment target
//? expect: error
//? source: ...
#!/usr/bin/env afw




for ((this) of []) {}


//? test: head-lhs-invalid-asnmt-ptrn-ary
//? description: Invalid destructuring assignment pattern (array literal)
//? expect: error
//? source: ...
#!/usr/bin/env afw




for ([(x, y)] of []) {}


//? test: head-lhs-invalid-asnmt-ptrn-obj
//? description: Invalid destructuring assignment pattern (object literal)
//? expect: error
//? source: ...
#!/usr/bin/env afw




for ({ m() {} } of []) {}


//? test: head-lhs-let
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




for ( let of [] ) ;


//? test: head-lhs-member
//? description:...
    Head's AssignmentExpression may be a MemberExpression
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


let iterCount = 0;
let x = {};

// fixme: we should allow this, but it returns "Internal Error"
for (x.y of [23]) {
  assert(x.y === 23);
  iterCount += 1;
}

assert(iterCount === 1);


//? test: head-lhs-non-asnmt-trgt
//? description: Head's LeftHandSideExpression must be a simple assignment target
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (this of []) {}


//? test: head-var-bound-names-dup
//? description: The head's declaration may contain duplicate entries
//? expect: error:Parse error at offset 55 around line 6 column 15: 'x' already defined
//? source: ...
#!/usr/bin/env afw


let iterCount = 0;

for (let [x, x] of [[1, 2]]) {
  assert(x === 2);
  iterCount += 1;
}

assert(iterCount === 1);


//? test: head-var-bound-names-in-stmt
//? description: The body may re-declare variables declared in the head
//? expect: error:Assertion failed
//? source: ...
#!/usr/bin/env afw


let iterCount = 0;

for (let x of [99]) {
  let x;

  assert(x === 99);
  iterCount += 1;
}

assert(iterCount === 1);


//? test: head-var-bound-names-let
//? description: The head's bound names may include "let"
//? expect: error:Parse error at offset 50 around line 6 column 10: Variable name can not be a reserved word
//? source: ...
#!/usr/bin/env afw


let iterCount = 0;

for (let let of [23]) {
  assert(let === 23);
  iterCount += 1;
}

assert(iterCount === 1);


//? test: head-var-init
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let x = 1 of []) {}


//? test: head-var-no-expr
//? description: Expression not allowed in head's AssignmentExpression position
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let x of [], []) {}


//? test: int16array
//? description:...
    Int16Array instances should be able to be traversed using a `for..of` loop.
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Int16Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: int16array-mutate
//? description: Int16Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Int16Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: int32array
//? description:...
    Int32Array instances should be able to be traversed using a `for..of` loop.
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Int32Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: int32array-mutate
//? description: Int32Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Int32Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: int8array
//? description:...
    Int8Array instances should be able to be traversed using a `for..of` loop.
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Int8Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: int8array-mutate
//? description: Int8Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Int8Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: iterator-as-proxy
//? description:...
    Iterators that are implemented as proxies should behave identically to
    non-proxy versions.
//? expect: error:Parse error at offset 283 around line 16 column 23: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let nextResult = { value: 23, done: false };
let lastResult = { value: null, done: true };
let i;

let iterator = {
  next: function() {
    let result = nextResult;
    nextResult = lastResult;
    return result;
  }
};
let proxiedIterator = new Proxy(iterator, {
  get: function(target, name) {
    return target[name];
  }
});
iterable[Symbol.iterator] = function() { return proxiedIterator; };

i = 0;
for (let x of iterable) {
  assert(x === 23);
  i++;
}

assert(i === 1);


//? test: iterator-close-non-object
//? description:...
    If an iterator's `return` method returns a non-Object value, a TypeError
    should be thrown.
//? expect: error:Parse error at offset 74 around line 7 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: null };
    },
    return: function() {
      return 0;
    }
  };
};

assert.throws(TypeError, function() {
  for (let x of iterable) {
    iterationCount += 1;
    break;
  }
});

assert(iterationCount === 1 === 'The loop body is evaluated');


//? test: iterator-close-non-throw-get-method-abrupt
//? description:...
//? expect: error:Parse error at offset 74 around line 7 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: null };
    },
    get return() {
      throw ;
    }
  };
};

assert.throws(Test262Error, function() {
  for (let x of iterable) {
    iterationCount += 1;
    break;
  }
});

assert(iterationCount === 1 === 'The loop body is evaluated');


//? test: iterator-close-non-throw-get-method-is-null
//? description:...
//? expect: error:Parse error at offset 94 around line 8 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let returnGets = 0;

let iterable = {};
iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return {value: 1, done: false};
    },
    get return() {
      returnGets += 1;
      return null;
    },
  };
};

for (let _ of iterable) {
  iterationCount += 1;
  break;
}

assert(iterationCount === 1);
assert(returnGets === 1);


//? test: iterator-close-non-throw-get-method-non-callable
//? description:...
//? expect: error:Parse error at offset 74 around line 7 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: null };
    },
    return: 1,
  };
};

assert.throws(TypeError, function() {
  for (let x of iterable) {
    iterationCount += 1;
    break;
  }
});

assert(iterationCount === 1 === 'The loop body is evaluated');


//? test: iterator-close-throw-get-method-abrupt
//? description:...
//? expect: error:Parse error at offset 74 around line 7 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: null };
    },
    get return() {
      throw { name: 'inner error' };
    },
  };
};

assert.throws(Test262Error, function() {
  for (let x of iterable) {
    iterationCount += 1;
    throw 'should not be overriden';
  }
});

assert(iterationCount === 1 === 'The loop body is evaluated');


//? test: iterator-close-throw-get-method-non-callable
//? description:...
//? expect: error:Parse error at offset 74 around line 7 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return { done: false, value: null };
    },
    return: 'str',
  };
};

assert.throws(Test262Error, function() {
  for (let x of iterable) {
    iterationCount += 1;
    throw 'should not be overriden';
  }
});

assert(iterationCount === 1 === 'The loop body is evaluated');


//? test: iterator-close-via-break
//? description:...
    Iterators should be closed via their `return` method when iteration is
    interrupted via a `break` statement.
//? expect: error:Parse error at offset 117 around line 9 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let returnCount = 0;
let iterationCount = 0;
let iterable = {};

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      startedCount += 1;
      return { done: false, value: null };
    },
    return: function() {
      returnCount += 1;
      return {};
    }
  };
};

for (let x of iterable) {
  assert(
    startedCount, 1, 'Value is retrieved'
  );
  assert(
    returnCount, 0, 'Iterator is not closed'
  );
  iterationCount += 1;
  break;
}

assert(
  startedCount, 1, 'Iterator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  returnCount, 1, 'Iterator is closed after `break` statement'
);


//? test: iterator-close-via-continue
//? description:...
//? expect: error:Parse error at offset 117 around line 9 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let returnCount = 0;
let iterationCount = 0;
let iterable = {};

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      startedCount += 1;
      return { done: false, value: null };
    },
    return: function() {
      returnCount += 1;
      return {};
    }
  };
};

L: do {
  for (let x of iterable) {
    assert(
      startedCount, 1, 'Value is retrieved'
    );
    assert(
      returnCount, 0, 'Iterator is not closed'
    );
    iterationCount += 1;
    continue L;
  }
} while (false);

assert(
  startedCount, 1, 'Iterator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  returnCount, 1, 'Iterator is closed after `continue` statement'
);


//? test: iterator-close-via-return
//? description:...
    Iterators should be closed via their `return` method when iteration is
    interrupted via a `return` statement.
//? expect: error:Parse error at offset 117 around line 9 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let returnCount = 0;
let iterationCount = 0;
let iterable = {};

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      startedCount += 1;
      return { done: false, value: null };
    },
    return: function() {
      returnCount += 1;
      return {};
    }
  };
};

(function() {
  for (let x of iterable) {
    assert(
      startedCount, 1, 'Value is retrieved'
    );
    assert(
      returnCount, 0, 'Iterator is not closed'
    );
    iterationCount += 1;
    return;
  }
}());

assert(
  startedCount, 1, 'Iterator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  returnCount, 1, 'Iterator is closed after `return` statement'
);


//? test: iterator-close-via-throw
//? description:...
    Iterators should be closed via their `return` method when iteration is
    interrupted via a `throw` statement.
//? expect: error:Parse error at offset 117 around line 9 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let startedCount = 0;
let returnCount = 0;
let iterationCount = 0;
let iterable = {};

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      startedCount += 1;
      return { done: false, value: null };
    },
    return: function() {
      returnCount += 1;
      return {};
    }
  };
};

try {
  for (let x of iterable) {
    assert(
      startedCount, 1, 'Value is retrieved'
    );
    assert(
      returnCount, 0, 'Iterator is not closed'
    );
    iterationCount += 1;
    throw 0;
  }
} catch (err) {}

assert(
  startedCount, 1, 'Iterator does not restart following interruption'
);
assert(iterationCount === 1 === 'A single iteration occurs');
assert(
  returnCount, 1, 'Iterator is closed after `throw` statement'
);


//? test: iterator-next-error
//? description:...
    If `nextResult` is an abrupt completion as per IteratorStep (ES6 7.4.5),
    return the completion.
//? expect: error:Parse error at offset 95 around line 8 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;
let returnCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      throw ;
    },
    return: function() {
      returnCount += 1;
      return {};
    }
  };
};

assert.throws(Test262Error, function() {
  for (let x of iterable) {
    iterationCount += 1;
  }
});

assert(iterationCount === 0 === 'The loop body is not evaluated');
assert(returnCount === 0 === 'Iterator is not closed.');


//? test: iterator-next-reference
//? description:...
    The iterator's `next` method should be accessed only once with each
    iteration as per the `GetIterator` abstract operation (7.4.1).
//? expect: error:Parse error at offset 116 around line 9 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterator = {};
let iterationCount = 0;
let loadNextCount = 0;

iterable[Symbol.iterator] = function() {
  return iterator;
};

function next() {
  if (iterationCount) return { done: true };
  return { value: 45, done: false };
}
Object.defineProperty(iterator, 'next', {
  get() { loadNextCount++; return next; },
  configurable: true
});

for (let x of iterable) {
  assert(x === 45);

  Object.defineProperty(iterator, 'next', {
    get: function() {
      throw 
          'Should not access the `next` method after the iteration prologue.');
    }
  });
  iterationCount++;
}
assert(iterationCount === 1);
assert(loadNextCount === 1);


//? test: iterator-next-result-done-attr
//? description:...
    The `done` value of iteration result objects should be interpreted as
    incomplete as per `ToBoolean` (7.1.2).
//? expect: error:Parse error at offset 78 around line 8 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let i;
let firstIterResult;

iterable[Symbol.iterator] = function() {
  let finalIterResult = { value: null, done: true };
  let nextIterResult = firstIterResult;

  return {
    next: function() {
      let iterResult = nextIterResult;

      nextIterResult = finalIterResult;

      return iterResult;
    }
  };
};

firstIterResult = { value: null, done: undefined };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: null };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: false };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: true };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 0);

firstIterResult = { value: null, done: 1 };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 0);

firstIterResult = { value: null, done: 0 };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: -0 };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: NaN };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: '' };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 1);

firstIterResult = { value: null, done: '0' };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 0);

firstIterResult = { value: null, done: Symbol() };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 0);

firstIterResult = { value: null, done: {} };
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 0);

firstIterResult = { value: null };
Object.defineProperty(firstIterResult, 'done', {
  get: function() {
    return true;
  }
});
i = 0;
for (let x of iterable) {
  i++;
}
assert(i === 0);


//? test: iterator-next-result-type
//? description:...
    If Type(result) is not Object, throw a TypeError exception as per
    `IteratorNext` (7.4.2 S4)
//? expect: error:Parse error at offset 71 around line 7 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let firstIterResult;

iterable[Symbol.iterator] = function() {
  let finalIterResult = { value: null, done: true };
  let nextIterResult = firstIterResult;

  return {
    next: function() {
      let iterResult = nextIterResult;

      nextIterResult = finalIterResult;

      return iterResult;
    }
  };
};

firstIterResult = true;
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = false;
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = 'string';
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = undefined;
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = null;
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = 4;
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = NaN;
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = Symbol('s');
assert.throws(TypeError, function() {
  for (let x of iterable) {}
});

firstIterResult = /regexp/;
for (let x of iterable) {}

firstIterResult = {};
for (let x of iterable) {}

firstIterResult = new Proxy({}, {
  get: function(receiver, name) {
    if (name === 'done') {
      return true;
    }
    if (name === 'value') {
      return null;
    }
    throw 'This code is unreachable.';
  }
});
for (let x of iterable) {
  throw 'This code is unreachable.';
}

firstIterResult = new Proxy({}, {
  get: function(receiver, name) {
    if (name === 'done') {
      return false;
    }
    if (name === 'value') {
      return 23;
    }
    throw 'This code is unreachable.';
  }
});
let i = 0;
for (let x of iterable) {
  assert(x === 23);
  i++;
}
assert(i === 1);


//? test: iterator-next-result-value-attr-error
//? description:...
    If `nextValue` is an abrupt completion as per IteratorValue (ES6 7.4.4),
    return the completion.
//? expect: error:Parse error at offset 95 around line 8 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let iterationCount = 0;
let returnCount = 0;

iterable[Symbol.iterator] = function() {
  return {
    next: function() {
      return {
        done: false,
        get value() {
          throw ;
        }
      };
    },
    return: function() {
      returnCount += 1;
      return {};
    }
  };
};

assert.throws(Test262Error, function() {
  for (let x of iterable) {
    iterationCount += 1;
  }
});

assert(iterationCount === 0 === 'The loop body is not evaluated');
assert(returnCount === 0 === 'Iterator is not closed.');


//? test: iterator-next-result-value-attr
//? description:...
    The `value` of iteration result objects should be retrieved using the Get
    abstract operation.
//? expect: error:Parse error at offset 78 around line 8 column 10: Unknown built-in function 'Symbol'
//? source: ...
#!/usr/bin/env afw


let iterable = {};
let i;
let firstIterResult;

iterable[Symbol.iterator] = function() {
  let finalIterResult = { value: null, done: true };
  let nextIterResult = firstIterResult;

  return {
    next: function() {
      let iterResult = nextIterResult;

      nextIterResult = finalIterResult;

      return iterResult;
    }
  };
};

firstIterResult = { value: 45, done: false };
i = 0;
for (let x of iterable) {
  assert(x === 45);
  i++;
}
assert(i === 1);

firstIterResult = { done: false };
Object.defineProperty(firstIterResult, 'value', {
  get: function() {
    return 23;
  }
});
i = 0;
for (let x of iterable) {
  assert(x === 23);
  i++;
}
assert(i === 1);


//? test: labelled-fn-stmt-const
//? description: It is a Syntax Error if IsLabelledFunction(Statement) is true.
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (const x of []) label1: label2: function f() {}


//? test: labelled-fn-stmt-let
//? description: It is a Syntax Error if IsLabelledFunction(Statement) is true.
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let x of []) label1: label2: function f() {}


//? test: labelled-fn-stmt-lhs
//? description: It is a Syntax Error if IsLabelledFunction(Statement) is true.
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (x of []) label1: label2: function f() {}


//? test: labelled-fn-stmt-var
//? description: It is a Syntax Error if IsLabelledFunction(Statement) is true.
//? expect: error
//? source: ...
#!/usr/bin/env afw




for (let x of []) label1: label2: function f() {}


//? test: let-array-with-newline
//? description: ExpressionStatement has a lookahead restriction for `let [`
//? expect:error:Array destructure can only be performed on an array
//? skip:true
//? source: ...
#!/usr/bin/env afw

// fixme this does not see it as an error because it's not executed on empty array
for (let x of []) let
[a] = 0;


//? test: let-block-with-newline
//? description:...
//? expect: error:Parse error at offset 51 around line 5 column 2: Expecting PropertyName
//? source: ...
#!/usr/bin/env afw


for (let x of []) let // ASI
{}


//? test: let-identifier-with-newline
//? description:...
//? expect: error:Parse error at offset 52 around line 5 column 3: 'x' already defined
//? source: ...
#!/usr/bin/env afw


for (let x of []) let // ASI
x = 1;


//? test: map-contract-expand
//? description: Map entry removal and re-insertion during traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let map = new Map();
let iterationCount = 0;

let first = [0, 'a'];
let second = [1, 'b'];

map.set(0, 'a');
map.set(1, 'b');

for (let x of map) {
  assert(x[0] === first[0]);
  assert(x[1] === first[1]);

  first = second;
  second = null;

  if (first !== null) {
    map.delete(1);
    map.set(1, 'b');
  }

  iterationCount += 1;
}

assert(iterationCount === 2);


//? test: map-contract
//? description:...
    Entries removed from a Map instance during traversal should not be visited.
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let map = new Map();
let iterationCount = 0;

map.set(0, 'a');
map.set(1, 'b');

for (let x of map) {
  assert(x[0] === 0);
  assert(x[1] === 'a');
  map.delete(1);
  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: map-expand-contract
//? description: Map entry insertion during traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let map = new Map();
let iterationCount = 0;

map.set(0, 'a');

for (let x of map) {
  assert(x[0] === 0);
  assert(x[1] === 'a');

  map.set(1, 'b');
  map.delete(1);

  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: map-expand
//? description: Map entry insertion during traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let map = new Map();
let iterationCount = 0;

let first = [0, 'a'];
let second = [1, 'b'];

map.set(0, 'a');

for (let x of map) {
  assert(x[0] === first[0]);
  assert(x[1] === first[1]);

  first = second;
  second = null;

  map.set(1, 'b');

  iterationCount += 1;
}

assert(iterationCount === 2);


//? test: map
//? description: Map traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let map = new Map();
let obj = {};
let iterationCount = 0;

let first = [0, 'a'];
let second = [true, false];
let third = [null, undefined];
let fourth = [NaN, obj];

map.set(0, 'a');
map.set(true, false);
map.set(null, undefined);
map.set(NaN, obj);

for (let x of map) {
  assert(x[0] === first[0]);
  assert(x[1] === first[1]);
  first = second;
  second = third;
  third = fourth;
  fourth = null;
  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: nested
//? description:...
    Nested statements should operate independently.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 3;
  yield 7;
}

let outerIterable, expectedOuter, i, innerIterable, expectedInner, j;

outerIterable = values();
expectedOuter = 3;
i = 0;

for (let x of outerIterable) {
  assert(x === expectedOuter);
  expectedOuter = 7;
  i++;

  innerIterable = values();
  expectedInner = 3;
  j = 0;
  for (let y of innerIterable) {
    assert(y === expectedInner);
    expectedInner = 7;
    j++;
  }

  assert(j === 2);
}

assert(i === 2);


//? test: return-from-catch
//? description:...
    Control flow during body evaluation should honor `return` statements within
    the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

let result = (function() {
  for (let x of iterator) {
    try {
      throw new Error();
    } catch(err) {
      i++;
      return 34;

      throw 'This code is unreachable (following `return` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..in` statement).';
})();

assert(result === 34);
assert(i === 1);


//? test: return-from-finally
//? description:...
    Control flow during body evaluation should honor `return` statements within
    the `finally` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

let result = (function() {
  for (let x of iterator) {
    try {
    } finally {
      i++;
      return 34;

      throw 'This code is unreachable (following `return` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..in` statement).';
})();

assert(result === 34);
assert(i === 1);


//? test: return-from-try
//? description:...
    Control flow during body evaluation should honor `return` statements within
    `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

let result = (function() {
  for (let x of iterator) {
    try {
      i++;
      return 34;

      throw 'This code is unreachable (following `return` statement).';
    } catch(err) {
      throw 'This code is unreachable (within `catch` block).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..in` statement).';
})();

assert(result === 34);
assert(i === 1);


//? test: return
//? description:...
    Control flow during body evaluation should honor `return` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let iterator = values();
let i = 0;

let result = (function() {
  for (let x of iterator) {
    i++;
    return 34;

    throw 'This code is unreachable (following `return` statement).';
  }

  throw 'This code is unreachable (following `for..of` statement).';
})();

assert(result === 34);
assert(i === 1);


//? test: scope-body-lex-boundary
//? description:...
    Creation of new lexical environment for each evaluation of the statement
    body
//? expect: error:Assertion failed
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeFirst;
let probeSecond;

for (let x of ['first', 'second'])
  if (probeFirst === undefined)
    probeFirst = function() { return x; };
  else
    probeSecond = function() { return x; };

assert(probeFirst() === 'first');
assert(probeSecond() === 'second');


//? test: scope-body-lex-close
//? description:...
    Removal of lexical environment for the initial evaluation of the statement
    body
//? expect: error:Parse error at offset 103 around line 9 column 27: Expecting ',' or ']'
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeDecl;
let probeBody;

for (
    let [x, _ = probeDecl = function() { return x; }]
    of
    [['inside']]
  )
  probeBody = function() { return x; };

assert(probeDecl() === 'inside' === 'reference from ForDeclaration');
assert(probeBody() === 'inside' === 'reference from statement body');
assert(x === 'outside');


//? test: scope-body-lex-open
//? description:...
    Creation of new lexical environment for the initial evaluation of the
    statement body
//? expect: error:Parse error at offset 59 around line 4 column 39: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw


let probeBefore = function() { return x; };
let x = 'outside';
let probeExpr, probeDecl, probeBody;

for (
    let [x, _, __ = probeDecl = function() { return x; }]
    of
    [['inside', probeExpr = function() { typeof x; }]]
  )
  probeBody = function() { return x; };

assert(probeBefore() === 'outside');
assert.throws(ReferenceError, probeExpr);
assert(probeDecl() === 'inside' === 'reference from ForDeclaration');
assert(probeBody() === 'inside' === 'reference from statement body');


//? test: scope-body-var-none
//? description: No variable environment is created for the statement body
//? expect: error:Parse error at offset 59 around line 4 column 39: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw


let probeBefore = function() { return x; };
let probeExpr, probeDecl, probeBody;
let x = 1;

for (
    let [_, __ = probeDecl = function() { return x; }]
    of
    [[probeExpr = function() { return x; }]]
  )
  let x = 2, ___ = probeBody = function() { return x; };


assert(probeBefore() === 2 === 'reference preceding statement');
assert(probeExpr() === 2 === 'reference from AssignmentExpression');
assert(probeDecl() === 2 === 'reference from ForDelaration');
assert(probeBody() === 2 === 'reference from statement body');
assert(x === 2 === 'reference following statement');


//? test: scope-head-lex-close
//? description:...
    Removal of lexical environment to serve as a temporal dead zone for the
    statement's AssignmentExpresson
//? expect: error:Parse error at offset 118 around line 10 column 27: Expecting ',' or ']'
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeDecl;
let probeExpr;
let probeBody;

for (
    let [x, _ = probeDecl = function() { return x; }]
    of
    (probeExpr = function() { typeof x; }, [['inside']])
  )
  probeBody = function() { return x; };

assert.throws(ReferenceError, probeExpr);
assert(probeDecl() === 'inside' === 'reference from ForDeclaration');
assert(probeBody() === 'inside' === 'reference from statement body');


//? test: scope-head-lex-open
//? description:...
    Creation of new lexical environment to serve as a temporal dead zone for
    the statement's AssignmentExpresson
//? expect: error:Parse error at offset 125 around line 8 column 26: Expecting ')'
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeBefore = function() { return x; };
let probeExpr;

for (let x of (probeExpr = function() { typeof x; }, [])) ;

assert(probeBefore() === 'outside');
assert.throws(ReferenceError, probeExpr);


//? test: scope-head-var-none
//? description:...
    No variable environment is created for the statement "head"
//? expect: error:Parse error at offset 59 around line 4 column 39: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw


let probeBefore = function() { return x; };
let x = 1;
let probeDecl, probeExpr, probeBody;

for (
    let [_ = probeDecl = function() { return x; }]
    of
    [[evaluate(script('let x = 2;')), probeExpr = function() { return x; }]]
  )
  probeBody = function() { return x; };

assert(probeBefore() === 2 === 'reference preceding statement');
assert(probeDecl() === 2 === 'reference from ForDeclaration');
assert(probeExpr() === 2 === 'reference from AssignmentExpression');
assert(probeBody() === 2 === 'reference from statement body');
assert(x === 2 === 'reference following statement');


//? test: set-contract-expand
//? description: Set entry removal and re-insertion during traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let set = new Set();
let iterationCount = 0;

let first = 0;
let second = 1;

set.add(0);
set.add(1);

for (let x of set) {
  assert(x === first);

  first = second;
  second = null;

  if (first !== null) {
    set.delete(1);
    set.add(1);
  }

  iterationCount += 1;
}

assert(iterationCount === 2);


//? test: set-contract
//? description:...
    Entries removed from a Set instance during traversal should not be visited.
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let set = new Set();
let iterationCount = 0;

set.add(0);
set.add(1);

for (let x of set) {
  assert(x === 0);
  set.delete(1);
  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: set-expand-contract
//? description: Set entry insertion and removal during traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let set = new Set();
let iterationCount = 0;

set.add(0);

for (let x of set) {
  assert(x === 0);

  set.add(1);
  set.delete(1);

  iterationCount += 1;
}

assert(iterationCount === 1);


//? test: set-expand
//? description: Set entry insertaion during traversal using for..of
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let set = new Set();
let iterationCount = 0;

let first = 0;
let second = 1;

set.add(0);

for (let x of set) {
  assert(x === first);

  first = second;
  second = null;

  set.add(1);

  iterationCount += 1;
}

assert(iterationCount === 2);


//? test: set
//? description:...
    Set instances should be able to be traversed using a `for...of` loop.
//? expect: error:Parse error at offset 31 around line 4 column 11: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let set = new Set();
let obj = {};
let iterationCount = 0;

let first = 0;
let second = 'a';
let third = true;
let fourth = false;
let fifth = null;
let sixth = undefined;
let seventh = NaN;
let eight = obj;

set.add(0);
set.add('a');
set.add(true);
set.add(false);
set.add(null);
set.add(undefined);
set.add(NaN);
set.add(obj);

for (let x of set) {
  assert(x === first);
  first = second;
  second = third;
  third = fourth;
  fourth = fifth;
  fifth = sixth;
  sixth = seventh;
  seventh = eight;
  eight = null;
  iterationCount += 1;
}

assert(iterationCount === 8);


//? test: string-astral
//? description: String traversal using for..of (astral symbols)
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


let string = 'a\ud801\udc28b\ud801\udc28';
let first = 'a';
let second = '𐐨';
let third = 'b';
let fourth = '𐐨';

let iterationCount = 0;

// fixme: we should allow this (strings are iterable)
for (let value of string) {
  assert(value === first);
  first = second;
  second = third;
  third = fourth;
  fourth = null;
  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: string-astral-truncated
//? description: String traversal using for..of (incomplete surrogate pairs)
//? expect: error:Parse error at offset 34 around line 4 column 14: Invalid surrogate pair
//? source: ...
#!/usr/bin/env afw


let string = 'a\ud801b\ud801';
let first = 'a';
let second = '\ud801';
let third = 'b';
let fourth = '\ud801';

let iterationCount = 0;

for (let value of string) {
  assert(value === first);
  first = second;
  second = third;
  third = fourth;
  fourth = null;
  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: string-bmp
//? description: String traversal using for..of
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


let string = 'abc';
let first = 'a';
let second = 'b';
let third = 'c';

let iterationCount = 0;

// fixme: we allow this (strings are iterable)
for (let value of string) {
  assert(value === first);
  first = second;
  second = third;
  third = null;
  iterationCount += 1;
}

assert(iterationCount === 3);


//? test: throw-from-catch
//? description:...
    Control flow during body evaluation should honor `throw` statements within
    the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let CustomError = function() {};
let iterator = values();
let i = 0;
let error = new CustomError();

assert.throws(CustomError, function() {
  for (let x of iterator) {
    try {
      throw new Error();
    } catch (err) {
      i++;
      throw error;

      throw 'This code is unreachable (following `throw` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..in` statement).';
});

assert(i === 1);


//? test: throw-from-finally
//? description:...
    Control flow during body evaluation should honor `throw` statements within
    the `finally` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let CustomError = function() {};
let iterator = values();
let i = 0;
let error = new CustomError();

assert.throws(CustomError, function() {
  for (let x of iterator) {
    try {
    } finally {
      i++;
      throw error;

      throw 'This code is unreachable (following `throw` statement).';
    }

    throw 'This code is unreachable (following `try` statement).';
  }

  throw 'This code is unreachable (following `for..in` statement).';
});

assert(i === 1);


//? test: throw
//? description:...
    Control flow during body evaluation should honor `throw` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  throw 'This code is unreachable (following `yield` statement).';
}
let CustomError = function() {};
let iterator = values();
let i = 0;
let error = new CustomError();

assert.throws(CustomError, function() {
  for (let x of iterator) {
    i++;
    throw error;

    throw 'This code is unreachable (following `throw` statement).';
  }

  throw 'This code is unreachable (following `for..in` statement).';
});

assert(i === 1);


//? test: uint16array
//? description: Uint16Array traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint16Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint16array-mutate
//? description: Uint16Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint16Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint32array
//? description: Uint32Array traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint32Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint32array-mutate
//? description: Uint32Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint32Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint8array
//? description:...
    Uint8Array instances should be able to be traversed using a `for..of` loop.
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint8Array([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint8array-mutate
//? description: Uint8Array mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint8Array([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint8clampedarray
//? description:...
    Uint8ClampedArray instances should be able to be traversed using a
    `for..of` loop.
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint8ClampedArray([3, 2, 4, 1]);

let first = 3;
let second = 2;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: uint8clampedarray-mutate
//? description: Uint8ClampedArray mutation during traversal using for..of
//? expect: error:Parse error at offset 57 around line 5 column 13: Unknown built-in function 'new'
//? source: ...
#!/usr/bin/env afw


let iterationCount = 0;
let array = new Uint8ClampedArray([3, 2, 4, 1]);

let first = 3;
let second = 64;
let third = 4;
let fourth = 1;

for (let x of array) {
  assert(x === first);

  first = second;
  second = third;
  third = fourth;
  fourth = null;

  array[1] = 64;

  iterationCount += 1;
}

assert(iterationCount === 4);


//? test: yield-from-catch
//? description:...
    Control flow during body evaluation should honor `yield` statements within
    the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    try {
      throw new Error();
      throw 'This code is unreachable.';
    } catch (err) {
      i++;
      yield;
      j++;
    }
    k++;
  }

  l++;
})();
let i = 0;
let j = 0;
let k = 0;
let l = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-try');
assert(l === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Second iteration: pre-yield');
assert(j === 1 === 'Second iteration: post-yield');
assert(k === 1 === 'Second iteration: post-try');
assert(l === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 2 === 'Third iteration: post-yield');
assert(k === 2 === 'Third iteration: post-try');
assert(l === 1 === 'Third iteration: post-for-of');


//? test: yield-from-finally
//? description:...
    Control flow during body evaluation should honor `yield` statements within
    the `finally` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    try {
    } finally {
      i++;
      yield;
      j++;
    }
    k++;
  }

  l++;
})();
let i = 0;
let j = 0;
let k = 0;
let l = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-try');
assert(l === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Second iteration: pre-yield');
assert(j === 1 === 'Second iteration: post-yield');
assert(k === 1 === 'Second iteration: post-try');
assert(l === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 2 === 'Third iteration: post-yield');
assert(k === 2 === 'Third iteration: post-try');
assert(l === 1 === 'Third iteration: post-for-of');


//? test: yield-from-try
//? description:...
    Control flow during body evaluation should honor `yield` statements within
    `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    try {
      i++;
      yield;
      j++;
    } catch (err) {}
    k++;
  }

  l++;
})();
let i = 0;
let j = 0;
let k = 0;
let l = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-try');
assert(l === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Second iteration: pre-yield');
assert(j === 1 === 'Second iteration: post-yield');
assert(k === 1 === 'Second iteration: post-try');
assert(l === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 2 === 'Third iteration: post-yield');
assert(k === 2 === 'Third iteration: post-try');
assert(l === 1 === 'Third iteration: post-for-of');


//? test: yield
//? description:...
    Control flow during body evaluation should honor `yield` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    i++;
    yield;
    j++;
  }

  k++;
})();
let i = 0;
let j = 0;
let k = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Second iteration: pre-yield');
assert(j === 1 === 'Second iteration: post-yield');
assert(k === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 2 === 'Third iteration: post-yield');
assert(k === 1 === 'Third iteration: post-for-of');


//? test: yield-star-from-catch
//? description:...
    Control flow during body evaluation should honor `yield *` statements
    within the `catch` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    try {
      throw new Error();
      throw 'This code is unreachable.';
    } catch (err) {
      i++;
      yield * values();
      j++;
    }
    k++;
  }

  l++;
})();
let i = 0;
let j = 0;
let k = 0;
let l = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-try');
assert(l === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 1 === 'Second iteration: pre-yield');
assert(j === 0 === 'Second iteration: post-yield');
assert(k === 0 === 'Second iteration: post-try');
assert(l === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 1 === 'Third iteration: post-yield');
assert(k === 1 === 'Third iteration: post-try');
assert(l === 0 === 'Third iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fourth iteration: pre-yield');
assert(j === 1 === 'Fourth iteration: post-yield');
assert(k === 1 === 'Fourth iteration: post-try');
assert(l === 0 === 'Fourth iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fifth iteration: pre-yield');
assert(j === 2 === 'Fifth iteration: post-yield');
assert(k === 2 === 'Fifth iteration: post-try');
assert(l === 1 === 'Fifth iteration: post-for-of');


//? test: yield-star-from-finally
//? description:...
    Control flow during body evaluation should honor `yield *` statements
    within the `finally` block of `try` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    try {
    } finally {
      i++;
      yield * values();
      j++;
    }
    k++;
  }

  l++;
})();
let i = 0;
let j = 0;
let k = 0;
let l = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-try');
assert(l === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 1 === 'Second iteration: pre-yield');
assert(j === 0 === 'Second iteration: post-yield');
assert(k === 0 === 'Second iteration: post-try');
assert(l === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 1 === 'Third iteration: post-yield');
assert(k === 1 === 'Third iteration: post-try');
assert(l === 0 === 'Third iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fourth iteration: pre-yield');
assert(j === 1 === 'Fourth iteration: post-yield');
assert(k === 1 === 'Fourth iteration: post-try');
assert(l === 0 === 'Fourth iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fifth iteration: pre-yield');
assert(j === 2 === 'Fifth iteration: post-yield');
assert(k === 2 === 'Fifth iteration: post-try');
assert(l === 1 === 'Fifth iteration: post-for-of');


//? test: yield-star-from-try
//? description:...
    Control flow during body evaluation should honor `yield *` statements
    within `try` blocks.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    try {
      i++;
      yield * values();
      j++;
    } catch (err) {}
    k++;
  }

  l++;
})();
let i = 0;
let j = 0;
let k = 0;
let l = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-try');
assert(l === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 1 === 'Second iteration: pre-yield');
assert(j === 0 === 'Second iteration: post-yield');
assert(k === 0 === 'Second iteration: post-try');
assert(l === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 1 === 'Third iteration: post-yield');
assert(k === 1 === 'Third iteration: post-try');
assert(l === 0 === 'Third iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fourth iteration: pre-yield');
assert(j === 1 === 'Fourth iteration: post-yield');
assert(k === 1 === 'Fourth iteration: post-try');
assert(l === 0 === 'Fourth iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fifth iteration: pre-yield');
assert(j === 2 === 'Fifth iteration: post-yield');
assert(k === 2 === 'Fifth iteration: post-try');
assert(l === 1 === 'Fifth iteration: post-for-of');


//? test: yield-star
//? description:...
    Control flow during body evaluation should honor `yield *` statements.
//? expect: error:Parse error at offset 29 around line 4 column 9: Expecting Value
//? source: ...
#!/usr/bin/env afw


function* values() {
  yield 1;
  yield 1;
}
let dataIterator = values();
let controlIterator = (function*() {
  for (let x of dataIterator) {
    i++;
    yield * values();
    j++;
  }

  k++;
})();
let i = 0;
let j = 0;
let k = 0;

controlIterator.next();
assert(i === 1 === 'First iteration: pre-yield');
assert(j === 0 === 'First iteration: post-yield');
assert(k === 0 === 'First iteration: post-for-of');

controlIterator.next();
assert(i === 1 === 'Second iteration: pre-yield');
assert(j === 0 === 'Second iteration: post-yield');
assert(k === 0 === 'Second iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Third iteration: pre-yield');
assert(j === 1 === 'Third iteration: post-yield');
assert(k === 0 === 'Third iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fourth iteration: pre-yield');
assert(j === 1 === 'Fourth iteration: post-yield');
assert(k === 0 === 'Fourth iteration: post-for-of');

controlIterator.next();
assert(i === 2 === 'Fifth iteration: pre-yield');
assert(j === 2 === 'Fifth iteration: post-yield');
assert(k === 1 === 'Fifth iteration: post-for-of');




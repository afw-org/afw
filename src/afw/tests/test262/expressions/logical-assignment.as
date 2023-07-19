#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: logical-assignment.as
//? customPurpose: Part of test262
//? description: expressions/logical-assignment
//? sourceType: script
//?
//? test: lgcl-and-assignment-operator-non-simple-lhs
//? description:  It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple.
//? expect: error
//? source: ...
#!/usr/bin/env afw


function test() {}
test() &&= 1;


//? test: lgcl-and-assignment-operator-unresolved-lhs
//? description:  ReferenceError is thrown if the LeftHandSideExpression of a Logical Assignment operator(&&=) evaluates to an unresolvable reference
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function unresolved
//? source: ...
#!/usr/bin/env afw

unresolved &&= 1;



//? test: lgcl-and-assignment-operator-unresolved-rhs
//? description:  ReferenceError is not thrown if the AssignmentExpression of a Logical Assignment operator(&&=) evaluates to an unresolvable reference and the AssignmentExpression is not evaluated.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


let value = 0;

assert(value &&= unresolved === 0, "value");


//? test: lgcl-and-assignment-operator-unresolved-rhs-put
//? description:  ReferenceError is thrown if the AssignmentExpression of a Logical Assignment operator(&&=) evaluates to an unresolvable reference and the AssignmentExpression is evaluated.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


let value = 2;

// \fixme we don't have assert.throws()
assert.throws(ReferenceError, function() {
  value &&= unresolved;
});
assert(value === 2, "value");


//? test: lgcl-and-eval-strict
//? description:  Strict Mode - SyntaxError is thrown if the identifier eval appear as the LeftHandSideExpression of a Logical Assignment operator(&&=)
//? expect: error
//? source: ...
#!/usr/bin/env afw



eval &&= 20;


//? test: lgcl-and-non-simple
//? description: Logical "&&=" assignment with non-simple target
//? expect: error
//? source: ...
#!/usr/bin/env afw



1 &&= 1;



//? test: lgcl-nullish-assignment-operator
//? description: Logical Nullish Assignment Operator
//? expect: null
//? source: ...
#!/usr/bin/env afw


let value = undefined;
value ??= 1;
assert(value === 1, "(value ??= 1) === 1; where value = undefined");

value = null;
value ??= 1;
assert(value === 1, "(value ??= 1) === 1; where value = null");

value = false;
value ??= 1;
assert(value === false, "(value ??= 1) === false; where value = false");

value = 0;
value ??= 1;
assert(value === 0, "(value ??= 1) === 0; where value = 0");

value = -0;
value ??= 1;
assert(value === -0, "(value ??= 1) === -0; where value = -0");

value = NaN;
value ??= 1;
assert(is_NaN(value), "(value ??= 1) === NaN; where value = NaN");


value = "";
value ??= 1;
assert(value === "", '(value ??= 1) === "" where value = ""');


value = true;
value ??= 1;
assert(value === true, "(value ??= 1) === true; where value = true");

value = 2;
value ??= 1;
assert(value === 2, "(value ??= 1) === 2; where value = 2");

value = "test";
value ??= 1;
assert(value === "test", '(value ??= 1) === "test"; where value = "test"');

let obj = {};
value = obj;
assert(value === obj, "(value ??= 1) === {}; where value = {}");



//? test: lgcl-nullish-assignment-operator-non-simple-lhs
//? description:  It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple.
//? expect: error
//? source: ...
#!/usr/bin/env afw

function test() {}
test() ??= 1;



//? test: lgcl-nullish-assignment-operator-unresolved-lhs
//? description:  ReferenceError is thrown if the LeftHandSideExpression of a Logical Assignment operator(??=) evaluates to an unresolvable reference
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function unresolved
//? source: ...
#!/usr/bin/env afw

unresolved ??= 1;



//? test: lgcl-nullish-assignment-operator-unresolved-rhs
//? description:  ReferenceError is not thrown if the AssignmentExpression of a Logical Assignment operator(??=) evaluates to an unresolvable reference and the AssignmentExpression is not evaluated.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


let value = 0;
// \fixme this is interesting - not a reference error if the rhs is not evaluated
value ??= unresolved;
assert(value === 0, "value");


//? test: lgcl-nullish-assignment-operator-unresolved-rhs-put
//? description:  ReferenceError is thrown if the AssignmentExpression of a Logical Assignment operator(??=) evaluates to an unresolvable reference and the AssignmentExpression is evaluated.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


let value = undefined;

// \fixme we don't allow for this
assert.throws(ReferenceError, function() {
  value ??= unresolved;
});
assert(value === undefined, "value");


//? test: lgcl-nullish-eval-strict
//? description:  Strict Mode - SyntaxError is thrown if the identifier eval appear as the LeftHandSideExpression of a Logical Assignment operator(??=)
//? expect: error
//? source: ...
#!/usr/bin/env afw

eval ??= 20;


//? test: lgcl-nullish-non-simple
//? description: Logical "??=" assignment with non-simple target
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 ??= 1;


//? test: lgcl-nullish-whitespace
//? description: Checking by evaluating expression "x[...]??=[...]y"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

let x;

x = null;
assert(x	??=	1 === 1, 'U+0009 (expression)');
assert(x === 1, 'U+0009 (side effect)');

x = null;
assert(x??=1 === 1, 'U+000B (expression)');
assert(x === 1, 'U+000B (side effect)');

x = null;
assert(x??=1 === 1, 'U+000C (expression)');
assert(x === 1, 'U+000C (side effect)');

x = null;
assert(x ??= 1 === 1, 'U+0020 (expression)');
assert(x === 1, 'U+0020 (side effect)');

x = null;
assert(x ??= 1 === 1, 'U+00A0 (expression)');
assert(x === 1, 'U+00A0 (side effect)');

x = null;
assert.sameValue(x
??=
1, 1, 'U+000A (expression)');
assert(x === 1, 'U+000A (side effect)');

x = null;
assert.sameValue(x
??=
1, 1, 'U+000D (expression)');
assert(x === 1, 'U+000D (side effect)');

x = null;
assert.sameValue(x ??= 1, 1, 'U+2028 (expression)');
assert(x === 1, 'U+2028 (side effect)');

x = null;
assert.sameValue(x ??= 1, 1, 'U+2029 (expression)');
assert(x === 1, 'U+2029 (side effect)');

x = null;
assert.sameValue(x
  ??=
  1, 1, 'U+0009U+000BU+000CU+0020U+00A0U+000AU+000DU+2028U+2029 (expression)');
assert(x === 1, 'U+0009U+000BU+000CU+0020U+00A0U+000AU+000DU+2028U+2029 (side effect)');



//? test: lgcl-or-assignment-operator-non-simple-lhs
//? description:  It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple.
//? expect: error
//? source: ...
#!/usr/bin/env afw

function test() {}
test() ||= 1;



//? test: lgcl-or-assignment-operator-unresolved-lhs
//? description:  ReferenceError is thrown if the LeftHandSideExpression of a Logical Assignment operator(||=) evaluates to an unresolvable reference
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function unresolved
//? source: ...
#!/usr/bin/env afw

unresolved ||= 1;



//? test: lgcl-or-assignment-operator-unresolved-rhs
//? description:  ReferenceError is not thrown if the AssignmentExpression of a Logical Assignment operator(||=) evaluates to an unresolvable reference and the AssignmentExpression is not evaluated.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

let value = 2;
// \fixme we don't allow this
value ||= unresolved;

assert(value === 2, "value");


//? test: lgcl-or-assignment-operator-unresolved-rhs-put
//? description:  ReferenceError is thrown if the AssignmentExpression of a Logical Assignment operator(||=) evaluates to an unresolvable reference and the AssignmentExpression is evaluated.
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

let value = 0;

// \fixme we don't allow for this
assert.throws(ReferenceError, function() {
  value ||= unresolved;
});
assert(value === 0, "value");


//? test: lgcl-or-eval-strict
//? description:  Strict Mode - SyntaxError is thrown if the identifier eval appear as the LeftHandSideExpression of a Logical Assignment operator(||=)
//? expect: error
//? source: ...
#!/usr/bin/env afw

eval ||= 20;


//? test: lgcl-or-non-simple
//? description: Logical "||=" assignment with non-simple target
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 ||= 1;
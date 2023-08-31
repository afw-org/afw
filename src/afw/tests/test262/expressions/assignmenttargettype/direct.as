#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: direct.as
//? customPurpose: Part of test262
//? description: expressions/assignmenttargettype/direct
//? sourceType: script
//?
//? test: direct-additiveexpression-minus-multiplicativeexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x - y = 1;


//? test: direct-additiveexpression-minus-multiplicativeexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 - 2 = 1;


//? test: direct-additiveexpression-minus-multiplicativeexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true - false = 1;


//? test: direct-additiveexpression-plus-multiplicativeexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x + 1 = 1;


//? test: direct-additiveexpression-plus-multiplicativeexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true + false = 1;


//? test: direct-callexpression-arguments
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

f() = 1;


//? test: direct-callexpression-templateliteral
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

f()`` = 1;


//? test: direct-coalesceexpressionhead-coalesce-bitwiseorexpression-0
//? description: CoalesceExpression: CoalesceExpressionHead ?? BitwiseORExpression Static Semantics AssignmentTargetType, Return invalid.
//? expect: error
//? source: ...
#!/usr/bin/env afw

x ?? y = 1;

//? test: direct-coalesceexpressionhead-coalesce-bitwiseorexpression-1
//? description: CoalesceExpression: CoalesceExpressionHead ?? BitwiseORExpression Static Semantics AssignmentTargetType, Return invalid.
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 ?? 2 = 1;


//? test: direct-coalesceexpressionhead-coalesce-bitwiseorexpression-2
//? description: CoalesceExpression: CoalesceExpressionHead ?? BitwiseORExpression Static Semantics AssignmentTargetType, Return invalid.
//? expect: error
//? source: ...
#!/usr/bin/env afw

true ?? false = 1;


//? test: direct-equalityexpression-abstract-equal-relationalexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x == y = 1;


//? test: direct-equalityexpression-abstract-equal-relationalexpression
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 == 2 = 1;


//? test: direct-equalityexpression-abstract-equal-relationalexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true == false = 1;


//? test: direct-equalityexpression-abstract-not-equal-relationalexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x != y = 1;


//? test: direct-equalityexpression-abstract-not-equal-relationalexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 != 2 = 1;


//? test: direct-equalityexpression-abstract-not-equal-relationalexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true != false = 1;


//? test: direct-equalityexpression-strict-equal-relationalexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x == y = 1;


//? test: direct-equalityexpression-strict-equal-relationalexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 == 2 = 1;


//? test: direct-equalityexpression-strict-equal-relationalexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true == false = 1;


//? test: direct-equalityexpression-strict-not-equal-relationalexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x !== y = 1;


//? test: direct-equalityexpression-strict-not-equal-relationalexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 !== 2 = 1;


//? test: direct-equalityexpression-strict-not-equal-relationalexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true !== false = 1;


//? test: direct-lefthandsideexpression-assignment-assignmentexpression-0
//? description: If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, the following Early Error rule is applied, It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x = y) = 1;


//? test: direct-lefthandsideexpression-assignment-assignmentexpression-1
//? description: If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, the following Early Error rule is applied, It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x = 1) = 1;


//? test: direct-lefthandsideexpression-assignment-assignmentexpression-2
//? description: If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, the following Early Error rule is applied, It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x = true) = 1;


//? test: direct-lefthandsideexpression-coalesce-assignment-assignmentexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ??= y) = 1;


//? test: direct-lefthandsideexpression-coalesce-assignment-assignmentexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ??= 1) = 1;


//? test: direct-lefthandsideexpression-coalesce-assignment-assignmentexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ??= true) = 1;


//? test: direct-lefthandsideexpression-logical-and-assignment-assignmentexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x &&= y) = 1;


//? test: direct-lefthandsideexpression-logical-and-assignment-assignmentexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x &&= 1) = 1;


//? test: direct-lefthandsideexpression-logical-and-assignment-assignmentexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x &&= true) = 1;


//? test: direct-lefthandsideexpression-logical-or-assignment-assignmentexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ||= y) = 1;


//? test: direct-lefthandsideexpression-logical-or-assignment-assignmentexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ||= 1) = 1;


//? test: direct-lefthandsideexpression-logical-or-assignment-assignmentexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ||= true) = 1;


//? test: direct-lefthandsideexpression-minus-minus
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x-- = 1;


//? test: direct-lefthandsideexpression-plus-plus
//? description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x++ = 1;


//? test: direct-logicalandexpression-logical-and-bitwiseorexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x && y = 1;


//? test: direct-logicalandexpression-logical-and-bitwiseorexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 && 2 = 1;


//? test: direct-logicalandexpression-logical-and-bitwiseorexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true && false = 1;


//? test: direct-logicalorexpression-logical-or-logicalandexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x || y = 1;


//? test: direct-logicalorexpression-logical-or-logicalandexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 || 2 = 1;


//? test: direct-logicalorexpression-logical-or-logicalandexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true || false = 1;


//? test: direct-memberexpression-templateliteral
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

a.f()`` = 1;


//? test: direct-minus-minus-unaryexpression
//? description: It is an early Syntax Error if AssignmentTargetType of UnaryExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

--x = 1;


//? test: direct-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-0
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x * y = 1;


//? test: direct-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-1
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 * 2 = 1;


//? test: direct-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-2
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true * false = 1;


//? test: direct-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-3
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x / y = 1;


//? test: direct-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-4
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 / 2 = 1;


//? test: direct-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-5
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true / false = 1;


//? test: direct-optionalexpression
//? description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x?.y = 1;


//? test: direct-plus-plus-unaryexpression
//? description: It is an early Syntax Error if AssignmentTargetType of UnaryExpression is not simple. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

++x = 1;


//? test: direct-primaryexpression-functionexpression
//? description: PrimaryExpression FunctionExpression, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

function() {} = 1;


//? test: direct-primaryexpression-literal-boolean
//? description: PrimaryExpression Literal BooleanLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true = 1;


//? test: direct-primaryexpression-literal-null
//? description: PrimaryExpression Literal NullLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

null = 1;


//? test: direct-primaryexpression-literal-numeric
//? description: PrimaryExpression Literal NumericLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

0 = 1;


//? test: direct-primaryexpression-literal-string
//? description: PrimaryExpression Literal StringLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

'' = 1;


//? test: direct-primaryexpression-objectliteral
//? description: PrimaryExpression ObjectLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

{} = 1;


//? test: direct-primaryexpression-regularexpressionliteral
//? description: PrimaryExpression RegularExpressionLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

/1/ = 1;


//? test: direct-primaryexpression-templateliteral
//? description: PrimaryExpression TemplateLiteral; Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

`` = 1;

//? test: direct-relationalexpression-greater-than-or-equal-to-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x >= y = 1;


//? test: direct-relationalexpression-greater-than-or-equal-to-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 >= 2 = 1;


//? test: direct-relationalexpression-greater-than-or-equal-to-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true >= false = 1;


//? test: direct-relationalexpression-greater-than-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x > y = 1;


//? test: direct-relationalexpression-greater-than-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 > 2 = 1;


//? test: direct-relationalexpression-greater-than-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true > false = 1;


//? test: direct-relationalexpression-in-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x in y = 1;


//? test: direct-relationalexpression-in-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 in 2 = 1;


//? test: direct-relationalexpression-in-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true in false = 1;


//? test: direct-relationalexpression-instanceof-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x instanceof y = 1;


//? test: direct-relationalexpression-instanceof-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 instanceof 2 = 1;


//? test: direct-relationalexpression-instanceof-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true instanceof false = 1;


//? test: direct-relationalexpression-less-than-or-equal-to-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x <= y = 1;


//? test: direct-relationalexpression-less-than-or-equal-to-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 <= 2 = 1;


//? test: direct-relationalexpression-less-than-or-equal-to-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true <= false = 1;


//? test: direct-relationalexpression-less-than-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x < y = 1;


//? test: direct-relationalexpression-less-than-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 < 2 = 1;


//? test: direct-relationalexpression-less-than-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true < false = 1;


//? test: direct-shortcircuitexpression-question-assignmentexpression-else-assignmentexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ? y : z) = 1;


//? test: direct-shortcircuitexpression-question-assignmentexpression-else-assignmentexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 ? 2 : 3) = 1;


//? test: direct-shortcircuitexpression-question-assignmentexpression-else-assignmentexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true ? false : true) = 1;


//? test: direct-unaryexpression-delete-unaryexpression
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

delete x.y = 1;


//? test: direct-unaryexpression-exclamation-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

!x = 1;


//? test: direct-unaryexpression-exclamation-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

!1 = 1;


//? test: direct-unaryexpression-exclamation-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

!true = 1;


//? test: direct-unaryexpression-minus-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

-x = 1;


//? test: direct-unaryexpression-minus-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

-1 = 1;


//? test: direct-unaryexpression-minus-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

-true = 1;


//? test: direct-unaryexpression-plus-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

+x = 1;


//? test: direct-unaryexpression-plus-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

+1 = 1;


//? test: direct-unaryexpression-plus-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

+true = 1;


//? test: direct-unaryexpression-tilde-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

~x = 1;


//? test: direct-unaryexpression-tilde-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

~1 = 1;


//? test: direct-unaryexpression-tilde-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

~true = 1;


//? test: direct-unaryexpression-typeof-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

typeof x = 1;


//? test: direct-unaryexpression-typeof-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

typeof 1 = 1;


//? test: direct-unaryexpression-typeof-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

typeof true = 1;


//? test: direct-unaryexpression-void-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

void x = 1;


//? test: direct-unaryexpression-void-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

void 1 = 1;


//? test: direct-unaryexpression-void-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

void true = 1;

//? test: direct-updateexpression-star-star-exponentiationexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

x ** y = 1;


//? test: direct-updateexpression-star-star-exponentiationexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 ** 2 = 1;


//? test: direct-updateexpression-star-star-exponentiationexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (Direct assignment)
//? expect: error
//? source: ...
#!/usr/bin/env afw

true ** false = 1;

